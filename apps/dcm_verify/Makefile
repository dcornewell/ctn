#	Makefile for dcm_verify

NAME = dcm_verify
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug

indent:
	$(INDENT) $(NAME).c
	$(INDENT) $(NAME).h

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) $(NAME).h; $(CI) $(NAME).h
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) $(NAME).h

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

$(NAME).o: $(NAME).c dicom.h lst.h condition.h dicom_ie.h $(NAME).h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
