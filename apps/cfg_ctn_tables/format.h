void formatAE(DMAN_APPLICATIONENTITY * ae, int index, char *buf);
int compareAE(DMAN_APPLICATIONENTITY * ae1, DMAN_APPLICATIONENTITY * ae2);

void formatSecurityMatrix(DMAN_SECURITYMATRIX * s, int index, char *buf);
int compareSecurityMatrix(DMAN_SECURITYMATRIX * sm1, DMAN_SECURITYMATRIX * sm2);

void formatPrintServerCFG(DMAN_PRINTSERVERCFG * p, int index, char *buf);
int comparePrintServerCFG(DMAN_PRINTSERVERCFG * p1, DMAN_PRINTSERVERCFG * p2);

void formatGroupGroup(DMAN_GROUPNAMES * p, int index, char *buf);

void formatGroupTitle(DMAN_GROUPNAMES * p, int index, char *buf);

void formatFISAccess(DMAN_FISACCESS * f, int index, char *buf);
void formatStorageAccess(DMAN_STORAGEACCESS * a, int index, char *buf);
int compareStorageAccess(DMAN_STORAGEACCESS * a1, DMAN_STORAGEACCESS * s2);
void formatStorageControl(DMAN_STORAGECONTROL * c, int index, char *buf);
int compareStorageControl(DMAN_STORAGECONTROL * s1, DMAN_STORAGECONTROL * c2);
void formatVideoImage(DMAN_VIDEOIMAGEDEST * c, int index, char *buf);
