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
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6E70A389_33B8_4286_AD98_3ECD475C4512__INCLUDED_)
#define AFX_STDAFX_H__6E70A389_33B8_4286_AD98_3ECD475C4512__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#import "PService.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PBasic.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PClient.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PCSS.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PView.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids


/*
#include "ModuleConstants.h"  Would not compile so removed it and included the necessary stuff from it below (3 lines)
*/

const short gmmtPoint  = 1;
const short gmmtLinear = 2;
const short gmmtAreal  = 4;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6E70A389_33B8_4286_AD98_3ECD475C4512__INCLUDED_)
