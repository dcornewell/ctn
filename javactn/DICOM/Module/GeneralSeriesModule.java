/* Patient Module Interface */

package DICOM.Module;

public class GeneralSeriesModule {

  private String mModality;
  private String mSeriesInstanceUID;
  private String mSeriesNumber;

  public GeneralSeriesModule(String modality, String seriesUID,
			    String seriesNumber) {
    mModality = new String(modality);
    mSeriesInstanceUID = new String(seriesUID);
    mSeriesNumber = new String(seriesNumber);
  }

  public String modality() {
    return mModality;
  }
  public String seriesUID() {
    return mSeriesInstanceUID;
  }
  public String seriesNumber() {
    return mSeriesNumber;
  }
  public void dumpState()  {
  }
}
