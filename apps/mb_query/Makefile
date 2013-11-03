#	Makefile for mb_query

NAME = mb_query
OBJ = $(NAME).o

include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
#LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS)
LIBS = $(LIBS_CTN)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug
	rm -rf *eucjp *iso2022 *dcm

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

$(NAME).o: $(NAME).c dicom.h dulprotocol.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c

data:
	dcm_make_object q1.dcm < q1.txt
	dcm_make_object q2.dcm < q2.txt
	dcm_make_object q3.dcm < q3.txt
	dcm_make_object q4.dcm < q4.txt
	dcm_make_object q5.dcm < q5.txt
	dcm_make_object q6.dcm < q6.txt

	dcm_make_object r1.dcm < r1.txt
	dcm_make_object r2.dcm < r2.txt
	dcm_make_object r3.dcm < r3.txt
	dcm_make_object r4.dcm < r4.txt
	dcm_make_object r5.dcm < r5.txt
	dcm_make_object r6.dcm < r6.txt

q1:
	./mb_query -f q1.dcm -a EUCJP -c EUCJP dinpacs 2100 > q1.iso2022
	ctn_cutil ISO2022JP EUCJP < q1.iso2022 > q1.eucjp
	cat q1.eucjp

q2:
	./mb_query -f q2.dcm -a EUCJP -c EUCJP dinpacs 2100 > q2.iso2022
	ctn_cutil ISO2022JP EUCJP < q2.iso2022 > q2.eucjp
	cat q2.eucjp
q3:
	./mb_query -f q3.dcm -a EUCJP -c EUCJP dinpacs 2100 > q3.iso2022
	ctn_cutil ISO2022JP EUCJP < q3.iso2022 > q3.eucjp
	cat q3.eucjp
q4:
	./mb_query -f q4.dcm -a EUCJP -c EUCJP dinpacs 2100 > q4.iso2022
	ctn_cutil ISO2022JP EUCJP < q4.iso2022 > q4.eucjp
	cat q4.eucjp
q5:
	./mb_query -l series -f q5.dcm -a EUCJP -c EUCJP dinpacs 2100 > q5.iso2022
	ctn_cutil ISO2022JP EUCJP < q5.iso2022 > q5.eucjp
	cat q5.eucjp
q6:
	./mb_query -f q6.dcm -a EUCJP -c EUCJP dinpacs 2100 > q6.iso2022
	ctn_cutil ISO2022JP EUCJP < q6.iso2022 > q6.eucjp
	cat q6.eucjp

r1:
	./mb_query -l series -f r1.dcm -a EUCJPROMAJI -c EUCJPROMAJI dinpacs 2100 > r1.iso2022
	ctn_cutil ISO2022JP ROMAJI < r1.iso2022 > r1.romaji
	cat r1.romaji

r2:
	./mb_query -l series -f r2.dcm -a EUCJPROMAJI -c EUCJPROMAJI dinpacs 2100 > r2.iso2022
	ctn_cutil ISO2022JP ROMAJI < r2.iso2022 > r2.romaji
	cat r2.romaji

r3:
	./mb_query -l series -f r3.dcm -a EUCJPROMAJI -c EUCJPROMAJI dinpacs 2100 > r3.iso2022
	ctn_cutil ISO2022JP ROMAJI < r3.iso2022 > r3.romaji
	cat r3.romaji

r4:
	./mb_query -l series -f r4.dcm -a EUCJPROMAJI -c EUCJPROMAJI dinpacs 2100 > r4.iso2022
	ctn_cutil ISO2022JP ROMAJI < r4.iso2022 > r4.romaji

r5:
	./mb_query -l series -f r5.dcm -a EUCJPROMAJI -c EUCJPROMAJI dinpacs 2100 > r5.iso2022
	ctn_cutil ISO2022JP ROMAJI < r5.iso2022 > r5.romaji
	cat r5.romaji
	cat r5.romaji

r6:
	./mb_query -l series -f r6.dcm -a EUCJPROMAJI -c EUCJPROMAJI dinpacs 2100 > r6.iso2022
	ctn_cutil ISO2022JP ROMAJI < r6.iso2022 > r6.romaji
	cat r6.romaji
