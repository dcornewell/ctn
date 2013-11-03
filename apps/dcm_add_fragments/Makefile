#	Makefile for dcm_add_fragments

NAME = dcm_add_fragments
#OBJ1 = $(NAME).o cstore.o
OBJ1 = $(NAME).o
include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS_SECURE_UCB) $(LIBPATH_DATABASE)
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
	$(INDENT) $(NAME).c;	$(CI) $(NAME).c
	rm -f *.o $(NAME) *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME):	$(OBJ1)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ1) $(LIBS) $(LFLAGS)

relink:
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ1) $(LIBS) $(LFLAGS)

$(NAME).o: $(NAME).c dicom.h lst.h dicom_objects.h condition.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c

t1:
	./dcm_add_fragments -t 0012.dcm a.dcm x/12.1 x/12.2  x/12.3  x/12.4  x/12.5  x/12.6  x/12.7  x/12.8  x/12.9 x/12.??

t2:
	./dcm_add_fragments -t 0012.dcm a.dcm x/12.1 x/12.2  x/12.3

t3:
	./dcm_add_fragments -t 0012.dcm b.dcm ../dcm_dump_compressed/x/12.1 ../dcm_dump_compressed/x/12.2  ../dcm_dump_compressed/x/12.3  ../dcm_dump_compressed/x/12.4  ../dcm_dump_compressed/x/12.5  ../dcm_dump_compressed/x/12.6  ../dcm_dump_compressed/x/12.7  ../dcm_dump_compressed/x/12.8  ../dcm_dump_compressed/x/12.9 ../dcm_dump_compressed/x/12.??

t4:	dcm_add_fragments
	./dcm_add_fragments -t 0012.dcm b.dcm text/a.txt

t5:	dcm_add_fragments
	./dcm_add_fragments -t 0012.dcm b.dcm text/a.txt text/b.txt
