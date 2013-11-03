# 
# 

include $(DICOM_MAKE)

TARGETS = archive_cgi.cgi
LFLAGS = -L$(CGIROOT) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
#LIBS = -lddr $(LIBS_CTN_STATIC) -lcgihtml $(LIBS_OS_SECURE_UCB)
LIBS = $(LIBS_CTN_STATIC) -lcgihtml $(LIBS_OS_SECURE_UCB)

CFLAGS += $(C_OPTS) -I$(CGIROOT)

OBJS = archive_cgi.o queryCTN.o dicomServerInterface.o

archive_cgi.cgi: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS) $(LIBS)

T_OBJS = t.o queryCTN.o dicomServerInterface.o

t: $(T_OBJS)
	$(CC) -o $@ $(T_OBJS) $(LFLAGS) $(LIBS)


all: $(TARGETS)

install: $(TARGETS)
	chmod a+x $(TARGETS)
	mv -f $(TARGETS) $(CGI_DIR)

clean:
	rm -f *.o $(TARGETS)
