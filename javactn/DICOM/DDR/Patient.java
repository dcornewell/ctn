/* DDR Patient Interface */

package DICOM.DDR;

public class Patient {

  private String PatientName;
  private String PatientID;
  private String BirthDate;
  private String Sex;

  public Patient() {
    PatientName = "";
    PatientID = "";
    BirthDate = "";
    Sex = "";
  }


  public String getPatientName() {
    return PatientName;
  }
  public String getPatientID() {
    return PatientID;
  }
  public String getBirthDate() {
    return BirthDate;
  }
  public String getSex() {
    return Sex;
  }
  public void dumpState()  {
    System.out.print(PatientName + " : " + PatientID + " : " + BirthDate +
			" : " + Sex);
    System.out.println();
  }
}
