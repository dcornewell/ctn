# Makefile for java classes of Information Object (InfoObj) level DICOM services

all:
	make CompositeFactory.class
	make CompositeObject.class
	make Image.class
	make Waveform.class
	cp *.h $(DICOM_ROOT)/javactn/jni_src

CompositeFactory.class: CompositeFactory.java
	$(JDK_ROOT)/bin/javac CompositeFactory.java

CompositeObject.class: CompositeObject.java
	$(JDK_ROOT)/bin/javac CompositeObject.java

Image.class: Image.java
	$(JDK_ROOT)/bin/javac Image.java
	$(JDK_ROOT)/bin/javah -jni DICOM.InfoObj.Image

Waveform.class: Waveform.java
	$(JDK_ROOT)/bin/javac Waveform.java
	$(JDK_ROOT)/bin/javah -jni DICOM.InfoObj.Waveform

clean:
	rm -f *.class *.h
