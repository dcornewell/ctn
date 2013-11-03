package DICOM.SR;

import DICOM.DICOMWrapper;
import DICOM.SR.ContentItem;
import java.util.Vector;

/*
 * Structured Report
 */


public class StructuredReport extends DICOM.DICOMWrapper {
  public native void setPatientAttributes(long dcmObj, DICOM.Module.PatientModule p);
  // private long fDCMObject;
  private Vector ContentItemVector;
  private DICOM.SR.ContentItem mContentItem;

  public StructuredReport() {
    super();
  }
  public StructuredReport(String fileName) {
    super(fileName);
  }

  public String getPatientName() {
    return getStringJNI(fDCMObject, (short)0x0010, (short)0x0010);
  }
  public DICOM.Module.PatientModule getPatient() {
    DICOM.Module.PatientModule patient = new DICOM.Module.PatientModule();
    setPatientAttributes(fDCMObject, patient);
    return patient;
  }

  public int expandSRTree( ) {
    int contentSequenceLength;

    contentSequenceLength = getSequenceLengthJNI(fDCMObject, (short)0x0040,
						 (short)0xa730);
    ContentItemVector = new Vector (contentSequenceLength, 1);

    readContentVector(1, fDCMObject, ContentItemVector);

    return 0;
  }

  public int expand( ) {
    String s = getStringJNI(fDCMObject, (short)0x0040, (short)0xa040);
    if (s.compareTo("CONTAINER") != 0) {
      System.out.println("<" + s + ">");
      return 1;
    }

    if (attributePresent(0x0040a010) == 1) {
      System.out.println("While expanding SR root, found attribute 0x0040 a010");
      return 1;
    }
    mContentItem = new DICOM.SR.ContentItem("CONTAINER", "CONTAINS");
    mContentItem.objectReference(fDCMObject);
    mContentItem.nestLevel(0);
    if (attributePresent(0x0040a730) == 1) {
        readContentVector(1, fDCMObject, mContentItem);
    }

    return 0;
  }

  public Vector contentItemVector( ) {
    return ContentItemVector;
  }
  public DICOM.SR.ContentItem contentItem( ) {
    return mContentItem;
  }

  private int readContentVector(int nestLevel, long dcmObj, Vector v) {
    int contentSequenceLength;

    contentSequenceLength = getSequenceLengthJNI(fDCMObject, (short)0x0040,
					         (short)0xa730);
    int index;
    for (index = 0; index < contentSequenceLength; index++) {

      long objRef = getSequenceObjJNI(dcmObj,
				      (short)0x0040, (short)0xa730, index);
      DICOM.DICOMWrapper f = new DICOM.DICOMWrapper(objRef);
      String relationshipType = f.getString((short)0x0040, (short)0xa010);
      String valueType = f.getString((short)0x0040, (short)0xa040);

      DICOM.SR.ContentItem item = new DICOM.SR.ContentItem(valueType,
							   relationshipType);
      item.nestLevel(nestLevel);
      item.objectReference(objRef);
      if (f.attributePresent(0x0040a730) == 1) {
        readContentVector(nestLevel+1, objRef, item);
      }
      v.insertElementAt(item, index);
    }
    return 0;
  }
  private int readContentVector(int nestLevel, long dcmObj,
				DICOM.SR.ContentItem contentItem) {
    int contentSequenceLength;


    contentSequenceLength = getSequenceLengthJNI(dcmObj, (short)0x0040,
						 (short)0xa730);
    int index;
    for (index = 0; index < contentSequenceLength; index++) {

      long objRef = getSequenceObjJNI(dcmObj,
				      (short)0x0040, (short)0xa730, index);
      DICOM.DICOMWrapper f = new DICOM.DICOMWrapper(objRef);
      String relationshipType = f.getString((short)0x0040, (short)0xa010);
      String valueType = f.getString((short)0x0040, (short)0xa040);

      DICOM.SR.ContentItem item = new DICOM.SR.ContentItem(valueType,
							   relationshipType);
      item.objectReference(objRef);
      item.nestLevel(nestLevel);
      if (f.attributePresent(0x0040a730) == 1) {
        readContentVector(nestLevel+1, objRef, item);
      }
      contentItem.addVectorItem(item, index);
    }
    return 0;
  }
}
