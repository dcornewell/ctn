#ifndef __SUTLIST_HEADER__
#define __SUTLIST_HEADER__

static char rcsID_SutList_h[] = "$Id: SutList.h,v 1.1 1994-08-10 18:48:04 smm Exp $";


#include "Sut.h"

typedef struct SutList {
    void **values;		/* the actual values as an array */
    int num;			/* the number of actaul items */
    int siz;			/* the max size before realloc */
}   SutList;

typedef struct SutList *pSutList;

/* allocate a new list. The list must be deallocated with SutDeleteList() */
pSutList SutNewList SUTPROTO(());

/* allocate a new list. The list must be deallocated with SutDeleteList() */
pSutList SutNewListSize SUTPROTO((int siz));

/* this deletes a list. It only deletes the pointers to elements and not the elements themselves. */
/* Therefor, to prevent memomry leaks, you must have a handle to elements before using this or */
/* you must use SutRemoveNList and free each element before calling this. */
int SutDeleteList SUTPROTO((pSutList p));

/* this destoys the list and deallocates all its elements */
int SutDestroyList SUTPROTO((pSutList p));

/* returns the size of the list. 0 is an empty list */
int SutSizList SUTPROTO((pSutList p));

/* adds an element to the end of a list */
void *SutAddList SUTPROTO((pSutList p, void *item));

/* returns the Nth (0 to size-1) element. */
/* If N is out-of-range, (void*)0 is returned. */
void *SutGetNList SUTPROTO((pSutList p, int n));

/* removes the Nth (0 to size-1) and returns a pointer to the element */
/* If N is out-of-range, nothing is removed and (void*)0 is returned. */
void *SutRemoveNList SUTPROTO((pSutList p, int n));

/* removes last element. This is faster than SutRemoveNList */
void *SutRemoveList SUTPROTO((pSutList p));

/* replaces the Nth (0 to size-1) and returns a pointer to the old Nth element */
/* If N is out-of-range, nothing is replaced and (void*)0 is returned. */
void *SutReplaceNList SUTPROTO((pSutList p, int n, void *item));

/* adds the element at position N (0 to size-1). */
/* The return is the item added except when N is out-of-range in which case (void*)0 is returned. */
void *SutAddNList SUTPROTO((pSutList p, int n, void *item));

/* Creates a copy of a list with the same size and pointers to the same elements */
/* WARNING : there will then be 2 pointers to each element. Care must be taken with deallocation */
pSutList SutCopyList SUTPROTO((pSutList p));

/* given a pointer, this returns its position N (0 to size-1) if that pointer is in the list. */
/* A return of -1 indicates that the pointer is not in the list */
int SutGetPosFromItem SUTPROTO((pSutList p, void *item));

#endif
