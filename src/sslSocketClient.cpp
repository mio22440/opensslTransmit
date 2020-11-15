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
: socketClient(ipSrv, portSrv, fileName){
    
}


int sslSocketClient::sslCliInit(void){

}
/*******************definition end************************/