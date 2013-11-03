package DICOM.DDR;

import DICOM.DICOMWrapper;
import DICOM.DDR.Patient;
import DICOM.DDR.Study;
import java.util.Vector;

/*
 * DIRInterface
 */


public class DIRInterface extends DICOM.DICOMWrapper {
  public native int getPatientCount(long dcmObj);
  public native void setPatientAttributes(long dcmObj, int index, DICOM.DDR.Patient p);
  public native int getStudyCount(long dcmObj, String patientID);
  public native void setStudyAttributes(long dcmObj, String patientID,
	int index, DICOM.DDR.Study study);
  public native int getSeriesCount(long dcmObj, String patientID,
	String studyInstanceUID);
  public native void setSeriesAttributes(long dcmObj, String patientID,
	String studyInstanceUID, int index, DICOM.DDR.Series series);
  public native int getLeafCount(long dcmObj, String patientID,
	String studyInstanceUID, String seriesInstanceUID);
  public native void setLeafAttributes(long dcmObj, String patientID,
	String studyInstanceUID, String seriesInstanceUID,
	int index, DICOM.DDR.SeriesLeaf leaf);
  /*
  static {
    System.loadLibrary("javactn");
  }
  */

  // private long fDCMObject;

  public DIRInterface(String fileName) {
    /*dcmFile = new  DICOMWrapper(fileName);*/
    fDCMObject = openFileJNI(fileName);
  }

  public int getPatientCount() {
    return getPatientCount(fDCMObject);
  }
  public DICOM.DDR.Patient getPatient(int index) {
    DICOM.DDR.Patient p = new DICOM.DDR.Patient();
    setPatientAttributes(fDCMObject, index, p);
    return p;
  }
  public Vector getPatientVector() {
    int count = getPatientCount(fDCMObject);
    Vector v = new Vector(count, 1);
    int index;
    for (index = 0; index < count; index++) {
      DICOM.DDR.Patient patient = new DICOM.DDR.Patient();
      setPatientAttributes(fDCMObject, index, patient);
      v.insertElementAt(patient, index);
    }
    return v;
  }

  public Vector getStudyVector(String patientID) {
    int count = getStudyCount(fDCMObject, patientID);
    Vector v = new Vector(count, 1);
    int index;
    for (index = 0; index < count; index++) {
      DICOM.DDR.Study study = new DICOM.DDR.Study();
      setStudyAttributes(fDCMObject, patientID, index, study);
      v.insertElementAt(study, index);
    }

    return v;
  }
  public Vector getSeriesVector(String patientID, String studyInstanceUID) {
    int count = getSeriesCount(fDCMObject, patientID, studyInstanceUID);
    Vector v = new Vector(count, 1);
    System.out.println("Series count = " + count);
    int index;
    for (index = 0; index < count; index++) {
      DICOM.DDR.Series series = new DICOM.DDR.Series();
      setSeriesAttributes(fDCMObject, patientID, studyInstanceUID, index, series);
      v.insertElementAt(series, index);
    }
    return v;
  }

  public Vector getLeafVector(String patientID, String studyInstanceUID,
				String seriesInstanceUID) {
    int count = getLeafCount(fDCMObject, patientID, studyInstanceUID,
				seriesInstanceUID);
    Vector v = new Vector(count, 1);
    int index;
    for (index = 0; index < count; index++) {
      DICOM.DDR.SeriesLeaf leaf = new DICOM.DDR.SeriesLeaf();
      setLeafAttributes(fDCMObject, patientID, studyInstanceUID,
			seriesInstanceUID, index, leaf);
      v.insertElementAt(leaf, index);
    }
    return v;
  }

}
