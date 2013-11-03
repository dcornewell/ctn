! UIMX ascii 2.5 key: 5077                                                      

*fileSelectionBoxDialog1.class: fileSelectionBoxDialog
*fileSelectionBoxDialog1.gbldecl: /*\
          Copyright (C) 1993, 1994, RSNA and Washington University\
 \
          The software and supporting documentation for the Radiological\
          Society of North America (RSNA) 1993, 1994 Digital Imaging and\
          Communications in Medicine (DICOM) Demonstration were developed\
          at the\
                  Electronic Radiology Laboratory\
                  Mallinckrodt Institute of Radiology\
                  Washington University School of Medicine\
                  510 S. Kingshighway Blvd.\
                  St. Louis, MO 63110\
          as part of the 1993, 1994 DICOM Central Test Node project for, and\
          under contract with, the Radiological Society of North America.\
 \
          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR\
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS\
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR\
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY\
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF\
          THE SOFTWARE IS WITH THE USER.\
 \
          Copyright of the software and supporting documentation is\
          jointly owned by RSNA and Washington University, and free access\
          is hereby granted as a license to use this software, copy this\
          software and prepare derivative works based upon this software.\
          However, any distribution of this software source code or\
          supporting documentation or derivative works (source code and\
          supporting documentation) must include the three paragraphs of\
          the copyright notice.\
*/\
/* Copyright marker.  Copyright will be inserted above.  Do not remove */\
\
/*\
**                     Electronic Radiology Laboratory\
**                   Mallinckrodt Institute of Radiology\
**                Washington University School of Medicine\
**\
** Module Name(s):        fileSelectionBoxDialog1\
**                        okCallback_fileSelectionBoxDialog1\
**                        cancelCB_fileSelectionBoxDialog1\
**\
** Author, Date:          Chander L. Sabharwal, 27-June-94  \
**      \
** Intenet:               This program pops up a file selection box \
**                        and lets the user select a file.\
**\
** Last Updates:          $Author: smm $, $Date: 1995-12-19 21:58:11 $\
**\
** Source File:           $RCSfile: fileSelectionBoxDialog1.i,v $\
**\
** Revision:              $Revision: 1.5 $\
**\
** Status:                $State: Exp $\
*/\
\
static char rcsid[] = "$Revision: 1.5 $ $RCSfile: fileSelectionBoxDialog1.i,v $";\
\
#include <stdio.h>\
\
/*char    info[500];*/\
\
void  copyWtext();\
void openfile();
*fileSelectionBoxDialog1.ispecdecl:
*fileSelectionBoxDialog1.funcdecl: swidget create_fileSelectionBoxDialog1(swidget UxParent)
*fileSelectionBoxDialog1.funcname: create_fileSelectionBoxDialog1
*fileSelectionBoxDialog1.funcdef: "swidget", "<create_fileSelectionBoxDialog1>(%)"
*fileSelectionBoxDialog1.argdecl: swidget UxParent;
*fileSelectionBoxDialog1.arglist: UxParent
*fileSelectionBoxDialog1.arglist.UxParent: "swidget", "%UxParent%"
*fileSelectionBoxDialog1.icode:
*fileSelectionBoxDialog1.fcode: return(rtrn);\

*fileSelectionBoxDialog1.auxdecl:
*fileSelectionBoxDialog1.name.source: public
*fileSelectionBoxDialog1.static: false
*fileSelectionBoxDialog1.name: fileSelectionBoxDialog1
*fileSelectionBoxDialog1.parent: NO_PARENT
*fileSelectionBoxDialog1.parentExpression: UxParent
*fileSelectionBoxDialog1.defaultShell: transientShell
*fileSelectionBoxDialog1.width: 568
*fileSelectionBoxDialog1.height: 548
*fileSelectionBoxDialog1.dialogType: "dialog_file_selection"
*fileSelectionBoxDialog1.isCompound: "true"
*fileSelectionBoxDialog1.compoundIcon: "fileboxD.xpm"
*fileSelectionBoxDialog1.compoundName: "fileSBox_Dialog"
*fileSelectionBoxDialog1.x: 420
*fileSelectionBoxDialog1.y: 272
*fileSelectionBoxDialog1.unitType: "pixels"
*fileSelectionBoxDialog1.okCallback: {\
/* okCallback_fileSelectionBoxDialog1\
**\
** Purpose:\
**      This subrouitne selects the DICOM filename from\
**      list of files in the selectionbox, calls a suroutine\
**      openfile and unmanages the selectionbox.\
**\
** Parameter Dictionary:\
**      cbs             input, pointer to the selected DICOM file\
**      filename        the selected DICOM filename \
**\
** Return Values:\
**      none\
**\
** Notes:\
**\
** Algorithm:\
**      Description of the algorithm (optional) and any other notes.\
**\
*/\
\
XmFileSelectionBoxCallbackStruct    *cbs;\
\
char     *filename;\
\
char     *info;\
\
cbs = (XmFileSelectionBoxCallbackStruct *)UxCallbackArg;\
\
if( (filename = (char *) malloc(100)) == NULL)\
  printf(" malloc filename failed\n");\
\
if( (info = (char *) malloc(500)) == NULL)\
  printf(" malloc info failed\n");\
\
if(!XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET,&filename))\
{\
 strcpy(info, "No DICOM filename selected from fileSelectionBoxDialog1, ERROR\n");\
 copyWtext(info);\
 free(info);\
}\
\
UxPopdownInterface(fileSelectionBoxDialog1);\
\
openfile(filename);\
\
\
}
*fileSelectionBoxDialog1.cancelCallback: {\
/* cancelCB_fileSelectionBOxDialog1\
**\
** Purpose:\
**      This subroutine lets the user close the file selection box \
**\
** Parameter Dictionary:\
**      None\
**\
** Return Values:\
**      None\
**\
** Notes:\
**\
** Algorithm:\
**      Description of the algorithm (optional) and any other notes.\
**\
*/ \
UxPopdownInterface(fileSelectionBoxDialog1);\
}

