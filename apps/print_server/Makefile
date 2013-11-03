#	Makefile for print_server
NAME = print_server
OBJ = $(NAME).o sscond.o association.o requests.o utils.o verify.o get.o \
	create.o set.o action.o delete.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = $(LIBPATH_OS) $(LIBPATH_UCB)
LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(LIBS_OS)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug print_se

indent:
	$(INDENT) $(NAME).c
	$(INDENT) $(NAME).h
	$(INDENT) private.h
	$(INDENT) sscond.c
	$(INDENT) association.c
	$(INDENT) requests.c
	$(INDENT) utils.c
	$(INDENT) verify.c
	$(INDENT) get.c
	$(INDENT) create.c
	$(INDENT) set.c
	$(INDENT) action.c
	$(INDENT) delete.c
	$(INDENT) static_variables.h

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) $(NAME).h; $(CI) $(NAME).h
	$(INDENT) private.h; $(CI) private.h
	$(INDENT) association.c; $(CI) association.c
	$(INDENT) requests.c; $(CI) requests.c
	$(INDENT) sscond.c; $(CI) sscond.c
	$(INDENT) utils.c; $(CI) utils.c
	$(INDENT) verify.c; $(CI) verify.c
	$(INDENT) get.c; $(CI) get.c
	$(INDENT) create.c; $(CI) create.c
	$(INDENT) set.c; $(CI) set.c
	$(INDENT) action.c; $(CI) action.c
	$(INDENT) delete.c; $(CI) delete.c
	$(INDENT) static_variables.h; $(CI) static_variables.h
	rm -rf $(NAME) *.o *.BAK *.bak *% core print_se

checkout:
	$(CO) $(NAME).c
	$(CO) $(NAME).h
	$(CO) private.h
	$(CO) association.c
	$(CO) requests.c
	$(CO) sscond.c
	$(CO) utils.c
	$(CO) verify.c
	$(CO) get.c
	$(CO) create.c
	$(CO) set.c
	$(CO) action.c
	$(CO) delete.c
	$(CO) static_variables.h

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBPATH) $(LIBS)

$(NAME).o: $(NAME).c print_server.h
association.o: association.c print_server.h
requests.o: requests.c print_server.h
sscond.o: sscond.c print_server.h
utils.o: utils.c print_server.h
verify.o: verify.c print_server.h
get.o: get.c print_server.h
create.o: create.c print_server.h
set.o: set.c print_server.h
action.o: action.c print_server.h
delete.o: delete.c print_server.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c sscond.c association.c requests.c \
		utils.c verify.c get.c create.c set.c action.c delete.c
