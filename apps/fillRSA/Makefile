#	Makefile for fillRSA

NAME = fillRSA
NAME1 = fillRSAImpInterp
OBJ = $(NAME).o work.o
OBJ1 = $(NAME1).o workImpInterp.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE) $(LIBS_OS)
LIBS = $(LIBS_CTN) $(DEBUG_LIBS)

application:	$(NAME) $(NAME1)

install: $(NAME) $(NAME1)
	$(INSTALL) $(NAME) $(DICOM_BIN)
	$(INSTALL) $(NAME1) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) $(NAME1) *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c
	$(INDENT) work.c
	$(INDENT) $(NAME1).c
	$(INDENT) workImpInterp.c

checkin:
	$(INDENT) $(NAME).c;	$(CI) $(NAME).c
	$(INDENT) work.c;	$(CI) work.c
	$(INDENT) $(NAME1).c;	$(CI) $(NAME1).c
	$(INDENT) workImpInterp.c;	$(CI) workImpInterp.c
	rm -f $(NAME) $(NAME1) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) work.c
	$(CO) $(NAME1).c
	$(CO) workImpInterp.c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) -lm
$(NAME1): $(OBJ1)
	$(CC) -o $(NAME1) -g $(LFLAGS) $(OBJ1) $(LIBS) -lm

$(NAME).o: $(NAME).c tbl.h idb.h
work.o: work.c tbl.h idb.h fis.h
$(NAME1).o: $(NAME1).c tbl.h idb.h
workImpInterp.o: workImpInterp.c tbl.h idb.h fis.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c work.c
	flexelint $(LINTOPTIONS) $(NAME1).c workImpInterp.c
