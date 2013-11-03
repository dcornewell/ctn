# Makefile for rsa_ui
# This makefile is used to build a Motif application, rsa_ui.  This
# is a Results Storage Application.


include $(DICOM_MAKE)
NAME = rsa_ui
OBJS = $(NAME).o applicationShell1.o UxXt.o UxMethod.o support.o format.o

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -Dsun4 -DRUNTIME -I.

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_OS)

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LIBPATH) $(LIBS) -lm

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
	$(INDENT) rsa.h
	$(INDENT) rsa_ui.c
	$(INDENT) support.c

checkin:
	chmod u+w *h
	$(INDENT) SaAddLogin.h;			$(CI) SaAddLogin.h
	$(INDENT) SaConsoleCalls.h;		$(CI) SaConsoleCalls.h
	$(INDENT) SaPopCalls.h;			$(CI) SaPopCalls.h
	$(INDENT) Sut.h;			$(CI) Sut.h
	$(INDENT) SutList.h;			$(CI) SutList.h
	$(INDENT) SutProto.h;			$(CI) SutProto.h
	$(INDENT) UxMethod.c;			$(CI) UxMethod.c
	$(INDENT) UxXt.c;			$(CI) UxXt.c
	$(INDENT) WorkingDialog.h;		$(CI) WorkingDialog.h
	$(INDENT) applicationShell1.c;		$(CI) applicationShell1.c
	$(INDENT) applicationShell1.h;		$(CI) applicationShell1.h
						$(CI) applicationShell1.i
	$(INDENT) format.c;			$(CI) format.c
	$(INDENT) format.h;			$(CI) format.h
	$(INDENT) rsa.h;			$(CI) rsa.h
	$(INDENT) rsa_ui.c;			$(CI) rsa_ui.c
						$(CI) rsa_ui.op
						$(CI) rsa_ui.prj
	$(INDENT) support.c;			$(CI) support.c
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
	$(CO) rsa.h
	$(CO) rsa_ui.c
	$(CO) rsa_ui.op
	$(CO) rsa_ui.prj
	$(CO) support.c

lint:
	flexelint -DRUNTIME $(LINTOPTIONS) $(NAME).c support.c format.c
