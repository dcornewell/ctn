#	Makefile for DBFILL

NAME = dbfill
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK core

indent:
	$(INDENT) dbfill.c
	$(INDENT) dbfill.h

checkin:
	$(INDENT) dbfill.c; $(CI) dbfill.c
	$(INDENT) dbfill.h; $(CI) dbfill.h
	rm -f $(NAME) *.o *.BAK core

checkout:
	$(CO) $(NAME).c
	$(CO) dbfill.h

# Turn off lint checking as we are going to retire this app
lint:
#	flexelint $(LINTOPTIONS) dbfill.c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(LIBS_OS)

$(NAME) .o: $(NAME).c dbfill.h
