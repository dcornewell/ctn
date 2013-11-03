# 	Makefile for print_server_display
#

NAME = print_server_display
OBJ = $(NAME).o display_prn.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I/usr/openwin/include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_X11) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.BAK *.bak *% *.o core

checkin:
	$(INDENT) $(NAME).c ;           $(CI) $(NAME).c
	$(INDENT) display_prn.h ;       $(CI) display_prn.h
	$(INDENT) display_prn.c ;       $(CI) display_prn.c
	rm -f $(NAME) *.BAK *.bak *% *.o core

indent:
	$(INDENT) $(NAME).c ;
	$(INDENT) display_prn.h ;
	$(INDENT) display_prn.c ;

checkout:
	$(CO) $(NAME).c
	$(CO) display_prn.h
	$(CO) display_prn.c	

$(NAME): $(OBJ)
	$(XCC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(LIBS_XAW) $(LIBS_X11) -lm

$(NAME).o: $(NAME).c dicom.h dicom_objects.h display_prn.h
	$(XCC) -c $(CFLAGS) $(NAME).c

display_prn.o: display_prn.h display_prn.c dicom_print.h
	$(XCC) -c $(CFLAGS) display_prn.c

lint:
	flexelint $(LINTOPTIONS) $(NAME).c display_prn.c
