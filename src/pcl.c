/*
 *
 * PCL Parser code by Antonio Dell'elce.
 * See README for more.
 *
 */

#include "pcl.h"

/* --- PCL functions here --- */

/*
 * -{}- pcl_newsession
 *
 * Creates an empty session structure 
 * with default values.
 *
 *
 * Returns:
 *
 * Pointer to structut pclsession_t
 *
 */

pclsession_t *
pcl_newsession (FILE *fh)
{
  pclsession_t *Session;

  /* Minimal Sanity Check */

  if (!fh)
    {
      return 0;
    }

  Session = malloc (sizeof (pclsession_t));
 
  if (!Session) /* Malloc error */
    {
      return 0;
    }

  memset (Session, 0, sizeof (pclsession_t));

  Session->Status   = NONE_STATUS_NULL;
  Session->Language = LANG_NONE;
  Session->In = fh;
  
  return (Session);
}

/*
 * -{}- pcl_closesession
 *
 * Deletes a session structure
 *
 *
 * Returns:
 *
 * 'Boolean' style result.
 *
 */

int
pcl_closesession (pclsession_t *Session)
{
  if (Session)
    {
      free (Session);
      return 0;
    }
  else
    {
      return 1;
    }
}
