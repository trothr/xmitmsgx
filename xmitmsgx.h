/*
 *
 *        Name: xmitmsgx.h, "transmit message" for POSIX
 *              read and emit enumerated messages from the repository
 *      Author: Rick Troth, rogue programmer
 *        Date: 2014-May-01 (Thu)
 *              2017-Nov-23 (Thu) Thanksgiving 2017
 *
 */

#ifndef _XMITMSG_H
#define _XMITMSG_H

/*               _DEBUG             SYSLOG 7 */
#define  MSGLEVEL_INFO      'I'  /* SYSLOG 6 */
#define  MSGLEVEL_REPLY     'R'  /* SYSLOG 5 NOTICE */
#define  MSGLEVEL_WARNING   'W'  /* SYSLOG 4 */
#define  MSGLEVEL_ERROR     'E'  /* SYSLOG 3 */
#define  MSGLEVEL_SEVERE    'S'  /* SYSLOG 2 CRIT */
#define  MSGLEVEL_TERMINAL  'T'  /* SYSLOG 1 ALERT */
/*               _EMERG             SYSLOG 0 */

/* the following are used by derivative functions, not by msgmake() itself */
#define  MSGFLAG_SYSLOG   0x01
#define  MSGFLAG_NOCODE   0x02
#define  MSGFLAG_NOPRINT  0x04


typedef struct MSGSTRUCT
  {
    int  msgnum;        /* message number */
    int  msgc;          /* count of replacement tokens */
    char **msgv;        /* vector of replacement tokens */
    unsigned char *msgbuf;   /* supplied by caller */
    int  msglen;        /* buffer size on input, msg length on return */
    unsigned char *msgtext;   /* offset past msg code/header */

    int  msgfmt;        /* message format number (for future use) */
    int  msgline;       /* message line number (for future use, zero means all) */
    int  msglevel;      /* message level/serverity (zero means use the letter in the file) */

    /* the following are probably not for external use */
    char *applid;       /* default is basename of messages file, optional SYSLOG identity */
    char *caller;       /* default is getenv("LOGNAME") roughly, msgu */
    char *prefix;       /* default is applid[0..2]||caller[0..2] */
    char *letter;       /* default taken from message file */

    /* the following are filled in by msgopen() not for external use */
    int  msgmax;        /* highest message number in table */
    char **msgtable;    /* array of messages (allocated memory) */
    char *msgdata;      /* messages file content (allocated memory) */
    char *msgfile;      /* name of message file found (for debugging) */

    char  pfxa[4];      /* truncated up-cased applid */
    char  pfxc[4];      /* truncated up-cased caller */
    char  locale[24];   /* possibly truncated to match the nearest file found */

    int  msgopts;
    char *escape;

  } MSGSTRUCT;


/* Open the messages file, read it, get ready for service. */
extern int msgopen(const char*,int,struct MSGSTRUCT*);
/* filename, flags, MSGSTRUCT */
/* Specify a syslog ident via applid in MSGSTRUCT. */
/* specify a syslog facility via optional MSGSTRUCT */

/* this is the heart of the utility */
extern int msgmake(struct MSGSTRUCT*);

/* follows snprintf() */
extern int msgprint(int,int,char**,int);
/* msgnum, msgc, msgv, flag */

extern int msgclose(struct MSGSTRUCT*);

#endif


