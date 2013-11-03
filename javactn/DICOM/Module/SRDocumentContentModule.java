
package DICOM.Module;

import java.util.Vector;
import DICOM.Code.CodeItem;
import DICOM.SR.ContentItem;

public class SRDocumentContentModule {

  private java.util.Vector mContentItemVector;
  private DICOM.Code.CodeItem mDocumentTitle;

  public SRDocumentContentModule(DICOM.Code.CodeItem documentTitle) {
    mContentItemVector		= new java.util.Vector(1,1);
    mDocumentTitle		= new DICOM.Code.CodeItem(documentTitle);
  }

  public void addContentItem(DICOM.SR.ContentItem item) {
    mContentItemVector.addElement(item);
  }

  public DICOM.Code.CodeItem documentTitle() {
    return mDocumentTitle;
  }
  public java.util.Vector contentItemVector( ) {
    return mContentItemVector;
  }

  public void dumpState()  {
  }
}
