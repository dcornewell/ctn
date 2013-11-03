/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore, 28-Oct-95
** Intent:		Provide a facility for binary logging.
** Last Update:		$Author: smm $, $Date: 1999-04-14 19:23:57 $
** Source File:		$RCSfile: blg.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

typedef struct {
    long date;			/* DICOM date */
    double time;		/* DICOM time */
    struct timeval binaryTime;	/* Binary time, includes fractions */
    U32 flag;			/* Arbitrary flag set by user */
    unsigned long length;	/* Amount of data in packet */
    void *d;			/* Pointer to data */
}   BLG_PACKET;

#define DELTATIME(A,B) ((double)((B).tv_sec-(A).tv_sec)) + \
        (((double)((B).tv_usec-(A).tv_usec)) /1000000.0)

void BLG_Create(char *directory, char *fileName, void **handle);
void BLG_Open(char *fileName, void **handle);
void BLG_Close(void **handle);
void BLG_Write(void **handle, U32 flag, void *buffer, unsigned long length);
BLG_PACKET *BLG_Read(void **handle);
void BLG_Free(BLG_PACKET ** packet);
