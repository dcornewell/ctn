# Makefile to make a static library for CTN software

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = -I/usr/local/src/jpeg-6b-12bit $(C_OPTS) -I$(DICOM_INCLUDE)

BASE_OBJS = \
 build.o cfg.o cmd_valid.o condition.o control.o ctnthread.o database.o \
 dcm.o dcm1.o dcmcond.o dcmdict.o dcmsupport.o ddr.o decode.o \
 delete.o  \
 dlroutines.o dulcond.o dulconstruct.o dulfsm.o dulparse.o \
 dulpresent.o dulprotocol.o dulsnoop.o dump.o event.o find.o fis.o fiscond.o \
 fisdelete.o fisget.o fisinsert.o get.o gq.o hapbuildinterp.o \
 hapbuildpatient.o hapbuildresults.o hapbuildstudy.o hapbuildvisit.o hapcond.o \
 hisdb.o hunk_man.o iap.o iapcond.o icon.o iconcond.o \
 ie.o iecond.o insert.o irs.o irscond.o lst.o lstcond.o \
 mancond.o messages.o motifutil.o move.o msgcond.o naction.o ncreate.o \
 ndelete.o neventreport.o nget.o nset.o print.o printcond.o private.o \
 record.o ref_item.o select.o send.o sequences.o set.o \
 snp.o snpcond.o sqcond.o srv1.o srv2.o srvcond.o storage.o \
 thrcond.o uid.o uidcond.o \
 update.o utility.o verify.o xutl.o dcm_compress_jpeg.o dcm_compress_jpeg8.o

# TBL_DB and TBLM_DB are defined in $DICOM_MAKE
OBJS = $(BASE_OBJS) $(TBL_DB) tblcond.o idb.o idbcond.o
OBJS_MBCTN = $(BASE_OBJS) dicom_chr.o $(TBLMB_DB) tblmbcond.o idbmb.o idbmbcond.o xutl_mb.o

BASE_HEADER_FILES = cfg.h condition.h ctn_os.h ctnthread.h dbquery.h decode.h dicom.h \
dicom_ddr.h dicom_ie.h dicom_irs.h dicom_messages.h \
dicom_objects.h dicom_platform.h dicom_print.h dicom_services.h \
dicom_sq.h dicom_uids.h dulfsm.h dulprotocol.h dulsnoop.h \
dulstructures.h fis.h gq.h hap.h hisdb.h hunk_man.h iap.h \
icon.h idb.h lst.h manage.h mut.h snp.h tables.h tbl.h \
tbl_msql.h tbl_psql.h tbl_sqlserver.h tbl_sybase.h tbl_ufs.h \
utility.h xutl.h

HEADER_FILES = $(BASE_HEADER_FILES) 
# tbl.h tbl_msql.h tbl_psql.h tbl_sqlserver.h\
# tbl_sybase.h tbl_ufs.h

HEADER_FILES_MBCTN = $(BASE_HEADER_FILES) dicom_chr.h idbmb.h tblmb.h tblmb_psql.h

libctn.a: $(OBJS)
#	cc $(CFLAGS) -G -o libctn.so $(OBJS)
	ar cvr libctn.a $(OBJS)

libctnmb.a: $(OBJS_MBCTN)
	ar cvr libctnmb.a $(OBJS_MBCTN)

libctn.so: $(OBJS)
	$(CC) $(CFLAGS) -G -o libctn.so $(OBJS)

#gcc -I/usr/local/src/jpeg-6b-12bit -g -DDEBUG -DLITTLE_ENDIAN_ARCHITECTURE -DLINUX -DSHARED_MEMORY -DSEMAPHORE -DX11 -DATHENA -DX11R4 -DUSLEEP -DMYSQLDB -I/home/horner/prj/ctn/include -I/usr/local/mysql/include -I/usr2/lesstif/lesstif-current/include/Motif-1.2 -I/usr/X11R6/include -DLONGSIZE=32 -DINTSIZE=32 -DSHORTSIZE=16 -DTIMEOFDAYARGS=2 -DMOTIF -I/home/horner/prj/ctn/include   -c -o dcm_compress_jpeg.o dcm_compress_jpeg.c

dcm_compress_jpeg8.o: dcm_compress_jpeg.c
	$(CC) -c -DJPEGBITDEPTH=8 $(C_OPTS) -I$(DICOM_INCLUDE) -o dcm_compress_jpeg8.o dcm_compress_jpeg.c

shared:	libctn.so
	cp libctn.so /usr/local/lib

windows-export:
	cp *.c *.h ../libwindows
	cp ctnlib.mak ../libwindows

install: libctn.a
	../apps/scripts/createdirectory $(DICOM_LIB)
	cp libctn.a $(DICOM_LIB)
	$(INSTALLINCLUDE) $(HEADER_FILES) $(DICOM_INCLUDE)

install-clean: install
	make clean

install.mb: libctnmb.a
	../apps/scripts/createdirectory $(DICOM_LIB)
	cp libctnmb.a $(DICOM_LIB)
	$(INSTALLINCLUDE) $(HEADER_FILES_MBCTN) $(DICOM_INCLUDE)

clean:
	rm -f *.a *.o

