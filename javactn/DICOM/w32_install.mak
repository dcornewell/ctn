
install: DICOMWrapper.class DICOM_DICOMWrapper.h
	$(MAKE) /f w32_install.mak DICOMWrapper.class
	$(MAKE) /f w32_install.mak DICOM_DICOMWrapper.h
	$(MAKE) /f w32_install.mak x_Module
	$(MAKE) /f w32_install.mak x_DDR
	$(MAKE) /f w32_install.mak x_InfoObj
	$(MAKE) /f w32_install.mak x_Misc
	$(MAKE) /f w32_install.mak x_SR
	$(MAKE) /f w32_install.mak x_Code
	$(MAKE) /f w32_install.mak x_Network

	copy DICOM_DICOMWrapper.h $(DICOM_ROOT)\javactn\jni_src

clean:
	del DICOMWrapper.class DICOM_DICOMWrapper.h

	cd "DDR"
	$(MAKE) /f w32_install.mak clean
	cd ".."

	cd "Module"
	$(MAKE) /f w32_install.mak clean
	cd ".."

	cd "InfoObj"
	$(MAKE)/f w32_install.mak clean
	cd ".."

	cd "Misc"
	$(MAKE)/f w32_install.mak clean
	cd ".."

	cd "SR"
	$(MAKE)/f w32_install.mak clean
	cd ".."

	cd "Code"
	$(MAKE)/f w32_install.mak clean
	cd ".."

	cd "Network"
	$(MAKE)/f w32_install.mak clean
	cd ".."

DICOMWrapper.class: DICOMWrapper.java
	javac DICOMWrapper.java

DICOM_DICOMWrapper.h: DICOMWrapper.java
	javah -jni DICOM.DICOMWrapper


x_DDR:
	cd "DDR"
	$(MAKE) /f w32_install.mak install
	cd ".."

x_InfoObj:
	cd "InfoObj"
	$(MAKE) /f w32_install.mak install
	cd ".."

x_Misc:
	cd "Misc"
	$(MAKE) /f w32_install.mak install
	cd ".."

x_Module:
	cd "Module"
	$(MAKE) /f w32_install.mak install
	cd ".."

x_SR:
	cd "SR"
	$(MAKE) /f w32_install.mak install
	cd ".."

x_Code:
	cd "Code"
	$(MAKE) /f w32_install.mak install
	cd ".."

x_Network:
	cd "Network"
	$(MAKE) /f w32_install.mak install
	cd ".."


