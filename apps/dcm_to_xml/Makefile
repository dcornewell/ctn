#	Makefile for dcm_to_xml

NAME = dcm_to_xml
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core *~ debug

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -rf $(NAME) *.o *.BAK *.bak *% core *~ debug

checkout:
	$(CO) $(NAME).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(DEBUG_LIBS)

$(NAME).o: $(NAME).c dicom.h condition.h dicom_objects.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
