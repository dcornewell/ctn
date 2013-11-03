/* Patient Module Interface */

package DICOM.Module;

public class GeneralEquipmentModule {

  private String mManufacturer;

  public GeneralEquipmentModule(String manufacturer) {
    mManufacturer = new String(manufacturer);
  }

  public String manufacturer() {
    return mManufacturer;
  }
  public void dumpState()  {
  }
}
