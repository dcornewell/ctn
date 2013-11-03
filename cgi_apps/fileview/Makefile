# 
# 
include $(DICOM_MAKE)

TARGETS = fileview.cgi
LFLAGS = -L$(CGIROOT) -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_STATIC) -lcgihtml $(LIBS_OS_SECURE_UCB)

CFLAGS += $(C_OPTS) -I$(CGIROOT)

OBJS = fileview.o

fileview.cgi: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS) $(LIBS)

all: $(TARGETS)

install: $(TARGETS)
	chmod a+x $(TARGETS)
	mv -f $(TARGETS) $(CGI_DIR)

clean:
	rm -f *.o $(TARGETS)
