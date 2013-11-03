# Makefile for cfg_ctn_tables
# This makefile is used to build a Motif application, cfg_ctn_tables.  It uses
# flags set in a global makefile (DICOM_MAKE)


include $(DICOM_MAKE)
NAME = cfg_ctn_tables
OBJS = $(NAME).o applicationShell1.o format.o support.o UxXt.o UxMethod.o \
applicationForm.o securityForm.o printServerCFGForm.o FISAccessForm.o \
imageStorageAccessForm.o imageStorageControlForm.o \
groupNamesForm.o videoImageForm.o

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -DRUNTIME -I.
LFLAGS =

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS            = $(LIBS_CTN) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_CTN) $(LIBS_OS)

application:	cfg_ctn_tables

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LFLAGS) $(LIBPATH) $(LIBS) -lm

install:	$(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *BAK *.bak *% core

indent:
	chmod u+w *h
	$(INDENT) FISAccessForm.c
	$(INDENT) FISAccessForm.h
	$(INDENT) SaAddLogin.h
	$(INDENT) SaConsoleCalls.h
	$(INDENT) SaPopCalls.h
	$(INDENT) Sut.h
	$(INDENT) SutList.h
	$(INDENT) SutProto.h
	$(INDENT) UxMethod.c
	$(INDENT) UxXt.c
	$(INDENT) WorkingDialog.h
	$(INDENT) applicationForm.c
	$(INDENT) applicationForm.h
	$(INDENT) applicationShell1.c
	$(INDENT) applicationShell1.h
	$(INDENT) cfg_ctn_tables.c
	$(INDENT) format.c
	$(INDENT) format.h
	$(INDENT) groupNamesForm.c
	$(INDENT) groupNamesForm.h
	$(INDENT) imageStorageAccessForm.c
	$(INDENT) imageStorageAccessForm.h
	$(INDENT) imageStorageControlForm.c
	$(INDENT) imageStorageControlForm.h
	$(INDENT) printServerCFGForm.c
	$(INDENT) printServerCFGForm.h
	$(INDENT) securityForm.c
	$(INDENT) securityForm.h
	$(INDENT) support.c
	$(INDENT) support.h
	$(INDENT) videoImageForm.c
	$(INDENT) videoImageForm.h

checkin:
	chmod u+w *h
	$(INDENT) FISAccessForm.c;		$(CI) FISAccessForm.c
	$(INDENT) FISAccessForm.h;		$(CI) FISAccessForm.h
						$(CI) FISAccessForm.i
	$(INDENT) SaAddLogin.h;			$(CI) SaAddLogin.h
	$(INDENT) SaConsoleCalls.h;		$(CI) SaConsoleCalls.h
	$(INDENT) SaPopCalls.h;			$(CI) SaPopCalls.h
	$(INDENT) Sut.h;			$(CI) Sut.h
	$(INDENT) SutList.h;			$(CI) SutList.h
	$(INDENT) SutProto.h;			$(CI) SutProto.h
	$(INDENT) UxMethod.c;			$(CI) UxMethod.c
	$(INDENT) UxXt.c;			$(CI) UxXt.c
	$(INDENT) WorkingDialog.h;		$(CI) WorkingDialog.h
	$(INDENT) applicationForm.c;		$(CI) applicationForm.c
	$(INDENT) applicationForm.h;		$(CI) applicationForm.h
						$(CI) applicationForm.i
	$(INDENT) applicationShell1.c;		$(CI) applicationShell1.c
	$(INDENT) applicationShell1.h;		$(CI) applicationShell1.h
						$(CI) applicationShell1.i
	$(INDENT) cfg_ctn_tables.c;		$(CI) cfg_ctn_tables.c
						$(CI) cfg_ctn_tables.op
						$(CI) cfg_ctn_tables.prj
	$(INDENT) format.c;			$(CI) format.c
	$(INDENT) format.h;			$(CI) format.h
	$(INDENT) groupNamesForm.c;		$(CI) groupNamesForm.c
	$(INDENT) groupNamesForm.h;		$(CI) groupNamesForm.h
						$(CI) groupNamesForm.i
	$(INDENT) imageStorageAccessForm.c;	$(CI) imageStorageAccessForm.c
	$(INDENT) imageStorageAccessForm.h;	$(CI) imageStorageAccessForm.h
						$(CI) imageStorageAccessForm.i
	$(INDENT) imageStorageControlForm.c;	$(CI) imageStorageControlForm.c
	$(INDENT) imageStorageControlForm.h;	$(CI) imageStorageControlForm.h
						$(CI) imageStorageControlForm.i
	$(INDENT) printServerCFGForm.c;		$(CI) printServerCFGForm.c
	$(INDENT) printServerCFGForm.h;		$(CI) printServerCFGForm.h
						$(CI) printServerCFGForm.i
	$(INDENT) securityForm.c;		$(CI) securityForm.c
	$(INDENT) securityForm.h;		$(CI) securityForm.h
						$(CI) securityForm.i
	$(INDENT) support.c;			$(CI) support.c
	$(INDENT) support.h;			$(CI) support.h
	$(INDENT) videoImageForm.c;		$(CI) videoImageForm.c
	$(INDENT) videoImageForm.h;		$(CI) videoImageForm.h
						$(CI) videoImageForm.i
	rm -f $(NAME) *.o *BAK *.bak *% core

checkout:
	$(CO) FISAccessForm.c
	$(CO) FISAccessForm.h
	$(CO) FISAccessForm.i
	$(CO) SaAddLogin.h
	$(CO) SaConsoleCalls.h
	$(CO) SaPopCalls.h
	$(CO) Sut.h
	$(CO) SutList.h
	$(CO) SutProto.h
	$(CO) UxMethod.c
	$(CO) UxXt.c
	$(CO) WorkingDialog.h
	$(CO) applicationForm.c
	$(CO) applicationForm.h
	$(CO) applicationForm.i
	$(CO) applicationShell1.c
	$(CO) applicationShell1.h
	$(CO) applicationShell1.i
	$(CO) cfg_ctn_tables.c
	$(CO) cfg_ctn_tables.op
	$(CO) cfg_ctn_tables.prj
	$(CO) format.c
	$(CO) format.h
	$(CO) groupNamesForm.c
	$(CO) groupNamesForm.h
	$(CO) groupNamesForm.i
	$(CO) imageStorageAccessForm.c
	$(CO) imageStorageAccessForm.h
	$(CO) imageStorageAccessForm.i
	$(CO) imageStorageControlForm.c
	$(CO) imageStorageControlForm.h
	$(CO) imageStorageControlForm.i
	$(CO) printServerCFGForm.c
	$(CO) printServerCFGForm.h
	$(CO) printServerCFGForm.i
	$(CO) securityForm.c
	$(CO) securityForm.h
	$(CO) securityForm.i
	$(CO) support.c
	$(CO) support.h
	$(CO) videoImageForm.c
	$(CO) videoImageForm.h
	$(CO) videoImageForm.i
lint:
	flexelint $(LINTOPTIONS) format.c support.c
