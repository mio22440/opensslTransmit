enum status
{
    LOGIN_FAILURE = -1,
    LOGIN_PROC = 0,
    LOGIN_SUCCESS = 1,
    CMD = 2,
    FSM = 3,
    FSMF = 4
};
class Ytp
{
public:
    char *type;
    char *status;
    int code;
    int len;
    char *content;
    Ytp(char *type, char *status, int code, int len);
    Ytp();
    char *parser(char *message);
    void setArgs(char *type, char *status, int code, int len);
    ~Ytp();
};