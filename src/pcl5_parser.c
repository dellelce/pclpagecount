/*
 *
 * PCL5 Parser code by Antonio Dell'elce.
 *
 */

#include "pcl.h"

int
pcl5_parser (pclsession_t *Session, int Ch)
{
  switch (Session->Status)
    {
    case PCL_STATUS_NORMAL:
      
      if (Ch == PCL_ESCAPE)
        {
          Session->Status = PCL_STATUS_COMMAND;
          break;
        }

      if (Ch == PCL_FORMFEED)
        {
#ifdef DEBUG
          printf ("\n>>>>>>> Increasing PageCount <<<<<<<<<\n\n");
#endif
          Session->PageCount += 1;
        }
      
      break;
      
    case PCL_STATUS_COMMAND:
      
      /*        
       * Command mode
       *
       */
      
      if (Ch == 'E')
        {
          Session->Status = PCL_STATUS_NORMAL;
          break;
        }

      if (Ch == '%')
        { 
          /* possible terminator??? */
         
          Session->Status = PCL_STATUS_READTERM;
          break;
        } 
     
      if (Ch >= 33 && Ch <= 47)
        {
          /* We have a parametrized command */
#ifdef DEBUG
          printf ("\n\n*********************\nPARMCOMMAND: [%c", Ch);
#endif

          switch (Ch)
            {
            case '&': /* We need 'b' or 'p' next */

              Session->LocalStatus = 1;
              break;
              
            case '(': /* We need 'f' or 's' next */
              
              Session->LocalStatus = 2;
              break;

            case ')': /* We need 's' next */
              
              Session->LocalStatus = 3;
              break;

            case '*': /* We need 'b', 'c', 'i', 'l' or 'v' */

              Session->LocalStatus = 4;
              break;        
            }

          Session->Status = PCL_STATUS_READPARC; /* we read and skip the first char after this */
          Session->IgnoreCnt = 0; /* we just be careful here */
          break;
        }

      Session->Status = PCL_STATUS_NORMAL;
      
      break;

    case PCL_STATUS_READTERM:

#ifdef DEBUG
      printf ("---READTERM---\n");
#endif

      if (!Session->LocalStatus)
        {
          if (Ch == '-')
            {              
              Session->LocalStatus = 1;
            }
          else
            {
#ifdef DEBUG
              printf ("NonTerm Char is %c\n", Ch);
#endif
              
              Session->Status = PCL_STATUS_NORMAL;
              break;
            }
        }
      else
        {
#ifdef DEBUG
          printf ("\n---\nProcessing READTERM (Char = %c)(LocalStatus = %d)\n", Ch, Session->LocalStatus);
#endif
          
          if (Session->LocalStatus != 6)
            {
              if ((Ch-'0') != Session->LocalStatus)
                {
#ifdef DEBUG
                  printf ("Error in Sequence\n");
#endif
                  Session->LocalStatus = 0;
                  Session->Status = PCL_STATUS_NORMAL; 
                  break;
                }
              Session->LocalStatus += 1;
            }
          else
            {
              Session->LocalStatus = 0;
              Session->Status = PCL_STATUS_NORMAL; 
              break;
            }
        }

#ifdef DEBUG
      printf ("READTERM uncompleted (LocalStatus = %d)(Char = %c)(IntVal = %d)\n", Session->LocalStatus, Ch, Ch-'0');
#endif
      
      break;
      
    case PCL_STATUS_PARCTEST1:

      /* This has been inserted  for testing purposes -- Should be eliminated?? */

#ifdef DEBUG
      printf ("PARCTEST: %d [%c]\n", Ch, Ch);
#endif

      if (Ch != PCL_ESCAPE)
        {
          Session->IgnoreCnt    -= 1;
          Session->Status        = PCL_STATUS_SKIPONLY;
          Session->LocalStatus   = 0;
        }
      else
        {
          Session->Status = PCL_STATUS_COMMAND;
        }

      break;
      
    case PCL_STATUS_READPARC:
      
      /* we check here if the command sequence 
         can be followed by binary data
      */
      
      D_printf ("%c]\n", Ch);

      switch (Session->LocalStatus)
        {
        case 1: /* previous char was '&' */
          
          if (Ch == 'b')
            {
              Session->LocalStatus = 6; /* we will need W to complete sequence */
              D_printf ("READPARC: Ending LocalStatus = %d\n", Session->LocalStatus);
              break;
            }

          if (Ch == 'p')
            {
              Session->LocalStatus = 5; /* we will need X to complete sequence */
#ifdef DEBUG
              printf ("READPARC: Ending LocalStatus = %d\n", Session->LocalStatus);
#endif
              break;
            }

          /* sequence will not continue */
          Session->LocalStatus = 0;
          break;

        case 2:

          if (Ch == 'f' || Ch == 's')
            {
              Session->LocalStatus = 6; /* we will need W to complete sequence */
#ifdef DEBUG
              printf ("READPARC: Ending LocalStatus = %d\n", Session->LocalStatus);
#endif
              break;
            }

          /* sequence will not continue */
          Session->LocalStatus = 0;
          break;

        case 3:

          if (Ch == 's')
            {
              Session->LocalStatus = 6; /* we will need W to complete sequence */
#ifdef DEBUG
              printf ("READPARC: Ending LocalStatus = %d\n", Session->LocalStatus);
#endif
              break;
            }

          /* sequence will not continue */
          Session->LocalStatus = 0;
          break;
          
        case 4:

          if (Ch == 'b')
            {
              Session->LocalStatus = 7; /* we will need W or V to complete sequence */
#ifdef DEBUG
              printf ("READPARC: Ending LocalStatus = %d\n", Session->LocalStatus);
#endif
              break;
            }

          if (Ch == 'c' || Ch == 'i' || Ch == 'l' || Ch == 'l')
            {
              Session->LocalStatus = 6; /* we will need W to complete sequence */
#ifdef DEBUG
              printf ("READPARC: Ending LocalStatus = %d\n", Session->LocalStatus);
#endif
              break;
            }

          /* sequence will not continue */
          Session->LocalStatus = 0;
          break;
        }

      Session->NumberStatus = 0;
      Session->Status = PCL_STATUS_PARCSTEP1;
      break;
      
    case PCL_STATUS_PARCSTEP1:

#ifdef DEBUg
      printf ("READPARC1\n");
#endif

      /* value field - must handle also . and fractional part (ignore?) */

      if (Ch >= '0' && Ch <= '9')
        {
          /* fill in this area */

#ifdef DEBUG
          printf ("Number: %d\n", Ch-'0');
#endif
          
          if (!Session->LocalStatus)
            {
              Session->IgnoreCnt = Ch-'0';
              Session->NumberStatus = 1;
            }
          else
            {
              Session->IgnoreCnt *= 10;
              Session->IgnoreCnt += Ch-'0';
              Session->NumberStatus += 1;
            }

          break;
        }
      else
        {
          Session->NumberStatus = 0;
        }

#ifdef DEBUG
      if (Ch == '.')
        {
          printf ("Possible Fractional????\n");
        }
#endif

      /* 
         Parameter Character - Any character from the
         ASCII table within the range 96-126 decimal
         ("`" through "~"). This character specifies
         the parameter to which the previous value
         eld applies. This character is used when
         combining escape sequences.
      */

      if (Ch >= 96 && Ch <= 126)
        {
#ifdef DEBUG
          printf ("ParChar: %c (LocalStatus = %d)\n", Ch, Session->LocalStatus);
#endif
          Session->IgnoreCnt = 0;
          break;
        }

      /*
        Termination Character - Any character
        from the ASCII table within the range 64-94
        decimal ("@" through "^"). This character
        species the parameter to which the previous
        value field applies. This character terminates
        the escape sequence.
      */


      if (Ch >= 64 && Ch <= 94)
        {
          /* If IgnoreCnt is set then we must check if there is
             actually something to ignore....
             How we do it ... we check if LocalStatus and Ch match!!!!
             No magics... just 'hi-performace' PCL5 stuff...
          */

          if (Session->IgnoreCnt && Session->LocalStatus)
            {
              
              if (Session->LocalStatus == 5)
                {
                  if (Ch == 'X')
                    {
#ifdef DEBUG
                      printf ("!!!! IGNORE Sequence Complete [Ch = %c, LocalStatus = %d]\n", Ch, Session->LocalStatus);
#endif
                      Session->Status = PCL_STATUS_SKIPONLY;
                      Session->LocalStatus = 0;
                      break;
                    }
                }

              if (Session->LocalStatus == 6)
                { 
                  if (Ch == 'W')
                    {
#ifdef DEBUG
                      printf ("!!!! IGNORE Sequence Complete [Ch = %c, LocalStatus = %d]\n", Ch, Session->LocalStatus);
#endif
                      Session->Status = PCL_STATUS_SKIPONLY;
                      Session->LocalStatus = 0;
                      break;
                    }
                }

              if (Session->LocalStatus == 7)
                { 
                  if (Ch == 'V' || Ch == 'W')
                    {
#ifdef DEBUG
                      printf ("!!!! IGNORE Sequence Complete [Ch = %c, LocalStatus = %d]\n", Ch, Session->LocalStatus);
#endif
                      Session->Status = PCL_STATUS_SKIPONLY;
                      Session->LocalStatus = 0;
                      break;
                    }
                }
            }


          Session->Status = PCL_STATUS_NORMAL;

          /* anti-bug cleanup??? just a bit paranoic */
          Session->LocalStatus = 0;
          Session->NumberStatus = 0;
          
#ifdef DEBUG
          printf ("TermChar: %c\n", Ch);
#endif
          break;
        }

      /* we now are reading the rest of it */

      break;
      
    case PCL_STATUS_SKIPONLY:
      
#ifdef DEBUG
      if (!Session->LocalStatus)
        {
          printf ("\n---> SKIPONLY IgnoreCount = %ld\n", Session->IgnoreCnt);
          Session->LocalStatus = 1;
        }
#endif

      /* will skip N characters -- not the most efficient? but very nice! */
      
      if (Session->IgnoreCnt != 1)
        {
          Session->IgnoreCnt -= 1;
        }
      else
        {
#ifdef DEBUG

      if (Session->LocalStatus)
        {
          printf ("---> SKIPONLY LastChar is %c [%d]\n", Ch, Ch);
          Session->LocalStatus = 0;
        }
#endif

          Session->Status = PCL_STATUS_NORMAL;
        }
      break;

       
    default:
      /*
        printf ("Unsupported status.(SHOULD NEVER GET HERE) (Status = %d)\n", Session->Status);
      */
      break;
    } /* switch (..Status) */
 
  return 0;
}
