#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"

static void usageerror();
static void processInput();

static char *controlDatabase = "CTNControl";
static DMAN_HANDLE *dmanHandle;

int
main(int argc, char **argv)
{
    CONDITION cond;
    CTNBOOLEAN verboseTBL = FALSE;

    while (--argc > 0 && *(++argv)[0] == '-') {
	switch ((*argv)[1]) {
	case 'f':
	    if (argc < 1)
		usageerror();
	    argc--;
	    argv++;
	    controlDatabase = *argv;
	    break;
	case 'h':
	    usageerror();
	    break;
	case 'v':
	    verboseTBL = TRUE;
	    break;
	case 'x':
	    if (--argc < 1)
		usageerror();
	    argv++;
	    if (strcmp(*argv, "TBL") == 0)
		verboseTBL = TRUE;
	    else
		usageerror();
	    break;
	default:
	    printf("Unrecognized option: %s\n", *argv);
	    break;
	}
    }

    THR_Init();
    (void) TBL_Debug(verboseTBL);

    cond = DMAN_Open(controlDatabase, "", "", &dmanHandle);
    if (cond != DMAN_NORMAL)
	goto abort;

    processInput();

    cond = DMAN_Close(&dmanHandle);
    if (cond != DMAN_NORMAL)
	goto abort;

    THR_Shutdown();
    return 0;

abort:
    COND_DumpConditions();
    THR_Shutdown();
    return 1;
}

static void
usageerror()
{
    char msg[] = "\
Usage: [-f db] \n\
\n\
    -f    Use db as control database instead of default (CTNControl)\n";

    fprintf(stderr, msg);
    exit(1);
}

typedef enum {
    ApplicationEntity,
    GroupNames,
    StorageAccess,
    StorageControl,
    SecurityMatrix,
    FISAccess,
    PrintServerCFG,
    VideoImageDestination
}   TABLE_TYPE;

typedef struct {
    char *tableName;
    TABLE_TYPE tableType;
}   MAP;

static MAP map[] = {
    {"ApplicationEntity", ApplicationEntity},
    {"GroupNames", GroupNames},
    {"StorageAccess", StorageAccess},
    {"StorageControl", StorageControl},
    {"SecurityMatrix", SecurityMatrix},
    {"FISAccess", FISAccess},
    {"PrintServerCFG", PrintServerCFG},
    {"VideoImageDestination", VideoImageDestination}
};

TABLE_TYPE
parseType(char *line)
{
    int i;
    char *token;

    while ((*line == '@') || (*line == '!') || (*line == ' '))
	line++;

    token = strtok(line, " \n");


    for (i = 0; i < (int) DIM_OF(map); i++) {
	if (strncmp(token, map[i].tableName, strlen(token)) == 0)
	    return map[i].tableType;
    }

    fprintf(stderr, "Could not find a match for table name %s\n", token);
    exit(1);
}

static void
copy(char *dst, const char *src)
{
    char *d;

    if (src == NULL) {
	*dst = '\0';
	return;
    }
    while (*src == ' ' || *src == '\t')
	src++;

    d = dst;
    while (*src != '\0')
	*d++ = *src++;

    *d-- = '\0';
    while (d >= dst) {
	if (*d != ' ' && *d != '\t')
	    break;
	*d-- = '\0';
    }
}

static void
processApplicationEntity(char *line)
{
    DMAN_APPLICATIONENTITY ae;
    char *tok;
    CONDITION cond;

    memset(&ae, 0, sizeof(ae));
    ae.Type = DMAN_K_APPLICATIONENTITY;

    tok = strtok(line, "^\n");
    copy(ae.Title, tok);
    ae.Flag |= DMAN_K_APPLICATION_TITLE;

    tok = strtok(NULL, "^\n");
    copy(ae.Node, tok);
    ae.Flag |= DMAN_K_APPLICATION_NODE;

    tok = strtok(NULL, "^\n");
    (void) sscanf(tok, "%d", &ae.Port);
    ae.Flag |= DMAN_K_APPLICATION_PORT;

    tok = strtok(NULL, "^\n");
    copy(ae.Organization, tok);
    ae.Flag |= DMAN_K_APPLICATION_ORGANIZATION;

    tok = strtok(NULL, "^\n");
    copy(ae.Comment, tok);
    ae.Flag |= DMAN_K_APPLICATION_COMMENT;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & ae);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processGroupNames(char *line)
{
    DMAN_GROUPNAMES group;
    char *tok;
    CONDITION cond;

    memset(&group, 0, sizeof(group));
    group.Type = DMAN_K_GROUPNAMES;

    tok = strtok(line, "^\n");
    copy(group.Title, tok);
    group.Flag |= DMAN_K_GROUP_TITLE;

    tok = strtok(NULL, "^\n");
    copy(group.GroupName, tok);
    group.Flag |= DMAN_K_GROUP_GROUP;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & group);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processStorageAccess(char *line)
{
    DMAN_STORAGEACCESS access;
    char *tok;
    CONDITION cond;
    int i = 0;

    memset(&access, 0, sizeof(access));
    access.Type = DMAN_K_STORAGEACCESS;

    tok = strtok(line, "^\n");
    (void) sscanf(tok, "%d", &i);
    access.Access = (S32) i;
    access.Flag |= DMAN_K_STORAGEACCESS_ACCESS;

    tok = strtok(NULL, "^\n");
    copy(access.Title, tok);
    access.Flag |= DMAN_K_STORAGEACCESS_TITLE;

    tok = strtok(NULL, "^\n");
    copy(access.DbKey, tok);
    access.Flag |= DMAN_K_STORAGEACCESS_DBKEY;

    tok = strtok(NULL, "^\n");
    copy(access.Owner, tok);
    access.Flag |= DMAN_K_STORAGEACCESS_OWNER;

    tok = strtok(NULL, "^\n");
    copy(access.GroupName, tok);
    access.Flag |= DMAN_K_STORAGEACCESS_GROUPNAME;

    tok = strtok(NULL, "^\n");
    copy(access.Comment, tok);
    access.Flag |= DMAN_K_STORAGEACCESS_COMMENT;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & access);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processStorageControl(char *line)
{
    DMAN_STORAGECONTROL control;
    char *tok;
    CONDITION cond;

    memset(&control, 0, sizeof(control));
    control.Type = DMAN_K_STORAGECONTROL;

    tok = strtok(line, "^\n");
    copy(control.RequestingTitle, tok);
    control.Flag |= DMAN_K_STORAGECONTROL_REQUESTING;

    tok = strtok(NULL, "^\n");
    copy(control.RespondingTitle, tok);
    control.Flag |= DMAN_K_STORAGECONTROL_RESPONDING;

    tok = strtok(NULL, "^\n");
    copy(control.Medium, tok);
    control.Flag |= DMAN_K_STORAGECONTROL_MEDIUM;

    tok = strtok(NULL, "^\n");
    copy(control.Root, tok);
    control.Flag |= DMAN_K_STORAGECONTROL_ROOT;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & control);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processSecurityMatrix(char *line)
{
    DMAN_SECURITYMATRIX security;
    char *tok;
    CONDITION cond;

    memset(&security, 0, sizeof(security));
    security.Type = DMAN_K_SECURITYMATRIX;

    tok = strtok(line, "^\n");
    copy(security.RequestingTitle, tok);
    security.Flag |= DMAN_K_SECURITY_REQUESTING;

    tok = strtok(NULL, "^\n");
    copy(security.RespondingTitle, tok);
    security.Flag |= DMAN_K_SECURITY_RESPONDING;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & security);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processFISAccess(char *line)
{
    DMAN_FISACCESS access;
    char *tok;
    CONDITION cond;
    int i;

    memset(&access, 0, sizeof(access));
    access.Type = DMAN_K_FISACCESS;

    tok = strtok(line, "^\n");
    (void) sscanf(tok, "%d", &i);
    access.Access = (S32) i;
    access.Flag |= DMAN_K_FISACCESS_ACCESS;

    tok = strtok(NULL, "^\n");
    copy(access.Title, tok);
    access.Flag |= DMAN_K_FISACCESS_TITLE;

    tok = strtok(NULL, "^\n");
    copy(access.DbKey, tok);
    access.Flag |= DMAN_K_FISACCESS_DBKEY;

    tok = strtok(NULL, "^\n");
    copy(access.Owner, tok);
    access.Flag |= DMAN_K_FISACCESS_OWNER;

    tok = strtok(NULL, "^\n");
    copy(access.GroupName, tok);
    access.Flag |= DMAN_K_FISACCESS_GROUPNAME;

    tok = strtok(NULL, "^\n");
    copy(access.Comment, tok);
    access.Flag |= DMAN_K_FISACCESS_COMMENT;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & access);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processPrintServerCFG(char *line)
{
    DMAN_PRINTSERVERCFG print;
    char *tok;
    CONDITION cond;
    int i;

    memset(&print, 0, sizeof(print));
    print.Type = DMAN_K_PRINTSERVERCFG;

    tok = strtok(line, "^\n");
    copy(print.RequestingTitle, tok);
    print.Flag |= DMAN_K_PRINTSERVER_REQUESTING;

    tok = strtok(NULL, "^\n");
    copy(print.RespondingTitle, tok);
    print.Flag |= DMAN_K_PRINTSERVER_RESPONDING;

    tok = strtok(NULL, "^\n");
    (void) sscanf(tok, "%d", &i);
    print.GQId = i;
    print.Flag |= DMAN_K_PRINTSERVER_GQID;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & print);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processVideoImageDestination(char *line)
{
    DMAN_VIDEOIMAGEDEST video;
    char *tok;
    CONDITION cond;

    memset(&video, 0, sizeof(video));
    video.Type = DMAN_K_VIDEOIMAGEDEST;

    tok = strtok(line, "^\n");
    copy(video.RequestingTitle, tok);
    video.Flag |= DMAN_K_VIDEOIMAGE_REQUESTING;

    tok = strtok(NULL, "^\n");
    copy(video.RespondingTitle, tok);
    video.Flag |= DMAN_K_VIDEOIMAGE_RESPONDING;

    tok = strtok(NULL, "^\n");
    copy(video.ImageType, tok);
    video.Flag |= DMAN_K_VIDEOIMAGE_IMAGETYPE;

    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & video);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
processLine(TABLE_TYPE currentType, char *line)
{
    switch (currentType) {
	case ApplicationEntity:
	processApplicationEntity(line);
	break;
    case GroupNames:
	processGroupNames(line);
	break;
    case StorageAccess:
	processStorageAccess(line);
	break;
    case StorageControl:
	processStorageControl(line);
	break;
    case SecurityMatrix:
	processSecurityMatrix(line);
	break;
    case FISAccess:
	processFISAccess(line);
	break;
    case PrintServerCFG:
	processPrintServerCFG(line);
	break;
    case VideoImageDestination:
	processVideoImageDestination(line);
	break;
    }
}

static void
clearApplicationEntity()
{
    DMAN_APPLICATIONENTITY ae;
    CONDITION cond;

    memset(&ae, 0, sizeof(ae));
    ae.Type = DMAN_K_APPLICATIONENTITY;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & ae);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
clearGroupNames()
{
    DMAN_GROUPNAMES group;
    CONDITION cond;

    memset(&group, 0, sizeof(group));
    group.Type = DMAN_K_GROUPNAMES;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & group);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}
static void
clearStorageAccess()
{
    DMAN_STORAGEACCESS access;
    CONDITION cond;

    memset(&access, 0, sizeof(access));
    access.Type = DMAN_K_STORAGEACCESS;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & access);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}
static void
clearStorageControl()
{
    DMAN_STORAGECONTROL control;
    CONDITION cond;

    memset(&control, 0, sizeof(control));
    control.Type = DMAN_K_STORAGECONTROL;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & control);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}
static void
clearSecurityMatrix()
{
    DMAN_SECURITYMATRIX security;
    CONDITION cond;

    memset(&security, 0, sizeof(security));
    security.Type = DMAN_K_SECURITYMATRIX;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & security);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}
static void
clearFISAccess()
{
    DMAN_FISACCESS access;
    CONDITION cond;

    memset(&access, 0, sizeof(access));
    access.Type = DMAN_K_FISACCESS;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & access);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}
static void
clearPrintServerCFG()
{
    DMAN_PRINTSERVERCFG print;
    CONDITION cond;

    memset(&print, 0, sizeof(print));
    print.Type = DMAN_K_PRINTSERVERCFG;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & print);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}
static void
clearVideoImageDestination()
{
    DMAN_VIDEOIMAGEDEST video;
    CONDITION cond;

    memset(&video, 0, sizeof(video));
    video.Type = DMAN_K_VIDEOIMAGEDEST;

    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & video);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
clearTable(TABLE_TYPE type)
{
    switch (type) {
	case ApplicationEntity:
	clearApplicationEntity();
	break;
    case GroupNames:
	clearGroupNames();
	break;
    case StorageAccess:
	clearStorageAccess();
	break;
    case StorageControl:
	clearStorageControl();
	break;
    case SecurityMatrix:
	clearSecurityMatrix();
	break;
    case FISAccess:
	clearFISAccess();
	break;
    case PrintServerCFG:
	clearPrintServerCFG();
	break;
    case VideoImageDestination:
	clearVideoImageDestination();
	break;
    }
}

static void
processInput()
{
    TABLE_TYPE currentType = ApplicationEntity;
    char line[1024];
    char *c;

    while (fgets(line, sizeof(line), stdin) != NULL) {
	fprintf(stdout, "%s", line);
	c = line;
	while (*c == ' ' || *c == '\t')
	    c++;

	if (c[0] == '#')
	    continue;
	else if (c[0] == '\n')
	    continue;
	else if (c[0] == '@')
	    currentType = parseType(c);
	else if (c[0] == '!') {
	    currentType = parseType(c);
	    clearTable(currentType);
	} else
	    processLine(currentType, c);
    }
}
