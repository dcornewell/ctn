# Makefile for worklist
# This makefile is used to build a Motif application, wlma_ui.  It uses
# flags set in a global makefile (DICOM_MAKE)


include $(DICOM_MAKE)
NAME = wlma_ui
OBJS = $(NAME).o topLevelShell1.o format.o UxXt.o UxMethod.o databaseForm.o demonstrationForm.o support.o

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -Dsun4 -DRUNTIME -I.

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_OS)

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LIBPATH) $(LIBS)
	@echo "Done"

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
	$(INDENT) databaseForm.c
	$(INDENT) databaseForm.h
	$(INDENT) demonstrationForm.c
	$(INDENT) demonstrationForm.h
	$(INDENT) format.c
	$(INDENT) format.h
	$(INDENT) support.c
	$(INDENT) topLevelShell1.c
	$(INDENT) topLevelShell1.h
	$(INDENT) wlma.h
	$(INDENT) wlma_ui.c

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
	$(INDENT) databaseForm.c;	$(CI) databaseForm.c
	$(INDENT) databaseForm.h;	$(CI) databaseForm.h
					$(CI) databaseForm.i
	$(INDENT) demonstrationForm.c;	$(CI) demonstrationForm.c
	$(INDENT) demonstrationForm.h;	$(CI) demonstrationForm.h
					$(CI) demonstrationForm.i
	$(INDENT) format.c;		$(CI) format.c
	$(INDENT) format.h;		$(CI) format.h
	$(INDENT) support.c;		$(CI) support.c
	$(INDENT) topLevelShell1.c;	$(CI) topLevelShell1.c
	$(INDENT) topLevelShell1.h;	$(CI) topLevelShell1.h
					$(CI) topLevelShell1.i
	$(INDENT) wlma.h;		$(CI) wlma.h
	$(INDENT) wlma_ui.c;		$(CI) wlma_ui.c
					$(CI) wlma_ui.op
					$(CI) wlma_ui.prj
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
	$(CO) databaseForm.c
	$(CO) databaseForm.h
	$(CO) databaseForm.i
	$(CO) demonstrationForm.c
	$(CO) demonstrationForm.h
	$(CO) demonstrationForm.i
	$(CO) format.c
	$(CO) format.h
	$(CO) support.c
	$(CO) topLevelShell1.c
	$(CO) topLevelShell1.h
	$(CO) topLevelShell1.i
	$(CO) wlma.h
	$(CO) wlma_ui.c
	$(CO) wlma_ui.op
	$(CO) wlma_ui.prj

lint:
	flexelint -DRUNTIME $(LINTOPTIONS) $(NAME).c format.c support.c
