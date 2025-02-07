#define NUMBER 257
#define VALUE 258
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union{
	unsigned long	num;
	char		str[DICOM_LO_LENGTH+1];
	char		*s;
	DCM_ELEMENT	*e;
	LST_HEAD	*l;
	void		*v;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
