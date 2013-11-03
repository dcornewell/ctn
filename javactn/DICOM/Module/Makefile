# Makefile for java classes of MODULE level DICOM services

all:
	make GeneralEquipmentModule.class
	make GeneralSeriesModule.class
	make GeneralStudyModule.class
	make PatientModule.class
	make ImagePixelModule.class
	make SOPCommonModule.class
	make SRDocumentContentModule.class
	make SRDocumentGeneralModule.class
	make SRDocumentSeriesModule.class
	make WaveformModule.class
	make WaveformChannel.class
	make include_files
	cp *.h $(DICOM_ROOT)/javactn/jni_src

GeneralEquipmentModule.class: GeneralEquipmentModule.java
	$(JDK_ROOT)/bin/javac GeneralEquipmentModule.java

GeneralSeriesModule.class: GeneralSeriesModule.java
	$(JDK_ROOT)/bin/javac GeneralSeriesModule.java

GeneralStudyModule.class: GeneralStudyModule.java
	$(JDK_ROOT)/bin/javac GeneralStudyModule.java

PatientModule.class: PatientModule.java
	$(JDK_ROOT)/bin/javac PatientModule.java

ImagePixelModule.class: ImagePixelModule.java
	$(JDK_ROOT)/bin/javac ImagePixelModule.java
	$(JDK_ROOT)/bin/javah -jni DICOM.Module.ImagePixelModule

SOPCommonModule.class: SOPCommonModule.java
	$(JDK_ROOT)/bin/javac SOPCommonModule.java

SRDocumentContentModule.class: SRDocumentContentModule.java
	$(JDK_ROOT)/bin/javac SRDocumentContentModule.java

SRDocumentGeneralModule.class: SRDocumentGeneralModule.java
	$(JDK_ROOT)/bin/javac SRDocumentGeneralModule.java

SRDocumentSeriesModule.class: SRDocumentSeriesModule.java
	$(JDK_ROOT)/bin/javac SRDocumentSeriesModule.java

WaveformModule.class: WaveformModule.java
	$(JDK_ROOT)/bin/javac WaveformModule.java
	$(JDK_ROOT)/bin/javah -jni DICOM.Module.WaveformModule

WaveformChannel.class: WaveformChannel.java
	$(JDK_ROOT)/bin/javac WaveformChannel.java

include_files:
	$(JDK_ROOT)/bin/javah -jni DICOM.Module.ImagePixelModule
	$(JDK_ROOT)/bin/javah -jni DICOM.Module.WaveformModule

clean:
	rm -f *.class *.h *.java~
