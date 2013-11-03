/* DDR Study Interface */

package DICOM.DDR;

public class Study {

  private String StudyDate;
  private String StudyTime;
  private String StudyDescription;
  private String StudyInstanceUID;
  private String StudyID;
  private String AccessionNumber;

  public Study() {
    StudyDate = "";
    StudyTime = "";
    StudyDescription = "";
    StudyInstanceUID = "";
    StudyID = "";
    AccessionNumber = "";
  }

  public String getStudyDate() {
    return StudyDate;
  }
  public String getStudyTime() {
    return StudyTime;
  }
  public String getStudyDescription() {
    return StudyDescription;
  }
  public String getStudyInstanceUID() {
    return StudyInstanceUID;
  }
  public String getStudyID() {
    return StudyID;
  }
  public String getAccessionNumber() {
    return AccessionNumber;
  }
  public String AccessionNumber() {
    return AccessionNumber;
  }
  public void dumpState()  {
    System.out.print(StudyDate + " : " + StudyTime + " : ");
    System.out.print(StudyID + " : " + AccessionNumber + " : ");
    System.out.println(StudyInstanceUID + " : " + StudyDescription);
  }
}
