
/****************************************************************************************/
/*                          COPYRIGHT INFORMATION                                       */
/*    This program contains proprietary information which is a trade                    */
/*    secret of whaley Co., Ltd.and also is protected as an   unpublished               */
/*    work under applicable Copyright laws. Recipient is to retain this                 */
/*    program in confidence and is not permitted to use or make copies thereof          */
/*    other than as permitted in a written agreement written agreement with             */
/*    whaley Co., Ltd.                                                                  */
/*                                                                                      */
/*    All rights reserved. shuaifengyun@126.com                                         */
/****************************************************************************************/
/*
 ****************************************************************************************
 *
 *               my_test_handle.c
 *
 * Filename      : my_test_handle.c
 * Programmer(s) : system BSP
 * Filename      : my_test_handle.c
 * Author        : Shuai Fengyun
 * Mail          : shuaifengyun@126.com
 * Create Time   : 2017年05月04日 星期四 10时43分49秒
 ****************************************************************************************
 */

#define MOUDLE_MY_TEST_HANDLE_C_

/** @defgroup MODULE_NAME_INFOR
* @{
*/

/*
 *******************************************************************************
 *                                INCLUDE FILES
 *******************************************************************************
*/
#include "tee_internal_api.h"
#include "tee_api_defines.h"
#include "trace.h"
#include "tee_api_defines_extensions.h"
#include "my_test_handle.h"
#include "my_test_ta.h"
#include "normrnd.h"
#include "GS.h"


/*
 *******************************************************************************
 *                         FUNCTIONS SUPPLIED BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                          VARIABLES SUPPLIED BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                          FUNCTIONS USED ONLY BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                          VARIABLES USED ONLY BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                               FUNCTIONS IMPLEMENT
 *******************************************************************************
*/

void g_TA_printf(CHAR* buf, UINT32 len)
{
    UINT32 index = 0U;
    for(index =0U; index < len; index++)
    {
        if(index < 15U)
        {

        }
        else if(0U == index%16U)
        {
            DMSG("\n");
        }
        else
        {

        }
        DMSG("0x%02x, ", (buf[index] & 0xFFU));
    }
    DMSG("\n\n");
}



/** @ingroup MOUDLE_NAME_C_
 *- #Description  This function for handle command.
 * @param   pMsg           [IN] The received request message
 *                               - Type: MBX_Msg *
 *                               - Range: N/A.
 *
 * @return     void
 * @retval     void
 *
 *
 */
static int l_CryptoTaHash_sha(EN_SHA_MODE shaMode, CHAR* input, UINT32 inLen, CHAR* output, UINT32* pOutLen)
{
    TEE_Result ret;
    TEE_OperationHandle l_OperationHandle;
    TEE_CRYPTO_ALGORITHM_ID l_AlgorithmId;
    int l_RetVal = OK;

    DMSG("Input data just like follow(0x%x), 0x%x:\n", inLen, (UINT32)(output));
    //g_TA_Printf(input, 20);

    /**1) Set the algorithm variable */
    switch(shaMode)
    {
        case EN_OP_SHA1:
            DMSG("The md is sha1@!!!!\n");
            l_AlgorithmId = TEE_ALG_SHA1;
            break;
        case EN_OP_SHA256:
            l_AlgorithmId = TEE_ALG_SHA256;
            break;
        default:
            l_AlgorithmId = TEE_ALG_INVALID ;
            l_RetVal = FAIL;
            goto cleanup_1;
            DMSG("Invalid sha mode\n");
            break;
    }

    /**2) Allocate the operation handle */
    ret = TEE_AllocateOperation(&l_OperationHandle, l_AlgorithmId, TEE_MODE_DIGEST, 0);
    if(ret != TEE_SUCCESS)
    {
        DMSG("Allocate SHA operation handle fail\n");
        l_RetVal = FAIL;
        goto cleanup_1;
    }

    TEE_DigestUpdate(l_OperationHandle, input, inLen);

    /**4) Do the final sha operation */
    ret = TEE_DigestDoFinal(l_OperationHandle, NULL, 0, output, pOutLen);
    DMSG("The out put length is :%d\n", *pOutLen);
    DMSG("The return value is :0x%x\n", ret);
    //ret = TEE_DigestDoFinal(l_OperationHandle, l_pInputTmp, inLen, output, pOutLen);
    if(ret != TEE_SUCCESS)
    {
        DMSG("Do the final sha operation fail\n");
        l_RetVal = FAIL;
        goto cleanup_2;
    }

    DMSG("Hash value just like folloe:\n");
    g_TA_printf(output, *pOutLen);

    /**5) Do the clean up operation& return the result */
    cleanup_2:
        TEE_FreeOperation(l_OperationHandle);
    cleanup_1:
        return l_RetVal;
}

static void l_CryptoTaOther_Random(UINT32 len, CHAR* output)
{
    DMSG("Entry random\n");
    TEE_GenerateRandom(output, len);
}







int g_CryptoTaHandle_Sha(uint32_t paramTypes, TEE_Param params[4])
{
    EN_SHA_MODE l_shaMode;
    CHAR* l_InputData= NULL;
    CHAR* l_OutPut = NULL;
    UINT32 l_InputLen = 0U;
    UINT32 l_OutputLen = 0U;
    uint32_t temp;
    temp = paramTypes;

    DMSG("%d\n", temp);

    /**1) Get the sha mode, input data info & output info */
    l_InputData = params[0].memref.buffer;
    l_InputLen = params[0].memref.size;
    l_shaMode = params[1].value.a;
    l_OutPut = params[2].memref.buffer;
    l_OutputLen = params[2].memref.size;

    /**2) Do sha operation */
    l_CryptoTaHash_sha(l_shaMode, l_InputData, l_InputLen, l_OutPut, &l_OutputLen);

    return OK;
}



int g_CryptoTaHandle_Random(uint32_t paramTypes, TEE_Param params[4])
{
    UINT32 l_RandomLen = 0U;
    CHAR* l_pBuf = NULL;
    uint32_t temp;
    temp = paramTypes;

    DMSG("%d\n", temp);

    /**1) Get the request length & point of responding buffer */
    l_RandomLen = params[0].memref.size;
    l_pBuf = params[0].memref.buffer;

    l_CryptoTaOther_Random(l_RandomLen, l_pBuf);

    return OK;
}


int g_CryptoTaHandle_Rand(uint32_t paramTypes, TEE_Param params[4])
{
	//int l_RetVal = FAIL;

	double randomMatrix [24][24] = {0};
	//double GSMatrix [24][24] = {0};
	double (*buffer) [24] = (double (*) [24])params[0]. memref.buffer;
    	int nRP = 24;
	paramTypes = paramTypes;
	params = params;

    	normrnd((double)0, (double)0.20412, nRP, nRP, randomMatrix);
	
	MatrixCopy(buffer,randomMatrix);
	

/*
	for (size_t i = 0; i != 24; i ++) {
		for (size_t j = 0; j != 24; j++) {
			(*(buffer+i))[j] = exp((*(buffer+i))[j]);
		}	
	}
*/
	return OK;
}

int g_CryptoTaHandle_GS(uint32_t paramTypes, TEE_Param params[4]) {
	double (*buffer) [24] = (double (*) [24])params[0]. memref.buffer;
	paramTypes = paramTypes;
	GS(buffer,buffer);
	return OK;

}
int g_CryptoTaHandle_GenModle(uint32_t paramTypes, TEE_Param params[4]){
	/* input some feature and output the model */
	/* define some varible */



	double (*feature) [24] = (double (*) [24])params[0]. memref.buffer;
	double randMatrix[24] [24] = {0};
	double y[24] [24] = {0};
	int nRP = 24;
	int noise = 0;
	/* unused params */
	paramTypes = paramTypes;
	// params = params;
	//feature[0][0] +=1;
	//randMatrix[0][0] += 1;
	//y[0][0] += 1;
	//nRP = nRP;
	//noise = noise;

	//DMSG(" enter the GenModle function");
	/* generate the normrnd matrix */
	if (params[2].value.a == 0) {
		
		normrnd((double)0, (double)0.20412, nRP, nRP, randMatrix);

		GS(randMatrix,randMatrix);
		MatrixCopy(params[1].memref.buffer,randMatrix);
	} else if (params[2].value.a == 1)
		MatrixCopy(randMatrix, params[1].memref.buffer);
	 else
		return FAIL;

	//DMSG("after the normrnd function");
		/* Gram-Schmidt */

	//GS(randMatrix, randMatrix);
	//DMSG("after the GS function");
	/* compute the R'*x, where R donates the random project matrix randMatrix, and the x donates the feature */
	
 

	for (int i = 0; i != 24; i ++) {
		for (int j = 0; j != 24; j ++) {
			double sum = 0;
			for (int m = 0; m != 24; m ++) {
				sum += randMatrix[m][i] * feature [m][j];
			}
			y[i][j] = sum;
			sum = 0;
			
		}
	}
    // MatrixCopy(feature, y);

	/* now y = R`*x */
	/* generate and add noise to y2. */
	//DMSG("after R`*x");
	//int noise [12][24] = {0};

	for (int i = 0; i != 12; i ++) {
		for (int j = 0; j != 24; j ++) {
			TEE_GenerateRandom(&noise, sizeof(noise)); // maybe the random is too large!
			y[12+i][j] += noise;
		}
	}
	//MatrixCopy(feature,y);


	//DMSG("after add noise");
	/* cross fussion y1 and y2e 
	 * ye =R2*y1 + R1*y2e
	 * save T = ye as a model
	 * saved in feature;
	 * R1 = randMatrix(:,1:12), R2 = randMatrix(:, 13:24)
	 * y1 = y(1:12,:),y2e = y(13:24,:)
     * 
     * */
	for (int i = 0; i != 24; i++) {
		
		for (int j = 0; j != 24; j ++) {
			double sum = 0;
			for (int m = 0; m != 12; m++) {
				sum += (randMatrix[i][m] * y[m+12][j] + randMatrix[i][m+12]*y[m][j]);
					
			}
			feature[i][j] = sum;
            sum = 0;
		}
	}
    //MatrixCopy(feature,y);
	DMSG("after cross fussion");


	return OK;





	
}























/**
 * @}
 */
