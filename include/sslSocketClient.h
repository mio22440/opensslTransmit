/**
 * @file sslSocketClient.h
 * @brief 
 * @author catfee (648137125@qq.com)
 * @version 0.1
 * @date 2020-11-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CATFEE_SSL_SOCKET_CLIENT_H
#define CATFEE_SSL_SOCKET_CLIENT_H
/*******************include start**********************/
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string.h>

#include "socketClient.h"
#include "catError.h"
/*******************include end************************/
/*******************define start**********************/
#define DEBUG_VER //一些调试用的输出
#define CAT_LOG_BUF_SIZE 100
/*******************define end************************/
/*******************declaration start**********************/
class sslSocketClient: public socketClient{
protected:
    int isSslInitialised = 0;//ssl是否初始化完成

    SSL_CTX *ctx;
    SSL *ssl;

    char catLogBuf[CAT_LOG_BUF_SIZE+1];//log不能直接用const char *表示的话
                                       //就先写到这里
public:

    sslSocketClient(char *ipSrv, char *portSrv, char *fileName);

    /**
     * @brief 完成所有初始化操作，下一步即可调用文件传输
     * 
     * @return int 
     */
    int sslClientInit(void);

    /**
     * @brief 展示服务端证书信息
     * 
     * @return int 
     */
    int showCert(void);
    /**
     * @brief 调用相关函数初始化ssl相关内容
     *        要求先完成socket初始化和监听
     *        成功则置位 isSslInitialised
     * 
     * @return int 
     */
    int sslLibCliInit(void);

    /**
     * @brief 通过ssl通道发送文件
     * 
     * @return int 
     */
    int sslFileTransfer(void);
    
};
/*******************declaration end************************/

#endif