
install:	jar
	copy javactn.jar $(JAR_DIRECTORY)

jar:	java_classes
	jar -c0vf javactn.jar DICOM\*.class DICOM\DDR\*.class DICOM\Module\*.class \
DICOM\InfoObj\*.class DICOM\Misc\*.class DICOM\SR\*.class DICOM\Code\*.class DICOM\Network\*.class


java_classes:
	cd DICOM
	nmake/f w32_install.mak install
	cd ..
