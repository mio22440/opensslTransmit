#include <regex>
#include <security/pam_appl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <pwd.h>
#include "user.h"
#include "ssl_util/ssl_util.h"
#include "pam_util/PamClass.h"
#include "pam_util/login.h"
#include "ytp_util/ytp.h"
using namespace std;
int sockfd, connfd;
#define PORT 9090
extern PamStatus err_mark;
#define ERR_ACTION(f, a) \
    do                   \
    {                    \
        if ((f) < 0)     \
        {                \
            perror(a);   \
            exit(1);     \
        }                \
    } while (0);
#define DEBUG                                    \
    do                                           \
    {                                            \
        fprintf(stderr, "debug:%d\n", __LINE__); \
    } while (0)

#define SSL_ERR_ACTION(f, a, ssl)                                 \
    do                                                            \
    {                                                             \
        if ((f) <= 0)                                             \
        {                                                         \
            perror(a);                                            \
            ERR_print_errors_fp(stdout);                          \
            printf("ssl error code:%d\n", SSL_get_error(ssl, f)); \
            exit(1);                                              \
        }                                                         \
    } while (0)
SSL_CTX *ctx;
SSL *ssl;
#define DEBUG
void client_clean_up(void)
{
    perror("");
    printf("I'm %d\n", getpid());
    DEBUG;
    ERR_print_errors_fp(stdout);
    DEBUG;
    SSL_shutdown(ssl);
    DEBUG;
    // if (!no_clean)
    //     SSL_free(ssl);
    free(ssl);
    DEBUG;
    close(connfd);
    DEBUG;
}
int main(void)
{
    signal(SIGCHLD, SIG_IGN);
    User::len = sysconf(_PC_NAME_MAX);
    User::name_len = sysconf(_SC_LOGIN_NAME_MAX);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (SSL_CTX_use_certificate_file(ctx, "/home/yaoxuetao/桌面/语言项目实践/keys/cacert.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "/home/yaoxuetao/桌面/语言项目实践/keys/privkey.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx))
    {

        ERR_print_errors_fp(stdout);

        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server, client;
    socklen_t server_len = sizeof(server), client_len = sizeof(client);
    memset(&server, server_len, 0);
    memset(&client, client_len, 0);
    int res;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = 0;
    //server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    ERR_ACTION(res = ::bind(sockfd, (sockaddr *)&server, server_len), "bind error");
    ERR_ACTION(listen(sockfd, 1024), "listen failed");
    int pid;
    while (1)
    {
        connfd = accept(sockfd, (sockaddr *)&client, &client_len);
        ERR_ACTION(connfd, "accept failed");
        pid = fork();
        switch (pid)
        {
        case 0:
        {
            //atexit(client_clean_up);
            close(sockfd);
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, connfd);
            if (SSL_accept(ssl) == -1)
            {

                perror("accept");

                close(connfd);

                exit(1);
            }
            err_mark.ssl = ssl;
            err_mark.fd_err = connfd;
            err_mark.fd_in = connfd;
            err_mark.fd_out = connfd;
            char login_buffer[1024];
            const char *login_tips1 = "your username:";
            const char *login_tips2 = "login success,congratulations!";
        restart:
            Ytp login_ytp_pre("LOGIN", "SETUP", LOGIN_PROC, strlen(login_tips1) + 1);
            strcpy(login_buffer, login_ytp_pre.content);
            strcat(login_buffer, login_tips1);
            int n = SSL_write(ssl, login_buffer, strlen(login_buffer) + 1);
            SSL_ERR_ACTION(n, "ssl write failed in 81", ssl);
            char name_buf[4096 + 1];
            int len;
            //printf("debug:%d\n", __LINE__);
            // n = recv(connfd, &len, sizeof(len), MSG_WAITALL);
            // ERR_ACTION(n, "recv failed in 115");
            // len = ntohl(len);
            n = SSL_read(ssl, name_buf, 4096 + 1);
            char *name = login_ytp_pre.parser(name_buf);
            //printf("%d", SSL_get_error(ssl, n));
            SSL_ERR_ACTION(n, "ssl read failed in 113", ssl);
            int res;
            do
            {
                DEBUG;
                res = pam_login(name);
                char buffer[4096];
                if (res < 0)
                {
                    //puts(name);
                    Ytp login_ytp_res("LOGIN", "FIALURE", LOGIN_FAILURE, strlen(err_mark.tips) + 1);
                    strcpy(buffer, login_ytp_res.content);
                    strcat(buffer, err_mark.tips);
                    n = SSL_write(ssl, buffer, strlen(buffer) + 1);
                    SSL_ERR_ACTION(n, "ssl write failed in 121", ssl);
                    if (err_mark.suberr == BEFOREAUTH)
                        goto restart;
                }
                else
                {
                    DEBUG;
                    Ytp login_ytp_res("LOGIN", "SUCCESS", LOGIN_SUCCESS, strlen(login_tips2) + 1);
                    //puts(login_ytp.content);
                    strcpy(buffer, login_ytp_res.content);
                    strcat(buffer, login_tips2);
                    n = SSL_write(ssl, buffer, strlen(buffer) + 1);
                    SSL_ERR_ACTION(n, "ssl write failed in 126", ssl);
                }
            } while (res < 0);
            DEBUG;
            //printf("debug 176:%s\n", name);
            //puts(name);
            char cmd_buffer[4096 + 1];
            char response_buffer[4096 + 1];
            string workdir = "/home/" + string(name);
            if (strcmp(name, "root") == 0)
            {
                workdir = "/root";
            }
            DEBUG;
            User user(name, workdir);
            DEBUG;
            n = chdir(workdir.c_str());
            DEBUG;
            ERR_ACTION(n, "cd to home fail");
            DEBUG;
            struct passwd *user_info;
            user_info = getpwnam(name);
            uid_t uid = user_info->pw_uid;
            gid_t gid = user_info->pw_gid;
            printf("debug in %d:uid=%d;gid=%d\n", __LINE__, uid, gid);
            n = setresgid(gid, gid, gid);
            n = setresuid(uid, uid, uid);
            ERR_ACTION(n, "set uid failed");
            //n = setresgid(gid, gid, gid);
            ERR_ACTION(n, "set gid failed");
            ERR_ACTION(user_info == NULL, "get user info failed");
            while (1)
            {
                DEBUG;
                n = SSL_read(ssl, cmd_buffer, 4096);
                SSL_ERR_ACTION(n, "ssl read failed in 213", ssl);
                Ytp cmd_ytp;
                char *p, *p_rest;
                char *part1, *part3;
                const char *part2;
                p = cmd_ytp.parser(cmd_buffer);
                char *mod = " ";
                //printf("debug in 220:p:%s\n", p);
                //part1 = strtok_r(p, mod, &p_rest);
                part1 = strtok(p, mod);
                if (strcmp(part1, "cd") == 0)
                {
                    DEBUG;
                    part2 = strtok(NULL, mod);
                    part3 = strtok(NULL, mod);
                    if (part3 != NULL)
                    {
                        strcpy(response_buffer, "cd:参数过多");
                    }
                    else
                    {
                        //printf("debug:%d:%s(%d)\n", __LINE__, part2, strlen(part2));
                        if (part2 == NULL)
                        {
                            char dir[4096];
                            strcpy(response_buffer, "now in:");
                            getcwd(dir, 4096);
                            strcat(response_buffer, dir);
                            cmd_ytp.setArgs("CMD", "ACTIVE", CMD, strlen(response_buffer) + 1);
                            strcpy(cmd_buffer, cmd_ytp.content);
                            strcat(cmd_buffer, response_buffer);
                            n = SSL_write(ssl, cmd_buffer, strlen(cmd_buffer) + 1);
                            SSL_ERR_ACTION(n, "ssl write failed in 208", ssl);
                            continue;
                        }
                        else if (strcmp(part2, "~") == 0)
                        {
                            part2 = workdir.c_str();
                        }
                        int res_cd = chdir(part2);
                        //printf("debug:cd %s!\n", part2);
                        if (res_cd < 0)
                        {
                            strcpy(response_buffer, strerror(errno));
                        }
                        else
                        {
                            char dir[4096];
                            strcpy(response_buffer, "now in:");
                            getcwd(dir, 4096);
                            strcat(response_buffer, dir);
                        }
                    }
                    cmd_ytp.setArgs("CMD", "ACTIVE", CMD, strlen(response_buffer) + 1);
                    strcpy(cmd_buffer, cmd_ytp.content);
                    strcat(cmd_buffer, response_buffer);
                    n = SSL_write(ssl, cmd_buffer, strlen(cmd_buffer) + 1);
                    SSL_ERR_ACTION(n, "ssl write failed in 208", ssl);
                }
                else if (strcmp("sendfile", part1) == 0)
                {
                    part2 = strtok(NULL, " ");
                    int filefd = open(part2, O_CREAT | O_RDWR | O_TRUNC, 0777);
                    if (filefd == -1)
                    {
                        perror("open failed");
                        strcpy(response_buffer, strerror(errno));
                        goto end1;
                    }
                    n = recv(connfd, &len, 4, 0);
                    ERR_ACTION(n, "recv failed in sendfile:recvlen");
                    //SSL_read(ssl, &len, sizeof(len));
                    len = ntohl(len);
                    lseek(filefd, len - 1, SEEK_SET);
                    n = write(filefd, "\0", 1);
                    ERR_ACTION(n, "prewrite failed in sendfile");
                    //int jlen = ((len - 1) / 4096 + 1) * 4096;
                    char *q;
                    if ((q = (char *)mmap(NULL, len, PROT_WRITE, MAP_SHARED, filefd, 0)) == MAP_FAILED)
                    {
                        perror("mmap failed in sendfile");
                        exit(1);
                    }
                    n = SSL_recv(ssl, (char *)q, len);
                    SSL_ERR_ACTION(n, "ssl recvfailed in sendfile", ssl);
                    ERR_ACTION(munmap(q, len), "munmap failed in error action");
                    strcpy(response_buffer, "sendfile success!");
                    close(filefd);
                end1:
                    cmd_ytp.setArgs("FILE", "ACTIVE", FSM, strlen(response_buffer) + 1);
                    strcpy(cmd_buffer, cmd_ytp.content);
                    strcat(cmd_buffer, response_buffer);
                    n = SSL_write(ssl, cmd_buffer, strlen(cmd_buffer) + 1);
                    SSL_ERR_ACTION(n, "ssl write failed in sendfile", ssl);
                }
                else if (strcmp("getfile", part1) == 0)
                {
                    DEBUG;
                    part2 = strtok(NULL, " ");
                    FILE *file = fopen(part2, "r");
                    if (file == NULL)
                    {
                        strcpy(response_buffer, strerror(errno));
                        DEBUG;
                        cmd_ytp.setArgs("FILE", "ALIVE", FSMF, strlen(response_buffer) + 1);
                        DEBUG;
                        //printf("debug in getfile:%d:%d\n", __LINE__, suc);
                        strcpy(cmd_buffer, cmd_ytp.content);
                        strcat(cmd_buffer, response_buffer);
                        n = SSL_write(ssl, cmd_buffer, strlen(cmd_buffer) + 1);
                        SSL_ERR_ACTION(n, "ssl write failed", ssl);
                        DEBUG;
                        //printf("debug in %d:%s\n", 323, cmd_buffer);
                    }
                    else
                    {
                        strcpy(response_buffer, "getfile success");
                        cmd_ytp.setArgs("FILE", "ALIVE", FSM, strlen(response_buffer) + 1);
                        //printf("debug in getfile:%d:%d\n", __LINE__, suc);
                        strcpy(cmd_buffer, cmd_ytp.content);
                        strcat(cmd_buffer, response_buffer);
                        n = SSL_write(ssl, cmd_buffer, strlen(cmd_buffer) + 1);
                        SSL_ERR_ACTION(n, "ssl write failed", ssl);
                        fseek(file, 0, SEEK_END);
                        len = ftell(file);
                        //printf("\nlen:%d\n", len);
                        int hlen = htonl(len);
                        n = send(connfd, &hlen, sizeof(hlen), 0);
                        DEBUG;
                        ERR_ACTION(n, "sendfile len send len failed in getfile");
                        int filefd = fileno(file);
                        fseek(file, 0, SEEK_SET);
                        void *addr_png_1;
                        if ((addr_png_1 = mmap(NULL, len, PROT_READ, MAP_SHARED, filefd, 0)) == MAP_FAILED)
                        {
                            perror("mmap failed in getfile");
                            exit(1);
                        };
                        n = SSL_write(ssl, addr_png_1, len);
                        SSL_ERR_ACTION(n, "ssl write failed", ssl);
                        ERR_ACTION(munmap(addr_png_1, len), "munmap failed in getfile");
                        fclose(file);
                    }
                }
                else
                {
                    DEBUG;
                    //char *rest_rest;
                    int ipc_pipe[2];
                    pipe(ipc_pipe);
                    int subpid = fork();
                    if (subpid == 0)
                    {
                        close(ipc_pipe[0]);
                        close(fileno(stdout));
                        close(fileno(stderr));
                        dup2(ipc_pipe[1], fileno(stdout));
                        dup2(ipc_pipe[1], fileno(stderr));
                        char *cmd_list[40 + 1];
                        cmd_list[0] = part1;
                        int i;
                        for (i = 1; i < 40; i++)
                        {
                            cmd_list[i] = strtok(NULL, mod);
                            // fprintf(stderr, "part %d:%s\n", i + 1, cmd_list[i]);
                            if (cmd_list[i] == NULL)
                            {
                                break;
                            }
                        }

                        execvp(part1, cmd_list);
                        //perror("execv failed");
                        printf("%s", strerror(errno));
                        exit(1);
                    }
                    else
                    {
                        char tmp[4096] = {0};
                        close(ipc_pipe[1]);
                        read(ipc_pipe[0], tmp, 4096);
                        //printf("%d:%s\n", __LINE__,tmp);
                        cmd_ytp.setArgs("CMD", "ACTIVE", CMD, strlen(tmp) + 1);
                        strcpy(response_buffer, cmd_ytp.content);
                        strcat(response_buffer, tmp);
                        n = SSL_write(ssl, response_buffer, strlen(response_buffer) + 1);
                        SSL_ERR_ACTION(n, "ssl write failed in 239", ssl);
                    }
                    DEBUG;
                }
                DEBUG;
            }
            break;
        }
        default:
            close(connfd);
            break;
        }
    }
}