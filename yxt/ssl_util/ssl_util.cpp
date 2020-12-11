#include "ssl_util.h"
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