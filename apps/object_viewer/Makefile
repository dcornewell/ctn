# Makefile for worklist
# This makefile is used to build a Motif application, worklist.  It uses
# flags set in a global makefile (DICOM_MAKE)


include $(DICOM_MAKE)
NAME = object_viewer
OBJS = $(NAME).o topLevelShell1.o fileSelectionBoxDialog1.o format.o UxXt.o UxMethod.o

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -I.
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS            = $(LIBS_CTN_NODB) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_OS)

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LIBPATH) $(LIBS)

install:	$(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *BAK *.bak *% core

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
	$(INDENT) fileSelectionBoxDialog1.c
	$(INDENT) fileSelectionBoxDialog1.h
	$(INDENT) format.c
	$(INDENT) format.h
	$(INDENT) object_viewer.c
	$(INDENT) topLevelShell1.c
	$(INDENT) topLevelShell1.h

checkin:
	$(INDENT) SaAddLogin.h;			$(CI) SaAddLogin.h
	$(INDENT) SaConsoleCalls.h;		$(CI) SaConsoleCalls.h
	$(INDENT) SaPopCalls.h;			$(CI) SaPopCalls.h
	$(INDENT) Sut.h;			$(CI) Sut.h
	$(INDENT) SutList.h;			$(CI) SutList.h
	$(INDENT) SutProto.h;			$(CI) SutProto.h
	$(INDENT) UxMethod.c;			$(CI) UxMethod.c
	$(INDENT) UxXt.c;			$(CI) UxXt.c
	$(INDENT) WorkingDialog.h;		$(CI) WorkingDialog.h
	$(INDENT) fileSelectionBoxDialog1.c;	$(CI) fileSelectionBoxDialog1.c
	$(INDENT) fileSelectionBoxDialog1.h;	$(CI) fileSelectionBoxDialog1.h
						$(CI) fileSelectionBoxDialog1.i
	$(INDENT) format.c;			$(CI) format.c
	$(INDENT) format.h;			$(CI) format.h
	$(INDENT) object_viewer.c;		$(CI) object_viewer.c
						$(CI) object_viewer.op
						$(CI) object_viewer.prj
	$(INDENT) topLevelShell1.c;		$(CI) topLevelShell1.c
	$(INDENT) topLevelShell1.h;		$(CI) topLevelShell1.h
						$(CI) topLevelShell1.i
	rm -f $(NAME) *.o *BAK *.bak *% core


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
	$(CO) fileSelectionBoxDialog1.c
	$(CO) fileSelectionBoxDialog1.h
	$(CO) fileSelectionBoxDialog1.i
	$(CO) format.c
	$(CO) format.h
	$(CO) object_viewer.c
	$(CO) object_viewer.op
	$(CO) object_viewer.prj
	$(CO) topLevelShell1.c
	$(CO) topLevelShell1.h
	$(CO) topLevelShell1.i

lint:
	flexelint $(LINTOPTIONS) object_viewer.c format.c
