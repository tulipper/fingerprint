
#ifndef MOUDLE_CRYPTO_VERIFY_TYPE_H_
#define MOUDLE_CRYPTO_VERIFY_TYPE_H_
/* AES operation type */
typedef struct _AesOperation
{
    CHAR* inBuf;
    CHAR* outBuf;
    CHAR* key;
    CHAR* iv;
    UINT32 dataLen;
    UINT32 keyLen;
    UINT32 ivLen;
    UINT32 algorithmId;
    TEE_OperationMode operMode;
}AesOperation;




/* AES operation type */
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



























#endif