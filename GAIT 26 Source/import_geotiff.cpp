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
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "share.h"

#include "shapefil.h"


#include <gdal.h>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>

#define FIN_HEADER 1
#define RAW_HEADER 2
#define HEM_HEADER 3
#define RLM_HEADER 4
#define SDM_HEADER 5
#define EDM_HEADER 6
#define WBM_HEADER 7
#define AMP_HEADER 8

#define R_HEM_HEADER 9
#define R_WAM_HEADER 10
#define R_COV_HEADER 11
#define R_AMP_HEADER 12
#define R_AM2_HEADER 13
#define R_COM_HEADER 14
#define R_LSM_HEADER 15





int USE_NUMX,USE_NUMY,USE_NUMZ,USE_ZONE2;
int USE_COORDTYPE; 
int new_tags_innitted = 0;
double USE_FACTOR,USE_MERIDIAN,USE_ZONE;
double USE_ULX,USE_ULY,USE_PIXELX,USE_PIXELY;

extern "C" int global_trex_layer;
extern "C" double min_g_xml_elev,max_g_xml_elev;
extern "C" double min_g_xml_elev_finished,max_g_xml_elev_finished;
extern "C" double min_g_xml_elev_raw,max_g_xml_elev_raw;

char USE_UNITS[100];
#define fakeval -198385287
#define fakecharval "!!!"

void CheckHeader(char *filename,int type, double llx, double lly,int finished);

extern "C" FILE *gridbin;
extern "C" int COORDINATEKEEPER,ZONE_IN_USE;
extern "C" char ErrStr[1000];
extern "C" unsigned char TIM_STRING;
extern "C" unsigned char TIM_UNI;
extern "C" unsigned char TIM_UNITLESS;

extern "C" void sh_testxyglobal(double x,double y,double z);
extern "C" int RealClose(double a, double b);

extern "C" FILE *modelbin;
extern "C" int num3point;
extern "C" long int NumPointNodes;
extern "C" int SH_Points_Done;
extern "C" int SzI,SzD;
extern "C" int global_gifd_id;
extern "C" void WriteGFID(int datafound,char *charval, int Ival, double Dval,
			   int datatype, int attr_type);
extern "C" int LONG_ATTR_AREA;
extern "C" int LONG_ATTR_HGT;
extern "C" int LONG_ATTR_LEN;
extern "C" int LONG_ATTR_NAM;
extern "C" int LONG_ATTR_ZV2;
extern "C" int NOTYPE;
extern "C" int C_TYPE;
extern "C" int LONG_ATTR_ID;
extern "C" void Record_LL_Mapping(int GFID_index,int longindex, int FID);
extern "C" char *GetSlash();
extern "C" int FileExists(char *filename);






int ParseProjection(char *filename,GDALDatasetH hDataset)
{
  OGRSpatialReference poORIG;
  char *tempunits;
  const char *tempproj;
  const char *projstr_mod;
  const char *projstr;


  projstr_mod = GDALGetProjectionRef(hDataset);
  projstr = GDALGetProjectionRef(hDataset);

  

  if(poORIG.importFromWkt((char **)&projstr_mod)!=OGRERR_NONE)
  {
	  printf("Error: Unable to parse WKT\n %s for\n %s\n",projstr_mod,filename);
	  sprintf(ErrStr,"Error: Unable to parse WKT\n %s\n for %s\n",projstr_mod,filename);
	  return 0;
  }

  if(!strncmp(projstr,"GEOGCS",6))
  {
	  USE_COORDTYPE = 1;
      USE_FACTOR = poORIG.GetAngularUnits(&tempunits);
	  USE_ZONE = -999;
	  USE_MERIDIAN = -999;

	  

	  if( (strcmp(tempunits,"degrees"))  &&  (strcmp(tempunits,"degree"))  )
	  {
        printf("Error:  File %s\nCannot process Geographic dataset with spacing in\n %s\n",filename,tempunits);
        sprintf(ErrStr,"Error:  File %s\nCannot process Geographic dataset with spacing in\n %s\n",filename,tempunits);
		return 0;
	  }
  }
  else if(!strncmp(projstr,"PROJCS",6))
  {
	  USE_COORDTYPE = 2;
      USE_FACTOR = poORIG.GetLinearUnits(&tempunits);
      USE_MERIDIAN = poORIG.GetProjParm(SRS_PP_CENTRAL_MERIDIAN,999);
      USE_ZONE = poORIG.GetProjParm(SRS_PP_ZONE,999);
	  USE_ZONE2 = poORIG.GetUTMZone();


	  if(ZONE_IN_USE==-999)
	  {
        ZONE_IN_USE = USE_ZONE2;
	  }
	  else if(ZONE_IN_USE!=USE_ZONE2)
	  {
        printf("Error:  cannot import %s\n because it is a different zone (%d) than a previous utm dataset\n in the InputSpecFile with zone %d\n",
			filename,USE_ZONE2,ZONE_IN_USE);
		sprintf(ErrStr,"Error:  cannot import %s\n because it is a different zone (%d) than a previous utm dataset\n in the InputSpecFile with zone %d\n",
			filename,USE_ZONE2,ZONE_IN_USE);
		return 0;
	  }


	  tempproj = poORIG.GetAttrValue("PROJECTION");

	  if(tempproj==NULL)
	  {
	    printf("Error: Unable to determine PROJECTION for\n %s\n",filename);
		return 0;
	  }

	  if(tempproj[0]=='\0')
	  {
	    printf("Error: Unable (2) to determine PROJECTION for\n %s\n",filename);
		return 0;
	  }


	  if(
		  (!strstr(tempproj,"mercator")) &&
		  (!strstr(tempproj,"MERCATOR")) &&
		  (!strstr(tempproj,"Mercator")) &&
		  (!strstr(tempproj,"Mercator")) &&
		  (!strstr(tempproj,"Mercater")) &&
		  (!strstr(tempproj,"mercater")) &&
		  (!strstr(tempproj,"UTM")) &&
		  (!strstr(tempproj,"TM"))
		)
	  {
	    printf("Error: Found PROJECTION %s\n not of Mercator type for\n %s\n",tempproj,filename);
	    sprintf(ErrStr,"Error: Found PROJECTION\n %s not of Mercator type for\n %s\n",tempproj,filename);
		return 0;
	  }

	  if( 
		  (strcmp(tempunits,"metres"))  && 
		  (strcmp(tempunits,"meters"))  && 
		  (strcmp(tempunits,"metre"))  && 
		  (strcmp(tempunits,"meter"))
		)
	  {
        printf("Error:  File %s\nCannot process Projected dataset with spacing in %s\n",filename,tempunits);
        sprintf(ErrStr,"Error:  File %s\nCannot process Projected dataset with spacing in %s\n",filename,tempunits);
		return 0;
	  }
  }
  else
  {
    printf("Error: Unable to determine projection type from\n %s\n for %s\n",projstr,filename);
    sprintf(ErrStr,"Error: Unable to determine projection type from\n %s\n for %s\n",projstr,filename);
	return 0;
  }


  sprintf(USE_UNITS,"%s",tempunits);

  return 1;
}



int SummarizeDataset(char *filename,GDALDatasetH hDataset)
{
  GDALDriverH   hDriver;
  double        adfGeoTransform[6];

  hDriver = GDALGetDatasetDriver( hDataset );

  USE_NUMX = GDALGetRasterXSize( hDataset );
  USE_NUMY = GDALGetRasterYSize( hDataset );
  USE_NUMZ = GDALGetRasterCount( hDataset );

  


  if(GDALGetProjectionRef(hDataset) != NULL )
  {
	  if(ParseProjection(filename,hDataset)==0)
	  {
		  return 0;
	  }
  }


  if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
  {
    USE_ULX = adfGeoTransform[0];
    USE_ULY = adfGeoTransform[3];
	USE_PIXELX = adfGeoTransform[1];
	USE_PIXELY = adfGeoTransform[5];


	

	

	if(  !RealClose(adfGeoTransform[2],0.0) || !RealClose(adfGeoTransform[4],0.0)  )
	{
      printf("Warning!  Bad params %lf %lf for %s\n",adfGeoTransform[2],adfGeoTransform[4],filename);
	  printf("Unable to process gridded data that is not \"north-up\"\n");
	  return 0;
	}

	if(USE_PIXELY>=0)
	{
	  printf("Unable to process gridded data that is not \"north-up\".  UPY: %lf\n",USE_PIXELY);
	  return 0;
	}

  }
  else
  {
    printf("Error in GDGT for %s\n",filename);
	return 0;
  }

  return 1;
}


double GetGCPNum(double thisid,char *dmsnum_in,int lowval,int highval, int type)
{
  char first[4],second[4],third[50],thisval[500],dmsnum[500];
  double i3,retval,errval = -999999999.9;
  int i,i1,i2,len,insertindex,dotindex,neg=0;

  


  len = strlen(dmsnum_in);
  insertindex = 0;
  for(i=0;i<len;i++)
  {
    if(dmsnum_in[i]!=' ')
	{
      dmsnum[insertindex] = dmsnum_in[i];
	  insertindex = insertindex + 1;
	}
  }
  dmsnum[insertindex] = '\0';


  if(dmsnum[0]=='-')
  {
    neg = 1;
    sprintf(thisval,"%s",&dmsnum[1]);
  }
  else
  {
    sprintf(thisval,"%s",dmsnum);
  }

  len = strlen(thisval);
  dotindex = -1;
  for(i=0;i<len;i++)
  {
    if(thisval[i]=='.')
	{
      dotindex = i;
	  break;
	}
  }


  if((dotindex!=5)&&(dotindex!=6)&&(dotindex!=7))
  {
    printf("Error: unable to process control point index %lf (%d %s)\n",thisid,type,dmsnum);
	printf("  Expected 5,6, or 7 numbers to the left of a decimal, got %d\n",dotindex);
	return errval;
  }


  if(dotindex==5)
  {
    first[0] = thisval[0];
    first[1] = '\0';

    second[0] = thisval[1];
    second[1] = thisval[2];
    second[2] = '\0';

	sprintf(third,"%s",&thisval[3]);
  }

  if(dotindex==6)
  {
    first[0] = thisval[0];
    first[1] = thisval[1];
    first[2] = '\0';

    second[0] = thisval[2];
    second[1] = thisval[3];
    second[2] = '\0';

	sprintf(third,"%s",&thisval[4]);
  }

  if(dotindex==7)
  {
    first[0] = thisval[0];
    first[1] = thisval[1];
    first[2] = thisval[2];
    first[3] = '\0';

    second[0] = thisval[3];
    second[1] = thisval[4];
    second[2] = '\0';

	sprintf(third,"%s",&thisval[5]);
  }

  sscanf(first,"%d",&i1);
  sscanf(second,"%d",&i2);
  sscanf(third,"%lf",&i3);

  if((i2<0.0)||(i3<0.0)||(i2>60.0)||(i3>60.0))
  {
    printf("Error: unable to process control point index %lf\n",thisid);
	printf("  Expected minutes 0-60 and seconds 0-60, got %d and %lf\n",i2,i3);
	return errval;
  }

  retval = i1 + i2/60.0 + i3/3600.0;

  if(neg==1)
  {
    retval = retval * -1.0;
  }

  

  if((retval<lowval)||(retval>highval))
  {
    printf("Error: unable to process control point index %lf\n",thisid);
	if(type==1)
	{
	  printf("  Expected longitude value in range %d-%d, got %lf\n",lowval,highval,retval);
	}
	else if(type==2)
	{
	  printf("  Expected latitude value in range %d-%d, got %lf\n",lowval,highval,retval);
	}

	return errval;
  }


  return retval;
}





extern "C" void GetGCPIndices(int *gcplong, int *gcpshort,int addtolong,int addtoshort, int type,char *filename)
{
  struct sh_long_attr_list  SH_SAC_LONG [SH_LISTLEN];   
  struct sh_short_attr_list SH_SAC_SHORT[SH_LISTLEN]; 
  char shortname[1000];
  int i,index,index2,sh_sac_short_index=0, sh_sac_long_index=0,attrindex,codeindex;

  extern int SH_FindSCC_LONG(int type, int scc, int numsac, 
			     struct sh_long_attr_list SAC_LONG[], int record_num);
  extern int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			      struct sh_short_attr_list SAC_SHORT[], int record_num);
  extern int SilentGetAttrIndex(char *eac);
  extern int SilentGetCodeIndex(char *ecc);
  extern void GetShortName(char shortname[],char *shape_to_import);
  extern int gcpindex1_0,gcpindex1_1,gcpindex1_2,gcpindex1_3;
  extern int gcpindex1s_0,gcpindex1s_1,gcpindex1s_2,gcpindex1s_3;


  if(type==1) 
  {
	if((global_trex_layer==0)&&(gcpindex1_0>=0)){*gcplong  = gcpindex1_0; *gcpshort  = gcpindex1s_0; return;}
	if((global_trex_layer==1)&&(gcpindex1_1>=0)){*gcplong  = gcpindex1_1; *gcpshort  = gcpindex1s_1; return;}
	if((global_trex_layer==2)&&(gcpindex1_2>=0)){*gcplong  = gcpindex1_2; *gcpshort  = gcpindex1s_2; return;}
	if((global_trex_layer==3)&&(gcpindex1_3>=0)){*gcplong  = gcpindex1_3; *gcpshort  = gcpindex1s_3; return;}

  }
  else
  {
    printf("bad type %d to GGPI\n",type);
  }


  codeindex = SilentGetCodeIndex((char *)"CA030") - 1;

  GetShortName(shortname,filename);
 



  


  sh_sac_short_index = 0;
  sh_sac_long_index  = 0;


  for(i=0;i<SH_LISTLEN;i++)
  {
    SH_SAC_LONG [i].lod_stuff = NULL;
    SH_SAC_SHORT[i].lod_stuff = NULL;
    SH_SAC_LONG [i].nam_stuff = NULL;
    SH_SAC_SHORT[i].nam_stuff = NULL;
    SH_SAC_LONG [i].Cvalue    = NULL;
  } 


  SH_SAC_LONG[sh_sac_long_index].lod_stuff = (char *)malloc(100);
  if(SH_SAC_LONG[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}

  SH_SAC_SHORT[sh_sac_short_index].lod_stuff = (char *)malloc(100);
  if(SH_SAC_SHORT[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}

  sprintf(SH_SAC_LONG [sh_sac_long_index ].lod_stuff,"%d 5 NGA Control Point",codeindex);
  sprintf(SH_SAC_SHORT[sh_sac_short_index].lod_stuff,"%d 5 NGA Control Point",codeindex);

  SH_SAC_LONG [sh_sac_long_index ].attr_code = -1;
  SH_SAC_SHORT[sh_sac_short_index].attr_code = -1;
  
  sh_sac_short_index++;
  sh_sac_long_index++;



   

   attrindex = SilentGetAttrIndex((char *)"TXT");
	  
   SH_SAC_LONG[sh_sac_long_index].attr_code  = attrindex;
   SH_SAC_LONG[sh_sac_long_index].unit_code  = TIM_UNITLESS;
   SH_SAC_LONG[sh_sac_long_index].scale_code = TIM_UNI;
   SH_SAC_LONG[sh_sac_long_index].SEDRIStype = TIM_STRING;
   SH_SAC_LONG[sh_sac_long_index].Cvalue     = NULL;
   SH_SAC_LONG[sh_sac_long_index].lod_stuff  = NULL;
   SH_SAC_LONG[sh_sac_long_index].nam_stuff  = NULL;
   SH_SAC_LONG[sh_sac_long_index].geomtype   = 2;  
   SH_SAC_LONG[sh_sac_long_index].Cvalue     =  (char *) malloc(50);
   sprintf(SH_SAC_LONG[sh_sac_long_index].Cvalue,"%s","\"NGA Control Point\"");

   SH_SAC_SHORT[sh_sac_long_index].attr_code = attrindex;
   SH_SAC_SHORT[sh_sac_long_index].lod_stuff = NULL;
   SH_SAC_SHORT[sh_sac_long_index].nam_stuff = NULL;

   sh_sac_short_index++;
   sh_sac_long_index++;

   index  = SH_FindSCC_LONG (5,codeindex,sh_sac_long_index, SH_SAC_LONG,-1);
   index2 = SH_FindSCC_SHORT(5,codeindex,sh_sac_short_index,SH_SAC_SHORT,-1);

   index  = index  + addtolong;
   index2 = index2 + addtoshort;



   

   for(i=0;i<sh_sac_long_index;i++)
   {
     if(SH_SAC_LONG[i].attr_code==-1)
	 { 
	   free(SH_SAC_LONG[i].lod_stuff);
       SH_SAC_LONG[i].lod_stuff = NULL;
	 } 

     if(SH_SAC_LONG[i].attr_code==-2)
	 { 
	   free(SH_SAC_LONG[i].nam_stuff);
	   SH_SAC_LONG[i].nam_stuff = NULL;
	 } 

     if(SH_SAC_LONG[i].Cvalue!=NULL)
	 { 
	   free(SH_SAC_LONG[i].Cvalue);
	   SH_SAC_LONG[i].Cvalue = NULL;
	 } 
   } 

   for(i=0;i<sh_sac_short_index;i++)
   { 
     if(SH_SAC_SHORT[i].attr_code==-1)
	 { 
	   free(SH_SAC_SHORT[i].lod_stuff);
	   SH_SAC_SHORT[i].lod_stuff = NULL;
	 } 

     if(SH_SAC_SHORT[i].attr_code==-2)
	 { 
	   free(SH_SAC_SHORT[i].nam_stuff);
       SH_SAC_SHORT[i].nam_stuff = NULL;
	 } 
   }


  *gcplong  = index;
  *gcpshort = index2;

  if(type==1)
  {
    if(global_trex_layer==0){gcpindex1_0  = index; gcpindex1s_0  = index2;}
    if(global_trex_layer==1){gcpindex1_1  = index; gcpindex1s_1  = index2;}
    if(global_trex_layer==2){gcpindex1_2  = index; gcpindex1s_2  = index2;}
    if(global_trex_layer==3){gcpindex1_3  = index; gcpindex1s_3  = index2;}
  }


}








void WriteGCP(double gcpx,double gcpy, double gcpz, int type, int featureid,int addtolong,int addtoshort, char *filename)
{
  int longindex,shortindex,numverts = -1,Ijunk=0;
  double Djunk=0.0;


  GetGCPIndices(&longindex,&shortindex,addtolong,addtoshort,type,filename);


  


  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_AREA); 
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_HGT);
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_LEN);
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_NAM);
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_ZV2);
  WriteGFID(0,(char *)"junk",-999,-999,C_TYPE,LONG_ATTR_ID);     


  num3point = num3point + 1;


  fwrite(&featureid,   SzI,1,modelbin);
  fwrite(&longindex,   SzI,1,modelbin);
  fwrite(&shortindex,  SzI,1,modelbin);      
  fwrite(&global_gifd_id,  SzI,1,modelbin);

  Record_LL_Mapping(global_gifd_id,longindex,featureid);

  fwrite(&numverts, SzI,1,modelbin);
  fwrite(&gcpx,     SzD,1,modelbin);
  fwrite(&gcpy,     SzD,1,modelbin);
  fwrite(&gcpz,     SzD,1,modelbin);
  
  NumPointNodes++;
  SH_Points_Done++;
  sh_testxyglobal(gcpx,gcpy,gcpz);

}


extern "C" void ImportGCP(char *filename, int type, int llx, int lly, int urx, int ury,int addtolong,int addtoshort)
{
  FILE *infile;
  char aline[1000],tempchar[1000],thischarx[500],thischary[500];
  int thisid;
  double thisy,thisx,thisz;
  extern int badGCP,goodGCP1;

  

  printf("Import type %d GCP file %s with x-range {%d,%d} and y-range {%d,%d}\n",
	  type,filename,llx,urx,lly,ury);

  infile = fopen(filename,"r");
  if(infile!=NULL)
  {

    fgets(aline,1000,infile);

	while(!feof(infile))
	{

	  

      sprintf(tempchar,"%s",aline);
	  tempchar[19] = '\0';
      sscanf(tempchar,"%d",&thisid);

      

	  sprintf(thischary,"%s",&aline[19]);
	  thischary[12] = '\0';

	  sprintf(thischarx,"%s",&aline[31]);
	  thischarx[12] = '\0';

	  sprintf(tempchar,"%s",&aline[43]);
	  sscanf(tempchar,"%lf",&thisz);

	  


	  

	  thisx = GetGCPNum(thisid,thischarx,llx,urx,1);
	  thisy = GetGCPNum(thisid,thischary,lly,ury,2);

	  if((thisx>-999999999)&&(thisy>-999999999))
	  {
        WriteGCP(thisx,thisy,thisz,type,thisid,addtolong,addtoshort,filename);
		goodGCP1 = goodGCP1 + 1;
	  }
	  else
	  {
        badGCP = badGCP + 1;
	  }

	  fgets(aline,1000,infile);

	}
    fclose(infile);
  }
}


extern "C" int IsOKGCPName(int type, char *gcpname, char *filename)
{
  extern FILE *badtrex;
  char thisnum[20];
  int llx;

  

  if(type==1)
  {
    if((gcpname[0]!='n')&&(gcpname[0]!='N')&&(gcpname[0]!='s')&&(gcpname[0]!='S'))
	{
	  fprintf(badtrex,"IMPORT_SENTINEL Error: NGA Control Point Filename is invalid:\n %s\n",filename);
	  fprintf(badtrex,"IMPORT_SENTINEL   This file should begin with 'n', 'N', 's', or 'S'\n");
      return 0;
	} 

	if(
		((gcpname[1]<'0')||(gcpname[1]>'9'))  ||
		((gcpname[2]<'0')||(gcpname[2]>'9'))  ||
		((gcpname[4]<'0')||(gcpname[4]>'9'))  ||
		((gcpname[5]<'0')||(gcpname[5]>'9'))  ||
		((gcpname[6]<'0')||(gcpname[6]>'9'))
	  )
	{
	  fprintf(badtrex,"IMPORT_SENTINEL Error: NGA Control Point Filename (%s) is invalid:\n %s\n",gcpname,filename);
	  fprintf(badtrex,"IMPORT_SENTINEL   Characters 6,7,9,10,11 should all be [0-9]\n");
      return 0;
	} 

	thisnum[0] = gcpname[4];
	thisnum[1] = gcpname[5];
	thisnum[2] = gcpname[6];
	thisnum[3] = '\0';
    sscanf(thisnum,"%d",&llx);

	if((llx<-180)||(llx>179))
	{
	  fprintf(badtrex,"IMPORT_SENTINEL Error: NGA Control Point Filename is invalid:\n %s\n",filename);
	  fprintf(badtrex,"IMPORT_SENTINEL   Easting %d is invalid\n",llx);
      return 0;
	} 

	thisnum[0] = gcpname[1];
	thisnum[1] = gcpname[2];
	thisnum[2] = '\0';
    sscanf(thisnum,"%d",&llx);

	if((llx<-90)||(llx>89))
	{
	  fprintf(badtrex,"IMPORT_SENTINEL Error: NGA Control Point Filename is invalid:\n %s\n",filename);
	  fprintf(badtrex,"IMPORT_SENTINEL   Northing %d is invalid\n",llx);
      return 0;
	} 

  }  


  return 1;
}


extern "C" void SaveTREXid(char *trexid)
{
  FILE *savetrexfile;
  char outtrexname[1000];
  extern char importoutputdir[1000];
  extern int SLASHTYPE;

  

  if(SLASHTYPE==NONDOS_TYPE)
  { 
    sprintf(outtrexname,"%sinternal_data/tidf.bin",importoutputdir);
  }
  else
  {
    sprintf(outtrexname,"%sinternal_data\\tidf.bin",importoutputdir);
  }

  savetrexfile = fopen(outtrexname,"w");
  if(savetrexfile==NULL)
  {
    printf("Error: Unable to open %s for write\n",outtrexname);
	return;
  }

  fprintf(savetrexfile,"%s",trexid);

  fclose(savetrexfile);
}


int MakePositive(int thenum)
{
  if(thenum<0)
  {
	return thenum*-1;
  }
  return thenum;
}

char GetNS(int northing)
{
  if(northing>=0)
  {
	return 'N';
  }
  return 'S';
}

char GetEW(int easting)
{
  if(easting>=0)
  {
	return 'E';
  }
  return 'W';
}



void GetIDs(int this_east,int this_north, char *topfolder, char *thefolder,char *theid,int northadder,int eastadder)
{
  int remainder;


  this_north = this_north + northadder;
  this_east  = this_east  + eastadder;

  if((this_north>89)||(this_north<-90))
  {
	sprintf(thefolder,"NOPATH");
	sprintf(theid,    "NOPATH");
	return;
  }
  if((this_east>179)||(this_east<-180))
  {
	sprintf(thefolder,"NOPATH");
	sprintf(theid,    "NOPATH");
	return;
  }


  sprintf(theid,"%c%02d%c%03d",GetNS(this_north),MakePositive(this_north),GetEW(this_east),MakePositive(this_east));

  if(
	  ((this_north>= 60)&&(this_north<= 79))  ||
	  ((this_north<=-60)&&(this_north>=-79))
	)
  {
	

	remainder = abs(this_east%2);  
	this_east=this_east-remainder;
  }
  else if((this_north>=80)||(this_north<=-80))
  {
	

	remainder = abs(this_east%4);  
	this_east=this_east-remainder;
  }

  sprintf(thefolder,"%s%s%c%02d%c%03d",topfolder,GetSlash(),GetNS(this_north),MakePositive(this_north),GetEW(this_east),MakePositive(this_east));

}




extern "C" void GetEdgeIndices(char *filename, char *annot, int addtolong, int addtoshort, int *short_ret, int *long_ret)
{
  char shortname[1000],shortannot[100],newannot[100];
  int codeindex;
  int i,index,index2,sh_sac_short_index=0, sh_sac_long_index=0,attrindex;
  struct sh_long_attr_list  SH_SAC_LONG [SH_LISTLEN];   
  struct sh_short_attr_list SH_SAC_SHORT[SH_LISTLEN]; 
  extern int SH_FindSCC_LONG(int type, int scc, int numsac, 
			     struct sh_long_attr_list SAC_LONG[], int record_num);
  extern int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			      struct sh_short_attr_list SAC_SHORT[], int record_num);
  extern int SilentGetAttrIndex(char *eac);
  extern int SilentGetCodeIndex(char *ecc);
  extern void GetShortName(char shortname[],char *shape_to_import);


  sprintf(shortannot,"%s",annot);
  shortannot[9] = '\0';  

  if(strstr(shortannot,"edge"))
  {
	
	shortannot[3] = '\0';
	sprintf(newannot,"%s neighbor edge posts",shortannot);
	sprintf(shortannot,"%s",newannot);
  }
  else
  {
    
	shortannot[3] = '\0';
	sprintf(newannot,"%s neighbor posts",shortannot);
	sprintf(shortannot,"%s",newannot);
  }


  codeindex = SilentGetCodeIndex((char *)TREX_EDGE_POINT_CODE) - 1;

  GetShortName(shortname,filename);
 
  sh_sac_short_index = 0;
  sh_sac_long_index  = 0;

  for(i=0;i<SH_LISTLEN;i++)
  {
    SH_SAC_LONG [i].lod_stuff = NULL;
    SH_SAC_SHORT[i].lod_stuff = NULL;
    SH_SAC_LONG [i].nam_stuff = NULL;
    SH_SAC_SHORT[i].nam_stuff = NULL;
    SH_SAC_LONG [i].Cvalue    = NULL;
  } 


  SH_SAC_LONG[sh_sac_long_index].lod_stuff = (char *)malloc(100);
  if(SH_SAC_LONG[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}

  SH_SAC_SHORT[sh_sac_short_index].lod_stuff = (char *)malloc(100);
  if(SH_SAC_SHORT[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}

  sprintf(SH_SAC_LONG [sh_sac_long_index ].lod_stuff,"%d 5 %s",codeindex,shortannot);
  sprintf(SH_SAC_SHORT[sh_sac_short_index].lod_stuff,"%d 5 %s",codeindex,shortannot);

  SH_SAC_LONG [sh_sac_long_index ].attr_code = -1;
  SH_SAC_SHORT[sh_sac_short_index].attr_code = -1;
  
  sh_sac_short_index++;
  sh_sac_long_index++;



  

  attrindex = SilentGetAttrIndex((char *)"TXT");
	  
  SH_SAC_LONG[sh_sac_long_index].attr_code  = attrindex;
  SH_SAC_LONG[sh_sac_long_index].unit_code  = TIM_UNITLESS;
  SH_SAC_LONG[sh_sac_long_index].scale_code = TIM_UNI;
  SH_SAC_LONG[sh_sac_long_index].SEDRIStype = TIM_STRING;
  SH_SAC_LONG[sh_sac_long_index].Cvalue     = NULL;
  SH_SAC_LONG[sh_sac_long_index].lod_stuff  = NULL;
  SH_SAC_LONG[sh_sac_long_index].nam_stuff  = NULL;
  SH_SAC_LONG[sh_sac_long_index].geomtype   = 2;  
  SH_SAC_LONG[sh_sac_long_index].Cvalue     =  (char *) malloc(strlen(annot)+50);
  sprintf(SH_SAC_LONG[sh_sac_long_index].Cvalue,"\"%s\"",annot);

  SH_SAC_SHORT[sh_sac_long_index].attr_code = attrindex;
  SH_SAC_SHORT[sh_sac_long_index].lod_stuff = NULL;
  SH_SAC_SHORT[sh_sac_long_index].nam_stuff = NULL;

  sh_sac_short_index++;
  sh_sac_long_index++;

  index  = SH_FindSCC_LONG (5,codeindex,sh_sac_long_index, SH_SAC_LONG,-1);
  index2 = SH_FindSCC_SHORT(5,codeindex,sh_sac_short_index,SH_SAC_SHORT,-1);

  index  = index  + addtolong;
  index2 = index2 + addtoshort;

  *long_ret  = index;
  *short_ret = index2;

  

  for(i=0;i<sh_sac_long_index;i++)
  {
    if(SH_SAC_LONG[i].attr_code==-1)
	{ 
	  free(SH_SAC_LONG[i].lod_stuff);
      SH_SAC_LONG[i].lod_stuff = NULL;
	}  

    if(SH_SAC_LONG[i].attr_code==-2)
	{ 
	  free(SH_SAC_LONG[i].nam_stuff);
	  SH_SAC_LONG[i].nam_stuff = NULL;
	} 

    if(SH_SAC_LONG[i].Cvalue!=NULL)
	{ 
	  free(SH_SAC_LONG[i].Cvalue);
	  SH_SAC_LONG[i].Cvalue = NULL;
	} 
  }  

  for(i=0;i<sh_sac_short_index;i++)
  { 
    if(SH_SAC_SHORT[i].attr_code==-1)
	{ 
	  free(SH_SAC_SHORT[i].lod_stuff);
	  SH_SAC_SHORT[i].lod_stuff = NULL;
	} 

    if(SH_SAC_SHORT[i].attr_code==-2)
	{ 
	  free(SH_SAC_SHORT[i].nam_stuff);
      SH_SAC_SHORT[i].nam_stuff = NULL;
	} 
  } 
}


  




void WriteGridAsPoint(double writex,double writey, double writez, int featureid, int longindex, int shortindex)
{
  int numverts = -1,Ijunk=0;
  double Djunk=0.0;

  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_AREA); 
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_HGT);
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_LEN);
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_NAM);
  WriteGFID(1,(char *)"junk",Ijunk,Djunk,NOTYPE,LONG_ATTR_ZV2);
  WriteGFID(0,(char *)"junk",-999,-999,C_TYPE,LONG_ATTR_ID);     

  num3point = num3point + 1;


  fwrite(&featureid,   SzI,1,modelbin);
  fwrite(&longindex,   SzI,1,modelbin);
  fwrite(&shortindex,  SzI,1,modelbin);      
  fwrite(&global_gifd_id,  SzI,1,modelbin);

  Record_LL_Mapping(global_gifd_id,longindex,featureid);

  fwrite(&numverts, SzI,1,modelbin);
  fwrite(&writex,   SzD,1,modelbin);
  fwrite(&writey,   SzD,1,modelbin);
  fwrite(&writez,   SzD,1,modelbin);
  
  NumPointNodes++;
  SH_Points_Done++;
  sh_testxyglobal(writex,writey,writez);

}




extern "C" void ImportDEMasPoints(char *filename,char *theid,char *annotation,int llx,int lly,int urx,int ury,int addtolong,int addtoshort,
								  char *allshapes,int files_done,int edgeorreal)
{
  double clip_llx,clip_lly,clip_urx,clip_ury;
  double small_edge_amount=.00001;
  int short_ind=0,long_ind=0,featureid=0,wroteone=0;
  extern FILE *badtrex;

  
  int pointUSE_NUMX,pointUSE_NUMY,pointUSE_NUMZ;
  double pointUSE_ULX,pointUSE_ULY,pointUSE_PIXELX,pointUSE_PIXELY,djunk;

  
  GDALDatasetH hDataset;
  GDALDriverH   hDriver;
  GDALRasterBandH hBand;
  int             nBlockXSize, nBlockYSize;
  int   nXSize;
  int i,j;
  double        adfGeoTransform[6];
  double *pafScanline9;
  double xval,yval,zval;
  OGRSpatialReference poORIG;
  const char *projstr_mod;
  const char *projstr;
  char *tempunits;

  
  extern void sh_testxyglobalZONLY(double z);
  extern int SH_Linears_Done;
  extern int SH_Areals_Done;
  extern int SH_bad_objects;
  extern int SH_BadCoords;
  extern int SH_Null_Shapes;
  extern int SH_Zero_Shapes;
  extern int SH_Bad_DBF;
  extern unsigned long int gridpointsread;
  extern void update_shape_import(char *allshapes,char *curr_file,int files_done, 
				int dbfdone, int dbftotal,
				int points_done,int linears_done,int areals_done,unsigned long int gptsdone,
				int DONE, int extra_type,int extra1, int extra2, int badstuff,
				int badstuff2, int nullshapes, int zeropart, int baddbf);


  clip_llx = llx - small_edge_amount;
  clip_lly = lly - small_edge_amount;
  clip_urx = urx + small_edge_amount;
  clip_ury = ury + small_edge_amount;

  
  
  

  GetEdgeIndices(filename,annotation,addtolong,addtoshort,&short_ind,&long_ind);

  



  GDALAllRegister();

  pointUSE_NUMX      =  -1;
  pointUSE_NUMY      =  -1;
  pointUSE_NUMZ      =  -1;
  pointUSE_ULX       = -9999999;
  pointUSE_ULY       = -9999999;
  pointUSE_PIXELX    = -9999999;
  pointUSE_PIXELY    = -9999999;



  hDataset = GDALOpen(filename,GA_ReadOnly);
  if(hDataset==NULL)
  {
	printf("could not open %s\n",filename);
	return;
  }

  hDriver = GDALGetDatasetDriver( hDataset );

  pointUSE_NUMX = GDALGetRasterXSize( hDataset );
  pointUSE_NUMY = GDALGetRasterYSize( hDataset );
  pointUSE_NUMZ = GDALGetRasterCount( hDataset );


  if(GDALGetProjectionRef(hDataset) != NULL )
  {
	projstr_mod = GDALGetProjectionRef(hDataset);
    projstr = GDALGetProjectionRef(hDataset);

    

    if(poORIG.importFromWkt((char **)&projstr_mod)!=OGRERR_NONE)
	{ 
	  printf("Error: Unable to parse WKT\n %s for\n %s\n",projstr_mod,filename);
	  sprintf(ErrStr,"Error: Unable to parse WKT\n %s\n for %s\n",projstr_mod,filename);
      GDALClose(hDataset);
	  return;
	} 

    djunk = poORIG.GetAngularUnits(&tempunits);

    if(!strncmp(projstr,"GEOGCS",6))
	{ 
	  if( (strcmp(tempunits,"degrees"))  &&  (strcmp(tempunits,"degree"))  )
	  {
        printf("Error:  File %s\nCannot process Geographic dataset with spacing in\n %s\n",filename,tempunits);
        sprintf(ErrStr,"Error:  File %s\nCannot process Geographic dataset with spacing in\n %s\n",filename,tempunits);
        GDALClose(hDataset);
		return;
	  }
	}
  }



  if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
  {
    pointUSE_ULX = adfGeoTransform[0];
    pointUSE_ULY = adfGeoTransform[3];
	pointUSE_PIXELX = adfGeoTransform[1];
	pointUSE_PIXELY = adfGeoTransform[5];


	

	

	if(  !RealClose(adfGeoTransform[2],0.0) || !RealClose(adfGeoTransform[4],0.0)  )
	{
      printf("Warning!  Bad params %lf %lf for %s\n",adfGeoTransform[2],adfGeoTransform[4],filename);
	  printf("Unable to process gridded data that is not \"north-up\"\n");
      GDALClose(hDataset);
	  return;
	}

	if(pointUSE_PIXELY>=0)
	{
	  printf("Unable to process gridded data that is not \"north-up\".  UPY: %lf\n",pointUSE_PIXELY);
      GDALClose(hDataset);
	  return;
	}
  }
  else
  {
    printf("Error in GDGT for %s\n",filename);
    GDALClose(hDataset);
	return;
  }

  if(edgeorreal==1)
  {
    printf("Import edge file: %s\n",filename);
  }
  else
  {
    printf("Import points from neighbor file: %s\n",filename);
  }

  if(GDALGetMetadataItem(hDataset,"AREA_OR_POINT",NULL)!=NULL)  
  {
    if(!strcmp(GDALGetMetadataItem(hDataset,"AREA_OR_POINT",NULL),"Point"))
	{  
      printf("Pixel type: Point, so using %lf %lf as upper left corner point location\n",pointUSE_ULX,pointUSE_ULY); 
	} 
    else
	{  
      printf("Pixel type: Area, so using %lf + %lf = %lf , %lf + %lf = %lf as upper left corner point location\n",
			  pointUSE_ULX,pointUSE_PIXELX/2.0,
			  pointUSE_ULX+pointUSE_PIXELX/2.0,
			  pointUSE_ULY,pointUSE_PIXELY/2.0,
			  pointUSE_ULY+pointUSE_PIXELY/2.0); 

	  pointUSE_ULX = pointUSE_ULX + pointUSE_PIXELX/2.0;
	  pointUSE_ULY = pointUSE_ULY + pointUSE_PIXELY/2.0;
	} 
  }


  if(
	  (pointUSE_ULX < -180)                              || 
	  (pointUSE_ULX >  180)                              || 
	  (pointUSE_ULY >   90)                              ||
	  (pointUSE_ULY <  -90)                              ||
	  ((pointUSE_ULX + (pointUSE_PIXELX*(pointUSE_NUMX-1))) > 180) ||
	  ((pointUSE_ULY + (pointUSE_PIXELY*(pointUSE_NUMY-1))) < -90)
	)
  {
	printf("Error: %s\nInvalid coordinates for Geographic data:\n  Base (NW) coord: (%lf %lf) Spacing: %lf %lf Numx,Numy: %d %d\n  Computed SE coord: (%lf %lf)\n",
				filename,pointUSE_ULX,pointUSE_ULY,pointUSE_PIXELX,pointUSE_PIXELY,pointUSE_NUMX,pointUSE_NUMY,
				pointUSE_ULX + (pointUSE_PIXELX*(pointUSE_NUMX-1)),pointUSE_ULY + (pointUSE_PIXELY*(pointUSE_NUMY-1)));
	sprintf(ErrStr,"Error: %s\nInvalid coordinates for Geographic data:\n  Base (NW) coord: (%lf %lf) Spacing: %lf %lf Numx,Numy: %d %d\n  Computed SE coord: (%lf %lf)\n",
				filename,pointUSE_ULX,pointUSE_ULY,pointUSE_PIXELX,pointUSE_PIXELY,pointUSE_NUMX,pointUSE_NUMY,
				pointUSE_ULX + (pointUSE_PIXELX*(pointUSE_NUMX-1)),pointUSE_ULY + (pointUSE_PIXELY*(pointUSE_NUMY-1)));
    GDALClose(hDataset);
	return;
  }



  hBand = GDALGetRasterBand(hDataset,1);

  nXSize = GDALGetRasterBandXSize(hBand);

  GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );

  

  pafScanline9 = (double *) malloc(sizeof(double)*nXSize);

  
  
 

  for(i=0;i<pointUSE_NUMY;i++)
  {
    if(i%10==0)
	{
	  update_shape_import(allshapes,filename, files_done,0,0,
			      SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
			      0,0,0,0,SH_bad_objects,SH_BadCoords,SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);
	}

    yval = pointUSE_ULY + pointUSE_PIXELY*i;
	if((yval<clip_lly)||(yval>clip_ury))
	{
	  
	  continue;
	}

    GDALRasterIO(hBand,GF_Read,0,i,nXSize,1,pafScanline9,nXSize,1,GDT_Float64,0,0);

    for(j=0;j<pointUSE_NUMX;j++)
	{ 

      xval = pointUSE_ULX + pointUSE_PIXELX*j;
	  if((xval<clip_llx)||(xval>clip_urx))
	  { 
	    continue;
	  } 


      

	  zval = pafScanline9[j];


	  

	  if(1) 
	  {
		featureid = featureid+1;  
		if(wroteone==0)
		{
          fprintf(badtrex,"EDGE_SENTINEL %s %s\n",annotation,theid);
		  wroteone = 1;
		}
        sh_testxyglobal(xval,yval,zval);
        WriteGridAsPoint(xval,yval,zval,featureid,long_ind,short_ind);
	  }
	} 
  }

  free(pafScanline9);

  GDALClose(hDataset);
}


void ImportNeighborEdges(char *thefolder,char *theid_in,char *edgetype_in,char *annotation_edge,int llx,int lly,int addtolong,int addtoshort,
						 char *allshapes,int files_done, int edgeorreal)
{
  char thefile[1000],annot[500],edgeid[10],edgetype[10],special_annotation[20],theid[100];
  int foundone=0,version,len;

  
  

  if(edgeorreal==0)
  {
	

	edgeid[0] = '\0';
    edgetype[0] = '\0';
	sprintf(special_annotation,"posts");
	sprintf(theid,"%s",theid_in); 
  }
  else
  {
	

	sprintf(edgeid,"_E");
	sprintf(edgetype,"%s",edgetype_in);  
	sprintf(special_annotation,"edges");

	
	

	len = strlen(thefolder);
	if(len>7)
	{
	  sprintf(theid,"%s",&thefolder[len-7]);
	}
	else
	{
	  printf("Internal error: bad folder len (%d) for edge folder path %s\n",len,thefolder);
	  return;
	}

	
  }

  if(!strcmp(edgetype_in,"N/A"))
  {
	if(strlen(edgeid)>0)
	{
	  printf("Error: TREM: %s %s (%d)\n",edgetype_in,edgeid,strlen(edgeid));
	}
  }


  for(version=99;version>=1;version--)  
  {
	if(foundone==1)
	{
	  break;
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_HEM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
      
	  foundone = 1;
	  break;
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_WBM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
      
	  foundone = 1;
	  break;
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_SDM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
      
	  foundone = 1;
	  break;
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_EDM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
      
	  foundone = 1;
	  break;
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sDEM%sTDF_%s_%02d_DEM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
      
	  foundone = 1;
	  break;
	}
  }


  if(foundone==1)
  {

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_HEM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
	  sprintf(annot,"HEM %s from %s",special_annotation,annotation_edge);
	  ImportDEMasPoints(thefile,theid,annot,llx,lly,llx+1,lly+1,addtolong,addtoshort,allshapes,files_done,edgeorreal);
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_WBM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
	  sprintf(annot,"WBM %s from %s",special_annotation,annotation_edge);
	  ImportDEMasPoints(thefile,theid,annot,llx,lly,llx+1,lly+1,addtolong,addtoshort,allshapes,files_done,edgeorreal);
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_SDM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
	  sprintf(annot,"SDM %s from %s",special_annotation,annotation_edge);
	  ImportDEMasPoints(thefile,theid,annot,llx,lly,llx+1,lly+1,addtolong,addtoshort,allshapes,files_done,edgeorreal);
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sAUXFILES%sTDF_%s_%02d_EDM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
	  sprintf(annot,"EDM %s from %s",special_annotation,annotation_edge);
	  ImportDEMasPoints(thefile,theid,annot,llx,lly,llx+1,lly+1,addtolong,addtoshort,allshapes,files_done,edgeorreal);
	}

    sprintf(thefile,"%s%sTDF_%s_%02d%s%sDEM%sTDF_%s_%02d_DEM%s.tif",thefolder,GetSlash(),theid,version,edgeid,GetSlash(),GetSlash(),theid,version,edgetype);
	if(FileExists(thefile))
	{
	  sprintf(annot,"DEM %s from %s",special_annotation,annotation_edge);
	  ImportDEMasPoints(thefile,theid,annot,llx,lly,llx+1,lly+1,addtolong,addtoshort,allshapes,files_done,edgeorreal);
	}
  }
}



void ProcessTRExEdges(char *filename,char *thefolder,char *gcphead,char *thename,int addtolong,int addtoshort,
					  char *allshapes,int files_done)
{
  char temp[20],topfolder[1000],
	  northfolder    [1000],northid    [20],
	  eastfolder     [1000],eastid     [20],
	  southfolder    [1000],southid    [20],
	  westfolder     [1000],westid     [20],
	  northeastfolder[1000],northeastid[20],
	  northwestfolder[1000],northwestid[20],
	  southeastfolder[1000],southeastid[20],
	  southwestfolder[1000],southwestid[20];
  int len,this_east=1000,this_north=1000;


  sprintf(topfolder,"%s",thefolder);
  len = strlen(topfolder);
  if(len<25)
  {
	printf("Length of folder too short.  Cannot perform edge matching\n");
	printf("%d: %s\n",len,topfolder);
	return;
  }
  topfolder[len-23] = '\0';
  


  


  temp[0] = gcphead[4];
  temp[1] = gcphead[5];
  temp[2] = gcphead[6];
  temp[3] = '\0';
  sscanf(temp,"%d",&this_east);


  temp[0] = gcphead[1];
  temp[1] = gcphead[2];
  temp[2] = '\0';
  sscanf(temp,"%d",&this_north);

  if((gcphead[3]=='w')||(gcphead[3]=='W'))
  {
    this_east = this_east*-1;
  }
  if((gcphead[0]=='s')||(gcphead[0]=='S'))
  {
    this_north = this_north*-1;
  }


  

  GetIDs(this_east,this_north,topfolder,northfolder,northid,1,0);
  GetIDs(this_east,this_north,topfolder,eastfolder,eastid,0,1);
  GetIDs(this_east,this_north,topfolder,southfolder,southid,-1,0);
  GetIDs(this_east,this_north,topfolder,westfolder,westid,0,-1);
  GetIDs(this_east,this_north,topfolder,northeastfolder,northeastid,1,1);
  GetIDs(this_east,this_north,topfolder,northwestfolder,northwestid,1,-1);
  GetIDs(this_east,this_north,topfolder,southeastfolder,southeastid,-1,1);
  GetIDs(this_east,this_north,topfolder,southwestfolder,southwestid,-1,-1);


  


  

  if((strcmp(northid,"NOPATH"))&&(strcmp(northfolder,"NOPATH")))
  {
	ImportNeighborEdges(northfolder,northid,(char *)"_ES",(char *)"the South edge of the North GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	ImportNeighborEdges(northfolder,northid,(char *)"N/A",(char *)"the North GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(southid,"NOPATH"))&&(strcmp(southfolder,"NOPATH")))
  {
	ImportNeighborEdges(southfolder,southid,(char *)"_EN",(char *)"the North edge of the South GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	ImportNeighborEdges(southfolder,southid,(char *)"N/A",(char *)"the South GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(eastid,"NOPATH"))&&(strcmp(eastfolder,"NOPATH")))
  {
	ImportNeighborEdges(eastfolder,eastid,(char *)"_EW",(char *)"the West edge of the East GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	ImportNeighborEdges(eastfolder,eastid,(char *)"N/A",(char *)"the East GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(westid,"NOPATH"))&&(strcmp(westfolder,"NOPATH")))
  {
	ImportNeighborEdges(westfolder,westid,(char *)"_EE",(char *)"the East edge of the West GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	ImportNeighborEdges(westfolder,westid,(char *)"N/A",(char *)"the West GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(northeastid,"NOPATH"))&&(strcmp(northeastfolder,"NOPATH")))
  {
	
	if(strcmp(northfolder,northeastfolder))
	{
	  ImportNeighborEdges(northeastfolder,northeastid,(char *)"_ES",(char *)"the South edge of the Northeast GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	}

	ImportNeighborEdges(northeastfolder,northeastid,(char *)"_EW",(char *)"the West edge of the Northeast GeoTile" ,this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);

	ImportNeighborEdges(northeastfolder,northeastid,(char *)"N/A",(char *)"the Northeast GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(northwestid,"NOPATH"))&&(strcmp(northwestfolder,"NOPATH")))
  {
	
	if(strcmp(northfolder,northwestfolder))
	{
	  ImportNeighborEdges(northwestfolder,northwestid,(char *)"_ES",(char *)"the South edge of the Northwest GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	}

	ImportNeighborEdges(northwestfolder,northwestid,(char *)"_EE",(char *)"the East edge of the Northwest GeoTile" ,this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);

	ImportNeighborEdges(northwestfolder,northwestid,(char *)"N/A",(char *)"the Northwest GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(southeastid,"NOPATH"))&&(strcmp(southeastfolder,"NOPATH")))
  {
	
	if(strcmp(southfolder,southeastfolder))
	{
	  ImportNeighborEdges(southeastfolder,southeastid,(char *)"_EN",(char *)"the North edge of the Southeast GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	}

	ImportNeighborEdges(southeastfolder,southeastid,(char *)"_EW",(char *)"the West edge of the Southeast GeoTile" ,this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);

	ImportNeighborEdges(southeastfolder,southeastid,(char *)"N/A",(char *)"the Southeast GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }
  if((strcmp(southwestid,"NOPATH"))&&(strcmp(southwestfolder,"NOPATH")))
  {
	
	if(strcmp(southfolder,southwestfolder))
	{
	  ImportNeighborEdges(southwestfolder,southwestid,(char *)"_EN",(char *)"the North edge of the Southwest GeoTile",this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);
	}

	ImportNeighborEdges(southwestfolder,southwestid,(char *)"_EE",(char *)"the East edge of the Southwest GeoTile" ,this_east,this_north,addtolong,addtoshort,allshapes,files_done,1);

	ImportNeighborEdges(southwestfolder,southwestid,(char *)"N/A",(char *)"the Southwest GeoCell",this_east,this_north,addtolong,addtoshort,allshapes,files_done,0);
  }

}



extern "C" void CheckRequiredFiles(char *filename,int rawfinished,int addtolong,int addtoshort,double llx,double lly,
								   char *allshapes,int files_done)
{
  int i,thislen,newlen,local_llx,local_lly,foundit=0,finished,version,foundthefile=0;
  char thefolder[1000],thefolder2[1000];
  char thename[1000];
  char thisnum[10],testname[1000];
  char gcphead[20],gcpname[1000];
  extern FILE *badtrex;
  extern int SLASHTYPE,TREX_PROCESS_EDGES;
  extern void CheckForExtraFiles(char *thefolder,char *ancfolder,char *thename,int rawfinished);

  


  sprintf(thefolder,"%s",filename);
  thislen = strlen(thefolder);

  for(i=0;i<thislen-6;i++)
  {
    if(
		((thefolder[i]=='/')   || (thefolder[i]=='\\'))   &&
        (thefolder[i+1]=='D')                             &&
        (thefolder[i+2]=='E')                             &&
        (thefolder[i+3]=='M')                             &&
		((thefolder[i+4]=='/') || (thefolder[i+4]=='\\'))
	  )
	{
      thefolder[i] = '\0';
      foundit = 1;
	}
  }


  if(foundit==1)
  {
    sprintf(thefolder2,"%s",thefolder);
	newlen = strlen(thefolder2);
	if(newlen>18)
	{
	 

	 thefolder2[newlen-14] = 'A';
	 thefolder2[newlen-13] = 'N';
	 thefolder2[newlen-12] = 'C';

	 thefolder2[newlen-6] = thefolder2[newlen-18];
	 thefolder2[newlen-5] = thefolder2[newlen-17];  
	 thefolder2[newlen-4] = thefolder2[newlen-16];

	 thefolder2[newlen-2] = '\0'; 
	}
	
	
  }

  if(thislen<24)
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error: finished DEM path too short:\n %s\n",filename);
	fprintf(badtrex,"IMPORT_SENTINEL   Cannot fully inspect data packaging/structure.\n\n");
    return;
  }

  sprintf(thename,"%s",&thefolder[thislen-22]);





  

  thename[14] = '\0';

  CheckForExtraFiles(thefolder,thefolder2,thename,rawfinished);

  




  if((rawfinished==2)  )
  {

    
    

    sprintf(gcphead,"%s",&thename[4]);

	SaveTREXid(gcphead);

	if(TREX_PROCESS_EDGES>0)
	{
	  ProcessTRExEdges(filename,thefolder,gcphead,thename,addtolong,addtoshort,allshapes,files_done); 
	} 


	for(version=99;version>=1;version--)
	{
      sprintf(&gcphead[7],"%s",".ctl");
      gcphead[11] = '\0';


      if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
	  {
        sprintf(gcpname,"%s%02d\\GCP\\GCP_%s",thefolder2,version,gcphead);
	  } 
      else
	  {  
        sprintf(gcpname,"%s%02d/GCP/GCP_%s",thefolder2,version,gcphead);
	  }  
	  if(FileExists(gcpname))
	  {
		foundthefile = 1;
		break; 
	  }
	}


	if(foundthefile==1)
	{
	  if(FileExists(gcpname))
	  { 
	    if(IsOKGCPName(1,gcphead,gcpname)) 
		{ 
		  thisnum[0] = gcphead[4];
		  thisnum[1] = gcphead[5];
		  thisnum[2] = gcphead[6];
		  thisnum[3] = '\0';
          sscanf(thisnum,"%d",&local_llx);

	      if((gcphead[3]=='w')||(gcphead[3]=='W'))
		  {  
		    local_llx = local_llx * -1;
		  }  

		  thisnum[0] = gcphead[1];
		  thisnum[1] = gcphead[2];
		  thisnum[2] = '\0';
          sscanf(thisnum,"%d",&local_lly);

	      if((gcphead[0]=='s')||(gcphead[0]=='S'))
		  {  
		    local_lly = local_lly * -1;
		  }  

	      ImportGCP(gcpname,1,local_llx,local_lly,local_llx+1,local_lly+1,addtolong,addtoshort); 
		}
	  }
	}
  }


  if(foundit==0)
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error: Could not find \"DEM\" folder in pathname:\n %s\n",thefolder);
	fprintf(badtrex,"IMPORT_SENTINEL   Cannot fully inspect data packaging/structure.\n\n");
    return;
  }


  if(rawfinished==1)
  {

	finished = 0;

	

    CheckHeader(filename,RAW_HEADER,llx,lly,finished);

	sprintf(testname,"%s%sAUXFILES%s%s_RLM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
    else
	{
	  CheckHeader(testname,RLM_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_HEM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	   CheckHeader(testname,R_HEM_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_WAM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
      CheckHeader(testname,R_WAM_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_COV.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,R_COV_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_AMP.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,R_AMP_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_AM2.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
      CheckHeader(testname,R_AM2_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_COM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,R_COM_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_LSM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}  
	else
	{
	  CheckHeader(testname,R_LSM_HEADER,llx,lly,finished);
	}



    

	sprintf(testname,"%s%sAUXFILES%s%s_SRC.shp",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}  
	sprintf(testname,"%s%sAUXFILES%s%s_SRC.prj",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}  
	sprintf(testname,"%s%sAUXFILES%s%s_SRC.shx",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}  
	sprintf(testname,"%s%sAUXFILES%s%s_SRC.dbf",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}  

	sprintf(testname,"%s%sRESOURCES%sTRExDEMMetadata.xsd",thefolder,GetSlash(),GetSlash());
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sRESOURCES%sTRExDEMMetadata.xsl",thefolder,GetSlash(),GetSlash());
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}




	sprintf(testname,"%s%sPREVIEW%s%s_DEM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_ABS_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_ABS_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_REL_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_REL_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_HEM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_HEM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_WAM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_WAM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_COV_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_COV_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_AMP_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_AMP_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_AM2_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_AM2_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_COM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_COM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_LSM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_LSM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_RLM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_RLM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}

    

  }
  if(rawfinished==2)
  {
	
	

	finished = 1;

    CheckHeader(filename,FIN_HEADER,llx,lly,finished);

	sprintf(testname,"%s%sAUXFILES%s%s_HEM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,HEM_HEADER,llx,lly,finished);
	}

	sprintf(testname,"%s%sAUXFILES%s%s_AMP.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,AMP_HEADER,llx,lly,finished);
	}


	sprintf(testname,"%s%sAUXFILES%s%s_EDM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,EDM_HEADER,llx,lly,finished);
	}

	sprintf(testname,"%s%sAUXFILES%s%s_SDM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,SDM_HEADER,llx,lly,finished);
	}

    sprintf(testname,"%s%sAUXFILES%s%s_WBM.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	else
	{
	  CheckHeader(testname,WBM_HEADER,llx,lly,finished);
	}

	sprintf(testname,"%s%s%s.xml",thefolder,GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}

	sprintf(testname,"%s%sRESOURCES%sTRExDEMMetadata.xsd",thefolder,GetSlash(),GetSlash());
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sRESOURCES%sTRExDEMMetadata.xsl",thefolder,GetSlash(),GetSlash());
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}


	sprintf(testname,"%s%sPREVIEW%s%s_DEM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_ABS_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_ABS_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_REL_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_DEM_SHADED_REL_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_AMP_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_AMP_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_EDM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_EDM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_SDM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_SDM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_HEM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_HEM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_WBM_QL.tif",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
	sprintf(testname,"%s%sPREVIEW%s%s_WBM_QL.jpg",thefolder,GetSlash(),GetSlash(),thename);
	if(FileExists(testname)==0)
	{
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Found:            %s\n",filename);
      fprintf(badtrex,"IMPORT_SENTINEL   But did not find: %s\n",testname);
	}
  }
}




extern "C" int CreateTIFF_SurfaceDifference(char *Finished, char *Reference, char *OutputFileName,
											char *isfilename, int files_done)
{
extern FILE *badtrex;
char *TempC;
int tc,tf,i,j,iceil,ifloor,jceil,jfloor,SrcDTkey, RefDTkey, tempi;
GDALDatasetH hDstDS,hSrcDS,hRefDS;
const char* pszDstFilename = NULL;
const char* pszProjectionRef;
char** papszMetadata;
GDALRasterBandH hSrcBand, hRefBand,hDstBand;
GDALDataType  s_eDataType,r_eDataType;
GDALDriverH hDriver;
double SrcGeoTransform[6];
double RefGeoTransform[6];
double SrcXspace, SrcYspace;
double RefXspace, RefYspace;
double SrcBaseX, SrcBaseY, RefBaseX, RefBaseY;
double Xfactor,Yfactor, multy, multx;
double X[4], Y[4], Z[4];
double Xcoord, Ycoord, Zvalue;
double t2, t1, u2, u1,temp1, temp2;
double num_valid, mean_error, variance, std_dev, RMSE, LE90;
double NoDataDefault = -32767.0;
double SrcNoDataValue;
double RefNoDataValue;
int NoDataValSet;
int sBands, sXSize, sYSize, rBands, rXSize, rYSize;
const char* pszFormat = "GTiff";
char str_temp1[100], str_temp2[100];
char SrcDtype[100];
char RefDtype[100];
char *C_SourceDB = NULL;
char *C_SourceDB2 = NULL;
char *C_ReferenceDB = NULL;
short int *SI_SourceDB = NULL;
short int *SI_SourceDB2 = NULL;
short int *SI_ReferenceDB = NULL;
unsigned short int *USI_SourceDB = NULL;
unsigned short int *USI_SourceDB2 = NULL;
unsigned short int *USI_ReferenceDB = NULL;
int SrcXvalues,SrcYvalues,RefXvalues,RefYvalues;
int *I_SourceDB = NULL;
int *I_SourceDB2 = NULL;
int *I_ReferenceDB = NULL;
unsigned int *UI_SourceDB = NULL;
unsigned int *UI_SourceDB2 = NULL;
unsigned int *UI_ReferenceDB = NULL;
float *F_SourceDB = NULL;
float *F_SourceDB2 = NULL;
float *F_ReferenceDB = NULL;
float *F_OutputDB = NULL;
double *D_SourceDB = NULL;
double *D_SourceDB2 = NULL;
double *D_ReferenceDB = NULL;



 extern int SH_Linears_Done;
 extern int SH_Areals_Done;
 extern int SH_bad_objects;
 extern int SH_BadCoords;
 extern int SH_Null_Shapes;
 extern int SH_Zero_Shapes;
 extern int SH_Bad_DBF;
 extern unsigned long int gridpointsread;
 extern void update_shape_import(char *allshapes,char *curr_file,int files_done, 
				int dbfdone, int dbftotal,
				int points_done,int linears_done,int areals_done,unsigned long int gptsdone,
				int DONE, int extra_type,int extra1, int extra2, int badstuff,
				int badstuff2, int nullshapes, int zeropart, int baddbf);


   
   hDstDS = NULL;
   GDALAllRegister();

   num_valid = mean_error = variance = 0.0;

   hRefDS = GDALOpen(Reference,GA_ReadOnly);

   if(hRefDS == NULL)
      {
      printf("could not open %s\n",Reference);
      return(0);
      }

   hSrcDS = GDALOpen(Finished,GA_ReadOnly);
   if(hSrcDS==NULL)
      {
      GDALClose(hRefDS);
      printf("could not open %s\n",Finished);
      return(0);
      }
   else
      {
      RefXvalues = GDALGetRasterXSize(hRefDS);
      RefYvalues = GDALGetRasterYSize(hRefDS);     
      SrcXvalues = GDALGetRasterXSize(hSrcDS);
      SrcYvalues = GDALGetRasterYSize(hSrcDS);
      if(RefXvalues > SrcXvalues)
         {
         if(RefYvalues > SrcYvalues)
            {
            GDALClose(hRefDS);
            GDALClose(hSrcDS);
            hSrcDS = GDALOpen(Reference,GA_ReadOnly);
            hRefDS = GDALOpen(Finished,GA_ReadOnly);
            TempC = (char *) (malloc(1000));
            strcpy(TempC,Finished);
            strcpy(Finished,Reference);
            strcpy(Reference,TempC);
            free(TempC);
            tempi = SrcXvalues;
            SrcXvalues = RefXvalues;
            RefXvalues = tempi;
            tempi = SrcYvalues;
            SrcYvalues = RefYvalues;
            RefYvalues = tempi;
            }
         else
            {
            printf("Finished Number of X values %d Number of Y values %d\n",SrcXvalues,SrcYvalues);
            printf("Reference Number of X values %d Number of Y values %d\n",RefXvalues,RefYvalues);
            GDALClose(hRefDS);
            GDALClose(hSrcDS);
            printf("could not make difference surface, %s and %s have unanticipated overlap in values \n",
                Reference, Finished);
            return(0);
            }
         }

      strcpy(str_temp1,GDALGetMetadataItem(hRefDS,"AREA_OR_POINT",NULL));
      strcpy(str_temp2,GDALGetMetadataItem(hSrcDS,"AREA_OR_POINT",NULL));
      if(strcmp(str_temp1,str_temp2) != 0)
         {
         
         
         
         
         printf("Calculating difference surface: %s and %s have different Area or Point specifications\n",
             Reference, Finished);
         
         }

      if( GDALGetGeoTransform( hSrcDS, SrcGeoTransform ) == CE_None )
         {
         if( GDALGetGeoTransform( hRefDS, RefGeoTransform ) == CE_None )
            {
            if(strcmp(str_temp1,str_temp2) != 0)
               {
               if(strcmp(str_temp1,"Point") == 0)
                  {
                  RefBaseX = RefGeoTransform[0];
                  RefBaseY = RefGeoTransform[3];
                  }
               else
                  {
                  RefBaseX = RefGeoTransform[0] + (RefGeoTransform[1] / 2.0);
                  RefBaseY = RefGeoTransform[3] + (RefGeoTransform[5] / 2.0);
                  }
               if(strcmp(str_temp2,"Point") == 0)
                  {
                  SrcBaseX = SrcGeoTransform[0];
                  SrcBaseY = SrcGeoTransform[3];
                  }
               else
                  {
                  SrcBaseX = SrcGeoTransform[0] + (SrcGeoTransform[1] / 2.0);
                  SrcBaseY = SrcGeoTransform[3] + (SrcGeoTransform[5] / 2.0);
                  }
               }
            else
               {
               RefBaseX = RefGeoTransform[0];
               RefBaseY = RefGeoTransform[3];
               SrcBaseX = SrcGeoTransform[0];
               SrcBaseY = SrcGeoTransform[3];
               }

            temp1 = (double) SrcXvalues * SrcGeoTransform[1];
            temp2 = (double) (RefXvalues - 1) * RefGeoTransform[1];
            if(temp1 < 0)
               temp1 = 0 - temp1;
            if(temp2 < 0)
               temp2 = 0 - temp2;
            if(temp2 > temp1)
               {
               GDALClose(hRefDS);
               GDALClose(hSrcDS);
               printf("could not make difference surface, %s and %s have unanticipated overlap in X extent\n",
                   Reference, Finished);
               return(0);
               }
            temp1 = (double) SrcYvalues * SrcGeoTransform[5];
            temp2 = (double) (RefYvalues - 1) * RefGeoTransform[5];
            if(temp1 < 0)
               temp1 = 0 - temp1;
            if(temp2 < 0)
               temp2 = 0 - temp2;
            if(temp2 > temp1)
               {
               GDALClose(hRefDS);
               GDALClose(hSrcDS);
               printf("could not make difference surface, %s and %s have unanticipated overlap in Y extent\n",
                   Reference, Finished);
               return(0);
               }

            temp1 = RefBaseX - SrcBaseX;
            if(temp1 < 0)
               temp1 = 0 - temp1;
            temp2 = RefBaseY - SrcBaseY;
            if(temp2 < 0)
               temp2 = 0 - temp2;

            if((temp1 > 0.000001) || (temp2 > 0.000001))
               {
               printf("First File Base Coordinates %.10lf %.10lf\n",SrcGeoTransform[0],SrcGeoTransform[3]);
               printf("Second File Base Coordinates %.10lf %.10lf\n",RefGeoTransform[0],RefGeoTransform[3]);
               GDALClose(hRefDS);
               GDALClose(hSrcDS);
               printf("could not make difference surface, %s and %s have different base coordinates\n",
                   Reference, Finished);
               return(0);
               }
            }
         else
            {
            GDALClose(hRefDS);
            GDALClose(hSrcDS);
            printf("could not make difference surface, bad geo transform in %s\n",Reference);
            return(0);
            }
         }
      else
         {
         GDALClose(hRefDS);
         GDALClose(hSrcDS);
         printf("could not make difference surface, bad geo transform in %s\n",Finished);
         return(0);
         }
      

      pszDstFilename = OutputFileName;

      sBands = GDALGetRasterCount(hSrcDS);
      
      sXSize = GDALGetRasterXSize(hSrcDS);
      sYSize = GDALGetRasterYSize(hSrcDS);
      hSrcBand = GDALGetRasterBand(hSrcDS, 1);
      s_eDataType = GDALGetRasterDataType(hSrcBand);
      SrcNoDataValue = GDALGetRasterNoDataValue(hSrcBand, &NoDataValSet);
      if (!NoDataValSet)
         SrcNoDataValue = NoDataDefault;


      rBands = GDALGetRasterCount(hRefDS);
      
      rXSize = GDALGetRasterXSize(hRefDS);
      rYSize = GDALGetRasterYSize(hRefDS);
      hRefBand = GDALGetRasterBand(hRefDS, 1);
      r_eDataType = GDALGetRasterDataType(hRefBand);
      RefNoDataValue = GDALGetRasterNoDataValue(hRefBand, &NoDataValSet);
      if (!NoDataValSet)
         RefNoDataValue = NoDataDefault;

      hDriver = GDALGetDriverByName(pszFormat);
      if (hDriver == NULL)
         {
         fprintf(stderr, "Can't find driver %s\n", pszFormat);
         exit(1);
         }


      hDstDS = GDALCreate(hDriver, pszDstFilename, rXSize, rYSize, rBands,GDT_Float32, NULL);
      
      F_OutputDB = (float *) (malloc(sizeof(float) * sXSize));

      if (hDstDS == NULL)
         {
         fprintf(stderr, "Can't create %s\n", pszDstFilename);
         exit(1);
         }






      if( GDALGetGeoTransform( hSrcDS, SrcGeoTransform ) == CE_None )
         {
         
         if(SrcGeoTransform[1] < 0)
            SrcXspace = 0 - SrcGeoTransform[1];
         else
            SrcXspace = SrcGeoTransform[1];
         if(SrcGeoTransform[5] < 0)
            SrcYspace = 0 - SrcGeoTransform[5];
         else
            SrcYspace = SrcGeoTransform[5];

         }
     else
         {
         printf("unexpected geo transform in fininshed raster\n");
         printf("surface comparison raster cannot be created\n");
         if(hDstDS != NULL)
            GDALClose(hDstDS);
         GDALClose(hSrcDS);
         GDALClose(hRefDS);
         return(0);
         }
      if( GDALGetGeoTransform( hRefDS, RefGeoTransform ) == CE_None )
         {
         GDALSetGeoTransform( hDstDS, RefGeoTransform );
         if(RefGeoTransform[1] < 0)
            RefXspace = 0 - RefGeoTransform[1];
         else
            RefXspace = RefGeoTransform[1];
         if(RefGeoTransform[5] < 0)
            RefYspace = 0 - RefGeoTransform[5];
         else
            RefYspace = RefGeoTransform[5];

         Xfactor = RefXspace / SrcXspace;
         Yfactor = RefYspace / SrcYspace;

         }
     else
         {
         printf("unexpected geo transform in raster raster\n");
         printf("surface comparison raster cannot be created\n");
         if(hDstDS != NULL)
            GDALClose(hDstDS);
         GDALClose(hSrcDS);
         GDALClose(hRefDS);
         return(0);
         }

      
      pszProjectionRef = GDALGetProjectionRef( hRefDS );
      if (pszProjectionRef != NULL)
         {
         GDALSetProjection( hDstDS, pszProjectionRef );
         }

      
      papszMetadata = GDALGetMetadata(hRefDS, NULL);
      GDALSetMetadata(hDstDS, papszMetadata, NULL);

      for(i = 0; i < sBands; i++)
         {
         GDALRasterBandH hSrcBand, hRefBand, hDstBand;
         GDALColorTableH hColorTable;
         GDALColorInterp eColorInterpretation;

         hSrcBand = GDALGetRasterBand(hSrcDS, i+1);
         hRefBand = GDALGetRasterBand(hRefDS, i+1);
         hDstBand = GDALGetRasterBand(hDstDS, i+1);
         

         sprintf(SrcDtype,"%s",GDALGetDataTypeName(GDALGetRasterDataType(hSrcBand)));
         if(strcmp(SrcDtype, "Byte") == 0)
            {
            SrcDTkey = 0;
            C_SourceDB = (char *) (malloc(sXSize));
            if(1) 
               {
               C_SourceDB2 = (char *) (malloc(sXSize));
               }
            }
         else if(strcmp(SrcDtype,"Int16") == 0)
            {
            SrcDTkey = 1;
            SI_SourceDB = (short int *) (malloc(sizeof(short int)*sXSize));
            if(1) 
               {
               SI_SourceDB2 = (short int *) (malloc(sizeof(short int) * sXSize));
               }
            }
         else if(strcmp(SrcDtype,"UInt16") == 0)
            {
            SrcDTkey = 2;
            USI_SourceDB = (unsigned short int *) (malloc(sizeof(unsigned short int)*sXSize));
            if(1) 
               {
               USI_SourceDB2 = (unsigned short int *) (malloc(sizeof(unsigned short int) * sXSize));
               }
            }
         else if(strcmp(SrcDtype,"Int32") == 0)
            {
            SrcDTkey = 3;
            I_SourceDB = (int *) (malloc(sizeof(int)*sXSize));
            if(1) 
               {
               I_SourceDB2 = (int *) (malloc(sizeof(int) * sXSize));
               }
            }
         else if(strcmp(SrcDtype,"Uint32") == 0)
            {
            SrcDTkey = 4;
            UI_SourceDB = (unsigned int *) (malloc(sizeof(unsigned int)*sXSize));
            if(1) 
               {
               UI_SourceDB2 = (unsigned int *) (malloc(sizeof(unsigned int) * sXSize));
               }
            }
         else if(strcmp(SrcDtype,"Float32") == 0)
            {
            SrcDTkey = 5;
            F_SourceDB = (float *) (malloc(sizeof(float)*sXSize));
            if(1) 
               {
               F_SourceDB2 = (float *) (malloc(sizeof(float) * sXSize));
               }
            }
         else if(strcmp(SrcDtype,"Float64") == 0)
            {
            SrcDTkey = 6;
            D_SourceDB = (double *) (malloc(sizeof(double)*sXSize));
            if(1) 
               {
               D_SourceDB2 = (double *) (malloc(sizeof(double) * sXSize));
               }
            }
         else
            {
            printf("unexpected data type for finished raster (%s)\n",SrcDtype);
            printf("Surface comparison raster cannot be generated\n");
            if(hDstDS != NULL)
               GDALClose(hDstDS);
            GDALClose(hSrcDS);
            GDALClose(hRefDS);
            return(0);
            }

         sprintf(RefDtype,"%s",GDALGetDataTypeName(GDALGetRasterDataType(hRefBand)));
         if(strcmp(RefDtype, "Byte") == 0)
            {
            RefDTkey = 0;
            C_ReferenceDB = (char *) (malloc(rXSize));
            }
         else if(strcmp(RefDtype,"Int16") == 0)
            {
            RefDTkey = 1;
            SI_ReferenceDB = (short int *) (malloc(sizeof(short int)*rXSize));
            }
         else if(strcmp(RefDtype,"UInt16") == 0)
            {
            RefDTkey = 2;
            USI_ReferenceDB = (unsigned short int *) (malloc(sizeof(unsigned short int)*rXSize));
            }
         else if(strcmp(RefDtype,"Int32") == 0)
            {
            RefDTkey = 3;
            I_ReferenceDB = (int *) (malloc(sizeof(int)*rXSize));
            }
         else if(strcmp(RefDtype,"Uint32") == 0)
            {
            RefDTkey = 4;
            UI_ReferenceDB = (unsigned int *) (malloc(sizeof(unsigned int)*rXSize));
            }
         else if(strcmp(RefDtype,"Float32") == 0)
            {
            RefDTkey = 5;
            F_ReferenceDB = (float *) (malloc(sizeof(float)*rXSize));
            }
         else if(strcmp(RefDtype,"Float64") == 0)
            {
            RefDTkey = 6;
            D_ReferenceDB = (double *) (malloc(sizeof(double)*rXSize));
            }
         else
            {
            printf("unexpected data type for reference raster (%s)\n",RefDtype);
            printf("Surface comparison raster cannot be generated\n");
            if(hDstDS != NULL)
               GDALClose(hDstDS);
            GDALClose(hSrcDS);
            GDALClose(hRefDS);
            return(0);
            }
         hColorTable = GDALGetRasterColorTable( hSrcBand );
         if (hColorTable)
            {
            GDALSetRasterColorTable(hDstBand, hColorTable);
            }

         papszMetadata = GDALGetMetadata(hSrcBand, NULL);
         GDALSetMetadata(hDstBand, papszMetadata, NULL);

         eColorInterpretation = GDALGetRasterColorInterpretation( hSrcBand );
         GDALSetRasterColorInterpretation( hDstBand, eColorInterpretation );

         GDALSetRasterNoDataValue(hDstBand, RefNoDataValue);
         }


      hDstBand = GDALGetRasterBand(hDstDS,1);
      for(i=0; i<rYSize; i++)
         {

		  if(i%10==0)
		  {
	        update_shape_import(isfilename,OutputFileName,files_done,0,0,
			      SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
			      0,0,0,0,SH_bad_objects,SH_BadCoords,SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);
		  }



         switch(RefDTkey)
            {
            case 0:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,C_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            case 1:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,SI_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            case 2:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,USI_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            case 3:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,I_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            case 4:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,UI_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            case 5:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,F_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            case 6:
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,D_ReferenceDB,rXSize,1,r_eDataType,0,0);
               break;
            }
         multy = ((double) i) * Yfactor;
         iceil = (int) ceil(multy);
         tc = sYSize - iceil - 1;
         if(tc < 0) tc = 0;

         ifloor = (int) floor(multy);
         tf = sYSize - ifloor - 1;
         if(tf < 0)  tf = 0;

         switch(SrcDTkey)
            {
            case 0:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,C_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,C_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            case 1:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,SI_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,SI_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            case 2:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,USI_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,USI_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            case 3:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,I_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,I_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            case 4:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,UI_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,UI_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            case 5:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,F_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,F_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            case 6:
               GDALRasterIO(hSrcBand,GF_Read,0,tc,sXSize,1,D_SourceDB,sXSize,1,s_eDataType,0,0);
               GDALRasterIO(hSrcBand,GF_Read,0,tf,sXSize,1,D_SourceDB2,sXSize,1,s_eDataType,0,0);
               break;
            }

         Y[1] = ((double) ifloor) * SrcYspace;
         Y[2] = Y[1];
         Y[0] = Y[1] + SrcYspace;
         Y[3] = Y[0];

         Ycoord = i * RefYspace;

         for(j=0; j<rXSize; j++)
            {
            multx = ((double) j) * Xfactor;
            jceil = (int) ceil(multx);
            jfloor = (int) floor(multx);

            X[0] = ((double) jfloor) * SrcXspace;
            X[1] = X[0];
            X[2] = X[0] + SrcXspace;
            X[3] = X[2];
            Xcoord = j * RefXspace;
            switch(SrcDTkey)
               {
               case 0:
                  Z[0] = (double) C_SourceDB[jfloor];
                  Z[3] = (double) C_SourceDB[jceil];
                  Z[1] = (double) C_SourceDB2[jfloor];
                  Z[2] = (double) C_SourceDB2[jceil];
                  break;
               case 1:
                  Z[0] = (double) SI_SourceDB[jfloor];
                  Z[3] = (double) SI_SourceDB[jceil];
                  Z[1] = (double) SI_SourceDB2[jfloor];
                  Z[2] = (double) SI_SourceDB2[jceil];
                  break;
               case 2:
                  Z[0] = (double) USI_SourceDB[jfloor];
                  Z[3] = (double) USI_SourceDB[jceil];
                  Z[1] = (double) USI_SourceDB2[jfloor];
                  Z[2] = (double) USI_SourceDB2[jceil];
                  break;
               case 3:
                  Z[0] = (double) I_SourceDB[jfloor];
                  Z[3] = (double) I_SourceDB[jceil];
                  Z[1] = (double) I_SourceDB2[jfloor];
                  Z[2] = (double) I_SourceDB2[jceil];
                  break;
               case 4:
                  Z[0] = (double) UI_SourceDB[jfloor];
                  Z[3] = (double) UI_SourceDB[jceil];
                  Z[1] = (double) UI_SourceDB2[jfloor];
                  Z[2] = (double) UI_SourceDB2[jceil];
                  break;
               case 5:
                  Z[0] = (double) F_SourceDB[jfloor];
                  Z[3] = (double) F_SourceDB[jceil];
                  Z[1] = (double) F_SourceDB2[jfloor];
                  Z[2] = (double) F_SourceDB2[jceil];
                  break;
               case 6:
                  Z[0] = (double) D_SourceDB[jfloor];
                  Z[3] = (double) D_SourceDB[jceil];
                  Z[1] = (double) D_SourceDB2[jfloor];
                  Z[2] = (double) D_SourceDB2[jceil];
                  break;
               }

            t1 = (X[3] - Xcoord) / (X[3] - X[0]);
            t2 = (Xcoord - X[0]) / (X[3] - X[0]);
            temp1 = t1 * Z[0] + t2 * Z[3];
            temp2 = t1 * Z[1] + t2 * Z[2];
            u1 = (Y[0] - Ycoord) / (Y[0] - Y[1]);
            u2 = (Ycoord - Y[1]) / (Y[0] - Y[1]);

            Zvalue = u1 * temp2 + u2 * temp1;




            switch(RefDTkey)
               {
               case 0:
                  temp1 = (float) C_ReferenceDB[j];
                  break;
               case 1:
                  temp1 = (float) SI_ReferenceDB[j];
                  break;
               case 2:
                  temp1 = (float) USI_ReferenceDB[j];
                  break;
               case 3:
                  temp1 = (float) I_ReferenceDB[j];
                  break;
               case 4:
                  temp1 = (float) UI_ReferenceDB[j];
                  break;
               case 5:
                  temp1 = F_ReferenceDB[j];
                  break;
               case 6:
                  temp1 = (float) D_ReferenceDB[j];
                  break;
               }

            if((Z[0] == SrcNoDataValue) || (Z[1] == SrcNoDataValue) || (Z[2] == SrcNoDataValue) ||
                   (Z[3] == SrcNoDataValue) || (temp1 == RefNoDataValue))
               F_OutputDB[j] = (float) RefNoDataValue;
            else
               {
               F_OutputDB[j] = (float) (temp1 - Zvalue);
               num_valid = num_valid + 1.0;
               if(temp1 >= Zvalue)
                  mean_error = mean_error + F_OutputDB[j];
               else
                  mean_error = mean_error - F_OutputDB[j];
               }
            }

         GDALRasterIO(hDstBand,GF_Write,0,rYSize-i-1,rXSize,1,F_OutputDB,rXSize,1,GDT_Float32,0,0);


         }

      }

   if(hDstDS != NULL)
      GDALClose(hDstDS);
   GDALClose(hSrcDS);
   GDALClose(hRefDS);

   fprintf(badtrex,"IMPORT_SENTINEL Created surface difference file:\n");
   fprintf(badtrex,"IMPORT_SENTINEL   %s\n",OutputFileName);
   fprintf(badtrex,"IMPORT_SENTINEL   from sources:\n");
   fprintf(badtrex,"IMPORT_SENTINEL    %s\n",Finished);
   fprintf(badtrex,"IMPORT_SENTINEL    %s\n",Reference);



   mean_error = mean_error / num_valid;



   if(C_SourceDB != NULL)
      free(C_SourceDB);
   if(SI_SourceDB != NULL)
      free(SI_SourceDB);
   if(USI_SourceDB != NULL)
      free(USI_SourceDB);
   if(I_SourceDB != NULL)
      free(I_SourceDB);
   if(UI_SourceDB != NULL)
      free(UI_SourceDB);
   if(F_SourceDB != NULL)
      free(F_SourceDB);
   if(D_SourceDB != NULL)
      free(D_SourceDB);
   

   if(C_SourceDB2 != NULL)
      free(C_SourceDB2);
   if(SI_SourceDB2 != NULL)
      free(SI_SourceDB2);
   if(USI_SourceDB2 != NULL)
      free(USI_SourceDB2);
   if(I_SourceDB2 != NULL)
      free(I_SourceDB2);
   if(UI_SourceDB2 != NULL)
      free(UI_SourceDB2);
   if(F_SourceDB2 != NULL)
      free(F_SourceDB2);
   if(D_SourceDB2 != NULL)
      free(D_SourceDB2);

   if(C_ReferenceDB != NULL)
      free(C_ReferenceDB);
   if(SI_ReferenceDB != NULL)
      free(SI_ReferenceDB);
   if(USI_ReferenceDB != NULL)
      free(USI_ReferenceDB);
   if(I_ReferenceDB != NULL)
      free(I_ReferenceDB);
   if(UI_ReferenceDB != NULL)
      free(UI_ReferenceDB);
   if(F_ReferenceDB != NULL)
      free(F_ReferenceDB);
   if(D_ReferenceDB != NULL)
      free(D_ReferenceDB);

   if(num_valid > 0) 
      {
      hRefDS = GDALOpen(OutputFileName,GA_ReadOnly);

      if(hRefDS == NULL)
         {
         printf("could not open %s\n",Finished);
         return(0);
         }
      else
         {
         RefXvalues = GDALGetRasterXSize(hRefDS);
         rXSize = RefXvalues;
         RefYvalues = GDALGetRasterYSize(hRefDS);
         rYSize = RefYvalues;
         rBands = GDALGetRasterCount(hRefDS);
         hRefBand = GDALGetRasterBand(hRefDS, 1);
         r_eDataType = GDALGetRasterDataType(hRefBand);
         RefNoDataValue = GDALGetRasterNoDataValue(hRefBand, &NoDataValSet);
         if (!NoDataValSet)
            RefNoDataValue = NoDataDefault;

         pszProjectionRef = GDALGetProjectionRef( hRefDS );
         papszMetadata = GDALGetMetadata(hRefDS, NULL);

         for(i = 0; i < rBands; i++)
            {
            GDALRasterBandH hRefBand;
   
            hRefBand = GDALGetRasterBand(hRefDS, i+1);
            sprintf(RefDtype,"%s",GDALGetDataTypeName(GDALGetRasterDataType(hRefBand)));
            RefDTkey = 5;

            num_valid = 0;
            RMSE = variance = 0;
            t1 = mean_error * 100;
            t2 = 0;
            for(i=0; i<rYSize; i++)
               {
               GDALRasterIO(hRefBand,GF_Read,0,rYSize-i-1,rXSize,1,F_OutputDB,rXSize,1,r_eDataType,0,0);

               for(j=0; j < rXSize; j++)
                  {
                  if(F_OutputDB[j] != (float) RefNoDataValue)
                     {
                     num_valid = num_valid + 1.0;
                     temp1 = (double) F_OutputDB[j] - mean_error;
                     RMSE = RMSE + ((double) F_OutputDB[j] * (double) F_OutputDB[j]);

                     variance = variance + (temp1 * temp1);
                     }
                  }
               }
            if(num_valid > 1)
               {
               variance = variance / (num_valid - 1);
               std_dev = sqrt(variance);
               temp1 = mean_error / std_dev;
               if(temp1  > 1.4)
                  temp2 = 1.28;
               else
                  temp2 = 1.6435 - (0.999556 * temp1) + (0.923237 * (temp1 * temp1)) - (0.282533 * (temp1 * temp1 * temp1));

               LE90 = mean_error + (temp2 * std_dev);
               }
            else
               {
               std_dev = 0;
               LE90 = 0;
               }
            }

         GDALClose(hRefDS);
         RMSE = sqrt(RMSE / num_valid);
         fprintf(badtrex,"IMPORT_SENTINEL    mean elevation error %.5lf     standard deviation %.5lf\n",mean_error,std_dev);
         fprintf(badtrex,"IMPORT_SENTINEL    RMSE %.5lf                     LE90 %.5lf\n",RMSE,LE90);


         }
      }


   if(F_OutputDB != NULL)
      free(F_OutputDB);
   return(1);
}






extern "C" void CheckTrexXML(char *filename,char *shortname,int type, double northing, double easting, char *destination)
{
  int i,len,numerrs=0,lastslash=0;
  char newname[1000],rootname[1000],newresultname[1000];
  FILE *outerrfile;
  extern int SLASHTYPE;
  extern char TrexXMLresultstub[];
  extern int ParseXML3(char *filename, char *outfilename, int *errors, char *rootname, int finished_raw, double northing,double easting);
  extern char *GetTRExSubName2(char *thepath, int id);

  

  len = strlen(filename);

  

  for(i=len-5;i>=0;i--)
  {
    if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
	{
	  if(filename[i]=='\\')	
	  {
		lastslash = i;
		break;
	  }
	}
	else
	{
	  if(filename[i]=='/')	
	  {
		lastslash = i;
		break;
	  }
	}
  }






  if(!strcmp(shortname,"finished"))
  {
	
    sprintf(newresultname,"%s%s_%s_results.txt",TrexXMLresultstub,shortname,GetTRExSubName2(destination,1));
  }
  else
  {
	
    sprintf(newresultname,"%s%s_%s_results.txt",TrexXMLresultstub,shortname,GetTRExSubName2(destination,0));
  }

  if(lastslash<=0)
  {
    outerrfile = fopen(newresultname,"w");
    if(outerrfile==NULL)
	{ 
	  printf("Error: unable to open %s for write\n",newresultname);
	  return ;
	} 

	fprintf(outerrfile,"Error: could not parse XML path from: %s\n  Cannot check %s TREx XML.",filename,shortname);

	fclose(outerrfile);

	return;
  }
  else
  {
    sprintf(newname,"%s",filename);
	sprintf(&newname[i-4],"%s",&filename[i]);
	sprintf(&newname[i+11],".xml");

	sprintf(rootname,"%s",&newname[i-3]);
	rootname[14] = '\0';
  }


  

  printf("Check XML %s...save results to %s\n",newname,newresultname);



  if(type==1)
  {
    ParseXML3(newname,newresultname,&numerrs, rootname,1,northing,easting);
  }
  else if(type==2)
  {
    ParseXML3(newname,newresultname,&numerrs, rootname,0,northing,easting);
  }

}



int CloseSpacing(double a, double b)
{
  

  if(fabs(a-b)<.000000001)
  {
    return 1;
  }
  return 0;
}


extern "C" char *GetTrexSpacings(double lly,double uly,double *xspacing_target,int *xnum_target, int finished)
{

  if(finished==0)
  {
    

    if((lly<50.01)&&(uly<50.01)&&(lly>-50.01)&&(uly>-50.01))
	{ 
     
     *xspacing_target = 0.4;
	 *xnum_target     = 9001;
 	 return (char *)"0.4";
	} 
    else if((lly<60.01)&&(uly<60.01)&&(lly>-60.01)&&(uly>-60.01))
	{ 
     
     *xspacing_target = 0.6;
	 *xnum_target     = 6001;
	 return (char *)"0.6";
	}
    else if((lly<70.01)&&(uly<70.01)&&(lly>-70.01)&&(uly>-70.01))
	{ 
     
     *xspacing_target = 0.8;
	 *xnum_target     = 9001; 
	 return (char *)"0.8";
	}
    else if((lly<80.01)&&(uly<80.01)&&(lly>-80.01)&&(uly>-80.01))
	{ 
     
     *xspacing_target = 1.2;
	 *xnum_target     = 6001; 
	 return (char *)"1.2";
	} 
    else if((lly<85.01)&&(uly<85.01)&&(lly>-85.01)&&(uly>-85.01))
	{ 
     
     *xspacing_target = 2.0;
	 *xnum_target     = 7201; 
	 return (char *)"2.0";
	} 
    else
	{ 
     
     *xspacing_target = 4.0;
	 *xnum_target     = 3601; 
	 return (char *)"4.0";
	} 
  }



  
  
  
  
  if((lly<50.01)&&(uly<50.01)&&(lly>-50.01)&&(uly>-50.01))
  {
    
    *xspacing_target = 0.4;
	*xnum_target     = 9001;
	return (char *)"0.4";
  }
  else if((lly<60.01)&&(uly<60.01)&&(lly>-60.01)&&(uly>-60.01))
  {
    
    *xspacing_target = 0.6;
	*xnum_target     = 6001;
	return (char *)"0.6";
  }
  else if((lly<70.01)&&(uly<70.01)&&(lly>-70.01)&&(uly>-70.01))
  {
    
    *xspacing_target = 0.8;
	*xnum_target     = 4501;
	return (char *)"0.8";
  }
  else if((lly<80.01)&&(uly<80.01)&&(lly>-80.01)&&(uly>-80.01))
  {
    
    *xspacing_target = 1.2;
	*xnum_target     = 3001;
	return (char *)"1.2";
  }
  else if((lly<85.01)&&(uly<85.01)&&(lly>-85.01)&&(uly>-85.01))
  {
    
    *xspacing_target = 2.0;
	*xnum_target     = 1801;
	return (char *)"2.0";
  }
  else
  {
    
    *xspacing_target = 4.0;
	*xnum_target     = 901;
	return (char *)"4.0";
  }

  return (char *)"Err";
}




extern "C" void CheckCoords(char *filename, char *lod_override,
                 int numx,int numy,                  
				 double xspacing,double yspacing,    
				 double llx, double lly, double uly, 
				 int finished)
{
  extern FILE *badtrex;
  int xnum_target;
  double yspacing_target = 0.4/3600.0;
  double xspacing_target;


  


  if(numy!=9001)
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Should have Latitude Pixels 9001, but has %d\n",numy);
  }

  if(CloseSpacing(yspacing,yspacing_target)==0)
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Should have Latitude Spacing %.10lf, but has %.10lf\n",yspacing_target,yspacing);
  }


  GetTrexSpacings(lly,uly,&xspacing_target,&xnum_target,finished);
  xspacing_target = xspacing_target / 3600.0;




  if(numx!=xnum_target)
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Should have Longitude Pixels %d, but has %d\n",xnum_target,numx);
  }

  if(CloseSpacing(xspacing,xspacing_target)==0)
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Should have Longitude Spacing %.10lf, but has %.10lf\n",xspacing_target,xspacing);
  }

}





extern "C" int CheckTREXtype(char *filename,char *lod_override,char *Dtype,int addtolong,int addtoshort,
                             int numx,int numy,                   
						     double xspacing,double yspacing,     
						     double llx, double lly, double uly,  
							 char *destination, 
							 char *allshapes,int files_done)
{
  int retval=0;
  extern FILE *badtrex;
  extern int CREATING_TREX,foundTREXraw,foundTREXfinished,TREX_TYPE;

  if(CREATING_TREX==0)
  {
    return 0;
  }

  

  if(!strcmp(lod_override,"TREx DEM Raw"))
  {

	

    

  	if(TREX_TYPE==3)
	{
	  
      fprintf(badtrex,"IMPORT_SENTINEL Warning:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Using TREx analysis type \"sQA\" on package containing \"TDR\" folder\n");
	}


	foundTREXraw = 1;
	retval = 7;
    CheckTrexXML(filename,(char *)"raw",2,lly,llx,destination);
    CheckRequiredFiles(filename,1,addtolong,addtoshort,llx,lly,allshapes,files_done);
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,0);

  
  }
  if(!strcmp(lod_override,"TREx DEM Finished"))
  {

	

    

	foundTREXfinished = 1;
	retval = 1;
    CheckTrexXML(filename,(char *)"finished",1,lly,llx,destination);
    CheckRequiredFiles(filename,2,addtolong,addtoshort,llx,lly,allshapes,files_done);
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,1);

  }


  if(!strcmp(lod_override,"Height Error Map"))
  {
    retval = 5;
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,1);
  }
  if(!strcmp(lod_override,"Source Data Mask"))
  {
    retval = 3;
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,1);
  }
  if(!strcmp(lod_override,"Edit Data Mask"))
  {
    retval = 2;
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,1);
  }
  if(!strcmp(lod_override,"Water Body Mask"))
  {
    retval = 4;
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,1);
  }
  if(!strcmp(lod_override,"Reliability Mask"))
  {
    retval = 6;
	CheckCoords(filename,lod_override,numx,numy,xspacing,yspacing,llx,lly,uly,0);
  }



  if( (!strcmp(lod_override,"Height Error Map"))  && (strcmp(Dtype,"Float32"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Height Error Map should be of type \"Float32\" but is \"%s\"\n",Dtype);
  }

  if( (!strcmp(lod_override,"Reliability Mask"))  && (strcmp(Dtype,"Byte"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Reliability Mask should be of type \"Byte\" but is \"%s\"\n",Dtype);
  }

  if( (!strcmp(lod_override,"Source Data Mask"))  && (strcmp(Dtype,"Byte"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Source Data Mask should be of type \"Byte\" but is \"%s\"\n",Dtype);
  }
  if( (!strcmp(lod_override,"Edit Data Mask"))  && (strcmp(Dtype,"Byte"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Edit Data Mask should be of type \"Byte\" but is \"%s\"\n",Dtype);
  }
  if( (!strcmp(lod_override,"Water Body Mask"))  && (strcmp(Dtype,"Byte"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   Water Body Mask should be of type \"Byte\" but is \"%s\"\n",Dtype);
  }
  if( (!strcmp(lod_override,"TREx DEM Raw"))  && (strcmp(Dtype,"Float32"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   TREx DEM Raw should be of type \"Float32\" but is \"%s\"\n",Dtype);
  }
  if( (!strcmp(lod_override,"TREx DEM Finished"))  && (strcmp(Dtype,"Float32"))  )
  {
    fprintf(badtrex,"IMPORT_SENTINEL Error:\nIMPORT_SENTINEL  File %s\n",filename);
    fprintf(badtrex,"IMPORT_SENTINEL   TREx DEM Finished should be of type \"Float32\" but is \"%s\"\n",Dtype);
  }

  return retval;
}




extern "C" int ReadData(GDALDatasetH hDataset, int band, char *allshapes,char *filename, int files_done,char *lod_override,int addtolong,int addtoshort,
                        int numx,int numy,                  
						double xspacing,double yspacing,    
						double llx, double lly, double uly, 
						char *destination) 
{

  GDALRasterBandH hBand;
  char Dtype[100];
  int             nBlockXSize, nBlockYSize,type,sizer;
  int             bGotMin, bGotMax;
  int   nXSize;
  int i,j,isvoid=0,nonvoid=0,trex_type=0,rlm128=0,signedbyte=0;
  double         adfMinMax[2],tgtval;

  unsigned char *pafScanline1;
  char *pafScanline1a;
  char **tempresults;
  short int *pafScanline2;
  unsigned short int *pafScanline3;
  int *pafScanline4;
  unsigned int *pafScanline5;
  float *pafScanline6;
  double *pafScanline9;

  extern long int TestNumGridNodes;
  extern int SH_Points_Done;
  extern int SH_Linears_Done;
  extern int SH_Areals_Done;
  extern int SH_bad_objects;
  extern int SH_BadCoords;
  extern int SH_Null_Shapes;
  extern int SH_Zero_Shapes;
  extern int SH_Bad_DBF;
  extern int ABORT_EXTRACT;
  extern unsigned long int gridpointsread;
  extern void sh_testxyglobalZONLY(double z);
  extern void update_shape_import(char *allshapes,char *curr_file,int files_done, 
				int dbfdone, int dbftotal,
				int points_done,int linears_done,int areals_done,unsigned long int gptsdone,
				int DONE, int extra_type,int extra1, int extra2, int badstuff,
				int badstuff2, int nullshapes, int zeropart, int baddbf);
  extern FILE *badtrex;
  extern void TestMinMax(int type, double value);


  hBand = GDALGetRasterBand(hDataset,band);

  nXSize = GDALGetRasterBandXSize(hBand);

  GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );

  

  adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
  adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
  if( ! (bGotMin && bGotMax) )
      GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );

  sprintf(Dtype,"%s",GDALGetDataTypeName(GDALGetRasterDataType(hBand)));

  if(!strcmp(Dtype,"Byte"))
  {
    

	tempresults = GDALGetMetadata(hBand,"IMAGE_STRUCTURE");

	if(tempresults!=NULL)
	{
	  i = 0;
	  while(tempresults[i]!=NULL)
	  {
		if(!strcmp(tempresults[i],"PIXELTYPE=SIGNEDBYTE"))
		{
		  printf("found this layer is of type SIGNEDBYTE\n");
		  signedbyte = 1;
		  break;
		}
	    i = i + 1;

		if(i>100) 
		{
		  printf("breaking after very large number of image structure metadata items\n");
		  break;
		}
	  }
	} 
  }


  
  trex_type = CheckTREXtype(filename,lod_override,Dtype,addtolong,addtoshort,
	                        numx,numy,          
						    xspacing,yspacing,  
						    llx,lly,uly,destination,
							allshapes,files_done);

  if((trex_type==1)||(trex_type==5))
  {
    tgtval = -32767.0;
  }
  if((trex_type==3)||(trex_type==4))
  {
    tgtval = 0.0;
  }
  if(trex_type==2)
  {
    tgtval = 0.0; 
  }


  
        


  

  



  if(!strcmp(Dtype,"Byte"))
  {


	

	

	if(signedbyte==1)
	{
      type = 7;
	  sizer = sizeof(char);
      pafScanline1a = (char *) malloc(nXSize);
	}
	else
	{
      type = 1;
	  sizer = sizeof(char);
      pafScanline1  = (unsigned char *) malloc(nXSize);
	}
  }
  else if(!strcmp(Dtype,"Int16"))
  {
    type = 2;
	sizer = sizeof(short int);
    pafScanline2 = (short int *) malloc(sizeof(short int)*nXSize);
  }
  else if(!strcmp(Dtype,"UInt16"))
  {
    type = 3;
	sizer = sizeof(unsigned short int);
    pafScanline3 = (unsigned short int *) malloc(sizeof(short int)*nXSize);
  }
  else if(!strcmp(Dtype,"Int32"))
  {
    type = 4;
	sizer = sizeof(int);
    pafScanline4 = (int *) malloc(sizeof(int)*nXSize);
  }
  else if(!strcmp(Dtype,"UInt32"))
  {
    type = 5;
	sizer = sizeof(unsigned int);
    pafScanline5 = (unsigned int *) malloc(sizeof(unsigned int)*nXSize);
  }
  else if(!strcmp(Dtype,"Float32"))
  {
    type = 6;
	sizer = sizeof(float);
    pafScanline6 = (float *) malloc(sizeof(float)*nXSize);
  }
  else
  {
    type = 9;
	sizer = sizeof(double);
    pafScanline9 = (double *) malloc(sizeof(double)*nXSize);
  }

  

  sizer = type;

  fwrite(&sizer,sizeof(int),1,gridbin);  





  for(i=0;i<USE_NUMY;i++)
  {

	if(type==7)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline1a,nXSize,1,GDT_Byte,0,0);

	  fwrite(&pafScanline1a[0],1,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX; 
      for(j=0;j<USE_NUMX;j++)
	  {  
		if(trex_type>0)
		{ 
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline1a[j]);
		  }

          if(CloseSpacing((double)pafScanline1a[j],tgtval))
		  { 
		    isvoid = isvoid + 1;
		  } 
		  else
		  { 
		    nonvoid = nonvoid + 1;
		  } 

		  if(trex_type==6)
		  { 
            if(CloseSpacing((double)pafScanline1a[j],128))
			{ 
			  rlm128 = rlm128 + 1;
			} 
		  } 
		} 
        sh_testxyglobalZONLY(pafScanline1a[j]); 
	  }
	}
	else if(type==1)
	{ 
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline1,nXSize,1,GDT_Byte,0,0);

	  fwrite(&pafScanline1[0],1,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX; 
      for(j=0;j<USE_NUMX;j++)
	  {  
		if(trex_type>0)
		{ 
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline1[j]);
		  }

          if(CloseSpacing((double)pafScanline1[j],tgtval))
		  { 
		    isvoid = isvoid + 1;
		  }
		  else
		  { 
		    nonvoid = nonvoid + 1;
		  } 

		  if(trex_type==6)
		  { 
            if(CloseSpacing((double)pafScanline1[j],128))
			{ 
			  rlm128 = rlm128 + 1;
			} 
		  } 
		} 
        sh_testxyglobalZONLY(pafScanline1[j]);
	  } 
	}
	else if(type==2)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline2,nXSize,1,GDT_Int16,0,0);

	  fwrite(&pafScanline2[0],2,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX;
      for(j=0;j<USE_NUMX;j++)
	  { 
		if(trex_type>0)
		{
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline2[j]);
		  }

          if(CloseSpacing((double)pafScanline2[j],tgtval))
		  {
			 isvoid = isvoid + 1;
		  }
		  else
		  {
			 nonvoid = nonvoid + 1;
		  }
		}

        sh_testxyglobalZONLY(pafScanline2[j]);
	  } 
	}

	else if(type==3)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline3,nXSize,1,GDT_UInt16,0,0);

	  fwrite(&pafScanline3[0],2,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX;
      for(j=0;j<USE_NUMX;j++)
	  { 
		if(trex_type>0)
		{
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline3[j]);
		  }

          if(CloseSpacing((double)pafScanline3[j],tgtval))
		  {
			 isvoid = isvoid + 1;
		  }
		  else
		  {
			 nonvoid = nonvoid + 1;
		  }
		}

        sh_testxyglobalZONLY(pafScanline3[j]);
	  } 
	}

	else if(type==4)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline4,nXSize,1,GDT_Int32,0,0);

	  fwrite(&pafScanline4[0],4,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX*2; 
      for(j=0;j<USE_NUMX;j++)
	  { 
		if(trex_type>0)
		{
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline4[j]);
		  }

          if(CloseSpacing((double)pafScanline4[j],tgtval))
		  {
			 isvoid = isvoid + 1;
		  }
		  else
		  {
			 nonvoid = nonvoid + 1;
		  }
		}

        sh_testxyglobalZONLY(pafScanline4[j]);
	  } 
	}

	else if(type==5)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline5,nXSize,1,GDT_UInt32,0,0);

	  fwrite(&pafScanline5[0],4,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX*2; 
      for(j=0;j<USE_NUMX;j++)
	  { 
		if(trex_type>0)
		{
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline5[j]);
		  }

          if(CloseSpacing((double)pafScanline5[j],tgtval))
		  {
			 isvoid = isvoid + 1;
		  }
		  else
		  {
			 nonvoid = nonvoid + 1;
		  }
		}

        sh_testxyglobalZONLY(pafScanline5[j]);
	  } 
	}
	else if(type==6)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline6,nXSize,1,GDT_Float32,0,0);

	  fwrite(&pafScanline6[0],4,USE_NUMX,gridbin);

      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX*2; 
      for(j=0;j<USE_NUMX;j++)
	  { 
		if(trex_type>0)
		{
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,(double)pafScanline6[j]);
		  }

          if(CloseSpacing(pafScanline6[j],tgtval))
		  {
			 isvoid = isvoid + 1;
		  }
		  else
		  {
			 nonvoid = nonvoid + 1;
		  }
		}

        sh_testxyglobalZONLY(pafScanline6[j]);
	  } 
	}
	else if(type==9)
	{
      GDALRasterIO(hBand,GF_Read,0,USE_NUMY-i-1,nXSize,1,pafScanline9,nXSize,1,GDT_Float64,0,0);

	  fwrite(&pafScanline9[0],8,USE_NUMX,gridbin);
      gridpointsread = gridpointsread + USE_NUMX;
      TestNumGridNodes = TestNumGridNodes + USE_NUMX*4; 
      for(j=0;j<USE_NUMX;j++)
	  { 
		if(trex_type>0)
		{
		  if((trex_type==1)||(trex_type==7))
		  {
		    TestMinMax(trex_type,pafScanline9[j]);
		  }

          if(CloseSpacing(pafScanline9[j],tgtval))
		  {
			 isvoid = isvoid + 1;
		  }
		  else
		  {
			 nonvoid = nonvoid + 1;
		  }
		}

        sh_testxyglobalZONLY(pafScanline9[j]);
	  } 
	}



    if(i%10==0)
	{
	  update_shape_import(allshapes,filename, files_done,0,0,
			      SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
			      0,0,0,0,SH_bad_objects,SH_BadCoords,SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);

	  if(ABORT_EXTRACT==1)
	  { 
        if(type==1)
		{ 
          free(pafScanline1);
		} 
        else if(type==2)
		{ 
          free(pafScanline2);
		} 
        else if(type==3)
		{ 
          free(pafScanline3);
		} 
        else if(type==4)
		{ 
          free(pafScanline4);
		} 
        else if(type==5)
		{ 
          free(pafScanline5);
		} 
        else if(type==6)
		{  
          free(pafScanline6);
		} 
        else if(type==9)
		{ 
          free(pafScanline9);
		} 
        return 0;
	  } 
	}
  }


  


  if(trex_type==1) 
  {
    if((isvoid+nonvoid)>0)
	{
      fprintf(badtrex,"VOID_SENTINEL File %s:\n",filename);
      fprintf(badtrex,"VOID_SENTINEL   Found %d/%d posts with value -32767 (void) (%lf%%)\n",isvoid,isvoid+nonvoid,100.0 * (double)isvoid/(double)(isvoid+nonvoid));
	}
  }
  if(trex_type==2) 
  {
    if((isvoid>0)&&(nonvoid==0))
	{
      fprintf(badtrex,"VOID_SENTINEL File %s:\n",filename);
      fprintf(badtrex,"VOID_SENTINEL   Found all posts had value %d\n",(int)tgtval);
	}
  }
  if(trex_type==3) 
  {
    if((isvoid>0)&&(nonvoid==0))
	{
      fprintf(badtrex,"VOID_SENTINEL File %s:\n",filename);
      fprintf(badtrex,"VOID_SENTINEL   Found all posts had value %d\n",(int)tgtval);
	}
  }
  if(trex_type==4) 
  {
    if((isvoid>0)&&(nonvoid==0))
	{
      fprintf(badtrex,"VOID_SENTINEL File %s:\n",filename);
      fprintf(badtrex,"VOID_SENTINEL   Found all posts had value %d\n",(int)tgtval);
	}
  }
  if(trex_type==5) 
  {
    if((isvoid>0)&&(nonvoid==0))
	{
      fprintf(badtrex,"VOID_SENTINEL File %s:\n",filename);
      fprintf(badtrex,"VOID_SENTINEL   Found all posts had value %d\n",(int)tgtval);
	}
  }
  if(trex_type==6) 
  {
    if(rlm128>0)
	{
	  fprintf(badtrex,"IMPORT_SENTINEL Error: Reliability Mask: %s\n",filename);
	  fprintf(badtrex,"IMPORT_SENTINEL   %d posts with invalid value 128\n",rlm128);
 	}
  }


  if(type==1)
  {
    free(pafScanline1);
  }
  else if(type==2)
  {
    free(pafScanline2);
  }
  else if(type==3)
  {
    free(pafScanline3);
  }
  else if(type==4)
  {
    free(pafScanline4);
  }
  else if(type==5)
  {
    free(pafScanline5);
  }
  else if(type==6)
  {
    free(pafScanline6);
  }
  else if(type==9)
  {
    free(pafScanline9);
  }

  return 1;
}


void BlankGTVars()
{
  USE_NUMX      =  -1;
  USE_NUMY      =  -1;
  USE_NUMZ      =  -1;
  USE_COORDTYPE =  -1;
  USE_FACTOR    = -9999999;
  USE_MERIDIAN  = -9999999;
  USE_ZONE      = -9999999;
  USE_ZONE2     = -9999999;
  USE_ULX       = -9999999;
  USE_ULY       = -9999999;
  USE_PIXELX    = -9999999;
  USE_PIXELY    = -9999999;
  USE_UNITS[0]  = '\0';
}





extern const char *GetDEMType(int type)
{
  if(type==1)
  {
    return "GEOTIFF";
  }
  else if(type==2)
  {
    return "NITF";
  }
  else
  {
    printf("bad type %d to GDT\n",type);
	exit(-1);
  }
}


extern "C" int AddGeoTiffFile(FILE *infile,char *GeoTiffFile, int type, char *annotation)
{
  GDALDatasetH hDataset;
  char thistype[100];

  

  sprintf(thistype,"%s",GetDEMType(type));

  GDALAllRegister();

  BlankGTVars();

  ZONE_IN_USE = -999;

  hDataset = GDALOpen(GeoTiffFile,GA_ReadOnly);
  if(hDataset==NULL)
  {
    printf("Error: could not open %s\n",GeoTiffFile);
    sprintf(ErrStr,"Error: could not open %s\n",GeoTiffFile);
    return 0;
  }
  else
  {
    if(SummarizeDataset(GeoTiffFile,hDataset)==0)
	{
      ZONE_IN_USE = -999;
      GDALClose(hDataset);
      return 0;
	}
 
    ZONE_IN_USE = -999;

    if(USE_COORDTYPE==1)
	{
      
	}
	else
	{
	  printf("Coordinate type: TM/UTM\n");
	  printf("Meridian: %lf Zone: %lf (%d)\n",USE_MERIDIAN,USE_ZONE,USE_ZONE2);
	}

	
  }

  GDALClose(hDataset);

  if(USE_COORDTYPE==1)
  {
    if(  CloseSpacing(USE_PIXELX,USE_PIXELY) ||  CloseSpacing(USE_PIXELX,(USE_PIXELY*-1.0)) )
	{
	  if(annotation==NULL)
	  {
        fprintf(infile,"%s \"%dx%d Geodetic %s: horizontal spacing %lf degrees\" \"%s\"\n",
	    thistype,USE_NUMX,USE_NUMY,thistype,USE_PIXELX,GeoTiffFile);
	  }
	  else
	  {
        fprintf(infile,"%s \"%s\" \"%s\"\n",
	      thistype,annotation,GeoTiffFile);
	  }
	}
	else
	{
	  if(annotation==NULL)
	  {
        

        fprintf(infile,"%s \"%dx%d Geodetic %s: horizontal spacing %lf x %lf degrees\" \"%s\"\n",
	      thistype,USE_NUMX,USE_NUMY,thistype,USE_PIXELX,USE_PIXELY,GeoTiffFile);
	  }
	  else
	  {
        

        fprintf(infile,"%s \"%s\" \"%s\"\n",
	      thistype,annotation,GeoTiffFile);
	  }
	}
  }
  else
  {
    if(  CloseSpacing(USE_PIXELX,USE_PIXELY) ||  CloseSpacing(USE_PIXELX,(USE_PIXELY*-1.0)) )
	{
      if(annotation==NULL)
	  {
        fprintf(infile,"%s \"%dx%d TM/UTM %s: horizontal spacing %lf meters\" \"%s\"\n",
	      thistype,USE_NUMX,USE_NUMY,thistype,USE_PIXELX,GeoTiffFile);
	  }
	  else
	  {
        fprintf(infile,"%s \"%s\" \"%s\"\n",
	      thistype,annotation,GeoTiffFile);
	  }
	}
	else
	{
      if(annotation==NULL)
	  {
        printf("%.20lf m not equal to %.20lf m\n",USE_PIXELX,USE_PIXELY);
        fprintf(infile,"%s \"%dx%d TM/UTM %s: horizontal spacing %lf x %lf meters\" \"%s\"\n",
	      thistype,USE_NUMX,USE_NUMY,thistype,USE_PIXELX,USE_PIXELY,GeoTiffFile);
	  }
	  else
	  {
        printf("%.20lf m not equal to %.20lf m\n",USE_PIXELX,USE_PIXELY);
        fprintf(infile,"%s \"%s\" \"%s\"\n",
	      thistype,annotation,GeoTiffFile);
	  }
	}
  }

  return 1;
}


double GetGSMod()
{
  if(COORDINATEKEEPER==2)
  {
    return 1.0;
  }
  return 100000.0;
}


int InSide(int x, int y, double xstart, double yend, double xend, double ystart)
{

   

  double thisx1,thisx2,thisy1,thisy2;


  thisx1 = (xstart + 180.0);
  thisx2 = (xend   + 180.0);

  thisy1 = (ystart + 90.0);
  thisy2 = (yend   + 90.0);


  if(
	  (thisx1<=(x+1))&&(thisx2>=x) &&
	  (thisy1<=(y+1))&&(thisy2>=y)
	 )
  {
    return 1;
  }

  return 0;
}







#include "tiffio.h"
#include "geotiffio.h"
#include "xtiffio.h"

#define tim_char      0
#define tim_uint16    1
#define tim_uint32    2
#define tim_float32   3
#define tim_float64   4
#define tim_3_doubles 5
#define tim_6_doubles 6



#define TIFFTAG_GDAL_METADATA  42112
#define TIFFTAG_GDAL_NODATA    42113
#define TIFFTAG_RPCCOEFFICIENT 50844








static TIFFExtendProc _ParentExtender = NULL;

static void GTiffTagExtender(TIFF *tif)
{
    static const TIFFFieldInfo xtiffFieldInfo[] = {
        { TIFFTAG_GDAL_METADATA,    -1,-1, TIFF_ASCII,	FIELD_CUSTOM,
          TRUE,	FALSE,	(char*) "GDALMetadata" },
        { TIFFTAG_GDAL_NODATA,	    -1,-1, TIFF_ASCII,	FIELD_CUSTOM,
          TRUE,	FALSE,	(char*) "GDALNoDataValue" },
        { TIFFTAG_RPCCOEFFICIENT,   -1,-1, TIFF_DOUBLE,	FIELD_CUSTOM,
          TRUE,	TRUE,	(char*) "RPCCoefficient" }
    };

    if (_ParentExtender) 
        (*_ParentExtender)(tif);

    TIFFMergeFieldInfo( tif, xtiffFieldInfo,
		        sizeof(xtiffFieldInfo) / sizeof(xtiffFieldInfo[0]) );
}




void InstallGDALTiffTags()
{
    _ParentExtender = TIFFSetTagExtender(GTiffTagExtender);
}


int ISFINISHED(int type)
{

  if(
	  (type==FIN_HEADER) ||
      (type==HEM_HEADER) ||
      (type==SDM_HEADER) ||
      (type==EDM_HEADER) ||
      (type==WBM_HEADER) ||
      (type==AMP_HEADER)
	)
  {
    return 1;
  }

  return 0;
}


extern "C" void RecordTagError(char *thestr)
{
  extern FILE *badtrexheaders;

  fprintf(badtrexheaders," Error: %s\n",thestr);
}



extern "C" int ValidateTIFFTag(int type, TIFF *tif,char *chartag,uint32 thetag,int tagtype, char *filename,
					int expected_int, double expected_double, char *expected_char,
					double d1,double d2,double d3,double d4,double d5,double d6,
					double *retval,char *retchar)
{
  int count=0;
  uint16  dummyuint16;
  uint32  dummyuint32;
  float dummyfloat;
  double dummydouble;
  double *doubles_3;
  double *doubles_6;
  char *data=NULL;
  char errstr[2000];
  extern int IsNumber(int num);
  extern int fin_header_date_found,raw_header_date_found;
  extern char fin_header_date[100],raw_header_date[100];

uint16 keycount;
uint16 *list_of_uint16=NULL;
uint32 *list_of_uint32=NULL;



  

  *retval = fakeval;
  sprintf(retchar,"%s",fakecharval);

  if(tagtype==tim_uint16)
  {

	if(expected_int==-12345)
	{
      

       if(TIFFGetField(tif,thetag,&keycount,&list_of_uint16)==0)
	   {
	     sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	     RecordTagError(errstr);
	     return 1;
	   }
	   return 0;
	}

    if(TIFFGetField(tif,thetag,&dummyuint16)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}


	if(expected_int==-1234)
	{
	  if((dummyuint16!=1)&&(dummyuint16!=5))
	  {
        sprintf(errstr,"  TIFF Tag %s (%d): %d...expected 1 or 5\n",chartag,thetag,dummyuint16);
	    RecordTagError(errstr);
	    return 1;
	  }
	}
	else if(expected_int==777)
	{
	  if((dummyuint16!=1)&&(dummyuint16!=2)&&(dummyuint16!=3))
	  {
        sprintf(errstr,"  TIFF Tag %s (%d): %d...expected 1, 3, or 3\n",chartag,thetag,dummyuint16);
	    RecordTagError(errstr);
	    return 1;
	  }
	}
	else
	{
	  if(dummyuint16!=expected_int)
	  {
        sprintf(errstr,"  TIFF Tag %s (%d): %d...expected %d\n",chartag,thetag,dummyuint16,expected_int);
	    RecordTagError(errstr);
	    return 1;
	  }
	}
  }
  else if(tagtype==tim_uint32)
  {

	if(expected_int==-12345)
	{
      

       if(TIFFGetField(tif,thetag,&keycount,&list_of_uint32)==0)
	   {
	     sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	     RecordTagError(errstr);
	     return 1;
	   }

	   
	   return 0;
	}



    if(TIFFGetField(tif,thetag,&dummyuint32)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

	if(expected_int==-1234)
	{
	  if((dummyuint32!=1)&&(dummyuint32!=5))
	  {
        sprintf(errstr,"  TIFF Tag %s (%d): %d...expected 1 or 5\n",chartag,thetag,dummyuint32);
	    RecordTagError(errstr);
	    return 1;
	  }
	}
	else if(expected_int==777)
	{
	  if((dummyuint32!=1)&&(dummyuint32!=2)&&(dummyuint32!=3))
	  {
        sprintf(errstr,"  TIFF Tag %s (%d): %d...expected 1, 3, or 3\n",chartag,thetag,dummyuint32);
	    RecordTagError(errstr);
	    return 1;
	  }
	}
	else
	{
	  if(dummyuint32!=(unsigned int)expected_int)
	  {
        sprintf(errstr,"  TIFF Tag %s (%d): %d...expected %d\n",chartag,thetag,dummyuint32,expected_int);
	    RecordTagError(errstr);
	    return 1;
	  }
	}
  }
  else if(tagtype==tim_float32)
  {
    if(TIFFGetField(tif,thetag,&dummyfloat)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

    if(!RealClose(dummyfloat,expected_double))
	{
        sprintf(errstr,"  TIFF Tag %s (%d): %f...expected %lf\n",chartag,thetag,dummyfloat,expected_double);
	    RecordTagError(errstr);
	    return 1;
	}
  }
  else if(tagtype==tim_float64)
  {
    if(TIFFGetField(tif,thetag,&dummydouble)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}
	if(RealClose(fakeval,expected_double))
	{
	  
	  *retval = dummydouble;
	  return 0;
	}
    else if(!RealClose(dummydouble,expected_double))
	{
        sprintf(errstr,"  TIFF Tag %s (%d): %lf...expected %lf\n",chartag,thetag,dummydouble,expected_double);
	    RecordTagError(errstr);
	    return 1;
	}
  }
  else if(tagtype==tim_char)
  {

    if(TIFFGetField(tif,thetag,&data)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

	if(data==NULL)
	{
	  sprintf(errstr,"  Read NULL value for TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

	if(strcmp(expected_char,"NULL"))
	{
	  if(!strcmp(expected_char,"YYYYMMDD1234"))
	  {
		
        

		if(strlen(data)!=19)
		{
		  sprintf(errstr,"  TIFF Tag %s (%d):\n  Expected: length 19 (\"YYYY:MM:DD HH:MM:SS\")\n  Found:    %s (length %d)\n",chartag,thetag,data,(int)strlen(data));
	      RecordTagError(errstr);
	      return 1;
		}


		if(
			(IsNumber(data[0])==0)  ||
			(IsNumber(data[1])==0)  ||
			(IsNumber(data[2])==0)  ||
			(IsNumber(data[3])==0)  ||
			(data[4]!=':')          ||
			(IsNumber(data[5])==0)  ||
			(IsNumber(data[6])==0)  ||
			(data[7]!=':')          ||
			(IsNumber(data[8])==0)  ||
			(IsNumber(data[9])==0)  ||
			(data[10]!=' ')         ||
			(IsNumber(data[11])==0) ||
			(IsNumber(data[12])==0) ||
			(data[13]!=':')         ||
			(IsNumber(data[14])==0) ||
			(IsNumber(data[15])==0) ||
			(data[16]!=':')         ||
			(IsNumber(data[17])==0) ||
			(IsNumber(data[18])==0)
			)
		{
          sprintf(errstr,"  TIFF Tag %s (%d):\n  Expected: format \"YYYY:MM:DD HH:MM:SS\"\n  Found:    %s\n",chartag,thetag,data);
	      RecordTagError(errstr);
	      return 1;
		}


        if(type==FIN_HEADER)
		{
		  fin_header_date_found = 1;
		  sprintf(fin_header_date,"%s",data);
		}
        if(type==RAW_HEADER)
		{
		  raw_header_date_found = 1;
		  sprintf(raw_header_date,"%s",data);
		}

	  }
      else if(strcmp(data,expected_char))
	  { 
		if(thetag==TIFFTAG_COPYRIGHT)
		{
		  if(ISFINISHED(type))
		  {
            sprintf(errstr,"  TIFF Tag %s (%d):\n  Expected: %s (Note: Expected year is taken from Finished XML 65.1.1)\n  Found:    %s\n",chartag,thetag,expected_char,data);
		  }
		  else
		  {
            sprintf(errstr,"  TIFF Tag %s (%d):\n  Expected: %s (Note: Expected year is taken from Raw XML 65.1.1)\n  Found:    %s\n",chartag,thetag,expected_char,data);
		  }
		}
		else
		{
          sprintf(errstr,"  TIFF Tag %s (%d):\n  Expected: %s\n  Found:    %s\n",chartag,thetag,expected_char,data);
		}

	    RecordTagError(errstr);
	    return 1;
	  } 
	}
	else
	{
	  
	  return 0;
	}
  }
  else if(tagtype==tim_3_doubles)
  {
    if(TIFFGetField(tif,thetag,&count,&doubles_3)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

    if(count==3)
	{ 
	  if(
		  (!RealClose(doubles_3[0],d1)) ||
		  (!RealClose(doubles_3[1],d2)) ||
		  (!RealClose(doubles_3[2],d3))
        )		
	  {
        sprintf(errstr,"  TIFF Tag %s (%d):\n\
          Found:       %16.12lf %16.12lf %16.12lf\n\
          Expected:    %16.12lf %16.12lf %16.12lf\n\
          Differences: %16.12lf %16.12lf %16.12lf",
			chartag,thetag,
			doubles_3[0],doubles_3[1],doubles_3[2],
			d1,d2,d3,
			fabs(d1-doubles_3[0]),fabs(d2-doubles_3[1]),fabs(d3-doubles_3[2]));
	    RecordTagError(errstr);
	    return 1;
	  }
	} 
	else
	{
	  sprintf(errstr,"  TIFF Tag %s (%d): expected 3 values, found %d\n",chartag,thetag,count);
	  RecordTagError(errstr);
	  return 1;
	}
  }
  else if(tagtype==tim_6_doubles)
  {
    if(TIFFGetField(tif,thetag,&count,&doubles_6)==0)
	{
	  sprintf(errstr,"  Did not find TIFF Tag %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

    if(count==6)
	{ 
	  if(
		  (!RealClose(doubles_6[0],d1)) ||
		  (!RealClose(doubles_6[1],d2)) ||
		  (!RealClose(doubles_6[2],d3)) ||
		  (!RealClose(doubles_6[3],d4)) ||
		  (!RealClose(doubles_6[4],d5)) ||
		  (!RealClose(doubles_6[5],d6))
        )		
	  {

        sprintf(errstr,"  TIFF Tag %s (%d):\n\
          Found:       %16.12lf %16.12lf %16.12lf %16.12lf %16.12lf %16.12lf\n\
          Expected:    %16.12lf %16.12lf %16.12lf %16.12lf %16.12lf %16.12lf\n\
          Differences: %16.12lf %16.12lf %16.12lf %16.12lf %16.12lf %16.12lf",
			chartag,thetag,
			doubles_6[0],doubles_6[1],doubles_6[2],doubles_6[3],doubles_6[4],doubles_6[5],
			d1,d2,d3,d4,d5,d6,
			fabs(d1-doubles_6[0]),fabs(d2-doubles_6[1]),fabs(d3-doubles_6[2]),fabs(d4-doubles_6[3]),fabs(d5-doubles_6[4]),fabs(d6-doubles_6[5]));
	    RecordTagError(errstr);
	    return 1;
	  }
	} 
	else
	{
	  sprintf(errstr,"  TIFF Tag %s (%d): expected 6 values, found %d\n",chartag,thetag,count);
	  RecordTagError(errstr);
	  return 1;
	}
  }
  else
  {
    sprintf(errstr,"Internal error - illegal value %d to VTT\n",tagtype);
    RecordTagError(errstr);
	return 1;
  }

  return 0;
}





char *GetTimType(int type)
{
  static char retval[50];

  if(type==tim_char)    return (char *)"ASCII";
  if(type==tim_uint16)  return (char *)"Short Integer";
  if(type==tim_uint32)  return (char *)"Integer";
  if(type==tim_float32) return (char *)"Float";
  if(type==tim_float64) return (char *)"Double";

  sprintf(retval,"t-unrecognized (%d)",type);

  return retval;
}



int ValidateGeoKey(GTIF *gtif,char *chartag,geokey_t thetag,int tagtype, char *filename, char *expectedchar, int expectedint)
{
  int thislen;
  char thestr[5010];
  tagtype_t type;
  short int theshort;
  int size;
  int LEN_LIMIT=5000; 
  char errstr[2000];

  

  thislen = GTIFKeyInfo(gtif,thetag,&size,&type);

  if(thislen<=0)
  {
	sprintf(errstr,"  Did not find GeoKey %s (%d)\n",chartag,thetag);
	RecordTagError(errstr);
	return 1;
  }



  if(type==TYPE_ASCII)
  {
	if(tagtype!=tim_char)
	{
	  sprintf(errstr,"  GeoKey %s (%d): Expected type %s, found ASCII\n",chartag,thetag,GetTimType(tagtype));
	  RecordTagError(errstr);
	  return 1;
	}


	if(thislen<LEN_LIMIT)
	{ 
	  if(GTIFKeyGet(gtif,thetag,thestr,0,thislen)==0)
	  {
	    sprintf(errstr,"  Did not find GeoKey %s (%d)\n",chartag,thetag);
	    RecordTagError(errstr);
	    return 1;
	  }

	  if(strcmp(expectedchar,thestr))
	  {
	    sprintf(errstr,"  GeoKey %s (%d):\n  Expected: %s\n  Found   : %s\n",chartag,thetag,expectedchar,thestr);
	    RecordTagError(errstr);
	    return 1;
	  }

	}
	else
	{  
	  sprintf(errstr,"  GeoKey %s (%d) with length %d...skipping\n",chartag,thetag,thislen);
	  RecordTagError(errstr);
	  return 1;
	}
  }

  else if(type==TYPE_SHORT)
  {
	if(tagtype!=tim_uint16)
	{
	  sprintf(errstr,"  GeoKey %s (%d): Expected type %s, found Short Integer\n",chartag,thetag,GetTimType(tagtype));
	  RecordTagError(errstr);
	  return 1;
	}


    if(GTIFKeyGet(gtif,thetag,&theshort,0,1)==0)
	{
	  sprintf(errstr,"  Did not find GeoKey %s (%d)\n",chartag,thetag);
	  RecordTagError(errstr);
	  return 1;
	}

	if(theshort!=expectedint)
	{
	  sprintf(errstr,"  GeoKey %s (%d):\n  Expected: %d\n  Found   : %d\n",chartag,thetag,expectedint,theshort);
	  RecordTagError(errstr);
	  return 1;
	}

  }

  else
  {
    sprintf(errstr,"Internal GTG error: bad type %d\n",type);
	RecordTagError(errstr);
	return 1;
  }


  return 0;
}




extern "C" void TestGT123(char *filename, double llx, double lly, int type,int finished)
{
  int expected_width,datasize,isdem,numerrs,founderr;
  double xspacing,Djunk;
  char junk[100],cjunk[500],nodatavalue[100],checkstring[300];
  extern int raw_copyright,finished_copyright;
  extern double SMALLAMOUNT,NORMAL_SMALLAMOUNT,TREX_SMALLAMOUNT;
  extern FILE *badtrexheaders;

  extern int min_fin_header_found0,
	         max_fin_header_found0,
			 min_fin_header_found1,
	         max_fin_header_found1,
			 min_fin_header_found2,
	         max_fin_header_found2,
			 min_fin_header_found3,
	         max_fin_header_found3,
			 min_raw_header_found,
			 max_raw_header_found;
  extern double min_fin_header0,
	            max_fin_header0,
				min_fin_header1,
	            max_fin_header1,
				min_fin_header2,
	            max_fin_header2,
				min_fin_header3,
	            max_fin_header3,
				min_raw_header,
				max_raw_header;

  

  if((type==FIN_HEADER)||(type==RAW_HEADER))
  {
    datasize = 32;
	sprintf(nodatavalue,"-32767");
	isdem = 1;
  }
  else if((type==HEM_HEADER)||(type==R_HEM_HEADER))
  {
    datasize = 32;
	sprintf(nodatavalue,"-32767");
	isdem = 0;
  }
  else if((type==AMP_HEADER)||(type==R_AMP_HEADER)||(type==R_AM2_HEADER))
  {
	datasize = 16;
	sprintf(nodatavalue,"0");
	isdem = 0;
  }
  else
  {
	datasize = 8;
	sprintf(nodatavalue,"0");
	isdem = 0;
  }






  TIFF* tif=(TIFF*)0;
  GTIF* gtif=(GTIF*)0;


  
  if(new_tags_innitted==0)
  {
    InstallGDALTiffTags();
	new_tags_innitted = 1;
  }


  
  TIFFErrorHandler oldone = TIFFSetWarningHandler(NULL);
  
  TIFFSetWarningHandler(NULL);


  sprintf(junk,"%s",GetTrexSpacings(lly,lly+1.0,&xspacing,&expected_width,finished));






  tif  = XTIFFOpen(filename,"r");
  if(!tif)
  {
    fprintf(badtrexheaders,"Could not open tif %s\n",filename);
	return;
  }


  fprintf(badtrexheaders,"Errors found in TIFF Tags and GeoKeys for:\n %s:\n\n",filename);


  
  TIFFSetWarningHandler(oldone);

  numerrs = 0;

  SMALLAMOUNT = TREX_SMALLAMOUNT;

  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_IMAGEWIDTH",       TIFFTAG_IMAGEWIDTH,       tim_uint32,   filename,expected_width,0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_IMAGELENGTH",      TIFFTAG_IMAGELENGTH,      tim_uint32,   filename,9001,          0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_BITSPERSAMPLE",    TIFFTAG_BITSPERSAMPLE,    tim_uint16,   filename,datasize,      0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_COMPRESSION",      TIFFTAG_COMPRESSION,      tim_uint16,   filename,-1234,         0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);  
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_PHOTOMETRIC",      TIFFTAG_PHOTOMETRIC,      tim_uint16,   filename,1,             0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);

  if(isdem==1)
  {
    numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_IMAGEDESCRIPTION", TIFFTAG_IMAGEDESCRIPTION, tim_char,     filename,0,             0.0,(char *)"Digital Surface Model, UNCLASSIFIED / LIMITED DISTRIBUTION",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  }
  else
  {
    numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_IMAGEDESCRIPTION", TIFFTAG_IMAGEDESCRIPTION, tim_char,     filename,0,             0.0,(char *)"TREx DEM Auxiliary File, UNCLASSIFIED / LIMITED DISTRIBUTION",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  }



  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_STRIPOFFSETS",     TIFFTAG_STRIPOFFSETS,     tim_uint32,   filename,-12345,        0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk); 
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_SAMPLESPERPIXEL",  TIFFTAG_SAMPLESPERPIXEL,  tim_uint16,   filename,1,             0.0,(char *)(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_ROWSPERSTRIP",     TIFFTAG_ROWSPERSTRIP,     tim_uint32,   filename,1,             0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_STRIPBYTECOUNTS",  TIFFTAG_STRIPBYTECOUNTS,  tim_uint32,   filename,-12345,        0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_XRESOLUTION",      TIFFTAG_XRESOLUTION,      tim_float32,  filename,0,1.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_YRESOLUTION",      TIFFTAG_YRESOLUTION,      tim_float32,  filename,0,1.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_PLANARCONFIG",     TIFFTAG_PLANARCONFIG,     tim_uint16,   filename,1,             0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_RESOLUTIONUNIT",   TIFFTAG_RESOLUTIONUNIT,   tim_uint16,   filename,2,             0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_SOFTWARE",         TIFFTAG_SOFTWARE,         tim_char,     filename,0,0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);   
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_DATETIME",         TIFFTAG_DATETIME,         tim_char,     filename,0,0.0,(char *)"YYYYMMDD1234",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);   




  if(ISFINISHED(type))
  {
	numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_ARTIST",          TIFFTAG_ARTIST,         tim_char,     filename,0,0.0,(char *)"This dataset has been created under the TanDEM-X High Resolution Elevation Data Exchange Program.",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);

	if(finished_copyright>0)
	{
	  sprintf(checkstring,"Includes material (c) DLR e.V. %d, Distribution: Airbus Defence & Space / Airbus DS Geo GmbH, Germany. All rights reserved.",finished_copyright);

	  

      numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_COPYRIGHT",       TIFFTAG_COPYRIGHT,      tim_char,     filename,0,0.0,(char *)checkstring,0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
	}
	else
	{
	  sprintf(checkstring,"  Could not check tag COPYRIGHT (%d) because a valid copyright year was not found in the finished XML (65.1.1)\n",TIFFTAG_COPYRIGHT);
	  RecordTagError(checkstring);
      numerrs = numerrs + 1;
	}
  } 
  else
  {
	numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_ARTIST",             TIFFTAG_ARTIST,         tim_char,     filename,0,0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);

	if(raw_copyright>0)
	{
	  sprintf(checkstring,"(C) DLR e.V. %d, Distribution: Airbus DS Geo GmbH. All rights reserved.",raw_copyright);

	  

      numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_COPYRIGHT",       TIFFTAG_COPYRIGHT,      tim_char,     filename,0,0.0,(char *)checkstring,0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
	}
	else
	{
	  sprintf(checkstring,"  Could not check tag COPYRIGHT (%d) because a valid copyright year was not found in the raw XML (65.1.1)\n",TIFFTAG_COPYRIGHT);
	  RecordTagError(checkstring);
      numerrs = numerrs + 1;
	}
  }


  if(isdem==1)
  {
    numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_SAMPLEFORMAT",       TIFFTAG_SAMPLEFORMAT,   tim_uint16,   filename,3,             0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);
  }
  else
  {
    numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_SAMPLEFORMAT",       TIFFTAG_SAMPLEFORMAT,   tim_uint16,   filename,777,             0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);  
  }



  if(isdem==1)
  {

    founderr = ValidateTIFFTag(type,tif,(char *)"TIFFTAG_SMINSAMPLEVALUE",    TIFFTAG_SMINSAMPLEVALUE,tim_float64,  filename,0,fakeval,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);   

	if(founderr==0)
	{
	  if(type==FIN_HEADER)
	  {
		if(global_trex_layer==0)
		{
		  min_fin_header_found0 = 1;
          min_fin_header0 = Djunk;
		}
		if(global_trex_layer==1)
		{
		  min_fin_header_found1 = 1;
          min_fin_header1 = Djunk;
		}
		if(global_trex_layer==2)
		{
		  min_fin_header_found2 = 1;
          min_fin_header2 = Djunk;
		}
		if(global_trex_layer==3)
		{
		  min_fin_header_found3 = 1;
          min_fin_header3 = Djunk;
		}
	  }
	  if(type==RAW_HEADER)
	  {
		min_raw_header_found = 1;
        min_raw_header = Djunk;
	  }
	}
	else
	{
	  numerrs = numerrs + founderr;
	}

    founderr = ValidateTIFFTag(type,tif,(char *)"TIFFTAG_SMAXSAMPLEVALUE",    TIFFTAG_SMAXSAMPLEVALUE,tim_float64,  filename,0,fakeval,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);   

	if(founderr==0)
	{
	  if(type==FIN_HEADER)
	  {
		if(global_trex_layer==0)
		{
		  max_fin_header_found0 = 1;
          max_fin_header0 = Djunk;
		}
		if(global_trex_layer==1)
		{
		  max_fin_header_found1 = 1;
          max_fin_header1 = Djunk;
		}
		if(global_trex_layer==2)
		{
		  max_fin_header_found2 = 1;
          max_fin_header2 = Djunk;
		}
		if(global_trex_layer==3)
		{
		  max_fin_header_found3 = 1;
          max_fin_header3 = Djunk;
		}
	  }
	  if(type==RAW_HEADER)
	  {
		max_raw_header_found = 1;
        max_raw_header = Djunk;
	  }
	}
	else
	{
	  numerrs = numerrs + founderr;
	}

  }


  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"ModelPixelScaleTag",         TIFFTAG_GEOPIXELSCALE,  tim_3_doubles,filename,0,0.0,(char *)"NULL", xspacing/3600.0,   0.4/3600.0, 1.0  ,0.0,0.0,0.0,&Djunk,cjunk);
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_GEOTIEPOINTS",       TIFFTAG_GEOTIEPOINTS,   tim_6_doubles,filename,0,0.0,(char *)"NULL",0.0,0.0,0.0,llx,lly+1.0,0.0,&Djunk,cjunk);  
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_GEOKEYDIRECTORY",    TIFFTAG_GEOKEYDIRECTORY,tim_uint16,   filename,-12345,0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);   
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_GEOASCIIPARAMS",     TIFFTAG_GEOASCIIPARAMS, tim_char,     filename,0,0.0,(char *)"NULL",0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk);   
  numerrs = numerrs + ValidateTIFFTag(type,tif,(char *)"TIFFTAG_GDAL_NODATA",        TIFFTAG_GDAL_NODATA,    tim_char,     filename,0,0.0,nodatavalue,0.0,0.0,0.0,0.0,0.0,0.0,&Djunk,cjunk); 

  SMALLAMOUNT = NORMAL_SMALLAMOUNT;


  





  

  gtif = GTIFNew(tif);
  if(!gtif)
  {
    fprintf(badtrexheaders,"Could not open gtif %s\n",filename);
	return;
  }


  


  if((type==FIN_HEADER)||(type==RAW_HEADER))
  {
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GTModelTypeGeoKey",     GTModelTypeGeoKey,      tim_uint16,filename,(char *)"Q",                                 2); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GTRasterTypeGeoKey",    GTRasterTypeGeoKey,     tim_uint16,filename,(char *)"Q",                                 2); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GeographicTypeGeoKey",  GeographicTypeGeoKey,   tim_uint16,filename,(char *)"Q",                              4326); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GeogCitationGeoKey",    GeogCitationGeoKey,     tim_char  ,filename,(char *)"WGS84",                             0); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"VerticalCSTypeGeoKey",  VerticalCSTypeGeoKey,   tim_uint16,filename,(char *)"Q",                             32767); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"VerticalCitationGeoKey",VerticalCitationGeoKey, tim_char  ,filename,(char *)"EGM2008",                           0); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"VerticalUnitsGeoKey",   VerticalUnitsGeoKey,    tim_uint16,filename,(char *)"Q",                              9001); 
  }
  else
  {
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GTModelTypeGeoKey",     GTModelTypeGeoKey,      tim_uint16,filename,(char *)"Q",                                 2); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GTRasterTypeGeoKey",    GTRasterTypeGeoKey,     tim_uint16,filename,(char *)"Q",                                 2); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GeographicTypeGeoKey",  GeographicTypeGeoKey,   tim_uint16,filename,(char *)"Q",                              4326); 
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GeogCitationGeoKey",    GeogCitationGeoKey,     tim_char  ,filename,(char *)"WGS84",                             0); 
  }



  if(ISFINISHED(type))
  {
	
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GTCitationGeoKey",      GTCitationGeoKey,       tim_char  ,filename,(char *)"TREx DEM Finished Specification v2.1",   0); 
  }
  else
  {
	
    numerrs = numerrs + ValidateGeoKey(gtif,(char *)"GTCitationGeoKey",      GTCitationGeoKey,       tim_char  ,filename,(char *)"TREx Metadata Definition v1.9.4",   0); 
  }







  GTIFFree(gtif);

  XTIFFClose(tif);

  if(numerrs==0)
  {
    fprintf(badtrexheaders," None\n\n");
  }


}




char *PrintHeaderType(int type)
{
  if(type==1)
  {
    return (char *)"Finished DEM";
  }
  if(type==2)
  {
    return (char *)"Raw DEM";
  }
  if(type==3)
  {
    return (char *)"HEM";
  }
  if(type==4)
  {
    return (char *)"RLM";
  }
  if(type==5)
  {
    return (char *)"SDM";
  }
  if(type==6)
  {
    return (char *)"EDM";
  }
  if(type==7)
  {
    return (char *)"WBM";
  }
  if(type==8)
  {
    return (char *)"AMP";
  }
  if(type==9)
  {
    return (char *)"R_HEM";
  }
  if(type==10)
  {
    return (char *)"WAM";
  }
  if(type==11)
  {
    return (char *)"COV";
  }
  if(type==12)
  {
    return (char *)"R_AMP";
  }
  if(type==13)
  {
    return (char *)"AM2";
  }
  if(type==14)
  {
    return (char *)"COM";
  }
  if(type==15)
  {
    return (char *)"LSM";
  }



  
  printf("Bad type %d to PHT\n",type);

  return (char *)"unknown!";

}






void CheckHeader(char *filename,int type,double llx,double lly,int finished)
{

  

  TestGT123(filename,llx,lly,type,finished);

}



extern "C" int import_geotiff(char *filename, char *allshapes, int files_done, char *lod_override_in, int addtolong, int addtoshort, int type, char *destination)
{
  int i,j,len,chopit;
  GDALDatasetH hDataset;
  char rawfoldername[1000],testdir[1000];
  static char lod_override[1000];
  extern long int TestNumGridNodes;
  extern int num_grids,EGRIDNODELIMIT,SH_Grids_Prev,CREATING_TREX;
  extern void ReSetGridFile();
  extern unsigned char OverLappingGrids[365][185];
  extern int UseThisGrid(double x1,double y1, double x2, double y2);
  extern int IsDirectory(char *filename);
  extern int foundrawfolder;



  



  global_trex_layer = -1;

  if(CREATING_TREX==1)
  {
	chopit = 0;

    if(!strcmp(lod_override_in,"TREx DEM Finished_0"))
	{
	  

	  sprintf(rawfoldername,"%s",filename);
	  len = strlen(rawfoldername);

	  for(i=len-1;i>0;i--)
	  {
		if((rawfoldername[i]=='\\')||(rawfoldername[i]=='/'))
		{
		  rawfoldername[i] = '\0';
		  break;
		}
	  }


	  
      len = strlen(rawfoldername);
	  if(len>20)
	  {
		rawfoldername[len-16] = 'R';
		rawfoldername[len-6] = '\0';
	  }
	  

	  for(i=0;i<=99;i++)
	  {
		if(i<10)
		{
		  sprintf(testdir,"%s0%d",rawfoldername,i);
		}
		else
		{
		  sprintf(testdir,"%s%d",rawfoldername,i);
		}

	    if(IsDirectory(testdir))
		{ 
		  
		  foundrawfolder = 1;
		  break;
		} 
	    else
		{ 
		  
		} 
	  }
	}

    if(
		(!strcmp(lod_override_in,"ASTER GDEM v2_0"))     ||
		(!strcmp(lod_override_in,"SRTM Level 2_0"))      ||
		(!strcmp(lod_override_in,"DTED Level 1_0"))      ||
		(!strcmp(lod_override_in,"CDEM_0"))              ||
		(!strcmp(lod_override_in,"GMTED mn75_0"))        ||
		(!strcmp(lod_override_in,"GMTED mn300_0"))       ||
		(!strcmp(lod_override_in,"Active Earthscape_0")) ||
		(!strcmp(lod_override_in,"Edit Data Mask_0"))    ||
		(!strcmp(lod_override_in,"Height Error Map_0"))  ||
		(!strcmp(lod_override_in,"Source Data Mask_0"))  ||
		(!strcmp(lod_override_in,"Water Body Mask_0"))   ||
		(!strcmp(lod_override_in,"TREx DEM Finished_0"))
      )
	{
	  global_trex_layer = 0;
	  chopit = 1;
	}
    else if(
		(!strcmp(lod_override_in,"ASTER GDEM v2_1"))     ||
		(!strcmp(lod_override_in,"SRTM Level 2_1"))      ||
		(!strcmp(lod_override_in,"DTED Level 1_1"))      ||
		(!strcmp(lod_override_in,"CDEM_1"))              ||
		(!strcmp(lod_override_in,"GMTED mn75_1"))        ||
		(!strcmp(lod_override_in,"GMTED mn300_1"))       ||
		(!strcmp(lod_override_in,"Active Earthscape_1")) ||
		(!strcmp(lod_override_in,"Edit Data Mask_1"))    ||
		(!strcmp(lod_override_in,"Height Error Map_1"))  ||
		(!strcmp(lod_override_in,"Source Data Mask_1"))  ||
		(!strcmp(lod_override_in,"Water Body Mask_1"))   ||
		(!strcmp(lod_override_in,"TREx DEM Finished_1"))
      )
	{
	  global_trex_layer = 1;
	  chopit = 1;
	}
    else if(
		(!strcmp(lod_override_in,"ASTER GDEM v2_2"))     ||
		(!strcmp(lod_override_in,"SRTM Level 2_2"))      ||
		(!strcmp(lod_override_in,"DTED Level 1_2"))      ||
		(!strcmp(lod_override_in,"CDEM_2"))              ||
		(!strcmp(lod_override_in,"GMTED mn75_2"))        ||
		(!strcmp(lod_override_in,"GMTED mn300_2"))       ||
		(!strcmp(lod_override_in,"Active Earthscape_2")) ||
		(!strcmp(lod_override_in,"Edit Data Mask_2"))    ||
		(!strcmp(lod_override_in,"Height Error Map_2"))  ||
		(!strcmp(lod_override_in,"Source Data Mask_2"))  ||
		(!strcmp(lod_override_in,"Water Body Mask_2"))   ||
		(!strcmp(lod_override_in,"TREx DEM Finished_2"))
      )
	{
	  global_trex_layer = 2;
	  chopit = 1;
	}
    else if(
		(!strcmp(lod_override_in,"ASTER GDEM v2_3"))     ||
		(!strcmp(lod_override_in,"SRTM Level 2_3"))      ||
		(!strcmp(lod_override_in,"DTED Level 1_3"))      ||
		(!strcmp(lod_override_in,"CDEM_3"))              ||
		(!strcmp(lod_override_in,"GMTED mn75_3"))        ||
		(!strcmp(lod_override_in,"GMTED mn300_3"))       ||
		(!strcmp(lod_override_in,"Active Earthscape_3")) ||
		(!strcmp(lod_override_in,"Edit Data Mask_3"))    ||
		(!strcmp(lod_override_in,"Height Error Map_3"))  ||
		(!strcmp(lod_override_in,"Source Data Mask_3"))  ||
		(!strcmp(lod_override_in,"Water Body Mask_3"))   ||
		(!strcmp(lod_override_in,"TREx DEM Finished_3"))
      )
	{
	  global_trex_layer = 3;
	  chopit = 1;
	}

    sprintf(lod_override,"%s",lod_override_in);
	if(chopit==1)
	{
	  len = strlen(lod_override);
	  if(lod_override[len-2]=='_')
	  {
		lod_override[len-2] = '\0';
	  }
	  else
	  {
		 printf("Error: didn't find expected _ at position %d of %s\n",len-2,lod_override);
	  }
	}

  }
  else
  {
    sprintf(lod_override,"%s",lod_override_in);
  }





  if(TestNumGridNodes>EGRIDNODELIMIT)
    {
      
      ReSetGridFile();
      TestNumGridNodes = 0;
      SH_Grids_Prev = num_grids;
    }



  GDALAllRegister();

  BlankGTVars();

  hDataset = GDALOpen(filename,GA_ReadOnly);
  if(hDataset==NULL)
  {
	  printf("could not open\n");
  }
  else
  {

	  if(SummarizeDataset(filename,hDataset)==0)
	  {
	    GDALClose(hDataset);
	    return 0;
	  }

	  printf("\n"); 

      if(GDALGetMetadataItem(hDataset,"AREA_OR_POINT",NULL)!=NULL)  
	  {
        if(!strcmp(GDALGetMetadataItem(hDataset,"AREA_OR_POINT",NULL),"Point"))
		{  
          printf("Pixel type: Point, so using %lf %lf as upper left corner point location\n",USE_ULX,USE_ULY); 
		} 
        else
		{  
          printf("Pixel type: Area, so using %lf + %lf = %lf , %lf + %lf = %lf as upper left corner point location\n",
			  USE_ULX,USE_PIXELX/2.0,
			  USE_ULX+USE_PIXELX/2.0,
			  USE_ULY,USE_PIXELY/2.0,
			  USE_ULY+USE_PIXELY/2.0); 
		  USE_ULX = USE_ULX + USE_PIXELX/2.0;
		  USE_ULY = USE_ULY + USE_PIXELY/2.0;
		} 
	  }

if(0)
{	  

      if(GDALGetMetadataItem(hDataset,"TIFFTAG_RESOLUTIONUNIT",NULL)!=NULL)
	  {
		printf("found RESOLUTIONUNIT %s\n",GDALGetMetadataItem(hDataset,"TIFFTAG_RESOLUTIONUNIT",NULL));
	  }
      if(GDALGetMetadataItem(hDataset,"TIFFTAG_SOFTWARE",NULL)!=NULL)
	  {
		printf("found TIFFTAG_SOFTWARE %s\n",GDALGetMetadataItem(hDataset,"TIFFTAG_SOFTWARE",NULL));
	  }


      if(GDALGetMetadataItem(hDataset,"TIFFTAG_MINSAMPLEVALUE",NULL)!=NULL)
	  {
		printf("found TIFFTAG_MINSAMPLEVALUE %s\n",GDALGetMetadataItem(hDataset,"TIFFTAG_MINSAMPLEVALUE",NULL));
	  }



      if(GDALGetMetadataItem(hDataset,"TIFFTAG_ROWSPERSTRIP",NULL)!=NULL)
	  {
		printf("found TIFFTAG_ROWSPERSTRIP %s\n",GDALGetMetadataItem(hDataset,"TIFFTAG_ROWSPERSTRIP",NULL));
	  }
}



	  if(USE_COORDTYPE==1)
	  {
	    


		if(COORDINATEKEEPER==2)
		{
		  printf("Error: Cannot import Geodetic %s %s\nBecause a previous file in the InputSpecFile.txt has\ndefined this project to be UTM\n",GetDEMType(type),filename);
		  sprintf(ErrStr,"Error: Cannot import Geodetic %s %s\nBecause a previous file in the InputSpecFile.txt has\ndefined this project to be UTM\n",GetDEMType(type),filename);
	      GDALClose(hDataset);
	      return 0;
		}

        COORDINATEKEEPER = 1;


		if (
			(USE_ULX < -180)                              || 
			(USE_ULX >  180)                              || 
			(USE_ULY >   90)                              ||
			(USE_ULY <  -90)                              ||
			((USE_ULX + (USE_PIXELX*(USE_NUMX-1))) > 180) ||
			((USE_ULY + (USE_PIXELY*(USE_NUMY-1))) < -90)
		   )
		{
			printf("Error: %s %s\nInvalid coordinates for Geographic data:\n  Base (NW) coord: (%lf %lf) Spacing: %lf %lf Numx,Numy: %d %d\n  Computed SE coord: (%lf %lf)\n",
				GetDEMType(type),filename,USE_ULX,USE_ULY,USE_PIXELX,USE_PIXELY,USE_NUMX,USE_NUMY,
				USE_ULX + (USE_PIXELX*(USE_NUMX-1)),USE_ULY + (USE_PIXELY*(USE_NUMY-1)));
			sprintf(ErrStr,"Error: %s %s\nInvalid coordinates for Geographic data:\n  Base (NW) coord: (%lf %lf) Spacing: %lf %lf Numx,Numy: %d %d\n  Computed SE coord: (%lf %lf)\n",
				GetDEMType(type),filename,USE_ULX,USE_ULY,USE_PIXELX,USE_PIXELY,USE_NUMX,USE_NUMY,
				USE_ULX + (USE_PIXELX*(USE_NUMX-1)),USE_ULY + (USE_PIXELY*(USE_NUMY-1)));
			return 0;
		}

	    if(UseThisGrid(
		                USE_ULX,
					    USE_ULY,
		                USE_ULX + (USE_PIXELX*(USE_NUMX-1)),
		                USE_ULY + (USE_PIXELY*(USE_NUMY-1)))==0)
		{ 
	      
	      printf("...skipping %s due to being outside import rectangle\n",filename);
          return 1;
		}


		sh_testxyglobal(USE_ULX,USE_ULY,MY2DSENTINEL);
		sh_testxyglobal(USE_ULX + (USE_PIXELX*(USE_NUMX-1)),
			            USE_ULY + (USE_PIXELY*(USE_NUMY-1)),
						MY2DSENTINEL);



        

        for(i=0;i<365;i++)
		{
          for(j=0;j<185;j++)
		  {
			if(InSide(i,j,USE_ULX,USE_ULY,USE_ULX + (USE_PIXELX*(USE_NUMX-1)),USE_ULY + (USE_PIXELY*(USE_NUMY-1))))
			{
	          OverLappingGrids[i][j] = OverLappingGrids[i][j] + 1;


			}
		  } 
		} 


	  }
	  else
	  {
		if(COORDINATEKEEPER==1)
		{
		  printf("Cannot import TM/UTM %s %s\nBecause a previous file in the InputSpecFile.txt has\ndefined this project to be in Geographic coordinates\n",GetDEMType(type),filename);
		  sprintf(ErrStr,"Cannot import TM/UTM %s %s\nBecause a previous file in the InputSpecFile.txt has\ndefined this project to be in Geographic coordinates\n",GetDEMType(type),filename);
	      GDALClose(hDataset);
	      return 0;
		}

        COORDINATEKEEPER = 2;

		sh_testxyglobal(USE_ULX,USE_ULY,MY2DSENTINEL);
		sh_testxyglobal(USE_ULX + (USE_PIXELX*(USE_NUMX-1)),
			            USE_ULY + (USE_PIXELY*(USE_NUMY-1)),
						MY2DSENTINEL);

        for(i=0;i<365;i++)
		{
          for(j=0;j<185;j++)
		  { 
	        OverLappingGrids[i][j] = OverLappingGrids[i][j] + 1;
		  } 
		} 


	    printf("Coordinate type: TM/UTM\n");
		printf("Meridian: %lf Zone: %lf (%d)\n",USE_MERIDIAN,USE_ZONE,USE_ZONE2);
	  }





	  double tempLLY;
	  int i,SzI=sizeof(int),SzD=sizeof(double),elev_index=-1,index,index2,sh_sac_short_index=0, sh_sac_long_index=0,attrindex;
      int first_axis=2;  
      char DTED_STRING[100],deconst[100];
      struct sh_long_attr_list  SH_SAC_LONG [SH_LISTLEN];   
      struct sh_short_attr_list SH_SAC_SHORT[SH_LISTLEN]; 
	  extern int num_grids,CREATING_TREX;
	  extern int trexfinishedindex0,trexfinishedindex1,trexfinishedindex2,trexfinishedindex3;
	  extern int trexwatermaskindex0,trexwatermaskindex1,trexwatermaskindex2,trexwatermaskindex3;

      extern double maxgridspace;
      extern int SH_FindSCC_LONG(int type, int scc, int numsac, 
			     struct sh_long_attr_list SAC_LONG[], int record_num);
      extern int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			      struct sh_short_attr_list SAC_SHORT[], int record_num);
      extern int SilentGetAttrIndex(char *eac);

      sh_sac_short_index = 0;
      sh_sac_long_index  = 0;

      printf("Importing %s file %s with %s\n",GetDEMType(type),filename,lod_override);

      for(i=0;i<SH_LISTLEN;i++)
	  {
        SH_SAC_LONG [i].lod_stuff = NULL;
        SH_SAC_SHORT[i].lod_stuff = NULL;
        SH_SAC_LONG [i].nam_stuff = NULL;
        SH_SAC_SHORT[i].nam_stuff = NULL;
        SH_SAC_LONG [i].Cvalue    = NULL;
	  } 


	  if(ZONE_IN_USE!=-999)
	  {
        sprintf(DTED_STRING,"-1 %d %s (zone %d)",C_GRID,lod_override,ZONE_IN_USE);
	  }
	  else
	  {
        sprintf(DTED_STRING,"-1 %d %s",C_GRID,lod_override);
	  }


      SH_SAC_LONG[sh_sac_long_index].lod_stuff = (char *)malloc(strlen(DTED_STRING)+10);
      if(SH_SAC_LONG[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}
  
      SH_SAC_SHORT[sh_sac_short_index].lod_stuff = (char *)malloc(strlen(DTED_STRING)+10);
      if(SH_SAC_SHORT[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}
  
      sprintf(SH_SAC_LONG [sh_sac_long_index ].lod_stuff,"%s",DTED_STRING);
      sprintf(SH_SAC_SHORT[sh_sac_short_index].lod_stuff,"%s",DTED_STRING);
  
      SH_SAC_LONG [sh_sac_long_index ].attr_code = -1;
      SH_SAC_SHORT[sh_sac_short_index].attr_code = -1;
  
      sh_sac_short_index++;
      sh_sac_long_index++;


      SH_SAC_LONG[sh_sac_long_index].nam_stuff = (char *)malloc(strlen(lod_override)+50);
      if(SH_SAC_LONG[sh_sac_long_index].nam_stuff==NULL){printf("out of memory!\n"); exit(-1);}
  
      SH_SAC_SHORT[sh_sac_short_index].nam_stuff = (char *)malloc(strlen(lod_override)+50);
      if(SH_SAC_SHORT[sh_sac_long_index].nam_stuff==NULL){printf("out of memory!\n"); exit(-1);}
  
      sprintf(SH_SAC_LONG [sh_sac_long_index ].nam_stuff,"(Layer: %s) ",lod_override);
      sprintf(SH_SAC_SHORT[sh_sac_short_index].nam_stuff,"(Layer: %s) ",lod_override);
  
      SH_SAC_LONG [sh_sac_long_index ].attr_code = -2;
      SH_SAC_SHORT[sh_sac_short_index].attr_code = -2;
  
      sh_sac_short_index++;
      sh_sac_long_index++;



      

	  sprintf(deconst,"TXT");

	  attrindex = SilentGetAttrIndex(deconst);
	  if(attrindex<0)
	  {
        sprintf(deconst,"MEM");
	    attrindex = SilentGetAttrIndex(deconst);
	  }

	  attrindex = -3;
	  
      SH_SAC_LONG[sh_sac_long_index].attr_code  = attrindex;
      SH_SAC_LONG[sh_sac_long_index].unit_code  = TIM_UNITLESS;
      SH_SAC_LONG[sh_sac_long_index].scale_code = TIM_UNI;
      SH_SAC_LONG[sh_sac_long_index].SEDRIStype = TIM_STRING;
      SH_SAC_LONG[sh_sac_long_index].Cvalue     = NULL;
      SH_SAC_LONG[sh_sac_long_index].lod_stuff  = NULL;
      SH_SAC_LONG[sh_sac_long_index].nam_stuff  = NULL;
      SH_SAC_LONG[sh_sac_long_index].geomtype   = 2;  
      SH_SAC_LONG[sh_sac_long_index].Cvalue     =  (char *) malloc(strlen(filename)+2);
      sprintf(SH_SAC_LONG[sh_sac_long_index].Cvalue,"%s",filename);

      SH_SAC_SHORT[sh_sac_long_index].attr_code = attrindex;
      SH_SAC_SHORT[sh_sac_long_index].lod_stuff = NULL;
      SH_SAC_SHORT[sh_sac_long_index].nam_stuff = NULL;

      sh_sac_short_index++;
      sh_sac_long_index++;

      index  = SH_FindSCC_LONG (9,elev_index,sh_sac_long_index, SH_SAC_LONG,-1);
      index2 = SH_FindSCC_SHORT(9,elev_index,sh_sac_short_index,SH_SAC_SHORT,-1);

      index  = index  + addtolong;
      index2 = index2 + addtoshort;


	  if(CREATING_TREX==1)
	  {
        if(!strcmp(lod_override,"TREx DEM Finished"))
		{
		  if(global_trex_layer==0)
		  {
		    trexfinishedindex0 = index;
		  }
		  if(global_trex_layer==1)
		  {
		    trexfinishedindex1 = index;
		  }
		  if(global_trex_layer==2)
		  {
		    trexfinishedindex2 = index;
		  }
		  if(global_trex_layer==3)
		  {
		    trexfinishedindex3 = index;
		  }
		}
        if(!strcmp(lod_override,"Water Body Mask"))
		{
		  if(global_trex_layer==0)
		  {
		    trexwatermaskindex0 = index;
		  }
		  if(global_trex_layer==1)
		  {
		    trexwatermaskindex1 = index;
		  }
		  if(global_trex_layer==2)
		  {
		    trexwatermaskindex2 = index;
		  }
		  if(global_trex_layer==3)
		  {
		    trexwatermaskindex3 = index;
		  }
		}
	  }



     

     for(i=0;i<sh_sac_long_index;i++)
     {
       if(SH_SAC_LONG[i].attr_code==-1)
	   { 
	     free(SH_SAC_LONG[i].lod_stuff);
         SH_SAC_LONG[i].lod_stuff = NULL;
	   } 

       if(SH_SAC_LONG[i].attr_code==-2)
	   { 
	     free(SH_SAC_LONG[i].nam_stuff);
	     SH_SAC_LONG[i].nam_stuff = NULL;
	   } 

       if(SH_SAC_LONG[i].Cvalue!=NULL)
	   {  
	     free(SH_SAC_LONG[i].Cvalue);
	     SH_SAC_LONG[i].Cvalue = NULL;
	   }  
	 } 

     for(i=0;i<sh_sac_short_index;i++)
	 { 
       if(SH_SAC_SHORT[i].attr_code==-1)
	   { 
	     free(SH_SAC_SHORT[i].lod_stuff);
	     SH_SAC_SHORT[i].lod_stuff = NULL;
	   } 

       if(SH_SAC_SHORT[i].attr_code==-2)
	   { 
	     free(SH_SAC_SHORT[i].nam_stuff);
         SH_SAC_SHORT[i].nam_stuff = NULL;
	   } 
	 }


     if((USE_PIXELX*GetGSMod())>maxgridspace)
	 { 
       maxgridspace = USE_PIXELX*GetGSMod();
	 } 
     if((USE_PIXELY*GetGSMod())>maxgridspace)
	 { 
       maxgridspace = USE_PIXELY*GetGSMod();
	 } 

	 if(USE_PIXELY<0) 
	 {
       USE_PIXELY = USE_PIXELY * -1;
	 }

     fwrite(&index ,SzI,1,gridbin);
     fwrite(&index2,SzI,1,gridbin);
     fwrite(&USE_PIXELX,SzD,1,gridbin);
     fwrite(&USE_PIXELY,SzD,1,gridbin);
     fwrite(&USE_ULX,SzD,1,gridbin);

	 tempLLY = USE_ULY - ((USE_NUMY-1)*(USE_PIXELY));





     fwrite(&tempLLY,SzD,1,gridbin);  
     fwrite(&first_axis,SzI,1,gridbin);
     fwrite(&USE_NUMX,SzI,1,gridbin);
     fwrite(&USE_NUMY,SzI,1,gridbin);

     


	 printf("Write %d x %d at spacing of %lf x %lf, with ULcorner (%lf %lf) and LLcorner (%lf %lf)\n",
		 USE_NUMX,USE_NUMY,USE_PIXELX,USE_PIXELY,USE_ULX,USE_ULY,USE_ULX,tempLLY);
		 


     


	  if(ReadData(hDataset,1,allshapes,filename,files_done,lod_override,addtolong,addtoshort,USE_NUMX,USE_NUMY,USE_PIXELX,USE_PIXELY,USE_ULX,tempLLY,USE_ULY,destination)==0)
	  {
		 GDALClose(hDataset);
		 return 0;
	  }







	  num_grids = num_grids + 1;


	  GDALClose(hDataset);
  }

  return 1;
}





