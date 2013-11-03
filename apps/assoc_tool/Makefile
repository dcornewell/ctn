# Makefile for assoc_tool 
# This makefile is used to build a Motif application, assoc_tool.  It uses
# flags set in a global makefile (DICOM_MAKE)


include $(DICOM_MAKE)
NAME = assoc_tool
OBJS = $(NAME).o topLevelShell1.o UxXt.o UxMethod.o load_sopClasses.o netWork.o
CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -Dsun4 -I.

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS            = $(LIBS_CTN) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_OS) 

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LIBPATH) $(LIBS)
	@echo "Done"

install:	$(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *BAK *% *bak *% core

indent:
	chmod u+w *h
	$(INDENT) SaAddLogin.h
	$(INDENT) SaConsoleCalls.h
	$(INDENT) SaPopCalls.h
	$(INDENT) Sut.h
	$(INDENT) SutList.h
	$(INDENT) SutProto.h
	$(INDENT) UxMethod.c
	$(INDENT) UxXt.c
	$(INDENT) WorkingDialog.h
	$(INDENT) netWork.c
	$(INDENT) topLevelShell1.c
	$(INDENT) topLevelShell1.h
	$(INDENT) assoc_tool.c
	$(INDENT) load_sopClasses.c
	$(INDENT) format.h
	$(INDENT) prototypes.h
	$(INDENT) structures.h
checkin:
	chmod u+w *h
	$(INDENT) SaAddLogin.h;		$(CI) SaAddLogin.h
	$(INDENT) SaConsoleCalls.h;	$(CI) SaConsoleCalls.h
	$(INDENT) SaPopCalls.h;		$(CI) SaPopCalls.h
	$(INDENT) Sut.h;		$(CI) Sut.h
	$(INDENT) SutList.h;		$(CI) SutList.h
	$(INDENT) SutProto.h;		$(CI) SutProto.h
	$(INDENT) UxMethod.c;		$(CI) UxMethod.c
	$(INDENT) UxXt.c;		$(CI) UxXt.c
	$(INDENT) WorkingDialog.h;	$(CI) WorkingDialog.h
	$(INDENT) netWork.c;		$(CI) netWork.c
	$(INDENT) topLevelShell1.c;	$(CI) topLevelShell1.c
	$(INDENT) topLevelShell1.h;	$(CI) topLevelShell1.h
	$(INDENT) load_sopClasses.c;	$(CI) load_sopClasses.c
	$(INDENT) format.h;		$(CI) format.h
	$(INDENT) prototypes.h;		$(CI) prototypes.h
	$(INDENT) structures.h;		$(CI) structures.h
	$(INDENT) assoc_tool.c;		$(CI) assoc_tool.c
					$(CI) assoc_tool.op
					$(CI) assoc_tool.prj
					$(CI) sopclasslist.h
					$(CI) topLevelShell1.i


	rm -f $(NAME) *.o *bak *BAK *% core


checkout:
	$(CO) SaAddLogin.h
	$(CO) SaConsoleCalls.h
	$(CO) SaPopCalls.h
	$(CO) Sut.h
	$(CO) SutList.h
	$(CO) SutProto.h
	$(CO) UxMethod.c
	$(CO) UxXt.c
	$(CO) WorkingDialog.h
	$(CO) netWork.c
	$(CO) topLevelShell1.c
	$(CO) topLevelShell1.h
	$(CO) load_sopClasses.c
	$(CO) format.h
	$(CO) prototypes.h
	$(CO) structures.h
	$(CO) assoc_tool.c
	$(CO) assoc_tool.op
	$(CO) assoc_tool.prj
	$(CO) sopclasslist.h
	$(CO) topLevelShell1.i

lint:
#	flexelint $(LINTOPTIONS) netWork.c load_sopClasses.c
