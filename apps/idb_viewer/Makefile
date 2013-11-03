# Makefile for idb_viewer.  Application for examining image database.
#

include $(DICOM_MAKE)
NAME = idb_viewer
OBJS = $(NAME).o applicationShell1.o UxXt.o UxMethod.o format.o

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -Dsun4 -DRUNTIME -I.
LFLAGS = -L$(DICOM_LIB) $(LIBS_OS) $(STATIC)

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(LIBS_MOTIF) $(LIBS_X11)

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LIBPATH) $(LIBS) $(LFLAGS)

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
	$(INDENT) applicationShell1.c
	$(INDENT) applicationShell1.h
	$(INDENT) format.c
	$(INDENT) format.h
	$(INDENT) idb_viewer.c
	$(INDENT) idb_viewer.h

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
	$(INDENT) applicationShell1.c;	$(CI) applicationShell1.c
	$(INDENT) applicationShell1.h;	$(CI) applicationShell1.h
					$(CI) applicationShell1.i
	$(INDENT) format.c;		$(CI) format.c
	$(INDENT) format.h;		$(CI) format.h
	$(INDENT) idb_viewer.c;		$(CI) idb_viewer.c
	$(INDENT) idb_viewer.h;		$(CI) idb_viewer.h
					$(CI) idb_viewer.op
					$(CI) idb_viewer.prj
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
	$(CO) applicationShell1.c
	$(CO) applicationShell1.h
	$(CO) applicationShell1.i
	$(CO) format.c
	$(CO) format.h
	$(CO) idb_viewer.c
	$(CO) idb_viewer.h
	$(CO) idb_viewer.op
	$(CO) idb_viewer.prj

lint:
	flexelint $(LINTOPTIONS) idb_viewer.c format.c
