#	Makefile for dcm_resize

NAME = dcm_resize
OBJ1 = $(NAME).o pixavg.o
include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf *.o $(NAME) *.BAK *.bak *% core debug

indent:
	$(INDENT) $(NAME).c
	$(INDENT) pixavg.c
	$(INDENT) pixavg.h
checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) pixavg.c;  $(CI) pixavg.c
	$(INDENT) pixavg.h;  $(CI) pixavg.h
	rm -f *.o $(NAME) *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) pixavg.c
	$(CO) pixavg.h

$(NAME):	$(OBJ1)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ1) $(LIBS)

$(NAME).o: $(NAME).c dicom.h lst.h dicom_objects.h condition.h pixavg.h
pixavg.o: pixavg.c pixavg.h dicom.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c pixavg.c
