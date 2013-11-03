#ifndef __SA_ADD_LOGIN__
#define __SA_ADD_LOGIN__

static char rcsID_SaAddLogin_h[] = "$Id: SaAddLogin.h,v 1.3 1996-01-22 20:22:28 smm Exp $";


#include "SutProto.h"


/*---------------------------------------------------------------------------
	SaIsValidLogin -
		This function accepts Login information for a given database server
	and returns TRUE if the info is valid or FALSE if its invalid or an
	internal error occurs.
---------------------------------------------------------------------------*/

int SaIsValidLogin
SUTPROTO((
	  char *server,		/* Database Server name to login to */
	  char *uname,		/* Username for database login */
	  char *passwd		/* Password for database login */
	  ));


/*---------------------------------------------------------------------------
	SaAddLogin -
		Login information for all servers that a client uses is kept in a
	dynamic list of structures.  There can be only one record per server in
	the list.  Therefore, if a record already exists for a server, it must be
	specified whether to overwrite the old information or not.  Also, an
	alternate servername may be specied so that all procedures which would
	have run under the normal servername will now be run using the alternate
	server name.  This is handy for re-mapping servers at run-time.  The
	valid return values are listed below the function declaration.
---------------------------------------------------------------------------*/

int SaAddLogin
SUTPROTO((
	  char *server,		/* Database Server name to login to */
	  char *altserver,	/* Alternate Server name for normal name used
				 * in map */
	  char *uname,		/* Username for database login */
	  char *passwd,		/* Password for database login */
	  int overwrite		/* True - overwrite if login exists. False
				 * don't */
	  ));

/* Return Values for SaAddLogin */

#define SA_LOG_FAIL       0	/* Login record failed.  No changes */
#define SA_LOG_OVERWROTE  1	/* Login Overwrote an old record.   */
#define SA_LOG_NOVERWROTE 2	/* Login did not change, but exists */
#define SA_LOG_ADDED      3	/* login was added successfully     */
#define SA_LOG_DEFAULT    4	/* login was added as default       */

#endif
