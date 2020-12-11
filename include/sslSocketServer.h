/**
 * @file sslSocketServer.h
 * @brief 
 * @author catfee (648137125@qq.com)
 * @version 0.1
 * @date 2020-11-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CATFEE_SSL_SOCKET_SERVER_H
#define CATFEE_SSL_SOCKET_SERVER_H
/*******************include start**********************/
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "socketServer.h"
#include "catError.h"
/*******************include end************************/
/*******************define start**********************/
#define DEBUG_VER //一些调试用的输出
/*******************define end************************/
/*******************declaration start**********************/
class sslSocketServer: public socketServer{
private:
    SSL_CTX *ctx;
    SSL *ssl;
    char *certPath;//证书地址
    char *pKeyPath;//私钥地址
public:
    int isSslInitialised = 0;//ssl是否初始化完成

    //sslSocketServer();
    //不指定 client 队列大小， 默认为 5
    sslSocketServer(char *port, const char *certPath, const char *pKeyPath);
    sslSocketServer(char *port, char *fileName, const char *certPath, const char *pKeyPath);
    //指定 client 队列大小
    sslSocketServer(char *port, int backlog, const char *certPath, const char *pKeyPath);
    sslSocketServer(char *port, int backlog, char *fileName, const char *certPath, const char *pKeyPath);

    /**
     * @brief 完成所有初始化操作，下一步即可调用文件传输
     * 
     * @return int 
     */
    int sslServerInit(void);

    /**
     * @brief 调用相关函数初始化ssl相关内容
     *        要求先完成socket初始化和监听
     *        成功则置位 isSslInitialised
     * @return int 
     */
    int sslLibSrvInit(void);

    /**
     * @brief 通过ssl通道接收文件
     * 
     * @return int 返回-1表示失败
     */
    int sslFileRecv(void);

};
/*******************declaration end************************/

#endif