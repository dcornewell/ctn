# Makefile for java classes of Code (Item) classes

all:
	make CodeItem.class
	make CodeManager.class

CodeItem.class: CodeItem.java
	$(JDK_ROOT)/bin/javac CodeItem.java

CodeManager.class: CodeManager.java
	$(JDK_ROOT)/bin/javac CodeManager.java

clean:
	rm -f *.class
