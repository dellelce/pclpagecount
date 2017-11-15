/* 
 *
 * PCL Sample Program v1.0
 *
 */

#include "pcl.h"

int
main (int argc, char **argv)
{
  FILE         *fh;
  pclsession_t *Session;
  char         *fname;
#ifdef DEBUGENV
  char         *debugenv;
#endif

#ifndef DEBUGENV
  /* This must be modified to support stdin as input */

  if (argc < 2)
    {
      printf ("Not enough arguments. First argument must be a filename\n");
      return 1;
    }

  fname = argv[1];

  fh = fopen (fname, "r");
  
  if (!fh)
    {
      printf ("Cannot open input file: %s\n", fname);
      return 1;
    }

#else

  debugenv = getenv ("DEBUGPRN");

  fname = debugenv;

  if (!fname)
    {
      if (argc < 2)
        {
          printf ("Not enough arguments. First argument must be a filename\n");
          return 1;
        }

      fname = argv[1];

      fh = fopen (fname, "r");

      if (!fh)
        {
          printf ("Cannot open input file: %s\n", fname);
          return 1;
        }
    }
  else
    {
      fh = fopen (fname, "r");

      if (!fh)
        {
          printf ("Cannot open input file: %s\n", fname);
          return 1;
        }
    }
     
#endif

  printf ("Filename : %s\n", fname);

  Session = pcl_newsession (fh);

  if (!Session)
    {
      printf ("Error while allocating session");
      return 1;
    }

#ifdef DEBUG
   
  /* Let's allocate debug structure */

  {
    FILE *fh;
    char *Var;

    Var = getenv ("DEBUGFILE");

    if (!Var || !*Var)
      {
        Var = DEFAULT_DEBUG_FILE;
      }

    fh = fopen (Var, "a");

    if (!fh)
      {
        printf ("** WARNING: Error while opening DEBUG file! DEBUGGING desabled!\n");
      }
    else
      {
        Session->DebugFH = fh;
      }
  }
#endif

  if (pcl_parse (Session))
    {
      /* pcl_parse returns a false !! - it failed somewhere */

      printf ("Error while parsing..\n");
      if (errno)
        {
          perror ("finishing parsing");
        }

      pcl_closesession (Session);
  
      return 1;
    }

  if (Session->PageCount)
    {
      printf ("Pages in file:    %d\n", Session->PageCount);
    }
  else
    {
      printf ("Pages in file:    1\n");
    }

  pcl_closesession (Session);

  return 0;
}
