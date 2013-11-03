/* Coded Value Interface */

package DICOM.SR;

import java.util.Vector;
import DICOM.DICOMWrapper;
import DICOM.Code.CodeItem;
import DICOM.SR.ContentItem;

public class ContentItemFactory {

  public ContentItemFactory() {
  }

  public DICOM.SR.ContentItem producePName (DICOM.Code.CodeItem codeItem,
					    String name) {
    DICOM.SR.ContentItem item =
	new DICOM.SR.ContentItem("PNAME", "HAS OBS CONTEXT");
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper();
    w.setString(0x0040a010, "HAS OBS CONTEXT");
    w.setString(0x0040a040, "PNAME");
    w.setString(0x0040a123, name);

    String s = codeItem.codeValue();
    w.setString(0x0040a043, 0x00080100, s, 1);
    w.setString(0x0040a043, 0x00080102, codeItem.codeDesignator(), 1);
    w.setString(0x0040a043, 0x00080104, codeItem.codeMeaning(), 1);
    item.setWrapper(w);

    return item;
  }
  public DICOM.SR.ContentItem produceContainer (DICOM.Code.CodeItem codeItem) {
    DICOM.SR.ContentItem item =
	new DICOM.SR.ContentItem("CONTAINER", "CONTAINS");
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper();
    w.setString(0x0040a010, "CONTAINS");
    w.setString(0x0040a040, "CONTAINER");

    String s = codeItem.codeValue();
    w.setString(0x0040a043, 0x00080100, s, 1);
    w.setString(0x0040a043, 0x00080102, codeItem.codeDesignator(), 1);
    w.setString(0x0040a043, 0x00080104, codeItem.codeMeaning(), 1);
    item.setWrapper(w);

    return item;
  }
  public DICOM.SR.ContentItem produceText (DICOM.Code.CodeItem codeItem,
					   String relationshipType,
					   String txt) {
    DICOM.SR.ContentItem item =
	new DICOM.SR.ContentItem("TEXT", relationshipType);
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper();
    w.setString(0x0040a010, relationshipType);
    w.setString(0x0040a040, "TEXT");
    w.setString(0x0040a160, txt);

    String s = codeItem.codeValue();
    w.setString(0x0040a043, 0x00080100, s, 1);
    w.setString(0x0040a043, 0x00080102, codeItem.codeDesignator(), 1);
    w.setString(0x0040a043, 0x00080104, codeItem.codeMeaning(), 1);
    item.setWrapper(w);

    return item;
  }
  public DICOM.SR.ContentItem produceImage (DICOM.Code.CodeItem codeItem,
					   String relationshipType) {
    DICOM.SR.ContentItem item =
	new DICOM.SR.ContentItem("IMAGE", relationshipType);
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper();
    w.setString(0x0040a010, relationshipType);
    w.setString(0x0040a040, "IMAGE");

    String s = codeItem.codeValue();
    w.setString(0x0040a043, 0x00080100, s, 1);
    w.setString(0x0040a043, 0x00080102, codeItem.codeDesignator(), 1);
    w.setString(0x0040a043, 0x00080104, codeItem.codeMeaning(), 1);
    item.setWrapper(w);

    return item;
  }
  public DICOM.SR.ContentItem produceCode (DICOM.Code.CodeItem codeItem,
					  String relationshipType,
					  DICOM.Code.CodeItem codedEntry) {
    DICOM.SR.ContentItem item =
	new DICOM.SR.ContentItem("CODE", relationshipType);
    DICOM.DICOMWrapper w = new DICOM.DICOMWrapper();
    w.setString(0x0040a010, relationshipType);
    w.setString(0x0040a040, "CODE");

    String s = codeItem.codeValue();
    w.setString(0x0040a043, 0x00080100, s, 1);
    w.setString(0x0040a043, 0x00080102, codeItem.codeDesignator(), 1);
    w.setString(0x0040a043, 0x00080104, codeItem.codeMeaning(), 1);

    w.setString(0x0040a168, 0x00080100, codedEntry.codeValue(), 1);
    w.setString(0x0040a168, 0x00080102, codedEntry.codeDesignator(), 1);
    w.setString(0x0040a168, 0x00080104, codedEntry.codeMeaning(), 1);
    item.setWrapper(w);

    return item;
  }
}
