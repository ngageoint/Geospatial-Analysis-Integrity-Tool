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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "share.h"

extern void SH_PrintLists();
extern void sh_write_headers(unsigned char coord);
extern void SH_FreeLists(char *sentinel, char *shapename);
extern void SH_FreeTree();
extern void init_shape();
extern int import_shape(char *shape_to_import,int files_done, char *allshapes, char *fcode,char *shortname,
		 int *longitemsfound, int *shortitemsfound, int addtolong, int addtoshort, char *override_curr_file);
extern int import_dted(char *dtedfile, char *allshapes, int files_done, char *lod_override, int addtolong, int addtoshort);
extern char * GetLayerPart(char aline[]);
extern char *GetLastPart(char aline[]);

extern int SLASHTYPE;
extern int DOMODELSUMMARY;
extern int CREATING_TREX;

extern char importoutputdir[1000];

int global_trex_layer;
int DO_TIFFSURFACE  =  1;



int min_fin_xml_found0,
    max_fin_xml_found0,
    min_fin_xml_found1,
    max_fin_xml_found1,
	min_fin_xml_found2,
    max_fin_xml_found2,
	min_fin_xml_found3,
    max_fin_xml_found3,
	min_raw_xml_found,
	max_raw_xml_found;

double min_xml_elev_finished0,
       max_xml_elev_finished0,
       min_xml_elev_finished1,
       max_xml_elev_finished1,
       min_xml_elev_finished2,
       max_xml_elev_finished2,
       min_xml_elev_finished3,
       max_xml_elev_finished3,
	   min_xml_elev_raw,
	   max_xml_elev_raw;

int minmax_dem_finished_found0,
    minmax_dem_finished_found1,
    minmax_dem_finished_found2,
    minmax_dem_finished_found3,
    minmax_dem_raw_found;

double min_g_dem_elev_finished0,
       max_g_dem_elev_finished0,
       min_g_dem_elev_finished1,
       max_g_dem_elev_finished1,
       min_g_dem_elev_finished2,
       max_g_dem_elev_finished2,
       min_g_dem_elev_finished3,
       max_g_dem_elev_finished3,
	   min_g_dem_elev_raw,
	   max_g_dem_elev_raw;

int min_fin_header_found0,
    max_fin_header_found0,
    min_fin_header_found1,
    max_fin_header_found1,
    min_fin_header_found2,
    max_fin_header_found2,
    min_fin_header_found3,
    max_fin_header_found3,
	min_raw_header_found,
	max_raw_header_found;

double min_fin_header0,
       max_fin_header0,
       min_fin_header1,
       max_fin_header1,
	   min_fin_header2,
       max_fin_header2,
	   min_fin_header3,
       max_fin_header3,
	   min_raw_header,
	   max_raw_header;

int fin_header_date_found,raw_header_date_found;
char fin_header_date[100],raw_header_date[100];

int fin_xml_date_found,raw_xml_date_found,raw_copyright,finished_copyright;
char fin_xml_date[100],raw_xml_date[100];


int numtables_to_extract;
char **TablesToExtract;
char **FCODEsOfTables;
int   *TableGeoms;
int   *TableHasData;
char **dtedlines=NULL;
char **dtedlayers=NULL;
char ErrStr[1000];

char **headers=NULL;
char **sortkeys=NULL;

int NULLINT  = 0;
int NULLFILE = 0;
int ClassLevel = -100;
int COORDINATEKEEPER = 0;
int foundTREXraw     = 0;
int foundTREXfinished = 0;
int badGCP = 0;
int goodGCP1           = 0;
int goodGCP2           = 0;
int gcpindex1          = 0;
int gcpindex1s_0       = 0;
int gcpindex1s_1       = 0;
int gcpindex1s_2       = 0;
int gcpindex1s_3       = 0;
int gcpindex1_0        = 0;
int gcpindex1_1        = 0;
int gcpindex1_2        = 0;
int gcpindex1_3        = 0;
int gcpindex2          = 0;
int gcpindex2_0        = 0;
int gcpindex2_1        = 0;
int gcpindex2_2        = 0;
int gcpindex2_3        = 0;
int trexfinishedindex  = 0;
int trexfinishedindex0 = 0;
int trexfinishedindex1 = 0;
int trexfinishedindex2 = 0;
int trexfinishedindex3 = 0;
int foundrawfolder     = 0;

int trexwatermaskindex  = 0;
int trexwatermaskindex0 = 0;
int trexwatermaskindex1 = 0;
int trexwatermaskindex2 = 0;
int trexwatermaskindex3 = 0;

int NAM_HTML_LIMIT=2000000;

#define DEBUG 0

nodeType        sentinel = { NIL, NIL, 0, BLACK, 0};
nodeType_LIST L_sentinel = { NILL, NILL, 0, BLACK, 0};

extern int GIFD_ATTR_NUM;


FILE *headertxt,*gridspacefile, *badattrtxtout, *utfout,
  *badholetxtout, *arealinfofile, *linearinfofile, *pointinfofile, *badcurves,
  *gridinfofile, *multiinfofile, *badfcodeout, *versionfile, *multipartfile,
  *multipartpoints, *multipartlines, *multipartareas, *badshapeout,*badcasetxtout,
  *nullshptxtout,*badcoordout, *badunktxtout, *badGunktxtout, *mixnull,*filenames,
  *nofcode, *badfilename, *badshapetype, *nullattrval, *nullattrcsv, *badtypeout, 
  *badlenout, *emptyshapes,*codeinfofile,*Gblanktxtout,*llmapbin,*fidmapbin;


FILE *roadnetbin, *modelbin,  *bridgebin, *polysbin, *modelpolysbin, 
     *arealbin,   *headerbin, *gridbin,   *id1bin,   *id2bin,        *sizesbin,
     *hgt1bin,    *hgt2bin,   *zv1bin,    *zv2bin,   *area1bin,      *area2bin,
     *len1bin,    *len2bin,   *nam1bin,   *nam2bin,   *nfnfile,       *savenames, 
	 *badtrex,    *badtrexheaders;

FILE  *modelsummaryout;

char NetworkBinFile     [1000];
char ModelBinFile       [1000];
char ArealBinFile       [1000];
char NFNFile            [1000];
char TREX_errs          [1000];
char SaveNameFile       [1000];
char TrexXMLresultstub  [1000];
char TrexHeaderResults  [1000];
char BridgeBinFile      [1000];
char ModelPolygonBinFile[1000];
char PolygonBinFile     [1000];
char HeaderTxt          [1000];
char GridSpaceTxt       [1000];
char BadFCODE           [1000];
char NoFCODE            [1000];
char BadFilename        [1000];
char MixNull            [1000];
char BadShapeType       [1000];
char BadLenOut          [1000];
char BadTypeOut         [1000];
char NullAttrVal        [1000];
char NullAttrCsv        [1000];
char BadAttrTxt         [1000];
char NullShpTxt         [1000];
char BadUnkTxt          [1000];
char BadGUnkTxt         [1000];
char GBlankTxt          [1000];
char EmptyShapesTxt     [1000];
char BadCaseTxt         [1000];
char BadCoordTxt        [1000];
char BadHoleTxt         [1000];
char HeaderBin          [1000];
char Id1Bin             [1000];
char Id2Bin             [1000];
char Area1Bin           [1000];
char Area2Bin           [1000];
char Hgt1Bin            [1000];
char Hgt2Bin            [1000];
char Len1Bin            [1000];
char Len2Bin            [1000];
char Nam1Bin            [1000];
char Nam2Bin            [1000];
char Zv1Bin             [1000];
char Zv2Bin             [1000];
char GridBin            [1000];
char codesoutfull       [1000];
char codesoutshort      [1000];
char codesoutfullbin    [1000];
char codesoutshortbin   [1000];
char ModelDumpTxt       [1000];
char ModelDumpTxt2      [1000];
char ModelDumpTxt3      [1000];
char SizesBin           [1000];
char ArealHeader        [1000];
char LinearHeader       [1000];
char PointHeader        [1000];
char CodeHeader         [1000];
char SGridHeader        [1000];
char MultiHeader        [1000];
char VersionSt          [1000];
char PartHeader         [1000];
char PartPoints         [1000];
char PartLines          [1000];
char PartAreas          [1000];
char BadShapes          [1000];
char BadCurves          [1000];
char LLmapBin           [1000];
char FIDmapBin          [1000];
char SaveClassTable     [1000];
char SaveClassValue     [1000];
char UTFfile            [1000];
char FileNames          [1000];


int DO_FMIS=1;
int DO_DAMAGED_MODELS=0;
int GMI_ASSIGN_ID = 0;


long int TERRAINPOLYSREAD = 0;
unsigned long int gridpointsread=0;
int scc_short_items = 0;
int scc_long_items = 0;

nodeType *root = NIL;  
nodeType_LIST *L_root = NILL;  

struct SccNodeSH * SHORT_LIST;

struct FirstList * ListOfFirsts;
char GlobalDest[500];

int STF_CONSUMED=-1;
int ID_INDEX  = 0;      
int idle_redraw=0;

int MODEL_PASS_ONE          = 0;
int MODEL_PASS_TWO          = 0;
int MODEL_PASS_THREE        = 0;

int num_points     = 0;
int num_linears    = 0;
int num_areals     = 0;
int num_polys      = 0;
int num_gmi        = 0;
int num_fmi        = 0;
int num_grids      = 0;
int num_modelpolys =0;
int num_coll       = 0;
int num_par        = 0;
int num_stamp      = 0;
int num_foot       = 0;
int num_fmareal    = 0;
int num_fmpoint    = 0;
int num_fmlinear   = 0;
 

int NumPointFiles;
int NumLinearFiles;
int NumArealFiles;
int NumGridFiles;
int NumCodeFiles;


int NumPointParts   ;
int NumLineParts    ;
int NumAreaParts    ; 
int NumHoles        ;
int NumMultiPartPointFeatures; 
int NumMultiPartLineFeatures ;
int NumMultiPartAreaFeatures ;



long int TestNumArealNodes = 0;
long int TestNumGridNodes = 0;
long int TestNumLinearNodes = 0;

long int NumLinearNodes = 0;
long int NumPointNodes = 0;
long int NumArealNodes = 0;
long int NumFMArealNodes = 0;


double MINXREAD  = 100000000.0;
double MINYREAD  = 100000000.0;
double MINZREAD  = 100000000.0;
double MINZREAD2 = 100000000.0;
double MAXXREAD  = -100000000.0;
double MAXYREAD  = -100000000.0;
double MAXZREAD  = -100000000.0;
double MAXZREAD2 = -100000000.0;

double MINY_P_READ  = 100000000.0;
double MINX_P_READ  = 100000000.0;
double MAXY_P_READ  = -100000000.0;
double MAXX_P_READ  = -100000000.0;

int SH_Areals_Done = 0;
int SH_Areals_Prev = 0;
int SH_Linears_Done = 0;
int SH_Linears_Prev = 0;
int SH_Points_Done = 0;
int SH_Points_Prev = 0;

char **longshapenames = NULL;
char **shortshapenames = NULL;
int *shapenums = NULL;


int IGNORE_ATTR_NUM=0;
char **IGNORE_ATTR;

int REPLACE_ATTR_NUM=0;
char **REPLACE_LHS;
char **REPLACE_RHS;

int FULL_ALLOC=0;
nodeType_LIST ** FULL_PTRS=NULL;
int nodes_freed = 0;


char *GetSlash()
{
  if((USE_DOS==0)||(SLASHTYPE==NONDOS_TYPE))
  {
	return "/";
  }
  return "\\";
}


void RecordFileName(int type,char *filename)
{
  extern int global_gifd_id;
  static int saveid;

  

  if(type==1)
  {
	saveid = global_gifd_id+1;  
  }
  else if(type==2)
  {
    if(saveid<=global_gifd_id)
	{
      fprintf(filenames,"%d , %d %s\n",saveid,global_gifd_id,filename);
	}
  }
}


void WriteVersionFile()
{
  extern double MINZREAD,MAXZREAD,MINZREAD2,MAXZREAD2;

  fprintf(versionfile,"%s\n",VersionStr);
  fprintf(versionfile,"%d\n",ClassLevel);
  fprintf(versionfile,"%lf\n",MINZREAD);
  fprintf(versionfile,"%lf\n",MAXZREAD);
  fprintf(versionfile,"%lf\n",MINZREAD2);
  fprintf(versionfile,"%lf\n",MAXZREAD2);
}


void ReSetGridFile()
{
  extern int SH_Grids_Prev;


  
  fprintf(gridinfofile,"%d\n",num_grids-SH_Grids_Prev);




  NumGridFiles = NumGridFiles + 1;
  
  sprintf(GridBin, "%sgrid%d.bin",GlobalDest,NumGridFiles);
  

  

  fclose(gridbin);
  gridbin = fopen(GridBin, "wb");
  if(gridbin==NULL)
    {
      printf("gridbin: couldnt open %s for writing\n",GridBin);
      exit(-1);
    }

}


void ReSetArealFile()
{

  
  fprintf(arealinfofile,"%d\n",SH_Areals_Done-SH_Areals_Prev);




  NumArealFiles = NumArealFiles + 1;
  
  sprintf(ArealBinFile,"%sareals%d.bin",GlobalDest,NumArealFiles);
  

  fclose(arealbin);
  arealbin = fopen(ArealBinFile, "wb");
  if(arealbin==NULL)
    {
      printf("arealbin: couldnt open %s for writing\n",ArealBinFile);
      exit(-1);
    }

}





void ReSetLinearFile()
{

  
  fprintf(linearinfofile,"%d\n",SH_Linears_Done - SH_Linears_Prev);


  NumLinearFiles = NumLinearFiles + 1;
  
  sprintf(NetworkBinFile,"%slinears%d.bin",GlobalDest,NumLinearFiles);
 

  fclose(roadnetbin);
  roadnetbin = fopen(NetworkBinFile, "wb");
  if(roadnetbin==NULL)
    {
      printf("linebin: couldnt open %s for writing\n",NetworkBinFile);
      exit(-1);
    }

}





void WriteMultiFile()
{
  extern unsigned char OverLappingGrids[365][185];
  extern int SH_Grids_Prev,ATTRINSTYPE;
  int maxoverlap,i,j;


  fprintf(multipartfile,"%d\n%d\n%d\n",
	  NumMultiPartPointFeatures,
	  NumMultiPartLineFeatures,
	  NumMultiPartAreaFeatures);

  fprintf(multiinfofile,"%d\n",NumPointFiles);
  fprintf(multiinfofile,"%d\n",NumLinearFiles);
  fprintf(multiinfofile,"%d\n",NumArealFiles);
  fprintf(multiinfofile,"%d\n",NumGridFiles);
  fprintf(multiinfofile,"%d\n",NumCodeFiles);  


  fprintf(multiinfofile,"%d\n",NumMultiPartPointFeatures); 
  fprintf(multiinfofile,"%d\n",NumMultiPartLineFeatures);  
  fprintf(multiinfofile,"%d\n",NumMultiPartAreaFeatures);  
  fprintf(multiinfofile,"%d\n",NumPointParts);             
  fprintf(multiinfofile,"%d\n",NumLineParts);              
  fprintf(multiinfofile,"%d\n",NumAreaParts);              
  fprintf(multiinfofile,"%d\n",NumHoles);                  


  if(CREATING_TREX==1)
  {
    fprintf(multiinfofile,"5\n");
  }
  else
  {
    fprintf(multiinfofile,"%d\n",ATTRINSTYPE);               
  }


  fprintf(multiinfofile,"%s\n",SaveClassTable);            
  fprintf(multiinfofile,"%s\n",SaveClassValue);            


  if(NGA_TYPE==1)
    {
      fprintf(pointinfofile, "%d\n",SH_Points_Done);
      fprintf(linearinfofile,"%d\n",SH_Linears_Done-SH_Linears_Prev);
      fprintf(arealinfofile, "%d\n",SH_Areals_Done-SH_Areals_Prev);
      fprintf(gridinfofile,  "%d\n",num_grids - SH_Grids_Prev);
    }
  else
    {

      fprintf(pointinfofile, "%d\n",num_fmpoint+num_coll+num_par+
	      num_stamp+num_foot+num_points);
      fprintf(linearinfofile,"%d\n",num_linears+num_fmlinear);
      fprintf(arealinfofile, "%d\n",num_fmareal+num_areals);
      fprintf(gridinfofile,  "%d\n",num_grids);
    }
  
  fprintf(codeinfofile,  "%d\n",scc_long_items);
  
  maxoverlap = 0;
  for(i=0;i<365;i++)
    {
      for(j=0;j<185;j++)
	{
	  if(OverLappingGrids[i][j]>maxoverlap)
	    {
	      maxoverlap = OverLappingGrids[i][j];
	    }
	}
    }

  fprintf(gridinfofile,  "%d\n",maxoverlap);


}





void FreeEntry(struct SccNode *current)
{
  int i;
  
  for(i=0;i<current->numsac;i++)
    {
      if(current->sac[i].lod_stuff!=NULL)
	{
	  free(current->sac[i].lod_stuff);
	}
      if(current->sac[i].nam_stuff!=NULL)
	{
	  free(current->sac[i].nam_stuff);
	}
    }
  free(current->sac);
  
  free(current);
}




void L_FreeTree(nodeType_LIST *node)
{  
  if((node->left!=NILL)&&(node->right!=NILL))
    {
      L_FreeTree(node->left);
      L_FreeTree(node->right);
      nodes_freed++;
      free(node->key);
      FreeEntry(node->rec.node);
      free(node);
    } 
  else if(node->left!=NILL)
    {
      L_FreeTree(node->left);
      nodes_freed++;
      free(node->key);
      FreeEntry(node->rec.node);
      free(node);  
    }
  else if(node->right!=NILL)
    {
      L_FreeTree(node->right);
      nodes_freed++;
      free(node->key);
      FreeEntry(node->rec.node);
      free(node);
    }
  else if(node!=NILL)
    {
      nodes_freed++;
      free(node->key);
      FreeEntry(node->rec.node);
      free(node);
    }  
}


void E_FreeTree()
{
  if(FULL_ALLOC>0)
    {
      FULL_ALLOC = 0;
      free(FULL_PTRS);
    }
  L_FreeTree(L_root);
  L_root = NILL;
}




char *GetEMLabel(int attr_code)
{

  #if(NGA_TYPE==1)
  return "EM Label?";
  #else
  EDCS_Status_Code result;
  const EDCS_Value_Characteristic_Dictionary_Entry *result_out_ptr;
  static char badda[100];
  
  result = EDCS_GetEVDictionaryEntry(attr_code, &result_out_ptr);
  
  if(result!=EDCS_SC_SUCCESS)
    {
      printf("failed to get em label for EMC %d\n",attr_code);
      sprintf(badda,"no EM label available for EMC %d",attr_code);
      return badda;
    }
  return (char *)result_out_ptr->label;  
  #endif
}



char *GetEMDefinition(int attr_code)
{
  #if(NGA_TYPE==1)
  return "EM Def?";
  #else
  EDCS_Status_Code result;
  const EDCS_Value_Characteristic_Dictionary_Entry *result_out_ptr;
  static char badda[100];

  result = EDCS_GetEVDictionaryEntry(attr_code, &result_out_ptr);
  
  if(result!=EDCS_SC_SUCCESS)
    {
      printf("failed to get em definition for EMC %d\n",attr_code);
      sprintf(badda,"no EM definition available for EMC %d",attr_code);
      return badda;
    }
  return (char *)result_out_ptr->definition;
  #endif
}





char *GetEAEQclass(int code)
{
  #if(NGA_TYPE==1)
  return "EQ Label?";
  #else
  EDCS_Status_Code result;
  const EDCS_Attribute_Dictionary_Entry *result_out_ptr;
  const EDCS_Unit_Equivalence_Dictionary_Entry *result_out_ptr2;
  
  result = EDCS_GetEADictionaryEntry(code, &result_out_ptr);
  
  if(result!=EDCS_SC_SUCCESS)
    {
      printf("failed to get ea dict entry for EAC %d\n",code);
      return "no EQ label available";
    }
  
  
  result = EDCS_GetEQDictionaryEntry(result_out_ptr->eq_class, &result_out_ptr2);
  
  if(result!=EDCS_SC_SUCCESS)
    {
      return "no EQ label available";
    }
  return (char *)EDCS_PrintUnitEquivalenceLabel(result_out_ptr2->label);
  #endif
}



char *GetEAValueType(int code)
{
  #if(NGA_TYPE==1)
  return "EAV Type?";
  #else
  EDCS_Status_Code result;
  const EDCS_Attribute_Dictionary_Entry *result_out_ptr;
  
  result = EDCS_GetEADictionaryEntry(code, &result_out_ptr);
  
  if(result!=EDCS_SC_SUCCESS)
    {
      printf("failed to get ea dict entry for EAC %d\n",code);
      return "no abstract value type available";
    }
  
  return (char *)EDCS_PrintAttributeValueType(result_out_ptr->value_type);
  #endif
}




char *GetScaleLabel(int code)
{
  #if(NGA_TYPE==1)
  return "ES Label?";
  #else
  EDCS_Status_Code result;
  const EDCS_Unit_Scale_Dictionary_Entry *result_out_ptr;

  result = EDCS_GetESDictionaryEntry(code, &result_out_ptr);

  if(result!=EDCS_SC_SUCCESS)
    {
      printf("failed to get es dict entry for ESC %d\n",code);
      return "no Scale label available";
    }  
  return (char *)result_out_ptr->label;
  #endif
}



char *GetUnitsLabel(int code)
{
  #if(NGA_TYPE==1)
  return "EU Label?";
  #else
  EDCS_Status_Code result;
  const EDCS_Unit_Dictionary_Entry *result_out_ptr;

  result = EDCS_GetEUDictionaryEntry(code, &result_out_ptr);

  if(result!=EDCS_SC_SUCCESS)
    {
      printf("failed to get eu dict entry for EUC %d\n",code);
      return "no Units label available";
    }
  return (char *)result_out_ptr->label;
  #endif
}


char GetOpen(int i)
{
  
  if(i==EDCS_NVT_OPEN_INTERVAL)
    {
      return '(';
    }
  else if (i==EDCS_NVT_CLOSED_INTERVAL)    
    {
      return '[';
    }
  else if (i==EDCS_NVT_GE_LT_INTERVAL)
    {
      return '[';
    }
  else if (i==EDCS_NVT_GT_LE_INTERVAL)
    {
      return '(';
    }
  else if (i==EDCS_NVT_GT_SEMI_INTERVAL)
    {
      return '(';
    }
  else if (i==EDCS_NVT_GE_SEMI_INTERVAL)
    {
      return '[';
    }
  else if (i==EDCS_NVT_LT_SEMI_INTERVAL)
    {
      return '{';
    }
  else if (i==EDCS_NVT_LE_SEMI_INTERVAL)
    {
      return '{';
    }


  printf("unknown interval type %d\n",i);
  return '!';
}



char GetClose(int i)
{

  if(i==EDCS_NVT_OPEN_INTERVAL)
    {
      return ')';
    }
  else if (i==EDCS_NVT_CLOSED_INTERVAL)
    {
      return ']';
    }
  else if (i==EDCS_NVT_GE_LT_INTERVAL)
    {
      return ')';
    }
  else if (i==EDCS_NVT_GT_LE_INTERVAL)
    {
      return ']';
    }
  else if (i==EDCS_NVT_GT_SEMI_INTERVAL)
    {
      return '}';
    }
  else if (i==EDCS_NVT_GE_SEMI_INTERVAL)
    {
      return '}';
    }
  else if (i==EDCS_NVT_LT_SEMI_INTERVAL)
    {
      return ')';
    }
  else if (i==EDCS_NVT_LE_SEMI_INTERVAL)
    {
      return ']';
    }

  printf("unknown interval type %d\n",i);
  return '!';
}


void FreeShapes(int total)
{
  int i;

  for(i=0;i<total;i++)
  {
    free(longshapenames[i]);
    free(shortshapenames[i]);
  }

  free(longshapenames);
  free(shortshapenames);
  free(shapenums);

  longshapenames  = NULL;
  shortshapenames = NULL;
  shapenums       = NULL;
}


void FreeIgnore()
{
  int i;
  
  if(IGNORE_ATTR_NUM>0)
  {
    for(i=0;i<IGNORE_ATTR_NUM;i++)
	{
	  free(IGNORE_ATTR[i]);
	}    
    free(IGNORE_ATTR);
  }

  if(REPLACE_ATTR_NUM>0)
  {
    for(i=0;i<REPLACE_ATTR_NUM;i++)
	{
	  free(REPLACE_LHS[i]);
	  free(REPLACE_RHS[i]);
	}    
    free(REPLACE_LHS);
    free(REPLACE_RHS);
  }

  IGNORE_ATTR_NUM  = 0;
  REPLACE_ATTR_NUM = 0;
}




int PartitionShapes(int p,int r)
{
  char x[2000];
  char *temp;
  int i,j;

  
  sprintf(x,"%s",shortshapenames[p]);
  i = p-1;
  j = r+1;
  while(1)
    {
      do{j=j-1;}
      while(strcmp(shortshapenames[j],x)>0);
      
      do{i=i+1;}
      while(strcmp(shortshapenames[i],x)<0);
      
      if(i<j)
	{
	  
	  temp = longshapenames[i];
	  longshapenames[i] = longshapenames[j];
	  longshapenames[j] = temp; 


	  
	  temp = shortshapenames[i];
	  shortshapenames[i] = shortshapenames[j];
	  shortshapenames[j] = temp; 
	}
      else
	{
	  return j;
	}
    }
}


void SortShapes(int p,int r)
{
  
  int q;
  if(p<r)
    {
      q = PartitionShapes(p,r);
      SortShapes(p,q);
      SortShapes(q+1,r);
    }
}






int PartitionDTED(int p,int r)
{
  char x[2000];
  char *temp;
  int i,j;
  
  
  sprintf(x,"%s",dtedlayers[p]);
  i = p-1;
  j = r+1;
  while(1)
    {
      do{j=j-1;}
      while(strcmp(dtedlayers[j],x)>0);
      
      do{i=i+1;}
      while(strcmp(dtedlayers[i],x)<0);
      
      if(i<j)
	{
	  

	  temp = dtedlayers[i];
	  dtedlayers[i] = dtedlayers[j];
	  dtedlayers[j] = temp; 

	  temp = dtedlines[i];
	  dtedlines[i] = dtedlines[j];
	  dtedlines[j] = temp; 
	}
      else
	{
	  return j;
	}
    }
}


void SortDTEDLayers(int p,int r)
{
  
  int q;
  if(p<r)
    {
      q = PartitionDTED(p,r);
      SortDTEDLayers(p,q);
      SortDTEDLayers(q+1,r);
    }
}




void FreeFirstList()
{
  struct FirstList * curr;
  struct FirstList * prev;

  curr = ListOfFirsts;
  while(curr!=NULL)
    {
      prev = curr;
      curr = curr->next;
      free(prev->first);
      free(prev);
    }
  ListOfFirsts = NULL;
}


char *UnzipShapefile(char filename[],char dest[])
{
  char firstpart[1000],lastpart[1000],filepart[1000],cmd[1000],destination[1000];
  int i,len,lastcolon=-1,lastslash=-1;
  static char retval[1000];


  sprintf(destination,"%s",dest);
  len = strlen(dest);
  if((destination[len-1]=='/')||(destination[len-1]=='\\'))
  {
    destination[len-1] = '\0';
  }

  

  

  len = strlen(filename);
  for(i=0;i<len;i++)
  {
   if(filename[i]==';')
   {
     lastcolon = i;
   }
  }

  len = strlen(filename);
  for(i=0;i<len;i++)
  {
    if(USE_DOS==1)
      {
	if(filename[i]=='\\')
	  {
	    lastslash = i;
	  }
      }
    else
      {
	if(filename[i]=='/')
	  {
	    lastslash = i;
	  }
      }
  }

   if(lastcolon<0)
   {
     printf("UZ: error: couldn't find lastcolon for %s\n",filename);
     printf("unable to unzip shapefile %s from %s...ignoring\n",filename,destination);
	 return "";
   }
   if(lastslash<0)
   {
     printf("UZ: error: couldn't find lastslash for %s\n",filename);
     printf("unable to unzip shapefile %s from %s...ignoring\n",filename,destination);
	 return "";
   }

   sprintf(lastpart,"%s",&filename[lastcolon+1]);



   

   if(lastslash>lastcolon)
   {
     sprintf(filepart,"%s",&filename[lastslash+1]);
   }
   else
   {
     sprintf(filepart,"%s",&filename[lastcolon+1]);
   }

   sprintf(firstpart,"%s",filename);
   firstpart[lastcolon] = '\0';

   

   


   sprintf(cmd,"unzip -o -j -C \"%s\" \"%s.shp\" -d\"%s\" > NUL",firstpart,lastpart,destination);
   
   system(cmd);
   sprintf(cmd,"unzip -o -j -C \"%s\" \"%s.shx\" -d\"%s\" > NUL",firstpart,lastpart,destination);
   
   system(cmd);
   sprintf(cmd,"unzip -o -j -C \"%s\" \"%s.dbf\" -d\"%s\" > NUL",firstpart,lastpart,destination);
   
   system(cmd);

   if(USE_DOS==1)
   {
     sprintf(retval,"%s\\%s",destination,filepart);
   }
   else
   {
     sprintf(retval,"%s/%s",destination,filepart);
   }
   

  

   return retval;
}



char *ModifyFileName(char infilename[],char destination[])
{

  
     
  int i;
  int badfile=0;
  char filename[1000],command1[1000],command2[1000],command3[1000],command4[1000];
  FILE *openfile,*batchfile;
  static char retstring[1000];
  extern int CheckFileFCODEs(char *shape_to_import);

  for(i=0;i<(int)strlen(infilename);i++)
    {
      if(infilename[i]<0)
	{
	  badfile = 1;
	}
    }


  if(badfile==1)
    {

      

      if(USE_DOS==0)
	{
	  sprintf(filename,"%s/copiedfile.shp",destination);
	  openfile = fopen(filename,"r");
	  if(openfile!=NULL)
	    {
	      fclose(openfile);
	      sprintf(command1,"rm %scopiedfile.shp",destination);
	      sprintf(command2,"rm %scopiedfile.shx",destination);
	      sprintf(command3,"rm %scopiedfile.dbf",destination);
	      sprintf(command4,"rm -f ./batchexe.bat");
	      system(command1);
	      system(command2);
	      system(command3);
	      system(command4);
	    }
	}
      else
	{
	  sprintf(filename,"%s/copiedfile.shp",destination);
	  openfile = fopen(filename,"r");
	  if(openfile!=NULL)
	    {
	      fclose(openfile);
	      sprintf(command1,"del \"%scopiedfile.shp\"",destination);
	      sprintf(command2,"del \"%scopiedfile.shx\"",destination);
	      sprintf(command3,"del \"%scopiedfile.dbf\"",destination);
	      sprintf(command4,"del .\\batchexe.bat");
	      system(command1);
	      system(command2);
	      system(command3);
	      system(command4);
	    }
	}  
      

      

      batchfile = fopen("batchexe.bat","w");
      if(batchfile==NULL)
	{
	  printf("unable top open batchexe for write\n");
	  printf("ignoring file %s\n",infilename);
	  return "";
	}

      
      if((USE_DOS==0)||(SLASHTYPE==NONDOS_TYPE))
	{
	  fprintf(batchfile,"cp %s.shp %s/copiedfile.shp\n",infilename,destination);
	  fprintf(batchfile,"cp %s.shx %s/copiedfile.shx\n",infilename,destination);
	  fprintf(batchfile,"cp %s.dbf %s/copiedfile.dbf\n",infilename,destination);
	  fclose(batchfile);

	  system("chmod a+x ./batchexe.bat");
	  system("./batchexe.bat");
	  system("rm -f ./batchexe.bat");

	  


	
	  batchfile = fopen("batchexe.bat","w");
	  if(batchfile==NULL)
	    {
	      printf("unable to open batchexe for write\n");
	      printf("ignoring file %s\n",infilename);
	      return "";
	    }


	  sprintf(filename,"%s/copiedfile.shp",destination);
	  openfile = fopen(filename,"r");
	  if(openfile==NULL)
	    {
	      fprintf(batchfile,"cp %s.SHP %s/copiedfile.shp",infilename,destination);
	    }
	  else
	    {
	      fclose(openfile);
	    }


	  sprintf(filename,"%s/copiedfile.shx",destination);
	  openfile = fopen(filename,"r");
	  if(openfile==NULL)
	    {
	      fprintf(batchfile,"cp %s.SHX %s/copiedfile.shx",infilename,destination);
	    }
	  else
	    {
	      fclose(openfile);
	    }


	  sprintf(filename,"%s/copiedfile.shp",destination);
	  openfile = fopen(filename,"r");
	  if(openfile==NULL)
	    {
	      fprintf(batchfile,"cp %s.DBF %s/copiedfile.dbf",infilename,destination);
	    }
	  else
	    {
	      fclose(openfile);
	    }

	  fclose(batchfile);

	  system("chmod a+x ./batchexe.bat");
	  system("./batchexe.bat");
	  system("rm -f ./batchexe.bat");

	  sprintf(retstring,"%s/copiedfile",destination);
	}
      else
	{
	  fprintf(batchfile,"copy \"%s.shp\" \"%scopiedfile.shp\"\n",infilename,destination);
	  fprintf(batchfile,"copy \"%s.shx\" \"%scopiedfile.shx\"\n",infilename,destination);
	  fprintf(batchfile,"copy \"%s.dbf\" \"%scopiedfile.dbf\"\n",infilename,destination);
	  fclose(batchfile);

	  system(".\\batchexe.bat");
	  sprintf(retstring,"%scopiedfile",destination);
	}  



      


     

      if(CheckFileFCODEs(retstring)<0)
	{
	  return "";
	}
      

      printf("import diacritic file:   %s\n",infilename);
    }
  else
    {
      sprintf(retstring,"%s",infilename);
    }

  return retstring;
}


int DoNFI()
{
  
  return 1;
}


void openheaders()
{
  gridspacefile = fopen(GridSpaceTxt,"wt");
  headertxt     = fopen(HeaderTxt,   "wt");
  headerbin     = fopen(HeaderBin,   "wb");
  sizesbin      = fopen(SizesBin,    "wb");
  llmapbin      = fopen(LLmapBin,    "wb");
  fidmapbin     = fopen(FIDmapBin,   "wb");

  if(DoNFI())
  {
    nfnfile        =  fopen(NFNFile,     "wt");
  }

  savenames        =  fopen(SaveNameFile,"wt");


  badtrex  =  fopen(TREX_errs,"wt");

  if(CREATING_TREX==1)
  {
	badtrexheaders = fopen(TrexHeaderResults,"wt");
  }


  arealinfofile  =  fopen(ArealHeader ,"wt");
  linearinfofile =  fopen(LinearHeader,"wt");
  pointinfofile  =  fopen(PointHeader ,"wt");
  codeinfofile   =  fopen(CodeHeader  ,"wt");
  gridinfofile   =  fopen(SGridHeader ,"wt");
  multiinfofile  =  fopen(MultiHeader ,"wt");
  multipartfile  =  fopen(PartHeader  ,"wt");
  versionfile    =  fopen(VersionSt   ,"wt");

  multipartpoints  =  fopen(PartPoints ,"wb");
  multipartlines   =  fopen(PartLines  ,"wb");
  multipartareas   =  fopen(PartAreas  ,"wb");

  filenames        =  fopen(FileNames  ,"wt");


  if(NGA_TYPE==1)
    {
      badholetxtout = fopen(BadHoleTxt,"w");
      if(badholetxtout==NULL)
	{
	  printf("BadHoleTxt: couldnt open %s for writing\n",BadHoleTxt);
	  exit(-1);
	}


      badattrtxtout = fopen(BadAttrTxt,"w");
      if(badattrtxtout==NULL)
	{
	  printf("BadAttrTxt: couldnt open %s for writing\n",BadAttrTxt);
	  exit(-1);
	}

      nullshptxtout = fopen(NullShpTxt,"w");
      if(nullshptxtout==NULL)
	{
	  printf("nullshptxtout: couldnt open %s for writing\n",NullShpTxt);
	  exit(-1);
	}

      badunktxtout = fopen(BadUnkTxt,"w");
      if(badunktxtout==NULL)
	{
	  printf("badunktxtout: couldnt open %s for writing\n",BadUnkTxt);
	  exit(-1);
	}

      badGunktxtout = fopen(BadGUnkTxt,"w");
      if(badGunktxtout==NULL)
	{
	  printf("badGunktxtout: couldnt open %s for writing\n",BadGUnkTxt);
	  exit(-1);
	}


      Gblanktxtout = fopen(GBlankTxt,"w");
      if(Gblanktxtout==NULL)
	{
	  printf("Gblanktxtout: couldnt open %s for writing\n",GBlankTxt);
	  exit(-1);
	}


      emptyshapes = fopen(EmptyShapesTxt,"w");
      if(emptyshapes==NULL)
	{
	  printf("emptyshapes: couldnt open %s for writing\n",EmptyShapesTxt);
	  exit(-1);
	}


      badcasetxtout = fopen(BadCaseTxt,"w");
      if(badcasetxtout==NULL)
	{
	  printf("BadCaseTxt: couldnt open %s for writing\n",BadCaseTxt);
	  exit(-1);
	}

      badcoordout = fopen(BadCoordTxt,"w");
      if(badcoordout==NULL)
	{
	  printf("BadCoordTxt: couldnt open %s for writing\n",BadCoordTxt);
	  exit(-1);
	}




      badfcodeout = fopen(BadFCODE,"w");
      if(badfcodeout==NULL)
	{
	  printf("BadFCODE: couldnt open %s for writing\n",BadFCODE);
	  exit(-1);
	}

      nofcode = fopen(NoFCODE,"w");
      if(nofcode==NULL)
	{
	  printf("NoFCODE: couldnt open %s for writing\n",NoFCODE);
	  exit(-1);
	}
      fprintf(nofcode,"Shapefiles/Tables not containing an \"FCODE\" column:\n\n");
      

      utfout = fopen(UTFfile,"w");
      if(utfout==NULL)
	{
	  printf("UTF: couldnt open %s for writing\n",UTFfile);
	  exit(-1);
	}
      fprintf(utfout,"UTF-8 encoding anomalies:\n\n");

      
      badfilename = fopen(BadFilename,"w");
      if(badfilename==NULL)
	{
	  printf("BadFilename: couldnt open %s for writing\n",BadFilename);
	  exit(-1);
	}

    if(ATTRTYPE_ANY_MGCP())
	{
      fprintf(badfilename,"Shapefiles with file names not meeting MGCP ShapeFile Implementation Rules:\n\n");
	}
	else
	{
      fprintf(badfilename,"This inspection is only run for MGCP attribution schema projects\n\n");
	}

      mixnull = fopen(MixNull,"w");
      if(mixnull==NULL)
	{
	  printf("MixNull: couldnt open %s for writing\n",MixNull);
	  exit(-1);
	}

   
      badtypeout = fopen(BadTypeOut,"w");
      if(badtypeout==NULL)
	{
	  printf("BadTypeOut: couldnt open %s for writing\n",BadTypeOut);
	  exit(-1);
	}
   

      badlenout = fopen(BadLenOut,"w");
      if(badlenout==NULL)
	{
	  printf("BadLenOut: couldnt open %s for writing\n",BadLenOut);
	  exit(-1);
	}
   


    badshapetype = fopen(BadShapeType,"w");
      if(badshapetype==NULL)
	{
	  printf("badshapetype: couldnt open %s for writing\n",BadShapeType);
	  exit(-1);
	}


    badcurves = fopen(BadCurves,"w");
      if(badcurves==NULL)
	{
	  printf("badcurves: couldnt open %s for writing\n",BadCurves);
	  exit(-1);
	}

	nullattrval = fopen(NullAttrVal,"w");
    if(nullattrval==NULL)
	{
	  printf("nullattrval: couldnt open %s for writing\n",NullAttrVal);
	  exit(-1);
	}

	nullattrcsv = fopen(NullAttrCsv,"w");
    if(nullattrcsv==NULL)
	{
	  printf("nullattrcsv: couldnt open %s for writing\n",NullAttrCsv);
	  exit(-1);
	}

    fprintf(nullattrcsv,  "\"File\",\"Column\",\"Record\",\"Value\"\n");


	  

      

      badshapeout = fopen(BadShapes,"w");
      if(badshapeout==NULL)
	{
	  printf("BadShapes: couldnt open %s for writing\n",BadShapes);
	  exit(-1);
	}
    }




  if(multiinfofile==NULL)
    {
      printf("multiinfofile: couldnt open %s for writing\n",MultiHeader);
      exit(-1);
    }
  if(multipartfile==NULL)
    {
      printf("multipartfile: couldnt open %s for writing\n",PartHeader);
      exit(-1);
    }
  if(multipartpoints==NULL)
    {
      printf("multipartpoints: couldnt open %s for writing\n",PartPoints);
      exit(-1);
    }
  if(multipartlines==NULL)
    {
      printf("multipartlines: couldnt open %s for writing\n",PartLines);
      exit(-1);
    }
  if(multipartareas==NULL)
    {
      printf("multipartareas: couldnt open %s for writing\n",PartAreas);
      exit(-1);
    }
  if(versionfile==NULL)
    {
      printf("versionfile: couldnt open %s for writing\n",VersionSt);
      exit(-1);
    }
  if(arealinfofile==NULL)
    {
      printf("arealinfofile: couldnt open %s for writing\n",ArealHeader);
      exit(-1);
    }
  if(linearinfofile==NULL)
    {
      printf("linearinfofile: couldnt open %s for writing\n",LinearHeader);
      exit(-1);
    }
  if(pointinfofile==NULL)
    {
      printf("pointinfofile: couldnt open %s for writing\n",PointHeader);
      exit(-1);
    }
  if(codeinfofile==NULL)
    {
      printf("codeinfofile: couldnt open %s for writing\n",CodeHeader);
      exit(-1);
    }
  if(gridinfofile==NULL)
    {
      printf("gridinfofile: couldnt open %s for writing\n",SGridHeader);
      exit(-1);
    }


  if(headertxt==NULL)
    {
      printf("headertxt: couldnt open %s for writing\n",HeaderTxt);
      exit(-1);
    }
  if(gridspacefile==NULL)
    {
      printf("gridspacefile: couldnt open %s for writing\n",GridSpaceTxt);
      exit(-1);
    }
  if(headerbin==NULL)
    {
      printf("headerbin: couldnt open %s for writing\n",HeaderBin);
      exit(-1);
    }
  if(sizesbin==NULL)
    {
      printf("sizesbin: couldnt open %s for writing\n",SizesBin);
      exit(-1);
    }
  if(llmapbin==NULL)
    {
      printf("llmapbin: couldnt open %s for writing\n",LLmapBin);
      exit(-1);
    }
  if(fidmapbin==NULL)
    {
      printf("fidmapbin: couldnt open %s for writing\n",FIDmapBin);
      exit(-1);
    }
  if(filenames==NULL)
    {
      printf("filenames: couldnt open %s for writing\n",FileNames);
      exit(-1);
    }


  

  id1bin = fopen(Id1Bin,"wb");
  id2bin = fopen(Id2Bin,"wb");

  if(id1bin==NULL)
    {
      printf("idbin: couldnt open %s for writing\n",Id1Bin);
      exit(-1);
    }
  if(id2bin==NULL)
    {
      printf("id2bin: couldnt open %s for writing\n",Id2Bin);
      exit(-1);
    }




  area1bin = fopen(Area1Bin,"wb");
  area2bin = fopen(Area2Bin,"wb");

  if(area1bin==NULL)
    {
      printf("area1bin: couldnt open %s for writing\n",Area1Bin);
      exit(-1);
    }
  if(area2bin==NULL)
    {
      printf("area2bin: couldnt open %s for writing\n",Area2Bin);
      exit(-1);
    }



  hgt1bin = fopen(Hgt1Bin,"wb");
  hgt2bin = fopen(Hgt2Bin,"wb");

  if(hgt1bin==NULL)
    {
      printf("hgt1bin: couldnt open %s for writing\n",Hgt1Bin);
      exit(-1);
    }
  if(hgt2bin==NULL)
    {
      printf("hgt2bin: couldnt open %s for writing\n",Hgt2Bin);
      exit(-1);
    }



  len1bin = fopen(Len1Bin,"wb");
  len2bin = fopen(Len2Bin,"wb");

  if(len1bin==NULL)
    {
      printf("len1bin: couldnt open %s for writing\n",Len1Bin);
      exit(-1);
    }
  if(len2bin==NULL)
    {
      printf("len2bin: couldnt open %s for writing\n",Len2Bin);
      exit(-1);
    }



  nam1bin = fopen(Nam1Bin,"wb");
  nam2bin = fopen(Nam2Bin,"wb");

  if(nam1bin==NULL)
    {
      printf("nam1bin: couldnt open %s for writing\n",Nam1Bin);
      exit(-1);
    }
  if(nam2bin==NULL)
    {
      printf("nam2bin: couldnt open %s for writing\n",Nam2Bin);
      exit(-1);
    }




  zv1bin = fopen(Zv1Bin,"wb");
  zv2bin = fopen(Zv2Bin,"wb");

  if(zv1bin==NULL)
    {
      printf("zv1bin: couldnt open %s for writing\n",Zv1Bin);
      exit(-1);
    }
  if(zv2bin==NULL)
    {
      printf("zv2bin: couldnt open %s for writing\n",Zv2Bin);
      exit(-1);
    }

}











int PartitionbyNFI(int p,int r,int dragheader)
{
  char x[500];
  char *temp;
  int i,j;

  sprintf(x,"%s",sortkeys[p]);
  i = p-1;
  j = r+1;
  while(1)
    {
      do{j=j-1;}
      while(strcmp(sortkeys[j],x)>0);

      do{i=i+1;}
      while(strcmp(sortkeys[i],x)<0);
    
      if(i<j)
	{
	  temp = sortkeys[i];
	  sortkeys[i] = sortkeys[j];
	  sortkeys[j] = temp;

	  if(dragheader>0)
	  {
	    
	    temp = headers[i];
	    headers[i] = headers[j];
	    headers[j] = temp;
	  } 
	}
      else
	{
	  return j;
	}
    }
}



int PartitionbyHeader(int p,int r)
{
  char x[500];
  char *temp;
  int i,j;

  sprintf(x,"%s",headers[p]);
  i = p-1;
  j = r+1;
  while(1)
    {
      do{j=j-1;}
      while(strcmp(headers[j],x)>0);

      do{i=i+1;}
      while(strcmp(headers[i],x)<0);
    
      if(i<j)
	{
	  temp = headers[i];
	  headers[i] = headers[j];
	  headers[j] = temp;
	}
      else
	{
	  return j;
	}
    }
}




void SortListbyNFI(int p,int r, int dragheader)
{
  int q;

  
  if(p<r)
    {
      q = PartitionbyNFI(p,r,dragheader);
      SortListbyNFI(p,q,dragheader);
      SortListbyNFI(q+1,r,dragheader);
    }
}



void SortHeaders(int p,int r)
{
  int q;

  
  if(p<r)
    {
      q = PartitionbyHeader(p,r);
      SortHeaders(p,q);
      SortHeaders(q+1,r);
    }
}


void ProcessNames()
{
  FILE *savenames2;
  char aline[1000];
  int i,j,k,len,lastquote,thislen,foundc,maxc=0,maxlen=0,numlines=0,insertindex=0,numsame=0,totalcount=0;
  int *sortcounts;
  extern void AddHTML(FILE *outfile,char *outstring);
  extern int numsavednames;

  
  


  sprintf(aline, "%s.html",SaveNameFile);
  
  savenames   =  fopen(SaveNameFile, "rt");
  if(savenames==NULL)
  {
    printf("Error: cannot open %s for read\n",SaveNameFile);
	return;
  }

  savenames2  =  fopen(aline, "wt");
  if(savenames2==NULL)
  {
    printf("Error: cannot open %s for write\n",aline);
	return;
  }



  if(numsavednames<1)
  {
	fclose(savenames);

    AddHTML(savenames2,"<html lang=\"en\">\n");
    AddHTML(savenames2," <head>\n");
    AddHTML(savenames2,"  <title>List of names in this data set</title>\n");
    AddHTML(savenames2,"  <style type=\"text/css\">\n");
    AddHTML(savenames2,"   pre { margin-bottom: 1em; padding-bottom: 1em; border-bottom: solid thin; }\n");
    AddHTML(savenames2,"  </style>\n");
    AddHTML(savenames2," </head>\n");
    AddHTML(savenames2," <body>\n");

    AddHTML(savenames2," <pre>\n");
    AddHTML(savenames2,"List of names in this data set:\n");
    AddHTML(savenames2," </pre>\n");
    AddHTML(savenames2," <pre>\n");

    AddHTML(savenames2,"None\n");

    AddHTML(savenames2," </pre>\n");
    AddHTML(savenames2," </body>\n");
    AddHTML(savenames2,"</html>\n");

	fclose(savenames2);

    return;
  }



  fgets(aline,1000,savenames);
  while(!feof(savenames))
  {
    thislen = strlen(aline) + 2;
	if(thislen>maxlen)
	{
      maxlen = thislen;
      
	}


	for(i=0;i<thislen;i++)
	{
      if(aline[i]==':')
	  {
		if(i>maxc)
		{
          maxc = i;
		}
		break;
	  }
	}

    numlines = numlines + 1;
    fgets(aline,1000,savenames);
  }

  


  if(numlines>NAM_HTML_LIMIT)
  {
	numlines = NAM_HTML_LIMIT;
  }


  sortkeys = (char **)malloc(sizeof(char *)*numlines);
  sortcounts = (int *)malloc(sizeof(int)*numlines);

  rewind(savenames);


  fgets(aline,1000,savenames);
  while(!feof(savenames))
  {
	sortkeys[insertindex]= (char *)malloc(strlen(aline)+5);
	sprintf(sortkeys[insertindex],"%s",aline);

	insertindex = insertindex + 1;
    fgets(aline,1000,savenames);
  }

  fclose(savenames);




  SortListbyNFI(0,numlines-1,0);


  AddHTML(savenames2,"<html lang=\"en\">\n");
  AddHTML(savenames2," <head>\n");
  AddHTML(savenames2,"  <title>List of names in this data set</title>\n");
  AddHTML(savenames2,"  <style type=\"text/css\">\n");
  AddHTML(savenames2,"   pre { margin-bottom: 1em; padding-bottom: 1em; border-bottom: solid thin; }\n");
  AddHTML(savenames2,"  </style>\n");
  AddHTML(savenames2," </head>\n");
  AddHTML(savenames2," <body>\n");

  AddHTML(savenames2," <pre>\n");
  AddHTML(savenames2,"List of names in this data set:\n");
  AddHTML(savenames2," </pre>\n");
  AddHTML(savenames2," <pre>\n");




  for(i=0;i<numlines;i++)
  {
	
	
	

	lastquote = 0;
	len = strlen(sortkeys[i]);
	for(j=len;j>0;j--)
	{
	  if(sortkeys[i][j]=='"')
	  {
        lastquote = j;
		break;
	  }
	}

	if(lastquote<=0)
	{
      printf("Err: could not find LQ in %s\n",sortkeys[i]);
	}

	

	sprintf(aline,"%s",&sortkeys[i][lastquote+1]);

	

    sscanf(aline,"%d",&sortcounts[i]);

	

	sortkeys[i][lastquote+1] = '\n';
	sortkeys[i][lastquote+2] = '\0';

    
  }


  for(i=0;i<numlines;i++)
  {
	

	lastquote = 0;
	len = strlen(sortkeys[i]);
	for(j=len;j>0;j--)
	{
	  if(sortkeys[i][j]=='"')
	  {
        lastquote = j;
		break;
	  }
	}

	if(lastquote<=0)
	{
      printf("Err: could not find LQ in %s\n",sortkeys[i]);
	  continue;
	}

	

	if(numsame>1)
	{
	  
	  numsame = numsame - 1;
      continue;
	}


    
	numsame = 1;
	totalcount = sortcounts[i];
	for(j=i+1;j<numlines;j++)
	{
	  if(!strcmp(sortkeys[i],sortkeys[j]))
	  {
		numsame = numsame + 1;
		totalcount = totalcount + sortcounts[j];
	  }
	  else
	  {
        break;
	  }
	}


	thislen = strlen(sortkeys[i]);

	


	foundc = 0;
	for(j=0;j<thislen;j++)
	{
	  if(j==thislen-1)
	  {
	    fprintf(savenames2," (%d)",totalcount);
	  }

      fprintf(savenames2,"%c",sortkeys[i][j]);

	  if((foundc==0)&&(sortkeys[i][j]==':'))
	  {
		for(k=j;k<maxc+5;k++)
		{
		  fprintf(savenames2,".");
		}
        foundc = 1;
	  }
	}
  }

  if(numlines>=NAM_HTML_LIMIT)
  {
    AddHTML(savenames2," <pre>\n");

	


	sprintf(aline,"***Name list limit reached***\n"); 


    AddHTML(savenames2,aline);
    AddHTML(savenames2," </pre>\n");
  }

  AddHTML(savenames2," </pre>\n");
  AddHTML(savenames2," </body>\n");
  AddHTML(savenames2,"</html>\n");


  fclose(savenames2);



  



  

  for(i=0;i<numlines;i++)
  {
    free(sortkeys[i]);
  }
  free(sortkeys);
  free(sortcounts);
}



char *GetQuoted(char *instring,int quotenum)
{
  int i,len,quotefound=0,foundit=0;
  static char retval[500];
  
  quotenum = 2*quotenum -1; 

  len = strlen(instring);

  for(i=0;i<len;i++)
  {
    if(instring[i]=='"')
	{
      quotefound = quotefound + 1;
	  if(quotefound==quotenum)
	  {
        foundit = i+1;
		break;
	  }
	}
  }


  if(foundit==0)
  {
    printf("Error: CFQ %d in %s\n",quotenum,instring);
	return "Error";
  }

  for(i=foundit;i<len;i++)
  {
    if(instring[i]=='"')
	{
	  retval[i-foundit] = '\0';
	  return retval;
	}
	retval[i-foundit] = instring[i];
  }

  printf("Error: CFQ2 %d in %s\n",quotenum,instring);
  return "Error2";
}



void ProcessNFN()
{
  FILE *nfnfile2,*nfnfile3,*nfnfile4;
  int i,j,thislen,insertindex,numquote,totallen,secondlen,realindex1,headerlen,numquotes,
	  startindex,maxlen=0,numlines = 0;
  char aline[1000],lastNFI[500],lastNFN[500],lastNAM[500],thisNFI[500],thisNFN[500],thisNAM[500],
	  fname1[500],fname2[500],geom1[10],geom2[10],nfi[500],nfn[500],nam1[500],
      lastPnfi[500],lastPnfn[500],lastPnam[500],lastPfname[500];
  char *index1;
  
  

  


  nfnfile   =  fopen(NFNFile,     "rt");
  if(nfnfile==NULL)
  {
    printf("Error: cannot open %s for read\n",NFNFile);
	return;
  }


  sprintf(aline, "%s2",NFNFile);
  nfnfile2  =  fopen(aline,     "wt");
  if(nfnfile2==NULL)
  {
    printf("Error: cannot open %s for write\n",aline);
	return;
  }

  sprintf(aline, "%s3",NFNFile);
  nfnfile3  =  fopen(aline,     "wt");
  if(nfnfile3==NULL)
  {
    printf("Error: cannot open %s for write\n",aline);
	return;
  }

  sprintf(aline, "%s4",NFNFile);
  nfnfile4  =  fopen(aline,     "wt");
  if(nfnfile4==NULL)
  {
    printf("Error: cannot open %s for write\n",aline);
	return;
  }



  

  rewind(nfnfile);

  fgets(aline,1000,nfnfile);
  while(!feof(nfnfile))
  {
    thislen = strlen(aline) + 2;
	if(thislen>maxlen)
	{
      maxlen = thislen;
	  
	}
    numlines = numlines + 1;
    fgets(aline,1000,nfnfile);
  }

  


  headers  = (char **)malloc(sizeof(char *)*numlines);
  sortkeys = (char **)malloc(sizeof(char *)*numlines);

  rewind(nfnfile);

  insertindex = 0;
  fgets(aline,1000,nfnfile);
  while(!feof(nfnfile))
  {
    

	thislen = strlen(aline);
	if(aline[thislen-1]<32)
	{
      aline[thislen-1] = '\0';
	}
	if(aline[thislen-2]<32)
	{
      aline[thislen-2] = '\0';
	}
	thislen = strlen(aline);

	numquote = 0;
	for(i=0;i<thislen;i++)
	{
      if(aline[i]=='"')
	  {
        numquote = numquote + 1;
	  }
	  if(numquote==4)
	  {
		break;
	  }
	}

	headers[insertindex]= (char *)malloc(i+5);
	strncpy(headers[insertindex],aline,i+1);
	headers[insertindex][i+1] = '\0';
	
	sortkeys[insertindex]= (char *)malloc(thislen-i+5);
	sprintf(sortkeys[insertindex],"%s",&aline[i+2]);

	insertindex = insertindex + 1;
    fgets(aline,1000,nfnfile);
  }

  

  SortListbyNFI(0,numlines-1,1);


  

  startindex = 0;
  for(i=1;i<numlines;i++)
  {
    if(strcmp(sortkeys[i],sortkeys[startindex]))
	{
      
	  if( (i-1)>(startindex) )
	  {
        
		SortHeaders(startindex,i-1);
	  }
      startindex = i;
	}

    else if(i==(numlines-1))
	{
      

      
	  SortHeaders(startindex,i);
	}

  }


  


  


  

  sprintf(lastPnfi,  "1^-!=.,fg");
  sprintf(lastPnfn,  "1^-!=.,fg");
  sprintf(lastPnam,  "1^-!=.,fg");
  sprintf(lastPfname,"1^-!=.,fg");


  for(i=0;i<numlines;i++)
  {

	

	if(i>0)
	{
      if(!strcmp(sortkeys[i],sortkeys[i-1]))
	  {
        

	    sprintf(fname1,"%s",GetQuoted(headers[i-1],1));
	    sprintf(fname2,"%s",GetQuoted(headers[i],1));
	    sprintf(geom1,"%s",GetQuoted(headers[i-1],2));
	    sprintf(geom2,"%s",GetQuoted(headers[i],2));

	    sprintf(nfi,"%s",GetQuoted(sortkeys[i-1],1));
	    sprintf(nfn,"%s",GetQuoted(sortkeys[i],2));
	    sprintf(nam1,"%s",GetQuoted(sortkeys[i-1],3));

		

		if(strcmp(fname1,fname2))
		{
		  

		  fprintf(nfnfile3,"%s\n%s\n%s\n%s\n",
			  fname1,geom1,fname2,geom2);

		  fprintf(nfnfile3,"\"%s\"\n\"%s\"\n\"%s\"\n\"%s\"\n",
			  nfi,nfn,nam1,nam1);
		}

		else if(!strcmp(geom1,geom2))
		{
		  if(!strcmp(geom1,"5"))
		  {
			

			if(
				(strcmp(lastPnfi,nfi))      ||
				(strcmp(lastPnfn,nfn))      || 
				(strcmp(lastPnam,nam1))     ||
				(strcmp(lastPfname,fname1))
			  )
			{
			  

		      fprintf(nfnfile4,"%s\n%s\n%s\n%s\n",
			    fname1,geom1,fname2,geom2);

		      fprintf(nfnfile4,"\"%s\"\n\"%s\"\n\"%s\"\n\"%s\"\n",
			    nfi,nfn,nam1,nam1);

			  sprintf(lastPnfi,  "%s",nfi);
			  sprintf(lastPnfn,  "%s",nfn);    
			  sprintf(lastPnam,  "%s",nam1);
			  sprintf(lastPfname,"%s",fname1);
			}
		  } 
		}
	  } 
	}


    

	index1 = strstr(sortkeys[i],"**!!!!**");

    totallen = strlen(sortkeys[i]);
	secondlen = strlen(index1);

	realindex1 = totallen - secondlen;

    

	strncpy(thisNFI,sortkeys[i],realindex1);
	thisNFI[realindex1] = '\0';

	sprintf(aline,"%s",&sortkeys[i][realindex1+9]); 

	


	index1 = strstr(aline,"**!!!!**");

    totallen = strlen(aline);
	secondlen = strlen(index1);

	realindex1 = totallen - secondlen;

    

	strncpy(thisNFN,aline,realindex1);
	thisNFN[realindex1] = '\0';

	sprintf(thisNAM,"%s",&aline[realindex1+9]); 

	

    if(i>0)
	{
	  if(
		  (!strcmp(thisNFI,lastNFI)) &&
		  (!strcmp(thisNFN,lastNFN))
		)
	  {
		if(strcmp(lastNAM,thisNAM))
		{
		  


		  


		  headerlen = strlen(headers[i-1]);
		  numquotes = 1;
          for(j=1;j<headerlen;j++)
		  {
            if(headers[i-1][j]!='"')
			{
              fprintf(nfnfile2,"%c",headers[i-1][j]);
			}
			else if(numquotes==3)
			{
              fprintf(nfnfile2,"\n");
			  break;
			}
			else if(numquotes==1)
			{
              fprintf(nfnfile2,"\n");
              numquotes = 3;
			  j = j + 2; 
			}
		  }


		  headerlen = strlen(headers[i]);
		  numquotes = 1;
          for(j=1;j<headerlen;j++)
		  {
            if(headers[i][j]!='"')
			{
              fprintf(nfnfile2,"%c",headers[i][j]);
			}
			else if(numquotes==3)
			{
              fprintf(nfnfile2,"\n");
			  break;
			}
			else if(numquotes==1)
			{
              fprintf(nfnfile2,"\n");
              numquotes = 3;
			  j = j + 2; 
			}
		  }


		  fprintf(nfnfile2,"%s\n",lastNFI);
		  fprintf(nfnfile2,"%s\n",lastNFN);
		  fprintf(nfnfile2,"%s\n",lastNAM);
		  fprintf(nfnfile2,"%s\n",thisNAM);
		}
	  }
	}

	sprintf(lastNFI,"%s",thisNFI);
    sprintf(lastNFN,"%s",thisNFN);
    sprintf(lastNAM,"%s",thisNAM);

  }


  for(i=0;i<numlines;i++)
  {
  	free(headers[i]);
  	free(sortkeys[i]);
  }

  free(headers);
  free(sortkeys);

  fclose(nfnfile);
  fclose(nfnfile2);
  fclose(nfnfile3);
  fclose(nfnfile4);
}





void closeheaders()
{
  char ttype[200];
  extern void MakeGunkTxt();
  extern void NFIWrapper(char *saveNFI, char *saveNFN, char *saveNAM, int classcode, int geomtype);
  extern char *GetTRExType(char *projectpath);


  fclose(gridspacefile);
  fclose(headertxt);
  fclose(headerbin); 
  fclose(llmapbin);
  fclose(fidmapbin);

  if(DoNFI())
  {
    fclose(nfnfile);
    ProcessNFN();
  }


  
  NFIWrapper("ignore","ignore","ignore",123456789,123456789);

  fclose(savenames);


  if((goodGCP1+goodGCP2)>0)
  {
    fprintf(badtrex,"Control Point Import Summary:\n\n");

	if(goodGCP1>0)
	{
      fprintf(badtrex,"Imported %d NGA Ground Control Points (GCP)\n",goodGCP1);
	}
	if(goodGCP2>0)
	{
      fprintf(badtrex,"Imported %d ICESAT Points\n",goodGCP2);
	}
  }


  if(CREATING_TREX==1)
  {
	sprintf(ttype,"%s",GetTRExType(importoutputdir));

    if(foundTREXraw==0)
	{ 
	  if(foundrawfolder==0)
	  { 
		if( (!strcmp(ttype,"QC")) || (!strcmp(ttype,"cQA")) )
		{
          fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
          fprintf(badtrex,"IMPORT_SENTINEL   Did not find TREx Raw folder in package with analysis type %s\n",ttype);
		}
	  } 
	  else
	  { 
        fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
        fprintf(badtrex,"IMPORT_SENTINEL   Did not find TREx Raw DEM in package, but did find a TREx Raw DEM folder\n");
	  } 
	} 
    if(foundTREXfinished==0)
	{ 
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Did not find TREx Finished DEM in project\n");
	} 
    if(badGCP>0)
	{ 
      fprintf(badtrex,"IMPORT_SENTINEL Error:\n");
      fprintf(badtrex,"IMPORT_SENTINEL   Failed to import %d Ground Control Points\n",badGCP);
	} 

    

	fclose(badtrexheaders);
  }




  ProcessNames();


  fclose(arealinfofile)  ;
  fclose(linearinfofile) ;
  fclose(pointinfofile)  ;
  fclose(codeinfofile)   ;
  fclose(gridinfofile)   ;
  fclose(multiinfofile)  ;
  fclose(multipartfile)  ;
  fclose(multipartpoints);
  fclose(multipartlines) ;
  fclose(multipartareas) ;
  fclose(versionfile)    ;
  fclose(filenames)      ;

  
  if(NGA_TYPE==1)
    {
      MakeGunkTxt();
      
      fprintf(badholetxtout,"\n");
      fprintf(badattrtxtout,"\n");
      fprintf(nullshptxtout,"\n");
      fprintf(badcasetxtout,"\n");
      fprintf(badunktxtout, "\n");
      fprintf(badGunktxtout,"\n");
      fprintf(Gblanktxtout, "\n");
      fprintf(badfcodeout,  "\n");
      fprintf(badshapeout,  "\n");
      fprintf(badcurves,    "\n");
      fprintf(badcoordout,  "\n");
      fprintf(nofcode,      "\n");
      fprintf(utfout,       "\n");
      fprintf(badfilename,  "\n");
      fprintf(mixnull,      "\n");
      fprintf(badtypeout,   "\n");
      fprintf(badlenout,    "\n");
      fprintf(badshapetype, "\n");
      fprintf(nullattrval,  "\n");


      fclose(badholetxtout);
      fclose(badattrtxtout);
      fclose(nullshptxtout);
      fclose(badcasetxtout);
      fclose(badunktxtout);
      fclose(badGunktxtout);
      fclose(Gblanktxtout);
      fclose(emptyshapes);
      fclose(badfcodeout);
      fclose(badshapeout);
      fclose(badcurves);
      fclose(badcoordout);
      fclose(nofcode);
      fclose(utfout);
      fclose(badfilename);
      fclose(mixnull);
      fclose(badlenout);
      fclose(badtypeout);
      fclose(badshapetype);
      fclose(nullattrval);
      fclose(nullattrcsv);

  }


  fclose(id1bin); 
  fclose(id2bin);  
  fclose(area1bin); 
  fclose(area2bin); 
  fclose(hgt1bin); 
  fclose(hgt2bin); 
  fclose(len1bin); 
  fclose(len2bin); 
  fclose(nam1bin); 
  fclose(nam2bin); 
  fclose(zv1bin); 
  fclose(zv2bin); 
  fclose(sizesbin);
}






void openmods()
{
  
  modelbin = fopen(ModelBinFile, "wb");
  if(modelbin==NULL)
    {
      printf("ModelBinFile: couldnt open %s for writing\n",ModelBinFile);
      exit(-1);
    }
  
  bridgebin = fopen(BridgeBinFile, "wb");
  if(bridgebin==NULL)
    {
      printf("BridgeBinFile: couldnt open %s for writing\n",BridgeBinFile);
      exit(-1);
    }


  if(DOMODELSUMMARY==1)
    {
      modelsummaryout = fopen(ModelDumpTxt,"wt");
      if(modelsummaryout==NULL)
	{
	  printf("modelsummaryout: couldnt open %s for writing\n",ModelDumpTxt);
	  exit(-1);
	}
    }
}


void closemods()
{
  fclose(modelbin);
  fclose(bridgebin);

  if(DOMODELSUMMARY==1)
    {
      fclose(modelsummaryout);
    }

}


void openareals()
{
  
  arealbin = fopen(ArealBinFile, "wb");
  if(arealbin==NULL)
    {
      printf("ArealBinFile: couldnt open %s for writing\n",ArealBinFile);
      exit(-1);
    }
}


void closeareals()
{
  fclose(arealbin);
}


void opennets()
{
  roadnetbin = fopen(NetworkBinFile,"wb");
  if(roadnetbin==NULL)
    {
      printf("NetworkBinFile: couldnt open %s for writing\n",NetworkBinFile);
      exit(-1);
    }
}


void closenets()
{
  fclose(roadnetbin);
}


void opengrid()
{
  gridbin = fopen(GridBin,"wb");
  if(gridbin==NULL)
    {
      printf("GridBin: couldnt open %s for writing\n",GridBin);
      exit(-1);
    }
}


void closegrid()
{
  fclose(gridbin);
}


void openpolys()
{
  
  polysbin = fopen(PolygonBinFile, "wb");
  if(polysbin==NULL)
    {
      printf("PolygonBinFile: couldnt open %s for writing\n",PolygonBinFile);
      exit(-1);
    }

  modelpolysbin = fopen(ModelPolygonBinFile, "wb");
  if(modelpolysbin==NULL)
    {
      printf("ModelPolygonBinFile: couldnt open %s for writing\n",ModelPolygonBinFile);
      exit(-1);
    }
}


void closepolys()
{
  fclose(polysbin);
  fclose(modelpolysbin);
}



void RecordTrexType(char *temp)
{
  FILE *tempfile;
  char errmsg[100];
  extern int TREX_TYPE;

  tempfile = fopen(temp,"w");
  if(tempfile==NULL)
  {
	printf("fatal error: unable to open %s for writing\n",temp);
	exit(-1);
  }

  if(TREX_TYPE==0)
  {
	fprintf(tempfile,"Not Specified");
  }
  else if(TREX_TYPE==1)
  {
	fprintf(tempfile,"QC");
  }
  else if(TREX_TYPE==2)
  {
	fprintf(tempfile,"cQA");
  }
  else if(TREX_TYPE==3)
  {
	fprintf(tempfile,"sQA");
  }
  else
  {
	sprintf(errmsg,"Error:%d",TREX_TYPE);
	fprintf(tempfile,"%s",errmsg);
  }
  fclose(tempfile);
}


void MakeFileNames(char dirname[])
{  
  FILE *tempfile;
  char temp[1000];
  char outdirname[1000];
  unsigned char cjunk;
  extern int ATTRIBUTION_TYPE;
  

  if(NGA_TYPE==1)
    {
      

      sprintf(temp,"%sattrsch.bin",dirname);
      tempfile = fopen(temp,"wb");
      if(tempfile==NULL)
	{
	  printf("fatal error: unable to open %s for writing\n",temp);
	  exit(-1);
	}
      cjunk = (unsigned char)ATTRIBUTION_TYPE;
      fwrite(&cjunk,  sizeof(unsigned char) ,1,tempfile);
      fclose(tempfile);
    }


  sprintf(temp,     "%sinternal_data%stimp2.bin",dirname,GetSlash());
  RecordTrexType(temp);

  sprintf(outdirname,"%sattribution_errors%s",dirname,GetSlash());
  sprintf(SaveNameFile,"%ssummary_files%snames",dirname,GetSlash());
  sprintf(TREX_errs,     "%sinternal_data%stimp.bin",dirname,GetSlash());
  sprintf(TrexXMLresultstub, "%sXML%sTREx_",dirname,GetSlash());
  sprintf(TrexHeaderResults, "%ssummary_files%sTREx_header_errors.txt",dirname,GetSlash());







  sprintf(NetworkBinFile,     "%slinears.bin",dirname);
  sprintf(ModelBinFile,       "%spoints.bin",dirname);
  sprintf(ArealBinFile,       "%sareals.bin",dirname);
  sprintf(NFNFile,            "%snfn.sf",dirname);
  sprintf(BridgeBinFile,      "%sbridges.bin",dirname);
  sprintf(ModelPolygonBinFile,"%smodelpolys.bin",dirname);
  sprintf(PolygonBinFile,     "%spolys.bin",dirname);
  sprintf(GridSpaceTxt,       "%sgridspace.txt",dirname);
  sprintf(HeaderTxt,          "%sheader.txt",dirname);
  sprintf(HeaderBin,          "%sheader.bin",dirname);
  sprintf(BadHoleTxt,         "%sbadhole.txt",outdirname);
  sprintf(BadAttrTxt,         "%sbadattr.txt",outdirname);
  sprintf(NullShpTxt,         "%snullshp.txt",outdirname);
  sprintf(EmptyShapesTxt,     "%semptyshapes.txt",outdirname);
  sprintf(BadGUnkTxt,         "%sunkcolumnG.txt",outdirname);
  sprintf(GBlankTxt,          "%sblankG.txt",outdirname);
  sprintf(BadUnkTxt,          "%sunkcolumn.txt",outdirname);
  sprintf(BadCaseTxt,         "%sbadcase.txt",outdirname);
  sprintf(BadCoordTxt,        "%sbadcoord.txt",outdirname);
  sprintf(BadShapeType,       "%sbadshapetype.txt",outdirname);
  sprintf(BadCurves,          "%sbadcurves.txt",outdirname);
  sprintf(NullAttrVal,        "%sblankattrval.txt",outdirname);
  sprintf(NullAttrCsv,        "%sblankattrval.csv",outdirname);
  sprintf(BadFCODE,           "%sbadfcode.txt",outdirname);
  sprintf(NoFCODE,            "%snofcodecolumn.txt",outdirname);
  sprintf(UTFfile,            "%sbadutf.txt",outdirname);
  sprintf(BadFilename,        "%sbadmgcpfilename.txt",outdirname);
  sprintf(MixNull,            "%smgcpmixednull.txt",outdirname);
  sprintf(BadLenOut,          "%sbadlen.txt",outdirname);
  sprintf(BadTypeOut,         "%sbadtype.txt",outdirname);
  sprintf(BadShapes,          "%sbadshapes.txt",outdirname);
  sprintf(Id1Bin,             "%sidlookup1.bin",dirname);
  sprintf(Id2Bin,             "%sidlookup2.bin",dirname);
  sprintf(Area1Bin,           "%sarealookup1.bin",dirname);
  sprintf(Area2Bin,           "%sarealookup2.bin",dirname);
  sprintf(Hgt1Bin,            "%shgtlookup1.bin",dirname);
  sprintf(Hgt2Bin,            "%shgtlookup2.bin",dirname);
  sprintf(Len1Bin,            "%slenlookup1.bin",dirname);
  sprintf(Len2Bin,            "%slenlookup2.bin",dirname);
  sprintf(Nam1Bin,            "%snamlookup1.bin",dirname);
  sprintf(Nam2Bin,            "%snamlookup2.bin",dirname);
  sprintf(Zv1Bin,             "%szvlookup1.bin",dirname);
  sprintf(Zv2Bin,             "%szvlookup2.bin",dirname);
  sprintf(codesoutfull,       "%scodesfull.txt",dirname);
  sprintf(codesoutshort,      "%scodesshort.txt",dirname);
  sprintf(codesoutfullbin,    "%scodesfull.bin",dirname);
  sprintf(codesoutshortbin,   "%scodesshort.bin",dirname);
  sprintf(GridBin,            "%sgrid.bin",dirname);
  sprintf(ModelDumpTxt ,      "%smodelsummary.txt" ,dirname);
  sprintf(ModelDumpTxt2,      "%smodelsummary2.txt",dirname);
  sprintf(ModelDumpTxt3,      "%smodelsummary3.txt",dirname);
  sprintf(SizesBin,           "%ssizes.bin",dirname);
  sprintf(LLmapBin,           "%sllmap.bin",dirname);
  sprintf(FIDmapBin,          "%sfidmap.bin",dirname);

  sprintf(ArealHeader, "%sarealinfo.txt"       ,dirname);
  sprintf(PointHeader, "%spointinfo.txt"       ,dirname);
  sprintf(LinearHeader,"%slinearinfo.txt"      ,dirname);
  sprintf(CodeHeader,  "%scodeinfo.txt"        ,dirname);
  sprintf(SGridHeader, "%sgridinfo.txt"        ,dirname);
  sprintf(MultiHeader, "%smultiinfo.txt"       ,dirname);
  sprintf(PartHeader,  "%smultipart.txt"       ,dirname);
  sprintf(PartPoints,  "%smultipartpoints.bin" ,dirname);
  sprintf(PartLines,   "%smultipartlines.bin"  ,dirname);
  sprintf(PartAreas,   "%smultipartareas.bin"  ,dirname);
  sprintf(VersionSt ,  "%sdbv.txt"             ,dirname);

  if(SLASHTYPE==NONDOS_TYPE)
  { 
    sprintf(FileNames,"%sinternal_data/sfs.bin",dirname);
  }
  else
  {
    sprintf(FileNames,"%sinternal_data\\sfs.bin",dirname);
  }
}



void AddIgnore()
{

  return;

  IGNORE_ATTR_NUM = 5;

  IGNORE_ATTR = (char **)malloc(sizeof(char *)*IGNORE_ATTR_NUM);

  IGNORE_ATTR[0] = (char *)malloc(10); sprintf(IGNORE_ATTR[0],"WD1");
  IGNORE_ATTR[1] = (char *)malloc(10); sprintf(IGNORE_ATTR[1],"WD2");
  IGNORE_ATTR[2] = (char *)malloc(10); sprintf(IGNORE_ATTR[2],"WD3");
  IGNORE_ATTR[3] = (char *)malloc(10); sprintf(IGNORE_ATTR[3],"WD4");
  IGNORE_ATTR[4] = (char *)malloc(10); sprintf(IGNORE_ATTR[4],"WD5");



  REPLACE_ATTR_NUM = 5;

  REPLACE_LHS = (char **)malloc(sizeof(char *)*REPLACE_ATTR_NUM);
  REPLACE_RHS = (char **)malloc(sizeof(char *)*REPLACE_ATTR_NUM);

  REPLACE_LHS[0] = (char *)malloc(10); REPLACE_RHS[0] = (char *)malloc(10); 
  sprintf(REPLACE_LHS[0],"WD1_");  sprintf(REPLACE_RHS[0],"WD1");

  REPLACE_LHS[1] = (char *)malloc(10); REPLACE_RHS[1] = (char *)malloc(10); 
  sprintf(REPLACE_LHS[1],"WD2_");  sprintf(REPLACE_RHS[1],"WD2");

  REPLACE_LHS[2] = (char *)malloc(10); REPLACE_RHS[2] = (char *)malloc(10); 
  sprintf(REPLACE_LHS[2],"WD3_");  sprintf(REPLACE_RHS[2],"WD3");

  REPLACE_LHS[3] = (char *)malloc(10); REPLACE_RHS[3] = (char *)malloc(10); 
  sprintf(REPLACE_LHS[3],"WD4_");  sprintf(REPLACE_RHS[3],"WD4");

  REPLACE_LHS[4] = (char *)malloc(10); REPLACE_RHS[4] = (char *)malloc(10); 
  sprintf(REPLACE_LHS[4],"WD5_");  sprintf(REPLACE_RHS[4],"WD5");

}


void RemoveFile(char *tname)
{
  char cmd[2000];
  extern int FileExists(char *filename);

  if(FileExists(tname))
  {
	if(USE_DOS==0)
	{
      sprintf(cmd,"rm -f \"%s\"",tname);
      system(cmd);
	}
	else
	{
      sprintf(cmd,"del \"%s\"",tname);
	  system(cmd);
	}
  }
}


void RemoveTempZip(char *filename)
{
  char tname[1000];

  sprintf(tname,"%s.shp",filename);
  RemoveFile(tname);
  sprintf(tname,"%s.SHP",filename);
  RemoveFile(tname);
  sprintf(tname,"%s.shx",filename);
  RemoveFile(tname);
  sprintf(tname,"%s.SHX",filename);
  RemoveFile(tname);
  sprintf(tname,"%s.dbf",filename);
  RemoveFile(tname);
  sprintf(tname,"%s.DBF",filename);
  RemoveFile(tname);
}


int CloseSentinel(double a, double b)
{
  if(fabs(a-b)<.0001)
  {
    return 1;
  }
  return 0;
}


void TestMinMax(int type, double value)
{

  if(CloseSentinel(-32767.0,value))
  {
	return;
  }

  if(type==1)
  {
	

	if(global_trex_layer==0)
	{
	  if(minmax_dem_finished_found0==0)
	  { 
        minmax_dem_finished_found0 = 1;
        min_g_dem_elev_finished0 = value;
        max_g_dem_elev_finished0 = value;
	  } 
	  else
	  { 
	    if(value<min_g_dem_elev_finished0)
		{ 
          min_g_dem_elev_finished0 = value;
		} 
	    if(value>max_g_dem_elev_finished0)
		{ 
          max_g_dem_elev_finished0 = value;
		} 
	  } 
	}


	if(global_trex_layer==1)
	{
	  if(minmax_dem_finished_found1==0)
	  { 
        minmax_dem_finished_found1 = 1;
        min_g_dem_elev_finished1 = value;
        max_g_dem_elev_finished1 = value;
	  } 
	  else
	  { 
	    if(value<min_g_dem_elev_finished1)
		{ 
          min_g_dem_elev_finished1 = value;
		} 
	    if(value>max_g_dem_elev_finished1)
		{ 
          max_g_dem_elev_finished1 = value;
		} 
	  } 
	}

	if(global_trex_layer==2)
	{
	  if(minmax_dem_finished_found2==0)
	  { 
        minmax_dem_finished_found2 = 1;
        min_g_dem_elev_finished2 = value;
        max_g_dem_elev_finished2 = value;
	  } 
	  else
	  { 
	    if(value<min_g_dem_elev_finished2)
		{ 
          min_g_dem_elev_finished2 = value;
		} 
	    if(value>max_g_dem_elev_finished2)
		{ 
          max_g_dem_elev_finished2 = value;
		} 
	  } 
	}

	if(global_trex_layer==3)
	{
	  if(minmax_dem_finished_found3==0)
	  { 
        minmax_dem_finished_found3 = 1;
        min_g_dem_elev_finished3 = value;
        max_g_dem_elev_finished3 = value;
	  } 
	  else
	  { 
	    if(value<min_g_dem_elev_finished3)
		{ 
          min_g_dem_elev_finished3 = value;
		} 
	    if(value>max_g_dem_elev_finished3)
		{ 
          max_g_dem_elev_finished3 = value;
		} 
	  } 
	}



  }
  else if(type==7)
  {
	
	if(minmax_dem_raw_found==0)
	{
      minmax_dem_raw_found = 1;
      min_g_dem_elev_raw = value;
      max_g_dem_elev_raw = value;
	}
	else
	{
	  if(value<min_g_dem_elev_raw)
	  {
        min_g_dem_elev_raw = value;
	  }
	  if(value>max_g_dem_elev_raw)
	  {
        max_g_dem_elev_raw = value;
	  }
	}
  }
}



void CrossCheckDates(int rawfinished,char *header_date,char *xml_date)
{
  char headeryear[10],headermonth[10],headerday[10],headerhour[10],headermin[10],headersec[10];
  char xmlyear[10],   xmlmonth[10],   xmlday[10],   xmlhour[10],   xmlmin[10],   xmlsec[10];
  static int foundone = 0;

  

  if(rawfinished==-1)
  {
    foundone = 0;
	return;
  }
  if(rawfinished==-2)
  {
    if(foundone==1)
	{
	  fprintf(badtrex,"IMPORT_SENTINEL  \n");	  
	}
	return;
  }


  

  if(strlen(header_date)!=19)
  {
    return;
  }
  if(strlen(xml_date)!=20)
  {
    return;
  }

  if(
	   (header_date[4] !=':') ||
	   (header_date[7] !=':') ||
	   (header_date[10]!=' ') ||
	   (header_date[13]!=':') ||
	   (header_date[16]!=':') ||

	   (xml_date[4] !='-') ||
	   (xml_date[7] !='-') ||
	   (xml_date[10]!='T') ||
	   (xml_date[13]!=':') ||
	   (xml_date[16]!=':') ||
	   (xml_date[19]!='Z')
	)
  {
    return;
  }


  strncpy(headeryear,&header_date[0],4);
  headeryear[4] = '\0';
  strncpy(headermonth,&header_date[5],2);
  headermonth[2] = '\0';
  strncpy(headerday,&header_date[8],2);
  headerday[2] = '\0';
  strncpy(headerhour,&header_date[11],2);
  headerhour[2] = '\0';
  strncpy(headermin,&header_date[14],2);
  headermin[2] = '\0';
  strncpy(headersec,&header_date[17],2);
  headersec[2] = '\0';


  strncpy(xmlyear,&xml_date[0],4);
  xmlyear[4] = '\0';
  strncpy(xmlmonth,&xml_date[5],2);
  xmlmonth[2] = '\0';
  strncpy(xmlday,&xml_date[8],2);
  xmlday[2] = '\0';
  strncpy(xmlhour,&xml_date[11],2);
  xmlhour[2] = '\0';
  strncpy(xmlmin,&xml_date[14],2);
  xmlmin[2] = '\0';
  strncpy(xmlsec,&xml_date[17],2);
  xmlsec[2] = '\0';


  

  if(
	  (strcmp(headeryear ,xmlyear))  ||
	  (strcmp(headermonth,xmlmonth)) ||
	  (strcmp(headerday  ,xmlday))   ||
	  (strcmp(headerhour ,xmlhour))  ||
	  (strcmp(headermin  ,xmlmin))   ||
	  (strcmp(headersec  ,xmlsec))
  )
  {

	if(foundone==0)
	{
	  fprintf(badtrex,"IMPORT_SENTINEL Different dates for GeoTIFF tag (DateTime: 306) and XML \"begin position\":\n");
	  foundone = 1;
	}

	if(rawfinished==1)
	{
      fprintf(badtrex,"IMPORT_SENTINEL   DEM Raw GeoTIFF tag (DateTime: 306): %s\n",header_date);
	  fprintf(badtrex,"IMPORT_SENTINEL   DEM Raw XML \"begin position\"       : %s\n",xml_date);
	}
	else if(rawfinished==2)
	{
      fprintf(badtrex,"IMPORT_SENTINEL   DEM Finished GeoTIFF tag (DateTime: 306): %s\n",header_date);
	  fprintf(badtrex,"IMPORT_SENTINEL   DEM Finished XML \"begin position\"       : %s\n",xml_date);
	}
  }
}




char *AssessTREX(char *oldfilename)
{
  FILE *infile,*outfile;
  int save1num,save2num,save3num,save4num;
  char aline[5000];
  char save1[1000],save2[1000],save3[1000],save4[1000];
  static char newfilename[1000];

  int bring_in_all_reference=0;


  



  infile = fopen(oldfilename,"r");
  if(infile==NULL)
  {
	printf("error: couldn't open %s for read\n",oldfilename);
	return oldfilename;
  }

  sprintf(newfilename,"%s2",oldfilename);
  outfile = fopen(newfilename,"w");
  if(outfile==NULL)
  {
	printf("error: couldn't open %s for write\n",newfilename);
	fclose(infile);
	return oldfilename;
  }

  save1[0] = '\0';
  save2[0] = '\0';
  save3[0] = '\0';
  save4[0] = '\0';

  save1num = 999;
  save2num = 999;
  save3num = 999;
  save4num = 999;

  fgets(aline,5000,infile);
  while(!feof(infile))
  { 	  
	if(strlen(aline)>3)
	{
	  

	  if(strncmp(aline,"GEOTIFF",7))
	  {
		fprintf(outfile,"%s",aline);
	  }
	  else
	  {
        

        

if(bring_in_all_reference==1)
{
 fprintf(outfile,"%s",aline);
 fgets(aline,5000,infile);
 continue;
}


		if(strstr(aline,"GEOTIFF \"Active Earthscape_0"))
		{
		  save1num = 1;
		  sprintf(save1,"%s",aline);
		}
		else if(strstr(aline,"GEOTIFF \"SRTM Level 2_0"))
		{
		  if(save1num>2)
		  {
		    save1num = 2;
		    sprintf(save1,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"ASTER GDEM v2_0"))
		{
		  if(save1num>3)
		  {
		    save1num = 3;
		    sprintf(save1,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"DTED Level 1_0"))
		{
		  if(save1num>4)
		  {
		    save1num = 4;
		    sprintf(save1,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"CDEM_0"))
		{
		  if(save1num>5)
		  {
		    save1num = 5;
		    sprintf(save1,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn75_0"))
		{
		  if(save1num>6)
		  {
		    save1num = 6;
		    sprintf(save1,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn300_0"))
		{
		  if(save1num>7)
		  {
		    save1num = 7;
		    sprintf(save1,"%s",aline);
		  }
		}



		else if(strstr(aline,"GEOTIFF \"Active Earthscape_1"))
		{
		  save2num = 1;
		  sprintf(save2,"%s",aline);
		}
		else if(strstr(aline,"GEOTIFF \"SRTM Level 2_1"))
		{
		  if(save2num>2)
		  {
		    save2num = 2;
		    sprintf(save2,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"ASTER GDEM v2_1"))
		{
		  if(save2num>3)
		  {
		    save2num = 3;
		    sprintf(save2,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"DTED Level 1_1"))
		{
		  if(save2num>4)
		  {
		    save2num = 4;
		    sprintf(save2,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"CDEM_1"))
		{
		  if(save2num>5)
		  {
		    save2num = 5;
		    sprintf(save2,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn75_1"))
		{
		  if(save2num>6)
		  {
		    save2num = 6;
		    sprintf(save2,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn300_1"))
		{
		  if(save2num>7)
		  {
		    save2num = 7;
		    sprintf(save2,"%s",aline);
		  }
		}


		else if(strstr(aline,"GEOTIFF \"Active Earthscape_2"))
		{
		  save3num = 1;
		  sprintf(save3,"%s",aline);
		}
		else if(strstr(aline,"GEOTIFF \"SRTM Level 2_2"))
		{
		  if(save3num>2)
		  {
		    save3num = 2;
		    sprintf(save3,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"ASTER GDEM v2_2"))
		{
		  if(save3num>3)
		  {
		    save3num = 3;
		    sprintf(save3,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"DTED Level 1_2"))
		{
		  if(save3num>4)
		  {
		    save3num = 4;
		    sprintf(save3,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"CDEM_2"))
		{
		  if(save3num>5)
		  {
		    save3num = 5;
		    sprintf(save3,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn75_2"))
		{
		  if(save3num>6)
		  {
		    save3num = 6;
		    sprintf(save3,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn300_2"))
		{
		  if(save3num>7)
		  {
		    save3num = 7;
		    sprintf(save3,"%s",aline);
		  }
		}


		else if(strstr(aline,"GEOTIFF \"Active Earthscape_3"))
		{
		  save4num = 1;
		  sprintf(save4,"%s",aline);
		}
		else if(strstr(aline,"GEOTIFF \"SRTM Level 2_3"))
		{
		  if(save4num>2)
		  {
		    save4num = 2;
		    sprintf(save4,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"ASTER GDEM v2_3"))
		{
		  if(save4num>3)
		  {
		    save4num = 3;
		    sprintf(save4,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"DTED Level 1_3"))
		{
		  if(save4num>4)
		  {
		    save4num = 4;
		    sprintf(save4,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"CDEM_3"))
		{
		  if(save4num>5)
		  {
		    save4num = 5;
		    sprintf(save4,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn75_3"))
		{
		  if(save4num>6)
		  {
		    save4num = 6;
		    sprintf(save4,"%s",aline);
		  }
		}
		else if(strstr(aline,"GEOTIFF \"GMTED mn300_3"))
		{
		  if(save4num>7)
		  {
		    save4num = 7;
		    sprintf(save4,"%s",aline);
		  }
		}

		else
		{
		  

          if(
			  (strstr(aline,"GEOTIFF \"Edit Data Mask_0")) ||
			  (strstr(aline,"GEOTIFF \"Edit Data Mask_1")) ||
			  (strstr(aline,"GEOTIFF \"Edit Data Mask_2")) ||
			  (strstr(aline,"GEOTIFF \"Edit Data Mask_3")) ||

			  (strstr(aline,"GEOTIFF \"Height Error Map_0")) ||
			  (strstr(aline,"GEOTIFF \"Height Error Map_1")) ||
			  (strstr(aline,"GEOTIFF \"Height Error Map_2")) ||
			  (strstr(aline,"GEOTIFF \"Height Error Map_3")) ||

			  (strstr(aline,"GEOTIFF \"Source Data Mask_0")) ||
			  (strstr(aline,"GEOTIFF \"Source Data Mask_1")) ||
			  (strstr(aline,"GEOTIFF \"Source Data Mask_2")) ||
			  (strstr(aline,"GEOTIFF \"Source Data Mask_3")) ||

			  (strstr(aline,"GEOTIFF \"Water Body Mask_0")) ||
			  (strstr(aline,"GEOTIFF \"Water Body Mask_1")) ||
			  (strstr(aline,"GEOTIFF \"Water Body Mask_2")) ||
			  (strstr(aline,"GEOTIFF \"Water Body Mask_3")) ||

			  (strstr(aline,"GEOTIFF \"TREx DEM Finished_0")) ||
			  (strstr(aline,"GEOTIFF \"TREx DEM Finished_1")) ||
			  (strstr(aline,"GEOTIFF \"TREx DEM Finished_2")) ||
			  (strstr(aline,"GEOTIFF \"TREx DEM Finished_3")) ||

			  (strstr(aline,"GEOTIFF \"TREx DEM Raw"))        ||
			  (strstr(aline,"GEOTIFF \"Reliability Mask"))
			)
		  {
		    fprintf(outfile,"%s",aline);
		  }
		}
	  }
	}
    fgets(aline,5000,infile);
  }


  

  if(save1num<999)
  {	
	fprintf(outfile,"%s",save1);
  }
  if(save2num<999)
  {	
	fprintf(outfile,"%s",save2);
  }
  if(save3num<999)
  {	
	fprintf(outfile,"%s",save3);
  }
  if(save4num<999)
  {	
	fprintf(outfile,"%s",save4);
  }



  fclose(outfile);
  fclose(infile);
  
  return newfilename;
}



char *GetTRExSubName2(char *thepath, int origid)
{
  
  FILE *infile;
  int len;
  char newname[1000];
  static char retval[25];

  len = strlen(thepath);

  if(thepath[len]=='\0') 
  { 
    if(
	    (thepath[len-1]!='/')  &&
	    (thepath[len-1]!='\\')
	 )
	{
	  sprintf(newname,"%s%sinternal_data%stspi",thepath,GetSlash(),GetSlash());
	}
	else
	{
	  sprintf(newname,"%sinternal_data%stspi",thepath,GetSlash());
	}
  }


  infile = fopen(newname,"r");
  if(infile==NULL)
  {
    printf("Error in GTSN: couldn't open %s for read\n",newname);
	return "Error";
  }

  fgets(newname,1000,infile);


  if(origid==0)
  {
	

	fclose(infile);
    if(strlen(newname)<20)
	{
      sprintf(retval,"%s",newname);

      len = strlen(retval);
      if(retval[len-1]<20)
	  {
		retval[len-1] = '\0';
	  }
      if(retval[len-2]<20)
	  {
		retval[len-2] = '\0';
	  }

	  return retval;
	} 
	printf("Error2: got %s\n",newname);
	return "Error2";
  }


  fgets(newname,1000,infile);
  fclose(infile);
  if(strlen(newname)<20)
  {
    sprintf(retval,"%s",newname);

    len = strlen(retval);
    if(retval[len-1]<20)
	{
	  retval[len-1] = '\0';
	}
    if(retval[len-2]<20)
	{
	  retval[len-2] = '\0';
	}
 
	return retval;
  } 
  printf("Error3: got %s\n",newname);
  return "Error3";

}





void CreateDifferenceSurfaces(char *file,int files_done,char *destination)
{
  FILE *fileptr;
  int i,foundfinished,foundsrtm;
  char aline[5000];
  char savefinished[2000],savesrtm[2000],outname[1000],import_type[1000],layerstring[1000],
	  targetsrtm[1000],targetfinished[1000];
  extern int CreateTIFF_SurfaceDifference(char *Finished, char *Reference, char *OutputFileName,
											char *filename, int files_done);

  


  fileptr = fopen(file,"r");
  if(fileptr==NULL)
  {
    printf("Error: cannot open %s\n",file);
	printf("  Cannot create Surface Difference file\n");
	return;
  }


  for(i=0;i<4;i++)
  {
	
	

    foundfinished = 0;
	foundsrtm     = 0;
	rewind(fileptr);

    sprintf(targetsrtm,"SRTM Level 2_%d",i);
    sprintf(targetfinished,"TREx DEM Finished_%d",i);


	fgets(aline,5000,fileptr);

    while(!feof(fileptr))
	{
	  import_type[0] = '\0';
	  layerstring[0] = '\0';
	  
	  sscanf(aline,"%s",import_type);

	  if(!strcmp(import_type,"GEOTIFF"))
	  {
	    sprintf(layerstring,"%s",GetLayerPart(aline));

		if(!strcmp(layerstring,targetsrtm))
		{
		  foundsrtm = 1;
	      sprintf(savesrtm,"%s",GetLastPart(aline));
		}
		if(!strcmp(layerstring,targetfinished))
		{
		  foundfinished = 1;
	      sprintf(savefinished,"%s",GetLastPart(aline));
		}
	  }
	  fgets(aline,5000,fileptr);
	}

    if((foundfinished>0)&&(foundsrtm>0))
	{
       sprintf(outname,"%ssummary_files%sTDF_SRT2F_Difference.tif",destination,GetSlash());

	   printf("Found srtm %s\n",savesrtm);
	   printf("Found finished %s\n",savefinished);
       printf("Create %s\n",outname);

       if(CreateTIFF_SurfaceDifference(savefinished,savesrtm,outname,file,files_done)==1)
	   { 
	     printf("Created %s\n",outname);
	   }
	   else
	   {
		 printf("Failed to create %s\n",outname);

		 
	   }
	}
  } 

  fclose(fileptr);
}


int AssessDiffs(double mindiff_to_report,
			           int min_found0,int max_found0,
                       double min0,double max0,
			           int min_found1,int max_found1,
 					   double min1,double max1,
			           int min_found2,int max_found2,
					   double min2,double max2)
{
  double min=9999999,max=-9999999;

  
  if((min_found0>0)&&(min0<min)){min = min0;}
  if((min_found1>0)&&(min1<min)){min = min1;}
  if((min_found2>0)&&(min2<min)){min = min2;}

  
  if((min_found0>0)&&(min0>max)){max = min0;}
  if((min_found1>0)&&(min1>max)){max = min1;}
  if((min_found2>0)&&(min2>max)){max = min2;}

  if((max-min)>mindiff_to_report)
  {
	
	return 1;
  }


  min=9999999;
  max=-9999999;

  
  if((max_found0>0)&&(max0<min)){min = max0;}
  if((max_found1>0)&&(max1<min)){min = max1;}
  if((max_found2>0)&&(max2<min)){min = max2;}

  
  if((max_found0>0)&&(max0>max)){max = max0;}
  if((max_found1>0)&&(max1>max)){max = max1;}
  if((max_found2>0)&&(max2>max)){max = max2;}

  if((max-min)>mindiff_to_report)
  {
	
	return 1;
  }

  return 0;
}





void WriteFile(char *outname,char *shortname,char *fallback_val,int createfile)
{
  FILE *outfile;
  char outval[100];

  

  if(createfile==1)
  {
    outfile = fopen(outname,"w");
    if(outfile==NULL)
	{
     printf("Error: could not open %s for write\n",outname);
	 return;
	} 
  }
  else
  {
    outfile = fopen(outname,"at");
    if(outfile==NULL)
	{
     printf("Error: could not open %s for append\n",outname);
	 return;
	} 
  }

  sprintf(outval,"%s",fallback_val);

  if(strlen(shortname)>10)
  {
	if(shortname[3]=='_')
	{
      sprintf(outval,"%s",&shortname[4]);

	  if(strlen(outval)>7)
	  {
	    if(outval[7]=='_')
		{
		  outval[7] = '\0';
		}
	  }
	}
  }

  fprintf(outfile,"%s\n",outval);
  fclose(outfile);
}


void CreateTREXSubPartFiles(char *in_filename,char *destination)
{
  FILE *fileptr;
  char aline[5000],import_type[500],layerstring[1000],filename[1000],
	  shortname[1000],outname[1000],saveraw[100],savetile[100];
  extern void GetShortName(char shortname[],char *shape_to_import);


  fileptr = fopen(in_filename,"r");
  if(fileptr==NULL){printf("CTSPF: couldnt open %s for reading\n",in_filename); exit(-1);}

  saveraw[0] = '\0';
  savetile[0] = '\0';


  fgets(aline,5000,fileptr);

  while(!feof(fileptr))
  {
	 sscanf(aline,"%s",import_type);

	 if(!strcmp(import_type,"GEOTIFF"))
	 {

	   sprintf(layerstring,"%s",GetLayerPart(aline));

	   sprintf(filename,"%s",GetLastPart(aline));		      
	   GetShortName(shortname,filename);


	   

       if(!strcmp(layerstring,"TREx DEM Raw"))
	   {
		 sprintf(saveraw,"%s",shortname);
	   }
	   if(
		   (!strncmp(layerstring,"TREx DEM Finished_0",19)) ||
		   (!strncmp(layerstring,"Edit Data Mask_0",16))    ||
		   (!strncmp(layerstring,"Source Data Mask_0",18))  ||
		   (!strncmp(layerstring,"Water Body Mask_0",17))   ||
		   (!strncmp(layerstring,"Height Error Map_0",18))  ||

		   (!strncmp(layerstring,"TREx DEM Finished_1",19)) ||
		   (!strncmp(layerstring,"Edit Data Mask_1",16))    ||
		   (!strncmp(layerstring,"Source Data Mask_1",18))  ||
		   (!strncmp(layerstring,"Water Body Mask_1",17))   ||
		   (!strncmp(layerstring,"Height Error Map_1",18))  ||

		   (!strncmp(layerstring,"TREx DEM Finished_2",19)) ||
		   (!strncmp(layerstring,"Edit Data Mask_2",16))    ||
		   (!strncmp(layerstring,"Source Data Mask_2",18))  ||
		   (!strncmp(layerstring,"Water Body Mask_2",17))   ||
		   (!strncmp(layerstring,"Height Error Map_2",18))  ||

		   (!strncmp(layerstring,"TREx DEM Finished_3",19)) ||
		   (!strncmp(layerstring,"Edit Data Mask_3",16))    ||
		   (!strncmp(layerstring,"Source Data Mask_3",18))  ||
		   (!strncmp(layerstring,"Water Body Mask_3",17))   ||
		   (!strncmp(layerstring,"Height Error Map_3",18))
		   )
	   {
		 sprintf(savetile,"%s",shortname);
	   }
	 }
    fgets(aline,5000,fileptr);
  } 

	 
  sprintf(outname,"%s%sinternal_data%stspi",destination,GetSlash(),GetSlash());

  WriteFile(outname,saveraw, "NO_RAW",1); 
  WriteFile(outname,savetile,"NO_RAW",2);  

}




int Extract_NGA(int TYPE, char file_in[],char destination[])
{
  
  
extern int ABORT_EXTRACT;
  
#if(NGA_TYPE==1)
  
#if(USE_DOS==1)
  int ostype,gmtype,startpos,type,thislen;
  char gdfilename[1000];
#endif
  int test,i,num_dted=0;
  FILE *fileptr,*openfile;
  char curr_file[1000],savefilename[1000],override_curr_file[1000];
  char aline[5000], layerstring[1000], import_type[500], shortname[2000],file[1000];
  char fcodestring[1000],filename[1000],command1[1000],command2[1000],command3[1000];
  int j,gd_to_import=0,gm_to_import=0,wroteone;
  int totallongfound=0,  totalshortfound=0,  longitemsfound=0,  shortitemsfound=0,   files_done=0;
  int      dted_done=0, shapes_to_import=0,   totaldtedlong=0,   totaldtedshort=0,  insertindex=0;
  double mindiff=0.1;
  extern int numsavednames;
  extern int global_trex_shape_layer;
  extern char specialAfilename[1000];
  extern void PopExtrErr();


#if (USE_DOS==1)
  int num_tables=0,len,Ijunk;
  char connectstring[1000],osfilename[1000],part1[1000],part2[1000],part3[1000];
#endif
  extern void GetShortName(char shortname[],char *shape_to_import);
  extern void sh_update_shape_import(char *alldata, int files_done);
  extern void SH_PrintListChunk(int done, int numtables, char *filename, char *tablename);
  extern int import_geotiff(char *filename, char *allshapes, int files_done, char *lod_override, int addtolong, int addtoshort, int type, char *destination);

  
#if(USE_DOS==1)
  extern int DoOSStuff(FILE *inspecfile,int BuildInputSpecFile,
		       char *OSname,char *DDname,char *inspecfilename,int type,
		       int addtolong,int addtoshort,
		       int *longitemsfound,int *shortitemsfound,
		       int *badostables, int *goodostables);
    
  extern int  OpenGeoDatabase(int AccessOrFile, char *filename, int MakeInputSpec,
							  int addtolong, int addtoshort,
		                      int *longitemsfound, int *shortitemsfound);

#endif

  minmax_dem_finished_found0 = 0;
  minmax_dem_finished_found1 = 0;
  minmax_dem_finished_found2 = 0;
  minmax_dem_finished_found3 = 0;
  minmax_dem_raw_found       = 0;

  min_fin_header_found0      = 0;
  max_fin_header_found0      = 0;
  min_fin_header_found1      = 0;
  max_fin_header_found1      = 0;
  min_fin_header_found2      = 0;
  max_fin_header_found2      = 0;
  min_fin_header_found3      = 0;
  max_fin_header_found3      = 0;
  min_raw_header_found       = 0;
  max_raw_header_found       = 0;

  fin_header_date_found      = 0;
  raw_header_date_found      = 0;
  fin_xml_date_found         = 0;
  raw_xml_date_found         = 0;
  min_fin_xml_found0         = 0;
  max_fin_xml_found0         = 0;
  min_fin_xml_found1         = 0;
  max_fin_xml_found1         = 0;
  min_fin_xml_found2         = 0;
  max_fin_xml_found2         = 0;
  min_fin_xml_found3         = 0;
  max_fin_xml_found3         = 0;
  min_raw_xml_found          = 0;
  max_raw_xml_found          = 0;
  finished_copyright         = 0;
  raw_copyright              = 0;


  sprintf(GlobalDest,"%s",destination);
  
  
  SHORT_LIST = NULL;
  
  ID_INDEX = 0;
  
  MakeFileNames(destination);
  
  STF_CONSUMED              = -1;
  TestNumLinearNodes        = 0;
  NumLinearNodes            = 0;
  TestNumArealNodes         = 0;
  NumArealNodes             = 0;
  NumFMArealNodes           = 0;
  NumPointNodes             = 0;
  ClassLevel                = -100;
  idle_redraw               = 0;
  NumPointFiles             = 1;
  NumLinearFiles            = 1;
  NumArealFiles             = 1;
  NumGridFiles              = 1;
  NumCodeFiles              = 1;
  NumPointParts             = 0;
  NumLineParts              = 0;
  NumAreaParts              = 0; 
  NumHoles                  = 0;
  NumMultiPartPointFeatures = 0;
  NumMultiPartLineFeatures  = 0;
  NumMultiPartAreaFeatures  = 0;
  MODEL_PASS_ONE            = 0;
  MODEL_PASS_TWO            = 0;
  MODEL_PASS_THREE          = 0;
  MINYREAD                  =  100000000.0;
  MINXREAD                  =  100000000.0;
  MAXYREAD                  = -100000000.0;
  MAXXREAD                  = -100000000.0;  
  MINY_P_READ               =  100000000.0;
  MINX_P_READ               =  100000000.0;
  MAXY_P_READ               = -100000000.0;
  MAXX_P_READ               = -100000000.0;
  num_polys                 = 0;
  num_areals                = 0;
  num_points                = 0;
  num_linears               = 0;
  num_modelpolys            = 0;
  num_gmi                   = 0;
  num_fmi                   = 0;
  num_grids                 = 0;
  num_coll                  = 0;
  num_par                   = 0;
  num_stamp                 = 0;
  num_foot                  = 0;
  num_fmareal               = 0;
  num_fmpoint               = 0;
  num_fmlinear              = 0;
  TERRAINPOLYSREAD          = 0;
  gridpointsread            = 0;
  scc_short_items           = 0;
  scc_long_items            = 0;
  foundTREXraw              = 0;
  foundTREXfinished         = 0;
  badGCP                    = 0;
  goodGCP1                  = 0;
  goodGCP2                  = 0;
  numsavednames             = 0;
  foundrawfolder            = 0;
  gcpindex1                 = -1;
  gcpindex1_0               = -1;
  gcpindex1_1               = -1;
  gcpindex1_2               = -1;
  gcpindex1_3               = -1;
  gcpindex1s_0              = -1;
  gcpindex1s_1              = -1;
  gcpindex1s_2              = -1;
  gcpindex1s_3              = -1;
  gcpindex2                 = -1;
  gcpindex2_0               = -1;
  gcpindex2_1               = -1;
  gcpindex2_2               = -1;
  gcpindex2_3               = -1;
  trexfinishedindex         = -1;
  trexfinishedindex0        = -1;
  trexfinishedindex1        = -1;
  trexfinishedindex2        = -1;
  trexfinishedindex3        = -1;
  trexwatermaskindex        = -1;
  trexwatermaskindex0       = -1;
  trexwatermaskindex1       = -1;
  trexwatermaskindex2       = -1;
  trexwatermaskindex3       = -1;
  ListOfFirsts              = NULL;
  specialAfilename[0]       = '\0';
  sprintf(SaveClassTable,"zzZZzzZZ_____");
  sprintf(SaveClassValue,"zzZZzzZZ_____");

  openpolys(); 
  opennets();  
  openmods();   
  openareals();
  openheaders();
  opengrid();


  if(CREATING_TREX==1)
  {
    sprintf(file,"%s",AssessTREX(file_in));  

    CreateTREXSubPartFiles(file,destination);
  }
  else
  {
	sprintf(file,"%s",file_in);
  }


  COORDINATEKEEPER = 0;


  if(TYPE==3)
    {
      
      
      
      
      
      
      
      init_shape();
      test = 0;
      
      
      fileptr = fopen(file,"r");
      if(fileptr==NULL){printf("couldnt open %s for reading\n",file); exit(-1);}
      
      
      fgets(aline,5000,fileptr);
      while(!feof(fileptr))
	{	  
	  if(strlen(aline)>3)
	    {
	      sscanf(aline,"%s%s",import_type,fcodestring);
	      
	      if(!strcmp(import_type,"IGNORE_ATTR"))
		{
		  IGNORE_ATTR_NUM = IGNORE_ATTR_NUM + 1;
		}
	      else if(!strcmp(import_type,"SHAPE"))
		{
		  shapes_to_import = shapes_to_import + 1;
		}
	      else if(!strcmp(import_type,"SHAPE_0"))
		{
		  shapes_to_import = shapes_to_import + 1;
		}
	      else if(!strcmp(import_type,"SHAPE_1"))
		{
		  shapes_to_import = shapes_to_import + 1;
		}
	      else if(!strcmp(import_type,"SHAPE_2"))
		{
		  shapes_to_import = shapes_to_import + 1;
		}
	      else if(!strcmp(import_type,"SHAPE_3"))
		{
		  shapes_to_import = shapes_to_import + 1;
		}
	      else if(!strcmp(import_type,"ZIPSHAPE"))
		{
		  shapes_to_import = shapes_to_import + 1;
		}
	      else if(!strcmp(import_type,"GEODATABASE"))
		{
		  gd_to_import = gd_to_import + 1;
		}
	      else if(!strcmp(import_type,"GEOMEDIAACC"))
		{
		  gm_to_import = gm_to_import + 1;
		}
	    }
	  fgets(aline,5000,fileptr);
	}
      
      
      
      
      if(IGNORE_ATTR_NUM>0)
	{
	  rewind(fileptr);
	  
	  IGNORE_ATTR = (char **)malloc(sizeof(char *)*IGNORE_ATTR_NUM);
	  insertindex = 0;
	  
	  fgets(aline,5000,fileptr);
	  while(!feof(fileptr))
	    {
	      sscanf(aline,"%s%s",import_type,fcodestring);
	      
	      if(!strcmp(import_type,"IGNORE_ATTR"))
		{
		  IGNORE_ATTR[insertindex] = (char *) malloc(strlen(fcodestring)+2);
		  sprintf(IGNORE_ATTR[insertindex],"%s",fcodestring);
		  insertindex++;
		}
	      fgets(aline,5000,fileptr);
	    }
	  if(insertindex!=IGNORE_ATTR_NUM)
	    {
	      printf(" Internal error:  insertindex %d ignorenum %d\n",
		     insertindex,IGNORE_ATTR_NUM);
	    }
	}
      
      
      
      if(shapes_to_import>0)
	{
	  if(longshapenames!=NULL)
	    {
	      free(longshapenames);
	      longshapenames = NULL;
	    }
	  if(shortshapenames!=NULL)
	    {
	      free(shortshapenames);
	      shortshapenames = NULL;
	    }
	  if(shapenums!=NULL)
	    {
	      free(shapenums);
	      shapenums = NULL;
	    }
	  
	  longshapenames  = (char **) malloc( sizeof(char *) * (shapes_to_import+5));
	  shortshapenames = (char **) malloc( sizeof(char *) * (shapes_to_import+5));
	  shapenums       = (int *)   malloc( sizeof(int)    * (shapes_to_import+5));
	  
	  insertindex = 0;
	  
	  rewind(fileptr);
	  
	  fgets(aline,5000,fileptr);
	  while(!feof(fileptr))
	    {
	      if(strlen(aline)>3)
		{
		  sscanf(aline,"%s%s",import_type,fcodestring);
		  
		  if( 
			  (!strcmp(import_type,"SHAPE"))    ||  
			  (!strcmp(import_type,"SHAPE_0"))  ||  
			  (!strcmp(import_type,"SHAPE_1"))  ||  
			  (!strcmp(import_type,"SHAPE_2"))  ||  
			  (!strcmp(import_type,"SHAPE_3"))  ||  
			  (!strcmp(import_type,"ZIPSHAPE")) 
			  )
		    {

		      sscanf(aline,"%s%s%s",import_type,fcodestring,curr_file);
		      
		      
		      sprintf(curr_file,"%s",GetLastPart(aline));
		      
		      
		      GetShortName(shortname,curr_file);
		      
		      longshapenames [insertindex] = (char *)malloc(sizeof(char)*(strlen(aline)+10));
		      shortshapenames[insertindex] = (char *)malloc(sizeof(char)*(strlen(shortname)+10));
		      
		      sprintf(longshapenames [insertindex],"%s",aline);
		      sprintf(shortshapenames[insertindex],"%s",shortname);
		      insertindex++;
		    }
		}
	      fgets(aline,5000,fileptr);
	    }
	  SortShapes(0,shapes_to_import-1);
	}
      
      
      for(insertindex=0;insertindex<shapes_to_import;insertindex++)
	{
	  shapenums[insertindex] = 1;
	}
      
      for(insertindex=shapes_to_import-2;insertindex>=0;insertindex--)
	{
	  if(!strcmp(shortshapenames[insertindex],shortshapenames[insertindex+1]))
	    {
	      shapenums[insertindex] = shapenums[insertindex] + shapenums[insertindex + 1];
	    }
	}
      
      
      
      
      
	  AddIgnore();
      
      
      
      
      for(i=0;i<shapes_to_import;i++)
	{
	  sscanf(longshapenames[i],"%s%s",import_type,fcodestring);
	  
	  
	  


	  if( 
		  (!strcmp(import_type,"SHAPE"))   || 
		  (!strcmp(import_type,"SHAPE_0")) || 
		  (!strcmp(import_type,"SHAPE_1")) || 
		  (!strcmp(import_type,"SHAPE_2")) || 
		  (!strcmp(import_type,"SHAPE_3")) || 
		  (!strcmp(import_type,"ZIPSHAPE")) 
		)
	    {
	      
	      
	      
          global_trex_shape_layer = -1;

	      if(!strcmp(import_type,"SHAPE_0"))
		  { 
		    sprintf(import_type,"SHAPE");
	   	    global_trex_shape_layer = 0;
		  } 
	      if(!strcmp(import_type,"SHAPE_1"))
		  { 
		    sprintf(import_type,"SHAPE");
	  	    global_trex_shape_layer = 1;
		  } 
	      if(!strcmp(import_type,"SHAPE_2"))
		  { 
		    sprintf(import_type,"SHAPE");
		    global_trex_shape_layer = 2;
		  } 
	      if(!strcmp(import_type,"SHAPE_3"))
		  { 
		    sprintf(import_type,"SHAPE");
	  	    global_trex_shape_layer = 3;
		  } 
		  

		  COORDINATEKEEPER = 1; 

	      for(j=1;j<=shapenums[i];j++)
		{
		  

		  sscanf(longshapenames[i+j-1],"%s%s%s",import_type,fcodestring,curr_file);
		  
		  
		  sprintf(curr_file,"%s",GetLastPart(longshapenames[i+j-1]));
		  
		  sprintf(savefilename,"%s",curr_file);
		  sprintf(specialAfilename,"%s",curr_file);
		  
		  
		  sprintf(curr_file,"%s",ModifyFileName(curr_file,destination));

if(!strcmp(import_type,"ZIPSHAPE"))
{
  sprintf(override_curr_file,"%s",curr_file);
  sprintf(curr_file,"%s",UnzipShapefile(curr_file,destination));

  
}
else
{
  override_curr_file[0] = '\0';
}
		  
		  if(strlen(curr_file)>0)
		    {

			  RecordFileName(1,curr_file);  

		      test = import_shape(curr_file,files_done,file,fcodestring,
					  shortshapenames[i+j-1],
					  &longitemsfound,&shortitemsfound,
					  totallongfound,totalshortfound,override_curr_file);

			  RecordFileName(2,curr_file);  

              if(!strcmp(import_type,"ZIPSHAPE"))
			  {
				RemoveTempZip(curr_file);
			  }

		      if(test==0)
			  { 
			    break;
			  } 
		      files_done++;
		    }
		  else
		    {
              printf("Ignoring shapefile %s due to it having no shapes\n",
			     GetLastPart(longshapenames[i+j-1]));
		      longitemsfound = 0;
		      shortitemsfound = 0;
		    }

         
	       SH_FreeLists("!@#$%^&*()",savefilename);
		}
	      
	      
	      i = i + shapenums[i] - 1;
	      
	      totallongfound  = totallongfound  + longitemsfound;
	      totalshortfound = totalshortfound + shortitemsfound;
	      
	      
	      
	      SH_PrintListChunk(0,-1,"","");
	      
	      SH_FreeLists("ok",savefilename);
	      SH_FreeTree(); 
	      specialAfilename[0] = '\0';
          global_trex_shape_layer = -1;
	    }
	  
	  else if(!strcmp(import_type,"IGNORE_ATTR"))
	    {
	      
	      test = 1;
	    }
	  else if(!strcmp(import_type,"DTED"))
	    {
	      
	      test = 1;
	    }
	  else if(!strcmp(import_type,"GEOTIFF"))
	    {
	      
	      test = 1;
	    }
	  else if(!strcmp(import_type,"NITF"))
	    {
	      
	      test = 1;
	    }
	  else if(!strcmp(import_type,"GEODATABASE"))
	    {
	      
	      test = 1;
	    }
	  else if(!strcmp(import_type,"GEOMEDIAACC"))
	    {
	      
	      test = 1;
	    }
	  else
	    {
	      printf("Bad input line in file %s:\n",file);
	      printf("%s\n",curr_file);
	      printf("(1) Each line must be of the form [DTED/SHAPE] <lod identifier> <filename>\n");
	      test = 0;
	    }
	  
	  if(test==0)
	    {
	      
	      printf("Failed (1) to import line %s.  Aborting import.\n",curr_file);
	      closepolys();
	      closenets();
	      closemods(); 
	      closeareals();   
	      closeheaders();
	      closegrid();
	      FreeIgnore();
          FreeShapes(shapes_to_import); 
	      SH_FreeLists("ok","");
	      SH_FreeTree();
	      root=NIL;
	      L_root=NILL;
	      SH_PrintListChunk(1,-1,"","");  
	      return 0;	      
	    }
	  
	  
	  
	  
	  fgets(aline,5000,fileptr);
	}
      
      



#if((USE_DOS==1)&&(NGA_TYPE==1))
      
      if(gm_to_import>0)
	{
	  rewind(fileptr);
	  
	  fgets(aline,5000,fileptr);
	  
	  while(!feof(fileptr))
	    {
	      sscanf(aline,"%s",import_type);
	      
		  ostype = 0;
		  gmtype = 0;

		  
	      if(!strcmp(import_type,"GEOMEDIAACC"))
		  { 
		    gmtype   = 1;
			startpos = 12;
		  }

		  if((ostype+gmtype)>0)
		  {
		    COORDINATEKEEPER = 1; 

		    if(DEBUG==1)printf("Found object space/geomedia access to import:\n");  	        
		    sscanf(aline,"%s%s%d",import_type,part2,&numtables_to_extract);
		    if(DEBUG==1)printf("  num tables %d\n",numtables_to_extract);
		  
		    fgets(aline,5000,fileptr);
		    for(i=startpos;i<(int)strlen(aline);i++)   
		    {
		      osfilename[i-startpos] = aline[i];
		    }
		  osfilename[i-startpos] = '\0';
		  len = strlen(osfilename);
		  for(i=0;i<len;i++)
		    {
		      if((osfilename[i] == 10)||(osfilename[i] == 13))
			{
			  osfilename[i] = '\0';
			}
		    }
		  
		  
		  if(ostype==1)
		  {
		    fgets(aline,5000,fileptr);
		    for(i=28;i<(int)strlen(aline);i++)   
		    {
		      connectstring[i-28] = aline[i];
		    }

		    connectstring[i-28] = '\0';
		    len = strlen(connectstring);
		    for(i=0;i<len;i++)
		    {
		      if((connectstring[i] == 10)||(connectstring[i] == 13))
			  {
			    connectstring[i] = '\0';
			  } 
			}
		  }
		  else
		  {
			  sprintf(connectstring,"NA");
		  }
		  
		  TablesToExtract = (char **)malloc(sizeof(char *)*(numtables_to_extract+5));
		  FCODEsOfTables  = (char **)malloc(sizeof(char *)*(numtables_to_extract+5));
		  TableGeoms      = (int *)malloc(sizeof(int)*(numtables_to_extract+5));
		  TableHasData    = (int *)malloc(sizeof(int)*(numtables_to_extract+5));
		  
		  
		  
		  for(i=0;i<numtables_to_extract;i++)
		    {
		      TableGeoms[i]   = -1;
		      TableHasData[i] = 0;
		      fgets(aline,5000,fileptr);
		      
		      sscanf(aline,"%s%s%s",part1,part2,part3);
		      if(DEBUG==1)printf("  FCODE %s tablename %s\n",part2,part3);
		      
		      FCODEsOfTables [i] = (char *)malloc(strlen(part2)+5);
		      TablesToExtract[i] = (char *)malloc(strlen(part3)+5);
		      sprintf(FCODEsOfTables [i],"%s",part2);
		      sprintf(TablesToExtract[i],"%s",part3);
		    }
		  
		  if(ostype==1)
		  {
			  RecordFileName(1,osfilename);  
		    test = DoOSStuff((FILE *)NULL,0,osfilename,connectstring,file,
				   1,totallongfound,totalshortfound,
				   &longitemsfound,&shortitemsfound,
				   &Ijunk,&Ijunk);
			  RecordFileName(2,osfilename);  
		  }
		  else if(gmtype==1)
		  {
			  RecordFileName(1,osfilename);  
		    test = DoOSStuff((FILE *)NULL,0,osfilename,connectstring,file,
				   2,totallongfound,totalshortfound,
				   &longitemsfound,&shortitemsfound,
				   &Ijunk,&Ijunk);
			  RecordFileName(2,osfilename);  
		  }
		  
		  totallongfound  = totallongfound  + longitemsfound;
		  totalshortfound = totalshortfound + shortitemsfound;			
		  
		  for(i=0;i<numtables_to_extract;i++)
		    {
		      free(FCODEsOfTables [i]);
		      free(TablesToExtract[i]);
		    }
		  free(FCODEsOfTables);
		  free(TablesToExtract);
		  free(TableHasData);
		  free(TableGeoms);
		  numtables_to_extract = 0;

		  if(test==0)
		    {
		      
		      printf("Failed (2) to import line %s.  Aborting import.\n",osfilename);
		      closepolys();
		      closenets();
		      closemods(); 
		      closeareals();   
		      closeheaders();
		      closegrid();
		      FreeIgnore();
              FreeShapes(shapes_to_import); 
		      SH_FreeLists("ok","");
		      SH_FreeTree();
		      root=NIL;
		      L_root=NILL;
		      SH_PrintListChunk(1,-1,"","");  
		      return 0;	      
		    }		  
		}
	      fgets(aline,5000,fileptr);
	    }
	} 




      if(gd_to_import>0)
	{
	  rewind(fileptr);
	  
	  fgets(aline,5000,fileptr);
	  
	  
	  
	  while(!feof(fileptr))
	    {
	      sscanf(aline,"%s",import_type);
	      
	      if(
		 (!strcmp(import_type,"GEODATABASE")) ||
		 (!strcmp(import_type,"GEOMEDIAACC")) 
		 )
		{
		  COORDINATEKEEPER = 1; 

		  if(!strcmp(import_type,"GEODATABASE"))
		    {
		      type = 1;
		    }
		  if(!strcmp(import_type,"GEOMEDIAACC"))
		    {
		      type = 2;
		    }
		  
		  if(DEBUG==1)printf("Found geodatabase/geomedia access to import:\n");  	        
		  sscanf(aline,"%s%s%d",import_type,part2,&numtables_to_extract);
		  if(DEBUG==1)printf("  num tables %d\n",numtables_to_extract);
		  
		  fgets(aline,5000,fileptr);
		  for(i=12;i<(int)strlen(aline);i++)
		    {
		      gdfilename[i-12] = aline[i];
		    }
		  gdfilename[i-12] = '\0';
		  len = strlen(gdfilename);
		  for(i=0;i<len;i++)
		    {
		      if((gdfilename[i] == 10)||(gdfilename[i] == 13))
			{
			  gdfilename[i] = '\0';
			}
		    }
		  
		    printf("extract found gdfilename %s\n",gdfilename);
		  
		    TablesToExtract = (char **)malloc(sizeof(char *)*(numtables_to_extract+5));
		    FCODEsOfTables  = (char **)malloc(sizeof(char *)*(numtables_to_extract+5));
		    TableHasData    = (int *)malloc(sizeof(int)*(numtables_to_extract+5));
	  		  
			
		  for(i=0;i<numtables_to_extract;i++)
		    {
			  TableHasData[i] = 0;

		      fgets(aline,5000,fileptr);
		      
		      sscanf(aline,"%s%s%s",part1,part2,part3);
		      
		      
		      FCODEsOfTables [i] = (char *)malloc(strlen(part2)+5);
		      TablesToExtract[i] = (char *)malloc(strlen(part3)+5);
		      sprintf(FCODEsOfTables [i],"%s",part2);
		      sprintf(TablesToExtract[i],"%s",part3);			  
		   }
		  
		  if(type==2)
		  {
            printf("should not be here\n");
			exit(-1);
		  }
		  else
		  {
			  thislen = strlen(gdfilename);

			  if(
	             (gdfilename[thislen-4]=='.')                                    &&
	             ( (gdfilename[thislen-3]=='m') || (gdfilename[thislen-3]=='M') ) &&
	             ( (gdfilename[thislen-2]=='d') || (gdfilename[thislen-2]=='D') ) &&
	             ( (gdfilename[thislen-1]=='b') || (gdfilename[thislen-1]=='B') )
				)
			  {
			  RecordFileName(1,gdfilename);  
			    printf("Import personal geodatabase %s\n",gdfilename);
                test = OpenGeoDatabase(1 , gdfilename,0 ,
					totallongfound,totalshortfound,&longitemsfound,&shortitemsfound);
			  RecordFileName(2,gdfilename);  
			  }
			  else
			  {
			  RecordFileName(1,gdfilename);  
			    printf("Import file geodatabase %s\n",gdfilename);
                test = OpenGeoDatabase(0   , gdfilename,0 ,
					totallongfound,totalshortfound,&longitemsfound,&shortitemsfound);
			  RecordFileName(2,gdfilename);  
			  }
		  }

		  totallongfound  = totallongfound  + longitemsfound;
		  totalshortfound = totalshortfound + shortitemsfound;			
		 

		  

		  for(i=0;i<numtables_to_extract;i++)
		  {
		    free(FCODEsOfTables [i]);
		    free(TablesToExtract[i]);
		  }
		  free(FCODEsOfTables);
		  free(TablesToExtract);
		  free(TableHasData);
		  FCODEsOfTables  = NULL;
		  TablesToExtract = NULL;
		  TableHasData    = NULL;



		  
		  if(test==0)
		    {
		      
		      printf("Failed (3) to import line %s.  Aborting import.\n",aline);
		      closepolys();
		      closenets();
		      closemods(); 
		      closeareals();   
		      closeheaders();
		      closegrid();
		      FreeIgnore();
              FreeShapes(shapes_to_import); 
		      SH_FreeLists("ok","");
		      SH_FreeTree();
		      root=NIL;
		      L_root=NILL;
		      SH_PrintListChunk(1,-1,"","");  
		      return 0;	      
		    }		  
		  numtables_to_extract = 0;
		}
	      fgets(aline,5000,fileptr);
	    }
	} 
      
#endif
      
      
      
      
      
      num_dted = 0;


      rewind(fileptr);      
      fgets(aline,5000,fileptr);
      
      while(!feof(fileptr))
	{
	  sscanf(aline,"%s",import_type);
	  
	  if(
	     (!strcmp(import_type,"DTED"))      ||
	     (!strcmp(import_type,"GEOTIFF"))   ||
	     (!strcmp(import_type,"NITF"))
	     )
	    {
	      num_dted = num_dted+1;
	    }
	  fgets(aline,5000,fileptr);
	}



      if(num_dted>0)
	{
	  dtedlines  = (char **)malloc(sizeof(char *)*num_dted);
	  if(dtedlines==NULL)
	    {
	      printf("out of memory when trying to allocate %d for dtedlines\n",num_dted);
	      exit(-1);
	    }
	  dtedlayers = (char **)malloc(sizeof(char *)*num_dted);
	  if(dtedlayers==NULL)
	    {
	      printf("out of memory when trying to allocate %d for dtedlayers\n",num_dted);
	      exit(-1);
	    }
	}

      
      rewind(fileptr);      
      num_dted = 0;
      fgets(aline,5000,fileptr);
      
      while(!feof(fileptr))
	{
	  import_type[0] = '\0';
	  layerstring[0] = '\0';
	  
	  sscanf(aline,"%s",import_type);

	  if(
	     (!strcmp(import_type,"DTED"))      ||
	     (!strcmp(import_type,"GEOTIFF"))   ||
	     (!strcmp(import_type,"NITF"))
	     )
	    {
	      sprintf(layerstring,"%s",GetLayerPart(aline));
	      
	      dtedlines [num_dted] = (char *)malloc(strlen(aline)       + 10);
	      dtedlayers[num_dted] = (char *)malloc(strlen(layerstring) + 10);

	      sprintf(dtedlines [num_dted],"%s",aline);
	      sprintf(dtedlayers[num_dted],"%s",GetLayerPart(aline));

	      num_dted = num_dted + 1;
	    }
	  fgets(aline,5000,fileptr);
	}


    



            
      
      SortDTEDLayers(0,num_dted-1);
      
      

      for(i=0;i<num_dted;i++)
	{
	  import_type[0] = '\0';
	  layerstring[0] = '\0';

	  if(ABORT_EXTRACT==1)
	  {
        break;
	  }

	  sscanf(dtedlines[i],"%s",import_type);
	 
	  if(!strcmp(import_type,"DTED"))
	    {
		  COORDINATEKEEPER = 1; 

	      
	      sprintf(curr_file,"%s",GetLastPart(dtedlines[i]));
	      
	      
	      sprintf(layerstring,"%s",dtedlayers[i]);
	      
	      test = import_dted(curr_file,file,files_done,layerstring,
				 totallongfound,totalshortfound);
	      files_done++;
	      dted_done = 1;

	    }
	  else if(!strcmp(import_type,"GEOTIFF"))
	    {
	      
	      sprintf(curr_file,"%s",GetLastPart(dtedlines[i]));
	      
	      
	      sprintf(layerstring,"%s",dtedlayers[i]);
	      
		  
	      test = import_geotiff(curr_file,file,files_done,layerstring,
				 totallongfound,totalshortfound,1,destination);
	      files_done++;
	      dted_done = 1;
	    }
	  else if(!strcmp(import_type,"NITF"))
	    {
	      
	      sprintf(curr_file,"%s",GetLastPart(dtedlines[i]));
	      
	      
	      sprintf(layerstring,"%s",dtedlayers[i]);
	      
		  
	      test = import_geotiff(curr_file,file,files_done,layerstring,
				 totallongfound,totalshortfound,2,destination);
	      files_done++;
	      dted_done = 1;
	    }
	  else
	    {
	      printf("(2) Each line must be of the form [DTED/SHAPE] <lod identifier> <filename>\n");
	      test = -1;
	    }
	  
	  if(test==0)
	    {
	      printf("Failed (4) to import line:\n%s\nIgnoring this line.\n",aline);
	    }
  	}
      

      if(num_dted>0)
	{
	  for(i=0;i<num_dted;i++)
	    {
	      free(dtedlines[i]);
	      free(dtedlayers[i]);
	    }
	}

      if(num_dted>0)
	  {
		 free(dtedlines);
         free(dtedlayers);
	  }
      dtedlines  = 0;
      dtedlayers = 0;
      num_dted   = 0;
      

	  if(CREATING_TREX==1)
	  {
	    wroteone = 0;

		

		if(global_trex_layer==0)
		{
	      if(minmax_dem_finished_found0>0)
		  { 
		    fprintf(badtrex,"RANGE_SENTINEL  %.3lf %.3lf\n",min_g_dem_elev_finished0,max_g_dem_elev_finished0);
		  } 
		}
		else if(global_trex_layer==1)
		{
	      if(minmax_dem_finished_found1>0)
		  { 
		    fprintf(badtrex,"RANGE_SENTINEL %.3lf %.3lf\n",min_g_dem_elev_finished1,max_g_dem_elev_finished1);
		  } 
		}
		else if(global_trex_layer==2)
		{
	      if(minmax_dem_finished_found2>0)
		  { 
		    fprintf(badtrex,"RANGE_SENTINEL %.3lf %.3lf\n",min_g_dem_elev_finished2,max_g_dem_elev_finished2);
		  } 
		}
		else if(global_trex_layer==3)
		{
	      if(minmax_dem_finished_found3>0)
		  { 
		    fprintf(badtrex,"RANGE_SENTINEL %.3lf %.3lf\n",min_g_dem_elev_finished3,max_g_dem_elev_finished3);
		  }
		}


		if(AssessDiffs(mindiff,
			           min_fin_xml_found0,max_fin_xml_found0,
					   min_xml_elev_finished0,max_xml_elev_finished0,

			           minmax_dem_finished_found0,minmax_dem_finished_found0,
					   min_g_dem_elev_finished0,max_g_dem_elev_finished0,

			           min_fin_header_found0,max_fin_header_found0,
					   min_fin_header0,max_fin_header0
					   ))
		{
		  if((min_fin_xml_found0>0)&&(max_fin_xml_found0>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM XML elevation range:              %lf - %lf\n",min_xml_elev_finished0,max_xml_elev_finished0);
		  } 
		  else
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Unable to compute Finished DEM XML elevation range\n");
		  } 


	      if(minmax_dem_finished_found0>0)
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM elevation range:                  %lf - %lf\n",min_g_dem_elev_finished0,max_g_dem_elev_finished0);
		  } 
		  else
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Unable to compute Finished DEM elevation range\n");
		  } 

		  if((min_fin_header_found0>0)&&(max_fin_header_found0>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM GeoTIFF tag elevation range:      %lf - %lf\n",min_fin_header0,max_fin_header0);
		  } 
		  else
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Unable to compute Finished DEM GeoTIFF tag elevation range\n");
		  }  
          
		  wroteone = 1;
		}


		if(AssessDiffs(mindiff,
			           min_fin_xml_found1,        max_fin_xml_found1,
					   min_xml_elev_finished1,    max_xml_elev_finished1,
			           minmax_dem_finished_found1,minmax_dem_finished_found1,
					   min_g_dem_elev_finished1,  max_g_dem_elev_finished1,
			           min_fin_header_found1,     max_fin_header_found1,
					   min_fin_header1,           max_fin_header1
					   ))
		{
		  if((min_fin_xml_found1>0)&&(max_fin_xml_found1>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (1) XML elevation range:          %lf - %lf\n",min_xml_elev_finished1,max_xml_elev_finished1);
		  } 
	      if(minmax_dem_finished_found1>0)
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (1) elevation range:              %lf - %lf\n",min_g_dem_elev_finished1,max_g_dem_elev_finished1);
		  } 
		  if((min_fin_header_found1>0)&&(max_fin_header_found1>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (1) GeoTIFF tag elevation range:  %lf - %lf\n",min_fin_header1,max_fin_header1);
		  } 
		  wroteone = 1;
		}



		if(AssessDiffs(mindiff,
			           min_fin_xml_found2,        max_fin_xml_found2,
					   min_xml_elev_finished2,    max_xml_elev_finished2,
			           minmax_dem_finished_found2,minmax_dem_finished_found2,
					   min_g_dem_elev_finished2,  max_g_dem_elev_finished2,
			           min_fin_header_found2,     max_fin_header_found2,
					   min_fin_header2,           max_fin_header2
					   ))
		{
		  if((min_fin_xml_found2>0)&&(max_fin_xml_found2>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (2) XML elevation range:          %lf - %lf\n",min_xml_elev_finished2,max_xml_elev_finished2);
		  } 
	      if(minmax_dem_finished_found2>0)
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (2) elevation range:              %lf - %lf\n",min_g_dem_elev_finished2,max_g_dem_elev_finished2);
		  } 
		  if((min_fin_header_found2>0)&&(max_fin_header_found2>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (2) GeoTIFF tag elevation range:  %lf - %lf\n",min_fin_header2,max_fin_header2);
		  } 
		  wroteone = 1;
		}

		if(AssessDiffs(mindiff,
			           min_fin_xml_found3,        max_fin_xml_found3,
					   min_xml_elev_finished3,    max_xml_elev_finished3,
			           minmax_dem_finished_found3,minmax_dem_finished_found3,
					   min_g_dem_elev_finished3,  max_g_dem_elev_finished3,
			           min_fin_header_found3,     max_fin_header_found3,
					   min_fin_header3,           max_fin_header3
					   ))
		{
		  if((min_fin_xml_found3>0)&&(max_fin_xml_found3>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (3) XML elevation range:          %lf - %lf\n",min_xml_elev_finished3,max_xml_elev_finished3);
		  } 
	      if(minmax_dem_finished_found3>0)
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (3) elevation range:              %lf - %lf\n",min_g_dem_elev_finished3,max_g_dem_elev_finished3);
		  } 
		  if((min_fin_header_found3>0)&&(max_fin_header_found3>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Finished DEM (3) GeoTIFF tag elevation range:  %lf - %lf\n",min_fin_header3,max_fin_header3);
		  } 
		  wroteone = 1;
		}






		

		if(AssessDiffs(mindiff,
			           min_raw_xml_found,max_raw_xml_found,
                       min_xml_elev_raw,max_xml_elev_raw,
			           minmax_dem_raw_found,minmax_dem_raw_found,
					   min_g_dem_elev_raw,max_g_dem_elev_raw,
			           min_raw_header_found,max_raw_header_found,
					   min_raw_header,max_raw_header
					   ))
		{
		  if((min_raw_xml_found>0)&&(max_raw_xml_found>0))
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Raw DEM XML elevation range:          %lf - %lf\n",min_xml_elev_raw,max_xml_elev_raw);
		  } 
		  else
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Unable to compute Raw DEM XML elevation range\n");
		  } 

	      if(minmax_dem_raw_found>0)
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Raw DEM elevation range:              %lf - %lf\n",min_g_dem_elev_raw,max_g_dem_elev_raw);
		  } 
		  else
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Unable to compute Raw DEM elevation range\n");
		  } 

		  if((min_raw_header_found>0)&&(max_raw_header_found>0))
		  {  
		    fprintf(badtrex,"IMPORT_SENTINEL Raw DEM GeoTIFF tag elevation range:  %lf - %lf\n",min_raw_header,max_raw_header);
		  }  
		  else
		  { 
		    fprintf(badtrex,"IMPORT_SENTINEL Unable to compute Raw DEM GeoTIFF tag elevation range\n");
		  } 

	      fprintf(badtrex,"IMPORT_SENTINEL \n");
		  wroteone = 1;
		}


		



		




        if(DO_TIFFSURFACE==1)
		{ 
          

		  CreateDifferenceSurfaces(file_in,files_done,destination);

		}
		else
		{
	      fprintf(badtrex,"IMPORT_SENTINEL Did not create Finished/SRTM surface difference file because:\n");

          if(DO_TIFFSURFACE==0)
		  {
	        fprintf(badtrex,"IMPORT_SENTINEL   Environment variable DO_TIFFSURFACE is set to 0\n");
		  }
		}

	  } 




	  fclose(fileptr);


      
      if(test==0)
	{
	  
	  printf("Failed (5) to import place 2\n");	  

	  if(ErrStr[0]!='\0')
	  {
        PopExtrErr();
	  }
	  closepolys();
	  closenets();
	  closemods(); 
	  closeareals();   
	  closeheaders();
	  closegrid();
	  FreeIgnore();
      FreeShapes(shapes_to_import); 
	  SH_FreeLists("ok","");
	  SH_FreeTree();
	  root=NIL;
	  L_root=NILL;
	  SH_PrintListChunk(1,-1,"","");  
	  return 0;	      
	}

    

    


     if(dted_done==1)
	{
	  
	  SH_PrintListChunk(0,-1,"","");
	  totaldtedlong  = scc_long_items;
	  totaldtedshort = scc_short_items;

	  SH_FreeLists("ok","");
	  SH_FreeTree();
	}

    
    
     
     
     if(USE_DOS==0)
       {
	 sprintf(filename,"%s/copiedfile.shp",destination);
	 openfile = fopen(filename,"r");
	 if(openfile!=NULL)
	   {
	     fclose(openfile);
	     sprintf(command1,"rm %scopiedfile.shp",destination);
	     sprintf(command2,"rm %scopiedfile.shx",destination);
	     sprintf(command3,"rm %scopiedfile.dbf",destination);
	     system(command1);
	     system(command2);
	     system(command3);
	   }
       }
     else
       {
	 sprintf(filename,"%s/copiedfile.shp",destination);
	 openfile = fopen(filename,"r");
	 if(openfile!=NULL)
	   {
	     fclose(openfile);
	     sprintf(command1,"del \"%scopiedfile.shp\"",destination);
	     sprintf(command2,"del \"%scopiedfile.shx\"",destination);
	     sprintf(command3,"del \"%scopiedfile.dbf\"",destination);
	     system(command1);
	     system(command2);
	     system(command3);
	   }
       }



    


      scc_long_items  = totaldtedlong  + totallongfound  ; 
      scc_short_items = totaldtedshort + totalshortfound ;

      sh_write_headers(1);

      SH_PrintListChunk(1,-1,"","");  

      closepolys();
      closenets();
      closemods();
      closeareals();   
      closeheaders();
      closegrid();
      FreeIgnore();
      FreeShapes(shapes_to_import); 

      
      sh_update_shape_import(file,files_done);
    }
  else
    {
      printf("bad database type! %d\n",TYPE);
    }

  FreeFirstList();

  return(test);

#endif
}






int Extract(int TYPE, char file[])
{
  int junk;

  if((TYPE==0)||(TYPE==1))
    {
      #if(NGA_TYPE==0)
      extern int Extract_STF(int TYPE, char file[],char destination[]);

      junk = Extract_STF(TYPE,file,importoutputdir);
      #endif
    }
  else
    {
      junk = Extract_NGA(TYPE,file,importoutputdir);
    }

  return junk;
}
