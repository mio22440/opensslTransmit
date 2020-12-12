//for interaction with pam
#include <security/pam_appl.h>
#include <openssl/ssl.h>
#define MAX_LOGIN 255
#define MAX_PAM_MSG_LEN 1024
enum err
{
    PAM_SUCC,
    PAM_FAIL,
    API_ERR,
};
enum suberr
{
    UNDEF = -1,
    BEFOREAUTH,
};
struct PamAct
{
    PamAct();
    ~PamAct();
    char news[PAM_MAX_MSG_SIZE + 1];
    int type;
};
//for pam status
struct PamStatus
{

    const char *tips = "";
    int err;
    int suberr;
    int fd_in;
    int fd_out;
    int fd_err;
    SSL *ssl;
};
char *getinput(int echoff, int fd, SSL *ssl);
void dooutput(PamAct &pam_act, int fd, SSL *ssl);