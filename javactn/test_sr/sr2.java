import java.util.Vector;
import DICOM.SR.StructuredReport;

class sr2 {
  public static void main(String[] args) {
    DICOM.SR.StructuredReport rpt = new DICOM.SR.StructuredReport(args[0]);

    rpt.linearizeAttributes( );


  }
}
