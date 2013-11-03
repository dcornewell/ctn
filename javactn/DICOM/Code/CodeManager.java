package DICOM.Code;

import java.util.Vector;
import java.util.StringTokenizer;
import java.io.BufferedReader;
import java.io.FileReader;

import DICOM.Code.CodeItem;

public class CodeManager {

  public CodeManager( ) {
  }

  public java.util.Vector readCodeTable(String t) {
    java.util.Vector v = new java.util.Vector(1,1);

    String base = System.getProperty("codetable.base");
    String fileName = new String(base + "/" + t);

    try {
      BufferedReader in = new BufferedReader(new FileReader(fileName));
      String s;
      while ((s = in.readLine()) != null) {
	java.util.StringTokenizer tokenizer = new
		java.util.StringTokenizer(s, "\t\n", false);
	String codeValue = (String)tokenizer.nextElement();
	String codeDesignator = (String)tokenizer.nextElement();
	String codeMeaning = (String)tokenizer.nextElement();
	CodeItem item = new CodeItem(codeValue, codeMeaning, codeDesignator);
	v.addElement(item);
      }
    } catch (java.io.FileNotFoundException e) {
      System.out.println("Could not find file: " + fileName);
      return v;
    } catch (java.io.IOException i) {
      return v;
    }

    return v;
  }
  public DICOM.Code.CodeItem matchMeaning(String meaning, String tableName) {
    String base = System.getProperty("codetable.base");
    String fileName = new String(base + "/" + tableName);

    try {
      BufferedReader in = new BufferedReader(new FileReader(fileName));
      String s;
      while ((s = in.readLine()) != null) {
	java.util.StringTokenizer tokenizer = new
		java.util.StringTokenizer(s, "\t\n", false);
	String codeValue = (String)tokenizer.nextElement();
	String codeDesignator = (String)tokenizer.nextElement();
	String codeMeaning = (String)tokenizer.nextElement();
	if (meaning.equals(codeMeaning)) {
	  CodeItem item = new CodeItem(codeValue, codeMeaning, codeDesignator);
	  return item;
	}
      }
    } catch (java.io.FileNotFoundException e) {
      System.out.println("Could not find file: " + fileName);
      CodeItem item = new CodeItem("", "", "");
      return item;
    } catch (java.io.IOException i) {
      CodeItem item = new CodeItem("", "", "");
      return item;
      
    }

    CodeItem item = new CodeItem("", "", "");
    return item;
  }

}
