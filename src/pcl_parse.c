/*
 *
 * 'PCL6' Parser code by Antonio Dell'elce.
 * See README for more.
 *
 */

#include "pcl.h"
#include "pclxlfunc.h"

/*
 * -{}- pcl_parse
 *
 * This function is the main function!
 *
 * Will process the file.
 *
 * Version 0.0
 *
 * Will count and print token information.
 *
 * Version 0.1
 * 
 * Search for ESC+E (Page Resets)
 *
 * Version 0.2
 *
 * Recognizes PJL header and ignores it.
 *
 * Version 0.3
 *
 * Base optmization for PCL-XL
 * 
 * Version 0.4/5 
 *
 * PCL-XL Completed (Maybe some minor bugs to fix).
 *
 * Version 0.6 
 * 
 * Better PJL handling and PCL5 support.
 *
 * Returns:
 *
 * Boolean value (success/fail).
 *
 */

int
pcl_parse (pclsession_t *Session)
{
  FILE *fh;
  int   Ch;
#ifdef USE_CHSTATS
  int   chvec[255];
  
  memset (chvec, 0, sizeof(chvec));
#endif

#ifdef DEBUG
  printf ("DEBUG: pcl_parse\n");
#endif

  if (!Session)
    {
      return 1;
    }

  /* we 'cache' the FILE structure */

  fh = Session->In;

  /* Validate contents of the structure */

  if (!fh)
    {
      
#ifdef DEBUG
      printf ("DEBUG: pcl_parse: bad filhandle\n");
#endif
      return 1;
    }

  /* MAIN LOOP */

  while (!feof (fh))
    {
      Ch = fgetc (fh);

#ifdef DEBUG  
      Session->ByteCnt += 1;
#endif
#ifdef USE_CHSTATS
      /* debug statistics.... */

      chvec[Ch] += 1;
#endif

      switch (Session->Language)
        {

        case LANG_NONE:

          switch (Session->Status)
            {
            case NONE_STATUS_NULL:

              if (Ch == PCL_ESCAPE)
                {
                  Session->Status = NONE_STATUS_NORMAL;
                }

              continue;

            case NONE_STATUS_NORMAL:
               
              if (Ch == 13)
                {
                  Session->Status = NONE_STATUS_NEWLINE;
                }

              continue;

            case NONE_STATUS_NEWLINE:

              switch (Ch)
                {
                case ')':
                  
                  /* We found the PCL-XL comment/header */

                  Session->Status = NONE_STATUS_PREPCL6;
                  continue;

                case PCL_ESCAPE:
                  
                  /* We found some PCL5 stuff */

                  printf ("Language : PCL\n");

                  Session->Language = LANG_PCL;
                  Session->Status   = PCL_STATUS_COMMAND;
                  Session->LocalStatus = 0;
                  continue;
                }

              break;

            case NONE_STATUS_PREPCL6:

              /* we are processing characters in the PCL6 heading */

              if (Ch == '\n')
                {
                  printf ("Language : PCLXL\n");

                  Session->Language = LANG_PCLXL;
                  Session->Status = PCLXL_STATUS_NORMAL;
                }
  
              break;

            } /* switch (Status) */

          continue;

        case LANG_PCL:

          if (pcl5_parser (Session, Ch) != 0)
            {
              /* Failed .. Should process errors here? */
              return 1;
            }
          
          break;

        case LANG_PCLXL:

          if (pclxl_parser (Session, Ch) != 0)
            {
              /* Failed .. Should process errors here? */
              return 1;
            }

          break;
        } /* switch (..Language) */
    } /* while (feof) : Reader Loop */
          
  return 0;
}
