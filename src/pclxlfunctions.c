/*
 *
 * PCL-XL Parser code by Antonio Dell'elce.
 *
 * 'Beta' Quality.
 *
 * This file has all the function that analyze all 
 */

#include "pcl.h"


/* pclxl_endofpage
 *
 * Used only for the EndPage Operator
 *
 */
int
pclxl_endofpage (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: pclxl_endofpage [Increasing PageCount!!!]");
  printf ("DEBUG: endofpage at %ld\n", Session->ByteCnt);
#endif
  /* Increase Page Counter */

//  Session->PageCount += 1;
  
  return 0;
}

/* pclxl_startofpage
 *
 * Used only for the BeginPage Operator (see pg 47)
 *
 * hex code is 0x43 (67)
 *
 * It is ignored for now
 */

int
pclxl_startofpage (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: pclxl_startofpage");
#endif
  Session->PageCount += 1;
  return 0;
}

/* pclxl_onebyte
 *
 * Generic function for One-byte operators 
 *
 */

int
pclxl_onebyte (pclsession_t *Session, char Input)
{
  ESETSTATUS (Session, PCLXL_STATUS_IGNORE);
  Session->IgnoreCnt = 1;
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: pclxl_onebyte");
  printf ("DEBUG: onebyte [Input = %02x]\n", Input);
#endif
  return 0;
}

/* pclxl_word16
 *
 * Generic function for two-bytes operators 
 *
 */

int
pclxl_word16 (pclsession_t *Session, char Input)
{
  ESETSTATUS (Session, PCLXL_STATUS_IGNORE);
  Session->IgnoreCnt = 2;
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: pclxl_word16");
  printf ("DEBUG: word16 [Input = %02x]\n", Input);
#endif

  return 0;
}

/* pclxl_word32
 *
 * Generic function for four-bytes operators 
 *
 */

int
pclxl_word32 (pclsession_t *Session, char Input)
{
  ESETSTATUS (Session, PCLXL_STATUS_IGNORE);
  Session->IgnoreCnt = 4;
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: pclxl_word32");
  printf ("DEBUG: word32: Input  %02x\n", Input);
#endif

  return 0;
}


/* pclxl_datalength8
 *
 * 0xFB - dataLengthByte
 *
 */

int
pclxl_datalength8 (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: dataLengthByte");
#endif

  Session->Status = PCLXL_STATUS_READIG8;

  return 0;
}

/* pclxl_datalength32
 *
 * 0xFA - dataLength
 *
 */

int
pclxl_datalength32 (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: dataLength");
#endif

  Session->Status = PCLXL_STATUS_READDL32;
  Session->LocalStatus = 0;

  return 0;
}

/* pclxl_array
 *
 *  - array
 *
 */

int
pclxl_array (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: bytearray");
#endif

  Session->Status = PCLXL_STATUS_READBAR;
  Session->LocalStatus = 0;
    
  return 0;
}

/* pclxl_array32
 *
 *  - array
 *
 */

int
pclxl_array32 (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: bytearray32");
#endif

  Session->Status = PCLXL_STATUS_READBAR;
  Session->LocalStatus = 0;
    
  return 0;
}


/* pclxl_array16
 *
 *  - array
 *
 */

int
pclxl_array16 (pclsession_t *Session, char Input)
{
#ifdef DEBUG
  SDEBUG (Session, D_DEFAULT, "FUNC: bytearray16");
  printf ("DEBUG: array16: input: %02x\n", Input);
#endif

  Session->Status = PCLXL_STATUS_READBAR;
  Session->LocalStatus = 0;
    
  return 0;
}


#ifdef DEBUG

/* pclxl_beginsession
 *
 * 0x41 - BeginSession
 *
 */

int
pclxl_beginsession (pclsession_t *Session, char Input)
{
  SDEBUG (Session, D_DEFAULT, "FUNC: BeginSession");

  return 0;
}

/* pclxl_endsession
 *
 * 0x42 - EndSession
 *
 */

int
pclxl_endsession (pclsession_t *Session, char Input)
{
  SDEBUG (Session, D_DEFAULT, "FUNC: EndSession");

  return 0;
}


/* pclxl_readchar
 *
 * 0x53 - ReadChar
 *
 */

int
pclxl_readchar (pclsession_t *Session, char Input)
{
  SDEBUG (Session, D_DEFAULT, "FUNC: ReadChar");

  return 0;
}

/* pclxl_endchar
 *
 * 0x54 - EndChar
 *
 */

int
pclxl_endchar (pclsession_t *Session, char Input)
{
  SDEBUG (Session, D_DEFAULT, "FUNC: EndChar");

  return 0;
}

/* pclxl_endchar
 *
 * 0xA8 - EndChar
 *
 */

int
pclxl_textoper (pclsession_t *Session, char Input)
{
  SDEBUG (Session, D_DEFAULT, "FUNC: Text");

  return 0;
}

/* pclxl_setfont
 *
 * 0x6F - SetFont
 *
 */

int
pclxl_setfont (pclsession_t *Session, char Input)
{
  SDEBUG (Session, D_DEFAULT, "FUNC: SetFont");
  printf ("DEBUG: SetFont\n");
  return 0;
}


#endif /* DEBUG */
