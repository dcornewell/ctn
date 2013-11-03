import java.util.Vector;
import DICOM.SR.StructuredReport;

class sr1 {
  public static void main(String[] args) {
    DICOM.SR.StructuredReport rpt = new DICOM.SR.StructuredReport(args[0]);

    String s = rpt.getString((short)0x0010, (short)0x0010);
    System.out.println("Patient Name: " + s);

/*
    rpt.expandSRTree();

    Vector v = rpt.contentItemVector();
    int count = v.size();
    int i;
    for (i = 0; i < count; i++) {   
      DICOM.SR.ContentItem contentItem = (DICOM.SR.ContentItem)v.elementAt(i);
      contentItem.dumpState();
    }
*/
    int x = rpt.expand();
    System.out.println(" " + x);
    DICOM.SR.ContentItem rootItem = rpt.contentItem();
    rootItem.dumpState();
  }
}
