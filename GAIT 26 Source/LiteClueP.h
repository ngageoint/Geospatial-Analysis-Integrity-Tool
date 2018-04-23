/************
GAIT (Geospatial Analysis Integrity Tool) is a geospatial data validation tool developed by the Institute for Defense Analyses (IDA) for the National Geospatial-Intelligence Agency (NGA).  

This source code was used to generate GAIT 26 executable software in accordance with Amendment 6 to Task Order DH-8-3815 under Contract HQ0034-14-D-0001.

IDA is furnishing this item "as is". IDA was not tasked or funded to generate developer documentation or to provide support for this source code. IDA does not provide any warranty of the item whatsoever, whether express, implied, or statutory, including, but not limited to, any warranty of fitness for a particular purpose or any warranty that the contents of the item will be error-free. In no event shall NGA or IDA be held liable for damages arising, directly or indirectly, from the use of this source code. 

This material may be reproduced by the U.S. Government pursuant to its unlimited use rights under DFARS 252.227-7014 [Feb 2014].

The Institute for Defense Analyses (IDA) is a Federally Funded Research and Development Center that provides scientific and technical expertise on issues important to national security to the Office of the Secretary of Defense, Joint Staff, Unified Commands, and Defense Agencies. 

© 2017 Institute for Defense Analyses
  4850 Mark Center Drive
  Alexandria, Virginia 22311-1882
  703.845-2000
  www.ida.org
************/
/* 
LiteClueP.h - Private definitions for LiteClue widget
	See LiteClue documentation

Copyright 1996 COMPUTER GENERATION, INC.,

The software is provided "as is", without warranty of any kind, express
or implied, including but not limited to the warranties of
merchantability, fitness for a particular purpose and noninfringement.
In no event shall Computer Generation, inc. nor the author be liable for
any claim, damages or other liability, whether in an action of contract,
tort or otherwise, arising from, out of or in connection with the
software or the use or other dealings in the software.

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation.

Author:
Gary Aviv 
Computer Generation, Inc.,
gary@compgen.com

*/

#ifndef _DEF_LiteClueP_h
#define _DEF_LiteClueP_h

#include <X11/ShellP.h>
/* Include public header file for this widget. */
#ifndef __VMS
# include "LiteClue.h"
#else
# include "LiteClue.h"
#endif
    
/* Doubly Linked List Processing */
struct list_thread_str
{
	struct list_thread_str * forw;	/* next pointer */
	struct list_thread_str * back;	/* prev pointer */
};
typedef struct list_thread_str  ListThread; 


typedef struct {
	int	nothing;	/* place holder */
} LiteClueClassPart;

/* Full class record declaration */
typedef struct _LiteClueClassRec {
	CoreClassPart	core_class;
	CompositeClassPart  composite_class;
	ShellClassPart  shell_class;
	OverrideShellClassPart  override_shell_class;
	LiteClueClassPart	LiteClue_class;
} LiteClueClassRec;

extern LiteClueClassRec xcgLiteClueClassRec;

/* New fields for the LiteClue widget record */
typedef struct {
	/* resources */
	Pixel foreground;

#if XtSpecificationRelease < 5 || defined(NO_FONT_SET)
	XFontStruct *fontset;	/* the font for text in box */
#else
	XFontSet fontset;	/* the font for text in box */
#endif
        int  waitPeriod;	/* the delay resource - pointer must be
				   in watched widget this long before
				   help is poped - in millisecs
				*/
        int  cancelWaitPeriod;	/* after help is popped-down - normal
				   wait period is cancelled for this
				   period - in millisecs
				*/

	/* -------- private state --------- */
	ListThread widget_list; 	/* list of widgets we are liteClue-ing */
	Dimension font_width;	/* width of '1' character */
	Dimension font_height;	/* height of font, rows are spaced using this */
	Dimension font_baseline;	/* relative displacement to baseline from top */
	GC text_GC;		/* for drawing text */
	XtIntervalId interval_id;	/* New field, holds timer id */
	Boolean	HelpIsUp;	/* the help is popup is up */
	Time	HelpPopDownTime;	/* the time at which help popup was popped down */
} LiteCluePart;


/*
 * Full instance record declaration
 */
typedef struct _LiteClueRec {
	CorePart		core;
	CompositePart  composite;
	ShellPart 	shell;
	OverrideShellPart override;
	LiteCluePart	liteClue;
} LiteClueRec;


#endif /* _DEF_LiteClueP_h */

