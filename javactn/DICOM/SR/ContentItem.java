/* Coded Value Interface */

package DICOM.SR;

import java.util.Vector;
import DICOM.DICOMWrapper;

public class ContentItem{

  private String ValueType;
  private String RelationshipType;
  private boolean fIsLeaf;
  private int NestLevel;
  private Vector ContentItemVector;
  //private long fDCMObject;
  private DICOM.DICOMWrapper mDICOMWrapper;

  public ContentItem(String valueType, String relationshipType) {
    ValueType = new String(valueType);
    RelationshipType = new String(relationshipType);
    fIsLeaf = true;
    NestLevel = 1;
    //fDCMObject = 0;
  }

  public String getValueType() {
    return ValueType;
  }
  public String getRelationshipType() {
    return RelationshipType;
  }
  public boolean isLeaf() {
    return fIsLeaf;
  }
  public int nestLevel( ) {
    return NestLevel;
  }
  public Vector contentItemVector( ) {
    return ContentItemVector;
  }
  public String getString(int tag) {
    return mDICOMWrapper.getString(tag);
  }
  public String getString(int seqTag, int tag, int item) {
    return mDICOMWrapper.getString(seqTag, tag, item);
  }
  public DICOM.DICOMWrapper wrapper() {
    return mDICOMWrapper;
  }

  public void valueType(String s) {
    ValueType = new String(s);
  }
  public void relationshipType(String s) {
    RelationshipType = new String(s);
  }
  public void addVectorItem(ContentItem item, int index) {
    if (fIsLeaf) {
      fIsLeaf = false;
      ContentItemVector = new Vector(1, 1);
      mDICOMWrapper.addSequence(0x0040a730);
    }
    ContentItemVector.insertElementAt(item, index);
    mDICOMWrapper.addToSequence(0x0040a730, index, item.wrapper());
  }
  public void addVectorItem(ContentItem item) {
    if (fIsLeaf) {
      fIsLeaf = false;
      ContentItemVector = new Vector(1, 1);
      mDICOMWrapper.addSequence(0x0040a730);
    }
    ContentItemVector.addElement(item);
    mDICOMWrapper.addToSequence(0x0040a730, 0, item.wrapper());
  }
  public void nestLevel(int i) {
    NestLevel = i;
  }
  public void objectReference(long objRef) {
    //fDCMObject = objRef;
    mDICOMWrapper = new DICOM.DICOMWrapper(objRef);
  }
  public void setWrapper(DICOM.DICOMWrapper w) {
    //fDCMObject = objRef;
    mDICOMWrapper = w;
  }

  public void dumpState()  {
    int i;
    for (i = 0; i < NestLevel; i++)
      System.out.print(" ");

    System.out.println(NestLevel + " : " + ValueType + " : " + RelationshipType);
    if (!fIsLeaf) {
      int count = ContentItemVector.size();
      for (i = 0; i < count; i++) {
        ContentItem item = (ContentItem)ContentItemVector.elementAt(i);
        item.dumpState();
      }
    }
  }
}
