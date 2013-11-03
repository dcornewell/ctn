package DICOM.Code;


public class CodeItem {

  private String mCodeValue;
  private String mCodeMeaning;
  private String mCodeDesignator;


  public CodeItem(String value, String meaning, String designator ) {
    mCodeValue = new String(value);
    mCodeMeaning = new String(meaning);
    mCodeDesignator = new String(designator);
  }
  public CodeItem(CodeItem item) {
    mCodeValue = new String(item.mCodeValue);
    mCodeMeaning = new String(item.mCodeMeaning);
    mCodeDesignator = new String(item.mCodeDesignator);
  }

  public String codeValue() {
    return mCodeValue;
  }
  public String codeMeaning() {
    return mCodeMeaning;
  }
  public String codeDesignator() {
    return mCodeDesignator;
  }
}
