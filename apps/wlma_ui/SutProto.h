#ifndef __SUTPROTO_HEADER__
#define __SUTPROTO_HEADER__

static char rcsID_SutProto_h[] = "$Id: SutProto.h,v 1.1 1994-08-10 19:19:34 smm Exp $";


#ifdef _NO_PROTO
#define SUTPROTO(x) ()
#else
#define SUTPROTO(x) x
#endif

#ifdef __cplusplus
#define DCPPCLASS(x) x
#else
#define DCPPCLASS(x)
#endif

#endif
