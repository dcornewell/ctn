/* DDR Series Interface */

package DICOM.DDR;

public class Series {

  private String Modality;
  private String SeriesInstanceUID;
  private String SeriesNumber;

  public Series() {
    Modality = "";
    SeriesInstanceUID = "";
    SeriesNumber = "";
  }

  public String getModality() {
    return Modality;
  }
  public String getSeriesInstanceUID() {
    return SeriesInstanceUID;
  }
  public String getSeriesNumber() {
    return SeriesNumber;
  }

  public void dumpState()  {
    System.out.print(Modality + " : " + SeriesNumber+ " : ");
    System.out.println(SeriesInstanceUID);
  }
}
