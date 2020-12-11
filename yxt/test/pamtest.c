/*
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <pwd.h>
#include <errno.h>
#include <security/pam_appl.h>
#pragma ident "@(#)pam_tty_conv.c	1.4	05/02/12 SMI"
#define __EXTENSIONS__ /* to expose flockfile and friends in stdio.h */
#include <errno.h>
#include <libgen.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termio.h>
extern int pam_tty_conv(int num_msg, const struct pam_message **msg,
                        struct pam_response **response, void *appdata_ptr);
static void
free_resp(int num_msg, struct pam_response *pr);
/* Disable keyboard interrupts (Ctrl-C, Ctrl-Z, Ctrl-\) */
static void disable_kbd_signals(void)
{
    (void)signal(SIGINT, SIG_IGN);
    (void)signal(SIGTSTP, SIG_IGN);
    (void)signal(SIGQUIT, SIG_IGN);
}
/* Terminate current user session, i.e., logout */
static void
logout()
{
    pid_t pgroup = getpgrp();
    (void)signal(SIGTERM, SIG_IGN);
    (void)fprintf(stderr, "Sorry, your session can't be restored.\n");
    (void)fprintf(stderr, "Press return to terminate this session.\n");
    (void)getchar();
    (void)kill(-pgroup, SIGTERM);
    (void)sleep(2);
    (void)kill(-pgroup, SIGKILL);
    exit(-1);
}
int
/*ARGSUSED*/
main(int argc, char *argv)
{
    struct pam_conv conv = {pam_tty_conv, NULL};
    pam_handle_t *pamh;
    struct passwd *pw;
    int err;
    //disable_kbd_signals();
    //getuid()
    if ((pw = getpwuid(1000)) == NULL)
    {
        (void)fprintf(stderr, "login: Can't get username: %s\n",
                      strerror(errno));
        exit(1);
    }

    /* Initialize PAM framework */
    err = pam_start("login", pw->pw_name, &conv, &pamh);
    if (err != PAM_SUCCESS)
    {
        (void)fprintf(stderr, "plock: pam_start failed: %s\n",
                      pam_strerror(pamh, err));
        exit(1);
    }
    /* Authenticate user in order to unlock screen */
    do
    {
        (void)fprintf(stderr, "Terminal locked for %s. ", pw->pw_name);
        err = pam_authenticate(pamh, 0);
        if (err == PAM_USER_UNKNOWN)
            logout();
        else if (err != PAM_SUCCESS)
        {
            printf("%d\n", err);
            (void)fprintf(stderr, "Invalid password.\n");
        }
    } while (err != PAM_SUCCESS);
    /* Make sure account and password are still valid */
    switch (err = pam_acct_mgmt(pamh, 0))
    {
    case PAM_PERM_DENIED:
        printf("23456");
        break;
    case PAM_USER_UNKNOWN:
    case PAM_ACCT_EXPIRED:
        /* User not allowed in anymore */
        logout();
        break;
    case PAM_NEW_AUTHTOK_REQD:
        /* The user's password has expired. Get a new one */
        do
        {
            err = pam_chauthtok(pamh, 0);
        } while (err == PAM_AUTHTOK_ERR);
        if (err != PAM_SUCCESS)
            logout();
        break;
    default:
        printf(" 1 %d:", err);
        printf("I am here\n");
    }
    if (pam_setcred(pamh, PAM_REFRESH_CRED) != PAM_SUCCESS)
    {
        logout();
    }
    switch (err = pam_open_session(pamh, 0))
    {
    case PAM_PERM_DENIED:
        printf("23456");
        break;
    case PAM_SUCCESS:
        printf("session opened!\n");
        break;
    default:
        printf(" 2 %d:", err);
        printf("I am here\n");
    }
    if (pam_close_session(pamh, 0) != PAM_SUCCESS)
    {
        logout();
    }
    printf("debug here\n");
    uid_t uid, euid, suid;
    gid_t gid, egid, sgid;
    while (1)
    {
        getresuid(&uid, &euid, &suid);
        printf("uid=%d euid=%d suid=%d\n", uid, euid, suid);
        getresgid(&gid, &egid, &sgid);
        printf("gid=%d egid=%d sgid=%d\n", gid, egid, sgid);
        system("pwd");
        sleep(1);
    }
    (void)pam_end(pamh, 0);
    exit(0);
    /*NOTREACHED*/
}
/* 
 * Copyright 2005 Sun Microsystems, Inc.  All rights reserved. 
 * Use is subject to license terms. 
 */

static int ctl_c; /* was the conversation interrupted? */
/* ARGSUSED 1 */
static void
interrupt(int x)
{
    ctl_c = 1;
}
/* getinput -- read user input from stdin abort on ^C
 *	Entry	noecho == TRUE, don't echo input.
 *	Exit	User's input.
 *		If interrupted, send SIGINT to caller for processing.
 */
static char *
getinput(int noecho)
{
    struct termio tty;
    unsigned short tty_flags;
    char input[PAM_MAX_RESP_SIZE];
    int c;
    int i = 0;
    void (*sig)(int);
    ctl_c = 0;
    sig = signal(SIGINT, interrupt);
    if (noecho)
    {
        (void)ioctl(fileno(stdin), TCGETA, &tty);
        tty_flags = tty.c_lflag;
        tty.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
        (void)ioctl(fileno(stdin), TCSETAF, &tty);
    }
    /* go to end, but don't overflow PAM_MAX_RESP_SIZE */
    flockfile(stdin);
    while (ctl_c == 0 &&
           (c = getchar_unlocked()) != '\n' &&
           c != '\r' &&
           c != EOF)
    {
        if (i < PAM_MAX_RESP_SIZE)
        {
            input[i++] = (char)c;
        }
    }
    funlockfile(stdin);
    input[i] = '\0';
    if (noecho)
    {
        tty.c_lflag = tty_flags;
        (void)ioctl(fileno(stdin), TCSETAW, &tty);
        (void)fputc('\n', stdout);
    }
    (void)signal(SIGINT, sig);
    if (ctl_c == 1)
        (void)kill(getpid(), SIGINT);
    return (strdup(input));
}
/* Service modules do not clean up responses if an error is returned.
 * Free responses here.
 */
static void
free_resp(int num_msg, struct pam_response *pr)
{
    int i;
    struct pam_response *r = pr;
    if (pr == NULL)
        return;
    for (i = 0; i < num_msg; i++, r++)
    {
        if (r->resp)
        {
            /* clear before freeing -- may be a password */
            bzero(r->resp, strlen(r->resp));
            free(r->resp);
            r->resp = NULL;
        }
    }
    free(pr);
}
/* ARGSUSED */
int pam_tty_conv(int num_msg, const struct pam_message **mess,
                 struct pam_response **resp, void *my_data)
{
    const struct pam_message *m = *mess;
    struct pam_response *r;
    int i;
    if (num_msg <= 0 || num_msg >= PAM_MAX_NUM_MSG)
    {
        (void)fprintf(stderr, "bad number of messages %d "
                              "<= 0 || >= %d\n",
                      num_msg, PAM_MAX_NUM_MSG);
        *resp = NULL;
        return (PAM_CONV_ERR);
    }
    if ((*resp = r = calloc(num_msg,
                            sizeof(struct pam_response))) == NULL)
        return (PAM_BUF_ERR);
    /* Loop through messages */
    for (i = 0; i < num_msg; i++)
    {
        int echo_off;
        /* bad message from service module */
        if (m->msg == NULL)
        {
            (void)fprintf(stderr, "message[%d]: %d/NULL\n",
                          i, m->msg_style);
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
        switch (m->msg_style)
        {
        case PAM_PROMPT_ECHO_OFF:
            printf("debug:272\n");
            echo_off = 1;
            /*FALLTHROUGH*/
        case PAM_PROMPT_ECHO_ON:
            printf("debug:276\n");
            (void)fputs(m->msg, stdout);
            r->resp = getinput(echo_off);
            break;
        case PAM_ERROR_MSG:
            printf("debug:281\n");
            (void)fputs(m->msg, stderr);
            (void)fputc('\n', stderr);
            break;
        case PAM_TEXT_INFO:
            printf("debug:286\n");
            (void)fputs(m->msg, stdout);
            (void)fputc('\n', stdout);
            break;
        default:
            (void)fprintf(stderr, "message[%d]: unknown type "
                                  "%d/val=\"%s\"\n",
                          i, m->msg_style, m->msg);
            /* error, service module won't clean up */
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
    free_resp(i, r);
    *resp = NULL;
    return (PAM_CONV_ERR);
}