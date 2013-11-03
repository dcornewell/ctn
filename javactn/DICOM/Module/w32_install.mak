install:
	$(MAKE)/f w32_install.mak GeneralEquipmentModule.class
	$(MAKE)/f w32_install.mak GeneralSeriesModule.class
	$(MAKE)/f w32_install.mak GeneralStudyModule.class
	$(MAKE)/f w32_install.mak ImagePixelModule.class
	$(MAKE)/f w32_install.mak PatientModule.class
	$(MAKE)/f w32_install.mak SOPCommonModule.class
	$(MAKE)/f w32_install.mak SRDocumentContentModule.class
	$(MAKE)/f w32_install.mak SRDocumentGeneralModule.class
	$(MAKE)/f w32_install.mak SRDocumentSeriesModule.class
	$(MAKE)/f w32_install.mak WaveformModule.class
	$(MAKE)/f w32_install.mak WaveformChannel.class
	$(MAKE)/f w32_install.mak include_files
	copy *.h $(DICOM_ROOT)\javactn\jni_src

GeneralEquipmentModule.class: GeneralEquipmentModule.java
	javac GeneralEquipmentModule.java

GeneralSeriesModule.class: GeneralSeriesModule.java
	javac GeneralSeriesModule.java

GeneralStudyModule.class: GeneralStudyModule.java
	javac GeneralStudyModule.java

ImagePixelModule.class: ImagePixelModule.java
	javac ImagePixelModule.java

PatientModule.class: PatientModule.java
	javac PatientModule.java

SOPCommonModule.class: SOPCommonModule.java
	javac SOPCommonModule.java

SRDocumentContentModule.class: SRDocumentContentModule.java
	javac SRDocumentContentModule.java

SRDocumentGeneralModule.class: SRDocumentGeneralModule.java
	javac SRDocumentGeneralModule.java

SRDocumentSeriesModule.class: SRDocumentSeriesModule.java
	javac SRDocumentSeriesModule.java

WaveformModule.class: WaveformModule.java
	javac WaveformModule.java

WaveformChannel.class: WaveformChannel.java
	javac WaveformChannel.java

include_files:
	javah -jni DICOM.Module.ImagePixelModule
	javah -jni DICOM.Module.WaveformModule


clean:
	del *.class
	del *.h
