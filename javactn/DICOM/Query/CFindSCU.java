/*
 * DICOMWrapper
 */

package DICOM.Query;

import java.util.Vector;
import DICOM.DICOMWrapper;
import DICOM.Network.Association;

public class CFindSCU {
  public native long newObjectJNI();
  public native long openFileJNI(String fileName);
  public native long sendQuery(long objReference);

  static {
    System.loadLibrary("javactn");
  }

  private java.util.Vector fResponseVector;

  public CFindSCU( ) {
  }

  public int callbackCFind(long objReference) {
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper(objReference);

    fResponseVector.addElement(w);

    return 0;
  }

  public int sendCFind(String callingAETitle,
			String calledAETitle,
			String hostName,
			String port,
			String SOPClass,
			DICOMWrapper cfind,
			java.util.Vector responseVector) {
    System.out.println(callingAETitle);
    DICOM.Network.Association assoc = new DICOM.Network.Association();
    fResponseVector = responseVector;

    String modSOPClass;

    if (SOPClass.equals("Patient"))
      modSOPClass = new String("1.2.840.10008.5.1.4.1.2.1.1");
    else if (SOPClass.equals("Study"))
      modSOPClass = new String("1.2.840.10008.5.1.4.1.2.2.1");
    else if (SOPClass.equals("MWL"))
      modSOPClass = new String("1.2.840.10008.5.1.4.31");
    else 
      modSOPClass = new String(SOPClass);

    int rslt = assoc.openAssociation(callingAETitle,
				     calledAETitle,
					hostName,
					port,
					modSOPClass);
    if (rslt != 0)
      return -1;

    long objReference = cfind.getObjectReference();

    sendQuery(objReference);

    assoc.closeAssociation();

    return 0;
  }
}
