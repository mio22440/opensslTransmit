#include <iostream>
#include <string.h>

#include "sslSocketClient.h"
#include "sslSocketServer.h"

using namespace std;

int main(int argc, char **argv){
    // int i = 0;
    // printf("%d\n", argc);
    // for(i=0; i<argc; i++){
    //     printf("%s ", argv[i]);
    // }
    int ret = 0;//用来判断有没有正常执行

    //命令解析
    if((argc == 5) && (strcmp("client", argv[1]) == 0)){
        //执行client工作
        socketClient client(argv[2], argv[3], argv[4]);
        client.create_client_socket();
        ret = client.fileTransfer();
        if(ret == -1){
            printf("[client] fail to transmit file\n");
            return -1;
        }
    }
    else if((argc == 4) && (strcmp("server", argv[1]) == 0)){
        //执行server工作:指定了文件名的情况
        sslSocketServer server(argv[2], argv[3], "../keys/cacert.pem", "../keys/privkey.pem");
        server.serverInit();
        ret = server.sslFileRecv();
        if(ret == -1){
            printf("[server] fail to receive file\n");
            return -1;
        }
    }
    else if((argc == 3) && (strcmp("server", argv[1]) == 0)){
        //未指定文件名的情况
        sslSocketServer server(argv[2], "../keys/cacert.pem", "../keys/privkey.pem");
        server.serverInit();
        ret = server.sslFileRecv();
        if(ret == -1){
            printf("[server] fail to receive file\n");
            return -1;
        }
    }
    else{
        printf("[%s] usage : %s client <ip_serv> <port_serv> <filename>\n",argv[0],argv[0]);
        printf("        or : %s server <port>\n",argv[0]);
		return -1;
    }

    return 0;
}