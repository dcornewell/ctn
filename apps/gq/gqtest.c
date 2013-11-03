#include <stdio.h>
#include <math.h>
#include "dicom.h"
#include "gq.h"

int atoi(char *s);
void pp(void *);

typedef struct _Q {
    int a;
    char b[10];
    long z;
}   Q;

main(int argc, char *argv[])
{

    CONDITION r;

    Q   e;

    if (GQ_GetQueue(1, sizeof(Q)) != GQ_NORMAL) {
	printf("\nGQ_GetQueue failed!\n\n");
	COND_DumpConditions();
	exit(-1);
    }
    e.a = 1;
    strcpy(e.b, "Element 1");
    e.z = 11;
    if ((r = GQ_Enqueue((void *) &e)) != GQ_NORMAL) {
	if (r == GQ_QUEUEFULL)
	    printf("\nThe queue is full!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    e.a = 2;
    strcpy(e.b, "Element 2");
    e.z = 22;
    if ((r = GQ_Enqueue((void *) &e)) != GQ_NORMAL) {
	if (r == GQ_QUEUEFULL)
	    printf("\nThe queue is full!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    e.a = 3;
    strcpy(e.b, "Element 3");
    e.z = 33;
    if ((r = GQ_Enqueue((void *) &e)) != GQ_NORMAL) {
	if (r == GQ_QUEUEFULL)
	    printf("\nThe queue is full!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    if ((r = GQ_Dequeue((void *) &e)) != GQ_NORMAL) {
	if (r == GQ_QUEUEEMPTY)
	    printf("\nThe queue is empty!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    pp(&e);
    e.a = 4;
    strcpy(e.b, "Element 4");
    e.z = 44;
    if ((r = GQ_Enqueue((void *) &e)) != GQ_NORMAL) {
	if (r == GQ_QUEUEFULL)
	    printf("\nThe queue is full!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    if ((r = GQ_PrintQueue(pp)) != GQ_NORMAL) {
	if (r == GQ_QUEUEFULL)
	    printf("\nThe queue is full!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    exit(0);
}
void
pp(void *e)
{

    Q  *f;

    f = (Q *) e;
    printf("a: %d\n", f->a);
    printf("b: %s\n", f->b);
    printf("z: %d\n", f->z);
    return;
}
