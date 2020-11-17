/*******************include start**********************/
#include "../include/sslConsole.h"
/*******************include end************************/
/*******************definition start**********************/

/*******************definition end************************/
int sslConsole::startAsClient(char *ipSrv, char *portSrv, char *fileName){
    this->myargc = 5;
    this->myargv[0] = "sslConsole";
    this->myargv[1] = "client";
    this->myargv[2] = ipSrv;
    this->myargv[3] = portSrv;
    this->myargv[4] = fileName;

    int ret = 0;
    ret = this->realStart(this->myargc, this->myargv);

    return ret;
}

int sslConsole::startAsServer(char *port, char *fileName){
    if(fileName == NULL){
        this->myargc = 3;
    }
    else{
        this->myargc = 4;
    }

    this->myargv[0] = "sslConsole";
    this->myargv[1] = port;
    this->myargv[2] = fileName;

    int ret = 0;
    ret = this->realStart(this->myargc, this->myargv);

    return ret;
}

int sslConsole::realStart(int myargc, char **myargv){
    // int i = 0;
    // printf("%d\n", argc);
    // for(i=0; i<argc; i++){
    //     printf("%s ", argv[i]);
    // }
    int ret = 0;//用来判断有没有正常执行

    //命令解析
    if((myargc == 5) && (strcmp("client", myargv[1]) == 0)){
        //执行client工作
        sslSocketClient client(myargv[2], myargv[3], myargv[4]);
        client.sslClientInit();
        ret = client.sslFileTransfer();
        if(ret == -1){
            printf("[client] fail to transmit file\n");
            return -1;
        }
    }
    else if((myargc == 4) && (strcmp("server", myargv[1]) == 0)){
        //执行server工作:指定了文件名的情况
        sslSocketServer server(myargv[2], myargv[3], "./certs/cert.crt", "./certs/rsa_aes_private.key");
        server.sslServerInit();
        ret = server.sslFileRecv();
        if(ret == -1){
            printf("[server] fail to receive file\n");
            return -1;
        }
    }
    else if((myargc == 3) && (strcmp("server", myargv[1]) == 0)){
        //未指定文件名的情况
        sslSocketServer server(myargv[2], "./certs/cert.crt", "./certs/rsa_aes_private.key");
        server.sslServerInit();
        ret = server.sslFileRecv();
        if(ret == -1){
            printf("[server] fail to receive file\n");
            return -1;
        }
    }
    else{
        printf("[%s] usage : %s client <ip_serv> <port_serv> <filename>\n",myargv[0],myargv[0]);
        printf("        or : %s server <port>\n",myargv[0]);
		return -1;
    }

    return 0;
}
