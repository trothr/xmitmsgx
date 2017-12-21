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

/* xmitmsgx-2.0.18 */
#define  XMITMSG_VERSION  (((2) << 24) + ((0) << 16) + ((18) << 8) + (0))

/*       MSGLEVEL_DEBUG             SYSLOG 7 */
#define  MSGLEVEL_INFO      'I'  /* SYSLOG 6 */
#define  MSGLEVEL_REPLY     'R'  /* SYSLOG 5 NOTICE */
#define  MSGLEVEL_WARNING   'W'  /* SYSLOG 4 */
#define  MSGLEVEL_ERROR     'E'  /* SYSLOG 3 */
#define  MSGLEVEL_SEVERE    'S'  /* SYSLOG 2 CRIT */
#define  MSGLEVEL_TERMINAL  'T'  /* SYSLOG 1 ALERT */
/*       MSGLEVEL_EMERG             SYSLOG 0 reserved */

/* the following are used by derivative functions, not by xmmake() itself */
#define  MSGFLAG_SYSLOG   0x01
#define  MSGFLAG_NOLOG    0x02
#define  MSGFLAG_NOCODE   0x04
#define  MSGFLAG_NOPRINT  0x08


typedef struct MSGSTRUCT
  {
    int  msgnum;                /* message number */
    int  msgc;                  /* count of replacement tokens */
    unsigned char **msgv;       /* vector of replacement tokens */
    unsigned char *msgbuf;      /* buffer supplied by caller */
    int  msglen;                /* buffer size on input, msg size on return */
    unsigned char *msgtext;     /* offset past msg code/header */

    int  msgfmt;        /* message format number (for future use) */
    int  msgline;       /* message line number (for future use, zero means all) */
    int  msglevel;      /* message level/serverity (zero means use the letter in the file) */
    int  msgopts;       /* set by xmopen(), sometimes overridden for xmmake() */

    /* the following are probably not for external use */
    unsigned char *caller;       /* default is getenv("LOGNAME") roughly, msgu */
    unsigned char *prefix;       /* default is applid[0..2]||caller[0..2] */
    unsigned char *letter;       /* default taken from message file */

    /* the following are filled in by xmopen() not for external use */
    int  msgmax;                /* highest message number in table */
    unsigned char **msgtable;   /* array of messages (allocated memory) */
    unsigned char *msgdata;     /* messages file content (allocated memory) */
    unsigned char *msgfile;     /* name of message file found (for debugging) */
    unsigned char *escape;

    unsigned char  pfxmaj[4];   /* truncated up-cased applid/major */
    unsigned char  pfxmin[4];   /* truncated up-cased caller/minor */
    unsigned char  locale[24];  /* possibly truncated to match the nearest file found */
    unsigned char  applid[16];  /* default is basename of messages file, used as SYSLOG identity */

  } MSGSTRUCT;


/* Open the messages file, read it, get ready for service. */
extern int xmopen(const unsigned char*,int,struct MSGSTRUCT*);
/* filename, opts, MSGSTRUCT */
/* Specify a syslog ident via applid in MSGSTRUCT. */
/* specify a syslog facility via optional MSGSTRUCT */

/* This is the heart of the utility. */
extern int xmmake(struct MSGSTRUCT*);

/* Print to stdout or stderr depending on level, optionally syslog. */
extern int xmprint(int,int,unsigned char**,int,struct MSGSTRUCT*);
/* msgnum, msgc, msgv, opts */

/* Write to file descriptor, optionally syslog. */
extern int xmwrite(int,int,int,unsigned char**,int,struct MSGSTRUCT*);
/* fd, msgnum, msgc, msgv, opts */

/* Generate a message and store it as a string. */
extern int xmstring(unsigned char*,int,int,int,unsigned char**,struct MSGSTRUCT*);
/* output, outlen, msgnum, msgc, msgv */

/* Clear the message repository struct. */
extern int xmclose(struct MSGSTRUCT*);

#endif


