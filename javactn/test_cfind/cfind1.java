import java.util.Vector;
import DICOM.DICOMWrapper;
import DICOM.Query.CFindSCU;

class cfind1 {
  public static void main(String[] args) {
    String callingAE = new String(args[0]);
    String calledAE = new String(args[1]);
    String host = new String(args[2]);
    String port = new String(args[3]);
    String sopClass = new String(args[4]);
    DICOM.DICOMWrapper query = new DICOM.DICOMWrapper(args[5]);
    java.util.Vector responseVector = new java.util.Vector(1,1);

    DICOM.Query.CFindSCU cfindSCU = new DICOM.Query.CFindSCU();
    cfindSCU.sendCFind(callingAE, calledAE, host, port, sopClass, 
	query, responseVector);
    System.out.println("Size of vector = " + responseVector.size());

    int i;
    for (i = 0; i < responseVector.size(); i++) {
      DICOM.DICOMWrapper w = (DICOM.DICOMWrapper)responseVector.elementAt(i);
      String n = w.getString((short)0x0010, (short)0x0010);
      System.out.println(" At top level: " + n);
    }
  }
}
