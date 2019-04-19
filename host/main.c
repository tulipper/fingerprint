/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> 

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include "my_test_ca.h"
#include "fingerdata.h"


#include "myfeature.h"
#include "readbmp.h"
#include "Mat.h"


static int g_TaskInitFlag = -1;    /* Flag if the task done initialize operation */
TEEC_UUID svc_id = TA_MY_TEST_UUID;
TEEC_Context g_TaskContext;
CHAR g_RandomOut[512] = {0};
/* Buffer for sha operation */
double threshold = 3.6;
double finger0 [24][24] = {0};
double finger1 [24][24] = {0};
CHAR g_ShaTestBuf[] ={
     'Y', 'o', 'u', ' ', 'y', 'o', 'u', ' ', 'c', 'h', 'e', 'c', 'k', ' ', 'n', 'o',
     'w', 'j', 'i', 'a', 'n', ' ', 'b', 'i', 'n', 'g', ' ', 'g', 'u', 'o', ' ', 'z',
     'i', ' ', 'l', 'a', 'i', ' ', 'y', 'i', ' ', 't', 'a', 'o', '!', '!', '!', '!'};
CHAR g_ShaOutput[80] = {0};

CHAR g_Sha1Result[] = 
{   
    0x21, 0x9b, 0x5b, 0x8b, 0x25, 0x6f, 0x0e, 0x52, 0xcb, 0x2f, 0xfe, 0xfd, 0x6c, 0x47, 0xd7, 0xb4, 
    0x44, 0x00, 0x57, 0xc3
};


CHAR g_Sha256Result[] = 
{   
    0xda, 0x52, 0xe9, 0xc2, 0x53, 0xae, 0x03, 0x30, 0xbd, 0x97, 0x3f, 0xa5, 0xf3, 0xea, 0x51, 0x1d, 
    0x31, 0x0a, 0xdf, 0x1f, 0x0a, 0xc0, 0x0e, 0x62, 0x0f, 0x2d, 0x5e, 0x99, 0xf5, 0xc8, 0x6b, 0x8f
};

/*buffer for finger feature*/
double feature [24][24] = {1.1,2.2,3.3,1.2,56.5,153.1,12.5,14.5,15.63,58.4,28.1,46.3};

double key [24][24] = {0};
double distance (double T1 [24][24], double T2 [24][24], double key [24][24]) {
	double F1 [12][24] = {0};
	double F2 [12][24] = {0};
	/* F1 = sqrt(2)*R2`*T1 */
	for (int i = 0; i != 12; i ++) {
		for (int j = 0; j != 24; j ++) {
			double sum = 0;
			for (int m = 0; m != 24; m ++) {
				sum += key[m][i+12]*T1[m][j];
			}
			F1[i][j] = sum * sqrt(2);
			sum = 0;
		}
	}
	/* F2 = sqrt(2)*R2`*T2 */
	for (int i = 0; i != 12; i ++) {
		for (int j = 0; j != 24; j ++) {
			double sum = 0;
			for (int m = 0; m != 24; m ++) {
				sum += key[m][i+12]*T2[m][j];
			}
			F2[i][j] = sum * sqrt(2);
			sum = 0;
		}
	}
	/* calcute the distance between F1 and F2 */
	double disc = 0;
	for ( int i = 0; i != 12; i ++){
		for (int j = 0; j != 24; j ++) {
		disc += (F1[i][j]-F2[i][j]) * (F1[i][j]-F2[i][j]);
		}
	}
	return sqrt(disc);
	
}


void g_CA_PrintfBuffer(CHAR* buf, UINT32 len)
{
    UINT32 index = 0U;
    for(index = 0U; index < len; index++)
    {
        if(index < 15U)
        {
        }
        else if(0U == index%16U)
        {
            printf("\n");
            
        }
        else
        {
        }
        
        printf("0x%02x, ", (buf[index] & 0x000000FFU));
        

    }
    printf("\n");
   
}


int l_CryptoVerifyCa_TaskInit(void)
{
    TEEC_Result result;
    int l_RetVal = OK;
    
    /**1) Check if need to do task initialization operation */
    if(-1 == g_TaskInitFlag)
    {
        result = TEEC_InitializeContext(NULL, &g_TaskContext);
        if(result != TEEC_SUCCESS) 
        {
            printf("InitializeContext failed, ReturnCode=0x%x\n", result);
            l_RetVal= FAIL;
        } 
        else 
        {
            g_TaskInitFlag = 0;
            printf("InitializeContext success\n");
            l_RetVal = OK;
        }
    }
    
    return l_RetVal;
}


int l_CryptoVerifyCa_OpenSession(TEEC_Session* session)
{
    TEEC_Result result;
    int l_RetVal = FAIL;
    uint32_t origin;

    result = TEEC_OpenSession(&g_TaskContext, session, &svc_id, 
                                TEEC_LOGIN_PUBLIC, NULL, NULL, &origin);
    if(result != TEEC_SUCCESS) 
    {
        printf("OpenSession failed, ReturnCode=0x%x, ReturnOrigin=0x%x\n", result, origin);
        g_TaskInitFlag = -1;
        l_RetVal = FAIL;
    } 
    else 
    {
        printf("OpenSession success\n");
        l_RetVal = OK;
    }

    return l_RetVal;
}


int l_CryptoVerifyCa_SendCommand(TEEC_Operation* operation, TEEC_Session* session, uint32_t commandID)
{
    TEEC_Result result;
    int l_RetVal = FAIL;
    uint32_t origin;

    result = TEEC_InvokeCommand(session, commandID, operation, &origin);
    if (result != TEEC_SUCCESS) 
    {
        printf("InvokeCommand failed, ReturnCode=0x%x, ReturnOrigin=0x%x\n", result, origin);
        l_RetVal = FAIL;
    } 
    else 
    {
        printf("InvokeCommand success\n");
        l_RetVal = OK;
    }


    return l_RetVal;
}



int g_VerifyCa_Random (void) {
    	TEEC_Session l_session;
	TEEC_Operation l_operation;
	int l_RetVal = FAIL;   //Fail difined in the head file
	l_RetVal = l_CryptoVerifyCa_TaskInit();
	if(FAIL == l_RetVal)
 	{
        	goto cleanup_1;
	}
	/* here the context has initialized*/
	/* then open the session*/
	l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    	if(FAIL == l_RetVal)
    	{
        	goto cleanup_2;
    	}
	/*here the session has been opened*/
	/*then build the operation and send command*/
	memset (&l_operation, 0x0, sizeof(l_operation));
	l_operation.started = 1;//dont know what it means
    	l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_VALUE_INPUT, 
                                              TEEC_NONE, TEEC_NONE);
	l_operation.params[0].tmpref.size = sizeof (feature);
	l_operation.params[0].tmpref.buffer = feature;
	l_operation.params[1].value.a = 24;
	l_operation.params[1].value.b = 24;

	/*send command*/
	l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_RAND);
    	if(FAIL == l_RetVal)
    	{
        	goto cleanup_3;
    	}
	printf("the Gauss random matrix is :\n");
	printMatrix(feature);
	l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_GS);
	if(FAIL == l_RetVal)
    	{
        	goto cleanup_3;
    	}
	printf("the  matrix after Grand-Smith transform is :\n");
	printMatrix(feature);
    	/**5) The clean up operation */
    	cleanup_3:
        	TEEC_CloseSession(&l_session);
	cleanup_2:
        	TEEC_FinalizeContext(&g_TaskContext);
		g_TaskInitFlag = -1;
	cleanup_1:
		return l_RetVal;
	
		
}
int g_VerifyCa_GenModel() {
	TEEC_Session l_session;
	TEEC_Operation l_operation;
	int l_RetVal = FAIL;   //Fail difined in the head file
	l_RetVal = l_CryptoVerifyCa_TaskInit();
	if(FAIL == l_RetVal)
 	{
        	goto cleanup_1;
	}
	/* here the context has initialized*/
	/* then open the session*/
	l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    	if(FAIL == l_RetVal)
    	{
        	goto cleanup_2;
    	}
	/*here the session has been opened*/
	/*then build the operation and send command*/
	memset (&l_operation, 0x0, sizeof(l_operation));
	l_operation.started = 1;//dont know what it means
    	l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_MEMREF_TEMP_OUTPUT, 
                                              TEEC_VALUE_INPUT, TEEC_NONE);
	l_operation.params[0].tmpref.size = sizeof (finger0);
	l_operation.params[0].tmpref.buffer = finger0;
	l_operation.params[1].tmpref.size = sizeof(key);
	l_operation.params[1].tmpref.buffer = key;
	l_operation.params[2].value.a = 0;  // 0 means generate the model 
    printf("the size of finer is %d", sizeof(finger0));
	/* send command */
	l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_GenModel);
    if(FAIL == l_RetVal) {
        goto cleanup_3;
    }
	printf("generate fingerprint model successfully!\n");
	printf("the genarated model is saved back:\n");
	// printMatrix(finger1_1);
    // g_CA_PrintfBuffer(finger1_1, sizeof(finger1_1));
	printf("got the random project Matrix from the secure world:\n");
	// printMatrix(key);
	cleanup_3:
        	TEEC_CloseSession(&l_session);
	cleanup_2:
        	TEEC_FinalizeContext(&g_TaskContext);
		g_TaskInitFlag = -1;
	cleanup_1:
		return l_RetVal;
}
int g_VerifyCa_Authentication() {
	TEEC_Session l_session;
	TEEC_Operation l_operation;
	int l_RetVal = FAIL;   //Fail difined in the head file
	l_RetVal = l_CryptoVerifyCa_TaskInit();
	if(FAIL == l_RetVal)
 	{
        	goto cleanup_1;
	}
	/* here the context has initialized*/
	/* then open the session*/
	l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    	if(FAIL == l_RetVal)
    	{
        	goto cleanup_2;
    	}

	/*here the session has been opened*/
	/*then build the operation and send command*/
	memset (&l_operation, 0x0, sizeof(l_operation));
	l_operation.started = 1;//dont know what it means
    l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,TEEC_MEMREF_TEMP_OUTPUT, 
                                              TEEC_NONE, TEEC_NONE);
	l_operation.params[0].tmpref.size = sizeof (finger1);
	l_operation.params[0].tmpref.buffer = finger1;
	l_operation.params[1].tmpref.size = sizeof (key);
	l_operation.params[1].tmpref.buffer = key;
	l_operation.params[2].value.a = 1;  // 1 means authentification
	/* send command */
	l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_GenModel);

	if(FAIL == l_RetVal)
    	{
        	goto cleanup_3;
    	}
    // printf("the genarated model in the generation process is as fellow:\n");
    // printMatrix(finger1_1);
    // printf("the genarated model in the authentication process is as fellow:\n");
    // printMatrix(finger2_1);
    // printf("the randmod Matrix is as fellow:\n");
    // printMatrix(key);
    //g_CA_PrintfBuffer(finger1, sizeof(finger1));
    double disc = distance(finger0, finger1, key);
    //disc /= 1E14;
    printf (" the distance of the two fingerprints is %f \n", disc/1E14);
    g_CA_PrintfBuffer(&disc, sizeof(disc));
	if (disc / 1E14 < threshold)
		printf ("match \n");
	else 
		printf("mismatch \n");
	cleanup_3:
        	TEEC_CloseSession(&l_session);
	cleanup_2:
        	TEEC_FinalizeContext(&g_TaskContext);
            g_TaskInitFlag = -1;
	cleanup_1:
		return l_RetVal;
}





int g_CryptoVerifyCa_Sha(CHAR* pData, UINT32 len, EN_SHA_MODE shaMode, CHAR* output, UINT32 outLen)
{
    TEEC_Session   l_session;    /* Define the session of TA&CA */
    TEEC_Operation l_operation;  /* Define the operation for communicating between TA&CA */
    int l_RetVal = FAIL;       /* Define the return value of function */

    /**1) Initialize this task */
    l_RetVal = l_CryptoVerifyCa_TaskInit();
    if(FAIL == l_RetVal)
    {
        goto cleanup_1;
    }

    /**2) Open session */
    l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    if(FAIL == l_RetVal)
    {
        goto cleanup_2;
    }

    /**3) Set the communication context between CA&TA */
    memset(&l_operation, 0x0, sizeof(TEEC_Operation));
    l_operation.started = 1;
    l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,TEEC_VALUE_INPUT, 
                                              TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE);
    l_operation.params[0].tmpref.size = len;
    l_operation.params[0].tmpref.buffer = pData;
    l_operation.params[1].value.a = shaMode;
    l_operation.params[2].tmpref.size = outLen;
    l_operation.params[2].tmpref.buffer = output;

    /**4) Send command to TA */
    l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, TA_MY_TEST_CMD_HASH);
    printf("The respond data length is 0x%02x\n", outLen);
    if(FAIL == l_RetVal)
    {
        goto cleanup_3;
    }

    /**5) The clean up operation */
    cleanup_3:
        TEEC_CloseSession(&l_session);
    cleanup_2:
        TEEC_FinalizeContext(&g_TaskContext);
    cleanup_1:
        return l_RetVal;
}

void MatrixCopy(double dest [24][24], double src [24][24]) {
	for (int i = 0; i != 24; i ++)
		for (int j = 0 ; j != 24; j ++)
			dest [i][j] = src [i][j];
}
void printMatrix(double feature [24][24]) {
	for (CHAR i = 0; i != 24; i ++) {
	for (CHAR j = 0; j != 24; j++) {
		if ( j != 23)
			printf("%e, ",feature[i][j]);
		else 
			printf("%e;\n",feature[i][j]);
	}	
    }
    printf("\n");
}
int g_CryptoVerifyCa_Aes(CHAR* pData, UINT32 len, EN_AES_MODE aesMode, 
                         EN_AES_OPERATION_ACTION operAction, CHAR* output)
{
    TEEC_Session   l_session;    /* Define the session of TA&CA */
    TEEC_Operation l_operation;  /* Define the operation for communicating between TA&CA */
    AesOperModeInfo l_aesMode;
    int l_RetVal = FAIL;       /* Define the return value of function */

    /**1) Initialize this task */
    l_RetVal = l_CryptoVerifyCa_TaskInit();
    if(FAIL == l_RetVal)
    {
        goto cleanup_1;
    }

    /**2) Open session */
    l_RetVal = l_CryptoVerifyCa_OpenSession(&l_session);
    if(FAIL == l_RetVal)
    {
        goto cleanup_2;
    }

    l_aesMode.active = operAction;
    l_aesMode.mode = aesMode;

    /**3) Set the communication context between CA&TA */
    memset(&l_operation, 0x0, sizeof(TEEC_Operation));
    l_operation.started = 1;
    l_operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_MEMREF_TEMP_INPUT, 
                                              TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INPUT);
    l_operation.params[0].value.a = operAction;
    l_operation.params[0].value.b = aesMode;
    l_operation.params[1].tmpref.size = len;
    l_operation.params[1].tmpref.buffer = pData;
    l_operation.params[2].tmpref.size = len;
    l_operation.params[2].tmpref.buffer = output;
    l_operation.params[3].value.a = len;

    /**4) Send command to TA */
    l_RetVal = l_CryptoVerifyCa_SendCommand(&l_operation, &l_session, CMD_AES_OPER);
    printf("The respond data length is 0x%02x\n", len);
    
    if(FAIL == l_RetVal)
    {
        goto cleanup_3;
    }

    /**5) The clean up operation */
    cleanup_3:
        TEEC_CloseSession(&l_session);
    cleanup_2:
        TEEC_FinalizeContext(&g_TaskContext);
    cleanup_1:
        return l_RetVal;
}


int main(int argc, char *argv[])
{
    printf ("in the finger print ca \n");

    dMat imagedata = readbmpfile("/1_1.bmp");
	if (imagedata.rows ==0 || imagedata.cols == 0 || imagedata.array == NULL) {
		printf("read file failed");
		return -1;
	}
	printdMat(&imagedata);
    printf("read file ok\n");
	dMat fingercode = myfeature(imagedata);
	printdMat(&fingercode);
    fflush(stdout);
	freemat(&imagedata);
	freemat(&fingercode);
// 	return 0;
	return 0;
    //printf("the result of R' * x");
    // g_VerifyCa_GenModel();
    // g_VerifyCa_Authentication();
    //g_VerifyCa_Random();
    if (0 == memcmp(argv[1], "finger1_1", 9))

        MatrixCopy(finger0, finger1_1);
    else if(0 == memcmp (argv[1], "finger1_8", 9))
        MatrixCopy(finger0, finger1_8);
    else if (0 == memcmp (argv[1], "finger2_1", 9))
        MatrixCopy(finger0, finger2_1);
    else {
        printf("do not find the  %s \n", argv[1]);
        return 0;
    }

    if (0 == memcmp(argv[2], "finger1_1", 9))

        MatrixCopy(finger1, finger1_1);
    else if(0 == memcmp (argv[2], "finger1_8", 9))
        MatrixCopy(finger1, finger1_8);
    else if (0 == memcmp (argv[2], "finger2_1", 9))
        MatrixCopy(finger1, finger2_1);
    else {
        printf("do not find the  %s \n", argv[1]);
        return 0;
    }
    //printf ("the feature input is : \n");
    //printMatrix(finger1_1);
    // printf ("got the feature of the two fingerprint \n");
    // printf("the %s is : \n", argv[1]);
    // printMatrix(finger0);
    // printf("the %s is : \n", argv[2]);
    // printMatrix(finger1);
    printf("the size of finger1_1 is %d \n", sizeof(finger1_1));
    printf("the size of finger0 is %d \n", sizeof(finger0));
    g_VerifyCa_GenModel(finger0);
    // printf("the model of %s is :", argv[1]);
    // printMatrix(finger0);
    // printf("the random matrix from the secure is : \n");
    // printMatrix(key);
    printf("the size of finger1 is %d \n", sizeof(finger1));
    g_VerifyCa_Authentication(finger1);
    // printf("the model of %s is :", argv[2]);
    // printMatrix(finger1);
    // double disc = distance(finger0, finger1, key);
    // printf (" the distance of the two fingerprints is %f \n", disc/1E14);
	// if (disc / 1E14 < threshold)
	// 	printf ("match \n");
	// else 
	// 	printf("mismatch \n");
    // printf("the model of the finperprint is : \n");
    // printMatrix(finger1_1);
    /*
    if(0 == memcmp(argv[1], "helloworld", 10))
    {
        printf("Entry get helloworld CA\n");
        g_CryptoVerifyCa_Helloworld();
        printf("The Respond helloworld from TA just like follow:\n");
    }

    if(0 == memcmp(argv[1], "sha1", 4))
    {
        printf("Entry sha1 CA\n");
        g_CryptoVerifyCa_Sha(g_ShaTestBuf, sizeof(g_ShaTestBuf), EN_OP_SHA1, g_ShaOutput, 20);
        printf("The Respond hash data from TA just like follow:\n");
        g_CA_PrintfBuffer(g_ShaOutput, 20);
    }



    if(0 == memcmp(argv[1], "sha256", 6))
    {

        printf("Entry sha256 CA\n");
        g_CryptoVerifyCa_Sha(g_ShaTestBuf, sizeof(g_ShaTestBuf), EN_OP_SHA256, g_ShaOutput, 32);
        printf("The Respond hash data from TA just like follow:\n");
        g_CA_PrintfBuffer(g_ShaOutput, 32);
    }
    */

    return 0;
}



