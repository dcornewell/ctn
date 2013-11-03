#ifdef _MSC_VER
void formatPatient(void *patient, int index, char *buf);
void formatStudy(void *study, int index, char *buf);
#else
void formatPatient(PATIENT_QUERY_MODULE * patient, int index, char *buf);
void formatStudy(STUDY_QUERY_MODULE * study, int index, char *buf);
#endif
