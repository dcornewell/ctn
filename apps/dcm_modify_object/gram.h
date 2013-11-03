#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union{
	unsigned long	num;
	char		str[DICOM_LO_LENGTH+1];
	char		*s;
	DCM_ELEMENT	*e;
	LST_HEAD	*l;
	void		*v;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	NUMBER	257
# define	VALUE	258


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */
