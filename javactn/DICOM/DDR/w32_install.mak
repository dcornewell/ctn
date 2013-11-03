install:
	$(MAKE)/f w32_install.mak DIRInterface.class
	$(MAKE)/f w32_install.mak Patient.class
	$(MAKE)/f w32_install.mak Series.class
	$(MAKE)/f w32_install.mak SeriesLeaf.class
	$(MAKE)/f w32_install.mak Study.class

	$(MAKE)/f w32_install.mak include_files
	copy *.h $(DICOM_ROOT)\javactn\jni_src

DIRInterface.class: DIRInterface.java
	javac DIRInterface.java

Patient.class: Patient.java
	javac Patient.java

Series.class: Series.java
	javac Series.java

SeriesLeaf.class: SeriesLeaf.java
	javac SeriesLeaf.java

Study.class: Study.java
	javac Study.java

include_files:
	javah -jni DICOM.DDR.DIRInterface

clean:
	del *.class
	del *.h
