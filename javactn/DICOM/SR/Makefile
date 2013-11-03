# Makefile for java classes of MISC level DICOM services

all:
	make ContentItem.class
	make ContentItemFactory.class
	make StructuredReport.class
#	cp *.h $(DICOM_ROOT)/javactn/jni_src

ContentItem.class: ContentItem.java
	$(JDK_ROOT)/bin/javac ContentItem.java

ContentItemFactory.class: ContentItemFactory.java
	$(JDK_ROOT)/bin/javac ContentItemFactory.java

StructuredReport.class: StructuredReport.java
	$(JDK_ROOT)/bin/javac StructuredReport.java

clean:
	rm -f *.class
