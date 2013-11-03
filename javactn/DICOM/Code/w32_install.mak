install:
	$(MAKE)/f w32_install.mak CodeItem.class
	$(MAKE)/f w32_install.mak CodeManager.class

CodeItem.class: CodeItem.java
	javac CodeItem.java

CodeManager.class: CodeManager.java
	javac CodeManager.java

clean:
	del *.class
