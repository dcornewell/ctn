# Makefile for java classes of second level DICOM services

all:
	make Patient.class
	make Study.class
	make Series.class
	make SeriesLeaf.class
	make DIRInterface.class
	cp *.h $(DICOM_ROOT)/javactn/jni_src

DIRInterface.class: DIRInterface.java
	$(JDK_ROOT)/bin/javac DIRInterface.java
	$(JDK_ROOT)/bin/javah -jni DICOM.DDR.DIRInterface

Patient.class: Patient.java
	$(JDK_ROOT)/bin/javac Patient.java

Study.class: Study.java
	$(JDK_ROOT)/bin/javac Study.java

Series.class: Series.java
	$(JDK_ROOT)/bin/javac Series.java

SeriesLeaf.class: SeriesLeaf.java
	$(JDK_ROOT)/bin/javac SeriesLeaf.java

clean:
	rm -f *.class *.h *.java~
