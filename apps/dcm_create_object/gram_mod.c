#ifndef lint
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
static int yygrowstack();
#define YYPREFIX "yy"
#line 2 "gram.y"
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
** Intent:		This file defines a Context Free grammar for defining
**			DICOM attributes that make up a DICOM object definition
** Last Update:		$Author: smm $, $Date: 2004-11-15 01:49:40 $
** Source File:		$RCSfile: gram_mod.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: gram_mod.c,v $";
#include "create.h"	/* common declarations */
CONDITION
	cond;		/* status code returned by DICOM facilities */
#line 25 "gram.y"
typedef union{
	unsigned long	num;
	char		str[4096];
	char		*s;
	DCM_OBJECT	*o;
	DCM_ELEMENT	*e;
	LST_HEAD	*l;
	void		*v;
} YYSTYPE;
#line 50 "y.tab.c"
#define YYERRCODE 256
#define NUMBER 257
#define VALUE 258
const short yylhs[] = {                                        -1,
    0,    1,    1,    3,    4,    4,    4,    4,    8,    8,
    6,    6,    7,    7,    9,    5,    5,    2,
};
const short yylen[] = {                                         2,
    1,    2,    1,    3,    1,    1,    1,    1,    3,    3,
    3,    1,    3,    1,    5,    2,    1,    3,
};
const short yydefred[] = {                                      0,
    0,    0,    0,    3,    0,    2,    5,    0,    0,    0,
   17,    4,    0,    7,    6,   12,    0,    0,   14,    0,
    0,   16,    9,    0,   10,    0,    0,   18,   11,   13,
    0,   15,
};
const short yydgoto[] = {                                       2,
    3,   11,    4,   12,   13,   17,   18,   14,   15,
};
const short yysindex[] = {                                   -249,
 -248,    0, -249,    0,  -40,    0,    0,  -58, -247, -249,
    0,    0,  -30,    0,    0,    0,  -39,  -37,    0,  -32,
  -38,    0,    0, -245,    0,  -46, -243,    0,    0,    0,
  -45,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,   16,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
const short yygindex[] = {                                      0,
    8,    6,    3,    0,    0,    0,    0,    0,   -4,
};
#define YYTABLESIZE 258
const short yytable[] = {                                      10,
    8,    9,   28,   19,   24,    6,   26,    1,    5,   10,
   20,   27,   29,    9,   31,    1,   32,   21,   22,    9,
    0,   30,    0,    6,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    8,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    8,    0,    0,   23,    0,   25,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   16,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    7,    1,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    8,
};
const short yycheck[] = {                                      40,
    0,   60,   41,    8,   44,    3,   44,  257,  257,   40,
  258,   44,  258,   60,  258,    0,   62,   10,   13,   60,
   -1,   26,   -1,   21,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  123,   -1,   -1,  125,   -1,  125,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  257,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 258
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'<'",0,"'>'",0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"NUMBER","VALUE",
};
const char * const yyrule[] = {
"$accept : dicom_object",
"dicom_object : element_list",
"element_list : element_list element",
"element_list : element",
"element : NUMBER NUMBER value",
"value : VALUE",
"value : tag",
"value : multiple_value",
"value : sequence",
"multiple_value : '{' value_list '}'",
"multiple_value : '{' tag_list '}'",
"value_list : value_list ',' VALUE",
"value_list : VALUE",
"tag_list : tag_list ',' tag",
"tag_list : tag",
"tag : '<' VALUE ',' VALUE '>'",
"sequence : sequence item",
"sequence : item",
"item : '(' element_list ')'",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 45 "gram.y"
{
		object = yyval.o;
	  }
break;
case 2:
#line 52 "gram.y"
{
	    /* Add element $2 to DCM Object $1 and then assign $1 to $$ */
	    /*cond = DCM_AddElement(&$1, $2);*/
	    cond = DCM_ModifyElements(&yyvsp[-1].o, yyvsp[0].e, 1, 0, 0, 0);
	    free(yyvsp[0].e);
	    if (cond != DCM_NORMAL){
		printf("DCM_AddElement: Cannot add element to object\n");
		COND_DumpConditions();
		exit(1);
	    }
	    yyval.o = yyvsp[-1].o;
	  }
break;
case 3:
#line 65 "gram.y"
{
	    DCM_OBJECT
		*obj;

	    cond = DCM_CreateObject(&obj, 0);
	    if (cond != DCM_NORMAL){
		printf("DCM_CreateObject: cannot create main object\n");
		COND_DumpConditions();
		exit(1);
	    }
	    /*cond = DCM_AddElement(&obj, $1);*/
	    cond = DCM_ModifyElements(&obj, yyvsp[0].e, 1, 0, 0, 0);
	    free(yyvsp[0].e);
	    if (cond != DCM_NORMAL){
		printf("DCM_AddElement: Cannot add element to object\n");
		COND_DumpConditions();
		exit(1);
	    }
	    yyval.o = obj;
	  }
break;
case 4:
#line 89 "gram.y"
{
		DCM_ELEMENT
			*e;

		cond = makeElement(yyvsp[-2].num, yyvsp[-1].num, yyvsp[0].v, &e);
		if (cond != DCM_NORMAL){
		   printf("Cannot make element for TAG (%x, %x)\n",
				yyvsp[-2].num, yyvsp[-1].num);
		   exit(1);
		}
		yyval.e = e;
	  }
break;
case 5:
#line 104 "gram.y"
{ yyval.v = strdup(yyvsp[0].str); }
break;
case 6:
#line 105 "gram.y"
{ yyval.v = strdup(yyvsp[0].str); }
break;
case 7:
#line 106 "gram.y"
{ yyval.v = yyvsp[0].s; }
break;
case 8:
#line 107 "gram.y"
{ yyval.v = yyvsp[0].l; }
break;
case 9:
#line 112 "gram.y"
{ 
			 char *str;

			 cond = DCM_ListToString(yyvsp[-1].l, 
				STRUCT_OFFSET(MULTVAL_ITEM,str), &str);
			 if (cond != DCM_NORMAL){
			     printf("DCM_ListToString failed\n");
			     COND_DumpConditions();
			     exit(1);
			 }
			 yyval.s = str;
		}
break;
case 10:
#line 125 "gram.y"
{ 
			 char *str;

			 cond = DCM_ListToString(yyvsp[-1].l, 
				STRUCT_OFFSET(MULTVAL_ITEM,str), &str);
			 if (cond != DCM_NORMAL){
			     printf("DCM_ListToString failed\n");
			     COND_DumpConditions();
			     exit(1);
			 }
			 yyval.s = str;
		}
break;
case 11:
#line 141 "gram.y"
{
	      MULTVAL_ITEM *mvi;

	      mvi = (MULTVAL_ITEM *)malloc(sizeof(MULTVAL_ITEM));
	      if (mvi == NULL){
		 printf("malloc: Out of memory for multi-valued item\n");
		 exit(1);
	      }
	      strcpy(mvi->str, yyvsp[0].str);
	      cond = LST_Enqueue(&yyvsp[-2].l,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      yyval.l = yyvsp[-2].l;
	  }
break;
case 12:
#line 159 "gram.y"
{
	      LST_HEAD *lh;
	      MULTVAL_ITEM *mvi;

	      lh = LST_Create();
	      if (lh == NULL){
		 printf("LST_Create: Out of memory for multi-valued list\n");
		 exit(1);
	      }
	      mvi = (MULTVAL_ITEM *)malloc(sizeof(MULTVAL_ITEM));
	      if (mvi == NULL){
		 printf("malloc: Out of memory for multi-valued item\n");
		 exit(1);
	      }
	      strcpy(mvi->str, yyvsp[0].str);
	      cond = LST_Enqueue(&lh,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      yyval.l = lh;
	  }
break;
case 13:
#line 186 "gram.y"
{
	      MULTVAL_ITEM *mvi;

	      mvi = (MULTVAL_ITEM *)malloc(sizeof(MULTVAL_ITEM));
	      if (mvi == NULL){
		 printf("malloc: Out of memory for multi-valued item\n");
		 exit(1);
	      }
	      strcpy(mvi->str, yyvsp[0].str);
	      cond = LST_Enqueue(&yyvsp[-2].l,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      yyval.l = yyvsp[-2].l;
	  }
break;
case 14:
#line 204 "gram.y"
{
	      LST_HEAD *lh;
	      MULTVAL_ITEM *mvi;

	      lh = LST_Create();
	      if (lh == NULL){
		 printf("LST_Create: Out of memory for multi-valued list\n");
		 exit(1);
	      }
	      mvi = (MULTVAL_ITEM *)malloc(sizeof(MULTVAL_ITEM));
	      if (mvi == NULL){
		 printf("malloc: Out of memory for multi-valued item\n");
		 exit(1);
	      }
	      strcpy(mvi->str, yyvsp[0].str);
	      cond = LST_Enqueue(&lh,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      yyval.l = lh;
	  }
break;
case 15:
#line 231 "gram.y"
{
	     unsigned short
		group, elem;

	     sscanf(yyvsp[-3].str, "%hx",&group);
	     sscanf(yyvsp[-1].str, "%hx",&elem);
	     sprintf(yyval.str,"%ld",DCM_MAKETAG(group,elem));
	  }
break;
case 16:
#line 243 "gram.y"
{
	     DCM_SEQUENCE_ITEM
		*dsi;

	     dsi = (DCM_SEQUENCE_ITEM *)malloc(sizeof(DCM_SEQUENCE_ITEM));
	     if (dsi == NULL){
		printf("malloc: Out of memory allocating dcm_sequence_item\n");
		exit(1);
	     }
	     dsi->object = yyvsp[0].o;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&yyvsp[-1].l, dsi);
	     if (cond != LST_NORMAL){
		printf("LST_Enqueue: Cannot enqueue sequence item\n");
		COND_DumpConditions();
		exit(1);
	     }
	     yyval.l = yyvsp[-1].l;
	  }
break;
case 17:
#line 263 "gram.y"
{
	     LST_HEAD
		*lh;
	     DCM_SEQUENCE_ITEM
		*dsi;

	     lh = LST_Create();
	     if (lh == NULL){
		printf("LST_Create: cannot create sequence list\n");
		COND_DumpConditions();
		exit(1);
	     }

	     dsi = (DCM_SEQUENCE_ITEM *)malloc(sizeof(DCM_SEQUENCE_ITEM));
	     if (dsi == NULL){
		printf("malloc: Out of memory allocating dcm_sequence_item\n");
		exit(1);
	     }
	     dsi->object = yyvsp[0].o;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&lh, dsi);
	     if (cond != LST_NORMAL){
		printf("LST_Enqueue: Cannot enqueue sequence item\n");
		COND_DumpConditions();
		exit(1);
	     }
	     yyval.l = lh;
	  }
break;
case 18:
#line 295 "gram.y"
{
		yyval.o = yyvsp[-1].o;
	  }
break;
#line 669 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
