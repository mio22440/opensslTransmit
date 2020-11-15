/**
 * @file sslSocketClient.cpp
 * @brief 增加了ssl功能的socketClient类
 * @author catfee (648137125@qq.com)
 * @version 0.1
 * @date 2020-11-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*******************include start**********************/
#include "sslSocketClient.h"
/*******************include end************************/
/*******************definition start**********************/
sslSocketClient::sslSocketClient(char *ipSrv, char *portSrv, char *fileName)
: socketClient(ipSrv, portSrv, fileName){}

int sslSocketClient::sslClientInit(void){
    this->create_client_socket();
    this->sslLibCliInit();

    return 1;
}

int sslSocketClient::showCert(void){
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(this->ssl);
    if (cert != NULL)
    {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("无证书信息！\n");

        return 1;
}

int sslSocketClient::sslLibCliInit(void){
    if(!this->isClientInitialized){
        return catError("client", "client is not initialised, call socketClient::create_client_socket(void) first");
    }

    this->isSslInitialised = 1;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    this->ctx = SSL_CTX_new(SSLv23_client_method());
    if(this->ctx == NULL){
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    catLog("client", "ssl client initialise done");
}

int sslSocketClient::sslFileTransfer(void){
/************************ local var start ********************/
int fileReadSize = 0;//一次read()从文件读取的字节数
int sslWriteSize = 0;//一次SSL_write()发送的字节数
int sslReadSize = 0;//一次SSL_read()读取的字节数
char sslWriteBuf[BUFFERT+1];//发送缓冲区
char sslReadBuf[BUFFERT+1];
/************************ local var end ********************/
    //如果ssl初始化未成功完成，则不能进行文件传输
    if(!this->isSslInitialised){
        printf("[client] error: SSL is not initialized, please call sslCliInit() first\n");
        return -1;
    }
    
    //打开文件
    if ((this->fd = open(fileName,O_RDONLY))==-1){
		printf("[client] error: fail to open file %s\n", fileName);
		return -1;
	}
    //得到文件大小（bytes）
    if (stat(this->fileName, &(this->file_state))==-1){
		printf("[client] error: fail to get status of file :%s\n", this->fileName);
		return -1;
	}
	else{
        this->totalSize=this->file_state.st_size;
    }
    
    //清空发送缓冲区
    bzero(sslWriteBuf, BUFFERT);
    //连接服务端
    if(connect(this->sockfd,(struct sockaddr*)&this->servAddr,sizeof(struct sockaddr_in))==-1){
        printf("[client] error: fail to connect to server\n");
        return -1;
    }
    snprintf(this->catLogBuf, CAT_LOG_BUF_SIZE, "connected to server %s:%d", this->ipSrv, this->portSrv);
    catLog("client", this->catLogBuf);

    //建立ssl连接
    this->ssl = SSL_new(ctx);
    SSL_set_fd(this->ssl, this->sockfd);

    if (SSL_connect(this->ssl) == -1)
        ERR_print_errors_fp(stderr);
    else
    {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        this->showCert();
    }

    //开始计时
    this->tc.startCounter();
    //从文件读取512字节
    fileReadSize = read(this->fd, sslWriteBuf, BUFFERT);
    //清空发送文件总字节数
    this->transSize = 0;
    while(fileReadSize){//当文件没读完时
        /*************************这里的readSize可不可以改为<512即跳出循环*******/
        if(fileReadSize == -1){
            printf("[client] error: file to read file\n");
            return -1;
        }
        //发送缓冲区的内容
        sslWriteSize = SSL_write(this->ssl, sslWriteBuf, fileReadSize);
        if(sslWriteSize == -1){
            printf("[client] error: fail to send\n");
            return -1;
        }
        //接收服务端回应，并且阻塞客户端，避免提前结束导致sigpipe
        sslReadSize = SSL_read(this->ssl, sslReadBuf, BUFFERT);
        //异常读取或者收到的消息不是recvOK
        if((sslReadSize == -1) || (strncmp("recvOK", sslReadBuf, 6) != 0)){
            return catError("client", "server closed unexpectedly");
        }
        //记录传输字节数
        this->transSize += sslWriteSize;
        //清空缓冲区
        bzero(sslWriteBuf, BUFFERT);
        //读文件
        fileReadSize = read(this->fd, sslWriteBuf, BUFFERT);
    }
    //收尾，发送0，（这样会sigpipe还是啥来着）
    sslWriteSize = SSL_write(this->ssl, sslWriteBuf, 0);
    //停止计时
    this->tc.stopCounter();
    //计算用时，存入tc->delta
    this->tc.getDurationUs();

    printf("transmit size (bytes): %ld\n", this->transSize);
    printf("total size (bytes):%ld\t\n", this->totalSize);
    this->tc.printDuration();

    catLog("client", "file transfer done");
		
    close(this->sockfd);

    SSL_shutdown(this->ssl);
    SSL_free(this->ssl);
    SSL_CTX_free(this->ctx);
    #ifdef DEBUG_VER
    printf("ssl closed\n");
    #endif

    return 0;
}
/*******************definition end************************/