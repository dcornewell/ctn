/* Patient Module Interface */

package DICOM.Module;

public class SRDocumentGeneralModule {

  private String mInstanceNumber;
  private String mCompletionFlag;
  private String mVerificationFlag;
  private String mContentDate;
  private String mContentTime;
  private String mVerifyingObserverName;
  private String mVerifyingObserverCode;
  private String mVerifyingObserverDesignator;
  private String mVerifyingObserverMeaning;
  private String mVerifyingOrganization;
  private String mVerificationDateTime;

  public SRDocumentGeneralModule(String instanceNumber,
	String completionFlag,		String verificationFlag,
	String contentDate,		String contentTime,
	String verifyingObserverName,	String verifyingObserverCode,
	String verifyingObserverDesignator,
	String verifyingObserverMeaning,
	String verifyingOrganization,	String verificationDateTime) {

    mInstanceNumber		= new String(instanceNumber);
    mCompletionFlag		= new String(completionFlag);
    mVerificationFlag		= new String(verificationFlag);
    mContentDate		= new String(contentDate);
    mContentTime		= new String(contentTime);
    mVerifyingObserverName	= new String(verifyingObserverName);
    mVerifyingObserverCode	= new String(verifyingObserverCode);
    mVerifyingObserverDesignator= new String(verifyingObserverDesignator);
    mVerifyingObserverMeaning	= new String(verifyingObserverMeaning);
    mVerifyingOrganization	= new String(verifyingOrganization);
    mVerificationDateTime	= new String(verificationDateTime);
  }

  public String instanceNumber() {
    return mInstanceNumber;
  }
  public String completionFlag() {
    return mCompletionFlag;
  }
  public String verificationFlag() {
    return mVerificationFlag;
  }
  public String contentDate() {
    return mContentDate;
  }
  public String contentTime( ) {
    return mContentTime;
  }
  public String verifyingObserverName() {
    return mVerifyingObserverName;
  }
  public String verifyingObserverCode( ) {
    return mVerifyingObserverCode;
  }
  public String verifyingObserverDesignator() {
    return mVerifyingObserverDesignator;
  }
  public String verifyingObserverMeaning() {
    return mVerifyingObserverMeaning;
  }
  public String verifyingOrganization() {
    return mVerifyingOrganization;
  }
  public String verificationDateTime() {
    return mVerificationDateTime;
  }
  public void dumpState()  {
  }
}
