/*
 *
 * PCL-XL Parser code by Antonio Dell'elce.
 *
 *
 */

#include "pcl.h"
#include "pclxlfunc.h"

int
pclxl_parser (pclsession_t *Session, int Input)
{
  /* 
   * The actual work is done by the functions in the 'big vector - pclxlvec (!!Better desc here..!!)
   * We only process IGNORE status here ...
   *
   */

  if (Input == EOF)
    {
      return 0;
    }

  if (Session->Status == PCLXL_STATUS_NORMAL)
    {
      if (pclxlvec[Input].Func != 0)
	{
	  if ((void*)pclxlvec[Input].Func == IGNORE_FUNC)
	    {
#ifdef DEBUG
	      printf ("DEBUG: IGNORE_FUNC for %d bytes [Input = %02x].\n", pclxlvec[Input].IgnoreCnt, Input);
#endif
	      Session->Status    = PCLXL_STATUS_IGNORE;
	      Session->IgnoreCnt = pclxlvec[Input].IgnoreCnt;
	    }
	  else
	    {
	      //      printf ("CallFunc : (Ignore=%d) %d\n", Session->IgnoreCnt, Input);
	      return pclxlvec[Input].Func (Session, Input);
	    }
	}
    }
  else
    {
      switch (Session->Status)
	{
	case PCLXL_STATUS_READIG8:
	
	  Session->Status = PCLXL_STATUS_IGNORE;
	  Session->IgnoreCnt = Input;
	  return 0;
	
	case PCLXL_STATUS_READBAR:
	  
	  switch (Session->LocalStatus)
	    {
	    case 0:

	      switch (Input)
		{
		case PCLXL_OP_UINT16:

		  D_printf ("DEBUG: READBAR OP_UINT16 [Input = %02x]\n", Input);

		  Session->LocalStatus = 1;
		  return 0;

		case PCLXL_OP_UBYTE:

		  D_printf ("DEBUG: READBAR OP_UBYTE [Input = %02x]\n", Input);
		  
		  Session->LocalStatus = 3;
		  return 0;

		default:
		  
		  D_printf ("DEBUG: Unsupported in array datatag [Input = %02x]\n", Input);
		  return 1;
		}

	    case 1: /* We need to read the size (2-bytes) - Reading first byte */
	
	
	      if (pclxlvec[Input].IgnoreCnt == 0 || pclxlvec[Input].IgnoreCnt == 1)
		{
		  Session->IgnoreCnt = Input;
		  D_printf ("DEBUG: Word16: %02x", Input); 
		}
	      else
		{
		  D_printf ("DEBUG: Word16: %02x", Input);
		  
		  Session->IgnoreCnt = Input * pclxlvec[Input].IgnoreCnt;
		}
	      Session->LocalStatus = 2; /* We can read second byte */
	      
	      return 0;

	    case 2:

	      if (pclxlvec[Input].IgnoreCnt == 0 || pclxlvec[Input].IgnoreCnt == 1)
		{
		  D_printf ("%02x\n", Input);
		  Session->IgnoreCnt += (Input << 8);
		}
	      else
		{
		  D_printf ("%02x\n", Input);
		  Session->IgnoreCnt += (Input << 8) * pclxlvec[Input].IgnoreCnt;
		}

	      Session->Status = PCLXL_STATUS_IGNORE;
	      Session->LocalStatus = 0;
	      return 0;
	    
	    case 3: /* we can read a single byte to use to ignore */

	      if (pclxlvec[Input].IgnoreCnt == 0 || pclxlvec[Input].IgnoreCnt == 1)
		{		  
		  Session->IgnoreCnt = Input;
		}
	      else
		{
		  Session->IgnoreCnt = Input * pclxlvec[Input].IgnoreCnt;
		}

	      Session->Status = PCLXL_STATUS_IGNORE;
	      Session->LocalStatus = 0;
	      return 0;
	    } /* switch (LocalStatus) Type of datatag (8/16 bits) 
                 which determines what we have to ignore          */

	  break;
	
	case PCLXL_STATUS_READDL32:
	  
	  switch (Session->LocalStatus)
	    {
	    case 0:
	      
	      /* Little endian mode.. PCL-XL is Little Endian */

#ifdef DEBUG
	      printf ("Word32: %02x", Input);
#endif

	      Session->IgnoreCnt = Input;
	      Session->LocalStatus = 1;
	      break;
	       
	    case 1:

	      Session->IgnoreCnt += Input<<8;
	      Session->LocalStatus = 2;

#ifdef DEBUG
	      printf ("%02x", Input);
#endif
	      break;

	    case 2:

	      Session->IgnoreCnt += Input<<8;
	      Session->LocalStatus = 3;

#ifdef DEBUG
	      printf ("%02x", Input);
#endif

	      break;
	      
	    case 3:

	      Session->IgnoreCnt += Input<<24;
	      Session->Status = PCLXL_STATUS_IGNORE;
	      Session->LocalStatus = 0;
	    
#ifdef DEBUG  
	      printf ("%02x\n", Input);
	      SDEBUG (Session, D_DEFAULT, "Exiting STATUS_READDL32");

	      printf ("DEBUG READDL32: %ld bytes to ignore.\n", Session->IgnoreCnt);
#endif
 
	      break;


	    default:
	      
	      SDEBUG (Session, D_DEFAULT, "READDL32 - PANIC - Unsupported status!\n");
	      break;
	    }

	  break;

	case PCLXL_STATUS_IGNORE:

	  /* Do a validity check -- than decrease IgnoreCount*/
	  
	  if (Session->IgnoreCnt)
	    {
	      if (Session->IgnoreCnt == 1)
		{
		  Session->Status = PCLXL_STATUS_NORMAL;
		  Session->IgnoreCnt = 0;
		}
	      else
		{
		  Session->IgnoreCnt -= 1;
		}
	    } /* if (Session->IgnoreCnt) */
	  else
	    {
	      /* else { ERROR !!!! } */
	      Session->Status = PCLXL_STATUS_NORMAL;
	    }
	} /* Switch (Session->Status) */
    } /* Status != PCLXL_STATUS_NORMAL */

  return 0;
}


