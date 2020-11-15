#ifndef CATFEE_SOCKET_CLIENT_H
#define CATFEE_SOCKET_CLIENT_H
/*******************include start**********************/
//socket
#include <sys/socket.h>
#include <arpa/inet.h>//sockaddr_in结构体

//文件操作
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>//stat()根据文件名获取文件状态

#include <stdlib.h>//for atoi()
#include <strings.h>//for bzero()
#include "timeCounter.h"
/*******************include end************************/
/************************ define start ********************/
#define BUFFERT 512 //发送多块文件的缓冲区大小
/************************ define end ********************/
/*******************declaration start**********************/
class socketClient{
private:
    int isClientInitialized = 0;//为1时表示初始化完成，已经获得socket

    int sockfd; //socket()返回的文件描述符
    int fd;     //要传输文件的描述符

    char *ipSrv;    //服务端ip地址
    int portSrv;  //服务端端口
    struct sockaddr_in servAddr;//服务器地址结构

    char *fileName; //要传输的文件名称
    struct stat file_state;//文件状态的存放区
    //char buf[BUFFERT];//发送文件的缓冲区

    long int transSize = 0;//传输总字节数
    long int totalSize = 0;//文件总字节数
    long int timeUsed = 0;//传输用时

    timeCounter tc;//计时器
public:
    /**
     * @brief           构造函数
     * @param  ipSrv    服务端ip地址        
     * @param  portSrv          
     * @param  fileName         
     */
    socketClient(char *ipSrv, char *portSrv, char *fileName);

    /**
     * @brief           创建客户端socket描述符
     * @param  port     要连接的服务端端口       
     * @param  ipaddr   要连接的服务端ip地址        
     * @return int      创建的socket描述符（一般可不使用）,返回-1表示失败
     */
    int create_client_socket (void);

    /**
     * @brief               文件传输
     * @param  ipSrv        服务端ip地址
     * @param  portSrv      服务端端口
     * @param  fileName     要传输的文件名称
     * @return int 
     */
    int fileTransfer(void);


    
};
/*******************declaration end************************/

#endif