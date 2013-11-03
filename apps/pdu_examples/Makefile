#	Makefile for applications that demonstrate PDUs

NAME1 = ex1_initiator
OBJ1 = $(NAME1).o
NAME2 = ex2_initiator
OBJ2 = $(NAME2).o
NAME3 = ex3_initiator
OBJ3 = $(NAME3).o
NAME3A = ex3_acceptor
OBJ3A = $(NAME3A).o ex3_cond.o
NAME4 = ex4_initiator
OBJ4 = $(NAME4).o
NAME4A = ex4_acceptor
OBJ4A = $(NAME4A).o ex4_cond.o

include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)


application:	$(NAME1) $(NAME2) $(NAME3) $(NAME3A) $(NAME4) $(NAME4A)

install: $(NAME1) $(NAME2) $(NAME3) $(NAME3A) $(NAME4) $(NAME4A)
	$(INSTALL) $(NAME1) $(DICOM_BIN)
	$(INSTALL) $(NAME2) $(DICOM_BIN)
	$(INSTALL) $(NAME3) $(DICOM_BIN)
	$(INSTALL) $(NAME3A) $(DICOM_BIN)
	$(INSTALL) $(NAME4) $(DICOM_BIN)
	$(INSTALL) $(NAME4A) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME1) $(NAME2) $(NAME3) $(NAME3A) $(NAME4) $(NAME4A) *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME1).c
	$(INDENT) $(NAME2).c
	$(INDENT) $(NAME3).c
	$(INDENT) $(NAME3A).c
	$(INDENT) ex3_cond.c
	$(INDENT) ex3_acceptor.h
	$(INDENT) $(NAME4).c
	$(INDENT) $(NAME4A).c
	$(INDENT) ex4_cond.c
	$(INDENT) ex4_acceptor.h

checkin:
	$(INDENT) $(NAME1).c; $(CI) $(NAME1).c
	$(INDENT) $(NAME2).c; $(CI) $(NAME2).c
	$(INDENT) $(NAME3).c; $(CI) $(NAME3).c
	$(INDENT) $(NAME3A).c; $(CI) $(NAME3A).c
	$(INDENT) ex3_cond.c; $(CI) ex3_cond.c
	$(INDENT) ex3_acceptor.h; $(CI) ex3_acceptor.h
	$(INDENT) $(NAME4).c; $(CI) $(NAME4).c
	$(INDENT) $(NAME4A).c; $(CI) $(NAME4A).c
	$(INDENT) ex4_cond.c; $(CI) ex4_cond.c
	$(INDENT) ex4_acceptor.h; $(CI) ex4_acceptor.h
	rm -f $(NAME1) $(NAME2) $(NAME3) $(NAME3A) $(NAME4) $(NAME4A) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME1).c
	$(CO) $(NAME2).c
	$(CO) $(NAME3).c
	$(CO) $(NAME3A).c
	$(CO) ex3_cond.c
	$(CO) ex3_acceptor.h
	$(CO) $(NAME4).c
	$(CO) ex4_cond.c
	$(CO) ex4_acceptor.h
	$(CO) $(NAME4A).c

$(NAME1): $(OBJ1)
	$(CC) -o $(NAME1) -g $(LFLAGS) $(OBJ1) $(LIBS)
$(NAME2): $(OBJ2)
	$(CC) -o $(NAME2) -g $(LFLAGS) $(OBJ2) $(LIBS)
$(NAME3): $(OBJ3)
	$(CC) -o $(NAME3) -g $(LFLAGS) $(OBJ3) $(LIBS)
$(NAME3A): $(OBJ3A)
	$(CC) -o $(NAME3A) -g $(LFLAGS) $(OBJ3A) $(LIBS)
$(NAME4): $(OBJ4)
	$(CC) -o $(NAME4) -g $(LFLAGS) $(OBJ4) $(LIBS)
$(NAME4A): $(OBJ4A)
	$(CC) -o $(NAME4A) -g $(LFLAGS) $(OBJ4A) $(LIBS)

$(NAME1).o: $(NAME1).c dicom.h dulprotocol.h dicom_objects.h dicom_services.h
$(NAME2).o: $(NAME2).c dicom.h dulprotocol.h dicom_objects.h dicom_services.h
$(NAME3).o: $(NAME3).c dicom.h dulprotocol.h dicom_objects.h dicom_services.h
$(NAME3A).o: $(NAME3A).c dicom.h dulprotocol.h dicom_objects.h \
dicom_services.h ex3_acceptor.h
$(NAME4).o: $(NAME4).c dicom.h dulprotocol.h dicom_objects.h dicom_services.h
$(NAME4A).o: $(NAME4A).c dicom.h dulprotocol.h dicom_objects.h \
dicom_services.h ex4_acceptor.h

lint:
	flexelint $(LINTOPTIONS) $(NAME1).c
	flexelint $(LINTOPTIONS) $(NAME2).c
	flexelint $(LINTOPTIONS) $(NAME3).c
	flexelint $(LINTOPTIONS) $(NAME3A).c
	flexelint $(LINTOPTIONS) $(NAME4).c
	flexelint $(LINTOPTIONS) $(NAME4A).c
