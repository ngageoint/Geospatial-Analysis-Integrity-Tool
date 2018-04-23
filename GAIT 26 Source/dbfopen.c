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
/******************************************************************************
 * $Id: dbfopen.c,v 1.48 2003/03/10 14:51:27 warmerda Exp $
 *
 * Project:  Shapelib
 * Purpose:  Implementation of .dbf access API documented in dbf_api.html.
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
 * $Log: dbfopen.c,v $
 * Revision 1.48  2003/03/10 14:51:27  warmerda
 * TimDBFWrite* calls now return FALSE if they have to truncate
 *
 * Revision 1.47  2002/11/20 03:32:22  warmerda
 * Ensure field name in TimDBFGetFieldIndex() is properly terminated.
 *
 * Revision 1.46  2002/10/09 13:10:21  warmerda
 * Added check that width is positive.
 *
 * Revision 1.45  2002/09/29 00:00:08  warmerda
 * added FTLogical and logical attribute read/write calls
 *
 * Revision 1.44  2002/05/07 13:46:11  warmerda
 * Added TimDBFWriteAttributeDirectly().
 *
 * Revision 1.43  2002/02/13 19:39:21  warmerda
 * Fix casting issues in TimDBFCloneEmpty().
 *
 * Revision 1.42  2002/01/15 14:36:07  warmerda
 * updated email address
 *
 * Revision 1.41  2002/01/15 14:31:49  warmerda
 * compute rather than copying nHeaderLength in TimDBFCloneEmpty()
 *
 * Revision 1.40  2002/01/09 04:32:35  warmerda
 * fixed to read correct amount of header
 *
 * Revision 1.39  2001/12/11 22:41:03  warmerda
 * improve io related error checking when reading header
 *
 * Revision 1.38  2001/11/28 16:07:31  warmerda
 * Cleanup to avoid compiler warnings as suggested by Richard Hash.
 *
 * Revision 1.37  2001/07/04 05:18:09  warmerda
 * do last fix properly
 *
 * Revision 1.36  2001/07/04 05:16:09  warmerda
 * fixed fieldname comparison in TimDBFGetFieldIndex
 *
 * Revision 1.35  2001/06/22 02:10:06  warmerda
 * fixed NULL shape support with help from Jim Matthews
 *
 * Revision 1.33  2001/05/31 19:20:13  warmerda
 * added TimDBFGetFieldIndex()
 *
 * Revision 1.32  2001/05/31 18:15:40  warmerda
 * Added support for NULL fields in TimDBF files
 *
 * Revision 1.31  2001/05/23 13:36:52  warmerda
 * added use of TimSHPAPI_CALL
 *
 * Revision 1.30  2000/12/05 14:43:38  warmerda
 * DBReadAttribute() white space trimming bug fix
 *
 * Revision 1.29  2000/10/05 14:36:44  warmerda
 * fix bug with writing very wide numeric fields
 *
 * Revision 1.28  2000/09/25 14:18:07  warmerda
 * Added some casts of strlen() return result to fix warnings on some
 * systems, as submitted by Daniel.
 *
 * Revision 1.27  2000/09/25 14:15:51  warmerda
 * added TimDBFGetNativeFieldType()
 *
 * Revision 1.26  2000/07/07 13:39:45  warmerda
 * removed unused variables, and added system include files
 *
 * Revision 1.25  2000/05/29 18:19:13  warmerda
 * avoid use of uchar, and adding casting fix
 *
 * Revision 1.24  2000/05/23 13:38:27  warmerda
 * Added error checks on return results of fread() and fseek().
 *
 * Revision 1.23  2000/05/23 13:25:49  warmerda
 * Avoid crashing if field or record are out of range in dbfread*attribute().
 *
 * Revision 1.22  1999/12/15 13:47:24  warmerda
 * Added stdlib.h to ensure that atof() is prototyped.
 *
 * Revision 1.21  1999/12/13 17:25:46  warmerda
 * Added support for upper case .TimDBF extention.
 *
 * Revision 1.20  1999/11/30 16:32:11  warmerda
 * Use atof() instead of sscanf().
 *
 * Revision 1.19  1999/11/05 14:12:04  warmerda
 * updated license terms
 *
 * Revision 1.18  1999/07/27 00:53:28  warmerda
 * ensure that whole old field value clear on write of string
 *
 * Revision 1.1  1999/07/05 18:58:07  warmerda
 * New
 *
 * Revision 1.17  1999/06/11 19:14:12  warmerda
 * Fixed some memory leaks.
 *
 * Revision 1.16  1999/06/11 19:04:11  warmerda
 * Remoted some unused variables.
 *
 * Revision 1.15  1999/05/11 03:19:28  warmerda
 * added new Tuple api, and improved extension handling - add from candrsn
 *
 * Revision 1.14  1999/05/04 15:01:48  warmerda
 * Added 'F' support.
 *
 * Revision 1.13  1999/03/23 17:38:59  warmerda
 * TimDBFAddField() now actually does return the new field number, or -1 if
 * it fails.
 *
 * Revision 1.12  1999/03/06 02:54:46  warmerda
 * Added logic to convert shapefile name to dbf filename in TimDBFOpen()
 * for convenience.
 *
 * Revision 1.11  1998/12/31 15:30:34  warmerda
 * Improved the interchangability of numeric and string attributes.  Add
 * white space trimming option for attributes.
 *
 * Revision 1.10  1998/12/03 16:36:44  warmerda
 * Use r+b instead of rb+ for binary access.
 *
 * Revision 1.9  1998/12/03 15:34:23  warmerda
 * Updated copyright message.
 *
 * Revision 1.8  1997/12/04 15:40:15  warmerda
 * Added newline character after field definitions.
 *
 * Revision 1.7  1997/03/06 14:02:10  warmerda
 * Ensure bUpdated is initialized.
 *
 * Revision 1.6  1996/02/12 04:54:41  warmerda
 * Ensure that TimDBFWriteAttribute() returns TRUE if it succeeds.
 *
 * Revision 1.5  1995/10/21  03:15:12  warmerda
 * Changed to use binary file access, and ensure that the
 * field name field is zero filled, and limited to 10 chars.
 *
 * Revision 1.4  1995/08/24  18:10:42  warmerda
 * Added use of SfRealloc() to avoid pre-ANSI realloc() functions such
 * as on the Sun.
 *
 * Revision 1.3  1995/08/04  03:15:16  warmerda
 * Fixed up header.
 *
 * Revision 1.2  1995/08/04  03:14:43  warmerda
 * Added header.
 */

#include "shapefil.h"
#include "share.h"  /* need for USE_DOS */

#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
   
#ifndef FALSE
#  define FALSE		0
#  define TRUE		1
#endif


/**
#if (USE_DOS==1)
 int __mb_cur_max;
 unsigned short* _pctype;
 int errno;
#endif
**/

/* Above does not seem necessary and gives link warning.
   Could maybe be replaced with:
   extern "C"{int __mb_cur_max; unsigned short* _pctype; int errno;}
*/


static int	nStringFieldLen = 0;
static char * pszStringField = NULL;

/************************************************************************/
/*                             SfRealloc()                              */
/*                                                                      */
/*      A realloc cover function that will access a NULL pointer as     */
/*      a valid input.                                                  */
/************************************************************************/

static void * SfRealloc( void * pMem, int nNewSize )

{
    if( pMem == NULL )
        return( (void *) malloc(nNewSize) );
    else
        return( (void *) realloc(pMem,nNewSize) );
}

/************************************************************************/
/*                           TimDBFWriteHeader()                           */
/*                                                                      */
/*      This is called to write out the file header, and field          */
/*      descriptions before writing any actual data records.  This      */
/*      also computes all the TimDBFDataSet field offset/size/decimals     */
/*      and so forth values.                                            */
/************************************************************************/

static void TimDBFWriteHeader(TimDBFHandle psTimDBF)

{
    unsigned char	abyHeader[XBASE_FLDHDR_SZ];
    int		i;

    if( !psTimDBF->bNoHeader )
        return;

    psTimDBF->bNoHeader = FALSE;

/* -------------------------------------------------------------------- */
/*	Initialize the file header information.				*/
/* -------------------------------------------------------------------- */
    for( i = 0; i < XBASE_FLDHDR_SZ; i++ )
        abyHeader[i] = 0;

    abyHeader[0] = 0x03;		/* memo field? - just copying 	*/

    /* date updated on close, record count preset at zero */

    abyHeader[8] = psTimDBF->nHeaderLength % 256;
    abyHeader[9] = psTimDBF->nHeaderLength / 256;
    
    abyHeader[10] = psTimDBF->nRecordLength % 256;
    abyHeader[11] = psTimDBF->nRecordLength / 256;

/* -------------------------------------------------------------------- */
/*      Write the initial 32 byte file header, and all the field        */
/*      descriptions.                                     		*/
/* -------------------------------------------------------------------- */
    fseek( psTimDBF->fp, 0, 0 );
    fwrite( abyHeader, XBASE_FLDHDR_SZ, 1, psTimDBF->fp );
    fwrite( psTimDBF->pszHeader, XBASE_FLDHDR_SZ, psTimDBF->nFields, psTimDBF->fp );

/* -------------------------------------------------------------------- */
/*      Write out the newline character if there is room for it.        */
/* -------------------------------------------------------------------- */
    if( psTimDBF->nHeaderLength > 32*psTimDBF->nFields + 32 )
    {
        char	cNewline;

        cNewline = 0x0d;
        fwrite( &cNewline, 1, 1, psTimDBF->fp );
    }
}

/************************************************************************/
/*                           TimDBFFlushRecord()                           */
/*                                                                      */
/*      Write out the current record if there is one.                   */
/************************************************************************/

static void TimDBFFlushRecord( TimDBFHandle psTimDBF )

{
    int		nRecordOffset;

    if( psTimDBF->bCurrentRecordModified && psTimDBF->nCurrentRecord > -1 )
    {
	psTimDBF->bCurrentRecordModified = FALSE;

	nRecordOffset = psTimDBF->nRecordLength * psTimDBF->nCurrentRecord 
	                                             + psTimDBF->nHeaderLength;

	fseek( psTimDBF->fp, nRecordOffset, 0 );
	fwrite( psTimDBF->pszCurrentRecord, psTimDBF->nRecordLength, 1, psTimDBF->fp );
    }
}

/************************************************************************/
/*                              TimDBFOpen()                               */
/*                                                                      */
/*      Open a .dbf file.                                               */
/************************************************************************/
   
TimDBFHandle TimSHPAPI_CALL
TimDBFOpen( const char * pszFilename, const char * pszAccess )

{
    TimDBFHandle		psTimDBF;
    unsigned char		*pabyBuf;
    int			nFields, nHeadLen, nRecLen, iField, i;
    char		*pszBasename, *pszFullname;

/* -------------------------------------------------------------------- */
/*      We only allow the access strings "rb" and "r+".                  */
/* -------------------------------------------------------------------- */
    if( strcmp(pszAccess,"r") != 0 && strcmp(pszAccess,"r+") != 0 
        && strcmp(pszAccess,"rb") != 0 && strcmp(pszAccess,"rb+") != 0
        && strcmp(pszAccess,"r+b") != 0 )
        return( NULL );

    if( strcmp(pszAccess,"r") == 0 )
        pszAccess = "rb";
 
    if( strcmp(pszAccess,"r+") == 0 )
        pszAccess = "rb+";

/* -------------------------------------------------------------------- */
/*	Compute the base (layer) name.  If there is any extension	*/
/*	on the passed in filename we will strip it off.			*/
/* -------------------------------------------------------------------- */
    pszBasename = (char *) malloc(strlen(pszFilename)+5);
    strcpy( pszBasename, pszFilename );
    for( i = strlen(pszBasename)-1; 
	 i > 0 /* tim took this out && pszBasename[i] != '.'*/ && pszBasename[i] != '/'
	       && pszBasename[i] != '\\';
	 i-- ) {}

    if( pszBasename[i] == '.' )
        pszBasename[i] = '\0';

    pszFullname = (char *) malloc(strlen(pszBasename) + 10);
    sprintf( pszFullname, "%s.dbf", pszBasename );
        
    psTimDBF = (TimDBFHandle) calloc( 1, sizeof(TimDBFInfo) );
    psTimDBF->fp = fopen( pszFullname, pszAccess );

    if( psTimDBF->fp == NULL )
    {
        sprintf( pszFullname, "%s.DBF", pszBasename );
        psTimDBF->fp = fopen(pszFullname, pszAccess );
    }
    if( psTimDBF->fp == NULL )
    {
        sprintf( pszFullname, "%s.TimDBF", pszBasename );
        psTimDBF->fp = fopen(pszFullname, pszAccess );
    }
    
    free( pszBasename );
    free( pszFullname );
    
    if( psTimDBF->fp == NULL )
    {
        free( psTimDBF );
        return( NULL );
    }

    psTimDBF->bNoHeader = FALSE;
    psTimDBF->nCurrentRecord = -1;
    psTimDBF->bCurrentRecordModified = FALSE;

/* -------------------------------------------------------------------- */
/*  Read Table Header info                                              */
/* -------------------------------------------------------------------- */
    pabyBuf = (unsigned char *) malloc(500);
    if( fread( pabyBuf, 32, 1, psTimDBF->fp ) != 1 )
    {
        fclose( psTimDBF->fp );
        free( pabyBuf );
        free( psTimDBF );
        return NULL;
    }

    psTimDBF->nRecords = 
     pabyBuf[4] + pabyBuf[5]*256 + pabyBuf[6]*256*256 + pabyBuf[7]*256*256*256;

    psTimDBF->nHeaderLength = nHeadLen = pabyBuf[8] + pabyBuf[9]*256;
    psTimDBF->nRecordLength = nRecLen = pabyBuf[10] + pabyBuf[11]*256;
    
    psTimDBF->nFields = nFields = (nHeadLen - 32) / 32;

    psTimDBF->pszCurrentRecord = (char *) malloc(nRecLen);

/* -------------------------------------------------------------------- */
/*  Read in Field Definitions                                           */
/* -------------------------------------------------------------------- */
    
    pabyBuf = (unsigned char *) SfRealloc(pabyBuf,nHeadLen);
    psTimDBF->pszHeader = (char *) pabyBuf;

    fseek( psTimDBF->fp, 32, 0 );
    if( fread( pabyBuf, nHeadLen-32, 1, psTimDBF->fp ) != 1 )
    {
        fclose( psTimDBF->fp );
        free( pabyBuf );
        free( psTimDBF );
        return NULL;
    }

    psTimDBF->panFieldOffset = (int *) malloc(sizeof(int) * nFields);
    psTimDBF->panFieldSize = (int *) malloc(sizeof(int) * nFields);
    psTimDBF->panFieldDecimals = (int *) malloc(sizeof(int) * nFields);
    psTimDBF->pachFieldType = (char *) malloc(sizeof(char) * nFields);

    for( iField = 0; iField < nFields; iField++ )
    {
	unsigned char		*pabyFInfo;

	pabyFInfo = pabyBuf+iField*32;

	if( pabyFInfo[11] == 'N' || pabyFInfo[11] == 'F' )
	{
	    psTimDBF->panFieldSize[iField] = pabyFInfo[16];
	    psTimDBF->panFieldDecimals[iField] = pabyFInfo[17];
	}
	else
	{
	    psTimDBF->panFieldSize[iField] = pabyFInfo[16] + pabyFInfo[17]*256;
	    psTimDBF->panFieldDecimals[iField] = 0;
	}

	psTimDBF->pachFieldType[iField] = (char) pabyFInfo[11];
	if( iField == 0 )
	    psTimDBF->panFieldOffset[iField] = 1;
	else
	    psTimDBF->panFieldOffset[iField] = 
	      psTimDBF->panFieldOffset[iField-1] + psTimDBF->panFieldSize[iField-1];
    }

    return( psTimDBF );
}

/************************************************************************/
/*                              TimDBFClose()                              */
/************************************************************************/

void TimSHPAPI_CALL
TimDBFClose(TimDBFHandle psTimDBF)
{
/* -------------------------------------------------------------------- */
/*      Write out header if not already written.                        */
/* -------------------------------------------------------------------- */
    if( psTimDBF->bNoHeader )
        TimDBFWriteHeader( psTimDBF );

    TimDBFFlushRecord( psTimDBF );

/* -------------------------------------------------------------------- */
/*      Update last access date, and number of records if we have	*/
/*	write access.                					*/
/* -------------------------------------------------------------------- */
    if( psTimDBF->bUpdated )
    {
	unsigned char		abyFileHeader[32];

	fseek( psTimDBF->fp, 0, 0 );
	fread( abyFileHeader, 32, 1, psTimDBF->fp );

	abyFileHeader[1] = 95;			/* YY */
	abyFileHeader[2] = 7;			/* MM */
	abyFileHeader[3] = 26;			/* DD */

	abyFileHeader[4] = psTimDBF->nRecords % 256;
	abyFileHeader[5] = (psTimDBF->nRecords/256) % 256;
	abyFileHeader[6] = (psTimDBF->nRecords/(256*256)) % 256;
	abyFileHeader[7] = (psTimDBF->nRecords/(256*256*256)) % 256;

	fseek( psTimDBF->fp, 0, 0 );
	fwrite( abyFileHeader, 32, 1, psTimDBF->fp );
    }

/* -------------------------------------------------------------------- */
/*      Close, and free resources.                                      */
/* -------------------------------------------------------------------- */
    fclose( psTimDBF->fp );

    if( psTimDBF->panFieldOffset != NULL )
    {
        free( psTimDBF->panFieldOffset );
        free( psTimDBF->panFieldSize );
        free( psTimDBF->panFieldDecimals );
        free( psTimDBF->pachFieldType );
    }

    free( psTimDBF->pszHeader );
    free( psTimDBF->pszCurrentRecord );

    free( psTimDBF );

    if( pszStringField != NULL )
    {
        free( pszStringField );
        pszStringField = NULL;
        nStringFieldLen = 0;
    }
}

/************************************************************************/
/*                             TimDBFCreate()                              */
/*                                                                      */
/*      Create a new .dbf file.                                         */
/************************************************************************/

TimDBFHandle TimSHPAPI_CALL
TimDBFCreate( const char * pszFilename )

{
    TimDBFHandle	psTimDBF;
    FILE	*fp;
    char	*pszFullname, *pszBasename;
    int		i;

/* -------------------------------------------------------------------- */
/*	Compute the base (layer) name.  If there is any extension	*/
/*	on the passed in filename we will strip it off.			*/
/* -------------------------------------------------------------------- */
    pszBasename = (char *) malloc(strlen(pszFilename)+5);
    strcpy( pszBasename, pszFilename );
    for( i = strlen(pszBasename)-1; 
	 i > 0 && pszBasename[i] != '.' && pszBasename[i] != '/'
	       && pszBasename[i] != '\\';
	 i-- ) {}

    if( pszBasename[i] == '.' )
        pszBasename[i] = '\0';

    pszFullname = (char *) malloc(strlen(pszBasename) + 5);
    sprintf( pszFullname, "%s.dbf", pszBasename );
    free( pszBasename );

/* -------------------------------------------------------------------- */
/*      Create the file.                                                */
/* -------------------------------------------------------------------- */
    fp = fopen( pszFullname, "wb" );
    if( fp == NULL )
        return( NULL );

    fputc( 0, fp );
    fclose( fp );

    fp = fopen( pszFullname, "rb+" );
    if( fp == NULL )
        return( NULL );

    free( pszFullname );

/* -------------------------------------------------------------------- */
/*	Create the info structure.					*/
/* -------------------------------------------------------------------- */
    psTimDBF = (TimDBFHandle) malloc(sizeof(TimDBFInfo));

    psTimDBF->fp = fp;
    psTimDBF->nRecords = 0;
    psTimDBF->nFields = 0;
    psTimDBF->nRecordLength = 1;
    psTimDBF->nHeaderLength = 33;
    
    psTimDBF->panFieldOffset = NULL;
    psTimDBF->panFieldSize = NULL;
    psTimDBF->panFieldDecimals = NULL;
    psTimDBF->pachFieldType = NULL;
    psTimDBF->pszHeader = NULL;

    psTimDBF->nCurrentRecord = -1;
    psTimDBF->bCurrentRecordModified = FALSE;
    psTimDBF->pszCurrentRecord = NULL;

    psTimDBF->bNoHeader = TRUE;

    return( psTimDBF );
}

/************************************************************************/
/*                            TimDBFAddField()                             */
/*                                                                      */
/*      Add a field to a newly created .dbf file before any records     */
/*      are written.                                                    */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFAddField(TimDBFHandle psTimDBF, const char * pszFieldName, 
            TimDBFFieldType eType, int nWidth, int nDecimals )

{
    char	*pszFInfo;
    int		i;

/* -------------------------------------------------------------------- */
/*      Do some checking to ensure we can add records to this file.     */
/* -------------------------------------------------------------------- */
    if( psTimDBF->nRecords > 0 )
        return( -1 );

    if( !psTimDBF->bNoHeader )
        return( -1 );

    if( eType != FTDouble && nDecimals != 0 )
        return( -1 );

    if( nWidth < 1 )
        return -1;

/* -------------------------------------------------------------------- */
/*      SfRealloc all the arrays larger to hold the additional field      */
/*      information.                                                    */
/* -------------------------------------------------------------------- */
    psTimDBF->nFields++;

    psTimDBF->panFieldOffset = (int *) 
      SfRealloc( psTimDBF->panFieldOffset, sizeof(int) * psTimDBF->nFields );

    psTimDBF->panFieldSize = (int *) 
      SfRealloc( psTimDBF->panFieldSize, sizeof(int) * psTimDBF->nFields );

    psTimDBF->panFieldDecimals = (int *) 
      SfRealloc( psTimDBF->panFieldDecimals, sizeof(int) * psTimDBF->nFields );

    psTimDBF->pachFieldType = (char *) 
      SfRealloc( psTimDBF->pachFieldType, sizeof(char) * psTimDBF->nFields );

/* -------------------------------------------------------------------- */
/*      Assign the new field information fields.                        */
/* -------------------------------------------------------------------- */
    psTimDBF->panFieldOffset[psTimDBF->nFields-1] = psTimDBF->nRecordLength;
    psTimDBF->nRecordLength += nWidth;
    psTimDBF->panFieldSize[psTimDBF->nFields-1] = nWidth;
    psTimDBF->panFieldDecimals[psTimDBF->nFields-1] = nDecimals;

    if( eType == FTLogical )
        psTimDBF->pachFieldType[psTimDBF->nFields-1] = 'L';
    else if( eType == FTString )
        psTimDBF->pachFieldType[psTimDBF->nFields-1] = 'C';
    else
        psTimDBF->pachFieldType[psTimDBF->nFields-1] = 'N';

/* -------------------------------------------------------------------- */
/*      Extend the required header information.                         */
/* -------------------------------------------------------------------- */
    psTimDBF->nHeaderLength += 32;
    psTimDBF->bUpdated = FALSE;

    psTimDBF->pszHeader = (char *) SfRealloc(psTimDBF->pszHeader,psTimDBF->nFields*32);

    pszFInfo = psTimDBF->pszHeader + 32 * (psTimDBF->nFields-1);

    for( i = 0; i < 32; i++ )
        pszFInfo[i] = '\0';

    if( (int) strlen(pszFieldName) < 10 )
        strncpy( pszFInfo, pszFieldName, strlen(pszFieldName));
    else
        strncpy( pszFInfo, pszFieldName, 10);

    pszFInfo[11] = psTimDBF->pachFieldType[psTimDBF->nFields-1];

    if( eType == FTString )
    {
        pszFInfo[16] = nWidth % 256;
        pszFInfo[17] = nWidth / 256;
    }
    else
    {
        pszFInfo[16] = nWidth;
        pszFInfo[17] = nDecimals;
    }
    
/* -------------------------------------------------------------------- */
/*      Make the current record buffer appropriately larger.            */
/* -------------------------------------------------------------------- */
    psTimDBF->pszCurrentRecord = (char *) SfRealloc(psTimDBF->pszCurrentRecord,
					       psTimDBF->nRecordLength);

    return( psTimDBF->nFields-1 );
}

/************************************************************************/
/*                          TimDBFReadAttribute()                          */
/*                                                                      */
/*      Read one of the attribute fields of a record.                   */
/************************************************************************/

void *TimDBFReadAttribute(TimDBFHandle psTimDBF, int hEntity, int iField,
                              char chReqType )

{
    int	       	nRecordOffset,i;
    unsigned char	*pabyRec;
    void	*pReturnField = NULL;
    char *badvalue;
    static double dDoubleField;
	extern int NULLINT,NULLFILE;

    badvalue = NULL;
	NULLINT  = 0;

/* -------------------------------------------------------------------- */
/*      Verify selection.                                               */
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity >= psTimDBF->nRecords )
      {
	/*
	printf("TimDBF returning NULL place 1\n");
	*/
        return( NULL );
      }

    if( iField < 0 || iField >= psTimDBF->nFields )
      {
	/*
	printf("TimDBF returning NULL place 2\n");
	*/
	return( NULL );
      }

/* -------------------------------------------------------------------- */
/*	Have we read the record?					*/
/* -------------------------------------------------------------------- */
    if( psTimDBF->nCurrentRecord != hEntity )
    {
	TimDBFFlushRecord( psTimDBF );

	nRecordOffset = psTimDBF->nRecordLength * hEntity + psTimDBF->nHeaderLength;

	if( fseek( psTimDBF->fp, nRecordOffset, 0 ) != 0 )
        {
	        printf("fseek(%d) failed on TimDBF file.\n",
                     nRecordOffset );
            NULLFILE = hEntity + 1000;
            return NULL;
        }

	if( fread( psTimDBF->pszCurrentRecord, psTimDBF->nRecordLength, 
                   1, psTimDBF->fp ) != 1 )
        {
            printf("fread(%d) failed on TimDBF file.\n",
                     psTimDBF->nRecordLength );
			NULLFILE = hEntity + 1000;
            return NULL;
        }

	psTimDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (unsigned char *) psTimDBF->pszCurrentRecord;

/* -------------------------------------------------------------------- */
/*	Ensure our field buffer is large enough to hold this buffer.	*/
/* -------------------------------------------------------------------- */
    if( psTimDBF->panFieldSize[iField]+1 > nStringFieldLen )
    {
	nStringFieldLen = psTimDBF->panFieldSize[iField]*2 + 10;
	pszStringField = (char *) SfRealloc(pszStringField,nStringFieldLen);
    }

/* -------------------------------------------------------------------- */
/*	Extract the requested field.					*/
/* -------------------------------------------------------------------- */

    
    strncpy( pszStringField, 
	     ((const char *) pabyRec) + psTimDBF->panFieldOffset[iField],
	     psTimDBF->panFieldSize[iField] );
    pszStringField[psTimDBF->panFieldSize[iField]] = '\0';

    pReturnField = pszStringField;

/* -------------------------------------------------------------------- */
/*      Decode the field.                                               */
/* -------------------------------------------------------------------- */

 
	if( chReqType == 'N' )
    {

     /* begin Tim add */
	 dDoubleField = strtod(pszStringField,&badvalue);

	 /* 
	    when the int value is NULL, atof returns 0, and pszStringField
	    is full of spaces
	 */

	 /* see if theres any illegal characters */
	 for(i=0;i<(int)strlen(pszStringField);i++)
	 {
		 if(pszStringField[i]>=58)
		 {
			 if(
				 (pszStringField[i]!=69)  &&
				 (pszStringField[i]!=101)
				 )
			 {
			   NULLINT = 1;
			   break;
			 }
		 }
		 if((pszStringField[i]<=42)&&(pszStringField[i]>=33))
		 {
			 NULLINT = 1;
			 break;
		 }
		 if(pszStringField[i]==47)
		 {
			 NULLINT = 1;
			 break;
		 }
	 }

	 if(NULLINT==1)
	   {
	     printf("NULLINT already! %d (%s) (%d)\n",pszStringField[i],pszStringField,iField);
	   }
	 
	 if(
	    (!strcmp(pszStringField,badvalue))  &&
	    (dDoubleField==0)                   &&
	    (pszStringField[0]<=44)
	    )
	   {
	     NULLINT = 1;
	   }
	 /* end Tim add */
	 
	 /* Tim delete 
	    dDoubleField = atof(pszStringField);
	 */
	 
	 
	 pReturnField = &dDoubleField;
	 
	 /*
	 if(NULLINT==1)
	   {
	     printf("NULLINT record %d field %d\n",hEntity,iField);
	   }
	 */
	 
    }
	
	


/* -------------------------------------------------------------------- */
/*      Should we trim white space off the string attribute value?      */
/* -------------------------------------------------------------------- */
#ifdef TRIM_TimDBF_WHITESPACE
    else
    {
        char	*pchSrc, *pchDst;

        pchDst = pchSrc = pszStringField;
        while( *pchSrc == ' ' )
            pchSrc++;

        while( *pchSrc != '\0' )
            *(pchDst++) = *(pchSrc++);
        *pchDst = '\0';

        while( pchDst != pszStringField && *(--pchDst) == ' ' )
            *pchDst = '\0';
    }
#endif
    
    return( pReturnField );
}

/************************************************************************/
/*                        TimDBFReadIntAttribute()                         */
/*                                                                      */
/*      Read an integer attribute.                                      */
/************************************************************************/

 
int TimDBFReadIntegerAttribute( TimDBFHandle psTimDBF, int iRecord, int iField )
     
{
  double	*pdValue;
  
  pdValue = (double *) TimDBFReadAttribute( psTimDBF, iRecord, iField, 'N' );
  
  if( pdValue == NULL )
    {
      return 0;
    }
  else
    {
      return( (int) *pdValue );
    }
}

/************************************************************************/
/*                        TimDBFReadDoubleAttribute()                      */
/*                                                                      */
/*      Read a double attribute.                                        */
/************************************************************************/

double TimSHPAPI_CALL
TimDBFReadDoubleAttribute( TimDBFHandle psTimDBF, int iRecord, int iField )

{
    double	*pdValue;

    pdValue = (double *) TimDBFReadAttribute( psTimDBF, iRecord, iField, 'N' );

    if( pdValue == NULL )
        return 0.0;
    else
        return( *pdValue );
}

/************************************************************************/
/*                        TimDBFReadStringAttribute()                      */
/*                                                                      */
/*      Read a string attribute.                                        */
/************************************************************************/

const char TimSHPAPI_CALL1(*)
TimDBFReadStringAttribute( TimDBFHandle psTimDBF, int iRecord, int iField )

{
    return( (const char *) TimDBFReadAttribute( psTimDBF, iRecord, iField, 'C' ) );
}

/************************************************************************/
/*                        TimDBFReadLogicalAttribute()                     */
/*                                                                      */
/*      Read a logical attribute.                                       */
/************************************************************************/

const char TimSHPAPI_CALL1(*)
TimDBFReadLogicalAttribute( TimDBFHandle psTimDBF, int iRecord, int iField )

{
    return( (const char *) TimDBFReadAttribute( psTimDBF, iRecord, iField, 'L' ) );
}

/************************************************************************/
/*                         TimDBFIsAttributeNULL()                         */
/*                                                                      */
/*      Return TRUE if value for field is NULL.                         */
/*                                                                      */
/*      Contributed by Jim Matthews.                                    */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFIsAttributeNULL( TimDBFHandle psTimDBF, int iRecord, int iField )

{
    const char	*pszValue;

    pszValue = TimDBFReadStringAttribute( psTimDBF, iRecord, iField );

    switch(psTimDBF->pachFieldType[iField])
    {
      case 'N':
      case 'F':
        /* NULL numeric fields have value "****************" */
        return pszValue[0] == '*';

      case 'D':
        /* NULL date fields have value "00000000" */
        return strncmp(pszValue,"00000000",8) == 0;

      case 'L':
        /* NULL boolean fields have value "?" */ 
        return pszValue[0] == '?';

      default:
        /* empty string fields are considered NULL */
        return strlen(pszValue) == 0;
    }
}

/************************************************************************/
/*                          TimDBFGetFieldCount()                          */
/*                                                                      */
/*      Return the number of fields in this table.                      */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFGetFieldCount( TimDBFHandle psTimDBF )

{
    return( psTimDBF->nFields );
}

/************************************************************************/
/*                         TimDBFGetRecordCount()                          */
/*                                                                      */
/*      Return the number of records in this table.                     */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFGetRecordCount( TimDBFHandle psTimDBF )

{
    return( psTimDBF->nRecords );
}

/************************************************************************/
/*                          TimDBFGetFieldInfo()                           */
/*                                                                      */
/*      Return any requested information about the field.               */
/************************************************************************/

TimDBFFieldType TimSHPAPI_CALL
TimDBFGetFieldInfo( TimDBFHandle psTimDBF, int iField, char * pszFieldName,
                 int * pnWidth, int * pnDecimals )

{
    if( iField < 0 || iField >= psTimDBF->nFields )
        return( FTInvalid );

    if( pnWidth != NULL )
        *pnWidth = psTimDBF->panFieldSize[iField];

    if( pnDecimals != NULL )
        *pnDecimals = psTimDBF->panFieldDecimals[iField];

    if( pszFieldName != NULL )
    {
	int	i;

	strncpy( pszFieldName, (char *) psTimDBF->pszHeader+iField*32, 11 );
	pszFieldName[11] = '\0';
	for( i = 10; i > 0 && pszFieldName[i] == ' '; i-- )
	    pszFieldName[i] = '\0';
    }

    if ( psTimDBF->pachFieldType[iField] == 'L' )
	return( FTLogical);


    if(  psTimDBF->pachFieldType[iField] == 'D' )
    return( FTSpecial ); /* to let me know its a date */


    if( psTimDBF->pachFieldType[iField] == 'N' 
             || psTimDBF->pachFieldType[iField] == 'F' )
    {
	  if( psTimDBF->panFieldDecimals[iField] > 0 )
	    return( FTDouble );
	  else
	    return( FTInteger );
    }
  
	return( FTString );
    



    
}

/************************************************************************/
/*                         TimDBFWriteAttribute()                          */
/*									*/
/*	Write an attribute record to the file.				*/
/************************************************************************/

static int TimDBFWriteAttribute(TimDBFHandle psTimDBF, int hEntity, int iField,
			     void * pValue )

{
    int	       	nRecordOffset, i, j, nRetResult = TRUE;
    unsigned char	*pabyRec;
    char	szSField[400], szFormat[20];

/* -------------------------------------------------------------------- */
/*	Is this a valid record?						*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity > psTimDBF->nRecords )
        return( FALSE );

    if( psTimDBF->bNoHeader )
        TimDBFWriteHeader(psTimDBF);

/* -------------------------------------------------------------------- */
/*      Is this a brand new record?                                     */
/* -------------------------------------------------------------------- */
    if( hEntity == psTimDBF->nRecords )
    {
	TimDBFFlushRecord( psTimDBF );

	psTimDBF->nRecords++;
	for( i = 0; i < psTimDBF->nRecordLength; i++ )
	    psTimDBF->pszCurrentRecord[i] = ' ';

	psTimDBF->nCurrentRecord = hEntity;
    }

/* -------------------------------------------------------------------- */
/*      Is this an existing record, but different than the last one     */
/*      we accessed?                                                    */
/* -------------------------------------------------------------------- */
    if( psTimDBF->nCurrentRecord != hEntity )
    {
	TimDBFFlushRecord( psTimDBF );

	nRecordOffset = psTimDBF->nRecordLength * hEntity + psTimDBF->nHeaderLength;

	fseek( psTimDBF->fp, nRecordOffset, 0 );
	fread( psTimDBF->pszCurrentRecord, psTimDBF->nRecordLength, 1, psTimDBF->fp );

	psTimDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (unsigned char *) psTimDBF->pszCurrentRecord;

    psTimDBF->bCurrentRecordModified = TRUE;
    psTimDBF->bUpdated = TRUE;

/* -------------------------------------------------------------------- */
/*      Translate NULL value to valid TimDBF file representation.          */
/*                                                                      */
/*      Contributed by Jim Matthews.                                    */
/* -------------------------------------------------------------------- */
    if( pValue == NULL )
    {
        switch(psTimDBF->pachFieldType[iField])
        {
          case 'N':
          case 'F':
	    /* NULL numeric fields have value "****************" */
            memset( (char *) (pabyRec+psTimDBF->panFieldOffset[iField]), '*', 
                    psTimDBF->panFieldSize[iField] );
            break;

          case 'D':
	    /* NULL date fields have value "00000000" */
            memset( (char *) (pabyRec+psTimDBF->panFieldOffset[iField]), '0', 
                    psTimDBF->panFieldSize[iField] );
            break;

          case 'L':
	    /* NULL boolean fields have value "?" */ 
            memset( (char *) (pabyRec+psTimDBF->panFieldOffset[iField]), '?', 
                    psTimDBF->panFieldSize[iField] );
            break;

          default:
            /* empty string fields are considered NULL */
            memset( (char *) (pabyRec+psTimDBF->panFieldOffset[iField]), '\0', 
                    psTimDBF->panFieldSize[iField] );
            break;
        }
        return TRUE;
    }

/* -------------------------------------------------------------------- */
/*      Assign all the record fields.                                   */
/* -------------------------------------------------------------------- */
    switch( psTimDBF->pachFieldType[iField] )
    {
      case 'D':
      case 'N':
      case 'F':
	if( psTimDBF->panFieldDecimals[iField] == 0 )
	{
            int		nWidth = psTimDBF->panFieldSize[iField];

            if( sizeof(szSField)-2 < nWidth )
                nWidth = sizeof(szSField)-2;

	    sprintf( szFormat, "%%%dd", nWidth );
	    sprintf(szSField, szFormat, (int) *((double *) pValue) );
	    if( (int)strlen(szSField) > psTimDBF->panFieldSize[iField] )
            {
	        szSField[psTimDBF->panFieldSize[iField]] = '\0';
                nRetResult = FALSE;
            }

	    strncpy((char *) (pabyRec+psTimDBF->panFieldOffset[iField]),
		    szSField, strlen(szSField) );
	}
	else
	{
            int		nWidth = psTimDBF->panFieldSize[iField];

            if( sizeof(szSField)-2 < nWidth )
                nWidth = sizeof(szSField)-2;

	    sprintf( szFormat, "%%%d.%df", 
                     nWidth, psTimDBF->panFieldDecimals[iField] );
	    sprintf(szSField, szFormat, *((double *) pValue) );
	    if( (int) strlen(szSField) > psTimDBF->panFieldSize[iField] )
            {
	        szSField[psTimDBF->panFieldSize[iField]] = '\0';
                nRetResult = FALSE;
            }
	    strncpy((char *) (pabyRec+psTimDBF->panFieldOffset[iField]),
		    szSField, strlen(szSField) );
	}
	break;

      case 'L':
        if (psTimDBF->panFieldSize[iField] >= 1  && 
            (*(char*)pValue == 'F' || *(char*)pValue == 'T'))
            *(pabyRec+psTimDBF->panFieldOffset[iField]) = *(char*)pValue;
        break;

      default:
	if( (int) strlen((char *) pValue) > psTimDBF->panFieldSize[iField] )
        {
	    j = psTimDBF->panFieldSize[iField];
            nRetResult = FALSE;
        }
	else
        {
            memset( pabyRec+psTimDBF->panFieldOffset[iField], ' ',
                    psTimDBF->panFieldSize[iField] );
	    j = strlen((char *) pValue);
        }

	strncpy((char *) (pabyRec+psTimDBF->panFieldOffset[iField]),
		(char *) pValue, j );
	break;
    }

    return( nRetResult );
}

/************************************************************************/
/*                     TimDBFWriteAttributeDirectly()                      */
/*                                                                      */
/*      Write an attribute record to the file, but without any          */
/*      reformatting based on type.  The provided buffer is written     */
/*      as is to the field position in the record.                      */
/************************************************************************/

int TimDBFWriteAttributeDirectly(TimDBFHandle psTimDBF, int hEntity, int iField,
                              void * pValue )

{
    int	       	nRecordOffset, i, j;
    unsigned char	*pabyRec;

/* -------------------------------------------------------------------- */
/*	Is this a valid record?						*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity > psTimDBF->nRecords )
        return( FALSE );

    if( psTimDBF->bNoHeader )
        TimDBFWriteHeader(psTimDBF);

/* -------------------------------------------------------------------- */
/*      Is this a brand new record?                                     */
/* -------------------------------------------------------------------- */
    if( hEntity == psTimDBF->nRecords )
    {
	TimDBFFlushRecord( psTimDBF );

	psTimDBF->nRecords++;
	for( i = 0; i < psTimDBF->nRecordLength; i++ )
	    psTimDBF->pszCurrentRecord[i] = ' ';

	psTimDBF->nCurrentRecord = hEntity;
    }

/* -------------------------------------------------------------------- */
/*      Is this an existing record, but different than the last one     */
/*      we accessed?                                                    */
/* -------------------------------------------------------------------- */
    if( psTimDBF->nCurrentRecord != hEntity )
    {
	TimDBFFlushRecord( psTimDBF );

	nRecordOffset = psTimDBF->nRecordLength * hEntity + psTimDBF->nHeaderLength;

	fseek( psTimDBF->fp, nRecordOffset, 0 );
	fread( psTimDBF->pszCurrentRecord, psTimDBF->nRecordLength, 1, psTimDBF->fp );

	psTimDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (unsigned char *) psTimDBF->pszCurrentRecord;

/* -------------------------------------------------------------------- */
/*      Assign all the record fields.                                   */
/* -------------------------------------------------------------------- */
    if( (int)strlen((char *) pValue) > psTimDBF->panFieldSize[iField] )
        j = psTimDBF->panFieldSize[iField];
    else
    {
        memset( pabyRec+psTimDBF->panFieldOffset[iField], ' ',
                psTimDBF->panFieldSize[iField] );
        j = strlen((char *) pValue);
    }

    strncpy((char *) (pabyRec+psTimDBF->panFieldOffset[iField]),
            (char *) pValue, j );

    psTimDBF->bCurrentRecordModified = TRUE;
    psTimDBF->bUpdated = TRUE;

    return( TRUE );
}

/************************************************************************/
/*                      TimDBFWriteDoubleAttribute()                       */
/*                                                                      */
/*      Write a double attribute.                                       */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFWriteDoubleAttribute( TimDBFHandle psTimDBF, int iRecord, int iField,
                         double dValue )

{
    return( TimDBFWriteAttribute( psTimDBF, iRecord, iField, (void *) &dValue ) );
}

/************************************************************************/
/*                      TimDBFWriteIntegerAttribute()                      */
/*                                                                      */
/*      Write a integer attribute.                                      */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFWriteIntegerAttribute( TimDBFHandle psTimDBF, int iRecord, int iField,
                          int nValue )

{
    double	dValue = nValue;

    return( TimDBFWriteAttribute( psTimDBF, iRecord, iField, (void *) &dValue ) );
}

/************************************************************************/
/*                      TimDBFWriteStringAttribute()                       */
/*                                                                      */
/*      Write a string attribute.                                       */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFWriteStringAttribute( TimDBFHandle psTimDBF, int iRecord, int iField,
                         const char * pszValue )

{
    return( TimDBFWriteAttribute( psTimDBF, iRecord, iField, (void *) pszValue ) );
}

/************************************************************************/
/*                      TimDBFWriteNULLAttribute()                         */
/*                                                                      */
/*      Write a string attribute.                                       */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFWriteNULLAttribute( TimDBFHandle psTimDBF, int iRecord, int iField )

{
    return( TimDBFWriteAttribute( psTimDBF, iRecord, iField, NULL ) );
}

/************************************************************************/
/*                      TimDBFWriteLogicalAttribute()                      */
/*                                                                      */
/*      Write a logical attribute.                                      */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFWriteLogicalAttribute( TimDBFHandle psTimDBF, int iRecord, int iField,
		       const char lValue)

{
    return( TimDBFWriteAttribute( psTimDBF, iRecord, iField, (void *) (&lValue) ) );
}

/************************************************************************/
/*                         TimDBFWriteTuple()                              */
/*									*/
/*	Write an attribute record to the file.				*/
/************************************************************************/

int TimSHPAPI_CALL
TimDBFWriteTuple(TimDBFHandle psTimDBF, int hEntity, void * pRawTuple )

{
    int	       	nRecordOffset, i;
    unsigned char	*pabyRec;

/* -------------------------------------------------------------------- */
/*	Is this a valid record?						*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity > psTimDBF->nRecords )
        return( FALSE );

    if( psTimDBF->bNoHeader )
        TimDBFWriteHeader(psTimDBF);

/* -------------------------------------------------------------------- */
/*      Is this a brand new record?                                     */
/* -------------------------------------------------------------------- */
    if( hEntity == psTimDBF->nRecords )
    {
	TimDBFFlushRecord( psTimDBF );

	psTimDBF->nRecords++;
	for( i = 0; i < psTimDBF->nRecordLength; i++ )
	    psTimDBF->pszCurrentRecord[i] = ' ';

	psTimDBF->nCurrentRecord = hEntity;
    }

/* -------------------------------------------------------------------- */
/*      Is this an existing record, but different than the last one     */
/*      we accessed?                                                    */
/* -------------------------------------------------------------------- */
    if( psTimDBF->nCurrentRecord != hEntity )
    {
	TimDBFFlushRecord( psTimDBF );

	nRecordOffset = psTimDBF->nRecordLength * hEntity + psTimDBF->nHeaderLength;

	fseek( psTimDBF->fp, nRecordOffset, 0 );
	fread( psTimDBF->pszCurrentRecord, psTimDBF->nRecordLength, 1, psTimDBF->fp );

	psTimDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (unsigned char *) psTimDBF->pszCurrentRecord;

    memcpy ( pabyRec, pRawTuple,  psTimDBF->nRecordLength );

    psTimDBF->bCurrentRecordModified = TRUE;
    psTimDBF->bUpdated = TRUE;

    return( TRUE );
}

/************************************************************************/
/*                          TimDBFReadTuple()                              */
/*                                                                      */
/*      Read one of the attribute fields of a record.                   */
/************************************************************************/

const char TimSHPAPI_CALL1(*)
TimDBFReadTuple(TimDBFHandle psTimDBF, int hEntity )

{
    int	       	nRecordOffset;
    unsigned char	*pabyRec;
    static char	*pReturnTuple = NULL;

    static int	nTupleLen = 0;

/* -------------------------------------------------------------------- */
/*	Have we read the record?					*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity >= psTimDBF->nRecords )
        return( NULL );

    if( psTimDBF->nCurrentRecord != hEntity )
    {
	TimDBFFlushRecord( psTimDBF );

	nRecordOffset = psTimDBF->nRecordLength * hEntity + psTimDBF->nHeaderLength;

	fseek( psTimDBF->fp, nRecordOffset, 0 );
	fread( psTimDBF->pszCurrentRecord, psTimDBF->nRecordLength, 1, psTimDBF->fp );

	psTimDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (unsigned char *) psTimDBF->pszCurrentRecord;

    if ( nTupleLen < psTimDBF->nRecordLength) {
      nTupleLen = psTimDBF->nRecordLength;
      pReturnTuple = (char *) SfRealloc(pReturnTuple, psTimDBF->nRecordLength);
    }
    
    memcpy ( pReturnTuple, pabyRec, psTimDBF->nRecordLength );
        
    return( pReturnTuple );
}

/************************************************************************/
/*                          TimDBFCloneEmpty()                              */
/*                                                                      */
/*      Read one of the attribute fields of a record.                   */
/************************************************************************/

TimDBFHandle TimSHPAPI_CALL
TimDBFCloneEmpty(TimDBFHandle psTimDBF, const char * pszFilename ) 
{
    TimDBFHandle	newTimDBF;

   newTimDBF = TimDBFCreate ( pszFilename );
   if ( newTimDBF == NULL ) return ( NULL ); 
   
   newTimDBF->pszHeader = (char *) malloc ( 32 * psTimDBF->nFields );
   memcpy ( newTimDBF->pszHeader, psTimDBF->pszHeader, 32 * psTimDBF->nFields );
   
   newTimDBF->nFields = psTimDBF->nFields;
   newTimDBF->nRecordLength = psTimDBF->nRecordLength;
   newTimDBF->nHeaderLength = 32 * (psTimDBF->nFields+1);
    
   newTimDBF->panFieldOffset = (int *) malloc ( sizeof(int) * psTimDBF->nFields ); 
   memcpy ( newTimDBF->panFieldOffset, psTimDBF->panFieldOffset, sizeof(int) * psTimDBF->nFields );
   newTimDBF->panFieldSize = (int *) malloc ( sizeof(int) * psTimDBF->nFields );
   memcpy ( newTimDBF->panFieldSize, psTimDBF->panFieldSize, sizeof(int) * psTimDBF->nFields );
   newTimDBF->panFieldDecimals = (int *) malloc ( sizeof(int) * psTimDBF->nFields );
   memcpy ( newTimDBF->panFieldDecimals, psTimDBF->panFieldDecimals, sizeof(int) * psTimDBF->nFields );
   newTimDBF->pachFieldType = (char *) malloc ( sizeof(int) * psTimDBF->nFields );
   memcpy ( newTimDBF->pachFieldType, psTimDBF->pachFieldType, sizeof(int) * psTimDBF->nFields );

   newTimDBF->bNoHeader = TRUE;
   newTimDBF->bUpdated = TRUE;
   
   TimDBFWriteHeader ( newTimDBF );
   TimDBFClose ( newTimDBF );
   
   newTimDBF = TimDBFOpen ( pszFilename, "rb+" );

   return ( newTimDBF );
}

/************************************************************************/
/*                       TimDBFGetNativeFieldType()                        */
/*                                                                      */
/*      Return the DBase field type for the specified field.            */
/*                                                                      */
/*      Value can be one of: 'C' (String), 'D' (Date), 'F' (Float),     */
/*                           'N' (Numeric, with or without decimal),    */
/*                           'L' (Logical),                             */
/*                           'M' (Memo: 10 digits .DBT block ptr)       */
/************************************************************************/

char TimSHPAPI_CALL
TimDBFGetNativeFieldType( TimDBFHandle psTimDBF, int iField )

{
    if( iField >=0 && iField < psTimDBF->nFields )
        return psTimDBF->pachFieldType[iField];

    return  ' ';
}

/************************************************************************/
/*                            str_to_upper()                            */
/************************************************************************/

static void str_to_upper (char *string)
{
    int len;
    short i = -1;

    len = strlen (string);

    while (++i < len)
        if (isalpha(string[i]) && islower(string[i]))
            string[i] = toupper ((int)string[i]);
}

/************************************************************************/
/*                          TimDBFGetFieldIndex()                          */
/*                                                                      */
/*      Get the index number for a field in a .dbf file.                */
/*                                                                      */
/*      Contributed by Jim Matthews.                                    */
/************************************************************************/

int TimSHPAPI_CALL
TimDBFGetFieldIndex(TimDBFHandle psTimDBF, const char *pszFieldName)

{
    char          name[12], name1[12], name2[12];
    int           i;

    strncpy(name1, pszFieldName,11);
    name1[11] = '\0';
    str_to_upper(name1);

    for( i = 0; i < TimDBFGetFieldCount(psTimDBF); i++ )
    {
        TimDBFGetFieldInfo( psTimDBF, i, name, NULL, NULL );
        strncpy(name2,name,11);
        str_to_upper(name2);

        if(!strncmp(name1,name2,10))
            return(i);
    }
    return(-1);
}
