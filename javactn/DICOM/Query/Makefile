# Makefile for java classes of second level DICOM services

all:
	make CFindSCU.class
	cp *.h $(DICOM_ROOT)/javactn/jni_src

CFindSCU.class: CFindSCU.java DICOM_Query_CFindSCU.h
	$(JDK_ROOT)/bin/javac CFindSCU.java

DICOM_Query_CFindSCU.h: CFindSCU.java
	$(JDK_ROOT)/bin/javah -jni DICOM.Query.CFindSCU

clean:
	rm -f *.class *.h *.java~
