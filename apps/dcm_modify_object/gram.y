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
** Intent:		This file defines a Context Free grammar for defining
**			DICOM attributes that make up a DICOM object definition
** Last Update:		$Author: asg $, $Date: 1995-07-07 16:41:14 $
** Source File:		$RCSfile: gram.y,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: gram.y,v $";
#include "modify.h"	/* common declarations */
CONDITION
	cond;		/* status code returned by DICOM facilities */
%}
%union{
	unsigned long	num;
	char		str[DICOM_LO_LENGTH+1];
	char		*s;
	DCM_ELEMENT	*e;
	LST_HEAD	*l;
	void		*v;
};
%token <num> 	NUMBER 
%token <str> 	VALUE
%type  <l>	dicom_object element_list sequence item value_list tag_list
%type  <e>	element
%type  <v>	value; 
%type  <s>	multiple_value
%type  <str>	tag
%%
dicom_object
	: element_list 
	  {
		modlist = $$;
	  }
	;

element_list
	: element_list element
	  {
	     APP_ELEMLIST *ael;

	     ael = (APP_ELEMLIST *)malloc(sizeof(APP_ELEMLIST));
	     if (ael == NULL){
		printf("malloc: failed to acquire memory for element list\n");
		exit(1);
	     }
	     ael->e = $2;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&$1, ael);
	     if (cond != LST_NORMAL){
		printf("LST_Enqueue: Cannot enqueue elementlist item\n");
		COND_DumpConditions();
		exit(1);
	     }
	     $$ = $1;
	  }
	| element
	  {
	     LST_HEAD *lh;
	     APP_ELEMLIST *ael;

	     lh = LST_Create();
	     if (lh == NULL){
		printf("LST_Create: Out of memory creating list");
		COND_DumpConditions();
		exit(1);
	     }

	     ael = (APP_ELEMLIST *)malloc(sizeof(APP_ELEMLIST));
	     if (ael == NULL){
		printf("malloc: failed to acquire memory for element list\n");
		exit(1);
	     }
	     ael->e = $1;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&lh, ael);
	     if (cond != LST_NORMAL){
		printf("LST_Enqueue: Cannot enqueue elementlist item\n");
		COND_DumpConditions();
		exit(1);
	     }
	     $$ = lh;
	  }
	;

element
	: NUMBER NUMBER value
	  {
		DCM_ELEMENT
			*e;

		cond = makeElement($1, $2, $3, &e);
		if (cond != DCM_NORMAL){
		   printf("Cannot make element for TAG (%x, %x)\n",
				$1, $2);
		   exit(1);
		}
		$$ = e;
	  }
	;

value
	: VALUE				{ $$ = strdup($1); }	
	| tag				{ $$ = strdup($1); }
	| multiple_value		{ $$ = $1; }
	| sequence			{ $$ = $1; }
	;

multiple_value
	: '{' value_list '}'
		{ 
			 char *str;

			 cond = DCM_ListToString($2, 
				STRUCT_OFFSET(MULTVAL_ITEM,str), &str);
			 if (cond != DCM_NORMAL){
			     printf("DCM_ListToString failed\n");
			     COND_DumpConditions();
			     exit(1);
			 }
			 $$ = str;
		}
	| '{' tag_list '}'
		{ 
			 char *str;

			 cond = DCM_ListToString($2, 
				STRUCT_OFFSET(MULTVAL_ITEM,str), &str);
			 if (cond != DCM_NORMAL){
			     printf("DCM_ListToString failed\n");
			     COND_DumpConditions();
			     exit(1);
			 }
			 $$ = str;
		}
	;

value_list
	: value_list ',' VALUE
	  {
	      MULTVAL_ITEM *mvi;

	      mvi = (MULTVAL_ITEM *)malloc(sizeof(MULTVAL_ITEM));
	      if (mvi == NULL){
		 printf("malloc: Out of memory for multi-valued item\n");
		 exit(1);
	      }
	      strcpy(mvi->str, $3);
	      cond = LST_Enqueue(&$1,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      $$ = $1;
	  }
	| VALUE
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
	      strcpy(mvi->str, $1);
	      cond = LST_Enqueue(&lh,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      $$ = lh;
	  }
	;

tag_list
	: tag_list ',' tag
	  {
	      MULTVAL_ITEM *mvi;

	      mvi = (MULTVAL_ITEM *)malloc(sizeof(MULTVAL_ITEM));
	      if (mvi == NULL){
		 printf("malloc: Out of memory for multi-valued item\n");
		 exit(1);
	      }
	      strcpy(mvi->str, $3);
	      cond = LST_Enqueue(&$1,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      $$ = $1;
	  }
	| tag
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
	      strcpy(mvi->str, $1);
	      cond = LST_Enqueue(&lh,mvi);
	      if (cond != LST_NORMAL){
		 printf("LST_Enqueue: failed to enqueue multi-valued item\n");
		 COND_DumpConditions();
		 exit(1);
	      }
	      $$ = lh;
	  }
	;

tag
	: '<' VALUE ',' VALUE '>'
	  {
	     unsigned short
		group, elem;

	     sscanf($2, "%hx",&group);
	     sscanf($4, "%hx",&elem);
	     sprintf($$,"%ld",DCM_MAKETAG(group,elem));
	  }
	;

sequence
	: sequence item
	  {
	     APP_ITEMSEQ
		*ais;

	     ais = (APP_ITEMSEQ *)malloc(sizeof(APP_ITEMSEQ));
	     if (ais == NULL){
		printf("malloc: Out of memory allocating APP_ITEMSEQ\n");
		exit(1);
	     }
	     ais->ael = $2;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&$1, ais);
	     if (cond != LST_NORMAL){
		printf("LST_Enqueue: Cannot enqueue sequence item\n");
		COND_DumpConditions();
		exit(1);
	     }
	     $$ = $1;
	  }
	| item
	  {
	     LST_HEAD
		*lh;
	     APP_ITEMSEQ
		*ais;

	     lh = LST_Create();
	     if (lh == NULL){
		printf("LST_Create: cannot create sequence list\n");
		COND_DumpConditions();
		exit(1);
	     }

	     ais = (APP_ITEMSEQ *)malloc(sizeof(APP_ITEMSEQ));
	     if (ais == NULL){
		printf("malloc: Out of memory allocating APP_ITEMSEQ\n");
		exit(1);
	     }
	     ais->ael = $1;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&lh, ais);
	     if (cond != LST_NORMAL){
		printf("LST_Enqueue: Cannot enqueue sequence item\n");
		COND_DumpConditions();
		exit(1);
	     }
	     $$ = lh;
	  }
	;

item
	: '(' element_list ')'
	  {
		$$ = $2;
	  }
	| '(' ')'	/* empty. No modifications in this item */
	  {
		$$ = NULL;
	  }
	;
%%
