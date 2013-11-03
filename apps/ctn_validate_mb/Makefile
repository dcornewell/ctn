#	Makefile for ctn_validate_mb

NAME = ctn_validate_mb
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(DEBUG_LIBS)

$(NAME).o: $(NAME).c dicom.h condition.h dicom_objects.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c

H = /opt/home/erl/smm
eucjp:
	./ctn_validate_mb -e eucjp $(H)/images/canon/f01_o_x_o.xxx
#	./ctn_validate_mb -v ~/images/canon/f02_o_o_x.xxx
#	./ctn_validate_mb -v ~/images/canon/f03_o_o_o.xxx
#	./ctn_validate_mb -v ~/images/canon/f08_x_x_o.xxx
#	./ctn_validate_mb -v ~/images/canon/f09_x_o_x.xxx
#	./ctn_validate_mb -v ~/images/canon/f10_x_o_o.xxx

romaji:
	./ctn_validate_mb -v -e eucjpromaji $(H)/images/canon/f04_h_x_x.xxx
#	./ctn_validate_mb -v -g eucjpromaji $(H)/images/canon/f05_h_x_o.xxx
#	./ctn_validate_mb -v -g eucjpromaji $(H)/images/canon/f06_h_o_x.xxx
#	./ctn_validate_mb -v -g eucjpromaji $(H)/images/canon/f07_h_o_o.xxx
