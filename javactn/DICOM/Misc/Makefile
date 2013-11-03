# Makefile for java classes of MISC level DICOM services

all:
	make CodedValue.class
	make PaletteColor.class
	make Timer.class
	cp *.h $(DICOM_ROOT)/javactn/jni_src

CodedValue.class: CodedValue.java
	$(JDK_ROOT)/bin/javac CodedValue.java

PaletteColor.class: PaletteColor.java
	$(JDK_ROOT)/bin/javac PaletteColor.java

Timer.class: Timer.java
	$(JDK_ROOT)/bin/javac Timer.java
	$(JDK_ROOT)/bin/javah -jni DICOM.Misc.Timer

clean:
	rm -f *.class *.h *.java~
