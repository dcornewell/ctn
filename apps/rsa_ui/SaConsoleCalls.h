#ifndef __SACONSOLECALLS_HEADER__
#define __SACONSOLECALLS_HEADER__

static char rcsID_SaConsoleCalls_h[] = "$Id: SaConsoleCalls.h,v 1.1 1994-10-12 22:05:49 smm Exp $";


#include "Sut.h"

#ifdef __cplusplus
extern "C" {
#endif

    void SaDisplayTheConsole SUTPROTO(());
    void SaAppendTheConsoleText SUTPROTO((char *text));

    void SaOpenConsole SUTPROTO(());

/**** this returns allocated memeory. You should free when done *****/
    char *SaGetConsoleText SUTPROTO(());

/**** old function, do not use ****/
    void SaAppendConsoleText SUTPROTO((char *text));
/**** old function, do not use ****/
    void SaDisplayConsole SUTPROTO(());

#ifdef __cplusplus
}
#endif

#endif
