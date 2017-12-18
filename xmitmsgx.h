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

/* xmitmsgx-2.0.17 */
#define  XMITMSG_VERSION  (((2) << 24) + ((0) << 16) + ((17) << 8) + (0))

/*       MSGLEVEL_DEBUG             SYSLOG 7 */
#define  MSGLEVEL_INFO      'I'  /* SYSLOG 6 */
#define  MSGLEVEL_REPLY     'R'  /* SYSLOG 5 NOTICE */
#define  MSGLEVEL_WARNING   'W'  /* SYSLOG 4 */
#define  MSGLEVEL_ERROR     'E'  /* SYSLOG 3 */
#define  MSGLEVEL_SEVERE    'S'  /* SYSLOG 2 CRIT */
#define  MSGLEVEL_TERMINAL  'T'  /* SYSLOG 1 ALERT */
/*       MSGLEVEL_EMERG             SYSLOG 0 reserved */

/* the following are used by derivative functions, not by msgmake() itself */
#define  MSGFLAG_SYSLOG   0x01
#define  MSGFLAG_NOLOG    0x02
#define  MSGFLAG_NOCODE   0x04
#define  MSGFLAG_NOPRINT  0x08


typedef struct MSGSTRUCT
  {
    int  msgnum;        /* message number */
    int  msgc;          /* count of replacement tokens */
    char **msgv;        /* vector of replacement tokens */
    unsigned char *msgbuf;   /* buffer supplied by caller */
    int  msglen;        /* buffer size on input, msg size on return */
    unsigned char *msgtext;   /* offset past msg code/header */

    int  msgfmt;        /* message format number (for future use) */
    int  msgline;       /* message line number (for future use, zero means all) */
    int  msglevel;      /* message level/serverity (zero means use the letter in the file) */
    int  msgopts;       /* set by msgopen(), sometimes overridden for msgmake() */

    /* the following are probably not for external use */
    char *caller;       /* default is getenv("LOGNAME") roughly, msgu */
    char *prefix;       /* default is applid[0..2]||caller[0..2] */
    char *letter;       /* default taken from message file */

    /* the following are filled in by msgopen() not for external use */
    int  msgmax;        /* highest message number in table */
    char **msgtable;    /* array of messages (allocated memory) */
    char *msgdata;      /* messages file content (allocated memory) */
    char *msgfile;      /* name of message file found (for debugging) */
    char *escape;

    char  pfxmaj[4];    /* truncated up-cased applid/major */
    char  pfxmin[4];    /* truncated up-cased caller/minor */
    char  locale[24];   /* possibly truncated to match the nearest file found */
    char  applid[16];   /* default is basename of messages file, used as SYSLOG identity */

  } MSGSTRUCT;


/* Open the messages file, read it, get ready for service. */
extern int msgopen(const char*,int,struct MSGSTRUCT*);
/* filename, opts, MSGSTRUCT */
/* Specify a syslog ident via applid in MSGSTRUCT. */
/* specify a syslog facility via optional MSGSTRUCT */

/* This is the heart of the utility. */
extern int msgmake(struct MSGSTRUCT*);

/* Print to stdout or stderr depending on level, optionally syslog. */
extern int msgprint(int,int,char**,int,struct MSGSTRUCT*);
/* msgnum, msgc, msgv, opts */

/* Write to file descriptor, optionally syslog. */
extern int msgwrite(int,int,int,char**,int,struct MSGSTRUCT*);
/* fd, msgnum, msgc, msgv, opts */

/* Generate a message and store it as a string. */
extern int msgstring(char*,int,int,int,char**,struct MSGSTRUCT*);
/* output, outlen, msgnum, msgc, msgv */

/* Clear the message repository struct. */
extern int msgclose(struct MSGSTRUCT*);

#endif


