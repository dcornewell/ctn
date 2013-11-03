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
** Last Update:		$Author: smm $, $Date: 2004-11-15 01:49:51 $
** Source File:		$RCSfile: gram.y,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: gram.y,v $";
#include "create.h"	/* common declarations */
CONDITION
	cond;		/* status code returned by DICOM facilities */
%}
%union{
	unsigned long	num;
	char		str[4096];
	char		*s;
	DCM_OBJECT	*o;
	DCM_ELEMENT	*e;
	LST_HEAD	*l;
	void		*v;
};
%token <num> 	NUMBER 
%token <str> 	VALUE
%type  <o>	dicom_object element_list item
%type  <e>	element
%type  <v>	value 
%type  <l>	sequence value_list tag_list
%type  <s>    	multiple_value
%type  <str>	tag
%%
dicom_object
	: element_list 
	  {
		object = $$;
	  }
	;

element_list
	: element_list element
	  {
	    /* Add element $2 to DCM Object $1 and then assign $1 to $$ */
	    /*cond = DCM_AddElement(&$1, $2);*/
	    cond = DCM_ModifyElements(&$1, $2, 1, 0, 0, 0);
	    free($2);
	    if (cond != DCM_NORMAL){
		printf("DCM_AddElement: Cannot add element to object\n");
		COND_DumpConditions();
		exit(1);
	    }
	    $$ = $1;
	  }
	| element
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
	    cond = DCM_ModifyElements(&obj, $1, 1, 0, 0, 0);
	    free($1);
	    if (cond != DCM_NORMAL){
		printf("DCM_AddElement: Cannot add element to object\n");
		COND_DumpConditions();
		exit(1);
	    }
	    $$ = obj;
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
	     DCM_SEQUENCE_ITEM
		*dsi;

	     dsi = (DCM_SEQUENCE_ITEM *)malloc(sizeof(DCM_SEQUENCE_ITEM));
	     if (dsi == NULL){
		printf("malloc: Out of memory allocating dcm_sequence_item\n");
		exit(1);
	     }
	     dsi->object = $2;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&$1, dsi);
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
	     dsi->object = $1;
	     /* now enqueue this item */
	     cond = LST_Enqueue(&lh, dsi);
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
	;
%%
