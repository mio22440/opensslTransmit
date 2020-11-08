/**
 * @file socketClient.cpp
 * @brief  利用socket传输文件的工具（客户端）         
 * @author catfee (648137125@qq.com)
 * @version 1.0
 * @date 2020-11-06
 * 
 * @copyright Copyright （c） 2020 catfee
 * 
 * @par 修改日志:
 * Date           Version     Author      Description
 * 2020-11-06     1.0         catfee      内容
 */

#include "socketUtil.h"

socketClient::socketClient(char *ipSrv, char *portSrv, char *fileName){
    this->ipSrv = ipSrv;
    this->portSrv = atoi(portSrv);
    this->fileName = fileName;
};

int socketClient::create_client_socket(void){
    this->sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(this->sockfd == -1){
        printf("[client] error: fail to get sockfd\n");
        return -1;
    }

    //目的地址的创建
	bzero(&(this->servAddr),sizeof(struct sockaddr_in));
	
	this->servAddr.sin_family=AF_INET;
	this->servAddr.sin_port=htons(this->portSrv);
    if (inet_pton(AF_INET,this->ipSrv,&(this->servAddr.sin_addr))==0){
		printf("[client] error: Invalid IP adress\n");
		return -1;
	}
    
    //初始化成功，置位标志
    this->isClientInitialized = 1;

    return this->sockfd;
}

int socketClient::fileTransfer(void){
/************************ local var start ********************/
int readSize = 0;//一次read()从文件读取的字节数
int sendSize = 0;//一次sendto()发送的字节数
char send_buf[BUFFERT];//发送缓冲区
/************************ local var end ********************/
    //如果初始化未成功完成，则不能进行文件传输
    if(!this->isClientInitialized){
        printf("[client] error: socket is not initialized, please initialise first\n");
        return -1;
    }
    

    //comment on 2020/11/8 14:51
    //创建套接字
    // int ret = create_client_socket();
    // if(ret == -1){
    //     printf("fail to create sockfd\n");
    //     return -1;
    // }
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
    bzero(send_buf, BUFFERT);
    //连接服务端
    if(connect(this->sockfd,(struct sockaddr*)&this->servAddr,sizeof(struct sockaddr_in))==-1){
        printf("[client] error: fail to connect to server\n");
        return -1;
    }
    //开始计时
    this->tc.startCounter();
    //从文件读取512字节
    readSize = read(this->fd, send_buf, BUFFERT);
    //清空发送文件总字节数
    this->transSize = 0;
    while(readSize){//当文件没读完时
        /*************************这里的readSize可不可以改为<512即跳出循环*******/
        if(readSize == -1){
            printf("[client] error: file to read file\n");
            return -1;
        }
        //发送缓冲区的内容
        sendSize = write(this->sockfd, send_buf, readSize);
        if(sendSize == -1){
            printf("[client] error: fail to send\n");
            return -1;
        }
        //记录传输字节数
        this->transSize += sendSize;
        //清空缓冲区
        bzero(send_buf, BUFFERT);
        //读文件
        readSize = read(this->fd, send_buf, BUFFERT);
    }
    //收尾，发送0，（这样会sigpipe还是啥来着）
    sendSize = write(this->sockfd, send_buf, 0);
    //停止计时
    this->tc.stopCounter();
    //计算用时，存入tc->delta
    this->tc.getDurationUs();

    printf("transmit size (bytes): %ld\n", this->transSize);
    printf("total size (bytes):%ld\t\n", this->totalSize);
    this->tc.printDuration();
		
    close(this->sockfd);
    return 0;
};