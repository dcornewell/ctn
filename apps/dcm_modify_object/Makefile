#	Makefile for dcm_modify_object

NAME = dcm_modify_object
#OBJ = gram_mod.o lex_mod.o $(NAME).o
OBJ = gram.o lex.o $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core lex.c gram.c
	rm -rf debug
	rm -f yacc.* y.tab.* lex.yy.c

indent:
	$(INDENT) $(NAME).c;
	$(INDENT) modify.h; 
	
checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c;
	$(CI) lex.l;
	$(CI) gram.y;
	$(CI) gram_mod.c
	$(CI) lex_mod.c
	$(CI) gram.h
	$(INDENT) modify.h; $(CI) modify.h;
	rm -f $(NAME) *.o *.BAK *.bak *% core lex.c gram.c
	rm -f yacc.* y.tab.* lex.yy.c

checkout:
	$(CO) $(NAME).c;
	$(CO) lex.l;
	$(CO) gram.y; 
	$(CO) modify.h; 
	$(CO) gram_mod.c
	$(CO) gram.h
	$(CO) lex_mod.c

$(NAME):	$(OBJ) 
	$(CC) -o $(NAME) -g $(OBJ) $(LFLAGS) $(LIBS) $(LIBS_LEX)

$(NAME).o:	$(NAME).c dicom.h dicom_objects.h condition.h modify.h

gram_mod.o:	gram_mod.c gram.h modify.h dicom.h dicom_objects.h condition.h

lex_mod.o:	lex_mod.c gram.h modify.h dicom.h dicom_objects.h condition.h

gram.c gram.h: gram.y modify.h dicom.h dicom_objects.h condition.h
	yacc -d gram.y
	mv y.tab.c gram.c
	mv y.tab.h gram.h

lex.c:	lex.l gram.h modify.h dicom.h dicom_objects.h condition.h
	$(LEX) lex.l
	mv lex.yy.c lex.c

source:
	yacc -d gram.y
	mv y.tab.c gram_mod.c
	mv y.tab.h gram.h

	flex lex.l
	mv lex.yy.c lex_mod.c

	echo "May be necessary to hand-edit gram_mod and lex_mod for NT"

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
