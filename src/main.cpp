#include <iostream>
#include <string.h>

#include "socketUtil.h"

using namespace std;

int main(int argc, char **argv){
    int i = 0;
    printf("%d\n", argc);
    for(i=0; i<argc; i++){
        printf("%s ", argv[i]);
    }

    //命令解析
    if((argc == 5) && (strcmp("client", argv[1]) == 0)){
        //todo: 执行client工作
        socketClient client(argv[2], argv[3], argv[4]);
        client.create_client_socket();
        client.fileTransfer();
    }
    else if((argc == 3) && (strcmp("server", argv[1]) == 0)){
        //todo: 执行server工作
    }
    else{
        printf("[%s] usage : %s client <ip_serv> <port_serv> <filename>\n",argv[0],argv[0]);
        printf("        or : %s server <port>\n",argv[0]);
		return EXIT_FAILURE;
    }


}