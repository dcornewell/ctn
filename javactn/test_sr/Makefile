#include $(DICOM_MAKE)
#CFLAGS = $(C_OPTS)

sr1:
	javac sr1.java

sr1_test:
	java sr1 a.dcm

sr2:
	javac sr2.java

sr2_test:
	java sr2 a.dcm

sr3:
	javac sr3.java

sr3_test:
	java sr3 i.dcm z.dcm

sr4:
	javac sr4.java

sr4_test:
	java -Dcodetable.base=$(MESA_TARGET)/runtime/codes sr4

sr5:
	javac sr5.java

sr5_test:
	$(JDK_ROOT)/bin/java -Dcodetable.base=$(MESA_TARGET)/runtime/codes sr5

clean:
	rm *.class
