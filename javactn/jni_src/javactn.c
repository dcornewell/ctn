#include <jni.h>
#include "ctn_os.h"

#include "DICOM_DICOMWrapper.h"
#include "DICOM_DDR_DIRInterface.h"
#include "DICOM_InfoObj_Image.h"
#include "DICOM_InfoObj_Waveform.h"
#include "DICOM_Module_ImagePixelModule.h"
#include "DICOM_Module_WaveformModule.h"
#include "DICOM_Misc_Timer.h"
#include "DICOM_Network_Association.h"
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "dicom_ddr.h"
#include "utility.h"
#include "dulprotocol.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_uids.h"

static int verboseFlag = -1;

static void setCodedValues(JNIEnv *env,
			   jobject obj,
			   DCM_OBJECT** dcmObj)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;
  CONDITION cond;

  long flag = 1;
  char codeValue[DICOM_CS_LENGTH+1] = "";
  char codingSchemeDesig[DICOM_SH_LENGTH+1] = "";
  char codingSchemeVersion[DICOM_SH_LENGTH+1] = "";
  char codeMeaning[DICOM_LO_LENGTH+1] = "";

  char mappingResource[DICOM_CS_LENGTH+1] = "";
  char contextGroupVersion[DICOM_DT_LENGTH+1] = "";
  char contextIdentifier[DICOM_CS_LENGTH+1] = "";

  DCM_FLAGGED_ELEMENT flagged[] = {
    {DCM_MAKETAG(0x0008, 0x0105), DCM_CS, "", 1, sizeof(mappingResource), NULL, 1, NULL},
    {DCM_MAKETAG(0x0008, 0x0106), DCM_DT, "", 1, sizeof(contextGroupVersion), NULL, 2, NULL},
    {DCM_MAKETAG(0x0008, 0x010f), DCM_CS, "", 1, sizeof(contextIdentifier), NULL, 4, NULL},
};

  DCM_ELEMENT required[] = {
    {DCM_MAKETAG(0x0008, 0x0100), DCM_SH, "", 1, sizeof(codeValue), NULL},
    {DCM_MAKETAG(0x0008, 0x0102), DCM_SH, "", 1, sizeof(codingSchemeDesig), NULL},
    {DCM_MAKETAG(0x0008, 0x0103), DCM_SH, "", 1, sizeof(codingSchemeVersion), NULL},
    {DCM_MAKETAG(0x0008, 0x0104), DCM_LO, "", 1, sizeof(codeMeaning), NULL}
  };

  flagged[0].e.d.string = mappingResource;
  flagged[0].flagAddress = &flag;
  flagged[1].e.d.string = contextGroupVersion;
  flagged[1].flagAddress = &flag;
  flagged[2].e.d.string = contextIdentifier;
  flagged[2].flagAddress = &flag;

  required[0].d.string = codeValue;
  required[1].d.string = codingSchemeDesig;
  required[2].d.string = codingSchemeVersion;
  required[3].d.string = codeMeaning;

  cond = DCM_ParseObject(dcmObj,
		  required, (int)DIM_OF(required),
		  flagged, (int)DIM_OF(flagged), NULL);
  if (cond != DCM_NORMAL)
    COND_DumpConditions();

  cls = (*env)->GetObjectClass(env, obj);

  fid = (*env)->GetFieldID(env, cls, "CodeValue", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, codeValue);
  (*env)->SetObjectField(env, obj, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "CodingSchemeDesignator", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, codingSchemeDesig);
  (*env)->SetObjectField(env, obj, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "CodingSchemeVersion", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, codingSchemeVersion);
  (*env)->SetObjectField(env, obj, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "CodeMeaning", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, codeMeaning);
  (*env)->SetObjectField(env, obj, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "MappingResource", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, mappingResource);
  (*env)->SetObjectField(env, obj, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "ContextGroupVersion", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, contextGroupVersion);
  (*env)->SetObjectField(env, obj, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "ContextIdentifier", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, contextIdentifier);
  (*env)->SetObjectField(env, obj, fid, jstr);
}

typedef struct {
  DCM_VALUEREPRESENTATION v;
  char* nm;
} VR_MAP;

static void mapVRToString(DCM_VALUEREPRESENTATION vr, char* s)
{
  VR_MAP m[] = {
	{ DCM_AE, "AE" },
	{ DCM_AS, "AS" },
	{ DCM_AT, "AT" },
	{ DCM_CS, "CS" },
	{ DCM_DA, "DA" },
	{ DCM_DD, "DD" },
	{ DCM_DS, "DS" },
	{ DCM_FD, "FD" },
	{ DCM_FL, "FL" },
	{ DCM_IS, "IS" },
	{ DCM_LO, "LO" },
	{ DCM_LT, "LT" },
	{ DCM_OT, "OT" },
	{ DCM_SH, "SH" },
	{ DCM_SL, "SL" },
	{ DCM_SQ, "SQ" },
	{ DCM_SS, "SS" },
	{ DCM_ST, "ST" },
	{ DCM_TM, "TM" },
	{ DCM_UI, "UI" },
	{ DCM_UL, "UL" },
	{ DCM_UN, "UN" },
	{ DCM_US, "US" },
	/*{ DCM_UNKNOWN, "UNK" },*/
	{ DCM_RET, "RET" },
	{ DCM_CTX, "CTX" },
	{ DCM_PN, "PN" },
	{ DCM_OB, "OB" },
	{ DCM_OW, "OW" },
	{ DCM_DT, "DT" },
	{ DCM_DLM, "DLM" },
	{ DCM_UT, "UT" }
  };
  int ix = 0;
  for (ix = 0; ix < (int)DIM_OF(m); ix++) {
    if (vr == m[ix].v) {
      strcpy(s, m[ix].nm);
      return;
    }
  }
  strcpy(s, "No VR");
}

/* Public entry points are below this line. */

static CONDITION
countTagCallback(const DCM_ELEMENT* e, void* ctx)
{
  int *count;

  count = (int *)ctx;
  (*count)++;

  return DCM_NORMAL;
}

typedef struct {
  int index;
  jint *tags;
} TAG_STRUCT;

static CONDITION
fillTagsCallback(const DCM_ELEMENT* e, void* ctx)
{
  TAG_STRUCT *t;

  t = (TAG_STRUCT *)ctx;
  t->tags[t->index] = e->tag;
  t->index++;

  return DCM_NORMAL;
}

JNIEXPORT jlong JNICALL Java_DICOM_DICOMWrapper_newObjectJNI
  (JNIEnv *env, jobject obj)
{
  jlong p = 0;
  CONDITION cond;

  cond = DCM_CreateObject((DCM_OBJECT**)&p, 0);
  if (cond != DCM_NORMAL) {
    COND_PopCondition(TRUE);
    return 0;
  }
  return p;
}

JNIEXPORT jlong JNICALL
Java_DICOM_DICOMWrapper_openFileJNI(JNIEnv *env, jobject obj, jstring st)
{
  jlong p = 0;
  DCM_OBJECT *o;
  CONDITION cond;

  char buf[512];
  int i;
  const char *str = (*env)->GetStringUTFChars(env, st, 0);
  strcpy(buf, str);

  for (i = 0; buf[i] != '\0'; i++) {
    if (buf[i] == '\\')
      buf[i] = '/';
  }
#if 0
  printf("%s\n", str);
#endif

/* We were using this flag for some Toshiba images on the
** NEMA CD:  DCM_ACCEPTVRMISMATCH.  We have turned it off
*/

  cond = DCM_OpenFile(buf,
	DCM_ORDERLITTLEENDIAN | DCM_TRYFILENAMECHANGE | DCM_ACCEPTVRMISMATCH,
			(DCM_OBJECT**)&p);
  if (cond != DCM_NORMAL)
    cond = DCM_OpenFile(buf,
	DCM_PART10FILE | DCM_TRYFILENAMECHANGE | DCM_ACCEPTVRMISMATCH,
			(DCM_OBJECT **)&p);

  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    /*exit(1);*/
  }
  (void)COND_PopCondition(TRUE);

  (*env)->ReleaseStringUTFChars(env, st, str);
  return p;
}

JNIEXPORT void JNICALL Java_DICOM_DICOMWrapper_closeFileJNI
  (JNIEnv *env, jobject obj, jlong dcmObj)
{
  CONDITION cond;

  if (verboseFlag < 0) {
    if (getenv("DCM_VERBOSE") == NULL)
      verboseFlag = 0;
    else
      verboseFlag = 1;
  }
  if (verboseFlag) {
    printf("Closing DICOM object\n");
    DCM_Debug(1);
    printf("Closing DICOM object\n");
  }

  cond = DCM_CloseObject((DCM_OBJECT**)&dcmObj);
  if (cond != DCM_NORMAL)
    COND_DumpConditions();

  (void)COND_PopCondition(TRUE);
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_writeFileJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring fileName)
{
  CONDITION cond;

  char buf[512];
  int i;
  const char *str = (*env)->GetStringUTFChars(env, fileName, 0);
  strcpy(buf, str);

  for (i = 0; buf[i] != '\0'; i++) {
    if (buf[i] == '\\')
      buf[i] = '/';
  }

  (*env)->ReleaseStringUTFChars(env, fileName, str);

  cond = DCM_WriteFile((DCM_OBJECT**)&dcmObj, DCM_ORDERLITTLEENDIAN, buf);
  if (cond != DCM_NORMAL) {
    COND_PopCondition(TRUE);
    return 1;
  }
  return 0;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_getInt
  (JNIEnv *env, jobject obj, jlong dcmObj, jshort group, jshort element)
{
  DCM_ELEMENT e;
  S32 value;
  void *ctx = NULL;
  U32 rtnLength = 0;

  e.tag = DCM_MAKETAG(group, element);
  DCM_LookupElement(&e);
  e.d.ot = (void*)&value;
  switch(e.representation) {
  case DCM_US:
  case DCM_SS:
    e.length = 2;
    break;
  case DCM_UL:
  case DCM_SL:
    e.length = 4;
    break;
  default:
    e.length = 0;
    break;
  }
  DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);
  COND_DumpConditions();
  if (e.length == 2)
    return *e.d.ss;
  else
    return *e.d.sl;
}

JNIEXPORT jfloat JNICALL Java_DICOM_DICOMWrapper_getFloat
  (JNIEnv *env, jobject obj, jlong dcmObj, jshort group, jshort element)
{
  DCM_ELEMENT e;
  char s[256] = "0";
  U32 rtnLength = 0;
  float f;

  e.tag = DCM_MAKETAG(group, element);
  DCM_LookupElement(&e);
  switch (e.representation) {
  case DCM_IS:
  case DCM_DS:
    e.length = sizeof(s);
    e.d.string = s;
    break;
  default:
    fprintf(stderr, "Cannot get decimal value for: %04x %04x %s\n",
	    group, element, e.description);
    exit(1);
  }
  DCM_ParseObject((DCM_OBJECT**)&dcmObj, &e, 1, NULL, 0, NULL);
  sscanf(s, "%f", &f);
  return f;
}

JNIEXPORT jstring JNICALL Java_DICOM_DICOMWrapper_getStringJNI__JSS
  (JNIEnv *env, jobject obj, jlong dcmObj, jshort group, jshort element)
{
  DCM_ELEMENT e;
  char s[128] = "<none>";
  char *sPtr;
  U32 length = 0;
  jstring rtnString;
  CONDITION cond;

  e.tag = DCM_MAKETAG(group, element);

#if 0
  DCM_LookupElement(&e);
  cond = DCM_GetElementSize((DCM_OBJECT**)&dcmObj, e.tag, &length);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    length = 2;
  }
  if (length >= sizeof(s)) {
    sPtr = (char *)malloc(length + 1);
    e.length = length + 1;
  } else {
    sPtr = s;
    e.length = sizeof(s);
  }
  e.d.string = sPtr;
  cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, &e, 1, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
     sPtr[0] = '\0';
  }
 #endif

  sPtr = DCM_GetString((DCM_OBJECT**)&dcmObj, e.tag);
  if (sPtr == 0) {
    sPtr = s;
  }
  rtnString = (*env)->NewStringUTF(env, sPtr);
  if (sPtr != s)
    free(sPtr);

  return rtnString;
}

JNIEXPORT jstring JNICALL Java_DICOM_DICOMWrapper_getStringJNI__JI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint tag)
{
  DCM_ELEMENT e;
  char s[128] = "<none>";
  char *sPtr;
  jstring rtnString;
  CONDITION cond;

  e.tag = tag;

#if 0
  memset(&e, 0, sizeof(e));
  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    e.length = 2;
  }
  if (e.representation == DCM_SQ) {
    sPtr = s;
    strcpy(s, "<Sequence>");
  } else {
    if (e.length >= sizeof(s)) {
      sPtr = (char *)malloc(e.length + 1);
      e.length = e.length + 1;
    } else {
      sPtr = s;
      e.length = sizeof(s);
    }
    e.d.string = sPtr;
    cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
      (void)COND_PopCondition(TRUE);
       sPtr[0] = '\0';
    }
  }
#endif

  sPtr = DCM_GetString((DCM_OBJECT**)&dcmObj, e.tag);
  if (sPtr == 0) {
    sPtr = s;
  }

  rtnString = (*env)->NewStringUTF(env, sPtr);
  if (sPtr != s)
    free(sPtr);

  return rtnString;
}

JNIEXPORT jstring JNICALL Java_DICOM_DICOMWrapper_getStringJNI__JIII
  (JNIEnv *env, jobject obj, jlong dcmObj, jint seqTag, jint tag, jint item)
{
  DCM_ELEMENT e;
  char s[128];
  char *sPtr;
  U32 length = 0;
  jstring rtnString;
  CONDITION cond;
  LST_HEAD* l = 0;
  DCM_SEQUENCE_ITEM* seqItem;

  cond = DCM_GetSequenceList((DCM_OBJECT**)&dcmObj, seqTag, &l);
  if (cond != DCM_NORMAL) {
    rtnString = (*env)->NewStringUTF(env, "");
    return rtnString;
  }

  seqItem = (DCM_SEQUENCE_ITEM*)LST_Head(&l);
  (void)LST_Position(&l, seqItem);
  while(item-- > 1) {
    seqItem = (DCM_SEQUENCE_ITEM*)LST_Next(&l);
  }

  if (seqItem == NULL) {
    rtnString = (*env)->NewStringUTF(env, "");
    return rtnString;
  }

  e.tag = tag;

  DCM_LookupElement(&e);
  cond = DCM_GetElementSize(&seqItem->object, e.tag, &length);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    length = 2;
  }
  if (length >= sizeof(s)) {
    sPtr = (char *)malloc(length + 1);
    e.length = length + 1;
  } else {
    sPtr = s;
    e.length = sizeof(s);
  }
  e.d.string = sPtr;
  cond = DCM_ParseObject(&seqItem->object, &e, 1, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
     sPtr[0] = '\0';
  }

  rtnString = (*env)->NewStringUTF(env, sPtr);
  if (sPtr != s)
    free(sPtr);

  return rtnString;
}


JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_getSequenceLengthJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jshort group, jshort element)
{
  CONDITION cond;
  DCM_TAG t;
  LST_HEAD* l = 0;
  unsigned length;

  t = DCM_MAKETAG(group, element);
  cond = DCM_GetSequenceList((DCM_OBJECT**)&dcmObj, t, &l);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    return -1;
  }
  length = LST_Count(&l);
  return (jint)length;
}

JNIEXPORT jlong JNICALL Java_DICOM_DICOMWrapper_getSequenceObjJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jshort group, jshort element,
   jint index)
{
  CONDITION cond;
  DCM_TAG t;
  LST_HEAD* l = 0;
  DCM_SEQUENCE_ITEM* item;
  jlong rtnValue = 0;
  DCM_OBJECT** dummyPtr;

  t = DCM_MAKETAG(group, element);
  cond = DCM_GetSequenceList((DCM_OBJECT**)&dcmObj, t, &l);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    return -1;
  }

  item = (DCM_SEQUENCE_ITEM*)LST_Head(&l);
  LST_Position(&l, item);

  while (index-- > 0) {
    item = (DCM_SEQUENCE_ITEM*)LST_Next(&l);
  }

  dummyPtr = (DCM_OBJECT**)&rtnValue;
  *dummyPtr = item->object;
  return rtnValue;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_attributePresentJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint tag)
{
  CONDITION cond;
  DCM_ELEMENT e;

  memset(&e, 0, sizeof(e));

  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond == DCM_NORMAL) {
    return 1;
  } else {
    (void)COND_PopCondition(TRUE);
    return 0;
  }
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_countTagsJNI
  (JNIEnv *env, jobject obj, jlong dcmObj)
{
  CONDITION cond;
  int count = 0;

  cond = DCM_ScanParseObject((DCM_OBJECT**)&dcmObj, 0, 0, 0, 0,
			     countTagCallback, &count);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    count = -1;
  }

  return count;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_fillTagsJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jintArray arr)
{
  jint *tags = 0;
  CONDITION cond;
  TAG_STRUCT t;
  jint rtnValue = 0;

  jsize len;
  len = (*env)->GetArrayLength(env, arr);
  tags = (*env)->GetIntArrayElements(env, arr, 0);

  t.index = 0;
  t.tags = tags;
  cond = DCM_ScanParseObject((DCM_OBJECT**)&dcmObj, 0, 0, 0, 0,
			     fillTagsCallback, &t);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    rtnValue = -1;
  }

  (*env)->ReleaseIntArrayElements(env, arr, tags, 0);
  return rtnValue;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_setStringJNI
  (JNIEnv *env, jobject obj, jlong dcmObject, jint tag, jstring s)
{
  CONDITION cond;
  DCM_ELEMENT e;

  const char *str = (*env)->GetStringUTFChars(env, s, 0);

  memset(&e, 0, sizeof(e));
  e.tag = tag;
  DCM_LookupElement(&e);
  e.d.string = str;
  e.length = strlen(str);

  cond = DCM_ModifyElements((DCM_OBJECT**)&dcmObject, &e, 1, 0, 0, 0);

  (*env)->ReleaseStringUTFChars(env, s, str);

  if (cond != DCM_NORMAL) {
    COND_PopCondition(TRUE);
    return 1;
  }
  return 0;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_setStringSeqJNI
  (JNIEnv *env, jobject obj, jlong dcmObject, jint seqTag, jint tag, jstring s,
   jint index)

{
  LST_HEAD* l1 = 0;
  CONDITION cond;
  DCM_SEQUENCE_ITEM* item1 = 0;

  cond = DCM_GetSequenceList((DCM_OBJECT**)&dcmObject, seqTag, &l1);
  if (cond != DCM_NORMAL) {
    DCM_ELEMENT e = {seqTag, DCM_SQ, "", 1, 0, 0 };
    DCM_SEQUENCE_ITEM* item = 0;

    (void)COND_PopCondition(TRUE);

    l1 = LST_Create();
    e.d.sq = l1;
    item = (DCM_SEQUENCE_ITEM*)malloc(sizeof(*item));
    DCM_CreateObject(&(item->object), 0);
    LST_Enqueue(&l1, item);
    cond = DCM_AddSequenceElement((DCM_OBJECT**)&dcmObject, &e);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      exit(1);
    }
  }

  if (index != 0) {
    item1 = (DCM_SEQUENCE_ITEM*) LST_Head(&l1);
    (void) LST_Position(&l1, item1);

    while(index-- > 1) {
      item1 = (DCM_SEQUENCE_ITEM*) LST_Next(&l1);
    }
  }

  if (item1 == 0) {
    item1 = (DCM_SEQUENCE_ITEM*)malloc(sizeof(*item1));
    DCM_CreateObject(&(item1->object), 0);
    LST_Enqueue(&l1, item1);
  }

  {
    DCM_ELEMENT e1;
    const char *str = (*env)->GetStringUTFChars(env, s, 0);

    memset(&e1, 0, sizeof(e1));
    e1.tag = tag;
    DCM_LookupElement(&e1);
    e1.d.string = str;
    e1.length = strlen(str);

    cond = DCM_ModifyElements(&item1->object, &e1, 1, 0, 0, 0);

    (*env)->ReleaseStringUTFChars(env, s, str);
  }

  return 0;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_addSequenceJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint seqTag)
{
  DCM_ELEMENT e;
  CONDITION cond;
  memset(&e, 0, sizeof(e));

  e.tag = seqTag;
  e.representation = DCM_SQ;
  e.d.sq = LST_Create();

  cond = DCM_AddSequenceElement((DCM_OBJECT**)&dcmObj, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    COND_PopCondition(TRUE);
    return -1;
  }
  return 0;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_addToSequenceJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint seqTag, jint index,
   jlong newObj)
{
  LST_HEAD* l = 0;
  CONDITION cond;
  DCM_OBJECT* copyObject = 0;
  DCM_SEQUENCE_ITEM* item;

  cond = DCM_GetSequenceList((DCM_OBJECT**)&dcmObj, seqTag, &l);
  if (cond != DCM_NORMAL)
    return -1;

  cond = DCM_CopyObject((DCM_OBJECT**)&newObj, &copyObject);
  if (cond != DCM_NORMAL)
    return -1;

  item = CTN_MALLOC(sizeof(*item));
  if (item == 0)
    return -1;

  item->object = copyObject;
  LST_Enqueue(&l, item);
  return 0;
}

JNIEXPORT jstring JNICALL Java_DICOM_DICOMWrapper_getVRJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint tag)
{
  DCM_ELEMENT e;
  char s[128] = "XX";
  jstring rtnString;
  CONDITION cond;

  e.tag = tag;

  memset(&e, 0, sizeof(e));
  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
  } else {
    mapVRToString(e.representation, s);
  }
  
  rtnString = (*env)->NewStringUTF(env, s);

  return rtnString;
}

JNIEXPORT jstring JNICALL Java_DICOM_DICOMWrapper_getDescrJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint tag)
{
  DCM_ELEMENT e;
  jstring rtnString;
  CONDITION cond;

  e.tag = tag;

  memset(&e, 0, sizeof(e));
  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    strcpy(e.description, "No description");
  }
  
  rtnString = (*env)->NewStringUTF(env, e.description);

  return rtnString;
}

JNIEXPORT jint JNICALL Java_DICOM_DICOMWrapper_getLengthJNI
  (JNIEnv *env, jobject obj, jlong dcmObj, jint tag)
{
  DCM_ELEMENT e;
  CONDITION cond;

  e.tag = tag;

  memset(&e, 0, sizeof(e));
  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
    return 0;
  }
  
  return (int)e.length;

}

/* Code for DICOM DIR Interface */


JNIEXPORT jint JNICALL Java_DICOM_DDR_DIRInterface_getPatientCount
  (JNIEnv *env, jobject obj, jlong dcmObj)
{
  jint patientCount;
  LST_HEAD *pList;
  DDR_PATIENT *p;

  pList = LST_Create();
  DDR_GetPatientList((DCM_OBJECT**)&dcmObj, &pList);
  patientCount = LST_Count(&pList);
  while ((p = LST_Dequeue(&pList)) != NULL)
    free(p);

  return patientCount;
}

JNIEXPORT void JNICALL Java_DICOM_DDR_DIRInterface_setPatientAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jint index, jobject patient)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;

  DDR_PATIENT *p;
  LST_HEAD *lst;

  lst = LST_Create();
  DDR_GetPatientList((DCM_OBJECT **)&dcmObj, &lst);
  p = LST_Dequeue(&lst);
  while (index-- > 0) {
    free(p);
    p = LST_Dequeue(&lst);
  }
  if (p == NULL)
    return;

  cls = (*env)->GetObjectClass(env, patient);

  fid = (*env)->GetFieldID(env, cls, "PatientName", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, p->PatientName);
  (*env)->SetObjectField(env, patient, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "PatientID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, p->PatientID);
  (*env)->SetObjectField(env, patient, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "BirthDate", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, p->BirthDate);
  (*env)->SetObjectField(env, patient, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "Sex", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, p->Sex);
  (*env)->SetObjectField(env, patient, fid, jstr);

  while (p != NULL) {
    free (p);
    p = LST_Dequeue(&lst);
  }
}


JNIEXPORT jint JNICALL Java_DICOM_DDR_DIRInterface_getStudyCount
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring patientID)
{
  jint studyCount;
  LST_HEAD *sList;
  DDR_STUDY *s;

  const char *str = (*env)->GetStringUTFChars(env, patientID, 0);

  sList = LST_Create();
  DDR_GetStudyList((DCM_OBJECT**)&dcmObj, str, &sList);

  (*env)->ReleaseStringUTFChars(env, patientID, str);

  studyCount = LST_Count(&sList);
  while ((s = LST_Dequeue(&sList)) != NULL)
    free(s);

  return studyCount;
}


JNIEXPORT void JNICALL Java_DICOM_DDR_DIRInterface_setStudyAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring patientID,
   jint index, jobject study)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;

  DDR_STUDY *s;
  LST_HEAD *lst;

  const char *str = (*env)->GetStringUTFChars(env, patientID, 0);

  lst = LST_Create();
  DDR_GetStudyList((DCM_OBJECT **)&dcmObj, str, &lst);
  (*env)->ReleaseStringUTFChars(env, patientID, str);

  s = LST_Dequeue(&lst);
  while (index-- > 0) {
    free (s);
    s = LST_Dequeue(&lst);
  }
  if (s == NULL)
    return;

  cls = (*env)->GetObjectClass(env, study);

  fid = (*env)->GetFieldID(env, cls, "StudyDate", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->StudyDate);
  (*env)->SetObjectField(env, study, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "StudyTime", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->StudyTime);
  (*env)->SetObjectField(env, study, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "StudyDescription", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->StudyDescription);
  (*env)->SetObjectField(env, study, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "StudyInstanceUID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->StudyInstanceUID);
  (*env)->SetObjectField(env, study, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "StudyID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->StudyID);
  (*env)->SetObjectField(env, study, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "AccessionNumber", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->AccessionNumber);
  (*env)->SetObjectField(env, study, fid, jstr);

  while (s != NULL) {
    free (s);
    s = LST_Dequeue(&lst);
  }
}

JNIEXPORT jint JNICALL Java_DICOM_DDR_DIRInterface_getSeriesCount
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring patientID,
   jstring studyInstanceUID)
{
  jint seriesCount;
  LST_HEAD *sList;
  DDR_SERIES *s;

  const char *strPatient = (*env)->GetStringUTFChars(env, patientID, 0);
  const char *strStudy = (*env)->GetStringUTFChars(env, studyInstanceUID, 0);

  sList = LST_Create();
  DDR_GetSeriesList((DCM_OBJECT**)&dcmObj, strPatient, strStudy, &sList);

  (*env)->ReleaseStringUTFChars(env, patientID, strPatient);
  (*env)->ReleaseStringUTFChars(env, studyInstanceUID, strStudy);

  seriesCount = LST_Count(&sList);
  while ((s = LST_Dequeue(&sList)) != NULL)
    free(s);

  return seriesCount;
}

JNIEXPORT void JNICALL Java_DICOM_DDR_DIRInterface_setSeriesAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring patientID,
   jstring studyInstanceUID, jint index, jobject series)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;

  DDR_SERIES *s;
  LST_HEAD *lst;

  const char *strPatient = (*env)->GetStringUTFChars(env, patientID, 0);
  const char *strStudy = (*env)->GetStringUTFChars(env, studyInstanceUID, 0);

  lst = LST_Create();
  DDR_GetSeriesList((DCM_OBJECT **)&dcmObj, strPatient, strStudy, &lst);
  (*env)->ReleaseStringUTFChars(env, patientID, strPatient);
  (*env)->ReleaseStringUTFChars(env, studyInstanceUID, strStudy);

  s = LST_Dequeue(&lst);
  while (index-- > 0) {
    free (s);
    s = LST_Dequeue(&lst);
  }
  if (s == NULL)
    return;

  cls = (*env)->GetObjectClass(env, series);

  fid = (*env)->GetFieldID(env, cls, "Modality", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->Modality);
  (*env)->SetObjectField(env, series, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "SeriesInstanceUID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->SeriesInstanceUID);
  (*env)->SetObjectField(env, series, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "SeriesNumber", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, s->SeriesNumber);
  (*env)->SetObjectField(env, series, fid, jstr);

  while (s != NULL) {
    free (s);
    s = LST_Dequeue(&lst);
  }
}

JNIEXPORT jint JNICALL Java_DICOM_DDR_DIRInterface_getLeafCount
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring patientID,
   jstring studyInstanceUID, jstring seriesInstanceUID)
{
  jint leafCount;
  LST_HEAD *lList;
  DDR_SERIES_LEAF *l;

  const char *strPatient = (*env)->GetStringUTFChars(env, patientID, 0);
  const char *strStudy = (*env)->GetStringUTFChars(env, studyInstanceUID, 0);
  const char *strSeries = (*env)->GetStringUTFChars(env, seriesInstanceUID, 0);

  lList = LST_Create();
  DDR_GetSeriesLeafList((DCM_OBJECT**)&dcmObj, strPatient, strStudy, strSeries,
			&lList);

  (*env)->ReleaseStringUTFChars(env, patientID, strPatient);
  (*env)->ReleaseStringUTFChars(env, studyInstanceUID, strStudy);
  (*env)->ReleaseStringUTFChars(env, seriesInstanceUID, strSeries);

  leafCount = LST_Count(&lList);
  while ((l = LST_Dequeue(&lList)) != NULL)
    free(l);

  return leafCount;
}

JNIEXPORT void JNICALL Java_DICOM_DDR_DIRInterface_setLeafAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jstring patientID,
   jstring studyInstanceUID, jstring seriesInstanceUID,
   jint index, jobject leaf)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;

  DDR_SERIES_LEAF *l;
  LST_HEAD *lst;

  const char *strPatient = (*env)->GetStringUTFChars(env, patientID, 0);
  const char *strStudy = (*env)->GetStringUTFChars(env, studyInstanceUID, 0);
  const char *strSeries = (*env)->GetStringUTFChars(env, seriesInstanceUID, 0);

  lst = LST_Create();
  DDR_GetSeriesLeafList((DCM_OBJECT **)&dcmObj,
			strPatient, strStudy, strSeries,
			&lst);
  (*env)->ReleaseStringUTFChars(env, patientID, strPatient);
  (*env)->ReleaseStringUTFChars(env, studyInstanceUID, strStudy);
  (*env)->ReleaseStringUTFChars(env, seriesInstanceUID, strSeries);

  l = LST_Dequeue(&lst);
  while (index-- > 0) {
    free (l);
    l = LST_Dequeue(&lst);
  }
  if (l == NULL)
    return;

  cls = (*env)->GetObjectClass(env, leaf);

  fid = (*env)->GetFieldID(env, cls, "RecordType", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, l->RecordType);
  (*env)->SetObjectField(env, leaf, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "FileID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, l->FileID);
  (*env)->SetObjectField(env, leaf, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "SOPClassUID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, l->SOPClassUID);
  (*env)->SetObjectField(env, leaf, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "SOPInstanceUID", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, l->SOPInstanceUID);
  (*env)->SetObjectField(env, leaf, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "TransferSyntax", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, l->TransferSyntaxUID);
  (*env)->SetObjectField(env, leaf, fid, jstr);


  while (l != NULL) {
    free (l);
    l = LST_Dequeue(&lst);
  }
}

/* Code for DICOM Image Interface */


JNIEXPORT void JNICALL Java_DICOM_InfoObj_Image_setPatientAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jobject patient)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;
  char patientName[DICOM_PN_LENGTH+1] = "";
  char patientID[DICOM_LO_LENGTH+1] = "";
  char birthDate[DICOM_DA_LENGTH+1] = "";
  char sex[DICOM_CS_LENGTH + 1] = "";
  long flag = 1;

  DCM_FLAGGED_ELEMENT attributes[] = {
    {DCM_PATNAME, DCM_PN, "", 1, sizeof(patientName), NULL, 1, NULL},
    {DCM_PATID, DCM_LO, "", 1, sizeof(patientID), NULL, 2, NULL},
    {DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(birthDate), NULL, 4, NULL},
    {DCM_PATSEX, DCM_CS, "", 1, sizeof(sex), NULL, 8, NULL}
  };

  attributes[0].e.d.string = patientName;
  attributes[0].flagAddress = &flag;
  attributes[1].e.d.string = patientID;
  attributes[1].flagAddress = &flag;
  attributes[2].e.d.string = birthDate;
  attributes[2].flagAddress = &flag;
  attributes[3].e.d.string = sex;
  attributes[3].flagAddress = &flag;

  DCM_ParseObject((DCM_OBJECT **)&dcmObj, NULL, 0, attributes,
		  (int)DIM_OF(attributes), NULL);

  cls = (*env)->GetObjectClass(env, patient);

  if (flag & 1) {
    fid = (*env)->GetFieldID(env, cls, "PatientName", "Ljava/lang/String;");
    if (fid == 0)
      return;
    jstr = (*env)->NewStringUTF(env, patientName);
    (*env)->SetObjectField(env, patient, fid, jstr);
  }

  if (flag & 2) {
    fid = (*env)->GetFieldID(env, cls, "PatientID", "Ljava/lang/String;");
    if (fid == 0)
      return;
    jstr = (*env)->NewStringUTF(env, patientID);
    (*env)->SetObjectField(env, patient, fid, jstr);
  }

  if (!(flag & 4))
    strcpy(birthDate, "");

  fid = (*env)->GetFieldID(env, cls, "BirthDate", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, birthDate);
  (*env)->SetObjectField(env, patient, fid, jstr);


  if (flag & 8) {
    fid = (*env)->GetFieldID(env, cls, "Sex", "Ljava/lang/String;");
    if (fid == 0)
      return;
    jstr = (*env)->NewStringUTF(env, sex);
    (*env)->SetObjectField(env, patient, fid, jstr);
  }
}


JNIEXPORT void JNICALL Java_DICOM_InfoObj_Image_setImgAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jobject img)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;
  char windowCenter[DICOM_DS_LENGTH+1] = "0";
  char windowWidth[DICOM_DS_LENGTH+1] = "0";
  char rescaleSlope[DICOM_DS_LENGTH+1] = "1";
  char rescaleIntercept[DICOM_DS_LENGTH + 1] = "0";
  char numberOfFrames[DICOM_IS_LENGTH + 1] = "1";
  long flag = 1;
  U16 samplesPerPixel;
  U16 rows, columns, bitsAllocated, bitsStored, highBit, pixelRepresentation;
  char photometricInterpretation[DICOM_CS_LENGTH+1] = "";
  int frameCount;

  DCM_FLAGGED_ELEMENT flagged[] = {
    {DCM_IMGWINDOWCENTER, DCM_DS, "", 1, sizeof(windowCenter), NULL, 1, NULL},
    {DCM_IMGWINDOWWIDTH, DCM_DS, "", 1, sizeof(windowWidth), NULL, 2, NULL},
    {DCM_IMGRESCALESLOPE, DCM_DS, "", 1, sizeof(rescaleSlope), NULL, 4, NULL},
    {DCM_IMGRESCALEINTERCEPT, DCM_DS, "", 1, sizeof(rescaleIntercept), NULL, 8, NULL},
    {DCM_IMGNUMBEROFFRAMES, DCM_IS, "", 1, sizeof(numberOfFrames), NULL, 0x10, NULL}
  };
  DCM_ELEMENT required[] = {
    {DCM_IMGROWS, DCM_US, "", 1, sizeof(rows), NULL},
    {DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(columns), NULL},
    {DCM_IMGBITSALLOCATED, DCM_US, "", 1, sizeof(bitsAllocated), NULL},
    {DCM_IMGBITSSTORED, DCM_US, "", 1, sizeof(bitsStored), NULL},
    {DCM_IMGHIGHBIT, DCM_US, "", 1, sizeof(highBit), NULL},
    {DCM_IMGPIXELREPRESENTATION, DCM_US, "", 1, sizeof(pixelRepresentation), NULL},
    {DCM_IMGSAMPLESPERPIXEL, DCM_US, "", 1, sizeof(samplesPerPixel), NULL},
    {DCM_IMGPHOTOMETRICINTERP, DCM_CS, "", 1, sizeof(photometricInterpretation), NULL}
  };

  float f = 0.;

  flagged[0].e.d.string = windowCenter;
  flagged[0].flagAddress = &flag;
  flagged[1].e.d.string = windowWidth;
  flagged[1].flagAddress = &flag;
  flagged[2].e.d.string = rescaleSlope;
  flagged[2].flagAddress = &flag;
  flagged[3].e.d.string = rescaleIntercept;
  flagged[3].flagAddress = &flag;
  flagged[4].e.d.string = numberOfFrames;
  flagged[4].flagAddress = &flag;

  required[0].d.us = &rows;
  required[1].d.us = &columns;
  required[2].d.us = &bitsAllocated;
  required[3].d.us = &bitsStored;
  required[4].d.us = &highBit;
  required[5].d.us = &pixelRepresentation;
  required[6].d.us = &samplesPerPixel;
  required[7].d.string = photometricInterpretation;

  DCM_ParseObject((DCM_OBJECT **)&dcmObj,
		  required, (int)DIM_OF(required),
		  flagged, (int)DIM_OF(flagged), NULL);

  cls = (*env)->GetObjectClass(env, img);

  frameCount = atoi(numberOfFrames);
  if (frameCount <= 0)
    frameCount = 1;


  if (getenv("CTN_REDUCED") != NULL) {
    if ((rows > 512) && (columns > 512) && (frameCount > 1)) {
      rows /= 2;
      columns /= 2;
    }
  }
  if (getenv("CTN_EXPANDED") != NULL) {
    if ((rows < 128) && (columns < 128)) {
      rows *= 4;
      columns *= 4;
    }
  }

  fid = (*env)->GetFieldID(env, cls, "DCMObject", "J");
  if (fid == 0)
    return;
  (*env)->SetLongField(env, img, fid, dcmObj);

  fid = (*env)->GetFieldID(env, cls, "PhotometricInterpretation",
			   "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, photometricInterpretation);
  (*env)->SetObjectField(env, img, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "SamplesPerPixel", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, samplesPerPixel);

  fid = (*env)->GetFieldID(env, cls, "Rows", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, rows);

  fid = (*env)->GetFieldID(env, cls, "Columns", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, columns);

  fid = (*env)->GetFieldID(env, cls, "BitsAllocated", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, bitsAllocated);

  fid = (*env)->GetFieldID(env, cls, "BitsStored", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, bitsStored);

  fid = (*env)->GetFieldID(env, cls, "HighBit", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, highBit);

  fid = (*env)->GetFieldID(env, cls, "PixelRepresentation", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, pixelRepresentation);


  if (flag & 1) {
    fid = (*env)->GetFieldID(env, cls, "WindowCenter", "F");
    if (fid == 0)
      return;
    sscanf(windowCenter, "%f", &f);
    (*env)->SetFloatField(env, img, fid, f);
  }

  if (flag & 2) {
    fid = (*env)->GetFieldID(env, cls, "WindowWidth", "F");
    if (fid == 0)
      return;
    sscanf(windowWidth, "%f", &f);
    (*env)->SetFloatField(env, img, fid, f);
  }

  if (flag & 4) {
    fid = (*env)->GetFieldID(env, cls, "RescaleSlope", "F");
    if (fid == 0)
      return;
    sscanf(rescaleSlope, "%f", &f);
    (*env)->SetFloatField(env, img, fid, f);
  }

  if (flag & 8) {
    fid = (*env)->GetFieldID(env, cls, "RescaleIntercept", "F");
    if (fid == 0)
      return;
    sscanf(rescaleIntercept, "%f", &f);
    (*env)->SetFloatField(env, img, fid, f);
  }

#if 0
  if (frameCount > 30)
    frameCount = 30;
#endif

  fid = (*env)->GetFieldID(env, cls, "FrameCount", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, img, fid, frameCount);

}

/* The following code supports the Image Pixel Module */

JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_getPixelsByte
  (JNIEnv *env, jobject obj, jlong dcmObj, jbyteArray arr,
   jint rows, jint cols, jint samplesPerPixel)
{
  jbyte *pixels = 0;
  DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OB, "", 1, 0, NULL};
  void *ctx = NULL;
  U32 rtnLength = 0;

  jsize len;
  len = (*env)->GetArrayLength(env, arr);
  pixels = (*env)->GetByteArrayElements(env, arr, 0);

  e.length = len;
  if (len != rows*cols*samplesPerPixel) {
    fprintf(stderr, "Len = %d, rows = %d, cols = %d, samples/pixel = %d\n",
	    len, rows, cols, samplesPerPixel);
    exit(1);
  }
  e.d.ot = pixels;
  DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);

  (*env)->ReleaseByteArrayElements(env, arr, pixels, 0);
}


JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_getPixelsShort
  (JNIEnv *env, jobject obj, jlong dcmObj, jshortArray arr,
   jint rows, jint cols, jint samplesPerPixel)
{
  jshort *pixels = 0;
  DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OB, "", 1, 0, NULL};
  void *ctx = NULL;
  U32 rtnLength = 0;

  jsize len;
  len = (*env)->GetArrayLength(env, arr);
  pixels = (*env)->GetShortArrayElements(env, arr, 0);

  e.length = len * 2;
  if (len != rows*cols*samplesPerPixel) {
    fprintf(stderr, "Len = %d, rows = %d, cols = %d, samples/pixel = %d\n",
	    len, rows, cols, samplesPerPixel);
    exit(1);
  }
  e.d.ot = pixels;
  DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);

  (*env)->ReleaseShortArrayElements(env, arr, pixels, 0);
}

JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_remapPixelsByte
  (JNIEnv *env, jobject obj, jlong dcmObj, jbyteArray arr,
   jint rows, jint cols, jint samplesPerPixel, jintArray arrayOut)
{
  jbyte *inPixels = 0;
  jint *outPixels;
  int index;
  unsigned char *c;
  U32 *u;

  jsize len;
  len = (*env)->GetArrayLength(env, arr);

  inPixels = (*env)->GetByteArrayElements(env, arr, 0);
  outPixels = (*env)->GetIntArrayElements(env, arrayOut, 0);

  c = (unsigned char*)inPixels;
  u = (U32*)outPixels;

  for (index = 0; index < rows * cols; index++)
    u[index] = c[index] << 16 | c[index] << 8 | c[index];

  (*env)->ReleaseByteArrayElements(env, arr, inPixels, 0);
  (*env)->ReleaseIntArrayElements(env, arr, outPixels, 0);
}

JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_getPixelFrameByte
  (JNIEnv *env, jobject obj, jlong dcmObj, jshortArray arr,
   jint rows, jint cols, jint samplesPerPixel, jint frame)
{
  jbyte *pixels = 0;
  DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OB, "", 1, 0, NULL};
  unsigned long offset;
  CONDITION cond;
  jsize len;

  len = (*env)->GetArrayLength(env, arr);
  pixels = (*env)->GetByteArrayElements(env, arr, 0);

  e.length = len;
  if (len != rows*cols*samplesPerPixel) {
    fprintf(stderr, "Len = %d, rows = %d, cols = %d, samplesPerPixel = %d\n",
	    len, rows, cols, samplesPerPixel);
    exit(1);  /* repair */
  }
  e.d.ot = pixels;
  offset = (frame - 1) * rows * cols * samplesPerPixel;
  cond = DCM_GetElementValueOffset((DCM_OBJECT**)&dcmObj, &e, offset);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);   /* repair */
  }

  (*env)->ReleaseByteArrayElements(env, arr, pixels, 0);
}


JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_getPixelFrameShort
  (JNIEnv *env, jobject obj, jlong dcmObj, jshortArray arr,
   jint rows, jint cols, jint samplesPerPixel, jint frame)
{
  jshort *pixels = 0;
  DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OB, "", 1, 0, NULL};
  unsigned long offset;
  CONDITION cond;
  jsize len;

  len = (*env)->GetArrayLength(env, arr);
  pixels = (*env)->GetShortArrayElements(env, arr, 0);

  e.length = len * 2;
  if (len != rows*cols*samplesPerPixel) {
    fprintf(stderr, "Len = %d, rows = %d, cols = %d, samples/pixel = %d\n",
	    len, rows, cols, samplesPerPixel);
    exit(1);	/* repair */
  }
  e.d.ot = pixels;

  offset = (frame - 1) * rows * cols * samplesPerPixel * 2;
  cond = DCM_GetElementValueOffset((DCM_OBJECT**)&dcmObj, &e, offset);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);   /* repair */
  }

  (*env)->ReleaseShortArrayElements(env, arr, pixels, 0);
}


static jint getIntParameter(JNIEnv *env, jobject obj, const char* fieldName)
{
  jfieldID fid;
  jclass cls;
  jint rtnValue;

  cls = (*env)->GetObjectClass(env, obj);

  fid = (*env)->GetFieldID(env, cls, fieldName, "I");
  if (fid == 0)
    return;

  rtnValue = (*env)->GetIntField(env, obj, fid);

  return rtnValue;
}

static jint getFloatParameter(JNIEnv *env, jobject obj, const char* fieldName)
{
  jfieldID fid;
  jclass cls;
  jfloat rtnValue;

  cls = (*env)->GetObjectClass(env, obj);

  fid = (*env)->GetFieldID(env, cls, fieldName, "F");
  if (fid == 0)
    return;

  rtnValue = (*env)->GetFloatField(env, obj, fid);

  return rtnValue;
}

typedef struct {
  jint rows;
  jint cols;
  jint bitsAllocated;
  jint bitsStored;
  jint highBit;
  jint pixelRepresentation;
  jint samplesPerPixel;

  jfloat windowCenter;
  jfloat windowWidth;
  jfloat rescaleSlope;
  jfloat rescaleIntercept;
} IMAGE_PARAMETERS;


static void getImageParameters(JNIEnv *env, jobject obj, IMAGE_PARAMETERS *params)
{
  params->rows = getIntParameter(env, obj, "Rows");
  params->cols = getIntParameter(env, obj, "Columns");
  params->bitsAllocated = getIntParameter(env, obj, "BitsAllocated");
  params->bitsStored = getIntParameter(env, obj, "BitsStored");
  params->highBit = getIntParameter(env, obj, "HighBit");
  params->pixelRepresentation = getIntParameter(env, obj, "PixelRepresentation");
  params->samplesPerPixel = getIntParameter(env, obj, "SamplesPerPixel");

  params->windowCenter = getFloatParameter(env, obj, "WindowCenter");
  params->windowWidth = getFloatParameter(env, obj, "WindowWidth");
  params->rescaleSlope = getFloatParameter(env, obj, "RescaleSlope");
  params->rescaleIntercept = getFloatParameter(env, obj, "RescaleIntercept");
}

static void* getPixels(jlong dcmObj, jint rows, jint cols, jint samplesPerPixel,
		       jint bitsPerPixel, jint frame)
{
  unsigned char* p;
  DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OB, "", 1, 0, NULL};
  unsigned long offset;
  CONDITION cond;
  U16 localRows = 0;
  U16 localCols = 0;

  DCM_ELEMENT row_cols[] = {
    { DCM_IMGROWS, DCM_US, "", 1, sizeof(U16), NULL},
    { DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(U16), NULL}
  };

  row_cols[0].d.us = &localRows;
  row_cols[1].d.us = &localCols;

  cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, row_cols, 2, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Could not get rows/cols in getPixels\n");  /* repair */
    COND_DumpConditions();
    exit(1);
  }

  e.length = localRows * localCols * samplesPerPixel * (bitsPerPixel/8);
  p = malloc(e.length);
  if (p == NULL) {                       /* repair */
    fprintf(stderr, "Unable to malloc %d bytes in getPixels\n",
	    rows * cols * samplesPerPixel * (bitsPerPixel/8));
    exit(1);
  }

  e.d.ot = p;
  offset = (frame - 1) * e.length;
  cond = DCM_GetElementValueOffset((DCM_OBJECT**)&dcmObj, &e, offset);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);   /* repair */
  }
  return p;
}


static void map16BitToColor32(const U16 *up, U32 *pixels,
			      const IMAGE_PARAMETERS* params,
			      U16 originalRows, U16 originalColumns,
			      jint center, jint width)
{
  if (params->pixelRepresentation == DCM_PIXELUNSIGNED) {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S32 intensity;
    float slope;

    shift = center - width/2;
    slope = 254./(float)width;

    for (index = 0; index < params->rows * params->cols; index++) {
      p = *up++;
      p = (p-shift) * slope;
      if (p < 0)
	p = 0;
      else if (p > 254)
	p = 254;

      *pixels++ = p << 16 | p << 8 | p;
    }
  } else {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S16 *sp;
    S32 intensity;
    float slope;

    sp = (S16 *)up;

    /*  Some of the NM images we see have no C/W values and terrible
    **  values for high bit.  We are going to try to recalculate here.
    */

    if (originalColumns == 64 && width > 4096) {
      S16 maxPixel = 0;
      S16 minPixel = 0;

      maxPixel = minPixel = sp[0];
      for (index = 0; index < originalRows * originalColumns; index++) {
	if (sp[index] > maxPixel)
	  maxPixel = sp[index];
	if (sp[index] < minPixel)
	  minPixel = sp[index];
      }

      width = maxPixel - minPixel;
      center = width/2;
    }
    shift = center - width/2;
    slope = 254./(float)width;

#if 0
    if (params->cols == 64) {
      for (index = 0; index < 64 * 64; index++) {
	fprintf(stderr, " %5d", sp[index]);
	if ((index % 8) == 1)
	  fprintf(stderr, "\n");
      }
    }
#endif

    if ((originalRows > 128) ||
	(originalColumns > 128) ||
	getenv("CTN_EXPANDED") == NULL) {

      for (index = 0; index < params->rows * params->cols; index++) {
	p = *sp++;
	p = (p-shift) * slope;
	if (p < 0)
	  p = 0;
	else if (p > 254)
	  p = 254;

	*pixels++ = p << 16 | p << 8 | p;
      }
    } else {
      int rowIndex = 0;
      int colIndex = 0;
      U32 newP;

      for (rowIndex = 0; rowIndex < originalRows; rowIndex++, pixels += 3*params->cols) {
	for (colIndex = 0; colIndex < originalColumns; colIndex++) {
	p = *sp++;
	p = (p-shift) * slope;
	if (p < 0)
	  p = 0;
	else if (p > 254)
	  p = 254;

	newP = p << 16 | p << 8 | p;

	pixels[0] = newP;
	pixels[1] = newP;
	pixels[2] = newP;
	pixels[3] = newP;

	pixels[params->cols] = newP;
	pixels[params->cols+1] = newP;
	pixels[params->cols+2] = newP;
	pixels[params->cols+3] = newP;

	pixels[(2*params->cols)] = newP;
	pixels[(2*params->cols) + 1] = newP;
	pixels[(2*params->cols) + 2] = newP;
	pixels[(2*params->cols) + 3] = newP;

	pixels[(3*params->cols)] = newP;
	pixels[(3*params->cols) + 1] = newP;
	pixels[(3*params->cols) + 2] = newP;
	pixels[(3*params->cols) + 3] = newP;

	pixels+=4;
	}
      }
    }
  }
}

static void map16BitToColor32Slope(const U16 *up, U32 *pixels,
				   const IMAGE_PARAMETERS* params,
				   jint center, jint width)
{
  if (params->pixelRepresentation == DCM_PIXELUNSIGNED) {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S32 intensity;
    float slope;

    shift = center - width/2;
    slope = 254./(float)width;

#if 0
    fprintf(stderr, "C/W = %d/%d\n", center,width);
    fprintf(stderr, "S/I = %f/%f\n", params->rescaleSlope, params->rescaleIntercept);
    fprintf(stderr, "shift/slope = %d/%f\n", shift, slope);
#endif


    for (index = 0; index < params->rows * params->cols; index++) {
      p = *up++;
      p = (p * params->rescaleSlope) + params->rescaleIntercept;
      p = (p-shift) * slope;
      if (p < 0)
	p = 0;
      else if (p > 254)
	p = 254;

      *pixels++ = p << 16 | p << 8 | p;
    }
  } else {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S32 intensity;
    float slope;

    shift = center - width/2;
    slope = 254./(float)width;

#if 0
    fprintf(stderr, "C/W = %d/%d\n", center,width);
    fprintf(stderr, "S/I = %f/%f\n", params->rescaleSlope, params->rescaleIntercept);
    fprintf(stderr, "shift/slope = %d/%f\n", shift, slope);
#endif

    for (index = 0; index < params->rows * params->cols; index++) {
      p = *up++;
      p = (p * params->rescaleSlope) + params->rescaleIntercept;
      p = (p-shift) * slope;
      if (p < 0)
	p = 0;
      else if (p > 254)
	p = 254;

      *pixels++ = p << 16 | p << 8 | p;
    }
  }
}

static void getPixelFrameAsColor_Monochrome_8(JNIEnv *env, jobject obj, jlong dcmObj,
					    jintArray arr, jint rows, jint cols,
					    jint frame, const IMAGE_PARAMETERS *params)
{
  U32 *pixels = 0;
  unsigned char *p = 0;
  CONDITION cond;
  jsize len;
  int index;
  int pixelCount;
  U16 localRows = 0;
  U16 localCols = 0;

  DCM_ELEMENT row_cols[] = {
    { DCM_IMGROWS, DCM_US, "", 1, sizeof(U16), NULL},
    { DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(U16), NULL}
  };

  len = (*env)->GetArrayLength(env, arr);
  pixels = (U32*)(*env)->GetIntArrayElements(env, arr, 0);

  p = getPixels(dcmObj, params->rows, params->cols, params->samplesPerPixel, 8, frame);

  pixelCount = params->rows * params->cols * params->samplesPerPixel;

  row_cols[0].d.us = &localRows;
  row_cols[1].d.us = &localCols;

  cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, row_cols, 2, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Could not get rows/cols in getPixels\n");  /* repair */
    COND_DumpConditions();
    exit(1);
  }

  if (params->rows == localRows) {
    for (index = 0; index < pixelCount; index++) {
      pixels[index] = p[index] << 16 | p[index] << 8 | p[index];
    }
  } else if (params->rows < localRows) {
    int rowIndex = 0;
    int colIndex = 0;
    int srcIndex;

    index = 0;
    for (rowIndex = 0; rowIndex < params->rows; rowIndex++) {
      srcIndex = rowIndex * 2 * localCols;
      for (colIndex = 0; colIndex < params->cols; colIndex++, index++, srcIndex += 2) {
	pixels[index] = p[srcIndex] << 16 | p[srcIndex] << 8 | p[srcIndex];
      }
    }
  } else {
    int rowIndex = 0;
    int colIndex = 0;
    int srcIndex = 0;
    int index = 0;

    U32 newP;

    for (rowIndex = 0; rowIndex < localRows; rowIndex ++, index += 3*params->cols) {
      for (colIndex = 0; colIndex < localCols; colIndex++) {
	newP = p[srcIndex] << 16 | p[srcIndex] << 8 | p[srcIndex];
	srcIndex++;

	pixels[index] = newP;
	pixels[index + 1] = newP;
	pixels[index + 2] = newP;
	pixels[index + 3] = newP;

	pixels[params->cols+ index] = newP;
	pixels[params->cols + index +1] = newP;
	pixels[params->cols + index + 2] = newP;
	pixels[params->cols + index + 3] = newP;

	pixels[(2*params->cols)] = newP;
	pixels[(2*params->cols) + 1] = newP;
	pixels[(2*params->cols) + 2] = newP;
	pixels[(2*params->cols) + 3] = newP;

	pixels[(2*params->cols) + index] = newP;
	pixels[(2*params->cols) + index + 1] = newP;
	pixels[(2*params->cols) + index + 2] = newP;
	pixels[(2*params->cols) + index + 3] = newP;

	index += 4;
      }
    }
  }


  (*env)->ReleaseIntArrayElements(env, arr, (jint*)pixels, 0);

  free(p);
}
static void getPixelFrameAsColor_Monochrome_16(JNIEnv *env, jobject obj, jlong dcmObj,
					    jintArray arr, jint rows, jint cols,
					    jint frame, const IMAGE_PARAMETERS *params)
{
  U32 *pixels = 0;
  U16 *up = 0;
  CONDITION cond;
  jsize len;
  int index;
  int pixelCount;
  jint computedWidth;
  jint computedCenter;

  U16 localRows = 0;
  U16 localCols = 0;

  DCM_ELEMENT row_cols[] = {
    { DCM_IMGROWS, DCM_US, "", 1, sizeof(U16), NULL},
    { DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(U16), NULL}
  };
  jboolean flag = 1;

  row_cols[0].d.us = &localRows;
  row_cols[1].d.us = &localCols;

  cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, row_cols, 2, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Could not get rows/cols in getPixels\n");  /* repair */
    COND_DumpConditions();
    exit(1);
  }

  len = (*env)->GetArrayLength(env, arr);

  pixels = (U32*)(*env)->GetIntArrayElements(env, arr, &flag);

  up = getPixels(dcmObj, params->rows, params->cols, params->samplesPerPixel, 16, frame);

  pixelCount = params->rows * params->cols * params->samplesPerPixel;
  computedCenter = (1 << params->highBit);
  computedWidth = computedCenter << 1;

  if (params->rescaleSlope == 0. && params->windowWidth == 0)
    map16BitToColor32(up, pixels, params, localRows, localCols,
		      computedCenter, computedWidth);
  else if (params->rescaleSlope == 0.)
    map16BitToColor32(up, pixels, params, localRows, localCols,
		      params->windowCenter, params->windowWidth);
  else if (params->windowWidth == 0)
    map16BitToColor32Slope(up, pixels, params, computedCenter, computedWidth);
  else
    map16BitToColor32Slope(up, pixels, params, params->windowCenter, params->windowWidth);

  free(up);

  (*env)->ReleaseIntArrayElements(env, arr, (jint*)pixels, 0);
}
static void getPixelFrameAsColor_Monochrome(JNIEnv *env, jobject obj, jlong dcmObj,
					    jintArray arr, jint rows, jint cols,
					    jint frame, const IMAGE_PARAMETERS *params)
{
  if (params->bitsAllocated == 8)
    getPixelFrameAsColor_Monochrome_8(env, obj, dcmObj, arr, rows, cols, frame, params);
  else
    getPixelFrameAsColor_Monochrome_16(env, obj, dcmObj, arr, rows, cols, frame, params);
}

static void fillPaletteDescriptor(jlong dcmObj, DCM_TAG tag, U16 *descriptor)
{
  DCM_ELEMENT e;
  CONDITION cond;
  void *ctx = NULL;
  U32 rtnLength;

  memset(&e, 0, sizeof e);
  e.tag = tag;

  e.representation = DCM_US;
  e.description[0] = '\0';
  e.multiplicity = 1;
  e.length = 3*sizeof(U16);
  e.d.us = descriptor;

  cond = DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);
  if (cond != DCM_NORMAL) {
    memset(descriptor, 0, 3*sizeof(U16));
    COND_PopCondition(TRUE);
  }
}

static void fillPaletteMap(jlong dcmObj, U16 *descriptor, U16 *map, DCM_TAG tag)
{
  DCM_ELEMENT e;
  void *ctx = NULL;
  U32 rtnLength = 0;
  CONDITION cond;

  e.tag = tag;

  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond != DCM_NORMAL)
    return;

  e.d.us = map;

  cond = DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);
  if (cond != DCM_NORMAL) {
    memset(map, 0, 256 * sizeof(S16));
    COND_PopCondition(TRUE);
  }

}

static void remap(U16 *map)
{
  int i;
  int flag = 0;

  for (i = 0; (i < 256) && (flag == 0); i++) {
    if ((map[i] & 0xff00) != 0)
      flag = 1;
  }
  if (flag != 0) {
    for (i = 0; i < 256; i++) {
      map[i] = (map[i] >> 8) & 0x00ff;
    }
  }
}

static void copyMap(U16 *descriptor, U16 *src, U16 *dst)
{
  int i;
  U16 entries;
  U16 firstPixel;

  entries = descriptor[0];
  firstPixel = descriptor[1];

  for (i = 0; i <= firstPixel; i++)
    dst[i] = src[0];

  for (i = 0; i < entries; i++)
    dst[firstPixel + i] = src[i];

  for (i = 255; i >= firstPixel + entries; i--)
    dst[i] = src[entries-1];

}

static void fillPalettes(jlong dcmObj, U16 *red, U16 *green, U16 *blue)
{
  DCM_ELEMENT e;
  CONDITION cond;
  U16 redDescriptor[3];
  U16 greenDescriptor[3];
  U16 blueDescriptor[3];
  void *ctx = NULL;
  jfieldID fid;
  jclass cls;
  U32 rtnLength;
  int i, j;
  U16 localRed[256];
  U16 localGreen[256];
  U16 localBlue[256];


  fillPaletteDescriptor(dcmObj, DCM_IMGLUTDESCRIPTRED, redDescriptor);
  fillPaletteDescriptor(dcmObj, DCM_IMGLUTDESCRIPTGREEN, greenDescriptor);
  fillPaletteDescriptor(dcmObj, DCM_IMGLUTDESCRIPTBLUE, blueDescriptor);

  fillPaletteMap(dcmObj, redDescriptor, localRed, DCM_IMGLOOKUPDATARED);
  fillPaletteMap(dcmObj, greenDescriptor, localGreen, DCM_IMGLOOKUPDATAGREEN);
  fillPaletteMap(dcmObj, blueDescriptor, localBlue, DCM_IMGLOOKUPDATABLUE);

  copyMap(redDescriptor, localRed, red);
  copyMap(greenDescriptor, localGreen, green);
  copyMap(blueDescriptor, localBlue, blue);

  remap(red);
  remap(green);
  remap(blue);
}


static void getPixelFrameAsColor_Palette(JNIEnv *env, jobject obj, jlong dcmObj,
					 jintArray arr, jint rows, jint cols,
					 jint frame, const IMAGE_PARAMETERS *params)
{
  U16 redTable[256];
  U16 greenTable[256];
  U16 blueTable[256];

  U32 *pixels = 0;
  unsigned char *p = 0;
  CONDITION cond;
  jsize len;
  int index;
  int pixelCount;

  fillPalettes(dcmObj, redTable, greenTable, blueTable);

  len = (*env)->GetArrayLength(env, arr);
  pixels = (U32*)(*env)->GetIntArrayElements(env, arr, 0);

  p = getPixels(dcmObj, params->rows, params->cols, params->samplesPerPixel, 8, frame);

  pixelCount = params->rows * params->cols;
  for (index = 0; index < pixelCount; index++) {
    pixels[index] =
      redTable[p[index]] << 16 |
      greenTable[p[index]] << 8 |
      blueTable[p[index]];
  }

  free(p);
}
static void getPixelFrameAsColor_RGB(JNIEnv *env, jobject obj, jlong dcmObj,
				     jintArray arr, jint rows, jint cols,
				     jint frame, const IMAGE_PARAMETERS *params)
{
  U32 *pixels = 0;
  unsigned char *p = 0;
  unsigned char *p1 = 0;
  CONDITION cond;
  jsize len;
  int index;
  int pixelCount;

  len = (*env)->GetArrayLength(env, arr);
  pixels = (U32*)(*env)->GetIntArrayElements(env, arr, 0);

  p = getPixels(dcmObj, params->rows, params->cols, params->samplesPerPixel, 8, frame);

  pixelCount = params->rows * params->cols;
  for (index = 0, p1 = p; index < pixelCount; index++, p1 += 3) {
    pixels[index] = p1[0] << 16 | p1[1] << 8 | p1[2];
  }

  free(p);
}

JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_getPixelFrameAsColor
  (JNIEnv *env, jobject obj, jlong dcmObj, jintArray arr,
   jint rows, jint cols, jint frame)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;
  jobject photometricInterpretation;
  const char *str = 0;
  IMAGE_PARAMETERS parameters;

  cls = (*env)->GetObjectClass(env, obj);

  fid = (*env)->GetFieldID(env, cls, "PhotometricInterpretation", "Ljava/lang/String;");
  if (fid == 0)
    return;

  photometricInterpretation = (*env)->GetObjectField(env, obj, fid);

  str = (*env)->GetStringUTFChars(env, photometricInterpretation, 0);

#if 0
  printf("%s\n", str);
#endif

  getImageParameters(env, obj, &parameters);

#if 0
  printf("%d %d %f %f %f %f\n", parameters.rows, parameters.cols,
	 parameters.windowCenter, parameters.windowWidth,
	 parameters.rescaleSlope, parameters.rescaleIntercept);
#endif

  if (strcmp(str, "MONOCHROME1") == 0)
    getPixelFrameAsColor_Monochrome(env, obj, dcmObj, arr, rows, cols, frame, &parameters);
  else if (strcmp(str, "MONOCHROME2") == 0)
    getPixelFrameAsColor_Monochrome(env, obj, dcmObj, arr, rows, cols, frame, &parameters);
  else if (strcmp(str, "RGB") == 0)
    getPixelFrameAsColor_RGB(env, obj, dcmObj, arr, rows, cols, frame, &parameters);
  else if (strcmp(str, "PALETTE COLOR") == 0)
    getPixelFrameAsColor_Palette(env, obj, dcmObj, arr, rows, cols, frame, &parameters);

#if 0
  (*env)->DeleteLocalRef(env, arr);
#endif

}


/* Now the stuff for indexed (monochrome images) */


static void map16BitToIndexed(const U16 *up, unsigned char *pixels,
			      const IMAGE_PARAMETERS* params,
			      U16 originalRows, U16 originalColumns,
			      jint center, jint width)
{
  if (params->pixelRepresentation == DCM_PIXELUNSIGNED) {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S32 intensity;
    float slope;

    shift = center - width/2;
    slope = 254./(float)width;

    for (index = 0; index < params->rows * params->cols; index++) {
      p = *up++;
      p = (p-shift) * slope;
      if (p < 0)
	p = 0;
      else if (p > 254)
	p = 254;

      *pixels++ = p;
    }
  } else {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S16 *sp;
    S32 intensity;
    float slope;

    sp = (S16 *)up;

    /*  Some of the NM images we see have no C/W values and terrible
    **  values for high bit.  We are going to try to recalculate here.
    */

    if (originalColumns == 64 && width > 4096) {
      S16 maxPixel = 0;
      S16 minPixel = 0;

      maxPixel = minPixel = sp[0];
      for (index = 0; index < originalRows * originalColumns; index++) {
	if (sp[index] > maxPixel)
	  maxPixel = sp[index];
	if (sp[index] < minPixel)
	  minPixel = sp[index];
      }

      width = maxPixel - minPixel;
      center = width/2;
    }
    shift = center - width/2;
    slope = 254./(float)width;

#if 0
    if (params->cols == 64) {
      for (index = 0; index < 64 * 64; index++) {
	fprintf(stderr, " %5d", sp[index]);
	if ((index % 8) == 1)
	  fprintf(stderr, "\n");
      }
    }
#endif

    if ((originalRows > 128) ||
	(originalColumns > 128) ||
	getenv("CTN_EXPANDED") == NULL) {

      for (index = 0; index < params->rows * params->cols; index++) {
	p = *sp++;
	p = (p-shift) * slope;
	if (p < 0)
	  p = 0;
	else if (p > 254)
	  p = 254;

	*pixels++ = p;
      }
    } else {
      int rowIndex = 0;
      int colIndex = 0;
      U32 newP;

      for (rowIndex = 0; rowIndex < originalRows; rowIndex++, pixels += 3*params->cols) {
	for (colIndex = 0; colIndex < originalColumns; colIndex++) {
	p = *sp++;
	p = (p-shift) * slope;
	if (p < 0)
	  p = 0;
	else if (p > 254)
	  p = 254;

	newP = p;

	pixels[0] = newP;
	pixels[1] = newP;
	pixels[2] = newP;
	pixels[3] = newP;

	pixels[params->cols] = newP;
	pixels[params->cols+1] = newP;
	pixels[params->cols+2] = newP;
	pixels[params->cols+3] = newP;

	pixels[(2*params->cols)] = newP;
	pixels[(2*params->cols) + 1] = newP;
	pixels[(2*params->cols) + 2] = newP;
	pixels[(2*params->cols) + 3] = newP;

	pixels[(3*params->cols)] = newP;
	pixels[(3*params->cols) + 1] = newP;
	pixels[(3*params->cols) + 2] = newP;
	pixels[(3*params->cols) + 3] = newP;

	pixels+=4;
	}
      }
    }
  }
}

static void map16BitToIndexedSlope(const U16 *up, unsigned char *pixels,
				   const IMAGE_PARAMETERS* params,
				   jint center, jint width)
{
  if (params->pixelRepresentation == DCM_PIXELUNSIGNED) {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S32 intensity;
    float slope;

    shift = center - width/2;
    slope = 254./(float)width;

#if 0
    fprintf(stderr, "C/W = %d/%d\n", center,width);
    fprintf(stderr, "S/I = %f/%f\n", params->rescaleSlope, params->rescaleIntercept);
    fprintf(stderr, "shift/slope = %d/%f\n", shift, slope);
#endif


    for (index = 0; index < params->rows * params->cols; index++) {
      p = *up++;
      p = (p * params->rescaleSlope) + params->rescaleIntercept;
      p = (p-shift) * slope;
      if (p < 0)
	p = 0;
      else if (p > 254)
	p = 254;

      *pixels++ = p;
    }
  } else {
    S32 shift = 0;
    S32 index = 0;
    S32 p;
    S32 intensity;
    float slope;

    shift = center - width/2;
    slope = 254./(float)width;

#if 0
    fprintf(stderr, "C/W = %d/%d\n", center,width);
    fprintf(stderr, "S/I = %f/%f\n", params->rescaleSlope, params->rescaleIntercept);
    fprintf(stderr, "shift/slope = %d/%f\n", shift, slope);
#endif

    for (index = 0; index < params->rows * params->cols; index++) {
      p = *up++;
      p = (p * params->rescaleSlope) + params->rescaleIntercept;
      p = (p-shift) * slope;
      if (p < 0)
	p = 0;
      else if (p > 254)
	p = 254;

      *pixels++ = p;
    }
  }
}

static void getPixelFrameAsIndexed_Monochrome_8(JNIEnv *env, jobject obj, jlong dcmObj,
					    jbyteArray arr, jint rows, jint cols,
					    jint frame, const IMAGE_PARAMETERS *params)
{
  unsigned char *pixels = 0;
  unsigned char *p = 0;
  CONDITION cond;
  jsize len;
  int index;
  int pixelCount;
  U16 localRows = 0;
  U16 localCols = 0;

  DCM_ELEMENT row_cols[] = {
    { DCM_IMGROWS, DCM_US, "", 1, sizeof(U16), NULL},
    { DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(U16), NULL}
  };

  len = (*env)->GetArrayLength(env, arr);
  pixels = (unsigned char*)(*env)->GetByteArrayElements(env, arr, 0);

  p = getPixels(dcmObj, params->rows, params->cols, params->samplesPerPixel, 8, frame);

  pixelCount = params->rows * params->cols * params->samplesPerPixel;

  row_cols[0].d.us = &localRows;
  row_cols[1].d.us = &localCols;

  cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, row_cols, 2, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Could not get rows/cols in getPixels\n");  /* repair */
    COND_DumpConditions();
    exit(1);
  }

  if (params->rows == localRows) {
#if 0
    for (index = 0; index < pixelCount; index++) {
      pixels[index] = p[index];
    }
#endif
    memcpy(pixels, p, pixelCount);
  } else if (params->rows < localRows) {
    int rowIndex = 0;
    int colIndex = 0;
    int srcIndex;

    index = 0;
    for (rowIndex = 0; rowIndex < params->rows; rowIndex++) {
      srcIndex = rowIndex * 2 * localCols;
      for (colIndex = 0; colIndex < params->cols; colIndex++, index++, srcIndex += 2) {
	pixels[index] = p[srcIndex];
      }
    }
  } else {
    int rowIndex = 0;
    int colIndex = 0;
    int srcIndex = 0;
    int index = 0;

    U32 newP;

    for (rowIndex = 0; rowIndex < localRows; rowIndex ++, index += 3*params->cols) {
      for (colIndex = 0; colIndex < localCols; colIndex++) {
	newP = p[srcIndex];
	srcIndex++;

	pixels[index] = newP;
	pixels[index + 1] = newP;
	pixels[index + 2] = newP;
	pixels[index + 3] = newP;

	pixels[params->cols+ index] = newP;
	pixels[params->cols + index +1] = newP;
	pixels[params->cols + index + 2] = newP;
	pixels[params->cols + index + 3] = newP;

	pixels[(2*params->cols)] = newP;
	pixels[(2*params->cols) + 1] = newP;
	pixels[(2*params->cols) + 2] = newP;
	pixels[(2*params->cols) + 3] = newP;

	pixels[(2*params->cols) + index] = newP;
	pixels[(2*params->cols) + index + 1] = newP;
	pixels[(2*params->cols) + index + 2] = newP;
	pixels[(2*params->cols) + index + 3] = newP;

	index += 4;
      }
    }
  }


  (*env)->ReleaseByteArrayElements(env, arr, (jbyte*)pixels, 0);

  free(p);
}
static void getPixelFrameAsIndexed_Monochrome_16(JNIEnv *env, jobject obj, jlong dcmObj,
					    jbyteArray arr, jint rows, jint cols,
					    jint frame, const IMAGE_PARAMETERS *params)
{
  unsigned char *pixels = 0;
  U16 *up = 0;
  CONDITION cond;
  jsize len;
  int index;
  int pixelCount;
  jint computedWidth;
  jint computedCenter;

  U16 localRows = 0;
  U16 localCols = 0;

  DCM_ELEMENT row_cols[] = {
    { DCM_IMGROWS, DCM_US, "", 1, sizeof(U16), NULL},
    { DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(U16), NULL}
  };
  jboolean flag = 1;

  row_cols[0].d.us = &localRows;
  row_cols[1].d.us = &localCols;

  cond = DCM_ParseObject((DCM_OBJECT**)&dcmObj, row_cols, 2, NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Could not get rows/cols in getPixels\n");  /* repair */
    COND_DumpConditions();
    exit(1);
  }

  len = (*env)->GetArrayLength(env, arr);

  pixels = (unsigned char*)(*env)->GetByteArrayElements(env, arr, &flag);

  up = getPixels(dcmObj, params->rows, params->cols, params->samplesPerPixel, 16, frame);

  pixelCount = params->rows * params->cols * params->samplesPerPixel;
  computedCenter = (1 << params->highBit);
  computedWidth = computedCenter << 1;

  if (params->rescaleSlope == 0. && params->windowWidth == 0)
    map16BitToIndexed(up, pixels, params, localRows, localCols,
		      computedCenter, computedWidth);
  else if (params->rescaleSlope == 0.)
    map16BitToIndexed(up, pixels, params, localRows, localCols,
		      params->windowCenter, params->windowWidth);
  else if (params->windowWidth == 0)
    map16BitToIndexedSlope(up, pixels, params, computedCenter, computedWidth);
  else
    map16BitToIndexedSlope(up, pixels, params, params->windowCenter, params->windowWidth);

  free(up);

  (*env)->ReleaseByteArrayElements(env, arr, (jbyte*)pixels, 0);
}

static void getPixelFrameAsIndexed_Monochrome(JNIEnv *env, jobject obj, jlong dcmObj,
					    jbyteArray arr, jint rows, jint cols,
					    jint frame, const IMAGE_PARAMETERS *params)
{
  if (params->bitsAllocated == 8)
    getPixelFrameAsIndexed_Monochrome_8(env, obj, dcmObj, arr, rows, cols, frame, params);
  else
    getPixelFrameAsIndexed_Monochrome_16(env, obj, dcmObj, arr, rows, cols, frame, params);
}


JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_getPixelFrameAsIndexed
  (JNIEnv *env, jobject obj, jlong dcmObj, jbyteArray arr,
   jint rows, jint cols, jint frame)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;
  jobject photometricInterpretation;
  const char *str = 0;
  IMAGE_PARAMETERS parameters;

  cls = (*env)->GetObjectClass(env, obj);

  fid = (*env)->GetFieldID(env, cls, "PhotometricInterpretation", "Ljava/lang/String;");
  if (fid == 0)
    return;

  photometricInterpretation = (*env)->GetObjectField(env, obj, fid);

  str = (*env)->GetStringUTFChars(env, photometricInterpretation, 0);

  getImageParameters(env, obj, &parameters);

#if 0
  printf("%d %d %f %f %f %f\n", parameters.rows, parameters.cols,
	 parameters.windowCenter, parameters.windowWidth,
	 parameters.rescaleSlope, parameters.rescaleIntercept);
#endif

  if (strcmp(str, "MONOCHROME1") == 0)
    getPixelFrameAsIndexed_Monochrome(env, obj, dcmObj, arr, rows, cols, frame, &parameters);
  else if (strcmp(str, "MONOCHROME2") == 0)
    getPixelFrameAsIndexed_Monochrome(env, obj, dcmObj, arr, rows, cols, frame, &parameters);
  else if (strcmp(str, "RGB") == 0) {
    fprintf(stderr, "Not supposed to ask for indexed pixel data with RGB image\n");
    exit(1);
  } else if (strcmp(str, "PALETTE COLOR") == 0) {
    fprintf(stderr, "Not supposed to ask for indexed pixel data with palette color\n");
    exit(1);
  }
}

JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_fillPaletteDescriptor(
  JNIEnv *env, jobject obj, jlong dcmObj, jstring color, jobject palette)
{
  DCM_ELEMENT e;
  CONDITION cond;
  U16 paletteDescriptor[3];
  void *ctx = NULL;
  jfieldID fid;
  jclass cls;
  U32 rtnLength;


  const char *str = (*env)->GetStringUTFChars(env, color, 0);

  memset(&e, 0, sizeof e);
  if (strcmp(str, "RED") == 0)
    e.tag = DCM_IMGLUTDESCRIPTRED;
  else if (strcmp(str, "GREEN") == 0)
    e.tag = DCM_IMGLUTDESCRIPTGREEN;
  else
    e.tag = DCM_IMGLUTDESCRIPTBLUE;

  e.representation = DCM_US;
  e.description[0] = '\0';
  e.multiplicity = 1;
  e.length = sizeof(paletteDescriptor);
  e.d.us = paletteDescriptor;

  cond = DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);
  if (cond != DCM_NORMAL) {
    memset(paletteDescriptor, 0, sizeof(paletteDescriptor));
    COND_PopCondition(TRUE);
  }

  cls = (*env)->GetObjectClass(env, palette);

  fid = (*env)->GetFieldID(env, cls, "Entries", "S");
  if (fid == 0)
    return;
  (*env)->SetShortField(env, palette, fid, paletteDescriptor[0]);

  fid = (*env)->GetFieldID(env, cls, "FirstPixel", "S");
  if (fid == 0)
    return;
  (*env)->SetShortField(env, palette, fid, paletteDescriptor[1]);

  fid = (*env)->GetFieldID(env, cls, "BitsPerPixel", "S");
  if (fid == 0)
    return;
  (*env)->SetShortField(env, palette, fid, paletteDescriptor[2]);
}

JNIEXPORT void JNICALL Java_DICOM_Module_ImagePixelModule_fillPaletteMap(
  JNIEnv *env, jobject obj, jlong dcmObj, jstring color,
  jint entries, jint bitsPerPixel, jshortArray map)
{
  DCM_ELEMENT e;
  CONDITION cond;
  void *ctx = NULL;
  jfieldID fid;
  jclass cls;
  U32 rtnLength;
  DCM_TAG tag;
  S16 *mapPtr;

  const char *str = (*env)->GetStringUTFChars(env, color, 0);

  memset(&e, 0, sizeof e);
  if (strcmp(str, "RED") == 0)
    tag = DCM_IMGLOOKUPDATARED;
  else if (strcmp(str, "GREEN") == 0)
    tag = DCM_IMGLOOKUPDATAGREEN;
  else
    tag = DCM_IMGLOOKUPDATABLUE;

  cond = DCM_GetElement((DCM_OBJECT**)&dcmObj, tag, &e);
  if (cond != DCM_NORMAL)
    return;

  e.representation = DCM_US;
  if (e.representation != DCM_US) {
    fprintf(stderr, "Representation for lookup table is not US\n");
    exit(1);
  }

  mapPtr = (*env)->GetShortArrayElements(env, map, 0);
  e.d.ss = mapPtr;

  cond = DCM_GetElementValue((DCM_OBJECT**)&dcmObj, &e, &rtnLength, &ctx);
  if (cond != DCM_NORMAL) {
    memset(mapPtr, 0, entries * sizeof(S16));
    COND_PopCondition(TRUE);
  }

  (*env)->ReleaseShortArrayElements(env, map, mapPtr, 0);
}


/* The following code supports the Waveform IOD */

JNIEXPORT void JNICALL Java_DICOM_InfoObj_Waveform_setPatientAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jobject patient)
{
  Java_DICOM_InfoObj_Image_setPatientAttributes(env, obj, dcmObj, patient);
}

JNIEXPORT void JNICALL Java_DICOM_InfoObj_Waveform_setWaveformModuleAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jobject waveform)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;

  LST_HEAD *waveformSequence;
  CONDITION cond;
  DCM_SEQUENCE_ITEM *seqItem;

  U16 numberOfChannels = 0;
  U32 numberOfSamples = 0;
  float samplingFrequency = 0.;
  char samplingFrequencyTxt[DICOM_DS_LENGTH*2 + 2] = "";
  char groupLabel[DICOM_SH_LENGTH + 1] = "";
  char *c;
  char waveformSampleValueRepresentation[DICOM_CS_LENGTH+1];
  long flag = 1;

  DCM_ELEMENT e[] = {
    { DCM_MAKETAG(0x003a, 0x0005), DCM_US, "", 1,
      sizeof(numberOfChannels), NULL },
    { DCM_MAKETAG(0x003a, 0x0010), DCM_UL, "", 1,
      sizeof(numberOfSamples), NULL },
    { DCM_MAKETAG(0x003a, 0x001a), DCM_DS, "", 1,
      sizeof(samplingFrequencyTxt), NULL},
    { DCM_MAKETAG(0x003a, 0x0103), DCM_CS, "", 1,
      sizeof(waveformSampleValueRepresentation), NULL}
  };

  DCM_FLAGGED_ELEMENT optional[] = {
    { DCM_MAKETAG(0x003a, 0x0020), DCM_SH, "", 1, sizeof(groupLabel), NULL, 1, NULL }
  };

  cond = DCM_GetSequenceList((DCM_OBJECT**) &dcmObj,
			     DCM_MAKETAG(0x003a, 0x0002),
			     &waveformSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  seqItem = LST_Head(&waveformSequence);
  if (seqItem == NULL) {
    fprintf(stderr, "Empty waveform sequence\n");
    exit(1);  /* repair */
  }

  e[0].d.us = &numberOfChannels;
  e[1].d.ul = &numberOfSamples;
  e[2].d.string = samplingFrequencyTxt;
  e[3].d.string = waveformSampleValueRepresentation;

  optional[0].e.d.string = groupLabel;
  optional[0].flagAddress = &flag;

  cond = DCM_ParseObject(&seqItem->object, e, (int)DIM_OF(e),
			 optional, (int)DIM_OF(optional), NULL);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair */
  }

  cls = (*env)->GetObjectClass(env, waveform);

  fid = (*env)->GetFieldID(env, cls, "DCMObject", "J");
  if (fid == 0)
    return;
  (*env)->SetLongField(env, waveform, fid, dcmObj);

  fid = (*env)->GetFieldID(env, cls, "NumberOfChannels", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, waveform, fid, numberOfChannels);

  fid = (*env)->GetFieldID(env, cls, "NumberOfSamples", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, waveform, fid, numberOfSamples);

  c = strtok(samplingFrequencyTxt, "\\");
  if (c == NULL) {
    samplingFrequency = 1.;
  } else {
    samplingFrequency = atof(c);

    c = strtok(NULL, "\\");
    if (c != NULL)
      samplingFrequency /= atof(c);
  }

  fid = (*env)->GetFieldID(env, cls, "SamplingFrequency", "F");
  if (fid == 0)
    return;
  (*env)->SetFloatField(env, waveform, fid, samplingFrequency);

  fid = (*env)->GetFieldID(env, cls, "GroupLabel", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, groupLabel);
  (*env)->SetObjectField(env, waveform, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "WaveformSampleValueRepresentation",
			   "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, waveformSampleValueRepresentation);
  (*env)->SetObjectField(env, waveform, fid, jstr);
}

/* These functions support the Waveform Module (the module inside
** inside the Waveform IOD.
*/

JNIEXPORT void JNICALL Java_DICOM_Module_WaveformModule_setChannelAttributes
  (JNIEnv *env, jobject obj, jlong dcmObj, jint ordinalNumber, jobject channel)
{
  jfieldID fid;
  jstring jstr;
  jclass cls;

  LST_HEAD *waveformSequence;
  LST_HEAD *channelSequence;
  CONDITION cond;
  DCM_SEQUENCE_ITEM *seqItem;
  DCM_SEQUENCE_ITEM *channelItem;

  char channelStatus[DICOM_CS_LENGTH + 1] = "";
  char channelLabel[DICOM_SH_LENGTH + 1] = "";
  char channelNumberTxt[DICOM_IS_LENGTH + 1] = "-1";
  char channelSensitivityTxt[DICOM_DS_LENGTH + 1] = "";
  char channelSensitivityCorrectionFactorTxt[DICOM_DS_LENGTH + 1] = "";
  char channelBaselineTxt[DICOM_DS_LENGTH + 1] = "";
  U16 bitsPerSample = 0;
  int channelNumber;
  float f;
  long flag = 0;
  int idx;

  DCM_FLAGGED_ELEMENT optional[] = {
    { DCM_MAKETAG(0x003a, 0x0205), DCM_CS, "", 1,
      sizeof(channelStatus), NULL, 1, NULL },
    { DCM_MAKETAG(0x003a, 0x0202), DCM_IS, "", 1,
      sizeof(channelNumberTxt), NULL, 2, NULL },
    { DCM_MAKETAG(0x003a, 0x0203), DCM_IS, "", 1,
      sizeof(channelLabel), NULL, 4, NULL },
  };
  DCM_ELEMENT required[] = {
    { DCM_MAKETAG(0x003a, 0x0210), DCM_DS, "", 1,
      sizeof(channelSensitivityTxt), NULL},
    { DCM_MAKETAG(0x003a, 0x0212), DCM_DS, "", 1,
      sizeof(channelSensitivityCorrectionFactorTxt), NULL},
    { DCM_MAKETAG(0x003a, 0x0213), DCM_DS, "", 1,
      sizeof(channelBaselineTxt), NULL},
    { DCM_MAKETAG(0x003a, 0x021a), DCM_US, "", 1,
      sizeof(bitsPerSample), NULL}
  };

  cond = DCM_GetSequenceList((DCM_OBJECT**) &dcmObj,
			     DCM_MAKETAG(0x003a, 0x0002),
			     &waveformSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  seqItem = LST_Head(&waveformSequence);
  if (seqItem == NULL) {
    fprintf(stderr, "Empty waveform sequence\n");
    exit(1);  /* repair */
  }

  cond = DCM_GetSequenceList((DCM_OBJECT**) &seqItem->object,
				DCM_MAKETAG(0x003a, 0x0200),
				&channelSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  channelItem = LST_Head(&channelSequence);
  (void)LST_Position(&channelSequence, channelItem);
  for (idx = 1; idx <= ordinalNumber; idx++)
    channelItem = LST_Next(&channelSequence);

  if (channelItem == NULL) {
    fprintf(stderr, "Empty channel sequence\n");
    exit(1);  /* repair */
  }

  optional[0].e.d.string = channelStatus;
  optional[0].flagAddress = &flag;
  optional[1].e.d.string = channelNumberTxt;
  optional[1].flagAddress = &flag;
  optional[2].e.d.string = channelLabel;
  optional[2].flagAddress = &flag;

  required[0].d.string = channelSensitivityTxt;
  required[1].d.string = channelSensitivityCorrectionFactorTxt;
  required[2].d.string = channelBaselineTxt;
  required[3].d.us = &bitsPerSample;

  cond = DCM_ParseObject(&channelItem->object, required, (int)DIM_OF(required),
			optional, (int)DIM_OF(optional), NULL);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair */
  }

  cls = (*env)->GetObjectClass(env, channel);

  /* First, handle the required attributes */
  f = atof(channelSensitivityTxt);
  fid = (*env)->GetFieldID(env, cls, "ChannelSensitivity", "F");
  if (fid == 0)
    return;
  (*env)->SetFloatField(env, channel, fid, f);

  f = atof(channelSensitivityCorrectionFactorTxt);
  fid = (*env)->GetFieldID(env, cls, "ChannelSensitivityCorrectionFactor", "F");
  if (fid == 0)
    return;
  (*env)->SetFloatField(env, channel, fid, f);

  f = atof(channelBaselineTxt);
  fid = (*env)->GetFieldID(env, cls, "ChannelBaseline", "F");
  if (fid == 0)
    return;
  (*env)->SetFloatField(env, channel, fid, f);

  fid = (*env)->GetFieldID(env, cls, "BitsPerSample", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, channel, fid, bitsPerSample);



  /* This section handles type 3 attributes */

  fid = (*env)->GetFieldID(env, cls, "ChannelStatus", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, channelStatus);
  (*env)->SetObjectField(env, channel, fid, jstr);

  fid = (*env)->GetFieldID(env, cls, "ChannelLabel", "Ljava/lang/String;");
  if (fid == 0)
    return;
  jstr = (*env)->NewStringUTF(env, channelLabel);
  (*env)->SetObjectField(env, channel, fid, jstr);

  if (flag & 2)
    channelNumber = atoi(channelNumberTxt);
  else
    channelNumber = ordinalNumber;

  fid = (*env)->GetFieldID(env, cls, "ChannelNumber", "I");
  if (fid == 0)
    return;
  (*env)->SetIntField(env, channel, fid, channelNumber);

  /* This section handles second level objects within the waveform channel */
  /* Channel Source */
  {
    LST_HEAD *channelSourceSequence = NULL;
    jobject channelSourceObj;
    DCM_SEQUENCE_ITEM *channelSourceItem;

    fid = (*env)->GetFieldID(env, cls,"ChannelSource", "LDICOM/Misc/CodedValue;");
    if (fid == 0) {
      fprintf(stderr, "Could not find ChannelSource\n");
      return;
    }

    cond = DCM_GetSequenceList((DCM_OBJECT**) &channelItem->object,
			     DCM_MAKETAG(0x003a, 0x0208),
			     &channelSourceSequence);
    channelSourceItem = LST_Head(&channelSourceSequence);

    channelSourceObj = (*env)->GetObjectField(env, channel, fid);

    setCodedValues(env,
		   channelSourceObj,
		   &channelSourceItem->object);

  }

  /* Channel Sensitivity Units */
  {
    LST_HEAD *channelSourceSequence = NULL;
    jobject channelSourceObj;
    DCM_SEQUENCE_ITEM *channelSourceItem;

    fid = (*env)->GetFieldID(env, cls,"ChannelSensitivityUnits", "LDICOM/Misc/CodedValue;");
    if (fid == 0) {
      fprintf(stderr, "Could not find ChannelSensitivityUnits\n");
      return;
    }

    cond = DCM_GetSequenceList((DCM_OBJECT**) &channelItem->object,
			       DCM_MAKETAG(0x003a, 0x0211),
			       &channelSourceSequence);
    channelSourceItem = LST_Head(&channelSourceSequence);

    channelSourceObj = (*env)->GetObjectField(env, channel, fid);

    setCodedValues(env,
		   channelSourceObj,
		   &channelSourceItem->object);

  }

}


JNIEXPORT void JNICALL Java_DICOM_Module_WaveformModule_setChannelSource
  (JNIEnv *env, jobject obj, jlong dcmObj, jint ordinalNumber, jobject channelSource)
{
#if 0
  jfieldID fid;
  jstring jstr;
  jclass cls;

  LST_HEAD *waveformSequence;
  LST_HEAD *channelSequence;
  CONDITION cond;
  DCM_SEQUENCE_ITEM *seqItem;
  DCM_SEQUENCE_ITEM *channelItem;

  cond = DCM_GetSequenceList((DCM_OBJECT**) &dcmObj,
			     DCM_MAKETAG(0x003a, 0x0002),
			     &waveformSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  seqItem = LST_Head(&waveformSequence);
  if (seqItem == NULL) {
    fprintf(stderr, "Empty waveform sequence\n");
    exit(1);  /* repair */
  }

  cond = DCM_GetSequenceList((DCM_OBJECT**) &seqItem->object,
				DCM_MAKETAG(0x003a, 0x0200),
				&channelSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  channelItem = LST_Head(&channelSequence);
  if (channelItem == NULL) {
    fprintf(stderr, "Empty channel sequence\n");
    exit(1);  /* repair */
  }

  {
    LST_HEAD *channelSourceSequence = NULL;
    DCM_SEQUENCE_ITEM *channelSourceItem;

    cond = DCM_GetSequenceList((DCM_OBJECT**) &channelItem->object,
			     DCM_MAKETAG(0x003a, 0x0208),
			     &channelSourceSequence);
    channelSourceItem = LST_Head(&channelSourceSequence);

    setCodedValues(env,
		   obj,
		   &channelSourceItem->object);

  }
#endif
}









JNIEXPORT void JNICALL Java_DICOM_Module_WaveformModule_getSamplesByte(
  JNIEnv *env, jobject obj, jlong dcmObj, jbyteArray arr, jint channels,
  jint samplesPerChannel)
{
  jbyte *samples = 0;
  DCM_ELEMENT e = {DCM_MAKETAG(0x003a, 0x1000), DCM_OB, "", 1, 0, NULL};
  void *ctx = NULL;
  U32 rtnLength = 0;
  CONDITION cond;
  LST_HEAD *waveformSequence = NULL;
  DCM_SEQUENCE_ITEM *seqItem;

  jsize len;
  len = (*env)->GetArrayLength(env, arr);
  samples = (*env)->GetByteArrayElements(env, arr, 0);


  cond = DCM_GetSequenceList((DCM_OBJECT**) &dcmObj,
			     DCM_MAKETAG(0x003a, 0x0002),
			     &waveformSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  seqItem = LST_Head(&waveformSequence);
  if (seqItem == NULL) {
    fprintf(stderr, "Empty waveform sequence\n");
    exit(1);  /* repair */
  }


  if (len != channels * samplesPerChannel) {
    fprintf(stderr, "Byte: Len = %d, channels = %d, samplesPerChannel = %d\n",
	    len, channels, samplesPerChannel);
    exit(1);
  }

  e.length = len;
  e.d.ot = samples;
  DCM_GetElementValue((DCM_OBJECT**)&seqItem->object, &e, &rtnLength, &ctx);

  (*env)->ReleaseByteArrayElements(env, arr, samples, 0);
}

JNIEXPORT void JNICALL Java_DICOM_Module_WaveformModule_getSamplesShort(
  JNIEnv *env, jobject obj, jlong dcmObj, jshortArray arr, jint channels,
  jint samplesPerChannel)
{
  jshort *samples = 0;
  DCM_ELEMENT e = {DCM_MAKETAG(0x003a, 0x1000), DCM_US, "", 1, 0, NULL};
  void *ctx = NULL;
  U32 rtnLength = 0;
  CONDITION cond;
  LST_HEAD *waveformSequence = NULL;
  DCM_SEQUENCE_ITEM *seqItem;

  jsize len;
  len = (*env)->GetArrayLength(env, arr);
  samples = (*env)->GetShortArrayElements(env, arr, 0);


  cond = DCM_GetSequenceList((DCM_OBJECT**) &dcmObj,
			     DCM_MAKETAG(0x003a, 0x0002),
			     &waveformSequence);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);  /* repair  */
  }

  seqItem = LST_Head(&waveformSequence);
  if (seqItem == NULL) {
    fprintf(stderr, "Empty waveform sequence\n");
    exit(1);  /* repair */
  }

  if (len != channels * samplesPerChannel) {
    fprintf(stderr, "Len = %d, channels = %d, samplesPerChannel = %d\n",
	    len, channels, samplesPerChannel);
    exit(1);
  }

  e.length = len*2;
  e.d.ot = samples;
  DCM_GetElementValue((DCM_OBJECT**)&seqItem->object, &e, &rtnLength, &ctx);

  (*env)->ReleaseShortArrayElements(env, arr, samples, 0);
}


/* The functions below support the MISC module
*/

JNIEXPORT jlong JNICALL Java_DICOM_Misc_Timer_timeStruct
  (JNIEnv *env, jobject obj)
{
  void * timeStamp;

  timeStamp = UTL_GetTimeStamp();
  return (jlong)timeStamp;
}

JNIEXPORT jdouble JNICALL Java_DICOM_Misc_Timer_computeDelta
  (JNIEnv *env, jobject obj, jlong timeStamp)
{
  double delta;

  if (timeStamp == 0)
    return 0.;

  delta = UTL_DeltaTime((void*)timeStamp);
  return delta;
}

JNIEXPORT jdouble JNICALL Java_DICOM_Misc_Timer_releaseTimeStruct
  (JNIEnv *env, jobject obj, jlong timeStamp)
{
  if (timeStamp == 0)
    return;

  UTL_ReleaseTimeStamp((void*)timeStamp);
}

static DUL_ASSOCIATESERVICEPARAMETERS* mParams = 0;
static DUL_NETWORKKEY* mNetwork = 0;
static DUL_ASSOCIATIONKEY* mAssociation = 0;

JNIEXPORT jint JNICALL Java_DICOM_Network_Association_open
  (JNIEnv *env, jobject obj, jstring callingAE, jstring calledAE,
   jstring host, jstring port, jstring sopClass)
{
  CONDITION cond;

  const char *s1 = (*env)->GetStringUTFChars(env, callingAE, 0);
  const char *s2 = (*env)->GetStringUTFChars(env, calledAE, 0);
  const char *s3 = (*env)->GetStringUTFChars(env, host, 0);
  const char *s4 = (*env)->GetStringUTFChars(env, port, 0);
  const char *s5 = (*env)->GetStringUTFChars(env, sopClass, 0);

  cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			       NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN,
			       &mNetwork);
  if (cond != DUL_NORMAL)
    return -1;


  mParams = CTN_MALLOC(sizeof(*mParams));
  if (mParams == NULL)
    return -1;

  DUL_DefaultServiceParameters(mParams);
  sprintf(mParams->calledPresentationAddress, "%s:%s", s3, s4);
  strcpy(mParams->callingPresentationAddress, "");
  strcpy(mParams->calledAPTitle, s2);
  strcpy(mParams->callingAPTitle, s1);

  cond = SRV_RequestServiceClass(s5, DUL_SC_ROLE_SCU, mParams);
  if (cond != SRV_NORMAL) {
    DUL_DropNetwork(&mNetwork);
    mNetwork = 0;
    return -1;
  }

  cond = DUL_RequestAssociation(&mNetwork, mParams, &mAssociation);
  if (cond != DUL_NORMAL) {
    DUL_DropNetwork(&mNetwork);
    mNetwork = 0;
    return -1;
  }

  (*env)->ReleaseStringUTFChars(env, callingAE, s1);
  (*env)->ReleaseStringUTFChars(env, calledAE, s2);
  (*env)->ReleaseStringUTFChars(env, host, s3);
  (*env)->ReleaseStringUTFChars(env, port, s4);
  (*env)->ReleaseStringUTFChars(env, sopClass, s5);

  return 0;
}

JNIEXPORT jint JNICALL Java_DICOM_Network_Association_close
  (JNIEnv *env, jobject obj)
{
  return 0;
}

typedef struct {
  JNIEnv *env;
  jobject obj;
} QUERY_CALLBACK_STRUCT;

static CONDITION queryCallback(MSG_C_FIND_REQ* findRequest,
	MSG_C_FIND_RESP *findResponse,
	int count,
	char *SOPClass, char* queryLevel, void* x)
{
  QUERY_CALLBACK_STRUCT* s;
  jclass cls;

  s = (QUERY_CALLBACK_STRUCT*) x;

  if (findResponse->status ==  MSG_K_C_FIND_MATCHCONTINUING ||
      findResponse->status == MSG_K_C_FIND_MATCHCONTINUINGWARN) {
    jmethodID mid;
    jlong l;
    cls = (*s->env)->GetObjectClass(s->env, s->obj);
    mid = (*s->env)->GetMethodID(s->env, cls, "callbackCFind", "(J)I");
    if (mid == 0)
      return;


    l = 0;
    DCM_CopyObject(&findResponse->identifier, (DCM_OBJECT**)&l);
    /*memcpy(&l, &findResponse->identifier, sizeof(findResponse->identifier));*/
    (*s->env)->CallIntMethod(s->env, s->obj, mid, l);
    /*DCM_DumpElements(&findResponse->identifier, 1);*/
  }

  return SRV_NORMAL;
}

JNIEXPORT jlong JNICALL Java_DICOM_Query_CFindSCU_sendQuery
  (JNIEnv *env, jobject obj, jlong dcmObj)
{
  MSG_C_FIND_REQ request;
  MSG_C_FIND_RESP response;
  CONDITION cond;
  QUERY_CALLBACK_STRUCT s;

  s.env = env;
  s.obj = obj;

  memset(&request, 0, sizeof(request));
  memset(&response, 0, sizeof(response));

  request.type = MSG_K_C_FIND_REQ;
  request.messageID = SRV_MessageIDOut();
  request.dataSetType = DCM_CMDDATAIDENTIFIER;
  request.identifier = *((DCM_OBJECT**)&dcmObj);
  strcpy(request.classUID, DICOM_SOPPATIENTQUERY_FIND);
  /*DCM_DumpElements((DCM_OBJECT**)&dcmObj, 1);*/

  cond = SRV_CFindRequest(&mAssociation, mParams, &request, &response,
			  queryCallback, &s, 0);
  if (cond != SRV_NORMAL) {
    COND_DumpConditions();
    COND_PopCondition(TRUE);
  }
}
