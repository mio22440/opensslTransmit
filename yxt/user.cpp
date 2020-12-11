#include "user.h"
#include <cstring>
#include <limits.h>
#include <unistd.h>
using namespace std;
long User::name_len;
long User::len;
User::User(char *name, string work_dir)
{
    //long len = sysconf(_PC_NAME_MAX);
    //name_len = sysconf(_SC_LOGIN_NAME_MAX);
    //printf("debug:USER:12\n");
    name = new char[name_len];
    //printf("debug:USER:14\n");
    work_dir = new char[len];
    //printf("debug:USER:16\n");
    this->name = new char[name_len];
    strcpy(this->name, name);
    //printf("debug:USER:18\n");
    this->work_dir = work_dir;
    //printf("debug:USER:20\n");
}
User::~User()
{
    //printf("debug:in delete User\n");
    delete[] name;
}
