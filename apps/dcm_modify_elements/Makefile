#	Makefile for dcm_modify_elements

NAME = dcm_modify_elements
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
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
	
checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME):	$(OBJ) 
	$(CC) -o $(NAME) -g $(OBJ) $(LFLAGS) $(LIBS)

relink:
	$(CC) -o $(NAME) -g $(OBJ) $(LFLAGS) $(LIBS)

$(NAME).o:	$(NAME).c dicom.h dicom_objects.h condition.h

t1:	$(NAME)
	./dcm_modify_elements -t -T -s BIRXA110 BIRXA110_x 28 3010 28 3002 z1
#	./dcm_modify_elements -t -T -s BIRXA110 BIRXA110_x 28 3010 28 3006 data1
