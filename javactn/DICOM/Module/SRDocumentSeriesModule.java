/* Patient Module Interface */

package DICOM.Module;

public class SRDocumentSeriesModule {

  private String mModality;
  private String mSeriesInstanceUID;
  private String mSeriesNumber;

  public SRDocumentSeriesModule(String modality, String seriesUID,
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
