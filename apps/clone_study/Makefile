#	Makefile for clone_study

OBJ = clone_study.o remap.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LIBS = $(LIBS_CTN) $(LIBS_OS)
LFLAGS = -L$(DICOM_LIB)

application:	clone_study

install: clone_study
	$(INSTALL) clone_study $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f clone_study *.o *.BAK *% *~ core

indent:
	$(INDENT) clone_study.c
	$(INDENT) clone_study.h
	$(INDENT) remap.c

checkin:
	$(INDENT) clone_study.c;	$(CI) clone_study.c
	$(INDENT) clone_study.h;	$(CI) clone_study.h
	$(INDENT) remap.c;		$(CI) remap.c
	rm -f clone_study *.o *.BAK core *% *~

checkout:
	$(CO) clone_study.c
	$(CO) clone_study.h
	$(CO) remap.c

clone_study: $(OBJ)
	$(CC) -o clone_study -g $(LFLAGS) $(OBJ) $(LIBS)

lint:
	flexelint $(LINTOPTIONS) clone_study.c remap.c
