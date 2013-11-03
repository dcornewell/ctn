package DICOM.InfoObj;

import DICOM.DICOMWrapper;
import DICOM.Module.*;

/*
 * CompositeFactory
 */


public class CompositeFactory {

  public void setPatientModule(DICOM.DICOMWrapper wrapper,
			       DICOM.Module.PatientModule patient) {
    String s = patient.patientName();
    wrapper.setString(0x00100010, s);
    s = patient.patientID();
    wrapper.setString(0x00100020, s);
    s = patient.birthDate();
    wrapper.setString(0x00100030, s);
    s = patient.sex();
    wrapper.setString(0x00100040, s);
  }

  public void setGeneralStudyModule(DICOM.DICOMWrapper w,
			       DICOM.Module.GeneralStudyModule study) {
    String s = study.studyUID();
    w.setString(0x0020000D, s);
    s = study.studyDate();
    w.setString(0x00080020, s);
    s = study.studyTime();
    w.setString(0x00080030, s);
    s = study.referringPhysician();
    w.setString(0x00080090, s);
    s = study.studyID();
    w.setString(0x00200010, s);
    s = study.accessionNumber();
    w.setString(0x00080050, s);
  }

  public void setGeneralSeriesModule(DICOM.DICOMWrapper w,
			       DICOM.Module.GeneralSeriesModule series) {
    String s = series.modality();
    w.setString(0x00080060, s);
    s = series.seriesUID();
    w.setString(0x0020000E, s);
    s = series.seriesNumber();
    w.setString(0x00200011, s);
  }

  public void setGeneralEquipmentModule(DICOM.DICOMWrapper w,
					DICOM.Module.GeneralEquipmentModule eqpt) {
    String s = eqpt.manufacturer();
    w.setString(0x00080070, s);
  }

  public void setSOPCommonModule(DICOM.DICOMWrapper w,
				 DICOM.Module.SOPCommonModule c) {
    String s = c.sopClassUID();
    w.setString(0x00080016, s);
    s = c.sopInstanceUID();
    w.setString(0x00080018, s);
  }
  public void setSRDocumentSeriesModule(DICOM.DICOMWrapper w,
					DICOM.Module.GeneralSeriesModule series) {
    String s = series.modality();
    w.setString(0x00080060, s);
    s = series.seriesUID();
    w.setString(0x0020000E, s);
    s = series.seriesNumber();
    w.setString(0x00200011, s);
  }
  public void setSRDocumentSeriesModule(DICOM.DICOMWrapper w,
					DICOM.Module.SRDocumentSeriesModule series) {
    String s = series.modality();
    w.setString(0x00080060, s);
    s = series.seriesUID();
    w.setString(0x0020000E, s);
    s = series.seriesNumber();
    w.setString(0x00200011, s);
  }

  public void setSRDocumentGeneralModule(DICOM.SR.StructuredReport w,
					 DICOM.Module.SRDocumentGeneralModule m) {
    String s = m.instanceNumber();
    w.setString(0x00200013, s);
    s = m.completionFlag();
    w.setString(0x0040a491, s);
    s = m.verificationFlag();
    w.setString(0x0040a493, s);
    s = m.contentDate();
    w.setString(0x00080023, s);
    s = m.contentTime();
    w.setString(0x00080033, s);

    s = m.verificationFlag();
    if (s.equals("VERIFIED")) {
      w.addSequence(0x0040a073);
      DICOM.DICOMWrapper x = new DICOM.DICOMWrapper();
      s = m.verifyingObserverName();
      x.setString(0x0040a075, s);
      s = m.verifyingOrganization();
      x.setString(0x0040a027, s);
      s = m.verificationDateTime();
      x.setString(0x0040a030, s);
      s = m.verifyingObserverCode();
      x.setString(0x0040a088, 0x00080100, s, 1);
      s = m.verifyingObserverDesignator();
      x.setString(0x0040a088, 0x00080102, s, 1);
      s = m.verifyingObserverMeaning();
      x.setString(0x0040a088, 0x00080104, s, 1);

      w.addToSequence(0x0040a073, 0, x);
    }
  }

  public void setSRDocumentContentModule(DICOM.SR.StructuredReport w,
					 DICOM.Module.SRDocumentContentModule m) {
    w.setString(0x0040a040, "CONTAINER");

    DICOM.Code.CodeItem docTitle = m.documentTitle();

    w.setString(0x0040a043, 0x00080100, docTitle.codeValue(), 1);
    w.setString(0x0040a043, 0x00080102, docTitle.codeDesignator(), 1);
    w.setString(0x0040a043, 0x00080104, docTitle.codeMeaning(), 1);

    w.addSequence(0x0040a730);
    java.util.Vector v = m.contentItemVector();
    int i = 0;
    for (i = 0; i < v.size(); i++) {
      DICOM.SR.ContentItem item = (DICOM.SR.ContentItem)v.elementAt(i);
      w.addToSequence(0x0040a730, 0, item.wrapper());
    }
  }

  public DICOM.Module.PatientModule
    getPatientModule(DICOM.DICOMWrapper w) {
    String name = w.getString(0x00100010);
    String id = w.getString(0x00100020);
    String dob = w.getString(0x00100030);
    String sex = w.getString(0x00100040);

    DICOM.Module.PatientModule m = new
	DICOM.Module.PatientModule(name, id, dob, sex);
    return m;
  }

  public DICOM.Module.GeneralStudyModule
    getGeneralStudyModule(DICOM.DICOMWrapper w) {
    String studyUID           = w.getString(0x0020000D);
    String studyDate          = w.getString(0x00080020);
    String studyTime          = w.getString(0x00080030);
    String referringPhysician = w.getString(0x00080090);
    String studyID            = w.getString(0x00200010);
    String accessionNumber    = w.getString(0x00080050);

    DICOM.Module.GeneralStudyModule m = new DICOM.Module.GeneralStudyModule(
	studyUID, studyDate, studyTime, referringPhysician,
	studyID, accessionNumber);
    return m;
  }

  public DICOM.Module.GeneralEquipmentModule
    getGeneralEquipmentModule(DICOM.DICOMWrapper w) {
    String manufacturer = w.getString(0x00080070);

    DICOM.Module.GeneralEquipmentModule m
	= new DICOM.Module.GeneralEquipmentModule(manufacturer);

    return m;
  }

  public DICOM.Module.SOPCommonModule
    getSOPCommonModule(DICOM.DICOMWrapper w) {
    String classUID    = w.getString(0x00080016);
    String instanceUID = w.getString(0x00080018);
    DICOM.Module.SOPCommonModule m =
	new DICOM.Module.SOPCommonModule(classUID, instanceUID);
    return m;
  }
  public DICOM.Module.GeneralSeriesModule
    getGeneralSeriesModule(DICOM.DICOMWrapper w) {
    String modality = w.getString(0x00080060);
    String seriesUID = w.getString(0x0020000E);
    String seriesNumber = w.getString(0x00200011);

    DICOM.Module.GeneralSeriesModule m =
	new DICOM.Module.GeneralSeriesModule(modality, seriesUID, seriesNumber);

    return m;
  }
  public DICOM.Module.SRDocumentSeriesModule
    getSRDocumentSeriesModule(DICOM.DICOMWrapper w) {
    String modality = w.getString(0x00080060);
    String seriesUID = w.getString(0x0020000E);
    String seriesNumber = w.getString(0x00200011);

    DICOM.Module.SRDocumentSeriesModule m =
	new DICOM.Module.SRDocumentSeriesModule(modality, seriesUID, seriesNumber);

    return m;
  }



  public int inflateStructuredReport(DICOM.DICOMWrapper report,
				     DICOM.DICOMWrapper w) {
    DICOM.Module.SOPCommonModule m = getSOPCommonModule(w);
    setSOPCommonModule(report, m);

    DICOM.Module.PatientModule p = getPatientModule(w);
    setPatientModule(report, p);

    DICOM.Module.GeneralStudyModule s = getGeneralStudyModule(w);
    setGeneralStudyModule(report, s);

    DICOM.Module.GeneralEquipmentModule e = getGeneralEquipmentModule(w);
    setGeneralEquipmentModule(report, e);

    if (isStructuredReport(w)) {
    } else {
      DICOM.Module.GeneralSeriesModule series = getGeneralSeriesModule(w);
      setSRDocumentSeriesModule(report, series);
    }
    return 0;
  }

  private boolean isStructuredReport(DICOM.DICOMWrapper w) {
    String sopClass = w.getString(0x00080016);
    if (sopClass.equals("1.2.840.10008.5.1.4.1.1.88.11"))
      return true;
    if (sopClass.equals("1.2.840.10008.5.1.4.1.1.88.22"))
      return true;
    if (sopClass.equals("1.2.840.10008.5.1.4.1.1.88.33"))
      return true;

    return false;
  }
}
