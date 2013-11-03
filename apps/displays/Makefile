#
#	Makefile for ctndisp and dcm_x_disp
#

TARGETS = ctndisp dcm_x_disp kill_ctndisp pq_ctndisp enq_ctndisp

CTNOBJ = ctndisp.o ctndisp_x_support.o image_utils.o
DCMOBJ = dcm_x_disp.o dcm_x_utils.o image_utils.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I/usr/openwin/include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_X11) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(TARGETS)

install: $(TARGETS)
	$(INSTALL) ctndisp $(DICOM_BIN)
	$(INSTALL) dcm_x_disp $(DICOM_BIN)
	$(INSTALL) kill_ctndisp $(DICOM_BIN)
	$(INSTALL) pq_ctndisp $(DICOM_BIN)
	$(INSTALL) enq_ctndisp $(DICOM_BIN)
install-clean:	install
	make clean

clean:
	rm -f *.o *.BAK *.bak *% core $(TARGETS)

checkin:
	$(INDENT) ctndisp.c;		$(CI) ctndisp.c
	$(INDENT) ctndisp.h;		$(CI) ctndisp.h
	$(INDENT) common_disp.h;	$(CI) common_disp.h
	$(INDENT) ctndisp_x_support.c;	$(CI) ctndisp_x_support.c
	$(INDENT) image_utils.c;	$(CI) image_utils.c
	$(INDENT) kill_ctndisp.c;	$(CI) kill_ctndisp.c
	$(INDENT) pq_ctndisp.c;		$(CI) pq_ctndisp.c
	$(INDENT) enq_ctndisp.c;	$(CI) enq_ctndisp.c
	$(INDENT) dcm_x_disp.c;		$(CI) dcm_x_disp.c
	$(INDENT) dcm_x_disp.h;		$(CI) dcm_x_disp.h
	$(INDENT) dcm_x_utils.c;	$(CI) dcm_x_utils.c
					$(CI) common_x.h
	rm -f *.o *.BAK *.bak *% core $(TARGETS)

indent:
	$(INDENT) ctndisp.c;
	$(INDENT) ctndisp.h;
	$(INDENT) common_disp.h;
	$(INDENT) ctndisp_x_support.c;
	$(INDENT) image_utils.c;
	$(INDENT) kill_ctndisp.c;
	$(INDENT) pq_ctndisp.c;
	$(INDENT) enq_ctndisp.c;
	$(INDENT) dcm_x_disp.c;
	$(INDENT) dcm_x_disp.h;
	$(INDENT) dcm_x_utils.c;

checkout:
	$(CO) ctndisp.c
	$(CO) ctndisp.h
	$(CO) common_disp.h
	$(CO) ctndisp_x_support.c
	$(CO) image_utils.c
	$(CO) kill_ctndisp.c
	$(CO) pq_ctndisp.c
	$(CO) enq_ctndisp.c
	$(CO) dcm_x_disp.c
	$(CO) dcm_x_disp.h
	$(CO) dcm_x_utils.c
	$(CO) common_x.h

ctndisp: $(CTNOBJ)
	$(XCC) -o ctndisp -g $(LFLAGS) $(CTNOBJ) $(LIBS) $(LIBS_X11) -lm

dcm_x_disp: $(DCMOBJ)
	$(XCC) -o dcm_x_disp -g $(LFLAGS) $(DCMOBJ) $(LIBS) $(LIBS_X11) -lm

pq_ctndisp: pq_ctndisp.o
	$(CC) -o pq_ctndisp pq_ctndisp.o -g $(LFLAGS) $(LIBS) -lm

kill_ctndisp: kill_ctndisp.o
	$(CC) -o kill_ctndisp kill_ctndisp.o -g $(LFLAGS) $(LIBS) -lm

enq_ctndisp: enq_ctndisp.o
	$(CC) -o enq_ctndisp enq_ctndisp.o -g $(LFLAGS) $(LIBS) -lm

ctndisp.o: ctndisp.c ctndisp.h common_disp.h dicom.h dicom.h
	$(XCC) -c $(CFLAGS) ctndisp.c

dcm_x_disp.o: dcm_x_disp.c dcm_x_disp.h common_disp.h dicom.h
	$(XCC) -c $(CFLAGS) dcm_x_disp.c

dcm_x_utils.o: dcm_x_utils.c dcm_x_disp.h dicom.h
	$(XCC) -c $(CFLAGS) dcm_x_utils.c

ctndisp_x_support.o: ctndisp_x_support.c ctndisp.h common_disp.h dicom.h
	$(XCC) -c $(CFLAGS) ctndisp_x_support.c

image_utils.o: image_utils.c dicom.h
	$(CC) -c $(CFLAGS) image_utils.c

pq_ctndisp.o: pq_ctndisp.c iqueues.h ctndisp.h common_disp.h
	$(CC) -c $(CFLAGS) pq_ctndisp.c

kill_ctndisp.o: kill_ctndisp.c iqueues.h ctndisp.h common_disp.h
	$(CC) -c $(CFLAGS) kill_ctndisp.c

enq_ctndisp.o: enq_ctndisp.c iqueues.h ctndisp.h common_disp.h
	$(CC) -c $(CFLAGS) enq_ctndisp.c

relink:
	$(XCC) -o dcm_x_disp -g $(LFLAGS) $(DCMOBJ) $(LIBS) $(LIBS_X11) -lm
lint:
	flexelint $(LINTOPTIONS) ctndisp.c ctndisp_x_support.c
	flexelint $(LINTOPTIONS) dcm_x_disp.c dcm_x_utils.c image_utils.c
	flexelint $(LINTOPTIONS) kill_ctndisp.c
	flexelint $(LINTOPTIONS) pq_ctndisp.c
	flexelint $(LINTOPTIONS) enq_ctndisp.c
