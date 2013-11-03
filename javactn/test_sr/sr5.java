import java.util.Vector;
import DICOM.*;

class sr5 {
  public static void main(String[] args) {
    DICOM.Code.CodeManager mgr = new DICOM.Code.CodeManager();
    java.util.Vector v = mgr.readCodeTable("sr/doc_title.txt");

    DICOM.InfoObj.CompositeFactory factory = new
	DICOM.InfoObj.CompositeFactory();
    DICOM.SR.StructuredReport rpt = new
	DICOM.SR.StructuredReport();
    DICOM.Code.CodeItem item = new DICOM.Code.CodeItem("a", "bc", "d");
    DICOM.Module.SRDocumentContentModule m = new
	DICOM.Module.SRDocumentContentModule(item);

    DICOM.SR.ContentItemFactory contentFactory = new
	DICOM.SR.ContentItemFactory();
    DICOM.Code.CodeItem codeItem = new
	DICOM.Code.CodeItem("A100", "Recording Observer", "MESA");
    DICOM.SR.ContentItem contentItem =
	contentFactory.producePName(codeItem, "MOORE");
    //factory.setSRDocumentContentModule(rpt, m);
    rpt.writeFile("zz.dcm");
  }
}
