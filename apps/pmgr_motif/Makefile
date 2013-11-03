# Makefile for pmgr_motif
# This makefile is used to build a Motif application, pmgr_motif.  This
# is a print manager with a Motif interface.


include $(DICOM_MAKE)
NAME = pmgr_motif
OBJS = $(NAME).o applicationShell1.o UxXt.o UxMethod.o selectPrinterDialog.o \
filmInformationForm.o filmSessionForm.o filmBoxForm.o previewStudyForm.o \
driver.o format.o association.o studyListForm.o \
print_shared.o get_patientList.o icon.o BasicFilmBox_Standard.o \
PrintMessage.o MultipleSelection.o ImageAttribute.o 

CFLAGS = $(C_OPTS) $(CFLAGS_X11) $(CFLAGS_MOTIF) -DXT_CODE -DRUNTIME -I.

LIBPATH         = $(LIBPATH_MOTIF) $(LIBPATH_X11) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(LIBS_MOTIF) $(LIBS_X11) $(LIBS_OS)

application:	$(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) -g $(OBJS) $(LIBPATH) $(LIBS) -lm
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
	$(INDENT) applicationShell1.c
	$(INDENT) applicationShell1.h
	$(INDENT) association.c
	$(INDENT) association.h
	$(INDENT) driver.c
	$(INDENT) filmBoxForm.c
	$(INDENT) filmBoxForm.h
	$(INDENT) filmInformationForm.c
	$(INDENT) filmInformationForm.h
	$(INDENT) filmSessionForm.c
	$(INDENT) filmSessionForm.h
	$(INDENT) format.c
	$(INDENT) format.h
	$(INDENT) icon.c
	$(INDENT) pmgr_motif.c
	$(INDENT) previewStudyForm.c
	$(INDENT) previewStudyForm.h
	$(INDENT) print_client_app.h
	$(INDENT) print_shared.c
	$(INDENT) print_shared.h
	$(INDENT) selectPrinterDialog.c
	$(INDENT) selectPrinterDialog.h
	$(INDENT) get_patientList.c
	$(INDENT) studyListForm.c
	$(INDENT) display.h
	$(INDENT) studyListForm.h
	$(INDENT) BasicFilmBox_Standard.c
	$(INDENT) BasicFilmBox_Standard.h
	$(INDENT) PrintMessage.c
	$(INDENT) PrintMessage.h
	$(INDENT) MultipleSelection.c
	$(INDENT) MultipleSelection.h
	$(INDENT) ImageAttribute.c
	$(INDENT) ImageAttribute.h

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
	$(INDENT) association.c;		$(CI) association.c
	$(INDENT) association.h;		$(CI) association.h
	$(INDENT) driver.c;			$(CI) driver.c
	$(INDENT) filmBoxForm.c;		$(CI) filmBoxForm.c
	$(INDENT) filmBoxForm.h;		$(CI) filmBoxForm.h
						$(CI) filmBoxForm.i
	$(INDENT) filmInformationForm.c;	$(CI) filmInformationForm.c
	$(INDENT) filmInformationForm.h;	$(CI) filmInformationForm.h
						$(CI) filmInformationForm.i
	$(INDENT) filmSessionForm.c;		$(CI) filmSessionForm.c
	$(INDENT) filmSessionForm.h;		$(CI) filmSessionForm.h
						$(CI) filmSessionForm.i
	$(INDENT) format.c;			$(CI) format.c
	$(INDENT) format.h;			$(CI) format.h
	$(INDENT) icon.c;			$(CI) icon.c
	$(INDENT) pmgr_motif.c;			$(CI) pmgr_motif.c
						$(CI) pmgr_motif.op
						$(CI) pmgr_motif.prj
	$(INDENT) previewStudyForm.c;		$(CI) previewStudyForm.c
	$(INDENT) previewStudyForm.h;		$(CI) previewStudyForm.h
						$(CI) previewStudyForm.i
	$(INDENT) print_client_app.h;		$(CI) print_client_app.h
	$(INDENT) print_shared.c;		$(CI) print_shared.c
	$(INDENT) print_shared.h;		$(CI) print_shared.h
	$(INDENT) selectPrinterDialog.c;	$(CI) selectPrinterDialog.c
	$(INDENT) selectPrinterDialog.h;	$(CI) selectPrinterDialog.h
						$(CI) selectPrinterDialog.i
	$(INDENT) get_patientList.c;		$(CI) get_patientList.c
	$(INDENT) studyListForm.c;		$(CI) studyListForm.c
	$(INDENT) studyListForm.h;		$(CI) studyListForm.h
						$(CI) studyListForm.i
	$(INDENT) display.h;			$(CI) display.h
	$(INDENT) BasicFilmBox_Standard.c;	$(CI) BasicFilmBox_Standard.c
	$(INDENT) BasicFilmBox_Standard.h;	$(CI) BasicFilmBox_Standard.h
						$(CI) BasicFilmBox_Standard.i
	$(INDENT) PrintMessage.c;		$(CI) PrintMessage.c
	$(INDENT) PrintMessage.h;		$(CI) PrintMessage.h
						$(CI) PrintMessage.i
	$(INDENT) MultipleSelection.c;		$(CI) MultipleSelection.c
	$(INDENT) MultipleSelection.h;		$(CI) MultipleSelection.h
						$(CI) MultipleSelection.i

	$(INDENT) ImageAttribute.c;		$(CI) ImageAttribute.c
	$(INDENT) ImageAttribute.h;		$(CI) ImageAttribute.h
						$(CI) ImageAttribute.i



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
	$(CO) association.c
	$(CO) association.h
	$(CO) driver.c
	$(CO) filmBoxForm.c
	$(CO) filmBoxForm.h
	$(CO) filmBoxForm.i
	$(CO) filmInformationForm.c
	$(CO) filmInformationForm.h
	$(CO) filmInformationForm.i
	$(CO) filmSessionForm.c
	$(CO) filmSessionForm.h
	$(CO) filmSessionForm.i
	$(CO) format.c
	$(CO) format.h
	$(CO) icon.c
	$(CO) pmgr_motif.c
	$(CO) pmgr_motif.op
	$(CO) pmgr_motif.prj
	$(CO) previewStudyForm.c
	$(CO) previewStudyForm.h
	$(CO) previewStudyForm.i
	$(CO) print_client_app.h
	$(CO) print_shared.c
	$(CO) print_shared.h
	$(CO) selectPrinterDialog.c
	$(CO) selectPrinterDialog.h
	$(CO) selectPrinterDialog.i
	$(CO) get_patientList.c
	$(CO) studyListForm.c
	$(CO) studyListForm.h
	$(CO) studyListForm.i
	$(CO) display.h
	$(CO) BasicFilmBox_Standard.c
	$(CO) BasicFilmBox_Standard.h
	$(CO) BasicFilmBox_Standard.i
	$(CO) PrintMessage.c
	$(CO) PrintMessage.h
	$(CO) PrintMessage.i
	$(CO) MultipleSelection.i
	$(CO) MultipleSelection.c
	$(CO) MultipleSelection.h
	$(CO) ImageAttribute.c
	$(CO) ImageAttribute.h
	$(CO) ImageAttribute.i

lint:
#	flexelint $(LINTOPTIONS) $(NAME).c driver.c format.c association.c print_shared.c get_patientList.c icon.c
