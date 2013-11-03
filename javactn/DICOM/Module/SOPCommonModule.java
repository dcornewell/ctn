/* Patient Module Interface */

package DICOM.Module;

public class SOPCommonModule {

  private String mSOPClassUID;
  private String mSOPInstanceUID;

  public SOPCommonModule(String classUID, String instanceUID) {
    mSOPClassUID = new String(classUID);
    mSOPInstanceUID = new String(instanceUID);
  }

  public String sopClassUID() {
    return mSOPClassUID;
  }
  public String sopInstanceUID() {
    return mSOPInstanceUID;
  }
  public void dumpState()  {
  }
}
