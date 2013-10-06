/*
 *
 * PCL Definitions by Antonio Dell'elce
 *
 */

#ifndef PCL_H
#define PCL_H 1


/* Optional local configuration file -- maybe use GNU configure in the future? */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* System includes -- may depend on optional config.h -- _must_ be included after config.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* 
 * Local types
 *
 */

#ifndef ulong
#define ulong unsigned long
#endif

/* 
 * WARNING: Static-sized token can be overflowed and make
 * the program crash and pose a security risk.
 *
 */

#ifndef PCL_TOKEN_SIZE
#define PCL_TOKEN_SIZE 1024
#endif

#ifndef PJL_LINE_SIZE
#define PJL_LINE_SIZE 1024
#endif

/* ---- Session Structure ---- */

/* pcl_sessiom
 *
 *
 */

typedef struct _pcl_session
{
  int    Language;                /* PCLXL, PCL5, PJL or 'None'??                         */
  int    Status;                  /* Our SM Status                                        */
  int    LocalStatus;             /* Information depending on main Status                 */
  int    NumberStatus;            /* Number Status -- Only PCL5e                          */
  FILE  *In;                      /* Our PCL file                                         */
  char   Token[PCL_TOKEN_SIZE];   /* Input Token                                          */
  int    TokenSize;               /* Current token size                                   */
  char   Line[PJL_LINE_SIZE];
  int    LineSize;                /* ???? Not sure if used                                */
  int    LastEvent;               /* Everything saved here                                */
  int    PageCount;               /* (?)This will have PageCnt                            */
  ulong  IgnoreCnt;               /* Bytes left to ignore                                 */
#ifdef DEBUG
  FILE  *DebugFH;                 /* Debugging File Handle                                */
  ulong  ByteCnt;

#define debugfh DebugFH
#endif
} pclsession_t;

/* statuses -- for Languages: 'NONE' and 'PCL' */

#define PCL_STATUS_NORMAL     1
#define PCL_STATUS_COMMAND    2
#define PCL_STATUS_SKIPONLY   3 /* Ignores N chars                  */
#define PCL_STATUS_READTERM   4 /* read term                        */
#define PCL_STATUS_READPARC   5 /* after reading a parametrized cmd */
#define PCL_STATUS_PARCSTEP1  6
#define PCL_STATUS_PARCTEST1  7 /* Temp KLUDGY Solution             */

/* 'NONE' language definitions */

#define NONE_STATUS_NULL      0 /* Wait for '@'               */
#define NONE_STATUS_NORMAL    1 /* Wait for new line          */
#define NONE_STATUS_NEWLINE   2 /* Wait for Reading after NL  */
#define NONE_STATUS_PREPCL6   3 /* Found PCLXL Heading        */

/* statuses -- for Languages: 'PJL' -- THIS WILL BE REMOVED */

#define PJL_STATUS_NORMAL     1 /* Wait for a Token                  */
#define PJL_STATUS_TOKEN      2 /* Processing a Token                */
#define PJL_STATUS_WAIT       3 /* Waiting for complete PJL sequence */

/* statuses -- for Languages: 'PCLXL' (only two statuses)            */

#define PCLXL_STATUS_NORMAL     1 /* Wait for a Token                    */
#define PCLXL_STATUS_READIG8    2 /* Read a single byte                  */
#define PCLXL_STATUS_IGNORE     3 /* Ignoring byteSs...                  */
#define PCLXL_STATUS_READBAR    4 /* Read two bytes for ByteArray Header */
#define PCLXL_STATUS_READDL32   5 /* Data length 32.. reading 32ulong    */

/* ESCape character - PCL (up to v5) */

#ifndef PCL_ESCAPE
#define PCL_ESCAPE 27
#endif

#ifndef PCL_FORMFEED
#define PCL_FORMFEED 12
#endif


/* Languages */

#define LANG_NONE  0 /* No language!  */
#define LANG_PJL   1 /* PJL           */
#define LANG_PCL   2 /* PCL up to v5  */
#define LANG_PCLXL 3 /* PCL-XL        */

/* PJL header skipping KLUDGE - Not used anymore */

#define PJL_LSTATUS_SEQSEARCH         0 /* Only reacehd if WAITMAINSEQ failed */
#define PJL_LSTATUS_WAITMAINSEQ       1
#define PJL_LSTATUS_MAINSEQFOUND      2 /* Waiting for PCL-XL header */
#define PJL_LSTATUS_WAITENDSEQ        3
#define PJL_LSTATUS_ENDSEQFOUND       4
#define PJL_LSTATUS_IGNORE5           5
#define PJL_LSTATUS_IGNORELAST        6

/* Macros */

#define SETLANG(x,y)       x->Language = y
#define SETSTATUS(x,y,z)   x->Status = y; x->LocalStatus = z
#define ESETSTATUS(x,y)    x->Status = y; x->LocalStatus = 0
#define SETLSTATUS(x,s)    x->LocalStatus = s

/* Defs for PCL-XL ... */

#ifdef HAVE_PCLXLFUNC_TYPEDEF
typedef int (pclxlfunc_t) (pclsession_t *Session, int Ch);
#endif

/* Protos */

pclsession_t *pcl_newsession (FILE *fh);
int pcl_closesession (pclsession_t *Session);
int pcl_parse (pclsession_t *Session);
int pcl5_parser (pclsession_t *Session, int Ch);

/* --- DEBUGGING --- */


/* DEFs - Debug message type?? (not sure if really needed...) */

#define D_DEFAULT  1
#define D_URGENT   2
#define D_FATAL    3

#ifdef DEBUG

/* MACROS */

inline static
char *debugcode2str (int code)
{
  switch (code)
    {
    case D_DEFAULT:

      return "D_DEFAULT";

    case D_URGENT:

      return "D_URGENT";

    case D_FATAL:

      return "D_FATAL";

    default:
    
      return "WARNING_UNKNOWN_CODE";
    }
}

#define SDEBUG(x,y,z)    fprintf(x->DebugFH, "DEBUG: (Level: %s) %s\n", debugcode2str(y), z)

#define D_printf(x,...)  printf(x, __VA_ARGS__)

#ifndef DEFAULT_DEBUG_FILE
#define DEFAULT_DEBUG_FILE "debug.log"
#endif

#else

#define SDEBUG(x,y,z)
#define D_printf(x,...)  ;

#endif /* DEBUG */

#endif /* PCL_H */
