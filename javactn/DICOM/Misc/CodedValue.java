/* Coded Value Interface */

package DICOM.Misc;

public class CodedValue {

  private String CodeValue;
  private String CodingSchemeDesignator;
  private String CodingSchemeVersion;
  private String CodeMeaning;
  private String MappingResource;
  private String ContextGroupVersion;
  private String ContextIdentifier;

  public CodedValue() {

  }

  public String getCodeValue() {
    return CodeValue;
  }
  public String getCodingSchemeDesignator() {
    return CodingSchemeDesignator;
  }
  public String getCodingSchemeVersion() {
    return CodingSchemeVersion;
  }
  public String getCodeMeaning () {
    return CodeMeaning;
  }
  public String getMappingResource () {
    return MappingResource;
  }
  public String getContextGroupVersion () {
    return ContextGroupVersion;
  }
  public String getContextIdentifier () {
    return ContextIdentifier;
  }
  public void dumpState()  {
    System.out.println("           Code Value: " + CodeValue);
    System.out.println("  Coding Scheme Desig: " + CodingSchemeDesignator);
    System.out.println("Coding Scheme Version: " + CodingSchemeVersion);
    System.out.println("         Code Meaning: " + CodeMeaning);
    System.out.println("     Mapping Resource: " + MappingResource);
    System.out.println("Context Group Version: " + ContextGroupVersion);
    System.out.println("   Context Identifier: " + ContextIdentifier);
  }
}
