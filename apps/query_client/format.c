#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GCCSUNOS
#include <stddef.h>
#endif

#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "query_interface.h"

void
formatPatient(PATIENT_QUERY_MODULE * patient, int index, char *buf)
{
    (void) sprintf(buf, "%-16s %-30s %-12s", patient->PatientID,
		   patient->PatientName, patient->BirthDate);
}
void
formatStudy(STUDY_QUERY_MODULE * study, int index, char *buf)
{
    int
        i;

    (void) sprintf(buf, "%-16s %-16s %-10s", study->AccessionNumber,
		   study->StudyID, study->StudyDate);

    strcat(buf, "   ");
    for (i = 0; i < (int) DIM_OF(study->SeriesModalities); i++) {
	if (strlen(study->SeriesModalities[i]) == 0)
	    break;
	strcat(buf, " ");
	strcat(buf, study->SeriesModalities[i]);
    }
}
