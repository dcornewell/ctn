install:
	$(MAKE)/f w32_install.mak CompositeFactory.class
	$(MAKE)/f w32_install.mak CompositeObject.class
	$(MAKE)/f w32_install.mak Image.class
	$(MAKE)/f w32_install.mak Waveform.class

	$(MAKE)/f w32_install.mak include_files
	copy *.h $(DICOM_ROOT)\javactn\jni_src

CompositeFactory.class: CompositeFactory.java
	javac CompositeFactory.java

CompositeObject.class: CompositeObject.java
	javac CompositeObject.java

Image.class: Image.java
	javac Image.java

Waveform.class: Waveform.java
	javac Waveform.java

include_files:
	javah -jni DICOM.InfoObj.Image
	javah -jni DICOM.InfoObj.Waveform

clean:
	del *.class
	del *.h
