/* Patient Module Interface */

package DICOM.Module;

public class PatientModule {

  private String mPatientName;
  private String mPatientID;
  private String mBirthDate;
  private String mSex;

  public PatientModule() {
    mPatientName = "";
    mPatientID = "";
    mBirthDate = "";
    mSex = "";
  }

  public PatientModule(String name, String id, String birthDate, String sex) {
    mPatientName = new String(name);
    mPatientID = new String(id);
    mBirthDate = new String(birthDate);
    mSex = new String(sex);
  }

  public String patientName() {
    return mPatientName;
  }
  public String patientID() {
    return mPatientID;
  }
  public String birthDate() {
    return mBirthDate;
  }
  public String sex() {
    return mSex;
  }
  public void patientName(String s) {
    mPatientName = new String(s);
  }
  public void patientID(String s) {
    mPatientID = new String(s);
  }
  public void birthDate(String s) {
    mBirthDate = new String(s);
  }
  public void sex(String s) {
    mSex = new String(s);
  }
  public void dumpState()  {
    System.out.print(mPatientName + " : " + mPatientID + " : " + mBirthDate +
			" : " + mSex);
    System.out.println();
  }
}
