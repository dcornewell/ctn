install:
	$(MAKE)/f w32_install.mak CodedValue.class
	$(MAKE)/f w32_install.mak PaletteColor.class
	$(MAKE)/f w32_install.mak Timer.class

	$(MAKE)/f w32_install.mak include_files
	copy *.h $(DICOM_ROOT)\javactn\jni_src

CodedValue.class: CodedValue.java
	javac CodedValue.java

PaletteColor.class: PaletteColor.java
	javac PaletteColor.java

Timer.class: Timer.java
	javac Timer.java

include_files:
	javah -jni DICOM.Misc.Timer

clean:
	del *.class
	del *.h
