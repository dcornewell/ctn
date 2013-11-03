/*
 * DICOMWrapper
 */

package DICOM;

import java.util.Vector;

public class DICOMWrapper {
  public native long newObjectJNI();
  public native long openFileJNI(String fileName);
  public native void closeFileJNI(long dcmObj);
  public native int writeFileJNI(long dcmObj, String fileName);
  public native long getLongJNI(long dcmObj, short group, short element);
  public native int getIntJNI(long dcmObj, short group, short element);
  public native float getFloatJNI(long dcmObj, short group, short element);
  public native String getStringJNI(long dcmObj, short group, short element);
  public native String getStringJNI(long dcmObj, int tag);
  public native String getStringJNI(long dcmObj, int seqTag, int tag, int item);
  public native int getSequenceLengthJNI(long dcmObj, short group, short element);
  public native long getSequenceObjJNI(long dcmObj, short group, short element,
				    int index);
  public native int attributePresentJNI(long dcmObj, int tag);
  public native int countTagsJNI(long dcmObj);
  public native int fillTagsJNI(long dcmObj, int tags[]);

  public native int setStringJNI(long dcmObj, int tag, String s);
  public native int setStringSeqJNI(long dcmObj, int seqTag, int tag, String s,
				 int index);
  public native int addSequenceJNI(long dcmObj, int seqTag);
  public native int addToSequenceJNI(long dcmObj, int seqTag, int index,
				     long newObj);
  public native String getVRJNI(long dcmObj, int tag);
  public native String getDescrJNI(long dcmObj, int tag);
  public native int getLengthJNI(long dcmObj, int tag);

  static {
    System.loadLibrary("javactn");
  }

  protected long fDCMObject;
  private int fOwnObject;

  public DICOMWrapper( ) {
    fDCMObject = newObjectJNI( );
    fOwnObject = 1;
  }

  public DICOMWrapper(String fileName) {
    fDCMObject = openFileJNI(fileName);
    fOwnObject = 1;
  }

  public DICOMWrapper(long objReference) {
    fDCMObject = objReference;
    fOwnObject = 0;
  }

  public void close() {
    if (fOwnObject == 1) {
      closeFileJNI(fDCMObject);
      fDCMObject = 0;
    }
    fOwnObject = 0;
  }

  public int writeFile(String fileName) {
    return writeFileJNI(fDCMObject, fileName);
  }

  public String getString(short group, short element) {
    return getStringJNI(fDCMObject, group, element);
  }
  public String getString(int tag) {
    return getStringJNI(fDCMObject, tag);
  }
  public String getString(int seqTag, int tag, int item) {
    return getStringJNI(fDCMObject, seqTag, tag, item);
  }
  public int getInt(short group, short element) {
   return getIntJNI(fDCMObject, group, element);
  }
  public Vector getSequenceVector(short group, short element) {
    Vector v = new Vector(3, 1);
    return v;
  }
  public int attributePresent(int tag) {
    return attributePresentJNI(fDCMObject, tag);
  }

  public int [] linearizeAttributes( ) {
    int tagCount = countTagsJNI(fDCMObject);
    //System.out.println("Tag count = " + tagCount);
    int tags[] = new int[tagCount];
    fillTagsJNI(fDCMObject, tags);
    int i;
    //for (i = 0; i < tagCount; i++) {
    //  Integer ix = new Integer(tags[i]);
    //  String s = ix.toString(tags[i], 16);
    //  System.out.println(" " + s);
    //}
    return tags;
  }
  public int setString(int tag, String s) {
    return setStringJNI(fDCMObject, tag, s);
  }
  public int setString(int seqTag, int tag, String s, int index) {
    return setStringSeqJNI(fDCMObject, seqTag, tag, s, index);
  }
  public int addSequence(int seqTag) {
    return addSequenceJNI(fDCMObject, seqTag);
  }
  public int addToSequence(int seqTag, int index, DICOMWrapper w) {
    return addToSequenceJNI(fDCMObject, seqTag, index, w.fDCMObject);
  }

  public long getObjectReference( ) {
    return fDCMObject;
  }

  public String getVR(int tag) {
    return getVRJNI(fDCMObject, tag);
  }

  public String getDescription(int tag) {
    return getDescrJNI(fDCMObject, tag);
  }

  public int getLength(int tag) {
    return getLengthJNI(fDCMObject, tag);
  }

}
