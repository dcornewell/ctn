#	Makefile for testing mb_query

# Server is a machine name.
SERVER = dinpacs
PORT = 2100

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

	dcm_make_object s1.dcm < s1.txt
	dcm_make_object s2.dcm < s2.txt
	dcm_make_object s3.dcm < s3.txt

eucjp:	q1 q2 q3 q4 q5 q6
romaji:	r1 r2 r3 r4 r5 r6

# The -a and -c flags give calling and called AE title.  These
# may differ for your institution.

q1:
	mb_query -f q1.dcm -a EUCJP -c EUCJP $(SERVER) $(PORT) > q1.iso2022
	ctn_cutil ISO2022JP EUCJP < q1.iso2022 > q1.eucjp
	cat q1.eucjp

q2:
	mb_query -f q2.dcm -a EUCJP -c EUCJP $(SERVER) $(PORT) > q2.iso2022
	ctn_cutil ISO2022JP EUCJP < q2.iso2022 > q2.eucjp
	cat q2.eucjp
q3:
	mb_query -f q3.dcm -a EUCJP -c EUCJP $(SERVER) $(PORT) > q3.iso2022
	ctn_cutil ISO2022JP EUCJP < q3.iso2022 > q3.eucjp
	cat q3.eucjp
q4:
	mb_query -f q4.dcm -a EUCJP -c EUCJP $(SERVER) $(PORT) > q4.iso2022
	ctn_cutil ISO2022JP EUCJP < q4.iso2022 > q4.eucjp
	cat q4.eucjp
q5:
	mb_query -l series -f q5.dcm -a EUCJP -c EUCJP $(SERVER) $(PORT) > q5.iso2022
	ctn_cutil ISO2022JP EUCJP < q5.iso2022 > q5.eucjp
	cat q5.eucjp
q6:
	mb_query -f q6.dcm -a EUCJP -c EUCJP $(SERVER) $(PORT) > q6.iso2022
	ctn_cutil ISO2022JP EUCJP < q6.iso2022 > q6.eucjp
	cat q6.eucjp

r1:
	mb_query -l series -f r1.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > r1.iso2022
	ctn_cutil ISO2022JP ROMAJI < r1.iso2022 > r1.romaji
	cat r1.romaji

r2:
	mb_query -l series -f r2.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > r2.iso2022
	ctn_cutil ISO2022JP ROMAJI < r2.iso2022 > r2.romaji
	cat r2.romaji

r3:
	mb_query -l series -f r3.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > r3.iso2022
	ctn_cutil ISO2022JP ROMAJI < r3.iso2022 > r3.romaji
	cat r3.romaji

r4:
	mb_query -l series -f r4.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > r4.iso2022
	ctn_cutil ISO2022JP ROMAJI < r4.iso2022 > r4.romaji

r5:
	mb_query -l series -f r5.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > r5.iso2022
	ctn_cutil ISO2022JP ROMAJI < r5.iso2022 > r5.romaji
	cat r5.romaji
	cat r5.romaji

r6:
	mb_query -l series -f r6.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > r6.iso2022
	ctn_cutil ISO2022JP ROMAJI < r6.iso2022 > r6.romaji
	cat r6.romaji

s1:
	mb_query -l series -f s1.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > s1.iso2022
	ctn_cutil ISO2022JP ROMAJI < s1.iso2022 > s1.romaji
	cat s1.romaji

s2:
	mb_query -l series -f s2.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > s2.iso2022
	ctn_cutil ISO2022JP ROMAJI < s2.iso2022 > s2.romaji
	cat s2.romaji

s3:
	mb_query -l series -f s3.dcm -a EUCJPROMAJI -c EUCJPROMAJI $(SERVER) $(PORT) > s3.iso2022
	ctn_cutil ISO2022JP ROMAJI < s3.iso2022 > s3.romaji
	cat s3.romaji
