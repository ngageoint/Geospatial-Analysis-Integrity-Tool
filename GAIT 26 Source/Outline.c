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
/* Outline.c: Methods for the Outline widget */
/* Copyright © 1994 Torgeir Veimo. */
/* See the README file for copyright details. */

#include <Xm/XmP.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h> 
#include "OutlineP.h"

static void Initialize
(
    Widget request,
    Widget newInit,
    ArgList args,
    Cardinal *numArgs
);
static void Resize
(
    Widget w
);

static void ChangeManaged
(
    Widget child
);

static void Redisplay
(
    Widget  w, 
    XEvent *event, 
    Region  region
);
static Boolean SetValues
(
    Widget current, 
    Widget request, 
    Widget newInit,
    ArgList args,
    Cardinal *numargs
);

static XtGeometryResult GeometryManager
(
    Widget widget, 
    XtWidgetGeometry *request,
    XtWidgetGeometry *reply
);

static XtGeometryResult QueryGeometry
(
    Widget widget,
    XtWidgetGeometry *intended,
    XtWidgetGeometry *reply
);


static void PreferredSize(XmOutlineWidget,
			  Dimension *,
			  Dimension *);

static void DoLayout(XmOutlineWidget);


static XtResource resources[] = {
  { XmNindentation, XmCIndentation, 
      XmRDimension, sizeof(Dimension),
      XtOffset(XmOutlineWidget, outline.indentation), 
      XtRString, "5" },

  { XmNspacing, XmCSpacing, 
      XmRDimension, sizeof(Dimension),
      XtOffset(XmOutlineWidget, outline.spacing), 
      XtRImmediate, (caddr_t) 2 },
  
  { XmNoutline, XmCOutline, 
      XmRBoolean, sizeof(Boolean),
      XtOffset(XmOutlineWidget, outline.draw_outline), 
      XtRString, "False" },

  { XmNmarginWidth, XmCMarginWidth, 
      XmRVerticalDimension, sizeof(Dimension),
      XtOffset(XmOutlineWidget, outline.marginWidth), 
      XtRImmediate, (caddr_t) 0 },

  { XmNmarginHeight, XmCMarginHeight, 
      XmRHorizontalDimension, sizeof(Dimension),
      XtOffset(XmOutlineWidget, outline.marginHeight), 
      XtRImmediate, (caddr_t) 0 },

  { XmNspacing, XmCSpacing, 
      XmRDimension, sizeof(Dimension),
      XtOffset(XmOutlineWidget, outline.spacing), 
      XtRImmediate, (caddr_t) 0 },

};


XmOutlineClassRec xmOutlineClassRec = { 
{
  /* core_class members */
  (WidgetClass) &xmManagerClassRec, /* superclass         */
  "XmOutline",                      /* class_name         */
  sizeof (XmOutlineRec),            /* widget_size        */
  NULL,                             /* class_initialize   */
  NULL,                             /* class_part_init    */  
  FALSE,                            /* class_inited       */  
  Initialize,                       /* initialize         */
  NULL,                             /* initialize_hook    */  
  XtInheritRealize,                 /* realize            */
  NULL,                             /* actions            */
  0,                                /* num_actions        */  
  resources,                        /* resources          */
  XtNumber(resources),              /* num_resources      */
  NULLQUARK,                        /* xrm_class          */
  TRUE,                             /* compress_motion    */  
  XtExposeCompressMaximal,          /* compress_exposure  */  
  TRUE,                             /* compress_enterleave*/  
  FALSE,                            /* visible_interest   */
  NULL,                             /* destroy            */
  Resize,                           /* resize             */
  Redisplay,                        /* expose             */
  SetValues,                        /* set_values         */
  NULL,                             /* set_values_hook    */
  XtInheritSetValuesAlmost,         /* set_values_almost  */
  NULL,                             /* get_values_hook    */  
  NULL,                             /* accept_focus       */
  XtVersion,                        /* version            */  
  NULL,                             /* callback_private   */
  XtInheritTranslations,            /* tm_table           */
  QueryGeometry,                    /* query_geometry     */  
  NULL,                             /* display_accelerator*/
  NULL,                             /* extension          */
},
{
  /* composite_class members */
  GeometryManager,                  /* geometry_manager   */
  ChangeManaged,                    /* change_managed     */
  XtInheritInsertChild,             /* insert_child       */  
  XtInheritDeleteChild,             /* delete_child       */  
  NULL,                             /* extension          */
},
{      
  /* constraint_class fields */
  NULL,                             /* resource list       */   
  0,                                /* num resources       */   
  0,                                /* constraint size     */   
  NULL,                             /* init proc           */   
  NULL,                             /* destroy proc        */   
  NULL,                             /* set values proc     */   
  NULL,                             /* extension           */
},
{ 
  /* XmManager class */
  XtInheritTranslations,            /* translations         */
  NULL,                             /* syn resources        */
  0,                                /* num syn_resources    */
  NULL,                             /* syn_cont_resources     */
  0,                                /* num_syn_cont_resources */
  XmInheritParentProcess,           /* parent_process       */
  NULL,                             /* extension            */    
},
{
  /* Outline class members */
  0,                                /* empty                */  
}
};

WidgetClass xmOutlineWidgetClass = (WidgetClass) &xmOutlineClassRec;

static void Initialize(Widget request, Widget newInit,
		       ArgList args, Cardinal *numArgs) 
{
  if (XtWidth(newInit) <= 0) 
    XtWidth(newInit) = 1;
  
  if (XtWidth(newInit) <= 0) 
    XtHeight(newInit) = 1;
} 
    
static void Resize(Widget w) 
{
  DoLayout((XmOutlineWidget) w);
} 
    
static void DoLayout(XmOutlineWidget ow) 
{
  Widget     child;
  int        i, indentation;
  Position   ypos;

  indentation = ow->outline.indentation + ow->outline.marginWidth;
  ypos = ow->outline.marginHeight;
  
  for (i = 0; i < (int)ow->composite.num_children; i++) {

    child = ow -> composite.children[i];

    if (child->core.managed) {

      _XmConfigureObject(child, (Position)indentation, ypos,
			 XtWidth(child), XtHeight(child),
			 XtBorderWidth(child));
      
      ypos += (ow->outline.spacing + XtHeight(child));
    }
  }
  
  /* Since the drawn outline might have been made invalid by the possible 
     new layout, the window is cleared, signaling a redraw operation. 
     
     It would probably be possible to add some intelligence here, detecting
     the changes made and only remove the drawn outline for that specific
     part.

     I have yet to test the blink effect that this method causes when the 
     outline widget have a large number of children. */
  
  if (XtIsRealized((Widget)ow))
    XClearArea(((Display *)XtDisplay((Widget)ow)), (Window)XtWindow((Widget)ow), 0, 0, 0, 0, True);
}
                    
static Boolean SetValues(Widget current, 
			 Widget request, 
			 Widget newInit,
			 ArgList args, 
			 Cardinal *numargs)
{
  XmOutlineWidget new_ow = (XmOutlineWidget) newInit;
  XmOutlineWidget cur_ow = (XmOutlineWidget) current;
  Boolean redraw = FALSE;

  if (new_ow->core.background_pixel != cur_ow->core.background_pixel)
    redraw = TRUE;
  
  if (new_ow->outline.indentation != cur_ow->outline.indentation)
    redraw = TRUE;
  
  if ((new_ow->outline.indentation != cur_ow->outline.indentation) ||
      (new_ow->outline.spacing != cur_ow->outline.spacing) ||
      (new_ow->outline.marginWidth != cur_ow->outline.marginWidth) ||
      (new_ow->outline.marginHeight != cur_ow->outline.marginHeight)) {
    DoLayout(new_ow);
    redraw = FALSE; /* DoLayout takes care of this. */
  }
  return (redraw);
}


static void Redisplay(Widget w, 
		      XEvent *event, 
		      Region region) 
{
  int x1, y1, x2, y2, i;
  XmOutlineWidget ow = (XmOutlineWidget) w;
  Widget child;

  _XmRedisplayGadgets(w, event, region);
  
  /* The outline is also drawn here. */

  if (ow->outline.draw_outline) {
    for (i = 0; i < (int)ow->composite.num_children; i++) {
      
      child = ow->composite.children[i];

      if (child->core.managed) {
	x1 = ow->outline.indentation / 2 + ow->outline.marginWidth;
	x2 = ow->outline.indentation;
	
	y1 = ow->outline.marginHeight;

	y2 = XtY(child) + ow->outline.indentation / 2
	  + ow->outline.marginHeight;
	
	XDrawLine(XtDisplay(w), XtWindow(w),
		  ow->manager.highlight_GC,
		  x1, y1, x1, y2);
	XDrawLine(XtDisplay(w), XtWindow(w),
		  ow->manager.highlight_GC,
		  x1, y2, x2, y2);
      }
    }
  }
}
        
static void PreferredSize(XmOutlineWidget ow,
			  Dimension *w,
			  Dimension *h) 
{
  int i, nchildren = 0;
  Dimension width = 0, height = 0;
  XtWidgetGeometry preferred;

  for (i = 0; i < (int)ow->composite.num_children; i++) {
    Widget child = ow->composite.children[i];
    
    if (child->core.managed) {

      XtQueryGeometry(child, NULL, &preferred);
      
      /* This is dangerous stuff! The preferred size isn't necessary
	 larger than the actual size, so we have to check both when
	 XmNpacking is set to XmPACK_NONE. Of course when XmNpacking
	 is set to XmPACK_TIGHT, the actual size should always be 
	 smaller or equal to the preferred size. */

      if (XtHeight(child) > preferred.height)
	height += XtHeight(child);
      else
	height += preferred.height;
      
      if (XtWidth(child) > preferred.width) {
	if (XtWidth(child) > width) 
	  width = XtWidth(child);
      } else {
	if (preferred.width > width)
	  width = preferred.width;
      }

      ++nchildren;
    }
  }

  width += (ow->outline.indentation + 2 * ow->outline.marginWidth);
  height += ((nchildren - 1) * ow->outline.spacing 
	     + 2 * ow->outline.marginHeight);
  
  *w = width; *h = height;
}


static XtGeometryResult QueryGeometry(Widget widget,
				      XtWidgetGeometry *intended,
				      XtWidgetGeometry *reply) 
{
  XmOutlineWidget ow = (XmOutlineWidget) widget;
  Dimension   w, h;
  
  if (intended->request_mode & (~(CWWidth | CWHeight))) 
      return  (XtGeometryYes);
  
  PreferredSize(ow, &w, &h);
  
  if (intended->request_mode & CWWidth &&
      intended->width == w &&
      intended->request_mode & CWHeight &&
      intended->height == h)
    return (XtGeometryYes);
  
  if (w == XtWidth(widget) &&
      h == XtHeight(widget))
    return (XtGeometryNo);
  
  reply->request_mode = CWWidth | CWHeight;
  reply->width = w;
  reply->height = h;
  return XtGeometryAlmost;
}
                                        
static XtGeometryResult GeometryManager(Widget widget, 
					XtWidgetGeometry *request,
					XtWidgetGeometry *reply) 
{
  XmOutlineWidget ow = (XmOutlineWidget) XtParent(widget);

  /* This is probably incomplete. It works for now though. */

  if (request->request_mode & (CWX | CWY)) 
    return (XtGeometryNo);
    
  if (request->request_mode & (CWWidth | CWHeight)) {
    Dimension w = 0, h = 0;
        
    if (request->request_mode & CWWidth) 
      XtWidth(widget) = request->width;
    
    if (request->request_mode & CWHeight) 
      XtHeight(widget) = request->height;
    
    PreferredSize(ow, &w, &h);

    if (w != XtWidth(ow) || h != XtHeight(ow)) {
      XtGeometryResult result;
      
      Dimension replyWidth, replyHeight;
      
      result = XtMakeResizeRequest((Widget) ow, w, h,
				   &replyWidth,
				   &replyHeight);

      if (result == XtGeometryAlmost) 
	XtMakeResizeRequest((Widget) ow,
			    replyWidth, replyHeight, 
			    NULL, NULL);       
    }     
    DoLayout(ow);
  }
  return (XtGeometryYes);
}
                                                    
static void ChangeManaged(Widget child) 
{
  XmOutlineWidget ow = (XmOutlineWidget) child; 
  Dimension w, h;
  
  PreferredSize(ow, &w, &h);
  
  if (w != XtWidth(ow) || h != XtHeight(ow)) {
    XtGeometryResult result;
    Dimension replyWidth, replyHeight;
    
    result = XtMakeResizeRequest((Widget) ow, w, h,
				 &replyWidth, &replyHeight);
    if (result == XtGeometryAlmost) 
      XtMakeResizeRequest((Widget) ow,
			  replyWidth, replyHeight, 
			  NULL, NULL);   
  }
  
  DoLayout(ow);

  _XmNavigChangeManaged(child);    
  
}

Widget XmCreateOutline(Widget parent,
		       char *name,
		       ArgList arglist,
		       Cardinal argcount)  
{
  return (XtCreateWidget(name, xmOutlineWidgetClass, 
			 parent, arglist, argcount));
}




