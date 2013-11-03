#	Makefile for print_mgr

NAME = print_mgr
NAME2 = print_client
PRINT_QUERY = print_query
OBJ = $(NAME).o Buttons.o MessageBox.o Fields.o display_error.o\
print_shared.o display_icon.o display_study.o display_message.o
OBJ2 = $(NAME2).o print_shared.o
PRINT_QUERY_OBJ = $(PRINT_QUERY).o print_shared.o

include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include -I/usr/openwin/include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_X11) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB) $(SOLARISUCB)

application:	$(NAME) $(NAME2)

install: $(NAME) $(NAME2)
	$(INSTALL) $(NAME) $(DICOM_BIN)
	$(INSTALL) $(NAME2) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) $(NAME2) $(PRINT_QUERY) test *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c
	$(INDENT) $(NAME).h
	$(INDENT) $(NAME2).c
	$(INDENT) $(PRINT_QUERY).c
	$(INDENT) Buttons.c
	$(INDENT) Buttons.h
	$(INDENT) MessageBox.c
	$(INDENT) MessageBox.h
	$(INDENT) Fields.c
	$(INDENT) Fields.h
	$(INDENT) print_client_app.h
#	$(INDENT) print_client_private.h
#	$(INDENT) print_client_private.c
	$(INDENT) print_mgr.h
	$(INDENT) print_mgr.c
#	$(INDENT) print_callback.c
#	$(INDENT) print_callback.h
	$(INDENT) print_shared.c
	$(INDENT) print_shared.h
	$(INDENT) display_icon.h
	$(INDENT) display_icon.c
	$(INDENT) display_study.h
	$(INDENT) display_study.c
	$(INDENT) display_error.h
	$(INDENT) display_error.c
	$(INDENT) display_message.h
	$(INDENT) display_message.c
	$(INDENT) test.c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) $(NAME).h; $(CI) $(NAME).h
	$(INDENT) $(NAME2).c; $(CI) $(NAME2).c
	$(INDENT) $(PRINT_QUERY).c; $(CI) $(PRINT_QUERY).c
	$(INDENT) Buttons.c; $(CI) Buttons.c
	$(INDENT) Buttons.h; $(CI) Buttons.h
	$(INDENT) MessageBox.c; $(CI) MessageBox.c
	$(INDENT) MessageBox.h; $(CI) MessageBox.h
	$(INDENT) Fields.c; $(CI) Fields.c
	$(INDENT) Fields.h; $(CI) Fields.h
	$(INDENT) print_client_app.h; $(CI) print_client_app.h
#	$(INDENT) print_client_private.h; $(CI) print_client_private.h
#	$(INDENT) print_client_private.c; $(CI) print_client_private.c
#	$(INDENT) print_callback.c; $(CI) print_callback.c
#	$(INDENT) print_callback.h; $(CI) print_callback.h
	$(INDENT) print_shared.c; $(CI)	print_shared.c
	$(INDENT) print_shared.h; $(CI)	print_shared.h
	$(INDENT) display_icon.h; $(CI)	display_icon.h
	$(INDENT) display_icon.c; $(CI)	display_icon.c
	$(INDENT) display_study.h; $(CI) display_study.h
	$(INDENT) display_study.c; $(CI) display_study.c
	$(INDENT) display_error.h; $(CI) display_error.h
	$(INDENT) display_error.c; $(CI) display_error.c
	$(INDENT) display_message.h; $(CI) display_message.h
	$(INDENT) display_message.c; $(CI) display_message.c
	$(INDENT) test.c; $(CI)	test.c
	rm -f $(NAME) $(NAME2) $(PRINT_QUERY) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) $(NAME).h
	$(CO) $(NAME2).c
	$(CO) $(PRINT_QUERY).c
	$(CO) Buttons.c
	$(CO) Buttons.h
	$(CO) MessageBox.c
	$(CO) MessageBox.h
	$(CO) Fields.c 
	$(CO) Fields.h 
	$(CO) print_client_app.h
#	$(CO) print_client_private.h
#	$(CO) print_client_private.c
#	$(CO) print_callback.c
#	$(CO) print_callback.h
	$(CO) print_shared.c
	$(CO) print_shared.h
	$(CO) display_icon.h
	$(CO) display_icon.c
	$(CO) display_study.h
	$(CO) display_study.c
	$(CO) display_error.h
	$(CO) display_error.c
	$(CO) display_message.h
	$(CO) display_message.c
	$(CO) test.c

$(NAME): $(OBJ) 
	$(CC) -o $(NAME) $(CFLAGS) $(OBJ) $(LFLAGS) $(XAW_LIBS) $(LIBS_XAW) $(LIBS_X11) $(LIBS)
	
$(NAME2): $(OBJ2) print_client_app.h
	$(CC) -o $(NAME2) $(CFLAGS) $(OBJ2) $(LFLAGS) $(LIBS)

$(PRINT_QUERY): $(PRINT_QUERY_OBJ)
	$(CC) -o $(PRINT_QUERY) $(CFLAGS) $(PRINT_QUERY_OBJ) $(LFLAGS) $(LIBS)

$(NAME).o: $(NAME).c $(NAME).h dicom.h dulprotocol.h lst.h condition.h \
dicom_services.h dicom_messages.h dicom_uids.h print_shared.h
	$(CC) -c $(CFLAGS) $(NAME).c

$(NAME2).o: $(NAME2).c dicom.h dulprotocol.h lst.h condition.h \
dicom_services.h dicom_messages.h dicom_uids.h dicom_print.h \
dicom_sq.h print_shared.h
	$(CC) -c $(CFLAGS) $(NAME2).c

$(PRINT_QUERY).o: $(PRINT_QUERY).c dicom.h dulprotocol.h lst.h condition.h \
dicom_services.h dicom_messages.h dicom_uids.h dicom_print.h \
dicom_sq.h print_shared.h
	$(CC) -c $(CFLAGS) $(PRINT_QUERY).c

Buttons.o: Buttons.c Buttons.h
	$(CC) -c $(CFLAGS) Buttons.c

MessageBox.o: MessageBox.c MessageBox.h
	$(CC) -c $(CFLAGS) MessageBox.c

Fields.o: Fields.h Fields.c
	$(CC) -c $(CFLAGS) Fields.c

display_icon.o: display_icon.h display_icon.c
	$(CC) -c $(CFLAGS) display_icon.c

display_study.o: display_study.h display_study.c
	$(CC) -c $(CFLAGS) display_study.c

display_error.o: display_error.h display_error.c
	$(CC) -c $(CFLAGS) display_error.c

display_message.o: display_message.h display_message.c
	$(CC) -c $(CFLAGS) display_message.c

print_shared.o: print_shared.c
	$(CC) -c $(CFLAGS) print_shared.c

print_callback.o: print_callback.c dicom_n_printer.h
	$(CC) -c $(CFLAGS) print_callback.c

print_client_private.o: print_client_private.c dicom.h lst.h condition.h \
dulprotocol.h dicom_objects.h dicom_messages.h dicom_services.h dicom_uids.h \
dicom_sq.h dicom_n_printer.h
	$(CC) -c $(CFLAGS) print_client_private.c

trial:	trial.o
	$(CC) -o trial $(CFLAGS) trial.o $(LFLAGS) $(LIBS)
 
trial.o: trial.c dicom.h dulprotocol.h lst.h condition.h \
dicom_services.h dicom_messages.h dicom_uids.h print_callback.h \
print_shared.h
	$(CC) -c $(CFLAGS) trial.c

test: test.o display_icon.o Buttons.o display_study.o
	$(CC) -o test $(CFLAGS) test.o display_icon.o Buttons.o display_study.o\
$(LFLAGS) $(XAW_LIBS) $(LIBS_X11) $(LIBS)

test.o: test.c
	$(CC) -c $(CFLAGS) test.c

test1: test1.o
	$(CC) -o test1 $(CFLAGS) test1.o $(LIBS)

test1.o: test1.c
	$(CC) -c $(CFLAGS) test1.c

lint:
	flexelint $(LINTOPTIONS) $(NAME).c Buttons.c MessageBox.c Fields.c \
print_shared.c display_icon.c display_study.c display_message.c
	flexelint $(LINTOPTIONS) $(NAME2).c print_shared.c
