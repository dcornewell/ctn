#ifndef __SUTPROTO_HEADER__
#define __SUTPROTO_HEADER__

static char rcsID_SutProto_h[] = "$Id: SutProto.h,v 1.1 1994-10-10 20:40:17 chander Exp $";


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
