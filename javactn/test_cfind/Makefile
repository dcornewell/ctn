#include $(DICOM_MAKE)
#CFLAGS = $(C_OPTS)

cfind1:
	javac cfind1.java

t1:
	java cfind1 DRNO DRNO localhost 2350 Patient q1.dcm

clean:
	rm *.class
data:
	dcm_create_object -i q1.txt q1.dcm
