#	Makefile for ramp

include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	ramp8 ramp16

install: ramp8 ramp16
	$(INSTALL) ramp8 $(DICOM_BIN)
	$(INSTALL) ramp16 $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core dcm_dump debug

ramp8: ramp8.o
	$(CC) -o ramp8 -g $(LFLAGS) ramp8.o $(LIBS) $(DEBUG_LIBS)

ramp16: ramp16.o
	$(CC) -o ramp16 -g $(LFLAGS) ramp16.o $(LIBS) $(DEBUG_LIBS)

ramp8.o: ramp8.c dicom.h condition.h dicom_objects.h
ramp16.o: ramp16.c dicom.h condition.h dicom_objects.h

t103:
	./ramp16 > 16.dat
	dd if=16.dat of=ramp16.dat conv=swab
	dcm_create_object -i params16.txt -p ramp16.dat ramp16.dcm

	./ramp8 > ramp8.dat
	dcm_create_object -i params8.txt -p ramp8.dat ramp8.dcm

	cp ramp8.dcm ramp16.dcm /cairo/ctn_test_data/t103
