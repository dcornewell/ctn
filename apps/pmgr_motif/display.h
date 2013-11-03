#ifndef _DISPLAY_H_
#define  _DISPLAY_H_
typedef struct {
    void
       *reserved[2];
    char
        PatientName[DICOM_PN_LENGTH + 1],
        AccessionNumber[DICOM_IS_LENGTH + 1];
    char
        combo[DICOM_PN_LENGTH + DICOM_IS_LENGTH + 6];
}   LIST_ITEM;

#endif
