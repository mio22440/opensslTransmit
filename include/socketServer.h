#ifndef CATFEE_SERVER_H
#define CATFEE_SERVER_H
/*******************include start**********************/
//socket
#include <sys/socket.h>
#include <arpa/inet.h>//sockaddr_in结构体


//文件操作
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>//bzero()
#include <sys/stat.h>//stat()根据文件名获取文件状态

#include <stdio.h>
#include <stdlib.h>//for atoi()

#include <string.h>//for strncmp()

//日期和时间
#include <time.h>
/*******************include end************************/
/*******************define start**********************/
#define BUFFERT 512 //发送多块文件的缓冲区大小
#define MAX_RECEIVE_FILE_NAME_LEN 50
/*******************define end************************/
/*******************declaration start**********************/
class socketServer{
protected:
    int listenfd;//监听的套接字
    int connfd;//建立连接的套接字
    int fd;//保存文件的文件描述符

    int srv_port;//服务端监听端口
    int backlog;//client 队列大小

    int cli_port;//客户端端口
    char cli_addr[INET_ADDRSTRLEN];//客户端地址

    long int  recvFileSize = 0;//接收到文件的总字节数
    char fileName[MAX_RECEIVE_FILE_NAME_LEN+1];//用来保存指定接收文件的文件名
                                               //因为要编辑保存的文件名，所以先分配空间
    struct sockaddr_in srv_sock_addr;//服务端的socket地址结构
    struct sockaddr_in cli_sock_addr;//客户端的socket地址结构
public:
    int isSocketCreated = 0;//为1时表示初始化完成，已经获得socket并且成功bind
    int isListening = 0;

    //不指定 client 队列大小， 默认为 5
    socketServer(char *port);
    socketServer(char *port, char *fileName);
    //指定 client 队列大小
    socketServer(char *port, int backlog);
    socketServer(char *port, int backlog, char *fileName);

    /**
     * @brief           创建用来监听的套接字
     *                  成功则置位 isSocketCreated
     * @return int      创建的socket描述符（一般可不使用）,返回-1表示创建失败
     */
    int create_server_socket(void);

    /**
     * @brief           调用listen()函数监听客户端连接
     *                  成功则置位 isListening
     * @return int      返回-1表示监听失败
     */
    int listen_client(void);
    
    /**
     * @brief           保存接收文件
     * @return int      返回-1表示接收文件失败
     */
    int fileReceive(void);


};
/*******************declaration end************************/

#endif