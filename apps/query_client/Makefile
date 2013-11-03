# Makefile for worklist
# This makefile is used to build a Motif application, worklist.  It uses
# flags set in a global makefile (DICOM_MAKE)


include $(DICOM_MAKE)
NAME = query_client
OBJS = $(NAME).o topLevelShell1.o UxXt.o UxMethod.o format.o query_interface.o

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -Dsun4 -I.

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_OS)

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LIBPATH) $(LIBS)
	@echo "Done"

install:	$(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *bak *% core debug

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
	$(INDENT) format.c
	$(INDENT) format.h
	$(INDENT) patient_query.c
	$(INDENT) query_client.c
	$(INDENT) query_interface.c
	$(INDENT) query_interface.h
	$(INDENT) topLevelShell1.c
	$(INDENT) topLevelShell1.h


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
	$(INDENT) format.c;		$(CI) format.c
	$(INDENT) format.h;		$(CI) format.h
	$(INDENT) patient_query.c;	$(CI) patient_query.c
	$(INDENT) query_client.c;	$(CI) query_client.c
					$(CI) query_client.op
					$(CI) query_client.prj
	$(INDENT) query_interface.c;	$(CI) query_interface.c
	$(INDENT) query_interface.h;	$(CI) query_interface.h
	$(INDENT) topLevelShell1.c;	$(CI) topLevelShell1.c
	$(INDENT) topLevelShell1.h;	$(CI) topLevelShell1.h
					$(CI) topLevelShell1.i
	rm -f $(NAME) *.o *.BAK *bak *% core

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
	$(CO) format.c
	$(CO) format.h
	$(CO) patient_query.c
	$(CO) query_client.c
	$(CO) query_client.op
	$(CO) query_client.prj
	$(CO) query_interface.c
	$(CO) query_interface.h
	$(CO) topLevelShell1.c
	$(CO) topLevelShell1.h
	$(CO) topLevelShell1.i

lint:
	flexelint $(LINTOPTIONS) $(NAME).c format.c query_interface.c
