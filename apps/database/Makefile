#
#	Makefile for example database applications
#

TARGETS = dbaddpat dbaddstudy dbaddseries dbaddimage \
	dblstpats dblststudies dblstseries dblstimages \
	dbdelpat dbdelstudy dbdelseries dbdelimage \
	dbnew dbremove dbdump dbnumstudies dbdeloldeststudy
#	dbquerytest

include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(SOLARISUCB)

application:	$(TARGETS)

install:	$(TARGETS)
	$(INSTALL) dbaddimage $(DICOM_BIN)
	$(INSTALL) dbaddpat $(DICOM_BIN)
	$(INSTALL) dbaddseries $(DICOM_BIN)
	$(INSTALL) dbaddstudy $(DICOM_BIN)
	$(INSTALL) dbdump $(DICOM_BIN)
	$(INSTALL) dbnew $(DICOM_BIN)
	$(INSTALL) dbremove $(DICOM_BIN)
	$(INSTALL) dbdelimage $(DICOM_BIN)
	$(INSTALL) dbdelpat $(DICOM_BIN)
	$(INSTALL) dbdelseries $(DICOM_BIN)
	$(INSTALL) dbdelstudy $(DICOM_BIN)
	$(INSTALL) dblstimages $(DICOM_BIN)
	$(INSTALL) dblstpats $(DICOM_BIN)
	$(INSTALL) dblstseries $(DICOM_BIN)
	$(INSTALL) dblststudies $(DICOM_BIN)
	$(INSTALL) dbnumstudies $(DICOM_BIN)
	$(INSTALL) dbdeloldeststudy $(DICOM_BIN)
#	$(INSTALL) dbquerytest $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f core *.o *.*.bak *.BAK *% $(TARGETS) dbquerytest

checkin:
	$(INDENT) dbaddimage.c;		$(CI) dbaddimage.c
	$(INDENT) dbaddpat.c;		$(CI) dbaddpat.c
	$(INDENT) dbaddseries.c;	$(CI) dbaddseries.c
	$(INDENT) dbaddstudy.c;		$(CI) dbaddstudy.c
	$(INDENT) dbdump.c;		$(CI) dbdump.c
	$(INDENT) dbnew.c;		$(CI) dbnew.c
	$(INDENT) dbremove.c;		$(CI) dbremove.c
	$(INDENT) dbdelimage.c;		$(CI) dbdelimage.c
	$(INDENT) dbdelpat.c;		$(CI) dbdelpat.c
	$(INDENT) dbdelseries.c;	$(CI) dbdelseries.c
	$(INDENT) dbdelstudy.c;		$(CI) dbdelstudy.c
	$(INDENT) dblstimages.c;	$(CI) dblstimages.c
	$(INDENT) dblstpats.c;		$(CI) dblstpats.c
	$(INDENT) dblstseries.c;	$(CI) dblstseries.c
	$(INDENT) dblststudies.c;	$(CI) dblststudies.c
	$(INDENT) dbnumstudies.c;	$(CI) dbnumstudies.c
	$(INDENT) dbdeloldeststudy.c;	$(CI) dbdeloldeststudy.c
	$(INDENT) dbquerytest.c;	$(CI) dbquerytest.c
	rm -f core *.o *.*.bak *.BAK *% $(TARGETS) dbquerytest

checkout:
	$(CO) dbaddimage.c
	$(CO) dbaddpat.c
	$(CO) dbaddseries.c
	$(CO) dbaddstudy.c
	$(CO) dbdump.c
	$(CO) dbnew.c
	$(CO) dbremove.c
	$(CO) dbdelimage.c
	$(CO) dbdelpat.c
	$(CO) dbdelseries.c
	$(CO) dbdelstudy.c
	$(CO) dblstimages.c
	$(CO) dblstpats.c
	$(CO) dblstseries.c
	$(CO) dblststudies.c
	$(CO) dbnumstudies.c
	$(CO) dbdeloldeststudy.c
	$(CO) dbquerytest.c

indent:
	$(INDENT) dbaddimage.c
	$(INDENT) dbaddpat.c
	$(INDENT) dbaddseries.c
	$(INDENT) dbaddstudy.c
	$(INDENT) dbdump.c
	$(INDENT) dbnew.c
	$(INDENT) dbremove.c
	$(INDENT) dbdelimage.c
	$(INDENT) dbdelpat.c
	$(INDENT) dbdelseries.c
	$(INDENT) dbdelstudy.c
	$(INDENT) dblstimages.c
	$(INDENT) dblstpats.c
	$(INDENT) dblstseries.c
	$(INDENT) dblststudies.c
	$(INDENT) dbnumstudies.c
	$(INDENT) dbdeloldeststudy.c
	$(INDENT) dbquerytest.c

dbaddimage:	dbaddimage.o
	$(CC) -o dbaddimage dbaddimage.o -g $(LFLAGS) $(LIBS)

dbaddpat:		dbaddpat.o
	$(CC) -o dbaddpat dbaddpat.o -g $(LFLAGS) $(LIBS)

dbaddseries:	dbaddseries.o
	$(CC) -o dbaddseries dbaddseries.o -g $(LFLAGS) $(LIBS)

dbaddstudy:	dbaddstudy.o
	$(CC) -o dbaddstudy dbaddstudy.o -g $(LFLAGS) $(LIBS)

dbdump:	dbdump.o
	$(CC) -o dbdump dbdump.o -g $(LFLAGS) $(LIBS)

dbnew:		dbnew.o
	$(CC) -o dbnew dbnew.o -g $(LFLAGS) $(LIBS)

dbremove:	dbremove.o
	$(CC) -o dbremove dbremove.o -g $(LFLAGS) $(LIBS)

dbdelimage:	dbdelimage.o
	$(CC) -o dbdelimage dbdelimage.o -g $(LFLAGS) $(LIBS)

dbdelpat:		dbdelpat.o
	$(CC) -o dbdelpat dbdelpat.o -g $(LFLAGS) $(LIBS)

dbdelseries:	dbdelseries.o
	$(CC) -o dbdelseries dbdelseries.o -g $(LFLAGS) $(LIBS)

dbdelstudy:	dbdelstudy.o
	$(CC) -o dbdelstudy dbdelstudy.o -g $(LFLAGS) $(LIBS)

dblstimages:	dblstimages.o
	$(CC) -o dblstimages dblstimages.o -g $(LFLAGS) $(LIBS)

dblstpats:	dblstpats.o
	$(CC) -o dblstpats dblstpats.o -g $(LFLAGS) $(LIBS)

dblstseries:	dblstseries.o
	$(CC) -o dblstseries dblstseries.o -g $(LFLAGS) $(LIBS)

dblststudies:	dblststudies.o
	$(CC) -o dblststudies dblststudies.o -g $(LFLAGS) $(LIBS)

dbnumstudies:	dbnumstudies.o
	$(CC) -o dbnumstudies dbnumstudies.o -g $(LFLAGS) $(LIBS)

dbdeloldeststudy:	dbdeloldeststudy.o
	$(CC) -o dbdeloldeststudy dbdeloldeststudy.o -g $(LFLAGS) $(LIBS)

dbquerytest:	dbquerytest.o
	$(CC) -o dbquerytest dbquerytest.o -g $(LFLAGS) $(LIBS)

# This code to be retired, so don't worry about lint
lint:
