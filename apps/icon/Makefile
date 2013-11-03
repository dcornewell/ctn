#	Makefile for icon applications

NAME = icon_append_index
NAME2 = icon_append_file
NAME3 = icon_dump_index
NAME4 = icon_dump_file
NAME5 = create_print_entry
OBJ = $(NAME).o
OBJ2 = $(NAME2).o
OBJ3 = $(NAME3).o
OBJ4 = $(NAME4).o
OBJ5 = $(NAME5).o

include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB) $(SOLARISUCB)

application:	$(NAME) $(NAME2) $(NAME3) $(NAME4) $(NAME5)

install: $(NAME) $(NAME2) $(NAME3) $(NAME4) $(NAME5)
	$(INSTALL) $(NAME) $(DICOM_BIN)
	$(INSTALL) $(NAME2) $(DICOM_BIN)
	$(INSTALL) $(NAME3) $(DICOM_BIN)
	$(INSTALL) $(NAME4) $(DICOM_BIN)
	$(INSTALL) $(NAME5) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) $(NAME2) $(NAME3) $(NAME4) $(NAME5) *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c
	$(INDENT) $(NAME2).c
	$(INDENT) $(NAME3).c
	$(INDENT) $(NAME4).c
	$(INDENT) $(NAME5).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) $(NAME2).c; $(CI) $(NAME2).c
	$(INDENT) $(NAME3).c; $(CI) $(NAME3).c
	$(INDENT) $(NAME4).c; $(CI) $(NAME4).c
	$(INDENT) $(NAME5).c; $(CI) $(NAME5).c
	rm -f $(NAME) $(NAME2) $(NAME3) $(NAME4) $(NAME5) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) $(NAME2).c
	$(CO) $(NAME3).c
	$(CO) $(NAME4).c
	$(CO) $(NAME5).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJ) $(LFLAGS) $(LIBS)

$(NAME).o: $(NAME).c
	$(CC) -c $(CFLAGS) $(NAME).c 

$(NAME2): $(OBJ2)
	$(CC) -o $(NAME2) $(CFLAGS) $(OBJ2) $(LFLAGS) $(LIBS)

$(NAME2).o: $(NAME2).c
	$(CC) -c $(CFLAGS) $(NAME2).c 

$(NAME3): $(OBJ3)
	$(CC) -o $(NAME3) $(CFLAGS) $(OBJ3) $(LFLAGS) $(LIBS)

$(NAME3).o: $(NAME3).c
	$(CC) -c $(CFLAGS) $(NAME3).c 

$(NAME4): $(OBJ4)
	$(CC) -o $(NAME4) $(CFLAGS) $(OBJ4) $(LFLAGS) $(LIBS)

$(NAME4).o: $(NAME4).c
	$(CC) -c $(CFLAGS) $(NAME4).c 

$(NAME5): $(OBJ5)
	$(CC) -o $(NAME5) $(CFLAGS) $(OBJ5) $(LFLAGS) $(LIBS)

$(NAME5).o: $(NAME5).c
	$(CC) -c $(CFLAGS) $(NAME5).c 

lint:
	flexelint $(LINTOPTIONS) icon_append_index.c
	flexelint $(LINTOPTIONS) icon_append_file.c
	flexelint $(LINTOPTIONS) icon_dump_index.c
	flexelint $(LINTOPTIONS) icon_dump_file.c
	flexelint $(LINTOPTIONS) create_print_entry.c
