/*
 * Association
 */

package DICOM.Network;

import java.util.Vector;

public class Association {
  public native int open(String callingAETitle,
			String calledAETitle,
			String hostName,
			String port,
			String SOPClass);
  public native int close();

  static {
    System.loadLibrary("javactn");
  }

  public Association ( ) {
  }

  public int openAssociation(String callingAETitle,
			String calledAETitle,
			String hostName,
			String port,
			String SOPClass) {

    int i = open(callingAETitle, calledAETitle, hostName, port, SOPClass);
    return i;
  }

  public int closeAssociation() {

    int i = close();
    return i;
  }
}
