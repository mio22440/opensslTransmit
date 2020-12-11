#include <iostream>
using namespace std;
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <security/pam_appl.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
#include "PamClass.h"
PamAct pam_news[PAM_MAX_NUM_MSG];
PamStatus err_mark;
int pam_msg_num;
extern int pam_tty_conv(int num_msg, const struct pam_message **msg,
                        struct pam_response **response, void *appdata_ptr);
int pam_login(char *username)
{
    struct pam_conv conv = {pam_tty_conv, NULL};
    pam_handle_t *pamh;
    struct passwd *pw;
    int err;
    if ((pw = getpwnam(username)) == NULL)
    {
        err_mark.tips = "user doesn't exist!\n";
        //"getpwnam failed in pam\n";
        err_mark.err = API_ERR;
        err_mark.suberr = BEFOREAUTH;
        return -1;
    }
    /* Initialize PAM framework */
    err = pam_start("system-auth", pw->pw_name, &conv, &pamh);
    if (err != PAM_SUCCESS)
    {
        err_mark.tips = "part start failed\n";
        err_mark.err = PAM_FAIL;
        err_mark.suberr = BEFOREAUTH;
        exit(1);
    }
    /* Authenticate user in order to unlock screen */
    do
    {
        err = pam_authenticate(pamh, 0);
        if (err == PAM_USER_UNKNOWN)
        {
            err_mark.tips = "user doesn't exist\n";
            err_mark.err = PAM_FAIL;
            err_mark.suberr = PAM_USER_UNKNOWN;
            return -1;
        }
        else if (err != PAM_SUCCESS)
        {
            err_mark.tips = "Invalid passwd!\n";
            err_mark.err = PAM_FAIL;
            err_mark.suberr = PAM_AUTH_ERR;
            return -1;
            //invalid password
        }
    } while (err != PAM_SUCCESS);
    // if (pam_setcred(pamh, PAM_REFRESH_CRED) != PAM_SUCCESS)
    // {
    //     err_mark.tips = "pam setcred failed\n";
    //     err_mark.err = PAM_FAIL;
    //     err_mark.suberr = UNDEF;
    //     return -1;
    // }
    // switch (err = pam_open_session(pamh, 0))
    // {
    // case PAM_PERM_DENIED:
    //     err_mark.tips = "open session denied\n";
    //     err_mark.err = PAM_FAIL;
    //     err_mark.suberr = PAM_PERM_DENIED;
    //     return -1;
    // case PAM_SUCCESS:
    //     break;
    // default:
    //     err_mark.tips = "open session failed\n";
    //     err_mark.err = PAM_FAIL;
    //     err_mark.suberr = UNDEF;
    //     return -1;
    // }
    // if (pam_close_session(pamh, 0) != PAM_SUCCESS)
    // {
    //     err_mark.tips = "session close failed\n";
    //     err_mark.err = API_ERR;
    //     err_mark.suberr = UNDEF;
    //     return -1;
    // }
    // // system("cd ~");
    // // system("ls ~");
    // // system("pwd");
    // (void)pam_end(pamh, 0);
    return 1;
}

int pam_tty_conv(int num_msg, const struct pam_message **mess,
                 struct pam_response **resp, void *my_data)
{
    //printf("debug:102\n");
    const struct pam_message *m = *mess;
    struct pam_response *r;
    int i;
    if (num_msg <= 0 || num_msg >= PAM_MAX_NUM_MSG)
    {
        // (void)fprintf(stderr, "bad number of messages %d "
        //                       "<= 0 || >= %d\n",
        //               num_msg, PAM_MAX_NUM_MSG);
        *resp = NULL;
        return (PAM_CONV_ERR);
    }
    if ((*resp = r = (pam_response *)calloc(num_msg,
                                            sizeof(struct pam_response))) == NULL)
        return (PAM_BUF_ERR);
    /* Loop through messages */
    for (i = 0; i < num_msg; i++)
    {
        int echo_off;
        /* bad message from service module */
        if (m->msg == NULL)
        {
            strcpy(pam_news[i].news, m->msg);
            pam_news[i].type = m->msg_style;
            goto err;
        }
        /*
		 * fix up final newline:
		 * 	removed for prompts
		 * 	added back for messages
		 */
        // if (m->msg[strlen(m->msg)] == '\n')
        //     m->msg[strlen(m->msg)] = '\0';
        r->resp = NULL;
        r->resp_retcode = 0;
        echo_off = 0;
        //printf("debug:138\n");
        switch (m->msg_style)
        {
        case PAM_PROMPT_ECHO_OFF:
            //printf("debug:142\n");
            echo_off = 1;
            /*FALLTHROUGH*/
        case PAM_PROMPT_ECHO_ON:
            //printf("debug:146\n");
            pam_news[i].type = m->msg_style;
            //printf("debug:%p\n", pam_news[i].news);
            strcpy(pam_news[i].news, m->msg);
            dooutput(pam_news[i], err_mark.fd_out, err_mark.ssl);
            r->resp = getinput(echo_off, err_mark.fd_in, err_mark.ssl);
            //printf("%s", r->resp);
            //free(r->resp);
            //can not
            break;
        case PAM_ERROR_MSG:
            //printf("debug:148\n");
            strcpy(pam_news[i].news, m->msg);
            pam_news[i].type = m->msg_style;
            dooutput(pam_news[i], err_mark.fd_err, err_mark.ssl);
            break;
        case PAM_TEXT_INFO:
            //printf("debug:156\n");
            strcpy(pam_news[i].news, m->msg);
            pam_news[i].type = m->msg_style;
            dooutput(pam_news[i], err_mark.fd_out, err_mark.ssl);
            break;
        default:

            goto err;
        }
        if (errno == EINTR)
            goto err;
        /* next message/response */
        m++;
        r++;
    }
    return (PAM_SUCCESS);
err:
    //隐患：当pam_unix.so崩溃，可能没有清理分配的r
    for (int j = i; j >= 0; j--)
    {
        free(r);
        r--;
    }
    *resp = NULL;
    return (PAM_CONV_ERR);
}