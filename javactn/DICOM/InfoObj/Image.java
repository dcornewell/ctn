package DICOM.InfoObj;

import DICOM.DICOMWrapper;
import DICOM.Module.PatientModule;
import DICOM.Module.ImagePixelModule;

/*
 * Image
 */


public class Image extends DICOM.DICOMWrapper {
  public native void setPatientAttributes(long dcmObj, DICOM.Module.PatientModule p);
  public native void setImgAttributes(long dcmObj, DICOM.Module.ImagePixelModule img);
  // private long fDCMObject;

  public Image(String fileName) {
    fDCMObject = openFileJNI(fileName);
  }

  public String getPatientName() {
    return getStringJNI(fDCMObject, (short)0x0010, (short)0x0010);
  }
  public DICOM.Module.PatientModule getPatient() {
    DICOM.Module.PatientModule patient = new DICOM.Module.PatientModule();
    setPatientAttributes(fDCMObject, patient);
    return patient;
  }
  public DICOM.Module.ImagePixelModule getImg() {
    DICOM.Module.ImagePixelModule img = new DICOM.Module.ImagePixelModule();
    setImgAttributes(fDCMObject, img);
    return img;
  }
}
