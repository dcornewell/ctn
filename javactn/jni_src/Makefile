include $(DICOM_MAKE)
CFLAGS = $(C_OPTS)


jni_code:
	$(CC) $(CFLAGS) $(SHARED_SWITCH) -I$(JDK_ROOT)/include \
-I$(JDK_ROOT)/include/solaris -I$(JDK_ROOT)/include/linux \
-I$(JDK_ROOT)/include/irix \
javactn.c -o libjavactn.so -L$(DICOM_LIB) -lctn

install:	jni_code
	cp libjavactn.so $(JAR_DIRECTORY)

clean:
	rm -f libjavactn.so

