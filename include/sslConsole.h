#ifndef CATFEE_SSL_CONSOLE_H
#define CATFEE_SSL_CONSOLE_H
/*******************include start**********************/
#include <iostream>
#include <string.h>

#include "sslSocketClient.h"
#include "sslSocketServer.h"

using namespace std;
/*******************include end************************/
/*******************define start**********************/

/*******************define end************************/
/*******************declaration start**********************/
class sslConsole{
protected:
    int myargc;
    char *myargv[6];
public:
    int startAsClient(char *ipSrv, char *portSrv, char *fileName);
    int startAsServer(char *port, char *fileName);
    int realStart(int argc, char **argv);
};
/*******************declaration end************************/

#endif