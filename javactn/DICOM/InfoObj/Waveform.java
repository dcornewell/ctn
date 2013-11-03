package DICOM.InfoObj;

import DICOM.DICOMWrapper;
import DICOM.Module.PatientModule;
import DICOM.Module.WaveformModule;

/*
 * Waveform
 */


public class Waveform extends DICOM.DICOMWrapper {
  public native void setPatientAttributes(long dcmObj, DICOM.Module.PatientModule p);
  public native void setWaveformModuleAttributes(long dcmObj,
		DICOM.Module.WaveformModule w);

  public Waveform(String fileName) {
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
  public DICOM.Module.WaveformModule getWaveformModule() {
    DICOM.Module.WaveformModule w = new DICOM.Module.WaveformModule();
    setWaveformModuleAttributes(fDCMObject, w);
    return w;
  }
}
