#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GCCSUNOS
#include <stddef.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "manage.h"

#include "format.h"

void
formatAE(DMAN_APPLICATIONENTITY * ae, int index, char *buf)
{
    char
        temp[30];

    buf[0] = '\0';
    if (ae->Flag & DMAN_K_APPLICATION_TITLE)
	sprintf(temp, "%16s", ae->Title);
    else
	sprintf(temp, "%16s", "");
    strcat(buf, temp);

    if (ae->Flag & DMAN_K_APPLICATION_NODE)
	sprintf(temp, "%20s", ae->Node);
    else
	sprintf(temp, "%20s", "");
    strcat(buf, temp);

    if (ae->Flag & DMAN_K_APPLICATION_ORGANIZATION)
	sprintf(temp, "%20s", ae->Organization);
    else
	sprintf(temp, "%20s", "");
    strcat(buf, temp);

    if (ae->Flag & DMAN_K_APPLICATION_PORT)
	sprintf(temp, "%10d", ae->Port);
    else
	sprintf(temp, "%10s", "");
    strcat(buf, temp);

    if (ae->Flag & DMAN_K_APPLICATION_COMMENT) {
	strcat(buf, "   ");
	strcat(buf, ae->Comment);
    }
}

int
compareAE(DMAN_APPLICATIONENTITY * ae1, DMAN_APPLICATIONENTITY * ae2)
{
    if (strcmp(ae1->Organization, ae2->Organization) < 0)
	return -1;
    if (strcmp(ae1->Organization, ae2->Organization) > 0)
	return 1;

    if (strcmp(ae1->Title, ae2->Title) < 0)
	return -1;
    if (strcmp(ae1->Title, ae2->Title) > 0)
	return 1;

    if (strcmp(ae1->Node, ae2->Node) <= 0)
	return -1;
    else
	return 1;
}

void
formatSecurityMatrix(DMAN_SECURITYMATRIX * s, int index, char *buf)
{
    (void) sprintf(buf, "%-16s     %-16s", s->RequestingTitle,
		   s->RespondingTitle);
}

int
compareSecurityMatrix(DMAN_SECURITYMATRIX * sm1,
		      DMAN_SECURITYMATRIX * sm2)
{
    if (strcmp(sm1->RequestingTitle, sm2->RequestingTitle) == 0) {
	if (strcmp(sm1->RespondingTitle, sm2->RespondingTitle) <= 0)
	    return -1;
	else
	    return 1;
    } else if (strcmp(sm1->RequestingTitle, sm2->RequestingTitle) <= 0) {
	return -1;
    } else {
	return 1;
    }
}

void
formatPrintServerCFG(DMAN_PRINTSERVERCFG * p, int index, char *buf)
{
    (void) sprintf(buf, "%-16s     %-16s %-5d", p->RequestingTitle,
		   p->RespondingTitle, p->GQId);
}

int
comparePrintServerCFG(DMAN_PRINTSERVERCFG * p1,
		      DMAN_PRINTSERVERCFG * p2)
{
    return 1;
}

void
formatGroupGroup(DMAN_GROUPNAMES * g, int index, char *buf)
{
    strcpy(buf, g->GroupName);
}

void
formatGroupTitle(DMAN_GROUPNAMES * g, int index, char *buf)
{
    strcpy(buf, g->Title);
}

void
formatFISAccess(DMAN_FISACCESS * f, int index, char *buf)
{
    sprintf(buf, "%-16s %-20s %-16s %-40s", f->Title, f->DbKey,
	    f->GroupName, f->Comment);
}

/* Now the code for Image Server tables */

void
formatStorageAccess(DMAN_STORAGEACCESS * s, int index, char *buf)
{
    char
        temp[40];

    sprintf(buf, "%-16s %-20s ", s->Title, s->DbKey);

    if (s->Flag & DMAN_K_STORAGEACCESS_OWNER) {
	sprintf(temp, "%-16s ", s->Owner);
    } else {
	sprintf(temp, "%-16s ", "");
    }
    strcat(buf, temp);

    if (s->Flag & DMAN_K_STORAGEACCESS_GROUPNAME) {
	sprintf(temp, "%-16s ", s->GroupName);
    } else {
	sprintf(temp, "%-16s ", "");
    }

    strcat(buf, temp);
    if (s->Flag & DMAN_K_STORAGEACCESS_COMMENT) {
	sprintf(temp, "%-30s ", s->Comment);
    } else {
	sprintf(temp, "%-30s ", "");
    }
    strcat(buf, temp);

}

int
compareStorageAccess(DMAN_STORAGEACCESS * s1, DMAN_STORAGEACCESS * s2)
{
    return 1;
}

void
formatStorageControl(DMAN_STORAGECONTROL * s, int index, char *buf)
{
    sprintf(buf, "%-16s %-16s %-80s", s->RequestingTitle, s->RespondingTitle,
	    s->Root);
}

int
compareStorageControl(DMAN_STORAGECONTROL * s1, DMAN_STORAGECONTROL * s2)
{
    return 1;
}
void
formatVideoImage(DMAN_VIDEOIMAGEDEST * c, int index, char *buf)
{
    sprintf(buf, "%-16s %-16s %-16s",
	    c->RequestingTitle,
	    c->RespondingTitle,
	    c->ImageType);
}
