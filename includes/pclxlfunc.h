/*
 *
 * PCL-XL Parser code by Antonio Dell'elce.
 *
 * Filename: pclxlfunc.h
 *
 * Prototypes for 'most' of PCLXL prototypes.
 * Other PCLXL related definitions are in pcl.h
 *
 */

#ifndef PCLXLFUNC_H
#define PCLXLFUNC_H 1

#include "pcl.h"

/*
 * All protos
 *
 */

int pclxl_parser (pclsession_t *Session, int Input);

int pclxl_endofpage (pclsession_t *Session, char Input);
int pclxl_startofpage (pclsession_t *Session, char Input);
int pclxl_onebyte (pclsession_t *Session, char Input);
int pclxl_word16 (pclsession_t *Session, char Input);
int pclxl_word32 (pclsession_t *Session, char Input);

int pclxl_datalength8 (pclsession_t *Session, char Input);
int pclxl_datalength32 (pclsession_t *Session, char Input);

//int pclxl_ignoregeneric (pclsession_t *Session, char Input);

int pclxl_array (pclsession_t *Session, char Input);
int pclxl_array32 (pclsession_t *Session, char Input);
int pclxl_array16 (pclsession_t *Session, char Input);

/* 
 * These functions are only useful for debugging 
 *
 */

#ifdef DEBUG
int pclxl_beginsession (pclsession_t *Session, char Input);
int pclxl_endsession (pclsession_t *Session, char Input);
int pclxl_endchar (pclsession_t *Session, char Input);
int pclxl_readchar (pclsession_t *Session, char Input);
int pclxl_textoper (pclsession_t *Session, char Input);
int pclxl_setfont (pclsession_t *Session, char Input);
#endif

/* Vector Definition */

#ifdef HAVE_PCLXLFUNC_TYPEDEF

typedef struct xlvecitem_struct
{
  pclxlfunc_t Func;
  int         SkipCount;
} xlvecitem_t;

#else

typedef struct xlvecitem_struct
{
  int       (*Func) (pclsession_t *Session, int Ch);
  int         IgnoreCnt;
} xlvecitem_t;

#endif /* HAVE_PCLXLFUNC_TYPEDEF */

extern xlvecitem_t pclxlvec[];

#define IGNORE_FUNC  (void *)-1

/* Special operators used for arrays.... */

#define PCLXL_OP_UINT16    0xC1
#define PCLXL_OP_UBYTE     0xC0

#endif /* PCLXLFUNC_H */
