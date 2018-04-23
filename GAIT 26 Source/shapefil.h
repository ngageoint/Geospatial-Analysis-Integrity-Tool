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
#ifndef _SHAPEFILE_H_INCLUDED
#define _SHAPEFILE_H_INCLUDED

/******************************************************************************
 * $Id: shapefil.h,v 1.26 2002/09/29 00:00:08 warmerda Exp $
 *
 * Project:  Shapelib
 * Purpose:  Primary include file for Shapelib.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see LICENSE.LGPL).  This
 * option is discussed in more detail in shapelib.html.
 *
 * --
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: shapefil.h,v $
 * Revision 1.26  2002/09/29 00:00:08  warmerda
 * added FTLogical and logical attribute read/write calls
 *
 * Revision 1.25  2002/05/07 13:46:30  warmerda
 * added TimTimDBFWriteAttributeDirectly().
 *
 * Revision 1.24  2002/04/10 16:59:54  warmerda
 * added TimTimSHPRewindObject
 *
 * Revision 1.23  2002/01/15 14:36:07  warmerda
 * updated email address
 *
 * Revision 1.22  2002/01/15 14:32:00  warmerda
 * try to improve TimSHPAPI_CALL docs
 *
 * Revision 1.21  2001/11/01 16:29:55  warmerda
 * move pabyRec into TimSHPInfo for thread safety
 *
 * Revision 1.20  2001/07/20 13:06:02  warmerda
 * fixed TimSHPAPI attribute for TimSHPTreeFindLikelyShapes
 *
 * Revision 1.19  2001/05/31 19:20:13  warmerda
 * added TimDBFGetFieldIndex()
 *
 * Revision 1.18  2001/05/31 18:15:40  warmerda
 * Added support for NULL fields in TimDBF files
 *
 * Revision 1.17  2001/05/23 13:36:52  warmerda
 * added use of TimSHPAPI_CALL
 *
 * Revision 1.16  2000/09/25 14:15:59  warmerda
 * added TimDBFGetNativeFieldType()
 *
 * Revision 1.15  2000/02/16 16:03:51  warmerda
 * added null shape support
 *
 * Revision 1.14  1999/11/05 14:12:05  warmerda
 * updated license terms
 *
 * Revision 1.13  1999/06/02 18:24:21  warmerda
 * added trimming code
 *
 * Revision 1.12  1999/06/02 17:56:12  warmerda
 * added quad'' subnode support for trees
 *
 * Revision 1.11  1999/05/18 19:11:11  warmerda
 * Added example searching capability
 *
 * Revision 1.10  1999/05/18 17:49:38  warmerda
 * added initial quadtree support
 *
 * Revision 1.9  1999/05/11 03:19:28  warmerda
 * added new Tuple api, and improved extension handling - add from candrsn
 *
 * Revision 1.8  1999/03/23 17:22:27  warmerda
 * Added extern "C" protection for C++ users of shapefil.h.
 *
 * Revision 1.7  1998/12/31 15:31:07  warmerda
 * Added the TRIM_TimDBF_WHITESPACE and DISABLE_MULTIPATCH_MEASURE options.
 *
 * Revision 1.6  1998/12/03 15:48:15  warmerda
 * Added TimSHPCalculateExtents().
 *
 * Revision 1.5  1998/11/09 20:57:16  warmerda
 * Altered TimSHPGetInfo() call.
 *
 * Revision 1.4  1998/11/09 20:19:33  warmerda
 * Added 3D support, and use of TimSHPObject.
 *
 * Revision 1.3  1995/08/23 02:24:05  warmerda
 * Added support for reading bounds.
 *
 * Revision 1.2  1995/08/04  03:17:39  warmerda
 * Added header.
 *
 */

#include <stdio.h>

#ifdef USE_DBMALLOC
#include <dbmalloc.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/*                        Configuration options.                        */
/************************************************************************/

/* -------------------------------------------------------------------- */
/*      Should the TimDBFReadStringAttribute() strip leading and           */
/*      trailing white space?                                           */
/* -------------------------------------------------------------------- */
#define TRIM_TimDBF_WHITESPACE

/* -------------------------------------------------------------------- */
/*      Should we write measure values to the Multipatch object?        */
/*      Reportedly ArcView crashes if we do write it, so for now it     */
/*      is disabled.                                                    */
/* -------------------------------------------------------------------- */
#define DISABLE_MULTIPATCH_MEASURE

/* -------------------------------------------------------------------- */
/*      TimSHPAPI_CALL                                                     */
/*                                                                      */
/*      The following two macros are present to allow forcing           */
/*      various calling conventions on the Shapelib API.                */
/*                                                                      */
/*      To force __stdcall conventions (needed to call Shapelib         */
/*      from Visual Basic and/or Dephi I believe) the makefile could    */
/*      be modified to define:                                          */
/*                                                                      */
/*        /DTimSHPAPI_CALL=__stdcall                                       */
/*                                                                      */
/*      If it is desired to force export of the Shapelib API without    */
/*      using the shapelib.def file, use the following definition.      */
/*                                                                      */
/*        /DSHAPELIB_DLLEXPORT                                          */
/*                                                                      */
/*      To get both at once it will be necessary to hack this           */
/*      include file to define:                                         */
/*                                                                      */
/*        #define TimSHPAPI_CALL __declspec(dllexport) __stdcall           */
/*        #define TimSHPAPI_CALL1 __declspec(dllexport) * __stdcall        */
/*                                                                      */
/*      The complexity of the situtation is partly caused by the        */
/*      peculiar requirement of Visual C++ that __stdcall appear        */
/*      after any "*"'s in the return value of a function while the     */
/*      __declspec(dllexport) must appear before them.                  */
/* -------------------------------------------------------------------- */

#ifdef SHAPELIB_DLLEXPORT
#  define TimSHPAPI_CALL __declspec(dllexport)
#  define TimSHPAPI_CALL1(x)  __declspec(dllexport) x
#endif

#ifndef TimSHPAPI_CALL
#  define TimSHPAPI_CALL
#endif

#ifndef TimSHPAPI_CALL1
#  define TimSHPAPI_CALL1(x)      x TimSHPAPI_CALL
#endif
    
/************************************************************************/
/*                             TimSHP Support.                             */
/************************************************************************/
typedef	struct
{
    FILE        *fpTimSHP;
    FILE	*fpSHX;

    int		nShapeType;				/* TimSHPT_* */
    
    int		nFileSize;				/* TimSHP file */

    int         nRecords;
    int		nMaxRecords;
    int		*panRecOffset;
    int		*panRecSize;

    double	adBoundsMin[4];
    double	adBoundsMax[4];

    int		bUpdated;

    unsigned char *pabyRec;
    int         nBufSize;
} TimSHPInfo;

typedef TimSHPInfo * TimSHPHandle;

/* -------------------------------------------------------------------- */
/*      Shape types (nTimSHPType)                                          */
/* -------------------------------------------------------------------- */
#define TimSHPT_NULL	0
#define TimSHPT_POINT	1
#define TimSHPT_ARC	3
#define TimSHPT_POLYGON	5
#define TimSHPT_MULTIPOINT	8
#define TimSHPT_POINTZ	11
#define TimSHPT_ARCZ	13
#define TimSHPT_POLYGONZ	15
#define TimSHPT_MULTIPOINTZ 18
#define TimSHPT_POINTM	21
#define TimSHPT_ARCM	23
#define TimSHPT_POLYGONM	25
#define TimSHPT_MULTIPOINTM 28
#define TimSHPT_MULTIPATCH 31


/* -------------------------------------------------------------------- */
/*      Part types - everything but TimSHPT_MULTIPATCH just uses           */
/*      TimSHPP_RING.                                                      */
/* -------------------------------------------------------------------- */

#define TimSHPP_TRISTRIP	0
#define TimSHPP_TRIFAN	1
#define TimSHPP_OUTERRING	2
#define TimSHPP_INNERRING	3
#define TimSHPP_FIRSTRING	4
#define TimSHPP_RING	5

/* -------------------------------------------------------------------- */
/*      TimSHPObject - represents on shape (without attributes) read       */
/*      from the .shp file.                                             */
/* -------------------------------------------------------------------- */
typedef struct
{
    int		nTimSHPType;

    int		nShapeId; /* -1 is unknown/unassigned */

    int		nParts;
    int		*panPartStart;
    int		*panPartType;
    
    int		nVertices;
    double	*padfX;
    double	*padfY;
    double	*padfZ;
    double	*padfM;

    double	dfXMin;
    double	dfYMin;
    double	dfZMin;
    double	dfMMin;

    double	dfXMax;
    double	dfYMax;
    double	dfZMax;
    double	dfMMax;
} TimSHPObject;

/* -------------------------------------------------------------------- */
/*      TimSHP API Prototypes                                              */
/* -------------------------------------------------------------------- */
TimSHPHandle TimSHPAPI_CALL
      TimSHPOpen( const char * pszShapeFile, const char * pszAccess );
TimSHPHandle TimSHPAPI_CALL
      TimSHPCreate( const char * pszShapeFile, int nShapeType );
void TimSHPAPI_CALL
      TimSHPGetInfo( TimSHPHandle hTimSHP, int * pnEntities, int * pnShapeType,
                  double * padfMinBound, double * padfMaxBound );

TimSHPObject TimSHPAPI_CALL1(*)
      TimSHPReadObject( TimSHPHandle hTimSHP, int iShape );
int TimSHPAPI_CALL
      TimSHPWriteObject( TimSHPHandle hTimSHP, int iShape, TimSHPObject * psObject );

void TimSHPAPI_CALL
      TimSHPDestroyObject( TimSHPObject * psObject );
void TimSHPAPI_CALL
      TimSHPComputeExtents( TimSHPObject * psObject );
TimSHPObject TimSHPAPI_CALL1(*)
      TimSHPCreateObject( int nTimSHPType, int nShapeId,
                       int nParts, int * panPartStart, int * panPartType,
                       int nVertices, double * padfX, double * padfY,
                       double * padfZ, double * padfM );
TimSHPObject TimSHPAPI_CALL1(*)
      TimSHPCreateSimpleObject( int nTimSHPType, int nVertices,
                             double * padfX, double * padfY, double * padfZ );

int TimSHPAPI_CALL
      TimSHPRewindObject( TimSHPHandle hTimSHP, TimSHPObject * psObject );

void TimSHPAPI_CALL
      TimSHPClose( TimSHPHandle hTimSHP );

const char TimSHPAPI_CALL1(*)
      TimSHPTypeName( int nTimSHPType );
const char TimSHPAPI_CALL1(*)
      TimSHPPartTypeName( int nPartType );

/* -------------------------------------------------------------------- */
/*      Shape quadtree indexing API.                                    */
/* -------------------------------------------------------------------- */

/* this can be two or four for binary or quad tree */
#define MAX_SUBNODE	4

typedef struct shape_tree_node
{
    /* region covered by this node */
    double	adfBoundsMin[4];
    double	adfBoundsMax[4];

    /* list of shapes stored at this node.  The papsShapeObj pointers
       or the whole list can be NULL */
    int		nShapeCount;
    int		*panShapeIds;
    TimSHPObject   **papsShapeObj;

    int		nSubNodes;
    struct shape_tree_node *apsSubNode[MAX_SUBNODE];
    
} TimSHPTreeNode;

typedef struct
{
    TimSHPHandle   hTimSHP;
    
    int		nMaxDepth;
    int		nDimension;
    
    TimSHPTreeNode	*psRoot;
} TimSHPTree;

TimSHPTree TimSHPAPI_CALL1(*)
      TimSHPCreateTree( TimSHPHandle hTimSHP, int nDimension, int nMaxDepth,
                     double *padfBoundsMin, double *padfBoundsMax );
void    TimSHPAPI_CALL
      TimSHPDestroyTree( TimSHPTree * hTree );

int	TimSHPAPI_CALL
      TimSHPWriteTree( TimSHPTree *hTree, const char * pszFilename );
TimSHPTree TimSHPAPI_CALL
      TimSHPReadTree( const char * pszFilename );

int	TimSHPAPI_CALL
      TimSHPTreeAddObject( TimSHPTree * hTree, TimSHPObject * psObject );
int	TimSHPAPI_CALL
      TimSHPTreeAddShapeId( TimSHPTree * hTree, TimSHPObject * psObject );
int	TimSHPAPI_CALL
      TimSHPTreeRemoveShapeId( TimSHPTree * hTree, int nShapeId );

void 	TimSHPAPI_CALL
      TimSHPTreeTrimExtraNodes( TimSHPTree * hTree );

int    TimSHPAPI_CALL1(*)
      TimSHPTreeFindLikelyShapes( TimSHPTree * hTree,
                               double * padfBoundsMin,
                               double * padfBoundsMax,
                               int * );
int     TimSHPAPI_CALL
      TimSHPCheckBoundsOverlap( double *, double *, double *, double *, int );

/************************************************************************/
/*                             TimDBF Support.                             */
/************************************************************************/
typedef	struct
{
    FILE	*fp;

    int         nRecords;

    int		nRecordLength;
    int		nHeaderLength;
    int		nFields;
    int		*panFieldOffset;
    int		*panFieldSize;
    int		*panFieldDecimals;
    char	*pachFieldType;

    char	*pszHeader;

    int		nCurrentRecord;
    int		bCurrentRecordModified;
    char	*pszCurrentRecord;
    
    int		bNoHeader;
    int		bUpdated;
} TimDBFInfo;

typedef TimDBFInfo * TimDBFHandle;

typedef enum {
  FTString,
  FTInteger,
  FTDouble,
  FTLogical,
  FTInvalid,
  FTSpecial
} TimDBFFieldType;

#define XBASE_FLDHDR_SZ       32

TimDBFHandle TimSHPAPI_CALL
      TimDBFOpen( const char * pszTimDBFFile, const char * pszAccess );
TimDBFHandle TimSHPAPI_CALL
      TimDBFCreate( const char * pszTimDBFFile );

int	TimSHPAPI_CALL
      TimDBFGetFieldCount( TimDBFHandle psTimDBF );
int	TimSHPAPI_CALL
      TimDBFGetRecordCount( TimDBFHandle psTimDBF );
int	TimSHPAPI_CALL
      TimDBFAddField( TimDBFHandle hTimDBF, const char * pszFieldName,
                   TimDBFFieldType eType, int nWidth, int nDecimals );

TimDBFFieldType TimSHPAPI_CALL
      TimDBFGetFieldInfo( TimDBFHandle psTimDBF, int iField, 
                       char * pszFieldName, int * pnWidth, int * pnDecimals );

int TimSHPAPI_CALL
      TimDBFGetFieldIndex(TimDBFHandle psTimDBF, const char *pszFieldName);

int 	TimSHPAPI_CALL
      TimDBFReadIntegerAttribute( TimDBFHandle hTimDBF, int iShape, int iField );
double 	TimSHPAPI_CALL
      TimDBFReadDoubleAttribute( TimDBFHandle hTimDBF, int iShape, int iField );
const char TimSHPAPI_CALL1(*)
      TimDBFReadStringAttribute( TimDBFHandle hTimDBF, int iShape, int iField );
const char TimSHPAPI_CALL1(*)
      TimDBFReadLogicalAttribute( TimDBFHandle hTimDBF, int iShape, int iField );
int     TimSHPAPI_CALL
      TimDBFIsAttributeNULL( TimDBFHandle hTimDBF, int iShape, int iField );

int TimSHPAPI_CALL
      TimDBFWriteIntegerAttribute( TimDBFHandle hTimDBF, int iShape, int iField, 
                                int nFieldValue );
int TimSHPAPI_CALL
      TimDBFWriteDoubleAttribute( TimDBFHandle hTimDBF, int iShape, int iField,
                               double dFieldValue );
int TimSHPAPI_CALL
      TimDBFWriteStringAttribute( TimDBFHandle hTimDBF, int iShape, int iField,
                               const char * pszFieldValue );
int TimSHPAPI_CALL
     TimDBFWriteNULLAttribute( TimDBFHandle hTimDBF, int iShape, int iField );

int TimSHPAPI_CALL
     TimDBFWriteLogicalAttribute( TimDBFHandle hTimDBF, int iShape, int iField,
			       const char lFieldValue);
int TimSHPAPI_CALL
     TimDBFWriteAttributeDirectly(TimDBFHandle psTimDBF, int hEntity, int iField,
                               void * pValue );
const char TimSHPAPI_CALL1(*)
      TimDBFReadTuple(TimDBFHandle psTimDBF, int hEntity );
int TimSHPAPI_CALL
      TimDBFWriteTuple(TimDBFHandle psTimDBF, int hEntity, void * pRawTuple );

TimDBFHandle TimSHPAPI_CALL
      TimDBFCloneEmpty(TimDBFHandle psTimDBF, const char * pszFilename );
 
void	TimSHPAPI_CALL
      TimDBFClose( TimDBFHandle hTimDBF );
char    TimSHPAPI_CALL
      TimDBFGetNativeFieldType( TimDBFHandle hTimDBF, int iField );

#ifdef __cplusplus
}
#endif

#endif /* ndef _SHAPEFILE_H_INCLUDED */
