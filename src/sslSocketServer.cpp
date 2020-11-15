/**
 * @file sslSocketServer.cpp
 * @brief 
 * @author catfee (648137125@qq.com)
 * @version 0.1
 * @date 2020-11-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*******************include start**********************/
#include "sslSocketServer.h"
/*******************include end************************/

/*******************definition start**********************/
sslSocketServer::sslSocketServer(char *port, const char *certPath, const char *pKeyPath)
: socketServer(port){
    this->certPath = (char*)certPath;
    this->pKeyPath = (char*)pKeyPath;
}

sslSocketServer::sslSocketServer(char *port, char *fileName, const char *certPath, const char *pKeyPath)
: socketServer(port, fileName){
    this->certPath = (char*)certPath;
    this->pKeyPath = (char*)pKeyPath;
}

sslSocketServer::sslSocketServer(char *port, int backlog, const char *certPath, const char *pKeyPath)
: socketServer(port, backlog){
    this->certPath = (char*)certPath;
    this->pKeyPath = (char*)pKeyPath;
}

sslSocketServer::sslSocketServer(char *port, int backlog, char *fileName, const char *certPath, const char *pKeyPath)
: socketServer(port, backlog, fileName){
    this->certPath = (char*)certPath;
    this->pKeyPath = (char*)pKeyPath;
}



int sslSocketServer::sslServerInit(void){
    this->create_server_socket();
    this->listen_client();
    this->sslLibSrvInit();

    return 1;
}


int sslSocketServer::sslLibSrvInit(void){
    if(!this->isListening){
        return catError("server", "is not listening, call listenClient() first");
    }

    this->isSslInitialised = 1;//置位

    SSL_library_init();//初始化库
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    this->ctx = SSL_CTX_new(SSLv23_server_method());

    //添加证书和私钥
    if (SSL_CTX_use_certificate_file(ctx, this->certPath, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, this->pKeyPath, SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx))
    {

        ERR_print_errors_fp(stdout);
        exit(1);
    }
    this->ssl = SSL_new(ctx);
    return 1;
}

int sslSocketServer::sslFileRecv(void){
    /************************ local var start ********************/
int fileWriteSize = 0;

int sslWriteSize = 0;//一次SSL_write()发送的字节数
int sslReadSize = 0;//一次SSL_read()读取的字节数
char sslWriteBuf[BUFFERT+1];//发送缓冲区
char sslReadBuf[BUFFERT+1];
unsigned int cli_addr_len = sizeof(struct sockaddr_in);//客户端地址结构长度
//日期和时间
time_t intps;
struct tm *tmi;
/************************ local var end ********************/
    if(!this->isSslInitialised){
        return catError("server", "ssl not initialised, call sslInit() first");
    }


    this->connfd = accept(this->listenfd, (struct sockaddr*)&this->cli_sock_addr, &cli_addr_len);
    if(this->connfd == -1){
        return  catError("server","fail to accept client");
    }

    //获得客户端ip地址和端口
    if(inet_ntop(AF_INET,&(this->cli_sock_addr.sin_addr),this->cli_addr,INET_ADDRSTRLEN)==NULL){
            return  catError("server","fail to get client ip");
    }
    this->cli_port = ntohs(this->cli_sock_addr.sin_port);
    printf("[server] get the client %s:%d\n", this->cli_addr, this->cli_port);

    //绑定ssl与socket
    SSL_set_fd(this->ssl, this->connfd);

    //设置非阻塞io,使下一步建立连接的函数 ssl_accept() 立即返回而不阻塞
    int flags = fcntl(this->connfd, F_GETFL, 0);
    //fcntl(this->connfd, F_SETFL, flags & ~O_NONBLOCK);

    //建立ssl连接
    if(SSL_accept(this->ssl) == -1){
        close(this->connfd);
        return catError("server", "fail to accept SSL client");
    }

    //做传输前的准备
    //若指定了文件名称，则this->fileName保存的即为文件名
    //当没有指定文件名称时
    if(strncmp(this->fileName, "none", 4) == 0){
        //获取日期和时间
        intps = time(NULL);
        tmi = localtime(&intps);
        sprintf(this->fileName, "./test/fileRecv/clt.%d.%d.%d.%d.%d.%d", 1900+tmi->tm_year, tmi->tm_mon+1, tmi->tm_mday, tmi->tm_hour, tmi->tm_min, tmi->tm_sec);
    }

    //创建文件
    if ((fd=open(this->fileName,O_CREAT|O_WRONLY,0600))==-1)
    {
        printf("[server] error: fail to create output file\n");
        return -1;
    }

    //清空接收缓存区
    bzero(sslReadBuf, BUFFERT);
    //清空接收文件总字节数
    this->recvFileSize = 0;
    sslReadSize = SSL_read(this->ssl, sslReadBuf, BUFFERT);
    while(sslReadSize){
        if(sslReadSize == -1){
            printf("[server] error: fail to receive\n");
            return -1;
        }
        //写入文件
        fileWriteSize = write(this->fd, sslReadBuf, sslReadSize);
        if(fileWriteSize == -1){
            printf("[server] error: fail to write file\n");
            return -1;
        }
        this->recvFileSize += fileWriteSize;

        sslWriteSize = SSL_write(this->ssl, "recvOK", 6);
        bzero(sslReadBuf, BUFFERT);
        sslReadSize = SSL_read(this->ssl, sslReadBuf, BUFFERT);
    }
    close(this->listenfd);
    close(this->fd);
    close(this->connfd);
    #ifdef DEBUG_VER
    catLog("server", "socket fd closed\n");
    #endif

    SSL_shutdown(this->ssl);
    SSL_free(this->ssl);
    SSL_CTX_free(this->ctx);
    #ifdef DEBUG_VER
    catLog("server", "ssl closed\n");
    #endif

    printf("transmit done, receved %ld bytes\n", this->recvFileSize);

    return 0;

}

/*******************definition end************************/