#include <openssl/ssl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define PORT 7899
int SSL_recv(SSL *ssl, char *buf, long long size)
{
    int n = 0;
    int t;
    while (n < size)
    {
        t = SSL_read(ssl, buf + n, size);
        if (t <= 0)
        {
            return 0;
        }
        n += t;
    }
    return n;
}
int main(void)
{
    SSL_CTX *ctx;
    int connfd = socket(AF_INET, SOCK_STREAM, 0), listenfd = socket(AF_INET, SOCK_STREAM, 0);
    char client_buffer[300];

    struct sockaddr_in server, client;
    socklen_t server_len, client_len;
    server_len = sizeof(server);
    client_len = sizeof(client);
    memset(&server, 0, server_len);
    memset(&client, 0, client_len);
    if (inet_aton("0.0.0.0", &server.sin_addr) == 0)
    {
        perror("addr transform err!");
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr *)&server, server_len) == -1)
    {
        perror("bind failed");
        exit(1);
    }
    listen(listenfd, 20);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_server_method());
    if (SSL_CTX_use_certificate_file(ctx, "../keys/cacert.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "../keys/privkey.pem", SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    if (!SSL_CTX_check_private_key(ctx))
    {

        ERR_print_errors_fp(stdout);

        exit(1);
    }
    SSL *ssl = SSL_new(ctx);

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr *)&client, &client_len);
        if (connfd < 0)
        {
            perror("accept error");
            exit(1);
        }
        SSL_set_fd(ssl, connfd);
        int flags = fcntl(connfd, F_GETFL, 0);

        fcntl(connfd, F_SETFL, flags & ~O_NONBLOCK); //ÉèÖÃ³É×èÈûÄ£Ê½£»
        if (SSL_accept(ssl) == -1)
        {

            perror("accept");

            close(connfd);

            break;
        }
        int n = SSL_read(ssl, client_buffer, 300);
        if (n < 0)
        {
            perror("ssl read failed");
            exit(1);
        }
        int len;
        puts(client_buffer);
        recv(connfd, &len, 4, 0);
        //SSL_read(ssl, &len, sizeof(len));
        len = ntohl(len);
        printf("\nlen:%d\n", len);
        void *p;
        int filefd = open("../1.png", O_CREAT | O_RDWR | O_TRUNC, 0777);
        if (filefd == -1)
        {
            perror("open failed");
            exit(1);
        }
        lseek(filefd, len, SEEK_SET);
        write(filefd, "1", 1);
        //int jlen = ((len - 1) / 4096 + 1) * 4096;
        if ((p = mmap(NULL, len, PROT_WRITE, MAP_SHARED, filefd, 0)) == MAP_FAILED)
        {
            perror("mmap failed");
            exit(1);
        }
        n = SSL_recv(ssl, (char *)p, len);
        //puts((char *)p);
        close(filefd);
        close(connfd);
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
}