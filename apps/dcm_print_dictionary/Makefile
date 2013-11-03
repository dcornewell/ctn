#	Makefile for dcm_print_dictionary

NAME = dcm_print_dictionary
OBJ = $(NAME).o
EXES = dcm_print_dictionary dcm_dump_vector

include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LIBS = $(LIBS_CTN_NODB)
#LIBS = -lctn
LFLAGS = -L$(DICOM_LIB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(EXES) *.o *.BAK *.bak *% core debug dcm_prin

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
				$(CI) dcm_dump_vector.c
	rm -f $(EXES) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) dcm_dump_vector.c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(DEBUG_LIBS)

relink:
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(DEBUG_LIBS)

$(NAME).o: $(NAME).c dicom.h condition.h dicom_objects.h

dcm_dump_vector: dcm_dump_vector.o
	$(CC) -o dcm_dump_vector -g $(LFLAGS) dcm_dump_vector.o $(LIBS) $(DEBUG_LIBS)

dcm_dump_vector.o: dcm_dump_vector.c dicom.h condition.h dicom_objects.h
lint:
	flexelint $(LINTOPTIONS) $(NAME).c
