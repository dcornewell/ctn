%{
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Aniruddha Gokhale, 24-May-1995
** Intent:		This file defines lexical rules that define valid tokens
**			of a DICOM object
** Last Update:		$Author: asg $, $Date: 1995-07-07 16:41:06 $
** Source File:		$RCSfile: lex.l,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: lex.l,v $";
#include <stdio.h>
#include "modify.h"
#include "gram.h"

static int count = 0;
static unsigned short IsMultVal = 0;
static unsigned short IsTag = 0;
#define RET(a)		if (verbose) ECHO; return (a);
%}

/* define an additional state */
%s STATE_VALUE

%%
"//"[^\n]* {  /* ignore comments */ }
<STATE_VALUE>[(] {
    		BEGIN 0;
    		RET( *yytext);
	}
<STATE_VALUE>"{" {
		IsMultVal = 1;
		RET( *yytext); /* start of a multiply-valued string */
	}
<STATE_VALUE>"}" {
		IsMultVal = 0;
		BEGIN 0;	/* end of value. */
		RET( *yytext); /* start of a multiply-valued string */
	}
<STATE_VALUE>"<" {
		IsTag = 1;
		RET( *yytext);
	}
<STATE_VALUE>">" {
		IsTag = 0;
		RET( *yytext);
	}
<STATE_VALUE>"," {
		  RET( *yytext);
	}
<STATE_VALUE>"\""[^"]*"\"" {
    		strcpy(yylval.str, &yytext[1]);
		yylval.str[yyleng-2] = 0;
    		if (!IsMultVal)
			BEGIN 0;
    		RET( VALUE);
	}
<STATE_VALUE>[^ \t\n{},()<>]+ {
    		strcpy(yylval.str, yytext);
    		if (!IsMultVal && !IsTag)
			BEGIN 0;
    		RET( VALUE);
	}
[0-9a-fA-F]+ 	{
    		sscanf(yytext, "%x", &yylval.num);
    		count = (count + 1) % 2;
    		if (!count)
			BEGIN STATE_VALUE;
    		/* next state is STATE_VALUE */
    		RET( NUMBER);
	}
"(" |
")" 	{ RET( *yytext); }
[ \t\n]* { if (verbose) ECHO; /* nothing */ }
.  	{ printf("Unknown character: %c\n", *yytext); }
%%
