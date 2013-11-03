include $(DICOM_MAKE)
CFLAGS = $(C_OPTS)


all:	java_classes jni_code

install:	jar jni_code
	$(INSTALL) javactn.jar $(JAR_DIRECTORY)
	$(INSTALL) jni_src/libjavactn.so $(JAR_DIRECTORY)

java_classes:
	cd DICOM; make all

jni_code:
	cd jni_src; make install

install:	jni_code

clean:
	cd DICOM; make clean
	cd jni_src; make clean

install_jar:	jar
	$(INSTALL) javactn.jar $(JAR_DIRECTORY)

jar:	java_classes
	$(JDK_ROOT)/bin/jar -c0vf javactn.jar DICOM/*class DICOM/*/*class 

fastjar:
	$(JDK_ROOT)/bin/jar -c0vf javactn.jar DICOM/*class DICOM/*/*class
