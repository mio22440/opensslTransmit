/**
 * @file socketUtil.h
 * @brief 
 * @author catfee (648137125@qq.com)
 * @version 1.0
 * @date 2020-11-06
 * 
 * @copyright Copyright （c） 2020 catfee
 * 
 * @par 修改日志:
 * Date           Version     Author      Description
 * 2020-11-06     1.0         catfee      增加了socketClient和socketServer的定义
 */
#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

/************************ include start ********************/
#include <stdio.h>
#include <stdlib.h>//for atoi()
//文件操作
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>//stat()根据文件名获取文件状态
//socket
#include <sys/socket.h>
#include <arpa/inet.h>//sockaddr_in结构体
//字符串处理
#include <string.h>//socketServer中的strncmp()
//日期和时间
#include <time.h>
//timeCounter
#include "timeCounter.h"

/************************ include end ********************/
/************************ define start ********************/
#define BUFFERT 512 //发送多块文件的缓冲区大小
#define MAX_RECEIVE_FILE_NAME_LEN 50
/************************ define end ********************/
/************************ declare start ********************/


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

class socketServer{
private:
    int isSocketCreated = 0;//为1时表示初始化完成，已经获得socket并且成功bind
    int isListening = 0;

    int sockfd;//监听的套接字
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


/************************ declare end ********************/

#endif