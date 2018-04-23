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
/* HANDLEP.h: Private header file for the Handle widget. */
/* Copyright © 1994 Torgeir Veimo. */
/* See the README file for copyright details. */

#ifndef HANDLEP_H
#define HANDLEP_H

#include "Handle.h"
#include <Xm/ManagerP.h>

typedef struct _XmHandleClassPart {
    int empty;
} XmHandleClassPart;
        
typedef struct _XmHandleClassRec {
  CoreClassPart        core_class;
  CompositeClassPart   composite_class;
  ConstraintClassPart  constraint_class;
  XmManagerClassPart   manager_class;
  XmHandleClassPart    handle_class;
} XmHandleClassRec;
    
typedef struct {
  Widget	child; /* child widget handeled by this widget */
  Dimension	marginWidth, marginHeight, spacing;
} XmHandlePart;
    
typedef struct _XmHandleRec {
  CorePart        core;
  CompositePart   composite;
  ConstraintPart  constraint;
  XmManagerPart   manager;
  XmHandlePart    handle;
} XmHandleRec;

#endif /* HANDLEP_H */

