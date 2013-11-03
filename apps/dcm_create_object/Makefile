#	Makefile for dcm_create_object

NAME = dcm_create_object
OBJ = gram_mod.o lex_mod.o $(NAME).o
YYDEBUG=
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DYYDEBUG
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core lex.c gram.c
	rm -f yacc.* y.tab.* lex.yy.c

indent:
	$(INDENT) $(NAME).c;
	$(INDENT) create.h; 
	
checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c;
	$(CI) lex.l;
	$(CI) gram.y;
	$(INDENT) create.h; $(CI) create.h;
	$(CI) gram_mod.c
	$(CI) gram.h
	$(CI) lex_mod.c
	rm -f $(NAME) *.o *.BAK *.bak *% core lex.c gram.c
	rm -f yacc.* y.tab.* lex.yy.c

checkout:
	$(CO) $(NAME).c;
	$(CO) lex.l;
	$(CO) gram.y; 
	$(CO) create.h; 
	$(CO) gram_mod.c
	$(CO) gram.h
	$(CO) lex_mod.c

$(NAME):	$(OBJ) 
	$(CC) -o $(NAME) -g $(OBJ) $(LFLAGS) $(LIBS)

relink:
	$(CC) -o $(NAME) -g $(OBJ) $(LFLAGS) $(LIBS)

$(NAME).o:	$(NAME).c dicom.h dicom_objects.h condition.h create.h

gram.o:	gram.c gram.h create.h dicom.h dicom_objects.h condition.h

lex.o:	lex.c gram.h create.h dicom.h dicom_objects.h condition.h

gram.c gram.h: gram.y create.h dicom.h dicom_objects.h condition.h
	yacc -dt gram.y
	mv y.tab.c gram.c
	mv y.tab.h gram.h

lex.c:	lex.l gram.h create.h dicom.h dicom_objects.h condition.h
	$(LEX) lex.l
	mv lex.yy.c lex.c

source:
	yacc -dt gram.y
	mv y.tab.c gram_mod.c
	mv y.tab.h gram.h

	flex lex.l
	mv lex.yy.c lex_mod.c

	echo "Make hand modifications if necessary"

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
