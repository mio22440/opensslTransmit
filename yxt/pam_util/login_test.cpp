int pam_login(char *username);
#include "PamClass.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;
extern PamStatus err_mark;
int main(void)
{
    err_mark.fd_in = fileno(stdin);
    err_mark.fd_out = fileno(stdout);
    char name[256];
    fgets(name, 256, stdin);
    int len = strlen(name);
    if (name[len - 1] == '\n')
    {
        name[len - 1] = '\0';
    }
    int res;
    while (1)
    {
        res = pam_login(name);
        if (res > 0)
        {
            break;
        }
        cout << err_mark.tips << endl;
    }
    cout << res << endl;
}