#include "../include/socketServer.h"

socketServer::socketServer(char *port){
    this->srv_port = atoi(port);
    this->backlog = 5;
    snprintf(this->fileName, MAX_RECEIVE_FILE_NAME_LEN, "%s", "none");
}

socketServer::socketServer(char *port, char *fileName){
    this->srv_port = atoi(port);
    this->backlog = 5;
    snprintf(this->fileName, MAX_RECEIVE_FILE_NAME_LEN, "%s", fileName);
}

socketServer::socketServer(char *port, int backlog){
    this->srv_port = atoi(port);
    this->backlog = backlog;
    snprintf(this->fileName, MAX_RECEIVE_FILE_NAME_LEN, "%s", "none");
}
socketServer::socketServer(char *port, int backlog, char *fileName){
    this->srv_port = atoi(port);
    this->backlog = backlog;
    snprintf(this->fileName, MAX_RECEIVE_FILE_NAME_LEN, "%s", fileName);
}

int socketServer::create_server_socket(void){

    //获得套接字
    this->listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if(this->listenfd == -1){
        printf("[server] error: fail to get sockfd\n");
        return -1;
    }

    bzero(&(this->srv_sock_addr), sizeof(struct sockaddr_in));

    this->srv_sock_addr.sin_family = AF_INET;
    this->srv_sock_addr.sin_port = htons(this->srv_port);
    this->srv_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定
    if(bind(this->listenfd, (struct sockaddr*)&(this->srv_sock_addr), sizeof(struct sockaddr_in)) == -1){
        printf("[server] error: fail to bind\n");
        return -1;
    }

    //创建绑定均成功，将标志置位
    this->isSocketCreated = 1;

    return this->listenfd;
}

int socketServer::listen_client(void){
    //判断套接字是否创建并绑定
    if(!this->isSocketCreated){
        printf("[server] error: socket is not created, please call socketServer.create_server_socket() first\n");
        return -1;
    }

    int ret = 0;
    ret = listen(this->listenfd, this->backlog);

    //置位正在监听标志
    this->isListening = 1;
}

int socketServer::fileReceive(void){
/************************ local var start ********************/
int readSize = 0;//一次read()从客户端读取的字节数
int writeSize = 0;//一次write()写入文件的字节数

char recv_buf[BUFFERT];
unsigned int cli_addr_len = sizeof(struct sockaddr_in);//客户端地址结构长度
//日期和时间
time_t intps;
struct tm *tmi;
/************************ local var end ********************/
    //没有正常获得socket或者绑定失败
    if(!this->isListening){
        printf("[server] error: server is not listening, please call socketServer::listen_client() first\n");
        return -1;
    }

    this->connfd = accept(this->listenfd, (struct sockaddr*)&this->cli_sock_addr, &cli_addr_len);
    if(this->connfd == -1){
        printf("[server] error: fail to accept client");
        return -1;
    }

    //获得客户端ip地址和端口
    if(inet_ntop(AF_INET,&(this->cli_sock_addr.sin_addr),this->cli_addr,INET_ADDRSTRLEN)==NULL){
            printf("[server] error: fail to get client ip\n");
            return -1;
    }
    this->cli_port = ntohs(this->cli_sock_addr.sin_port);
    printf("[server] get the client %s:%d\n", this->cli_addr, this->cli_port);

    //做传输前的准备
    //若指定了文件名称，则this->fileName保存的即为文件名
    //当没有指定文件名称时
    if(strncmp(this->fileName, "none", 4) == 0){
        //获取日期和时间
        intps = time(NULL);
        tmi = localtime(&intps);
        sprintf(this->fileName, "clt.%d.%d.%d.%d.%d.%d", 1900+tmi->tm_year, tmi->tm_mon+1, tmi->tm_mday, tmi->tm_hour, tmi->tm_min, tmi->tm_sec);
    }

    //创建文件
    if ((fd=open(this->fileName,O_CREAT|O_WRONLY,0600))==-1)
        {
            printf("[server] error: fail to create output file\n");
            return -1;
        }

    //清空接收缓存区
    bzero(recv_buf, BUFFERT);
    //清空接收文件总字节数
    this->recvFileSize = 0;
    readSize = read(this->connfd, recv_buf, BUFFERT);
    while(readSize){
        if(readSize == -1){
            printf("[server] error: fail to receive\n");
            return -1;
        }
        //写入文件
        writeSize = write(this->fd, recv_buf, readSize);
        if(writeSize == -1){
            printf("[server] error: fail to write file\n");
            return -1;
        }
        this->recvFileSize += writeSize;
        bzero(recv_buf, BUFFERT);
        readSize = read(this->connfd, recv_buf, BUFFERT);
    }
    close(this->listenfd);
    close(this->fd);
    close(this->connfd);

    printf("transmit done, receved %ld bytes\n", this->recvFileSize);

    return 0;
}
