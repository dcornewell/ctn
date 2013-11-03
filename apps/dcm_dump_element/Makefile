#	Makefile for dcm_dump_element

NAME = dcm_dump_element
OBJ1 = $(NAME).o 
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

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -f *.o $(NAME) *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME):	$(OBJ1)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ1) $(LIBS)

$(NAME).o: $(NAME).c dicom.h lst.h condition.h dicom_objects.h


t1:	dcm_dump_element
	./dcm_dump_element -i 1 0040 a730 a.dcm a1.dcm
	dcm_dump_file -f a1.dcm

t2:	dcm_dump_element
	./dcm_dump_element -i 2 0040 a730 a.dcm a2.dcm
	dcm_dump_file -f a2.dcm

t3:	dcm_dump_element
	./dcm_dump_element -i 3 0040 a730 a.dcm a3.dcm
	dcm_dump_file -f a3.dcm

t4:	dcm_dump_element
	./dcm_dump_element -i 4 0040 a730 a.dcm a4.dcm
	dcm_dump_file -f a4.dcm

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
