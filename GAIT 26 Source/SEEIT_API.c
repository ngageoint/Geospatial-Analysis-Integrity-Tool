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
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "share.h" 

extern int API_DRAW;
extern char * ParseGAITgeometry(unsigned char geom, int CaseFlag);

#define DEBUG 0
double EXPORT2DSENTINEL=0.0;
extern void DrawIndividualConditionObject(struct ConditionList *C); 
extern int AbortConditionDraw();
extern void ResetZoom();
extern void SkipDynamicInfo(int keyval, FILE *infile);
extern int SLASHTYPE;
#define DOS_TYPE 1
#define NONDOS_TYPE 0
int global_ctype,global_cnum,global_instance;
long int global_fileposn;
extern int DRAW_NOW;
extern long int use_other_fileposn;
extern double db_min_z,db_max_z;
double modified_db_min_z,modified_db_max_z;

int VertexPrintLimit = 25000;
int UniqueMagsFound  = 0;
int LastMagFound     = -1;

int CoordinateSystem=1;

int TotalConditions;



double Xtranslation = 0.0;
double Ytranslation = 0.0; 


struct ConditionTable *ConditionLookup = NULL;


#define DT_int     0
#define DT_long    1
#define DT_double  2
#define DT_char    3
#define DT_short   4
#define DT_float   5
int SzD=sizeof(double);
int SzI=sizeof(int);
int SzL=sizeof(long int);
int SzShort=sizeof(unsigned short int);
int SzUC = sizeof(unsigned char);
int SzF = sizeof(float);



int machine_endianness;
int file_endianness;
int condition_endianness;
int UseDMScoordinates = 0;



int bytes_written = 0;

int    *G_length;
int    *G_offsets;

int MaxAnnLenFound  = 0;
int MaxNam2LenFound = 0;
int MaxNam3LenFound = 0;
int MaxNamLenFound  = 0;
int MaxSedLenFound  = 0;
int maxtablelen     = 0;

int lines_found_export,points_found_export;
int lines_done,points_done;

double Gminx,Gmaxx,Gminy,Gmaxy,Gminz,Gmaxz;
int LINETYPE  = 13;
int POINTTYPE = 11;


const unsigned char SH_POINT    = 1;
const unsigned char SH_CYLINDER = 2;
const unsigned char SH_SPHERE   = 3;
const unsigned char SH_CIRCLE   = 4;
const unsigned char SH_GRIDPT   = 10;

const unsigned char SH_PARALLELEPIPED = 5;
const unsigned char SH_POLYGON        = 6;
const unsigned char SH_LINE           = 7;
const unsigned char SH_AREAL          = 8;

const unsigned char SH_NONE           = 9;












int Ctype()
{

  if(
     (CoordinateSystem == 2)||
     (CoordinateSystem == 3)||
     (CoordinateSystem == 4)||
     (CoordinateSystem == 5)||
     (CoordinateSystem == 6)||
     (CoordinateSystem == 11)||
     (CoordinateSystem == 12)||
     (CoordinateSystem == 13)||
     (CoordinateSystem == 14)||
     (CoordinateSystem == 15)||
     (CoordinateSystem == 16)
     )
    {
      return 2;
    }
  else if(
	  (CoordinateSystem == 1)||
	  (CoordinateSystem == 7)||
	  (CoordinateSystem == 8)||
	  (CoordinateSystem == 9)||
	  (CoordinateSystem == 10)
	  )
    {
      return 1;
    }
  else
    {
      printf("Invalid Coordinate System identifier %d\n",CoordinateSystem);
      exit(1);
    }
  return -999;
}


int determine_machine_endianness(void)
{
  union {
    char c[4];
    long i;
  } x;
  
  x.i = 0;
  x.c[3] = 1;
  
  if (x.i == 1) 
    {
      return BIG;
    } 
  else 
    {
      return LITTLE;
    }
}





#define FIX_ENDIANW(data, dtype) fix_endianw((char *) data, dtype)
#define FIX_ENDIAN( data, dtype) fix_endian( (char *) data, dtype)
#define byte_swap(x,y) { c=x; x=y; y=c; }




/* Write data according to correct endianness */


void fix_endianw(char *data, int data_type)
{
  char c;

  /* if machine type matches file type, nothing to do */
  if (machine_endianness == file_endianness) return ;


  /* if data type is character, we don't swap */
  if (data_type == DT_char) return;


  /* for real types, just reverse order of bytes */

  /* longs and 4 byte ints */
  if ( (data_type == DT_long) || (data_type == DT_float) ||
      ((data_type == DT_int)  && (SzI == 4)) ) {
    byte_swap(data[0], data[3]);
    byte_swap(data[1], data[2]);
    return ;
  }

  /* 2 byte ints */
  if ((data_type == DT_int) || (data_type == DT_short)) {
    byte_swap(data[0], data[1]);
  }

  /* double */
  if (data_type == DT_double) {
    byte_swap(data[0], data[7]);
    byte_swap(data[1], data[6]);
    byte_swap(data[2], data[5]);
    byte_swap(data[3], data[4]);

  }

  return ;
}

void SEEIT_fwrite_int(int *i, FILE *fp)
{
  int ic;

  memcpy( &ic, i, SzI);

  FIX_ENDIANW(&ic, DT_int);

  fwrite(&ic, SzI, 1, fp);
}


void SEEIT_fwrite_long(long *li, FILE *fp)
{
  long lic;

  memcpy( &lic, li, SzL);

  FIX_ENDIANW(&lic, DT_long);

  fwrite(&lic, SzL, 1, fp);
}

void SEEIT_fwrite_short(short int *si, FILE *fp)
{
  short sic;

  memcpy(&sic, si, SzShort);

  FIX_ENDIANW(&sic, DT_short);

  fwrite(&sic, SzShort, 1, fp);
}

void SEEIT_fwrite_double(double *d, FILE *fp)
{
  double dc;

  memcpy(&dc, d, SzD);

  FIX_ENDIANW(&dc, DT_double);

  fwrite(&dc, SzD, 1, fp);
}

void SEEIT_fwrite_char(char *c, FILE *fp)
{
  fwrite(c, SzUC, 1, fp);
}





void fix_endian(char *data, int data_type)
{
  char c;
  
  /* if machine type matches file type, nothing to do */
  if (machine_endianness == file_endianness) return ;

  /* if data type is character, we don't swap */
  if (data_type == DT_char) return;  
  
  /* for real types, just reverse order of bytes */
  
  /* longs and 4 byte ints */
  if ( (data_type == DT_long) ||
       ((data_type == DT_int)  && (SzI == 4)) ) 
    {
      byte_swap(data[0], data[3]);
      byte_swap(data[1], data[2]);
      return ;
    }
  
  /* 2 byte ints */
  if ((data_type == DT_int) || (data_type == DT_short)) 
    {
      byte_swap(data[0], data[1]);
    }
  
  /* double */
  if (data_type == DT_double) 
    {
      byte_swap(data[0], data[7]);
      byte_swap(data[1], data[6]);
      byte_swap(data[2], data[5]);
      byte_swap(data[3], data[4]);
    }  
  return ;
}



void SEEIT_fread_int(int *i, FILE *fp)
{
  fread(i, SzI, 1, fp);


  FIX_ENDIAN(i, DT_int);

}

void SEEIT_fread_uint(unsigned int *i, FILE *fp)
{
  fread(i, SzI, 1, fp);

  FIX_ENDIAN(i, DT_int);

}



void SEEIT_fread_float(float *f, FILE *fp)
{
  fread(f, SzF, 1, fp);
   
  FIX_ENDIAN(f, DT_float);
}


void SEEIT_fread_long(long *li, FILE *fp)
{
  fread(li, SzL, 1, fp);
  
  FIX_ENDIAN(li, DT_long);
}

void SEEIT_fread_short(short int *si, FILE *fp)
{
  fread(si, SzShort, 1, fp);
  
  FIX_ENDIAN(si, DT_short);
}
void SEEIT_fread_ushort(unsigned short int *si, FILE *fp)
{
  fread(si, SzShort, 1, fp);
  
  FIX_ENDIAN(si, DT_short);
}


void SEEIT_fread_double(double *d, FILE *fp)
{
  fread(d, SzD, 1, fp);
  
  FIX_ENDIAN(d, DT_double);
}

void SEEIT_fread_char(char *c, FILE *fp)
{
  fread(c, SzUC, 1, fp);
}


void SEEIT_MassRead_double(double *d, int NumValues, FILE *fp)
{
int i;
   fread(d, SzD, NumValues, fp);
   if (machine_endianness == file_endianness) 
      return ;
   else
      {
      for(i=0; i<NumValues; i++)
         FIX_ENDIAN(&d[i], DT_double);
      }
}

void SEEIT_MassRead_short(short int *si, int NumValues, FILE *fp)
{
int i;
   fread(si, SzShort, NumValues, fp);
   if (machine_endianness == file_endianness)
      return ;
   else
      {
      for(i=0; i<NumValues; i++)
         FIX_ENDIAN(&si[i], DT_short);
      }
}





void SkipObject(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1;
  char gform1,keepit;
  double localid,radius1,height1;
  int numverts1,ECC;
  int Lindex;
  double x1, y1, z1;
  
  if(infile == NULL)
    return;
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  fread(&keepit,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}


void SkipTwoObjects(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1;
  char gform1;
  double localid;
  double radius1,height1;
  int numverts1, ECC;
  double x1, y1, z1;
  int Lindex1;
  
  if(infile == NULL)
    return;
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  /** second object ***/
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}



void SkipObjectAndMessage(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1;
  char gform1;
  double localid;
  double radius1,height1;
  int numverts1, ECC;
  double x1, y1, z1;
  int msglen;
  int Lindex;
  
  if(infile == NULL)
    return;
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&msglen,infile);
  fseek(infile,msglen,SEEK_CUR);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}


void SkipObjectAndMagnitude(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1, ECC;
  char gform1;
  double localid;
  double radius1,height1;
  int numverts1;
  double x1, y1, z1;
  double magnitude;
  int Lindex;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&magnitude,infile);
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}



void SkipMsgMagPointObjects(int keyval, FILE *infile)
{
  int i,slen;
  int idn1,SIDlen1;
  char gform1;
  double localid;
  double radius1,height1;
  int numverts1, ECC;
  double x1, y1, z1;
  double pointx, pointy, pointz;
  double magnitude;
  int Lindex1, Lindex2;
  char message[500];
  char * bigmessage = NULL;

  if(infile == NULL)
    return;

  SEEIT_fread_int(&slen,infile);
  if(slen > 0)
    {
    if(slen < 500)
       fread(&message[0],1,slen,infile);
    else
       {
       bigmessage = (char *) (malloc(slen + 2));
       fread(&bigmessage[0],1,slen,infile);
       }
    }
  SEEIT_fread_double(&magnitude,infile);



  SEEIT_fread_double(&pointx, infile);
  SEEIT_fread_double(&pointy, infile);
  SEEIT_fread_double(&pointz, infile);

  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);

  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);

  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid,infile);

  SEEIT_fread_double(&radius1,infile);

  SEEIT_fread_double(&height1,infile);

  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }

  /** second object ***/
  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);

  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);

  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex2,infile);

  SEEIT_fread_double(&localid,infile);

  SEEIT_fread_double(&radius1,infile);

  SEEIT_fread_double(&height1,infile);

  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }

  if(slen >= 500)
    {
    free(bigmessage);
    bigmessage = NULL;
    }

  return;
}




void SkipPointEdgeAndMagnitude(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1, ECC;
  char gform1;
  double localid;
  double x1, y1, z1;
  double magnitude;
  int Lindex;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&magnitude,infile);
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&x1, infile);
  SEEIT_fread_double(&y1, infile);
  SEEIT_fread_double(&z1, infile);
  
  /** second object **/
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);
  
  SEEIT_fread_double(&localid,infile);

  for (i=0; i<2; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}



void SkipPointObjectAndMagnitude(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1, ECC,numverts;
  char gform1;
  double localid;
  double x1, y1, z1;
  double magnitude;
  int Lindex;

  if(infile == NULL)
    return;

  SEEIT_fread_double(&magnitude,infile);

  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);

  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);

  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid, infile);

  SEEIT_fread_double(&x1, infile);
  SEEIT_fread_double(&y1, infile);
  SEEIT_fread_double(&z1, infile);

  /** second object **/
  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);

  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);

  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid, infile);

  SEEIT_fread_int(&numverts,infile);

  for (i=0; i<numverts; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }

  return;
}



void SkipMagnitudeAndTwoObjects(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1;
  char gform1;
  double localid;
  int ECC;
  double radius1,height1;
  int numverts1;
  double x1, y1, z1;
  double magnitude;
  int Lindex1, Lindex2;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&magnitude,infile);
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC, infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1, infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  /** second object ***/
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC, infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex2, infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}



void SkipPointAndTwoObjects(int keyval, FILE *infile)
{
  int i;
  int idn1,SIDlen1;
  char gform1;
  double localid;
  double radius1,height1;
  int numverts1, ECC;
  double x1, y1, z1;
  double pointx, pointy, pointz;
  int Lindex1, Lindex2;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&pointx, infile);
  SEEIT_fread_double(&pointy, infile);
  SEEIT_fread_double(&pointz, infile);
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  /** second object ***/
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex2,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}





void SkipPointAndObject(int keyval, FILE *infile)
{
  int i;
  int idn1, idn2, SIDlen1, SIDlen2, ECC;
  char gform1;
  double localid;
  double radius1,height1;
  int numverts1;
  int Lindex;
  double x1, y1, z1;
  double pointx, pointy, pointz;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&pointx, infile);
  SEEIT_fread_double(&pointy, infile);
  SEEIT_fread_double(&pointz, infile);
  
  SEEIT_fread_int(&idn2,infile);
  SEEIT_fread_int(&SIDlen2,infile);
 
  fseek(infile,SIDlen2,SEEK_CUR);

  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  fseek(infile,SIDlen1,SEEK_CUR);

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1, infile);
      SEEIT_fread_double(&y1, infile);
      SEEIT_fread_double(&z1, infile);
    }
  
  return;
}




void InstanceConditionObjects(struct ConditionList * condition,
			      int ObjID, char * SID, char gform, double radius, double height, int numverts,
			      int Lindex, double FandP, double *x, double *y, double *z)
{
  int i;
  switch(gform)
    {
    case G_AREAL:
      if(condition->num_areals == 0)
	{
	  condition->areal1.threeD = 1;
          condition->areal1.objectid = ObjID;
	  condition->areal1.numverts = numverts;
          condition->areal1.Lindex = Lindex;
          condition->areal1.FileAndPosn = FandP;
	  condition->areal1.x = x;
	  condition->areal1.y = y;
	  condition->areal1.z = z;
	  condition->areal1.IDENT = SID;
	  condition->num_areals = 1;
	}
      else
	{
	  condition->areal2.threeD = 1;
          condition->areal2.objectid = ObjID;
	  condition->areal2.numverts = numverts;
          condition->areal2.Lindex = Lindex;
          condition->areal2.FileAndPosn = FandP;
	  condition->areal2.x = x;
	  condition->areal2.y = y;
	  condition->areal2.z = z;
	  condition->areal2.IDENT = SID;
	  ++condition->num_areals;
	}
      break;
    case G_CIRCLE:
      if(condition->num_circles == 0)
	{
	  condition->circle1.threeD = 1;
          condition->circle1.objectid = ObjID;
          condition->circle1.Lindex = Lindex;
          condition->circle1.FileAndPosn = FandP;
	  condition->circle1.x = x[0];
	  condition->circle1.y = y[0];
	  condition->circle1.z = z[0];
	  condition->circle1.IDENT = SID;
	  condition->circle1.radius = radius;
	  condition->num_circles = 1;
	  free(x);
	  free(y);
	  free(z);
	}
      else
	{
	  condition->circle2.threeD = 1;
          condition->circle2.objectid = ObjID;
          condition->circle2.Lindex = Lindex;
          condition->circle2.FileAndPosn = FandP;
	  condition->circle2.x = x[0];
	  condition->circle2.y = y[0];
	  condition->circle2.z = z[0];
	  condition->circle2.IDENT = SID;
	  condition->circle2.radius = radius;
	  ++condition->num_circles;
	  free(x);
	  free(y);
	  free(z);
	}
      break;
    case G_SPHERE:
      if(condition->num_sphere == 0)
	{
	  condition->sphere1.threeD = 1;
          condition->sphere1.objectid = ObjID;
          condition->sphere1.Lindex = Lindex;
          condition->sphere1.FileAndPosn = FandP;
	  condition->sphere1.x = x[0];
	  condition->sphere1.y = y[0];
	  condition->sphere1.z = z[0];
	  condition->sphere1.IDENT = SID;
	  condition->sphere1.radius = radius;
	  condition->num_sphere = 1;
	  free(x);
	  free(y);
	  free(z);
	}
      else
	{
	  condition->sphere2.threeD = 1;
          condition->sphere2.objectid = ObjID;
          condition->sphere2.Lindex = Lindex;
          condition->sphere2.FileAndPosn = FandP;
	  condition->sphere2.x = x[0];
	  condition->sphere2.y = y[0];
	  condition->sphere2.z = z[0];
	  condition->sphere2.IDENT = SID;
	  condition->sphere2.radius = radius;
	  ++condition->num_sphere;
	  free(x);
	  free(y);
	  free(z);
	}
      break;
    case G_POLYGON:
      if(condition->num_polys == 0)
	{
	  condition->poly1.threeD = 1;
          condition->poly1.objectid = ObjID;
          condition->poly1.Lindex = Lindex;
          condition->poly1.FileAndPosn = FandP;
	  condition->poly1.numverts = numverts;
	  condition->poly1.x = x;
	  condition->poly1.y = y;
	  condition->poly1.z = z;
	  condition->poly1.IDENT = SID;
	  condition->num_polys = 1;
	}
      else
	{
	  condition->poly2.threeD = 1;
          condition->poly2.objectid = ObjID;
          condition->poly2.Lindex = Lindex;
          condition->poly2.FileAndPosn = FandP;
	  condition->poly2.numverts = numverts;
	  condition->poly2.x = x;
	  condition->poly2.y = y;
	  condition->poly2.z = z;
	  condition->poly2.IDENT = SID;
	  condition->num_polys = 2;
	}
      break;
    case G_LINE:
      if(condition->num_lines == 0)
	{
	  condition->linear1.threeD = 1;
          condition->linear1.objectid = ObjID;
          condition->linear1.Lindex = Lindex;
          condition->linear1.FileAndPosn = FandP;
	  condition->linear1.numverts = numverts;
	  condition->linear1.x = x;
	  condition->linear1.y = y;
	  condition->linear1.z = z;
	  condition->linear1.IDENT = SID;
	  condition->num_lines = 1;
	}
      else
	{
	  condition->linear2.threeD = 1;
          condition->linear2.objectid = ObjID;
          condition->linear2.Lindex = Lindex;
          condition->linear2.FileAndPosn = FandP;
	  condition->linear2.numverts = numverts;
	  condition->linear2.x = x;
	  condition->linear2.y = y;
	  condition->linear2.z = z;
	  condition->linear2.IDENT = SID;
	  condition->num_lines = 2;
	}
      break;
    case G_POINT:
      if(condition->num_points == 0)
	{
	  condition->point1.threeD = 1;
          condition->point1.objectid = ObjID;
          condition->point1.Lindex = Lindex;
          condition->point1.FileAndPosn = FandP;
	  condition->point1.x = x[0];
	  condition->point1.y = y[0];
	  condition->point1.z = z[0];
	  condition->point1.IDENT = SID;
	  condition->num_points = 1;
	  free(x);
	  free(y);
	  free(z);
	}
      else
	{
	  condition->point2.threeD = 1;
          condition->point2.objectid = ObjID;
          condition->point2.Lindex = Lindex;
          condition->point2.FileAndPosn = FandP;
	  condition->point2.x = x[0];
	  condition->point2.y = y[0];
	  condition->point2.z = z[0];
	  condition->point2.IDENT = SID;
	  condition->num_points = 2;
	  free(x);
	  free(y);
	  free(z);
	}
      break;
    case G_GRIDPT:
      if(condition->num_gridpt == 0)
	{
	  condition->gridobj1.threeD = 1;
          condition->gridobj1.objectid = ObjID;
          condition->gridobj1.Lindex = Lindex;
          condition->gridobj1.FileAndPosn = FandP;
	  condition->gridobj1.numverts = numverts;
	  condition->gridobj1.x = x;
	  condition->gridobj1.y = y;
	  condition->gridobj1.z = z;
	  condition->gridobj1.IDENT = SID;
	  condition->num_gridpt = 1;
	}
      else
	{
	  condition->gridobj2.threeD = 1;
          condition->gridobj2.objectid = ObjID;
          condition->gridobj2.Lindex = Lindex;
          condition->gridobj2.FileAndPosn = FandP;
	  condition->gridobj2.numverts = numverts;
	  condition->gridobj2.x = x;
	  condition->gridobj2.y = y;
	  condition->gridobj2.z = z;
	  condition->gridobj2.IDENT = SID;
	  condition->num_gridpt = 2;
	}
      break;
    case G_CYLINDER:
      if(condition->num_cylinder == 0)
	{
          condition->cylinder1.objectid = ObjID;
          condition->cylinder1.Lindex = Lindex;
          condition->cylinder1.FileAndPosn = FandP;
	  condition->cylinder1.x = x[0];
	  condition->cylinder1.y = y[0];
	  condition->cylinder1.z = z[0];
	  condition->cylinder1.radius = radius;
	  condition->cylinder1.height = height;
	  condition->num_cylinder = 1;
	  free(x);
	  free(y);
	  free(z);
	}
      else
	{
          condition->cylinder2.objectid = ObjID;
          condition->cylinder2.Lindex = Lindex;
          condition->cylinder2.FileAndPosn = FandP;
	  condition->cylinder2.x = x[0];
	  condition->cylinder2.y = y[0];
	  condition->cylinder2.z = z[0];
	  condition->cylinder2.radius = radius;
	  condition->cylinder2.height = height;
	  condition->num_cylinder = 2;
	  free(x);
	  free(y);
	  free(z);
	}
      break;
    case G_PIPED:
      if(condition->num_piped == 0)
	{
	  condition->piped1.height = height;
	  condition->piped1.IDENT = SID;
          condition->piped1.objectid = ObjID;
          condition->piped1.Lindex = Lindex;
          condition->piped1.FileAndPosn = FandP;
	  for(i=0; i<4; i++)
	    {
	      condition->piped1.x[i] = x[i];
	      condition->piped1.y[i] = y[i];
	      condition->piped1.z[i] = z[i];
	    }
	  condition->num_piped = 1;
	  free(x);
	  free(y);
	  free(z);
	}
      else
	{
	  condition->piped2.height = height;
	  condition->piped2.IDENT = SID;
          condition->piped2.objectid = ObjID;
          condition->piped2.Lindex = Lindex;
          condition->piped2.FileAndPosn = FandP;
	  for(i=0; i<4; i++)
	    {
	      condition->piped2.x[i] = x[i];
	      condition->piped2.y[i] = y[i];
	      condition->piped2.z[i] = z[i];
	    }
	  condition->num_piped = 2;
	  free(x);
	  free(y);
	  free(z);
	}
      break;
      
    default:
      printf("bad data read from condition report file\n");
      printf("terminating execution\n");
      exit(1);
    }
  
  return;
}




void FreadPointAndObject(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1, idn2, SIDlen1, SIDlen2;
  char *SID1;
  char gform1;
  double localid;
  double radius1;
  double height1;
  int ECC;
  int numverts1;
  double *x1, *y1, *z1;
  int Lindex1;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&condition->pointx, infile);
  SEEIT_fread_double(&condition->pointy, infile);
  SEEIT_fread_double(&condition->pointz, infile);
  condition->AdditionalPointUsed = 1;

  SEEIT_fread_int(&idn2,infile);
 
  SEEIT_fread_int(&SIDlen2,infile);
 
  condition->AdditionalIDENT = (char *) (malloc(SIDlen2+1));
  if(condition->AdditionalIDENT == NULL)
    {
      printf("1 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&condition->AdditionalIDENT[0],1,SIDlen2,infile);
  condition->AdditionalIDENT[SIDlen2] = '\0';
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1+1));
  if(SID1 == NULL)
    {
      printf("2 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC,infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("3 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn1,SID1, gform1,radius1, height1, numverts1, Lindex1, localid, x1, y1, z1);
  condition->ECC1 = ECC;
  
  return;
}




void FreadObject(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1;
  char *SID1;
  char gform1,keepit;
  double localid1, radius1,height1;
  int numverts1, ECC;
  double *x1, *y1, *z1;
  int Lindex1;
  
  if(infile == NULL)
    return;
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("4 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';

  SEEIT_fread_int(&ECC,infile);
  
  fread(&gform1,1,1,infile);

  fread(&keepit,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid1,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("5 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      printf("  request was made to allocate memory for %s object with %d vertices\n",
          ParseGAITgeometry(gform1, 2), numverts1);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn1,SID1, gform1,radius1, height1, numverts1, Lindex1, localid1, x1, y1, z1);
  condition->ECC1 = ECC;
  return;
}




void FreadTwoObjects(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1,idn2,SIDlen2;
  char *SID1, *SID2;
  char gform1, gform2;
  double localid1, localid2;
  double radius1, radius2;
  double height1, height2;
  int numverts1, numverts2;
  int ECC1, ECC2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  int Lindex1, Lindex2;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("6 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC1,infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid1,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("7 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  InstanceConditionObjects(condition,
			   idn1,SID1, gform1,radius1, height1, numverts1, Lindex1, localid1, x1, y1, z1);
  condition->ECC1 = ECC1;
  
  /** second object ***/
  SEEIT_fread_int(&idn2,infile);
  
  SEEIT_fread_int(&SIDlen2,infile);
  
  SID2 = (char *) (malloc(SIDlen2 + 1));
  if(SID2 == NULL)
    {
      printf("8 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID2[0],1,SIDlen2,infile);
  SID2[SIDlen2] = '\0';
  
  SEEIT_fread_int(&ECC2,infile);
  fread(&gform2,1,1,infile);

  SEEIT_fread_int(&Lindex2,infile);

  SEEIT_fread_double(&localid2,infile);
  
  SEEIT_fread_double(&radius2,infile);
  
  SEEIT_fread_double(&height2,infile);
  
  SEEIT_fread_int(&numverts2, infile);
  x2 = (double *) (malloc(SzD * numverts2));
  y2 = (double *) (malloc(SzD * numverts2));
  z2 = (double *) (malloc(SzD * numverts2));
  if(z2 == NULL)
    {
      printf("9 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts2; i++)
    {
      SEEIT_fread_double(&x2[i], infile);
      SEEIT_fread_double(&y2[i], infile);
      SEEIT_fread_double(&z2[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn2,SID2, gform2,radius2, height2, numverts2, Lindex2, localid2, x2, y2, z2);
  condition->ECC2 = ECC2;
  
  return;
}


void FreadObjectAndMessage(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1;
  char *SID1;
  char gform1;
  double localid;
  double radius1;
  double height1;
  int numverts1;
  int ECC;
  double *x1, *y1, *z1;
  char *condmsg;
  int msglen;
  int Lindex1;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("10 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC,infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&msglen,infile);
  condmsg = (char *) (malloc(msglen + 1));
  if(condmsg == NULL)
    {
      printf("11 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  
  fread(&condmsg[0],1,msglen,infile);
  condmsg[msglen] = '\0';
  
  switch(condition->CONDITION_TYPE)
     {
     case ATTR_PAIR: /*** NGA unexpected fcode - geom pair ***/
     case ATTR_UNEXP: /** NGA unexpected attribute assigned ***/
     case RPTD_ATTR: /** attribute error as reported  ****/
     case CONFLATE: /*** line is unique among conflation sets of data ***/
     case ATTR_VVT: /*** attribute dependency violation  **/
     case ATTR_RNULL: /*** MGCP Required attribute assigned NULL value ***/
     case ATTR_MISSING: /** missing a required attribute ***/
     case ATTR_DT:  /** NGA - datatype encountered not as presecribed **/
     case ATTR_RNG: /** NGA attribute value range violation ***/
     case ATTR_PICK: /** NGA - pick list allowed domain violation **/
     case ATTR_META: /** NGA - GIFD D4 metadata violation ***/
     case VVTERR1WAY: /** feature with designated attribute & value ***/
     case VVTERR2WAY:  /*** valid value type error ***/
     case VVTERR3WAY:  /*** valid values conflict between 3 attribute of a single feature ***/
     case ATTRERR: /*** attribution error **/
        condition->AttrErrMsg = condmsg;
        break;
     default:
        condition->Message = condmsg;
        break;
     }
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("12 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn1,SID1, gform1,radius1, height1, numverts1, Lindex1, localid, x1, y1, z1);
  condition->ECC1 = ECC;
  return;
}



double FreadObjectAndMagnitude(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1;
  char *SID1;
  char gform1;
  double localid;
  double radius1;
  double height1;
  int ECC;
  int numverts1;
  double *x1, *y1, *z1;
  double magnitude;
  int Lindex1;
  
  if(infile == NULL)
    return(0.0);
  
  SEEIT_fread_double(&magnitude,infile);
  condition->magnitude = magnitude;
  
  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("13 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC,infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid, infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("14 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  if((keyval == Z_KINK) || (keyval == KINK) || (keyval == INTERNALKINK) || (keyval == ISOTURN) ||
      (keyval == SLOPEDIRCH) || (keyval == CLAMP_SDC) || (keyval == L_A_KINK) ||
        (keyval == CLAMP_JOINSDC))
      {
      condition->pointx = x1[1];
      condition->pointy = y1[1];
      condition->pointz = z1[1];
      condition->AdditionalPointUsed = 1;
      }
  else if((keyval == ELEVLT) && (gform1 != G_POINT) && (radius1 != -1.0) && (height1 != -1.0))
      {
      condition->pointx = radius1;
      condition->pointy = height1;
      condition->pointz = 0;
      condition->AdditionalPointUsed = 1;
      }
  
  InstanceConditionObjects(condition,
			   idn1, SID1, gform1,radius1, height1, numverts1, Lindex1, localid, x1, y1, z1);
  condition->ECC1 = ECC;

  return(magnitude);
}



void FreadDynamicInfo(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i,slen;
  int idn1,SIDlen1,idn2,SIDlen2;
  int MagUsed;
  int PtUsed;
  int NumObjects;
  char *SID1, *SID2;
  char gform1, gform2;
  double localid1, localid2;
  double radius1, radius2;
  double height1, height2;
  double magnitude;
  int ECC1, ECC2;
  int numverts1, numverts2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  int Lindex1, Lindex2;
  char *message;

  if(infile == NULL)
    return;

  SEEIT_fread_int(&PtUsed,infile);
  SEEIT_fread_int(&MagUsed,infile);
  SEEIT_fread_int(&NumObjects,infile);
/**printf("FreadDynamicDef - just had PtUsed %d MagUsed %d NumObj %d\n",PtUsed,MagUsed,NumObjects);**/

  SEEIT_fread_int(&slen,infile);
  if(slen > 0)
    {
    message = (char *) (malloc(slen + 2));
    fread(&message[0],1,slen,infile);
    message[slen] = '\0';
    }
  else
    message = NULL;
  condition->AttrErrMsg = message;

  magnitude = -1;
  if(MagUsed > 0)
     SEEIT_fread_double(&magnitude,infile);
  condition->magnitude = magnitude;

  condition->pointx = condition->pointy = condition->pointz = -1.0;
  condition->AdditionalPointUsed = 0;
  if(PtUsed > 0)
     {
     SEEIT_fread_double(&condition->pointx, infile);
     SEEIT_fread_double(&condition->pointy, infile);
     SEEIT_fread_double(&condition->pointz, infile);
     condition->AdditionalPointUsed = 1;
     }
  condition->AdditionalIDENT = NULL;

  if(NumObjects > 0)
     {
     SEEIT_fread_int(&idn1,infile);

     SEEIT_fread_int(&SIDlen1,infile);

     SID1 = (char *) (malloc(SIDlen1+1));
     if(SID1 == NULL)
        {
        printf("15 System allocation memory has been exhausted during condition read\n");
        printf("   execution cannot continue\n");
        printf("Condition identification number %d\n",keyval);
        exit(1);
        }
     fread(&SID1[0],1,SIDlen1,infile);
     SID1[SIDlen1] = '\0';

     SEEIT_fread_int(&ECC1, infile);
     fread(&gform1,1,1,infile);

     SEEIT_fread_int(&Lindex1, infile);

     SEEIT_fread_double(&localid1,infile);

     SEEIT_fread_double(&radius1,infile);

     SEEIT_fread_double(&height1,infile);

     SEEIT_fread_int(&numverts1, infile);
     x1 = (double *) (malloc(SzD * numverts1));
     y1 = (double *) (malloc(SzD * numverts1));
     z1 = (double *) (malloc(SzD * numverts1));
     if(z1 == NULL)
        {
        printf("16 System allocation memory has been exhausted during condition read\n");
        printf("   execution cannot continue\n");
        printf("Condition identification number %d\n",keyval);
        exit(1);
        }
     for (i=0; i<numverts1; i++)
        {
        SEEIT_fread_double(&x1[i], infile);
        SEEIT_fread_double(&y1[i], infile);
        SEEIT_fread_double(&z1[i], infile);
        }

     InstanceConditionObjects(condition,
                           idn1, SID1, gform1,radius1, height1, numverts1, Lindex1, localid1, x1, y1, z1);
     condition->ECC1 = ECC1;
     }

  /** second object ***/

  if(NumObjects > 1)
     {
     SEEIT_fread_int(&idn2,infile);

     SEEIT_fread_int(&SIDlen2,infile);
   
     SID2 = (char *) (malloc(SIDlen2 + 1));
     if(SID2 == NULL)
        {
        printf("17 System allocation memory has been exhausted during condition read\n");
        printf("   execution cannot continue\n");
        printf("Condition identification number %d\n",keyval);
        exit(1);
        }
     fread(&SID2[0],1,SIDlen2,infile);
     SID2[SIDlen2] = '\0';

     SEEIT_fread_int(&ECC2, infile);
     fread(&gform2,1,1,infile);

     SEEIT_fread_int(&Lindex2, infile);

     SEEIT_fread_double(&localid2,infile);

     SEEIT_fread_double(&radius2,infile);

     SEEIT_fread_double(&height2,infile);
     SEEIT_fread_int(&numverts2, infile);
     x2 = (double *) (malloc(SzD * numverts2));
     y2 = (double *) (malloc(SzD * numverts2));
     z2 = (double *) (malloc(SzD * numverts2));
     if(z2 == NULL)
        {
        printf("18 System allocation memory has been exhausted during condition read\n");
        printf("   execution cannot continue\n");
        printf("Condition identification number %d\n",keyval);
        exit(1);
        }
     for (i=0; i<numverts2; i++)
        {
        SEEIT_fread_double(&x2[i], infile);
        SEEIT_fread_double(&y2[i], infile);
        SEEIT_fread_double(&z2[i], infile);
        }

     InstanceConditionObjects(condition,
                           idn2, SID2, gform2,radius2, height2, numverts2, Lindex2, localid2, x2, y2, z2);
     condition->ECC2 = ECC2;
     }

  return;
}






void FreadMsgMagPointObjects(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i,slen;
  int idn1,SIDlen1,idn2,SIDlen2;
  char *SID1, *SID2;
  char gform1, gform2;
  double localid1, localid2;
  double radius1, radius2;
  double height1, height2;
  double magnitude;
  int ECC1, ECC2;
  int numverts1, numverts2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  int Lindex1, Lindex2;
  char *message;

  if(infile == NULL)
    return;

  SEEIT_fread_int(&slen,infile);
  if(slen > 0)
    {
    message = (char *) (malloc(slen + 2));
    fread(&message[0],1,slen,infile);
    message[slen] = '\0';
    }
  else
    message = NULL;
  condition->AttrErrMsg = message;

  SEEIT_fread_double(&magnitude,infile);
  condition->magnitude = magnitude;

  SEEIT_fread_double(&condition->pointx, infile);
  SEEIT_fread_double(&condition->pointy, infile);
  SEEIT_fread_double(&condition->pointz, infile);
  if((keyval == LGEOM_UNM_LAT) || (keyval == LGEOM_UNM_LON) || (keyval == AGEOM_UNM_LAT) ||
         (keyval == AGEOM_UNM_LON) || (keyval == AUNM_ATTR_A) || (keyval == LUNM_ATTR_A))
     condition->AdditionalPointUsed = 1;
  condition->AdditionalIDENT = NULL;

  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);

  SID1 = (char *) (malloc(SIDlen1+1));
  if(SID1 == NULL)
    {
      printf("15 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';

  SEEIT_fread_int(&ECC1, infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1, infile);

  SEEIT_fread_double(&localid1,infile);

  SEEIT_fread_double(&radius1,infile);

  SEEIT_fread_double(&height1,infile);

  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("16 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }

  InstanceConditionObjects(condition,
                           idn1, SID1, gform1,radius1, height1, numverts1, Lindex1, localid1, x1, y1, z1);
  condition->ECC1 = ECC1;

  /** second object ***/

  SEEIT_fread_int(&idn2,infile);

  SEEIT_fread_int(&SIDlen2,infile);

  SID2 = (char *) (malloc(SIDlen2 + 1));
  if(SID2 == NULL)
    {
      printf("17 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID2[0],1,SIDlen2,infile);
  SID2[SIDlen2] = '\0';

  SEEIT_fread_int(&ECC2, infile);
  fread(&gform2,1,1,infile);

  SEEIT_fread_int(&Lindex2, infile);

  SEEIT_fread_double(&localid2,infile);

  SEEIT_fread_double(&radius2,infile);

  SEEIT_fread_double(&height2,infile);

  SEEIT_fread_int(&numverts2, infile);
  x2 = (double *) (malloc(SzD * numverts2));
  y2 = (double *) (malloc(SzD * numverts2));
  z2 = (double *) (malloc(SzD * numverts2));
  if(z2 == NULL)
    {
      printf("18 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts2; i++)
    {
      SEEIT_fread_double(&x2[i], infile);
      SEEIT_fread_double(&y2[i], infile);
      SEEIT_fread_double(&z2[i], infile);
    }

  InstanceConditionObjects(condition,
                           idn2, SID2, gform2,radius2, height2, numverts2, Lindex2, localid2, x2, y2, z2);
  condition->ECC2 = ECC2;

  return;
}




double FreadPointEdgeAndMagnitude(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1,idn2,SIDlen2;
  char *SID1, *SID2;
  double localid1, localid2;
  char gform1, gform2;
  int ECC1, ECC2;
  int numverts1, numverts2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  double magnitude;
  int Lindex1, Lindex2;
  
  if(infile == NULL)
    return(0.0);
  
   
  SEEIT_fread_double(&magnitude,infile);
  condition->magnitude = magnitude;
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("19 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC1,infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid1,infile);
  
  numverts1 = 1;  /** no need for a read here ***/
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("20 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  
  if((keyval == ENCONNECT) || (keyval == FEATBRIDGE) ||(keyval == BADENCON))
      {
      condition->pointx = x1[0];
      condition->pointy = y1[0];
      condition->pointz = z1[0];
      condition->AdditionalPointUsed = 1;
      free(x1);
      free(y1);
      free(z1);
      free(SID1);
      }
  else
      {
      InstanceConditionObjects(condition,
			   idn1, SID1, gform1,-1.0, -1.0, numverts1, Lindex1, localid1, x1, y1, z1);
      }
  condition->ECC1 = ECC1;
  /** second object ***/
  
  SEEIT_fread_int(&idn2,infile);
  
  SEEIT_fread_int(&SIDlen2,infile);
  
  SID2 = (char *) (malloc(SIDlen2 + 1));
  if(SID2 == NULL)
    {
      printf("21 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID2[0],1,SIDlen2,infile);
  SID2[SIDlen2] = '\0';
  
  SEEIT_fread_int(&ECC2,infile);
  fread(&gform2,1,1,infile);

  SEEIT_fread_int(&Lindex2,infile);

  SEEIT_fread_double(&localid2,infile);
  
  numverts2 = 2;  /** no need for read of this field, always have 2 vertices **/
  x2 = (double *) (malloc(SzD * numverts2));
  y2 = (double *) (malloc(SzD * numverts2));
  z2 = (double *) (malloc(SzD * numverts2));
  if(z2 == NULL)
    {
      printf("22 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts2; i++)
    {
      SEEIT_fread_double(&x2[i], infile);
      SEEIT_fread_double(&y2[i], infile);
      SEEIT_fread_double(&z2[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn2, SID2, gform2,-1.0, -1.0, numverts2, Lindex2, localid2, x2, y2, z2);
  if((keyval != ENCONNECT) && (keyval != FEATBRIDGE) && (keyval != BADENCON))
      condition->ECC2 = ECC2;
  return(magnitude);
}



double FreadPointObjectAndMagnitude(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1,idn2,SIDlen2;
  char *SID1, *SID2;
  char gform1, gform2;
  double localid1,localid2;
  int ECC1, ECC2;
  int numverts1, numverts2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  double magnitude;
  int Lindex1, Lindex2;

  if(infile == NULL)
    return(0.0);


  SEEIT_fread_double(&magnitude,infile);
  condition->magnitude = magnitude;

  SEEIT_fread_int(&idn1,infile);

  SEEIT_fread_int(&SIDlen1,infile);

  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("23 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue (FreadPointObjectAndMagnitude1)\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';

  SEEIT_fread_int(&ECC1,infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1,infile);

  SEEIT_fread_double(&localid1, infile);

  numverts1 = 1;  /** no need for a read here ***/
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("24 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue (FreadPointObjectAndMagnitude2)\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  if((keyval == ENCONNECT) || (keyval == AREAKINK) ||(keyval == INCLSLIVER) || (keyval == FEATBRIDGE) || (keyval == ISOTURN) ||
      (keyval == CLAMP_DIF) || (keyval == ZUNCLOSED) || (keyval == NOT_FLAT) ||(keyval == CLAMP_NFLAT)  || (keyval == AREAUNCLOSED))
      {
      condition->pointx = x1[0];
      condition->pointy = y1[0];
      condition->pointz = z1[0];
      condition->AdditionalPointUsed = 1;
      free(x1);
      free(y1);
      free(z1);
      free(SID1);
      }
  else
      {
      InstanceConditionObjects(condition,
                           idn1, SID1, gform1,-1.0, -1.0, numverts1, Lindex1, localid1, x1, y1, z1);
      }
  condition->ECC1 = ECC1;
  /** second object ***/

  SEEIT_fread_int(&idn2,infile);

  SEEIT_fread_int(&SIDlen2,infile);

  SID2 = (char *) (malloc(SIDlen2 + 1));
  if(SID2 == NULL)
    {
      printf("25 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue (FreadPointObjectAndMagnitude3)\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID2[0],1,SIDlen2,infile);
  SID2[SIDlen2] = '\0';

  SEEIT_fread_int(&ECC2,infile);
  fread(&gform2,1,1,infile);

  SEEIT_fread_int(&Lindex2,infile);

  SEEIT_fread_double(&localid2, infile);

  SEEIT_fread_int(&numverts2,infile);

  x2 = (double *) (malloc(SzD * numverts2));
  y2 = (double *) (malloc(SzD * numverts2));
  z2 = (double *) (malloc(SzD * numverts2));
  if(z2 == NULL)
    {
      printf("26 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue (FreadPointObjectAndMagnitude4)\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts2; i++)
    {
      SEEIT_fread_double(&x2[i], infile);
      SEEIT_fread_double(&y2[i], infile);
      SEEIT_fread_double(&z2[i], infile);
    }

  InstanceConditionObjects(condition,
                           idn2, SID2, gform2,-1.0, -1.0, numverts2, Lindex2, localid2, x2, y2, z2);

  if((keyval != ENCONNECT) && (keyval != AREAKINK) && (keyval != INCLSLIVER) && (keyval != FEATBRIDGE) && (keyval != ISOTURN) &&
     (keyval != CLAMP_DIF) && (keyval != ZUNCLOSED) && (keyval != NOT_FLAT) && (keyval != CLAMP_NFLAT) && (keyval != AREAUNCLOSED))
    condition->ECC2 = ECC2;
  return(magnitude);
}





double FreadMagnitudeAndTwoObjects(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1,idn2,SIDlen2;
  char *SID1, *SID2;
  char gform1, gform2;
  double localid1, localid2;
  double radius1, radius2;
  double height1, height2;
  int ECC1, ECC2;
  int numverts1, numverts2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  double magnitude;
  int Lindex1, Lindex2;
  
  if(infile == NULL)
    return(0.0);
  
  SEEIT_fread_double(&magnitude,infile);
  condition->magnitude = magnitude;
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1 + 1));
  if(SID1 == NULL)
    {
      printf("27 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC1, infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1, infile);

  SEEIT_fread_double(&localid1,infile);

  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("28 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }

  if(keyval == DUPLICATESEG)
     {
     i = (int) radius1;
     condition->pointx = x1[i];
     condition->pointy = y1[i];
     condition->pointz = z1[i];
     condition->AdditionalPointUsed = 1;
     }
   else if(keyval == CONTEXT_KINK)
      {
      condition->pointx = x1[2];
      condition->pointy = y1[2];
      condition->pointz = z1[2];
      condition->AdditionalPointUsed = 1;
      }
   else if((keyval == FAILMERGEL) || (keyval == FAILMERGEL2))
      {
      condition->pointx = x1[0];
      condition->pointy = y1[0];
      condition->pointz = z1[0];
      condition->AdditionalPointUsed = 1;
      }
   else if(keyval == LJOINSLOPEDC)
      {
      condition->pointx = x1[1];
      condition->pointy = y1[1];
      condition->pointz = z1[1];
      condition->AdditionalPointUsed = 1;
      }

  
  InstanceConditionObjects(condition,
			   idn1, SID1, gform1,radius1, height1, numverts1, Lindex1,localid1, x1, y1, z1);
  if((keyval == INTRA_NET) || (keyval == EXTRA_NET))
     condition->circle1.radius = radius1;
  condition->ECC1 = ECC1;
  
  /** second object ***/
  
  SEEIT_fread_int(&idn2,infile);
  
  SEEIT_fread_int(&SIDlen2,infile);
  
  SID2 = (char *) (malloc(SIDlen2 + 1));
  if(SID2 == NULL)
    {
      printf("29 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID2[0],1,SIDlen2,infile);
  SID2[SIDlen2] = '\0';
  
  SEEIT_fread_int(&ECC2, infile);
  fread(&gform2,1,1,infile);

  SEEIT_fread_int(&Lindex2, infile);

  SEEIT_fread_double(&localid2,infile);
  
  SEEIT_fread_double(&radius2,infile);
  
  SEEIT_fread_double(&height2,infile);
  
  SEEIT_fread_int(&numverts2, infile);
  x2 = (double *) (malloc(SzD * numverts2));
  y2 = (double *) (malloc(SzD * numverts2));
  z2 = (double *) (malloc(SzD * numverts2));
  if(z2 == NULL)
    {
      printf("30 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts2; i++)
    {
      SEEIT_fread_double(&x2[i], infile);
      SEEIT_fread_double(&y2[i], infile);
      SEEIT_fread_double(&z2[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn2, SID2, gform2,radius2, height2, numverts2, Lindex2, localid2, x2, y2, z2);
  condition->ECC2 = ECC2;
  if((keyval == INTRA_NET) || (keyval == EXTRA_NET))
     condition->circle2.radius = radius2;
  
  return(magnitude);
}




void FreadPointAndTwoObjects(int keyval, FILE *infile, struct ConditionList * condition)
{
  int i;
  int idn1,SIDlen1,idn2,SIDlen2;
  char *SID1, *SID2;
  char gform1, gform2;
  double localid1, localid2;
  double radius1, radius2;
  double height1, height2;
  int ECC1, ECC2;
  int numverts1, numverts2;
  double *x1, *y1, *z1;
  double *x2, *y2, *z2;
  int Lindex1, Lindex2;
  
  if(infile == NULL)
    return;
  
  SEEIT_fread_double(&condition->pointx, infile);
  SEEIT_fread_double(&condition->pointy, infile);
  SEEIT_fread_double(&condition->pointz, infile);
  condition->AdditionalPointUsed = 1;
  condition->AdditionalIDENT = NULL;
  
  SEEIT_fread_int(&idn1,infile);
  
  SEEIT_fread_int(&SIDlen1,infile);
  
  SID1 = (char *) (malloc(SIDlen1+1));
  if(SID1 == NULL)
    {
      printf("31 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID1[0],1,SIDlen1,infile);
  SID1[SIDlen1] = '\0';
  
  SEEIT_fread_int(&ECC1, infile);
  fread(&gform1,1,1,infile);

  SEEIT_fread_int(&Lindex1, infile);

  SEEIT_fread_double(&localid1,infile);
  
  SEEIT_fread_double(&radius1,infile);
  
  SEEIT_fread_double(&height1,infile);
  
  SEEIT_fread_int(&numverts1, infile);
  x1 = (double *) (malloc(SzD * numverts1));
  y1 = (double *) (malloc(SzD * numverts1));
  z1 = (double *) (malloc(SzD * numverts1));
  if(z1 == NULL)
    {
      printf("32 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts1; i++)
    {
      SEEIT_fread_double(&x1[i], infile);
      SEEIT_fread_double(&y1[i], infile);
      SEEIT_fread_double(&z1[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn1, SID1, gform1,radius1, height1, numverts1, Lindex1, localid1, x1, y1, z1);
  condition->ECC1 = ECC1;
  
  /** second object ***/
  
  SEEIT_fread_int(&idn2,infile);
  
  SEEIT_fread_int(&SIDlen2,infile);
  
  SID2 = (char *) (malloc(SIDlen2 + 1));
  if(SID2 == NULL)
    {
      printf("33 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  fread(&SID2[0],1,SIDlen2,infile);
  SID2[SIDlen2] = '\0';
  
  SEEIT_fread_int(&ECC2, infile);
  fread(&gform2,1,1,infile);

  SEEIT_fread_int(&Lindex2, infile);

  SEEIT_fread_double(&localid2,infile);
  
  SEEIT_fread_double(&radius2,infile);
  
  SEEIT_fread_double(&height2,infile);
  
  SEEIT_fread_int(&numverts2, infile);
  x2 = (double *) (malloc(SzD * numverts2));
  y2 = (double *) (malloc(SzD * numverts2));
  z2 = (double *) (malloc(SzD * numverts2));
  if(z2 == NULL)
    {
      printf("34 System allocation memory has been exhausted during condition read\n");
      printf("   execution cannot continue\n");
      printf("Condition identification number %d\n",keyval);
      exit(1);
    }
  for (i=0; i<numverts2; i++)
    {
      SEEIT_fread_double(&x2[i], infile);
      SEEIT_fread_double(&y2[i], infile);
      SEEIT_fread_double(&z2[i], infile);
    }
  
  InstanceConditionObjects(condition,
			   idn2, SID2, gform2,radius2, height2, numverts2, Lindex2, localid2, x2, y2, z2);
  condition->ECC2 = ECC2;
  return;
}




void drawcircle(double x,double y,double radius,int color)
{
  extern void MAPdrawcircle(double x,double y,double diameter,int color, int Lindex);

  API_DRAW = 1;

  if(color==1)
    {
      MAPdrawcircle(x,y,radius*2,3,DRAW_NOW);
    }
  else
    {
      MAPdrawcircle(x,y,radius*2,color,DRAW_NOW);
    }

  API_DRAW = 0;
}



void drawStar(double x1,double y1,int color)
{
  extern void MAPdrawStar(double x1,double y1,double z1,int color,int Lindex,int type);
  API_DRAW = 1;
  MAPdrawStar(x1,y1,MY2DSENTINEL,color,DRAW_NOW,1);
  API_DRAW = 0;
}

void drawline(double x1,double y1,double x2,double y2, int color)
{
  extern void MAPdrawline(int flag, double x1,double y1,double x2,double y2, int color, int Lindex);
  API_DRAW = 1;
  MAPdrawline(-1,x1,y1,x2,y2,color,DRAW_NOW);
  API_DRAW = 0;
}

void drawfullline(double *x, double *y, int numverts, int color)
{
  extern void MAPdrawfullline(double X[],double Y[],int numnodes,int color, int Lindex);
  API_DRAW = 1;
  MAPdrawfullline(x,y,numverts,color,DRAW_NOW);
  API_DRAW = 0;
}


void drawboldfullline(double *x, double *y, int numverts, int color)
{
  extern void MAPdrawfullline(double X[],double Y[],int numnodes,int color, int Lindex);
  extern void MySetLineThickness(int thickness);

  API_DRAW = 1;
  MySetLineThickness(4);
  MAPdrawfullline(x,y,numverts,2000,DRAW_NOW);
  MySetLineThickness(0);
  API_DRAW = 0;

}



void drawpoly(int thickness,double *x, double *y,int numb, 
              unsigned char color, int convex)
{
  extern void MAPdrawpoly(int num,
			  double X[],double Y[],
			  int color,int convex,int clip,int thickness, int Lindex);
  API_DRAW = 1;
  MAPdrawpoly(numb,x,y,color,convex,1,thickness,DRAW_NOW);
  API_DRAW = 0;
}


void drawboldpoly(double *x, double *y,int numb, int convex)
{
  extern void MAPdrawpoly(int num,
			  double X[],double Y[],
			  int color,int convex,int clip,int thickness, int Lindex);
  extern void MySetLineThickness(int thickness);

  API_DRAW = 1;
  MySetLineThickness(4);
  MAPdrawpoly(numb,x,y,2000,convex,1,1,DRAW_NOW);
  MySetLineThickness(0);
  API_DRAW = 0;
}


void drawX(double x1,double y1,int color)
{
  extern void MAPdrawX(double x1,double y1,int color, int Lindex);
  API_DRAW = 1;
  MAPdrawX(x1,y1,color,DRAW_NOW);
  API_DRAW = 0;
}


void drawBoldX(double x1,double y1,int color)
{
  extern void MAPdrawX(double x1,double y1,int color, int Lindex);
  extern void MySetLineThickness(int thickness);

  API_DRAW = 1;
  MySetLineThickness(4);
  MAPdrawX(x1,y1,2000,DRAW_NOW);
  MySetLineThickness(0);
  API_DRAW = 0;
}





double XCoordinateToDouble(double x)
{
  if(Ctype()==2)
    {
      return (x+Xtranslation);
    }
  
  else if(Ctype()==1)
    {
      return (x/100000.0 + Xtranslation);
    }
  else
    {
      printf("bad coord system detected in XCoordinateToDouble (%d)\n",CoordinateSystem);
      exit(1);
    }
  return -999;
}



int tempis2D(double val)
{
  
  if (val>(MY2DSENTINEL+SENTINEL_SMIDGE))
    {
      return 0;
    }
  
  if (val<(MY2DSENTINEL-SENTINEL_SMIDGE))
    {
      return 0;
    }
  
  return 1;
}

int tempis3D(double val)
{
  return (!tempis2D(val));
}




double YCoordinateToDouble(double y)
{
  if(Ctype()==2)
    {
      return (y+Ytranslation);
    }
  else if(Ctype()==1)
    {
      return (y/100000.0 + Ytranslation);
    }
  else
    {
      printf("bad coord system detected in YCoordinateToDouble (%d)\n",CoordinateSystem);
      exit(1);
    }
  return -999;
}



int GetDegreeSymbol()
{

  if(USE_DOS==1)
    {
      return 176;
    }
  return 32;
}




char * DMS_CoordinateToString(double x,double y,double z, int is3D)
{
  int xD, xM, yM, yD,xneg=0,yneg=0;
  double temp;
  double xS, yS;
  extern int DIGITS_TO_PRINT;
  
  static char MsgBuff[500];
  
  x = x/100000.0 + Xtranslation;
  y = y/100000.0 + Ytranslation;

 

  if(x<0)
  {
	  xneg = 1;
	  x = x * -1.0;
  }
  if(y<0)
  {
	  yneg = 1;
	  y = y * -1.0;
  }

  xD = (int) x;
  temp = x - (double) xD;
  temp = (60.0 * temp);
  
  xM = (int) temp;
  


  xS = (temp - (double) xM) * 60.0;
  

  if(xS > 59.99999)
    {
      xM = xM + 1;
      xS = 0;
    }
  if(xS < 0.000001)
    {
      xS = 0;
    }
  if(xM > 59)
    {
      xD = xD + 1;
      xM = xM - 60;
    }

 
  yD = (int) y;
  temp = y - (double) yD;
  temp = (60.0 * temp);
  
  yM = (int) temp;
  
  yS = (temp - (double) yM) * 60.0;
  
  if(yS > 59.99999)
    {
      yM = yM + 1;
      yS = 0;
    }
  if(yS < 0.000001)
    {
      yS = 0;
    }
  if(yM > 59)
    {
      yD = yD + 1;
      yM = yM - 60;
    }
  
  if(xneg==1)
  {
	  xD = xD * -1;
  }
  if(yneg==1)
  {
	  yD = yD * -1;
  }

  
  if(is3D==1)
    {
      if(tempis2D(z) > 0)
	{
	  sprintf(MsgBuff,"(%d%c%d'%.5lf\", %d%c%d'%.5lf\", (no z coordinate))",
		  xD,GetDegreeSymbol(),xM,xS,
		  yD,GetDegreeSymbol(),yM,yS);
	}
      else
	{
	  sprintf(MsgBuff,"(%d%c%d'%.5lf\", %d%c%d'%.5lf\", %.*lf)",
		  xD,GetDegreeSymbol(),xM,xS,
		  yD,GetDegreeSymbol(),yM,yS,
		  DIGITS_TO_PRINT,z);
	}
    }
  else
    {
      sprintf(MsgBuff,"(%d%c%d'%.5lf\", %d%c%d'%.5lf\")",
	      xD,GetDegreeSymbol(),xM,xS,
	      yD,GetDegreeSymbol(),yM,yS);
    }
  
  
  return MsgBuff;
}



char *GetCoordString(int type, double coord)
{
  int len,i,j,decloc,choplocation,addplus=0,allzero=1;
  char tempstr30[500];
  static char retval[4][50];

  extern Display *mydisplay;
  extern int DIGITS_TO_PRINT;


  sprintf(tempstr30,"%.30lf",coord);

  decloc = -1;
  len = strlen(tempstr30);
  for(i=0;i<len;i++)
  {
	if(tempstr30[i]=='.')
	{
	  decloc = i;
	  break;
	}
  }


  if(decloc<1)
  {
	printf("error: did not find dec in %lf\n",coord);
	sprintf(retval[type-1],"%lf",coord);
	return retval[type-1];
  }

  if(type!=4)
  {
    choplocation = decloc+DIGITS_TO_PRINT+1;
  }
  else
  {
    choplocation = decloc+6+1;
  }


  for(i=choplocation;i<len;i++)
  {
	if(tempstr30[i]!='0')
	{
	  addplus = 1;
	  break;
	}
  }

  if(choplocation>47)
  {
	printf("bad (1) CL %d for type %d value %s\n",choplocation,type,tempstr30);
	choplocation = 15;
	addplus = 0;
    XBell(mydisplay,50);
  }


  if(addplus==1)
  {
	if((type==4)&&(i<len))
	{

	  for(j=0;j<i;j++)
	  {
	    if((tempstr30[j]!='0')&&(tempstr30[j]!='.'))
		{
		  allzero = 0;
		  break;
		} 
	  }

	  if(allzero==1)
	  {

          addplus = 0; /* resetting from 1 */

		  for(j=i+1;j<len;j++)
		  {
	        if(tempstr30[j]!='0')
			{ 
	         addplus = 1;
	         break;
			}
		  }

		  choplocation = i+1;

		  if(choplocation>47)
		  {
			printf("bad (2) CL %d for type %d value %s\n",choplocation,type,tempstr30);
			choplocation = 15;
	        XBell(mydisplay,50);
		  }

		  if(addplus==1)
		  {
            tempstr30[choplocation+1] = '\0';
            tempstr30[choplocation]   = '+';
		  }
		  else
		  {
		    tempstr30[choplocation] = '\0';		 
		  }
	  }
	  else
	  {		 
        tempstr30[choplocation+1] = '\0';
        tempstr30[choplocation]   = '+';
	  }
	}
	else
	{
      tempstr30[choplocation+1] = '\0';
      tempstr30[choplocation]   = '+';
	}
  }
  else if((type==3)||(type==4))
  {
    tempstr30[choplocation] = '\0';
  }
  else
  {
    tempstr30[choplocation+1] = '\0';
    tempstr30[choplocation]   = ' ';
  }


  sprintf(retval[type-1],"%s",tempstr30);

  return retval[type-1];
}


char * Coordinate3DtoString(double x,double y,double z)
{
  static char MsgBuff[300];
  
  if(Ctype()==2)
    {
      if(tempis2D(z) > 0)
	{
	  sprintf(MsgBuff,"(%.4lf, %.4lf, (no z coordinate)",x+Xtranslation,y+Ytranslation);
	}
      else
	{
	  sprintf(MsgBuff,"(%.4lf, %.4lf, %.8lf)",x+Xtranslation,y+Ytranslation,z);
	}
    }
  else if(Ctype()==1)
    {
      if(UseDMScoordinates == 1)
	{
	  sprintf(MsgBuff,"%s",DMS_CoordinateToString(x,y,z,1));
	}      
      else if(tempis2D(z) > 0)
	{
	  sprintf(MsgBuff,"(%s, %s, (no z coordinate))",
		  GetCoordString(1,x/100000.0 + Xtranslation),
		  GetCoordString(2,y/100000.0 + Ytranslation));
	}
      else
	{
	  sprintf(MsgBuff,"(%s, %s, %s)",
		  GetCoordString(1,x/100000.0 + Xtranslation),
		  GetCoordString(2,y/100000.0 + Ytranslation),
		  GetCoordString(3,z));
	}
    }
  else
    {
      printf("bad coord system detected in Coordinate3DtoString (%d)\n",CoordinateSystem);
      exit(1);
    }
  return MsgBuff;
}




char * Coordinate2DtoString(double x,double y)
{
  static char MsgBuff[300];
  
  if(Ctype()==1)
    {
      if(UseDMScoordinates == 1)
	{
	  sprintf(MsgBuff,"%s",DMS_CoordinateToString(x,y,0,0));
	}      
      else
	{
	  sprintf(MsgBuff,"(%.7lf, %.7lf)",x/100000.0 + Xtranslation,
		  y/100000.0 + Ytranslation);
	}
    }
  else if(Ctype()==2)
    {
      sprintf(MsgBuff,"(%.2lf, %.2lf)",x+Xtranslation,y+Ytranslation);
    }
  return MsgBuff;
}




void RestoreOriginalCoordinates(struct ConditionList * ConditionToRestore)
{
  int i;
  int j;
  
  for(i=1; i <= ConditionToRestore->num_areals; i++)
    {
      if(i == 1)
	{
	  for(j=0; j<ConditionToRestore->areal1.numverts; j++)
            {
	      ConditionToRestore->areal1.x[j] = XCoordinateToDouble(ConditionToRestore->areal1.x[j]);
	      ConditionToRestore->areal1.y[j] = YCoordinateToDouble(ConditionToRestore->areal1.y[j]);
            }
	}
      else
	{
	  for(j=0; j<ConditionToRestore->areal2.numverts; j++)
            {
	      ConditionToRestore->areal2.x[j] = XCoordinateToDouble(ConditionToRestore->areal2.x[j]);
	      ConditionToRestore->areal2.y[j] = YCoordinateToDouble(ConditionToRestore->areal2.y[j]);
            }
	}
    }
  
  for(i=1; i <= ConditionToRestore->num_polys; i++)
    {
      if(i == 1)
	{
	  for(j=0; j<ConditionToRestore->poly1.numverts; j++)
            {
	      ConditionToRestore->poly1.x[j] = XCoordinateToDouble(ConditionToRestore->poly1.x[j]);
	      ConditionToRestore->poly1.y[j] = YCoordinateToDouble(ConditionToRestore->poly1.y[j]);
            }
	}
      else
	{
	  for(j=0; j<ConditionToRestore->poly2.numverts; j++)
            {
	      ConditionToRestore->poly2.x[j] = XCoordinateToDouble(ConditionToRestore->poly2.x[j]);
	      ConditionToRestore->poly2.y[j] = YCoordinateToDouble(ConditionToRestore->poly2.y[j]);
            }
	}
    }
  
  for(i=1; i <= ConditionToRestore->num_lines; i++)
    {
      if(i == 1)
	{
	  for(j=0; j<ConditionToRestore->linear1.numverts; j++)
            {
	      ConditionToRestore->linear1.x[j] = XCoordinateToDouble(ConditionToRestore->linear1.x[j]);
	      ConditionToRestore->linear1.y[j] = YCoordinateToDouble(ConditionToRestore->linear1.y[j]);
            }
	}
      else
	{
	  for(j=0; j<ConditionToRestore->linear2.numverts; j++)
            {
	      ConditionToRestore->linear2.x[j] = XCoordinateToDouble(ConditionToRestore->linear2.x[j]);
	      ConditionToRestore->linear2.y[j] = YCoordinateToDouble(ConditionToRestore->linear2.y[j]);
            }
	}
    }
  
  for(i=1; i <= ConditionToRestore->num_points; i++)
    {
      if(i == 1)
	{
	  ConditionToRestore->point1.x = XCoordinateToDouble(ConditionToRestore->point1.x);
	  ConditionToRestore->point1.y = YCoordinateToDouble(ConditionToRestore->point1.y);
	}
      else
	{
	  ConditionToRestore->point2.x = XCoordinateToDouble(ConditionToRestore->point2.x);
	  ConditionToRestore->point2.y = YCoordinateToDouble(ConditionToRestore->point2.y);
	}
    }
  
  
  for(i=1; i <= ConditionToRestore->num_circles; i++)
    {
      if(i == 1)
	{
	  ConditionToRestore->circle1.x = XCoordinateToDouble(ConditionToRestore->circle1.x);
	  ConditionToRestore->circle1.y = YCoordinateToDouble(ConditionToRestore->circle1.y);
	}
      else
	{
	  ConditionToRestore->circle2.x = XCoordinateToDouble(ConditionToRestore->circle2.x);
	  ConditionToRestore->circle2.y = YCoordinateToDouble(ConditionToRestore->circle2.y);
	}
    }

  for(i=1; i <= ConditionToRestore->num_sphere; i++)
    {
      if(i == 1)
	{
	  ConditionToRestore->sphere1.x = XCoordinateToDouble(ConditionToRestore->sphere1.x);
	  ConditionToRestore->sphere1.y = YCoordinateToDouble(ConditionToRestore->sphere1.y);
	}
      else
	{
	  ConditionToRestore->sphere2.x = XCoordinateToDouble(ConditionToRestore->sphere2.x);
	  ConditionToRestore->sphere2.y = YCoordinateToDouble(ConditionToRestore->sphere2.y);
	}
    }
  
  for(i=1; i <= ConditionToRestore->num_gridpt; i++)
    {
      if(i == 1)
	{
	  for(j=0; j < ConditionToRestore->gridobj1.numverts; j++)
            {
	      ConditionToRestore->gridobj1.x[j] = XCoordinateToDouble(ConditionToRestore->gridobj1.x[j]);
	      ConditionToRestore->gridobj1.y[j] = YCoordinateToDouble(ConditionToRestore->gridobj1.y[j]);
            }
	}
      else
	{
	  for(j=0; j < ConditionToRestore->gridobj2.numverts; j++)
            {
	      ConditionToRestore->gridobj2.x[j] = XCoordinateToDouble(ConditionToRestore->gridobj2.x[j]);
	      ConditionToRestore->gridobj2.y[j] = YCoordinateToDouble(ConditionToRestore->gridobj2.y[j]);
            }
	}
    }
  
  if(ConditionToRestore->AdditionalPointUsed == 1)
    {
      ConditionToRestore->pointx = XCoordinateToDouble(ConditionToRestore->pointx);
      ConditionToRestore->pointy = YCoordinateToDouble(ConditionToRestore->pointy);
    }
  
  return;
}





void SEEIT_AddAdditionalFlag(struct AdditionalConditions **AdditionalConditionFlags,
			int condnum,int addnum)
{

  struct AdditionalConditions *temp;

  temp = (struct AdditionalConditions *)malloc(sizeof(struct AdditionalConditions));
  if(temp==NULL)
    {
      printf("Malloc Additional: out of memory!\n");
      exit(1);
    }

  temp->condition_number = condnum;
  temp->additional_instance_number = addnum;
  temp->next = *AdditionalConditionFlags;
  
  *AdditionalConditionFlags = temp;

}
	

		
void SEEIT_FreeAdditionalFlags(struct AdditionalConditions *AdditionalConditionFlags)
{
  struct AdditionalConditions *temp,*prev;


  temp = AdditionalConditionFlags;
  while(temp!=NULL)
    {
      prev = temp;
      temp=temp->next;
      free(prev);
    }
  AdditionalConditionFlags = NULL;
}





char *FastStrCat(char *dest, char *src)
{
  while(*dest)dest++;
  while((*dest++ = *src++));
  return --dest;
}





char *MakeIDString(char *gfid, int fid, int Lindex)
{
  char temp[100];
  int lodnum,thisfid;
  static char answer[5000];
  extern int SACfull,NumLODbands,LLFidTableCount;
  extern struct CrossWalk *CrsWlk;
  extern struct LODdisplay *LODindex;
  extern char *GetFrontPart();
  extern char *GetFrontPart2();
  extern int GetParticularFID(int num);
  extern int    HAS_ID[NUM_ATTR_SCHEMA+5];
  extern int ATTRIBUTION_TYPE;
  extern int GetLongAttrVal(int type,int uniqueid,int *Ival,double *Dval,char *Cval);

  sprintf(answer,"\n");

  if(HAS_ID[ATTRIBUTION_TYPE]==1)
    {
      if(gfid!=NULL)
	{
        if(strcmp(gfid,"Error") != 0)
            {
	    strcat(answer,"     ");
	    strcat(answer,GetFrontPart());
	    strcat(answer,": ");	
	    strcat(answer,gfid);
	    strcat(answer,"\n");
            }
	}
    }

  if((Lindex>=0)&&(Lindex<SACfull))
  {
    lodnum = CrsWlk[Lindex].LODband - 1;
	if((lodnum>=0)&&(lodnum<NumLODbands))
	{
      strcat(answer,"     ");
	  strcat(answer,"Layer Identifier: ");
	  strcat(answer,LODindex[lodnum].filename);
      strcat(answer,"\n");
	}
  }

  if((fid>=0)&&(fid<LLFidTableCount))
  {
	  thisfid = GetParticularFID(fid);
      if(thisfid>=0)
	  {
	   sprintf(temp,"%d",thisfid);
       strcat(answer,"     ");
	   strcat(answer,"FID/OBJECTID/Objid: ");
	   strcat(answer,temp);
       strcat(answer,"\n");
	  }
  }

  return answer;
}




int GetRevisedcloneindex(int CONDITIONTYPE, int instance)
{
  int i, num_found=0;
  
  for(i=0;i<TotalConditions;i++)
    {
      if((CONDITIONTYPE==ConditionLookup[i].conditiontype)&&(num_found==instance))
	{
	  return i;
	}
      else if(CONDITIONTYPE==ConditionLookup[i].conditiontype)
	{
	  num_found = num_found + 1;
	}
    }
  printf("SEEIT_API: error!  could not locate condition type %d in the list!\n",CONDITIONTYPE);
  return 0;
}




void SEEIT_PrintConditionByComponents(struct ConditionList * ConditionToDescribe, int begin,int end)
{
  char msgline[5000];
  int i,objectsinvolved = 0;
  double RadToDegreeFactor = 57.29578;
  int con_index;
  char *MessageString;
  char *p;
  int BIGNUM   = 5000;
  int SMALLNUM = 150;
  
  
  
  if(ConditionToDescribe->Message!=NULL)
    {
      free(ConditionToDescribe->Message);
    }
  
  
 
  

  con_index = GetRevisedcloneindex(
				   ConditionToDescribe->CONDITION_TYPE,
				   ConditionToDescribe->CONDITION_INSTANCE);  



 if(ConditionToDescribe->CONDITION_TYPE==COVERFAIL)
  {
    MessageString = (char *)malloc(BIGNUM);

    sprintf(MessageString,"%s\n  ",ParseErrType(COVERFAIL));

    if(ConditionLookup[con_index].Annotation!=NULL)
	{
	  strcat(MessageString,"\"");
	  strcat(MessageString,ConditionLookup[con_index].Annotation);
	  strcat(MessageString,"\"\n  ");
	}

	if(begin==end)
	{
	  sprintf(msgline,"Condition #%d",begin);
      strcat(MessageString,msgline);
	}

    ConditionToDescribe->Message    = (char *) (malloc(strlen(MessageString)+10));
    sprintf(ConditionToDescribe->Message,"%s",MessageString);
  
    free(MessageString);
    return;
 }






  
  MessageString = (char *)malloc(BIGNUM);
  sprintf(MessageString,"%s\n",ConditionLookup[con_index].name);
  
  if(ConditionLookup[con_index].Annotation!=NULL)
  {
	strcat(MessageString,"Annotation: \"");
	strcat(MessageString,ConditionLookup[con_index].Annotation);
	strcat(MessageString,"\"\n\n  ");
  }

  
  
  
  
  
  switch(ConditionToDescribe->CONDITION_TYPE)
    {
    case G_DUPS: /** duplicate poly (by x,y,z of vertices) **/
    case C_DUPS: /** complete duplicate, attribution and all **/
    case SAMEID: /** same GFID or FLDBID, diff geom **/
    case SAMEID_GDUP:  /** same GFID or FLDBID, same geom, diff attr ***/
    case SAMEID_CDUP: /** same GFID or FLDBID, geom & attr ***/
    case ANY_SAMEID:  /** same unique identifier, except those that are complete duplicates ***/
      objectsinvolved = 2;
      
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      break;
      
    case POLYINTPOLY: /** two polygons, of selected types, intersect **/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      break;
      
      
    case POLYINAREA: /** polygon lies wholly inside an areal **/
    case PTINREGION:  /** point feature inside a typed polygon or areal **/
    case PTINPROPER:  /** point feature inside an area feature - not within tolerance of edge (or edge or hole) **/
    case ACOVERA: /* area covered by area ***/
    case FAILMERGEA:  /** area feature that should be merged with area that shares edge ***/
    case FAILMERGEA2:  /** area feature that should be merged with area that shares edge - no accounting for metadata  ***/
    case AINSIDEHOLE: /** area inside another areal's cutout ('illegal holes') ***/
    case FEATNOTCUT:  /*** feature not cut at end node of second feature ***/
    case ISOLINE:  /** line feature completely inside an area feature ***/
    case LINSIDEA: /** line partly or entirely inside area feature ***/
    case LSEGCOVERA: /** line segment overlaps an area feature perimeter ***/
    case LRNGE_UNM_LAT:
    case LRNGE_UNM_LON:
      objectsinvolved = 2;
      break;
      
    case AOVERLAPA: /** overlapping area features (second can also be inside first) **/
      objectsinvolved = 2;
      if(ConditionToDescribe->areal1.numverts == 0) /** one area contains the other completly ***/
	{
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"\n Second area feature completely contained inside first area feature\n");
          strcat(MessageString,msgline);
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"  (use information mode point click to draw containing area feature)\n\n");
          strcat(MessageString,msgline);
	}
      else
	{
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"\n Area features have intersecting perimeters\n");
          strcat(MessageString,msgline);
	}
      break;
      
    case LLINT: /** line - line intersection **/
    case BADFEATCUT: /** feature cut when no need ***/
    case LLIEX: /** line - line except intersection **/
    case LAIEX: /** line - area intersection with 3rd feature exception ***/
    case LMINT: /** line - model intersection **/
    case LAINT:  /* line - areal intersection **/
    case LAINTNOEND: /** line - area intersection not at line end node ***/
    case LACUTFAIL:  /** line not cut at intersection with area perimeter **/
    case LEAON_NOTIN: /** line end node on area edge, line not inside area ***/
    case POLYINTAREA: /* polygon - areal intersection of edges **/
    case AREAINTAREA: /* areal objects intersect */
    case PART_ISF: /** two area features have intersecting edges and share part of their faces **/
    case CUT_INT: /** cut-out intersects parent feature outer ring ***/
    case LLNONODEINT: /* features intersect, but not at a shared node **/
    case NONODEOVLP: /** line, area have overlapping edge without common node ***/
    case LLNOENDINT: /** lines intersect, but not at end point **/
    case LLINTAWAY: /** two lines intersect, and cross over each other ***/
    case LLINTNOEND: /** two lines intersect, pt of intersection is away from either primary particpant end node ***/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline," intersection location:\n  %s\n",
              Coordinate2DtoString(ConditionToDescribe->pointx,ConditionToDescribe->pointy));
      strcat(MessageString,msgline);
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      break;

    case LGEOM_UNM_LAT:
    case LGEOM_UNM_LON:
    case AGEOM_UNM_LAT:
    case AGEOM_UNM_LON:
    case AUNM_ATTR_A:
    case LUNM_ATTR_A:
      objectsinvolved = 2;
      if(ConditionToDescribe->AttrErrMsg != NULL)
         {
         MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
         sprintf(msgline," %s\n",ConditionToDescribe->AttrErrMsg);
         strcat(MessageString,msgline);
         MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
         }
      break;

   case MASKCONSTANT:
   case MASKZERO: /** DEM not zero elev at point defined by specified mask value ***/
   case MASKEDIT_0:
   case MASKEDIT_1: /** EDM has primary tolerance value, diff between TDR and TDF is > secondary tolerance **/
   case MASKCONF2: /** variation of Grids with conflicting values **/
   case MASKCONFLICT: /** Grid DEM Masks have conflicting values ***/
   case PT_GRID_DIF: /** point and grid z value mismatch at exact coord, no interpolation **/

   case RAISEDPC: /** number of raised shoreline points exceeds tolerance **/
   case FLOWSTEP:  /** step size in river flow above threshold ***/
   case BREAKLINE: /** river elevation change at bad angle with shorelines ***/
   case WATERMMU: /** minimum mapping unit for water body below threshold ***/
   case AVGSPIKE: /** spike / well as compared to average elevation of neighbor posts ***/
   case GSHELF:  /** looking for shelf formations like PUE in DEM ***/
   case AWITHOUTA:  /** area that does not fully contain a second area ***/
   case LOSMINHGT:
   case ELEVGT:
   case ELEVLT: 
   case ELEVEQ:  
   case ELEVEQOPEN:
   case FEATOUTSIDE:  /*** a feature lies at least partly outside the MGCP cell ***/
      objectsinvolved = ConditionToDescribe->num_areals + ConditionToDescribe->num_lines + ConditionToDescribe->num_points +
                               ConditionToDescribe->num_gridpt;
      if(ConditionToDescribe->AttrErrMsg != NULL)
         {
         MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
         sprintf(msgline," %s\n",ConditionToDescribe->AttrErrMsg);
         strcat(MessageString,msgline);
         MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
         }
      break;


      
    case FAILMERGEL:  /** line object that should be merged with connecting line ***/
    case FAILMERGEL2:  /** line object that should be merged with connecting line no accounting for metadata  ***/

      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline," line features share end node at:\n  %s\n",
	      Coordinate2DtoString(ConditionToDescribe->pointx,ConditionToDescribe->pointy));
      strcat(MessageString,msgline);
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      break;
      
    case VERTSLOPE: /** vertical poly **/
    case PLPFAIL: /** point oline coincidence failure **/
    case PNOCOVERLE:  /** point not covered by linear end node ***/
    case PNOCOV2LEA: /** point not covered by 2 line terminal nodes or area edges***/
    case PNOCOVERLV:  /** point not covered by any line vertex  **/
    case POLYOSIDEAREA: /** Poly completely outside all areals of given type **/
    case PTOSIDEREGION: /** point feature not inside any typed areal or poly **/
    case OBJECTWITHOUT: /** poly or areal without a point or linear inside **/
    case OBJ_WO_TWO: /** area contains secondary P,A,L but not tertiary P,A,L ***/
    case FSFAIL: /*** face sharing failure ***/
    case PSHAREFAIL:  /*** an area feature fails to share any of its perimeter with a 2d area feature ***/
    case NOCOINCIDE: /** area without line end node or segment on its perimeter ***/
    case V_DUPS: /** duplicate vertices inside object **/
    case LNOCOVERLA: /** line not covered by linear or areal ***/
    case LSPANFAIL: /** line not covered by face of doesnt span between edges ***/
    case LNOCOV2A:  /** line not covered by edges of 2 area features ***/
    case LEINSIDEA: /** line end node properly inside an area ***/
    case COINCIDEFAIL: /** line or area feature segment fails to coincide with 2 other line or area features **/
    case ISOLATEDA:  /*** area feature does not instersect another area or a line feature ***/
    case NETISOA: /** like ISOLATEDA except allowed a transitive connection through other like features ***/
    case ANETISOA: /** area not trans connected to another area by shared edges ***/
    case NETISOFEAT: /** form a network - check for nets with one feature, but not another ***/
    case MULTIDFEAT: /** single line or area with both 2 and 3 D coordinates ***/
    case MULTISENTINEL: /** single line or area has more than one sentinel z value ***/
    case CONNECTFAIL: /** point, line, or area feature without 'connection' to specified 2nd feature **/
    case HIGHLIGHTED: /** feature is on the highlight list from view by attribution ***/
    case LENOCOVERP: /** linear end node not covered by point feature ***/
    case ENCONFAIL: /** end node connectivity failure **/
    case NOENDCON: /** both end nodes of a line fail to connect or be covered **/
    case BOTHENDCON: /** both end nodes of a line feature are covered by specified-type point features **/
    case LENOCOVERL:  /*** line end node not within tolerance distance to another line ***/
    case NOLCOVLE: /**line end node not within tolerance distance to another line, including itself ***/
    case ANOCOVERLA:  /** areal not covered by linear or areal ***/
    case QUALANOCOVLA: /** area permin not covered by line or area AND is inside a third area ***/
    case ANOCOVERA: /** area not covered by second area feature **/
    case OVERUNDER: /** any feature outside a perimeter-defining area or a line end node undershooting it **/
    case AMCOVAFAIL: /** area not coverer by adjoining areas **/
    case CUTOUT:   /** simply identifies a cut-out of an area feature ***/
    case PORTRAYF: /** write feature that fails all MGCP4 portrayal rules ***/
    case TPORTRAYF: /** write feature that fails all TDS6 portrayal rules ***/
    case MASKMONO: /** DEM not monotonic at point defined by specified mask value ***/
    case LLNOINT:  /*** line line intersection failure ***/
    case LFNOINT: /** line fails to intersect another line, area, or point and no end node on 1/4 degree line ***/
    case PLLPROXFAIL: /*** point line intersection coincidence failure ***/
    case P_O_LOOP: /*** self-intersecting line that includes P & O formations using end nodes - lines only ****/
    case AHANG_LON: /** hanging area feature at a specified longitude meridian ***/
    case AHANG_LAT: /** hanging area feature at a specified latitude parallel ***/
    case AUNM_ACRS_A: /** area feature edge incorrectly matched across a bounding area feature ***/
      objectsinvolved = 1;
      break;
      
    case ENDPTINT: /** line endpoints are the same ***/
    case L_UNM_A:  /*** line endpt unmatched at area feature boundary ***/
    case LSAME_UNM_A: /*** line endpt unmatched with line of same FCODE at Area boundary ***/
    case LUNM_ACRS_A: /*** line mismatch across poly edge ***/
    case LUNMA_ACRS_A: /** line end not matched to area node across area perimeter ***/
    case LHANG_LON: /** hanging line feature at a specified longitude meridian ***/
    case LHANG_LAT: /** hanging line feature at a specified latitude parallel ***/
    case LE_A_UNM_LON: /** line end node not coincident with area node at longitude meridian **/
    case LE_A_UNM_LAT: /** line end node not coincident with area node at latitude parallel **/
    case LOOPS: /*** self-intersecting linear or areal ***/
    case COLINEAR: /** 3 consecutive vertices on line or area perim are collinear - middle one is not connecting node ***/
    case KICKBACK:  /** 180 degree kink ***/
    case LATTRCHNG:  /** line end point connects to same fdcode line, but attributes differ between the 2 features **/
      objectsinvolved = 1; /** fully describe this condition here **/
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      if(ConditionToDescribe->CONDITION_TYPE == KICKBACK)
        {
          sprintf(msgline,"   Kickback occurs vicinity: %s\n",
                  Coordinate2DtoString(ConditionToDescribe->pointx,
                                       ConditionToDescribe->pointy));
        }
      else if(ConditionToDescribe->CONDITION_TYPE == COLINEAR)
        {
          sprintf(msgline,"   Collinear vertex coordinate: %s\n",
                  Coordinate2DtoString(ConditionToDescribe->pointx,
                                       ConditionToDescribe->pointy));
        }
      else
        {
	  sprintf(msgline,"\n Condition location:\n   %s\n",
		  Coordinate2DtoString(ConditionToDescribe->pointx,
				       ConditionToDescribe->pointy));
        }
      strcat(MessageString,msgline);
      break;
      
    case ENCONNECT: /** linear end node connectivity **/
    case BADENCON: /** bad sequence on line feature connections ***/
      objectsinvolved = 1;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %d %s\n",
	      (int) ConditionToDescribe->magnitude,
	      ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      break;

    case FEATBRIDGE:
      objectsinvolved = 1;

      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.3lf %s\n",
              ConditionToDescribe->magnitude,
              ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      break;
      
    case LOUTSIDEA: /** linear vertex falls outtside areal **/
    case LLAINT: /** line - line endpt connect at area perimeter **/
    case L_NOTL_AINT: /** line end point connects to 'not type line' at area perimeter **/
    case TVERT: /** 'T' vertex **/
      objectsinvolved = 0; /** fully describe this condition here **/
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"\n Condition found at location:\n   %s\n",
	      Coordinate2DtoString(ConditionToDescribe->pointx,
				   ConditionToDescribe->pointy));
      strcat(MessageString,msgline);
      
      
      if(ConditionToDescribe->num_polys > 0)
	{
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," associated with Polygon:  %s\n\n",
		      MakeIDString(
			  ConditionToDescribe->poly1.IDENT,
			  ConditionToDescribe->poly1.objectid,
			  ConditionToDescribe->poly1.Lindex)
			  );
	      strcat(MessageString,msgline);   
	}
      else if(ConditionToDescribe->num_areals > 0)
	{
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," associated with Area Feature:  %s\n\n",
		      MakeIDString(
			  ConditionToDescribe->areal1.IDENT,
			  ConditionToDescribe->areal1.objectid,
			  ConditionToDescribe->areal1.Lindex)
			  );
	      strcat(MessageString,msgline);
	    
	}
      else if(ConditionToDescribe->num_lines > 0)
	{
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," associated with Line Feature:  %s\n\n",
		      MakeIDString(
			  ConditionToDescribe->linear1.IDENT,
			  ConditionToDescribe->linear1.objectid,
			  ConditionToDescribe->linear1.Lindex)
			  );
	      strcat(MessageString,msgline);   
	}

      else if(ConditionToDescribe->num_points > 0)
        {
              MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
              sprintf(msgline," Point Feature:  %s\n",
                      MakeIDString(
                          ConditionToDescribe->point1.IDENT,
                          ConditionToDescribe->point1.objectid,
                          ConditionToDescribe->point1.Lindex)
                          );
              strcat(MessageString,msgline);

          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline," Point Feature vertex:\n");
          strcat(MessageString,msgline);

          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   %s\n",
                  Coordinate3DtoString(ConditionToDescribe->point1.x,
                                       ConditionToDescribe->point1.y,ConditionToDescribe->point1.z));
          strcat(MessageString,msgline);
        }
      break;
      
    case ATTR_PAIR: /*** NGA unexpected fcode - geom pair ***/
    case ATTR_UNEXP: /** NGA unexpected attribute assigned ***/
    case RPTD_ATTR: /** attribute error as reported  ****/
    case CONFLATE: /*** line is unique among conflation sets of data ***/
    case ATTR_VVT: /*** attribute dependency violation  **/
    case ATTR_RNULL: /*** MGCP Required attribute assigned NULL value ***/
    case ATTR_MISSING: /** missing a required attribute ***/
    case ATTR_DT:  /** NGA - datatype encountered not as presecribed **/
    case ATTR_RNG: /** NGA attribute value range violation ***/
    case ATTR_PICK: /** NGA - pick list allowed domain violation **/
    case ATTR_META: /** NGA - GIFD D4 metadata violation ***/
    case VVTERR1WAY: /** feature with designated attribute & value ***/
    case VVTERR2WAY:  /** valid value table type error ***/
    case VVTERR3WAY:  /*** valid values conflict between 3 attribute of a single feature ***/
    case ATTRERR: /*** attribution error **/
      objectsinvolved = 1;
      
      if(ConditionToDescribe->CONDITION_TYPE == RPTD_ATTR)
         {
         sprintf(MessageString,"Condition Description: %s\n",ConditionToDescribe->AttrErrMsg);
         }
      else if(ConditionToDescribe->CONDITION_TYPE == CONFLATE)
         {
         sprintf(MessageString,"Source File: %s\n",ConditionToDescribe->AttrErrMsg);
         }
      else if(ConditionToDescribe->AttrErrMsg != NULL)
         {
         strcat(MessageString,"Condition Description: ");
         strcat(MessageString,ConditionToDescribe->AttrErrMsg);
         strcat(MessageString,"\n");
         }
      
      if(ConditionToDescribe->CONDITION_TYPE == ATTRERR)
	{
	  strcat(MessageString,"\n related Object:\n");
	}
      
      break;
      
      
    case ISOTURN: /** high turn angle w/o 3d feature present ***/
    case KINK:  /** high angle between adjacent linear segments **/
    case Z_KINK: /** consecutive kinks form a 'Z' ***/
    case L_A_KINK: /** kink at intersection of line end node  and area feature perim **/
    case INTERNALKINK: /** kink internal to single line feature **/
    case SLIVER: /** sliver triangle **/
    case FACESIZE: /*** small area on face of area feature **/
    case ARNGE_UNM_LAT:
    case ARNGE_UNM_LON:
    case NARROW:  /** narrow triangle **/
    case SMALLOBJ:  /** small 3d area poly **/
    case HSLOPE: /** high slope poly **/
    case HTEAR: /** horizontal tear **/
    case OVERC: /** over-lapping edges **/
    case GSPIKE:  /** grid data spike point ***/
    case GRID_STD_DEV: /** grid elev value, inside feature polygon, over range offset from std deviation **/
    case ELEVADJCHANGE:  /** change in adjacent node elevations > threshold ***/
    case FEATSPIKE: /** elevation spike along 3D feature ***/
    case SEGLEN: /** linear feature segment length below threshold ***/
    case LONGSEG: /** linear or areal perimeter segment with length at or above threshold ***/
    case BIGAREA: /** area feat in excess of set square area **/
    case SMALLAREA:  /** area feat with small square area **/
    case SMLCUTOUT: /** small included area inner ring of area feature ***/
    case OSIDE_LAT:   /**** feature coordinate above or below latitude range    **/
    case OSIDE_LON:   /**** feature coordinate above or below longitude range    **/
    case BNDRYUNDERSHT: /** feature undershoots whole degree project outside boundary ***/
    case LBNDUSHT:  /** unconnected line end node undershoots whole-degree boundary ***/
    case PERIMLEN: /** areal perimeter or ttl line len below threshold ***/
    case SHORTFEAT:  /** short length line feature not on quarter degree 'boundary' ***/
    case LONGFEAT:   /** line or area feature with total length above threshold ***/
    case SHARE3SEG: /** line feature segment overlaps 2 other line or area feature segments ***/
    case PC_SLOPE: /*** line feature segment with percent slope above tolerance ****/
    case CALC_AREA:  /*** point feature with LEN and WID attr values product < tolerance ***/
    case AREAKINK: /** high angle kink in perimeter of areal **/
    case ZUNCLOSED: /** area feat not closed in Z **/
    case AREAUNCLOSED: /** area feature unclosed in any dimension **/
    case NOT_FLAT:  /*** area feature with surface that is not uiform elevation ***/
    case CLAMP_NFLAT: /** area feature does not have constant elevation when clamped to underlying DEM ***/
    case CLAMP_DIF: /** difference between feature vertex z value and interpolated DEM value ***/
    case INCLSLIVER: /** areal with included sliver **/
      objectsinvolved = 1;
      
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.6lf %s\n",
	      ConditionToDescribe->magnitude,
	      ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      
      if(strcmp(ConditionLookup[con_index].units,"radians") == 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   (Magnitude in degrees: %.2lf)\n",
		  ConditionToDescribe->magnitude * RadToDegreeFactor);
	  strcat(MessageString,msgline);
	}
      
      if(ConditionToDescribe->CONDITION_TYPE == KINK)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Angle measured at %s\n",
		  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
				       ConditionToDescribe->linear1.y[1]));
	  strcat(MessageString,msgline);
	}
      else if((ConditionToDescribe->CONDITION_TYPE == ZUNCLOSED) || 
	      (ConditionToDescribe->CONDITION_TYPE == AREAUNCLOSED))
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   First vertex: %s\n",
		  Coordinate3DtoString(ConditionToDescribe->pointx,
				       ConditionToDescribe->pointy,ConditionToDescribe->pointz));
          strcat(MessageString,msgline);
          sprintf(msgline,"   Last vertex: %s\n",
		  Coordinate3DtoString(ConditionToDescribe->areal1.x[0],
				       ConditionToDescribe->areal1.y[0],ConditionToDescribe->areal1.z[0]));
          strcat(MessageString,msgline);
        }
      
      else if(ConditionToDescribe->CONDITION_TYPE == NOT_FLAT)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Minimum Elevation Value: %s\n",
		  Coordinate3DtoString(ConditionToDescribe->pointx,
				       ConditionToDescribe->pointy,ConditionToDescribe->pointz));
          strcat(MessageString,msgline);
          sprintf(msgline,"   Maximum Elevation Value: %s\n",
		  Coordinate3DtoString(ConditionToDescribe->areal1.x[0],
				       ConditionToDescribe->areal1.y[0],ConditionToDescribe->areal1.z[0]));
          strcat(MessageString,msgline);
        }

      else if(ConditionToDescribe->CONDITION_TYPE == CLAMP_NFLAT)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Minimum Clamped Elevation Value: %s\n",
                  Coordinate3DtoString(ConditionToDescribe->pointx,
                                       ConditionToDescribe->pointy,ConditionToDescribe->pointz));
          strcat(MessageString,msgline);
          sprintf(msgline,"   Maximum Clamped Elevation Value: %s\n",
                  Coordinate3DtoString(ConditionToDescribe->areal1.x[0],
                                       ConditionToDescribe->areal1.y[0],ConditionToDescribe->areal1.z[0]));
          strcat(MessageString,msgline);
        }
      
      else if((ConditionToDescribe->CONDITION_TYPE == Z_KINK) || (ConditionToDescribe->CONDITION_TYPE == CONTEXT_KINK))
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angles measured at %s",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
                                       ConditionToDescribe->linear1.y[1]));
          strcat(MessageString,msgline);
          sprintf(msgline," and %s\n",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[2],
                                       ConditionToDescribe->linear1.y[2]));
          strcat(MessageString,msgline);
        }
      
      else if(ConditionToDescribe->CONDITION_TYPE == L_A_KINK)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angle measured at %s\n",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
                                       ConditionToDescribe->linear1.y[1]));
          strcat(MessageString,msgline);
        }
      
      else if(ConditionToDescribe->CONDITION_TYPE == INTERNALKINK)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angle measured at %s\n",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
                                       ConditionToDescribe->linear1.y[1]));
          strcat(MessageString,msgline);
        }
      
      else if(ConditionToDescribe->CONDITION_TYPE == AREAKINK)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Angle measured at %s\n",
		  Coordinate2DtoString(ConditionToDescribe->pointx,
				       ConditionToDescribe->pointy));
	  strcat(MessageString,msgline);
	}
      else if(ConditionToDescribe->CONDITION_TYPE == INCLSLIVER)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Sliver occurs vicinity: %s\n",
		  Coordinate2DtoString(ConditionToDescribe->pointx,
				       ConditionToDescribe->pointy));
	  strcat(MessageString,msgline);
	}
      break;
      
    case COVERFAIL: /** to detect holes in surface; MGCP landcover requirement ***/
      objectsinvolved = 1;
      break;
      
    case MULTIPARTL:  /** multi-part line ***/
    case MULTIPARTA: /** multi-part area **/
    case MULTIPARTP: /** multi-part point **/
      
      objectsinvolved = 1;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %d %s\n",
              (int) ConditionToDescribe->magnitude,
              ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      break;
      
    case LVPROX: /** line vertex near another line **/
    case LENOCOVERA: /** line end node not covered by area perimeter ***/
    case PLPROX: /** point feature within x of a line feature **/
    case PSHOOTL: /*** point feature over or undershoots a line feature ***/
    case PLPROXEX:  /** pt to line prox with exception for line end node ***/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.6lf %s\n",
	      ConditionToDescribe->magnitude,
	      ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      
      if(strcmp(ConditionLookup[con_index].units,"radians") == 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   (Magnitude in degrees: %.2lf)\n",
		  ConditionToDescribe->magnitude * RadToDegreeFactor);
	  strcat(MessageString,msgline);
	}
      break;
      
    case VTEAR: /** vertical tear **/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.6lf %s\n",
	      ConditionToDescribe->magnitude,
	      ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      
      
      if(strcmp(ConditionLookup[con_index].units,"radians") == 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   (Magnitude in degrees: %.2lf)\n",
		  ConditionToDescribe->magnitude * RadToDegreeFactor);
	  strcat(MessageString,msgline);
	}
      if(ConditionToDescribe->num_polys > 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Tear measured at location %s\n",
		  Coordinate2DtoString(ConditionToDescribe->poly1.x[0],
				       ConditionToDescribe->poly1.y[0]));
	  strcat(MessageString,msgline);
	  
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  strcat(MessageString,"   Tear along edge of adjacent objects:\n");
	}
      else if(ConditionToDescribe->num_areals > 0)
        {
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Tear measured at location %s\n",
		  Coordinate2DtoString(ConditionToDescribe->areal1.x[0],
				       ConditionToDescribe->areal1.y[0]));
	  strcat(MessageString,msgline);
	  
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  strcat(MessageString,"   Tear along edge of adjacent objects:\n");
        }
      break;
      
      
    case LODELEVDIF: /** elev difference between surface grids or polys in different LOD **/
    case GRIDEXACTDIF: /** Grids have post value difference at same X,Y ***/
    case CLAMP_SEG: /*** catenary segment below associated DEM ****/
    case MASKSHOREL: /** water body not contained by shoreline ***/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.6lf %s\n",
	      ConditionToDescribe->magnitude,
	      ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      
      
      if(ConditionToDescribe->num_polys > 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Surface elevation difference measured at location %s\n",
		  Coordinate2DtoString(ConditionToDescribe->poly1.x[0],
				       ConditionToDescribe->poly1.y[0]));
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  strcat(MessageString,"   A vertex of polygon:\n");
	}
      else if(ConditionToDescribe->num_gridpt > 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Surface elevation difference measured at location %s\n",
		  Coordinate2DtoString(ConditionToDescribe->gridobj1.x[0],
				       ConditionToDescribe->gridobj1.y[0]));
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  strcat(MessageString,"   An elevation data point of grid:\n");
        }
      break;
    case DUPLICATESEG: /*** duplicated segments ***/
    case SHAREPERIM: /** shared areal perimeter segments ***/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      
      if(ConditionToDescribe->magnitude > 1.0)
	{
	  sprintf(msgline,"   Condition magnitude %d %s\n",
		  (int) ConditionToDescribe->magnitude,
		  ConditionLookup[con_index].units);
	}
      else
	{
	  sprintf(msgline,"   Condition magnitude %d shared segment\n",
		  (int) ConditionToDescribe->magnitude);
	}
      strcat(MessageString,msgline);
      break;
      
    case EXTRA_NET:
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.15lf %s\n",
              ConditionToDescribe->magnitude,
              ConditionLookup[con_index].units);
      
      strcat(MessageString,msgline);
      
      sprintf(msgline,"     Near Miss between networks number %d and %d (inspection instance %d)\n",
	      (int) ConditionToDescribe->circle1.radius,(int) ConditionToDescribe->circle2.radius,
	      ConditionToDescribe->CONDITION_INSTANCE+1);
      strcat(MessageString,msgline);
      break;
      
      
    case INTRA_NET:   /*** vertex is close to but not identical with another vertex in the same network ***/
      objectsinvolved = 2;
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.15lf %s\n",
              ConditionToDescribe->magnitude,
              ConditionLookup[con_index].units);
      
      strcat(MessageString,msgline);
      
      sprintf(msgline,"     Near Miss is on network number %d (inspection instance %d)\n",
	      (int) ConditionToDescribe->circle1.radius,
	      ConditionToDescribe->CONDITION_INSTANCE+1);
      strcat(MessageString,msgline);
      break;
      
    case SLOPEDIRCH:
    case CLAMP_SDC: /*slope direction change along a line that has been elevation-value clamped to underlying DEM ***/
    case CLAMP_JOINSDC: /** slope direction change at line feature connection when both are clamped to DEM ***/
      objectsinvolved = 1;
      if(ConditionToDescribe->num_areals > 0)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angle of change measured at %s\n Magnitude %lf\n",
                  Coordinate2DtoString(ConditionToDescribe->areal1.x[1],
                                       ConditionToDescribe->areal1.y[1]),
		  ConditionToDescribe->magnitude);
          strcat(MessageString,msgline);
        }
      else if(ConditionToDescribe->num_lines > 0)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angle of change measured at %s\n Magnitude %lf\n",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
                                       ConditionToDescribe->linear1.y[1]),
		  ConditionToDescribe->magnitude);
          strcat(MessageString,msgline);
        }
      break;
      
    case LJOINSLOPEDC:
    case CONTEXT_KINK:
      objectsinvolved = 2;
      if(ConditionToDescribe->CONDITION_TYPE == LJOINSLOPEDC)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angle of change measured at %s\n Magnitude %lf\n",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
                                       ConditionToDescribe->linear1.y[1]),
		  ConditionToDescribe->magnitude);
          strcat(MessageString,msgline);
        }
      else if(ConditionToDescribe->CONDITION_TYPE == CONTEXT_KINK)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   Angle measured at %s\n",
                  Coordinate2DtoString(ConditionToDescribe->linear1.x[1],
                                       ConditionToDescribe->linear1.y[1]));
          strcat(MessageString,msgline);
        }
      break;
      
      
      
    case LELINEPROX:  /** line end node and line (vertex, segment ...) proximity ***/
    case EN_EN_PROX:  /** undershoot end nodes connected by another feature **/
    case LUNDERSHTL:  /** line end - line undershoot **/
    case LUSHTL_CLEAN: /* like line - line undershoot, but no condition if feature mid-undershoot **/
    case LUSHTL_DF: /** line - line undershoot, different line feature types ***/
    case LOSHTL_DF: /** line - line overshoot, different line feature types ***/
    case LVUSHTL: /** interior line vertex undershoots a different line feature **/
    case VUSHTL_CLEAN: /* like vertex - line undershoot, but no condition if feature mid-undershoot **/
    case LVOSHTL: /** interior line vertex overshoots a different line feature ***/
    case LOVERSHTL:   /** line end - line overshoot **/
    case LUNDERSHTA:  /** line end area perimeter undershoot **/
    case LOVERSHTA:  /** line end - area perimeter overshoot **/
    case LAPROX:  /** line to area proximity - smallest dist between the two features ***/
    case LASLIVER: /** sliver formed between line and area features **/
    case LSLICEA: /** line 'slices' area so as create a small piece ***/
    case LLSLIVER:  /** sliver formed between two line features **/
    case AUNDERSHTA: /** area edge undershoots neighbor area edge ***/
    case AOVERSHTA: /** area edge overshoots neighbor area edge ***/
    case L2D_L3D_MATCH: /** linear end node Z mismatch ***/
    case LEZ_PROX_3D: /** apply check L2D_L3D_MATCH to 3d line features only **/
    case CNODE_ZBUST:  /*** Z mismatch between any two connecting nodes (in x,y) ***/
    case LSPINT: /** line intersects poly with slope > x **/
    case SHARESEG: /** line feature segment overlaps 1 other line feature segment ***/
    /*** case SHARE3SEG:  old way  ** line feature segment overlaps 2 other line feature segments ***/
    case LLI_ANGLE: /*** 2 lines intersect at severe angle ***/
    case LSPIEXP: /** line - poly (slope > x) except when intersection **/
    case PTPTPROX: /** point to point proximity **/
    case PUNDERSHTA: /** point not on area perimeter and is outside that area feature **/
    case POVERSHTA: /** point not on area perimeter and is inside that area feature **/
      objectsinvolved = 2;
      
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %.6lf %s\n",
	      ConditionToDescribe->magnitude,
	      ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      
      if(strcmp(ConditionLookup[con_index].units,"radians") == 0)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   (Magnitude in degrees: %.2lf)\n",
		  ConditionToDescribe->magnitude * RadToDegreeFactor);
	  strcat(MessageString,msgline);
	}
      break;
      
    case LOC_MULTINT: /** lines with no or compatible LOC values intersect each other multiple times **/
    case LLMULTINT: /** lines intersect each other multiple times **/
      objectsinvolved = 2;
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      sprintf(msgline,"   Condition magnitude %d %s\n",
              (int) ConditionToDescribe->magnitude,
              ConditionLookup[con_index].units);
      strcat(MessageString,msgline);
      
      if(strcmp(ConditionLookup[con_index].units,"radians") == 0)
        {
          MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
          sprintf(msgline,"   (Magnitude in degrees: %.2lf)\n",
                  ConditionToDescribe->magnitude * RadToDegreeFactor);
          strcat(MessageString,msgline);
        }
      break;
      
    default:
      printf("unrecognized condition type (%d) in condition file\n",ConditionToDescribe->CONDITION_TYPE);
      exit(1);
    }  /** end switch on Sortkey ***/
  
  
  
  
  
  
  
  
  if(objectsinvolved > 0)
    {
      extern char *GetECCCode(int code);
      extern char * GetECCLabel(int code);
      
      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
      
      if(NGA_TYPE==1)
	  {
	   sprintf(msgline,"\nCondition involves feature of type: %s %s\n",GetECCCode(ConditionToDescribe->ECC1),GetECCLabel(ConditionToDescribe->ECC1));
	  }
      else
	  {
	   sprintf(msgline,"Condition involves feature of type: %s\n",GetECCLabel(ConditionToDescribe->ECC1));
	  }
      strcat(MessageString,msgline);
      
      if(objectsinvolved > 1)
	{
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  if(NGA_TYPE==1)
	    {
	      sprintf(msgline,"Condition involves feature of type: %s %s\n",GetECCCode(ConditionToDescribe->ECC2),GetECCLabel(ConditionToDescribe->ECC2));
	    }
	  else
	    {
	      sprintf(msgline,"Condition involves feature of type: %s\n",GetECCLabel(ConditionToDescribe->ECC2));
	    }
	  strcat(MessageString,msgline); 
	}
    }
  
  strcat(MessageString,"\n");
  
  
  
  if(objectsinvolved > 0)
    {
      if(ConditionToDescribe->num_areals > 0)
	{

	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Area Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->areal1.IDENT,
			  ConditionToDescribe->areal1.objectid,
			  ConditionToDescribe->areal1.Lindex)
			  );
	      strcat(MessageString,msgline);
        


	  if(ConditionToDescribe->areal1.numverts < VertexPrintLimit)
            {
	      if((ConditionToDescribe->CONDITION_TYPE != AOVERLAPA) && (ConditionToDescribe->CONDITION_TYPE != ZUNCLOSED) &&
		 (ConditionToDescribe->CONDITION_TYPE != NOT_FLAT) && (ConditionToDescribe->CONDITION_TYPE != CLAMP_DIF) &&
                 (ConditionToDescribe->CONDITION_TYPE != CLAMP_NFLAT) && (ConditionToDescribe->CONDITION_TYPE != AREAUNCLOSED))
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM + SMALLNUM*ConditionToDescribe->areal1.numverts);
                  if(ConditionToDescribe->areal1.numverts == 1)
                     sprintf(msgline," Area Feature vertex:\n");
                  else
		     sprintf(msgline," Area Feature vertices:\n");
		  strcat(MessageString,msgline);
		  p = MessageString;

		  for(i=0; i<ConditionToDescribe->areal1.numverts; i++)
		    {
		      sprintf(msgline,"   %s\n",
			      Coordinate3DtoString(ConditionToDescribe->areal1.x[i],
						   ConditionToDescribe->areal1.y[i],ConditionToDescribe->areal1.z[i]));
		      p = FastStrCat(p,msgline);
		    }
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"\n");
		  strcat(MessageString,msgline);
		}
	      else if((ConditionToDescribe->CONDITION_TYPE == AOVERLAPA) && (ConditionToDescribe->areal1.numverts > 0))
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM + SMALLNUM*ConditionToDescribe->areal1.numverts);
		  sprintf(msgline," Area Feature perimeter vertices around intersection point:\n");
		  strcat(MessageString,msgline);
		  p = MessageString;
		  
		  for(i=0; i<ConditionToDescribe->areal1.numverts; i++)
		    {
		      sprintf(msgline,"   %s\n",
			      Coordinate3DtoString(ConditionToDescribe->areal1.x[i],
						   ConditionToDescribe->areal1.y[i],ConditionToDescribe->areal1.z[i]));
		      p = FastStrCat(p,msgline);
		    }
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"\n");
		  strcat(MessageString,msgline);
		}
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Area Feature has %d vertices\n",ConditionToDescribe->areal1.numverts);
	      strcat(MessageString,msgline);
            }
	}
      if(ConditionToDescribe->num_areals > 1)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Area Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->areal2.IDENT,
			  ConditionToDescribe->areal2.objectid,
			  ConditionToDescribe->areal2.Lindex)
			  );
	      strcat(MessageString,msgline);
		  p = MessageString;
            

	  if(ConditionToDescribe->areal2.numverts < VertexPrintLimit)
            {
              if((ConditionToDescribe->CONDITION_TYPE == AOVERLAPA) && (ConditionToDescribe->areal2.numverts > 0))
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM + SMALLNUM*ConditionToDescribe->areal2.numverts);
		  sprintf(msgline," Area Feature perimeter vertices around intersection point:\n");
		  strcat(MessageString,msgline);
		  p = MessageString;
		  
		  for(i=0; i<ConditionToDescribe->areal2.numverts; i++)
		    {
		      sprintf(msgline,"   %s\n",
			      Coordinate3DtoString(ConditionToDescribe->areal2.x[i],
						   ConditionToDescribe->areal2.y[i],ConditionToDescribe->areal2.z[i]));
		      p = FastStrCat(p,msgline);
		    }
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"\n");
		  strcat(MessageString,msgline);
		}
              else if((ConditionToDescribe->CONDITION_TYPE == AOVERLAPA) && (ConditionToDescribe->areal2.numverts == 0))
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM + SMALLNUM*ConditionToDescribe->areal2.numverts);
                  if(ConditionToDescribe->areal2.numverts == 1)
                     sprintf(msgline," Area Feature vertex of contained feature:\n");
                  else
		     sprintf(msgline," Area Feature vertices of contained feature:\n");
		  strcat(MessageString,msgline);
		  p = MessageString;

		  for(i=0; i<ConditionToDescribe->areal2.numverts; i++)
		    {
		      sprintf(msgline,"   %s\n",
			      Coordinate3DtoString(ConditionToDescribe->areal2.x[i],
						   ConditionToDescribe->areal2.y[i],ConditionToDescribe->areal2.z[i]));
		      p = FastStrCat(p,msgline);
		    }
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"\n");
		  strcat(MessageString,msgline);
		}
              else
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM + SMALLNUM*ConditionToDescribe->areal2.numverts);
                  if(ConditionToDescribe->areal2.numverts == 1)
                     sprintf(msgline," Area Feature vertex:\n");
                  else
		     sprintf(msgline," Area Feature vertices:\n");
		  strcat(MessageString,msgline);
		  p = MessageString;

		  for(i=0; i<ConditionToDescribe->areal2.numverts; i++)
		    {
		      sprintf(msgline,"   %s\n",
			      Coordinate3DtoString(ConditionToDescribe->areal2.x[i],
						   ConditionToDescribe->areal2.y[i],ConditionToDescribe->areal2.z[i]));
		      p = FastStrCat(p,msgline);
		    }
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"\n");
		  strcat(MessageString,msgline);
		}
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Area Feature has %d vertices\n\n",ConditionToDescribe->areal2.numverts);
	      strcat(MessageString,msgline);
            }
	}
      
      
      if(ConditionToDescribe->num_polys > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Polygon:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->poly1.IDENT,
			  ConditionToDescribe->poly1.objectid,
			  ConditionToDescribe->poly1.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  if((ConditionToDescribe->poly1.numverts < VertexPrintLimit) && (ConditionToDescribe->CONDITION_TYPE != LODELEVDIF) &&
                        (ConditionToDescribe->CONDITION_TYPE != GRIDEXACTDIF) &&
                        (ConditionToDescribe->CONDITION_TYPE != MASKZERO) && (ConditionToDescribe->CONDITION_TYPE != MASKCONSTANT) &&
                          (ConditionToDescribe->CONDITION_TYPE != MASKSHOREL) &&
                          (ConditionToDescribe->CONDITION_TYPE != MASKEDIT_0) &&
                          (ConditionToDescribe->CONDITION_TYPE != MASKEDIT_1) &&
                          (ConditionToDescribe->CONDITION_TYPE != MASKMONO))
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM +SMALLNUM*ConditionToDescribe->poly1.numverts);
	      sprintf(msgline," Polygon Object vertices:\n");
	      strcat(MessageString,msgline);
	      p = MessageString;
	      
	      for(i=0; i<ConditionToDescribe->poly1.numverts; i++)
		{
		  sprintf(msgline,"   %s\n",
			  Coordinate3DtoString(ConditionToDescribe->poly1.x[i],
					       ConditionToDescribe->poly1.y[i],ConditionToDescribe->poly1.z[i]));
		  p = FastStrCat(p,msgline);
		}
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"\n");
	      strcat(MessageString,msgline);
            }
	  else if((ConditionToDescribe->CONDITION_TYPE != LODELEVDIF) && (ConditionToDescribe->CONDITION_TYPE != MASKZERO) &&
                   (ConditionToDescribe->CONDITION_TYPE != MASKCONSTANT) && (ConditionToDescribe->CONDITION_TYPE != MASKSHOREL) &&
                          (ConditionToDescribe->CONDITION_TYPE != GRIDEXACTDIF) &&
                          (ConditionToDescribe->CONDITION_TYPE != MASKEDIT_0) &&
                          (ConditionToDescribe->CONDITION_TYPE != MASKEDIT_1) &&
                           (ConditionToDescribe->CONDITION_TYPE != MASKMONO))
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Polygon Object has %d vertices\n\n",ConditionToDescribe->poly1.numverts);
	      strcat(MessageString,msgline);
            }
	}
      if(ConditionToDescribe->num_polys > 1)
	{
          if((ConditionToDescribe->CONDITION_TYPE == LODELEVDIF) || (ConditionToDescribe->CONDITION_TYPE == GRIDEXACTDIF))
            {
              MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
              sprintf(msgline," Compared elevation from polygon:\n");
              strcat(MessageString,msgline);
            }

		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Polygon:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->poly2.IDENT,
			  ConditionToDescribe->poly2.objectid,
			  ConditionToDescribe->poly2.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  if(ConditionToDescribe->poly2.numverts < VertexPrintLimit)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM +SMALLNUM*ConditionToDescribe->poly2.numverts);
	      sprintf(msgline," Polygon Object vertices:\n");
	      strcat(MessageString,msgline);
	      p = MessageString;
	      
	      for(i=0; i<ConditionToDescribe->poly2.numverts; i++)
		{
		  sprintf(msgline,"   %s\n",
			  Coordinate3DtoString(ConditionToDescribe->poly2.x[i],
					       ConditionToDescribe->poly2.y[i],ConditionToDescribe->poly2.z[i]));
		  p = FastStrCat(p,msgline);
		}
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"\n");
	      strcat(MessageString,msgline);
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Polygon Object has %d vertices\n\n",ConditionToDescribe->poly2.numverts);
	      strcat(MessageString,msgline);
            }
	}
      
      if(ConditionToDescribe->num_lines > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Line Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->linear1.IDENT,
			  ConditionToDescribe->linear1.objectid,
			  ConditionToDescribe->linear1.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  if(ConditionToDescribe->linear1.numverts < VertexPrintLimit)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM +SMALLNUM*ConditionToDescribe->linear1.numverts);
	      
	      if(ConditionToDescribe->linear1.numverts == 1)
		{
		  sprintf(msgline," Line Feature vertex:\n");
		}
	      else
		{
		  sprintf(msgline," Line Feature vertices:\n");
		}
	      
	      strcat(MessageString,msgline);
	      p = MessageString;


	      for(i=0; i<ConditionToDescribe->linear1.numverts; i++)
		{
		  sprintf(msgline,"   %s\n",
			  Coordinate3DtoString(ConditionToDescribe->linear1.x[i],
					       ConditionToDescribe->linear1.y[i],ConditionToDescribe->linear1.z[i]));
		  p = FastStrCat(p,msgline);
		}
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"\n");
	      strcat(MessageString,msgline);
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Line Feature has %d vertices\n\n",
		      ConditionToDescribe->linear1.numverts);
	      strcat(MessageString,msgline);
            }
	}
      if(ConditionToDescribe->num_lines > 1)
	{
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Line Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->linear2.IDENT,
			  ConditionToDescribe->linear2.objectid,
			  ConditionToDescribe->linear2.Lindex)
			  );
	      strcat(MessageString,msgline);


 	  if(ConditionToDescribe->linear2.numverts < VertexPrintLimit)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM +SMALLNUM*ConditionToDescribe->linear2.numverts);
	      
	      if(ConditionToDescribe->linear2.numverts == 1)
		{
		  sprintf(msgline," Line Feature vertex:\n");
		}
	      else
		{
		  sprintf(msgline," Line Feature vertices:\n");
		}
	      
	      strcat(MessageString,msgline);
	      p = MessageString;


	      for(i=0; i<ConditionToDescribe->linear2.numverts; i++)
		{
		  sprintf(msgline,"   %s\n",
			  Coordinate3DtoString(ConditionToDescribe->linear2.x[i],
					       ConditionToDescribe->linear2.y[i],ConditionToDescribe->linear2.z[i]));
		  p = FastStrCat(p,msgline);
		}
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"\n");
	      strcat(MessageString,msgline);
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Line Feature has %d vertices\n\n",
		      ConditionToDescribe->linear2.numverts);
	      strcat(MessageString,msgline);
            }
	}
      
      if(ConditionToDescribe->num_points > 0)
	{
	 
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Point Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->point1.IDENT,
			  ConditionToDescribe->point1.objectid,
			  ConditionToDescribe->point1.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Point Feature vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->point1.x,
				       ConditionToDescribe->point1.y,ConditionToDescribe->point1.z));
	  strcat(MessageString,msgline);
	}
      if(ConditionToDescribe->num_points > 1)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Point Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->point2.IDENT,
			  ConditionToDescribe->point2.objectid,
			  ConditionToDescribe->point2.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Point Feature vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->point2.x,
				       ConditionToDescribe->point2.y,ConditionToDescribe->point2.z));
	  strcat(MessageString,msgline);
	}
      
      if(ConditionToDescribe->num_circles > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Circular Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->circle1.IDENT,
			  ConditionToDescribe->circle1.objectid,
			  ConditionToDescribe->circle1.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Circular Object radius: %lf\n",ConditionToDescribe->circle1.radius);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Circular Object vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->circle1.x,
				       ConditionToDescribe->circle1.y,ConditionToDescribe->circle1.z));
	  strcat(MessageString,msgline);
	}
      if(ConditionToDescribe->num_circles > 1)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Circular Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->circle2.IDENT,
			  ConditionToDescribe->circle2.objectid,
			  ConditionToDescribe->circle2.Lindex)
			  );
	      strcat(MessageString,msgline);
 	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Circular Object radius: %lf\n",ConditionToDescribe->circle2.radius);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Circular Object vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->circle2.x,
				       ConditionToDescribe->circle2.y,ConditionToDescribe->circle2.z));
	  strcat(MessageString,msgline);
	}
      
      
      if(ConditionToDescribe->num_sphere > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Spherical Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->sphere1.IDENT,
			  ConditionToDescribe->sphere1.objectid,
			  ConditionToDescribe->sphere1.Lindex)
			  );
	      strcat(MessageString,msgline);


	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Spherical Object radius: %lf\n",ConditionToDescribe->sphere1.radius);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Spherical Object vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->sphere1.x,
				       ConditionToDescribe->sphere1.y,ConditionToDescribe->sphere1.z));
	  strcat(MessageString,msgline);
	}
      if(ConditionToDescribe->num_sphere > 1)
	{
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Spherical Feature:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->sphere2.IDENT,
			  ConditionToDescribe->sphere2.objectid,
			  ConditionToDescribe->sphere2.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Spherical Object radius: %lf\n",ConditionToDescribe->sphere2.radius);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Spherical Object vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->sphere2.x,
				       ConditionToDescribe->sphere2.y,ConditionToDescribe->sphere2.z));
	  strcat(MessageString,msgline);
	}
      
      
      if(ConditionToDescribe->num_gridpt > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Grid Point:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->gridobj1.IDENT,
			  -1,
			  ConditionToDescribe->gridobj1.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  if(ConditionToDescribe->gridobj1.numverts < VertexPrintLimit)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Grid Point Object locations:\n");
	      strcat(MessageString,msgline);
	      
	      for(i=0; i<ConditionToDescribe->gridobj1.numverts; i++)
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"   %s\n",
			  Coordinate3DtoString(ConditionToDescribe->gridobj1.x[i],
					       ConditionToDescribe->gridobj1.y[i],ConditionToDescribe->gridobj1.z[i]));
		  strcat(MessageString,msgline);
		}
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"\n");
	      strcat(MessageString,msgline);
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Grid Point Object has %d vertices\n\n",ConditionToDescribe->gridobj1.numverts);
	      strcat(MessageString,msgline);
            }
	}
      if(ConditionToDescribe->num_gridpt > 1)
	{
          if((ConditionToDescribe->CONDITION_TYPE == LODELEVDIF) || (ConditionToDescribe->CONDITION_TYPE == CLAMP_SEG) ||
                 (ConditionToDescribe->CONDITION_TYPE == MASKZERO) || (ConditionToDescribe->CONDITION_TYPE == MASKCONSTANT) ||
                    (ConditionToDescribe->CONDITION_TYPE == MASKSHOREL) ||
                          (ConditionToDescribe->CONDITION_TYPE == GRIDEXACTDIF) ||
                          (ConditionToDescribe->CONDITION_TYPE == MASKEDIT_0) ||
                          (ConditionToDescribe->CONDITION_TYPE == MASKEDIT_1) ||
                          (ConditionToDescribe->CONDITION_TYPE == MASKMONO))
            {
              MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
              sprintf(msgline," Compared elevation from grid:\n");
              strcat(MessageString,msgline);
            }

	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Grid Point:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->gridobj2.IDENT,
			  -1,
			  ConditionToDescribe->gridobj2.Lindex)
			  );
	      strcat(MessageString,msgline);
            
	  if(ConditionToDescribe->gridobj2.numverts < VertexPrintLimit)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Grid Point Object location:\n");
	      strcat(MessageString,msgline);
	      
	      for(i=0; i<ConditionToDescribe->gridobj2.numverts; i++)
		{
		  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
		  sprintf(msgline,"   %s\n",
			  Coordinate3DtoString(ConditionToDescribe->gridobj2.x[i],
					       ConditionToDescribe->gridobj2.y[i],ConditionToDescribe->gridobj2.z[i]));
		  strcat(MessageString,msgline);
		}
	      
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"\n");
	      strcat(MessageString,msgline);
            }
	  else
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Grid Point Object has %d vertices\n\n",ConditionToDescribe->gridobj2.numverts);
	      strcat(MessageString,msgline);
            }
	}
      
      if(ConditionToDescribe->num_piped > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Parallelpiped:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->piped1.IDENT,
			  ConditionToDescribe->piped1.objectid,
			  ConditionToDescribe->piped1.Lindex)
			  );
	      strcat(MessageString,msgline);

	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Object height:\n   %.4lf\n",ConditionToDescribe->piped1.height);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Parallelpiped Object vertices:\n");
	  strcat(MessageString,msgline);
	  
	  for(i=0; i<4; i++)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"   %s\n",
		      Coordinate3DtoString(ConditionToDescribe->piped1.x[i],
					   ConditionToDescribe->piped1.y[i],ConditionToDescribe->piped1.z[i]));
	      strcat(MessageString,msgline);
            }
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"\n");
	  strcat(MessageString,msgline);
	}
      if(ConditionToDescribe->num_piped > 1)
	{

	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Parallelpiped:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->piped2.IDENT,
			  ConditionToDescribe->piped2.objectid,
			  ConditionToDescribe->piped2.Lindex)
			  );
	      strcat(MessageString,msgline);

	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   Object height:\n   %.4lf\n",ConditionToDescribe->piped2.height);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Parallelpiped Object vertices:\n");
	  strcat(MessageString,msgline);
	  
	  for(i=0; i<4; i++)
            {
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline,"   %s\n",
		      Coordinate3DtoString(ConditionToDescribe->piped2.x[i],
					   ConditionToDescribe->piped2.y[i],ConditionToDescribe->piped2.z[i]));
	      strcat(MessageString,msgline);
            }
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"\n");
	  strcat(MessageString,msgline);
	}
      
      if(ConditionToDescribe->num_cylinder > 0)
	{
	  
	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Cylinder:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->cylinder1.IDENT,
			  ConditionToDescribe->cylinder1.objectid,
			  ConditionToDescribe->cylinder1.Lindex)
			  );
	      strcat(MessageString,msgline);

	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Cylinder Object radius: %lf\n",ConditionToDescribe->cylinder1.radius);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Cylinder Object height: %lf\n",ConditionToDescribe->cylinder1.height);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Cylinder Object vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->cylinder1.x,
				       ConditionToDescribe->cylinder1.y,ConditionToDescribe->cylinder1.z));
	  strcat(MessageString,msgline);
	}
      if(ConditionToDescribe->num_cylinder > 1)
	{

	      MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	      sprintf(msgline," Cylinder:  %s\n",
		      MakeIDString(
			  ConditionToDescribe->cylinder2.IDENT,
			  ConditionToDescribe->cylinder2.objectid,
			  ConditionToDescribe->cylinder2.Lindex)
			  );
	      strcat(MessageString,msgline);


	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Cylinder Object radius: %lf\n",ConditionToDescribe->cylinder2.radius);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Cylinder Object height: %lf\n",ConditionToDescribe->cylinder2.height);
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline," Cylinder Object vertex:\n");
	  strcat(MessageString,msgline);
	  
	  MessageString = (char *) realloc(MessageString,strlen(MessageString)+BIGNUM);
	  sprintf(msgline,"   %s\n",
		  Coordinate3DtoString(ConditionToDescribe->cylinder2.x,
				       ConditionToDescribe->cylinder2.y,ConditionToDescribe->cylinder2.z));
	  strcat(MessageString,msgline);
	}
      
    } /** end if at least one object involved ***/
  
  
  ConditionToDescribe->Message    = (char *) (malloc(strlen(MessageString)+10));
  sprintf(ConditionToDescribe->Message,"%s",MessageString);
  
  free(MessageString);
}




void DrawConditionObjects(struct ConditionList * ConditionToDraw)
{
  int i;
  int j;
  
  
  if(ConditionToDraw->AdditionalPointUsed == 1)
    {
      drawcircle(ConditionToDraw->pointx,ConditionToDraw->pointy, 10.0, 1);
    }
  if(ConditionToDraw->CONDITION_TYPE == TVERT)
    {
      return;
    }
  
  
  for(i=1; i <= ConditionToDraw->num_areals; i++)
    {
      if(i == 1)
	{
	  if(ConditionToDraw->areal1.numverts == 1)
	    {
	      drawcircle(ConditionToDraw->areal1.x[0],ConditionToDraw->areal1.y[0],10.0,1);
	    }
	  else if(ConditionToDraw->areal1.numverts == 2)
	    {
	      drawline(ConditionToDraw->areal1.x[0],ConditionToDraw->areal1.y[0],
		       ConditionToDraw->areal1.x[1],ConditionToDraw->areal1.y[1],1);
	    }
	  else if(ConditionToDraw->areal1.numverts > 0)
	    {
             if(ConditionToDraw->CONDITION_TYPE == AREAKINK)
                {
                  drawline(ConditionToDraw->areal1.x[0],ConditionToDraw->areal1.y[0],
                       ConditionToDraw->areal1.x[1],ConditionToDraw->areal1.y[1],1);
                  drawline(ConditionToDraw->areal1.x[2],ConditionToDraw->areal1.y[2],
                       ConditionToDraw->areal1.x[1],ConditionToDraw->areal1.y[1],1);
                }
              else if((ConditionToDraw->CONDITION_TYPE == SLOPEDIRCH) || (ConditionToDraw->CONDITION_TYPE == CLAMP_SDC) ||
                         (ConditionToDraw->CONDITION_TYPE == CLAMP_JOINSDC))
                {
                  drawline(ConditionToDraw->areal1.x[0],ConditionToDraw->areal1.y[0],
                       ConditionToDraw->areal1.x[1],ConditionToDraw->areal1.y[1],1);
                  drawline(ConditionToDraw->areal1.x[2],ConditionToDraw->areal1.y[2],
                       ConditionToDraw->areal1.x[1],ConditionToDraw->areal1.y[1],1);
                }
              else
                {
                  drawpoly(1,ConditionToDraw->areal1.x,ConditionToDraw->areal1.y,
                       ConditionToDraw->areal1.numverts,1,1);
                }
	    }
	}
      else
	{
	  if(ConditionToDraw->areal2.numverts == 1)
	    {
	      drawcircle(ConditionToDraw->areal2.x[0],ConditionToDraw->areal2.y[0],10.0,1);
	    }
	  else if(ConditionToDraw->areal2.numverts == 2)
	    {
	      drawline(ConditionToDraw->areal2.x[0],ConditionToDraw->areal2.y[0],
		       ConditionToDraw->areal2.x[1],ConditionToDraw->areal2.y[1],1);
	    }
	  else if(ConditionToDraw->areal2.numverts > 0)
	    {
	      drawpoly(1,ConditionToDraw->areal2.x,ConditionToDraw->areal2.y,
		       ConditionToDraw->areal2.numverts,1,1);
	    }
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_polys; i++)
    {
      if(i == 1)
	{
	  if(ConditionToDraw->poly1.numverts == 1)
	    {
	      drawcircle(ConditionToDraw->poly1.x[0],ConditionToDraw->poly1.y[0],10.0,1);
	    }
	  else if(ConditionToDraw->poly1.numverts == 2)
	    {
	      drawline(ConditionToDraw->poly1.x[0],ConditionToDraw->poly1.y[0],
		       ConditionToDraw->poly1.x[1],ConditionToDraw->poly1.y[1],1);
	    }
	  else if(ConditionToDraw->poly1.numverts > 0)
	    {
	      drawpoly(1,ConditionToDraw->poly1.x,ConditionToDraw->poly1.y,
		       ConditionToDraw->poly1.numverts,1,1);
	    }
	}
      else
	{
	  if(ConditionToDraw->poly2.numverts == 1)
	    {
	      drawcircle(ConditionToDraw->poly2.x[0],ConditionToDraw->poly2.y[0],10.0,1);
	    }
	  else if(ConditionToDraw->poly2.numverts == 2)
	    {
	      drawline(ConditionToDraw->poly2.x[0],ConditionToDraw->poly2.y[0],
		       ConditionToDraw->poly2.x[1],ConditionToDraw->poly2.y[1],1);
	    }
	  else if(ConditionToDraw->poly2.numverts > 0)
	    {
	      drawpoly(1,ConditionToDraw->poly2.x,ConditionToDraw->poly2.y,
		       ConditionToDraw->poly2.numverts,1,1);
	    }
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_lines; i++)
    {
      if(i == 1)
	{
	  if(ConditionToDraw->linear1.numverts == 1)
	    {
	      drawcircle(ConditionToDraw->linear1.x[0],ConditionToDraw->linear1.y[0],10.0,1);
	    }
          else
            {
              if(ConditionToDraw->CONDITION_TYPE == Z_KINK)
                {
                  drawcircle(ConditionToDraw->linear1.x[1],ConditionToDraw->linear1.y[1],10.0,1);
                  drawcircle(ConditionToDraw->linear1.x[2],ConditionToDraw->linear1.y[2],10.0,1);
                }

	      drawfullline(ConditionToDraw->linear1.x,ConditionToDraw->linear1.y,
			   ConditionToDraw->linear1.numverts,1);
	      
            }
	}
      else
	{
	  if(ConditionToDraw->linear2.numverts == 1)
	    {
	      drawcircle(ConditionToDraw->linear2.x[0],ConditionToDraw->linear2.y[0],10.0,1);
	    }
          else
	    {
	      drawfullline(ConditionToDraw->linear2.x,ConditionToDraw->linear2.y,
			   ConditionToDraw->linear2.numverts,1);
	    }
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_points; i++)
    {
      if(i == 1)
	{
	  drawX(ConditionToDraw->point1.x,
		ConditionToDraw->point1.y, 1);
	}
      else
	{
	  drawX(ConditionToDraw->point2.x,
		ConditionToDraw->point2.y, 1);
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_sphere; i++)
    {
      if(i == 1)
	{
	  drawcircle(ConditionToDraw->sphere1.x,
		     ConditionToDraw->sphere1.y,
		     ConditionToDraw->sphere1.radius,2);
	}
      else
	{
	  drawcircle(ConditionToDraw->sphere2.x,
		     ConditionToDraw->sphere2.y,
		     ConditionToDraw->sphere2.radius,2);
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_circles; i++)
    {
      if(i == 1)
	{
	  drawcircle(ConditionToDraw->circle1.x,
		     ConditionToDraw->circle1.y,
		     ConditionToDraw->circle1.radius,2);
	}
      else
	{
	  drawcircle(ConditionToDraw->circle2.x,
		     ConditionToDraw->circle2.y,
		     ConditionToDraw->circle2.radius,2);
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_gridpt; i++)
    {
      if(i == 1)
	{
	  for(j=0; j < ConditionToDraw->gridobj1.numverts; j++)
            {
	      drawStar(ConditionToDraw->gridobj1.x[j],
		       ConditionToDraw->gridobj1.y[j], 1);
            }
	}
      else
	{
	  for(j=0; j < ConditionToDraw->gridobj2.numverts; j++)
            {
	      drawStar(ConditionToDraw->gridobj2.x[j],
		       ConditionToDraw->gridobj2.y[j], 1);
            }
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_piped; i++)
    {
      if(i == 1)
	{
	  drawpoly(1,ConditionToDraw->piped1.x,ConditionToDraw->piped1.y,4,1,1);
	}
      else
	{
	  drawpoly(1,ConditionToDraw->piped2.x,ConditionToDraw->piped2.y,4,1,1);
	}
    }
  
  for(i=1; i <= ConditionToDraw->num_cylinder; i++)
    {
      if(i == 1)
	{
	  drawcircle(ConditionToDraw->cylinder1.x,
		     ConditionToDraw->cylinder1.y,
		     ConditionToDraw->cylinder1.radius,2);
	}
      else
	{
	  drawcircle(ConditionToDraw->cylinder2.x,
		     ConditionToDraw->cylinder2.y,
		     ConditionToDraw->cylinder2.radius,2);
	}
    }
  
  
  return;
}


void SEEIT_FreeConditionReportMemory(void)
{
  int i;
  
  if(ConditionLookup != NULL)
    {
      for(i=0;i<TotalConditions;i++)
	{
	  if(ConditionLookup[i].Annotation!=NULL)
	    {
	      free(ConditionLookup[i].Annotation);
	    }
	}
      free(ConditionLookup);
      ConditionLookup = NULL;
    }
}



void ReadES(char *esname)
{
  FILE *loadfile;
  unsigned char endianness;
  int Ijunk,i,j,namelen;
  char dbname[1000];
  int annotlen;
  
  loadfile = fopen(esname,"rb");
  if(loadfile==NULL)
    {
      printf("Could not open %s for read",esname);
      exit(1);
    }
  
  fread(&endianness,sizeof(unsigned char),1,loadfile);

  if(endianness==0)
    {
      condition_endianness = 0;
    }
  if(endianness==1)
    {
      condition_endianness = 1;
    }

  if(DEBUG==1)
    {
      printf("Have determined that the condition files have endianness: ");
      if(condition_endianness==0)
	{
	  printf("LITTLE\n\n\n");
	}
      else if(condition_endianness==1)
	{
	  printf("BIG\n\n\n");
	}
      else
	{
	  printf("UNDETERMINED!!\n");
	  exit(1);
	}
    }




  if(ConditionLookup != NULL)
    {
      for(i=0;i<TotalConditions;i++)
	{
	  if(ConditionLookup[i].Annotation!=NULL)
	    {
	      free(ConditionLookup[i].Annotation);
	    }
	}
      free(ConditionLookup);
      ConditionLookup = NULL;
    }  









  
  file_endianness = condition_endianness;
  
  SEEIT_fread_double(&Xtranslation,loadfile);
  SEEIT_fread_double(&Ytranslation,loadfile);
  
  SEEIT_fread_int(&CoordinateSystem,loadfile);
  
  SEEIT_fread_int(&namelen,loadfile);
  fread(&dbname[0],SzUC,namelen,loadfile);
  

  SEEIT_fread_int(&TotalConditions, loadfile); /** a global variable defined at the top of this file **/
  

  ConditionLookup = (struct ConditionTable *) (malloc((TotalConditions + 1) * sizeof(struct ConditionTable)));
  if(ConditionLookup == NULL)
    {
      printf("available memory insufficient for allocation of condition lookup table structure\n");
      printf("execution must terminate at this point\n");
      exit(1);
    }
  

  
  for(i=0;i<TotalConditions;i++)
    {
      
      SEEIT_fread_int(&ConditionLookup[i].conditiontype,loadfile); 
      
      SEEIT_fread_int(&ConditionLookup[i].instancenumber,loadfile); 


      SEEIT_fread_int(&ConditionLookup[i].numthresholds,loadfile);
      
      if(ConditionLookup[i].numthresholds>=1)
	{
	  SEEIT_fread_double(&ConditionLookup[i].sensitivity,loadfile);
	}
      if(ConditionLookup[i].numthresholds==2)
	{
	  SEEIT_fread_double(&ConditionLookup[i].sensitivity2,loadfile);
	}

      
      SEEIT_fread_int(&annotlen,loadfile);
      if(annotlen==0)
	{
	  ConditionLookup[i].Annotation = NULL;
	}
      else
	{
	  ConditionLookup[i].Annotation = (char *)malloc(annotlen+5);
	  fread(&ConditionLookup[i].Annotation[0],SzUC,annotlen,loadfile);
	  ConditionLookup[i].Annotation[annotlen] = '\0';
	}


      
      SEEIT_fread_int(&Ijunk     ,loadfile);
      for(j=0;j<NUM_C;j++)
	{
	  fread(&ConditionLookup[i].Config1[j]  ,SzUC,1,loadfile);
	  fread(&ConditionLookup[i].Config2[j]  ,SzUC,1,loadfile);
	  fread(&ConditionLookup[i].Config3[j]  ,SzUC,1,loadfile);
	}
      for(j=0;j<NUM_S;j++)
	{
	  fread(&ConditionLookup[i].Stratum1[j]  ,SzUC,1,loadfile);
	  fread(&ConditionLookup[i].Stratum2[j]  ,SzUC,1,loadfile);
	  fread(&ConditionLookup[i].Stratum3[j]  ,SzUC,1,loadfile);
	}
      for(j=0;j<NUM_D;j++)
	{
	  fread(&ConditionLookup[i].Domain1[j] ,SzUC,1,loadfile);
	  fread(&ConditionLookup[i].Domain2[j] ,SzUC,1,loadfile);
	  fread(&ConditionLookup[i].Domain3[j] ,SzUC,1,loadfile);
	}      
    }
  
  fclose(loadfile);
}




void ReadLK(char *lkname)
{
  FILE *lookup;
  int i,Ijunk;
  int TotalConditionsCheck, ConditionType, ConditionInstance;
  
  lookup = fopen(lkname,"rb");
  if(lookup==NULL)
    {
      printf("could not open %s for read\n",lkname);
      exit(1);
    }
  
  file_endianness = condition_endianness;
  
  SEEIT_fread_int(&TotalConditionsCheck, lookup);
  
  if(TotalConditionsCheck != TotalConditions)
    {
      printf("misalignment in condition files - total number of conditions reported mismatch\n");
      exit(1);
    }

  for(i=0;i<TotalConditions;i++)
    {
      SEEIT_fread_int(&ConditionType, lookup);

      if(ConditionType != ConditionLookup[i].conditiontype)
        {
          printf("LK: misalignment in condition files - condition type identifier mismatch\n");
          printf("Read value %d Expected value %d\n",ConditionType,ConditionLookup[i].conditiontype);
          exit(1);
        }

      SEEIT_fread_int(&ConditionInstance, lookup);
      if(ConditionInstance != ConditionLookup[i].instancenumber)
        {
          printf("misalignment in condition files - condition instance identifier mismatch\n");
          printf("Read value %d Expected value %d\n",ConditionInstance,ConditionLookup[i].instancenumber);
          exit(1);
        }


      SEEIT_fread_int(&ConditionLookup[i].number,lookup);
      SEEIT_fread_int(&Ijunk,lookup);
      SEEIT_fread_long(&ConditionLookup[i].fileposn,lookup);
    }
  fclose(lookup);
}





void InitArrayNames()
{
int i;

  for(i=0; i<TotalConditions; i++)
    {

    ConditionLookup[i].units  = "none";
    ConditionLookup[i].units2 = "none";
    ConditionLookup[i].units3 = "none";
    ConditionLookup[i].units4 = "none";
    ConditionLookup[i].units5 = "none";
    ConditionLookup[i].units6 = "none";

    switch(ConditionLookup[i].conditiontype)
     {
      case LELINEPROX:
       ConditionLookup[i].name = "Line End - Point, Line Or Area Feature Proximity";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is coincident with or within the primary tolerance (2D distance)\n\
to any part (including any inter-node segment) of the secondary participant line or area feature or a secondary point feature";
       ConditionLookup[i].units = "meters";
       break;

      case EN_EN_PROX:  /** undershoot end nodes connected by another feature **/
       ConditionLookup[i].name = "Connected Line End Node Proximity";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance)\n\
to an end node of a secondary participant feature, but the two end nodes are separated by a distance\n\
less than or equal to the secondary tolerance (2D distance) and the two end nodes are both coincident\n\
with some segment of a feature as described by the tertiary participant specification";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case LUSHTL_CLEAN: /* like line - line undershoot, but no condition if feature mid-undershoot **/
       ConditionLookup[i].name = "Line - Line Undershoot, No Feature Mid-undershoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance) to any\n\
part of another primary participant feature or to any part of a tertiary participant feature, but is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of another primary\n\
participant line feature and the two line features do not intersect on the primary participant segment\n\
defined by the point at which the undershoot is reported - this\n\
inspection can be configured to ignore undershoots: 1) involving nearly parallel and adjacent line features by\n\
setting appropriate values in the 3rd and 4th tolerances - the 3rd tolerance sets a lower bound on undershoot\n\
magnitude for filtering to be applied (undershoots with magnitude < the 3rd tolerance will not be filtered) -\n\
the 4th tolerance sets a minimum length for adjacency of the undershoot features (features that have adjacent\n\
lengths less than the 4th tolerance will not be filtered); or 2) when a secondary participant lies between the\n\
two features that undershoot each other";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "meters";
       break;


      case LUNDERSHTL:
       ConditionLookup[i].name = "Line - Line Undershoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance) to any\n\
part of a secondary participant feature or to any part of a tertiary participant feature, but is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of the secondary\n\
participant line feature and the two line features do not intersect on the primary participant segment\n\
defined by the point at which the undershoot is reported - this\n\
inspection can be configured to ignore undershoots involving nearly parallel and adjacent line features by\n\
setting appropriate values in the 3rd and 4th tolerances - the 3rd tolerance sets a lower bound on undershoot\n\
magnitude for filtering to be applied (undershoots with magnitude < the 3rd tolerance will not be filtered) -\n\
the 4th tolerance sets a minimum length for adjacency of the undershoot features (features that have adjacent\n\
lengths less than the 4th tolerance will not be filtered)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "meters";
       break;

      case LOVERSHTL:
       ConditionLookup[i].name = "Line - Line Overshoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance) to any\n\
part of a secondary participant feature or to any part of a tertiary participant feature, but is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of the secondary\n\
participant line feature and the two line features intersect on the primary participant segment\n\
defined by the point at which the overshoot is reported - this\n\
inspection can be configured to ignore overshoots involving nearly parallel and adjacent line features by\n\
setting appropriate values in the 3rd and 4th tolerances - the 3rd tolerance sets a lower bound on overshoot\n\
magnitude for filtering to be applied (overshoot with magnitude < the 3rd tolerance will not be filtered) -\n\
the 4th tolerance sets a minimum length for adjacency of the overshoot features (features that have adjacent\n\
lengths less than the 4th tolerance will not be filtered)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "meters";

       break;

      case LUSHTL_DF:
       ConditionLookup[i].name = "Different Feature Type Line - Line Undershoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance) to any\n\
part of a secondary participant feature or to any part of a tertiary participant feature, but is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of the secondary\n\
participant line feature and these two line features are different feature types and they do not intersect\n\
on the primary participant segment defined by the point at which the undershoot is reported - this\n\
inspection can be configured to ignore undershoots involving nearly\n\
parallel and adjacent line features by setting appropriate values in the 3rd and 4th tolerances - the 3rd\n\
tolerance sets a lower bound on undershoot magnitude for filtering to be applied (undershoots with magnitude\n\
< the 3rd tolerance will not be filtered) - the 4th tolerance sets a minimum length for adjacency of the\n\
undershoot features (features that have adjacent lengths less than the 4th tolerance will not be filtered)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "meters";
       break;

      case LOSHTL_DF:
       ConditionLookup[i].name = "Different Feature Type Line - Line Overshoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance) to any\n\
part of a secondary participant feature or to any part of a tertiary participant feature, but is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of the secondary\n\
participant line feature and these two line features are different feature types and they intersect on the\n\
primary participant segment defined by the point at which the overshoot is reported \n\
- this inspection can be configured to ignore undershoots involving nearly\n\
parallel and adjacent line features by setting appropriate values in the 3rd and 4th tolerances - the 3rd\n\
tolerance sets a lower bound on undershoot magnitude for filtering to be applied (undershoots with magnitude\n\
< the 3rd tolerance will not be filtered) - the 4th tolerance sets a minimum length for adjacency of the\n\
overshoot features (features that have adjacent lengths less than the 4th tolerance will not be filtered)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "meters";
       break;

      case VUSHTL_CLEAN: /* like vertex - line undershoot, but no condition if feature mid-undershoot **/
       ConditionLookup[i].name = "Interior Line Vertex - Line Undershoot, No Feature Mid-undershoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature interior node that is not within the primary tolerance (2D distance) to any\n\
part of another primary participant feature or to any part of a tertiary participant feature, is the common node\n\
between adjacent line segments that define a heading change along the line feature and the angle of the change\n\
is greater than or equal to the tertiary tolerance, the interior node at the point of heading change is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of another primary\n\
participant line feature, the primary line features do not intersect near the heading-change node and there is no\n\
secondary participant that lies between the vertex and feature near the undershoot location";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       break;


      case LVUSHTL: /** interior line vertex undershoots a different line feature **/
       ConditionLookup[i].name = "Interior Line Vertex - Line Undershoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature interior node that is not within the primary tolerance (2D distance) to any\n\
part of a secondary participant feature or to any part of a tertiary participant feature, is the common node\n\
between adjacent line segments that define a heading change along the line feature and the angle of the change\n\
is greater than or equal to the tertiary tolerance, the interior node at the point of heading change is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of the secondary\n\
participant line feature and the primary and secondary line features do not intersect near the heading-change node";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       break;



      case LVOSHTL: /** interior line vertex overshoots a different line feature **/
       ConditionLookup[i].name = "Interior Line Vertex - Line Overshoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature interior node that is not within the primary tolerance (2D distance) to any\n\
part of a secondary participant feature or to any part of a tertiary participant feature, is the common node\n\
between adjacent line segments that define a heading change along the line feature and the angle of the change\n\
is greater than or equal to the tertiary tolerance, the interior node at the point of heading change is within\n\
the secondary tolerance (2D distance) to some part (including any inter-node segment) of the secondary\n\
participant line feature and the primary and secondary line features intersect each other near the heading-change node";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       break;

      case LUNDERSHTA:
       ConditionLookup[i].name = "Line - Area Perimeter Undershoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance)\n\
to any secondary participant line feature node, secondary participant area feature perimeter edge, \n\
or tertiary participant area perimeter edge, but is within the secondary tolerance (2D distance)\n\
to some part (but not coincident with) of a tertiary participant area feature perimeter and the line\n\
feature end node causing the condition is not inside the tertiary participant area feature perimeter";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case LOVERSHTA:
       ConditionLookup[i].name = "Line - Area Perimeter Overshoot";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not within the primary tolerance (2D distance)\n\
to any secondary participant line feature node, but is within the secondary tolerance (2D distance)\n\
to some part (but not coincident with) of the tertiary participant area feature perimeter and\n\
the line feature does have some portion of its length inside the area feature perimeter";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case LAPROX:
       ConditionLookup[i].name = "Line To Area Proximity";
       ConditionLookup[i].description = "\
The primary participant is a line feature that is within the\n\
specified tolerance (distance) to some part of (including those inside or coincident with)\n\
a secondary participant area feature perimeter";
       ConditionLookup[i].units = "meters";
       break;

      case LASLIVER:
       ConditionLookup[i].name = "Line - Area Sliver";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes an inter-vertex portion\n\
such that the average distance from both related vertices to the secondary participant area feature\n\
perimeter is within the open interval range (distance in meters) defined by the primary\n\
and secondary tolerance specifications and the two inter-vertex portions are nearly parallel -\n\
if another tertiary participant line or area feature is on the area edge, or closer to that edge than\n\
the primary participant inter-vertex portion, then no condition will be reported";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


      case LSLICEA: /** line 'slices' area so as create a small piece ***/
       ConditionLookup[i].name = "Line 'Slices' Area";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects a secondary participant area feature\n\
in such a way as to create two partitions of the area feature where one of the partitions has a\n\
square area less than the primary tolerance and neither partition forms a sliver with the line feature";
       ConditionLookup[i].units = "square meters";
       break;

      case LLSLIVER:
       ConditionLookup[i].name = "Line - Line Sliver";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes an inter-vertex portion\n\
such that the distance from both related vertices to the secondary participant line feature\n\
(any part) is within the open interval range (distance in meters) defined by the primary\n\
and secondary tolerance specifications and the two inter-vertex portions are nearly parallel";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case AUNDERSHTA:
       ConditionLookup[i].name = "Area - Area Undershoot (Gap)";
       ConditionLookup[i].description = "\
The primary participant is an area feature edge that is not coincident\n\
with a secondary participant area feature edge, but is within the open interval range (distance in meters)\n\
defined by the primary and secondary tolerance specifications and the primary participant feature edge is\n\
entirely outside of the secondary participant area feature and the two adjacent edges from an angle <= the tertiary\n\
tolerance - conditions will not be reported if the secondary participant edge is also coincident with an edge of a\n\
feature meeting the specifications of the tertiary participant";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       break;

      case AOVERSHTA:
       ConditionLookup[i].name = "Area - Area Overshoot (Sliver)";
       ConditionLookup[i].description = "\
The primary participant is an area feature edge that is not coincident\n\
with a secondary participant area feature edge, but is within the open interval range (distance in meters)\n\
defined by the primary and secondary tolerance specifications and some part of the primary participant\n\
feature edge is inside of the secondary participant area feature and the two adjacent edges from an angle <= the tertiary tolerance";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       break;


      case LSAME_UNM_A:
       ConditionLookup[i].name = "Line Not Matched By Same Type Line At Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not connected (within primary tolerance distance\n\
in meters) to another line feature node having the same feature type (e.g., FCODE), and is within the \n\
secondary tolerance (2D distance in meters) to some part of the secondary participant area feature perimeter";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

  
      case L_UNM_A:
       ConditionLookup[i].name = "Line Not Matched At Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not connected (within primary tolerance distance\n\
in meters) to any line feature node of any feature type, and is within the secondary tolerance (2D distance in meters)\n\
to some part of the secondary participant area feature perimeter";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

     case LUNMA_ACRS_A: /** line end not matched to area node across area perimeter ***/
       ConditionLookup[i].name = "Line End Not Matched To Area Node Across Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not connected (within primary tolerance distance\n\
in meters) to a node of an area feature as specified by the secondary participant and is within the \n\
secondary tolerance (2D distance in meters) to some part of the tertiary participant area feature perimeter";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

     case LUNM_ACRS_A: /*** line mismatch across poly edge ***/
       ConditionLookup[i].name = "Line Not Matched As Specified Across Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is not connected (within primary tolerance distance\n\
in meters) to a node of another line feature as specified by the secondary participant and is within the \n\
secondary tolerance (2D distance in meters) to some part of the tertiary participant area feature perimeter";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case AUNM_ACRS_A: /** area feature edge incorrectly matched across a bounding area feature ***/
       ConditionLookup[i].name = "Area Not Matched As Specified Across Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that does not overlap (within primary tolerance distance\n\
in meters) a perimeter segment of another area feature as specified by the secondary participant and is within the \n\
secondary tolerance (2D distance in meters) to some perimeter segment of the tertiary participant area feature";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;
  
      case LGEOM_UNM_LAT:
       ConditionLookup[i].name = "Line Geometry Match, Attribute Mismatch At Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is very close to (within third tolerance distance in meters)\n\
a line of latitude described by\n\
an integer multiple of the primary tolerance and the end node is connected (within the distance specified by the\n\
secondary tolerance) to another same-feature-type line feature node on the opposite side of the parallel and the\n\
two connected features have different attributes or the same attributes with different values. See the GAIT Inspections\n\
document entry for this inspection to determine the attributes considered.";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case LGEOM_UNM_LON:
       ConditionLookup[i].name = "Line Geometry Match, Attribute Mismatch At Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is very close to (within third tolerance distance in meters)\n\
a line of longitude described by\n\
an integer multiple of the primary tolerance and the end node is connected (within the distance specified by the\n\
secondary tolerance) to another same-feature-type line feature node on the opposite side of the meridian and the\n\
two connected features have different attributes or the same attributes with different values. See the GAIT Inspections\n\
document entry for this inspection to determine the attributes considered.";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case AGEOM_UNM_LON:
       ConditionLookup[i].name = "Area Geometry Match, Attribute Mismatch At Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that is very close to (within third tolerance distance in meters)\n\
a line of longitude described by\n\
an integer multiple of the primary tolerance and the segment is coincident (within the distance specified by the\n\
secondary tolerance) to another same-feature-type area feature perimeter segment on the opposite side of the meridian and the\n\
two connected features have different attributes or the same attributes with different values. See the GAIT Inspections\n\
document entry for this inspection to determine the attributes considered.";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case AGEOM_UNM_LAT:
       ConditionLookup[i].name = "Area Geometry Match, Attribute Mismatch At Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that is very close to (within third tolerance distance in meters)\n\
a line of latitude described by\n\
an integer multiple of the primary tolerance and the segment is coincident (within the distance specified by the\n\
secondary tolerance) to another same-feature-type area feature perimeter segment on the opposite side of the parallel and the\n\
two connected features have different attributes or the same attributes with different values. See the GAIT Inspections\n\
document entry for this inspection to determine the attributes considered.";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

    case AUNM_ATTR_A:
       ConditionLookup[i].name = "Area Geometry Match, Attribute Mismatch Across Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that does overlap (within primary tolerance distance\n\
in meters) a perimeter segment of another area feature as specified by the secondary participant and is within the \n\
secondary tolerance (2D distance in meters) to some perimeter segment of the tertiary participant area feature and\n\
the designated attribute(s) of the primary and secondary participants are not identical";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

    case LUNM_ATTR_A:
       ConditionLookup[i].name = "Line Geometry Match, Attribute Mismatch Across Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is line feature end node that is coincident with (within primary tolerance distance\n\
in meters) a node of another line feature as specified by the secondary participant and is within the \n\
secondary tolerance (2D distance in meters) to some perimeter segment of the tertiary participant area feature and\n\
the designated attribute(s) of the primary and secondary participants are not identical";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


    case LE_A_UNM_LAT: /** line end node not coincident with area node at latitude parallel **/
       ConditionLookup[i].name = "Line End Node Not Matched To Area Node Across Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is close to or on (within the third tolerance distance)\n\
a latitude parallel that is a multiple of the second tolerance, is not connected\n\
(within the distance specified by the primary tolerance) to an area feature node as specified by\n\
the secondary participant feature type, and the primary participant is on the opposite side of a whole-degree\n\
latitude parallel in relation to the secondary participant";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "degrees";
       ConditionLookup[i].units3 = "meters";
       break;
    case LE_A_UNM_LON: /** line end node not coincident with area node at longitude meridian **/
       ConditionLookup[i].name = "Line End Node Not Matched To Area Node Across Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is close to or on (within the third tolerance distance)\n\
a longitude meridian that is a multiple of the second tolerance, is not connected\n\
(within the distance specified by the primary tolerance) to an area feature node as specified by\n\
the secondary participant feature type, and the primary participant is on the opposite side of a whole-degree\n\
longitude meridian in relation to the secondary participant";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "degrees";
       ConditionLookup[i].units3 = "meters";
       break;



    case LRNGE_UNM_LAT:
       ConditionLookup[i].name = "Line Pair Mismatched Across Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is close to or on (within the fourth tolerance distance)\n\
a latitude parallel that is a multiple of the third tolerance, is not connected\n\
(within the distance specified by the primary tolerance) to another line feature node as specified by\n\
the secondary participant feature type, is within the secondary tolerance distance to a line feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
latitude parallel in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";
       break;

    case LRNGE_UNM_LON:
       ConditionLookup[i].name = "Line Pair Mismatched Across Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is close to or on (within the fourth tolerance distance)\n\
a longitude meridian that is a multiple of the third tolerance, is not connected\n\
(within the distance specified by the primary tolerance) to another line feature node as specified by\n\
the secondary participant feature type, is within the secondary tolerance distance to a line feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
longitude meridian in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";
       break;

      case ARNGE_UNM_LON:
       ConditionLookup[i].name = "Area Perimeter Segment Pair Mismatched Across Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that is close to or on (within the fourth tolerance distance)\n\
a longitude meridian that is a multiple of the third tolerance, is not overlapping\n\
(within the distance specified by the primary tolerance) to another area feature perimeter segment as specified by\n\
the secondary participant feature type, is within the secondary tolerance distance to an area feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
longitude meridian in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";

       break;

      case ARNGE_UNM_LAT:
       ConditionLookup[i].name = "Area Perimeter Segment Pair Mismatched Across Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that is close to or on (within the fourth tolerance distance)\n\
a latitude parallel that is a multiple of the third tolerance, is not overlapping\n\
(within the distance specified by the primary tolerance) to another area feature perimeter segment as specified by\n\
the secondary participant feature type, is within the secondary tolerance distance to an area feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
latitude parallel in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";

       break;



    case LHANG_LAT:
       ConditionLookup[i].name = "Line Hanging At Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is within the fourth tolerance distance in meters to a\n\
latitude parallel that is a multiple of the third tolerance, is not connected\n\
(within the distance specified by the primary tolerance) to another line feature node as specified by\n\
the secondary participant feature type, is not within the secondary tolerance distance to a line feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
latitude parallel in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";
       break;

    case LHANG_LON:
       ConditionLookup[i].name = "Line Hanging At Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is a line feature end node that is within the fourth tolerance distance in meters to a\n\
longitude meridian that is a multiple of the third tolerance, is not connected\n\
(within the distance specified by the primary tolerance) to another line feature node as specified by\n\
the secondary participant feature type, is not within the secondary tolerance distance to a line feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
longitude meridian in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";
       break;

    case AHANG_LAT:
       ConditionLookup[i].name = "Area Hanging At Latitude Parallel";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that is within the fourth tolerance distance in meters to a\n\
latitude parallel that is a multiple of the third tolerance, is not overlapping\n\
(within the distance specified by the primary tolerance) to another area feature perimeter segment as specified by\n\
the secondary participant feature type, is not within the secondary tolerance distance to an area feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
latitude parallel in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";
       break;

    case AHANG_LON:
       ConditionLookup[i].name = "Area Hanging At Longitude Meridian";
       ConditionLookup[i].description = "\
The primary participant is an area feature perimeter segment that is within the fourth tolerance distance in meters to a\n\
longitude meridian that is a multiple of the third tolerance, is not overlapping\n\
(within the distance specified by the primary tolerance) to another area feature perimeter segment as specified by\n\
the secondary participant feature type, is not within the secondary tolerance distance to an area feature\n\
as specified by the tertiary participant, and the primary participant is on the opposite side of a whole-degree\n\
longitude meridian in relation to the secondary and tertiary participants";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "degrees";
       ConditionLookup[i].units4 = "meters";
       break;


      case LLMULTINT:
       ConditionLookup[i].name = "Intertwined Lines";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects a secondary participant\n\
line feature multiple times and the number of such intersections is\n\
greater than or equal to the primary tolerance: for 3D features, intersections will not be counted when the\n\
intersecting feature's Z value difference at the point of intersection is between the second and third tolerance\n\
specifications (intended to exclude counting overpass/underpass constructions as intersections) - to preclude\n\
use of this range, specify a lower limit (second tolerance) greater than the upper limit (third tolerance)";

       ConditionLookup[i].units = "intersections";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case LOC_MULTINT: /** lines with no or compatible LOC values intersect each other multiple times **/
       ConditionLookup[i].name = "Intertwined Lines - Features Have Compatible LOC Values";
       ConditionLookup[i].description = "\
The primary and secondary participants are line features having equivalent or compatible values specified as\n\
their LOC attribute, when assigned - The primary participant intersects the secondary participant\n\
multiple times and the number of such intersections is\n\
greater than or equal to the primary tolerance: for 3D features, intersections will not be counted when the\n\
intersecting feature's Z value difference at the point of intersection is between the second and third tolerance\n\
specifications (intended to exclude counting overpass/underpass constructions as intersections) - to preclude\n\
use of this range, specify a lower limit (second tolerance) greater than the upper limit (third tolerance)";
       ConditionLookup[i].units = "intersections";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;
  
  
      case L2D_L3D_MATCH:
       ConditionLookup[i].name = "2D Line End - 3D Line End Match";
       ConditionLookup[i].description = "\
The primary participant is an end node of a line feature that\n\
connects in 2D to an end node of a secondary participant line feature and these two line features\n\
have different dimensionality (one line feature is 3D and the other line feature is 2D)";
       break;
                                                                                                                           
      case LEZ_PROX_3D:
       ConditionLookup[i].name = "3D Line End - 3D Line End Z Mismatch";
       ConditionLookup[i].description = "\
The primary participant is an end node of a 3D line feature that\n\
connects in 2D to the end node of the secondary participant, another 3D line feature,\n\
but the two end node Z values differ by an amount greater than the primary tolerance and \n\
the difference is outside the closed range defined by the second and third tolerances (to make the\n\
range inactive, set the upper bound less than or equal to the lower bound)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case CNODE_ZBUST:  /*** Z mismatch between any two connecting nodes (in x,y) ***/
       ConditionLookup[i].name = "3D Feature Connected Node - 3D Feature Connected Node Z Mismatch";
       ConditionLookup[i].description = "\
The primary participant is any node of a 3D point, line, or area feature that\n\
connects in 2D (less than the primary tolerance distance apart) to any node of a secondary participant,\n\
another 3D point, line, or area feature, but the two nodes have Z values that differ by an amount greater than \n\
or equal to the secondary tolerance - such conditions will not be reported when a tertiary participant feature is\n\
also present at the connecting node location or when the Z difference is between the third and fourth tolerance\n\
specifications (intended to exclude reporting overpass/underpass constructions) - to preclude use of this range,\n\
specify a lower limit (third tolerance) greater than the upper limit (fourth tolerance)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "meters";
       break;

      case DUPLICATESEG:
       ConditionLookup[i].name = "Duplicated Line Segments";
       ConditionLookup[i].description = "\
The primary participant is a line feature that\n\
duplicates line segments (inter-vertex portions) with a secondary participant, another line\n\
feature, more than the number of times indicated by\n\
the primary tolerance value";
       ConditionLookup[i].units = "duplicated segments";
       break;


      case SHAREPERIM:
       ConditionLookup[i].name = "Shared Perimeter Segments";
       ConditionLookup[i].description = "\
The primary participant is an area feature that shares perimeter segments (inter-vertex portions)\n\
with a secondary participant, another area feature, more than the number of times indicated by\n\
the primary tolerance value - two area features are shared when the vertices defining the endpoints\n\
of the segment are exactly equal or within the secondary tolerance of being exactly equal";
       ConditionLookup[i].units = "shared segments";
       ConditionLookup[i].units2 = "meters";
       break;

      case LVPROX:
       ConditionLookup[i].name = "Line Vertex - Line Segment Proximity";
       ConditionLookup[i].description =  "\
The primary participant is a line feature with a vertex having 3D\n\
distance to any segment of the secondary participant, another line feature,\n\
that is greater than or equal to the primary tolerance and less than the secondary\n\
tolerance; for 2D features, the 2D distance will be compared to the tolerance specifications";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case PLPROX:
       ConditionLookup[i].name = "Point To Line Proximity";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having 2D distance to\n\
any segment of any secondary participant line feature\n\
that is less than the distance defined by the primary tolerance specification; \n\
and the primary participant is not coincident with a tertiary participant line feature; for 3D features, the\n\
2D distance will still be compared to the tolerance";
       ConditionLookup[i].units = "meters";
       break;

      case PSHOOTL: /*** point feature over or undershoots a line feature ***/
       ConditionLookup[i].name = "Point - Line Undershoot Or Overshoot";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having closest 2D distance to\n\
any segment of a secondary participant line feature\n\
that is within the range (closed interval) defined by the primary and secondary tolerance specifications; for 3D features, the\n\
2D distance will still be compared to the tolerance.  The secondary participant closest to the primary participant will\n\
always be used to evaluate this condition";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


      case PLPROXEX:  /** pt to line prox with exception for line end node ***/
       ConditionLookup[i].name = "Point To Line Proximity With End Node Exception";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having 2D distance to any\n\
part of the secondary participant line feature that is less\n\
than or equal to the specified tolerance, except those cases where the closest part of the line feature\n\
is a line feature end node";
       ConditionLookup[i].units = "meters";
       break;


      case PTPTPROX:
       ConditionLookup[i].name = "Point To Point Proximity";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having 3D distance to\n\
a secondary participant point feature that is\n\
less than or equal to the specified tolerance; for 2D features, the\n\
2D distance will be compared to the tolerance";
       ConditionLookup[i].units = "meters";
       break;

      case PUNDERSHTA:
       ConditionLookup[i].name = "Point - Area Perimeter Undershoot";
       ConditionLookup[i].description = "\
The primary participant is a point feature that is within the specified\n\
tolerance (2D distance) to some part (but not coincident with) of a secondary participant area feature\n\
perimeter and the point feature is outside the area feature perimeter in 2 dimensions";
       ConditionLookup[i].units = "meters";
       break;
  
      case POVERSHTA:
       ConditionLookup[i].name = "Point - Area Perimeter Overshoot";
       ConditionLookup[i].description = "\
The primary participant is a point feature that is within the specified\n\
tolerance (2D distance) to some part (but not coincident with) of a secondary participant area feature\n\
perimeter and the point feature is inside the area feature perimeter in 2 dimensions";
       ConditionLookup[i].units = "meters";
       break;

      case PLPFAIL:
       ConditionLookup[i].name = "Point To Feature Coincidence Failure";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having minimum 2D distance\n\
to any portion of a secondary participant point, area, or line feature\n\
that is greater than the specified threshold (a point inside an area\n\
feature is considered to be 0 meters distant from that area feature)";
       ConditionLookup[i].units = "meters";
       break;

      case PNOCOVERLE:
       ConditionLookup[i].name = "Point Not Covered By Line End Node Or Area Perimeter";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having minimum 3D distance\n\
to an end node of a secondary participant line feature or minimum 2D distance to the\n\
perimeter of a secondary participant area feature\n\
that is greater than the specified threshold; for 2D line features, the 2D\n\
distance will be compared to the tolerance";
       ConditionLookup[i].units = "meters";
       break;


      case PNOCOV2LEA: /** point not covered by 2 line terminal nodes or area edges***/
       ConditionLookup[i].name = "Point Not Covered By 2 Line End Nodes Or 2 Area Edges";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having minimum 3D distance\n\
to end nodes (line) or edges (area) of two different secondary participant features\n\
that is greater than or equal to the specified threshold; for 2D features, the 2D\n\
distance will be compared to the tolerance";
       ConditionLookup[i].units = "meters";
       break;

      case PNOCOVERLV:
       ConditionLookup[i].name = "Point Not Covered By Line Vertex";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having minimum 3D distance\n\
to any vertex of a secondary participant line feature\n\
that is greater than or equal to the specified threshold; for 2D features, the 2D\n\
distance will be compared to the tolerance";
       ConditionLookup[i].units = "meters";
       break;

      case PLLPROXFAIL:
       ConditionLookup[i].name = "Point - Feature Intersection Coincidence Failure";
       ConditionLookup[i].description =  "\
The primary participant is a point feature having minimum 3D distance\n\
to the intersection of the secondary and tertiary participant line or area features\n\
where that distance is greater than the specified threshold; for 2D features, the 2D\n\
distance will be compared to the tolerance";
       ConditionLookup[i].units = "meters";
       break;
  
      case CONNECTFAIL:
       ConditionLookup[i].name = "Feature Connectivity Failure";
       ConditionLookup[i].description =  "\
The primary participant is a point, line, or area feature that does not have any vertex (area\n\
features), both end nodes (line feature) or its location (point features) within the specified distance in meters to any\n\
vertex of a secondary participant  point, line or area feature";
       ConditionLookup[i].units = "meters";
       break;


      case FEATOUTSIDE:  /*** a feature lies at least partly outside the MGCP cell ***/
       ConditionLookup[i].name = "Feature Coordinate Outside Whole-Degree Project Boundaries";
       ConditionLookup[i].description =  "\
The primary participant is a vertex of a point, line, or area feature that is located\n\
more than tolerance distance outside the extreme whole-degree boundaries of the project";
       ConditionLookup[i].units = "meters";
       break;

      case ENCONFAIL: /** end node connectivity failure **/
       ConditionLookup[i].name = "Line End Node Connectivity Failure";
       ConditionLookup[i].description =  "\
The primary participant is a line feature end node that is not\n\
within the primary tolerance distance in meters to an end node of a secondary participant line feature";
       ConditionLookup[i].units = "meters";
       break;

      case NOENDCON: /** both end nodes of a line fail to connect or be covered **/
       ConditionLookup[i].name = "Both Line End Nodes Unconnected";
       ConditionLookup[i].description =  "\
The primary participant is a line feature that does not intersect a secondary participant point \n\
or line feature, does not intersect or have any part inside of a secondary participant area feature\n\
and neither endpoint of the primary participant line feature is within the primary\n\
tolerance distance (in meters) to any part of a tertiary participant\n\
point or line feature or to any part of an area feature perimeter";
       ConditionLookup[i].units = "meters";
       break;

      case BOTHENDCON: /** both end nodes of a line feature are covered by specified-type point features **/
       ConditionLookup[i].name = "Both Line End Nodes Covered By Point Feature";
       ConditionLookup[i].description =  "\
The primary participant is a line feature that has secondary participant point features \n\
coincident (within the tolerance distance) with both of its end nodes.  The tolerance is\n\
measured in either 2 or 3 dimensions, as appropriate for the primary and secondary features";
       ConditionLookup[i].units = "meters";
       break;



      case LENOCOVERL:  /*** line end node not within tolerance distance to another line ***/
       ConditionLookup[i].name = "Line End Node Not Covered By Another Line";
       ConditionLookup[i].description =  "\
The primary participant is a line feature end node that is not\n\
within the primary tolerance distance in meters to any segment of a secondary participant line feature\n\
(Note the primary and secondary lines must be different line features) or to an interior node of a tertiary\n\
participant line segment";
       ConditionLookup[i].units = "meters";
       break;

      case NOLCOVLE: /**line end node not within tolerance distance to another line, including itself ***/
       ConditionLookup[i].name = "Line End Node Not Covered By Any Line";
       ConditionLookup[i].description =  "\
The primary participant is a line feature end node that is not\n\
within the primary tolerance distance in meters to any segment of a secondary participant line feature\n\
(Note the primary and secondary lines could be exactly the same individual line feature)";
       ConditionLookup[i].units = "meters";
       break;

      case FEATNOTCUT:  /** feature not cut at end node of second feature ***/
       ConditionLookup[i].name = "Line Not Cut At Coincidence With Another Line End Node";
       ConditionLookup[i].description =  "\
The primary participant is a line feature that overlaps (at least\n\
partially) a secondary participant line feature and does not have an end node within tolerance\n\
distance (in 2D) to an end node of the secondary participant, and the secondary participant line feature\n\
end node is not also connected to another secondary participant line feature end node";
       ConditionLookup[i].units = "meters";
       break;
  
      case ATTRERR:
       ConditionLookup[i].name = "Attribution Schema Error";
       ConditionLookup[i].description = "\
The primary participant is a feature that has 1 or more incorrectly assigned or missing attributes";
       break;

      case ATTR_PAIR:
       ConditionLookup[i].name = "Unexpected FCODE - Geometry Pair";
       ConditionLookup[i].description = "\
The primary participant is assigned an FCODE and geometry (e.g., point, line, or\n\
area) that have not been prescribed for use together\n\
(per the schema used to conduct the attribute analysis)";
       break;

     case ATTR_UNEXP:
       ConditionLookup[i].name = "Unexpected Attribute";
       ConditionLookup[i].description = "\
The primary participant is assigned an FCODE and attribute\n\
that have not been prescribed for use together\n\
(per the schema used to conduct the attribute analysis)";
       break;


     case RPTD_ATTR: /** attribute error as reported  ****/
       ConditionLookup[i].name = "Reported Attribute Error Matches Annotation";
       ConditionLookup[i].description = "\
The primary participant has been reported as an \"Unexpected FCODE-Geometry Pair\", \"Unexpected Attribute\",\n\
\"MGCP Required Attribute Assigned Null Value\", \"Value Range\",\n\
\"Domain Specific Pick-list\", \"Attribute Dependency\", or \"Missing Attribute Field Or Value\" error during the\n\
attribute inspection and the short description of the exact error corresponds to the check annotation";
       break;

     case ATTR_RNULL:
       ConditionLookup[i].name = "Required MGCP Attribute Has Null Value";
       ConditionLookup[i].description = "\
The primary participant is a feature assigned an attribute that the MGCP has designated as \n\
REQUIRED and that attribute has been assigned a NULL value - note these violations will only be\n\
identified for projects created using the MGCP schema";
       break;

      case ATTR_VVT:
       ConditionLookup[i].name = "Attribute Dependency Violation";
       ConditionLookup[i].description = "\
The primary participant is a single feature assigned two attributes and values that conflict with each \n\
other";
       break;
  

      case ATTR_MISSING:
       ConditionLookup[i].name = "Missing Attribute Field Or Value";
       ConditionLookup[i].description = "\
The primary participant, of the specified geometry, is assigned an FCODE\n\
that has been required to have an attribute assigned and that\n\
attribute is missing (per the schema used to conduct the attribute analysis)";
       break;

      case ATTR_DT:
       ConditionLookup[i].name = "Data Type";
       ConditionLookup[i].description = "\
The primary participant is assigned an FCODE, attribute, and attribute value \n\
and the data type (e.g., integer, float, etc.) of the assigned attribute value differs\n\
from the type prescribed for use (per the schema used to conduct the attribute analysis)";
       break;

      case ATTR_RNG:
       ConditionLookup[i].name = "Value Range";
       ConditionLookup[i].description = "\
The primary participant is assigned an FCODE, numeric-type attribute, and attribute\n\
value and the assigned value is outside (above or below) the range of allowed values\n\
prescribed for use (per the schema used to conduct the attribute analysis)";
       break;

      case ATTR_PICK:
       ConditionLookup[i].name = "Domain-Specific Pick List";
       ConditionLookup[i].description = "\
The primary participant is assigned an FCODE, pick list attribute, and attribute value \n\
and the assigned pick list value is not a member of the set of allowed values\n\
prescribed for use (per the schema used to conduct the attribute analysis)";
       break;

      case ATTR_META:
       ConditionLookup[i].name = "Metadata Error";
       ConditionLookup[i].description = "\
The primary participant is not assigned metadata attributes or attribute values as described in\n\
the associated specification for metadata requirements\n\
(as specified during creation of the GAIT project)";
       break;


      case VVTERR1WAY: /** feature with designated attribute & value ***/
       ConditionLookup[i].name = "Single Feature Meeting Specific Specification";
       ConditionLookup[i].description = "\
This inspection identifies individual features that meet the primary participant specification";
       break;


      case VVTERR2WAY:
       ConditionLookup[i].name = "Single Feature Meeting 2 Specific Specifications";
       ConditionLookup[i].description = "\
This inspection identifies individual (single) features that meet both primary and secondary participant specifications";
  /***     ConditionLookup[VVTERR2WAY].checkapplies = (char) SEEITcheck; ***/
       break;


      case VVTERR3WAY:
       ConditionLookup[i].name = "Single Feature Meeting 3 Specific Specifications";
       ConditionLookup[i].description = "\
This inspection identifies individual features that meet all primary, secondary, and tertiary participant specifications";
       break;
  
  
      case V_DUPS:
       ConditionLookup[i].name = "Duplicate Vertices";
       if(NGA_TYPE > 0)
          {
          ConditionLookup[i].description = "\
The primary participant is an area or line feature that includes\n\
consecutive vertices with the same (x,y,z) coordinate values; for 2D features, the\n\
comparison will not consider z coordinate values";
          }
       else
          {
          ConditionLookup[i].description = "\
The primary participant is a polygon, area or line feature that includes\n\
consecutive vertices with the same (x,y,z) coordinate values; for 2D features, the\n\
comparison will not consider z coordinate values";
          }
       ConditionLookup[i].units = "duplicates";
       break;

  

      case ISOTURN: /** high turn angle w/o 3d feature present ***/
       ConditionLookup[i].name = "Isolated Feature Direction Change";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes consecutive inter-vertex portions\n\
(segments) forming an angle greater than or equal to the primary tolerance or when one\n\
feature end node covers (is within secondary tolerance distance to) a secondary participant line\n\
feature vertex and the joined segments of the two features form such an angle.  If there is a tertiary\n\
participant within the secondary tolerance distance to the heading change location, no condition exists";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       break;

  
      case KINK:
       ConditionLookup[i].name = "Line Kink";
       if(NGA_TYPE == 0)
         {
           ConditionLookup[i].description = "\
Adjacent segments of a line feature form an angle greater than specified coincident with\n\
a specified polygon";
         }
       else
         {
           ConditionLookup[i].description = "\
The primary participant is a line feature that includes consecutive inter-vertex portions\n\
(segments) forming an angle greater than or equal to the specified tolerance or when one\n\
feature end node covers another primary participant line feature vertex and the\n\
joined segments of the two features form such an angle";
         }
       ConditionLookup[i].units = "degrees";
       break;
  

      case Z_KINK:
       ConditionLookup[i].name = "'Z' Shaped Line Kink";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes 3 consecutive inter-vertex portions\n\
(segments) including 2 angles that are within the range denoted by the primary and secondary tolerances,\n\
or when one feature end node covers another primary participant line feature vertex and the\n\
joined segments of the two features together form such consecutive angles";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "degrees";
       break;


      case L_A_KINK:
       ConditionLookup[i].name = "Line - Area Kink";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes an end node\n\
that is coincident with the perimeter of a secondary participant area feature\n\
and one of the two angles formed by the line and area feature coincidence, as\n\
measured about the line feature end node, is greater than or equal to the specified tolerance";
       ConditionLookup[i].units = "degrees";
       break;


      case INTERNALKINK:
       ConditionLookup[i].name = "Line Kink (Single Feature)";
       if(NGA_TYPE == 0)
         {
           ConditionLookup[i].description = "\
Adjacent inter-vertex portions (segments) of a line feature form an angle greater than specified coincident with\n\
a specified polygon";
         }
       else
         {
           ConditionLookup[i].description = "\
The primary participant is a line feature that includes consecutive inter-vertex portions\n\
(segments) forming an angle that is within the range denoted by the primary and secondary\n\
tolerances\n";
         }
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "degrees";
       break;

      case CONTEXT_KINK:
       ConditionLookup[i].name = "Line Feature Join Kink";
       if(NGA_TYPE == 0)
         {
           ConditionLookup[i].description = "\
Adjacent segments of two connected line features form an angle within the range defined by\n\
the primary and secondary tolerances - an immediately adjacent angle is also >= 45 degrees";
         }
       else
         {
           ConditionLookup[i].description = "\
The primary participant is a line feature that includes one end\n\
node that is covered by a connecting node of a line feature of the same feature type, the angle\n\
formed at the point the features join is in the interval defined by the primary and secondary\n\
tolerances, and the preceding or following angle is greater than 45 degrees";
         }
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "degrees";
       break;

  

      case KICKBACK:
       ConditionLookup[i].name = "Line Kickback";
       if(NGA_TYPE == 0)
         {
           ConditionLookup[i].description = "\
Adjacent segments of a line feature form an angle greater than specified coincident with\n\
a specified polygon";
         }
       else
         {
           ConditionLookup[i].description = "\
The primary participant is a line feature that includes consecutive inter-vertex portions\n\
(segments that overlap each other - this inspection does not detect kickbacks caused by different\n\
features that have overlapping segments (use the Shared Segment inspection)";
         }
       break;


      case AREAKINK:
       ConditionLookup[i].name = "Area Kink Or Kickback";
       ConditionLookup[i].description = "\
The primary participant is an area feature that includes consecutive perimeter inter-vertex\n\
portions (perimeter segments) that form an angle greater than or equal to the specified tolerance";
       ConditionLookup[i].units = "degrees";
       break;


      case INCLSLIVER:
       ConditionLookup[i].name = "Area Perimeter Sliver";
       ConditionLookup[i].description = "\
The primary participant is an area feature whose inter-vertex perimeter portions (perimeter segments)\n\
include a subset of contiguous segments that, if closed themselves, would form a sliver\n\
(2D area /perimeter) less than the ratio indicated by the primary tolerance";
       ConditionLookup[i].units = "ratio";
       break;

      case LONGSEG: /** linear or areal perimeter segment with length at or above threshold ***/
       ConditionLookup[i].name = "Long Length Between Feature Vertices";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature having inter-vertex portions\n\
(segments) on its length or perimeter (respectively) where the segment length is greater than or\n\
equal to the specified tolerance; if the feature is a 3D feature, the measurement will be made\n\
in 3D, otherwise the measurement will be made in 2D";
       ConditionLookup[i].units = "meters";
       break;


      case SEGLEN:
       ConditionLookup[i].name = "Short Length Between Feature Vertices";
       if(NGA_TYPE==1)
         {
          ConditionLookup[i].description = "\
The primary participant is a line or area feature having inter-vertex portions\n\
(segments) on its length or perimeter (respectively) where the segment length is less than or\n\
equal to the primary tolerance; if the feature is a 3D feature, the measurement will be made\n\
in 3D, otherwise the measurement will be made in 2D";
          }
       else
         {
          ConditionLookup[i].description = "\
The primary participant is a polygon, line or area feature (of the specified type) having segments\n\
on its length or perimeter (respectively) where the segment length is less than or equal to\n\
the specified tolerance; if the feature is a 3D feature, the measurement will be made in 3D,\n\
otherwise the measurement will be made in 2D";
          }
       ConditionLookup[i].units = "meters";
       break;


      case FEATBRIDGE:
       ConditionLookup[i].name = "Illogical Connecting Feature";
          ConditionLookup[i].description = "\
The primary participant is a line feature that has total 2D length\n\
less than or equal to the primary tolerance (in meters) and that serves as the only\n\
connecting feature between line features as described by the secondary and tertiary\n\
participant specifications";
       ConditionLookup[i].units = "meters";
       break;


      case SMALLAREA:
       ConditionLookup[i].name = "Small-Area Area Feature";
       ConditionLookup[i].description = "\
The primary participant is an area feature having a total (GAIT-calculated) square\n\
area that is less than the area specified as the primary tolerance.  The area will\n\
be calculated in 2 dimensions for the outer ring of all area features and will not\n\
subtract the included area of the feature's inner rings";
       ConditionLookup[i].units = "square meters";
       break;
      
      case SMLCUTOUT: /** small included area inner ring of area feature ***/
       ConditionLookup[i].name = "Small-Area Area Feature Cut-out";
       ConditionLookup[i].description = "\
The primary participant is an area feature inner ring (cut-out) having a GAIT-calculated\n\
square area that is less than the area specified as the primary tolerance.  The area will\n\
be calculated in 2 dimensions for all specified area features inner rings (cut-outs) and\n\
will not consider the area of the parent feature's outer ring";
       ConditionLookup[i].units = "square meters";
       break;

      case OSIDE_LAT:   /**** feature coordinate above or below latitude range    **/
       ConditionLookup[i].name = "Feature Coordinate Outside Specified Latitude Range";
       ConditionLookup[i].description =  "\
The primary participant is a vertex of a point, line, or area feature that is located\n\
outside the decimal-degree upper and lower latitude value range defined by the primary and\n\
secondary tolerances by an amount greater than the third tolerance in meters";
       ConditionLookup[i].units = "decimal degrees";
       ConditionLookup[i].units2 = "decimal degrees";
       ConditionLookup[i].units3 = "meters";
       break;

      case OSIDE_LON:   /**** feature coordinate above or below longitude range    **/
       ConditionLookup[i].name = "Feature Coordinate Outside Specified Longitude Range";
       ConditionLookup[i].description =  "\
The primary participant is a vertex of a point, line, or area feature that is located\n\
outside the decimal-degree upper and lower longitude value range defined by the primary and\n\
secondary tolerances by an amount greater than the third tolerance in meters";
       ConditionLookup[i].units = "decimal degrees";
       ConditionLookup[i].units2 = "decimal degrees";
       ConditionLookup[i].units3 = "meters";
       break;

      case LBNDUSHT:  /** unconnected line end node undershoots whole-degree boundary ***/
       ConditionLookup[i].name = "Line Feature End Node Undershoots Project Whole-Degree Boundary";
       ConditionLookup[i].description =  "\
The primary participant is a line feature end node that is located\n\
close to but not on (within the open interval of distance range defined by the primary and\n\
secondary tolerance values) one of the whole-degree project boundaries and the line feature end\n\
node is not connected (within the primary tolerance distance) to any part of a secondary participant";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


      case BNDRYUNDERSHT: /** feature undershoots whole degree project outside boundary ***/
       ConditionLookup[i].name = "Feature Coordinate Undershoots Project Whole-Degree Boundary";
       ConditionLookup[i].description =  "\
The primary participant is a vertex of a point, line, or area feature that is located\n\
close to but not on (within the open interval of distance range defined by the primary and\n\
secondary tolerance values) one of the whole-degree project boundaries";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


  
  
      case BIGAREA:
       ConditionLookup[i].name = "Large-Area Area Feature";
       ConditionLookup[i].description = "\
The primary participant is an area feature having a total square\n\
area that is greater than the area specified as the primary tolerance.  The area will\n\
be calculated in 2 dimensions for all area features";
       ConditionLookup[i].units = "square meters";
       break;

      case CLAMP_NFLAT: /** area feature does not have constant elevation when clamped to underlying DEM ***/
       ConditionLookup[i].name = "Non-uniform Values In Grid Under Area Feature Perimeter Nodes";
       ConditionLookup[i].description = "\
The primary participant is an area feature that has a difference between its minimum and maximum\n\
vertex elevation values greater than the primary tolerance, in meters when the value at each\n\
vertex has been assigned (interpolated) from the posts in a corresponding grid (e.g., the area\n\
feature perimeter has been 'clamped' to the surface of a grid at its location and it not flat)";
       ConditionLookup[i].units = "meters";
       break;

      case NOT_FLAT:  /*** area feature with surface that is not uiform elevation ***/
       ConditionLookup[i].name = "Area Feature With Non-Uniform Surface Elevation";
       ConditionLookup[i].description = "\
The primary participant is a 3D area feature having minimum and\n\
maximum elevation values that differ by more than the specified threshold values, an\n\
elevation difference expressed in meters";
       ConditionLookup[i].units = "meters";
       break;

      case CLAMP_DIF: /** difference between feature vertex z value and interpolated DEM value ***/
       ConditionLookup[i].name = "Difference Between Feature Z Value And Corresponding DEM Value";
       ConditionLookup[i].description = "\
The primary participant is a 3D point, line, or area feature vertex having a Z value\n\
different from the corresponding DEM value by more than the primary tolerance.  Feature\n\
vertex Z values less than the secondary tolerance will be ignored.  DEM values less than the\n\
tertiary tolerance will be ignored. Feature vertices that do not overlap a DEM will not be reported.\n\
Condition magnitude is the absolute value of the vertex elevation and interpolated DEM value.  If\n\
the 4th tolerance is '0', the inspection only applies to feature vertices; otherwise it applies at\n\
features vertices and at DEM grid intersections with each feature segment";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "Boolean";
       break;


      case ZUNCLOSED:
       ConditionLookup[i].name = "Area Not Closed In Z";
       ConditionLookup[i].description = "\
The primary participant is an area feature having first and last\n\
vertices that are different in the Z dimension and the difference is greater than that\n\
specified as the primary tolerance";
       ConditionLookup[i].units = "meters";
       break;
  
  
      case AREAUNCLOSED:
       ConditionLookup[i].name = "Unclosed Area";
       ConditionLookup[i].description = "\
The primary participant is an area feature having first and last\n\
vertices that are different and the distance (3D) between them is greater than that\n\
specified as the primary tolerance";
       ConditionLookup[i].units = "meters";
       break;


     case MULTIPARTL:
       ConditionLookup[i].name = "Part Of Multi-part Line";
       ConditionLookup[i].description = "\
The primary participant is a line feature that is a single\n\
feature that is composed of physically separate parts and the number of parts is\n\
greater than the specified tolerance";
       ConditionLookup[i].units = "parts";
       break;


      case MULTIPARTA:
       ConditionLookup[i].name = "Part Of Multi-part Area";
       ConditionLookup[i].description = "\
The primary participant is an area feature that is a single\n\
feature that is composed of physically separate parts and the number of parts is\n\
greater than the specified tolerance";
       ConditionLookup[i].units = "parts";
       break;


      case MULTIPARTP:
       ConditionLookup[i].name = "Part Of Multi-part Point";
       ConditionLookup[i].description = "\
The primary participant is a point feature that is a single\n\
feature that is composed of physically separate parts and the number of parts is\n\
greater than the specified tolerance";
       ConditionLookup[i].units = "parts";
       break;


      case CALC_AREA:  /*** point feature with LEN and WID attr values product < tolerance ***/
       ConditionLookup[i].name = "Calculated Area Above Threshold";
       ConditionLookup[i].description = "\
The primary participant is a point feature that has applicable Length and Width\n\
attributes (with valid, non-sentinel values) and the product of these two attribute values (the square\n\
area) is greater than the specified tolerance";
       ConditionLookup[i].units = "square meters";
       break;


      case LONGFEAT:   /** line or area feature with total length above threshold ***/
       ConditionLookup[i].name = "Total Feature Length Or Perimeter Above Threshold";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature that has total calculated\n\
length or perimeter (length) greater than the primary tolerance.\n\
Feature length with be calculated in 2D or 3D, as appropriate for\n\
the primary participant";
       ConditionLookup[i].units = "meters";
       break;


      case PC_SLOPE: /*** line feature segment with percent slope above tolerance ****/
       ConditionLookup[i].name = "Line Feature Segment Percent Slope Above Threshold";
       ConditionLookup[i].description = "\
The primary participant is an inter-vertex portion (segment) of a 3D line feature that has calculated\n\
percent slope (e.g., (rise / run) * 100) greater than or equal to the specified threshold - only segments\n\
having segment length greater than the secondary tolerance (in meters) will be inspected";
       ConditionLookup[i].units = "rise / run";
       ConditionLookup[i].units2 = "meters";
       break;


      case SHORTFEAT:  /** short length line feature not on quarter degree 'boundary' ***/
       ConditionLookup[i].name = "Total Length Below Threshold With Coordinate Exception";
       ConditionLookup[i].description = "\
The primary participant is a line feature that has total calculated\n\
length less than the primary tolerance and does not have an end node on\n\
or within the secondary tolerance distance to a 1/4 degree line\n\
of latitude or longitude - feature length with be calculated in 2D or 3D, as appropriate for\n\
the primary participant";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;



      case PERIMLEN:
       ConditionLookup[i].name = "Short Length Feature";
       if(NGA_TYPE == 1)
          {
       ConditionLookup[i].description =  "\
The primary participant is a line or area feature having total\n\
length or perimeter (respectively) that is less than or equal to the specified tolerance:\n\
the measurement will be made in 2D for 2D features and 3D for 3D features. Short length line\n\
features will not be reported when an end node of the feature is coincident (within tolerance)\n\
with the perimeter of a secondary participant area feature or with the extent of a secondary\n\
participant line feature (without overlap) or coincident with a secondary participant point feature";
          }
       else
          {
       ConditionLookup[i].description =  "\
The primary participant is a polygon, line or area feature (of the specified type) having total\n\
length or perimeter (respectively) that is less than or equal to the specified tolerance;\n\
the measurement will be made in 2D for all features";
          }
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;
  
  
      case COVERFAIL:
       ConditionLookup[i].name = "100% Area Coverage Failure";
       ConditionLookup[i].description = "\
A gap or overlap exists in the space covered by the set of area features specified\n\
as the primary participant(s) - feature edges closer than the secondary tolerance to\n\
multiples of the primary tolerance latitude or longitude will not be reported as conditions";
       ConditionLookup[i].units = "decimal degrees";
       ConditionLookup[i].units2 = "meters";
       break;
  

      case LAINT:
       ConditionLookup[i].name = "Line - Area Intersection";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects (in 2D)\n\
the perimeter of a secondary participant area feature";
       break;


      case CLAMP_SEG: /*** catenary segment below associated DEM ****/
       ConditionLookup[i].name = "Interpolated Inter-vertex Elevation Below Elevation Grid Offset";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature having inter-vertex (catenary) segments that\n\
have interpolated elevation values less than the required offset (primary tolerance) above an underlying elevation\n\
grid";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case GRIDEXACTDIF: /** Grids have post value difference at same X,Y ***/
        ConditionLookup[i].name = "Grid Post Value Difference At Same Coordinate";
        ConditionLookup[i].description = "\
The participants are two grids where post values differ between them\n\
at exactly the same location by more than the specified primary tolerance - \n\
Note that post values equal to tolerance 2 or tolerance 3 will be ignored and that\n\
post values will only be compared when the two posts have the same latitude and longitude (i.e., \n\
post values will not be interpolated values)";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";

       break;

  
  
      case LODELEVDIF:
       if(NGA_TYPE==1)
         {
           ConditionLookup[i].name = "Grid Post Value Difference";
           ConditionLookup[i].description = "\
The participants are two grids where post values differ between them\n\
at the same location by more than the specified primary tolerance - \n\
Note that post values equal to tolerance 2 or tolerance 3 will be ignored and that\n\
post values may be interpolated (using bi-linear method from 4 nearest neighbors) when necessary";
         }
       else
         {
           ConditionLookup[i].name = "Surface Elevation Difference By LOD";
           ConditionLookup[i].description = "\
Surface elevations vary between overlapping grids or polygons in different LODs and the difference is greater\n\
than the specified tolerance";
         }
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";

       break;


     case MASKZERO: /** DEM not zero elev at point defined by specified mask value ***/
       if(NGA_TYPE==1)
         {
           ConditionLookup[i].name = "Grid Post Value Not Zero Under Mask";
           ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is a grid;\n\
both the mask and the grid have exactly the same\n\
post spacing and offsets - the mask has value equal to the primary tolerance value\n\
(plus or minus the secondary tolerance) and the grid has non-zero post value\n\
less than zero minus the secondary tolerance or greater than the secondary tolerance - \n\
the tertiary tolerance controls the condition representation; when set to\n\
0, all conditions are written as points, when set to 1, convex polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
         }
       else
         {
           ConditionLookup[i].name = "Grid Elevation Value Not Zero Under Mask";
           ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is an elevation grid;\n\
both the mask and the elevation grid have exactly the same\n\
post spacing and offsets - the mask has value equal to the primary tolerance value\n\
(plus or minus the secondary tolerance) and the DEM has non-zero elevation value\n\
less than zero minus the secondary tolerance or greater than the secondary tolerance";
         }
       ConditionLookup[i].units = "sentinel value";
       ConditionLookup[i].units2 = "sentinel offset";
       ConditionLookup[i].units3 = "none"; 
       break;

      
      case MASKCONFLICT: /** Grid DEM Masks have conflicting values ***/
        ConditionLookup[i].name = "Grid Value Conflict - Out Of Range";
        ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is either an elevation grid\n\
or a raster mask and both the primary and secondary participant have exactly the same post\n\
spacing and offsets - the primary participant mask value is in the closed interval defined by the\n\
primary and secondary tolerance specifications and the corresponding secondary participant raster value\n\
is outside the closed interval defined by the third and fourth tolerance specifications - (example,\n\
the EDM has value 8 ('Ocean') and the WBM does not have value 1 ('Ocean')) - \n\
the fifth tolerance controls the condition representation; when set to\n\
0, all conditions are written as points, when set to 1, convex polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";

       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "post value";
       ConditionLookup[i].units5 = "none";
       break;

      case MASKCONF2: /** variation of Grids with conflicting values **/
        ConditionLookup[i].name = "Grid Value Conflict - Within Range";
        ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is either an elevation grid\n\
or a raster mask and both the primary and secondary participant have exactly the same post\n\
spacing and offsets - the primary participant mask value is in the closed interval defined by the\n\
primary and secondary tolerance specifications and the corresponding secondary participant raster value\n\
is inside the closed interval defined by the third and fourth tolerance specifications - (example,\n\
the EDM has value 8 ('Ocean') and the WBM has the value 0 ('Not Water')) - \n\
the fifth tolerance controls the condition representation; when set to\n\
0, all conditions are written as points, when set to 1, convex polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";

       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "post value";
       ConditionLookup[i].units5 = "none";
       break;



      case MASKSHOREL: /** water body not contained by shoreline ***/
        ConditionLookup[i].name = "Grid Post Value Below Water At Shoreline";
        ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is a grid;\n\
both the mask and the grid have exactly the same post spacing and offsets - the\n\
mask post has value zero and is adjacent to a mask post having value greater than or equal to the\n\
primary tolerance value and less than or equal to the secondary tolerance and the grid\n\
value under the mask zero post is less than or equal to the grid value added to the tertiary\n\
tolerance and coincident with the mask post where the primary and secondary tolerances have been applied\n\
and satisfied (the grid post value on the shoreline, minus the tertiary tolerance, is less than\n\
or equal to the grid post value in water)";
         
       ConditionLookup[i].units = "sentinel value";
       ConditionLookup[i].units2 = "sentinel value";
       ConditionLookup[i].units3 = "post value";
       break;


      case MASKEDIT_0: /** Raw DEM and Edited DEM different & Edit Mask has value zero**/
        ConditionLookup[i].name = "Edit Mask: 'No Editing' - Grids Have Different Values";
        ConditionLookup[i].description = "\
The primary participant is a raster mask, the secondary and tertiary participants are a raw grid and an\n\
edited grid (in either order) and all three grids align exactly, having exactly the same post\n\
spacing and offsets - a condition is reported when the difference between the raw and edited grid values\n\
at the same post is greater than the tertiary tolerance and the edit mask has post value equal to the primary\n\
tolerance at that same location - the secondary tolerance controls the condition representation; when set to\n\
0, all conditions are written as points, when set to 1, convex polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
        ConditionLookup[i].units = "sentinel value";
        ConditionLookup[i].units2 = "none";
        ConditionLookup[i].units3 = "meters";
         break;

      case MASKEDIT_1: /** EDM has primary tolerance value, diff between TDR and TDF is > secondary tolerance **/
        ConditionLookup[i].name = "TDF and TDR Value Difference Above Maximum";
        ConditionLookup[i].description = "\
The primary participant is a raster mask (e.g., the EDM), the secondary and tertiary participants are a raw grid and an\n\
finished grid (in either order) and all three grids align exactly, having exactly the same post\n\
spacing and offsets - a condition is reported when the primary participant post value is equal to the primary\n\
tolerance and the difference between the post values of the secondary and tertiary participant\n\
at the same post is greater than the secondary tolerance - conditions will not be reported when either the\n\
secondary or tertiary participant post values equal the third or fourth tolerance value - \n\
the fifth tolerance controls the condition representation; when set to\n\
0, all conditions are written as points, when set to 1, convex polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "post value";
       ConditionLookup[i].units5 = "none";
         break;


      case MASKCONSTANT: /*** DEM not constant elev at pointdefined by specified mask value ***/
       ConditionLookup[i].name = "Grid Post Value Not Constant Under Mask";
       if(NGA_TYPE==1)
         {
           ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is a grid;\n\
both the mask and the grid have exactly the same\n\
post spacing and offsets - the mask has value equal to the primary tolerance value\n\
(plus or minus the secondary tolerance) and the grid has constant post value\n\
(plus or minus the secondary tolerance) in relation to its neighbor posts also under the mask - \n\
the tertiary tolerance controls the condition representation; when set to\n\
0, all conditions are written as points, when set to 1, convex polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
         }
       else
         {
           ConditionLookup[i].description = "\
The primary participant is a raster mask and the secondary participant is an elevation grid;\n\
both the mask and the elevation grid have exactly the same\n\
post spacing and offsets - the mask has value equal to the primary tolerance value\n\
(plus or minus the secondary tolerance) and the DEM has constant elevation value\n\
(plus or minus the secondary tolerance) in relation to its neighbor posts also under the mask";
         }
         
       ConditionLookup[i].units = "sentinel value";
       ConditionLookup[i].units2 = "sentinel offset";
       ConditionLookup[i].units3 = "none";
       break;

      case MASKMONO: /** DEM not monotonic at point defined by specified mask value ***/
       ConditionLookup[i].name = "Grid Post Value Not Monotonic Under Mask";

        ConditionLookup[i].description = "\
The primary participant is a raster mask (WBM) and the secondary participant is a grid (TDF);\n\
both the mask and the grid have exactly the same\n\
post spacing and offsets - the mask has value equal to the primary tolerance value\n\
(plus or minus the secondary tolerance) and the corresponding grid post has value\n\
indicating an opposing direction of gravity-driven flow compared to at least one adjacent\n\
neighbor post that is also under the same mask - when tolerance 3 is any value other than\n\
0, the algorithm sensitivity increases and more conditions (including more false positive\n\
conditions) may be reported (NOTE: no new instances of this inspection can be created)";

       ConditionLookup[i].units = "sentinel value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "none";
       break;

      case PART_ISF: /** two area features have intersecting edges and share part of their faces **/
       ConditionLookup[i].name = "Area Shares Part Of Face With Another Area";
       ConditionLookup[i].description = "\
The primary participant is an area feature that shares only part of its face with a secondary participant\n\
area feature; a condition can exist when the primary and secondary area features have intersecting\n\
perimeters or when a primary participant fully covers the face of a secondary participant and the two\n\
features do not have identical perimeters (including cut-outs)";
       break;

      case AREAINTAREA:
       ConditionLookup[i].name = "Area - Area Intersection";
       ConditionLookup[i].description = "\
The primary participant is an area feature that intersects\n\
a secondary participant area feature; this inspection\n\
does not consider containment to be intersection - the two area feature perimeters must\n\
intersect";
       break;

      case CUT_INT: /** cut-out intersects parent feature outer ring ***/
       ConditionLookup[i].name = "Cut-out Touches Parent Feature Outer Ring";
       ConditionLookup[i].description = "\
The primary participant is an area feature cut-out (inner ring) that intersects or touches\n\
(comes within tolerance distance to) the outer ring of its parent area feature";
       ConditionLookup[i].units = "meters";
       break;


      case ACOVERA:
       ConditionLookup[i].name = "Area Fully Covered By Area";
       ConditionLookup[i].description = "\
The primary participant is an area feature that shares its\n\
entire face with (is entirely covered by) a secondary participant area feature;\n\
this inspection identifies only those area features where the\n\
primary participant is entirely contained in the secondary participant";
       break;


      case AINSIDEHOLE:
       ConditionLookup[i].name = "Illegal Hole";
       ConditionLookup[i].description = "\
The primary participant is an area feature cut-out that includes in\n\
its interior, either in part or in whole, a secondary participant area feature";
       break;

      case AOVERLAPA:
       ConditionLookup[i].name = "Illegal Shared Face";
       ConditionLookup[i].description = "\
The primary participant is an area feature that shares any part of\n\
its face with a secondary participant area feature,\n\
including the  case where either feature is partially or completely inside the other\n\
feature; this inspection identifies area features with intersecting perimeters or those where\n\
one area feature is entirely contained inside the other area feature - no ISF condition exists\n\
if the secondary participant area feature also shares its face with a tertiary\n\
participant area feature as well";
       break;


      case NONODEOVLP: /** line, area have overlapping edge without common node ***/
       ConditionLookup[i].name = "Line - Area Edges Overlap Without Common Node";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature that has a coincident edge with\n\
a secondary participant, another line or area feature, and one primary participant node\n\
on the coincident edge is not common (within the primary tolerance distance) to both features";
       ConditionLookup[i].units = "meters";
       break;
  
      case LLNONODEINT:
       ConditionLookup[i].name = "Feature - Feature Intersection Not At Connecting Node";
       ConditionLookup[i].description = "\
The primary participant is a point, line or area feature that intersects\n\
a secondary participant, another point, line, or area feature, and the point of intersection\n\
is not within the primary tolerance distance to a node (a connecting node) on both features or within the tolerance\n\
distance to a tertiary participant, a point, any part of a line, or on the perimeter or inside of an area feature";
       ConditionLookup[i].units = "meters";
       break;
  
  
      case LLNOENDINT:
       ConditionLookup[i].name = "Line - Line Intersection Not At Shared End Node";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects\n\
a secondary participant line feature and the intersection\n\
location does not correspond to the location of an end node belonging to both line features";
       break;


      case LLINTNOEND: /** two lines intersect, pt of intersection is away from either primary particpant end node ***/
       ConditionLookup[i].name = "Line - Line Intersection Away From Primary End Node";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects\n\
a secondary participant, another line feature, and the point of intersection\n\
is not within the primary tolerance distance to either end node of the primary participant";
       ConditionLookup[i].units = "meters";
       break;

      case LLINTAWAY: /** two lines intersect, and cross over each other ***/
       ConditionLookup[i].name = "Line Feature Crosses Over Another Line Feature";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects and crosses over a secondary participant\n\
line feature - crossing over can be achieved by two primary participant line features joining to each\n\
other at the point of crossing the secondary participant.  Conditions will not be reported when a\n\
primary participant line features ends at a secondary participant (including overlaps)";
       ConditionLookup[i].units = "meters";
       break;
  
  
  
      case LLINT:
       ConditionLookup[i].name = "Line - Line Intersection";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects\n\
a secondary participant line feature";
       break;

      case BADFEATCUT: /** feature cut when no need ***/
       ConditionLookup[i].name = "Feature Cut At Overlap With Another Feature";
       ConditionLookup[i].description = "\
The primary participant is a line feature that has an end node to end node connection\n\
(within primary tolerance distance in meters) with a secondary participant line feature, the connected end\n\
node is located within the primary tolerance distance to a tertiary participant line feature, and the secondary and tertiary\n\
participant line features at least partly overlap each other";
       ConditionLookup[i].units = "meters";
       break;

  
      case FAILMERGEL:
       ConditionLookup[i].name = "Line Feature Merge Failure";
       ConditionLookup[i].description = "\
The primary participant is a line feature that could\n\
have been merged with a connected but separate line feature (detects pseudo-nodes)\n\
- A line could have been merged when it has an endpoint that connects to (lies within primary\n\
'tolerance' distance to) endpoints of other line features having the same feature type,\n\
assigned attributes, and attribute values (except length, which is not considered), the\n\
number of endpoints that are connected is less than or equal to the secondary tolerance,\n\
and the primary participant endpoint does not connect (within primary tolerance distance) to\n\
any part of another feature (point, line, or area) meeting the secondary participant specifications";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "connections";
       break;


      case FAILMERGEL2:  /** line object that should be merged with connecting line no accounting for metadata  ***/
       ConditionLookup[i].name = "Line Feature Merge Failure - 1/4 Degree Boundaries Excluded";
       ConditionLookup[i].description = "\
The primary participant is a line feature that could\n\
have been merged with a connected but separate line feature (detects pseudo-nodes)\n\
- A line could have been merged when it has an endpoint that connects to (lies within primary\n\
'tolerance' distance to) endpoints of other line features having the same feature type,\n\
assigned attributes, and attribute values (except length, which is not considered), the \n\
endpoint is not on or very close to a quarter-degree cell boundary, the\n\
number of endpoints that are connected is less than or equal to the secondary tolerance,\n\
and the primary participant endpoint does not connect (within primary tolerance distance) to\n\
any part of another feature (point, line, or area) meeting the secondary participant specifications";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "connections";
       break;

      case FAILMERGEA:
       ConditionLookup[i].name = "Area Feature Merge Failure";
       ConditionLookup[i].description = "\
The primary participant is an area feature that could\n\
have been merged with a connected but separate area feature - Adjacent area features\n\
could have been merged when they have at least one coincident edge and the two area\n\
features have the same feature type, assigned attributes, and attribute values\n\
(exclusive of the area attribute, which is not considered when assigned to either feature)";
       ConditionLookup[i].units = "meters";
       break;


      case FAILMERGEA2:
       ConditionLookup[i].name = "Area Feature Merge Failure - 1/4 Degree Boundaries Excluded";
       ConditionLookup[i].description = "\
The primary participant is an area feature that could\n\
have been merged with a connected but separate area feature - Adjacent area features\n\
could have been merged when they have at least one coincident edge (within the primary tolerance in meters of each other), the \n\
common edge is not on or very close to a quarter-degree cell boundary (within the secondary tolerance in meters), and the two area\n\
features have the same feature type, assigned attributes, and attribute values\n\
(exclusive of the area attribute, which is not considered when assigned to either feature)";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

  
  
      case LLIEX:
       ConditionLookup[i].name = "Line - Line Intersection With Exception";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects a\n\
secondary participant line feature, except in any case where\n\
the point of intersection is coincident (within 'tolerance' distance) with a tertiary participant\n\
point, line, or area feature (for area tertiary participants, 'coincident' implies 'inside')";
       ConditionLookup[i].units = "meters";
       break;


      case LAIEX: /** line - area intersection with 3rd feature exception ***/
       ConditionLookup[i].name = "Line - Area Intersection With Exception";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects or is inside the perimeter of a\n\
secondary participant area feature, except in any case where\n\
the primary participant is coincident (within 'tolerance' distance) with a tertiary participant\n\
point, line, or area feature (for area tertiary participants, 'coincident' implies 'inside')";
       ConditionLookup[i].units = "meters";
       break;


      case LFNOINT: /** line fails to intersect another line, area, or point and no end node on 1/4 degree line ***/
       ConditionLookup[i].name = "Line - Feature Intersection Failure";
       ConditionLookup[i].description = "\
The primary participant is a line feature that fails to intersect (in 2D) any portion\n\
of a secondary participant that is another point, line, or area feature\n\
and the primary participant line feature does not have an end node coincident with a 1/4 degree line of\n\
latitude or longitude";
       break;

      case LLNOINT:
       ConditionLookup[i].name = "Line - Line / Area Intersection Failure";
       ConditionLookup[i].description = "\
The primary participant is a line feature that fails to intersect (in 2D) any portion of\n\
or have a node within the primary tolerance distance to a secondary participant, another line or area feature";
       ConditionLookup[i].units = "meters";
       break;


      case LOUTSIDEA:
       ConditionLookup[i].name = "Line Vertex Outside Area";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes one or more\n\
vertices having 2D locations outside the perimeter of any instance of a secondary\n\
participant area feature";
       break;


      case LLAINT:
       ConditionLookup[i].name = "Point Or Line End Node - Line Connect On Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a point or line feature that shares its location (point) or at least one end\n\
node (line), within the primary tolerance in meters, with a secondary participant line feature and that\n\
location or end node is also within the primary tolerance to the perimeter of a tertiary participant area feature";
       ConditionLookup[i].units = "meters";
       break;


      case L_NOTL_AINT:
       ConditionLookup[i].name = "Line Without End Node Connect On Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature that has an end node that\n\
is on the perimeter of a tertiary participant area feature but that end node is not also an end\n\
node of a secondary participant line feature or on the perimeter of a secondary participant\n\
area feature";
       ConditionLookup[i].units = "meters";
       break;


      case PTINPROPER:  /** point feature inside an area feature - not within tolerance of edge (or edge or hole) **/
       ConditionLookup[i].name = "Point Properly Contained Inside Area";
       ConditionLookup[i].description = "\
The primary participant is a point feature of the specified type that\n\
is located inside (in 2D) the perimeter of at least one instance of a secondary participant area feature - \n\
points on or within the tolerance distance of the area perimeter are considered outside of the area";
       ConditionLookup[i].units = "meters";
       break;


      case PTINREGION:
       if(NGA_TYPE > 0)
          {
          ConditionLookup[i].name = "Point Inside Or On Area Perimeter";
          ConditionLookup[i].description = "\
The primary participant is a point feature that\n\
is located inside (in 2D) the perimeter of or outside but within tolerance distance to the\n\
perimeter of at least one instance of a secondary participant area feature";
          }
        else
          {
          ConditionLookup[i].name = "Point Inside Region";
          ConditionLookup[i].description = "\
The primary participant is a point feature of the specified type that\n\
is located inside (in 2D) the perimeter of or outside but within tolerance distance to the\n\
perimeter of at least one instance of a secondary participant polygon feature";
          }
       ConditionLookup[i].units = "meters";
       break;


      case PTOSIDEREGION:
       if(NGA_TYPE > 0)
          {
       ConditionLookup[i].name = "Point Not Inside Area";
       ConditionLookup[i].description = "\
The primary participant is a point feature that\n\
is located (2D) outside the perimeter of all instances of any secondary\n\
participant area feature";
          }
       else
          {
       ConditionLookup[i].name = "Point Not Inside Area Or Polygon";
       ConditionLookup[i].description = "\
The primary participant is a point feature of the specified type that\n\
is located (2D) outside the perimeter of all instances of the secondary\n\
participant, an area or polygon of the specified type";
          }
       break;



      case OBJECTWITHOUT:
       if(NGA_TYPE == 0)
         {
         ConditionLookup[i].name = "Polygon Or Area Not Containing Point Or Line";
       ConditionLookup[i].description = "\
The primary participant is an area or polygon of the specified type\n\
that fails to contain any portion or entire instance of the secondary\n\
participant, either a point or line of the specified type";
         }
       else
         {
         ConditionLookup[i].name = "Area Not Containing Any Part Of Another Feature";
       ConditionLookup[i].description = "\
The primary participant is an area feature\n\
that fails to contain any portion or entire instance of the secondary\n\
participant, either a point, line, or area feature";
         }
       break;


      case OBJ_WO_TWO: /** area contains secondary P,A,L but not tertiary P,A,L ***/
       ConditionLookup[i].name = "Area Containing Secondary But Not Tertiary Feature";
       ConditionLookup[i].description = "\
The primary participant is an area feature that contains some part of a secondary participant point,\n\
line, or area feature and does not contain any part of a tertiary participant point, line, or area\n\
feature";
       break;



      case AWITHOUTA:  /** area that does not fully contain a second area ***/
       ConditionLookup[i].name = "Area Not Fully Containing Second Feature";
       ConditionLookup[i].description = "\
The primary participant is an area feature that fails to completely\n\
contain the entire extent of the secondary participant which could be\n\
a point, a line, a different area feature, or a grid post having\n\
value in the closed interval defined by the primary and secondary tolerances";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       break;


      case FSFAIL: /*** face sharing failure ***/
       ConditionLookup[i].name = "Area Face-Sharing Failure";
       ConditionLookup[i].description = "\
The primary participant is an area feature that\n\
fails to share any portion of its face with the face of a secondary\n\
participant area feature";
       break;


      case PSHAREFAIL:  /*** an area feature fails to share any of its perimeter with a 2d area feature ***/
       ConditionLookup[i].name = "Perimeter Sharing Failure";
       ConditionLookup[i].description = "\
The primary participant is an area feature\n\
that fails to share (be less that the specified distance from) any segment of its\n\
perimeter with any instance of a secondary participant area feature and fails to be\n\
fully contained inside the perimeter of a tertiary area feature participant";
       ConditionLookup[i].units = "meters";
       break;



      case NOCOINCIDE: /** area without line end node or segment on its perimeter ***/
       ConditionLookup[i].name = "Area Line Coincidence Failure";
       ConditionLookup[i].description = "\
The primary participant is an area feature\n\
that fails to have a secondary participant line feature terminal node or\n\
inter-vertex portion (segment) coincident with\n\
(within the tolerance distance to) an inter-vertex portion (segment) of its perimeter";
       ConditionLookup[i].units = "meters";
       break;


      case ANOCOVERLA:
       ConditionLookup[i].name = "Area Edge Not Covered By Area Edge Or Line Segment";
       ConditionLookup[i].description = "\
The primary participant is an area feature having a\n\
perimeter that fails to be entirely covered by instance(s) of the tertiary\n\
participant, either another area (perimeter) or line feature. The secondary\n\
participant is an area feature cut-out having a\n\
perimeter that fails to include and be entirely covered by instance(s) of the tertiary\n\
participant area feature - tolerance measures distance between edge segments at vertices";
       ConditionLookup[i].units = "meters";
       break;

      case QUALANOCOVLA: /** area permin not covered by line or area AND is inside a third area ***/
       ConditionLookup[i].name = "Qualified Area Edge Not Covered By Area Edge Or Line Segment";
       ConditionLookup[i].description = "\
The primary participant is an area feature having a\n\
perimeter that fails to be entirely covered by instance(s) of the secondary\n\
participant, either another area (perimeter) or line feature and\n\
the area feature is at least partially inside a tertiary participant\n\
area feature - tolerance measures distance between edge segments at vertices";
       ConditionLookup[i].units = "meters";
       break;


      case AMCOVAFAIL: /** area not coverer by adjoining areas **/
       ConditionLookup[i].name = "Area Not Covered By Set Of Connected Area(s)";
       ConditionLookup[i].description = "\
The primary participant is an area feature\n\
that does not share its entire face with (fails to be entirely covered by)\n\
a set of secondary participant area features that share edges (e.g., the secondary\n\
participant area features are connected to each other by shared edges)";
       break;

      case ANOCOVERA:
       ConditionLookup[i].name = "Area Not Covered By Area";
       ConditionLookup[i].description = "\
The primary participant is an area feature\n\
that does not share its entire face with (fails to be entirely covered by)\n\
a single instance of any secondary participant area feature";
       break;

      case OVERUNDER: /** any feature outside a perimeter-defining area or a line end node undershooting it **/
       ConditionLookup[i].name = "Feature - Area Perimeter Over- Or Undershoot";
       ConditionLookup[i].description = "\
The primary participant is either: 1) a line feature an end node inside a secondary\n\
participant area feature perimeter and within the distance range (2D) to that perimeter where\n\
the range is specified by the primary (lower distance bound) and secondary (upper distance\n\
bound) tolerances or 2) any point, line, or area vertex that lies outside the perimeter of\n\
the secondary participant area feature";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;



      case CUTOUT:
       ConditionLookup[i].name = "Cut-out";
       ConditionLookup[i].description = "\
The primary participant is an area feature cut-out (inner ring)";
       break;

      case PORTRAYF: /** write feature that fails all MGCP4 portrayal rules ***/
       ConditionLookup[i].name = "Feature Fails All MGCP TRD4 Portrayal Rules";
       ConditionLookup[i].description = "\
The primary participant is a point, line, or area feature that did not satisfy any of the MGCP TRD4 portrayal\n\
specifications and thus will not be portrayed (only applies to projects created using the MGCP TRD4 schema)";
       break;

      case TPORTRAYF: /** write feature that fails all TDS6 portrayal rules ***/
       ConditionLookup[i].name = "Feature Fails All TDS v6.1 Portrayal Rules";
       ConditionLookup[i].description = "\
The primary participant is a point, line, or area feature that did not satisfy any of the TDS v6.1 portrayal\n\
specifications (derived from NAS 6.2) and thus will not be portrayed (only applies to projects created using\n\
the TDS v6.1 schema)";
       break;

      case COLINEAR: /** 3 consecutive verts on line or area perim are collinear - middle one not connecting node ***/
       ConditionLookup[i].name = "Non-connecting Node Collinear Vertices";
       ConditionLookup[i].description = "\
The primary participant is an area perimeter or line feature that includes 3 consecutive nearly collinear vertices\n\
(within the primary tolerance of being exactly collinear), the middle vertex of the three is not a connecting\n\
node of a secondary participant feature (is not within the secondary tolerance of a vertex of a secondary\n\
participant feature) and the distance between the two primary participant adjacent vertices is less than the tertiary tolerance";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

      case CONF_STATS: /**** just generates conflation-information statistics, etc - no conditions to read/write **/
       ConditionLookup[i].name = "Generate Conflation Information";
       ConditionLookup[i].description = "\
The primary participant is a line feature type about which attribute value\n\
fill rate statistics will be generated.  This inspection does not generate conditions, but\n\
it does generate spreadsheet and annotated point of interest files that provide information\n\
about the characteristics of different sets of data that could be used in the conflation\n\
process.";
       /**ConditionLookup[i].units = "meters";**/
       break;

      case KERNELSTATS: /** just write DEM comparison statistics - no conditions produced ***/
       ConditionLookup[i].name = "Generate Kernel-based DEM Comparison Statistics";
       ConditionLookup[i].description = "\
The primary (source) participant and secondary (reference) participants are elevation grids for which comparative\n\
statistical measures will be calculated. This inspection generates mean, standard deviation, RMSE, and LE90 to compare\n\
the source and reference grids. The 5x5 weighted kernel method is used to calculate an elevation value in the reference\n\
grid corresponding to an elevation value in the source grid. Elevation values <= than the primary tolerance\n\
are ignored. Elevation differences greater than the fourth tolerance will not be used in producing the statistics.\n\
The tertiary participant specification is optional; it can be used to preclude elevation value comparison. When\n\
specified, elevation posts must coincide with a tertiary participant (e.g., a Water Body Mask) mask post having value\n\
>= the secondary tolerance and <= the tertiary tolerance (e.g., 0 and 0, indicating the elevation value is not in water).\n\
This inspection does not produce conditions, but it does generate a report,\n\
'KernelStatistics.txt', that can be found in the project's 'summary_files' folder.";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "meters";
       break;

      case BILINSTATS: /** just write DEM comparison statistics - no conditions produced ***/
       ConditionLookup[i].name = "Generate Nearest Neighbor-based DEM Comparison Statistics";
       ConditionLookup[i].description = "\
The primary (source) participant and secondary (reference) participants are elevation grids for which comparative\n\
statistical measures will be calculated. This inspection generates mean, standard deviation, RMSE, and LE90 to compare\n\
the source and reference grids. The 2x2 bilinear interpolation method is used to calculate an elevation value in the\n\
reference grid corresponding to an elevation value in the source grid. Elevation values <= than the primary tolerance\n\
are ignored. Elevation differences greater than the fourth tolerance will not be used in producing the statistics.\n\
The tertiary participant specification is optional; it can be used to preclude elevation value comparison. When\n\
specified, elevation posts must coincide with a tertiary participant (e.g., a Water Body Mask) mask post having value\n\
>= the secondary tolerance and <= the tertiary tolerance (e.g., 0 and 0, indicating the elevation value is not in water).\n\
This inspection does not produce conditions, but it does generate a report,\n\
'NearNeighborStatistics.txt', that can be found in the project's 'summary_files' folder.";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "meters";
       break;

      case CONFLATE: /*** line is unique among conflation sets of data ***/
       ConditionLookup[i].name = "Line Feature Is Unique";
       ConditionLookup[i].description = "\
The primary participant is a line feature about which uniqueness information will be\n\
collected.  This inspection identifies primary participant line features that fail to\n\
be entirely covered by instance(s) of the secondary participant (either perimeter\n\
edges of an area feature or segments of another line feature) or inside the perimeter\n\
of a tertiary participant area feature. The tolerance specification is used as an upper\n\
limit on coordinate equality during the uniqueness analyses.";
       ConditionLookup[i].units = "meters";
       break;



      case LNOCOVERLA:
       ConditionLookup[i].name = "Line Not Covered By Area Or Line";
       ConditionLookup[i].description = "\
The primary participant is a line feature that fails to\n\
be entirely covered by instance(s) of the secondary participant (either perimeter\n\
edges of an area feature or segments of another line feature)\n\
or fully inside instances of the tertiary type area feature - the tolerance\n\
is a measure of distance between edge segments at vertices and is used as a lower limit\n\
on the length of uncovered segments that will be reported.";
       ConditionLookup[i].units = "meters";
       break;


      case LNOCOV2A:  /** line not covered by edges of 2 area features ***/
       ConditionLookup[i].name = "Line Not Covered By 2 Area Perimeters";
       ConditionLookup[i].description = "\
The primary participant is a line feature\n\
that fails to be entirely covered by 2 different instances of a secondary\n\
participant area feature -\n\
tolerance measures distance between edge segments at vertices";
       ConditionLookup[i].units = "meters";
       break;
  
    
      case ISOLINE:
       ConditionLookup[i].name = "Line Completely Inside Area";
       ConditionLookup[i].description = "\
The primary participant is a line feature that has all of its vertices inside\n\
and not within the primary tolerance distance to an edge of a single instance of a\n\
secondary participant area feature and also has no vertex that is within the primary\n\
tolerance distance to any vertex of a tertiary participant point, line, or area feature";
       ConditionLookup[i].units = "meters";
       break;


      case LSEGCOVERA: /** line segment overlaps an area feature perimeter ***/
       ConditionLookup[i].name = "Line Segment Overlaps Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature that has part or all of one or more of its interior\n\
segments coincident with (in 2D) or within tolerance distance to\n\
a single instance of any perimeter segment of a secondary participant area feature";
       ConditionLookup[i].units = "meters";
       break;



      case LINSIDEA: /** line partly or entirely inside area feature ***/
       ConditionLookup[i].name = "Line Inside Area";
       ConditionLookup[i].description = "\
The primary participant is a line feature that has\n\
any of its vertices or segments partly or entirely inside (2D)\n\
a single instance of a secondary participant area feature or any vertex within the primary\n\
tolerance distance to the secondary feature and, if the line and area intersect, the angle at\n\
the point of intersection is less than the secondary tolerance - conditions will not be reported\n\
when the primary participant is also overlapping or inside a tertiary participant line or area feature";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "degrees";
       break;


      case LEINSIDEA: /** line end node properly inside an area ***/
       ConditionLookup[i].name = "Line End Node Inside Area";
       ConditionLookup[i].description = "\
The primary participant is an end node of a line feature that is not\n\
connected to (within tolerance distance to) another line feature vertex (also described by the primary\n\
participant specification) and that end node lies properly inside (not on an edge) of an instance of a\n\
secondary participant area feature";
       ConditionLookup[i].units = "meters";
       break;

      case LEAON_NOTIN: /** line end node on area edge, line not inside area ***/
       ConditionLookup[i].name = "Line End - Area Edge Coincidence Without Containment";
       ConditionLookup[i].description = "\
The primary participant is an end node of a line feature that is not\n\
connected to (within tolerance distance to) another line feature vertex (also described by the primary\n\
participant specification) and is coincident with the perimeter of a secondary participant\n\
area feature, and the line feature is not inside the area feature in the vicinity of the\n\
coincident end node";
       ConditionLookup[i].units = "meters";
       break;

      case COINCIDEFAIL: /** line or area feature segment fails to coincide with 2 other line or area features **/
       ConditionLookup[i].name = "Three Feature Coincidence Failure";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature that\n\
fails to have a single inter-vertex portion (segment) that is coincident with\n\
(within primary tolerance distance to) 2 other secondary participants, either 2 other line feature\n\
end nodes (of different line features) or 2 area feature perimeter segments (of different\n\
area features), or 1 line feature end node and 1 area feature perimeter segment";
       ConditionLookup[i].units = "meters";
       break;

      case ISOLATEDA:  /*** area feature does not instersect another area or a line feature ***/
       ConditionLookup[i].name = "Area Feature Perimeter Intersection Failure";
       ConditionLookup[i].description = "\
The primary participant is an area feature having a perimeter that\n\
fails to intersect an instance of the secondary participant, either another area feature or \n\
a line feature.  Neither containment nor face sharing is viewed as an intersection - line or\n\
area features that are within tolerance distance will be considered as intersecting";
       ConditionLookup[i].units = "meters";
       break;

      case NETISOFEAT: /** form a network - check for nets with one feature, but not another ***/
       ConditionLookup[i].name = "Line Or Area Feature Transitive Intersection Failure";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature that fails to connect to or to intersect (if secondary\n\
tolerance is 1) an instance of the tertiary participant (a specified line or area feature) and also\n\
fails to share edges with or intersect at least one other primary or secondary participant (line or area inner or\n\
outer ring) in a set of connected primary and secondary participant features, at least one of which intersects\n\
or connects to a specified tertiary (line or area) feature.  Neither containment nor face sharing\n\
is viewed as an intersection or connection. Participating area and line features\n\
that have vertices within the primary tolerance distance of each other will be considered as being\n\
connected. Two features are also considered connected if they intersect each other and the secondary tolerance is\n\
set to 1.";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "Boolean";
       break;

      case ANETISOA: /** area not trans connected to another area by shared edges ***/
       ConditionLookup[i].name = "Area Feature Not Transitively Sharing Edges With Another Area";
       ConditionLookup[i].description = "\
The primary participant is an area feature that is not a cut-out and has a perimeter that\n\
fails to share edges with any instance of the tertiary participant, a specified area feature, and also\n\
fails to share edges with a secondary participant (either inner or outer ring) area feature set,\n\
at least one of which shares edges with a specified\n\
tertiary participant area feature.  Neither containment nor face sharing is viewed as edge sharing.\n\
Participants that have edges within the primary tolerance distance of overlapping each other will be\n\
considered as sharing those edges";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case NETISOA: /** like ISOLATEDA except allowed a transitive connection through other like features ***/
       ConditionLookup[i].name = "Area Feature Perimeter Transitive Intersection Failure";
       ConditionLookup[i].description = "\
The primary participant is an area feature that is not a cut-out and has a perimeter that\n\
fails to intersect an instance of the secondary participant, a specified line feature and also\n\
fails to share edges with another primary participant (either inner or outer ring) connected area feature set,\n\
at least one of which intersects a specified\n\
secondary participant line feature.  Neither containment nor face sharing is viewed as an intersection. Secondary participant\n\
line features that are within the secondary tolerance distance will be considered as intersecting. Primary\n\
participants that have edges within the primary tolerance distance of overlapping each other will be considered as sharing those edges";
       ConditionLookup[i].units = "meters";
       break;



      case SHARE3SEG: /** line feature segment overlaps 2 other line or area feature segments ***/
       ConditionLookup[i].name = "Three Feature Edges Overlap (Coincident Segments)";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes an inter-vertex portion (a segment) that\n\
overlaps both a secondary and tertiary participant, either of which could be other\n\
line feature segments or area feature edges; segments / edges are coincident if they are\n\
closer to each other than specified by the primary tolerance value: Conditions are reported\n\
when the length of the overlap of all 3 features is greater than the secondary tolerance value";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case SHARESEG: /** line feature segment overlaps 1 other line feature segment ***/
       ConditionLookup[i].name = "Two Lines Overlap (Coincident Segments)";
       ConditionLookup[i].description = "\
The primary participant is a line feature that\n\
shares inter-vertex portions (segments) with a secondary participant line feature\n\
and is not coincident with a tertiary participant line feature or\n\
inside the perimeter of a tertiary participant area feature.  Segments are coincident\n\
if they are closer to each other than specified by the primary tolerance value in 2D and at least\n\
as close together in elevation as specified by the tertiary tolerance (which when set <0 causes this\n\
tolerance to be ignored); only overlapping segments with length greater than or equal to the\n\
secondary tolerance are reported";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


      case LLI_ANGLE: /*** 2 lines intersect at severe angle ***/
       ConditionLookup[i].name = "Two Lines Intersect At High Angle";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects a secondary participant line\n\
feature forming angles greater than the primary tolerance, except in any case where\n\
the point of intersection is coincident (within 'secondary tolerance' distance) with a tertiary\n\
participant feature (for area tertiary participants, 'coincident' implies 'inside')";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       break;


      case LSPANFAIL: /** line not covered by face of doesnt span between edges ***/
       ConditionLookup[i].name = "Line Not Spanning Area Feature";
       ConditionLookup[i].description = "\
The primary participant is a line feature that fails to have both of its terminal nodes\n\
within the tolerance distance to the perimeter of a secondary participant area feature\n\
and its extent fully covered by secondary participant area feature face";
       ConditionLookup[i].units = "meters";
       break;

      case MULTIDFEAT:
       ConditionLookup[i].name = "Line Or Area Feature Includes Both Sentinel And Actual Z Values";
       ConditionLookup[i].description = "\
The primary participant is a single line or area feature\n\
that includes both sentinel and actual values in its set of Z coordinate values";
       ConditionLookup[i].units = "none";
       break;


      case MULTISENTINEL: /** single line or area has more than one sentinel z value ***/
       ConditionLookup[i].name = "Line Or Area Feature Includes Different Sentinel Z Values";
       ConditionLookup[i].description = "\
The primary participant is a single line or area feature\n\
that includes more than one type of sentinel values in its set of Z coordinate values";
       ConditionLookup[i].units = "none";
       break;

  
      case LENOCOVERP:
       ConditionLookup[i].name = "Line End Node Not Covered By Point";
       ConditionLookup[i].description = "\
The primary participant is a line feature\n\
that includes an end node such that no secondary participant\n\
point feature is within 'tolerance' distance (3D) to that end node\n\
(for 2D features, the distance will be measured in 2D)";
       ConditionLookup[i].units = "meters";
       break;
  

      case LENOCOVERA:
       ConditionLookup[i].name = "Line End Node Not Covered By Area Perimeter";
       ConditionLookup[i].description = "\
The primary participant is a line feature\n\
that includes an end node such that a secondary participant area feature perimeter\n\
is within the distance range (2D) to that end node; the range is specified\n\
by the primary (lower distance bound) and secondary (upper distance bound) tolerances";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;

      case LACUTFAIL:  /** line not cut at intersection with area perimeter **/
       ConditionLookup[i].name = "Line Not Cut At Area Perimeter Intersection";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects\n\
the perimeter of a secondary participant area feature and the point of intersection\n\
is not within the primary tolerance distance to an end node of the line feature";
       ConditionLookup[i].units = "meters";
       break;


      case LAINTNOEND:
       ConditionLookup[i].name = "Line - Area Intersection Without Line End Node Intersection";
       ConditionLookup[i].description = "\
The primary participant is a line feature that intersects\n\
the perimeter of a secondary participant area feature and the point of intersection\n\
is not within the primary tolerance distance to an end node of the line feature and no end node\n\
of the line feature is coincident with the area feature perimeter (distances are measured in 2D meters)";
       ConditionLookup[i].units = "meters";
       break;


      case GSPIKE:
       ConditionLookup[i].name = "Outlier Grid Data";
       ConditionLookup[i].description = "\
The primary participant is an individual grid post that appears to be either a spike or well.\n\
Spikes and wells are identified by measuring the angle between a single grid post and its immediate\n\
8 adjacent posts.  The angle to be exceeded is specified as the primary tolerance. Neighbor posts\n\
that do not have a post value difference >= tolerance 2 will not be considered in the calculations.\n\
The number of immediate neighbors that must exceed the angle is specified as tolerance 3.  Posts with\n\
values equal to tolerance 4 or 5 will be ignored.";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units4 = "post value";
       ConditionLookup[i].units5 = "post value";
       ConditionLookup[i].units3 = "neighbors";
       ConditionLookup[i].units2 = "post value";
       break;

      case PT_GRID_DIF: /** point and grid z value mismatch at exact coord, no interpolation **/
       ConditionLookup[i].name = "Grid Edge-Matching Difference";
       ConditionLookup[i].description = "\
The primary participant is an individual point feature that has a position within the\n\
primary tolerance in meters to a post of the secondary participant grid and the point\n\
z value is different than the grid post value by more than the secondary tolerance";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "post value difference";
       break;

      case RAISEDPC: /** number of raised shoreline points exceeds tolerance **/
       ConditionLookup[i].name = "Raised Shoreline Percentage";
       ConditionLookup[i].description = "\
The primary participant is an individual mask post (typically from the Edit Data Mask).\n\
The secondary participant is a mask post (typically from the Water Body Mask) that can be\n\
identified as a shoreline post by having value 0 and being adjacent to a post having a value\n\
>= the secondary tolerance and <= the tertiary tolerance. When the number of these posts (the\n\
identified shoreline posts) that are coincident with primary participant posts having value equal\n\
to the primary tolerance exceeds the per cent of all shoreline posts specified as the fourth tolerance,\n\
a condition will be reported. When the fifth tolerance is set to\n\
0, all conditions are written as points, when set to 1, individual polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "per cent";
       ConditionLookup[i].units5 = "none";
       break;


      case WATERMMU: /** minimum mapping unit for water body below threshold ***/
       ConditionLookup[i].name = "MMU Below Threshold";
       ConditionLookup[i].description = "\
The primary participant is an individual mask post (typically a water body mask post) having value\n\
>= the primary tolerance and <= the secondary tolerance that is also part of a contiguous set of like\n\
post values and the area (in square meters) represented by the number of posts in the set is less than\n\
the tertiary tolerance. Conditions are always reported as billboards. Typically the primary (lower) and\n\
secondary (upper) tolerances will have the same value.";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "square meters";
       ConditionLookup[i].units4 = "none";
       break;

      case BREAKLINE: /** river elevation change at bad angle with shorelines ***/
       ConditionLookup[i].name = "River Elevation-Change Orientation To Shoreline";
       ConditionLookup[i].description = "\
The primary participant is an individual mask post (a water body mask post) having value\n\
>= the third tolerance and <= the fourth tolerance.  The secondary participant designates an\n\
elevation grid. The elevation grid exhibits a localized elevation change >= 0.1 m at the same\n\
location as the primary participant and the best-fit line to adjacent elevation change post locations includes\n\
a number of posts >= the primary tolerance and forms an angle with the shortest distance line between the\n\
two shores (through one of the best-fit line endpoints) that is >= the secondary tolerance in degrees";
       ConditionLookup[i].units = "posts";
       ConditionLookup[i].units2 = "degrees";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units4 = "post value";
       ConditionLookup[i].units5 = "none";
       break;

      case FLOWSTEP:  /** step size in river flow above threshold ***/
       ConditionLookup[i].name = "Step Size Above Threshold";
       ConditionLookup[i].description = "\
The primary participant is an individual mask post (typically a water body mask post) having value\n\
>= the primary tolerance and <= the secondary tolerance.  The secondary participant designates an\n\
elevation grid. The elevation grid exhibits a localized elevation change at the same location as the\n\
primary participant and the amount of elevation change is greater than or equal to the tertiary tolerance.\n\
When the fourth tolerance is set to 0, all conditions are written as points, when set to 1, individual\n\
convex polygons that enclose contiguous points are used, when set to 2, both individual points and\n\
enclosing polygons are written";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units2 = "post value";
       ConditionLookup[i].units3 = "meters";
       ConditionLookup[i].units4 = "none";
       break;


      case GSHELF:  /** looking for shelf formations like PUE in DEM ***/
       ConditionLookup[i].name = "Elevation 'Shelf' Indicative Of A PUE";
       ConditionLookup[i].description = "\
The primary participant is a grid post that is either above or below the post value of at least\n\
as many neighbor posts specified by tolerance 4 and is contiguous with an number of similar posts\n\
that is greater than the lower limit specified by tolerance 5. The difference between the post and its\n\
neighbors must be >= tolerance 2 and <= tolerance 3.  Additionally, the angle formed by the neighbor\n\
posts with this elevation difference must be greater than the angle specified by tolerance 1.  Posts\n\
in a secondary participant grid having a value greater than tolerance 6 (e.g., a WBM post with value > 0\n\
meaning water is present) and at or neighboring the primary participant post will cause the primary\n\
participant post to be ignored.  All primary participant posts with value <= -500 will be ignored.";
       ConditionLookup[i].units = "decimal degrees";
       ConditionLookup[i].units2 = "post value difference";
       ConditionLookup[i].units3 = "post value difference";
       ConditionLookup[i].units4 = "neighbors";
       ConditionLookup[i].units5 = "number of posts";
       ConditionLookup[i].units6 = "post value";
       break;


      case LOSMINHGT:
       ConditionLookup[i].name = "Height AGL To Achieve LOS";
       ConditionLookup[i].description = "\
The primary participant is an individual grid post that is inside an area feature specified\n\
as the secondary participant and is within the distance (primary tolerance) to the coordinate\n\
specified by the secondary tolerance (longitude) and tertiary tolerance (latitude). A condition\n\
will be written at that point using the height AGL to achieve LOS with the coordinate.";
       ConditionLookup[i].units = "decimal degrees";
       ConditionLookup[i].units2 = "decimal degrees";
       ConditionLookup[i].units3 = "decimal degrees";
       break;

      case AVGSPIKE:
       ConditionLookup[i].name = "Spike / Well From Neighbor Average Post Values";
       ConditionLookup[i].description = "\
The primary participant is an individual grid post that appears to be either a spike or well in that it\n\
is either above or below the post value of at least as many neighbor posts specified by tolerance 5.\n\
Spikes and wells are identified by calculating the average post value of neighboring posts and when\n\
the primary participant post value differs from this average by an amount greater than or equal\n\
to the primary tolerance, a condition is reported. Neighbor posts that have values equal to\n\
tolerance 3 or tolerance 4 will not be used in forming the average. The number of neighbor posts used\n\
to calculate the average must be greater than or equal to tolerance 2. When the sixth tolerance is set to\n\
0, all conditions are written as points, when set to 1, individual polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written. Posts that are coincident with secondary participant non-zero posts will not be reported\n\
as conditions (this can be used to preclude reporting conditions in Lake, River, or Tidal Water areas)";
       ConditionLookup[i].units = "post value";
       ConditionLookup[i].units4 = "post value";
       ConditionLookup[i].units5 = "neighbors";
       ConditionLookup[i].units3 = "post value";
       ConditionLookup[i].units2 = "neighbors";
       ConditionLookup[i].units6 = "none";
       break;



     case GRID_STD_DEV: /** grid elev value, inside feature polygon, over range offset from std deviation **/
       ConditionLookup[i].name = "Outlier Grid Post Value Inside Area Feature";
       ConditionLookup[i].description = "\
The primary participant is a grid post having a value that lies inside the perimeter of\n\
a secondary participant area feature and the difference between the post value and the average post value\n\
of other grid values also inside the secondary participant and in the immediate area is more than the\n\
primary tolerance (standard deviation exceeds primary tolerance) - grid values less than the secondary\n\
tolerance will not be considered";
       ConditionLookup[i].units = "standard deviation in meters";
       ConditionLookup[i].units2 = "post value";
       break;



     case ELEVGT:
       ConditionLookup[i].name = "Elevation Or Post Value Above";
       ConditionLookup[i].description = "\
The primary participant is a grid,  or 3-dimensional point, line, or area feature,\n\
having one or more post values or vertex Z values greater than to the specified\n\
primary tolerance - the secondary tolerance applies only to grid data; when set to\n\
0, all conditions are written as points, when set to 1, individual polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
       ConditionLookup[i].units = "value";
       ConditionLookup[i].units2 = "none";
       break;



      case ELEVLT:
       ConditionLookup[i].name = "Elevation Or Post Value Below";
       ConditionLookup[i].description = "\
The primary participant is a grid,  or 3-dimensional point, line, or area feature,\n\
having one or more post values or vertex Z values less than to the specified\n\
primary tolerance - the secondary tolerance applies only to grid data; when set to\n\
0, all conditions are written as points, when set to 1, individual polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
       ConditionLookup[i].units = "value";
       ConditionLookup[i].units2 = "none";
       break;
  
  
      case ELEVEQ:
       ConditionLookup[i].name = "Elevation Or Post Value Within Range, Closed Interval";
       ConditionLookup[i].description = "\
The primary participant is a grid, or 3-dimensional point, line, or area feature,\n\
having one or more post values or vertex Z values >= the primary tolerance and <= the\n\
secondary tolerance - the tertiary tolerance applies only to grid data; when set to\n\
0, all conditions are written as points, when set to 1, individual polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
       ConditionLookup[i].units = "value";
       ConditionLookup[i].units2 = "value";
       ConditionLookup[i].units3 = "none";
       break;

      case ELEVEQOPEN:  /** elevation in range, open interval**/
       ConditionLookup[i].name = "Elevation Or Post Value Within Range, Open Interval";
       ConditionLookup[i].description = "\
The primary participant is a grid, or 3-dimensional point, line, or area feature,\n\
having one or more post values or vertex Z values > the primary tolerance and < the\n\
secondary tolerance - the tertiary tolerance applies only to grid data; when set to\n\
0, all conditions are written as points, when set to 1, individual polygons that enclose\n\
contiguous points are used, when set to 2, both individual points and enclosing polygons\n\
are written";
       ConditionLookup[i].units = "value";
       ConditionLookup[i].units2 = "value";
       ConditionLookup[i].units3 = "none";
       break;

      case HIGHLIGHTED: /** feature is on the highlight list from view by attribution ***/
       ConditionLookup[i].name = "Feature Currently Designated For Highlighting";
       ConditionLookup[i].description = "\
The primary participant is a point, line, or area feature\n\
that meets the characteristics of one (or more) features currently\n\
designated for highlighting using the 'View -> By Attribution ...' capability";
       ConditionLookup[i].units = "(none)";
       break;


     case ENCONNECT:
       ConditionLookup[i].name = "Connecting Line End Nodes";
       ConditionLookup[i].description = "\
The primary participant is a line feature\n\
having one or more end nodes that connect to\n\
a number of other secondary participant line feature end nodes and that number is\n\
within the range defined by the primary and secondary tolerances";
       ConditionLookup[i].units = "connections";
       ConditionLookup[i].units2 = "connections";
       break;
  

      case BADENCON: /** bad sequence on line feature connections ***/
       ConditionLookup[i].name = "Reverse Order Connecting Line End Nodes";
       ConditionLookup[i].description = "\
The primary participant is a line feature\n\
having one or more end nodes that connect to (are within the primary tolerance distance of)\n\
only one other secondary participant\n\
line feature end node and the connecting nodes are both the first or both the last\n\
node on both features";
       ConditionLookup[i].units = "meters";
       break;

      case EXTRA_NET:
       ConditionLookup[i].name = "Near Miss Connection Between Networks";
       ConditionLookup[i].description = "\
The primary participant is a point or a vertex of a line or area feature\n\
that is close to (distance is in the range defined by the primary and secondary\n\
tolerance specifications) a vertex of a feature in another, separate network";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters"; 
       break;

      case CREATENET: /*** the internal check for creating networks - shouldn't appear in inspection menu ***/
       ConditionLookup[i].name = "Create Networks";
       ConditionLookup[i].description = "\
This inspection does not write conditions, but it does create either\n\
predefined or user-defined networks of point, line, and area features";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


      case INTRA_NET:
       ConditionLookup[i].name = "Near Miss Connection Inside One Network";
       ConditionLookup[i].description = "\
The primary participant is a point or a vertex of a line or area feature\n\
that is close to (distance is in the range defined by the primary and secondary\n\
tolerance specifications) a vertex of a another feature already connected in the same network";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;


      case FEATSPIKE:
       ConditionLookup[i].name = "Adjacent Vertex Elevation Spike Or Pit";
       ConditionLookup[i].description = "\
The primary participant is a 3D line or area feature having\n\
consecutive vertices with Z values that differ by an amount sufficient to form a spike\n\
or pit, measured in degrees, greater than or equal to the primary tolerance, and the length\n\
of the segment forming the spike is greater than the secondary tolerance (in meters)";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       break;


      case ELEVADJCHANGE:
       ConditionLookup[i].name = "Adjacent Vertex Elevation Change";
       ConditionLookup[i].description = "\
The primary participant is a 3D line or area feature having\n\
consecutive vertices with Z values that differ by more than the primary tolerance";
       ConditionLookup[i].units = "meters";
       break;


      case CLAMP_SDC: /*slope direction change along a line that has been elevation-value clamped to underlying DEM ***/
       ConditionLookup[i].name = "Changing Slope Direction Along An Elevation-clamped Feature";
       ConditionLookup[i].description = "\
The primary participant is a line or area feature that includes a direction of slope change (from positive\n\
to negative or negative to positive) at one or more vertices along the feature length (line features) or\n\
perimeter (area features) when the vertex elevation value has been calculated from an overlapping DEM (e.g.,\n\
the line or area feature perimeter has been 'clamped' to the surface of a corresponding DEM). The change magnitude,\n\
in degrees, is greater than or equal to the primary tolerance and the difference between the elevation change from the\n\
maximum change that would preserve monotonic slope is greater than the secondary tolerance, in meters";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       break;

      case SLOPEDIRCH:
       ConditionLookup[i].name = "Changing Slope Direction Along A 3D Feature";
       ConditionLookup[i].description = "\
The primary participant is a 3D feature such that the\n\
direction of slope changes (from positive to negative or vice versa) at one or\n\
more nodes along its length (line features) or perimeter (area features), and the\n\
change magnitude, in degrees, is greater than or equal to the primary tolerance";
       ConditionLookup[i].units = "degrees";
       break;


      case  LJOINSLOPEDC:
       ConditionLookup[i].name = "Changing Slope Direction Between Line Features";
       ConditionLookup[i].description = "\
The primary participant is a 3D line feature such that the\n\
direction of slope changes (from positive to negative or vice versa) at the end\n\
node where it joins another 3D secondary participant line feature and\n\
the magnitude of the change is greater than or equal to the primary tolerance";
       ConditionLookup[i].units = "degrees";
       break;


      case CLAMP_JOINSDC: /** slope direction change at line feature connection when both are clamped to DEM ***/
       ConditionLookup[i].name = "Changing Slope Direction Between Elevation-clamped Line Features";
       ConditionLookup[i].description = "\
The primary participant is a line feature that includes a direction of slope change (from positive\n\
to negative or negative to positive) at one of its end nodes that is connected to a secondary\n\
participant line feature end node when all elevation values have been calculated from a corresponding DEM (e.g.,\n\
the primary and secondary line features have been 'clamped' to the surface of a corresponding DEM). The change magnitude,\n\
in degrees, is greater than or equal to the primary tolerance and the difference between the elevation change from the\n\
maximum change that would preserve monotonic slope is greater than the secondary tolerance, in meters";
       ConditionLookup[i].units = "degrees";
       ConditionLookup[i].units2 = "meters";
       ConditionLookup[i].units3 = "meters";
       break;

  
  
      case LOOPS:
       ConditionLookup[i].name = "Loop (Self-intersecting Area Or Line Feature)";
       ConditionLookup[i].description = "\
The primary participant is a line or individual ring of an area feature\n\
such that the feature intersects itself along its perimeter (area) or segments (line)\n\
(exclusive of intersections at feature end points, including line features) - the intersection\n\
test is performed in 3D for line features and 2D for area features";
       break;
  

      case P_O_LOOP: /*** self-intersecting line that includes P & O formations using end nodes - lines only ****/
       ConditionLookup[i].name = "Line Feature Loop (any type)";
       ConditionLookup[i].description = "\
The primary participant is a line feature such that the feature\n\
intersects itself along its length and these intersections may involve the feature end\n\
nodes (unlike the inspection  \"Loop (Self-intersecting Area Or Line Feature)\")";
       break;


      case ENDPTINT:
       ConditionLookup[i].name = "Line With Same Start And End Node";
       ConditionLookup[i].description = "\
The primary participant is a line feature such\n\
that the line start and end nodes have the same coordinates ((x & y) for\n\
2D line features and (x, y, & z) for 3D line features)";
       break;

  
      case LATTRCHNG:
       ConditionLookup[i].name = "Connecting Lines With Different Attributes";
       ConditionLookup[i].description = "\
The primary participant is a line feature such that all connections\n\
between one or more of its end nodes to other line feature end nodes having the same\n\
feature type (e.g., Road, River, etc.) involve line features with different assigned\n\
attributes and / or attribute values (connections will be 3D for 3D features and 2D otherwise)";
       break;
  

      case G_DUPS:
       ConditionLookup[i].name = "Geometric Duplicates";
       if(NGA_TYPE > 0)
          {
          ConditionLookup[i].description = "\
The primary and secondary participants are two point, line, or area features\n\
that have exactly the same coordinates (all x,y,z vertex locations),\n\
but have different attribution and unique identifiers (UFI, GFID, FLDBID, or UID,\n\
as appropriate - this inspection is designed to not report conditions that would be\n\
reported by other inspections that require duplication of unique identifiers)";
          }
       else
          {
          ConditionLookup[i].description = "\
The participants are any features of the specified types, having two or more\n\
instances that identically replicate exact geometry (all x,y,z vertex locations)";
          }
       break;
  
  
      case C_DUPS:
       ConditionLookup[i].name = "Complete Duplicates";
       if(NGA_TYPE > 0)
         {
         ConditionLookup[i].description = "\
The primary participant describes two point, line, or area features that have\n\
exactly the same coordinates (all x,y,z vertices) and attribution (including assigned\n\
values), but have different unique identifiers (UFI, GFID, FLDBID, or UID, as appropriate - this\n\
inspection is designed to not report conditions that would be reported by inspections\n\
that require duplication of unique identifiers)";
          }
       else
         {
         ConditionLookup[i].description = "\
The primary participant is any feature of the specified type, having\n\
two or more instances that identically replicate exact geometry (all x, y, and z\n\
vertex locations) and attribution (including assigned values)";
          }
       break;
  

      case SAMEID_GDUP:
       ConditionLookup[i].name = "Geometric Duplicates - Same UFI/GFID/FLDBID/UID";
       ConditionLookup[i].description = "\
The primary and secondary participants are two point, line, or area features\n\
that have the same UFI, GFID, FLDBID, or UID (as appropriate) and exactly\n\
the same coordinates (all x,y,z vertices), but have different attribution";
       break;
  


      case SAMEID:
       ConditionLookup[i].name = "UFI/GFID/FLDBID/UID Duplication (different features, unique geometry)";
       ConditionLookup[i].description = "\
The primary and secondary participants are two point, line, or area features\n\
that have the same UFI, GFID, FLDBID, or UID (as appropriate), but do not have the same\n\
coordinates (this inspection excludes all conditions reported by the \"Complete Duplicates -\n\
Same UFI/GFID/FLDBID/UID\" and the \"Geometric Duplicates - Same UFI/GFID/FLDBID/UID\" inspections)";
       break;


      case SAMEID_CDUP:
       ConditionLookup[i].name = "Complete Duplicates - Same UFI/GFID/FLDBID/UID";
       ConditionLookup[i].description = "\
The primary participant describes two point, line, or area features\n\
that have the same UFI, GFID, FLDBID, or UID (as appropriate), the same coordinates\n\
(all x,y,z vertices) and the same attribution (including assigned values)";
       break;

      case ANY_SAMEID:  /** same unique identifier, except those that are complete duplicates ***/
       ConditionLookup[i].name = "Any Two Features With The Same Identifier";
       ConditionLookup[i].description = "\
The participants are point, line, or area features\n\
having the same UFI, GFID, FLDBID, or UID (as appropriate), including those that could\n\
also be reported as conditions by the \"Complete Duplicates - Same UFI/GFID/FLDBID/UID\" and / or \n\
the \"Geometric Duplicates - Same UFI/GFID/FLDBID/UID\" inspections)";
       break;


      case FACESIZE: /*** small area on face of area feature **/
         ConditionLookup[i].name = "Minimum Face Size Below Threshold";
         ConditionLookup[i].description =  "\
The primary participant, an area feature as specified, is not closer than the 4th tolerance to any\n\
part of a secondary participant feature and has a portion of its face that has a\n\
width less than the primary tolerance, in meters - the entire area feature square area must\n\
be greater than the secondary tolerance for the inspection to apply and the distance around the\n\
feature perimeter between opposing edges of the small face area must be greater than the\n\
tertiary tolerance";
         ConditionLookup[i].units = "meters";
         ConditionLookup[i].units2 = "square meters";
         ConditionLookup[i].units3 = "meters";
         ConditionLookup[i].units4 = "meters";
         break;

      case SLIVER:
       if(NGA_TYPE == 1)
         {
           ConditionLookup[i].name = "Sliver Area";
           ConditionLookup[i].description =  "\
The primary participant is an area feature\n\
having a ratio of area to perimeter less than the primary tolerance";
           ConditionLookup[i].units = "ratio";
         }
       else
         {
           ConditionLookup[i].name = "Sliver (triangular) Object";
           ConditionLookup[i].description = "This triangular object has a small ratio of width to height";
           ConditionLookup[i].units = "ratio";
         }
       break;

      case NARROW:
       ConditionLookup[i].name = "Narrow Object";
       ConditionLookup[i].description = "This triangular object has a small width";
       ConditionLookup[i].units = "meters";
       break;


      case SMALLOBJ:
       ConditionLookup[i].name = "Small Area Object";
       ConditionLookup[i].description = "This (assumed convex) object has a small 3D area";
       ConditionLookup[i].units = "square meters";
       break;
  

      case HSLOPE:
       ConditionLookup[i].name = "High Slope";
       ConditionLookup[i].description = "This (assumed) planar object has slope exceeding specified limit";
       ConditionLookup[i].units = "degrees";
       break;
  

      case VERTSLOPE:
       ConditionLookup[i].name = "Verticals";
       ConditionLookup[i].description = "This (assumed) planar object is vertical";
       break;


      case VTEAR:
       ConditionLookup[i].name = "Vertical Tear";
       ConditionLookup[i].description = "Adjacent polygons-areals (in 2D) do not match in elevation at this location ";
       ConditionLookup[i].units = "meters";
       break;
    


       case HTEAR:
       ConditionLookup[i].name = "Surface Hole";
       ConditionLookup[i].description = "A hole exists in the stratum surface greater than\nthe minimum specified sensitivity";
       ConditionLookup[i].units = "meters";
       break;
    

     case OVERC:
       ConditionLookup[i].name = "Overlapping Polygons";
       ConditionLookup[i].description = "More than two polygons share the same location and the horizontal extent of their overlap exceeds\n\
the minimum distance specified by the primary sensitivity and the vertical displacement between the two surfaces\n\
exceeds that specified by the secondary sensitivity -- these conditions are sorted based on vertical displacement";
       ConditionLookup[i].units = "meters";
       ConditionLookup[i].units2 = "meters";
       break;
  

      case TVERT:
       ConditionLookup[i].name = "'T'-Vertex";
       ConditionLookup[i].description = "Adjacent polygons do not share a vertex on the edge of adjacency";
       break;


      case LMINT:
       ConditionLookup[i].name = "Line - Model Intersection";
       ConditionLookup[i].description = "An intersection occurs between a line and a model";
       break;


      case LSPINT:
       ConditionLookup[i].name = "Line - Slope Polygon Intersection";
       ConditionLookup[i].description = "An intersection occurs between a line and a polygon with slope exceeding the specified limit";
       ConditionLookup[i].units = "degrees";
       break;


      case LSPIEXP:
       ConditionLookup[i].name = "Line - Slope Polygon Intersection With Exception";
       ConditionLookup[i].description = "An intersection occurs between a line and polygon meeting the given specifications\nunless the intersection is coincident to an excepted object";
       ConditionLookup[i].units = "degrees";
       break;


      case POLYINAREA:
       ConditionLookup[i].name = "Polygon Completely Inside Areal";
       ConditionLookup[i].description = "A polygon of the specified type(s) lies inside an area feature of given type";
       break;


     case POLYOSIDEAREA:
       ConditionLookup[i].name = "Polygon Not Completely Inside Areal";
       ConditionLookup[i].description = "A polygon of the specified type(s) is not\ncompletely inside any area feature of given type";
       break;


      case POLYINTPOLY:
       ConditionLookup[i].name = "Polygon - Polygon Intersection";
       ConditionLookup[i].description = "An intersection occurs between two polygons meeting the given specifications";
       break;


      case POLYINTAREA:
       ConditionLookup[i].name = "Polygon - Area Intersection";
       ConditionLookup[i].description = "An intersection occurs between an area feature and a polygon\n\
meeting the given specifications";
       break;
     }  /**** end switch ***/

    } /*** end for i ... **/

}




void SEEIT_FreeConditions(struct ConditionList * ListOfConditions)
{
  struct ConditionList * curr,* prev;
  
  curr = ListOfConditions;
  
  while(curr!=NULL)
    {
      prev=curr;
      curr=curr->next;
      
      if((prev->AdditionalPointUsed > 0) && (prev->AdditionalIDENT != NULL))
        free(prev->AdditionalIDENT);

      if(prev->num_areals>=1)
	{
          if(prev->areal1.IDENT != NULL)
	    free(prev->areal1.IDENT);
	  free(prev->areal1.x);
	  free(prev->areal1.y);
	  free(prev->areal1.z);
	}
      if(prev->num_areals==2)
	{
          if(prev->areal2.IDENT != NULL)
	    free(prev->areal2.IDENT);
	  free(prev->areal2.x);
	  free(prev->areal2.y);
	  free(prev->areal2.z);
	}
      if(prev->num_polys>=1)
	{          
	  if(prev->poly1.IDENT != NULL)
	    free(prev->poly1.IDENT);
	  free(prev->poly1.x);
	  free(prev->poly1.y);
	  free(prev->poly1.z);
	}
      if(prev->num_polys==2)
	{
	  if(prev->poly2.IDENT != NULL)
	    free(prev->poly2.IDENT);
	  free(prev->poly2.x);
	  free(prev->poly2.y);
	  free(prev->poly2.z);
	}
      if(prev->num_lines>=1)
	{
	  if(prev->linear1.IDENT != NULL)
	    free(prev->linear1.IDENT);
	  free(prev->linear1.x);
	  free(prev->linear1.y);
	  free(prev->linear1.z);
	}
      if(prev->num_lines==2)
	{
	  if(prev->linear2.IDENT != NULL)
	    free(prev->linear2.IDENT);
	  free(prev->linear2.x);
	  free(prev->linear2.y);
	  free(prev->linear2.z);
	}
      if(prev->num_points>=1)
        {          
	  if(prev->point1.IDENT != NULL)
	    free(prev->point1.IDENT);
        }
      if(prev->num_points==2)
        {
	  if(prev->point2.IDENT != NULL)
	    free(prev->point2.IDENT);
        }
      if(prev->num_circles>=1)
        {          
	  if(prev->circle1.IDENT != NULL)
	    free(prev->circle1.IDENT);
        }
      if(prev->num_circles==2)
        {
	  if(prev->circle2.IDENT != NULL)
	    free(prev->circle2.IDENT);
        }
      
      if(prev->num_cylinder>=1)
        {
	  if(prev->cylinder1.IDENT != NULL)
	    free(prev->cylinder1.IDENT);
        }
      if(prev->num_cylinder==2)
        {
	  if(prev->cylinder2.IDENT != NULL)
	    free(prev->cylinder2.IDENT);
        }
      
      if(prev->num_piped>=1)
        {
	  if(prev->piped1.IDENT != NULL)
	    free(prev->piped1.IDENT);
        }
      if(prev->num_piped==2)
        {
	  if(prev->piped2.IDENT != NULL)
	    free(prev->piped2.IDENT);
        }
      if(prev->num_gridpt>=1)
        {
	  if(prev->gridobj1.IDENT != NULL)
	    free(prev->gridobj1.IDENT);
          free(prev->gridobj1.x);
          free(prev->gridobj1.y);
          free(prev->gridobj1.z);
        }
      if(prev->num_gridpt==2)
        {
	  if(prev->gridobj2.IDENT != NULL)
	    free(prev->gridobj2.IDENT);
          free(prev->gridobj2.x);
          free(prev->gridobj2.y);
          free(prev->gridobj2.z);
        }
      
      if(prev->AttrErrMsg != NULL)
	free(prev->AttrErrMsg);
      
      if(prev->Message != NULL)
	free(prev->Message);
      
      
      free(prev);
    }
}




void DrawObjectMultiParts(char *filename, int KeyValueIndex, int ObjectID,int SpecificCondition)
{
  int ConditionKey;
  int ConditionCopy;
  int errcount;
  struct ConditionList * OneCondition;
  FILE *errfile;
  
  

  errfile = fopen(filename,"rb");
  
  fseek(errfile,ConditionLookup[KeyValueIndex].fileposn, SEEK_SET);
  SEEIT_fread_int(&ConditionKey, errfile);
  if(ConditionKey != ConditionLookup[KeyValueIndex].conditiontype)
    {
      printf("file condition alignment problem during file random access");
      printf("file position index %d read %d expected %d\n",
	     KeyValueIndex,ConditionKey,ConditionLookup[KeyValueIndex].conditiontype);
      fclose(errfile);
      return;
    }
  SEEIT_fread_int(&ConditionCopy,errfile);
  
  if(ConditionCopy != ConditionLookup[KeyValueIndex].instancenumber)
    { 
      printf("Misallignment in condition instance numbers read %d expected %d\n",
	     ConditionCopy,ConditionLookup[KeyValueIndex].instancenumber);
      fclose(errfile);
      return;
    }
  
  OneCondition = (struct ConditionList *)malloc(sizeof(struct ConditionList));
  if(OneCondition == NULL)
    {
      printf("available allocation memory has been exhausted\n");
      printf("execution must terminate at this point\n");
      exit(1);
    }
 
  OneCondition->CONDITION_TYPE      = ConditionLookup[KeyValueIndex].conditiontype;
  OneCondition->CONDITION_INSTANCE  = ConditionLookup[KeyValueIndex].instancenumber;
  OneCondition->num_areals          = 0;
  OneCondition->num_polys           = 0;
  OneCondition->num_lines           = 0;
  OneCondition->num_points          = 0;
  OneCondition->num_circles         = 0;
  OneCondition->num_gridpt          = 0;
  OneCondition->num_cylinder        = 0;
  OneCondition->num_piped           = 0;
  OneCondition->num_sphere          = 0;
  OneCondition->AdditionalPointUsed = 0;
  OneCondition->magnitude           = 0.0;
  OneCondition->ECC1                = -1;
  OneCondition->ECC2                = -1;
  OneCondition->AdditionalIDENT     = NULL;
  OneCondition->areal1.IDENT        = NULL;
  OneCondition->areal2.IDENT        = NULL;
  OneCondition->linear1.IDENT       = NULL;
  OneCondition->linear2.IDENT       = NULL;
  OneCondition->point1.IDENT        = NULL;
  OneCondition->point2.IDENT        = NULL;
  OneCondition->poly1.IDENT         = NULL;
  OneCondition->poly2.IDENT         = NULL;
  OneCondition->circle1.IDENT       = NULL;
  OneCondition->circle2.IDENT       = NULL;
  OneCondition->gridobj1.IDENT      = NULL;
  OneCondition->gridobj2.IDENT      = NULL;
  OneCondition->piped1.IDENT        = NULL;
  OneCondition->piped2.IDENT        = NULL;
  OneCondition->cylinder1.IDENT     = NULL;
  OneCondition->cylinder2.IDENT     = NULL;
  OneCondition->AttrErrMsg          = NULL;
  OneCondition->Message             = NULL;
  OneCondition->next                = NULL;
  
  
  --SpecificCondition;
  
  for(errcount = 0; errcount < ConditionLookup[KeyValueIndex].number; errcount++)
    {
      switch(ConditionLookup[KeyValueIndex].conditiontype)
	{
	case MULTIPARTA:  /** multi-part area ***/
	  OneCondition->magnitude = FreadObjectAndMagnitude(ConditionKey, errfile, OneCondition);
	  if((OneCondition->num_areals > 0) && (errcount == SpecificCondition))
	    {
	      /** draw in bold **/
	      drawboldpoly(OneCondition->areal1.x,OneCondition->areal1.y,
			   OneCondition->areal1.numverts,1);
	    }
	  
	  else if((OneCondition->num_areals > 0) && (OneCondition->areal1.objectid == ObjectID))
	    {
	      drawpoly(1,OneCondition->areal1.x,OneCondition->areal1.y,
                       OneCondition->areal1.numverts,1,1);
	    }
	  
	  OneCondition->num_areals = 0;
	  
	  break;
	  
	case MULTIPARTL: /** multi-part line **/
	  OneCondition->magnitude = FreadObjectAndMagnitude(ConditionKey, errfile, OneCondition);
	  if((OneCondition->num_lines > 0) && (errcount == SpecificCondition))
	    {
	      /** draw in bold **/
	      drawboldfullline(OneCondition->linear1.x,OneCondition->linear1.y,OneCondition->linear1.numverts,1);
	    }
	  
	  else if((OneCondition->num_lines > 0) && (OneCondition->linear1.objectid == ObjectID))
	    {
	      drawfullline(OneCondition->linear1.x,OneCondition->linear1.y,OneCondition->linear1.numverts,1);
	    }
	  
	  OneCondition->num_lines = 0;
	  
	  break;


	case MULTIPARTP: /** multi-part point **/
	  OneCondition->magnitude = FreadObjectAndMagnitude(ConditionKey, errfile, OneCondition);
	  if((OneCondition->num_points > 0) && (errcount == SpecificCondition))
	    {
	      /** draw in bold **/
	      drawBoldX(OneCondition->point1.x,OneCondition->point1.y,1);
	    }
	  
	  else if((OneCondition->num_points > 0) && (OneCondition->point1.objectid == ObjectID))
	    {
	      drawX(OneCondition->point1.x,OneCondition->point1.y,1);
	    }
	  
	  OneCondition->num_points = 0;
	  
	  break;

	  
	default:
	  printf("unexpected condition type submitted for complete parts drawing\n");
	  
	}
      SEEIT_fread_int(&ConditionKey, errfile);
      if(ConditionKey != ConditionLookup[KeyValueIndex].conditiontype)
	{
	  break;
	}
      SEEIT_fread_int(&ConditionCopy,errfile);
    }
  
  free(OneCondition);
  
  fclose(errfile);
}




void SetLK2(struct ConditionList * ListOfConditions,FILE *lookfile2)
{
  struct ConditionList * temp;
  int i,totalentries;
  int *errnums,*instances,*numbers;
  long int *fileposns;
  int lastinstance;
  int lastcondtype;
  int lastnumber;
  int foundone;

  if(ListOfConditions==NULL)
    {
      return;
    }

  
  SEEIT_fread_int(&totalentries,lookfile2);

  errnums   = (int *)     malloc(SzI * totalentries);
  instances = (int *)     malloc(SzI * totalentries);
  numbers   = (int *)     malloc(SzI * totalentries);
  fileposns = (long int *)malloc(SzL * totalentries);

  for(i=0;i<totalentries;i++)
    {
      SEEIT_fread_int (&errnums  [i], lookfile2);
      SEEIT_fread_int (&instances[i], lookfile2);
      SEEIT_fread_int (&numbers  [i], lookfile2);
      SEEIT_fread_long(&fileposns[i], lookfile2);
    }

  
  temp = ListOfConditions;
  lastcondtype = -100;
  lastinstance = -100;
  lastnumber   = -100;
  
  while(temp!=NULL)
    {
      if(
	 (temp->CONDITION_TYPE    == lastcondtype)  &&
	 (temp->CONDITION_INSTANCE== lastinstance)  &&
	 (temp->ordinal_num       ==(lastnumber+1))
	 )
	{
	  /* then we can just read without seeking */
	  SEEIT_fread_int(&temp->retainignore,lookfile2);
	}
      else
	{
	  /* find this condnum,instance in our list */
	  foundone = 0;
	  for(i=0;i<totalentries;i++)
	    {
	      if(
		 (errnums[i]==temp->CONDITION_TYPE) &&
		 (instances[i]==(temp->CONDITION_INSTANCE+1))
		 )
		{
		  foundone = 1;
		  break;
		}
	    }
	  if(foundone==0)
	    {
	      printf("could not find condtype %d instance %d\n",
		     temp->CONDITION_TYPE,temp->CONDITION_INSTANCE+1);
	      printf("aborting attempt to load retain info\n");
	      free(errnums);
	      free(instances);
	      free(fileposns);
	      free(numbers);
	      return;
	    }
	  fseek(lookfile2,fileposns[i]+SzI*(temp->ordinal_num-1),SEEK_SET);

	  SEEIT_fread_int(&temp->retainignore,lookfile2);
	}
      lastinstance = temp->CONDITION_INSTANCE;
      lastcondtype = temp->CONDITION_TYPE;
      lastnumber   = temp->ordinal_num;
      

      temp = temp->next;
    }

  free(errnums);
  free(instances);
  free(fileposns);
  free(numbers);
}



int GetRevisedindex(int CONDITIONTYPE)
{
  int i;
  
  for(i=0;i<TotalConditions;i++)
    {
      if(CONDITIONTYPE==ConditionLookup[i].conditiontype)
	{
	  return i;
	}
    }
  printf("SEEIT_API: error!  could not locate condition type %d in the list!\n",CONDITIONTYPE);
  return 0;
}










struct ConditionList * SEEIT_GetConditions(char *dirname,
					   int ConditionsToUse[CONDITION_ARRAY_SIZE],
					   int SpecificConditionBegin,
					   int SpecificConditionEnd,
					   struct AdditionalConditions *AdditionalConditionFlags, 
					   int NO_DRAW,
					   int NO_TEXT_MESSAGE,
					   int DRAW_ONLY,
					   int SpecArray[],
					   int specnum
					   )
{
  int Sortkey;
  int ConditionCopy;
  int i,j,curr_index=0,skipcondition;
  int errcount;
  int DrawRelatedParts = 0;
  int RelatedPartObjectID;
  int RelatedPartKeyValueIndex;
  struct ConditionList * newnode, *tail;
  struct ConditionList * ListOfConditions=NULL;
  struct AdditionalConditions *ACF; 
  char nsname[1000],lk2name[1000];
  FILE *errtypeinout,*lk2;
  
  
  for(i=0; i<TotalConditions; i++)
    {
      ConditionLookup[i].selected = 0;
    }
  
  UniqueMagsFound = 0;
  LastMagFound    = -1;
  
  for(i=1;i<=CONDITION_DEFINITIONS;i++)
    {
      if((ConditionsToUse[i]==0)||(ConditionsToUse[i]==1))
	{
          for(j=0; j<TotalConditions; j++)
	    {
	      if((ConditionLookup[j].conditiontype == i) && 
		 (ConditionLookup[j].instancenumber == 0))
                {
		  ConditionLookup[j].selected = ConditionsToUse[i];
                }
	    }
	}
      else
	{
	  printf("SEEIT_GetConditions: error.\n");
	  printf("called with %d set for \"Conditions to use\" #%d\n",ConditionsToUse[i],i);
	  printf("this should be a 0 or a 1.  returning abnormally.\n");
	  return NULL;
	}
    }
  
  if((specnum>0)&&(SpecificConditionBegin>0))
    {
      printf("SEEIT_GetConditions: error\n");
      printf("called with specnum %d and SpecificConditionBegin %d\n",
	     specnum,SpecificConditionBegin);
      return NULL;
    }
  
  
  
  ACF = AdditionalConditionFlags;
  while(ACF != NULL)
    {
      for(j=0; j<TotalConditions; j++)
        {
	  if((ACF->condition_number == ConditionLookup[j].conditiontype) &&
             (ACF->additional_instance_number == ConditionLookup[j].instancenumber))
	    {
	      ConditionLookup[j].selected = 1;
	    }
        }
      ACF = ACF->next;
    }
  
  
  sprintf(nsname,  "%s/condreport.ns"  ,dirname);
  errtypeinout = fopen(nsname,"rb");  
  if(errtypeinout==NULL)
    {
      printf("couldn't open %s\n",nsname);
      printf("be sure the first argument passed to this executable is a directory\n");
      printf("containing the file condreport.ns.1 (binary condition output file)\n");
      exit(1);
    }
 



  sprintf(lk2name,  "%s/condreport.lk2"  ,dirname);
  lk2 = fopen(lk2name,"rb");  
  if(lk2==NULL)
    {
      printf("couldn't open %s for read\n",lk2name);
      exit(1);
    }
  
  
  
  for(i=0; i<TotalConditions; i++) 
    {
      
      if((ConditionLookup[i].selected == 1) && (ConditionLookup[i].number > 0))
	{
	  fseek(errtypeinout,ConditionLookup[i].fileposn,SEEK_SET);
	  SEEIT_fread_int(&Sortkey,errtypeinout);
	  
	  if(Sortkey != ConditionLookup[i].conditiontype)
            {
	      printf("bad %d at %d expected count %d file position %ld\n",
		     Sortkey,i,ConditionLookup[i].number,ConditionLookup[i].fileposn);
	    }
	  
	  
          SEEIT_fread_int(&ConditionCopy,errtypeinout);
	  
          if(ConditionCopy != ConditionLookup[i].instancenumber)
            {
	      printf("Misallignment in condition instance numbers read %d expected %d\n",
		     ConditionCopy,ConditionLookup[i].instancenumber);
	      exit(1);
            }
	  
     
 
	  
	  for(errcount=0; errcount<ConditionLookup[i].number; errcount++)
            {
	      


	      global_fileposn = ftell(errtypeinout);
	      if((use_other_fileposn>=0)&&(errcount==0))
		{                 
		  SpecificConditionBegin = errcount+1;
		  SpecificConditionEnd   = errcount+1;
		  fseek(errtypeinout,use_other_fileposn,SEEK_SET);
		}
	      else if(use_other_fileposn>=0)
		{
		  break;
		}



	      
	      
	        if((SpecificConditionBegin > 0) && (errcount >= SpecificConditionEnd))
			{
		      break;
			}
	        else if( (specnum>0) && (curr_index>=specnum))
			{
		     /* we're processing an array of indices, and have finished it */
		     break;
			} 

	      
	      if((SpecificConditionBegin<=0)&&(specnum<1))
		{
		  skipcondition = 0;
		}
	      else if (SpecificConditionBegin>0)
		{
		  
		  if(
		     ((errcount+1) >= SpecificConditionBegin)  &&
		     ((errcount+1) <= SpecificConditionEnd)
		     )
		    {
		      skipcondition = 0;
		    }
		  else
		    {
		      skipcondition = 1;
		    }
		}
	      else if(specnum>0)
		{

		  if((errcount+1)==SpecArray[curr_index])
		    {
		      skipcondition = 0;
		      curr_index = curr_index + 1;
		    }
		  else
		    {
		      skipcondition = 1;
		    }
		}




	      if(skipcondition==0)		
                {
		  newnode = (struct ConditionList *)malloc(sizeof(struct ConditionList));
		  if(newnode == NULL)
		    {
		      printf("available allocation memory has been exhausted\n");
		      printf("execution must terminate at this point\n");
		      exit(1);
		    }
		  newnode->CONDITION_TYPE      = ConditionLookup[i].conditiontype;
          newnode->CONDITION_INSTANCE  = ConditionLookup[i].instancenumber;
		  newnode->ordinal_num         = errcount + 1;
		  newnode->retainignore        = 1;
		  newnode->num_areals          = 0;
		  newnode->num_polys           = 0;
		  newnode->num_lines           = 0;
		  newnode->num_points          = 0;
		  newnode->num_circles         = 0;
		  newnode->num_gridpt          = 0;
		  newnode->num_cylinder        = 0;
		  newnode->num_piped           = 0;
		  newnode->num_sphere          = 0;
		  newnode->AdditionalPointUsed = 0;
		  newnode->magnitude           = 0.0;
		  newnode->ECC1                = -1;
		  newnode->ECC2                = -1;
		  newnode->AdditionalIDENT     = NULL;
		  newnode->areal1.IDENT        = NULL;
		  newnode->areal2.IDENT        = NULL;
		  newnode->linear1.IDENT       = NULL;
		  newnode->linear2.IDENT       = NULL;
		  newnode->point1.IDENT        = NULL;
		  newnode->point2.IDENT        = NULL;
		  newnode->poly1.IDENT         = NULL;
		  newnode->poly2.IDENT         = NULL;
		  newnode->circle1.IDENT       = NULL;
		  newnode->circle2.IDENT       = NULL;
		  newnode->gridobj1.IDENT      = NULL;
		  newnode->gridobj2.IDENT      = NULL;
		  newnode->piped1.IDENT        = NULL;
		  newnode->piped2.IDENT        = NULL;
		  newnode->cylinder1.IDENT     = NULL;
		  newnode->cylinder2.IDENT     = NULL;
		  newnode->AttrErrMsg          = NULL;
		  newnode->Message             = NULL;
		  newnode->next                = NULL;
		  
		 

		  
                  Sortkey = ConditionLookup[i].conditiontype;
		  switch(ConditionLookup[i].conditiontype)
		    {
		      /** first groups below have non-sorted conditions not involving a condition magnitude **/
		    case G_DUPS: /** duplicate poly (by x,y,z of vertices) **/
		    case C_DUPS: /** complete duplicate, attribution and all **/
                    case SAMEID: /** same GFID or FLDBID, diff geom **/
                    case SAMEID_GDUP:  /** same GFID or FLDBID, same geom, diff attr ***/
                    case SAMEID_CDUP: /** same GFID or FLDBID, geom & attr ***/
                    /**case AGEOM_UNM_LAT:
                    case AGEOM_UNM_LON:**/
                    case ANY_SAMEID:  /** same unique identifier, except those that are complete duplicates ***/
		    case POLYINTPOLY: /** two polygons, of selected types, intersect **/
		    case POLYINAREA: /** polygon lies wholly inside an areal **/
		    case PTINREGION:  /** point feature inside a typed polygon or areal **/
                    case PTINPROPER:  /** point feature inside an area feature - not within tolerance of edge (or edge or hole) **/
                    case ACOVERA: /* area covered by area ***/
                    case FAILMERGEA:  /** area feature that should be merged with area that shares edge ***/
                    case FAILMERGEA2:  /** area feature that should be merged with area that shares edge - no accounting for metadata  ***/
                    case AINSIDEHOLE: /** area inside another areal's cutout ('illegal holes') ***/
                    case FEATNOTCUT:  /*** feature not cut at end node of second feature ***/
                    case ISOLINE:  /** line feature completely inside an area feature ***/
                    case LINSIDEA: /** line partly or entirely inside area feature ***/
                    case LSEGCOVERA: /** line segment overlaps an area feature perimeter ***/
		      FreadTwoObjects(Sortkey,errtypeinout,newnode);
		      break;

                    case LGEOM_UNM_LAT:
                    case LGEOM_UNM_LON:
                    case AGEOM_UNM_LAT:
                    case AGEOM_UNM_LON:
                    case AUNM_ATTR_A:
                    case LUNM_ATTR_A:
                      FreadMsgMagPointObjects(Sortkey,errtypeinout,newnode);
                      break;

                    case MASKCONSTANT:
                    case MASKZERO: /** DEM not zero elev at point defined by specified mask value ***/
                    case MASKEDIT_0: /** Raw DEM and Edited DEM different & Edit Mask has value zero**/
                    case MASKEDIT_1: /** EDM has primary tolerance value, diff between TDR and TDF is > secondary tolerance **/
                    case MASKCONF2: /** variation of Grids with conflicting values **/
                    case MASKCONFLICT: /** Grid DEM Masks have conflicting values ***/
                    case PT_GRID_DIF: /** point and grid z value mismatch at exact coord, no interpolation **/

                    case RAISEDPC: /** number of raised shoreline points exceeds tolerance **/
                    case FLOWSTEP:  /** step size in river flow above threshold ***/
                    case BREAKLINE: /** river elevation change at bad angle with shorelines ***/
                    case WATERMMU: /** minimum mapping unit for water body below threshold ***/
                    case AVGSPIKE: /** spike / well as compared to average elevation of neighbor posts ***/
                    case GSHELF:  /** looking for shelf formations like PUE in DEM ***/
                    case AWITHOUTA:  /** area that does not fully contain a second area ***/
                    case LOSMINHGT:
                    case ELEVGT:
                    case ELEVLT: 
                    case ELEVEQ:  
                    case ELEVEQOPEN:
                    case FEATOUTSIDE:  /*** a feature lies at least partly outside the MGCP cell ***/
                      FreadDynamicInfo(Sortkey,errtypeinout,newnode);
                      break;

		      
		    case LLINT: /** line - line inetrsection **/
                    case BADFEATCUT: /** feature cut when no need ***/
                    /**case LGEOM_UNM_LAT:
                    case LGEOM_UNM_LON:**/
		    case LLIEX: /** line - line except intersection **/
                    case LAIEX: /** line - area intersection with 3rd feature exception ***/
		    case LMINT: /** line - model intersection **/
                    case LAINT:  /* line - areal intersection **/
                    case LAINTNOEND: /** line - area intersection not at line end node ***/
                    case LACUTFAIL:  /** line not cut at intersection with area perimeter **/
                    case LEAON_NOTIN: /** line end node on area edge, line not inside area ***/
                    case POLYINTAREA: /* polygon - areal intersection of edges **/
                    case AREAINTAREA: /* areal objects intersect */
                    case PART_ISF: /** two area features have intersecting edges and share part of their faces **/
                    case CUT_INT: /** cut-out intersects parent feature outer ring ***/
                    case LLNONODEINT: /* features intersect, but not at a shared node **/
                    case NONODEOVLP: /** line, area have overlapping edge without common node ***/
                    case LLNOENDINT: /** lines intersect, but not at end point **/
                    case LLINTAWAY: /** two lines intersect, and cross over each other ***/
                    case LLINTNOEND: /** two lines intersect, pt of intersection is away from either primary particpant end node ***/
                    case P_O_LOOP: /*** self-intersecting line that includes P & O formations using end nodes - lines only ****/
                    case AOVERLAPA: /** overlapping area features (second can also be inside first) **/
		      FreadPointAndTwoObjects(Sortkey,errtypeinout,newnode);
		      break;
		    		      
		    case VERTSLOPE: /** vertical poly **/
		    case PLPFAIL: /** point oline coincidence failure **/
                    case PNOCOVERLE:  /** point not covered by lionear end node ***/
                    case PNOCOV2LEA: /** point not covered by 2 line terminal nodes or area edges***/
                    case PNOCOVERLV:  /** point not covered by any line vertex  **/
		    case POLYOSIDEAREA: /** Poly completely outside all areals of given type **/
		    case PTOSIDEREGION: /** point feature not inside any typed areal or poly **/
                    case OBJECTWITHOUT: /** poly or areal without a point or linear inside **/
                    case OBJ_WO_TWO: /** area contains secondary P,A,L but not tertiary P,A,L ***/
                    /***case AWITHOUTA:  area that does not fully contain a second area ***/
                    case FSFAIL: /*** face sharing failure ***/
                    case PSHAREFAIL:  /*** an area feature fails to share any of its perimeter with a 2d area feature ***/
                    case NOCOINCIDE: /** area without line end node or segment on its perimeter ***/
                    case V_DUPS: /** duplicate vertices inside object **/
                    case LNOCOVERLA: /** line not covered by linear or areal ***/
                    case LSPANFAIL: /** line not covered by face of doesnt span between edges ***/
                    case LNOCOV2A:  /** line not covered by edges of 2 area features ***/
                    /**case ISOLINE:  ** line feature completely inside an area feature ***/
                    /**case LINSIDEA: ** line partly or entirely inside area feature ***/
                    case LEINSIDEA: /** line end node properly inside an area ***/
                    case COINCIDEFAIL: /** line or area feature segment fails to coincide with 2 other line or area features **/
                    case ISOLATEDA:  /*** area feature does not instersect another area or a line feature ***/
                    case NETISOA: /** like ISOLATEDA except allowed a transitive connection through other like features ***/
                    case ANETISOA: /** area not trans connected to another area by shared edges ***/
                    case NETISOFEAT: /** form a network - check for nets with one feature, but not another ***/
                    case MULTIDFEAT: /** single line or area with both 2 and 3 D coordinates ***/
                    case MULTISENTINEL: /** single line or area has more than one sentinel z value ***/
                    case CONNECTFAIL: /** point, line, or area feature without 'connection' to specified 2nd feature **/
                    /***case FEATOUTSIDE:  *** a feature lies at least partly outside the MGCP cell ***/
                    case HIGHLIGHTED: /** feature is on the highlight list from view by attribution ***/
                    case ANOCOVERLA:  /** areal not covered by linear or areal ***/
                    case QUALANOCOVLA: /** area permin not covered by line or area AND is inside a third area ***/
                    case ANOCOVERA: /** area not covered by second area feature **/
                    case OVERUNDER: /** any feature outside a perimeter-defining area or a line end node undershooting it **/
                    case AMCOVAFAIL: /** area not coverer by adjoining areas **/
                    case CUTOUT:   /** simply identifies a cut-out of an area feature ***/
                    case PORTRAYF:  /** write feature that fails all MGCP4 portrayal rules ***/
                    case TPORTRAYF: /** write feature that fails all TDS6 portrayal rules ***/
                    case MASKMONO: /** DEM not monotonic at point defined by specified mask value ***/
                    case LLNOINT: /** line - line intersection failure ***/
                    case LFNOINT: /** line fails to intersect another line, area, or point and no end node on 1/4 degree line ***/
                    case PLLPROXFAIL: /*** point line intersection coincidence failure ***/
                    case AHANG_LON: /** hanging area feature at a specified longitude meridian ***/
                    case AHANG_LAT: /** hanging area feature at a specified latitude parallel ***/
                    case AUNM_ACRS_A: /** area feature edge incorrectly matched across a bounding area feature ***/
		      FreadObject(Sortkey,errtypeinout,newnode);
		      break;
		      
		    case LOUTSIDEA: /** linear vertex falls outtside areal **/
                    case LLAINT: /** line - line endpt connect at area perimeter **/
                    case L_NOTL_AINT: /** line end point connects to 'not type line' at area perimeter **/
		    case TVERT: /** 'T' vertex **/
                    case LENOCOVERP: /** linear end node not covered by point feature ***/
                    case ENCONFAIL: /** end node connectivity failure **/
                    case NOENDCON: /** both end nodes of a line fail to connect or be covered **/
                    case BOTHENDCON: /** both end nodes of a line feature are covered by specified-type point features **/
                    case LENOCOVERL:  /*** line end node not within tolerance distance to another line ***/
                    case NOLCOVLE: /**line end node not within tolerance distance to another line, including itself ***/
                    case LOOPS: /*** self-intersecting linear or areal ***/
                    case COLINEAR: /** 3 consecutive vertices on line or area perim are collinear - middle one is not connecting node ***/
                    case ENDPTINT: /** line endpoints are the same ***/
                    case L_UNM_A:  /*** line endpt unmatched at area feature boundary ***/
                    case LSAME_UNM_A: /*** line endpt unmatched with line of same FCODE at Area boundary ***/
                    case LUNM_ACRS_A: /*** line mismatch across poly edge ***/
                    case LUNMA_ACRS_A: /** line end not matched to area node across area perimeter ***/
                    case LHANG_LON: /** hanging line feature at a specified longitude meridian ***/
                    case LHANG_LAT: /** hanging line feature at a specified latitude parallel ***/
                    case LE_A_UNM_LON: /** line end node not coincident with area node at longitude meridian **/
                    case LE_A_UNM_LAT: /** line end node not coincident with area node at latitude parallel **/
                    case LATTRCHNG:  /** line end point connects to same fdcode line, but attributes differ between the 2 features **/
                    case KICKBACK:  /** 180 degree kink ***/
		      FreadPointAndObject(Sortkey,errtypeinout,newnode);
		      break;
		      
                    case ATTR_PAIR: /*** NGA unexpected fcode - geom pair ***/
                    case ATTR_UNEXP: /** NGA unexpected attribute assigned ***/
                    case RPTD_ATTR: /** attribute error as reported  ****/
                    case CONFLATE: /*** line is unique among conflation sets of data ***/
/**case CONF_STATS: **** just generates conflation-information statistics, etc - no conditions to read/write **/
                    case ATTR_VVT: /*** attribute dependency violation  **/
                    case ATTR_RNULL: /*** MGCP Required attribute assigned NULL value ***/
                    case ATTR_MISSING: /** missing a required attribute ***/
                    case ATTR_DT:  /** NGA - datatype encountered not as presecribed **/
                    case ATTR_RNG: /** NGA attribute value range violation ***/
                    case ATTR_PICK: /** NGA - pick list allowed domain violation **/
                    case ATTR_META: /** NGA - GIFD D4 metadata violation ***/
                    case VVTERR1WAY: /** feature with designated attribute & value ***/
                    case VVTERR2WAY:  /** valid value table type error ***/
                    case VVTERR3WAY:  /*** valid values conflict between 3 attribute of a single feature ***/
		    case ATTRERR: /*** attribution error **/
		      FreadObjectAndMessage(Sortkey,errtypeinout,newnode);
		      break;
		      
		      /** next groups are for magnitude-related conditions **/

                    case AREAKINK: /** high angle on perimeter of area eature **/
                    case INCLSLIVER: /** areal with included sliver **/
                    case ZUNCLOSED: /** area feat not closed in Z **/
                    case AREAUNCLOSED: /** area feature unclosed in any dimension **/
                    case NOT_FLAT:  /*** area feature with surface that is not uiform elevation ***/
                    case CLAMP_NFLAT: /** area feature does not have constant elevation when clamped to underlying DEM ***/
                    case CLAMP_DIF: /** difference between feature vertex z value and interpolated DEM value ***/
                    /**case LAINTNOEND: ** line - area intersection not at line end node ***/
                      newnode->magnitude = FreadPointObjectAndMagnitude(Sortkey, errtypeinout,newnode);
                      break;
		      
                    case ISOTURN: /** high turn angle w/o 3d feature present ***/
		    case KINK:  /** high angle between adjacent linear segments **/
                    case Z_KINK: /** consecutive kinks form a 'Z' ***/
                    case L_A_KINK: /** kink at intersection of line end node  and area feature perim **/
                    case INTERNALKINK: /** kink internal to single line feature **/
		    case SLOPEDIRCH: /*** slope direction change along linear **/
                    case CLAMP_SDC: /*slope direction change along a line that has been elevation-value clamped to underlying DEM ***/
                    case CLAMP_JOINSDC: /** slope direction change at line feature connection when both are clamped to DEM ***/
                    /**case LOOPS: ** self-intersecting linear or areal ***/
		    case SLIVER: /** sliver triangle **/
                    case FACESIZE: /*** small area on face of area feature **/
                    case ARNGE_UNM_LAT:
                    case ARNGE_UNM_LON:
		    case NARROW:  /** narrow triangle **/
		    case SMALLOBJ:  /** small 3d area poly **/
		    case HSLOPE: /** high slope poly **/
		    case HTEAR: /** horizontal tear **/
		    case OVERC: /** over-lapping edges **/
		    case GSPIKE:  /** grid data spike point ***/
                    /***case AVGSPIKE: ** spike / well as compared to average elevation of neighbor posts ***/
                    /*** case FLOWSTEP:  ** step size in river flow above threshold ***/
                    /**case WATERMMU: ** minimum mapping unit for water body below threshold ***/
                    /**case RAISEDPC: ** number of raised shoreline points exceeds tolerance **/
                    case GRID_STD_DEV: /** grid elev value, inside feature polygon, over range offset from std deviation **/
/*************************
		    case ELEVGT:  
		    case ELEVLT:  
                    case ELEVEQ:  
                    case ELEVEQOPEN:  
********************************/
                    case ELEVADJCHANGE:  /** change in adjacent node elevations > threshold ***/
                    case FEATSPIKE: /** elevation spike along 3D feature ***/
                    case SEGLEN: /** areal or linear feature segment length below threshold ***/
                    case LONGSEG: /** linear or areal perimeter segment with length at or above threshold ***/
                    case BIGAREA: /** area feat in excess of set square area **/
                    case SMALLAREA:  /** area feat with small square area **/
/**case MASKMONO: *** DEM not monotonic at point defined by specified mask value ***/
                    case SMLCUTOUT: /** small included area inner ring of area feature ***/
                    case OSIDE_LAT:   /**** feature coordinate above or below latitude range    **/
                    case OSIDE_LON:   /**** feature coordinate above or below longitude range    **/
                    case BNDRYUNDERSHT: /** feature undershoots whole degree project outside boundary ***/
                    case LBNDUSHT:  /** unconnected line end node undershoots whole-degree boundary ***/
                    case PERIMLEN: /** areal perimeter or ttl line len below threshold ***/
                    case SHORTFEAT:  /** short length line feature not on quarter degree 'boundary' ***/
                    case LONGFEAT:   /** line or area feature with total length above threshold ***/
                    case SHARE3SEG: /** line feature segment overlaps 2 other line or area feature segments ***/
                    case PC_SLOPE: /*** line feature segment with percent slope above tolerance ****/
                    case CALC_AREA:  /*** point feature with LEN and WID attr values product < tolerance ***/
                    case COVERFAIL: /** to detect holes in surface; MGCP landcover requirement ***/

                      newnode->magnitude =  FreadObjectAndMagnitude(Sortkey, errtypeinout,newnode);
                      break;

                    case MULTIPARTL:  /** multi-part line ***/
                    case MULTIPARTA: /** multi-part area **/
                    case MULTIPARTP: /** multi-part point **/

                      newnode->magnitude =  FreadObjectAndMagnitude(Sortkey, errtypeinout,newnode);
                      if((SpecificConditionBegin > 0) && (SpecificConditionBegin == SpecificConditionEnd))
			{
			  DrawRelatedParts = 1;
			  RelatedPartKeyValueIndex = i;
			  if(ConditionLookup[i].conditiontype == MULTIPARTL)
                            RelatedPartObjectID = newnode->linear1.objectid;
			  else if(ConditionLookup[i].conditiontype == MULTIPARTA)
                            RelatedPartObjectID = newnode->areal1.objectid;
			  else if(ConditionLookup[i].conditiontype == MULTIPARTP)
                            RelatedPartObjectID = newnode->point1.objectid;
			}
		      break;
		      
		    case LVPROX: /** line vertex near another line **/
                    case LENOCOVERA: /** line end node not covered by area perimeter ***/
		    case PLPROX: /** point feature within x of a line feature **/
                    case PSHOOTL: /*** point feature over or undershoots a line feature ***/
                    case PLPROXEX:  /** pt to line prox with exception for line end node ***/
                    case ENCONNECT: /** linear end node connectivity ***/
                    case BADENCON: /** bad sequence on line feature connections ***/
                    case FEATBRIDGE:
		      newnode->magnitude = FreadPointEdgeAndMagnitude(Sortkey, errtypeinout,newnode);
		      break;
		      
		    case VTEAR: /** vertical tear **/
                    case LELINEPROX:  /** line end node and line (vertex, segment ...) proximity ***/
                    case EN_EN_PROX:  /** undershoot end nodes connected by another feature **/
                    case LUNDERSHTL:  /** line end - line undershoot **/
                    case LUSHTL_CLEAN: /* like line - line undershoot, but no condition if feature mid-undershoot **/
                    case LVUSHTL: /** interior line vertex undershoots a different line feature **/
                    case VUSHTL_CLEAN: /* like vertex - line undershoot, but no condition if feature mid-undershoot **/
                    case LVOSHTL: /** interior line vertex overshoots a different line feature ***/
                    case LUSHTL_DF: /** line - line undershoot, different line feature types ***/
                    case LOSHTL_DF: /** line - line overshoot, different line feature types ***/
                    case LOVERSHTL:   /** line end - line overshoot **/
                    case LUNDERSHTA:  /** line end area perimeter undershoot **/
                    case LOVERSHTA:  /** line end - area perimeter overshoot **/
                    case LAPROX:  /** line to area proximity - smallest dist between the two features ***/
                    case LASLIVER: /** sliver formed between line and area features **/
                    case LSLICEA: /** line 'slices' area so as create a small piece ***/
                    case LLSLIVER:  /** sliver formed between two line features **/
                    case AUNDERSHTA: /** area edge undershoots neighbor area edge ***/
                    case AOVERSHTA: /** area edge overshoots neighbor area edge ***/
                    case LLMULTINT: /** lines intersect each other multiple times **/
                    case LOC_MULTINT: /** lines with no or compatible LOC values intersect each other multiple times **/
                    case L2D_L3D_MATCH: /** linear end node Z mismatch ***/
                    case LEZ_PROX_3D: /** apply check L2D_L3D_MATCH to 3d line features only **/
                    case CNODE_ZBUST:  /*** Z mismatch between any two connecting nodes (in x,y) ***/
                    case DUPLICATESEG: /*** duplicated segments ***/
                    case EXTRA_NET:   /*** vertex is a near miss in connecting to another vertex to join networks ***/
                    case INTRA_NET:   /*** vertex is close to but not identical with another vertex in the same network ***/
                    case LJOINSLOPEDC: /** slope direction change along linear **/
                    case CONTEXT_KINK:  /*** kink based on one high angle next to one lower (moderate) angle ***/
                    case FAILMERGEL: /** line feature that should be merged with a connected line feature **/
                    case FAILMERGEL2:  /** line object that should be merged with connecting line no accounting for metadata  ***/
                    case LRNGE_UNM_LAT:
                    case LRNGE_UNM_LON:
                    case SHAREPERIM: /** areals share perimeter segments ***/
		    case LSPINT: /** line intersects poly with slope > x **/
                    case SHARESEG: /** line feature segment overlaps 1 other line feature segment ***/
                    /*** case SHARE3SEG:  old way  ** line feature segment overlaps 2 other line feature segments ***/
                    case LLI_ANGLE: /*** 2 lines intersect at severe angle ***/
		    case LSPIEXP: /** line - poly (slope > x) except when intersection **/
		    case PTPTPROX: /** point to point proximity **/
                    case PUNDERSHTA: /** point not on area perimeter and is outside that area feature **/
                    case POVERSHTA: /** point not on area perimeter and is inside that area feature **/
                    case LODELEVDIF: /** elev difference between grids in different LOD **/
                    case GRIDEXACTDIF: /** Grids have post value difference at same X,Y ***/
                    case MASKSHOREL: /** water body not contained by shoreline ***/
                    case CLAMP_SEG: /*** catenary segment below associated DEM ****/
		      newnode->magnitude = FreadMagnitudeAndTwoObjects(Sortkey, errtypeinout,newnode);
		      break;
		      
		    default:
		      printf("bad Sortkey (%d) in condition file\n",Sortkey);
		      exit(1);
		    }  /** end switch on Sortkey ***/

			


		    if(ListOfConditions==NULL)
		    {
		      tail = newnode;
		      ListOfConditions = tail;
		    }
		    else
		    {
		      tail->next = newnode;
		      tail = newnode;
		    }





		  if((DrawRelatedParts == 0) && (NO_DRAW == 0)) /** then want to draw this just-retrieved condition **/
		    {
 

			  global_instance = newnode->CONDITION_INSTANCE;
              global_ctype    = newnode->CONDITION_TYPE;
              global_cnum     = errcount+1;
                      
		      if((SpecificConditionBegin > 0)||(specnum>0))
			  {
			  if(newnode->num_areals>0)
			    {
			      DrawIndividualConditionObject(newnode);
			    }
			  }

			  /********  end remove **********/

		       DrawConditionObjects(newnode);
		    }

		  
		  if(NO_TEXT_MESSAGE == 0)
		    {
		      SEEIT_PrintConditionByComponents(newnode,SpecificConditionBegin,SpecificConditionEnd);
		    }
		  
		  RestoreOriginalCoordinates(newnode);
		  
		}  /** end if(skipcondition==0) **/


	      else if(skipcondition==1)
                {
                  Sortkey = ConditionLookup[i].conditiontype;
                  switch(ConditionLookup[i].conditiontype)
		    {
		    case G_DUPS: /** duplicate poly (by x,y,z of vertices) **/
		    case C_DUPS: /** complete duplicate, attribution and all **/
                    case SAMEID: /** same GFID or FLDBID, diff geom **/
                    case SAMEID_GDUP:  /** same GFID or FLDBID, same geom, diff attr ***/
                    case SAMEID_CDUP: /** same GFID or FLDBID, geom & attr ***/
                    /**case AGEOM_UNM_LAT:
                    case AGEOM_UNM_LON:**/
                    case ANY_SAMEID:  /** same unique identifier, except those that are complete duplicates ***/
		    case POLYINTPOLY: /** two polygons, of selected types, intersect **/
		    case POLYINAREA: /** polygon lies wholly inside an areal **/
		    case PTINREGION:  /** point feature inside a typed polygon or areal **/
                    case PTINPROPER:  /** point feature inside an area feature - not within tolerance of edge (or edge or hole) **/
                    case ACOVERA: /* area covered by area ***/
                    case FAILMERGEA:  /** area feature that should be merged with area that shares edge ***/
                    case FAILMERGEA2:  /** area feature that should be merged with area that shares edge - no accounting for metadata  ***/
                    case AINSIDEHOLE: /** area inside another areal's cutout ('illegal holes') ***/
                    case FEATNOTCUT:  /*** feature not cut at end node of second feature ***/
                    case ISOLINE:  /** line feature completely inside an area feature ***/
                    case LINSIDEA: /** line partly or entirely inside area feature ***/
                    case LSEGCOVERA: /** line segment overlaps an area feature perimeter ***/
		      SkipTwoObjects(Sortkey,errtypeinout);
		      break;

                    case LGEOM_UNM_LAT:
                    case LGEOM_UNM_LON:
                    case AGEOM_UNM_LAT:
                    case AGEOM_UNM_LON:
                    case AUNM_ATTR_A:
                    case LUNM_ATTR_A:
                      SkipMsgMagPointObjects(Sortkey,errtypeinout);
                      break;

                    case MASKCONSTANT:
                    case MASKZERO: /** DEM not zero elev at point defined by specified mask value ***/
                    case MASKEDIT_0: /** Raw DEM and Edited DEM different & Edit Mask has value zero**/
                    case MASKEDIT_1: /** EDM has primary tolerance value, diff between TDR and TDF is > secondary tolerance **/
                    case MASKCONF2: /** variation of Grids with conflicting values **/
                    case MASKCONFLICT: /** Grid DEM Masks have conflicting values ***/
                    case PT_GRID_DIF: /** point and grid z value mismatch at exact coord, no interpolation **/

                    case RAISEDPC: /** number of raised shoreline points exceeds tolerance **/
                    case FLOWSTEP:  /** step size in river flow above threshold ***/
                    case BREAKLINE: /** river elevation change at bad angle with shorelines ***/
                    case WATERMMU: /** minimum mapping unit for water body below threshold ***/
                    case AVGSPIKE: /** spike / well as compared to average elevation of neighbor posts ***/
                    case GSHELF:  /** looking for shelf formations like PUE in DEM ***/
                    case AWITHOUTA:  /** area that does not fully contain a second area ***/
                    case LOSMINHGT:
                    case ELEVGT:
                    case ELEVLT: 
                    case ELEVEQ:  
                    case ELEVEQOPEN:
                    case FEATOUTSIDE:  /*** a feature lies at least partly outside the MGCP cell ***/
                      SkipDynamicInfo(Sortkey,errtypeinout);
                      break;

		      
		    case LLINT: /** line - line inetrsection **/
                    case BADFEATCUT: /** feature cut when no need ***/
                    /**case LGEOM_UNM_LAT:
                    case LGEOM_UNM_LON:***/
		    case LLIEX: /** line - line except intersection **/
                    case LAIEX: /** line - area intersection with 3rd feature exception ***/
		    case LMINT: /** line - model intersection **/
                    case LAINT:  /* line - areal intersection **/
                    case LAINTNOEND: /** line - area intersection not at line end node ***/
                    case LACUTFAIL:  /** line not cut at intersection with area perimeter **/
                    case LEAON_NOTIN: /** line end node on area edge, line not inside area ***/
                    case POLYINTAREA: /* polygon - areal intersection of edges **/
                    case AREAINTAREA: /* areal objects intersect */
                    case PART_ISF: /** two area features have intersecting edges and share part of their faces **/
                    case CUT_INT: /** cut-out intersects parent feature outer ring ***/
                    case LLNONODEINT: /* features intersect, but not at a shared node **/
                    case NONODEOVLP: /** line, area have overlapping edge without common node ***/
                    case LLNOENDINT: /** lines intersect, but not at end point **/
                    case LLINTAWAY: /** two lines intersect, and cross over each other ***/
                    case LLINTNOEND: /** two lines intersect, pt of intersection is away from either primary particpant end node ***/
                    case P_O_LOOP: /*** self-intersecting line that includes P & O formations using end nodes - lines only ****/
                    case AOVERLAPA: /** overlapping area features (second can also be inside first) **/
		      SkipPointAndTwoObjects(Sortkey,errtypeinout);
		      break;
		      
		    case VERTSLOPE: /** vertical poly **/
		    case PLPFAIL: /** point oline coincidence failure **/
                    case PNOCOVERLE:  /** point not covered by lionear end node ***/
                    case PNOCOV2LEA: /** point not covered by 2 line terminal nodes or area edges***/
                    case PNOCOVERLV:  /** point not covered by any line vertex  **/
		    case POLYOSIDEAREA: /** Poly completely outside all areals of given type **/
		    case PTOSIDEREGION: /** point feature not inside any typed areal or poly **/
                    case OBJECTWITHOUT: /** poly or areal without a point or linear inside **/
                    case OBJ_WO_TWO: /** area contains secondary P,A,L but not tertiary P,A,L ***/
                    /***case AWITHOUTA:  area that does not fully contain a second area ***/
                    case FSFAIL: /*** face sharing failure ***/
                    case PSHAREFAIL:  /*** an area feature fails to share any of its perimeter with a 2d area feature ***/
                    case NOCOINCIDE: /** area without line end node or segment on its perimeter ***/
                    case V_DUPS: /** duplicate vertices inside object **/
                    case LNOCOVERLA: /** line not covered by linear or areal ***/
                    case LSPANFAIL: /** line not covered by face of doesnt span between edges ***/
                    case LNOCOV2A:  /** line not covered by edges of 2 area features ***/
                    /**case ISOLINE:  ** line feature completely inside an area feature ***/
                    /**case LINSIDEA: ** line partly or entirely inside area feature ***/
                    case LEINSIDEA: /** line end node properly inside an area ***/
                    case COINCIDEFAIL: /** line or area feature segment fails to coincide with 2 other line or area features **/
                    case ISOLATEDA:  /*** area feature does not instersect another area or a line feature ***/
                    case NETISOA: /** like ISOLATEDA except allowed a transitive connection through other like features ***/
                    case ANETISOA: /** area not trans connected to another area by shared edges ***/
                    case NETISOFEAT: /** form a network - check for nets with one feature, but not another ***/
                    case MULTIDFEAT: /** single line or area with both 2 and 3 D coordinates ***/
                    case MULTISENTINEL: /** single line or area has more than one sentinel z value ***/
                    case CONNECTFAIL: /** point, line, or area feature without 'connection' to specified 2nd feature **/
                    /***case FEATOUTSIDE:  *** a feature lies at least partly outside the MGCP cell ***/
                    case HIGHLIGHTED: /** feature is on the highlight list from view by attribution ***/
                    case ANOCOVERLA:  /** areal not covered by linear or areal ***/
                    case QUALANOCOVLA: /** area permin not covered by line or area AND is inside a third area ***/
                    case ANOCOVERA: /** area not covered by second area feature **/
                    case OVERUNDER: /** any feature outside a perimeter-defining area or a line end node undershooting it **/
                    case AMCOVAFAIL: /** area not coverer by adjoining areas **/
                    case CUTOUT:   /** simply identifies a cut-out of an area feature ***/
                    case PORTRAYF:  /** write feature that fails all MGCP4 portrayal rules ***/
                    case TPORTRAYF: /** write feature that fails all TDS6 portrayal rules ***/
                    case MASKMONO: /** DEM not monotonic at point defined by specified mask value ***/
                    case LLNOINT: /** line - line intersection failure ***/
                    case LFNOINT: /** line fails to intersect another line, area, or point and no end node on 1/4 degree line ***/
                    case PLLPROXFAIL: /*** point line intersection coincidence failure ***/
                    case AHANG_LON: /** hanging area feature at a specified longitude meridian ***/
                    case AHANG_LAT: /** hanging area feature at a specified latitude parallel ***/
                    case AUNM_ACRS_A: /** area feature edge incorrectly matched across a bounding area feature ***/
		      SkipObject(Sortkey,errtypeinout);
		      break;
		      
		    case LOUTSIDEA: /** linear vertex falls outtside areal **/
                    case LLAINT: /** line - line endpt connect at area perimeter **/
                    case L_NOTL_AINT: /** line end point connects to 'not type line' at area perimeter **/
		    case TVERT: /** 'T' vertex **/
                    case LENOCOVERP: /** linear end node not covered by point feature ***/
                    case ENCONFAIL: /** end node connectivity failure **/
                    case NOENDCON: /** both end nodes of a line fail to connect or be covered **/
                    case BOTHENDCON: /** both end nodes of a line feature are covered by specified-type point features **/
                    case LENOCOVERL:  /*** line end node not within tolerance distance to another line ***/
                    case NOLCOVLE: /**line end node not within tolerance distance to another line, including itself ***/
                    case LOOPS: /*** self-intersecting linear or areal ***/
                    case COLINEAR: /** 3 consecutive vertices on line or area perim are collinear - middle one is not connecting node ***/
                    case KICKBACK:  /** 180 degree kink ***/
                    case ENDPTINT: /** line endpoints are the same ***/ 
                    case L_UNM_A:  /*** line endpt unmatched at area feature boundary ***/
                    case LSAME_UNM_A: /*** line endpt unmatched with line of same FCODE at Area boundary ***/
                    case LUNM_ACRS_A: /*** line mismatch across poly edge ***/
                    case LUNMA_ACRS_A: /** line end not matched to area node across area perimeter ***/
                    case LHANG_LON: /** hanging line feature at a specified longitude meridian ***/
                    case LHANG_LAT: /** hanging line feature at a specified latitude parallel ***/
                    case LE_A_UNM_LON: /** line end node not coincident with area node at longitude meridian **/
                    case LE_A_UNM_LAT: /** line end node not coincident with area node at latitude parallel **/
                    case LATTRCHNG:  /** line end point connects to same fdcode line, but attributes differ between the 2 features **/
		      SkipPointAndObject(Sortkey,errtypeinout);
		      break;
		      
                    case ATTR_PAIR: /*** NGA unexpected fcode - geom pair ***/
                    case ATTR_UNEXP: /** NGA unexpected attribute assigned ***/
                    case RPTD_ATTR: /** attribute error as reported  ****/
                    case CONFLATE: /*** line is unique among conflation sets of data ***/
/**case CONF_STATS: **** just generates conflation-information statistics, etc - no conditions to read/write **/
                    case ATTR_VVT: /*** attribute dependency violation  **/
                    case ATTR_RNULL: /*** MGCP Required attribute assigned NULL value ***/
                    case ATTR_MISSING: /** missing a required attribute ***/
                    case ATTR_DT:  /** NGA - datatype encountered not as presecribed **/
                    case ATTR_RNG: /** NGA attribute value range violation ***/
                    case ATTR_PICK: /** NGA - pick list allowed domain violation **/
                    case ATTR_META: /** NGA - GIFD D4 metadata violation ***/
                    case VVTERR1WAY: /** feature with designated attribute & value ***/
                    case VVTERR2WAY:  /** valid value table type error ***/
                    case VVTERR3WAY:  /*** valid values conflict between 3 attribute of a single feature ***/
		    case ATTRERR: /*** attribution error **/
		      SkipObjectAndMessage(Sortkey,errtypeinout);
		      break;
		      
		      /** next groups are for magnitude-related conditions **/
                    case AREAKINK: /** high angle on perimeter of area eature **/
                    case INCLSLIVER: /** areal with included sliver **/
                    case ZUNCLOSED: /** area feat not closed in Z **/
		    case AREAUNCLOSED:
                    case NOT_FLAT:  /*** area feature with surface that is not uiform elevation ***/
                    case CLAMP_NFLAT: /** area feature does not have constant elevation when clamped to underlying DEM ***/
                    case CLAMP_DIF: /** difference between feature vertex z value and interpolated DEM value ***/
                    /**case LAINTNOEND: ** line - area intersection not at line end node ***/
                      SkipPointObjectAndMagnitude(Sortkey, errtypeinout);
                      break;

                    case ISOTURN: /** high turn angle w/o 3d feature present ***/
		    case KINK:  /** high angle between adjacent linear segments **/
                    case Z_KINK: /** consecutive kinks form a 'Z' ***/
                    case L_A_KINK: /** kink at intersection of line end node  and area feature perim **/
                    case INTERNALKINK: /** kink internal to single line feature **/
		    case SLOPEDIRCH: /*** slope direction change along linear **/
                    case CLAMP_SDC: /*slope direction change along a line that has been elevation-value clamped to underlying DEM ***/
                    case CLAMP_JOINSDC: /** slope direction change at line feature connection when both are clamped to DEM ***/
		    case SLIVER: /** sliver triangle **/
                    case FACESIZE: /*** small area on face of area feature **/
                    case ARNGE_UNM_LAT:
                    case ARNGE_UNM_LON:
		    case NARROW:  /** narrow triangle **/
		    case SMALLOBJ:  /** small 3d area poly **/
		    case HSLOPE: /** high slope poly **/
		    case HTEAR: /** horizontal tear **/
		    case OVERC: /** over-covered edge **/
		    case GSPIKE:  /** grid data spike point ***/
                    /***case AVGSPIKE: ** spike / well as compared to average elevation of neighbor posts ***/
                    /*** case FLOWSTEP:  ** step size in river flow above threshold ***/
                    /**case WATERMMU: ** minimum mapping unit for water body below threshold ***/
                    /**case RAISEDPC: ** number of raised shoreline points exceeds tolerance **/
                    case GRID_STD_DEV: /** grid elev value, inside feature polygon, over range offset from std deviation **/
/********************************
		    case ELEVGT:
		    case ELEVLT: 
                    case ELEVEQ:  
                    case ELEVEQOPEN:
***************************/
                    case ELEVADJCHANGE:  /** change in adjacent node elevations > threshold ***/
                    case FEATSPIKE: /** elevation spike along 3D feature ***/
                    case SEGLEN: /** areal or linear feature segment length below threshold ***/
                    case LONGSEG: /** linear or areal perimeter segment with length at or above threshold ***/
                    case BIGAREA: /** area feat in excess of set square area **/
                    case SMALLAREA:  /** area feat with small square area **/
/***case MASKMONO: *** DEM not monotonic at point defined by specified mask value ***/
                    case SMLCUTOUT: /** small included area inner ring of area feature ***/
                    case OSIDE_LAT:   /**** feature coordinate above or below latitude range    **/
                    case OSIDE_LON:   /**** feature coordinate above or below longitude range    **/
                    case BNDRYUNDERSHT: /** feature undershoots whole degree project outside boundary ***/
                    case LBNDUSHT:  /** unconnected line end node undershoots whole-degree boundary ***/
                    case MULTIPARTL:  /** multi-part line ***/
                    case MULTIPARTA: /** multi-part area **/
                    case MULTIPARTP: /** multi-part point **/ 
                    case PERIMLEN: /** areal perimeter or ttl line len below threshold ***/
                    case SHORTFEAT:  /** short length line feature not on quarter degree 'boundary' ***/
                    case LONGFEAT:   /** line or area feature with total length above threshold ***/
                    case SHARE3SEG: /** line feature segment overlaps 2 other line or area feature segments ***/
                    case PC_SLOPE: /*** line feature segment with percent slope above tolerance ****/
                    case CALC_AREA:  /*** point feature with LEN and WID attr values product < tolerance ***/
                    case COVERFAIL: /** to detect holes in surface; MGCP landcover requirement ***/
		      SkipObjectAndMagnitude(Sortkey, errtypeinout);
		      break;
		      
		    case LVPROX: /** line vertex near another line **/
                    case LENOCOVERA: /** line end node not covered by area perimeter ***/
		    case PLPROX: /** point feature within x of a line feature **/
                    case PSHOOTL: /*** point feature over or undershoots a line feature ***/
                    case PLPROXEX:  /** pt to line prox with exception for line end node ***/
                    case ENCONNECT: /** linear end node connectivity ***/
                    case BADENCON: /** bad sequence on line feature connections ***/
                    case FEATBRIDGE:
		      SkipPointEdgeAndMagnitude(Sortkey, errtypeinout);
		      break;
		      
		    case VTEAR: /** vertical tear **/
                    case LELINEPROX:  /** line end node and line (vertex, segment ...) proximity ***/
                    case EN_EN_PROX:  /** undershoot end nodes connected by another feature **/
                    case LUNDERSHTL:  /** line end - line undershoot **/
                    case LUSHTL_CLEAN: /* like line - line undershoot, but no condition if feature mid-undershoot **/
                    case LVUSHTL: /** interior line vertex undershoots a different line feature **/
                    case VUSHTL_CLEAN: /* like vertex - line undershoot, but no condition if feature mid-undershoot **/
                    case LVOSHTL: /** interior line vertex overshoots a different line feature ***/
                    case LUSHTL_DF: /** line - line undershoot, different line feature types ***/
                    case LOSHTL_DF: /** line - line overshoot, different line feature types ***/
                    case LOVERSHTL:   /** line end - line overshoot **/
                    case LUNDERSHTA:  /** line end area perimeter undershoot **/
                    case LOVERSHTA:  /** line end - area perimeter overshoot **/
                    case LAPROX:  /** line to area proximity - smallest dist between the two features ***/
                    case LASLIVER: /** sliver formed between line and area features **/
                    case LSLICEA: /** line 'slices' area so as create a small piece ***/
                    case LLSLIVER:  /** sliver formed between two line features **/
                    case AUNDERSHTA: /** area edge undershoots neighbor area edge ***/
                    case AOVERSHTA: /** area edge overshoots neighbor area edge ***/
                    case LLMULTINT: /** lines intersect each other multiple times **/
                    case LOC_MULTINT: /** lines with no or compatible LOC values intersect each other multiple times **/
                    case L2D_L3D_MATCH: /** linear end node Z mismatch ***/
                    case LEZ_PROX_3D: /** apply check L2D_L3D_MATCH to 3d line features only **/
                    case CNODE_ZBUST:  /*** Z mismatch between any two connecting nodes (in x,y) ***/
                    case DUPLICATESEG: /*** duplicated segments ***/
                    case LJOINSLOPEDC: /** slope direction change along linear **/
                    case CONTEXT_KINK:  /*** kink based on one high angle next to one lower (moderate) angle ***/
                    case FAILMERGEL: /** line feature that should be merged with a connected line feature **/
                    case FAILMERGEL2:  /** line object that should be merged with connecting line no accounting for metadata  ***/
                    case LRNGE_UNM_LAT:
                    case LRNGE_UNM_LON:
                    case EXTRA_NET:   /*** vertex is a near miss in connecting to another vertex to join networks ***/
                    case INTRA_NET:   /*** vertex is close to but not identical with another vertex in the same network ***/
                    case SHAREPERIM: /** areals share perimeter segments ***/
		    case LSPINT: /** line intersects poly with slope > x **/
                    case SHARESEG: /** line feature segment overlaps 1 other line feature segment ***/
                    /*** case SHARE3SEG:  old way  ** line feature segment overlaps 2 other line feature segments ***/
                    case LLI_ANGLE: /*** 2 lines intersect at severe angle ***/
		    case LSPIEXP: /** line - poly (slope > x) except when intersection **/
		    case PTPTPROX: /** point to point proximity **/
                    case PUNDERSHTA: /** point not on area perimeter and is outside that area feature **/
                    case POVERSHTA: /** point not on area perimeter and is inside that area feature **/
                    case LODELEVDIF: /** elev difference between grids in different LOD **/
                    case GRIDEXACTDIF: /** Grids have post value difference at same X,Y ***/
                    case MASKSHOREL: /** water body not contained by shoreline ***/
                    case CLAMP_SEG: /*** catenary segment below associated DEM ****/
		      SkipMagnitudeAndTwoObjects(Sortkey, errtypeinout);
		      break;
		      
		    default:
		      printf("bad Sortkey (%d) in condition file\n",Sortkey);
		      exit(1);
		    }  /** end switch on Sortkey  ***/
		  
                } /** end else if(skipcondition==1) **/
	      SEEIT_fread_int(&Sortkey,errtypeinout);
              SEEIT_fread_int(&ConditionCopy,errtypeinout);

	      
	      
	      if(DRAW_ONLY==1)
		{
		  if(ListOfConditions!=NULL)
		    {
		      SEEIT_FreeConditions(ListOfConditions);
		      ListOfConditions = NULL;
		    }		  
		}
	      
	      if(AbortConditionDraw()==1)
		{
		  printf("Aborting condition load/draw\n");
		  ResetZoom();

		  SetLK2(ListOfConditions,lk2);
		  fclose(lk2);

          fclose(errtypeinout); /* added for B15 */

		  return ListOfConditions;
		}	      	     
            } /** end for(errcount...  ***/
	} /** end if selected and have some of these conditions ***/
    } /** for i=1; i<=TotalConditions ***/
  
  fclose(errtypeinout);
  
  if(NO_DRAW==0)
    {
      if(DrawRelatedParts > 0)
	{
	 
		  global_ctype    = ConditionLookup[RelatedPartKeyValueIndex].conditiontype;
		  global_instance = ConditionLookup[RelatedPartKeyValueIndex].instancenumber;
		  global_cnum     = SpecificConditionBegin;
		  /*** end remove ***/

	  DrawObjectMultiParts(nsname,RelatedPartKeyValueIndex,RelatedPartObjectID,SpecificConditionBegin);

	  
	  DrawRelatedParts = 0;
	}
    }


  SetLK2(ListOfConditions,lk2);
  fclose(lk2);

  return ListOfConditions;
}


void SEEIT_LoadConditionReport(char *dirname)
{
  char lkname[500];
  char nsname[500];
  char esname[500];
  FILE *temp;
  
  
  machine_endianness = determine_machine_endianness();
  
  if(DEBUG==1)
    {
      printf("Have determined that this machine is %s-endian.\n", 
	     (machine_endianness==BIG) ? "big" : "little");
    }
  
  if((USE_DOS==0)||(SLASHTYPE==NONDOS_TYPE))
    {
      sprintf(lkname,       "%s/condreport.lk"  ,dirname);
      sprintf(nsname,       "%s/condreport.ns"  ,dirname);
      sprintf(esname,       "%s/condreport.es"  ,dirname);
    }
  else
    {
      sprintf(lkname,       "%s\\condreport.lk"  ,dirname);
      sprintf(nsname,       "%s\\condreport.ns"  ,dirname);
      sprintf(esname,       "%s\\condreport.es"  ,dirname);
    }
  
  temp = fopen(lkname,"rb");  
  if(temp==NULL)
    {
      printf("couldn't open %s\n",lkname);
      printf("be sure the first argument passed to this executable is a directory\n");
      printf("containing the file condreport.lk (binary condition output file)\n");
      exit(1);
    }
  fclose(temp);
  
  temp = fopen(nsname,"rb");  
  if(temp==NULL)
    {
      printf("couldnt open %s\n",nsname);
      printf("be sure the first argument passed to this executable is a directory\n");
      printf("containing the file condreport.ns (binary condition output file)\n");
      exit(1);
    }
  fclose(temp);
  temp = fopen(esname,"rb");  
  if(temp==NULL)
    {
      printf("couldnt open %s\n",esname);
      printf("be sure the first argument passed to this executable is a directory\n");
      printf("containing the file condreport.es (binary condition output file)\n");
      exit(1);
    }
  fclose(temp);
  
  ReadES(esname);  
  ReadLK(lkname);
  InitArrayNames();
}







int SEEIT_GetNumConditions(int CONDITIONTYPE, int additional_instance_num)
{
  int i,index;
  
  
  for(i=0;i<TotalConditions;i++)
    {
      
      if(ConditionLookup[i].conditiontype==CONDITIONTYPE)
	{
	  index = i+additional_instance_num;
	  
	  if(index>=TotalConditions)
	    {
	      printf("Error in SEEIT_GetNumConditions: indexing %d when there are\n",index);
	      printf("only %d conditions for %d %d\n",TotalConditions,CONDITIONTYPE,additional_instance_num);
	      return -1;
	    }
	  
	  if(
	     (ConditionLookup[index].conditiontype !=CONDITIONTYPE) ||
	     (ConditionLookup[index].instancenumber!=additional_instance_num)
	     )
	    {
	      printf("Error in SEEIT_GetNumConditions: indexing %d yields %d %d\n",
		     index,ConditionLookup[index].conditiontype,ConditionLookup[index].instancenumber);
	      printf("when it should yield %d %d\n",CONDITIONTYPE,additional_instance_num);
	      return -1;
	    }
	  return ConditionLookup[index].number;
	}
    }
  printf("Error in SEEIT_GetNumConditions: could not find %d %d\n",CONDITIONTYPE,additional_instance_num);
  return -1;
}






char * SEEIT_GetConditionAnnotation(struct ConditionList *curr)
{
  int CONDITIONTYPE = curr->CONDITION_TYPE;
  int instance      = curr->CONDITION_INSTANCE;
  static char retval[1000];
  
  if((NGA_TYPE==0)&&(curr->AttrErrMsg!=NULL))
    {
      CONDITIONTYPE = GetRevisedcloneindex(CONDITIONTYPE,instance);
      
      if(ConditionLookup[CONDITIONTYPE].Annotation==NULL)
	{
	  return curr->AttrErrMsg;
	}
      else
	{
	  sprintf(retval,"%s %s",ConditionLookup[CONDITIONTYPE].Annotation,curr->AttrErrMsg);
	  return retval;
	}
    }
  
  CONDITIONTYPE = GetRevisedcloneindex(CONDITIONTYPE,instance);
  
  if(ConditionLookup[CONDITIONTYPE].Annotation==NULL)
    {
      return "No Annotation";
    }

  return ConditionLookup[CONDITIONTYPE].Annotation;
}


char * SEEIT_GetConditionName(int CONDITIONTYPE)
{
  CONDITIONTYPE = GetRevisedindex(CONDITIONTYPE);
  return ConditionLookup[CONDITIONTYPE].name;
}








int InBetween(double z, double z1, double z2)
{
  double lowval,highval;

  if(z1<z2)
  {
	  lowval  = z1 - .0000001;
	  highval = z2 + .0000001;
  }
  else
  {
 	  lowval  = z2 - .0000001;
	  highval = z1 + .0000001;
  }

  if(tempis2D(z))
  {
	  z = EXPORT2DSENTINEL;
  }

  if((z>=lowval)&&(z<=highval))
  {
    return 1;
  }
   
  return 0;
}


double TestAdditionalZ(struct ConditionList *curr)
{
  int i,foundit=0;
  double dist, closetol = .0000001;

  extern double PointToLineDist2D(double xj,double yj,double xk,double yk,double xl,double yl);

  if(curr->CONDITION_TYPE==AOVERLAPA)
  {
	  return curr->pointz;
  }

  if((curr->CONDITION_TYPE==NOT_FLAT) || (curr->CONDITION_TYPE == CLAMP_DIF) ||
       (curr->CONDITION_TYPE == CLAMP_NFLAT))
  {
	  return curr->pointz;
  }

  if(curr->num_areals>=1)
  {
	  if(curr->areal1.numverts==1)
	  {
        if(InBetween(curr->pointz,curr->areal1.z[0]-.01,curr->areal1.z[0]+.01))
		  {
			return curr->pointz;
		  }
	  }

	  for(i=1;i<curr->areal1.numverts;i++)
	  {
	    dist = PointToLineDist2D(curr->pointx,curr->pointy,
			curr->areal1.x[i-1],curr->areal1.y[i-1],
			curr->areal1.x[ i ],curr->areal1.y[ i ]);

		if(dist<closetol)
		{
		  foundit = foundit + 1;

          if(InBetween(curr->pointz,curr->areal1.z[i-1],curr->areal1.z[i]))
		  {
			return curr->pointz;
		  }
		}
 	  }

	  if(curr->num_areals>=2)
	  {
	    if(curr->areal2.numverts==1)
		{
          if(InBetween(curr->pointz,curr->areal2.z[0]-.01,curr->areal2.z[0]+.01))
		  {
			return curr->pointz;
		  }
		}

  	    for(i=1;i<curr->areal2.numverts;i++)
		{
	      dist = PointToLineDist2D(curr->pointx,curr->pointy,
			curr->areal2.x[i-1],curr->areal2.y[i-1],
			curr->areal2.x[ i ],curr->areal2.y[ i ]);

		  if(dist<closetol)
		  {
		    foundit = foundit + 1;

            if(InBetween(curr->pointz,curr->areal2.z[i-1],curr->areal2.z[i]))
			{
			  return curr->pointz;
			} 
		  } 
		} 
	  } 
  }



   if(curr->num_lines>=1)
  {
	  for(i=1;i<curr->linear1.numverts;i++)
	  {
	    dist = PointToLineDist2D(curr->pointx,curr->pointy,
			curr->linear1.x[i-1],curr->linear1.y[i-1],
			curr->linear1.x[ i ],curr->linear1.y[ i ]);

		if(dist<closetol)
		{
		  foundit = foundit + 1;


          if(InBetween(curr->pointz,curr->linear1.z[i-1],curr->linear1.z[i]))
		  {
			return curr->pointz;
		  }

		}
 	  }

	  if(curr->num_lines>=2)
	  {
  	    for(i=1;i<curr->linear2.numverts;i++)
		{
	      dist = PointToLineDist2D(curr->pointx,curr->pointy,
			curr->linear2.x[i-1],curr->linear2.y[i-1],
			curr->linear2.x[ i ],curr->linear2.y[ i ]);

		  if(dist<closetol)
		  {
  		    foundit = foundit + 1;

            if(InBetween(curr->pointz,curr->linear2.z[i-1],curr->linear2.z[i]))
			{
			  return curr->pointz;
			} 

		  } 
		} 
	  } 
  }

  if(foundit==0)
  {
	printf("couldn't find coincident segment for condition type %s instance %d ord %d\n",
		  SEEIT_GetConditionName(curr->CONDITION_TYPE),
	      curr->CONDITION_INSTANCE+1,curr->ordinal_num);
  }
  else
  {
    printf("bad case for testing %lf for condition type %s instance %d ord %d\n",
	  curr->pointz,SEEIT_GetConditionName(curr->CONDITION_TYPE),
	  curr->CONDITION_INSTANCE+1,curr->ordinal_num);
  }

  return -999;
}


int mySHmax(int a, int b)
{
  if(a>b)
    {
      return a;
    }
  return b;
}


void WriteMainOrIndexFileHeader(FILE *outfile, int type)
{
  int tempint;
  double tempdouble;

  file_endianness = BIG;

  tempint = 9994;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  tempint = 0;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;
  

  file_endianness = LITTLE;

  tempint = 1000;
  SEEIT_fwrite_int(&tempint,  outfile);  bytes_written = bytes_written+4;

  SEEIT_fwrite_int(&type,     outfile);  bytes_written = bytes_written+4;


  SEEIT_fwrite_double(&Gminx,       outfile);    bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Gminy,       outfile);    bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Gmaxx,       outfile);    bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Gmaxy,       outfile);    bytes_written = bytes_written+8;

  SEEIT_fwrite_double(&Gminz,       outfile);    bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Gmaxz,       outfile);    bytes_written = bytes_written+8;
  tempdouble = 0.0;
  SEEIT_fwrite_double(&tempdouble,  outfile);    bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&tempdouble,  outfile);    bytes_written = bytes_written+8;
}



void WritePolygon(FILE *outfile,int RECNUM,int numverts,
		  double Xpoints[],double Ypoints[],double Zpoints[],int closepoly)
{
  int con_len,i,tempint;
  double minx,miny,minz,maxx,maxy,maxz,minm=0.0,maxm=0.0,mval=0.0;


  for(i=0;i<numverts;i++)
  {
	if(tempis2D(Zpoints[i]))
	{
	  Zpoints[i] = EXPORT2DSENTINEL;
	}
  }


  minx=Xpoints[0];
  miny=Ypoints[0];
  minz=Zpoints[0];
  maxx=Xpoints[0];
  maxy=Ypoints[0];
  maxz=Zpoints[0];
  minm=0.0;
  maxm=0.0;
  mval=0.0;




  numverts = numverts + closepoly;


  file_endianness = BIG;
  SEEIT_fwrite_int(&RECNUM,       outfile);  bytes_written = bytes_written+4;

  con_len = (numverts)*4*8;    
  con_len = con_len + 80;      
  con_len = con_len/2;         
  
  G_length   [RECNUM] = con_len + 4;  


  SEEIT_fwrite_int(&con_len,       outfile);  bytes_written = bytes_written+4;

  file_endianness = LITTLE;

  for(i=0;i<(numverts-closepoly);i++)
    {
      if(Xpoints[i]<minx)
	{ 
	  minx = Xpoints[i];
	}
      if(Ypoints[i]<miny)
	{
	  miny = Ypoints[i];
	}
      if(Zpoints[i]<minz)
	{
	  minz = Zpoints[i];
	}

      if(Xpoints[i]>maxx)
	{
	  maxx = Xpoints[i];
	}
      if(Ypoints[i]>maxy)
	{
	  maxy = Ypoints[i];
	}
      if(Zpoints[i]>maxz)
	{
	  maxz = Zpoints[i];
	}
    }

  SEEIT_fwrite_int(&LINETYPE,       outfile); bytes_written = bytes_written+4;
  SEEIT_fwrite_double(&minx,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&miny,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&maxx,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&maxy,       outfile);  bytes_written = bytes_written+8;

  tempint = 1;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;

  tempint = numverts;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;

  tempint = 0;
  SEEIT_fwrite_int(&tempint,       outfile);  bytes_written = bytes_written+4;

  for(i=0;i<numverts;i++)
    {
	  if((closepoly==1)&&(i==numverts-1))
	  {
       SEEIT_fwrite_double(&Xpoints[0], outfile);  bytes_written = bytes_written+8;
       SEEIT_fwrite_double(&Ypoints[0], outfile);  bytes_written = bytes_written+8;
	  }
	  else
	  {
       SEEIT_fwrite_double(&Xpoints[i], outfile);  bytes_written = bytes_written+8;
       SEEIT_fwrite_double(&Ypoints[i], outfile);  bytes_written = bytes_written+8;
	  }
   }


  SEEIT_fwrite_double(&minz,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&maxz,       outfile);  bytes_written = bytes_written+8;

  for(i=0;i<numverts;i++)
  {
	if((closepoly==1)&&(i==numverts-1))
	{
      SEEIT_fwrite_double(&Zpoints[0], outfile);  bytes_written = bytes_written+8;
	}
	else
	{
      SEEIT_fwrite_double(&Zpoints[i], outfile);  bytes_written = bytes_written+8;
	}
  }

  SEEIT_fwrite_double(&minm,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&maxm,       outfile);  bytes_written = bytes_written+8;

  for(i=0;i<numverts;i++)
  {
    SEEIT_fwrite_double(&mval, outfile);  bytes_written = bytes_written+8;
  }

}


void record_bytes(FILE *outfile)
{

  fseek(outfile,(long int)24,SEEK_SET);
  

  file_endianness = BIG;
  bytes_written = bytes_written/2;
  SEEIT_fwrite_int(&bytes_written,       outfile);  
  bytes_written = 0;
}



void CalculateOffsets(int recnum)
{
  int adder = 50;
  int i;

  for(i=1;i<=recnum;i++)
    {
      G_offsets[i] = adder;
      adder = adder + G_length[i];
    }
}


void WriteIndexFileEntry(FILE *outfile, int num)
{
  file_endianness = BIG;
  SEEIT_fwrite_int(&G_offsets[num],       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_int(&G_length[num],       outfile);   bytes_written = bytes_written+4;
}




char *ParseSHGeom(unsigned char i)
{

  if(i==SH_POINT)      
    return "POINT";
  else if(i==SH_CYLINDER)
    return "CYLINDER";
  else if(i==SH_SPHERE)
    return "SPHERE";
  else if(i==SH_CIRCLE)
    return "CIRCLE";
  else if(i==SH_PARALLELEPIPED)
    return "PARALLELEPIPED";
  else if(i==SH_POLYGON)
    return "POLYGON";
  else if(i==SH_LINE)
    return "LINE";
  else if(i==SH_AREAL)
    if(NGA_TYPE==1)
      {
	return "AREA";
      }
    else
      {
	return "AREAL";
      }
  else if(i==SH_GRIDPT)
    return "GRID POINT";
  else if(i==SH_NONE)
    return "NONE";
  else
    {
      printf("ParseSHGeom got bad input %c!\n",i);
      exit(1);
    }

  return "OOPS!?";
}



void WriteDBEntry(FILE *outfile, int withradius, 
		  int G_type, double G_magnitude, int G_instance, int G_condnum,
		  int G_form, double G_radius,    char *SEDID,
		  int code1,  int code2,          char *annotation_in,
		  int retainignore,
		  int idn, int Lindex)
{
  char tempchar[10];
  char recvalue  [100];
  char recvalue2 [30];
  char recvalue3 [20];
  char recvalue3a[20];
  char recvalue4 [20];
  char recvalue5 [20];
  char recvalue6 [500];
  char recvalue7 [500];
  char recvalue8 [500];
  char recvalue9 [500];
  char recvalue10[10];
  char recvalue11[10];
  char annotation[500];
  int i,j,lodnum,blankout;

  extern char **gifd_class_codes;
  extern char **gifd_class_names;
  extern int LLFidTableCount;
  extern int *FidFidTable;
  extern int SACfull,NumLODbands;
  extern struct ErrorTable ErrorLookup[];
  extern struct CrossWalk *CrsWlk;
  extern struct LODdisplay *LODindex;

  j = strlen(annotation_in);
  for(i=0;i<j;i++)
  {
    annotation[i] = annotation_in[i];
	if(i>250)
	{
      break;
	}
  }
  annotation[i] = '\0';


  if(G_type==COVERFAIL)
  {
	  blankout = 1;
	  idn = -999999999;
  }
  else
  {
	  blankout = 0;
  }



  G_instance = G_instance+1;


  if(NGA_TYPE==1)
    {
      withradius = 0;
    }


  tempchar[4] = 32;

  
  for(j=0;j<20;j++)
    {
      recvalue2[j] = ' ';
    }



  if(blankout==1)
  {
    sprintf(recvalue2,"-9999999999.999");
  }
  else
  {
    if(ErrorLookup[G_type].usemagnitude==1)
	{
     sprintf(recvalue2,"%.8lf",G_magnitude);
	}
    else if(ErrorLookup[G_type].usemagnitude==2)
	{ 
     sprintf(recvalue2,"%d",(int)G_magnitude);
	}
    else
	{  
     sprintf(recvalue2,"-9999999999.999");
	}
  }
 


  recvalue2[19]='\0';
  j = strlen(recvalue2);
  recvalue2[j]=' ';
  
  
  
  
  for(j=0;j<MaxNamLenFound;j++)
    {
      recvalue[j] = ' ';
    }


  
  sprintf(recvalue,"%s",SEEIT_GetConditionName(G_type));
  for(j=0;j<(int)strlen(recvalue);j++)
    {
      if(recvalue[j]=='\n')
	{
	  recvalue[j] = ' ';
	}
    }

  
  j = strlen(recvalue);
  recvalue[j]=' ';
  
  
  
  
  for(j=0;j<3;j++)
    {
      recvalue3[j] = ' ';
    }
  
  sprintf(recvalue3,"%d",G_instance);
  
  j = strlen(recvalue3);
  recvalue3[j]=' ';
  recvalue3[3]='\0';
      


  for(j=0;j<10;j++)
    {
      recvalue3a[j] = ' ';
    }
  
  sprintf(recvalue3a,"%d",G_condnum);
  
  j = strlen(recvalue3a);
  recvalue3a[j]=' ';
  recvalue3a[10]='\0';
      


      

  
  for(j=0;j<15;j++)
    {
      recvalue4[j] = ' ';
    }
  
  if(blankout==1)
  {
    sprintf(recvalue4,"NONE");
  }
  else
  {
    sprintf(recvalue4,"%s",ParseSHGeom((unsigned char)G_form));
  }
  
  
  j = strlen(recvalue4);
  recvalue4[j]=' ';
  recvalue4[15]='\0';
  
  




  for(j=0;j<MaxSedLenFound;j++)
    {
      recvalue6[j] = ' ';
    }

  if(blankout==1)
  {
    sprintf(recvalue6,"N/A");
  }
  else
  {
    sprintf(recvalue6,"%s",SEDID);
  }

  j = strlen(recvalue6);
  recvalue6[j]=' ';
  




  if(NGA_TYPE==1)
    {
      
      for(j=0;j<MaxNam2LenFound;j++)
	{
	  recvalue7[j] = ' ';
	}
      
      if((code1>0)&&(blankout==0))
	  { 
	   sprintf(recvalue7,"%s",gifd_class_names[code1-1]);
	  }
      else
	  { 
	   sprintf(recvalue7,"N/A");
	  } 
	  

      j = strlen(recvalue7);
      recvalue7[j]=' ';
      
      
      
      for(j=0;j<MaxNam3LenFound;j++)
	  { 
	   recvalue8[j] = ' ';
	  } 
      
      if((code2>0)&&(blankout==0))
	  { 
	    sprintf(recvalue8,"%s",gifd_class_names[code2-1]);
	  } 
      else
	{
	  sprintf(recvalue8,"N/A");
	}
      

      j = strlen(recvalue8);
      recvalue8[j]=' ';



      for(j=0;j<7;j++)
	{
	  recvalue10[j] = ' ';
	}
      
      if((code1>0)&&(blankout==0))
	{
	  sprintf(recvalue10,"%s",gifd_class_codes[code1-1]);
	}
      else
	{
	  sprintf(recvalue10,"N/A");
	}
      
      recvalue10[5]='\0';
      j = strlen(recvalue10);
      recvalue10[j]=' ';
      
      
      
      for(j=0;j<7;j++)
	{
	  recvalue11[j] = ' ';
	}
      
      if((code2>0)&&(blankout==0))
	{
	  sprintf(recvalue11,"%s",gifd_class_codes[code2-1]);
	}
      else
	{
	  sprintf(recvalue11,"N/A");
	}
      
      recvalue11[5]='\0';
      j = strlen(recvalue11);
      recvalue11[j]=' ';
    }


  
  
  for(j=0;j<MaxAnnLenFound;j++)
    {
      recvalue9[j] = ' ';
    }
  
  sprintf(recvalue9,"%s",annotation);
  

  j = strlen(recvalue9);
  recvalue9[j]=' ';
  
  
  
  



  if(withradius==1)
    { 
      for(j=0;j<15;j++)
	{
	  recvalue5[j] = ' ';
	}
      
      sprintf(recvalue5,"%.6lf",G_radius);
      
      j = strlen(recvalue5);
      recvalue5[j]=' ';
      recvalue5[15]='\0';
    }    


      
  
  fwrite(&tempchar   [4], 1, 1             ,outfile);
  fwrite(&recvalue   [0], 1,MaxNamLenFound ,outfile);
  fwrite(&recvalue2  [0], 1,20             ,outfile);
  fwrite(&recvalue3  [0], 1,3              ,outfile);
  fwrite(&recvalue3a [0], 1,10             ,outfile);



  if(NGA_TYPE==1)
    {
      fwrite(&recvalue10 [0], 1,5 ,outfile);
      fwrite(&recvalue11 [0], 1,5 ,outfile);
      fwrite(&recvalue7  [0], 1,MaxNam2LenFound ,outfile);
      fwrite(&recvalue8  [0], 1,MaxNam3LenFound ,outfile);
    }

  fwrite(&recvalue9  [0], 1,MaxAnnLenFound  ,outfile);

  fwrite(&recvalue4  [0], 1,15             ,outfile);
  fwrite(&recvalue6  [0], 1,MaxSedLenFound ,outfile);


  if(withradius==1)
    {
      fwrite(&recvalue5  [0], 1,15      ,outfile);
    }

 

  if(NGA_TYPE==1)
    {
      extern char *ParseAttributionType(int attr_type);
      extern int ATTRIBUTION_TYPE;
     
      for(j=0;j<20;j++)
	{
	  recvalue2[j] = ' ';
	}
      
      sprintf(recvalue2,"%s",ParseAttributionType(ATTRIBUTION_TYPE));
      
      j = strlen(recvalue2);
      recvalue2[j]=' ';
      recvalue2[20]='\0';
       
      fwrite(&recvalue2  [0], 1,20             ,outfile);
 
    }


  if(retainignore==1)
    {
      sprintf(recvalue5,"RETAIN");
    }
  else
    {
      sprintf(recvalue5,"IGNORE");  
    }
  fwrite(&recvalue5  [0], 1,6      ,outfile);




  
  for(j=0;j<500;j++)
	{
	  recvalue7[j] = ' ';
	}
  fwrite(&recvalue7  [0], 1,10   ,outfile);
  fwrite(&recvalue7  [0], 1,8    ,outfile);
  fwrite(&recvalue7  [0], 1,80   ,outfile);
  fwrite(&recvalue7  [0], 1,10   ,outfile);
  fwrite(&recvalue7  [0], 1,8    ,outfile);
  fwrite(&recvalue7  [0], 1,80   ,outfile);



  for(j=0;j<10;j++)
  {
     recvalue5[j] = ' ';
  }

  if((idn>=0)&&(idn<LLFidTableCount))
  {
    sprintf(recvalue5,"%d",FidFidTable[idn]);
      
    j = strlen(recvalue5);
    recvalue5[j]=' ';
    recvalue5[10]='\0';
  }
  else
  {
	sprintf(recvalue5,"-999999999");
  }
  recvalue5[10]='\0';


  for(j=0;j<500;j++)
	{
	  recvalue7[j] = ' ';
	}
  sprintf(recvalue7,"N/A");
  j = strlen(recvalue7);
  recvalue7[j] = ' ';

  if(blankout==0)
  {
   if((Lindex>=0)&&(Lindex<SACfull))
   {
     lodnum = CrsWlk[Lindex].LODband - 1;
	 if((lodnum>=0)&&(lodnum<NumLODbands))
	 {
		sprintf(recvalue7,"%s",LODindex[lodnum].filename);
	 }
   }
  }



  fwrite(&recvalue5  [0], 1,10            ,outfile);
  fwrite(&recvalue7  [0], 1,maxtablelen   ,outfile);
}




void WriteEndOfDBFile(FILE *outfile)
{
  char tempchar[10];

  tempchar[5] = 26;
  fwrite(&tempchar   [5], 1, 1,outfile);    
}


void WriteDBHeader(FILE *outfile, int num_records, int withradius)
{
  char tempchar[10];
  short int headbytes = 0;
  short int recbytes = 0;
  int i;

  char fieldname[100];
  char fieldtype[10];
  unsigned char fieldlen;
  unsigned char fieldDcount;


  if(NGA_TYPE==1)
    {
      withradius = 0;
    }


  recbytes = 21                + 
    MaxNamLenFound             + 
    3                          + 
    10                         +
    15                         + 
    MaxSedLenFound             + 
    MaxAnnLenFound             +
    (NGA_TYPE*5)               +
    (NGA_TYPE*5)               +
    (NGA_TYPE*MaxNam2LenFound) +
    (NGA_TYPE*MaxNam3LenFound) +
    (15*withradius)            +
    (NGA_TYPE*20)              +
    6                          +  
    10                         +  
    8                          +  
    80                         +  
    10                         +  
    8                          +  
    80                         +  	
    10                         +  
    maxtablelen                   
	;

  headbytes = 545     +  
	  (withradius*32) +  
	  (NGA_TYPE*32*5) ;  
 

  file_endianness = LITTLE;
  tempchar[0] = 3;
  tempchar[1] = 72;
  tempchar[2] = 7;
  tempchar[3] = 17;
  SEEIT_fwrite_char (&tempchar[0], outfile);  
  SEEIT_fwrite_char (&tempchar[1], outfile);  
  SEEIT_fwrite_char (&tempchar[2], outfile);  
  SEEIT_fwrite_char (&tempchar[3], outfile);  
  SEEIT_fwrite_int  (&num_records, outfile);
  SEEIT_fwrite_short(&headbytes  , outfile); 
  SEEIT_fwrite_short(&recbytes   , outfile); 


  tempchar[4] = 0;
  tempchar[5] = 87;
  tempchar[6] = 13;

  for(i=12;i<29;i++)
    {
      SEEIT_fwrite_char (&tempchar[4], outfile);  
    }
  SEEIT_fwrite_char (&tempchar[5], outfile);  
  SEEIT_fwrite_char (&tempchar[4], outfile);  
  SEEIT_fwrite_char (&tempchar[4], outfile);  



  sprintf(fieldname,"ERRTYPE");
  sprintf(fieldtype,"C");
  fieldlen = MaxNamLenFound;
  fieldDcount = 0;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);


  sprintf(fieldname,"MAGNITUDE");
  sprintf(fieldtype,"N");
  fieldlen = 20;
  fieldDcount = 8;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);


  sprintf(fieldname,"INSTANCE");
  sprintf(fieldtype,"N");
  fieldlen = 3;
  fieldDcount = 0;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);


  sprintf(fieldname,"COND_NUM");
  sprintf(fieldtype,"N");
  fieldlen = 10;
  fieldDcount = 0;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);



  if(NGA_TYPE==1)
    {
      
      sprintf(fieldname,"CODE1");
      sprintf(fieldtype,"C");
      fieldlen = 5;
      fieldDcount = 0;
      fwrite(&fieldname  [0], 1,11,outfile);
      fwrite(&fieldtype  [0], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&fieldlen      , 1, 1,outfile);
      fwrite(&fieldDcount   , 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      
      
      sprintf(fieldname,"CODE2");
      sprintf(fieldtype,"C");
      fieldlen = 5;
      fieldDcount = 0;
      fwrite(&fieldname  [0], 1,11,outfile);
      fwrite(&fieldtype  [0], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&fieldlen      , 1, 1,outfile);
      fwrite(&fieldDcount   , 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      
      
      
      sprintf(fieldname,"LABEL1");
      sprintf(fieldtype,"C");
      fieldlen = MaxNam2LenFound;
      fieldDcount = 0;
      fwrite(&fieldname  [0], 1,11,outfile);
      fwrite(&fieldtype  [0], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&fieldlen      , 1, 1,outfile);
      fwrite(&fieldDcount   , 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      
      
      sprintf(fieldname,"LABEL2");
      sprintf(fieldtype,"C");
      fieldlen = MaxNam3LenFound;
      fieldDcount = 0;
      fwrite(&fieldname  [0], 1,11,outfile);
      fwrite(&fieldtype  [0], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&fieldlen      , 1, 1,outfile);
      fwrite(&fieldDcount   , 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
    }




  sprintf(fieldname,"ANNOTATION");
  sprintf(fieldtype,"C");
  fieldlen = MaxAnnLenFound;

  fieldDcount = 0;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);




  
  sprintf(fieldname,"ORIG_GEOM");
  sprintf(fieldtype,"C");
  fieldlen = 15;
  fieldDcount = 0;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);



  if(NGA_TYPE==1)
    {
      extern char *GetFrontPart();
      sprintf(fieldname,"%s",GetFrontPart());
      if(fieldname[0]=='\0')
	{
	  sprintf(fieldname,"unused");
	}
    }
  else
    {
      sprintf(fieldname,"SEDRISID");
    }
  sprintf(fieldtype,"C");
  fieldlen = MaxSedLenFound;
  fieldDcount = 0;
  fwrite(&fieldname  [0], 1,11,outfile);
  fwrite(&fieldtype  [0], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&fieldlen      , 1, 1,outfile);
  fwrite(&fieldDcount   , 1, 1,outfile);
  fwrite(&tempchar   [4], 1, 1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);
  fwrite(&tempchar   [4], 1,1,outfile);


  if(withradius==1)
    {
      sprintf(fieldname,"RADIUS");
      sprintf(fieldtype,"N");
      fieldlen = 15;
      fieldDcount = 8;
      fwrite(&fieldname  [0], 1,11,outfile);
      fwrite(&fieldtype  [0], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&fieldlen      , 1, 1,outfile);
      fwrite(&fieldDcount   , 1, 1,outfile);
      fwrite(&tempchar   [4], 1, 1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
      fwrite(&tempchar   [4], 1,1,outfile);
    }



 if(NGA_TYPE==1)
   {
     sprintf(fieldname,"ATTRSCHEMA");
     sprintf(fieldtype,"C");
     fieldlen = 20;
     fieldDcount = 0;
     fwrite(&fieldname  [0], 1,11,outfile);
     fwrite(&fieldtype  [0], 1, 1,outfile);
     fwrite(&tempchar   [4], 1, 1,outfile);
     fwrite(&tempchar   [4], 1, 1,outfile);
     fwrite(&tempchar   [4], 1, 1,outfile);
     fwrite(&tempchar   [4], 1, 1,outfile);
     fwrite(&fieldlen      , 1, 1,outfile);
     fwrite(&fieldDcount   , 1, 1,outfile);
     fwrite(&tempchar   [4], 1, 1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);
     fwrite(&tempchar   [4], 1,1,outfile);      
    }



 sprintf(fieldname,"RETAINIGN");
 sprintf(fieldtype,"C");
 fieldlen = 6;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   






 sprintf(fieldname,"COR_TECH");
 sprintf(fieldtype,"C");
 fieldlen = 10;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   

 sprintf(fieldname,"COR_DATE");
 sprintf(fieldtype,"D");
 fieldlen = 8;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   

 sprintf(fieldname,"C_COMMENTS");
 sprintf(fieldtype,"C");
 fieldlen = 80;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   

 
 sprintf(fieldname,"VER_TECH");
 sprintf(fieldtype,"C");
 fieldlen = 10;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   

 sprintf(fieldname,"VER_DATE");
 sprintf(fieldtype,"D");
 fieldlen = 8;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   

 sprintf(fieldname,"V_COMMENTS");
 sprintf(fieldtype,"C");
 fieldlen = 80;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   


 

 sprintf(fieldname,"ORIG_FID");
 sprintf(fieldtype,"N");
 fieldlen = 10;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);


 sprintf(fieldname,"ORIG_TABLE");
 sprintf(fieldtype,"C");
 fieldlen = maxtablelen;
 fieldDcount = 0;
 fwrite(&fieldname  [0], 1,11,outfile);
 fwrite(&fieldtype  [0], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&fieldlen      , 1, 1,outfile);
 fwrite(&fieldDcount   , 1, 1,outfile);
 fwrite(&tempchar   [4], 1, 1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);
 fwrite(&tempchar   [4], 1,1,outfile);   

 fwrite(&tempchar   [6], 1,1,outfile);

}



void DOLines(struct ConditionList * conditions,char *outname)
{
  FILE *outfile,*outDBfile;
  char MAINFILE[1000];
  char INDEXFILE[1000];
  char DBASEFILE[1000];
  int i,RECNUM=1;
  struct ConditionList *curr;


  sprintf(MAINFILE, "%sLN.shp",outname);
  sprintf(INDEXFILE,"%sLN.shx",outname);
  sprintf(DBASEFILE,"%sLN.dbf",outname);
  

  outfile = fopen(MAINFILE,"wb");

  bytes_written = 0;

  WriteMainOrIndexFileHeader(outfile, LINETYPE);


  outDBfile = fopen(DBASEFILE,"wb");
  if(outDBfile==NULL)
    {
      printf("could not open %s for write\n",DBASEFILE);
      exit(1);
    }
  WriteDBHeader(outDBfile,lines_found_export,0);



  curr = conditions;

  while(curr!=NULL)
    {
      if(curr->num_areals==1)
	{
	  if(curr->areal1.numverts>1)
	    {
	        WritePolygon(outfile, RECNUM, curr->areal1.numverts,curr->areal1.x,curr->areal1.y,curr->areal1.z,1);

	        WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_AREAL,-1,curr->areal1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->areal1.objectid,curr->areal1.Lindex);

	        RECNUM++; 
	    }
	}
      else if(curr->num_areals==2)
	{
	  if(curr->areal1.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->areal1.numverts,curr->areal1.x,curr->areal1.y,curr->areal1.z,1);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_AREAL,-1,curr->areal1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->areal1.objectid,curr->areal1.Lindex);

	      RECNUM++;
	    }

	  if(curr->areal2.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->areal2.numverts,curr->areal2.x,curr->areal2.y,curr->areal2.z,1);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_AREAL,-1,curr->areal2.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->areal2.objectid,curr->areal2.Lindex);

	      RECNUM++;
	    }
	}
      
      
      if(curr->num_polys==1)
	{
	  if(curr->poly1.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->poly1.numverts,curr->poly1.x,curr->poly1.y,curr->poly1.z,1);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POLYGON,-1,curr->poly1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->poly1.objectid,curr->poly1.Lindex);
	      
	      RECNUM++;
	    }
	}
      else if(curr->num_polys==2)
	{
	  if(curr->poly1.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->poly1.numverts,curr->poly1.x,curr->poly1.y,curr->poly1.z,1);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POLYGON,-1,curr->poly1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->poly1.objectid,curr->poly1.Lindex);
	      
	      RECNUM++;
	    }
	  
	  if(curr->poly2.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->poly2.numverts,curr->poly2.x,curr->poly2.y,curr->poly2.z,1);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POLYGON,-1,curr->poly2.IDENT,
			   curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->poly2.objectid,curr->poly2.Lindex);

	      RECNUM++;
	    }
	}
      
      
      
      if(curr->num_lines==1)
	{
	  if(curr->linear1.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->linear1.numverts,curr->linear1.x,curr->linear1.y,curr->linear1.z,0);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_LINE,-1,curr->linear1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->linear1.objectid,curr->linear1.Lindex);

	      RECNUM++;
	    }
	}
      else if(curr->num_lines==2)
	{
	  if(curr->linear1.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->linear1.numverts,curr->linear1.x,curr->linear1.y,curr->linear1.z,0);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_LINE,-1,curr->linear1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->linear1.objectid,curr->linear1.Lindex);

	      RECNUM++;
	    }
	  
	  if(curr->linear2.numverts>1)
	    {
	      WritePolygon(outfile, RECNUM, curr->linear2.numverts,curr->linear2.x,curr->linear2.y,curr->linear2.z,0);

	      WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_LINE,-1,curr->linear2.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->linear2.objectid,curr->linear2.Lindex);

	      RECNUM++;
	    }
	}
      
      
      if(curr->num_piped==1)
	{
	  WritePolygon(outfile, RECNUM, 4,curr->piped1.x,curr->piped1.y,curr->piped1.z,1);

	  WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_PARALLELEPIPED,-1,curr->piped1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->piped1.objectid,curr->piped1.Lindex);

	  RECNUM++;
	}
      else if(curr->num_piped==2)
	{
	  WritePolygon(outfile, RECNUM, 4,curr->piped1.x,curr->piped1.y,curr->piped1.z,1);

	  WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_PARALLELEPIPED,-1,curr->piped1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->piped1.objectid,curr->piped1.Lindex);

	  RECNUM++;

	  WritePolygon(outfile, RECNUM, 4,curr->piped2.x,curr->piped2.y,curr->piped2.z,1);

	  WriteDBEntry(outDBfile,0, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_PARALLELEPIPED,-1,curr->piped2.IDENT,
		       curr->ECC1, curr->ECC2,
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->piped2.objectid,curr->piped2.Lindex);

	  RECNUM++;
	}

      lines_done = RECNUM-1;
      
      curr = curr->next;
    }

  RECNUM--;

  record_bytes(outfile);
  fclose(outfile);





  outfile = fopen(INDEXFILE,"wb");
  
  bytes_written = 0;
  WriteMainOrIndexFileHeader(outfile, LINETYPE);

  CalculateOffsets(RECNUM);

  for(i=1;i<=RECNUM;i++)
    {
      WriteIndexFileEntry(outfile, i);
    }
  record_bytes(outfile);
  fclose(outfile);



  WriteEndOfDBFile(outDBfile);
  fclose(outDBfile);
}






void WritePoint(FILE *outfile,int RECNUM,
		double Xpoint, double Ypoint, double Zpoint)
{
  int con_len;
  int numverts=1;
  double Zmsure=0;
   
  if(tempis2D(Zpoint))
  {
	Zpoint = EXPORT2DSENTINEL;
  }


  file_endianness = BIG;
  SEEIT_fwrite_int(&RECNUM,       outfile);  bytes_written = bytes_written+4;

  con_len = (numverts)*4*8;  
  con_len = con_len + 4;     
  con_len = con_len/2;       
  
  G_length   [RECNUM] = con_len + 4;  

  SEEIT_fwrite_int(&con_len,       outfile);  bytes_written = bytes_written+4;

  file_endianness = LITTLE;

  SEEIT_fwrite_int(&POINTTYPE,       outfile);  bytes_written = bytes_written+4;
  SEEIT_fwrite_double(&Xpoint,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Ypoint,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Zpoint,       outfile);  bytes_written = bytes_written+8;
  SEEIT_fwrite_double(&Zmsure,       outfile);  bytes_written = bytes_written+8;

}




void DoPoints(struct ConditionList * conditions,char *outname)
{
  FILE *outfile,*outDBfile;
  char MAINFILE[1000];
  char INDEXFILE[1000];
  char DBASEFILE[1000];
  int i,RECNUM=1;
  struct ConditionList * curr;
  

  sprintf(MAINFILE, "%sPT.shp",outname);
  sprintf(INDEXFILE,"%sPT.shx",outname);
  sprintf(DBASEFILE,"%sPT.dbf",outname);
  

  outfile = fopen(MAINFILE,"wb");

  bytes_written = 0;

  WriteMainOrIndexFileHeader(outfile, POINTTYPE);



  outDBfile = fopen(DBASEFILE,"wb");  
  if(outDBfile==NULL)
    {
      printf("could not open %s\n",DBASEFILE);
      exit(1);
    }


  WriteDBHeader(outDBfile,points_found_export,1);



  
  curr = conditions;
  
  
  while(curr!=NULL)
    {
  
      if(curr->num_points==1)
	{
	  WritePoint(outfile, RECNUM,curr->point1.x,curr->point1.y,curr->point1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POINT,-1.0,curr->point1.IDENT,
	  		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->point1.objectid,curr->point1.Lindex);

	  RECNUM++;
	}
      else if(curr->num_points==2)
	{
	  WritePoint(outfile, RECNUM,curr->point1.x,curr->point1.y,curr->point1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POINT,-1.0,curr->point1.IDENT,
		       curr->ECC1, curr->ECC2,
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->point1.objectid,curr->point1.Lindex);
	  RECNUM++;


	  WritePoint(outfile, RECNUM,curr->point2.x,curr->point2.y,curr->point2.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POINT,-1.0,curr->point2.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->point2.objectid,curr->point2.Lindex);
	  RECNUM++;
	}

      
      if(curr->num_cylinder==1)
	{
	  WritePoint(outfile, RECNUM,curr->cylinder1.x,curr->cylinder1.y,curr->cylinder1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_CYLINDER,curr->cylinder1.radius,curr->cylinder1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->cylinder1.objectid,curr->cylinder1.Lindex);
	  RECNUM++;
	}
      else if(curr->num_cylinder==2)
	{
	  WritePoint(outfile, RECNUM,curr->cylinder1.x,curr->cylinder1.y,curr->cylinder1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_CYLINDER,curr->cylinder1.radius,curr->cylinder1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->cylinder1.objectid,curr->cylinder1.Lindex);
	  RECNUM++;
	  WritePoint(outfile, RECNUM,curr->cylinder2.x,curr->cylinder2.y,curr->cylinder2.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_CYLINDER,curr->cylinder1.radius,curr->cylinder2.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->cylinder2.objectid,curr->cylinder2.Lindex);
	  RECNUM++;
	}


      if(curr->num_sphere==1)
	{
	  WritePoint(outfile, RECNUM,curr->sphere1.x,curr->sphere1.y,curr->sphere1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_SPHERE,curr->sphere1.radius,curr->sphere1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->sphere1.objectid,curr->sphere1.Lindex);
	  RECNUM++;
	}
      else if(curr->num_sphere==2)
	{
	  WritePoint(outfile, RECNUM,curr->sphere1.x,curr->sphere1.y,curr->sphere1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_SPHERE,curr->sphere1.radius,curr->sphere1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->sphere1.objectid,curr->sphere1.Lindex);
	  RECNUM++;

	  WritePoint(outfile, RECNUM,curr->sphere2.x,curr->sphere2.y,curr->sphere2.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_SPHERE,curr->sphere1.radius,curr->sphere2.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->sphere2.objectid,curr->sphere2.Lindex);
	  RECNUM++;
	}


      if(curr->num_circles==1)
	{
	  WritePoint(outfile, RECNUM,curr->circle1.x,curr->circle1.y,curr->circle1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_CIRCLE,curr->circle1.radius,curr->circle1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->circle1.objectid,curr->circle1.Lindex);
	  RECNUM++;
	}
      else if(curr->num_circles==2)
	{
	  WritePoint(outfile, RECNUM,curr->circle1.x,curr->circle1.y,curr->circle1.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_CIRCLE,curr->circle1.radius,curr->circle1.IDENT,
		       curr->ECC1, curr->ECC2,
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->circle1.objectid,curr->circle1.Lindex);
	  RECNUM++;


	  WritePoint(outfile, RECNUM,curr->circle2.x,curr->circle2.y,curr->circle2.z);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_CIRCLE,curr->circle1.radius,curr->circle2.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->circle2.objectid,curr->circle2.Lindex);
	  RECNUM++;
	}


      if(curr->num_gridpt==1)
	{
	  WritePoint(outfile, RECNUM,curr->gridobj1.x[0],curr->gridobj1.y[0],curr->gridobj1.z[0]);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_GRIDPT,-1,curr->gridobj1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->gridobj1.objectid,curr->gridobj1.Lindex);
	  RECNUM++;
	}
      else if(curr->num_gridpt==2)
	{
	  WritePoint(outfile, RECNUM,curr->gridobj1.x[0],curr->gridobj1.y[0],curr->gridobj1.z[0]);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_GRIDPT,-1,curr->gridobj1.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->gridobj1.objectid,curr->gridobj1.Lindex);
	  RECNUM++;


	  WritePoint(outfile, RECNUM,curr->gridobj2.x[0],curr->gridobj2.y[0],curr->gridobj2.z[0]);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_GRIDPT,-1,curr->gridobj2.IDENT,
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->gridobj2.objectid,curr->gridobj2.Lindex);
	  RECNUM++;
	}





      if(curr->num_areals==1)
	{
	  if(curr->areal1.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->areal1.x[0],curr->areal1.y[0],curr->areal1.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_AREAL,-1,curr->areal1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->areal1.objectid,curr->areal1.Lindex);
	      RECNUM++;
	    }
	}
      else if(curr->num_areals==2)
	{
	  if(curr->areal1.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->areal1.x[0],curr->areal1.y[0],curr->areal1.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_AREAL,-1,curr->areal1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->areal1.objectid,curr->areal1.Lindex);

	      RECNUM++;
	    }
	  
	  if(curr->areal2.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->areal2.x[0],curr->areal2.y[0],curr->areal2.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_AREAL,-1,curr->areal2.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->areal2.objectid,curr->areal2.Lindex);

	      RECNUM++;
	    }
	}
      
      
      if(curr->num_polys==1)
	{
	  if(curr->poly1.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->poly1.x[0],curr->poly1.y[0],curr->poly1.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POLYGON,-1,curr->poly1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->poly1.objectid,curr->poly1.Lindex);
	      
	      RECNUM++;
	    }
	}
      else if(curr->num_polys==2)
	{
	  if(curr->poly1.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->poly1.x[0],curr->poly1.y[0],curr->poly1.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POLYGON,-1,curr->poly1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->poly1.objectid,curr->poly1.Lindex);
	      
	      RECNUM++;
	    }
	  
	  if(curr->poly2.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->poly2.x[0],curr->poly2.y[0],curr->poly2.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_POLYGON,-1,curr->poly2.IDENT,
			   curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->poly2.objectid,curr->poly2.Lindex);

	      RECNUM++;
	    }
	}
      
     
      
      if(curr->num_lines==1)
	{
	  if(curr->linear1.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->linear1.x[0],curr->linear1.y[0],curr->linear1.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_LINE,-1,curr->linear1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->linear1.objectid,curr->linear1.Lindex);
	      
	      RECNUM++;
	    }
	}
      else if(curr->num_lines==2)
	{
	  if(curr->linear1.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->linear1.x[0],curr->linear1.y[0],curr->linear1.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_LINE,-1,curr->linear1.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->linear1.objectid,curr->linear1.Lindex);

	      RECNUM++;
	    }
	  
	  if(curr->linear2.numverts==1)
	    {
	      WritePoint(outfile, RECNUM ,curr->linear2.x[0],curr->linear2.y[0],curr->linear2.z[0]);

	      WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
			   curr->CONDITION_INSTANCE,curr->ordinal_num,SH_LINE,-1,curr->linear2.IDENT,
			   curr->ECC1, curr->ECC2, 
			   SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,
			   curr->linear2.objectid,curr->linear2.Lindex);

	      RECNUM++;
	    }
	}
      

      if(curr->AdditionalPointUsed==1)
	{
	  WritePoint(outfile, RECNUM,curr->pointx,curr->pointy,curr->pointz);

	  WriteDBEntry(outDBfile,1, curr->CONDITION_TYPE,curr->magnitude,
		       curr->CONDITION_INSTANCE,curr->ordinal_num,SH_NONE,-1.0,"Not Applicable",
		       curr->ECC1, curr->ECC2, 
		       SEEIT_GetConditionAnnotation(curr),
			   curr->retainignore,-1,-1);
	  RECNUM++;
	}

      
      points_done = RECNUM-1;
      
      curr = curr->next;
    }

  RECNUM--;

  record_bytes(outfile);
  fclose(outfile);







  outfile = fopen(INDEXFILE,"wb");
  
  bytes_written = 0;
  WriteMainOrIndexFileHeader(outfile, POINTTYPE);

  CalculateOffsets(RECNUM);

  for(i=1;i<=RECNUM;i++)
    {
      WriteIndexFileEntry(outfile, i);
    }
  record_bytes(outfile);
  fclose(outfile);





  WriteEndOfDBFile(outDBfile);
  fclose(outDBfile);

}





void TestVert(double Xvert, double Yvert, double Zvert)
{

  if(tempis2D(Zvert))
	{
	  Zvert = EXPORT2DSENTINEL;
	}


  if(Xvert<Gminx)
    {
      Gminx = Xvert;
    }
  if(Yvert<Gminy)
    {
      Gminy = Yvert;
    }
  if(Zvert<Gminz)
    {
      Gminz = Zvert;
    }

  if(Xvert>Gmaxx)
    {
      Gmaxx = Xvert;
    }
  if(Yvert>Gmaxy)
    {
      Gmaxy = Yvert;
    }
  if(Zvert>Gmaxz)
    {
      Gmaxz = Zvert;
    }
}




void TestVerts(int numverts, double Xverts[], double Yverts[], double Zverts[])
{
  int i;
    
  for(i=0;i<numverts;i++)
    {
      TestVert(Xverts[i],Yverts[i],Zverts[i]);
    }
}



void TestNameLen(int G_type)
{
  if((int)strlen(SEEIT_GetConditionName(G_type))>MaxNamLenFound)
    {
      MaxNamLenFound = strlen(SEEIT_GetConditionName(G_type));
    }
}





void TestNam2Len(int index)
{

  if(index<0)
    {
      return;
    }
  
  else if( ((int)strlen(GetECCLabel(index)) + 10)>MaxNam2LenFound)
    {
      MaxNam2LenFound = strlen(GetECCLabel(index)) + 10;
    }
}
  

void TestNam3Len(int index)
{

  if(index<0)
    {
      return;
    }

  else if( ((int)strlen(GetECCLabel(index)) + 10)>MaxNam3LenFound)
    {
      MaxNam3LenFound = strlen(GetECCLabel(index)) + 10;
    }
}
  


void TestAnnLen(struct ConditionList *curr)
{
  
  if(SEEIT_GetConditionAnnotation(curr)==NULL)
    {
      return;
    }
  
  
  if((int)strlen(SEEIT_GetConditionAnnotation(curr))>MaxAnnLenFound)
    {
      MaxAnnLenFound = strlen(SEEIT_GetConditionAnnotation(curr));
    }
}



void TestSedNam(char *totest)
{
  if((int)strlen(totest)>MaxSedLenFound)
    {
      MaxSedLenFound = strlen(totest);
    }
}



int GetMaxLayerLen()
{ 
  int i,thislen,maxlen=0;
  extern int NumLODbands;
  extern struct LODdisplay *LODindex;

  for(i=0;i<NumLODbands;i++)
  {
	thislen = strlen(LODindex[i].filename);
	if(thislen>maxlen)
	{
	  maxlen = thislen;
	}
  }

  return maxlen + 5;
}









void GatherInformation(struct ConditionList * conditions)
{
  struct ConditionList * curr;

 

  maxtablelen = GetMaxLayerLen();

 
  curr = conditions;
  while(curr!=NULL)
    {
      
      TestNameLen(curr->CONDITION_TYPE);


      TestAnnLen (curr);



      if(NGA_TYPE==1)
	{
	  TestNam2Len(curr->ECC1);
	  TestNam3Len(curr->ECC2);
	}

      if(curr->num_areals==1)
	{
	  TestVerts(curr->areal1.numverts,curr->areal1.x,curr->areal1.y,curr->areal1.z);
	  TestSedNam(curr->areal1.IDENT);

	  if(curr->areal1.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
          lines_found_export = lines_found_export + 1;
	    }
	}
      else if(curr->num_areals==2)
	{
	  TestVerts(curr->areal1.numverts,curr->areal1.x,curr->areal1.y,curr->areal1.z);
	  TestVerts(curr->areal2.numverts,curr->areal2.x,curr->areal2.y,curr->areal2.z);
	  TestSedNam(curr->areal1.IDENT);
	  TestSedNam(curr->areal2.IDENT);

	  if(curr->areal1.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	  
	  if(curr->areal2.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	}

      if(curr->num_polys==1)
	{
	  TestVerts(curr->poly1.numverts,curr->poly1.x,curr->poly1.y,curr->poly1.z);
	  TestSedNam(curr->poly1.IDENT);

	  if(curr->poly1.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	}

      else if(curr->num_polys==2)
	{
	  TestVerts(curr->poly1.numverts,curr->poly1.x,curr->poly1.y,curr->poly1.z);
	  TestVerts(curr->poly2.numverts,curr->poly2.x,curr->poly2.y,curr->poly2.z);
	  TestSedNam(curr->poly1.IDENT);
	  TestSedNam(curr->poly2.IDENT);

	  if(curr->poly1.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	  
	  if(curr->poly2.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	}

      if(curr->num_lines==1)
	{
	  TestVerts(curr->linear1.numverts,curr->linear1.x,curr->linear1.y,curr->linear1.z);
	  TestSedNam(curr->linear1.IDENT);

	  if(curr->linear1.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	}
      else if(curr->num_lines==2)
	{
	  TestVerts(curr->linear1.numverts,curr->linear1.x,curr->linear1.y,curr->linear1.z);
	  TestVerts(curr->linear2.numverts,curr->linear2.x,curr->linear2.y,curr->linear2.z);
	  TestSedNam(curr->linear1.IDENT);
	  TestSedNam(curr->linear2.IDENT);

	  if(curr->linear1.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	  
	  if(curr->linear2.numverts==1)
	    {
	      points_found_export = points_found_export + 1;
	    }
	  else
	    {
	      lines_found_export = lines_found_export + 1;
	    }
	}

      if(curr->num_piped==1)
	{
	  TestVerts(4,curr->piped1.x,curr->piped1.y,curr->piped1.z);
	  TestSedNam(curr->piped1.IDENT);
	  lines_found_export = lines_found_export + 1;
	}
      else if(curr->num_piped==2)
	{
	  TestVerts(4,curr->piped1.x,curr->piped1.y,curr->piped1.z);
	  TestVerts(4,curr->piped2.x,curr->piped2.y,curr->piped2.z);
	  TestSedNam(curr->piped1.IDENT);
	  TestSedNam(curr->piped2.IDENT);
	  lines_found_export = lines_found_export + 2;
	}


      if(curr->num_circles==1)
	{
	  TestVert(curr->circle1.x,curr->circle1.y,curr->circle1.z);
	  TestSedNam(curr->circle1.IDENT);
	  points_found_export = points_found_export + 1;
	}
      else if(curr->num_circles==2)
	{
	  TestVert(curr->circle1.x,curr->circle1.y,curr->circle1.z);
	  TestVert(curr->circle2.x,curr->circle2.y,curr->circle2.z);
	  TestSedNam(curr->circle1.IDENT);
	  TestSedNam(curr->circle2.IDENT);
	  points_found_export = points_found_export + 2;
	}

      if(curr->num_points==1)
	{
	  TestVert(curr->point1.x,curr->point1.y,curr->point1.z);
	  TestSedNam(curr->point1.IDENT);
	  points_found_export = points_found_export + 1;
	}
      else if(curr->num_points==2)
	{
	  TestVert(curr->point1.x,curr->point1.y,curr->point1.z);
	  TestVert(curr->point2.x,curr->point2.y,curr->point2.z);
	  TestSedNam(curr->point1.IDENT);
	  TestSedNam(curr->point2.IDENT);
	  points_found_export = points_found_export + 2;
	}


      if(curr->num_cylinder==1)
	{
	  TestVert(curr->cylinder1.x,curr->cylinder1.y,curr->cylinder1.z);
	  TestSedNam(curr->cylinder1.IDENT);
	  points_found_export = points_found_export + 1;
	}
      else if(curr->num_cylinder==2)
	{
	  TestVert(curr->cylinder1.x,curr->cylinder1.y,curr->cylinder1.z);
	  TestVert(curr->cylinder2.x,curr->cylinder2.y,curr->cylinder2.z);
	  TestSedNam(curr->cylinder1.IDENT);
	  TestSedNam(curr->cylinder2.IDENT);
	  points_found_export = points_found_export + 2;
	}

      
      if(curr->num_sphere==1)
	{
	  TestVert(curr->sphere1.x,curr->sphere1.y,curr->sphere1.z);
	  TestSedNam(curr->sphere1.IDENT);
	  points_found_export = points_found_export + 1;
	}
      else if(curr->num_sphere==2)
	{
	  TestVert(curr->sphere1.x,curr->sphere1.y,curr->sphere1.z);
	  TestVert(curr->sphere2.x,curr->sphere2.y,curr->sphere2.z);
	  TestSedNam(curr->sphere1.IDENT);
	  TestSedNam(curr->sphere2.IDENT);
	  points_found_export = points_found_export + 2;
	}


      if(curr->num_gridpt==1)
	{
	  TestVert(curr->gridobj1.x[0],curr->gridobj1.y[0],curr->gridobj1.z[0]);
	  TestSedNam(curr->gridobj1.IDENT);
	  points_found_export = points_found_export + 1;
	}
      else if(curr->num_gridpt==2)
	{
	  TestVert(curr->gridobj1.x[0],curr->gridobj1.y[0],curr->gridobj1.z[0]);
	  TestVert(curr->gridobj2.x[0],curr->gridobj2.y[0],curr->gridobj2.z[0]);
	  TestSedNam(curr->gridobj1.IDENT);
	  TestSedNam(curr->gridobj2.IDENT);
	  points_found_export = points_found_export + 2;
	}



      if(curr->AdditionalPointUsed==1)
	{
	  TestVert(curr->pointx,curr->pointy,curr->pointz);
	  TestSedNam("Not Applicable");
	  points_found_export = points_found_export + 1;
	}

     

	if(Gmaxz>(modified_db_max_z+.0001))
	{
      printf("condition type %s (#%d) has z %lf > the max: %lf \n",
		SEEIT_GetConditionName(curr->CONDITION_TYPE),
		curr->ordinal_num,
		Gmaxz,
		modified_db_max_z);
	  Gmaxz = modified_db_max_z;
	}

	if(Gminz<(modified_db_min_z-.0001))
	{
      printf("condition type %s (#%d) has z %lf < the min: %lf \n",
		SEEIT_GetConditionName(curr->CONDITION_TYPE),
		curr->ordinal_num,
		Gminz,
		modified_db_min_z);
	  Gminz = modified_db_min_z;
	}

      curr=curr->next;
    }
}



void FreeArrays()
{
  free(G_length);
  free(G_offsets);
}






void MallocArrays()
{
  int maxnum = mySHmax(lines_found_export,points_found_export);


  G_length = (int *)(malloc(sizeof(int)*(maxnum+5)));
  if(G_length==NULL)
    {
      printf("unable to alloc G_length %d\n",maxnum+5);
      exit(1);
    }
  
  G_offsets = (int *)(malloc(sizeof(int)*(maxnum+5)));
  if(G_offsets==NULL)
    {
      printf("unable to alloc G_offsets %d\n",maxnum+5);
      exit(1);
    }

}




void SEEIT_ExportShapefile(struct ConditionList * conditions, 
		     char *outname)
{
  int temp_endianness = file_endianness;
  extern int TREX_WRITE_LN_FILE;

  machine_endianness = determine_machine_endianness();

  Gminx =  10000000;
  Gminy =  10000000;
  Gminz =  10000000;
  Gmaxx = -10000000;
  Gmaxy = -10000000;
  Gmaxz = -10000000;

  lines_found_export  = 0;
  points_found_export = 0;
  lines_done   = 0;
  points_done  = 0;
 

  MaxAnnLenFound  = 0;
  MaxNam2LenFound = 0;
  MaxNam3LenFound = 0;
  MaxNamLenFound  = 0;
  MaxSedLenFound  = 0;

  modified_db_min_z = db_min_z;
  modified_db_max_z = db_max_z;

  if(db_min_z>EXPORT2DSENTINEL)
  {
	modified_db_min_z = EXPORT2DSENTINEL;
  }
  if(db_max_z<EXPORT2DSENTINEL)
  {
	modified_db_max_z = EXPORT2DSENTINEL;
  }

  GatherInformation(conditions);


  if(ATTRTYPE_TREX())
  {
	if(TREX_WRITE_LN_FILE==0)
	{
      lines_found_export = 0;
	}
  }





  MaxNamLenFound  = MaxNamLenFound  + 5;
  MaxSedLenFound  = MaxSedLenFound  + 5;
  MaxAnnLenFound  = MaxAnnLenFound  + 5;
  if(MaxAnnLenFound>253)
  {
    MaxAnnLenFound = 253;
  }
  MaxNam2LenFound = MaxNam2LenFound + 5;
  MaxNam3LenFound = MaxNam3LenFound + 5;


  MallocArrays();
 

  if(lines_found_export>0)
    {     
      DOLines(conditions,outname);
    }
  
  
  if(points_found_export>0)
    {
      DoPoints(conditions,outname);
    }
  
  FreeArrays();
  
  file_endianness = temp_endianness;
}
