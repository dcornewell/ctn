package DICOM.InfoObj;

import DICOM.DICOMWrapper;
import DICOM.Module.*;

/*
 * CompositeObject
 */


public class CompositeObject extends DICOM.DICOMWrapper {

  public void setPatientModule(DICOM.Module.PatientModule patient) {
    String s = patient.patientName();
    setString(0x00100010, s);
    s = patient.patientID();
    setString(0x00100020, s);
    s = patient.birthDate();
    setString(0x00100030, s);
    s = patient.sex();
    setString(0x00100040, s);
  }
}
