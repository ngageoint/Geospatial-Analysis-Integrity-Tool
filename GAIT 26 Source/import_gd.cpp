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
#include "shapefil.h"
#include "math.h"


#include <afxwin.h>         
#include <afxdisp.h>        





#include "GAIT_API.h"



#pragma warning(push)
#pragma warning(disable:4192) 
#pragma warning(disable:4146) 
#pragma warning(disable:4042)

#import <C:\Program Files (x86)\ArcGIS\Desktop10.0\com\esriSystem.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids exclude("OLE_COLOR", "OLE_HANDLE", "ICursorPtr", "VARTYPE")
#import <C:\Program Files (x86)\ArcGIS\Desktop10.0\com\esriDataSourcesGDB.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <C:\Program Files (x86)\ArcGIS\Desktop10.0\com\esriGeometry.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <C:\Program Files (x86)\ArcGIS\Desktop10.0\com\esriGeoDatabase.olb> raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import <C:\Program Files (x86)\Common Files\ArcGIS\bin\ArcGISVersion.dll> no_namespace raw_interfaces_only no_implementation rename("esriProductCode","esriVersionProductCode")

#pragma warning(pop)


extern "C" int REPORT_BEZIER;
extern "C" int NoZ_Values;






char *ParseProduct(int product)
{
  if(product==0)
  {
    return "esriArcGIS"; 
  }
  if(product==1)
  {
    return "esriArcGISDesktop";
  }
  if(product==2)
  {
    return "esriArcGISEngine";
  }

  

  if(product==3)
  {
    return "esriArcGISReader";
  }
  if(product==4)
  {
    return "esriArcGISExplorer";
  }
  if(product==5)
  {
    return "esriArcGISServer";
  }

  printf("returning unknown product %d\n",product);
  return "unknown!";
}


void DoVersionStuff()
{
  IArcGISVersionPtr ipVersion;

  HRESULT hr = ipVersion.CreateInstance(__uuidof(VersionManager));

  if (SUCCEEDED(hr) && ipVersion != NULL)
  { 
    IEnumVersionsPtr ipVersions;;
    ipVersion->GetVersions(&ipVersions);
    ipVersions->Reset();
    esriVersionProductCode product;

    BSTR ver;
    BSTR path;

    bool notFound = true;
 

    while (S_OK == ipVersions->Next(&product, &ver, &path))
    { 
	  

      if (product == 1) 
	  {  
        VARIANT_BOOL bWorked;
        ipVersion->LoadVersion(product, ver, &bWorked);
        notFound = false;
        break;
	  }  
      else if (product == 2) 
	  {  
        VARIANT_BOOL bWorked;
        ipVersion->LoadVersion(product, ver, &bWorked);
        notFound = false;
        break;
	  }  
	}  

    if (notFound)
	{ 
      printf("Error: ArcGIS10 found but could not find ArcGIS Desktop or Engine product\n");
	} 
    else
	{ 
      printf("ArcGIS10 detected...found product %s\n",ParseProduct(product));
	} 

  } 
  else
  {
    printf("ArcGIS 10.0 not detected\n");
  }

}





#define SH_LISTLEN 1000 


struct sh_long_attr_list
{
  short int attr_code;         
  unsigned char unit_code;     
  unsigned char scale_code;    
  unsigned char SEDRIStype;    
  
  unsigned char geomtype;   
  
  double Dvalue;
  char  *Cvalue; 
  char *lod_stuff;
  char *nam_stuff;
};


struct sh_short_attr_list
{
  char *lod_stuff;
  char *nam_stuff;
  int attr_code;
};





int badhole = 0;
int verts_allocced = 0;	
double *bigX = NULL;
double *bigY = NULL;
double *bigZ = NULL;

double **Xcoords;
double **Ycoords;
double **Zcoords;
int *NumvertArray;    
int *NumvertMaxArray; 
int *ExteriorArray;   
int *ParentRing;      
int maxnumparts = 0;


extern "C" FILE *badcurves;

extern "C" int ATTRTYPE_ANY_MGCP();
extern "C" int ATTRTYPE_ANY_GGDM();
extern "C" void NFIWrapper(char *saveNFI, char *saveNFN, char *saveNAM, int classcode, int geomtype);
extern "C" int DoNFI();
extern "C" char *MakePlural(int num);


extern "C" void WritePointFeature(char *dbname,int addtolong, int addtoshort, 
								  int class_code, int featureid,
					              int sh_sac_long_index,int sh_sac_short_index,
			                      struct sh_long_attr_list  SH_SAC_LONG[], struct sh_short_attr_list SH_SAC_SHORT[],
					              double xcoord,double ycoord,double zcoord)
{
  int shortindex,longindex,seeitgeom = 5, numverts = -1; 	      	      
  char tempstr[1000],mdbname[1000];
  extern FILE *modelbin;
  extern long int NumPointNodes;
  extern int global_gifd_id,SzI,SzD,SH_Points_Done;
  extern void Record_LL_Mapping(int GFID_index,int longindex, int FID);
  extern void sh_testxyglobal(double x,double y,double z);
  extern int SH_FindSCC_LONG(int type, int scc, int numsac, 
			   struct sh_long_attr_list SAC_LONG[], int record_num);
  extern int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			    struct sh_short_attr_list SAC_SHORT[], int record_num);
  extern void GetShortName(char shortname[],char *shape_to_import);


  GetShortName(mdbname,dbname);

  sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
  sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s;Table %s",class_code,seeitgeom,mdbname,tempstr);
  sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s;Table %s",class_code,seeitgeom,mdbname,tempstr);


  longindex  = SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index, SH_SAC_LONG, featureid); 
  shortindex = SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, featureid);
	  

  longindex  = longindex  + addtolong;
  shortindex = shortindex + addtoshort;

  
  fwrite(&featureid,      SzI,1,modelbin);
  fwrite(&longindex,      SzI,1,modelbin);
  fwrite(&shortindex,     SzI,1,modelbin);      
  fwrite(&global_gifd_id, SzI,1,modelbin);

  Record_LL_Mapping(global_gifd_id,longindex,featureid);

  fwrite(&numverts, SzI,1,modelbin);
  fwrite(&xcoord,   SzD,1,modelbin);
  fwrite(&ycoord,   SzD,1,modelbin);
  fwrite(&zcoord,   SzD,1,modelbin);
  
  NumPointNodes++;
  
  sh_testxyglobal(xcoord,ycoord,zcoord);


  SH_Points_Done = SH_Points_Done + 1;			  

}



extern "C" void WriteLineFeature(int addtolong, int addtoshort, 
								  int class_code, int featureid,
					              int sh_sac_long_index,int sh_sac_short_index,
			                      struct sh_long_attr_list  SH_SAC_LONG[], struct sh_short_attr_list SH_SAC_SHORT[],
					              int numcoords, int part_num, int numparts, int longindex,int shortindex)
{
  int i,seeitgeom=4 ;
  extern FILE *roadnetbin, *multipartlines;
  extern long int NumLinearNodes,TestNumLinearNodes;
  extern int global_gifd_id,SzI,SzD,SH_Linears_Done,ELINEARNODELIMIT,SH_Linears_Prev;
  extern void Record_LL_Mapping(int GFID_index,int longindex, int FID);
  extern void sh_testxyglobal(double x,double y,double z);
  extern void ReSetLinearFile();


  if(TestNumLinearNodes>ELINEARNODELIMIT)
    {      
      
      ReSetLinearFile();
      TestNumLinearNodes = 0;
      SH_Linears_Prev = SH_Linears_Done;
    }
  
  fwrite(&featureid,     SzI,1,roadnetbin);
  fwrite(&longindex,     SzI,1,roadnetbin);
  fwrite(&shortindex,    SzI,1,roadnetbin);
  fwrite(&global_gifd_id,SzI,1,roadnetbin);
  fwrite(&numcoords,     SzI,1,roadnetbin);


   if(part_num==0)
  {
    Record_LL_Mapping(global_gifd_id,longindex,featureid);
    SH_Linears_Done = SH_Linears_Done + 1;	
  }

  for(i=0;i<numcoords;i++)
    {  
      fwrite(&bigX[i],SzD,1,roadnetbin);
      fwrite(&bigY[i],SzD,1,roadnetbin);
      fwrite(&bigZ[i],SzD,1,roadnetbin);
      
      NumLinearNodes     = NumLinearNodes     + 1;
      TestNumLinearNodes = TestNumLinearNodes + 1;
      
      sh_testxyglobal(bigX[i],bigY[i],bigZ[i]);
    }
  
  
  
  if(numparts>1)
  {
    fwrite(&featureid,      SzI,1,multipartlines);
    fwrite(&longindex,      SzI,1,multipartlines);
    fwrite(&shortindex,     SzI,1,multipartlines);
    fwrite(&global_gifd_id, SzI,1,multipartlines);
    fwrite(&numcoords,      SzI,1,multipartlines);  

    for(i=0;i<numcoords;i++)
	{	  
	  fwrite(&bigX[i],SzD,1,multipartlines);
	  fwrite(&bigY[i],SzD,1,multipartlines);
	  fwrite(&bigZ[i],SzD,1,multipartlines);
	}  
  } 

}



extern "C" void WriteAreaFeature(int numcoords,int featureid,int longindex,int shortindex,
								 int ishole,int multipart,int toppart,int part_num)
{
  int i;
  extern FILE *multipartareas;
  extern FILE *arealbin, *multipartareas;
  extern long int NumArealNodes,TestNumArealNodes;
  extern int global_gifd_id,SzI,SzD,SH_Areals_Done,EAREALNODELIMIT,SH_Areals_Prev;
  extern void Record_LL_Mapping(int GFID_index,int longindex, int FID);
  extern void sh_testxyglobal(double x,double y,double z);
  extern void ReSetArealFile();
  extern void AddNullShape(int FID, int type, int numverts, int part_num);
  static int savegifd = 5341222;
  static int savelongindex = 51232343;
  static int savefeatureid = 53454344;


  if(numcoords<3)
  {
    if(part_num>0)
	{
      printf("Error: found part %d of area feature with FID %d with only %d vertices\n",
			  part_num,featureid,numcoords);
	}
    else
	{
      printf("Error: found area feature with FID %d with only %d vertices\n",featureid,numcoords);
	}

    AddNullShape(featureid,0,numcoords,part_num);
    return;
  }

  if((TestNumArealNodes>EAREALNODELIMIT)&&(ishole==0))
    {
      
      
      ReSetArealFile();
      TestNumArealNodes = 0;
      SH_Areals_Prev = SH_Areals_Done;
    }


  fwrite(&featureid,     SzI,1,arealbin);
  fwrite(&ishole,        SzI,1,arealbin);
  fwrite(&longindex,     SzI,1,arealbin);
  fwrite(&shortindex,    SzI,1,arealbin);
  fwrite(&global_gifd_id,SzI,1,arealbin);
  fwrite(&numcoords,     SzI,1,arealbin);
  
  if((toppart==0)&&(ishole==0))
  {
	if(badhole==1)
	{
      
	  if((global_gifd_id!=savegifd)||(longindex!=savelongindex)||(featureid!=savefeatureid))
	  {
        Record_LL_Mapping(global_gifd_id,longindex,featureid);
	  }
	}
	else
	{
      Record_LL_Mapping(global_gifd_id,longindex,featureid);
	}
  }

  savegifd = global_gifd_id;
  savelongindex = longindex;
  savefeatureid = featureid;


  if((ishole==1)||(badhole==1))
  {
    for(i=0;i<numcoords;i++)
	{       
      fwrite(&bigX[i],SzD,1,arealbin);
      fwrite(&bigY[i],SzD,1,arealbin);
      fwrite(&bigZ[i],SzD,1,arealbin);
      
      NumArealNodes     = NumArealNodes     + 1;
      TestNumArealNodes = TestNumArealNodes + 1;
      
      sh_testxyglobal(bigX[i],bigY[i],bigZ[i]);
    }
  }
  else
  {
    for(i=0;i<numcoords;i++)
	{       
      fwrite(&Xcoords[part_num][i],SzD,1,arealbin);
      fwrite(&Ycoords[part_num][i],SzD,1,arealbin);
      fwrite(&Zcoords[part_num][i],SzD,1,arealbin);
      
      NumArealNodes     = NumArealNodes     + 1;
      TestNumArealNodes = TestNumArealNodes + 1;
      
      sh_testxyglobal(Xcoords[part_num][i],Ycoords[part_num][i],Zcoords[part_num][i]);
    }
  }
  
  if((multipart==1)&&(ishole==0))
    {	   
      fwrite(&featureid,      SzI,1,multipartareas);
      fwrite(&longindex,      SzI,1,multipartareas);
      fwrite(&shortindex,     SzI,1,multipartareas);
      fwrite(&global_gifd_id, SzI,1,multipartareas);
      fwrite(&numcoords,      SzI,1,multipartareas);  
      
      for(i=0;i<numcoords;i++)
	{ 
	  if(badhole==1)
	  {
        fwrite(&bigX[i],SzD,1,multipartareas);
        fwrite(&bigY[i],SzD,1,multipartareas);
        fwrite(&bigZ[i],SzD,1,multipartareas);
	  }
	  else
	  {
	   fwrite(&Xcoords[part_num][i],SzD,1,multipartareas);
	   fwrite(&Ycoords[part_num][i],SzD,1,multipartareas);
	   fwrite(&Zcoords[part_num][i],SzD,1,multipartareas);
	  }
	}
    }  
}




extern "C" void GetDateComponents(int intdate,int *day,int *month,int *year)
{
  int i,j,l,n;

  

  l = intdate + 68569 + 2415019;
  n = int(( 4 * l ) / 146097);

  l = l - int(( 146097 * n + 3 ) / 4);
  i = int(( 4000 * ( l + 1 ) ) / 1461001);
  l = l - int(( 1461 * i ) / 4) + 31;
  j = int(( 80 * l ) / 2447);

  *day = l - int(( 2447 * j ) / 80);
  l = int(j / 11);
  *month = j + 2 - ( 12 * l );
  *year = 100 * ( n - 49 ) + i + l;
}



extern "C" int Arc_ClipTest(int featureid, char *dbname, int numcoords, int type, int numparts)
{
  
  

  extern int CLIPEXTRACT;
  extern double CLIPLLX;
  extern double CLIPLLY;
  extern double CLIPURX;
  extern double CLIPURY;
  extern int TestGeodetic(double x, double y,int FID,char *shape_to_import);
  
  int i,j,part_num;
  
  

  
  if((type==0)||(type==3)) 
  {
    for(i=0;i<numcoords;i++)
    {
      if(TestGeodetic(bigX[i],bigY[i],featureid,dbname)==0)
	  {
	    return 0;
	  }
    }
  }
  else if(type==1)
  {
    for(part_num=0;part_num<numparts;part_num++)
	{
	  if(ExteriorArray[part_num]!=1)
	  {
	    continue;
	  }

	  for(i=0;i<NumvertArray[part_num];i++)
	  {
        if(TestGeodetic(Xcoords[part_num][i],Ycoords[part_num][i],featureid,dbname)==0)
		{
	      return 0;
		}
	  }
	}
  }

  if(type==3)
  {
	return 1;
  }
  
  if(CLIPEXTRACT==1)
  {
	if(type==0)
	{
      for(j=0;j<numcoords;j++)
	  {
	    if((bigX[j]>=CLIPLLX) &&
	       (bigY[j]>=CLIPLLY) &&
	       (bigX[j]<=CLIPURX) &&
	       (bigY[j]<=CLIPURY))
		{
	      return 1;
		}
	  }
	}
	else if(type==1)
	{
      for(part_num=0;part_num<numparts;part_num++)
	  {
		if(ExteriorArray[part_num]!=1)
		{
		  continue;
		}

	    for(i=0;i<NumvertArray[part_num];i++)
		{
	      if((Xcoords[part_num][i]>=CLIPLLX) &&
	         (Ycoords[part_num][i]>=CLIPLLY) &&
	         (Xcoords[part_num][i]<=CLIPURX) &&
	         (Ycoords[part_num][i]<=CLIPURY))
		  {
	        return 1;
		  }
		}
	  }
	}
    
    return 0;
  } 
  
  return 1;   
  
} 



extern "C" void FreeArcMem()
{
  int indexer;


  

  if(verts_allocced>0)
  {
    free(bigX);
    free(bigY);
    free(bigZ);
	bigX = NULL;
	bigY = NULL;
	bigZ = NULL;
  }
  verts_allocced = 0;



  if(maxnumparts>0)
  {
    for(indexer=0;indexer<maxnumparts;indexer++)
	{
	  if(NumvertMaxArray[indexer]>0)
	  {
		
	    free(Xcoords[indexer]);
	    free(Ycoords[indexer]);
	    free(Zcoords[indexer]);
	  }
	}

	free(Xcoords);
	free(Ycoords);
	free(Zcoords);
	free(NumvertArray);
	free(NumvertMaxArray);
	free(ExteriorArray);
	free(ParentRing);

	
    maxnumparts=0;
  }

}



extern "C" void ESRIErrorHandle(int errnum, char *errstr,HRESULT hr)
{
  char errmsg[1000],hmessage[1000];

  


  switch(errnum)
  {
    case 1:
      sprintf(errmsg,"ERIS %d: failed to open %s\n",hr,errstr);
      break;
      
    case 2:
      sprintf(errmsg,"ERIS %d: error while %s\n",hr,errstr);
      break;

    default:
      printf("(%d) unexpected ERIS error code %d\n",hr,errnum);
      break;
  }

  printf("%s",errmsg);

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	  NULL, hr, 0, hmessage, 1000, NULL);

  printf("  error 0x%08x: %s %s",hr,_com_error(hr).ErrorMessage(),hmessage);

}




double *tempVertX=NULL,*tempVertY=NULL,*tempVertZ=NULL;
int tempvertnum = 0;
IPoint *OUTpoint;
	

int Init_Outpoint(int reset)
{	   
  HRESULT hr;
  static int done=0;


  if(reset==1)
  {
    done = 0;
	return 1;
  }


  

  if(done==1)
  {
    return 1;
  }

  hr = CoCreateInstance(CLSID_Point, NULL, 
			CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, 
			IID_IPoint, (void**)&OUTpoint);
  if(hr!=S_OK)
  { 
    printf("failed to create IOP instance\n");
    return 0;
  } 

  done = 1;

  return 1;
}



int SuperClose(double a, double b)
{
  if(fabs(a-b)<0.0000001)
    {
      return 1;
    }
  return 0;
}


int GetCurveVertices(IGeometryPtr thispart,int featureid, char *mdbname,char *table)
{
  ISegmentCollectionPtr segcoll = thispart;
  VARIANT_BOOL has_nonlinear,hasZ;
  HRESULT hr;
  int num_verts_made=0,reported,SzD=sizeof(double);
  int thisseg,needtodensify=1;
  long num_segments=0;


  hr = segcoll->HasNonLinearSegments(&has_nonlinear);




  if(SUCCEEDED(hr))
  { 

    if(has_nonlinear==VARIANT_TRUE)
	{ 

	  Init_Outpoint(0);

	  segcoll->get_SegmentCount(&num_segments);

printf("Handling curve (Bezier, Circle, or Ellipse) with %d segments (featureid %d) (%s;Table %s)\n",num_segments,featureid,mdbname,table);

      reported = 0;

      for(thisseg=0;thisseg<num_segments;thisseg++)
	  {
	    ISegmentPtr segment;

        hr = segcoll->get_Segment(thisseg,&segment);
	    if(SUCCEEDED(hr))
		{
		  ILinePtr outsegs;
		  double seglen,x,x2,y,y2,z,z2;
		  int pieces;

          segment->get_Length(&seglen);

          esriGeometryType thistype;
          segment->get_GeometryType(&thistype);

          if(thistype==esriGeometryBezier3Curve)
		  {

			 if(reported==0)  
			 {
			   if(REPORT_BEZIER==1)
			   {
		         fprintf(badcurves,"Bezier curve with %d segment%s found for FID %d in %s;Table %s\n",num_segments,MakePlural(num_segments),featureid,mdbname,table);
			   }
			   reported = 1;
			 }
             needtodensify = 1;
		  }
		  else if(thistype==esriGeometryCircularArc)
		  {

			 if(reported==0) 
			 {
			   if(REPORT_BEZIER==1)
			   {
		         fprintf(badcurves,"Circular curve with %d segment%s found for FID %d in %s;Table %s\n",num_segments,MakePlural(num_segments),featureid,mdbname,table);
			   }
			   reported = 1;
			 }
             needtodensify = 1;
		  }
		  else if(thistype==esriGeometryEllipticArc)
		  {

			 if(reported==0) 
			 {
			   if(REPORT_BEZIER==1)
			   {
		         fprintf(badcurves,"Elliptic curve with %d segment%s found for FID %d in %s;Table %s\n",num_segments,MakePlural(num_segments),featureid,mdbname,table);
			   }
			   reported = 1;
			 }
             needtodensify = 1;
		  }
		  else
		  {

             needtodensify = 0; 
		  }


		  

         

         if(seglen>1) 
		 { 
           pieces = 1000000;
		 } 
         else if(seglen>.1) 
		 { 
           pieces = 100000;
		 } 
         else if(seglen>.01) 
		 {
           pieces = 10000;
		 } 
         else if(seglen>.001) 
		 { 
           pieces = 1000;
		 } 
         else if(seglen>.0001) 
		 {
           pieces = 100;
		 } 
         else if(seglen>.00001) 
		 { 
           pieces = 10;
		 } 
 
  ILine *qq[5100];  
  long numsegs;
  int hh;


if(needtodensify==1)
{
  pieces = 100;

  segment->Densify(5000,0.00000001,&numsegs,qq);

  pieces = numsegs;
}
else
{
  pieces = 1;
}

 if(tempvertnum==0)
		   {
             tempVertX = (double *)malloc(SzD*(pieces+1000));
             tempVertY = (double *)malloc(SzD*(pieces+1000));
             tempVertZ = (double *)malloc(SzD*(pieces+1000));
             tempvertnum = pieces+1000;
		   }
           else if( (pieces+num_verts_made) > (tempvertnum-10))
		   {
             tempVertX = (double *)realloc(tempVertX,SzD*(pieces+num_verts_made+1000));
             tempVertY = (double *)realloc(tempVertY,SzD*(pieces+num_verts_made+1000));
             tempVertZ = (double *)realloc(tempVertZ,SzD*(pieces+num_verts_made+1000));
             tempvertnum = pieces+1000;
		   }

for(hh=0;hh<pieces;hh++)
{
    IPointPtr ipPointFrom;
    IPointPtr ipPointTo;

	

	if(needtodensify==0)
	{
	  segment->get_FromPoint(&ipPointFrom);
	  segment->get_ToPoint(&ipPointTo);
	}
	else
	{
	  qq[hh]->get_FromPoint(&ipPointFrom);
	  qq[hh]->get_ToPoint(&ipPointTo);
	}

    IZAwarePtr ipZawareFrom = ipPointFrom;
    IZAwarePtr ipZawareTo = ipPointTo;

    ipZawareFrom->get_ZAware(&hasZ);      
    ipPointFrom->get_X(&x);                   
	ipPointFrom->get_Y(&y);

	if((hasZ==VARIANT_TRUE)&&(NoZ_Values==0))  
	{
	  hr = ipPointFrom->get_Z(&z);              
	}   
   else
   {
	  z = MY2DSENTINEL;
   }  


    ipZawareTo->get_ZAware(&hasZ);      
    ipPointTo->get_X(&x2);                   
	ipPointTo->get_Y(&y2);

	if((hasZ==VARIANT_TRUE)&&(NoZ_Values==0))  
	{
	  hr = ipPointTo->get_Z(&z2);
	}
   else
   {
	  z2 = MY2DSENTINEL;
   }  


   if(num_verts_made==4990)
   {
     printf("location of curve (circle, ellipse, or Bezier) with extremely large number of vertices: %lf %lf\n",x,y);
   }

   int addFrompoint = 0;

   if(num_verts_made==0)
   {
     addFrompoint = 1;
   }
   else
   {
	   if(
		   (SuperClose(tempVertX[num_verts_made-1],x)) &&
		   (SuperClose(tempVertY[num_verts_made-1],y)) &&
		   (SuperClose(tempVertZ[num_verts_made-1],z))
		   )
	   {
         addFrompoint = 0;
	   }
	   else
	   {
         addFrompoint = 1;
	   }
   }

   if(addFrompoint==1)
   {
           tempVertX[num_verts_made] = x;
           tempVertY[num_verts_made] = y;
           tempVertZ[num_verts_made] = z;
           num_verts_made = num_verts_made + 1;
   }

           tempVertX[num_verts_made] = x2;
           tempVertY[num_verts_made] = y2;
           tempVertZ[num_verts_made] = z2;
           num_verts_made = num_verts_made + 1;


		 } 
		} 
	  }  
	} 
    else
	{ 
      
	} 
 }


 return num_verts_made;

}






extern "C" int StoreHoleVerts(IGeometryCollectionPtr theParts, int part_num,IAoInitializePtr ipInit, int featureid, char *mdbname, char *table)
{
  HRESULT hr;
  IGeometryPtr thispart;
  VARIANT_BOOL hasZ;
  char errstr[1000];
  int pointnum;
  long numverts;
  extern int num2area;
  extern int num3area;
  extern int SzD;


  hr = theParts->get_Geometry(part_num,&thispart);
  if(!SUCCEEDED(hr))
  { 

	sprintf(errstr,"Unable to GetG1 PG\n");
    ESRIErrorHandle(2,errstr,hr);
	FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
  }  
 

  IPointCollectionPtr thiscollection = thispart;



  numverts = GetCurveVertices(thispart,featureid,mdbname,table);

  if(numverts>0)
  { 



    if(numverts>verts_allocced)
	{ 
	  if(verts_allocced!=0)
	  { 
	    free(bigX);
	    free(bigY);
	    free(bigZ);
	  } 
						 
	  bigX = (double *)malloc(SzD*(numverts + 1000));
	  bigY = (double *)malloc(SzD*(numverts + 1000));
	  bigZ = (double *)malloc(SzD*(numverts + 1000));
      verts_allocced = numverts + 990;
	}

    for(pointnum=0;pointnum<numverts;pointnum++)
	{ 
      bigX[pointnum] = tempVertX[pointnum];
      bigY[pointnum] = tempVertY[pointnum];
      bigZ[pointnum] = tempVertZ[pointnum];
	  if(bigZ[pointnum]==MY2DSENTINEL)
	  { 
        num2area = num2area + 1;
	  } 
	  else
	  { 
        num3area = num3area + 1;
	  } 
	} 

	
	if(
		(bigX[numverts-1] != tempVertX[0])||
        (bigY[numverts-1] != tempVertY[0])||
        (bigZ[numverts-1] != tempVertZ[0])
	   )
	{
      bigX[numverts] = tempVertX[0];
      bigY[numverts] = tempVertY[0];
      bigZ[numverts] = tempVertZ[0];
	  numverts = numverts + 1;
	}

	return numverts;
  } 




  hr = thiscollection->get_PointCount(&numverts);
  if(!SUCCEEDED(hr))
  {
	sprintf(errstr,"Unable to GetPCSH PG\n");
    FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
  }

  if(numverts<1)
  {
	printf("error:\ngot %d verts on part %d for featureid %d\n\n",numverts,part_num,featureid);
	return 0;
  }


  if(numverts>verts_allocced)
  {
	if(verts_allocced!=0)
	{
	  free(bigX);
	  free(bigY);
	  free(bigZ);
	}
						 
	bigX = (double *)malloc(SzD*(numverts + 1000));
	bigY = (double *)malloc(SzD*(numverts + 1000));
	bigZ = (double *)malloc(SzD*(numverts + 1000));
    verts_allocced = numverts + 990;
  }

  for(pointnum=0;pointnum<numverts;pointnum++)
  {
	IPointPtr thispoint;
	thiscollection->get_Point(pointnum,&thispoint);
    if(!SUCCEEDED(hr))
	{
	  sprintf(errstr,"Unable to GetP2SH PG\n");
      ESRIErrorHandle(2,errstr,hr);
	  FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
	}  

	IZAwarePtr hasZptr = thispoint;

	hr = thispoint->get_X(&bigX[pointnum]);
    if(!SUCCEEDED(hr))
	{
	  sprintf(errstr,"Unable to GetX1SH PG\n");
      ESRIErrorHandle(2,errstr,hr);
	  FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
	}


	hr = thispoint->get_Y(&bigY[pointnum]);
    if(!SUCCEEDED(hr))
	{ 
	  sprintf(errstr,"Unable to GetY1SH PG\n");
      ESRIErrorHandle(2,errstr,hr);
	  FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
	}


	hr = hasZptr->get_ZAware(&hasZ);
    if(!SUCCEEDED(hr))
	{ 
	  sprintf(errstr,"Unable to GetZA1SH PG\n");
      ESRIErrorHandle(2,errstr,hr);
	  FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
	}

    if((hasZ==VARIANT_TRUE)&&(NoZ_Values==0))  
	{
	  hr = thispoint->get_Z(&bigZ[pointnum]);
      if(!SUCCEEDED(hr))
	  { 
	    sprintf(errstr,"Unable to GetZ1SH PG\n");
        ESRIErrorHandle(2,errstr,hr);
	    FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return -1;
	  }
      num3area = num3area + 1;
	} 
	else
	{
	  bigZ[pointnum] = MY2DSENTINEL;
	  num2area = num2area + 1;
	} 
  }
  return numverts;
}


extern "C" void CoUninit()
{
  extern int CO_INNITTED;

  

  if(CO_INNITTED==0)
  {
    return;
  }

printf("un-innitting\n");
  CoUninitialize();
  CO_INNITTED = 0;
printf("done\n");
}




extern "C" int HandleFeatureDatasets(IWorkspacePtr ipWorkspace,int count)
{
   HRESULT hr;
   IDatasetNamePtr datasetName2;
   BSTR shortname;
   IDatasetPtr thisdataset;
   IEnumDatasetPtr f_datasets;
   IEnumDatasetNamePtr ipFeatureDatasetNameEnum;
   IEnumDatasetPtr emb_classes;
   extern char *tables_found[1000];
     
	 
   hr = ipWorkspace->get_DatasetNames(esriDTFeatureDataset,&ipFeatureDatasetNameEnum);

   while(ipFeatureDatasetNameEnum->Next(&datasetName2)==S_OK)
   {
	 datasetName2->get_Name(&shortname);

	 

	 printf("Found Feature Dataset name %ws\n",static_cast<wchar_t *>(shortname));

   }


   ipWorkspace->get_Datasets(esriDTFeatureDataset,&f_datasets);

   while(f_datasets->Next(&thisdataset)==S_OK)
	 {
	   thisdataset->get_Name(&shortname);
	   IDatasetPtr emb_class;

	   IFeatureDatasetPtr thisFdataset = thisdataset;

  	   thisFdataset->get_Subsets(&emb_classes);

	   while(emb_classes->Next(&emb_class)==S_OK)
	   {
		  emb_class->get_Name(&shortname);
		  
          tables_found[count] = (char *)malloc(SysStringLen(shortname)+50);


		  

          sprintf(tables_found[count],"%ws",static_cast<wchar_t *>(shortname));

		  
          count = count + 1;
	   }
	 }
   return count;
}



extern "C" char * ParseFieldType(esriFieldType fieldtype)
{

  if(fieldtype==esriFieldTypeString)
  {
	  return "String";
  }
  else if(fieldtype==esriFieldTypeSingle)
  {
	  return "Single";
  }
  else if(fieldtype==esriFieldTypeDouble)
  {
	  return "Double";
  }
  else if(fieldtype==esriFieldTypeSmallInteger)
  {
	  return "SmallInteger";
  }
  else if(fieldtype==esriFieldTypeInteger)
  {
	  return "Integer";
  }
  else if(fieldtype==esriFieldTypeDate)
  {
	  return "Date";
  }

  return "Unknown!";
}









extern "C" void UpdateTable(int *addtolong, int *addtoshort, char gdfilename[], char tablename[], int numtables)
{
  extern void SH_PrintListChunk(int done, int numtables,char *filename, char *tablename);
  extern void SH_FreeLists(char *sentinel,char *shapename);
  extern void SH_FreeTree();
  extern int scc_short_items;
  extern int scc_long_items;
  char tempfilename[1000];


  *addtolong  = *addtolong  + scc_long_items;
  *addtoshort = *addtoshort + scc_short_items;
  
  SH_PrintListChunk(0,numtables,gdfilename,tablename);
  
  sprintf(tempfilename,"%s: Table %s",gdfilename,tablename);
  SH_FreeLists("ok",tempfilename);
  SH_FreeTree();		  
  
}


unsigned char **AlreadyFound=NULL;
int max_subs = 5;

unsigned char **AlreadyFoundNeg=NULL;
int min_subs = 5;

int AlreadyFoundDomain(int subtypenum,int fieldnum, int flag,ISubtypesPtr ipSubtypes,BSTR b_attr_name,int numfields)
{




  HRESULT hr;
  int i,j,tempnum;

  


  


  if(flag==0)
  {
    

    AlreadyFound = (unsigned char **)malloc(sizeof(unsigned char *)*numfields);

    for(i=0;i<numfields;i++)
	{
      AlreadyFound[i] = (unsigned char *)malloc(sizeof(unsigned char)*max_subs);
      for(j=0;j<max_subs;j++)
	  {
        AlreadyFound[i][j] = 0;
	  }
	}


    AlreadyFoundNeg = (unsigned char **)malloc(sizeof(unsigned char *)*numfields);

    for(i=0;i<numfields;i++)
	{
      AlreadyFoundNeg[i] = (unsigned char *)malloc(sizeof(unsigned char)*min_subs);
      for(j=0;j<min_subs;j++)
	  {
        AlreadyFoundNeg[i][j] = 0;
	  }
	}

	return 999;
  }


  if(flag==2)
  {

    for(i=0;i<numfields;i++)
	{
      free(AlreadyFound[i]);
	}
	free(AlreadyFound);
	AlreadyFound = NULL;


    for(i=0;i<numfields;i++)
	{
      free(AlreadyFoundNeg[i]);
	}
	free(AlreadyFoundNeg);
	AlreadyFoundNeg = NULL;

	return 999;
  }




  if(subtypenum>=max_subs)
  {

    

    for(i=0;i<numfields;i++)
	{
      AlreadyFound[i] = (unsigned char *)realloc(AlreadyFound[i],sizeof(unsigned char)*(subtypenum+5));

      for(j=max_subs;j<(subtypenum+5);j++)
	  { 
        AlreadyFound[i][j] = 0;
	  } 
	}

    max_subs = subtypenum+5;
  }


  if(subtypenum<0)
  {

    

    
	tempnum = subtypenum * -1;

    if(tempnum>=min_subs)
	{

      

      for(i=0;i<numfields;i++)
	  { 
        AlreadyFoundNeg[i] = (unsigned char *)realloc(AlreadyFoundNeg[i],sizeof(unsigned char)*(tempnum+5));

        for(j=min_subs;j<(tempnum+5);j++)
		{  
          AlreadyFoundNeg[i][j] = 0;
		}  
	  } 
      min_subs = tempnum+5;
	}
  }




  if(flag==1)
  {
    IDomainPtr ipDomain;
    BSTR domain_name;
    char thisdomain[500];

	if(subtypenum<0)
	{
      

	  if(AlreadyFoundNeg[fieldnum][tempnum]==1)
	  { 
        return 2;
	  }
	  if(AlreadyFoundNeg[fieldnum][tempnum]==2)
	  {
        return 999;
	  } 

      hr = ipSubtypes->get_Domain(subtypenum,b_attr_name,&ipDomain);


      

      AlreadyFoundNeg[fieldnum][tempnum] = 2; 
	}
	else
	{

      

	  if(AlreadyFound[fieldnum][subtypenum]==1)
	  { 
        return 2;
	  }
	  if(AlreadyFound[fieldnum][subtypenum]==2)
	  {
        return 999;
	  } 

      hr = ipSubtypes->get_Domain(subtypenum,b_attr_name,&ipDomain);


      

      AlreadyFound[fieldnum][subtypenum] = 2; 
	}



    if(ipDomain)
	{ 
      hr = ipDomain->get_Name(&domain_name);

	  sprintf(thisdomain,"%ws",static_cast<wchar_t *>(domain_name));

	  SysFreeString(domain_name);

	  if(strlen(thisdomain)>4)
	  { 
	    if(!strncmp(thisdomain,"Mst_",4))
		{ 
          
		  if(subtypenum<0)
		  {
            AlreadyFoundNeg[fieldnum][tempnum] = 1;
		  }
		  else
		  {
            AlreadyFound[fieldnum][subtypenum] = 1;
		  }
		  return 2;
		} 
	  } 
	} 
  }

  return 999;
}




extern "C" int ReservedAttr(char *attr_name)
{
  extern int StringCaseInsensitiveMatch(char * a, char *b);

  if(StringCaseInsensitiveMatch(attr_name,"fcsubtype"))
  {
    return 1;
  }
  if(StringCaseInsensitiveMatch(attr_name,"shape"))
  {
    return 1;
  }
  if(StringCaseInsensitiveMatch(attr_name,"shape_length"))
  {
    return 1;
  }
  if(StringCaseInsensitiveMatch(attr_name,"shape_area"))
  {
    return 1;
  }

  return 0;
}


extern "C" int OpenGeoDatabase(int AccessOrFile, char *filename, int MakeInputSpec,
							  int addtolong, int addtoshort,
		                      int *longitemsfound, int *shortitemsfound)
{
  HRESULT hr;
  BSTR NameOfFile;
  IWorkspacePtr ipWorkspace = NULL;
  esriLicenseStatus status;
  char chartype[50],errstr[100],tempstr[1000],mdbname[1000];
  int havelicense=0,table_num,indexer,mgcpfcodefound,origaddtolong,origaddtoshort;
  struct sh_long_attr_list  SH_SAC_LONG [SH_LISTLEN];   
  struct sh_short_attr_list SH_SAC_SHORT[SH_LISTLEN]; 
  int sh_sac_short_index=0, sh_sac_long_index=0;

  extern int numtables_to_extract;
  extern char **TablesToExtract;
  extern char **FCODEsOfTables;

  extern int ABORT_EXTRACT;
  extern int CO_INNITTED;
  extern void AddBadClass(char *badcode, char *expectedcode, int FID);
  extern int NOTYPE;
  extern int C_TYPE;
  extern int D_TYPE;
  extern int I_TYPE;

  extern int LONG_ATTR_ID     ;
  extern int LONG_ATTR_ID_INFO;
  extern int LONG_ATTR_AREA   ;
  extern int LONG_ATTR_HGT    ;
  extern int LONG_ATTR_LEN    ;
  extern int LONG_ATTR_NAM    ;
  extern int LONG_ATTR_ZV2    ;
  extern int START_ID         ;
  extern int STOP_ID          ;
  extern int LONG_ATTR_LEN    ;
  extern int LONG_ATTR_WID    ;

  extern char *tables_found[1000];

  extern FILE *badshapeout,*emptyshapes,*mixnull;

  extern int sh_bad_codes_found     ;
  extern int SH_bad_objects         ;
  extern int SH_Grids_Prev          ;
  extern int SH_BadCoords           ;
  extern int SH_Null_Shapes         ;
  extern int SH_Zero_Shapes         ;
  extern int SH_Bad_DBF             ;
  extern unsigned char TIM_STRING   ;
  extern unsigned char TIM_INTEGER  ;
  extern unsigned char TIM_ENUMERANT;
  extern unsigned char TIM_DOUBLE   ;
  extern unsigned char TIM_UNI      ;
  extern unsigned char TIM_METRE    ;
  extern unsigned char TIM_UNITLESS ;
  extern int SzI, SzD, SzL, SzUC;
  extern int global_gifd_id;
  extern int num2point;
  extern int num3point;
  extern int num2line;
  extern int num3line;
  extern int num2area;
  extern int num3area;

  extern int DO_TXT;
  extern int CLIPEXTRACT;
  extern double CLIPLLX;
  extern double CLIPLLY;
  extern double CLIPURX;
  extern double CLIPURY;
  extern int SH_Points_Done  ;
  extern int SH_Linears_Done ;
  extern int SH_Areals_Done  ;
  extern int SH_Areals_Prev  ;
  extern int NumPointParts   ;
  extern int NumLineParts    ;
  extern int NumAreaParts    ; 
  extern int NumMultiPartPointFeatures;
  extern int NumMultiPartLineFeatures ;
  extern int NumMultiPartAreaFeatures ;
  extern int EAREALNODELIMIT;
  extern int ELINEARNODELIMIT;
  extern int NumHoles        ;


  extern unsigned long int gridpointsread;

  extern void sh_testxyglobal(double x,double y,double z);

  extern int SH_FindSCC_LONG(int type, int scc, int numsac, 
			   struct sh_long_attr_list SAC_LONG[], int record_num);
  extern int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			    struct sh_short_attr_list SAC_SHORT[], int record_num);
  extern void update_shape_import(char *allshapes,char *curr_file,int files_done, 
				int dbfdone,int dbftotal,
				int points_done,int linears_done,int areals_done,unsigned long int gptsdone,
				int DONE, int extra_type,int extra1, int extra2, 
				int badstuff, int badstuff2, int nullshapes, int zeropart, int baddbf);

  extern void SH_AddShortAttribute(struct sh_short_attr_list SAC_SHORT[],int addindex,int attr_code);

  extern int SH_AddLongAttribute(struct sh_long_attr_list SAC_LONG[],int addindex,int attr_code, 
			       unsigned char unit_code, unsigned char scale_code, unsigned char SEDRIStype, 
			       int Ivalue, double Dvalue, char Cvalue[]);
  extern void ReSetArealFile();
  extern int ConvertAttrCode(char *oldcode);
  extern int PossibleFCODE(char possiblecode[]);
  extern int LongCode(char possiblecode[]);
  extern char timtoupper(char inchar);
  extern int ConvertClassCode(char *oldcode, int FID);
  extern int IsGFID(char possiblecode[]);
  extern int PossibleOBJECTID(char possiblecode[]);
  extern void AddBadAttr(int badattr, char *ENUM, int record_num);
  extern int GetNewEnum(int attr_code,char *oldenum, int report_if_bad, int record_num);
  extern void AddBadEnumType(int badattr, char *filename, char *ENUM, char *goodenum);
  extern int SetClassLevel(char columnname[],char columnvalue[],char filename[],char tablename[]);
  extern void AddUnkAttr(char *badcode);
  extern void AddBadType(int badattr, char *expect_type, char *got_type);
  extern void WriteGFID(int datafound,char *charval, int Ival, double Dval,
			   int datatype, int attr_type);
  extern int IsArea(char *tester);
  extern int IsHgt (char *tester);
  extern int IsZv2 (char *tester);
  extern int IsNam (char *tester);
  extern int IsLen (char *tester);
  extern void HandleDoublePickListVal(int attr_code, double thisrealval,int *GEOMTYPE,
							 unsigned char *SEDRIStype, int *retenum, int record_num);
  extern void AddNullShape(int FID, int type, int numverts, int part_num);
  extern void GetShortName(char shortname[],char *shape_to_import);
  extern int SH_PointInAreal(double px,double py,double * x,double * y,
			     int startindex, int stopindex);
  extern void AddBadHole(int record_num, int part_num, char *shapename, int type);
  extern void AddNoFCODE(char *filename1);
  extern void AddBlankAttr(char filename[],char attribute[],int record_num,int datatype);
  extern int GetOptional(char *fcode,char *attrcode,int geom);
  extern int EvalValue(int oldval, char *shape_to_import,char attr_name[],int thisintval,double thisrealval,
	       char *thisstrval,int type);
  extern void TestWidth(int fieldtype,char attrcode[], int width);
  extern char *GetNFI();
  extern char *GetNFN();
  extern int TestGeodetic(double x, double y,int FID,char *shape_to_import);


  if(AccessOrFile==1)
  {
    sprintf(chartype,"personal");
  }
  else
  {
    sprintf(chartype,"file");
  }


  if(MakeInputSpec==1)
  {
    printf("Trying to open suspected %s geodatabase %s\n",chartype,filename);
  }
  else
  {
    printf("Import %s geodatabase %s\n",chartype,filename);
  }


  CString junk=filename;
  NameOfFile = junk.AllocSysString();

  if(CO_INNITTED==0)
    {
      hr = CoInitialize(NULL);
      if(!SUCCEEDED(hr))
	  { 
	    printf("CoInit failed\n");
	    if(hr==RPC_E_CHANGED_MODE)
		{
		 printf("...case 3\n");
		}
	    else
		{ 
		  printf("...case 4: %d %x\n",hr,hr);
		} 
	  } 
      CO_INNITTED = 1;
    }

  Init_Outpoint(1);

  DoVersionStuff();


  origaddtolong     = addtolong;
  origaddtoshort    = addtoshort;


  


  


  IAoInitializePtr ipInit = NULL;
  
  hr = ipInit.CreateInstance(CLSID_AoInitialize);




  if(!SUCCEEDED(hr))
  {
    ESRIErrorHandle(2,"Unable to start ArcObjects",hr);
	return 0;
  }


  

  

  havelicense = 0;

  status = esriLicenseFailure;
  ipInit->IsProductCodeAvailable(esriLicenseProductCodeArcView,&status);
  if(status==esriLicenseAvailable)
  {
    printf("ArcView license available.  Checking it out...\n");
    ipInit->Initialize(esriLicenseProductCodeArcView,&status);
    if(status==esriLicenseCheckedOut)
	{
	  printf("checked out license for ArcView\n");
	  havelicense = 1;
	}
    else if(status==esriLicenseAlreadyInitialized)
	{
	  printf("ArcView license already initialized\n");
	  havelicense = 1;
	}
  }


  if(havelicense==0)
  {
    status = esriLicenseFailure;
    ipInit->IsProductCodeAvailable(esriLicenseProductCodeArcEditor,&status);
    if(status==esriLicenseAvailable)
	{
      printf("ArcEditor license available.  Checking it out...\n");
      ipInit->Initialize(esriLicenseProductCodeArcEditor,&status);
      if(status==esriLicenseCheckedOut)
	  {
	    printf("checked out license for ArcEditor\n");
	    havelicense = 1;
	  }
      else if(status==esriLicenseAlreadyInitialized)
	  { 
	    printf("ArcEditor license already initialized\n");
	    havelicense = 1;
	  }
	}
  }

  if(havelicense==0)
  {
    status = esriLicenseFailure;
    ipInit->IsProductCodeAvailable(esriLicenseProductCodeArcInfo,&status);
    if(status==esriLicenseAvailable)
	{
      printf("ArcInfo license available.  Checking it out...\n");
      ipInit->Initialize(esriLicenseProductCodeArcInfo,&status);
      if(status==esriLicenseCheckedOut)
	  {
	    printf("checked out license for ArcInfo\n");
	    havelicense = 1;
	  }
      else if(status==esriLicenseAlreadyInitialized)
	  {
	    printf("ArcInfo license already initialized\n");
	    havelicense = 1;
	  }
	}
  }



  if(havelicense==0)
  {
	printf("No available ESRI license.  Cannot import %s\n",filename);
    printf("Shutting down ArcObjects\n");
    hr = ipInit->Shutdown();
    if(!SUCCEEDED(hr))
	{
      ESRIErrorHandle(2,"Shutting down ArcObjects",hr);
	} 
    return 0;
  }



  if(AccessOrFile==1)
  {
	

    IWorkspaceFactoryPtr ipAccessWorkspaceFactory(CLSID_AccessWorkspaceFactory);

	{

    hr = ipAccessWorkspaceFactory->OpenFromFile(NameOfFile, 0, &ipWorkspace);

    if(!SUCCEEDED(hr))
	{
      ESRIErrorHandle(1,filename,hr);
      printf("Shutting down ArcObjects\n");
      hr = ipInit->Shutdown();
      if(!SUCCEEDED(hr))
	  {
        ESRIErrorHandle(2,"Shutting down ArcObjects",hr);
	  }  
      return 0;
	}
	printf("Opened personal geodatabase %s\n",filename);
	}
  }
 else if(AccessOrFile==0)
  {
	
    IWorkspaceFactoryPtr ipAccessWorkspaceFactory(CLSID_FileGDBWorkspaceFactory);

    hr = ipAccessWorkspaceFactory->OpenFromFile(NameOfFile, 0, &ipWorkspace);

    if(!SUCCEEDED(hr))
	{
      ESRIErrorHandle(1,filename,hr);
      printf("Shutting down ArcObjects\n");
      hr = ipInit->Shutdown();
      if(!SUCCEEDED(hr))
	  {
        ESRIErrorHandle(2,"Shutting down ArcObjects",hr);
	  }  
      return 0;
	}
	printf("Opened file geodatabase %s\n",filename);
 }

  



  if(MakeInputSpec==1)
   {
     int count = 0;
     extern char *tables_found[1000];
 
     IEnumDatasetNamePtr ipFeatureDatasetNameEnum;
     IEnumDatasetNamePtr ipEmbeddedDatasetNameEnum;
     IEnumDatasetNamePtr ipDatasetNameEnum;
     IDatasetNamePtr datasetName;
     

	 

	 count = HandleFeatureDatasets(ipWorkspace,count);


    


    hr = ipWorkspace->get_DatasetNames(esriDTFeatureClass,&ipDatasetNameEnum);

     if(!SUCCEEDED(hr))
	 {
       ESRIErrorHandle(2,"unable to get_DatasetNames",hr);
       hr = ipInit->Shutdown();    
	   if(!SUCCEEDED(hr)) 
	   {
		 ESRIErrorHandle(2,"Shutting down ArcObjects",hr);
	   }
	   return 0;
	 }

     while(ipDatasetNameEnum->Next(&datasetName)==S_OK)
	 {       
	   BSTR thename;

       hr = datasetName->get_Name(&thename);

	   if(!SUCCEEDED(hr))
	   { 
         ESRIErrorHandle(2,"unable to get_Name for a feature class.  ignoring it.",hr);
	   }
	   else
	   { 
         tables_found[count] = (char *)malloc(SysStringLen(thename)+50);

		 

         sprintf(tables_found[count],"%ws",static_cast<wchar_t *>(thename));

         count = count + 1;
	   } 

	   SysFreeString(thename);
       
	 }

     printf("Shutting down ArcObjects\n");
     hr = ipInit->Shutdown();
     if(!SUCCEEDED(hr))
	 { 
       ESRIErrorHandle(2,"Shutting down ArcObjects",hr);
	 }  
  
    return count;
  } 
  else
  {

    


	for(indexer=0;indexer<SH_LISTLEN;indexer++)
	{ 
      SH_SAC_LONG[indexer].nam_stuff = NULL;
      SH_SAC_LONG[indexer].lod_stuff = NULL;

      SH_SAC_SHORT[indexer].nam_stuff = NULL;
      SH_SAC_SHORT[indexer].lod_stuff = NULL;

      SH_SAC_LONG[indexer].Cvalue = NULL;
	}



    for(table_num=0; table_num < numtables_to_extract; table_num++)
    {
	  

	  IFeatureWorkspacePtr ipFeatureWorkspace = ipWorkspace;
	  IFieldsPtr ipFields = NULL;
	  IFieldPtr ipField = NULL;
      esriGeometryType ftype;
	  esriFieldType fieldtype;
	  IFeatureClassPtr ipFeatureclass = NULL;
      BSTR thisname;
	  CString junk2=TablesToExtract[table_num];
	  char ReportName[1000];
	  char **attr_names; 
	  int i,fieldnum=0,tempcode,class_code,foundclass=-1,badfound,possiblefcode,skiptable,lastindex=0;
      int   *attr_types,*attr_codes,*optional_attribute,SEEITGEOM;
 	  long numFields,fieldlen;
      extern char **gifd_attr_types;
      extern char **gifd_attr_codes;
      extern int IsSubtype(char *attr_name);


      printf("  Process: table (%d/%d) %s fcode %s\n",
		  table_num+1,numtables_to_extract,
		  TablesToExtract[table_num],FCODEsOfTables[table_num]);

	  

	  numFields      = 0;
	  class_code     = -100;
	  possiblefcode  = 0;
	  skiptable      = 0;
      mgcpfcodefound = 0;


      sprintf(ReportName,"%s: Table %s",filename,TablesToExtract[table_num]);


	  if(strcmp(FCODEsOfTables[table_num],"*****"))
	  {
	    class_code  = ConvertClassCode(FCODEsOfTables[table_num],-999);
	  }


      thisname = junk2.AllocSysString();

      hr = ipFeatureWorkspace->OpenFeatureClass(thisname,&ipFeatureclass);
      if(!SUCCEEDED(hr))
	  {
        printf("could not open %S (%s)\n",TablesToExtract[table_num],TablesToExtract[table_num]);
		fprintf(emptyshapes,"Invalid Geodatabase Table %s;Table %s\n",filename,TablesToExtract[table_num]);
		continue; 
	  }




	  SysFreeString(thisname);

      hr = ipFeatureclass->get_ShapeType(&ftype);
      if(!SUCCEEDED(hr))
	  {
        printf("could not get ST for %s\n",TablesToExtract[table_num]);
	  }


	  if(ftype==esriGeometryPoint)
	  {
		

		SEEITGEOM = 5;
	  }
	  else if(ftype==esriGeometryPolyline)
	  {
		
		SEEITGEOM = 4;
	  }
	  else if(ftype==esriGeometryPolygon)
	  {
		
		SEEITGEOM = 1;
	  }
	  else
	  {
		printf("\n\n    Feature class %s has unrecognized geom type %d\n",TablesToExtract[table_num],ftype);
		SEEITGEOM = -1;
	  }


	  hr = ipFeatureclass->get_Fields(&ipFields);
      if(!SUCCEEDED(hr))
	  {
        printf("error: could not get fields from %s\n",TablesToExtract[table_num]);
	  }
	  ipFields->get_FieldCount(&numFields);


	  i = AlreadyFoundDomain(numFields,-1,0,NULL,NULL,numFields);


	  

	  attr_codes         = (int *)  malloc(sizeof(int)   *(numFields+10));
      attr_types         = (int *)  malloc(sizeof(int)   *(numFields+10));
      attr_names         = (char **)malloc(sizeof(char *)*(numFields+10));

	  if(ATTRTYPE_ANY_MGCP())
	  {
        optional_attribute = (int *)  malloc(sizeof(int)   *(numFields+10));
	  }


	  for(i=0;i<numFields;i++)
	  {
        attr_codes[i] = -1;
	  }




VARIANT_BOOL isSubtyped=VARIANT_FALSE;
ISubtypesPtr ipSubtypes = ipFeatureclass;
BSTR fieldname;
long int fieldindex;
if(ipSubtypes!=NULL)
{
  hr = ipSubtypes->get_HasSubtype(&isSubtyped);
  hr = ipSubtypes->get_SubtypeFieldName(&fieldname);
  hr = ipSubtypes->get_SubtypeFieldIndex(&fieldindex);
}



	  for(fieldnum=0;fieldnum<numFields;fieldnum++)
	  {
		BSTR fieldname;
		
	    hr = ipFields->get_Field(fieldnum,&ipField);

        if(!SUCCEEDED(hr))
		{
		  sprintf(errstr,"Unable to GetField %d\n",fieldnum);
          ESRIErrorHandle(2,errstr,hr);
	      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
		} 
		
		hr = ipField->get_Name(&fieldname);
        if(!SUCCEEDED(hr))
		{
		  sprintf(errstr,"Unable to GetName %d\n",fieldnum);
          ESRIErrorHandle(2,errstr,hr);
	      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
		} 


		ipField->get_Type(&fieldtype);
        if(!SUCCEEDED(hr))
		{
		  sprintf(errstr,"Unable to GetType %d\n",fieldnum);
          ESRIErrorHandle(2,errstr,hr);
	      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
		} 

		if(ATTRTYPE_ANY_MGCP())
		{
		  ipField->get_Length(&fieldlen);
          if(!SUCCEEDED(hr))
		  {
		    sprintf(errstr,"Unable to GetLength %d\n",fieldnum);
            ESRIErrorHandle(2,errstr,hr);
	        FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
		  }  
		}

		attr_names[fieldnum] = (char *)malloc(SysStringLen(fieldname)+10);


		

		sprintf(attr_names[fieldnum],"%ws",static_cast<wchar_t *>(fieldname));

		


		attr_types[fieldnum] = fieldtype;

		if(attr_codes[fieldnum]==-2)
		{
          tempcode = -2;
		}
		else
		{
		  tempcode = ConvertAttrCode(attr_names[fieldnum]);
		}


		if(tempcode>=0)
		{
          
		 
		  badfound = 0;

		  


		  

		  if(!strcmp(gifd_attr_types[tempcode],"String"))
			{
			  if(fieldtype!=esriFieldTypeString)
			  {
			    AddBadType(tempcode,gifd_attr_types[tempcode],ParseFieldType(fieldtype));
			    badfound = 1;
			  }
		    }
		  else if(
			 (!strcmp(gifd_attr_types[tempcode],"Double")) ||
			 (!strcmp(gifd_attr_types[tempcode],"Real"))   ||
			 (!strcmp(gifd_attr_types[tempcode],"Float"))
			 )
			{
  			  if(
				  (fieldtype!=esriFieldTypeSingle) &&
				  (fieldtype!=esriFieldTypeDouble)
				  )
			  {
			    AddBadType(tempcode,gifd_attr_types[tempcode],ParseFieldType(fieldtype));
			    badfound = 1;
			  }
			}
		  else if(
			 (!strcmp( gifd_attr_types[tempcode],"Short Integer")) ||
			 (!strcmp( gifd_attr_types[tempcode],"Integer"))       ||
			 (!strcmp( gifd_attr_types[tempcode],"ShortInteger"))  ||
			 (!strcmp( gifd_attr_types[tempcode],"Long Integer"))  ||
			 (!strcmp( gifd_attr_types[tempcode],"LongInteger")) 
			 )
			{
   			  if(
				  (fieldtype!=esriFieldTypeSmallInteger) &&
				  (fieldtype!=esriFieldTypeInteger)
				  )
			  {
			    AddBadType(tempcode,gifd_attr_types[tempcode],ParseFieldType(fieldtype));
			    badfound = 1;
			  } 
		    }
		  else if(!strcmp( gifd_attr_types[tempcode],"Date"))
			{
   			  if(
				  (fieldtype!=esriFieldTypeDate) &&
				  (fieldtype!=esriFieldTypeString)
				  )
			  {
			    AddBadType(tempcode,gifd_attr_types[tempcode],ParseFieldType(fieldtype));
			    badfound = 1;
			  } 
		    }

		  if(badfound==0)
		  {
		    if(ATTRTYPE_ANY_MGCP())
			{
		      if(badfound==0)
			  {
				if(fieldtype==esriFieldTypeString)
				{
			      TestWidth(1,attr_names[fieldnum],fieldlen);
				}

				
			  } 
		    }
		  }
		}


        if(PossibleFCODE(attr_names[fieldnum]))
		{
		  tempcode = -999; 

		  possiblefcode = 1;

		  
		}
	      if(IsGFID(attr_names[fieldnum]))
		{
		  tempcode = -998; 
		  
		}
	      if(PossibleOBJECTID(attr_names[fieldnum]))
		{
		  tempcode = -997; 

          
		}
		  if(IsSubtype(attr_names[fieldnum]))
		  {
			tempcode = -996;
		  }


        if(
			(!strcmp(attr_names[fieldnum],"SEC_CLASS")) ||
			(!strcmp(attr_names[fieldnum],"ZSAX_RS0"))  ||
			(!strcmp(attr_names[fieldnum],"SEC"))
		  )
		{
		  foundclass = fieldnum;
		}


	    if(!strcmp(attr_names[fieldnum],"FCODE"))
	    {
	      mgcpfcodefound = 1;
	    }
	  
		attr_codes[fieldnum] = tempcode;



		if(ATTRTYPE_ANY_MGCP())
		{
	      if((attr_codes[fieldnum]>=0)&&(strcmp(FCODEsOfTables[table_num],"*****"))&&(SEEITGEOM>0))
		  {
	        optional_attribute[fieldnum] = GetOptional(FCODEsOfTables[table_num],attr_names[fieldnum],SEEITGEOM);
		  }
	      else
		  {
	        optional_attribute[fieldnum] = -100;
		  }

		  
		}

		SysFreeString(fieldname);

	  }  

  	  
	  
  
  
  if(mgcpfcodefound==0)
    {
      AddNoFCODE(ReportName);
    }





	  if( (possiblefcode==0)&&  (!strcmp(FCODEsOfTables[table_num],"*****")))
	  {
        
		skiptable = 1;
	  }


	  

	  IFeatureCursorPtr ipFeatureCursor = NULL;
	  HRESULT hr = ipFeatureclass->Search(NULL, VARIANT_TRUE, &ipFeatureCursor);
      if(!SUCCEEDED(hr))
	  {
        ESRIErrorHandle(2,"trying to create feature cursor",hr);
	  }  


 
	  VARIANT cvarValue;
	  VARIANT_BOOL emptyVar,hasZ;
      unsigned char SEDRIStype;
	  char Cvalue[5000],OldCvalue[5000],enumvalue[1000],scrubenumvalue[1000],layername[1000],thisGFcode[1000],
		  SaveAreaC[1000],SaveHgtC[1000],SaveZv2C[1000],SaveLenC[1000],SaveNamC[1000],
		  saveNFI[1000],saveNFN[1000],saveNAM[1000],sav996[500],sav999[500];
	  int indexer,GEOMTYPE,gifd_found,area_found,hgt_found,zv2_found,featureid,oldvalset,OldIvalue,
		  newenum,j,k,other_class_code,SaveAreaI,SaveHgtI,SaveZv2I,SaveLenI,SaveNamI,firstbad,
		  longcode,day,month,year,nullfeature,len_found,nam_found,len,zz,madeachange;
	  double xval,yval,zval,OldDvalue,SaveAreaD,SaveHgtD,SaveZv2D,SaveLenD,SaveNamD;
      extern int *gifd_attr_enum_count;
 
	  IGeometryPtr ipGeometry = NULL;
	  IFeaturePtr ipFeature = NULL;
	  hr = ipFeatureCursor->NextFeature(&ipFeature);
      if(!SUCCEEDED(hr))
	  {
        ESRIErrorHandle(2,"trying to get first feature",hr);
	  }  



	  if(ipFeature==NULL)
	  {
		printf("    This feature class is empty\n");
		fprintf(emptyshapes,"Empty Geodatabase Table %s;Table %s\n",filename,TablesToExtract[table_num]);
	  }


	  while(ipFeature)
	  {
         


		  


          SH_SAC_LONG[sh_sac_long_index].lod_stuff = (char *)malloc(strlen(TablesToExtract[table_num])+200); 
          if(SH_SAC_LONG[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}
      
          SH_SAC_SHORT[sh_sac_short_index].lod_stuff = (char *)malloc(strlen(TablesToExtract[table_num])+200); 
          if(SH_SAC_SHORT[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(-1);}
      
          sprintf(SH_SAC_LONG [sh_sac_long_index ].lod_stuff,"%s",TablesToExtract[table_num]);
          sprintf(SH_SAC_SHORT[sh_sac_short_index].lod_stuff,"%s",TablesToExtract[table_num]);
      
          SH_SAC_LONG [sh_sac_long_index ].attr_code = -1;
          SH_SAC_SHORT[sh_sac_short_index].attr_code = -1;
      
          sh_sac_short_index++;
          sh_sac_long_index++;

          gifd_found = 0;
	      area_found = NOTYPE;
 	      hgt_found  = NOTYPE;
 	      len_found  = NOTYPE;
 	      nam_found  = NOTYPE;
 	      zv2_found  = NOTYPE;
          featureid  = -1;

		  saveNFI[0] = '\0';
		  saveNFN[0] = '\0';
		  saveNAM[0] = '\0';

	      sprintf(sav996,"ABCPRT");
		  sprintf(sav999,"AAAABBBBRRRRFFFFTTTT");
		  firstbad = 0;


	      for(fieldnum=0;fieldnum<numFields;fieldnum++)
		  {

             

if(isSubtyped==VARIANT_TRUE)  
  {
    VARIANT var_val;
	int subtypeval;
	CString junk(attr_names[fieldnum]);
	BSTR b_attr_name = junk.AllocSysString();

    


	

	ipFeature->get_Value(fieldindex,&var_val);

	subtypeval = -9999;
	if((var_val.vt==VT_I2)||(var_val.vt==VT_UI2))
	{
      subtypeval = var_val.iVal;
	}
	else if((var_val.vt==VT_I4)||(var_val.vt==VT_UI4))
	{
      subtypeval = var_val.lVal;
	}
	else
	{
      printf("***** Error: could not determine data type for %d (%d %d %d %d)!\n",
		  var_val.vt,VT_I2,VT_UI2,VT_I4,VT_UI4);
	  continue;
	}





	if(subtypeval==-9999)
	{
      SysFreeString(b_attr_name);
	}
	else
	{
	  if(AlreadyFoundDomain(subtypeval,fieldnum,1,ipSubtypes,b_attr_name,numFields)==2)
	  {
        SysFreeString(b_attr_name);
        continue;
	  }
      SysFreeString(b_attr_name);
	}
}



             GEOMTYPE = -999;
             Cvalue[0] = '\0';
             OldCvalue[0] = '\0';
			 longcode = 0;





			 

			 if(skiptable==1)
			 {
                

			   if(foundclass!=fieldnum)
			   {
				 
				   
			     continue;
			   }
			 }

	         if(attr_codes[fieldnum]==-100)
			 {
			   if(ReservedAttr(attr_names[fieldnum])==0)
			   {
	             AddUnkAttr(attr_names[fieldnum]);
			   }

			   if(foundclass!=fieldnum)
			   {
				 
			     continue;
			   }
			 }




			 

			 VariantInit(&cvarValue);

			 

			 

			 hr = ipFeature->get_Value(fieldnum,&cvarValue);
             if(!SUCCEEDED(hr))
			 {
		       sprintf(errstr,"Unable to GetValue %d\n",fieldnum);
               ESRIErrorHandle(2,errstr,hr);
	           FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
			 }

			 oldvalset = 0;



			 switch(cvarValue.vt)
			 {

		       case VT_NULL:
		       GEOMTYPE = -999;

			   
			   if(ATTRTYPE_ANY_MGCP())
			   {
			     if(
				   (attr_types[fieldnum]==esriFieldTypeSingle) ||
				   (attr_types[fieldnum]==esriFieldTypeDouble)
				   )
				 {
			       AddBlankAttr(ReportName,attr_names[fieldnum],featureid,1); 
				 }
			     if(
				    (attr_types[fieldnum]==esriFieldTypeSmallInteger) ||
				    (attr_types[fieldnum]==esriFieldTypeInteger)
				   )
				 {
			       AddBlankAttr(ReportName,attr_names[fieldnum],featureid,0); 
				 }
			   }
				   
		       break;

		       case VT_I2:
		       case VT_UI2:
		       OldIvalue = cvarValue.iVal;
		       oldvalset = 1;
		       case VT_I4:
		       case VT_UI4:
		       if(oldvalset==0)
			   {
			     OldIvalue = cvarValue.lVal;
			   }

			   if(attr_codes[fieldnum]<0)
			   {
			     GEOMTYPE = 0; 
			     SEDRIStype = TIM_INTEGER;
			   }
		       else if(gifd_attr_enum_count[attr_codes[fieldnum]]==0)
			   {
			     
			     GEOMTYPE = 0; 
			     SEDRIStype = TIM_INTEGER;
			   }
		      else
			  { 
			    enumvalue[0] = '\0';
			    sprintf(enumvalue,"%d",OldIvalue);
			  
			    newenum = GetNewEnum(attr_codes[fieldnum],
					       enumvalue,
					       1,
					       featureid);  
			  
			    if(newenum>=0)
			    {
			      
			      GEOMTYPE = 3;
			      SEDRIStype = TIM_ENUMERANT;
			    }
			    else
			    {
			      GEOMTYPE = -999;
			    }
			  } 

		      break;


		    case VT_R4:

				OldDvalue  = (double)cvarValue.fltVal;

			    if(attr_codes[fieldnum]<0)
				{
		          GEOMTYPE   = 1;  
		          SEDRIStype = TIM_DOUBLE;
				}
			    else if(gifd_attr_enum_count[attr_codes[fieldnum]]>0)
				{
				  HandleDoublePickListVal(attr_codes[fieldnum],OldDvalue,&GEOMTYPE,&SEDRIStype,&newenum,featureid+1);
				}
			    else
				{
		          GEOMTYPE   = 1;  
		          SEDRIStype = TIM_DOUBLE;
				}
		      break;
		    case VT_R8:

		      OldDvalue  = (double)cvarValue.dblVal;

		      if(attr_codes[fieldnum]<0)
			  {
		        GEOMTYPE   = 1;  
		        SEDRIStype = TIM_DOUBLE;
			  }
			  else if(gifd_attr_enum_count[attr_codes[fieldnum]]>0)
			  { 
			 	HandleDoublePickListVal(attr_codes[fieldnum],OldDvalue,&GEOMTYPE,&SEDRIStype,&newenum,featureid+1);
			  }
			  else
			  {
		        GEOMTYPE   = 1;  
		        SEDRIStype = TIM_DOUBLE;
			  }
		      break;
			case VT_DATE:

			  GetDateComponents((int)cvarValue.date,&day,&month,&year);

	          sprintf(OldCvalue,"\"%04d%02d%02d\"",year,month,day);
			   
			  GEOMTYPE = 2;
			  SEDRIStype = TIM_STRING;
	          break;

		    case VT_BSTR:
		      		    
			  


			  if( (cvarValue.bstrVal[0]=='\0')   || ((cvarValue.bstrVal[0]==' ') && (cvarValue.bstrVal[1]=='\0'))  )
			  { 

                

				if( (attr_codes[fieldnum]==-998)||(attr_codes[fieldnum]==-999)||(LongCode(attr_names[fieldnum])) )
				{
                  sprintf(OldCvalue,"NULL string found");
				}
				else
				{
                  sprintf(OldCvalue,"\"NULL string found\"");
				}
			    GEOMTYPE = 2;
			    SEDRIStype = TIM_STRING;

			    if(ATTRTYPE_ANY_MGCP())
			    {
			      AddBlankAttr(ReportName,attr_names[fieldnum],featureid,2); 
			    }
			  }
		      else if(attr_codes[fieldnum]<0)
			  {
				if( (attr_codes[fieldnum]==-998)||(attr_codes[fieldnum]==-999)||(LongCode(attr_names[fieldnum])) )
				{
		 	      sprintf(OldCvalue,"%S",cvarValue.bstrVal);
				}
				else
				{
		 	      sprintf(OldCvalue,"\"%S\"",cvarValue.bstrVal);
				}
			    GEOMTYPE = 2;
			    SEDRIStype = TIM_STRING;
			  }
		      else if(gifd_attr_enum_count[attr_codes[fieldnum]]==0)
			  {

				 if(
					 (!strcmp(attr_names[fieldnum],"ZI005_FNA1")) ||  
					 (!strcmp(attr_names[fieldnum],"ADR"))        ||  
					 (!strcmp(attr_names[fieldnum],"ZI006_MEM"))  ||  
					 (!strcmp(attr_names[fieldnum],"FUFI"))           
				   )
				 {
                   sprintf(OldCvalue,"\"Value not imported\"");
			       GEOMTYPE = 2;
			       SEDRIStype = TIM_STRING;
				 }
				 else if(1)
				 { 
                   int kk,insertindex;
                   char thisstr[1000];

                   insertindex = 0;
                   for(kk=0;kk<300;kk++)
				   { 
                     if(cvarValue.bstrVal[kk]=='\0')
					 { 
	                   break;
					 } 

                     if((cvarValue.bstrVal[kk]>=0)&&(cvarValue.bstrVal[kk]<256))
					 { 
                       thisstr[insertindex] = (unsigned char)cvarValue.bstrVal[kk];
	                   insertindex = insertindex + 1;
                       thisstr[insertindex] = '\0';
					 }
					 else  
					 {

					   

                       WideCharToMultiByte(CP_UTF8,0,cvarValue.bstrVal,-1,thisstr,1000,NULL,NULL);

					   insertindex = strlen(thisstr);
					   break;

                       
					 }
				   } 

				   if(insertindex>0)
				   {
                     
					 if(LongCode(attr_names[fieldnum]))
					 {
				       sprintf(OldCvalue,"%s",thisstr);
					 }
					 else
					 {
				       sprintf(OldCvalue,"\"%s\"",thisstr);

			           if(DoNFI())
					   {  
                         if(!strcmp(attr_names[fieldnum],GetNFI()))
						 {    
                           sprintf(saveNFI,"\"%s\"",thisstr);
						 }    
                         if(!strcmp(attr_names[fieldnum],GetNFN()))
						 {    
                           sprintf(saveNFN,"\"%s\"",thisstr);
						 }   
					   }  
					 }

                     
				   }
				   else
				   {
                     sprintf(OldCvalue,"\"Failed to read\"");
				   }
				 } 

		 	    
			    GEOMTYPE = 2;
			    SEDRIStype = TIM_STRING;
			  }
		      else
			{
		 	    sprintf(OldCvalue,"%S",cvarValue.bstrVal);

			    
			  
			  
			  enumvalue[0] = '\0'; 
			  sprintf(enumvalue,"%s",OldCvalue);
			  
			  if(enumvalue[0]=='\0')
			    {
			      
			      GEOMTYPE = -999;
			      
			      
			      AddBadAttr(attr_codes[fieldnum],enumvalue,featureid);  
			    }
			  else 
			    {
			      newenum = GetNewEnum(attr_codes[fieldnum],
						   enumvalue,
						   0,
						   featureid);  
			      
			      if(newenum>=0)
				{
				  
				  GEOMTYPE = 3;
				  SEDRIStype = TIM_ENUMERANT;
				  
				}
			      else
				{
				  
				  
				  sprintf(scrubenumvalue,"%s",enumvalue);
				  
				  for(j=0;j<(int)strlen(scrubenumvalue);j++)
				    {
				      if(scrubenumvalue[j]!='0')
					{
					  break;
					}
				    }
				  
				  
				  if(j<(int)strlen(scrubenumvalue))
				    {
				      for(k=j; k<(int)strlen(scrubenumvalue);k++)
					{
					  enumvalue[k-j] = scrubenumvalue[k];
					}
				      enumvalue[k-j] = '\0';
				      
				      newenum = GetNewEnum(attr_codes[fieldnum],
							   enumvalue,
							   1,
							   featureid);			      
				      
				      if(newenum>=0)
					{
					  
					  GEOMTYPE = 3;
					  SEDRIStype = TIM_ENUMERANT;
					  
				  	  sprintf(layername,"%s (Table %s)",filename,TablesToExtract[table_num]);

					  AddBadEnumType(attr_codes[fieldnum],layername,scrubenumvalue,enumvalue);
					  
					  
					  
					}
				      else
					{
					  
					  GEOMTYPE = -999;
					}
				    }
				  else
				  {				      
				    AddBadAttr(attr_codes[fieldnum],scrubenumvalue,featureid);
				    GEOMTYPE = -999;
				  }
				} 
			  }  
			} 

		      break;
		    case VT_ERROR:
		      printf("ERROR type\n");
		      break;
		    case 8209:
		      break;
				
		    default:
		      printf("unknown v type %d for %s\n",cvarValue.vt,attr_names[fieldnum]);
		      break;
			} 


			 hr = VariantClear(&cvarValue);

             if(!SUCCEEDED(hr))
			 {
		       sprintf(errstr,"Unable to cClear\n");
               ESRIErrorHandle(2,errstr,hr);
	           FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
			 }



			 

			 if((GEOMTYPE==2)||(GEOMTYPE==3))
			 {
		      len = strlen(OldCvalue);

			  madeachange = 0;

		      for(zz=0;zz<len;zz++)
			  { 
			    if ((OldCvalue[zz]<32)&&(OldCvalue[zz]>=0))
				{
			      
				  if(OldCvalue[zz]==10)
				  {
		            printf("Replacing newline character (%d/%d) in %s = %s with !:\n",zz+1,OldCvalue[zz],attr_names[fieldnum],OldCvalue);
				  }
				  else if(OldCvalue[zz]==13)
				  {
		            printf("Replacing carriage return character (%d/%d) in %s = %s with !:\n",zz+1,OldCvalue[zz],attr_names[fieldnum],OldCvalue);
				  }
				  else
				  {
		            printf("Replacing non-printing character (%d/%d) in %s = %s with !:\n",zz+1,OldCvalue[zz],attr_names[fieldnum],OldCvalue);
				  }

			      OldCvalue[zz] = '!';
				  madeachange = 1;
			    }
			  } 

			  if(madeachange>0)
			  {
				printf(" new value: %s\n",OldCvalue);
			  }		     
			 }


			if(foundclass==fieldnum)
			{
			  if((GEOMTYPE==2)||(GEOMTYPE==3))
			  {
                if(SetClassLevel(attr_names[fieldnum],OldCvalue,filename,TablesToExtract[table_num])>0)
				{
	              FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
				}
			  }
			}




			if(attr_codes[fieldnum]<0)
			{

	          if(attr_codes[fieldnum]==-997)
			  {
			    if(GEOMTYPE==0)
				{
	              featureid = OldIvalue;
				} 
			    else
				{ 
				  if(GEOMTYPE==-999)
				  {
				    printf("NULL %s for a feature in %s table %s.  Assigning 9999999\n",attr_names[fieldnum],filename,TablesToExtract[table_num]);
				  }
				  else if(GEOMTYPE==2)
				  {
				    printf("String value for %s for a feature in %s table %s (expected int).  Assigning 9999999\n",attr_names[fieldnum],filename,TablesToExtract[table_num]);
				  }
				  else if(GEOMTYPE==1)
				  {
				    printf("Floating value for %s a feature in %s table %s (expected int).  Assigning 9999999\n",attr_names[fieldnum],filename,TablesToExtract[table_num]);
				  }
				  else if(GEOMTYPE==3) 
				  {
				    printf("Invalid value for %s for a feature in %s table %s (expected int).  Assigning 9999999\n",attr_names[fieldnum],filename,TablesToExtract[table_num]);
				  }

				  if(gifd_found>0)
				  {
					printf("...GFID/FLDBID/UID/UFI = %s\n",thisGFcode);
				  }
				  else
				  {
					printf("...No GFID/FLDBID/UID/UFI found for this feature either\n");
				  }

				  featureid = 9999999;
				}
			  } 
  	          else if(attr_codes[fieldnum]==-998)
			  { 
			    if(GEOMTYPE==2)
				{
	              sprintf(thisGFcode,"%s",OldCvalue);
				  gifd_found = 1;
				}
				else
				{
				  if((GEOMTYPE==0)||(GEOMTYPE==3))
				  {
				    printf("(2) bad type %d for %s:\n   %s %s %d\n",
					  GEOMTYPE,filename,TablesToExtract[table_num],attr_names[fieldnum],featureid);

	                sprintf(thisGFcode,"%d",OldIvalue);
				    gifd_found = 1;
				  }
				  else if(GEOMTYPE==2)
				  {
				    printf("(2) bad type %d for %s:\n   %s %s %d\n",
					  GEOMTYPE,filename,TablesToExtract[table_num],attr_names[fieldnum],featureid);

	                sprintf(thisGFcode,"%lf",OldDvalue);
				    gifd_found = 1;
				  }
				  else if(GEOMTYPE==-999)
				  {
					
				  }
				}
			  }
   	          else if(attr_codes[fieldnum]==-999)
			  {
				if(GEOMTYPE==2)
				{
	              if(OldCvalue[0]=='\0')
				  {
		            AddBadClass(OldCvalue,FCODEsOfTables[table_num],featureid);
				  }
	              else if(strlen(OldCvalue)<5)
				  {
		            AddBadClass(OldCvalue,FCODEsOfTables[table_num],featureid);
				  }
	              else
				  { 
		            for(j=0;j<(int)strlen(OldCvalue);j++)
					{ 
		              OldCvalue[j] = timtoupper(OldCvalue[j]);
					} 

					sprintf(sav999,"%s",OldCvalue);
		  
		            other_class_code = ConvertClassCode(OldCvalue,featureid);
		  
		            if(!strcmp(FCODEsOfTables[table_num],"*****"))
					{ 
		              class_code = other_class_code;				  
					} 
		            else
					{ 
		              
		              if(other_class_code!=class_code)
					  { 
 			            AddBadClass(OldCvalue,FCODEsOfTables[table_num],featureid);
					  } 
					}
				  }
				}
				else
				{
                  printf("(1) bad geomtype %d for %s %s %s\n",GEOMTYPE,filename,TablesToExtract[table_num],attr_names[fieldnum]);
				}
			  }
   	          else if(attr_codes[fieldnum]==-996)
			  {
				if(GEOMTYPE==0)
				{
				  sprintf(sav996,"%d",OldIvalue);
				}
				else
				{
				  if(firstbad==0)
				  {
				    printf("non-integer data type for FCSubType...cannot check FCSubType/F_CODE compliance\n");
					firstbad = 1;
				  }
				}
			  }
			}

			else 
			{
			  if(IsArea(attr_names[fieldnum]))
			  {
				longcode = 1;
				area_found = NOTYPE;
				if(GEOMTYPE==0)
				{
		          area_found   = I_TYPE;
				  SaveAreaI    = OldIvalue;
				  SaveAreaC[0] = '\0';
				  SaveAreaD    = 0;
				}
				else if(GEOMTYPE==1)
				{
		          area_found   = D_TYPE;
				  SaveAreaI    = 0;
				  SaveAreaC[0] = '\0';
				  SaveAreaD    = OldDvalue;
				}
				else if(GEOMTYPE==2)
				{
		          area_found   = C_TYPE;
				  SaveAreaI    = 0;
				  sprintf(SaveAreaC,"%s",OldCvalue);
				  SaveAreaD    = 0;
				}
			  }
			  if(IsHgt(attr_names[fieldnum]))
			  {
				longcode = 1;
				hgt_found = NOTYPE;
				if(GEOMTYPE==0)
				{
		          hgt_found   = I_TYPE;
				  SaveHgtI    = OldIvalue;
				  SaveHgtC[0] = '\0';
				  SaveHgtD    = 0;
				}
				else if(GEOMTYPE==1)
				{
		          hgt_found   = D_TYPE;
				  SaveHgtI    = 0;
				  SaveHgtC[0] = '\0';
				  SaveHgtD    = OldDvalue;
				}
				else if(GEOMTYPE==2)
				{
		          hgt_found   = C_TYPE;
				  SaveHgtI    = 0;
				  sprintf(SaveHgtC,"%s",OldCvalue);
				  SaveHgtD    = 0;
				}
			  }
			  if(IsLen(attr_names[fieldnum]))
			  {
				longcode = 1;
				len_found = NOTYPE;
				if(GEOMTYPE==0)
				{
		          len_found   = I_TYPE;
				  SaveLenI    = OldIvalue;
				  SaveLenC[0] = '\0';
				  SaveLenD    = 0;
				}
				else if(GEOMTYPE==1)
				{
		          len_found   = D_TYPE;
				  SaveLenI    = 0;
				  SaveLenC[0] = '\0';
				  SaveLenD    = OldDvalue;
				}
				else if(GEOMTYPE==2)
				{
		          len_found   = C_TYPE;
				  SaveLenI    = 0;
				  sprintf(SaveLenC,"%s",OldCvalue);
				  SaveLenD    = 0;
				}
			  }
			  if(IsNam(attr_names[fieldnum]))
			  {
				longcode = 1;
				nam_found = NOTYPE;
				if(GEOMTYPE==0)
				{
		          nam_found   = I_TYPE;
				  SaveNamI    = OldIvalue;
				  SaveNamC[0] = '\0';
				  SaveNamD    = 0;
				}
				else if(GEOMTYPE==1)
				{
		          nam_found   = D_TYPE;
				  SaveNamI    = 0;
				  SaveNamC[0] = '\0';
				  SaveNamD    = OldDvalue;
				}
				else if(GEOMTYPE==2)
				{
		          nam_found   = C_TYPE;
				  SaveNamI    = 0;
				  sprintf(SaveNamC,"%s",OldCvalue);
				  SaveNamD    = 0;

				  if(DoNFI())
				  {
                    sprintf(saveNAM,"\"%s\"",SaveNamC);
				  } 
				}
			  }
			  if(IsZv2(attr_names[fieldnum]))
			  {
				longcode = 1;
				zv2_found = NOTYPE;
				if(GEOMTYPE==0)
				{
		          zv2_found   = I_TYPE;
				  SaveZv2I    = OldIvalue;
				  SaveZv2C[0] = '\0';
				  SaveZv2D    = 0;
				}
				else if(GEOMTYPE==1)
				{
		          zv2_found   = D_TYPE;
				  SaveZv2I    = 0;
				  SaveZv2C[0] = '\0';
				  SaveZv2D    = OldDvalue;
				}
				else if(GEOMTYPE==2)
				{
		          zv2_found   = C_TYPE;
				  SaveZv2I    = 0;
				  sprintf(SaveZv2C,"%s",OldCvalue);
				  SaveZv2D    = 0;
				}
			  }
			}

			if(longcode==0)
			{
	          if(attr_codes[fieldnum]>=0)
			  {
				if(ATTRTYPE_ANY_MGCP())
				{


				  if((GEOMTYPE==0)||(GEOMTYPE==3))
				  {
			        optional_attribute[fieldnum] = EvalValue(optional_attribute[fieldnum],
				                                    ReportName,
				                                    attr_names[fieldnum],
				                                    OldIvalue, 
				                                    OldDvalue, 
				                                    OldCvalue,
				                                    1); 
				  }
				  else if(GEOMTYPE==1)
				  {
			        optional_attribute[fieldnum] = EvalValue(optional_attribute[fieldnum],
				                                    ReportName,
				                                    attr_names[fieldnum],
				                                    OldIvalue,
				                                    OldDvalue,
				                                    OldCvalue,
				                                    2); 
				  }
				  else if(GEOMTYPE==2)
				  {
			        optional_attribute[fieldnum] = EvalValue(optional_attribute[fieldnum],
				                                    ReportName,
				                                    attr_names[fieldnum],
				                                    OldIvalue,
				                                    OldDvalue,
				                                    OldCvalue,
				                                    4); 
				  }
				}




	            if((GEOMTYPE==0)||(GEOMTYPE==1)||(GEOMTYPE==2)||(GEOMTYPE==3))
				{
				  if(GEOMTYPE==3)
				  {
		            OldIvalue = newenum;
				  }

		          if(SH_AddLongAttribute (SH_SAC_LONG, sh_sac_long_index,attr_codes[fieldnum],TIM_UNITLESS,TIM_UNI,SEDRIStype,
					  OldIvalue,OldDvalue,OldCvalue))
				  { 
		            SH_AddShortAttribute(SH_SAC_SHORT,sh_sac_short_index,attr_codes[fieldnum]);
		      
		            sh_sac_short_index++;
		            sh_sac_long_index++;
				  }  
				}  
			  }  
			}  
		  } 




		  

		  if(ATTRTYPE_ANY_GGDM())
		  {
			int totalnum,thisindex,starter;
			char expmessage[300],foundmessage[300];
			char **thearr;

            extern int ggdm21_extra_num;
            extern int ggdm22_extra_num;
            extern int ggdm3_extra_num;
            extern char *ggdm21_extra[];  
            extern char *ggdm22_extra[];  
            extern char *ggdm3_extra[];   
            extern int ATTRTYPE_GGDM21();
            extern int ATTRTYPE_GGDM22();
            extern int ATTRTYPE_GGDM3();

			if(
				(strcmp(sav996,"ABCPRT")) &&
			    (strcmp(sav999,"AAAABBBBRRRRFFFFTTTT"))
			  )
			{
			  if(ATTRTYPE_GGDM21())
			  { 
			    totalnum = ggdm21_extra_num*2;
				thearr = ggdm21_extra;
			  } 
			  else if(ATTRTYPE_GGDM22())
			  { 
			    totalnum = ggdm22_extra_num*2;
				thearr = ggdm22_extra;
			  } 
			  else if(ATTRTYPE_GGDM3())
			  { 
			    totalnum = ggdm3_extra_num*2;
				thearr = ggdm3_extra;
			  } 

			  starter = 0;

			  if(lastindex<totalnum)
			  {
				if(!strcmp(sav999,thearr[lastindex]))
				{
				   starter = lastindex;
				}
			  }

			  for(thisindex=starter;thisindex<totalnum;thisindex=thisindex+2)
			  { 
				if(!strcmp(sav999,thearr[thisindex]))
				{ 
				   if(strcmp(sav996,thearr[thisindex+1]))
				   { 
		             

					 sprintf(expmessage,  "F_CODE/FCSubType: %s/%s",thearr[thisindex],thearr[thisindex+1]);
					 sprintf(foundmessage,"F_CODE/FCSubType: %s/%s",thearr[thisindex],sav996);

					 AddBadClass(foundmessage,expmessage,featureid);
				   }
				   
				   lastindex = thisindex;
				   break;
				}
			  }
			}
		  }



  	      if((NumHoles+SH_Points_Done+SH_Linears_Done+SH_Areals_Done+SH_bad_objects)%50==0)  
		  {
	        update_shape_import(filename,TablesToExtract[table_num],table_num,-1,-1,
	           SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
	           0,0,0,0,SH_bad_objects, SH_BadCoords, SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);

		    if(ABORT_EXTRACT==1)
			{ 
		      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
			}
		  }





          if((class_code<0))
		  {
	        
	        SH_bad_objects = SH_bad_objects + 1;
	        AddBadClass("NO FCODE FOUNDzzzSentinel",FCODEsOfTables[table_num],-999);
		  }
          else
		  {

		     

             nullfeature = 0;
			 hr = ipFeature->get_ShapeCopy(&ipGeometry);
             if(!SUCCEEDED(hr))
			 { 
               ESRIErrorHandle(2,"trying to get geometry of feature",hr);
			 } 
			 
			 if(ipGeometry==NULL)
			 {
               AddNullShape(featureid,1,0,0);

			   printf("Found NULL feature:\n Geodatabase: %s\n  Table: %s featureid: %d\n",
				   filename,FCODEsOfTables[table_num],featureid);
			   nullfeature = 1;
			 }



			 emptyVar = VARIANT_FALSE;

			 if(nullfeature==0)
			 {
               hr = ipGeometry->get_IsEmpty(&emptyVar);

               if(!SUCCEEDED(hr))
			   {  
                 ESRIErrorHandle(2,"trying to test if geometry is empty",hr);
			   }
			 }

			 if(emptyVar==VARIANT_TRUE)
			 {
			   SH_Zero_Shapes = SH_Zero_Shapes + 1;
			   printf("Found Zero part feature:\n Geodatabase: %s\n  Table: %s featureid: %d\n",
				   filename,FCODEsOfTables[table_num],featureid);

			   if(ftype==esriGeometryPoint)
			   {
			     fprintf(badshapeout,"Point feature detected with 0 parts.  FID %d\n  Geodatabase: %s Table %s\n",
	                  featureid,filename,FCODEsOfTables[table_num]);
			   }
			   else if(ftype==esriGeometryPolyline)
			   {
			     fprintf(badshapeout,"Line feature detected with 0 parts.  FID %d\n  Geodatabase: %s Table %s\n",
	                  featureid,filename,FCODEsOfTables[table_num]);
			   }
			   else if(ftype==esriGeometryPolygon)
			   {
			     fprintf(badshapeout,"Area feature detected with 0 parts.  FID %d\n  Geodatabase: %s Table %s\n",
	                  featureid,filename,FCODEsOfTables[table_num]);
			   }
			   else
			   {
			     fprintf(badshapeout,"Feature detected with 0 parts.  FID %d\n  Geodatabase: %s Table %s\n",
	                  featureid,filename,FCODEsOfTables[table_num]);
			   }
			 }
			else if(nullfeature==0)
			 {
			   switch(ftype)
			   {
			     case esriGeometryPoint:
				  {
				   IPointPtr ipPoint = ipGeometry;
				   IZAwarePtr ipZaware = ipPoint;
				   int doit;

				   hr = ipZaware->get_ZAware(&hasZ);
                   if(!SUCCEEDED(hr))
				   {
		             sprintf(errstr,"Unable to GetZA PT\n");
                     ESRIErrorHandle(2,errstr,hr);
	                 FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
				   }

				   hr = ipPoint->get_X(&xval);
                   if(!SUCCEEDED(hr))
				   {
		             sprintf(errstr,"Unable to GetXA PT\n");
                     ESRIErrorHandle(2,errstr,hr);
	                 FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
				   }

				   hr = ipPoint->get_Y(&yval);
                   if(!SUCCEEDED(hr))
				   {
		             sprintf(errstr,"Unable to GetYA PT\n");
                     ESRIErrorHandle(2,errstr,hr);
	                 FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
				   }


				   if((hasZ==VARIANT_TRUE)&&(NoZ_Values==0))  
				   {
				 	hr = ipPoint->get_Z(&zval);
                    if(!SUCCEEDED(hr))
					{
		             sprintf(errstr,"Unable to GetZ2 PT\n");
                     ESRIErrorHandle(2,errstr,hr);
	                 FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
					} 
                    num3point = num3point + 1;
				   }  
				   else
				   {  
					zval = MY2DSENTINEL;
				    num2point = num2point + 1;
				   }  

				   
 
				    doit = 1;
				    if(CLIPEXTRACT==1)
					{
	                  if((xval<CLIPLLX)  ||
	                     (yval<CLIPLLY)  ||
	                     (xval>CLIPURX)  ||
	                     (yval>CLIPURY))
					  {
	                    doit = 0;
					  } 
					} 


                    if(TestGeodetic(xval,yval,featureid,filename)==0)
					{
	                  doit = 0;
					} 


				   if(doit==1)
				   {
                     NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,5);  

                     WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
                     WriteGFID(1,SaveHgtC ,SaveHgtI ,SaveHgtD ,hgt_found ,LONG_ATTR_HGT);

                     WriteGFID(1,SaveLenC ,SaveLenI ,SaveLenD ,len_found ,LONG_ATTR_LEN);
                     WriteGFID(1,SaveNamC ,SaveNamI ,SaveNamD ,nam_found ,LONG_ATTR_NAM);

                     WriteGFID(1,SaveZv2C ,SaveZv2I ,SaveZv2D ,zv2_found ,LONG_ATTR_ZV2);

                     WriteGFID(gifd_found,thisGFcode,-999,-999,C_TYPE,LONG_ATTR_ID);


				     WritePointFeature(filename,
					   addtolong,addtoshort,class_code,featureid,
					   sh_sac_long_index,sh_sac_short_index,
			           SH_SAC_LONG,SH_SAC_SHORT,
					   xval,yval,zval);
				   }
				  }
				break;
			    case esriGeometryPolyline:
				{
                  long numverts = 0;
				  long numparts = 0;
				  int doit = 0;
				  int part_num,longindex,shortindex,seeitgeom=4;

			      

				  


                  GetShortName(mdbname,filename);


  

				    IGeometryCollectionPtr theParts = ipGeometry;

				    hr = theParts->get_GeometryCount(&numparts);
                    if(!SUCCEEDED(hr))
					{
		             sprintf(errstr,"Unable to GetGC PL\n");
                     ESRIErrorHandle(2,errstr,hr);
	                 FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
					}



				    if(numparts>1)
					{
					  long totalpoints=0,pointnum;

					  

					  doit = 0;

					  if(CLIPEXTRACT==0)
					  {
						doit = 1;
				        NumMultiPartLineFeatures = NumMultiPartLineFeatures + 1;
					  }
					  else
					  {
                        IPointCollectionPtr thiscollection = ipGeometry;
					    hr = thiscollection->get_PointCount(&totalpoints);

                        if(!SUCCEEDED(hr))
						{
		                  sprintf(errstr,"Unable to GetPC PL\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						}

				        

						doit = 0;
 
					     for(pointnum=0;pointnum<totalpoints;pointnum++)
						 {
						   

						   IPointPtr thisMpoint;
						   double Dx,Dy;

					       hr = thiscollection->get_Point(pointnum,&thisMpoint);
                           if(!SUCCEEDED(hr))
						   {
		                     sprintf(errstr,"Unable to GetPPC PL\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }

						   hr = thisMpoint->get_X(&Dx);
                           if(!SUCCEEDED(hr))
						   { 
		                     sprintf(errstr,"Unable to GetDx PL\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }
						   hr = thisMpoint->get_Y(&Dy);
                           if(!SUCCEEDED(hr))
						   { 
		                     sprintf(errstr,"Unable to GetDy PL\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }

	                       if((Dx>=CLIPLLX)  &&
	                          (Dy>=CLIPLLY)  &&
	                          (Dx<=CLIPURX)  &&
	                          (Dy<=CLIPURY))
						   {
	                         doit = 1;
				             NumMultiPartLineFeatures = NumMultiPartLineFeatures + 1;
							 break;
						   }  
						 } 
					  } 
					} 


				    for(part_num=0;part_num<numparts;part_num++)
					{
					  IGeometryPtr thispart;
					  int pointnum,vertices_made;
				      theParts->get_Geometry(part_num,&thispart);

					  vertices_made = GetCurveVertices(thispart,featureid,mdbname,TablesToExtract[table_num]);

					  IPointCollectionPtr thiscollection = thispart;


					  if(vertices_made>0)
					  {
                        numverts = vertices_made;
					  }
					  else
					  {
					    hr = thiscollection->get_PointCount(&numverts);
                        if(!SUCCEEDED(hr))
						{  
		                  sprintf(errstr,"Unable to GetPC2 PL\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						} 
					  }

					  if(numverts>verts_allocced)
					  {
						if(verts_allocced!=0)
						{
						  free(bigX);
						  free(bigY);
						  free(bigZ);
						}
						 
						bigX = (double *)malloc(SzD*(numverts + 1000));
						bigY = (double *)malloc(SzD*(numverts + 1000));
						bigZ = (double *)malloc(SzD*(numverts + 1000));
                        verts_allocced = numverts + 990;
					  }



					  for(pointnum=0;pointnum<numverts;pointnum++)
					  {
                         if(vertices_made>0)
						   { 
                              bigX[pointnum] = tempVertX[pointnum];
                              bigY[pointnum] = tempVertY[pointnum];
                              bigZ[pointnum] = tempVertZ[pointnum];
							  if(bigZ[pointnum]==MY2DSENTINEL)
							  {
                                num2line = num2line + 1;
							  }
							  else
							  {
                                num3line = num3line + 1;
							  }
                              continue;
						   } 



						IPointPtr thispoint;
					    hr = thiscollection->get_Point(pointnum,&thispoint);
                        if(!SUCCEEDED(hr))
						{ 
		                  sprintf(errstr,"Unable to GetPC3 PL\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						}

						IZAwarePtr hasZptr = thispoint;

						hr = thispoint->get_X(&bigX[pointnum]);
                        if(!SUCCEEDED(hr))
						{
		                  sprintf(errstr,"Unable to GetX3 PL\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						}

						hr = thispoint->get_Y(&bigY[pointnum]);
                        if(!SUCCEEDED(hr))
						{
		                  sprintf(errstr,"Unable to GetY3 PL\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						}

				        hr = hasZptr->get_ZAware(&hasZ);
                        if(!SUCCEEDED(hr))
						{
		                  sprintf(errstr,"Unable to GetZA3 PL\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						}

				        if((hasZ==VARIANT_TRUE)&&(NoZ_Values==0))  
						{
				 	      hr = thispoint->get_Z(&bigZ[pointnum]);
                          if(!SUCCEEDED(hr))
						  {
		                    sprintf(errstr,"Unable to GetZ3 PL\n");
                            ESRIErrorHandle(2,errstr,hr);
	                        FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						  } 
                          num3line = num3line + 1;
						}
				        else
						{
					      bigZ[pointnum] = MY2DSENTINEL;
				          num2line = num2line + 1;
						}

                        

						if(doit==1)
						{
	                      if(TestGeodetic(bigX[pointnum],bigY[pointnum],featureid,filename)==0)
						  {
                            NumMultiPartLineFeatures = NumMultiPartLineFeatures - 1; 
							doit = 0;
						  }
						}
					  }

					  




					  if(numparts==1)
					  {
					    
					    doit = Arc_ClipTest(featureid,filename,numverts,0,-1);
					  }


					  if(doit==1)
						{
 
					      if(part_num==0)
						  {
                           sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
                           sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s;Table %s",class_code,seeitgeom,mdbname,tempstr);
                           sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s;Table %s",class_code,seeitgeom,mdbname,tempstr);

                           longindex  = SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index, SH_SAC_LONG, featureid); 
                           shortindex = SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, featureid);
	 
                           longindex  = longindex  + addtolong;
                           shortindex = shortindex + addtoshort;


                           NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,seeitgeom);

                           WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
                           WriteGFID(1,SaveHgtC ,SaveHgtI ,SaveHgtD ,hgt_found ,LONG_ATTR_HGT);

                           WriteGFID(1,SaveLenC ,SaveLenI ,SaveLenD ,len_found ,LONG_ATTR_LEN);
                           WriteGFID(1,SaveNamC ,SaveNamI ,SaveNamD ,nam_found ,LONG_ATTR_NAM);

                           WriteGFID(1,SaveZv2C ,SaveZv2I ,SaveZv2D ,zv2_found ,LONG_ATTR_ZV2);

                           WriteGFID(gifd_found,thisGFcode,-999,-999,C_TYPE,LONG_ATTR_ID);
						  } 
		

					      if(numparts>1)
						  {
					        NumLineParts = NumLineParts + 1;
						  } 

				          WriteLineFeature(
					        addtolong,addtoshort,class_code,featureid,
					        sh_sac_long_index,sh_sac_short_index,
			                SH_SAC_LONG,SH_SAC_SHORT,
					        numverts,part_num,numparts,longindex,shortindex);

					} 

				  } 

				} 
			    break;
			    case esriGeometryPolygon:
				{
                  long numverts = 0,numparts = 0,numexterior = 0,numholes = 0,numholeverts = 0;
				  int doit = 0,pointnum,multipart,toppart;
				  int part_num,longindex,shortindex,seeitgeom=1,thishole;




                  
				  IPolygonPtr ipPolygon = ipGeometry;
                  IGeometryCollectionPtr theParts = ipGeometry;


				  hr = theParts->get_GeometryCount(&numparts);
                  if(!SUCCEEDED(hr))
				  {
		            sprintf(errstr,"Unable to GetGC PG\n");
                    ESRIErrorHandle(2,errstr,hr);
	                FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
				  }

				  numexterior = 0;

				  


				  if(numparts>maxnumparts)
				  {
					

                    

					if(maxnumparts>0)
					{
                      for(indexer=0;indexer<maxnumparts;indexer++)
					  {
					    if(NumvertMaxArray[indexer]>0)
						{ 
						  
					      free(Xcoords[indexer]);
					      free(Ycoords[indexer]);
					      free(Zcoords[indexer]);
						} 
					  } 

					  free(Xcoords);
					  free(Ycoords);
					  free(Zcoords);
					  free(NumvertArray);
					  free(NumvertMaxArray);
					  free(ExteriorArray);
					  free(ParentRing);
					}

					Xcoords = (double **)malloc(sizeof(double *) * (numparts + 1000));
					Ycoords = (double **)malloc(sizeof(double *) * (numparts + 1000));
					Zcoords = (double **)malloc(sizeof(double *) * (numparts + 1000));
			        NumvertArray    = (int *)malloc(SzI * (numparts+1000));
			        NumvertMaxArray = (int *)malloc(SzI * (numparts+1000));
			        ExteriorArray   = (int *)malloc(SzI * (numparts+1000));
			        ParentRing      = (int *)malloc(SzI * (numparts+1000));

					maxnumparts = numparts + 1000;

					for(indexer=0;indexer<maxnumparts;indexer++)
					{
                      NumvertMaxArray[indexer] = -1;
                      NumvertArray   [indexer] = -1;
                      ExteriorArray  [indexer] = -1;
                      ParentRing     [indexer] = -1;
					}
				  }


				  for(part_num=0;part_num<numparts;part_num++)
				  {
					  VARIANT_BOOL isExterior;
					  IGeometryPtr thispart;
					  int pointnum,vertices_made;


				      hr = theParts->get_Geometry(part_num,&thispart);
                      if(!SUCCEEDED(hr))
					  {
		                sprintf(errstr,"Unable to GetG1 PG\n");
                        ESRIErrorHandle(2,errstr,hr);
	                    FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
					  } 

                      IRingPtr thisring = thispart;

					  vertices_made = GetCurveVertices(thispart,featureid,mdbname,TablesToExtract[table_num]);

					  hr = thisring->get_IsExterior(&isExterior);
                      if(!SUCCEEDED(hr))
					  {
		                sprintf(errstr,"Unable to GetIE PG\n");
                        ESRIErrorHandle(2,errstr,hr);
	                    FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
					  } 

					  IPointCollectionPtr thiscollection = thispart;

                      if(vertices_made==0)
					  { 
					    hr = thiscollection->get_PointCount(&numverts);
                        if(!SUCCEEDED(hr))
						{
		                  sprintf(errstr,"Unable to GetPC PG\n");
                          ESRIErrorHandle(2,errstr,hr);
	                      FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						}   
					  } 
                      else 
					  {
                        numverts = vertices_made;

	                    if(
                            (tempVertX[numverts-1] != tempVertX[0])||
                            (tempVertY[numverts-1] != tempVertY[0])||
                            (tempVertZ[numverts-1] != tempVertZ[0])
	                      )
						{ 
	                       tempVertX[numverts] = tempVertX[0];
                           tempVertY[numverts] = tempVertY[0];
                           tempVertZ[numverts] = tempVertZ[0];
                           numverts = numverts + 1;
						}
					  } 
					
	 				  if(isExterior==VARIANT_TRUE)
					  {
						ExteriorArray[part_num] = 1;
						numexterior = numexterior + 1;
					  }
					  else
					  {
						ExteriorArray[part_num] = 0;
					  }


					  

					  if(ExteriorArray[part_num]==1)
					  {
					     if(NumvertMaxArray[part_num]<=numverts)
						 {
						   if(NumvertArray[part_num]>0)
						   { 
						    free(Xcoords[part_num]);
						    free(Ycoords[part_num]);
						    free(Zcoords[part_num]);
						   } 

						   Xcoords[part_num] = (double *)malloc(SzD*(numverts+990));
						   Ycoords[part_num] = (double *)malloc(SzD*(numverts+990));
						   Zcoords[part_num] = (double *)malloc(SzD*(numverts+990));

						   NumvertMaxArray[part_num] = numverts+990;
						 } 

					     

					     NumvertArray[part_num] = numverts;


				         for(pointnum=0;pointnum<numverts;pointnum++)
						 {
                           if(vertices_made>0)
						   { 
                              Xcoords[part_num][pointnum] = tempVertX[pointnum];
                              Ycoords[part_num][pointnum] = tempVertY[pointnum];
                              Zcoords[part_num][pointnum] = tempVertZ[pointnum];
							  if(Zcoords[part_num][pointnum]==MY2DSENTINEL)
							  {
                                num2area = num2area + 1;
							  }
							  else
							  {
                                num3area = num3area + 1;
							  }
                              continue;
						   } 

						   IPointPtr thispoint;
					       thiscollection->get_Point(pointnum,&thispoint);
                           if(!SUCCEEDED(hr))
						   {
		                     sprintf(errstr,"Unable to GetP2 PG\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }  

						   IZAwarePtr hasZptr = thispoint;

						   hr = thispoint->get_X(&Xcoords[part_num][pointnum]);
                           if(!SUCCEEDED(hr))
						   {
		                     sprintf(errstr,"Unable to GetX1 PG\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }

						   hr = thispoint->get_Y(&Ycoords[part_num][pointnum]);
                           if(!SUCCEEDED(hr))
						   { 
		                     sprintf(errstr,"Unable to GetY1 PG\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }

				           hr = hasZptr->get_ZAware(&hasZ);
                           if(!SUCCEEDED(hr))
						   { 
		                     sprintf(errstr,"Unable to GetZA1 PG\n");
                             ESRIErrorHandle(2,errstr,hr);
	                         FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
						   }

				           if((hasZ==VARIANT_TRUE)&&(NoZ_Values==0))  
						   {
				 	         hr = thispoint->get_Z(&Zcoords[part_num][pointnum]);
                             if(!SUCCEEDED(hr))
							 { 
		                       sprintf(errstr,"Unable to GetZ1 PG\n");
                               ESRIErrorHandle(2,errstr,hr);
	                           FreeArcMem();  hr = ipInit->Shutdown();    if(!SUCCEEDED(hr)) {ESRIErrorHandle(2,"Shutting down ArcObjects",hr);}  return 0;
							 }
                             num3area = num3area + 1;
						   } 
				           else
						   {
					         Zcoords[part_num][pointnum] = MY2DSENTINEL;
				             num2area = num2area + 1;
						   } 
						 } 



					  } 



					  

                      

				  } 


                  GetShortName(mdbname,filename);


 			      doit = Arc_ClipTest(featureid,filename,numverts,1,numparts);

				  if(doit==1)
				  {

				    

				    multipart = 0;
					if(numexterior>1)
					{
					  
                      NumMultiPartAreaFeatures = NumMultiPartAreaFeatures + 1;
                      NumAreaParts             = NumAreaParts + numexterior;
					  multipart = 1;
 					}



	                SH_Areals_Done           = SH_Areals_Done + 1;	


                    sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
                    sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s;Table %s",class_code,seeitgeom,mdbname,tempstr);
                    sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s;Table %s",class_code,seeitgeom,mdbname,tempstr);

                    longindex  = SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index, SH_SAC_LONG, featureid); 
                    shortindex = SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, featureid);
	 
                    longindex  = longindex  + addtolong;
                    shortindex = shortindex + addtoshort;

					NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,seeitgeom);

                    WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
                    WriteGFID(1,SaveHgtC ,SaveHgtI ,SaveHgtD ,hgt_found ,LONG_ATTR_HGT);
                    WriteGFID(1,SaveLenC ,SaveLenI ,SaveLenD ,len_found ,LONG_ATTR_LEN);
                    WriteGFID(1,SaveNamC ,SaveNamI ,SaveNamD ,nam_found ,LONG_ATTR_NAM);
                    WriteGFID(1,SaveZv2C ,SaveZv2I ,SaveZv2D ,zv2_found ,LONG_ATTR_ZV2);

                    WriteGFID(gifd_found,thisGFcode,-999,-999,C_TYPE,LONG_ATTR_ID);


				    toppart = 0;

				    for(part_num=0;part_num<numparts;part_num++)
					{ 
                       if (ExteriorArray[part_num]==1)
					   {
						  

						  WriteAreaFeature(NumvertArray[part_num],featureid,longindex,shortindex,0,multipart,toppart,part_num);
						  toppart = toppart + 1;

						  for(thishole=0;thishole<numparts;thishole++)
						  {
						    

						    if(ExteriorArray[thishole]==0)
							{ 
			
                              

                              

                              numholeverts = StoreHoleVerts(theParts,thishole,ipInit,featureid,mdbname,TablesToExtract[table_num]);

							  if(numholeverts>0)
							  {
							    for(pointnum=0;pointnum<1;pointnum++)  
								{
							      if(SH_PointInAreal(bigX[pointnum],bigY[pointnum],
		                               Xcoords[part_num],Ycoords[part_num],
		                               0,NumvertArray[part_num]-1)==1)
								  {
								    if(Arc_ClipTest(featureid,filename,numholeverts,3,numparts)==1)
									{
	  					              WriteAreaFeature(numholeverts,featureid,longindex,shortindex,1,multipart,toppart,thishole);
						              NumHoles = NumHoles + 1;
									  ExteriorArray[thishole] = 2; 
									}
								    else
									{ 
									  
									  printf("Featureid: %d: (interior part) %d/%d not in the clip box when an exterior part is in the clip box with %d verts\n",featureid,thishole+1,numparts,numholeverts);
									}
								  } 
								} 
							  } 
							} 
						  } 
					   } 
					} 

					


                    for(thishole=0;thishole<numparts;thishole++)
					{
					  if(ExteriorArray[thishole]==0)
					  {
						

                        numholeverts = StoreHoleVerts(theParts,thishole,ipInit,featureid,mdbname,TablesToExtract[table_num]);

                        badhole = 1;
                        AddBadHole(featureid,thishole,TablesToExtract[table_num],3);
						WriteAreaFeature(numholeverts,featureid,longindex,shortindex,0,multipart,toppart,part_num);
						badhole = 0;
					  }
					}



					if((numparts>1000)||(NumvertMaxArray[0]>50000))
					{
					  
					  FreeArcMem();
					}


				  } 


				 break;
				 } 
			    default:
					printf("unexpected geometry %d\n",ftype);
				 break;
			   }  
			 }  
		  }  



          for(indexer=0;indexer<sh_sac_long_index;indexer++)   
		  {
	        if(SH_SAC_LONG[indexer].lod_stuff!= NULL)
			{
	          free(SH_SAC_LONG[indexer].lod_stuff);
	          SH_SAC_LONG[indexer].lod_stuff = NULL;
			}

	        if(SH_SAC_SHORT[indexer].lod_stuff!=NULL)
			{
	          free(SH_SAC_SHORT[indexer].lod_stuff);
	          SH_SAC_SHORT[indexer].lod_stuff = NULL;
			}

	        if(SH_SAC_LONG[indexer].nam_stuff!= NULL)
			{ 
	          free(SH_SAC_LONG[indexer].nam_stuff);
	          SH_SAC_LONG[indexer].nam_stuff = NULL;
			}

	        if(SH_SAC_SHORT[indexer].nam_stuff!=NULL)
			{
	          free(SH_SAC_SHORT[indexer].nam_stuff);
	          SH_SAC_SHORT[indexer].nam_stuff = NULL;
			}

	        if(SH_SAC_LONG[indexer].geomtype==2)
			{
	          if(SH_SAC_LONG[indexer].Cvalue!=NULL)
			  {
		        free(SH_SAC_LONG[indexer].Cvalue);
		        SH_SAC_LONG[indexer].Cvalue = NULL;
			  }
			}
		  }
      
          sh_sac_short_index = 0;
          sh_sac_long_index = 0;


		  

		  hr = ipFeatureCursor->NextFeature(&ipFeature);  
          if(!SUCCEEDED(hr))
		  {
			if(hr==0x8004020a)
			{
			  printf("Serious error!  Got EVENT_E_MISSING_EVENTCLASS error on featureid %d\n",featureid);
			}
			else
			{
			  printf("\n\nFailed to get next feature after featureid %d...terminating load of this table\n\n",featureid);
              ESRIErrorHandle(2,"trying to get next feature",hr);
			}
		  }   

	  } 




	  
	  if(ATTRTYPE_ANY_MGCP())
	  { 

  	    for(fieldnum=0;fieldnum<numFields;fieldnum++)
		{ 
	      if(optional_attribute[fieldnum]==-11)
		  {
	        if(
				(attr_types[fieldnum]==esriFieldTypeSmallInteger) ||
				(attr_types[fieldnum]==esriFieldTypeInteger)
			  )
			{ 
		      fprintf(mixnull,"%s:\n  Column %s has mix of Null (-32768) and Non-Null Integer values\n",
			    ReportName,attr_names[fieldnum]);
			} 
	        else if(
				(attr_types[fieldnum]==esriFieldTypeSingle) ||
				(attr_types[fieldnum]==esriFieldTypeDouble)
			  )

			{ 
		      fprintf(mixnull,"%s:\n  Column %s has mix of Null (-32768.0) and Non-Null Real values\n",
			    ReportName,attr_names[fieldnum]);
			} 
	        else if(attr_types[fieldnum]==esriFieldTypeString)
			{
	          fprintf(mixnull,"%s:\n  Column %s has mix of Null (N/A) and Non-Null Character values\n",
		       ReportName,attr_names[fieldnum]);
			} 
		  } 
		} 
	  } 


      


	  for(fieldnum=0;fieldnum<numFields;fieldnum++)
	  {
		free(attr_names[fieldnum]);
	  }
	  free(attr_codes);
      free(attr_types);
      free(attr_names);
	  attr_codes = NULL;
      attr_types = NULL;
      attr_names = NULL;


	  if(ATTRTYPE_ANY_MGCP())
	  {
		  free(optional_attribute);
		  optional_attribute = NULL;
	  }




      

	  UpdateTable(&addtolong,&addtoshort,filename,TablesToExtract[table_num],table_num);

      


	  i = AlreadyFoundDomain(numFields,-1,2,NULL,NULL,numFields);


    } 
  } 






  


  FreeArcMem();

  if(tempVertX!=NULL)
  { 
	if(tempvertnum==0)
	{
      printf("Error: TVN 0 but NNTVX\n");
	}
	else
	{
	  free(tempVertX);
	  free(tempVertY);
	  free(tempVertZ);
	}

    tempVertX = NULL;
    tempVertY = NULL;
    tempVertZ = NULL;
    tempvertnum = 0;
  } 


  printf("Shutting down ArcObjects\n");
  hr = ipInit->Shutdown();
  if(!SUCCEEDED(hr))
  {
    ESRIErrorHandle(2,"Shutting down ArcObjects",hr);
  } 

  printf("final update table\n");
  printf("%s %s\n",filename,TablesToExtract[table_num-1]);

  UpdateTable(&addtolong,&addtoshort,filename,TablesToExtract[table_num-1],-1);

  *longitemsfound  =  addtolong  - origaddtolong;
  *shortitemsfound =  addtoshort - origaddtoshort;

  
  
  return 1;
}




