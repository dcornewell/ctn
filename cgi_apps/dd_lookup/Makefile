# 
# 

include $(DICOM_MAKE)

TARGETS = dd_lookup.cgi
LFLAGS = -L$(CGIROOT) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_STATIC) -lcgihtml $(LIBS_OS_SECURE_UCB)

CFLAGS += $(C_OPTS) -I$(CGIROOT)

OBJS = dd_lookup.o

dd_lookup.cgi: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS) $(LIBS)

all: $(TARGETS)

install: $(TARGETS)
	chmod a+x $(TARGETS)
	mv -f $(TARGETS) $(CGI_DIR)

clean:
	rm -f *.o $(TARGETS)
