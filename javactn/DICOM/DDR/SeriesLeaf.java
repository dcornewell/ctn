/* DDR SeriesLeaf Interface */

package DICOM.DDR;

public class SeriesLeaf {

  private String RecordType;
  private String FileID;
  private String SOPClassUID;
  private String SOPInstanceUID;
  private String TransferSyntax;

  public SeriesLeaf() {
    RecordType = "";
    FileID = "";
    SOPClassUID = "";
    SOPInstanceUID = "";
    TransferSyntax = "";
  }

  public String getRecordType() {
    return RecordType;
  }
  public String getFileID() {
    return FileID;
  }
  public String getSOPClassUID() {
    return SOPClassUID;
  }
  public String getInstanceUID() {
    return SOPInstanceUID;
  }
  public String getTransferSyntax() {
    return TransferSyntax;
  }

  public void dumpState()  {
    System.out.print(RecordType + " : " + FileID + " : ");
    System.out.print(SOPClassUID + " : " + SOPInstanceUID+ " : ");
    System.out.println(TransferSyntax);
  }
}
