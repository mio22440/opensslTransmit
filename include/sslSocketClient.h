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
#include "socketClient.h"
/*******************include end************************/
/*******************define start**********************/

/*******************define end************************/
/*******************declaration start**********************/
class sslSocketClient: public socketClient{
protected:
    int isSslInitialised = 0;//ssl是否初始化完成
public:

    sslSocketClient(char *ipSrv, char *portSrv, char *fileName);

    /**
     * @brief 完成所有初始化操作，下一步即可调用文件传输
     * 
     * @return int 
     */
    int clientInit(void);

    /**
     * @brief 调用相关函数初始化ssl相关内容
     *        要求先完成socket初始化和监听
     *        成功则置位 isSslInitialised
     * 
     * @return int 
     */
    int sslCliInit(void);
    
};
/*******************declaration end************************/

#endif