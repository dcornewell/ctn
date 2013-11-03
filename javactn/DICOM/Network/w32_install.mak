install:
	$(MAKE)/f w32_install.mak Association.class

	$(MAKE)/f w32_install.mak include_files
	copy *.h $(DICOM_ROOT)\javactn\jni_src

Association.class: Association.java
	javac Association.java

include_files:
	javah -jni DICOM.Network.Association

clean:
	del *.class
	del *.h
