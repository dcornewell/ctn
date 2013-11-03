import java.util.Vector;
import DICOM.*;

class sr4 {
  public static void main(String[] args) {
    DICOM.Code.CodeManager mgr = new DICOM.Code.CodeManager();
    java.util.Vector v = mgr.readCodeTable("sr/doc_title.txt");

    int i;
    for (i = 0; i < v.size(); i++) {
      DICOM.Code.CodeItem item = (DICOM.Code.CodeItem)v.elementAt(i);
      System.out.println(item.codeValue());
    }
  }
}
