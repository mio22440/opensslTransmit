#include <openssl/ssl.h>
#include <openssl/err.h>
int SSL_recv(SSL *ssl, char *buf, long long size);