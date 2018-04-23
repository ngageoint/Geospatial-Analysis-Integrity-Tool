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







#include <afxdisp.h>        


#pragma warning(push)
#pragma warning(disable:4192) 
#pragma warning(disable:4146) 
#pragma warning(disable:4042)

#import "PService.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PBasic.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PClient.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PCSS.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids
#import "PView.tlb" raw_interfaces_only, raw_native_types, no_namespace, named_guids

#import <C:\Program Files (x86)\Common Files\microsoft shared\Office14\mso.dll> rename("RGB","MSRGB")
#import <C:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\vbe6ext.olb> rename("Reference","ignorethis"), rename("VBE","testVBE"), named_guids exclude("CommandBars")
#import <C:\Program Files (x86)\Microsoft Office\Office14\excel.exe> exclude("IFont", "IPicture") rename("RGB", "ignorethis"), rename("DialogBox", "ignorethis"), rename("VBE", "testVBE"), rename("ReplaceText", "EReplaceText"), rename("CopyFile","ECopyFile"), rename("FindText", "EFindText"), rename("NoPrompt", "ENoPrompt")

#pragma warning(pop)



#include "GAIT_API.h"

int CHECKLISTDEBUG=0;

const short gmmtPoint  = 1;
const short gmmtLinear = 2;
const short gmmtAreal  = 4;



  extern "C" long int NumPointNodes,NumLinearNodes,NumArealNodes,TestNumLinearNodes,TestNumArealNodes;
  extern "C" unsigned long int gridpointsread;
  extern "C" int SH_Points_Done, SH_Linears_Done, SH_Areals_Done, SH_bad_objects,SH_BadCoords,
    SH_Linears_Prev, SH_Areals_Prev, EAREALNODELIMIT,ELINEARNODELIMIT, numtables_to_extract,
	SzD, SzI, SzL, scc_short_items, scc_long_items, ABORT_EXTRACT,CO_INNITTED,SH_Null_Shapes,
	global_gifd_id, SH_Zero_Shapes, SH_Bad_DBF, FORCE_REDRAW,NumPointParts,NumLineParts,NumAreaParts,
	NumMultiPartPointFeatures,NumMultiPartLineFeatures,NumMultiPartAreaFeatures,
	num3point, num3line, num3area, NumHoles,ATTRIBUTION_TYPE;
  extern "C" int *TableGeoms, *gifd_attr_enum_count;
  extern "C" char **TablesToExtract, **FCODEsOfTables;
  extern "C" unsigned char TIM_STRING,TIM_INTEGER,TIM_ENUMERANT,TIM_DOUBLE,
    TIM_UNI,TIM_METRE,TIM_UNITLESS;
  extern "C" FILE *arealbin,*roadnetbin,*modelbin,*emptyshapes,*headertxt,*headerbin,*badshapeout,
	  *multipartlines,*multipartareas,*mixnull;

  extern "C" int ATTRTYPE_ANY_MGCP();
  extern "C" int DoNFI();

  extern "C" void NFIWrapper(char *saveNFI, char *saveNFN, char *saveNAM, int classcode, int geomtype);

  extern "C" int NOTYPE;
  extern "C" int C_TYPE;
  extern "C" int D_TYPE;
  extern "C" int I_TYPE;

  extern "C" int LONG_ATTR_ID     ;
  extern "C" int LONG_ATTR_ID_INFO;
  extern "C" int LONG_ATTR_AREA   ;
  extern "C" int LONG_ATTR_HGT    ;
  extern "C" int LONG_ATTR_LEN    ;
  extern "C" int LONG_ATTR_NAM    ;
  extern "C" int LONG_ATTR_ZV2    ;
  extern "C" int START_ID         ;
  extern "C" int STOP_ID          ;
  extern "C" int LONG_ATTR_LEN    ;
  extern "C" int LONG_ATTR_WID    ;
  extern "C" double CLIPLLX,CLIPLLY,CLIPURX,CLIPURY;
  extern "C" int CLIPEXTRACT;

  extern "C" void sh_testxyglobal(double x,double y,double z);
  extern "C" void GetShortName(char shortname[],char *shape_to_import);
  extern "C" char *GetECCCode(int code);
  extern "C" char *GetECCLabel(int code);
  extern "C" char *GetEACLabel(int code);
  extern "C" int GetClassCodeAsSubstring(char *shortname);
  extern "C" int GetClassCodeFromName(char *shortname);
  extern "C" int ConvertAttrCode(char *oldcode, int lastgoodcode);
  extern "C" int SH_AddLongAttribute(struct sh_long_attr_list SAC_LONG[],
				 int addindex,int attr_code, unsigned char unit_code, 
				 unsigned char scale_code, unsigned char SEDRIStype, 
				 int Ivalue, double Dvalue, char Cvalue[]);
  extern "C" void SH_AddShortAttribute(struct sh_short_attr_list SAC_SHORT[],
				   int addindex,int attr_code);
  extern "C" int SH_FindSCC_LONG(int type, int scc, int numsac, 
			     struct sh_long_attr_list SAC_LONG[], int record_num);
  extern "C" int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			      struct sh_short_attr_list SAC_SHORT[], int record_num);
  extern "C" int ConvertClassCode(char *oldcode, int FID);
  extern "C" void SH_PrintListChunk(int done, int numtables,char *filename, char *tablename);
  extern "C" void SH_FreeLists(char *sentinel,char *shapename);
  extern "C" void SH_FreeTree(); 
  extern "C" void update_shape_import(char *allshapes,char *curr_file,int files_done, 
				  int dbfdone,int dbftotal,
				  int points_done,int linears_done,int areals_done,
				  unsigned long int gptsdone,
				  int DONE, int extra_type,int extra1, int extra2, int badstuff,
				  int badstuff2, int nullshapes, int zeroparts, int baddbf);
  extern "C" char timtoupper(char inchar);
  extern "C" void AddBadAttr(int badattr, char *ENUM, int record_num);
  extern "C" int GetNewEnum(int attr_code,char *oldenum, int report_if_bad, int record_num);
  extern "C" void AddBadEnumType(int badattr, char *filename, char *ENUM, char *goodenum);
  extern "C" void ReSetArealFile();
  extern "C" void ReSetLinearFile();
  extern "C" int SH_PointInAreal(double px,double py,double * x,double * y,
			     int startindex, int stopindex);
  extern "C" int TestGeodetic(double x, double y, int FID, char *shape_to_import);
  extern "C" void WriteGFID(int datafound,char *charval, int Ival, double Dval,
			   int datatype, int attr_type);
  extern "C" void OSErrorHandle(int type,int errnum, char *errstr);
  extern "C" void AddBadHole(int record_num, int part_num, char *shapename, int type);
  extern "C" void AddUnkAttr(char *badcode);
  extern "C" int LongCode(char possiblecode[]);
  extern "C" void Record_LL_Mapping(int GFID_index,int longindex, int FID);
  extern "C" int IsGFID(char *tester);
  extern "C" int IsArea(char *tester);
  extern "C" int IsHgt(char *tester);
  extern "C" int IsLen(char *tester);
  extern "C" int IsNam(char *tester);
  extern "C" int IsZv2(char *tester);
  extern "C" int IsObjid(char *tester);
  extern "C" void HandleDoublePickListVal(int attr_code, double thisrealval,int *GEOMTYPE,
							 unsigned char *SEDRIStype, int *retenum, int record_num);
  extern "C" void AddNullShape(int FID);
  extern "C" char *GetFrontPart();
  extern "C" int SetClassLevel(char classcode[],char tablename[]);
  extern "C" extern void AddBlankAttr(char filename[],char attribute[],int record_num,int datatype);
  extern "C" int PossibleFCODE(char possiblecode[]);
  extern "C" void AddNoFCODE(char *filename1);
  extern "C" void AddBadClass(char *badcode, char *expectedcode, int FID);
  extern "C" void TestWidth(int fieldtype,char attrcode[], int width);
  extern "C" int GetOptional(char *fcode,char *attrcode,int geom);
  extern "C" int EvalValue(int oldval, char *shape_to_import,char attr_name[],int thisintval,double thisrealval,
	       char *thisstrval,int type);
  extern "C" char *GetGMType(int type);
  extern "C" void GetDateComponents(int intdate,int *day,int *month,int *year);


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



double SaveAreaD,SaveHgtD,SaveZv2D,SaveLenD,SaveNamD;
int SaveAreaI,SaveHgtI,SaveZv2I,SaveLenI,SaveNamI,
    area_found,hgt_found,len_found,nam_found,zv2_found,featureid,
	newfeatureid,gifd_found,class_code,sh_sac_short_index,sh_sac_long_index,seeitgeom;
char SaveAreaC[1000],SaveHgtC[1000],SaveZv2C[1000],SaveLenC[1000],
     SaveNamC[1000],saveNFI[1000],saveNFN[1000],saveNAM[1000],
     layername[1000],thisgifd[1000],tempstr[1000];
struct sh_long_attr_list  SH_SAC_LONG [1000];   
struct sh_short_attr_list SH_SAC_SHORT[1000];



void GetCoords(char *trexid,int *llx,int *lly)
{
  int this_east;
  int this_north;
  char temp[100];

  temp[0] = trexid[4];
  temp[1] = trexid[5];
  temp[2] = trexid[6];
  temp[3] = '\0';
  sscanf(temp,"%d",&this_east);


  temp[0] = trexid[1];
  temp[1] = trexid[2];
  temp[2] = '\0';
  sscanf(temp,"%d",&this_north);

  if((trexid[3]=='w')||(trexid[3]=='W'))
  {
    this_east = this_east*-1;
  }
  if((trexid[0]=='s')||(trexid[0]=='S'))
  {
    this_north = this_north*-1;
  }

  *llx = this_east;
  *lly = this_north;
}



extern "C" int GetHeaderErrCount()
{
  FILE *readfile;
  char txtfile[1000],aline[1000];
  int count;
  extern char indirectory[];
  extern char *GetSlash();

  count = 0;  

  sprintf(txtfile,"%ssummary_files%sTREx_header_errors.txt",indirectory,GetSlash());

  

  readfile = fopen(txtfile,"r");
  if(readfile!=NULL)
  {   
    fgets(aline,1000,readfile);

	while(!feof(readfile))
	{   
	  if(!strncmp(aline," Error:   ",10))
	  { 
	    count = count + 1;
	  }  

      fgets(aline,1000,readfile);
	}   
	fclose(readfile);
  }  
  else
  {
	printf("couldnt open %s\n",txtfile);
    return -1;
  }

  return count;
}


extern "C" void GetPostMaskCount(int *numvoid, int *nummask)
{
  FILE *readfile;
  char txtfile[1000],aline[1000],builder[100];
  int i,testnum=0;
  extern char outdirectory[];

  *numvoid = 0;
  *nummask = 0;

  sprintf(txtfile,"%stimp.bin",outdirectory);
  readfile = fopen(txtfile,"r");
  if(readfile!=NULL)
  {
    fgets(aline,1000,readfile);
    while(!feof(readfile))
	{
	  if(!strncmp(aline,"VOID_SENTINEL ",14))
	  {
        if(strstr(aline,"Found all posts had value"))
		{
		  *nummask = *nummask + 1;
		}
		if(strstr(aline,"posts with value -32767"))
		{
		  if(strlen(aline)>50)
		  {
		    
		    for(i=22;i<35;i++)
			{ 
              builder[i-22]=aline[i];
			  if(aline[i]=='/')
			  { 
			    builder[i-22] = '\0';
			    break;
			  } 
			} 
			if(CHECKLISTDEBUG==1)printf("Built %s\n",builder);
			testnum = atoi(builder);
			if((testnum>=0)&&(testnum<=1000000000))
			{
			  *numvoid = testnum;
			}
		  } 
		}
	  }
	  fgets(aline,1000,readfile);
	}
    fclose(readfile);
  } 
}



void SetCell(Excel::_WorksheetPtr pSheet, char *CellAlias,int type,char *SetStr,int SetInt)
{
  

  VARIANT xVariant,yVariant,vVariant;

  if(CHECKLISTDEBUG==1)
  {
    printf("Set checklist with cell alias %s\n",CellAlias);
    if(type==1)
	{
	  printf("...use (1) value %s\n",SetStr);
	}
	else if(type==2)
	{
	  printf("...use (2) value %d\n",SetInt);
	}
  }


  Excel::RangePtr pRange = pSheet->GetRange(_bstr_t(CellAlias));

  if(pSheet==NULL)
  {
    printf("NULL sheet %s\n",CellAlias);
  }

  xVariant.vt = VT_UI4;
  xVariant.lVal = 1; 

  yVariant.vt = VT_UI4;
  yVariant.lVal = 1; 

  vVariant.vt = VT_I4; 
  vVariant.lVal = SetInt;

  if(type==1)
  {
	
	pRange->Item[xVariant][yVariant] = _bstr_t(SetStr);
  }
  else if(type==2)
  {
    
    pRange->Item[xVariant][yVariant] = vVariant;
  }	
}


extern "C" void ProcessExcel(char *infile, char *sheetname, char *analysis_type)
{
  HRESULT hr;
  FILE *readfile;
  char temp[1000],minz[1000],maxz[1000],txtfile[1000],aline[1000],junk[1000],lpc_found[1000],env_found[1000];
  int i,checkfound,checkcount,llx,lly,count,numvoid,nummask,num_unexpected,num_missing,len;
  extern char VersionStrCopy[];
  extern char indirectory[];
  extern char outdirectory[];
  extern char *TREX_CHECKLIST_ALIASES[38];
  extern char *TREX_ANNOTATIONS[38];
  extern void CoUninit();
  extern int FileExists(char *filename);
  extern char *GetTRExSubName2(char *thepath, int id);
  extern int GetTRExMetaCount(char *txtfile, int verbose);
  extern char *GetSlash();
  extern char *MakePlural(int num);
  extern void GetNumConditionsByAnnotation(char *annotation,int *checkfound,int *checkcount);

  char *ControlPointAliases[10]=
  {
	"ICESAT_NUM",
	"ICESAT_STD",
	"ICESAT_MEAN",
	"ICESAT_RMSE",
	"ICESAT_LE90",
	"GCP_NUM",
	"GCP_STD",
	"GCP_MEAN",
	"GCP_RMSE",
	"GCP_LE90"
  };

  if(CHECKLISTDEBUG==1)
  {
	printf("Create TREx checklist %s, sheet %s, for analysis type %s\n",infile,sheetname,analysis_type);
  }

  if(FileExists(infile)==0)
  {
	printf("File does not exist: %s\n",infile);
	printf("...not creating TREx checklist file\n");
	return;
  }


  Excel::_ApplicationPtr XL;
  Excel::_WorkbookPtr pWorkbook;

  try
  {
    if(CO_INNITTED==0)
	{  
      hr = CoInitialize(NULL);
      if(!SUCCEEDED(hr))
	  {  
	    printf("CoInit failed.  Unable to create TREx Excel checklist\n");
	    return;
	  }  
	  CO_INNITTED = 1;
	}  

    XL.CreateInstance("Excel.Application");

    pWorkbook=XL->Workbooks->Open(infile);


    XL->PutVisible(0,FALSE);  

    Excel::_WorksheetPtr pSheet = pWorkbook->Sheets->Item[sheetname];


    

	SetCell(pSheet,"GAIT_VERSION",1,VersionStrCopy,-999);            
	SetCell(pSheet,"CELL_ID",1,GetTRExSubName2(indirectory,1),-999); 

	if(strlen(analysis_type)>10) 
	{
	  SetCell(pSheet,"REV_TYPE",1,analysis_type,-999); 
	}

	GetCoords(GetTRExSubName2(indirectory,1),&llx,&lly);

	SetCell(pSheet,"SW_LON",2,"N/A",llx);   
	SetCell(pSheet,"SW_LAT",2,"N/A",lly);   
	SetCell(pSheet,"NE_LON",2,"N/A",llx+1); 
	SetCell(pSheet,"NE_LAT",2,"N/A",lly+1); 



    sprintf(temp ,"%sXML%sTREx_finished_%s_results.txt",indirectory,GetSlash(),GetTRExSubName2(indirectory,1));     
	count = GetTRExMetaCount(temp,1); 
	SetCell(pSheet,"XML_ERRORS",2,"N/A",count); 


	SetCell(pSheet,"GEOTIFF_ERRORS",2,"N/A",GetHeaderErrCount()); 


	GetPostMaskCount(&numvoid,&nummask);
	SetCell(pSheet,"VOIDS",2,"N/A",numvoid); 
	SetCell(pSheet,"EMPTY_MASKS",2,"N/A",nummask); 


	num_unexpected = 0;
	num_missing    = 0;
	lpc_found[0] = '\0';
	env_found[0] = '\0';

    sprintf(txtfile,"%stimp.bin",outdirectory);
    readfile = fopen(txtfile,"r");
    if(readfile!=NULL)
	{ 
      fgets(aline,1000,readfile);
      while(!feof(readfile))
	  { 
	    if(!strncmp(aline,"RANGE_SENTINEL ",14))
		{ 
		  sscanf(aline,"%s%s%s",junk,minz,maxz);
	 	  SetCell(pSheet,"DEM_MIN",1,minz,-999); 
	 	  SetCell(pSheet,"DEM_MAX",1,maxz,-999); 
		  break;
		} 
		else if(!strncmp(aline,"IMPORT_SENTINEL   Found unexpected file",39))
		{ 
		  num_unexpected = num_unexpected + 1;
		}
		else if(!strncmp(aline,"IMPORT_SENTINEL   But did not find",34))
		{ 
		  num_missing = num_missing + 1;
		}
		else if(!strncmp(aline,"LPC_SENTINEL ",13))
		{
		  sprintf(lpc_found,"%s",&aline[13]);
		}
		else if(!strncmp(aline,"ENV_SENTINEL ",13))
		{
		  sprintf(env_found,"%s",&aline[13]);
		}

	    fgets(aline,1000,readfile);
	  }
	}
    fclose(readfile);


	sprintf(junk,"%d unexpected file%s ; %d missing file%s",num_unexpected,MakePlural(num_unexpected),num_missing,MakePlural(num_missing));
	SetCell(pSheet,"TDF_PACKAGE",1,junk,-999); 


	if(strlen(lpc_found)==0)
	{
	  sprintf(lpc_found,"Not found");
	}
	if(strlen(env_found)==0)
	{
	  sprintf(env_found,"Not found");
	}
    len = strlen(lpc_found);
	if(lpc_found[len-1]<32)
	{
	  lpc_found[len-1] = '\0';
	}
    len = strlen(env_found);
	if(env_found[len-1]<32)
	{
	  env_found[len-1] = '\0';
	}

	SetCell(pSheet,"LPC_NAME",1,lpc_found,-999); 
	SetCell(pSheet,"ENVIRONMENT",1,env_found,-999); 




	

	for(i=0;i<38;i++)
	{
	  checkfound = 0;
	  checkcount = 0;
      if(CHECKLISTDEBUG==1)
	  {
		printf("Look for %s\n",TREX_ANNOTATIONS[i]);
	  }
	  GetNumConditionsByAnnotation(TREX_ANNOTATIONS[i],&checkfound,&checkcount);

	  if(checkfound>0)
	  {
	    SetCell(pSheet,TREX_CHECKLIST_ALIASES[i],2,"N/A",checkcount); 
	  }
	  else
	  {
	    
	    SetCell(pSheet,TREX_CHECKLIST_ALIASES[i],1,"N/A",-999); 
	  }
	}






	

    sprintf(txtfile,"%sctrlpt.bin",outdirectory);
    readfile = fopen(txtfile,"r");
    if(readfile!=NULL)
	{ 
	  for(i=0;i<10;i++)
	  {
	    fgets(aline,1000,readfile);
		if(feof(readfile))
		{
		  printf("unexpected EOF, unable to populate control point stats\n");
		  break;
		}
		if(strlen(aline)>20)
		{
		  printf("internal error: invalid copo value (%d) %s, unable to populate control point stats\n",strlen(aline),aline);
		  break;
		}

		len = strlen(aline);
	    if(aline[len-1]<32)
		{
	      aline[len-1] = '\0';
		}
        SetCell(pSheet,ControlPointAliases[i],1,aline,-999); 
	  }
	}
    fclose(readfile);




    pWorkbook->Close(VARIANT_TRUE); 




    
    

    XL->Quit(); 

    
    

    
  }
  catch(const _com_error& e)
  {
	printf("COM error:\n");
    printf(" Code = %08lx\n",      e.Error());
    printf(" Code meaning = %s\n", e.ErrorMessage());
	printf("   Cannot generate TREx checklist XLSX file from %s\n",infile);

	printf("Possibly you don't have Excel installed?\n");
	return;
  }

  
  
  
  

printf("done\n");

}


extern "C" int FME_AddBadType(int badattr, char *expect_type, char *got_type,
					int table_num,int last_table_num,char *dbname,
					char *tablename)
{
  extern FILE *badtypeout;
  extern char **gifd_attr_codes;
  extern int ATTRIBUTION_TYPE;

    if(ATTRIBUTION_TYPE<7)
	{
 	 if(
		(!strcmp(gifd_attr_codes[badattr],"ACE_EVAL_METHOD_CD")) ||
		(!strcmp(gifd_attr_codes[badattr],"ALE_EVAL_METHOD_CD")) ||
		(!strcmp(gifd_attr_codes[badattr],"SEC_CLS_AUTHORITY"))  ||
		(!strcmp(gifd_attr_codes[badattr],"SPEC_ID"))            ||
		(!strcmp(gifd_attr_codes[badattr],"HAS_INFO"))           ||
		(!strcmp(gifd_attr_codes[badattr],"ORIGINATING_SOURCE")) ||
		(!strcmp(gifd_attr_codes[badattr],"SEC_CLASS"))          ||
		(!strcmp(gifd_attr_codes[badattr],"UPDATE_SOURCE"))      ||
		(!strcmp(gifd_attr_codes[badattr],"Z_VALUE_TYPE"))
		)
	{
		return 0;
	}
	}


  if(table_num!=last_table_num)
  {
   fprintf(badtypeout,"\n\nAttributes with invalid data types found in:\n  %s: Table %s:\n",
	   dbname,tablename);
  }

  fprintf(badtypeout,"   Attribute %s:  expected %s, got %s\n",
      gifd_attr_codes[badattr], expect_type, got_type);

  return 1;

}


extern "C" int TestOSDataType(int tempcode,char *osname, char *tablename, char *attrname,
					int realtype, int table_num, int last_table_num)
{
  int Ijunk;
  extern char **gifd_attr_types;
  extern char **gifd_attr_codes;
  extern int ATTRIBUTION_TYPE;

    if(ATTRIBUTION_TYPE<7)
	{
	 if(
		(!strcmp(gifd_attr_codes[tempcode],"ACE_EVAL_METHOD_CD")) ||
		(!strcmp(gifd_attr_codes[tempcode],"ALE_EVAL_METHOD_CD")) ||
		(!strcmp(gifd_attr_codes[tempcode],"SEC_CLS_AUTHORITY"))  ||
		(!strcmp(gifd_attr_codes[tempcode],"SPEC_ID"))            ||
		(!strcmp(gifd_attr_codes[tempcode],"HAS_INFO"))           ||
		(!strcmp(gifd_attr_codes[tempcode],"ORIGINATING_SOURCE")) ||
		(!strcmp(gifd_attr_codes[tempcode],"SEC_CLASS"))          ||
		(!strcmp(gifd_attr_codes[tempcode],"UPDATE_SOURCE"))      ||
		(!strcmp(gifd_attr_codes[tempcode],"Z_VALUE_TYPE"))
		)
	 {
		return 0;
	 }
	}
	
	if(!strcmp( gifd_attr_types[tempcode],"Date"))
	{
		
		return 0;  
	}
    

    


	if(realtype==VT_BSTR)
	  {
	    if(
	       strcmp( gifd_attr_types[tempcode],"Notgiven") &&
	       strcmp( gifd_attr_types[tempcode],"String") 
	       )
	      {
		Ijunk = FME_AddBadType(tempcode,gifd_attr_types[tempcode],"String",
				       table_num,last_table_num,osname,tablename);
		return 1;
	      }
	  }
	
	else if((realtype==VT_R4)||(realtype==VT_R8))
	  {
 	  if(
	     strcmp( gifd_attr_types[tempcode],"Notgiven") &&
	     strcmp( gifd_attr_types[tempcode],"Double") &&
	     strcmp( gifd_attr_types[tempcode],"Real") &&
	     strcmp( gifd_attr_types[tempcode],"Float")
		)
		{
		  Ijunk = FME_AddBadType(tempcode,gifd_attr_types[tempcode],"Real",
				table_num,last_table_num,osname,tablename);
  	 	  return 1;
		}
	}
  else if((realtype==VT_I2)||(realtype==VT_UI2)||(realtype==VT_I4)||(realtype==VT_UI4))
	{
	  if(
	     strcmp( gifd_attr_types[tempcode],"Notgiven")      &&
	     strcmp( gifd_attr_types[tempcode],"Short Integer") &&
	     strcmp( gifd_attr_types[tempcode],"Integer")       &&
	     strcmp( gifd_attr_types[tempcode],"ShortInteger")  &&
	     strcmp( gifd_attr_types[tempcode],"Long Integer")  &&
	     strcmp( gifd_attr_types[tempcode],"LongInteger") 
	   )
		 {
		    Ijunk = FME_AddBadType(tempcode,gifd_attr_types[tempcode],"Integer",
				table_num,last_table_num,osname,tablename);
  		    return 1;
		 }
	}
  return 0;
}


extern "C" int ExtractPointFeature( _DGMPointGeometry *point_geom,int addtolong, int addtoshort, int type, int newfeatureid)
{
  
  HRESULT hr;
  _DGMPoint *pt;
  
  double x,y,z;
  int numverts,usethis,longindex,shortindex;
  
  
  numverts = -1;
  usethis  = 1;

  if(point_geom==NULL)
  {
    printf("Error: Null (pt) feature id %d\n",newfeatureid);
    AddNullShape(newfeatureid);
	return 1;
  }
  
  hr = point_geom->get_Origin(&pt);
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,49,"");
      return 0;
    } 
  
  pt->get_X(&x);
  pt->get_Y(&y);
  pt->get_Z(&z);
  pt->Release();
  
  usethis = 1;
  
  if(CLIPEXTRACT==1)
    {
      if((x<CLIPLLX) || (y<CLIPLLY) || 
	 (x>CLIPURX) || (y>CLIPURY))
	{
	  usethis = 0;
	}
    }
  
  if(TestGeodetic(x,y,newfeatureid,layername)==0)
    {
      usethis = 0;
      return 0;
    }
  
  
  if(usethis==1)
    {
      NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,seeitgeom);

      WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
      WriteGFID(1,SaveHgtC,SaveHgtI,SaveHgtD,hgt_found,LONG_ATTR_HGT);
      WriteGFID(1,SaveLenC,SaveLenI,SaveLenD,len_found,LONG_ATTR_LEN);
      WriteGFID(1,SaveNamC,SaveNamI,SaveNamD,nam_found,LONG_ATTR_NAM);
      WriteGFID(1,SaveZv2C,SaveZv2I,SaveZv2D,zv2_found,LONG_ATTR_ZV2);
      
      WriteGFID(gifd_found,thisgifd,-999,-999,C_TYPE,LONG_ATTR_ID);
      
      sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
      sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
      sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
      
      longindex  = SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index ,SH_SAC_LONG, newfeatureid );
      shortindex = SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, newfeatureid);
      
      sprintf(SH_SAC_LONG [0].lod_stuff,"%s",tempstr);
      sprintf(SH_SAC_SHORT[0].lod_stuff,"%s",tempstr);
      
      longindex  = longindex  + addtolong;
      shortindex = shortindex + addtoshort;
      
      
      num3point = num3point + 1;
      fwrite(&newfeatureid,    SzI,1,modelbin);
      fwrite(&longindex,       SzI,1,modelbin);
      fwrite(&shortindex,      SzI,1,modelbin);      
      fwrite(&global_gifd_id,  SzI,1,modelbin);
      
      Record_LL_Mapping(global_gifd_id,longindex,newfeatureid);
      
      fwrite(&numverts, SzI,1,modelbin);
      fwrite(&x,        SzD,1,modelbin);
      fwrite(&y,        SzD,1,modelbin);
      fwrite(&z,        SzD,1,modelbin);
      
      NumPointNodes = NumPointNodes + 1;
      SH_Points_Done = SH_Points_Done + 1;
      
      sh_testxyglobal(x,y,z);
    } 
  
  return 1;
}




extern "C" int ExtractLineFeature(_DGMPolylineGeometry *line_geom,int addtolong, int addtoshort, int thispart, int type, int newfeatureid)
{
  HRESULT hr;
  VARIANT vName;
  _DGMPoint *pt;
  _DGMPoints *pts;
  
  double x,y,z;
  long l_numverts;
  int numverts,nCount,usethis,longindex,shortindex;
  
  

  if(line_geom==NULL)
  {
    printf("Error: Null (ln) feature id %d\n",newfeatureid);
    AddNullShape(newfeatureid);
	return 1;
  }
  
  hr = line_geom->get_Points(&pts);
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,43,"");
      return 0;
    } 
  
  pts->get_Count(&l_numverts);
  if(!SUCCEEDED(hr))
    { 
      OSErrorHandle(type,44,"");
      return 0;
    }  
  
  
  numverts = (int)l_numverts;
  
  usethis = 1;
  
  for(nCount=1;nCount<=l_numverts;nCount++)
    { 
      vName.vt = VT_UI4;
      vName.lVal = (long)nCount;
      
      hr = pts->Item(vName,&pt);
      if(!SUCCEEDED(hr))
	{ 
	  OSErrorHandle(type,45,"");
	  return 0;
	}   
      
      pt->get_X(&x);
      pt->get_Y(&y);
      pt->get_Z(&z);
      pt->Release();
      
      if(CLIPEXTRACT==1)
	{  
	  if((x<CLIPLLX) || (y<CLIPLLY) || 
	     (x>CLIPURX) || (y>CLIPURY))
	    {
	      usethis = 0;
	      break;
	    } 
	}  
      
      if(TestGeodetic(x,y,newfeatureid,layername)==0)
	{
	  usethis = 0;
	  break;
	}  
    }   
  
  if(usethis==1)
    {
      
      if(thispart<=1)
	{
      NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,seeitgeom);

	  WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
	  WriteGFID(1,SaveHgtC,SaveHgtI,SaveHgtD,hgt_found,LONG_ATTR_HGT);
      WriteGFID(1,SaveLenC,SaveLenI,SaveLenD,len_found,LONG_ATTR_LEN);
      WriteGFID(1,SaveNamC,SaveNamI,SaveNamD,nam_found,LONG_ATTR_NAM);
	  WriteGFID(1,SaveZv2C,SaveZv2I,SaveZv2D,zv2_found,LONG_ATTR_ZV2);
	  WriteGFID(gifd_found,thisgifd,-999,-999,C_TYPE,LONG_ATTR_ID);
	}
      
      sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
      sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
      sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
      
      longindex  = SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index ,SH_SAC_LONG,  newfeatureid);
      shortindex = SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, newfeatureid);
      
      sprintf(SH_SAC_LONG [0].lod_stuff,"%s",tempstr);
      sprintf(SH_SAC_SHORT[0].lod_stuff,"%s",tempstr);
      
      longindex  = longindex  + addtolong;
      shortindex = shortindex + addtoshort;
      
      
      num3line = num3line + 1;
      fwrite(&newfeatureid,  SzI,1,roadnetbin);
      fwrite(&longindex,     SzI,1,roadnetbin);
      fwrite(&shortindex,    SzI,1,roadnetbin);
      fwrite(&global_gifd_id,SzI,1,roadnetbin);
      fwrite(&numverts,      SzI,1,roadnetbin);
      
      if(thispart<=1)
	{
	  Record_LL_Mapping(global_gifd_id,longindex,newfeatureid);
	}
      
      if(thispart>=1)
	{
	  fwrite(&newfeatureid,  SzI,1,multipartlines);
	  fwrite(&longindex,     SzI,1,multipartlines);
	  fwrite(&shortindex,    SzI,1,multipartlines);
	  fwrite(&global_gifd_id,SzI,1,multipartlines);
	  fwrite(&numverts,      SzI,1,multipartlines);
	}
      
      
      for(nCount=1;nCount<=l_numverts;nCount++)
	{
	  vName.vt = VT_UI4;
	  vName.lVal = (long)nCount;
	  
	  hr = pts->Item(vName,&pt);
	  if(!SUCCEEDED(hr))
	    {
	      OSErrorHandle(type,46,"");
	      return 0;
	    } 
	  
	  pt->get_X(&x);
	  pt->get_Y(&y);
	  pt->get_Z(&z);
	  pt->Release();
	  
	  
	  fwrite(&x,SzD,1,roadnetbin);
	  fwrite(&y,SzD,1,roadnetbin);
	  fwrite(&z,SzD,1,roadnetbin);
	  
	  if(thispart>=1)
	    {
	      fwrite(&x,SzD,1,multipartlines);
	      fwrite(&y,SzD,1,multipartlines);
	      fwrite(&z,SzD,1,multipartlines);
	    }
	  
	  
	  if(thispart<=1)
	    {
	      NumLinearNodes++;
	      TestNumLinearNodes++;
	    }
	  
	  sh_testxyglobal(x,y,z);
	}
      pts->Release();
      
      if(thispart<=1)
	{
	  SH_Linears_Done = SH_Linears_Done + 1;
	}
      
    } 
  
  return 1;
}



extern "C" int ExtractAreaNoHoles(_DGMPolygonGeometry *area_geom,int addtolong, int addtoshort, int thispart, int type, int newfeatureid)
{
  HRESULT hr;
  VARIANT vName;
  _DGMPoint *pt;
  _DGMPoints *pts;
  
  double x,y,z;
  long l_numverts;
  int numverts,ishole,nCount,usethis,longindex,shortindex;
  
  
  

  if(area_geom==NULL)
  {
    printf("Error: Null (ar) feature id %d\n",newfeatureid);
    AddNullShape(newfeatureid);
	return 1;
  }

  hr = area_geom->get_Points(&pts);
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,28,"");
      return 0;
    } 
  
  
  pts->get_Count(&l_numverts);
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,29,"");
      return 0;
    } 
  
  numverts = (int)l_numverts;
  
  
  usethis = 1;
  
  for(nCount=1;nCount<=l_numverts;nCount++)
    {
      vName.vt = VT_UI4;
      vName.lVal = (long)nCount;
      
      hr = pts->Item(vName,&pt);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,30,"");
	  return 0;
	} 
      
      
      pt->get_X(&x);
      pt->get_Y(&y);
      pt->get_Z(&z);
      pt->Release();
      
      
      if(CLIPEXTRACT==1)
	{
	  if((x<CLIPLLX) || (y<CLIPLLY) || 
	     (x>CLIPURX) || (y>CLIPURY))
	    {
	      usethis = 0;  
	      break;
	    }
	}			 
      
      if(TestGeodetic(x,y,newfeatureid,layername)==0)
	{
	  usethis = 0;
	  break;
	}
      
    }
		      
  if(usethis==1) 
    {
      
      if(thispart<=1)
	{
      NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,seeitgeom);

	  WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
	  WriteGFID(1,SaveHgtC,SaveHgtI,SaveHgtD,hgt_found,LONG_ATTR_HGT);
      WriteGFID(1,SaveLenC,SaveLenI,SaveLenD,len_found,LONG_ATTR_LEN);
      WriteGFID(1,SaveNamC,SaveNamI,SaveNamD,nam_found,LONG_ATTR_NAM);
	  WriteGFID(1,SaveZv2C,SaveZv2I,SaveZv2D,zv2_found,LONG_ATTR_ZV2);
	  
	  WriteGFID(gifd_found,thisgifd,-999,-999,C_TYPE,LONG_ATTR_ID);
	}
      
      sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
      sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
      sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
      
      
      longindex  = SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index ,
				    SH_SAC_LONG, newfeatureid);
      
      shortindex = SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,
				    SH_SAC_SHORT, newfeatureid);
      
      
      sprintf(SH_SAC_LONG [0].lod_stuff,"%s",tempstr);
      sprintf(SH_SAC_SHORT[0].lod_stuff,"%s",tempstr);
      
      longindex  = longindex  + addtolong;
      shortindex = shortindex + addtoshort;
      
      num3area = num3area + 1;
      
      ishole = 0;
      
      fwrite(&newfeatureid,   SzI,1,arealbin);
      fwrite(&ishole,         SzI,1,arealbin);
      fwrite(&longindex,      SzI,1,arealbin);
      fwrite(&shortindex,     SzI,1,arealbin);
      fwrite(&global_gifd_id, SzI,1,arealbin);
      fwrite(&numverts,       SzI,1,arealbin);
      
      
      if(thispart>=1)
	{
	  fwrite(&newfeatureid,   SzI,1,multipartareas);
	  fwrite(&longindex,      SzI,1,multipartareas);
	  fwrite(&shortindex,     SzI,1,multipartareas);
	  fwrite(&global_gifd_id, SzI,1,multipartareas);
	  fwrite(&numverts,       SzI,1,multipartareas);
	}
      
      if(thispart<=1)
	{
	  Record_LL_Mapping(global_gifd_id,longindex,newfeatureid);
	}
      
      for(nCount=1;nCount<=l_numverts;nCount++)
	{
	  vName.vt = VT_UI4;
	  vName.lVal = (long)nCount;
	  
	  hr = pts->Item(vName,&pt);
	  if(!SUCCEEDED(hr))
	    {
	      OSErrorHandle(type,31,"");
	      return 0;
	    } 
	  
	  pt->get_X(&x);
	  pt->get_Y(&y);
	  pt->get_Z(&z);
	  pt->Release();
	  
	  
	  fwrite(&x,SzD,1,arealbin);
	  fwrite(&y,SzD,1,arealbin);
	  fwrite(&z,SzD,1,arealbin);
	  
	  if(thispart>=1)
	    {
	      fwrite(&x,SzD,1,multipartareas);
	      fwrite(&y,SzD,1,multipartareas);
	      fwrite(&z,SzD,1,multipartareas);
	    }
	  
	  if(thispart<=1)
	    {
	      NumArealNodes = NumArealNodes + 1;
	      TestNumArealNodes = TestNumArealNodes + 1;
	    }
	  
	  sh_testxyglobal(x,y,z);
	}
      pts->Release();
      
      if(thispart<=1)
	{
	  SH_Areals_Done = SH_Areals_Done + 1;
	}
      
    }

  
  return 1;
  
}


extern "C" int ExtractAreaWithHoles(_DGMBoundaryGeometry *holy_geom,int addtolong, int addtoshort, int thispart, int type, int newfeatureid)
{
  HRESULT hr;
  VARIANT vName;
  _DGMPoint *pt;
  _DGMPoints *pts;
  IDispatch *exterior, *hole;
  _DGMPolygonGeometry *external_area_geom,*internal_area_geom;
  _DGMGeometryCollection *area_holes;  
  
  double *savex, *savey;
  double x,y,z;
  long l_numverts,numholes;
  int i,numverts,ishole,nCount,usethis,longindex,shortindex,wroteone=0,numsaveverts=0;
  
  

  if(holy_geom==NULL)
  {
    printf("Error: Null (hl) feature id %d\n",newfeatureid);
    AddNullShape(newfeatureid);
	return 1;
  }

  hr = holy_geom->get_Exterior(&exterior);
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,32,"");
      return 0;
    } 



  hr = exterior->QueryInterface(IID__DGMPolygonGeometry, (void**)&external_area_geom);
  if(SUCCEEDED(hr))
    {
      usethis = 1;
    }
  else
    {
      printf("for feature %d found exterior not of poly type.  ignoring\n",
	     newfeatureid);
      usethis = 0;
    }
  
  if(usethis==1) 
    {
      wroteone = 0;
      
      
      hr = external_area_geom->get_Points(&pts);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,33,"");
	  return 0;
	} 
      
      
      pts->get_Count(&l_numverts);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,34,"");
	  return 0;
	} 
      
      numverts = (int)l_numverts;

      for(nCount=1;nCount<=l_numverts;nCount++)
	{
	  vName.vt = VT_UI4;
	  vName.lVal = (long)nCount;
	  
	  hr = pts->Item(vName,&pt);
	  if(!SUCCEEDED(hr))
	    {
	      OSErrorHandle(type,35,"");
	      return 0;
	    } 
	  
	  pt->get_X(&x);
	  pt->get_Y(&y);
	  pt->get_Z(&z);
	  pt->Release();
	  
	  if(CLIPEXTRACT==1)
	    {
	      if((x<CLIPLLX) || (y<CLIPLLY) || 
		 (x>CLIPURX) || (y>CLIPURY))
		{
		  usethis = 0;
		  break;
		}
	    }
	  if(TestGeodetic(x,y,newfeatureid,layername)==0)
	    {
	      usethis = 0;
	      break;
	    }				  
	}



      if(usethis==1) 
	{ 
	  
	  if(thispart<=1)
	    { 
          NFIWrapper(saveNFI,saveNFN,saveNAM,class_code+1,seeitgeom);

	      WriteGFID(1,SaveAreaC,SaveAreaI,SaveAreaD,area_found,LONG_ATTR_AREA);
	      WriteGFID(1,SaveHgtC,SaveHgtI,SaveHgtD,hgt_found,LONG_ATTR_HGT);
          WriteGFID(1,SaveLenC,SaveLenI,SaveLenD,len_found,LONG_ATTR_LEN);
          WriteGFID(1,SaveNamC,SaveNamI,SaveNamD,nam_found,LONG_ATTR_NAM);
	      WriteGFID(1,SaveZv2C,SaveZv2I,SaveZv2D,zv2_found,LONG_ATTR_ZV2);

	      WriteGFID(gifd_found,thisgifd,-999,-999,C_TYPE,LONG_ATTR_ID);
	    }
	  
	  sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
	  sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
	  sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
	  
	  longindex  = 
	    SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index ,SH_SAC_LONG, newfeatureid );
	  shortindex = 
	    SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, newfeatureid);
	  
	  
	  sprintf(SH_SAC_LONG [0].lod_stuff,"%s",tempstr);
	  sprintf(SH_SAC_SHORT[0].lod_stuff,"%s",tempstr);
	  
	  longindex  = longindex  + addtolong;
	  shortindex = shortindex + addtoshort;
	  
	  
	  
	  ishole = 0;
	  wroteone = 1;
	  fwrite(&newfeatureid,  SzI,1,arealbin);
	  fwrite(&ishole,        SzI,1,arealbin);
	  fwrite(&longindex,     SzI,1,arealbin);
	  fwrite(&shortindex,    SzI,1,arealbin);
	  fwrite(&global_gifd_id,SzI,1,arealbin);
	  fwrite(&numverts,      SzI,1,arealbin);
	  num3area = num3area + 1;
	  
	  
	  if(thispart>=1)
	    {
	      fwrite(&newfeatureid,   SzI,1,multipartareas);
	      fwrite(&longindex,      SzI,1,multipartareas);
	      fwrite(&shortindex,     SzI,1,multipartareas);
	      fwrite(&global_gifd_id, SzI,1,multipartareas);
	      fwrite(&numverts,       SzI,1,multipartareas);
	    }


	  if(thispart<=1)
	    { 
	      Record_LL_Mapping(global_gifd_id,longindex,newfeatureid);
	    }
	  
	  numsaveverts = (int)numverts;
	  savex = (double *)malloc(sizeof(double)*numsaveverts);
	  if(savex==NULL){printf("failed to allocate %d savex\n",numsaveverts); exit(1);}
	  savey = (double *)malloc(sizeof(double)*numsaveverts);
	  if(savey==NULL){printf("failed to allocate %d savey\n",numsaveverts); exit(1);}
	  
	  
	  for(nCount=1;nCount<=l_numverts;nCount++)
	    {
	      vName.vt = VT_UI4;
	      vName.lVal = (long)nCount;
	      
	      hr = pts->Item(vName,&pt);
	      if(!SUCCEEDED(hr))
		{
		  OSErrorHandle(type,36,"");
		  return 0;
		} 
	      
	      pt->get_X(&x);
	      pt->get_Y(&y);
	      pt->get_Z(&z);
	      pt->Release();
	      
	      savex[nCount-1] = x;
	      savey[nCount-1] = y;
	      
	      fwrite(&x,SzD,1,arealbin);
	      fwrite(&y,SzD,1,arealbin);
	      fwrite(&z,SzD,1,arealbin);
	      
	      if(thispart>=1)
		{
		  
		  fwrite(&x,SzD,1,multipartareas);
		  fwrite(&y,SzD,1,multipartareas);
		  fwrite(&z,SzD,1,multipartareas);
		}
	      
	      
	      if(thispart<=1)
		{
		  NumArealNodes = NumArealNodes + 1;
		  TestNumArealNodes = TestNumArealNodes + 1;
		}
	      
	      sh_testxyglobal(x,y,z);
	    }
	  pts->Release();
	  
	  if(thispart<=1)
	    { 
	      SH_Areals_Done = SH_Areals_Done + 1;
	    }
	  
	}

    }
  
  
  
  
  
  
  
  if(wroteone==1)
    {
      
      hr = holy_geom->get_Holes(&area_holes);
      if(SUCCEEDED(hr))
	{
	  area_holes->get_Count(&numholes);

	  for(i=1;i<=numholes;i++)
	    {
	      vName.vt = VT_UI4;
	      vName.lVal = (long)i;
	      
	      hr = area_holes->Item(vName,&hole);
	      if(!SUCCEEDED(hr))
		{
		  OSErrorHandle(type,37,"");
		  return 0;
		} 
	      
	      
	      hr = hole->QueryInterface(IID__DGMPolygonGeometry, (void**)&internal_area_geom);
	      if(SUCCEEDED(hr)) 
		{
		  usethis = 1;
		  
		  hr = internal_area_geom->get_Points(&pts);
		  if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,38,"");
		      return 0;
		    } 
		  
		  hr = pts->get_Count(&l_numverts);
		  if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,58,"");
		      return 0;
		    } 

		  

		  numverts = (int)l_numverts;
		  
		  
		  usethis = 1;
		  ishole  = 1;
		  
		  for(nCount=1;nCount<=l_numverts;nCount++)
		    {
		      vName.vt = VT_UI4;
		      vName.lVal = (long)nCount;
		      
		      hr = pts->Item(vName,&pt);
		      if(!SUCCEEDED(hr))
			{
			  OSErrorHandle(type,39,"");
			  return 0;
			} 
		      
		      
		      pt->get_X(&x);
		      pt->get_Y(&y);
		      pt->get_Z(&z);
		      pt->Release();
		      
		      
		      if(CLIPEXTRACT==1)
			{
			  if((x<CLIPLLX) || (y<CLIPLLY) || 
			     (x>CLIPURX) || (y>CLIPURY))
			    {
			      usethis = 0;
			      break;
			    }
			}
		      
		      if(TestGeodetic(x,y,newfeatureid,layername)==0)
			{
			  usethis = 0;
			  break;
			}
		      
		      
			  if(
				  (nCount==1)     ||
				  (l_numverts<20) ||  
				  (nCount%100==0)
				)
			  {
		        if(SH_PointInAreal(x,y,savex,savey,0,numsaveverts-1)==0)
				{
			      printf("Invalid hole found\n");
			      AddBadHole(featureid,-999,layername,2);
			      ishole = 0;
				} 
			  } 
		  }

		  if(usethis==1) 
		    {				  
		      
		      
		      
		      sprintf(tempstr,"%s",SH_SAC_LONG[0].lod_stuff);
		      sprintf(SH_SAC_LONG [0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
		      sprintf(SH_SAC_SHORT[0].lod_stuff,"%d %d %s",class_code,seeitgeom,tempstr);
		      
		      
		      longindex  = 
			SH_FindSCC_LONG (seeitgeom,  class_code,sh_sac_long_index ,SH_SAC_LONG, newfeatureid);
		      shortindex = 
			SH_FindSCC_SHORT(seeitgeom,  class_code,sh_sac_short_index,SH_SAC_SHORT, newfeatureid);
		      
		      sprintf(SH_SAC_LONG [0].lod_stuff,"%s",tempstr);
		      sprintf(SH_SAC_SHORT[0].lod_stuff,"%s",tempstr);
		      
		      
		      longindex  = longindex  + addtolong;
		      shortindex = shortindex + addtoshort;
		      
		      
		      num3area = num3area + 1;
		      
		      fwrite(&newfeatureid,  SzI,1,arealbin);
		      fwrite(&ishole,        SzI,1,arealbin);
		      fwrite(&longindex,     SzI,1,arealbin);
		      fwrite(&shortindex,    SzI,1,arealbin);
		      fwrite(&global_gifd_id,SzI,1,arealbin);
		      fwrite(&numverts,      SzI,1,arealbin);
		      
		      		      
		      for(nCount=1;nCount<=l_numverts;nCount++)
			{
			  vName.vt = VT_UI4;
			  vName.lVal = (long)nCount;
			  
			  hr = pts->Item(vName,&pt);
			  if(!SUCCEEDED(hr))
			    {
			      OSErrorHandle(type,40,"");
			      return 0;
			    } 
			  
			  
			  pt->get_X(&x);
			  pt->get_Y(&y);
			  pt->get_Z(&z);
			  pt->Release();
			  
			  
			  fwrite(&x,SzD,1,arealbin);
			  fwrite(&y,SzD,1,arealbin);
			  fwrite(&z,SzD,1,arealbin);
			  
			  NumArealNodes = NumArealNodes + 1;
			  TestNumArealNodes = TestNumArealNodes + 1;
			  
			  sh_testxyglobal(x,y,z);
			}
		      pts->Release();
		      if(ishole==1)
			{
			  NumHoles = NumHoles + 1;
			}
		      else 
			{
              SH_Areals_Done = SH_Areals_Done + 1;
			}
		    } 
		  
		  internal_area_geom->Release();		
		}
	      else
		{
		  printf("found hole for Objid %d not of type polygon\n",newfeatureid);
		}
	      hole->Release();
	    }
	  area_holes->Release();
	}	
      else
	{
	  printf("failed to get holes for area Objid %d\n",newfeatureid);
	}
    }

  
  if(numsaveverts==0)
    {
      
    }
  else
    {
      free(savex);
      free(savey);
      savex = NULL;
      savey = NULL;
      numsaveverts = 0;
    }

  return 1;
}



extern "C" int DoOSStuff(FILE *inspecfile,int BuildInputSpecFile,char *OSname,char *DDname,
			 char *inspecfilename, int type,
			 int addtolong,int addtoshort,
			 int *longitemsfound,int *shortitemsfound,
			 int *badostables, int *goodostables)
{
  int i,j,k,OSMULT,numdone,numtables=0,code_found,num_real_entries=0,curr_table,
	  SH_LISTLEN=1000,GEOMTYPE,newenum, Ivalue, OldIvalue,zz,last_table_num,len,
	  oldvalset,tempI,usethis,goodos=0,bados=0,AddToLongOrig,AddToShortOrig,
	  geomfound,mgcpfcodefound=0;
  double Dvalue,OldDvalue;
  char Cvalue[5000],OldCvalue[5000],enumvalue[1000],scrubenumvalue[1000],testclass[1000];
  unsigned char SEDRIStype;
  short int num_fields = 0,ii,geomtype;
  char **TableNames;
  char tempchar[1000],shortname[1000],ReportName[1000],foundclass[1000];
  long LBound, UBound, nCount, TableMask,numtotalos;  
  _variant_t vTables;
  VARIANT vName, vIndx;
  VARIANT_BOOL leof;
  GField *Geom;
  DGMConnection* lConn;
  CString strTable, tempC = OSname, tempC2 = DDname, Cstrtablename;
  OriginatingPipe** lOP;
  SAFEARRAY * pSA;
  _MetadataServicePtr ptrMetaData;
  _MetadataServicePtr OptrMetaData[5];
  HRESULT hr = NOERROR;
  extern char *GetNFI();
  extern char *GetNFN();

  if(type==1)
  {
    
    return 0;
  }

  sh_sac_long_index = 0;
  sh_sac_short_index = 0;

  numtotalos = 0;  


  

  if(type==2)
  {
    OSMULT = 250000;
  }
  else
  {
    OSMULT = 25000;
  }

  

  AddToLongOrig  = addtolong;
  AddToShortOrig = addtoshort;

 

  
  
  if(CO_INNITTED==0)
    {
      hr = CoInitialize(NULL);
      if(!SUCCEEDED(hr))
	  { 
	    printf("CoInit failed\n");
	    if(hr==RPC_E_CHANGED_MODE)
		{
		 printf("...case 1\n");
		}
	    else
		{ 
		  printf("...case2: %d %x\n",hr,hr);
		} 
	  } 
      CO_INNITTED = 1;
    }


  hr = CoCreateInstance(CLSID_Connection, NULL, 
			CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, 
			IID_DGMConnection, (void**)&lConn);
  if(hr!=S_OK)
    {
      printf("failed to create instance\n");
    }


  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,1,"");
      return 0;
    } 
  
  GetShortName(shortname,OSname);

  if(type==1)
  {
    
  }
  else if(type==2)
  {
	BSTR tempBstr;

    printf("filename %s BuildInputSpec %d\n",OSname,BuildInputSpecFile);

    tempBstr  = tempC.AllocSysString();

    hr = lConn->put_Type(L"Access.GDatabase");
    if(!SUCCEEDED(hr))
    {
      printf("Error with PT Access.GDatabase\n");
    }

    hr = lConn->put_Location(tempBstr);
    if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,3,OSname);
      return 0;
    } 

    SysFreeString(tempBstr);
  }


  hr = lConn->put_Mode (gmcModeReadOnly);
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,5,"");
    } 


  printf("Connecting to %s...this may take some time\n",OSname);


  if(BuildInputSpecFile==0) 
  {
    FORCE_REDRAW = 1;

    update_shape_import(OSname,"Connecting to data set...please be patient",0,-1,-1,
		      SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
		      0,0,0,0,SH_bad_objects, SH_BadCoords, SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);

    FORCE_REDRAW = 0;
  }


  hr = lConn->Connect();
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,6,"");
      return 0;
    }  
  
  
  if(BuildInputSpecFile==1)
    {
      
      
      hr = ptrMetaData.CreateInstance(CLSID_MetadataService);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,7,"");
	  lConn->Disconnect();
	  lConn->Release();
	  return 0;
	} 


      
      hr = ptrMetaData->putref_Connection((IDispatch**)&lConn);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,8,"");
	  hr = lConn->Disconnect();
	  lConn->Release();
	  return 0;
	} 


      
      TableMask = gmmtPoint | gmmtLinear | gmmtAreal;  
      
      hr = ptrMetaData->GetTables(&TableMask,&vTables);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,9,"");
	  hr = lConn->Disconnect();
	  lConn->Release();
	  return 0;
	} 



      if(vTables.vt != (VT_ARRAY | VT_BSTR))
	{
	  printf("bad table type %d\n",vTables.vt);
	  printf("expected table type 8200\n");
	  lConn->Release();
	  return 0;
	}
      
      pSA = vTables.parray;
      LBound = pSA->rgsabound[0].lLbound;
      UBound = LBound + pSA->rgsabound[0].cElements;
      numtables = UBound - LBound;
      TableNames = (char **)malloc(sizeof(char *)*(numtables+5));
      
      


      for(nCount = LBound; nCount < UBound; nCount++)
	{
	  BSTR pBstr;
	  SafeArrayGetElement(pSA, &nCount, (void *)&pBstr);
	  
	  strTable = pBstr;
	  TableNames[nCount-LBound] = (char *)malloc(strlen(strTable)+5);
	  sprintf(TableNames[nCount-LBound],"%s",strTable.operator LPCTSTR());
	  

	  

	  if(strlen(TableNames[nCount-LBound])>0)
	  { 
	    num_real_entries = num_real_entries + 1;
	  }


	  

	  SysFreeString(pBstr);
	}
      
      
    if(num_real_entries>0)
	{
    

      for(i=0;i<numtables;i++)
	  {
	   if((i==0)&&(type==1))
	    {
          
	    }
	  else if((i==0)&&(type==2))
	    {
	      fprintf(inspecfile,"GEOMEDIAACC NUM_TABLES %d\n",num_real_entries);
	      fprintf(inspecfile,"GEOMEDIAACC %s\n",OSname);
	    }

	  
	  code_found = -100;
	  if(strlen(TableNames[i])>0)
	    {
	      code_found = GetClassCodeAsSubstring(TableNames[i]);
	      if(code_found==-100)
		{
		  code_found = GetClassCodeFromName(TableNames[i]);
		}
	      if(code_found==-100)
		{
		  if(type==1)
		  {
		    
		  }
		  else if(type==2)
		  {
		    fprintf(inspecfile,"GEOMEDIAACC_TABLE ***** %s\n",TableNames[i]);
		  }

		  bados = bados + 1;
		}
	      else
		{
		  if(type==1)
		  {
		    
		  }
		  else if(type==2)
		  {
		    fprintf(inspecfile,"GEOMEDIAACC_TABLE %s %s\n",GetECCCode(code_found+1),TableNames[i]);
		  }

		  goodos = goodos + 1;
		}
	      free(TableNames[i]);
	    }
	  } 
	}
      free(TableNames);
      hr = lConn->Disconnect();     
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,10,"");
	  hr = lConn->Disconnect();
	  lConn->Release();
	  return 0;
	} 

      lConn->Release();
      *badostables = bados;
      *goodostables = goodos;

      return 1;
    }
  
  
  

  
  
  
  
  for(i=0;i<3;i++)
    {
      if(i==0)
	{
	  TableMask = gmmtPoint;  
	  seeitgeom = 5;
	}
      if(i==1)
	{
	  TableMask = gmmtLinear;  
	  seeitgeom = 4;
	}
      if(i==2)
	{
	  TableMask = gmmtAreal;  
	  seeitgeom = 1;
	}
      
      hr = OptrMetaData[i].CreateInstance(CLSID_MetadataService);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,11,"");
	  return 0;
	} 



      hr = OptrMetaData[i]->putref_Connection((IDispatch**)&lConn);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,12,"");
	  return 0;
	}




      hr = OptrMetaData[i]->GetTables(&TableMask,&vTables);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,13,"");
	  return 0;
	} 


      
      if(vTables.vt != (VT_ARRAY | VT_BSTR))
	{
	  printf("bad table type %d\n",vTables.vt);
	  printf("expected table type 8200\n");
 	  return 0;
	}
      
      pSA = vTables.parray;
      LBound = pSA->rgsabound[0].lLbound;
      UBound = LBound + pSA->rgsabound[0].cElements;
      numtables = UBound - LBound;
      

      
      
      for(nCount = LBound; nCount < UBound; nCount++)
	{
	  BSTR pBstr; 

	  SafeArrayGetElement(pSA, &nCount, (void *)&pBstr);
	  
	  strTable = pBstr;
	  
	  
	  
	  if(strlen(strTable)>0)
	    {
	      for(j=0;j<numtables_to_extract;j++)
		  { 
		    if(!strcmp(TablesToExtract[j],strTable))
			{

			  



		      if(i==0)
			  {
			    printf("found table %s (%d) --- point geom\n",strTable.operator LPCTSTR(),j);
			  }
		      if(i==1)
			  { 
			    printf("found table %s (%d) -- line geom\n",strTable.operator LPCTSTR(),j);
			  } 
		      if(i==2)
			  { 
			    printf("found table %s (%d) -- area geom\n",strTable.operator LPCTSTR(),j);
			  } 


		      TableGeoms[j] = seeitgeom;


			  


			  


           

              

 

              






			}  
		  } 
	  } 

        SysFreeString(pBstr);

	  } 
  } 




  
  

  
  
  
  
  
  
  
  
  
  
  lOP = (OriginatingPipe**) malloc(sizeof(OriginatingPipe *)*numtables_to_extract);
  if(lOP==NULL)
    {
      printf("failed to malloc for %d lOP\n",num_fields);	
      printf("aborting %s load\n",GetGMType(type));
      return 0;
    }




  for(i=0;i<SH_LISTLEN;i++)
    {
      SH_SAC_LONG[i].lod_stuff  = NULL;
      SH_SAC_LONG[i].nam_stuff  = NULL;
      SH_SAC_SHORT[i].lod_stuff = NULL;
      SH_SAC_SHORT[i].nam_stuff = NULL;
      SH_SAC_LONG[i].Cvalue     = NULL;
    }
  
  featureid = 0;
  
  for(curr_table=0;curr_table<numtables_to_extract;curr_table++)
    {
      DGMGeometryStorageService *gss;
      GRecordset *lRS;
      GFields *lFlds;
      GField **allfields;
      BSTR tableBstr;
	  int *optional_attribute;
      int *allfieldcodes;
	  short *allfieldtypes;
      char **allfieldnames;

	  sprintf(ReportName,"%s (Table %s)",shortname,TablesToExtract[curr_table]);

      numtotalos = numtotalos + featureid;
      if(numtotalos>OSMULT)
	  {
        
		  printf("starting over due to %d total\n",numtotalos);


	      hr = lConn->Disconnect();
	      if(!SUCCEEDED(hr))
		  {
           printf("FAILED to disconnect!\n");
		   OSErrorHandle(type,53,"");
		   return 0;
		  }	      
	   

		  
		  printf("re-connecting\n");
          hr = lConn->Connect();
          if(!SUCCEEDED(hr))
		  {
            OSErrorHandle(type,6,"");
            return 0;
		  } 

	      numtotalos = 0;
	  }



	  


      update_shape_import(inspecfilename,layername, curr_table ,-1,-1,
			  SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
			  0,0,0,0,SH_bad_objects, SH_BadCoords, SH_Null_Shapes, SH_Zero_Shapes,SH_Bad_DBF);


      hr = lConn->CreateOriginatingPipe(&lOP[curr_table]);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,14,"");
	  return 0;
	} 


      
      featureid = 0;


      printf("Loading table %s\n",TablesToExtract[curr_table]);


      Cstrtablename = TablesToExtract[curr_table];
      tableBstr  = Cstrtablename.AllocSysString();

      
      hr = lOP[curr_table]->put_Table(tableBstr);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,15,TablesToExtract[curr_table]);
	  return 0;
	} 
      
      
     hr = lOP[curr_table]->get_OutputRecordset(&lRS);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,16,TablesToExtract[curr_table]);
	  return 0;
	} 





      lOP[curr_table]->Release();
      

      
      hr = lRS->MoveFirst();
      if(!SUCCEEDED(hr))
	  { 
	    

		fprintf(emptyshapes,"Empty GeoMedia Access Table %s;Table %s\n",OSname,TablesToExtract[curr_table]);

        hr = lRS->Close();
        if(!SUCCEEDED(hr))
		{  
	      OSErrorHandle(type,54,"");
		}  

        lRS->Release();

	    continue;
	  }   



      hr = lRS->get_GFields(&lFlds);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,18,TablesToExtract[curr_table]);
	  return 0;
	} 


      hr = lFlds->get_Count(&num_fields);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,19,TablesToExtract[curr_table]);
	  return 0;
	}


      
      
      allfields = (GField **) malloc(sizeof(GField *)*num_fields);
      if(allfields==NULL)
	  {
	    printf("failed to malloc for %d fields\n",num_fields);	
        printf("aborting %s load\n",GetGMType(type));
 	    return 0;
	  } 
      
      allfieldnames = (char **) malloc(sizeof(char *)*num_fields);
      if(allfieldnames==NULL)
	  { 
	    printf("failed (0) to malloc for %d field names\n",num_fields);	
        printf("aborting %s load\n",GetGMType(type));
  	    return 0;
	  } 
      
      allfieldcodes = (int *) malloc(sizeof(int)*num_fields);
      if(allfieldcodes==NULL)
	  { 
	    printf("failed (1) to malloc for %d field codes\n",num_fields);	
        printf("aborting %s load\n",GetGMType(type));
	    return 0;
	  } 

      allfieldtypes = (short *) malloc(sizeof(short)*num_fields);
      if(allfieldtypes==NULL)
	  { 
	    printf("failed (2) to malloc for %d field codes\n",num_fields);	
        printf("aborting %s load\n",GetGMType(type));
	    return 0;
	  } 

  	  if(ATTRTYPE_ANY_MGCP())
	  {
        optional_attribute = (int *)  malloc(sizeof(int)   *(num_fields+10));
	  }



	  geomfound = -1;

      for(ii=0;ii<num_fields;ii++)
	{    
	  BSTR wstr;

	  _FieldPropertyPtr ptrFieldProps;			   
	  IDispatch *pFieldProps;
	  VARIANT_BOOL disp;
	  long nSuccess;
	  int stillok;


	  allfieldcodes[ii] = -100;  

	  lFlds->get_Item(_variant_t(ii), &allfields[ii]);


	  allfields[ii]->get_Name(&wstr);
	  sprintf(tempchar,"%S",(char *)wstr);
	  
	  allfieldnames[ii] = (char *)malloc(strlen(tempchar)+5);
	  sprintf(allfieldnames[ii],"%s",tempchar);
	  
	  
	  for(tempI=0;tempI<(int)strlen(allfieldnames[ii]);tempI++)
	    {
	      allfieldnames[ii][tempI] = timtoupper(allfieldnames[ii][tempI]);
	    }

	  

	  hr = allfields[ii]->get_Type(&allfieldtypes[ii]);


	  if( (ATTRTYPE_ANY_MGCP())  &&  ((allfieldtypes[ii]==gdbText)||(allfieldtypes[ii]==gdbMemo)))
	  {
	    long pSize;
	    hr = allfields[ii]->get_Size(&pSize);

		

        if(SUCCEEDED(hr))
		{
		  if((allfieldtypes[ii]==gdbText)||(allfieldtypes[ii]==gdbMemo)) 
		  {
            TestWidth(1,allfieldnames[ii],pSize);
		  }
		  else if((allfieldtypes[ii]==gdbSingle)||(allfieldtypes[ii]==gdbDouble))  
		  {
            TestWidth(3,allfieldnames[ii],pSize);
		  }
		  else if((allfieldtypes[ii]==gdbInteger)||(allfieldtypes[ii]==gdbLong)||(allfieldtypes[ii]==gdbBoolean)||(allfieldtypes[ii]==gdbByte))  
		  {
            TestWidth(2,allfieldnames[ii],pSize);
		  }
		  else if(allfieldtypes[ii]!=gdbSpatial)  
		  {
		    
		  }
		}
        else
		{
          printf("FAILED to get size for field %s, type %d\n",allfieldnames[ii],allfieldtypes[ii]);
		}
	  }


	  

	          stillok = 1;

              hr = ptrFieldProps.CreateInstance(CLSID_FieldProperty);

              if(SUCCEEDED(hr))
			  { 
		         
			  } 
	          else
			  { 
		         printf("FAILED to create field property ob\n");
		         stillok = 0;
			  } 


			  if(stillok==1)
			  {
                hr = ptrFieldProps->put_Name(&wstr);

                if(SUCCEEDED(hr))
				{ 
		          
				}  
	            else
				{  
		          printf("FAILED to set name on field property ob\n");
		          stillok=0;
				} 


             if((TableGeoms[curr_table]==5)&&(stillok==1))
			 {
               hr = OptrMetaData[0]->put_TableName(&tableBstr);

               if(SUCCEEDED(hr))
			   {  
		         
			   } 
	           else
			   {
		         printf("FAILED to set name on meta ob 0\n");
		         stillok = 0;
			   }   

               pFieldProps = (IDispatch *)ptrFieldProps.GetInterfacePtr();

               hr = OptrMetaData[0]->GetFieldMetadata(&pFieldProps,&nSuccess);
			 }

			 else if((TableGeoms[curr_table]==4)&&(stillok==1))
			 {
               hr = OptrMetaData[1]->put_TableName(&tableBstr);

               if(SUCCEEDED(hr))
			   {  
		         
			   } 
	           else
			   {     
		         printf("FAILED to set name on meta ob 1\n");
		         stillok = 0;
			   }   

               pFieldProps = (IDispatch *)ptrFieldProps.GetInterfacePtr();

               hr = OptrMetaData[1]->GetFieldMetadata(&pFieldProps,&nSuccess);
			 }

			 else if((TableGeoms[curr_table]==1)&&(stillok==1))
			 {
               hr = OptrMetaData[2]->put_TableName(&tableBstr);

               if(SUCCEEDED(hr))
			   {  
		         
			   } 
	           else
			   {     
		         printf("FAILED to set name on meta ob 2\n");
		         stillok = 0;
			   }
               pFieldProps = (IDispatch *)ptrFieldProps.GetInterfacePtr();
               hr = OptrMetaData[2]->GetFieldMetadata(&pFieldProps,&nSuccess);
			 }
 

			 if(stillok==1)
			 {
               

               if(SUCCEEDED(hr))
			   {  
		         
			   }  
	           else
			   { 
		         printf("FAILED to get FM\n");
			   }

			   hr = ptrFieldProps->get_Displayable(&disp);

               if(SUCCEEDED(hr))
			   {
			     if(disp)
				 { 
                   
				 } 
			     else if(nSuccess==1)
				 {
				   
				   allfieldcodes[ii] = -9999;
				 } 
			   }
			 }
			}

		


	  


      SysFreeString(wstr);


	  if(allfieldtypes[ii]==gdbSpatial)
	  {
		  
		  if(geomfound==-1)
		  {
		    geomfound = ii;
		    Geom = allfields[ii];
		  }
		  else
		  {
			printf("error:  multiple spatial fields (%s and %s) found for table %s\n",
				allfieldnames[geomfound],allfieldnames[ii],TablesToExtract[curr_table]);

			if(!stricmp(allfieldnames[ii],"Geometry"))
			{
			  
			  geomfound = ii;
			  Geom = allfields[ii];
			}
			else if(!stricmp(allfieldnames[ii],"FLDBGEOM"))
			{
			  
			  if(stricmp(allfieldnames[geomfound],"Geometry"))
			  {
                geomfound = ii;
                Geom = allfields[ii];
			  }
			}
		  }
	  }
	}


	  if(geomfound==-1)
	  {
		  printf("could not find spatial field in table %s\n",TablesToExtract[curr_table]);
		  printf("ignoring\n");
		  continue;
	  }
	  else
	  {
		  
	  }





      for(ii=0;ii<num_fields;ii++)
	{
	  if(allfieldcodes[ii]==-9999)
	  {
		  
		  allfieldcodes[ii] = -100;
		  continue;
	  }

	  if(IsGFID(allfieldnames[ii]))
	    {
	      allfieldcodes[ii] = 100000;
	    }
	  else if(IsArea(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100001;
		}
	  else if(IsHgt(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100002;
		}
	  else if(IsZv2(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100003;
		}
	  else if(IsObjid(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100004;
		}
      else if(PossibleFCODE(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100005;
		  if(!strcmp(allfieldnames[ii],"FCODE"))
		  {
			
		    mgcpfcodefound = 1;  
		  }
		}
	  else if(IsLen(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100006;
		}
	  else if(IsNam(allfieldnames[ii]))
		{
		  allfieldcodes[ii] = 100007;
		}
	  else
	    {
		  if(LongCode(allfieldnames[ii])==0)
		  {
	        allfieldcodes[ii] = ConvertAttrCode(allfieldnames[ii],-100);   


		    if(ATTRTYPE_ANY_MGCP())
			{ 
	          if((allfieldcodes[ii]>=0)&&(strcmp(FCODEsOfTables[curr_table],"*****")))
			  { 
	            optional_attribute[ii] = GetOptional(FCODEsOfTables[curr_table],allfieldnames[ii],TableGeoms[curr_table]);
			  } 
	          else
			  { 
	            optional_attribute[ii] = -100;
			  } 
			}
		  }
		  else
		  {
            allfieldcodes[ii] = -100;
		  }


		  if( (allfieldcodes[ii]==-100) && (LongCode(allfieldnames[ii])==0) )
		  {
			if(ii!=geomfound) 
			{
		      AddUnkAttr(allfieldnames[ii]);
			}
		  }
	    }

		

	 } 

      
	 if(mgcpfcodefound==0)
	 {
       AddNoFCODE(ReportName);
	 } 


      

      VariantClear(&vName);


      lFlds->Release();


      CoCreateInstance(CLSID_GeometryStorageService, NULL,
		       CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, 
		       IID_DGMGeometryStorageService, (void**)&gss);
      
      
      
      numdone = 0;
    
	  
      hr = lRS->get_EOF(&leof);
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,21,TablesToExtract[curr_table]);
	  return 0;
	} 


     last_table_num = -1;


      while(!leof)
	{

      numdone = numdone + 1;

	  
	  sh_sac_short_index = 0;
	  sh_sac_long_index  = 0;
	  
	  
	  

	  sprintf(layername,"%s (Table %s)",shortname,TablesToExtract[curr_table]);
	  
	  SH_SAC_LONG[sh_sac_long_index].lod_stuff = 
	    (char *)malloc(strlen(shortname)+strlen(TablesToExtract[curr_table])+50);
	  if(SH_SAC_LONG[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(1);}
	  
	  SH_SAC_SHORT[sh_sac_short_index].lod_stuff = 
	    (char *)malloc(strlen(shortname)+strlen(TablesToExtract[curr_table])+50);
	  if(SH_SAC_SHORT[sh_sac_long_index].lod_stuff==NULL){printf("out of memory!\n"); exit(1);}
	  
	  sprintf(SH_SAC_LONG [sh_sac_long_index ].lod_stuff,"%s",layername);
	  sprintf(SH_SAC_SHORT[sh_sac_short_index].lod_stuff,"%s",layername);
	  
	  SH_SAC_LONG [sh_sac_long_index ].attr_code = -1;
	  SH_SAC_SHORT[sh_sac_short_index].attr_code = -1;
	  
	  sh_sac_short_index++;
	  sh_sac_long_index++;
	  gifd_found     = 0;
	  area_found     = NOTYPE;
	  hgt_found      = NOTYPE;
	  len_found      = NOTYPE;
	  nam_found      = NOTYPE;
	  zv2_found      = NOTYPE;

      saveNFI[0] = '\0';
      saveNFN[0] = '\0';
      saveNAM[0] = '\0';


	  mgcpfcodefound = 0;
      foundclass[0] = '\0';  


	  for(i=0;i<num_fields;i++) 
	    {


	      GEOMTYPE = -999;
	      
		




		  VariantInit(&vIndx);



	      if(allfieldcodes[i]>=0)
		{
		  
		  hr = allfields[i]->get_Value(&vIndx);
		  if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,22,allfieldnames[i]);
		      return 0;
		    } 




		  if(numdone==1)
		  {
		     int tempcode;

			 
			 if(allfieldcodes[i]!=100005) 
			 {
				if(allfieldcodes[i]>=100000) 
				{
				  tempcode = ConvertAttrCode(allfieldnames[i],-100);

				  if(tempcode>=0)
				  {
			        if(TestOSDataType(tempcode,OSname,TablesToExtract[curr_table],
				      allfieldnames[i],vIndx.vt,numdone,last_table_num)==1)
					{
 				     last_table_num = numdone;
					}
				  }
				  else
				  {
					 if(allfieldcodes[i]!=100004)
					  {
					    printf("OS Error: could not find code for %s (%d) (%d)\n",
						  allfieldnames[i],tempcode,allfieldcodes[i]);
					  }
				  }
				}
				else
				{
			      if(TestOSDataType(allfieldcodes[i],OSname,TablesToExtract[curr_table],
				    allfieldnames[i],vIndx.vt,numdone,last_table_num)==1)
				  {
 				   last_table_num = numdone;
				  }
				}			  
			 }
		  }




		  oldvalset = 0;

		  
		  switch(vIndx.vt)
		    {
		    case VT_NULL:

			  int sendtype;


			  sendtype = 0; 


              
				  if( (allfieldtypes[i]==gdbSingle) || (allfieldtypes[i]==gdbDouble) )
				  {
                    sendtype = 1; 
				  }
				  else if( (allfieldtypes[i]==gdbCurrency) || (allfieldtypes[i]==gdbText) || (allfieldtypes[i]==gdbDate) || (allfieldtypes[i]==gdbMemo))
				  {
                    sendtype = 2; 
				  }
			  

		      if(ATTRTYPE_ANY_MGCP())
			  {  
			    AddBlankAttr(ReportName,allfieldnames[i],featureid+1,sendtype);
			  }

		      GEOMTYPE = -999;  
		      break;
		    case VT_I2:
		    case VT_UI2:
		      OldIvalue = vIndx.iVal;
		      oldvalset = 1;
		    case VT_I4:
		    case VT_UI4:
		      if(oldvalset==0)
			{
			  OldIvalue = vIndx.lVal;
			}
		      
		      if(allfieldcodes[i]>=100000)
			{
			  if(allfieldcodes[i]!=100005)
			  {
			    GEOMTYPE = 0; 
			    SEDRIStype = TIM_INTEGER;
			  }
			  else
			  {
                GEOMTYPE = -999;
			  }
			}
		      else if(gifd_attr_enum_count[allfieldcodes[i]]==0)
			{
			  
			  GEOMTYPE = 0; 
			  SEDRIStype = TIM_INTEGER;
			}
		      else
			{

	          if(!strcmp(allfieldnames[i],"SEC_CLASS"))
			  {
				  sprintf(testclass,"%d",OldIvalue);
				  if(SetClassLevel(testclass,TablesToExtract[curr_table])>0)
				  {
					return 0;
				  }
			  }

			  enumvalue[0] = '\0';
			  sprintf(enumvalue,"%d",OldIvalue);
			  
			  newenum = GetNewEnum(allfieldcodes[i],
					       enumvalue,
					       1,
					       featureid + 1);
			  
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

				OldDvalue  = (double)vIndx.fltVal;


				if(allfieldcodes[i]>=100000) 
				{
                  GEOMTYPE   = 1;  
                  SEDRIStype = TIM_DOUBLE;
                  OldDvalue  = (double)vIndx.fltVal;
				}
				else
				{
				
			      if(gifd_attr_enum_count[allfieldcodes[i]]>0)
				  {
				    HandleDoublePickListVal(allfieldcodes[i],OldDvalue,&GEOMTYPE,&SEDRIStype,&newenum,featureid+1);


	                if(!strcmp(allfieldnames[i],"SEC_CLASS"))
					{ 
				      sprintf(testclass,"%d",(int)OldDvalue);
				      if(SetClassLevel(testclass,TablesToExtract[curr_table])>0)
					  { 
					    return 0;
					  } 
					}

				  }
			      else
				  {
		            GEOMTYPE   = 1;  
		            SEDRIStype = TIM_DOUBLE;
		            OldDvalue  = (double)vIndx.fltVal;
				  }
				}
		      break;
		    case VT_R8:

		      OldDvalue  = (double)vIndx.fltVal;

			  if(allfieldcodes[i]>=100000) 
			  {
                GEOMTYPE   = 1;  
                SEDRIStype = TIM_DOUBLE;
                OldDvalue = vIndx.dblVal;
			  }
			  else
			  {
			    if(gifd_attr_enum_count[allfieldcodes[i]]>0)
				{ 
				  HandleDoublePickListVal(allfieldcodes[i],OldDvalue,&GEOMTYPE,&SEDRIStype,&newenum,featureid+1);

	              if(!strcmp(allfieldnames[i],"SEC_CLASS"))
				  { 
				    sprintf(testclass,"%d",(int)OldDvalue);
				    if(SetClassLevel(testclass,TablesToExtract[curr_table])>0)
					{ 
				      return 0;
					} 
				  }
				}
			    else
				{
		          GEOMTYPE   = 1;  
		          SEDRIStype = TIM_DOUBLE;
		          OldDvalue = vIndx.dblVal;
				} 
			  }
		      break;


			case VT_DATE:  

			  int day,month,year;

			  GetDateComponents((int)vIndx.date,&day,&month,&year);

	          sprintf(OldCvalue,"%04d%02d%02d",year,month,day);

			  GEOMTYPE = 2;
			  SEDRIStype = TIM_STRING;
              break;

		    case VT_BSTR:


		      sprintf(OldCvalue,"%S",vIndx.bstrVal);


			  

              if((OldCvalue[0]==' ')&&(OldCvalue[1]=='\0'))
			  {
                

                GEOMTYPE = 2;
                SEDRIStype = TIM_STRING;

				sprintf(OldCvalue,"NULL string found");

			    if(ATTRTYPE_ANY_MGCP())
				{
			      AddBlankAttr(ReportName,allfieldnames[i],featureid+1,2); 
				}
			  }
		      else if(allfieldcodes[i]>=100000) 
			  {

                GEOMTYPE = 2;
                SEDRIStype = TIM_STRING;

				if(allfieldcodes[i]==100005)
				{
				  if(!strcmp(FCODEsOfTables[curr_table],"*****"))
				  {
					int tempcode1;

					
                    tempcode1 = ConvertClassCode(OldCvalue,-999);

                    if(tempcode1!=-100)
					{
					  if(foundclass[0]=='\0')
					  {
						
                        sprintf(foundclass,"%s",OldCvalue);
					  }
					}
				  }
				  else
				  {
					  
					  if(stricmp(FCODEsOfTables[curr_table],OldCvalue))
					  {
                        AddBadClass(OldCvalue,FCODEsOfTables[curr_table],featureid+1);
					  }
				  }
                  GEOMTYPE = -999;
				}
			  }
			  else if(gifd_attr_enum_count[allfieldcodes[i]]==0)
				{
                  GEOMTYPE = 2;
                  SEDRIStype = TIM_STRING;
				}

		      else 
			{ 			  
			  

              if(!strcmp(allfieldnames[i],"SEC_CLASS"))
			  { 
		        sprintf(testclass,"%s",OldCvalue);
                if(SetClassLevel(testclass,TablesToExtract[curr_table])>0)
				{ 
                  return 0;
				} 
			  }


			  
			  enumvalue[0] = '\0'; 
			  sprintf(enumvalue,"%s",OldCvalue);
			  
			  if(enumvalue[0]=='\0')
			    {
			      
			      GEOMTYPE = -999;
			      
			      AddBadAttr(allfieldcodes[i],enumvalue,featureid + 1);
			    }
			  else 
			    {
			      newenum = GetNewEnum(allfieldcodes[i],
						   enumvalue,
						   0,
						   featureid + 1);
			      			      
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
				      
				      newenum = GetNewEnum(allfieldcodes[i],
							   enumvalue,
							   1,
							   featureid + 1);			      
				      
				      if(newenum>=0)
					{
					  
					  GEOMTYPE = 3;
					  SEDRIStype = TIM_ENUMERANT;
					  
					  AddBadEnumType(allfieldcodes[i],layername,scrubenumvalue,enumvalue);
					  
					  
					  
					}
				      else
					{
					  
					  GEOMTYPE = -999;
					}
				    }
				  else
				    {				      
				      AddBadAttr(allfieldcodes[i],scrubenumvalue,featureid + 1);
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
		      printf("unknown type %d\n",vIndx.vt);
		      break;
			} 






		  hr = VariantClear(&vIndx);
		  if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,23,allfieldnames[i]);
		    } 

		  VariantInit(&vIndx);



		} 



	      if(GEOMTYPE!=-999)
		{
		  Cvalue[0] = '\0';
		  Ivalue = 0;
		  Dvalue = 0.0;
		  
		  if (GEOMTYPE==0)   
		    {
		      Ivalue = OldIvalue;
		    }
		  else if(GEOMTYPE==3) 
		    {  
		      Ivalue = newenum;
		    }
		  else if(GEOMTYPE==1)
		    {
		      Dvalue = OldDvalue;
		    }
		  else if(GEOMTYPE==2)
		  {
			  if(LongCode(allfieldnames[i]))
			  {
		        sprintf(Cvalue,"%s",OldCvalue);
			  }
			  else
			  {
		        sprintf(Cvalue,"\"%s\"",OldCvalue);

			    if(DoNFI())
				{  
                  if(!strcmp(allfieldnames[i],GetNFI()))
				  {
                    sprintf(saveNFI,"\"%s\"",OldCvalue);
				  }
                  if(!strcmp(allfieldnames[i],GetNFN()))
				  {
                    sprintf(saveNFN,"\"%s\"",OldCvalue);
				  }
				}  

			  }
		      
		      len = strlen(Cvalue);
		      
		      for(zz=0;zz<len;zz++)
			  { 
			    if ((Cvalue[zz]<32)&&(Cvalue[zz]>=0))
			    {
			      
			      
			      Cvalue[zz] = '!';
			    }
			  } 
		      
		      
		  } 
		   

          



		  if(allfieldcodes[i]==100000)
		    {
		      gifd_found = 1;
		      if(GEOMTYPE==0)
			  {
			   sprintf(thisgifd,"%d",Ivalue);
			  }
		      else if(GEOMTYPE==1)
			  {
			   sprintf(thisgifd,"%lf",Dvalue);
			  }
		      else if(GEOMTYPE==2)
			  {
               if((OldCvalue[0]==' ')&&(OldCvalue[1]=='\0'))
			   {
                 sprintf(thisgifd," Blank %s found ",GetFrontPart());
			   }
			   else
			   {
			     sprintf(thisgifd,"%s",OldCvalue);
			   }

			   len = strlen(thisgifd);
		      
		       for(zz=0;zz<len;zz++)
			   {  
			     if ((thisgifd[zz]<32)&&(thisgifd[zz]>=0))
				 { 
			      
			      thisgifd[zz] = '!';
				 }
			   }  
			  } 
		      else
			  {
			   sprintf(thisgifd,"<Invalid>");
			   printf("Invalid GFID/FLDBID found\n");
			  }
		    }
		   else if(allfieldcodes[i]==100001) 
		    {
		      if(GEOMTYPE==0)
			  {
				SaveAreaI  = Ivalue;
				area_found = I_TYPE;
			  }
		      else if(GEOMTYPE==1)
			  {
				SaveAreaD  = Dvalue;
				area_found = D_TYPE;
			  }
		      else if(GEOMTYPE==2)
			  {
			    sprintf(SaveAreaC,"%s",Cvalue);
				area_found = C_TYPE;
			  }
		      else
			  {
			    printf("unexpected data type found for OS area.  %d\n",GEOMTYPE);
				area_found = NOTYPE;
			  }
		    }
		   else if(allfieldcodes[i]==100002) 
		    {
		      if(GEOMTYPE==0)
			  {
				SaveHgtI  = Ivalue;
				hgt_found = I_TYPE;
			  }
		      else if(GEOMTYPE==1)
			  {
				SaveHgtD  = Dvalue;
				hgt_found = D_TYPE;
			  }
		      else if(GEOMTYPE==2)
			  {
			    sprintf(SaveHgtC,"%s",Cvalue);
				hgt_found = C_TYPE;
			  }
		      else
			  {
			    printf("unexpected data type found for OS hgt.  %d\n",GEOMTYPE);
				hgt_found = NOTYPE;
			  }
		    }
		   else if(allfieldcodes[i]==100006) 
		    {
		      if(GEOMTYPE==0)
			  {
				SaveLenI  = Ivalue;
				len_found = I_TYPE;
			  }
		      else if(GEOMTYPE==1)
			  {
				SaveLenD  = Dvalue;
				len_found = D_TYPE;
			  }
		      else if(GEOMTYPE==2)
			  {
			    sprintf(SaveLenC,"%s",Cvalue);
				len_found = C_TYPE;
			  }
		      else
			  {
			    printf("unexpected data type found for GM len.  %d\n",GEOMTYPE);
				len_found = NOTYPE;
			  }
		    }
		   else if(allfieldcodes[i]==100007) 
		    {
		      if(GEOMTYPE==0)
			  {
				SaveNamI  = Ivalue;
				nam_found = I_TYPE;
			  }
		      else if(GEOMTYPE==1)
			  {
				SaveNamD  = Dvalue;
				nam_found = D_TYPE;
			  }
		      else if(GEOMTYPE==2)
			  {
			    sprintf(SaveNamC,"%s",Cvalue);
				nam_found = C_TYPE;

			    if(DoNFI())
				{
                  sprintf(saveNAM,"\"%s\"",Cvalue);
				}
			  }
		      else
			  {
			    printf("unexpected data type found for GM nam.  %d\n",GEOMTYPE);
				nam_found = NOTYPE;
			  }
		    }
		   else if(allfieldcodes[i]==100003) 
		    {
		      if(GEOMTYPE==0)
			  {
				SaveZv2I  = Ivalue;
				zv2_found = I_TYPE;
			  }
		      else if(GEOMTYPE==1)
			  {
				SaveZv2D  = Dvalue;
				zv2_found = D_TYPE;
			  }
		      else if(GEOMTYPE==2)
			  {
			    sprintf(SaveZv2C,"%s",Cvalue);
				zv2_found = C_TYPE;
			  }
		      else
			  {
			    printf("unexpected data type found for OS zv2.  %d\n",GEOMTYPE);
				zv2_found = NOTYPE;
			  }
		    }
		   else if(allfieldcodes[i]==100004) 
		    {
			  newfeatureid = -1;

		      if(GEOMTYPE==0)
			  {
				newfeatureid = Ivalue;
			  }
		      else if(GEOMTYPE==1)
			  {
				newfeatureid = (int)Dvalue;
			  }
		      else if(GEOMTYPE==2)
			  {
				
			  }
		      else
			  {
			    printf("unexpected data type found for OS Objid.  %d\n",GEOMTYPE);
			  }
		    }
   		   else if(allfieldcodes[i]==100005) 
		    {
			   
		    }
 		  else
		    {

              if(ATTRTYPE_ANY_MGCP())
			  {
                if((GEOMTYPE==0)||(GEOMTYPE==3))
				{
                  optional_attribute[i] = EvalValue(optional_attribute[i],
				                                    ReportName,
				                                    allfieldnames[i],
				                                    OldIvalue, 
				                                    OldDvalue, 
				                                    OldCvalue,
				                                    1); 
				}
                else if(GEOMTYPE==1)
				{
                  optional_attribute[i] = EvalValue(optional_attribute[i],
				                                    ReportName,
				                                    allfieldnames[i],
				                                    OldIvalue,
				                                    OldDvalue,
				                                    OldCvalue,
				                                    2); 
				}
                else if(GEOMTYPE==2)
				{
			      optional_attribute[i] = EvalValue(optional_attribute[i],
				                                    ReportName,
				                                    allfieldnames[i],
				                                    OldIvalue,
				                                    OldDvalue,
				                                    OldCvalue,
				                                    3); 
				}
			  } 


		      if(SH_AddLongAttribute (SH_SAC_LONG, sh_sac_long_index,
				  allfieldcodes[i],
					      TIM_UNITLESS,TIM_UNI,SEDRIStype,
					      Ivalue,Dvalue,Cvalue))
			  { 
			    SH_AddShortAttribute(SH_SAC_SHORT,sh_sac_short_index,
				  allfieldcodes[i]);

			    sh_sac_short_index = sh_sac_short_index + 1;
			    sh_sac_long_index = sh_sac_long_index + 1;
			  }	 

			}  
		} 


	    } 


      featureid = featureid + 1;


	  

	  if(strcmp(FCODEsOfTables[curr_table],"*****"))
	  {
        class_code = ConvertClassCode(FCODEsOfTables[curr_table],-999);
	  }
	  else if(foundclass[0]!='\0')
	  {
        class_code = ConvertClassCode(foundclass,-999);
	  }
	  else
	  {
        
        class_code = -100;
        SH_bad_objects = SH_bad_objects + 1;
        AddBadClass("NO FCODE FOUNDzzzSentinel","*****",featureid+1);
	  }


	  if(class_code!=-100)
	    {


	      seeitgeom = TableGeoms[curr_table];


	      if(TableGeoms[curr_table]==1) 
		  { 
             IDispatch *pdisp, *pdisp2;
             _DGMPolygonGeometry  *area_geom;  
             _DGMBoundaryGeometry *holy_geom;  

		  if(TestNumArealNodes>EAREALNODELIMIT)
		    {
		      
		      ReSetArealFile();
		      TestNumArealNodes = 0;
		      SH_Areals_Prev = SH_Areals_Done;
		    }
		  
		  hr = Geom->get_Value(&vIndx);
		  if(!SUCCEEDED(hr))
		    {
		      printf("Error: Null feature id %d\n",newfeatureid);
			  AddNullShape(newfeatureid);
			  usethis = 0;
		    }
		  else
		  {
		    hr = Geom->get_Type(&geomtype);
		    if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,25,"");
		      return 0;
		    } 

		    hr = gss->StorageToGeometry(vIndx, &pdisp);
		    if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,26,"");
		      return 0;
		    } 

		    hr = gss->StorageToGeometry(vIndx, &pdisp2);
		    if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,27,"");
		      return 0;
		    } 

		  
		    usethis = 0;
		  
            if(pdisp==NULL)
			{
			  
			  printf("Error: Zero-part area feature. id %d %s\n",newfeatureid,thisgifd);

              fprintf(badshapeout,"Area feature detected with 0 parts.  FID %d\n  Geomedia Access database: %s Table %s\n",
	                  newfeatureid,layername,TablesToExtract[curr_table]);

              usethis = 0;
			} 
			else
			{
		      hr = pdisp->QueryInterface(IID__DGMPolygonGeometry, (void**)&area_geom);
		      if(SUCCEEDED(hr))
			  { 
		        usethis = 1;
			  } 
		      else
			  { 
		        hr = pdisp2->QueryInterface(IID__DGMBoundaryGeometry, (void**)&holy_geom);
		        if(SUCCEEDED(hr))
				{ 
			      usethis = 2;   
				} 
			  } 
		  
		      pdisp->Release();
		      pdisp2->Release();
			}
		  }
		  
		  
		  
		  if(usethis==1)  
		    {
              VariantClear(&vIndx);

			  if(ExtractAreaNoHoles(area_geom,addtolong,addtoshort,0,type,newfeatureid)==0)
			  {
                 return 0;
			  }

			  if(area_geom!= NULL)
			  {
		        area_geom->Release();		  
			  }
		    }
		  
		  else if(usethis==2) 
		    {
              VariantClear(&vIndx);

			  if(ExtractAreaWithHoles(holy_geom,addtolong,addtoshort,0,type,newfeatureid)==0)
			  {
                 return 0;
			  }

			  if(holy_geom!=NULL)
			  {
		        holy_geom->Release();
			  }
		    }
		  else
		    {
			  IDispatch *pdisp3;
			  _DGMGeometryCollection *mpa_geom;

			  

              hr = gss->StorageToGeometry(vIndx, &pdisp3);
		      if(!SUCCEEDED(hr))
			  { 
		        OSErrorHandle(type,27,"");
		        return 0;
			  }

              VariantClear(&vIndx);

		      hr = pdisp3->QueryInterface(IID__DGMGeometryCollection, (void**)&mpa_geom);

              if(!SUCCEEDED(hr))
			  { 
                printf("Skipping area feature with id %d because it is not a Polygon, Boundary, or GeometryCollection\n",
					newfeatureid);
			  }
			  else
			  {
				  long numparts;

				  hr = mpa_geom->get_Count(&numparts);

                  if(!SUCCEEDED(hr))
				  {
					 printf("Error locating number of parts of multi part area feature %d..ignoring this feature\n",newfeatureid);
				  }
				  else
				  {
 					 VARIANT part_num;

					 VariantInit(&part_num);
					 part_num.vt = VT_UI4;

					 

			         NumMultiPartAreaFeatures = NumMultiPartAreaFeatures + 1;

					 for(i=1;i<=numparts;i++)
					 {
                       IDispatch *mpa_dispatch;

		               part_num.intVal = i;

   					   hr = mpa_geom->Item(part_num,&mpa_dispatch);

	                   if(!SUCCEEDED(hr))
					   {
						 printf("Error: could not get multi-part area part %d/%d for %d\n",i,numparts,newfeatureid);
					   }
					   else
					   { 
                         _DGMPolygonGeometry  *mp_area_geom;  
                         _DGMBoundaryGeometry *mp_holy_geom;  

						 
		  
                         hr = mpa_dispatch->QueryInterface(IID__DGMPolygonGeometry, (void**)&mp_area_geom);
                         if(SUCCEEDED(hr))
						 { 
						   
                           NumAreaParts = NumAreaParts + 1;						 

                           if(ExtractAreaNoHoles(mp_area_geom,addtolong,addtoshort,i,type,newfeatureid)==0)
						   {
                             return 0;
						   }
						 }
                         else
						 {
                           hr = mpa_dispatch->QueryInterface(IID__DGMBoundaryGeometry, (void**)&mp_holy_geom);
                           if(SUCCEEDED(hr))
						   {

                             printf("it was a boundary part\n");

                             NumAreaParts = NumAreaParts + 1;

                             if(ExtractAreaWithHoles(mp_holy_geom,addtolong,addtoshort,i,type,newfeatureid)==0)
							 { 
                               return 0;
							 }

						   }
						   else
						   {
                             printf("could not get multi part area feature part # %d/%d for %d\n",i,numparts,newfeatureid);
						   }
						 } 
					   }

					 mpa_dispatch->Release();

					 } 

				  } 

			  } 
            pdisp3->Release();
		  }

		}
	      else if(TableGeoms[curr_table]==4) 
		{

		  if(TestNumLinearNodes>ELINEARNODELIMIT)
		    {
		      
		      ReSetLinearFile();
		      TestNumLinearNodes = 0;
		      SH_Linears_Prev = SH_Linears_Done;
		    }
		  

          hr = Geom->get_Value(&vIndx);

          if(!SUCCEEDED(hr))
		    {
			  

			  printf("Error: Null feature. id %d %s\n",newfeatureid,thisgifd);
			  AddNullShape(newfeatureid);
			  usethis = 0;
		    }
		  else
		  {
            IDispatch *pdisp;
			_DGMPolylineGeometry *line_geom;

		    hr = gss->StorageToGeometry(vIndx, &pdisp);
		    if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,42,"");
		      return 0;
		    } 

		    VariantClear(&vIndx);

            if(pdisp==NULL)
			{ 
			  printf("Error: Zero-part line feature. id %d %s\n",newfeatureid,thisgifd);

              fprintf(badshapeout,"Line feature detected with 0 parts.  FID %d\n  Geomedia Access database: %s Table %s\n",
	                  newfeatureid,layername,TablesToExtract[curr_table]);

              usethis = 0;
			} 
			else 
			{
		      hr = pdisp->QueryInterface(IID__DGMPolylineGeometry, (void**)&line_geom);

			  if(!SUCCEEDED(hr))
			  { 
			    _DGMGeometryCollection *mp_geom;

		        hr = pdisp->QueryInterface(IID__DGMGeometryCollection, (void**)&mp_geom);

                if(!SUCCEEDED(hr))
				{  
                  printf("Skipping line feature with id %d because it is not Polyline or GeometryCollection\n",
					  newfeatureid);
				  usethis = 0;
				} 
			    else
				{ 
				  long numparts;

				  hr = mp_geom->get_Count(&numparts);

                  if(!SUCCEEDED(hr))
				  {
					 printf("Error locating number of parts of multi part line feature %d\n",newfeatureid);
				  }
				  else
				  {
 					 VARIANT part_num;

					 VariantInit(&part_num);
					 part_num.vt = VT_UI4;

					 

			         NumMultiPartLineFeatures = NumMultiPartLineFeatures + 1;

					 for(i=1;i<=numparts;i++)
					 {
                       IDispatch *mp_dispatch;

		               part_num.intVal = i;

   					   hr = mp_geom->Item(part_num,&mp_dispatch);

	                   if(!SUCCEEDED(hr))
					   {
						 printf("Error: could not get line part %d/%d for %d\n",i,numparts,newfeatureid);
					   }
					   else
					   { 
                         _DGMPolylineGeometry *mp_line_geom;

						 

					     hr = mp_dispatch->QueryInterface(IID__DGMPolylineGeometry, (void**)&mp_line_geom);
   	                     if(!SUCCEEDED(hr))
						 { 
						   printf("could not get multi part line feature part # %d/%d for %d\n",i,numparts,newfeatureid);
						 }
					     else
						 { 
						   

                           if(ExtractLineFeature(mp_line_geom,addtolong,addtoshort,i,type,newfeatureid)==0)
						   {  
		                     
		                     return 0;
						   }
						   
						   if(mp_line_geom!=NULL)
						   {
                             NumLineParts = NumLineParts + 1;
			  
                             mp_line_geom->Release();
						   }
						 } 
					   }

					 mp_dispatch->Release();

					 } 

				  } 

				  mp_geom->Release();

				} 

			  }  
			  else
			  { 
		        pdisp->Release();

			    if(ExtractLineFeature(line_geom,addtolong,addtoshort,0,type,newfeatureid)==0)
				{  
		          
		          return 0;
				}  

				if(line_geom!=NULL)
				{
  	              line_geom->Release();
				}
			  } 
			} 
		  } 
		} 
		
		else if(TableGeoms[curr_table]==5) 
		{
		  
		  
          hr = Geom->get_Value(&vIndx);
		  if(!SUCCEEDED(hr))
		    { 
			  printf("Error: Null feature id %d\n",newfeatureid);
			  AddNullShape(newfeatureid);
			  usethis = 0;
		    } 
		  else
		  {
            IDispatch *pdisp;
			_DGMPointGeometry *point_geom;

		    hr = gss->StorageToGeometry(vIndx, &pdisp);
		    if(!SUCCEEDED(hr))
		    {
		      OSErrorHandle(type,48,"");
		      return 0;
		    }

		    VariantClear(&vIndx);

            if(pdisp==NULL)
			{
			  
			  printf("Error: Zero-part point feature. id %d %s\n",newfeatureid,thisgifd);

              fprintf(badshapeout,"Point feature detected with 0 parts.  FID %d\n  Geomedia Access database: %s Table %s\n",
	                  newfeatureid,layername,TablesToExtract[curr_table]);
			}
			else
			{
		      pdisp->QueryInterface(IID__DGMPointGeometry, (void**)&point_geom);
		      pdisp->Release();


              if(ExtractPointFeature(point_geom,addtolong,addtoshort,type,newfeatureid)==0)
			  { 
		        
		        return 0;
			  }

			  if(point_geom!=NULL)
			  {
			    point_geom->Release();
			  }

			} 
		  } 
		} 
      } 


	  hr = lRS->MoveNext();
	  if(!SUCCEEDED(hr))
	    {
	      OSErrorHandle(type,50,"");
	      return 0;
	    }
 
	  hr = lRS->get_EOF(&leof);
	  if(!SUCCEEDED(hr))
	    {
	      OSErrorHandle(type,51,"");
	      return 0;
	    } 

	  
	  if((SH_Points_Done+SH_Linears_Done+SH_Areals_Done)%250==0)
	    {
	      update_shape_import(inspecfilename,layername, curr_table+1,-1,-1,
				  SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
				  0,0,0,0,SH_bad_objects, SH_BadCoords, SH_Null_Shapes, SH_Zero_Shapes,SH_Bad_DBF);	      
	    }


	  if(ABORT_EXTRACT==1)
	    {
	      SH_FreeLists("ok",layername); 
	      SH_FreeTree(); 
	      
	      for(i=0;i<num_fields;i++)
		{
		  allfields[i]->Release();
		  free(allfieldnames[i]);
		}
	      
	      free(allfields);
	      free(allfieldnames);
	      free(allfieldcodes);
	      free(allfieldtypes);

		  if(ATTRTYPE_ANY_MGCP())
		  { 
            free(optional_attribute);
		  }
	      
	      gss->Release();
	      
	      hr = lRS->Close();
	      if(!SUCCEEDED(hr))
		{
		  OSErrorHandle(type,52,"");
		}

	      lRS->Release();
	      
	      free(lOP);
	      hr = lConn->Disconnect();
	      if(!SUCCEEDED(hr))
		{
		  OSErrorHandle(type,53,"");
		}
	      
	      lConn->Release();
	      SH_PrintListChunk(1,curr_table,OSname,TablesToExtract[curr_table]);  
 	      return 0;
	    }


	  

	  for(i=0;i<SH_LISTLEN;i++)
	    {
	      if(SH_SAC_LONG[i].lod_stuff!=NULL)
		  { 
		   free(SH_SAC_LONG[i].lod_stuff);
		  } 
	      if(SH_SAC_SHORT[i].lod_stuff!=NULL)
		  {
		   free(SH_SAC_SHORT[i].lod_stuff);
		  }
	      if(SH_SAC_LONG[i].Cvalue!=NULL)
		  { 
		   free(SH_SAC_LONG[i].Cvalue);
		  } 
	      
	      SH_SAC_LONG[i].lod_stuff  = NULL;
	      SH_SAC_SHORT[i].lod_stuff = NULL;
	      SH_SAC_LONG[i].Cvalue     = NULL;
	  }




	}  


     


      
      
      
      
      addtolong  = addtolong  + scc_long_items;
      addtoshort = addtoshort + scc_short_items;
      
      SH_PrintListChunk(0,curr_table+1,inspecfilename,layername);

      
      SH_FreeLists("ok",layername); 
      SH_FreeTree(); 
      	 

	  


	  if(ATTRTYPE_ANY_MGCP())
	  {
		int fieldnum;

  	    for(fieldnum=0;fieldnum<num_fields;fieldnum++)
		{ 
		  if((allfieldcodes[fieldnum]<0)||(allfieldcodes[fieldnum]>=100000))
		  {
			
            continue;
		  }

	      if(optional_attribute[fieldnum]==-11)
		  {
	        if(
				(allfieldtypes[fieldnum]==gdbInteger) ||
				(allfieldtypes[fieldnum]==gdbLong)    ||
				(allfieldtypes[fieldnum]==gdbBoolean) ||
				(allfieldtypes[fieldnum]==gdbByte)
			  )
			{ 
		      fprintf(mixnull,"%s:\n  Column %s has mix of Null (-32768) and Non-Null Integer values\n",
			    ReportName,allfieldnames[fieldnum]);
			} 
	        else if(
				(allfieldtypes[fieldnum]==gdbSingle) ||
				(allfieldtypes[fieldnum]==gdbDouble)
			  )
			{ 
		      fprintf(mixnull,"%s:\n  Column %s has mix of Null (-32768.0) and Non-Null Real values\n",
			    ReportName,allfieldnames[fieldnum]);
			} 
	        else if(
				    (allfieldtypes[fieldnum]==gdbText) ||
				    (allfieldtypes[fieldnum]==gdbMemo)
				   )
			{
	          fprintf(mixnull,"%s:\n  Column %s has mix of Null (N/A) and Non-Null Character values\n",
		       ReportName,allfieldnames[fieldnum]);
			} 
		  } 
		} 
	  } 


      
      for(i=0;i<num_fields;i++)
	  {
	    allfields[i]->Release();
        free(allfieldnames[i]);
	  } 
      
      free(allfields);
      free(allfieldnames);
      free(allfieldcodes);
      free(allfieldtypes);
      
      if(ATTRTYPE_ANY_MGCP())
	  { 
        free(optional_attribute);
	  }


      gss->Release();
      
      hr = lRS->Close();
      if(!SUCCEEDED(hr))
	{
	  OSErrorHandle(type,54,"");
	}

      lRS->Release();


	  


    SysFreeString(tableBstr);


    } 


  	
  free(lOP);
  
  *longitemsfound  = addtolong  - AddToLongOrig ; 
  *shortitemsfound = addtoshort - AddToShortOrig; 

      
  hr = lConn->Disconnect();
  if(!SUCCEEDED(hr))
    {
      OSErrorHandle(type,55,"");
    }

  lConn->Release(); 

  return 1;
}

