#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define NUMBER 257
#define VALUE 258
typedef union{
	unsigned long	num;
	char		str[4096];
	char		*s;
	DCM_OBJECT	*o;
	DCM_ELEMENT	*e;
	LST_HEAD	*l;
	void		*v;
} YYSTYPE;
extern YYSTYPE yylval;
