install:
	$(MAKE)/f w32_install.mak ContentItem.class
	$(MAKE)/f w32_install.mak ContentItemFactory.class
	$(MAKE)/f w32_install.mak StructuredReport.class

ContentItem.class: ContentItem.java
	javac ContentItem.java

ContentItemFactory.class: ContentItemFactory.java
	javac ContentItemFactory.java

StructuredReport.class: StructuredReport.java
	javac StructuredReport.java

clean:
	del *.class
