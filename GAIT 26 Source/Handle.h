/************
The GAIT Tool was developed at the National Geospatial-Intelligence Agency (NGA) in collaboration with the   
Institute for Defense Analyses (IDA) under a Government contract. The government is releasing this software   
to increase the impact of government investments by providing developers with the opportunity to take things   
in new directions. The software use, modification, and distribution rights are stipulated within the MIT license.

As-is warranty is covered within the MIT license but it's re-stated here for clarity: The GAIT tool contains several   
software components. The software is provided "as is," and no warranty, express or implied, including but not limited   
to the implied warranties of merchantability and fitness for particular purpose or arising by statute or otherwise in   
law or from a course of dealing or usage in trade, is made with respect to the accuracy or functioning of the software.   
Neither NGA or IDA are liable for any claims, losses, or damages arising from or connected with the use of the software.   
The user's sole and exclusive remedy is to stop using the software.
************/
/* Handle.h: Public header file for the Outline Handle widget. */

#ifndef HANDLE_H
#define HANDLE_H

#ifdef __cplusplus 
extern "C" {
#endif

extern WidgetClass xmHandleWidgetClass;
typedef struct _XmHandleClassRec *XmHandleWidgetClass;
typedef struct _XmHandleRec *XmHandleWidget;

#define XmNsubWidget		"subWidget"
#define XmCSubWidget		"SubWidget"

#ifdef _NO_PROTO
Widget XmCreateHandle();
#else
Widget XmCreateHandle(Widget, char *, ArgList, Cardinal);
#endif

#ifdef __cplusplus
}
#endif

#endif /* HANDLE_H */
        
