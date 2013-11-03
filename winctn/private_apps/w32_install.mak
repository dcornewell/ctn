install:
	cd dcm_to_analyze
	nmake/f dcm_to_analyze.mak CFG="dcm_to_analyze - Win32 Release"
	cd ".."

	copy dcm_to_analyze\Release\dcm_to_analyze.exe D:\ctn\bin

clean:
	cd dcm_to_analyze
	nmake/f dcm_to_analyze.mak CFG="dcm_to_analyze - Win32 Release" clean
	cd ".."

