# Makefile for java classes of first level DICOM services

all:
	make DICOMWrapper.class
	cd Module; make all
	cd DDR; make all
	cd InfoObj; make all
	cd Misc; make all
	cd SR; make all
	cd Code; make all
	cd Network; make all
	cp *.class $(DICOM_ROOT)/javactn/jni_src
	cp DICOM_DICOMWrapper.h $(DICOM_ROOT)/javactn/jni_src

DICOMWrapper.class: DICOMWrapper.java
	$(JDK_ROOT)/bin/javac DICOMWrapper.java
	$(JDK_ROOT)/bin/javah -jni DICOM.DICOMWrapper

clean:
	rm -f *.class *.o *.h
	cd DDR; make clean
	cd InfoObj; make clean
	cd Misc; make clean
	cd Module; make clean
	cd SR; make clean;
	cd Code; make clean;
