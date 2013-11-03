/* Patient Module Interface */

package DICOM.Module;

public class GeneralStudyModule {

  private String mStudyInstanceUID;
  private String mStudyDate;
  private String mStudyTime;
  private String mReferringPhysicianName;
  private String mStudyID;
  private String mAccessionNumber;
  private String mStudyDescription;
  private String mPhysicianOfRecord;

  public GeneralStudyModule(String studyUID, String studyDate, String studyTime,
			    String referringPhysician, String studyID,
			    String accessionNumber) {
    mStudyInstanceUID = new String(studyUID);
    mStudyDate = new String(studyDate);
    mStudyTime = new String(studyTime);
    mReferringPhysicianName = new String(referringPhysician);
    mStudyID = new String(studyID);
    mAccessionNumber = new String(accessionNumber);
  }

  public String studyUID() {
    return mStudyInstanceUID;
  }
  public String studyDate() {
    return mStudyDate;
  }
  public String studyTime() {
    return mStudyTime;
  }
  public String referringPhysician() {
    return mReferringPhysicianName;
  }
  public String studyID() {
    return mStudyID;
  }
  public String accessionNumber() {
    return mAccessionNumber;
  }
  public void dumpState()  {
  }
}
