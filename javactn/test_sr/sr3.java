import java.util.Vector;
import DICOM.*;

class sr3 {
  public static void main(String[] args) {
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper(args[0]);
    DICOM.DICOMWrapper rpt = new DICOM.DICOMWrapper();

    DICOM.InfoObj.CompositeFactory factory =
	new DICOM.InfoObj.CompositeFactory();
    factory.inflateStructuredReport(rpt, w);
    rpt.writeFile(args[1]);
  }
}
