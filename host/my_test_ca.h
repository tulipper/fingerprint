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
 *               test.h
 *
 * Filename      : test.h
 * Programmer(s) : system BSP
 * Filename      : test.h
 * Author        : Shuai Fengyun
 * Mail          : shuaifengyun@126.cn
 * Create Time   : 2017年05月04日 星期四 11时50分54秒
 ****************************************************************************************
 */

#ifndef MOUDLE_NAME_H_
#define MOUDLE_NAME_H_




/*
 *******************************************************************************
 *                                INCLUDE FILES
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                  MACRO DEFINITION USED ONLY BY THIS MODULE
 *******************************************************************************
*/
#define TA_MY_TEST_UUID { 0x34b1c7a2, 0x45f7, 0x11e9, \
		{ 0xb2, 0x10, 0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93} }

#define TA_MY_TEST_CMD_INC_VALUE 	0
#define TA_MY_TEST_CMD_HASH 		1
#define TA_MY_TEST_CMD_RANDOM 		2
#define TA_MY_TEST_CMD_RAND	    3
#define TA_MY_TEST_CMD_GS	    4
#define TA_MY_TEST_CMD_GenModel     5
#define CMD_AES_OPER                6

#define FAIL -1
#define OK 0


/*
 *******************************************************************************
 *                STRUCTRUE DEFINITION USED ONLY BY THIS MODULE
 *******************************************************************************
*/
/* SHA operation type */
typedef enum
{
    EN_OP_SHA1 = 1,
    EN_OP_SHA224,
    EN_OP_SHA256,
    EN_OP_SHA384,
    EN_OP_SHA512,
    EN_OP_SHA_INVALID
}EN_SHA_MODE;


/* Define the type of variable */
typedef unsigned char  UINT8;    /**< Typedef for 8bits unsigned integer  */
typedef unsigned short UINT16;   /**< Typedef for 16bits unsigned integer */
typedef uint32_t       UINT32;   /**< Typedef for 32bits unsigned integer */
typedef signed char    INT8;     /**< Typedef for 8bits signed integer    */
typedef signed short   INT16;    /**< Typedef for 16bits signed integer   */
typedef signed int     INT32;    /**< Typedef for 32bits signed integer   */
typedef char           CHAR;     /**< Typedef for char                    */


#ifndef MODULE_NAME_C_


/*
 *******************************************************************************
 *                      VARIABLES SUPPLIED BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                      FUNCTIONS SUPPLIED BY THIS MODULE
 *******************************************************************************
*/

typedef enum
{
    EN_OP_AES_ENCRYPT = 1,
    EN_OP_AES_DECRYPT,
    EN_OP_AES_INVALID
}EN_AES_OPERATION_ACTION;

/* AES mode type */
typedef enum
{
    EN_MODE_CBC = 1,
    EN_MODE_ECB,
    EN_MODE_CTR,
    EN_MODE_CBC_CTS,
    EN_MODE_INVALIE
}EN_AES_MODE;
typedef struct _AesOperModeInfo
{
    EN_AES_OPERATION_ACTION active;
    EN_AES_MODE mode;
}AesOperModeInfo;

extern int g_CryptoVerifyCa_Aes(CHAR* pData, UINT32 len, EN_AES_MODE aesMode, 
                         EN_AES_OPERATION_ACTION operAction, CHAR* output);






















#endif

#endif  /* MOUDLE_NAME_H*/
