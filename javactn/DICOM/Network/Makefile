# Makefile for java classes of second level DICOM services

all:
	make Association.class
	make DICOM_Network_Association.h
	cp *.h $(DICOM_ROOT)/javactn/jni_src

Association.class: Association.java
	$(JDK_ROOT)/bin/javac Association.java

DICOM_Network_Association.h: Association.java
	$(JDK_ROOT)/bin/javah -jni DICOM.Network.Association

clean:
	rm -f *.class *.h *.java~
