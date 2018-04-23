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
#include <time.h>
#include <math.h>
#include "Markup.h"
#include "share.h"








#define DOOUTXML 1
#define ThisYear 2012
#define MAX_LINEAGE 100

int VERBOSE = 0;


char theRealpath[1000];
char thepath[1000];
const char **thearray;

char **AllDataFiles;
int *AllDataFilesPresent;
long int *FilePosns=NULL;
char *ShapeFolder=NULL;
char *SaveLocalFilename;
char *SaveLocalSrc;
char *SaveLocalDesc;
char *SaveLocalName;
char *csTag;
char *thedata;
char *tempstring;
int DoLineNumbers;
char SaveLocalFilenameLine[100];
char SaveLocalNameLine[100];
char CellFCName[200];
char SubregionFCName[200];
char *CLSTAT_OVERRIDE = NULL;
char *SSCALE_OVERRIDE = NULL;
char *CCPYRT_OVERRIDE = NULL;
char *SCPYRT_OVERRIDE = NULL;
int use_sscale;
int use_clstat;
int use_ccpyrt;
int use_scpyrt;
int local_filename_max;
int local_src_max;
int local_desc_max;
int local_name_max;
int local_tag_max;
int local_thedata_max;
int local_tempstring_max;
int offsetStart;
int offsetLen;
int totallines;
int trex_type=0;
int subregion_version_found = -1;

int numattributes,version,trd1_override=0;
char attributes[10][1000];
char attributeV[10][1000];
char Ofirst[1000],Osecond[1000],Othird[1000];

char SAVECELLID[500];
char SAVECURI[500];
char SAVECURIPATH[500];
char SAVECCDATE[50],SAVECEDDAT[50],SAVECMDATE[50],
     SAVESVDAT[50],SAVENEWEST[50],SAVEOLDEST[50],
	 SAVESCDATE[50],SAVESMDATE[50],SAVESEDDAT[50],
	 SavedSubregionDates[1000];

int savespecialpath = 0;
char specialpath[500];


int working_geom_type = -1;
int InsideDataFile    = -1;
int InsideDataTree    = -1;
int PolyID            = -1;
int HoleID            = -1;
int lastsubregionid   = -1;
int pointtotal        = -999;
int linetotal         = -999;
int areatotal         = -999;
int InSubregionFound  = 0;
int datafilenum       = 0;
int subregionfound    = 0;
int foundcellid       = 0;
int foundcuri         = 0;
int foundsubregionid  = 0;
int foundSVDATE       = 0;
int foundCCDATE       = 0;
int foundCEDDAT       = 0;
int foundCMDATE       = 0;
int foundCMSTDV       = 0;
int foundCMSTDN       = 0; 
int foundSubCMSTDV    = 0;
int foundSubCMSTDN    = 0; 
int foundCMSHND       = 0;
int foundCSHNDI       = 0;
int SAVE_STIERN       = 0;
int SAVE_SMSHND       = 0;
int max_SMSHND_found  = 0;
int SingleSR          = 0;
int NonSingleSR       = 0;
int SDESCR_found      = 0;
int DO_C5             = 1;

int SSVRTI_found      = 0;
int SSVCDT_found      = 0;
int SSVCTY_found      = 0;

int potential_SSVRTI_found      = 0;
int potential_SSVCDT_found      = 0;

char potentialSSVCDT[260];
char SSVCDT[260];
char potentialSSVRTI[260];
char SSVRTI[260];

int SSCRTI_found      = 0;
int SSRCDT_found      = 0;
int SSRCTY_found      = 0;
int SEDDAT_found      = 0;
int SMDATE_found      = 0;
int SCDATE_found      = 0;
int CCDATEyear,CCDATEmonth,CCDATEday;
int CEDDATyear,CEDDATmonth,CEDDATday;
int CMDATEyear,CMDATEmonth,CMDATEday;
int SEDDATyear,SEDDATmonth,SEDDATday;
int SMDATEyear,SMDATEmonth,SMDATEday;
int SCDATEyear,SCDATEmonth,SCDATEday;
int SVDATyear,SVDATmonth,SVDATday;
int SchemaErrorCount  = 0;
int ACE_ALE           = 0;
int UseCDESCRNumber   = 0;
char SaveCDESCRNumber[45];



extern "C" int FoundNewest, NewestYear, NewestMonth, NewestDay;
extern "C" int FoundOldest, OldestYear, OldestMonth, OldestDay;


int NumFixedStringsTRD2 = 24;
int FoundFixedStringTRD2[24];


int NumFixedStringsTRD3 = 22;
int FoundFixedStringTRD3[22];

int NumFixedStringsTRD4 = 20;
int FoundFixedStringTRD4[20];


int FoundAdditionalStrings[10];
int NumAdditionalStrings = 3;

int foundCellWest  = 0;
int foundCellEast  = 0;
int foundCellSouth = 0;
int foundCellNorth = 0;
double cellWest    = 0;
double cellEast    = 0;
double cellSouth   = 0;
double cellNorth   = 0;
int clstat_found   = 0;
int slstat_found   = 0;
int cdescr_found   = 0;
int sscrti_found   = 0;
int stypeu_found   = 0;
char cellWestLine [100];
char cellEastLine [100];
char cellSouthLine[100];
char cellNorthLine[100];

char saveCellWest [300];
char saveCellEast [300];
char saveCellSouth[300];
char saveCellNorth[300];
char saveCLSTAT[300];
char saveCDESCR[300];
char saveSTYPEU[300];
char *saveSLSTAT=NULL;
char *saveSSCRTI=NULL;


FILE *outxmlfile;
FILE *outerrfile;
FILE *outPOIfile;




#if(0)
char *GET_CSTR(CString x, int type)
{

  if(type==1)
  {
    if((int)strlen(x)>local_tag_max)
	{ 
	  printf("reallocating for %d (vice %d)\n",strlen(x),x.GetLength());

	  
	  csTag = (char *)realloc(csTag,strlen(x) + 10);
	  local_tag_max = strlen(x);

	  free(thedata);
	  thedata = (char *)malloc(strlen(x) + 10);
	  local_thedata_max = strlen(x);

	  free(tempstring);
	  tempstring = (char *)malloc(strlen(x) + 10);
	  local_tempstring_max = strlen(x);
	} 
  }

  sprintf(tempstring,"%s",x.GetBuffer(local_tempstring_max+5));
  return tempstring;
}
#else
char *GET_CSTR(std::string x, int type)
{

  if(type==1)
  {
    if((int)x.length()>local_tag_max)
	{
      printf("reallocating for %d\n",(int)x.length());
      
	  
      csTag = (char *)realloc(csTag,x.length() + 10);
      local_tag_max = x.length();
     
      free(thedata);
      thedata = (char *)malloc(x.length() + 10);
      local_thedata_max = x.length();
      
      free(tempstring);
      tempstring = (char *)malloc(x.length() + 10);
      local_tempstring_max = x.length();
	}
  }

  sprintf(tempstring,"%s",x.c_str());
  return tempstring;
}
#endif



































#define num_trex_specs 409

char *TREx_specs[num_trex_specs] = 
{
(char *)"ID,Name,Definition,Data Type,Domain,Obligation,XPath",
(char *)"1,fileIdentifier,Unique identifier for the metadata file,CharacterString,TDx_bDDcDDD_vv.xml,M (TDR only),/gmd:MD_Metadata[1]/gmd:fileIdentifier[1]/gco:CharacterString[1]",
(char *)"1,fileIdentifier,Unique identifier for the metadata file,CharacterString,TDx_eDDfDDD_vv.xml,M (TDF only),/gmd:MD_Metadata[1]/gmd:fileIdentifier[1]/gco:CharacterString[1]",
(char *)"2,language,Language used for documenting metadata (Alpha3),CharacterString,eng,M,/gmd:MD_Metadata[1]/gmd:language[1]/gco:CharacterString[1]",
(char *)"3,characterSet,Full name of the character coding standard used for the metadata,MD_CharacterSetCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_CharacterSetCode,M,/gmd:MD_Metadata[1]/gmd:characterSet[1]/gmd:MD_CharacterSetCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = utf8,,/gmd:MD_Metadata[1]/gmd:characterSet[1]/gmd:MD_CharacterSetCode[1]/@codeListValue",
(char *)"4,hierarchyLevel,Scope to which the metadata applies,MD_ScopeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_ScopeCode,M,/gmd:MD_Metadata[1]/gmd:hierarchyLevel[1]/gmd:MD_ScopeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = dataset,,/gmd:MD_Metadata[1]/gmd:hierarchyLevel[1]/gmd:MD_ScopeCode[1]/@codeListValue",
(char *)"5,hierarchyLevelName,Name of the hierarchy levels for which the metadata is provided,CharacterString,Geotile,M (TDR only),/gmd:MD_Metadata[1]/gmd:hierarchyLevelName[1]/gco:CharacterString[1]",
(char *)"5,hierarchyLevelName,Name of the hierarchy levels for which the metadata is provided,CharacterString,Geocell,M (TDF only),/gmd:MD_Metadata[1]/gmd:hierarchyLevelName[1]/gco:CharacterString[1]",
(char *)"6.1,individualName,\"Name of the responsible person-surname, given name, title separated by a delimeter \",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:individualName[1]/gco:CharacterString[1]",
(char *)"7.1,organisationName,Name of the responsible organization,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]",
(char *)"8.1,voice,Telephone number by which individuals can speak to the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:phone[1]/gmd:CI_Telephone[1]/gmd:voice[1]/gco:CharacterString[1]",
(char *)"9.1,deliveryPoint,Address line for the location (as described in ISO 11180),CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:deliveryPoint[1]/gco:CharacterString[1]",
(char *)"10.1,city,City of the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:city[1]/gco:CharacterString[1]",
(char *)"11.1,administrativeArea,\"State,province of the location\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:administrativeArea[1]/gco:CharacterString[1]",
(char *)"12.1,postalCode,ZIP or postal code,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:postalCode[1]/gco:CharacterString[1]",
(char *)"13.1,country,ISO3166-1 Digraph Country Codes,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:country[1]/gco:CharacterString[1]",
(char *)"14.1,electronicMailAddress,Address of the electronic mailbox of the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:electronicMailAddress[1]/gco:CharacterString[1]",
(char *)"15.1,linkage,Location for on-line access using a Uniform Resource Locator address ,URL,Free text character string,O,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"16.1,role,Function performed by the responsible party,CI_RoleCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_RoleCode,M,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = pointOfContact,,/gmd:MD_Metadata[1]/gmd:contact[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeListValue",
(char *)"6.2,individualName,\"Name of the responsible person-surname, given name, title separated by a delimeter \",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:individualName[1]/gco:CharacterString[1]",
(char *)"7.2,organisationName,Name of the responsible organization,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]",
(char *)"8.2,voice,Telephone number by which individuals can speak to the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:phone[1]/gmd:CI_Telephone[1]/gmd:voice[1]/gco:CharacterString[1]",
(char *)"9.2,deliveryPoint,\"Address line for the location (as described in ISO 11180, Annex A)\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:deliveryPoint[1]/gco:CharacterString[1]",
(char *)"10.2,city,City of the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:city[1]/gco:CharacterString[1]",
(char *)"11.2,administrativeArea,\"State,province of the location\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:administrativeArea[1]/gco:CharacterString[1]",
(char *)"12.2,postalCode,ZIP or postal code,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:postalCode[1]/gco:CharacterString[1]",
(char *)"13.2,country,ISO3166-1 Digraph Country Codes,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:country[1]/gco:CharacterString[1]",
(char *)"14.2,electronicMailAddress,Address of the electronic mailbox of the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:electronicMailAddress[1]/gco:CharacterString[1]",
(char *)"15.2,linkage,Location for on-line access using a Uniform Resource Locator address ,URL,Free text character string,O,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"16.2,role,Function performed by the responsible party,CI_RoleCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_RoleCode,M,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = originator,,/gmd:MD_Metadata[1]/gmd:contact[2]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeListValue",
(char *)"17,dateStamp,Date that the metadata was created,Date,YYYY-MM-DD,M,/gmd:MD_Metadata[1]/gmd:dateStamp[1]/gco:Date[1]",
(char *)"18,metadataStandardName,Name of the metadata standard (including profile name) used,CharacterString,TREx Metadata Definition,M,/gmd:MD_Metadata[1]/gmd:metadataStandardName[1]/gco:CharacterString[1]",
(char *)"19,metadataStandardVersion,Version (profile) of the metadata standard used,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:metadataStandardVersion[1]/gco:CharacterString[1]",
(char *)"20,dataSetURI,Uniformed Resource Identifier of the dataset to which the metadata applies,CharacterString,./DEM/TDx_bDDcDDD_vv_DEM.tif,M (TDR only),/gmd:MD_Metadata[1]/gmd:dataSetURI[1]/gco:CharacterString[1]",
(char *)"20,dataSetURI,Uniformed Resource Identifier of the dataset to which the metadata applies,CharacterString,./DEM/TDx_eDDfDDD_vv_DEM.tif,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataSetURI[1]/gco:CharacterString[1]",
(char *)"21,numberOfDimensions,Enumerator defining the dimension of the dataset,Integer,2,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:numberOfDimensions[1]/gco:Integer[1]",
(char *)"22.1,dimensionName,Name of dimension,MD_DimensionNameTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_DimensionNameTypeCode,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[1]/gmd:MD_Dimension[1]/gmd:dimensionName[1]/gmd:MD_DimensionNameTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = row,,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[1]/gmd:MD_Dimension[1]/gmd:dimensionName[1]/gmd:MD_DimensionNameTypeCode[1]/@codeListValue",
(char *)"23.1,dimensionSize,Number of rows in the dataset,Integer,9001,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[1]/gmd:MD_Dimension[1]/gmd:dimensionSize[1]/gco:Integer[1]",
(char *)"24.1,resolution,Approximate resolution,Measure,0.4,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[1]/gmd:MD_Dimension[1]/gmd:resolution[1]/gco:Measure[1]",
(char *)"24.1,resolution,Approximate resolution,Measure Attribute Value(s),uom = arcsecond,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[1]/gmd:MD_Dimension[1]/gmd:resolution[1]/gco:Measure[1]/@uom",
(char *)"22.2,dimensionName,Name of dimension,MD_DimensionNameTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_DimensionNameTypeCode,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[2]/gmd:MD_Dimension[1]/gmd:dimensionName[1]/gmd:MD_DimensionNameTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = column,,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[2]/gmd:MD_Dimension[1]/gmd:dimensionName[1]/gmd:MD_DimensionNameTypeCode[1]/@codeListValue",
(char *)"23.2,dimensionSize,Number of columns in the dataset,Integer,variable,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[2]/gmd:MD_Dimension[1]/gmd:dimensionSize[1]/gco:Integer[1]",
(char *)"24.2,resolution,Approximate resolution,Measure,Variable,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[2]/gmd:MD_Dimension[1]/gmd:resolution[1]/gco:Measure[1]",
(char *)"24.2,resolution,Approximate resolution,Measure Attribute Value(s),uom = arcsecond,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:axisDimensionProperties[2]/gmd:MD_Dimension[1]/gmd:resolution[1]/gco:Measure[1]/@uom",
(char *)"25,cellGeometry,Type of cellgeometry,MD_CellGeometryCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_CellGeometryCode,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:cellGeometry[1]/gmd:MD_CellGeometryCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = point,,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:cellGeometry[1]/gmd:MD_CellGeometryCode[1]/@codeListValue",
(char *)"26,transformationParameterAvailability,Indicates presence of transformation parameters,Boolean,1,M,/gmd:MD_Metadata[1]/gmd:spatialRepresentationInfo[1]/gmd:MD_GridSpatialRepresentation[1]/gmd:transformationParameterAvailability[1]/gco:Boolean[1]",
(char *)"27,code,EPSG Code Number,CharacterString,4326,M,/gmd:MD_Metadata[1]/gmd:referenceSystemInfo[1]/gmd:MD_ReferenceSystem[1]/gmd:referenceSystemIdentifier[1]/gmd:RS_Identifier[1]/gmd:code[1]/gco:CharacterString[1]",
(char *)"28,codeSpace,Name of Codespace,CharacterString,urn:ogc:def:crs:EPSG,M,/gmd:MD_Metadata[1]/gmd:referenceSystemInfo[1]/gmd:MD_ReferenceSystem[1]/gmd:referenceSystemIdentifier[1]/gmd:RS_Identifier[1]/gmd:codeSpace[1]/gco:CharacterString[1]",
(char *)"29,version,Version of Codespace,CharacterString,8.6.1,O,/gmd:MD_Metadata[1]/gmd:referenceSystemInfo[1]/gmd:MD_ReferenceSystem[1]/gmd:referenceSystemIdentifier[1]/gmd:RS_Identifier[1]/gmd:version[1]/gco:CharacterString[1]",
(char *)"30,title,Name by which the cited resource is known,CharacterString,TDx_bDDcDDD_vv,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",
(char *)"30,title,Name by which the cited resource is known,CharacterString,TDx_eDDfDDD_vv,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",
(char *)"31,date,Date that the dataset was created,Date,YYYY-MM-DD,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
(char *)"32,dateType,Code to wich the date applies,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = creation,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue",
(char *)"33,title,Name of the Specification the dataset applies,CharacterString,TREx DEM Raw Specification,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",
(char *)"33,title,Name of the Specification the dataset applies,CharacterString,TREx DEM Finished Specification,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",
(char *)"34,date,Date that the Documentation was created,Date,YYYY-MM-DD,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
(char *)"35,dateType,Reference date type for the cited date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = creation,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue",
(char *)"36,presentationForm,Information about the type of data being referenced,CI_PresentationFormCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_PresentationFormCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:presentationForm[1]/gmd:CI_PresentationFormCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = documentDigital,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:authority[1]/gmd:CI_Citation[1]/gmd:presentationForm[1]/gmd:CI_PresentationFormCode[1]/@codeListValue",
(char *)"37,code,Unique identification of the dataset,CharacterString,TDx_bDDcDDD_vv,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:code[1]/gco:CharacterString[1]",
(char *)"37,code,Unique identification of the dataset,CharacterString,TDx_eDDfDDD_vv,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:citation[1]/gmd:CI_Citation[1]/gmd:identifier[1]/gmd:MD_Identifier[1]/gmd:code[1]/gco:CharacterString[1]",
(char *)"38,abstract,Brief narrative summary of the content of the resource,CharacterString,Free text character string (as defined in Annex C),M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:abstract[1]/gco:CharacterString[1]",
(char *)"39,purpose,Summary of the intentions with which the resource was developed,CharacterString,Free text character string (as defined in Annex C),M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:purpose[1]/gco:CharacterString[1]",
(char *)"40,status,Information about the status of the resource,MD_ProgressCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_ProgressCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:status[1]/gmd:MD_ProgressCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = onGoing (TDR) or codeListValue = completed (TDF),M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:status[1]/gmd:MD_ProgressCode[1]/@codeListValue",
(char *)"41.1,individualName,\"Name of the responsible person-surname, given name, title separated by a delimeter \",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:individualName[1]/gco:CharacterString[1]",
(char *)"42.1,organisationName,Name of the responsible organization,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]",
(char *)"43.1,voice,Telephone number by which individuals can speak to the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:phone[1]/gmd:CI_Telephone[1]/gmd:voice[1]/gco:CharacterString[1]",
(char *)"44.1,deliveryPoint,Address line for the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:deliveryPoint[1]/gco:CharacterString[1]",
(char *)"45.1,city,City of the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:city[1]/gco:CharacterString[1]",
(char *)"46.1,administrativeArea,\"State,province of the location\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:administrativeArea[1]/gco:CharacterString[1]",
(char *)"47.1,postalCode,Zip or postcode,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:postalCode[1]/gco:CharacterString[1]",
(char *)"48.1,country,ISO3166-1 Digraph Country Codes,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:country[1]/gco:CharacterString[1]",
(char *)"49.1,electronicMailAddress,Address of the electronic mailbox of the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:electronicMailAddress[1]/gco:CharacterString[1]",
(char *)"50.1,linkage,Location for on-line access using a Uniform Resource Locator address ,URL,Free text character string,O,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"51.1,role,Function performed by the responsible party,CI_RoleCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_RoleCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = originator,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeListValue",
(char *)"41.2,individualName,\"Name of the responsible person-surname, given name, title separated by a delimeter \",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:individualName[1]/gco:CharacterString[1]",
(char *)"42.2,organisationName,Name of the responsible organization,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]",
(char *)"43.2,voice,Telephone number by which individuals can speak to the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:phone[1]/gmd:CI_Telephone[1]/gmd:voice[1]/gco:CharacterString[1]",
(char *)"44.2,deliveryPoint,Address line for the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:deliveryPoint[1]/gco:CharacterString[1]",
(char *)"45.2,city,City of the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:city[1]/gco:CharacterString[1]",
(char *)"46.2,administrativeArea,\"State,province of the location\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:administrativeArea[1]/gco:CharacterString[1]",
(char *)"47.2,postalCode,ZIP or postal code,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:postalCode[1]/gco:CharacterString[1]",
(char *)"48.2,country,ISO3166-1 Digraph Country Codes,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:country[1]/gco:CharacterString[1]",
(char *)"49.2,electronicMailAddress,Address of the electronic mailbox of the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:electronicMailAddress[1]/gco:CharacterString[1]",
(char *)"50.2,linkage,Location for on-line access using a Uniform Resource Locator address ,URL,Free text character string,O,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"51.2,role,Function performed by the responsible party,CI_RoleCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_RoleCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = pointOfContact,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:pointOfContact[2]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeListValue",
(char *)"52,maintenanceAndUpdateFrequency,Frequency with which changes and additions are made to the resource after the initial metadata is complete,MD_MaintenanceFrequencyCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_MaintenanceFrequencyCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceMaintenance[1]/gmd:MD_MaintenanceInformation[1]/gmd:maintenanceAndUpdateFrequency[1]/gmd:MD_MaintenanceFrequencyCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = asNeeded,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceMaintenance[1]/gmd:MD_MaintenanceInformation[1]/gmd:maintenanceAndUpdateFrequency[1]/gmd:MD_MaintenanceFrequencyCode[1]/@codeListValue",
(char *)"53.1,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDx_bDDcDDD_vv_DEM_SHADED_REL_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[1]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"53.1,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDx_eDDfDDD_vv_DEM_SHADED_REL_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[1]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.1,fileDescription,Brief description of the Graphic Overview file,CharacterString,Digital Elevation Model,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[1]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.1,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[1]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.2,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDx_bDDcDDD_vv_HEM_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[2]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"53.2,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDx_eDDfDDD_vv_HEM_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[2]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.2,fileDescription,Brief description of the Graphic Overview file,CharacterString,Height Error Map,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[2]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.2,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[2]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.3,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDx_bDDcDDD_vv_AMP_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[3]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"53.3,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDx_eDDfDDD_vv_AMP_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[3]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.3,fileDescription,Brief description of the Graphic Overview file,CharacterString,SAR Amplitude Mosaic (Mean Value),M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[3]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.3,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[3]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.4,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDR_bDDcDDD_vv_AM2_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[4]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.4,fileDescription,Brief description of the Graphic Overview file,CharacterString,SAR Amplitude Mosaic (Minimum Value),M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[4]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.4,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[4]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.5,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDR_bDDcDDD_vv_WAM_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[5]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.5,fileDescription,Brief description of the Graphic Overview file,CharacterString,Water Indication Mask,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[5]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.5,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[5]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.6,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDR_bDDcDDD_vv_COV_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[6]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.6,fileDescription,Brief description of the Graphic Overview file,CharacterString,Coverage Map,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[6]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.6,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[6]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.7,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDR_bDDcDDD_vv_COM_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[7]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.7,fileDescription,Brief description of the Graphic Overview file,CharacterString,Consistency Mask,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[7]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.7,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[7]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.8,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDR_bDDcDDD_vv_LSM_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[8]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.8,fileDescription,Brief description of the Graphic Overview file,CharacterString,Layover and Shadow Mask,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[8]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.8,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[8]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.9,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDR_bDDcDDD_vv_RLM_QL.jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[9]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.9,fileDescription,Brief description of the Graphic Overview file,CharacterString,Reliability Mask,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[9]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.9,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[9]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.1,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDF_bDDcDDD_vv_EDM_QL.jpg,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[4]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.1,fileDescription,Brief description of the Graphic Overview file,CharacterString,Edit Data Mask,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[4]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.1,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[4]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.11,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDF_bDDcDDD_vv_SDM_QL.jpg,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[5]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.11,fileDescription,Brief description of the Graphic Overview file,CharacterString,Source Data Mask,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[5]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.11,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[5]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"53.12,fileName,Path of the Graphic Overview file,CharacterString,./PREVIEW/TDF_bDDcDDD_vv_WBM_QL.jpg,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[6]/gmd:MD_BrowseGraphic[1]/gmd:fileName[1]/gco:CharacterString[1]",
(char *)"54.12,fileDescription,Brief description of the Graphic Overview file,CharacterString,Water Body Mask,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[6]/gmd:MD_BrowseGraphic[1]/gmd:fileDescription[1]/gco:CharacterString[1]",
(char *)"55.12,fileType,Type of file provided for Graphic Overview,CharacterString,jpg,M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:graphicOverview[6]/gmd:MD_BrowseGraphic[1]/gmd:fileType[1]/gco:CharacterString[1]",
(char *)"56.1,keyword,Commonly used word(s) or phrase(s) used to describe the subject,CharacterString,Elevation,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:keyword[1]/gco:CharacterString[1]",
(char *)"56.2,keyword,Commonly used word(s) or phrase(s) used to describe the subject,CharacterString,Interferometry,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:keyword[2]/gco:CharacterString[1]",
(char *)"56.3,keyword,Commonly used word(s) or phrase(s) used to describe the subject,CharacterString,TanDEM-X,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:keyword[3]/gco:CharacterString[1]",
(char *)"56.4,keyword,Commonly used word(s) or phrase(s) used to describe the subject,CharacterString,SAR,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:keyword[4]/gco:CharacterString[1]",
(char *)"56.5,keyword,Commonly used word(s) or phrase(s) used to describe the subject,CharacterString,TREx DEM,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:keyword[5]/gco:CharacterString[1]",
(char *)"56.6,keyword,Commonly used word(s) or phrase(s) used to describe the subject,CharacterString,DSM,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:keyword[6]/gco:CharacterString[1]",
(char *)"57,title,Reference to Abbreviations and Acronyms used,CharacterString,TREx DEM Raw Specification,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:thesaurusName[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",
(char *)"58,date,Creation date of document,Date,YYYY-MM-DD,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:thesaurusName[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
(char *)"59,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:thesaurusName[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = creation,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:descriptiveKeywords[1]/gmd:MD_Keywords[1]/gmd:thesaurusName[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue",
(char *)"60,useLimitation,Information regarding the limitations of resource usage ,CharacterString,\"For use limitations, see TREx MoU\",M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[1]/gmd:MD_SecurityConstraints[1]/gmd:useLimitation[1]/gco:CharacterString[1]",
(char *)"61,classification,Name of the handling restrictions on the resource or metadata,MD_ClassificationCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_ClassificationCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[1]/gmd:MD_SecurityConstraints[1]/gmd:classification[1]/gmd:MD_ClassificationCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = unclassified,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[1]/gmd:MD_SecurityConstraints[1]/gmd:classification[1]/gmd:MD_ClassificationCode[1]/@codeListValue",
(char *)"62,classificationSystem,Name of the classification system for the resource or metadata,CharacterString,TREx MoU,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[1]/gmd:MD_SecurityConstraints[1]/gmd:classificationSystem[1]/gco:CharacterString[1]",
(char *)"63,handlingDescription,Information regarding handling of the resource,CharacterString,LIMITED DISTRIBUTION,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[1]/gmd:MD_SecurityConstraints[1]/gmd:handlingDescription[1]/gco:CharacterString[1]",
(char *)"64.1,useConstraints,Restrictions and legal prerequisites for accessing and using the resource or metadata,MD_RestrictionCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_RestrictionCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:useConstraints[1]/gmd:MD_RestrictionCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = copyright,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:useConstraints[1]/gmd:MD_RestrictionCode[1]/@codeListValue",
(char *)"65.1.1,otherConstraints,Other restrictions and legal prerequisites for accessing and using the resource,CharacterString,\"!! DLR e.V. YYYY, Distribution: Airbus DS Geo GmbH. All rights reserved.\",M (TDR only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:otherConstraints[1]/gco:CharacterString[1]",
(char *)"65.1.1,otherConstraints,Other restrictions and legal prerequisites for accessing and using the resource,CharacterString,\"Includes material !! DLR e.V. YYYY, Distribution: Airbus DS Geo GmbH. All rights reserved.\",M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:otherConstraints[1]/gco:CharacterString[1]",
(char *)"65.1.2,otherConstraints,Other restrictions and legal prerequisites for accessing and using the resource,CharacterString,\"ICESat GLAS data has been used to create this dataset: Zwally, H.J., R. Schutz, C. Bentley, J. Bufton, T. Herring, J. Minster, J. Spinhirne, and R. Thomas. 2014. GLAS/ICESat L2 Antarctic and Greenland Ice Sheet Altimetry Data, Version 34. Boulder, Colorado USA: NASA DAAC at the National Snow and Ice Data Center.\",M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:otherConstraints[2]/gco:CharacterString[1]",
(char *)"65.1.N,otherConstraints,\"Copyright Information for each source used in the product, as described in lineage\",CharacterString,Free text character string (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:otherConstraints[N]/gco:CharacterString[1]",
(char *)"64.2,useConstraints,Restrictions and legal prerequisites for accessing and using the resource or metadata,MD_RestrictionCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_RestrictionCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[3]/gmd:MD_LegalConstraints[1]/gmd:useConstraints[1]/gmd:MD_RestrictionCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = license,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[3]/gmd:MD_LegalConstraints[1]/gmd:useConstraints[1]/gmd:MD_RestrictionCode[1]/@codeListValue",
(char *)"65.2.1,otherConstraints,Other restrictions and legal prerequisites for accessing and using the resource,CharacterString,License terms and conditions apply. See end user license agreement document.,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[3]/gmd:MD_LegalConstraints[1]/gmd:otherConstraints[1]/gco:CharacterString[1]",
(char *)"66,spatialRepresentationType,Method used to spatially represent geographic information,MD_SpatialRepresentationTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_SpatialRepresentationTypeCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:spatialRepresentationType[1]/gmd:MD_SpatialRepresentationTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = grid,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:spatialRepresentationType[1]/gmd:MD_SpatialRepresentationTypeCode[1]/@codeListValue",
(char *)"67,language,Language(s) used within the dataset (Alpha 3),CharacterString,eng,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:language[1]/gco:CharacterString[1]",
(char *)"68,characterSet,Full name of the character coding standard used for the dataset,MD_CharacterSetCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_CharacterSetCode,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:characterSet[1]/gmd:MD_CharacterSetCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = utf8,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:characterSet[1]/gmd:MD_CharacterSetCode[1]/@codeListValue",
(char *)"69,topicCategory,Main theme(s) of the dataset,Enumeration,elevation,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:topicCategory[1]/gmd:MD_TopicCategoryCode[1]",
(char *)"147,environmentDescription,Describes the dataset's processing environment. Includes processing software and DEMES Parameters File ID.,CharacterString,\"Free text character string (e.g. DEMES 5.2.1.3185A, Parameters File Creation Key 1461734981)\",M (TDF only),/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:environmentDescription[1]/gco:CharacterString[1]",
(char *)"70,westBoundLongitude,\"Western-most coordinate of the limit of the resource extent, expressed in longitude in decimal degrees (positive east)\",Decimal,\"Float value (format: -180.00000000 to +179.00000000, use 8 decimal places, zero filled as necessary)\",M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:geographicElement[1]/gmd:EX_GeographicBoundingBox[1]/gmd:westBoundLongitude[1]/gco:Decimal[1]",
(char *)"71,eastBoundLongitude,\"Eastern-most coordinate of the limit of the resource extent, expressed in longitude in decimal degrees (positive east)\",Decimal,\"Float value (format: -179.00000000 to +180.00000000, use 8 decimal places, zero filled as necessary)\",M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:geographicElement[1]/gmd:EX_GeographicBoundingBox[1]/gmd:eastBoundLongitude[1]/gco:Decimal[1]",
(char *)"72,southBoundLatitude,\"Southern-most coordinate of the limit of the resource extent, expressed in latitude in decimal degrees (positive north)\",Decimal,\"Float value (format: -90.00000000 to +89.00000000, use 8 decimal places, zero filled as necessary)\",M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:geographicElement[1]/gmd:EX_GeographicBoundingBox[1]/gmd:southBoundLatitude[1]/gco:Decimal[1]",
(char *)"73,northBoundLatitude,\"Northern-most coordinate of the limit of the resource extent, expressed in latitude in decimal degrees (positive north)\",Decimal,\"Float value (format: -89.00000000 to +90.00000000, use 8 decimal places, zero filled as necessary)\",M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:geographicElement[1]/gmd:EX_GeographicBoundingBox[1]/gmd:northBoundLatitude[1]/gco:Decimal[1]",
(char *)"74,extent,Date and time for the content of the dataset (collection date and time),TimePeriod,frame = #ISO-8601,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/@frame",
(char *)",,,Attribute Value(s),gml:id = timespan,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/@gml:id",
(char *)"75,begin,Time Instant idicator start,TimeInstant,frame = #ISO-8601,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/gml:begin[1]/gml:TimeInstant[1]/@frame",
(char *)",,,Attribute Value(s),gml:id = start,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/gml:begin[1]/gml:TimeInstant[1]/@gml:id",
(char *)"76,TimeInstant,Date and time for the begin position (UTC),timePosition,YYYY-MM-DDThh:mm:ssZ,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/gml:begin[1]/gml:TimeInstant[1]/gml:timePosition[1]",
(char *)"77,end,Time Instant idicator stop,TimeInstant,frame = #ISO-8601,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/gml:end[1]/gml:TimeInstant[1]/@frame",
(char *)",,,Attribute Value(s),gml:id = stop,,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/gml:end[1]/gml:TimeInstant[1]/@gml:id",
(char *)"78,TimeInstant,Date and time for the end position (UTC),timePosition,YYYY-MM-DDThh:mm:ssZ,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:temporalElement[1]/gmd:EX_TemporalExtent[1]/gmd:extent[1]/gml:TimePeriod[1]/gml:end[1]/gml:TimeInstant[1]/gml:timePosition[1]",
(char *)"79,minimumValue,Lowest vertical extent contained in the dataset,Real,Float value,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:minimumValue[1]/gco:Real[1]",
(char *)"80,maximumValue,Highest vertical extent contained in the dataset,Real,Float value,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:maximumValue[1]/gco:Real[1]",
(char *)"81,verticalCRS,1-dimension coordinate reference system,VerticalCRS Attribute Value(s),gml:id = ogp-crs-3855,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/@gml:id",
(char *)"82,VerticalCRS,Unique identifier of the vertical coordinate reference system,identifier,urn:ogc:def:crs:EPSG::3855,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:identifier[1]",
(char *)"83,VerticalCRS,Unique identifier of the vertical coordinate reference system,Identifier Attribute Value(s),codeSpace = OGP,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:identifier[1]/@codeSpace",
(char *)"84,VerticalCRS,Name of the vertical coordinate reference system,name,EGM2008 geoid height,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:name[1]",
(char *)"85,VerticalCRS,Description of vertical coordinate reference system,remarks,Height surface resulting from the application of the EGM2008 geoid model to the WGS 84 ellipsoid. Replaces EGM96 geoid (CRS code 5773).,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:remarks[1]",
(char *)"86,VerticalCRS,Range which is valid for the reference system,domainOfValidity Attribute Value(s),xlink:href = urn:ogc:def:area:EPSG::1262,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:domainOfValidity[1]/@xlink:href",
(char *)"87,VerticalCRS,Purpose of the vertical coordinate reference system,scope,Geodesy.,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:scope[1]",
(char *)"88,VerticalCRS,Name of coordinate system,verticalCS Attribute Value(s),xlink:href = urn:ogc:def:cs:EPSG::6499,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:verticalCS[1]/@xlink:href",
(char *)"89,VerticalCRS,Name of vertical datum,verticalDatum Attribute Value(s),xlink:href = urn:ogc:def:datum:EPSG::1027,M,/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:extent[1]/gmd:EX_Extent[1]/gmd:verticalElement[1]/gmd:EX_VerticalExtent[1]/gmd:verticalCRS[1]/gml:VerticalCRS[1]/gml:verticalDatum[1]/@xlink:href",
(char *)"90,attributeDescription,Type of surface and data value described by the data,RecordType,\"reflective surface, real\",M,/gmd:MD_Metadata[1]/gmd:contentInfo[1]/gmd:MD_CoverageDescription[1]/gmd:attributeDescription[1]/gco:RecordType[1]",
(char *)"91,contentType,Type of information represented by the cell value,MD_CoverageContentTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_CoverageContentTypeCode,M,/gmd:MD_Metadata[1]/gmd:contentInfo[1]/gmd:MD_CoverageDescription[1]/gmd:contentType[1]/gmd:MD_CoverageContentTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = physicalMeasurement,,/gmd:MD_Metadata[1]/gmd:contentInfo[1]/gmd:MD_CoverageDescription[1]/gmd:contentType[1]/gmd:MD_CoverageContentTypeCode[1]/@codeListValue",
(char *)"92,name,Name of the dataset format,CharacterString,GeoTIFF,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributionFormat[1]/gmd:MD_Format[1]/gmd:name[1]/gco:CharacterString[1]",
(char *)"93,version,Version of the dataset format,CharacterString,1.0,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributionFormat[1]/gmd:MD_Format[1]/gmd:version[1]/gco:CharacterString[1]",
(char *)"94,specification,Specifiaction of the dataset format,CharacterString,1.8.2,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributionFormat[1]/gmd:MD_Format[1]/gmd:specification[1]/gco:CharacterString[1]",
(char *)"95,individualName,\"Name of the responsible person-surname, given name, title separated by a delimeter \",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:individualName[1]/gco:CharacterString[1]",
(char *)"96,organisationName,Name of the responsible organization,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]",
(char *)"97,voice,Telephone number by which individuals can speak to the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:phone[1]/gmd:CI_Telephone[1]/gmd:voice[1]/gco:CharacterString[1]",
(char *)"98,deliveryPoint,\"Address line for the location (as described in ISO 11180, Annex A)\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:deliveryPoint[1]/gco:CharacterString[1]",
(char *)"99,city,City of the location,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:city[1]/gco:CharacterString[1]",
(char *)"100,administrativeArea,\"State,province of the location\",CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:administrativeArea[1]/gco:CharacterString[1]",
(char *)"101,postalCode,ZIP or postal code,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:postalCode[1]/gco:CharacterString[1]",
(char *)"102,country,ISO3166-1 Digraph Country Codes,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:country[1]/gco:CharacterString[1]",
(char *)"103,electronicMailAddress,Address of the electronic mailbox of the responsible organization or individual,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:address[1]/gmd:CI_Address[1]/gmd:electronicMailAddress[1]/gco:CharacterString[1]",
(char *)"104,linkage,Location for on-line access using a Uniform Resource Locator address ,URL,Free text character string,O,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"105,role,Function performed by the responsible party,CI_RoleCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_RoleCode,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = distributor,,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:distributor[1]/gmd:MD_Distributor[1]/gmd:distributorContact[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeListValue",
(char *)"106,unitsOfDistribution,Distribution unit of resource,CharacterString,Geotile,M (TDR only),/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:unitsOfDistribution[1]/gco:CharacterString[1]",
(char *)"106,unitsOfDistribution,Distribution unit of resource,CharacterString,Geocell,M (TDF only),/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:unitsOfDistribution[1]/gco:CharacterString[1]",
(char *)"107,transferSize,Transfer size of resource (Megabytes),Real,Float value,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:transferSize[1]/gco:Real[1]",
(char *)"108,linkage,Location for on-line access using a Uniform Resource Locator address ,URL,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:onLine[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"109,name,Name of online resource,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:onLine[1]/gmd:CI_OnlineResource[1]/gmd:name[1]/gco:CharacterString[1]",
(char *)"110,description,Brief description of online resource,CharacterString,Free text character string,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:onLine[1]/gmd:CI_OnlineResource[1]/gmd:description[1]/gco:CharacterString[1]",
(char *)"111,function,Description of the utility of the online resource,CI_OnLineFunctionCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_OnLineFunctionCode,M,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:onLine[1]/gmd:CI_OnlineResource[1]/gmd:function[1]/gmd:CI_OnLineFunctionCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = download,,/gmd:MD_Metadata[1]/gmd:distributionInfo[1]/gmd:MD_Distribution[1]/gmd:transferOptions[1]/gmd:MD_DigitalTransferOptions[1]/gmd:onLine[1]/gmd:CI_OnlineResource[1]/gmd:function[1]/gmd:CI_OnLineFunctionCode[1]/@codeListValue",
(char *)"112,level,The specific data to which the data quality information applies,MD_ScopeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_ScopeCode,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:scope[1]/gmd:DQ_Scope[1]/gmd:level[1]/gmd:MD_ScopeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = dataset,,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:scope[1]/gmd:DQ_Scope[1]/gmd:level[1]/gmd:MD_ScopeCode[1]/@codeListValue",
(char *)"113,nameOfMeasure,Name of the test applied to the data,CharacterString,Coverage Completeness,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_CompletenessCommission[1]/gmd:nameOfMeasure[1]/gco:CharacterString[1]",
(char *)"114,valueUnit,Attribute identifier for base unit type,UnitDefinition Attribute Value(s),gml:id = percent,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_CompletenessCommission[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/gml:UnitDefinition[1]/@gml:id",
(char *)"115,UnitDefinition,Identifier and codespace for the referencing object,CharacterString ,percent,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_CompletenessCommission[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/gml:UnitDefinition[1]/gml:identifier[1]",
(char *)"116,UnitDefinition,Identifier and codespace for the referencing object,Attribute Value(s),codeSpace = urn:ogc:def:uom:OGC:percent,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_CompletenessCommission[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/gml:UnitDefinition[1]/gml:identifier[1]/@codeSpace",
(char *)"117,catalogSymbol,Symbol for the measure,CharacterString,%,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_CompletenessCommission[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/gml:UnitDefinition[1]/gml:catalogSymbol[1]",
(char *)"118,Record,Resulting value of the test,Real,Real Value,M,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_CompletenessCommission[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Real[1]",
(char *)"119.1,nameOfMeasure,Name of the test applied to the data,CharacterString,Number of ICESat Validation Points,M,\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of ICESat Validation Points\"\"]\"",
(char *)"148.1,measureDescription,Brief description of the test applied,CharacterString,\"Where available, ICESat laser altimeter points have been used to calculate statistical vertical accuracy measures. Automatic filtering processes restrict the availability of the ICESat points to flat and open terrain and the points are distributed very inhomogeneously. The measures provided might therefore not be representative for the whole geocell.\",M (TDF only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of ICESat Validation Points\"\"]/../../gmd:measureDescription/gco:CharacterString\"",
(char *)"120.1,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,M,\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of ICESat Validation Points\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"121.1,Record,Quantitative value,Integer,Integer Value,M,\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of ICESat Validation Points\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer\"",
(char *)"119.2,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to ICESat LE90,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat LE90\"\"]\"",
(char *)"120.2,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat LE90\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.2,Record,Quantitative value,Real,Real Value,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat LE90\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.3,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to ICESat Vertical Mean,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical Mean\"\"]\"",
(char *)"120.3,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical Mean\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.3,Record,Quantitative value,Real,Real Value,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical Mean\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.4,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to ICESat Vertical Standard Deviation,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical Standard Deviation\"\"]\"",
(char *)"120.4,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical Standard Deviation\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.4,Record,Quantitative value,Real,Real Value,C / (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical Standard Deviation\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.5,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to ICESat Vertical RMSE,C / (TDF only) (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical RMSE\"\"]\"",
(char *)"120.5,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (TDF only) (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical RMSE\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.5,Record,Quantitative value,Real,Real Value,C / (TDF only) (if 121.1 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to ICESat Vertical RMSE\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.6,nameOfMeasure,Name of the test applied to the data,CharacterString,Number of GCP Validation Points,M (TDF only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of GCP Validation Points\"\"]\"",
(char *)"148.6,measureDescription,Brief description of the test applied,CharacterString,\"Where available, independent GCPs from other sources have been used to calculate statistical vertical accuracy measures of the TREx DEM Finished geocell.\",M (TDF only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of GCP Validation Points\"\"]/../../gmd:measureDescription/gco:CharacterString\"",
(char *)"120.6,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,M (TDF only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of GCP Validation Points\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"121.6,Record,Quantitative value,Integer,Integer Value,M (TDF only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Number of GCP Validation Points\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer\"",
(char *)"119.7,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to GCP LE90,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP LE90\"\"]\"",
(char *)"120.7,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP LE90\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.7,Record,Quantitative value,Real,Real Value,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP LE90\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.8,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to GCP Vertical Mean,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical Mean\"\"]\"",
(char *)"120.8,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical Mean\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.8,Record,Quantitative value,Real,Real Value,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical Mean\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.9,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to GCP Vertical Standard Deviation,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical Standard Deviation\"\"]\"",
(char *)"120.9,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical Standard Deviation\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.9,Record,Quantitative value,Real,Real Value,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical Standard Deviation\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"119.1,nameOfMeasure,Name of the test applied to the data,CharacterString,Difference to GCP Vertical RMSE,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical RMSE\"\"]\"",
(char *)"120.1,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical RMSE\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href\"",
(char *)"121.1,Record,Quantitative value,Real,Real Value,C / (TDF only) (if 121.6 ? 30),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Difference to GCP Vertical RMSE\"\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real\"",
(char *)"122.1,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = north,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/@id\"",
(char *)"123.1,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching North,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.1,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the northern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.1,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the northern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.1,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.1,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.1,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.1,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.1,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.1,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.1,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.1,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.1 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.1,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.1 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"north\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.2,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = northeast,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/@id\"",
(char *)"123.2,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching North East,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.2,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the north eastern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.2,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the north eastern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.2,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.2,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.2,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.2,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.2,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.2,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.2,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.2,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.2 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.2,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.2 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northeast\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.3,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = east,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/@id\"",
(char *)"123.3,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching East,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.3,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the eastern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.3,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the eastern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.3,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.3,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.3,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.3,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.3,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.3,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.3,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.3,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.3 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.3,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.3 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"east\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.4,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = southeast,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/@id\"",
(char *)"123.4,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching South East,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.4,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the south eastern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.4,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the south eastern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.4,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.4,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.4,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.4,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.4,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.4,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.4,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.4,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.4 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.4,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.4 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southeast\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.5,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = south,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/@id\"",
(char *)"123.5,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching South,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.5,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the southern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.5,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the southern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.5,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.5,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.5,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.5,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.5,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.5,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.5,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.5,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.5 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.5,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.5 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"south\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.6,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = southwest,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/@id\"",
(char *)"123.6,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching South West,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.6,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the south western neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.6,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the south western neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.6,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.6,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.6,date,date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.6,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.6,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.6,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.6,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.6,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.6 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.6,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.6 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"southwest\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.7,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = west,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/@id\"",
(char *)"123.7,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching West,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.7,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the western neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.7,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the western neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.7,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.7,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.7,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.7,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.7,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.7,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.7,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.7,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.7 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.7,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.7 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"west\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"122.8,report,Name of tile neighbour,DQ_TopologicalConsistency Attribute Value(s),id = northwest,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/@id\"",
(char *)"123.8,nameOfMeasure,Name of the test applied to the data,CharacterString,Edge Matching North West,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:nameOfMeasure[1]/gco:CharacterString[1]\"",
(char *)"124.8,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the north western neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"124.8,measureDescription,Brief description of the test applied,CharacterString,Describes the edge matching result for the north western neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:measureDescription[1]/gco:CharacterString[1]\"",
(char *)"125.8,title,Name of document where the test is described,CharacterString,TREx DEM Raw Specification,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"125.8,title,Name of document where the test is described,CharacterString,TREx DEM Finished Specification,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]\"",
(char *)"126.8,date,Date identifying the Document,Date,YYYY-MM-DD,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]\"",
(char *)"127.8,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode,M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList\"",
(char *)",,,Attribute Value(s),codeListValue = publication,,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue\"",
(char *)"128.8,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document,M (TDR only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"128.8,explanation,Explanation of the meaning of conformance for this result,CharacterString,See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document,M (TDF only),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]\"",
(char *)"129.8,pass,Indication if neighboring tile was available 0 = not available and 1 = available,Boolean,\"Boolean value (1 = true, 0 = false)\",M,\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]\"",
(char *)"130.8,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,C / (if 129.8 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"131.8,Record,Number of non matching posts,Integer,Integer Value,C / (if 129.8 == 1),\"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"\"northwest\"\"]/gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]\"",
(char *)"132.1,nameOfMeasure,Name of the test applied to the data,CharacterString,Quality Inspection,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Inspection\"\"]\"",
(char *)"133.1,measureDescription,Brief description of the test applied,CharacterString,MCP processor internal quality control result.,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Inspection\"\"]/../../gmd:measureDescription/gco:CharacterString\"",
(char *)"134.1,valueType,Information regarding Quality inspection,RecordType,Quality Inspection Result,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Inspection\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]\"",
(char *)"135.1,valueType,Information regarding Quality inspection,RecordType Attribute Value(s),xlink:href = urn:dgiwg:metadata:dmf:1.0:xmlns:isotc211:gmd:1.0:PT_FreeText_Type,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Inspection\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]/@xlink:href\"",
(char *)"136.1,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Inspection\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"137.1,Record,Resulting value of the test,CharacterString,Free text character string ,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Inspection\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:CharacterString[1]\"",
(char *)"132.2,nameOfMeasure,Name of the test applied to the data,CharacterString,Quality Remark,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Remark\"\"]\"",
(char *)"133.2,measureDescription,Brief description of the test applied,CharacterString,MCP processor quality remark.,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Remark\"\"]/../../gmd:measureDescription/gco:CharacterString\"",
(char *)"134.2,valueType,Information regarding Quality remark,RecordType,Quality Remark Result,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Remark\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]\"",
(char *)"135.2,valueType,Information regarding Quality remark,RecordType Attribute Value(s),xlink:href = urn:dgiwg:metadata:dmf:1.0:xmlns:isotc211:gmd:1.0:PT_FreeText_Type,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Remark\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]/@xlink:href\"",
(char *)"136.2,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/unity,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Remark\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href\"",
(char *)"137.2,Record,Resulting value of the test,CharacterString,Free text character string ,M (TDR only),\"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"\"Quality Remark\"\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:CharacterString[1]\"",
(char *)"138.N,description,A descriptions of the Sourcedata used.,CharacterString,Free text character string (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:description[1]/gco:CharacterString[1]",
(char *)"139.N,title,Title of the Source,CharacterString,Free text character string (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",
(char *)"140.N,date,Date describing the resource,Date,Date (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
(char *)"141.N,dateType,Type of date,CI_DateTypeCode,codeList = ./RESOURCES/19139/20070417/resources/codelist/gmxCodelists.xml#CI_DateTypeCode,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = (As described in Annex D),,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]/@codeListValue",
(char *)"142.N,edition,Version of the sourcedata,CharacterString,Free text character string (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:edition[1]/gco:CharacterString[1]",
(char *)"143.N,organisationName,Name of organisation providing sourcedata,CharacterString,Free text character string (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]",
(char *)"144.N,linkage,Link to the providing organisiation or link to description of the data,URL,Free text character string (As described in Annex D),M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]",
(char *)"145.N,role,Function performed by the responsible party,CI_RoleCode,codeList = ./RESOURCES/19139/20070417/resources/codelist/gmxCodelists.xml#CI_RoleCode,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = publisher,,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]/@codeListValue",
(char *)"149,level,The specific data to which the data quality information applies,MD_ScopeCode,codeList = ./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_ScopeCode,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:scope[1]/gmd:DQ_Scope[1]/gmd:level[1]/gmd:MD_ScopeCode[1]/@codeList",
(char *)",,,Attribute Value(s),codeListValue = collectionSession,,/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:scope[1]/gmd:DQ_Scope[1]/gmd:level[1]/gmd:MD_ScopeCode[1]/@codeListValue",
(char *)"150,MD_ScopeDescription,Description to which the data quality information applies,CharacterString,The accuracy values contained in the reports of this DQ_DataQuality element state the mission requirements of the TanDEM-X Mission and represent predicted maximum design values for the global elevation dataset.,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:scope[1]/gmd:DQ_Scope[1]/gmd:levelDescription[1]/gmd:MD_ScopeDescription[1]/gmd:other[1]/gco:CharacterString[1]",
(char *)"151.1,nameOfMeasure,Name of the test applied to the data,CharacterString,Absolute horizontal accuracy CE90,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_AbsoluteExternalPositionalAccuracy[1]/gmd:nameOfMeasure[1]/gco:CharacterString[1]",
(char *)"152.1,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_AbsoluteExternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href",
(char *)"153.1,Record,Quantitative value,Real,10,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[1]/gmd:DQ_AbsoluteExternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Real[1]",
(char *)"151.2,nameOfMeasure,Name of the test applied to the data,CharacterString,Absolute vertical accuracy LE90,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[2]/gmd:DQ_AbsoluteExternalPositionalAccuracy[1]/gmd:nameOfMeasure[1]/gco:CharacterString[1]",
(char *)"152.2,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[2]/gmd:DQ_AbsoluteExternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href",
(char *)"153.2,Record,Quantitative value,Real,10,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[2]/gmd:DQ_AbsoluteExternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Real[1]",
(char *)"151.3,nameOfMeasure,Name of the test applied to the data,CharacterString,\"Relative vertical accuracy, 90% linear point-to-point error for slope !!! 20%\",M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[3]/gmd:DQ_RelativeInternalPositionalAccuracy[1]/gmd:nameOfMeasure[1]/gco:CharacterString[1]",
(char *)"152.3,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[3]/gmd:DQ_RelativeInternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href",
(char *)"153.3,Record,Quantitative value,Real,2,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[3]/gmd:DQ_RelativeInternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Real[1]",
(char *)"151.4,nameOfMeasure,Name of the test applied to the data,CharacterString,\"Relative vertical accuracy, 90% linear point-to-point error for slope > 20%\",M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[4]/gmd:DQ_RelativeInternalPositionalAccuracy[1]/gmd:nameOfMeasure[1]/gco:CharacterString[1]",
(char *)"152.4,DQ_QuantitativeResult,Value unit for reporting a data quality result,valueUnit Attribute Value(s),xlink:href = http://www.opengis.net/def/uom/OGC/1.0/metre,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[4]/gmd:DQ_RelativeInternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href",
(char *)"153.4,Record,Quantitative value,Real,4,M (TDF only),/gmd:MD_Metadata[1]/gmd:dataQualityInfo[2]/gmd:DQ_DataQuality[1]/gmd:report[4]/gmd:DQ_RelativeInternalPositionalAccuracy[1]/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Real[1]",
(char *)"146,useLimitation,Information regarding the limitations of resource usage ,CharacterString,\"For use limitations, see TREx MoU\",M,/gmd:MD_Metadata[1]/gmd:metadataConstraints[1]/gmd:MD_Constraints[1]/gmd:useLimitation[1]/gco:CharacterString[1]"
};











int num_mgcp_trd2_codes = 224;

const char *MGCP_TRD2_FCODES[224*3] = 
{
  "A","Aerodrome","GB005",
  "P","Aeronautical NAVAID","GA035",
  "P","Air Obstruction","GB220",
  "L","Aircraft Revetment","GB050",
  "P","Airport Navigation Lights","GB010",
  "A","Amusement Park","AK030",
  "A","Apron","GB015",
  "L","Aqueduct","BH010",
  "P","Aqueduct","BH010",
  "A","Archeological Site","AL012",
  "P","Archeological Site","AL012",
  "A","Beach","BA050",
  "P","Beach","BA050",
  "A","Berthing Structure","BB190",
  "L","Berthing Structure","BB190",
  "A","Bog","BH015",
  "A","Breakwater","BB041",
  "L","Breakwater","BB041",
  "L","Bridge","AQ040",
  "L","Bridge Span","AQ045",
  "A","Building","AL015",
  "P","Building","AL015",
  "A","Built-Up Area","AL020",
  "P","Built-Up Area","AL020",
  "L","Cableway","AT041",
  "P","Cairn","AL025",
  "A","Camp-site","AK060",
  "A","Canal","BH020",
  "L","Canal","BH020",
  "A","Cane","EC010",
  "L","Cart Track","AP010",
  "P","Catalytic Cracker","AC020",
  "L","Causeway","AQ064",
  "P","Cave","DB030",
  "A","Cemetery","AL030",
  "P","Cemetery","AL030",
  "P","Checkpoint","AH070",
  "A","Circular Irrigation System","AJ010",
  "P","Cistern","BI010",
  "A","Cleared Way","EC040",
  "L","Communication Line","AT060",
  "A","Communication Station","AT050",
  "P","Communication Tower","AT080",
  "P","Control Tower","AQ060",
  "L","Conveyor","AF020",
  "P","Cooling Tower","AF030",
  "P","Crane","AF040",
  "A","Crop Land","EA010",
  "L","Cut","DB070",
  "A","Dam","BI020",
  "L","Dam","BI020",
  "P","Dam","BI020",
  "P","Dish Aerial","AT010",
  "A","Disposal Site","AB000",
  "A","Ditch","BH030",
  "L","Ditch","BH030",
  "A","Dry Dock","BB090",
  "A","Embankment","DB090",
  "L","Embankment","DB090",
  "P","Entrance and/or Exit","AQ090",
  "A","Extraction Mine","AA010",
  "P","Extraction Mine","AA010",
  "A","Fairground","AK090",
  "L","Fence","AL070",
  "L","Ferry Crossing","AQ070",
  "P","Ferry Crossing","AQ070",
  "P","Flare Pipe","AF070",
  "L","Ford","BH070",
  "P","Ford","BH070",
  "A","Forest Clearing","EC060",
  "A","Fortification","AH050",
  "P","Fortification","AH050",
  "P","Gantry","AL080",
  "L","Geologic Fault","DB110",
  "A","Geothermal Outlet","DB115",
  "P","Geothermal Outlet","DB115",
  "A","Glacier","BJ030",
  "A","Golf Course","AK100",
  "A","Grain Elevator","AM030",
  "P","Grain Elevator","AM030",
  "A","Grain Storage Structure","AM020",
  "P","Grain Storage Structure","AM020",
  "A","Grassland","EB010",
  "A","Groin","BB043",
  "L","Groin","BB043",
  "A","Gully","DB200",
  "L","Gully","DB200",
  "A","Harbour","BB005",
  "A","Harbour Waters","BB008",
  "P","Hazardous Rock","BD130",
  "A","Heating Facility","AD050",
  "P","Heating Facility","AD050",
  "L","Hedgerow","EA020",
  "P","Helipad","GB030",
  "A","Heliport","GB035",
  "A","Holding Pen","AJ030",
  "A","Hydrocarbons Field","AA052",
  "L","Ice Cliff","BJ040",
  "P","Ice Peak","BJ060",
  "L","Ice Route","AQ075",
  "A","Island","BA030",
  "A","Lake","BH080",
  "A","Land Subject to Inundation","BH090",
  "L","Land Water Boundary","BA010",
  "P","Launch Pad","GB040",
  "P","Light Vessel","BC070",
  "P","Lighthouse","BC050",
  "A","Lock","BI030",
  "L","Lock","BI030",
  "P","Lock","BI030",
  "A","Marine Farm","BH050",
  "P","Maritime Signal Station","BB155",
  "A","Marsh","ED010",
  "P","Memorial Monument","AL130",
  "A","Military Installation","SU001",
  "P","Military Installation","SU001",
  "A","Mineral Pile","AM040",
  "P","Mineral Pile","AM040",
  "P","Mooring Mast","AQ110",
  "A","Moraine","BJ020",
  "P","Mountain Pass","DB150",
  "P","Named Location","ZD040",
  "P","Natural Pool","BH170",
  "P","Non-communication Tower","AL240",
  "A","Oasis","EC020",
  "P","Offshore Platform","BD110",
  "A","Park","AK120",
  "A","Particle Accelerator","AL140",
  "L","Penstock","BH110",
  "L","Pipeline","AQ113",
  "L","Power Line","AT030",
  "A","Power Station","AD010",
  "P","Power Station","AD010",
  "A","Power Substation","AD030",
  "P","Power Substation","AD030",
  "A","Processing Facility","AC000",
  "P","Processing Facility","AC000",
  "A","Public Square","AL170",
  "A","Pumping Station","AQ116",
  "P","Pumping Station","AQ116",
  "A","Quarry","AA012",
  "P","Quarry","AA012",
  "L","Racetrack","AK130",
  "P","Radar Station","AT045",
  "L","Railway","AN010",
  "L","Railway Sidetrack","AN050",
  "P","Railway Turntable","AN075",
  "A","Railway Yard","AN060",
  "A","Rapids","BH120",
  "L","Rapids","BH120",
  "P","Rapids","BH120",
  "A","Recreational Pier","AK190",
  "L","Recreational Pier","AK190",
  "A","Recycling Site","AB010",
  "A","Reservoir","BH130",
  "A","Rice Field","BH135",
  "P","Rig","AA040",
  "A","River","BH140",
  "L","River","BH140",
  "L","Road","AP030",
  "P","Road Interchange","AP020",
  "A","Roadside Rest Area","AQ135",
  "A","Rock Formation","DB160",
  "P","Rock Formation","DB160",
  "A","Ruins","AL200",
  "A","Runway","GB055",
  "A","Sabkha","BH160",
  "A","Salt Evaporator","BH155",
  "A","Sand Dunes","DB170",
  "P","Seaplane Base","GB065",
  "L","Seawall","BB230",
  "A","Settlement","AL105",
  "P","Settlement","AL105",
  "A","Settling Pond","AC030",
  "A","Slipway","BB240",
  "L","Sluice Gate","BI040",
  "P","Sluice Gate","BI040",
  "P","Smokestack","AF010",
  "A","Snow Field and/or Ice-field","BJ100",
  "A","Soil Surface Region","DA010",
  "P","Solar Panel","AD020",
  "A","Stadium","AK160",
  "P","Stadium","AK160",
  "L","Steep Terrain Face","DB010",
  "A","Stopway","GB045",
  "A","Storage Depot","AM010",
  "A","Storage Tank","AM070",
  "P","Storage Tank","AM070",
  "A","Structural Pile","BD100",
  "P","Structural Pile","BD100",
  "A","Surface Bunker","AM060",
  "P","Surface Bunker","AM060",
  "A","Swamp","ED020",
  "A","Taxiway","GB075",
  "A","Test Site","FA100",
  "A","Thicket","EB020",
  "A","Tidal Water","BA040",
  "L","Trail","AP050",
  "A","Training Wall","BB140",
  "L","Training Wall","BB140",
  "A","Transportation Station","AQ125",
  "P","Transportation Station","AQ125",
  "A","Tundra","BJ110",
  "L","Tunnel","AQ130",
  "P","Vanishing Point","BH145",
  "A","Vehicle Lot","AQ140",
  "A","Void Collection Area","ZD020",
  "A","Volcano","DB180",
  "P","Volcano","DB180",
  "L","Wall","AL260",
  "L","Water Gate","BI041",
  "P","Water Gate","BI041",
  "P","Water Intake Tower","BI050",
  "P","Water Tower","AM080",
  "A","Water Treatment Bed","BH040",
  "L","Waterfall","BH180",
  "P","Waterfall","BH180",
  "P","Well","AA050",
  "P","Windmill","AJ050",
  "P","Windmotor","AJ051",
  "A","Wood","EC030",
  "L","Wood","EC030",
  "P","Wood","EC030",
  "A","Zoo","AK180"
};



int num_mgcp_trd3_codes = 239;

const char *MGCP_TRD3_FCODES[239*3] =
{
  "A","Aerodrome","GB005",
  "P","Aeronautical NAVAID","GA035",
  "P","Air Obstruction","GB220",
  "L","Aircraft Revetment","GB050",
  "P","Airport Navigation Lights","GB010",
  "A","Amusement Park","AK030",
  "A","Apron","GB015",
  "L","Aqueduct","BH010",
  "P","Aqueduct","BH010",
  "A","Archeological Site","AL012",
  "P","Archeological Site","AL012",
  "A","Beach","BA050",
  "P","Beach","BA050",
  "A","Berthing Structure","BB190",
  "L","Berthing Structure","BB190",
  "A","Bog","BH015",
  "A","Breakwater","BB041",
  "L","Breakwater","BB041",
  "L","Bridge","AQ040",
  "L","Bridge Span","AQ045",
  "A","Building","AL015",
  "P","Building","AL015",
  "A","Built-Up Area","AL020",
  "P","Built-Up Area","AL020",
  "L","Cableway","AT041",
  "P","Cairn","AL025",
  "A","Camp-site","AK060",
  "A","Canal","BH020",
  "L","Canal","BH020",
  "A","Cane","EC010",
  "L","Cart Track","AP010",
  "P","Catalytic Cracker","AC020",
  "L","Causeway","AQ064",
  "P","Cave","DB030",
  "A","Cemetery","AL030",
  "P","Cemetery","AL030",
  "P","Checkpoint","AH070",
  "A","Circular Irrigation System","AJ010",
  "P","Cistern","BI010",
  "A","Cleared Way","EC040",
  "L","Communication Line","AT060",
  "A","Communication Station","AT050",
  "P","Communication Tower","AT080",
  "P","Control Tower","AQ060",
  "L","Conveyor","AF020",
  "P","Cooling Tower","AF030",
  "P","Crane","AF040",
  "A","Crevasse","BJ031",
  "L","Crevasse","BJ031",
  "A","Crevice","DB061",
  "L","Crevice","DB061",
  "A","Crop Land","EA010",
  "P","Culvert","AQ065",
  "L","Cut","DB070",
  "L","Cut Line","DB071",
  "A","Dam","BI020",
  "L","Dam","BI020",
  "P","Dam","BI020",
  "P","Dish Aerial","AT010",
  "A","Disposal Site","AB000",
  "A","Ditch","BH030",
  "L","Ditch","BH030",
  "A","Dry Dock","BB090",
  "A","Embankment","DB090",
  "L","Embankment","DB090",
  "P","Entrance and/or Exit","AQ090",
  "L","Esker","DB100",
  "A","Extraction Mine","AA010",
  "P","Extraction Mine","AA010",
  "A","Fairground","AK090",
  "L","Fence","AL070",
  "L","Ferry Crossing","AQ070",
  "P","Ferry Crossing","AQ070",
  "A","Firing Range","FA015",
  "P","Flare Pipe","AF070",
  "L","Flume","BH060",
  "L","Ford","BH070",
  "P","Ford","BH070",
  "A","Forest Clearing","EC060",
  "A","Fortification","AH050",
  "P","Fortification","AH050",
  "P","Gantry","AL080",
  "L","Geologic Fault","DB110",
  "L","Geophysical Prospecting Grid","FA090",
  "A","Geothermal Outlet","DB115",
  "P","Geothermal Outlet","DB115",
  "A","Glacier","BJ030",
  "A","Golf Course","AK100",
  "A","Grain Elevator","AM030",
  "P","Grain Elevator","AM030",
  "A","Grain Storage Structure","AM020",
  "P","Grain Storage Structure","AM020",
  "A","Grassland","EB010",
  "A","Groin","BB043",
  "L","Groin","BB043",
  "A","Gully","DB200",
  "L","Gully","DB200",
  "A","Harbour","BB005",
  "A","Harbour Waters","BB008",
  "P","Hazardous Rock","BD130",
  "A","Heating Facility","AD050",
  "P","Heating Facility","AD050",
  "L","Hedgerow","EA020",
  "P","Helipad","GB030",
  "A","Heliport","GB035",
  "A","Holding Pen","AJ030",
  "P","Hut","AL100",
  "A","Hydrocarbons Field","AA052",
  "L","Ice Cliff","BJ040",
  "P","Ice Peak","BJ060",
  "L","Ice Route","AQ075",
  "A","Island","BA030",
  "A","Lake","BH080",
  "A","Land Subject to Inundation","BH090",
  "L","Land Water Boundary","BA010",
  "P","Launch Pad","GB040",
  "P","Lighthouse","BC050",
  "A","Lock","BI030",
  "L","Lock","BI030",
  "P","Lock","BI030",
  "A","Marine Farm","BH050",
  "P","Maritime Signal Station","BB155",
  "A","Marsh","ED010",
  "P","Memorial Monument","AL130",
  "A","Military Installation","SU001",
  "P","Military Installation","SU001",
  "A","Mineral Pile","AM040",
  "P","Mineral Pile","AM040",
  "P","Mooring Mast","AQ110",
  "A","Moraine","BJ020",
  "P","Mountain Pass","DB150",
  "P","Named Location","ZD040",
  "P","Natural Pool","BH170",
  "P","Non-communication Tower","AL240",
  "A","Oasis","EC020",
  "P","Offshore Platform","BD110",
  "A","Park","AK120",
  "A","Particle Accelerator","AL140",
  "L","Penstock","BH110",
  "L","Pipeline","AQ113",
  "L","Power Line","AT030",
  "A","Power Station","AD010",
  "P","Power Station","AD010",
  "A","Power Substation","AD030",
  "P","Power Substation","AD030",
  "A","Processing Facility","AC000",
  "P","Processing Facility","AC000",
  "L","Protection Shed","AL210",
  "P","Protection Shed","AL210",
  "A","Public Square","AL170",
  "A","Pumping Station","AQ116",
  "P","Pumping Station","AQ116",
  "P","Pylon","AT042",
  "A","Quarry","AA012",
  "P","Quarry","AA012",
  "L","Racetrack","AK130",
  "P","Radar Station","AT045",
  "L","Railway","AN010",
  "L","Railway Sidetrack","AN050",
  "P","Railway Turntable","AN075",
  "A","Railway Yard","AN060",
  "A","Rapids","BH120",
  "L","Rapids","BH120",
  "P","Rapids","BH120",
  "A","Recreational Pier","AK190",
  "L","Recreational Pier","AK190",
  "A","Recycling Site","AB010",
  "A","Reservoir","BH130",
  "A","Rice Field","BH135",
  "P","Rig","AA040",
  "A","River","BH140",
  "L","River","BH140",
  "L","Road","AP030",
  "P","Road Interchange","AP020",
  "A","Roadside Rest Area","AQ135",
  "A","Rock Formation","DB160",
  "P","Rock Formation","DB160",
  "A","Ruins","AL200",
  "A","Runway","GB055",
  "A","Sabkha","BH160",
  "A","Salt Evaporator","BH155",
  "A","Sand Dunes","DB170",
  "P","Seaplane Base","GB065",
  "L","Seawall","BB230",
  "A","Settlement","AL105",
  "P","Settlement","AL105",
  "A","Settling Pond","AC030",
  "A","Slipway","BB240",
  "L","Sluice Gate","BI040",
  "P","Sluice Gate","BI040",
  "P","Smokestack","AF010",
  "A","Snow Field and/or Ice-field","BJ100",
  "A","Soil Surface Region","DA010",
  "P","Solar Panel","AD020",
  "A","Sports Ground","AK040",
  "P","Sports Ground","AK040",
  "A","Stadium","AK160",
  "P","Stadium","AK160",
  "L","Steep Terrain Face","DB010",
  "A","Stopway","GB045",
  "A","Storage Depot","AM010",
  "A","Storage Tank","AM070",
  "P","Storage Tank","AM070",
  "A","Structural Pile","BD100",
  "P","Structural Pile","BD100",
  "A","Surface Bunker","AM060",
  "P","Surface Bunker","AM060",
  "A","Swamp","ED020",
  "A","Taxiway","GB075",
  "A","Test Site","FA100",
  "A","Thicket","EB020",
  "A","Tidal Water","BA040",
  "L","Trail","AP050",
  "A","Training Wall","BB140",
  "L","Training Wall","BB140",
  "A","Transportation Station","AQ125",
  "P","Transportation Station","AQ125",
  "A","Tundra","BJ110",
  "L","Tunnel","AQ130",
  "P","Vanishing Point","BH145",
  "A","Vehicle Lot","AQ140",
  "A","Void Collection Area","ZD020",
  "A","Volcano","DB180",
  "P","Volcano","DB180",
  "L","Wall","AL260",
  "L","Water Gate","BI041",
  "P","Water Gate","BI041",
  "P","Water Intake Tower","BI050",
  "P","Water Tower","AM080",
  "A","Water Treatment Bed","BH040",
  "L","Waterfall","BH180",
  "P","Waterfall","BH180",
  "P","Well","AA050",
  "P","Windmill","AJ050",
  "P","Windmotor","AJ051",
  "A","Wood","EC030",
  "L","Wood","EC030",
  "P","Wood","EC030",
  "A","Zoo","AK180"
};



int num_mgcp_trd4_codes = 267;

const char *MGCP_TRD4_FCODES[267*3] = 
{  
  "P","Aeronautical Obstacle","GB220",
  "P","Aeronautical Radio Navigation Service","GA034",
  "A","Aircraft Hangar","GB230",
  "P","Aircraft Hangar","GB230",
  "L","Aircraft Revetment","GB050",
  "P","Aircraft Revetment","GB050",
  "A","Amusement Park","AK030",
  "P","Approach Lighting System","GB485",
  "A","Apron","GB015",
  "A","Aqueduct","BH010",
  "L","Aqueduct","BH010",
  "P","Aqueduct","BH010",
  "A","Archeological Site","AL012",
  "P","Archeological Site","AL012",
  "A","Beach","BA050",
  "P","Beach","BA050",
  "A","Berthing Structure","BB190",
  "L","Berthing Structure","BB190",
  "A","Bog","BH015",
  "A","Breakwater","BB041",
  "L","Breakwater","BB041",
  "A","Bridge","AQ040",
  "L","Bridge","AQ040",
  "A","Built-Up Area","AL020",
  "P","Built-Up Area","AL020",
  "L","Cableway","AT041",
  "P","Cairn","AL025",
  "A","Camp","AI030",
  "A","Camp-site","AK060",
  "A","Canal","BH020",
  "L","Canal","BH020",
  "A","Cane","EC010",
  "L","Cart Track","AP010",
  "P","Catalytic Cracker","AC020",
  "L","Causeway Structure","AQ063",
  "P","Cave Mouth","DB029",
  "A","Cemetery","AL030",
  "P","Cemetery","AL030",
  "P","Checkpoint","AH070",
  "A","Circular Irrigation System","AJ010",
  "P","Cistern","BI010",
  "A","Cleared Way","EC040",
  "L","Communication Line","AT060",
  "A","Communication Station","AT050",
  "L","Conveyor","AF020",
  "P","Crane","AF040",
  "A","Crevasse","BJ031",
  "L","Crevasse","BJ031",
  "A","Crevice","DB061",
  "L","Crevice","DB061",
  "A","Crop Land","EA010",
  "P","Culvert","AQ065",
  "L","Cut","DB070",
  "L","Cut Line","DB071",
  "A","Dam","BI020",
  "L","Dam","BI020",
  "P","Dam","BI020",
  "P","Dish Aerial","AT010",
  "A","Disposal Site","AB000",
  "A","Ditch","BH030",
  "L","Ditch","BH030",
  "A","Dragon's Teeth","AL060",
  "L","Dragon's Teeth","AL060",
  "A","Dry Dock","BB090",
  "A","Electric Power Station","AD010",
  "P","Electric Power Station","AD010",
  "A","Embankment","DB090",
  "L","Embankment","DB090",
  "L","Engineered Earthwork","AH025",
  "P","Entrance and/or Exit","AQ090",
  "L","Esker","DB100",
  "A","Extraction Mine","AA010",
  "P","Extraction Mine","AA010",
  "A","Facility","AL010",
  "A","Fairground","AK090",
  "L","Fence","AL070",
  "L","Ferry Crossing","AQ070",
  "P","Ferry Crossing","AQ070",
  "A","Firing Range","FA015",
  "A","Fish Farm Facility","BH051",
  "P","Flare Pipe","AF070",
  "L","Flume","BH060",
  "L","Ford","BH070",
  "P","Ford","BH070",
  "A","Forest Clearing","EC060",
  "A","Fortification","AH050",
  "P","Fortification","AH050",
  "A","General Building","AL015",
  "P","General Building","AL015",
  "L","Geologic Fault","DB110",
  "L","Geophysical Prospecting Grid","FA090",
  "A","Geothermal Outlet","DB115",
  "P","Geothermal Outlet","DB115",
  "A","Glacier","BJ030",
  "A","Golf Course","AK100",
  "A","Grain Elevator","AM030",
  "P","Grain Elevator","AM030",
  "A","Grain Storage Structure","AM020",
  "P","Grain Storage Structure","AM020",
  "A","Grassland","EB010",
  "A","Greenhouse","AJ110",
  "P","Greenhouse","AJ110",
  "A","Groin","BB043",
  "L","Groin","BB043",
  "A","Gully","DB200",
  "L","Gully","DB200",
  "A","Harbour","BB005",
  "P","Hazardous Rock","BD130",
  "A","Heating Facility","AD050",
  "P","Heating Facility","AD050",
  "L","Hedgerow","EA020",
  "P","Helipad","GB030",
  "A","Heliport","GB035",
  "A","Holding Pen","AJ030",
  "P","Holding Pen","AJ030",
  "A","Hop Field","EA055",
  "P","Hut","AL099",
  "A","Hydrocarbons Field","AA052",
  "L","Ice Cliff","BJ040",
  "P","Ice Peak","BJ060",
  "L","Ice Route","AQ075",
  "A","Island","BA030",
  "A","Lake","BH080",
  "A","Land Aerodrome","GB005",
  "A","Land Subject to Inundation","BH090",
  "L","Land Water Boundary","BA010",
  "P","Launch Pad","GB040",
  "A","Lock","BI030",
  "L","Lock","BI030",
  "P","Lock","BI030",
  "A","Mangrove Swamp","ED030",
  "A","Mariculture Site","BH050",
  "P","Maritime Signal Station","BB155",
  "A","Marsh","ED010",
  "P","Memorial Monument","AL130",
  "A","Military Installation","SU001",
  "P","Military Installation","SU001",
  "A","Mineral Pile","AM040",
  "P","Mineral Pile","AM040",
  "P","Mooring Mast","AQ110",
  "A","Moraine","BJ020",
  "P","Mountain Pass","DB150",
  "P","Named Location","ZD040",
  "P","Natural Pool","BH170",
  "A","Oasis","EC020",
  "P","Offshore Platform","BD110",
  "A","Orchard","EA040",
  "A","Park","AK120",
  "A","Particle Accelerator","AL140",
  "L","Penstock","BH110",
  "L","Pipeline","AQ113",
  "L","Power Line","AT030",
  "A","Power Substation","AD030",
  "P","Power Substation","AD030",
  "A","Processing Facility","AC000",
  "P","Processing Facility","AC000",
  "L","Protection Shed","AL210",
  "P","Protection Shed","AL210",
  "A","Public Square","AL170",
  "A","Pumping Station","AQ116",
  "P","Pumping Station","AQ116",
  "P","Pylon","AT042",
  "A","Quarry","AA012",
  "P","Quarry","AA012",
  "L","Racetrack","AK130",
  "P","Radar Station","AT045",
  "L","Railway","AN010",
  "L","Railway Sidetrack","AN050",
  "P","Railway Turntable","AN075",
  "A","Railway Yard","AN060",
  "A","Rapids","BH120",
  "L","Rapids","BH120",
  "P","Rapids","BH120",
  "A","Recreational Pier","AK190",
  "L","Recreational Pier","AK190",
  "A","Recycling Site","AB010",
  "A","Reef","BD120",
  "L","Reef","BD120",
  "A","Reservoir","BH130",
  "A","Rice Field","BH135",
  "P","Rig","AA040",
  "A","River","BH140",
  "L","River","BH140",
  "L","Road","AP030",
  "A","Roadside Rest Area","AQ135",
  "A","Rock Formation","DB160",
  "L","Rock Formation","DB160",
  "P","Rock Formation","DB160",
  "A","Roundhouse","AN076",
  "P","Roundhouse","AN076",
  "A","Ruins","AL200",
  "A","Runway","GB055",
  "A","Sabkha","BH160",
  "A","Salt Evaporator","BH155",
  "A","Salt Flat","BH150",
  "A","Sand Dunes","DB170",
  "L","Seawall","BB230",
  "A","Settlement","AL105",
  "P","Settlement","AL105",
  "A","Settling Pond","AC030",
  "P","Settling Pond","AC030",
  "A","Shanty Town","AL208",
  "A","Shed","AL019",
  "P","Shed","AL019",
  "L","Ski-jump","AK150",
  "P","Ski-jump","AK150",
  "A","Slipway","BB240",
  "L","Sluice Gate","BI040",
  "P","Sluice Gate","BI040",
  "P","Smokestack","AF010",
  "A","Snow Field and/or Ice-field","BJ100",
  "A","Soil Surface Region","DA010",
  "P","Solar Panel","AD020",
  "A","Spillway","BH165",
  "L","Spillway","BH165",
  "A","Sports Ground","AK040",
  "P","Sports Ground","AK040",
  "A","Stadium","AK160",
  "P","Stadium","AK160",
  "L","Steep Terrain Face","DB010",
  "A","Stopway","GB045",
  "A","Storage Depot","AM010",
  "A","Storage Tank","AM070",
  "P","Storage Tank","AM070",
  "A","Structural Pile","BD100",
  "P","Structural Pile","BD100",
  "A","Surface Bunker","AM060",
  "P","Surface Bunker","AM060",
  "A","Swamp","ED020",
  "A","Swimming Pool","AK170",
  "P","Swimming Pool","AK170",
  "A","Taxiway","GB075",
  "A","Test Site","FA100",
  "A","Thicket","EB020",
  "A","Tidal Water","BA040",
  "P","Tower","AL241",
  "L","Trail","AP050",
  "A","Training Wall","BB140",
  "L","Training Wall","BB140",
  "A","Transportation Station","AQ125",
  "P","Transportation Station","AQ125",
  "A","Tundra","BJ110",
  "A","Tunnel","AQ130",
  "L","Tunnel","AQ130",
  "P","Vanishing Point","BH145",
  "A","Vehicle Lot","AQ140",
  "A","Vineyard","EA050",
  "A","Void Collection Area","ZD020",
  "A","Volcano","DB180",
  "P","Volcano","DB180",
  "L","Wall","AL260",
  "A","Water Aerodrome","GB065",
  "P","Water Aerodrome","GB065",
  "L","Water Gate","BI041",
  "P","Water Gate","BI041",
  "P","Water Intake Tower","BI050",
  "A","Water Treatment Bed","BH040",
  "L","Waterfall","BH180",
  "P","Waterfall","BH180",
  "P","Well","AA050",
  "P","Wind Turbine","AJ051",
  "P","Windmill","AJ050",
  "A","Wood","EC030",
  "L","Wood","EC030",
  "P","Wood","EC030",
  "P","Wreck","BD180",
  "A","Zoo","AK180"
};

const char *FixedStringsTRD2[24*2] =
{
   
   "MGCP_Cell *** has *** MD_Metadata *** hierarchyLevelName *** CharacterString",
   "Cell",

    
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString",
   "Limited Distribution (limdis)",

   
   "MGCP_Cell *** has *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString",
   "Limited Distribution (limdis)",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString",
   "Limited Distribution (limdis)",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** hierarchyLevelName *** CharacterString",
   "Subregion",


   

   
   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** nameOfMeasure *** CharacterString",
   "GAIT",




   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** series *** CI_Series *** name *** CharacterString",
   "MGCP",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** name *** CharacterString",
   "SHAPEFILE",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** version *** CharacterString",
   "July 1998",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** specification *** CharacterString",
   "ESRI Shapefile Technical Description - An ESRI White Paper",

   

   "MGCP_Cell *** has *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** title *** Anchor",
   "MGCP Feature Catalogue",


   
   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** explanation *** CharacterString",
   "geometry conformant to specification",


   
   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** pass *** Boolean",
   "true",


   "MGCP_Cell *** dataFile *** MX_DataFile *** fileType *** MimeFileType",
   "ESRI SHAPEFILE (.SHP, .DBF, .SHX)",


   "MGCP_Cell *** has *** MD_Metadata *** contact *** CI_ResponsibleParty *** role *** CI_RoleCode",
   "originator",

   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** pointOfContact *** CI_ResponsibleParty *** role *** CI_RoleCode",
   "originator",

   "MGCP_Cell *** has *** MD_Metadata *** hierarchyLevel *** MD_ScopeCode",
   "dataset",

   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** scope *** DQ_Scope *** level *** MD_ScopeCode",
   "dataset",

   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   "MGCP_Cell *** has *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   

   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
   "creation",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
   "publication",

   
   "MGCP_Cell *** has *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
   "publication"
};



const char *FixedStringsTRD3[22*2]=
{
   
   "MGCP_Cell *** has *** MD_Metadata *** hierarchyLevelName *** CharacterString",
   "Cell",

    
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString",
   "Limited Distribution (limdis)",

   
   "MGCP_Cell *** has *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString",
   "Limited Distribution (limdis)",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString",
   "Limited Distribution (limdis)",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** hierarchyLevelName *** CharacterString",
   "Subregion",



   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** nameOfMeasure *** CharacterString",
   "GAIT",




   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** series *** CI_Series *** name *** CharacterString",
   "MGCP",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** name *** CharacterString",
   "SHAPEFILE",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** version *** CharacterString",
   "July 1998",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** specification *** CharacterString",
   "ESRI Shapefile Technical Description - An ESRI White Paper",



   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** explanation *** CharacterString",
   "geometry conformant to specification",


   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** pass *** Boolean",
   "true",


   "MGCP_Cell *** dataFile *** MX_DataFile *** fileType *** MimeFileType",
   "ESRI SHAPEFILE (.SHP, .DBF, .SHX)",


   
   "MGCP_Cell *** has *** MD_Metadata *** contact *** CI_ResponsibleParty *** role *** CI_RoleCode",
   "originator",

   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** pointOfContact *** CI_ResponsibleParty *** role *** CI_RoleCode",
   "originator",

   "MGCP_Cell *** has *** MD_Metadata *** hierarchyLevel *** MD_ScopeCode",
   "dataset",

   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** scope *** DQ_Scope *** level *** MD_ScopeCode",
   "dataset",

   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   "MGCP_Cell *** has *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   

    "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
	"creation",

    "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
	"publication"
};



const char *FixedStringsTRD4[20*2]=
{
   
   "MGCP_Cell *** has *** MD_Metadata *** hierarchyLevelName *** CharacterString",
   "Cell",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** hierarchyLevelName *** CharacterString",
   "Subregion",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** nameOfMeasure *** CharacterString",
   "GAIT",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** series *** CI_Series *** name *** CharacterString",
   "MGCP",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** name *** CharacterString",
   "SHAPEFILE",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** version *** CharacterString",
   "July 1998",

   
   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceFormat *** MD_Format *** specification *** CharacterString",
   "ESRI Shapefile Technical Description - An ESRI White Paper",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** explanation *** CharacterString",
   "geometry conformant to specification",

   
   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** pass *** Boolean",
   "true",

   "MGCP_Cell *** dataFile *** MX_DataFile *** fileType *** MimeFileType",
   "ESRI SHAPEFILE (.SHP, .DBF, .SHX)",

   
   "MGCP_Cell *** has *** MD_Metadata *** contact *** CI_ResponsibleParty *** role *** CI_RoleCode",
   "originator",

   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** pointOfContact *** CI_ResponsibleParty *** role *** CI_RoleCode",
   "originator",

   "MGCP_Cell *** has *** MD_Metadata *** hierarchyLevel *** MD_ScopeCode",
   "dataset",

   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** scope *** DQ_Scope *** level *** MD_ScopeCode",
   "dataset",

   "MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   "MGCP_Cell *** has *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** useLimitation *** CharacterString",
   "Military Classification",

   

   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
   "creation",

   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** date *** CI_Date *** dateType *** CI_DateTypeCode",
   "publication",

   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_LegalConstraints *** otherConstraints *** CharacterString",
   "SENTINEL1"

};


const char *zeropointer = "0";
const char *onepointer = "1";

const char *ParticipantNationCodes2[28*3] = 
 {
   "AUS", "Australia", "1",
   "BEL", "Belgium", "1",
   "BGR", "Bulgaria", "1",
   "CAN", "Canada", "1",
   "CZE", "Czech Republic", "1",
   "DNK", "Denmark", "1",
   "EST", "Estonia", "1",
   "FIN", "Finland", "1",
   "FRA", "France", "1",
   "DEU", "Germany", "1",
   "GRC", "Greece", "1",
   "HUN", "Hungary", "1",
   "ITA", "Italy", "1",
   "LVA", "Latvia", "1",
   "LTU", "Lithuania", "1",
   "MDA", "Moldova", "1",
   "NLD", "Netherlands", "1",
   "NZL", "New Zealand", "1",
   "NOR", "Norway", "1",
   "POL", "Poland", "1",
   "PRT", "Portugal", "1",
   "ROU", "Romania", "1",
   "SVK", "Slovakia", "1",
   "ESP", "Spain", "1",
   "SWE", "Sweden", "1",
   "TUR", "Turkey", "1",
   "GBR", "United Kingdom", "1",
   "USA", "United States",  "1"
 };

const char *ParticipantNationCodes3[28*3] = 
 {
   "AUS", "Australia", "1",
   "BEL", "Belgium", "1",
   "BGR", "Bulgaria", "1",
   "CAN", "Canada", "1",
   "CZE", "Czech Republic", "1",
   "DNK", "Denmark", "1",
   "EST", "Estonia", "1",
   "FIN", "Finland", "1",
   "FRA", "France", "1",
   "DEU", "Germany", "1",
   "GRC", "Greece", "1",
   "HUN", "Hungary", "1",
   "ITA", "Italy", "1",
   "LVA", "Latvia", "1",
   "LTU", "Lithuania", "1",
   "MDA", "Moldova", "1",
   "NLD", "Netherlands", "1",
   "NZL", "New Zealand", "1",
   "NOR", "Norway", "1",
   "POL", "Poland", "1",
   "PRT", "Portugal", "1",
   "ROU", "Romania", "1",
   "SVK", "Slovakia", "1",
   "ESP", "Spain", "1",
   "SWE", "Sweden", "1",
   "TUR", "Turkey", "1",
   "GBR", "United Kingdom", "1",
   "USA", "United States",  "1"
 };

 int ParticipantNationCodesMax2 = 28;
 int ParticipantNationCodesMax3 = 28;



extern "C" const char *ParticipantNationCodes4[32*3] = 
 {
    "AUS" , "Australia" , "1",
    "BEL" , "Belgium" , "1",
    "BGR" , "Bulgaria" , "1",
    "CAN" , "Canada" , "1",
    "HRV" , "Croatia" , "1",
    "CZE" , "Czech Republic" , "1",
    "DNK" , "Denmark" , "1",
    "EST" , "Estonia" , "1",
    "FIN" , "Finland" , "1",
    "FRA" , "France" , "1",
    "DEU" , "Germany" , "1",
    "GRC" , "Greece" , "1",
    "HUN" , "Hungary" , "1",
    "ITA" , "Italy" , "1",
    "JPN" , "Japan" , "1",
    "KOR" , "Korea" , "1",
    "LVA" , "Latvia" , "1",
    "LTU" , "Lithuania" , "1",
    "MDA" , "Moldova" , "1",
    "NLD" , "Netherlands" , "1",
    "NZL" , "New Zealand" , "1",
    "NOR" , "Norway" , "1",
    "POL" , "Poland" , "1",
    "PRT" , "Portugal" , "1",
    "ROU" , "Romania" , "1",
    "SVK" , "Slovakia" , "1",
    "ZAF" , "South Africa" , "1",
    "ESP" , "Spain" , "1",
    "SWE" , "Sweden" , "1",
    "TUR" , "Turkey" , "1",
    "GBR" , "United Kingdom" , "1",
    "USA" , "United States" , "1"
   };

int ParticipantNationCodesMax4 = 32;



const char *ParticipantNationCodes41[32*3] = 
 {
    "AUS" , "Australia" , "1",
    "BEL" , "Belgium" , "1",
    "BGR" , "Bulgaria" , "1",
    "CAN" , "Canada" , "1",
    "HRV" , "Croatia" , "1",
    "CZE" , "Czech Republic" , "1",
    "DNK" , "Denmark" , "1",
    "EST" , "Estonia" , "1",
    "FIN" , "Finland" , "1",
    "FRA" , "France" , "1",
    "DEU" , "Germany" , "1",
    "GRC" , "Greece" , "1",
    "HUN" , "Hungary" , "1",
    "ITA" , "Italy" , "1",
    "JPN" , "Japan" , "1",
    "KOR" , "Korea" , "1",
    "LVA" , "Latvia" , "1",
    "LTU" , "Lithuania" , "1",
    "MDA" , "Moldova" , "1",
    "NLD" , "Netherlands" , "1",
    "NZL" , "New Zealand" , "1",
    "NOR" , "Norway" , "1",
    "POL" , "Poland" , "1",
    "PRT" , "Portugal" , "1",
    "ROU" , "Romania" , "1",
    "SVK" , "Slovakia" , "1",
    "ZAF" , "South Africa" , "1",
    "ESP" , "Spain" , "1",
    "SWE" , "Sweden" , "1",
    "TUR" , "Turkey" , "1",
    "GBR" , "United Kingdom" , "1",
    "USA" , "United States" , "1"
   };

int ParticipantNationCodesMax41 = 32;




 const char *ParticipantAgencyCodes3[28*3] = 
 	{
      "DIGO",       "Defence Imagery and Geospatial Organisation", "1",
      "BELGS",      "Belgium Geographic Service", "1",
      "BULMTS",     "Military Topographic Service", "1",
      "MCE",        "Mapping and Charting Establishment", "1",
      "GSCAF",      "Geographic Service of the Czech Armed Forces", "1",
      "DALO",       "The Danish Defence Acquisition and Logistics Organization", "1",
      "MGG",        "Military Geographic Group", "1",
      "TopK",       "Topgraphic Service of Finnish Defence Forces", "1",
      "EMABGI",     "Etat-Major des Arm?es / Bureau G?ographique Interarm?es", "1",
      "BGIO",       "Bundeswehr Geoinformation Office", "1",
      "HMGS",       "Hellenic Military Geographic Service", "1",
      "GEOS_HDF",   "Geoinformation Service of the Hungarian Defence Forces", "1",
      "DGS-II_DIV", "Defence General Staff", "1",
      "LGIA",       "Latvian Geospatial Information Agency", "1",
      "MMC",        "Military Mapping Center of Lithuanian National Defence Forces", "1",
      "TSM",        "Topographic Service of Moldova", "1",
      "RNLAGA",     "Royal Netherlands Army Geographic Agency", "1",
      "GIO",        "New Zealand Defence Force Geospatial Intelligence Organisation", "1",
      "FMGT",       "Norwegian Military Geographic Service", "1",
      "STSZRP",     "S?u?ba Topograficzna Si? Zbrojnych Rzeczpospolitej Polskiej", "1",
      "IGEOE",      "Instituto Geogr?fico do Ex?rcito", "1",
      "MTD",        "Military Topographic Directorate", "1",
      "TOPU",       "Topografick? ?stav", "1",
      "SCC",        "Cartographic Coordination Service", "1",
      "SWAF",       "Swedish Armed Forces", "1",
      "GCM",        "General Command of Mapping", "1",
      "DGC_ICG",    "Defence Geographic Centre, Intelligence Collection Group", "1",
      "NGA",        "National Geospatial-Intelligence Agency",  "1"
  };


const char *ParticipantAgencyCodes2[28*3] = 
 	{
"DIGO", "Defence Imagery and Geospatial Organisation", "1",
"BELGS", "Belgium Geographic Service", "1",
"BULMTS", "Military Topographic Service", "1",
"MCE", "Mapping and Charting Establishment", "1",
"gscaf", "Geographic Service of the Czech Armed Forces (GeoS&A?R)", "1",
"DALO", "The Danish Defence Acquisition and Logistics Organization", "1",
"MGG", "Military Geographic Group", "1",
"TopK", "Topgraphic Service of Finnish Defence Forces", "1",
"emabgi", "Etat-Major des Arm?es / Bureau G?ographique Interarm?es (EMA/BGI)", "1",
"bgio", "Bundeswehr Geoinformation Office", "1",
"hmgs", "Hellenic Military Geographic Service", "1",
"MSHDF", "Mapping Service of the Hungarian Defence Forces", "1",

"DGS-II_DIV", "Defence General Staff", "1",
"LGIA", "Latvian Geospatial Information Agency", "1",
"MMC", "Military Mapping Center of Lithuanian National Defence Forces", "1",
"tsm", "Topographic Service of Moldova", "1",
"RNLAGA", "Royal Netherlands Army Geographic Agency", "1",
"JGSF", "Joint Geospatial Support Facility", "1",
"FMGT", "Norwegian Military Geographic Service", "1",
"ZGW_SG_WP", "Zarz?d Geografii Wojskowej Sztabu Generalnego Wojska Polskiego", "1",
"IGeoE", "Instituto Geogr?fico do Ex?rcito", "1",
"mtd", "Military Topographic Directorate", "1",
"topu", "Topografict? ?stav (TOPU)", "1",
"CEGET", "Spanish Army Geographic Centre", "1",
"swaf", "Swedish Armed Forces", "1",
"gcm", "General Command of Mapping (GCM)", "1",
"DGCDGI", "Defence Geographic Centre Defence Geospatial Intelligence", "1",
"nga", "National Geospatial-Intelligence Agency", "1"

  };

int ParticipantAgencyCodesMax2 = 28;
int ParticipantAgencyCodesMax3 = 28;




  const char *ParticipantAgencyCodes4[32*3] = 
  {
   "DIGO"      ,  "Defence Imagery and Geospatial Organisation" ,  "1", 
   "BELGS"     ,  "Belgium Geographic Service" ,  "1", 
   "BULMTS"    ,  "Military Topographic Service" ,  "1", 
   "MCE"       ,  "Mapping and Charting Establishment" ,  "1", 
   "MGD"       ,  "Military Geographic Divsion" ,  "1",  
   "GSCAF"     ,  "Geographic Service of the Czech Armed Forces " ,  "1",  
   "DALO"      ,  "The Danish Defence Acquisition and Logistics Organization" ,  "1", 
   "MGG"       ,  "Military Geographic Group" ,  "1", 
   "TOPK"      ,  "Topographic Service of Finnish Defence Forces" ,  "1", 
   "EMA_BGHOM" ,  "Etat-Major des Arm?es / Bureau G?ographie, Hydrographie, Oc?anographie et M?t?orologie" ,  "1", 
   "BGIO"      ,  "Bundeswehr Geoinformation Office" ,  "1", 
   "HMGS"      ,  "Hellenic Military Geographic Service" ,  "1", 
   "GEOS_HDF"  ,  "Geoinformation Service of the Hungarian Defence Forces" ,  "1", 
   "DGS-II_DIV",  "Defense General Staff" ,  "1", 
   "DGI_DIH"   ,  "Directorate for Geospatial Intelligence, Defense Intelligence Headquarters" ,  "1", 
   "KDIA"      ,  "Korean Defense Intelligence Agency" ,  "1", 
   "LGIA"      ,  "Latvian Geospatial Information Agency" ,  "1", 
   "MMC"       ,  "Military Mapping Center of Lithuanian National Defence Forces" ,  "1", 
   "TSM"       ,  "Topographic Service of Moldova" ,  "1", 
   "RNLAGA"    ,  "Royal Netherlands Army Geographic Agency" ,  "1", 
   "GIO"       ,  "New Zealand Defence Force Geospatial Intelligence Organisation" ,  "1", 
   "FMGT"      ,  "Norwegian Military Geographic Service" ,  "1", 
   "STSZRP"    ,  "S?u?ba Topograficzna Si? Zbrojnych Rzeczpospolitej Polskiej" ,  "1", 
   "IGEOE"     ,  "Instituto Geogr?fico do Ex?rcito" ,  "1", 
   "MTD"       ,  "Military Topographic Directorate" ,  "1", 
   "TOPU"      ,  "Topografick? ?stav " ,  "1", 
   "DGI"       ,  "Directorate Geospatial Information" ,  "1", 
   "SCC"       ,  "Cartographic Coordination Service" ,  "1", 
   "SWAF"      ,  "Swedish Armed Forces" ,  "1", 
   "GCM"       ,  "General Command of Mapping " ,  "1", 
   "DGC_JFIG"  ,  "Defence Geographic Centre, Joint Forces Intelligence Group" ,  "1", 
   "NGA"       ,  "National Geospatial-Intelligence Agency",                      "1"
};

int ParticipantAgencyCodesMax4 = 32;




const char *ParticipantAgencyCodes41[32*3] = 
{
 "AGO","Australian Geospatial-Intelligence Organisation","1", 
 "BELGS","Belgian Defense Geographic Service","1", 
 "BULMTS","Military Topographic Service","1", 
 "MCE","Mapping and Charting Establishment","1", 
 "MGD","Military Geographic Division","1", 
 "GSCAF","Geographic Service of the Czech Armed Forces","1", 
 "DALO","The Danish Defence Acquisition and Logistics Organization","1", 
 "MGG","Military Geographic Group","1", 
 "TOPK","Topographic Service of Finnish Defence Forces","1", 
 "EMA_BGHOM","Etat-Major des Arm?es / Bureau G?ographie, Hydrographie, Oc?anographie et M?t?orologie","1", 
 "BGIO","Bundeswehr Geoinformation Office","1", 
 "HMGS","Hellenic Military Geographic Service","1", 
 "GEOS_HDF","Geoinformation Service of the Hungarian Defence Forces","1", 
 "DGS-II_DIV","Defence General Staff","1", 
 "DGI_DIH","Directorate for Geospatial Intelligence, Defense Intelligence Headquarters","1", 
 "KDIA","Korean Defense Intelligence Agency","1", 
 "LGIA","Latvian Geospatial Information Agency","1", 
 "MMC","Military Mapping Center of Lithuanian National Defence Forces","1", 
 "TSM","Topographic Service of Moldova","1", 
 "RNLAGA","Royal Netherlands Army Geographic Agency","1", 
 "GNZ","New Zealand Defence Force GEOINT New Zealand","1", 
 "FMGT","Norwegian Military Geographic Service","1", 
 "SGeoW","Szefostwo Geografii Wojskowej","1", 
 "IGEOE","Instituto Geogr?fico do Ex?rcito","1", 
 "MTD","Military Topographic Directorate","1", 
 "TOPU","Topografick? ?stav","1", 
 "DGI","Directorate Geospatial Information","1", 
 "SCC","Cartographic Coordination Service","1", 
 "SWAF","Swedish Armed Forces","1", 
 "GCM","General Command of Mapping","1", 
 "DGC_JFIG","Defence Geographic Centre, Joint Forces Intelligence Group","1", 
 "NGA","National Geospatial-Intelligence Agency", "1" 
};


int ParticipantAgencyCodesMax41 = 32;




const char *ParticipantAgencyCodes42[32*3] = 
{
 "AGO","Australian Geospatial-Intelligence Organisation","1", 
 "BELGS","Belgian Defense Geographic Service","1", 
 "BULMTS","Military Topographic Service","1", 
 "MCE","Mapping and Charting Establishment","1", 
 "MGD","Military Geographic Division","1", 
 "GSCAF","Geographic Service of the Czech Armed Forces","1", 
 "DALO","The Danish Defence Acquisition and Logistics Organization","1", 
 "MGG","Military Geographic Group","1", 
 "TOPK","Topographic Service of Finnish Defence Forces","1", 
 "EMA_BGHOM","Etat-Major des Arm?es / Bureau G?ographie, Hydrographie, Oc?anographie et M?t?orologie","1", 
 "BGIC","Bundeswehr Geoinformation Centre","1", 
 "HMGS","Hellenic Military Geographic Service","1", 
 "GEOS_HDF","Geoinformation Service of the Hungarian Defence Forces","1", 
 "DGS-II_DIV","Defence General Staff","1", 
 "DGI_DIH","Directorate for Geospatial Intelligence, Defense Intelligence Headquarters","1", 
 "KDIA","Korean Defense Intelligence Agency","1", 
 "LGIA","Latvian Geospatial Information Agency","1", 
 "MMC","Military Mapping Center of Lithuanian National Defence Forces","1", 
 "TSM","Topographic Service of Moldova","1", 
 "RNLAGA","Royal Netherlands Army Geographic Agency","1", 
 "GNZ","GEOINT New Zealand","1", 
 "FMGT","Norwegian Military Geographic Service","1", 
 "SGeoW","Szefostwo Geografii Wojskowej","1", 
 "IGEOE","Instituto Geogr?fico do Ex?rcito","1", 
 "MTD","Military Topographic Directorate","1", 
 "TOPU","Topografick? ?stav","1", 
 "DGI","Directorate Geospatial Information","1", 
 "SCC","Cartographic Coordination Service","1", 
 "SWAF","Swedish Armed Forces","1", 
 "GCM","General Command of Mapping","1", 
 "DGC_JFIG","Defence Geographic Centre, Joint Forces Intelligence Group","1", 
 "NGA","National Geospatial-Intelligence Agency", "1" 
};


int ParticipantAgencyCodesMax42 = 32;



const char *ParticipantAgencyCodes43[32*3] = 
{
 "AGO",       "Australian Geospatial-Intelligence Organisation","1", 
 "BELGS",     "Belgian Defense Geographic Service","1", 
 "BULMTS",    "Military Topographic Service","1", 
 "MCE",       "Mapping and Charting Establishment","1", 
 "MGD",       "Military Geographic Division","1", 
 "GSCAF",     "Geographic Service of the Czech Armed Forces","1", 
 "DALO",      "The Danish Defence Acquisition and Logistics Organization","1", 
 "MGG",       "Military Geographic Group","1", 
 "TOPK",      "Topographic Service of Finnish Defence Forces","1", 
 "EMA_BGHOM", "Etat-Major des Arm?es / Bureau G?ographie, Hydrographie, Oc?anographie et M?t?orologie","1", 
 "BGIC",      "Bundeswehr Geoinformation Centre","1", 
 "HMGS",      "Hellenic Military Geographic Service","1", 
 "GEOS_HDF",  "Geoinformation Service of the Hungarian Defence Forces","1", 
 "DGS-II_DIV","Defence General Staff","1", 
 "DGI_DIH",   "Directorate for Geospatial Intelligence, Defense Intelligence Headquarters","1", 
 "KDIA",      "Korean Defense Intelligence Agency","1", 
 "LGIA",      "Latvian Geospatial Information Agency","1", 
 "MMC",       "Military Mapping Center of Lithuanian National Defence Forces","1", 
 "TSM",       "Topographic Service of Moldova","1", 
 "DGA",       "Defence Geographic Agency","1", 
 "GNZ",       "GEOINT New Zealand","1", 
 "FMGT",      "Norwegian Military Geographic Service","1", 
 "SGeoW",     "Szefostwo Geografii Wojskowej","1", 
 "IGEOE",     "Instituto Geogr?fico do Ex?rcito","1", 
 "MTD",       "Military Topographic Directorate","1", 
 "TOPU",      "Topografick? ?stav","1", 
 "DGI",       "Directorate Geospatial Information","1", 
 "SCC",       "Cartographic Coordination Service","1", 
 "SWAF",      "Swedish Armed Forces","1", 
 "GCM",       "General Command of Mapping","1", 
 "DGC_JFIG",  "Defence Geographic Centre, Joint Forces Intelligence Group","1", 
 "NGA",       "National Geospatial-Intelligence Agency", "1" 
};


int ParticipantAgencyCodesMax43 = 32;




const char *ParticipantAgencyCodes44[32*3] = 
{
 "AGO",       "Australian Geospatial-Intelligence Organisation","1", 
 "BELGS",     "Belgian Defense Geographic Service","1", 
 "BULMTS",    "Military Topographic Service","1", 
 "MCE",       "Mapping and Charting Establishment","1", 
 "MoD_HRV",   "Ministry of Defence of the Republic of Croatia","1", 
 "GSCAF",     "Geographic Service of the Czech Armed Forces","1", 
 "DALO",      "The Danish Defence Acquisition and Logistics Organization","1", 
 "MGG",       "Military Geographic Group","1", 
 "TOPK",      "Topographic Service of Finnish Defence Forces","1", 
 "EMA_BGHOM", "Etat-Major des Arm?es / Bureau G?ographie, Hydrographie, Oc?anographie et M?t?orologie","1", 
 "BGIC",      "Bundeswehr Geoinformation Centre","1", 
 "HMGS",      "Hellenic Military Geographic Service","1", 
 "GEOS_HDF",  "Geoinformation Service of the Hungarian Defence Forces","1", 
 "DGS-II_DIV","Defence General Staff","1", 
 "DGI_DIH",   "Directorate for Geospatial Intelligence, Defense Intelligence Headquarters","1", 
 "KDIA",      "Korean Defense Intelligence Agency","1", 
 "LGIA",      "Latvian Geospatial Information Agency","1", 
 "MMC",       "Military Mapping Center of Lithuanian National Defence Forces","1", 
 "TSM",       "Topographic Service of Moldova","1", 
 "DGA",       "Defence Geographic Agency","1", 
 "GNZ",       "GEOINT New Zealand","1", 
 "FMGT",      "Norwegian Military Geographic Service","1", 
 "SGeoW",     "Szefostwo Geografii Wojskowej","1", 
 "CIGEOE",    "Centro de Informa?o Geoespacial do Ex?rcito","1", 
 "MTD",       "Military Topographic Directorate","1", 
 "TOPU",      "Topografick? ?stav","1", 
 "DGI",       "Directorate Geospatial Information","1", 
 "SCC",       "Cartographic Coordination Service","1", 
 "SWAF",      "Swedish Armed Forces","1", 
 "GCM",       "General Command of Mapping","1", 
 "DGC_JFIG",  "Defence Geographic Centre, Joint Forces Intelligence Group","1", 
 "NGA",       "National Geospatial-Intelligence Agency", "1" 
};


int ParticipantAgencyCodesMax44 = 32;



















const char *RoleCodesTRD2[1*3] = 
{ 
   "originator",            "originator", "1"
};
int RoleCodes2Max = 1;


const char *RoleCodesTRD3[2*3] = 
 	{
      "originator",            "originator", "1",
      "processor",             "processor", "1"
  };
int RoleCodes3Max = 2;

const char *RoleCodesTRD4[2*3] = 
 	{
      "originator",            "originator", "1",
      "processor",             "processor", "1"
  };
int RoleCodes4Max = 2;


const char *ObjectTypeCodes[3*3] = 
  {
    "point",    "point", "1",
    "curve",    "curve", "1",
    "surface",  "surface", "1"
  }; 
int ObjectTypeCodesMax = 3;




const char *TopicCodes[9] = 
  {
    "farming",
    "biota",
    "inlandWaters",
	"location",
	"oceans",
	"society",
	"structure",
	"transportation",
	"utilitiesCommunication"
  };
int NumTopicCodes = 9;

int TopicCodesFound[9];




const char *DateTypeCodes[3*3] = 
  {
    "creation",    "creation", "1",
    "publication", "publication", "1",
    "revision",    "revision", "1"
  }; 
int DateTypeCodesMax = 3;

const char *ClassificationCodes[1*3] = 
  {
    "unclassified",    "unclassified", "1"
  };
int ClassificationCodesMax = 1;

const char *RestrictionCodes[1*3] = 
  {
    "copyright",    "copyright", "1"
  }; 
int RestrictionCodesMax = 1;


const char *SpatialRepresentationTypeCodes[1*3] = 
  {
    "vector",    "vector", "1"
  }; 
int SpatialRepresentationTypeCodesMax = 1;






const char *LanguageCodes[1*3] = 
  {
    "eng",    "English", "1",
  }; 
int LanguageCodesMax = 1;


const char *CharacterSetCodes[1*3] = 
  {
    "utf8",    "UTF-8", "1"
  }; 
int CharacterSetCodesMax = 1;

const char *ScopeCodes1[1*3] = 
  {
    "tile",    "tile", "1"
  }; 
int ScopeCodes1Max = 1;


const char *ScopeCodes2[1*3] = 
  {
    "dataset",    "dataset", "1"
  }; 
int ScopeCodes2Max = 1;



const char *AccuracyEvaluationMethods23[22*3] = 
  {
    "UNK",    "Unknown", "1",
    "ACA",    "Geodetic Survey Control - adequate sample", "1",
    "ACS",    "Geodetic Survey Control - small sample", "1",
    "ANA",    "Photogrammetric Control - adequate sample", "1",
    "ANS",    "Photogrammetric Control - small sample", "1",
    "EMC",    "Product specification accuracy value (assumed adequate)", "1",
    "FZD",    "Evaluation deferred (no measurement)", "1",
    "NA",     "Not applicable", "1",

    "ACA",    "ACA: Geodetic Survey Adequate", "1",
    "ACS",    "ACS: Geodetic Survey Small", "1",
    "ANA",    "ANA: Photogrammetric GDAS Adequate", "1",
    "ANS",    "ANS: Photogrammetric GDAS Small", "1",
    "EMC",    "EMC: Product Specification", "1",
    "FZD",    "FZD: Evaluation Deferred", "1",

    "UNK",    "The attribute value is unknown.", "1",
    "ACA",    "Geodetic survey control - adequate sample.", "1",
    "ACS",    "Geodetic survey control - small sample.", "1",
    "ANA",    "Photogrammetric control (GDAS II) - adequate sample.", "1",
    "ANS",    "Photogrammetric control (GDAS II) - small sample.", "1",
    "EMC",    "Product specification accuracy value - assumed adequate sample.", "1",
    "FZD",    "Evaluation deferred (no measurement).", "1",
    "NA",     "There is no possible value in the attribute range that would be applicable. ( May occur when the attribute is not applicable to the feature type (for example: the Airfield Type attribute of a Settlement feature type). )",  "1"
  };
int AccuracyEvaluationMethodsMax23 = 22;


const char *AccuracyEvaluationMethods4[8*3] = 
{
  "UNK"                    , "Unknown" , "1",
  "geodeticSurveyAdequate" , "Geodetic Survey Adequate" , "1",
  "geodeticSurveySmall"    , "Geodetic Survey Small" , "1",
  "photogrammGdasAdequate" , "Photogrammetric GDAS Adequate" , "1",
  "photogrammGdasSmall"    , "Photogrammetric GDAS Small" , "1",
  "productSpecification"   , "Product Specification" , "1",
  "evaluationDeferred"     , "Evaluation Deferred" , "1",
  "N_A"                    , "Not Applicable" , "1"
};
int AccuracyEvaluationMethodsMax4 = 8;



const char *AccuracyEvaluationMethods41[8*3] = 
{
 "UNK","Unknown", "1",
 "geodeticSurveyAdequate","Geodetic Survey Adequate", "1",
 "geodeticSurveySmall","Geodetic Survey Small", "1",
 "photogrammGdasAdequate","Photogrammetric GDAS Adequate", "1",
 "photogrammGdasSmall","Photogrammetric GDAS Small", "1",
 "productSpecification","Product Specification", "1",
 "evaluationDeferred","Evaluation Deferred", "1",
 "N_A","Not Applicable", "1"
};
int AccuracyEvaluationMethodsMax41 = 8;



const char *SourceTypeIds2[207*3] = 
  {
      "unk",                        "Source is not known", "1",
      "adrg",                       "Arc Digitized Raster Graphic", "1",
      "aafif",                      "Automated Aeronautical Facilities Information File", "1",
      "chum",                       "Chart Update Manual", "1",
      "cit",                        "City Graphic", "1",
      "combat",                     "Combat Chart", "1",
      "cadrg",                      "Compressed Arc Digitized Raster Graphic" ,  "1",
      "cib1",                       "Controlled Imagery Base 1 Metre" ,  "1",
      "cib5",                       "Controlled Imagery Base 5 Metre" ,  "1",
      "qbrd",                       "QuickBird Imagery" ,  "1",
      "dnc",                        "Digital Nautical Chart" ,  "1",
      "dpf",                        "Digital Print File" ,  "1",
      "dtop1",                      "Digital Topographic Data 1" ,  "1",
      "dtop2",                      "Digital Topographic Data 2" ,  "1",
      "dtop",                       "Digital Topographic Data 3" ,  "1",
      "dtop4",                      "Digital Topographic Data 4" ,  "1",
      "dtop",                       "Digital Topographic Data 5" ,  "1",
      "dvof",                       "Digital Vertical Obstruction File" ,  "1",
      "ffd",                        "Foundation Feature Data" ,  "1",
      "cover",                      "LandCover" ,  "1",
      "names",                      "GeoNames" ,  "1",
      "field",                      "GPS Field Collected Open Source" ,  "1",
      "icm",                        "Image City Map" ,  "1",
      "ikonos",                     "IKONOS Satellite Imagery" ,  "1",
      "ntm",                        "NTM Imagery" ,  "1",
      "imag",                       "Imagery" ,  "1",
      "itd",                        "ITD" ,  "1",
      "ivd",                        "Interim Vector Data" ,  "1",
      "ibd",                        "International Boundaries Database" ,  "1",
      "jog",                        "Joint Operational Graphic" ,  "1",
      "landsat",                    "LANDSAT Satellite Imagery" ,  "1",
      "lwd",                        "Littoral Warfare Data" ,  "1",
      "mcg",                        "Mapping Charting and Geodetic" ,  "1",
      "msd1",                       "Mission Specific Data 1" ,  "1",
      "msd2",                       "Mission Specific Data 2" ,  "1",
      "msd3",                       "Mission Specific Data 3" ,  "1",
      "msd4",                       "Mission Specific Data 4" ,  "1",
      "msd5",                       "Mission Specific Data 5" ,  "1",
      "midb",                       "Modernized Integrated Data Base" ,  "1",
      "natdat",                     "Native Data Source" ,  "1",
      "natmap",                     "Native Map Source" ,  "1",
      "med",                        "NGA GIS Medical Facilities Database" ,  "1",
      "nate",                       "Nominally Attributed Topographic Evaluation Map" ,  "1",
      "plan",                       "Planning Graphic" ,  "1",
      "refmap",                     "Reference Map" ,  "1",
      "srtm",                       "Shuttle Radar Topographic Mission" ,  "1",
      "sitmap",                     "Site Map" ,  "1",
      "tod0",                       "Tactical Oceanographic Data Level 0" ,  "1",
      "tod1",                       "Tactical Oceanographic Data Level" ,  "1",
      "tod2",                       "Tactical Oceanographic Data Level 2" ,  "1",
      "tod3",                       "Tactical Oceanographic Data Level 3" ,  "1",
      "tod4",                       "Tactical Oceanographic Data Level 4" ,  "1",
      "tod5",                       "Tactical Oceanographic Data Level 5" ,  "1",
      "tlm",                        "Topographic Line Map" ,  "1",
      "und",                        "United Nations Data" ,  "1",
      "uvmap",                      "Urban Vector Map" ,  "1",
      "vitd",                       "Vector Interim Terrain Data" ,  "1",
      "vmap",                       "Vector Map Level 0" ,  "1",
      "vmap1",                      "Vector Map Level 1" ,  "1",
      "vmap2",                      "Vector Map Level 2" ,  "1",
      "wvspls",                     "World Vector Shoreline Plu" ,  "1",
      "qbrdmm",                     "QuickBird Multispectral Monoscopic Imagery" ,  "1",
      "qbrdms",                     "QuickBird Multispectral Stereoscopic Imagery" ,  "1",
      "qbrdpm",                     "QuickBird Panchromatic Monoscopic Imagery" ,  "1",
      "qbrdps",                     "QuickBird Panchromatic Stereoscopic Imagery" ,  "1",
      "ikonmm",                     "Ikonos Multispectral Monoscopic Imagery" ,  "1",
      "ikonms",                     "Ikonos Multispectral Stereoscopic Imagery" ,  "1",
      "ikonpm",                     "Ikonos Panchromatic Monoscopic Imagery" ,  "1",
      "ikonps",                     "Ikonos Panchromatic Stereoscopic Imagery" ,  "1",
      "spt10",                      "SPOT 10m Imagery" ,  "1",
      "spt5",                       "SPOT 5m Imagery" ,  "1",
      "spt5mm",                     "SPOT 5m Multispectral Monoscopic Imagery" ,  "1",
      "spt5ms",                     "SPOT 5m Multispectral Stereoscopic Imagery" ,  "1",
      "spt5pm",                     "SPOT 5m Panchromatic Monoscopic Imagery" ,  "1",
      "spt5ps",                     "SPOT 5m Panchromatic Stereoscopic Imagery" ,  "1",
      "spt2pm",                     "SPOT 2.5m Panchromatic Monoscopic Imagery" ,  "1",
      "spt2mm",                     "SPOT 2.5m Multispectral Monoscopic Imagery" ,  "1",
      "spt5m",                      "SPOT 5m Monoscopic Imagery" ,  "1",
      "spt5s",                      "SPOT 5m Stereoscopic Imagery" ,  "1",
      "spt10mm",                    "SPOT 10m Multispectral Monoscopic Imagery" ,  "1",
      "spthrg",                     "SPOT HRG Digital Terrain Model" ,  "1",
      "btm2",                       "BGIO 1:25k Thematic Map" ,  "1",
      "btm50",                      "BGIO 1:50k Thematic Ma" ,  "1",
      "btm100",                     "BGIO 1:100k Thematic Map" ,  "1",
      "bto25",                      "BGIO 1:25k Topographic Map" ,  "1",
      "bto50",                      "BGIO 1:50k Topographic Map" ,  "1",
      "bto100",                     "BGIO 1:100k Topographic Map" ,  "1",
      "bvd",                        "BGIO Vector Data" ,  "1",
      "bvd25",                      "BGIO 1:25k Vector Data" ,  "1",
      "bvd50",                      "BGIO 1:50k Vector Data" ,  "1",
      "bvd100",                     "BGIO 1:100k Vector Data" ,  "1",
      "mult",                       "Multiple sources" ,  "1",
      "np",                         "Unpopulated" ,  "1",
      "na",                         "Not Applicable" ,  "1",
      "other",                      "Source not identified" ,  "1",
      "frimag",                     "FR Image Product" ,  "1",
      "frsnsr",                     "FR Defense Sensor" ,  "1",
      "asrp",                       "Arc Standard Raster" ,  "1",
      "gaz",                        "Gazetteer" ,  "1",
      "www",                        "World-Wide Web" ,  "1",
      "orbvu3",                     "OrbView3" ,  "1",
      "vmap2i",                     "FR HR Vector Data (VMAP2i)" ,  "1",
      "city",                       "City Graphic" ,  "1",
      "dtop3",                      "Digital Topographic Data 3" ,  "1",
      "dtop5",                      "Digital Topographic Data 5" ,  "1",
      "tod1",                       "Tactical Oceanographic Data Level 1" ,  "1",
      "vmap0",                      "Vector Map Level 0" ,  "1",
      "wvspls",                     "World Vector Shoreline Plus" ,  "1",
      "btm25",                      "BGIO 1:25k Thematic Map" ,  "1",
      "btm50",                      "BGIO 1:50k Thematic Map" ,  "1",
      "N_P",                        "Unpopulated" ,  "1",
      "N_A",                        "Not Applicable" ,  "1",
      "Unknown",                    "The attribute value is unknown." ,  "1",
      "ADRG",                       "NGA Arc Digitized Raster Graphic." ,  "1",
      "AAFIF",                      "NGA Automated Aeronautical Facilities Information File." ,  "1",
      "CHUM",                       "NGA Chart Update Manual (CHUM)" ,  "1",
      "City Graphic",               "NGA City Graphic" ,  "1",
      "Combat Chart",               "NGA Combat Chart data." ,  "1",
      "CADRG",                      "NGA Compressed Arc Digitized Raster Graphic." ,  "1",
      "CIB1",                       "NGA Controlled Imagery Base 1 metre data." ,  "1",
      "CIB5",                       "NGA Controlled Imagery Base 5 metre data." ,  "1",
      "QuickBird Imagery",          "QuickBird imagery of unspecified type and resolution." ,  "1",
      "DNC",                        "NGA Digital Nautical Chart." ,  "1",
      "DPF",                        "NGA Digital Print File." ,  "1",
      "DTOP 1",                     "NGA Digital Topographic Data Level 1." ,  "1",
      "DTOP 2",                     "NGA Digital Topographic Data Level 2." ,  "1",
      "DTOP 3",                     "NGA Digital Topographic Data Level 3." ,  "1",
      "DTOP 4",                     "NGA Digital Topographic Data Level 4." ,  "1",
      "DTOP 5",                     "NGA Digital Topographic Data Level 5." ,  "1",
      "DVOF",                       "NGA Digital Vertical Obstruction File." ,  "1",
      "FFD",                        "NGA Foundation Feature Data." ,  "1",
      "Land Cover",                 "NGA GeoCover or LandCover." ,  "1",
      "GeoNames",                   "NGA GeoNames." ,  "1",
      "GPS",                        "GPS-based field-collected open source data." ,  "1",
      "ICM",                        "NGA Image City Map (ICM)" ,  "1",
      "IKONOS Imagery",             "Ikonos imagery of unspecified type and resolution." ,  "1",
      "NTM Imagery",                "Imagery from (US) National Technical Means (NTM)." ,  "1",
      "Imagery",                    "Imagery of unspecified type and resolution." ,  "1",
      "ITD",                        "NGA Interim Terrain Data." ,  "1",
      "IVD",                        "NGA Interim Vector Data." ,  "1",
      "International Boundaries",   "NGA International Boundaries database." ,  "1",
      "JOG",                        "NGA Joint Operational Graphic." ,  "1",
      "Landsat Imagery",            "Landsat imagery of unspecified type and resolution." ,  "1",
      "LWD",                        "NGA Littoral Warfare Data." ,  "1",
      "MC&G",                       "Mapping, charting and/or geodetic data." ,  "1",
      "MSD 1",                      "NGA Mission Specific Data Level 1." ,  "1",
      "MSD 2",                      "NGA Mission Specific Data Level 2." ,  "1",
      "MSD 3",                      "NGA Mission Specific Data Level 3." ,  "1",
      "MSD 4",                      "NGA Mission Specific Data Level 4." ,  "1",
      "MSD 5",                      "NGA Mission Specific Data Level 5." ,  "1",
      "MIDB",                       "(US) Modernized Integrated Data Base." ,  "1",
      "Native Data",                "Native data source." ,  "1",
      "Native Map",                 "Native map source." ,  "1",
      "Medical Facilities",         "NGA GIS medical facilities database." ,  "1",
      "NATE",                       "NGA Nominally Attributed Topographic Evaluation map." ,  "1",
      "Planning Graphic",           "NGA Planning Graphic." ,  "1",
      "Reference Map",              "Reference map." ,  "1",
      "SRTM",                       "(US) Shuttle Radar Topographic Mission." ,  "1",
      "Site Map",                   "Site map (for example: at a nuclear facility)." ,  "1",
      "TOD 0",                      "NGA Tactical Oceanographic Data Level 0." ,  "1",
      "TOD 1",                      "NGA Tactical Oceanographic Data Level 1." ,  "1",
      "TOD 2",                      "NGA Tactical Oceanographic Data Level 2." ,  "1",
      "TOD 3",                      "NGA Tactical Oceanographic Data Level 3." ,  "1",
      "TOD 4",                      "NGA Tactical Oceanographic Data Level 4." ,  "1",
      "TOD 5",                      "NGA Tactical Oceanographic Data Level 5." ,  "1",
      "TLM",                        "NGA Topographic Line Map." ,  "1",
      "UN Data",                    "United Nations data." ,  "1",
      "UVMap",                      "NGA Urban Vector Map." ,  "1",
      "VITD",                       "NGA Vector Interim Terrain Data." ,  "1",
      "VMap 0",                     "NGA Vector Map Level 0." ,  "1",
      "VMap 1",                     "NGA Vector Map Level 1." ,  "1",
      "VMap 2",                     "NGA Vector Map Level 2." ,  "1",
      "WVS Plus",                   "NGA World Vector Shoreline Plus." ,  "1",
      "QuickBird Multispectral Monoscopic Imagery",  "QuickBird multispectral monoscopic imagery with nominal 2.44 metre resolution." ,  "1",
      "QuickBird Multispectral Stereoscopic Imagery",  "QuickBird multispectral stereoscopic imagery with nominal 2.44 metre resolution." ,  "1",
      "QuickBird Panchromatic Monoscopic Imagery",  "QuickBird panchromatic monoscopic imagery with nominal 60 centimetre resolution." ,  "1",
      "QuickBird Panchromatic Stereoscopic Imagery",  "QuickBird panchromatic stereoscopic imagery with nominal 60 centimetre resolution." ,  "1",
      "Ikonos Multispectral Monoscopic Imagery",  "Ikonos multispectral monoscopic imagery with nominal 4 metre resolution." ,  "1",
      "Ikonos Multispectral Stereoscopic Imagery",  "Ikonos multispectral stereoscopic imagery with nominal 4 metre resolution." ,  "1",
      "Ikonos Panchromatic Monoscopic Imagery",  "Ikonos panchromatic monoscopic imagery with nominal 1 metre resolution." ,  "1",
      "Ikonos Panchromatic Stereoscopic Imagery",  "Ikonos panchromatic stereoscopic imagery with nominal 1 metre resolution." ,  "1",
      "SPOT 10m Imagery",           "SPOT 4 imagery of unspecified type with nominal 10 metre resolution." ,  "1",
      "SPOT 5m Imagery",            "SPOT 5 imagery of unspecified type with nominal 5 metre resolution." ,  "1",
      "SPOT 5m Multispectral Monoscopic Imagery",  "SPOT 5 multispectral monoscopic imagery with nominal 5 metre resolution." ,  "1",
      "SPOT 5m Multispectral Stereoscopic Imagery",  "SPOT 5 multispectral stereoscopic imagery with nominal 5 metre resolution." ,  "1",
      "SPOT 5m Panchromatic Monoscopic Imagery",  "SPOT 5 panchromatic monoscopic imagery with nominal 5 metre resolution." ,  "1",
      "SPOT 5m Panchromatic Stereoscopic Imagery",  "SPOT 5 panchromatic stereoscopic imagery with nominal 5 metre resolution." ,  "1",
      "SPOT 2.5m Panchromatic Monoscopic Imagery",  "SPOT 5: High Resolution Geometric (HRG) THR 'supermode' imagery with nominal 2.5 metre resolution." ,  "1",
      "SPOT 2.5m Multispectral Monoscopic Imagery",  "SPOT 5: High Resolution Geometric (HRG) THX 'supermode' multispectral imagery with nominal 2.5 metre resolution." ,  "1",
      "SPOT 5m Monoscopic Imagery",  "SPOT 5: High Resolution Geometric (HRG) M imagery with nominal 5 metre resolution." ,  "1",
      "SPOT 5m Stereoscopic Imagery",  "SPOT 5 High Resolution Stereoscopic (HRS) imagery with nominal 5 metre resolution." ,  "1",
      "SPOT 10m Multispectral Monoscopic Imagery",  "SPOT 5 High Resolution Geometric (HRG) 'XI' multispectral mode imagery with nominal 10 metre resolution." ,  "1",
      "SPOT HRG Digital Terrain Model",  "SPOT 5 High Resolution Geometric (HRG) Digital Terrain Model (DTM) of unspecified resolution." ,  "1",
      "BGIO 1:25k Thematic Map",    "Bundeswehr Geoinformation Office (BGIO) 1:25,000 scale thematic map.", "1",
      "BGIO 1:50k Thematic Map",    "Bundeswehr Geoinformation Office (BGIO) 1:50,000 scale thematic map.", "1",
      "BGIO 1:100k Thematic Map",   "Bundeswehr Geoinformation Office (BGIO) 1:100,000 scale thematic map.", "1",
      "BGIO 1:25k Topographic Map",  "Bundeswehr Geoinformation Office (BGIO) 1:25,000 scale topographic map.", "1",
      "BGIO 1:50k Topographic Map",  "Bundeswehr Geoinformation Office (BGIO) 1:50,000 scale topographic map.", "1",
      "BGIO 1:100k Topographic Map",  "Bundeswehr Geoinformation Office (BGIO) 1:100,000 scale topographic map.", "1",
      "BGIO Vector Data",           "Bundeswehr Geoinformation Office (BGIO) vector data of unspecified scale.", "1",
      "BGIO 1:25k Vector Data",     "Bundeswehr Geoinformation Office (BGIO) 1:25,000 scale vector data.", "1",
      "BGIO 1:50k Vector Data",     "Bundeswehr Geoinformation Office (BGIO) 1:50,000 scale vector data.", "1",
      "BGIO 1:100k Vector Data",    "Bundeswehr Geoinformation Office (BGIO) 1:100,000 scale vector data.", "1",
      "Multiple",                   "The attribute has more than one known value.", "1",
      "Unpopulated",                "The attribute value exists, but due to policy considerations it cannot be given.", "1",
      "Not Applicable",             "There is no possible value in the attribute range that would be applicable. ( May occur when the attribute is not applicable to the feature type (for example: the Airfield Type attribute of a Settlement feature type). )", "1",
      "Other",                      "The attribute value is known, but is not currently a valid member of the attribute range. ( The actual value may have been previously, or may become in the future, a valid member of the attribute range. )",  "1"
  };
int SourceTypeIdsMax2 = 207;


const char *SourceTypeIds3[229*3] = 
  {
      "unk",                        "Source is not known", "1",
      "adrg",                       "Arc Digitized Raster Graphic", "1",
      "aafif",                      "NGA Automated Aeronautical Facilities Information File", "1",
      "chum",                       "Chart Update Manual", "1",
      "city",                       "City Graphic", "1",
      "combat",                     "Combat Chart", "1",
      "cadrg",                      "Compressed Arc Digitized Raster Graphic", "1",
      "cib1",                       "Controlled Imagery Base 1 Metre", "1",
      "cib5",                       "Controlled Imagery Base 5 Metre", "1",
      "dnc",                        "Digital Nautical Chart", "1",
      "dpf",                        "Digital Print File", "1",
      "dtop1",                      "Digital Topographic Data 1", "1",
      "dtop2",                      "Digital Topographic Data 2", "1",
      "dtop3",                      "Digital Topographic Data 3", "1",
      "dtop4",                      "Digital Topographic Data 4", "1",
      "dtop5",                      "Digital Topographic Data 5", "1",
      "dvof",                       "Digital Vertical Obstruction File", "1",
      "ffd",                        "Foundation Feature Data", "1",
      "cover",                      "LandCover", "1",
      "names",                      "GeoNames", "1",
      "field",                      "GPS Field Collected Open Source", "1",
      "icm",                        "Image City Map", "1",
      "ntm",                        "NTM Imagery", "1",
      "imag",                       "Imagery", "1",
      "itd",                        "ITD", "1",
      "ivd",                        "Interim Vector Data", "1",
      "ibd",                        "International Boundaries Database", "1",
      "jog",                        "Joint Operational Graphic", "1",
      "lwd",                        "Littoral Warfare Data", "1",
      "mcg",                        "Mapping Charting and Geodetic", "1",
      "msd1",                       "Mission Specific Data 1", "1",
      "msd2",                       "Mission Specific Data 2", "1",
      "msd3",                       "Mission Specific Data 3", "1",
      "msd4",                       "Mission Specific Data 4", "1",
      "msd5",                       "Mission Specific Data 5", "1",
      "midb",                       "Modernized Integrated Data Base", "1",
      "natdat",                     "Native Data Source", "1",
      "natmap",                     "Native Map Source", "1",
      "med",                        "NGA GIS Medical Facilities Database", "1",
      "nate",                       "Nominally Attributed Topographic Evaluation Map", "1",
      "plan",                       "Planning Graphic", "1",
      "srtm",                       "Shuttle Radar Topographic Mission", "1",
      "sitmap",                     "Site Map", "1",
      "tod0",                       "Tactical Oceanographic Data Level 0", "1",
      "tod1",                       "Tactical Oceanographic Data Level 1", "1",
      "tod2",                       "Tactical Oceanographic Data Level 2", "1",
      "tod3",                       "Tactical Oceanographic Data Level 3", "1",
      "tod4",                       "Tactical Oceanographic Data Level 4", "1",
      "tod5",                       "Tactical Oceanographic Data Level 5", "1",
      "und",                        "United Nations Data", "1",
      "uvmap",                      "Urban Vector Map", "1",
      "vitd",                       "Vector Interim Terrain Data", "1",
      "vmap0",                      "Vector Map Level 0", "1",
      "vmap1",                      "Vector Map Level 1", "1",
      "vmap2",                      "Vector Map Level 2", "1",
      "wvspls",                     "World Vector Shoreline Plus", "1",
      "spthrg",                     "SPOT HRG Digital Terrain Model", "1",
      "VectorData",                 "Vector Data", "1",
      "25kVectorData",              "1:25k Vector Data", "1",
      "50kVectorData",              "1:50k Vector Data", "1",
      "100kVectorData",             "1:100k Vector Data", "1",
      "VeryHighResCommMonoImage",   "Very High Resolution Commerical Monoscopic Imagery", "1",
      "VeryHighResCommStereoImage", "Very High Resolution Commerical Stereoscopic Imagery", "1",
      "HighResCommMonoImage",       "High Resolution Commerical Monoscopic Imagery", "1",
      "HighResCommStereoImage",     "High Resolution Commerical Stereoscopic Imagery", "1",
      "MediumResCommMonoImage",     "Medium Resolution Commerical Monoscopic Imagery", "1",
      "MediumResCommStereoImage",   "Medium Resolution Commerical Stereoscopic Imagery", "1",
      "LowResCommMonoImage",        "Low Resolution Commerical Monoscopic Imagery", "1",
      "LowResCommStereoImage",      "Low Resolution Commerical Stereoscopic Imagery", "1",
      "MapOneToTwentyFiveK",        "Map 1:25k", "1",
      "MapOneToFiftyK",             "Map 1:50k", "1",
      "MapOneToHundredK",           "Map 1:100k", "1",
      "RoutingData",                "Routing Data", "1",
      "mult",                       "Multiple sources", "1",
      "N_P",                        "The attribute value exists, but due to policy considerations it cannot be given.", "1",
      "N_A",                        "There is no possible value in the attribute range that would be applicable. (May occur when the attribute is not applicable to the feature type (for example: the Airfield Type attribute of a Settlement feature type).)", "1",
      "other",                      "The attribute value is known, but is not currently a valid member of the attribute range. (The actual value may have been previously, or may become in the future, a valid member of the attribute range.)", "1",
      "unk",                        "Source is not known.", "1",
      "adrg",                       "Arc Digitized Raster Graphic.", "1",
      "aafif",                      "NGA Automated Aeronautical Facilities Information File.", "1",
      "chum",                       "NGA Chart Update Manual (CHUM).", "1",
      "city",                       "NGA City Graphic.", "1",
      "combat",                     "NGA Combat Chart data.", "1",
      "cib1",                       "Controlled Imagery Base 1 metre data.", "1",
      "cib5",                       "Controlled Imagery Base 5 metre data.", "1",
      "dnc",                        "Digital Nautical Chart.", "1",
      "dpf",                        "Digital Print File.", "1",
      "dtop1",                      "Digital Topographic Data Level 1.", "1",
      "dtop2",                      "Digital Topographic Data Level 2.", "1",
      "dtop3",                      "Digital Topographic Data Level 3.", "1",
      "dtop4",                      "Digital Topographic Data Level 4.", "1",
      "dtop5",                      "Digital Topographic Data Level 5.", "1",
      "dvof",                       "NGA Digital Vertical Obstruction File.", "1",
      "ffd",                        "Foundation Feature Data.", "1",
      "cover",                      "GeoCover or LandCover.", "1",
      "names",                      "NGA GeoNames.", "1",
      "field",                      "GPS-based field-collected open source data.", "1",
      "icm",                        "Image City Map (ICM)", "1",
      "ntm",                        "Imagery from (US) National Technical Means (NTM).", "1",
      "imag",                       "Imagery of unspecified type and resolution.", "1",
      "itd",                        "NGA Interim Terrain Data.", "1",
      "ivd",                        "NGA Interim Vector Data.", "1",
      "ibd",                        "NGA International Boundaries database.", "1",
      "jog",                        "Joint Operational Graphic.", "1",
      "lwd",                        "NGA Littoral Warfare Data.", "1",
      "mcg",                        "Mapping, charting and/or geodetic data.", "1",
      "msd1",                       "NGA Mission Specific Data Level 1.", "1",
      "msd2",                       "NGA Mission Specific Data Level 2.", "1",
      "msd3",                       "NGA Mission Specific Data Level 3.", "1",
      "msd4",                       "NGA Mission Specific Data Level 4.", "1",
      "msd5",                       "NGA Mission Specific Data Level 5.", "1",
      "midb",                       "(US) Modernized Integrated Data Base.", "1",
      "natdat",                     "Native data source.", "1",
      "natmap",                     "Native map source.", "1",
      "med",                        "NGA GIS medical facilities database.", "1",
      "nate",                       "NGA Nominally Attributed Topographic Evaluation map.", "1",
      "plan",                       "NGA Planning Graphic.", "1",
      "srtm",                       "(US) Shuttle Radar Topographic Mission.", "1",
      "sitmap",                     "Site map (for example: at a nuclear facility).", "1",
      "tod0",                       "NGA Tactical Oceanographic Data Level 0.", "1",
      "tod1",                       "NGA Tactical Oceanographic Data Level 1.", "1",
      "tod2",                       "NGA Tactical Oceanographic Data Level 2.", "1",
      "tod3",                       "NGA Tactical Oceanographic Data Level 3.", "1",
      "tod4",                       "NGA Tactical Oceanographic Data Level 4.", "1",
      "tod5",                       "NGA Tactical Oceanographic Data Level 5.", "1",
      "und",                        "United Nations data.", "1",
      "uvmap",                      "Urban Vector Map.", "1",
      "vitd",                       "NGA Vector Interim Terrain Data.", "1",
      "vmap0",                      "Vector Map Level 0.", "1",
      "vmap1",                      "Vector Map Level 1.", "1",
      "vmap2",                      "Vector Map Level 2.", "1",
      "wvspls",                     "NGA World Vector Shoreline Plus.", "1",
      "spthrg",                     "SPOT 5 High Resolution Geometric (HRG) Digital Terrain Model (DTM) of unspecified resolution.", "1",
      "VectorData",                 "Vector data of unspecified scale", "1",
      "25kVectorData",              "1:25,000 scale vector data.", "1",
      "50kVectorData",              "1:50,000 scale vector data.", "1",
      "100kVectorData",             "1:100,000 scale vector data.", "1",
      "VeryHighResCommMonoImage",   "Commercial monoscopic imagery with a resolution equal or better than 1 metre", "1",
      "VeryHighResCommStereoImage",  "Commercial stereoscopic imagery with a resolution equal or better than 1 metre", "1",
      "HighResCommMonoImage",       "Commercial monoscopic imagery with a resolution  more than  1 metre up to 5 metre", "1",
      "HighResCommStereoImage",     "Commercial stereoscopic imagery with a resolution  more than  1 metre up to 5 metre", "1",
      "MediumResCommMonoImage",     "Commercial monoscopic imagery with a resolution  lower than 5 metre", "1",
      "MediumResCommStereoImage",   "Commercial stereoscopic imagery with a resolution lower than 5 metre", "1",
      "LowResCommMonoImage",        "Commercial monoscopic imagery with a resolution  lower than 15 metre", "1",
      "LowResCommStereoImage",      "Commercial stereoscopic imagery with a resolution lower than 15 metre", "1",
      "MapOneToTwentyFiveK",        "1:25,000 scale topographic or thematic map.", "1",
      "MapOneToFiftyK",             "1:50,000 scale topographic or thematic map.", "1",
      "MapOneToHundredK",           "1:100,000 scale topographic or thematic map.", "1",
      "RoutingData",                "Routing Data for navigation on roads", "1",
      "mult",                       "The attribute has more than one known value.", "1",
      "N_A",                        "There is no possible value in the attribute range that would be applicable. ( May occur when the attribute is not applicable to the feature type (for example: the Airfield Type attribute of a Settlement feature type). )", "1",
      "other",                      "The attribute value is known, but is not currently a valid member of the attribute range. ( The actual value may have been previously, or may become in the future, a valid member of the attribute range. )", "1",
      "Unknown",                    "The attribute value is unknown.", "1",
      "ADRG",                       "NGA Arc Digitized Raster Graphic.", "1",
      "AAFIF",                      "NGA Automated Aeronautical Facilities Information File.", "1",
      "CHUM",                       "NGA Chart Update Manual (CHUM)", "1",
      "City Graphic",               "NGA City Graphic", "1",
      "Combat Chart",               "NGA Combat Chart data.", "1",
      "CADRG",                      "NGA Compressed Arc Digitized Raster Graphic.", "1",
      "CIB1",                       "NGA Controlled Imagery Base 1 metre data.", "1",
      "CIB5",                       "NGA Controlled Imagery Base 5 metre data.", "1",
      "DNC",                        "NGA Digital Nautical Chart.", "1",
      "DPF",                        "NGA Digital Print File.", "1",
      "DTOP 1",                     "NGA Digital Topographic Data Level 1.", "1",
      "DTOP 2",                     "NGA Digital Topographic Data Level 2.", "1",
      "DTOP 3",                     "NGA Digital Topographic Data Level 3.", "1",
      "DTOP 4",                     "NGA Digital Topographic Data Level 4.", "1",
      "DTOP 5",                     "NGA Digital Topographic Data Level 5.", "1",
      "DVOF",                       "NGA Digital Vertical Obstruction File.", "1",
      "FFD",                        "NGA Foundation Feature Data.", "1",
      "Land Cover",                 "NGA GeoCover or LandCover.", "1",
      "GeoNames",                   "NGA GeoNames.", "1",
      "GPS",                        "GPS-based field-collected open source data.", "1",
      "ICM",                        "NGA Image City Map (ICM)", "1",
      "NTM Imagery",                "Imagery from (US) National Technical Means (NTM).", "1",
      "Imagery",                    "Imagery of unspecified type and resolution.", "1",
      "ITD",                        "NGA Interim Terrain Data.", "1",
      "IVD",                        "NGA Interim Vector Data.", "1",
      "International Boundaries",   "NGA International Boundaries database.", "1",
      "JOG",                        "NGA Joint Operational Graphic.", "1",
      "LWD",                        "NGA Littoral Warfare Data.", "1",
      "MC&G",                       "Mapping, charting and/or geodetic data.", "1",
      "MSD 1",                      "NGA Mission Specific Data Level 1.", "1",
      "MSD 2",                      "NGA Mission Specific Data Level 2.", "1",
      "MSD 3",                      "NGA Mission Specific Data Level 3.", "1",
      "MSD 4",                      "NGA Mission Specific Data Level 4.", "1",
      "MSD 5",                      "NGA Mission Specific Data Level 5.", "1",
      "MIDB",                       "(US) Modernized Integrated Data Base.", "1",
      "Native Data",                "Native data source.", "1",
      "Native Map",                 "Native map source.", "1",
      "Medical Facilities",         "NGA GIS medical facilities database.", "1",
      "NATE",                       "NGA Nominally Attributed Topographic Evaluation map.", "1",
      "Planning Graphic",           "NGA Planning Graphic.", "1",
      "SRTM",                       "(US) Shuttle Radar Topographic Mission.", "1",
      "Site Map",                   "Site map (for example: at a nuclear facility).", "1",
      "TOD 0",                      "NGA Tactical Oceanographic Data Level 0.", "1",
      "TOD 1",                      "NGA Tactical Oceanographic Data Level 1.", "1",
      "TOD 2",                      "NGA Tactical Oceanographic Data Level 2.", "1",
      "TOD 3",                      "NGA Tactical Oceanographic Data Level 3.", "1",
      "TOD 4",                      "NGA Tactical Oceanographic Data Level 4.", "1",
      "TOD 5",                      "NGA Tactical Oceanographic Data Level 5.", "1",
      "UN Data",                    "United Nations data.", "1",
      "UVMap",                      "NGA Urban Vector Map.", "1",
      "VITD",                       "NGA Vector Interim Terrain Data.", "1",
      "VMap 0",                     "NGA Vector Map Level 0.", "1",
      "VMap 1",                     "NGA Vector Map Level 1.", "1",
      "VMap 2",                     "NGA Vector Map Level 2.", "1",
      "WVS Plus",                   "NGA World Vector Shoreline Plus.", "1",
      "SPOT HRG Digital Terrain Model",  "SPOT 5 High Resolution Geometric (HRG) Digital Terrain Model (DTM) of unspecified resolution.", "1",
      "Vector Data",                "Vector data of unspecified scale.", "1",
      "1:25k Vector Data",          "1:25,000 scale vector data.", "1",
      "1:50k Vector Data",          "1:50,000 scale vector data.", "1",
      "1:100k Vector Data",         "1:100,000 scale vector data.", "1",
      "Very High Resolution Commercial Monoscopic Imagery",  "Commercial monoscopic imagery with a resolution equal or better than 1 metre.", "1",
      "Very High Resolution Commercial Stereoscopic Imagery",  "Commercial stereoscopic imagery with a resolution equal or better than 1 metre.", "1",
      "High Resolution Commercial Monoscopic Imagery",  "Commercial monoscopic imagery with a resolution more than 1 metre up to 5 metre.", "1",
      "High Resolution Commercial Stereoscopic Imagery",  "Commercial stereoscopic imagery with a resolution more than 1 metre up to 5 metre.", "1",
      "Medium Resolution Commercial Monoscopic Imagery",  "Commercial monoscopic imagery with a resolution lower than 5 metre.", "1",
      "Medium Resolution Commercial Stereoscopic Imagery",  "Commercial stereoscopic imagery with a resolution lower than 5 metre.", "1",
      "Low Resolution Commercial Monoscopic Imagery",  "Commercial monoscopic imagery with a resolution lower than 15 metre.", "1",
      "Low Resolution Commercial Stereoscopic Imagery",  "Commercial stereoscopic imagery with a resolution lower than 15 metre.", "1",
      "Map 1:25k",                  "1:25,000 scale topographic or thematic map.", "1",
      "Map 1:50k",                  "1:50,000 scale topographic or thematic map.", "1",
      "Map 1:100k",                 "1:100,000 scale topographic or thematic map.", "1",
      "Routing Data",               "Routing Data for navigation on roads.", "1",
      "Multiple",                   "The attribute has more than one known value.", "1",
      "Unpopulated",                "The attribute value exists, but due to policy considerations it cannot be given.", "1",
      "Not Applicable",             "There is no possible value in the attribute range that would be applicable. ( May occur when the attribute is not applicable to the feature type (for example: the Airfield Type attribute of a Settlement feature type). )", "1",
      "Other",                      "The attribute value is known, but is not currently a valid member of the attribute range. ( The actual value may have been previously, or may become in the future, a valid member of the attribute range. )",  "1"
  };
int SourceTypeIdsMax3 = 229;


const char *VerticalSourceTypeIds4[8*3] =   
{
  "UNK" , "Unknown" , "1",
  "interpolatedDted1" , "DTED 1" , "1",
  "interpolatedDted2" , "DTED 2" , "1",
  "noElevations" , "No Elevations" , "1",
  "reflectiveSurface" , "Reflective Surface" , "1",
  "stereoscopicImagery" , "Stereoscopic Imagery" , "1",
  "tinData" , "TIN Data" , "1",
  "interpolatedSrtm1", "SRTM 1", "1"  
};

int VerticalSourceTypeIdsMax4 = 8;  


const char *VerticalSourceTypeIds41[7*3] =
{
 "UNK","Unknown","1",
 "interpolatedDted1","DTED 1","1",
 "interpolatedDted2","DTED 2","1",
 "noElevations","No Elevations","1",
 "reflectiveSurface","Reflective Surface","1",
 "stereoscopicImagery","Stereoscopic Imagery","1",
 "tinData","TIN Data","1"
};

int VerticalSourceTypeIdsMax41 = 7;






const char *SourceTypeIds4[77*3] = 
{
  "UNK" , "Unknown", "1",
  "ngaArcDigRasterGraphic" , "ADRG", "1",
  "ngaAutoAirFacInfoFile" , "AAFIF", "1",
  "ngaChartUpdateManual" , "CHUM", "1",
  "ngaCityGraphic" , "City Graphic", "1",
  "ngaCombatChart" , "Combat Chart", "1",
  "ngaCompressedAdrg" , "CADRG", "1",
  "ngaControlledImageBase1" , "CIB1", "1",
  "ngaControlledImageBase5" , "CIB5", "1",
  "ngaDigitalNauticalChart" , "DNC", "1",
  "ngaDigitalPrintFile" , "DPF", "1",
  "ngaDigitalTopoData1" , "DTOP 1", "1",
  "ngaDigitalTopoData2" , "DTOP 2", "1",
  "ngaDigitalTopoData3" , "DTOP 3", "1",
  "ngaDigitalTopoData4" , "DTOP 4", "1",
  "ngaDigitalTopoData5" , "DTOP 5", "1",
  "ngaDigitalVertObstruction" , "DVOF", "1",
  "ngaFoundationFeatureData" , "FFD", "1",
  "ngaGeoLandCover" , "Land Cover", "1",
  "ngaGeoNames" , "GeoNames", "1",
  "gpsBasedFieldCollect" , "GPS", "1",
  "ngaImageCityMap" , "ICM", "1",
  "usNtmImagery" , "NTM Imagery", "1",
  "imageryUnspecified" , "Imagery", "1",
  "ngaInterimTerrainData" , "ITD", "1",
  "ngaInterimVectorData" , "IVD", "1",
  "ngaInternationalBoundary" , "International Boundaries", "1",
  "ngaJointOperatGraphic" , "JOG", "1",
  "ngaLittoralWarfareData" , "LWD", "1",
  "mapChartOrGeodeticData" , "MC&G", "1",
  "ngaMissionSpecificData1" , "MSD 1", "1",
  "ngaMissionSpecificData2" , "MSD 2", "1",
  "ngaMissionSpecificData3" , "MSD 3", "1",
  "ngaMissionSpecificData4" , "MSD 4", "1",
  "ngaMissionSpecificData5" , "MSD 5", "1",
  "usModernizedIntegratedDB" , "MIDB", "1",
  "nativeData" , "Native Data", "1",
  "nativeMap" , "Native Map", "1",
  "ngaMedicalFacilities" , "Medical Facilities", "1",
  "ngaNominalAttTopoEvalMap" , "NATE", "1",
  "ngaPlanningGraphic" , "Planning Graphic", "1",
  "usShuttleRadarTopoMission" , "SRTM", "1",
  "siteMap" , "Site Map", "1",
  "ngaTacticalOceanData0" , "TOD 0", "1",
  "ngaTacticalOceanData1" , "TOD 1", "1",
  "ngaTacticalOceanData2" , "TOD 2", "1",
  "ngaTacticalOceanData3" , "TOD 3", "1",
  "ngaTacticalOceanData4" , "TOD 4", "1",
  "ngaTacticalOceanData5" , "TOD 5", "1",
  "unitedNationsData" , "UN Data", "1",
  "ngaUrbanVectorMap" , "UVMap", "1",
  "ngaVectorInterimTerrain" , "VITD", "1",
  "ngaVectorMap0" , "VMap 0", "1",
  "ngaVectorMap1" , "VMap 1", "1",
  "ngaVectorMap2" , "VMap 2", "1",
  "ngaWorldVectorShorePlus" , "WVS Plus", "1",
  "spotHrgDtm" , "SPOT HRG Digital Terrain Model", "1",
  "vectorData" , "Vector Data", "1",
  "vectorData25k" , "1:25k Vector Data", "1",
  "vectorData50k" , "1:50k Vector Data", "1",
  "vectorData100k" , "1:100k Vector Data", "1",
  "veryHighResCommMonoImage" , "Very High Resolution Commercial Monoscopic Imagery", "1",
  "veryHighResCommStereoImage" , "Very High Resolution Commercial Stereoscopic Imagery", "1",
  "highResCommMonoImage" , "High Resolution Commercial Monoscopic Imagery", "1",
  "highResCommStereoImage" , "High Resolution Commercial Stereoscopic Imagery", "1",
  "mediumResCommMonoImage" , "Medium Resolution Commercial Monoscopic Imagery", "1",
  "mediumResCommStereoImage" , "Medium Resolution Commercial Stereoscopic Imagery", "1",
  "lowResCommMonoImage" , "Low Resolution Commercial Monoscopic Imagery", "1",
  "lowResCommStereoImage" , "Low Resolution Commercial Stereoscopic Imagery", "1",
  "mapOneToTwentyFiveK" , "Map 1:25k", "1",
  "mapOneToFiftyK" , "Map 1:50k", "1",
  "mapOneToHundredK" , "Map 1:100k", "1",
  "routingData" , "Routing Data", "1",
  "MULT" , "Multiple", "1",
  "N_P" , "Unpopulated", "1",
  "N_A" , "Not Applicable", "1",
  "OTH" , "Other", "1"
};
int SourceTypeIdsMax4 = 77;




const char *SourceTypeIds41[77*3] =
{
 "UNK",  "Unknown",  "1",
 "ngaArcDigRasterGraphic",  "ADRG",  "1",
 "ngaAutoAirFacInfoFile",  "AAFIF",  "1",
 "ngaChartUpdateManual",  "CHUM",  "1",
 "ngaCityGraphic",  "City Graphic",  "1",
 "ngaCombatChart",  "Combat Chart",  "1",
 "ngaCompressedAdrg",  "CADRG",  "1",
 "ngaControlledImageBase1",  "CIB1",  "1",
 "ngaControlledImageBase5",  "CIB5",  "1",
 "ngaDigitalNauticalChart",  "DNC",  "1",
 "ngaDigitalPrintFile",  "DPF",  "1",
 "ngaDigitalTopoData1",  "DTOP 1",  "1",
 "ngaDigitalTopoData2",  "DTOP 2",  "1",
 "ngaDigitalTopoData3",  "DTOP 3",  "1",
 "ngaDigitalTopoData4",  "DTOP 4",  "1",
 "ngaDigitalTopoData5",  "DTOP 5",  "1",
 "ngaDigitalVertObstruction",  "DVOF",  "1",
 "ngaFoundationFeatureData",  "FFD",  "1",
 "ngaGeoLandCover",  "Land Cover",  "1",
 "ngaGeoNames",  "GeoNames",  "1",
 "gpsBasedFieldCollect",  "GPS",  "1",
 "ngaImageCityMap",  "ICM",  "1",
 "usNtmImagery",  "NTM Imagery",  "1",
 "imageryUnspecified",  "Imagery",  "1",
 "ngaInterimTerrainData",  "ITD",  "1",
 "ngaInterimVectorData",  "IVD",  "1",
 "ngaInternationalBoundary",  "International Boundaries",  "1",
 "ngaJointOperatGraphic",  "JOG",  "1",
 "ngaLittoralWarfareData",  "LWD",  "1",
 "mapChartOrGeodeticData",  "MC&G",  "1",
 "ngaMissionSpecificData1",  "MSD 1",  "1",
 "ngaMissionSpecificData2",  "MSD 2",  "1",
 "ngaMissionSpecificData3",  "MSD 3",  "1",
 "ngaMissionSpecificData4",  "MSD 4",  "1",
 "ngaMissionSpecificData5",  "MSD 5",  "1",
 "usModernizedIntegratedDB",  "MIDB",  "1",
 "nativeData",  "Native Data",  "1",
 "nativeMap",  "Native Map",  "1",
 "ngaMedicalFacilities",  "Medical Facilities",  "1",
 "ngaNominalAttTopoEvalMap",  "NATE",  "1",
 "ngaPlanningGraphic",  "Planning Graphic",  "1",
 "usShuttleRadarTopoMission",  "SRTM",  "1",
 "siteMap",  "Site Map",  "1",
 "ngaTacticalOceanData0",  "TOD 0",  "1",
 "ngaTacticalOceanData1",  "TOD 1",  "1",
 "ngaTacticalOceanData2",  "TOD 2",  "1",
 "ngaTacticalOceanData3",  "TOD 3",  "1",
 "ngaTacticalOceanData4",  "TOD 4",  "1",
 "ngaTacticalOceanData5",  "TOD 5",  "1",
 "unitedNationsData",  "UN Data",  "1",
 "ngaUrbanVectorMap",  "UVMap",  "1",
 "ngaVectorInterimTerrain",  "VITD",  "1",
 "ngaVectorMap0",  "VMap 0",  "1",
 "ngaVectorMap1",  "VMap 1",  "1",
 "ngaVectorMap2",  "VMap 2",  "1",
 "ngaWorldVectorShorePlus",  "WVS Plus",  "1",
 "spotHrgDtm",  "SPOT HRG Digital Terrain Model",  "1",
 "vectorData",  "Vector Data",  "1",
 "vectorData25k",  "1:25k Vector Data",  "1",
 "vectorData50k",  "1:50k Vector Data",  "1",
 "vectorData100k",  "1:100k Vector Data",  "1",
 "veryHighResCommMonoImage",  "Very High Resolution Commercial Monoscopic Imagery",  "1",
 "veryHighResCommStereoImage",  "Very High Resolution Commercial Stereoscopic Imagery",  "1",
 "highResCommMonoImage",  "High Resolution Commercial Monoscopic Imagery",  "1",
 "highResCommStereoImage",  "High Resolution Commercial Stereoscopic Imagery",  "1",
 "mediumResCommMonoImage",  "Medium Resolution Commercial Monoscopic Imagery",  "1",
 "mediumResCommStereoImage",  "Medium Resolution Commercial Stereoscopic Imagery",  "1",
 "lowResCommMonoImage",  "Low Resolution Commercial Monoscopic Imagery",  "1",
 "lowResCommStereoImage",  "Low Resolution Commercial Stereoscopic Imagery",  "1",
 "mapOneToTwentyFiveK",  "Map 1:25k",  "1",
 "mapOneToFiftyK",  "Map 1:50k",  "1",
 "mapOneToHundredK",  "Map 1:100k",  "1",
 "routingData",  "Routing Data",  "1",
 "MULT",  "Multiple",  "1",
 "N_P",  "Unpopulated",  "1",
 "N_A",  "Not Applicable",  "1",
 "OTH",  "Other",  "1"
};

int SourceTypeIdsMax41 = 77;



const char *SourceTypeIds44[70*3] =
{
 "UNK",  "Unknown",  "1",
 "ngaArcDigRasterGraphic",  "ADRG",  "1",
 "ngaAutoAirFacInfoFile",  "AAFIF",  "1",
 "ngaChartUpdateManual",  "CHUM",  "1",
 "ngaCityGraphic",  "City Graphic",  "1",
 "ngaCombatChart",  "Combat Chart",  "1",
 "ngaCompressedAdrg",  "CADRG",  "1",
 "ngaControlledImageBase1",  "CIB1",  "1",
 "ngaControlledImageBase5",  "CIB5",  "1",
 "ngaDigitalNauticalChart",  "DNC",  "1",
 "ngaDigitalPrintFile",  "DPF",  "1",
 "ngaDigitalTopoData1",  "DTOP 1",  "1",
 "ngaDigitalTopoData2",  "DTOP 2",  "1",
 "ngaDigitalTopoData3",  "DTOP 3",  "1",
 "ngaDigitalTopoData4",  "DTOP 4",  "1",
 "ngaDigitalTopoData5",  "DTOP 5",  "1",
 "ngaDigitalVertObstruction",  "DVOF",  "1",
 "ngaFoundationFeatureData",  "FFD",  "1",
 "ngaGeoLandCover",  "Land Cover",  "1",
 "ngaGeoNames",  "GeoNames",  "1",
 "gpsBasedFieldCollect",  "GPS",  "1",
 "ngaImageCityMap",  "ICM",  "1",
 "usNtmImagery",  "NTM Imagery",  "1",
 "imageryUnspecified",  "Imagery",  "1",
 "ngaInterimTerrainData",  "ITD",  "1",
 "ngaInterimVectorData",  "IVD",  "1",
 "ngaInternationalBoundary",  "International Boundaries",  "1",
 "ngaJointOperatGraphic",  "JOG",  "1",
 "ngaLittoralWarfareData",  "LWD",  "1",
 "mapChartOrGeodeticData",  "MC&G",  "1",
 "ngaMissionSpecificData1",  "MSD 1",  "1",
 "ngaMissionSpecificData2",  "MSD 2",  "1",
 "ngaMissionSpecificData3",  "MSD 3",  "1",
 "ngaMissionSpecificData4",  "MSD 4",  "1",
 "ngaMissionSpecificData5",  "MSD 5",  "1",
 "usModernizedIntegratedDB",  "MIDB",  "1",
 "nativeData",  "Native Data",  "1",
 "nativeMap",  "Native Map",  "1",
 "ngaMedicalFacilities",  "Medical Facilities",  "1",
 "ngaNominalAttTopoEvalMap",  "NATE",  "1",
 "ngaPlanningGraphic",  "Planning Graphic",  "1",
 "usShuttleRadarTopoMission",  "SRTM",  "1",
 "siteMap",  "Site Map",  "1",
 "unitedNationsData",  "UN Data",  "1",
 "ngaUrbanVectorMap",  "UVMap",  "1",
 "ngaVectorInterimTerrain",  "VITD",  "1",
 "ngaVectorMap0",  "VMap 0",  "1",
 "ngaVectorMap1",  "VMap 1",  "1",
 "ngaVectorMap2",  "VMap 2",  "1",
 "ngaWorldVectorShorePlus",  "WVS Plus",  "1",
 "spotHrgDtm",  "SPOT HRG Digital Terrain Model",  "1",
 "vectorData",  "Vector Data",  "1",
 "vectorData25k",  "1:25k Vector Data",  "1",
 "vectorData50k",  "1:50k Vector Data",  "1",
 "vectorData100k",  "1:100k Vector Data",  "1",
 "veryHighResCommMonoImage",  "Very High Resolution Commercial Monoscopic Imagery",  "1",
 "veryHighResCommStereoImage",  "Very High Resolution Commercial Stereoscopic Imagery",  "1",
 "highResCommMonoImage",  "High Resolution Commercial Monoscopic Imagery",  "1",
 "highResCommStereoImage",  "High Resolution Commercial Stereoscopic Imagery",  "1",
 "mediumResCommMonoImage",  "Medium Resolution Commercial Monoscopic Imagery",  "1",
 "mediumResCommStereoImage",  "Medium Resolution Commercial Stereoscopic Imagery",  "1",
 "lowResCommMonoImage",  "Low Resolution Commercial Monoscopic Imagery",  "1",
 "lowResCommStereoImage",  "Low Resolution Commercial Stereoscopic Imagery",  "1",
 "mapOneToTwentyFiveK",  "Map 1:25k",  "1",
 "mapOneToFiftyK",  "Map 1:50k",  "1",
 "mapOneToHundredK",  "Map 1:100k",  "1",
 "routingData",  "Routing Data",  "1",
 "MULT",  "Multiple",  "1",
 "N_P",  "Unpopulated",  "1",
 "OTH",  "Other",  "1"
};

int SourceTypeIdsMax44 = 70;


const char *SourceScales44[70] =
{
 "BlankOnly",
 ">=5,000,000",
 "BlankOnly",
 "BlankOnly",
 "2,500 to 25,000",
 "25,000 or 50,000",
 ">=5,000,000",
 "10,000",
 "50,000",
 "50,000 to 500,000",
 "NoSpec",
 "50,000 or 100,000",
 "50,000 or 100,000",
 "50,000 or 100,000",
 "50,000 or 100,000",
 "50,000 or 100,000",
 "BlankOnly",
 "50,000 to 250,000",
 "BlankOnly",
 "BlankOnly",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "BlankOnly",
 "50,000 to 250,000",
 "BlankOnly",
 "BlankOnly",
 "250,000",
 "5,000 to 50,000",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "BlankOnly",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "50,000",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "NoSpec",
 "50,000",
 "1,000,000",
 "250,000",
 "50,000",
 "250,000 to 120,000,000",
 "25,000 to 50,000",
 "BlankOnly",
 "25,000",
 "50,000",
 "100,000",
 "<=10,000",
 "<=10,000",
 "10,001 to 50,000",
 "10,001 to 50,000",
 "50,001 to 150,000",
 "50,001 to 150,000",
 ">150,000",
 ">150,000",
 "25,000",
 "50,000",
 "100,000",
 "NoSpec",
 "BlankOnly",
 "BlankOnly",
 "NoSpec"
};





int *SubregionACEI = NULL;
int *SubregionALEI = NULL;
double *SubregionACEV = NULL;
double *SubregionALEV = NULL;
char **SubregionSCPYRT;
char *CCPYRT_SAVE = NULL;




extern "C" char GErr[];
extern "C" int alldatafiles;
extern "C" int SLASHTYPE;
extern "C" int POIINDEX;
extern "C" int DoAce;

extern "C" struct SubregionPolyStruct *SubregionPolys;
extern "C" int NumSubregionPolys;

extern "C" struct SubregionDataStruct *SubregionData;
extern "C" int NumSubregionData;

extern "C" struct SubregionSourceStruct *SubregionSources;
extern "C" int NumSubregionSources;

extern "C" int ProcessXMLShapes(FILE *outerrfile, char *shape_to_import, int *pointcount, int *linecount, int *areacount, int shpnum, char *feattype, FILE *poifile, FILE *errfile, char *CCPYRTSAVE, int version);
extern "C" int SH_PointInAreal(double px,double py,double * x,double * y, int startindex, int stopindex);
extern "C" void FreeShapeMemory();
extern "C" int NUM_CHECK(char *value);
extern "C" int GetTimeValues(int *year, int *month, int *day);




#include <string>
#include <memory>   
#include <cstddef>  


#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/framework/XMLGrammarPoolImpl.hpp>

using namespace std;
using namespace xercesc;


class error_handler: public DOMErrorHandler
{
public:
  error_handler () : failed_ (false) {}

  bool
  failed () const { return failed_; }

  virtual bool
  handleError (const xercesc::DOMError&);

private:
  bool failed_;
};

DOMLSParser * create_parser (XMLGrammarPool* pool)
{
  const XMLCh ls_id [] = {chLatin_L, chLatin_S, chNull};

  DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation (ls_id);

  DOMLSParser       *parser = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0,
      XMLPlatformUtils::fgMemoryManager,
      pool);


  DOMConfiguration* conf = parser->getDomConfig ();

  conf->setParameter (XMLUni::fgDOMComments, false);
  conf->setParameter (XMLUni::fgDOMDatatypeNormalization, true);
  conf->setParameter (XMLUni::fgDOMEntities, false);
  conf->setParameter (XMLUni::fgDOMNamespaces, true);
  conf->setParameter (XMLUni::fgDOMElementContentWhitespace, false);

  conf->setParameter (XMLUni::fgDOMValidate, true);
  conf->setParameter (XMLUni::fgXercesSchema, true);
  conf->setParameter (XMLUni::fgXercesSchemaFullChecking, true);  

  conf->setParameter (XMLUni::fgXercesUseCachedGrammarInParse, true);

  conf->setParameter (XMLUni::fgXercesLoadSchema, false);

#if _XERCES_VERSION >= 30100
  conf->setParameter (XMLUni::fgXercesHandleMultipleImports, true);
#endif

  conf->setParameter (XMLUni::fgXercesUserAdoptsDOMDocument, true);

  return parser;
}






extern "C" int TestGrammar (char *xsdfile, char *xmlfile)
{
  int r=0;

  XMLPlatformUtils::Initialize ();


  while (true)
  {
    MemoryManager* mm = XMLPlatformUtils::fgMemoryManager;
    auto_ptr<XMLGrammarPool> gp (new XMLGrammarPoolImpl (mm));

    {
      DOMLSParser* parser = create_parser (gp.get ());

      error_handler eh;
      parser->getDomConfig ()->setParameter (XMLUni::fgDOMErrorHandler, &eh);

  

      if (!parser->loadGrammar (xsdfile, Grammar::SchemaGrammarType, true))
	  {
        fprintf(outerrfile,"Error:  Failed to load xsd %s\n\n",xsdfile);
        r = 1;
        break;
	  }

      if (eh.failed ())
	  {
        r = 1;
        break;
	  }

      parser->release ();

      if (r != 0)
	  {
        break;
	  }
    }

    gp->lockPool ();

    DOMLSParser* parser = create_parser (gp.get ());

    error_handler eh;
    parser->getDomConfig ()->setParameter (XMLUni::fgDOMErrorHandler, &eh);


    DOMDocument* doc = parser->parseURI (xmlfile);

    if (doc)
	{
      doc->release ();
	}

    parser->release ();
    break;
  }

  XMLPlatformUtils::Terminate ();

  return 1;
}



const char *GetWarnType(DOMError::ErrorSeverity warn)
{
  if(warn==DOMError::DOM_SEVERITY_WARNING)
  {
    return "Warning";
  }
  else if(warn==DOMError::DOM_SEVERITY_ERROR)
  {
    return "Error";
  }
  else if(warn==DOMError::DOM_SEVERITY_FATAL_ERROR)
  {
    return "Fatal Error";
  }

  printf("error in GWT: %d\n",warn);

  
  return "error";
}



bool error_handler::handleError (const xercesc::DOMError& e)
{

  bool warn (e.getSeverity() == DOMError::DOM_SEVERITY_WARNING);

  if (!warn)
    failed_ = true;

  DOMLocator* loc = e.getLocation ();

  char* uri = XMLString::transcode (loc->getURI ());
  char* msg = XMLString::transcode (e.getMessage ());


  fprintf(outerrfile,"%s line %d column %d:\n   %s: %s\n\n",
	  uri,(int)loc->getLineNumber(),(int)loc->getColumnNumber(),GetWarnType(e.getSeverity()),msg);


  SchemaErrorCount = SchemaErrorCount + 1;

  XMLString::release (&uri);
  XMLString::release (&msg);

  return true;
}





void PrintAttributesToErrFile()
{
  int i;

  if(numattributes==0)
  {  
    fprintf(outerrfile,"   No attributes found at this location.\n");
  }  
  else
  {  
    fprintf(outerrfile,"   %d attributes found:\n",numattributes);
 
    for(i=0;i<numattributes;i++)
	{  
       fprintf(outerrfile,"     {\"%s\":\"%s\"}\n",attributes[i],attributeV[i]);
	}  
  }
  
  fprintf(outerrfile,"\n");
}


void SetUpLineOffsets(CMarkup xml)
{
  xml.GetOffsets(&offsetStart,&offsetLen);

  if(trex_type==1)
  {
    offsetStart = offsetStart + 4; 
  }
}



void ScrubPath()
{
  int len,i;

  


  len = strlen(thepath);
  for(i=len;i>=4;i--)
  {
	if(
		(thepath[i]==' ')   &&
		(thepath[i-1]=='*') &&
		(thepath[i-2]=='*') &&
		(thepath[i-3]=='*') &&
		(thepath[i-4]==' ')
		)
	{
      thepath[i-4] = '\0';
      break;
	}
  }


  len = strlen(theRealpath);
  for(i=len;i>=4;i--)
  {
	if(
		(theRealpath[i]==' ')   &&
		(theRealpath[i-1]=='*') &&
		(theRealpath[i-2]=='*') &&
		(theRealpath[i-3]=='*') &&
		(theRealpath[i-4]==' ')
		)
	{
      theRealpath[i-4] = '\0';
      break;
	}
  }



}


char *GetLineNumber()
{
  int i;
  static char retval[100];

  if(DoLineNumbers==0)
  {
    sprintf(retval,"Line numbers unavailable.");
    return retval;
  }

  if(UseCDESCRNumber>0)
  {
    return SaveCDESCRNumber;
  }

  for(i=0;i<totallines;i++)
  {
    if(FilePosns[i]>offsetStart)
	{
      sprintf(retval,"%d",i);
      return retval;
	}
  }

  sprintf(retval,"Unknown error generating line number");
  return retval;
}


char *ParsePath(int type)
{
  static char badtype[25];
  static char retval[1000];

  

  if(trex_type==1)
  {
	if(savespecialpath==1)
	{
	  sprintf(retval,"  Line: %s\n  Path: %s",GetLineNumber(),specialpath);
      return retval;
	}

    if(type==0)
	{
	  sprintf(retval,"  Line: %s\n  Path: %s",GetLineNumber(),theRealpath);
      return retval;
	} 
    else if(type==1)
	{ 
	  sprintf(retval,"  Line: %s\n  Path: %s",GetLineNumber(),SAVECURIPATH);
	  return retval;
	} 
  }

  if(type==0)
  {
	sprintf(retval,"  Line: %s\nPath: %s",GetLineNumber(),theRealpath);
    return retval;
  }
  else if(type==1)
  {
	sprintf(retval,"  Line: %s\nPath: %s",GetLineNumber(),SAVECURIPATH);
	return retval;
  }


  

  sprintf(badtype,"PP Error: %d",type);
  return badtype;
}


void HandleBlankValues(char *lasttoken)
{

  if(!strcmp(lasttoken,"CharacterString"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty CharacterString value\n\n",ParsePath(0));
	}
    else if(!strcmp(lasttoken,"Date"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty Date value\n\n",ParsePath(0));
	}
    else if(!strcmp(lasttoken,"Integer"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty Integer value\n\n",ParsePath(0));
	}
    else if(!strcmp(lasttoken,"Decimal"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty Decimal value\n\n",ParsePath(0));
	}
    else if(!strcmp(lasttoken,"Boolean"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty Boolean value\n\n",ParsePath(0));
	}
    else if(!strcmp(lasttoken,"LocalName"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty LocalName value\n\n",ParsePath(0));
	}
    else if(!strcmp(lasttoken,"Decimal"))
	{
		fprintf(outerrfile,"Error:  %s\n  Empty Decimal value\n\n",ParsePath(0));
	}
	else if(!strncmp(lasttoken,"",4)) 
	{
		fprintf(outerrfile,"Error:  %s\n  Empty %s value\n\n",ParsePath(0),lasttoken);
	}
}




void TestS1AttributeList(const char *attributename)
{
  int i;

  for(i=0;i<numattributes;i++)
  {
    if(trd1_override==1)
	{
      if(
          (!strcmp(attributename,attributes[i])) &&
          (  
            (!strcmp("MGCP_FeatureCatalogue_TRD1.0_20051231.xml",attributeV[i]))  || 
            (!strcmp("MGCP_FeatureCatalogue_TRD1.1_20061019.xml",attributeV[i]))
          )
        )
	  {
        return;
	  } 
	}
	else
	{
      if(
          (!strcmp(attributename,attributes[i])) &&
          (  
            (!strcmp("MGCP_FeatureCatalogue_TRD2.0_20070328.xml",attributeV[i]))  || 
            (!strcmp("MGCP_FeatureCatalogue_TRD2.0_20070425.xml",attributeV[i]))
          )
        )
	  {
        return;
	  } 
	}
  }  

  if(trd1_override==1)
  {
    fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"MGCP_FeatureCatalogue_TRD1.0_20051231.xml\" or \"MGCP_FeatureCatalogue_TRD1.1_20061019.xml\"> }\n",
	  ParsePath(0),attributename);
  }
  else
  {
    fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\"> }\n",
	  ParsePath(0),attributename);
  }

  PrintAttributesToErrFile();

}








void TestS2AttributeList(const char *attributename)
{
  int i;

  for(i=0;i<numattributes;i++)
  {
    if(
        (!strcmp(attributename,attributes[i])) &&
        (  
          (!strcmp("MGCP_FeatureCatalogue_TRD2.0_20070328.xml",attributeV[i]))  || 
          (!strcmp("MGCP_FeatureCatalogue_TRD2.0_20070425.xml",attributeV[i]))  ||
          (!strcmp("MGCP_FeatureCatalogue_TRD3.0_20091103.xml",attributeV[i]))
        )
      )
	{
      return;
	}
  }  

  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\" or \"MGCP_FeatureCatalogue_TRD3.0_20091103.xml\"> }\n",
	  ParsePath(0),attributename);

  PrintAttributesToErrFile();
}



void TestS3AttributeList(const char *attributename)
{
  int i;

  for(i=0;i<numattributes;i++)
  {
    if(
        (!strcmp(attributename,attributes[i])) &&
        (  
          (!strcmp("MGCP_FeatureCatalogue_TRD2.0_20070328.xml",attributeV[i]))  || 
          (!strcmp("MGCP_FeatureCatalogue_TRD2.0_20070425.xml",attributeV[i]))  ||
          (!strcmp("MGCP_FeatureCatalogue_TRD3.0_20091103.xml",attributeV[i]))  ||
          (!strcmp("MGCP_FeatureCatalogue_TRD4.0_20121231.xml",attributeV[i]))  ||
          (!strcmp("MGCP_FeatureCatalogue_TRD4.1_20130628.xml",attributeV[i]))  ||
          (!strcmp("MGCP_FeatureCatalogue_TRD4_v4.4_2016-12-31.xml",attributeV[i]))
        )
      )
	{


	  if(!strcmp("MGCP_FeatureCatalogue_TRD3.0_20091103.xml",attributeV[i])){subregion_version_found = 3;}
      if(!strcmp("MGCP_FeatureCatalogue_TRD4.0_20121231.xml",attributeV[i])){subregion_version_found = 4;}
      if(!strcmp("MGCP_FeatureCatalogue_TRD4.1_20130628.xml",attributeV[i])){subregion_version_found = 41;}
      if(!strcmp("MGCP_FeatureCatalogue_TRD4_v4.4_2016-12-31.xml",attributeV[i])){subregion_version_found = 44;}

      return;
	}
  }  

  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\" or \"MGCP_FeatureCatalogue_TRD3.0_20091103.xml\" or \"MGCP_FeatureCatalogue_TRD4.0_20121231.xml\" or \"MGCP_FeatureCatalogue_TRD4.1_20130628.xml\" or \"MGCP_FeatureCatalogue_TRD4_v4.4_2016-12-31.xml\"> }\n",
	  ParsePath(0),attributename);

  PrintAttributesToErrFile();
}



void TestAttributeList(const char *attributename,const char *attributevalue)
{
  int i;

  for(i=0;i<numattributes;i++)
	{
	   if(
		 (!strcmp(attributename,attributes[i])) &&
		 (!strcmp(attributevalue,attributeV[i]))
		 )
	   { 
		   return;
	   }
	}  

  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\":\"%s\"}\n",
	  ParsePath(0),attributename,attributevalue);

  PrintAttributesToErrFile();

}


extern "C" void WriteExpected(int type,const char *expectstring,const char *required, char *valuefound)
{

  if(type==1)
  {
    fprintf(outerrfile,"Error:  %s\n  %s: %s\n  Found:  \"%s\"\n\n",
	  ParsePath(0),expectstring,required,valuefound);
  }
  else
  {
    fprintf(outerrfile,"Error:  %s\n  %s: %s\n  Found:  %s\n\n",
	  ParsePath(0),expectstring,required,valuefound);
  }

}




int StringMatch(char *a, const char *b, int case_sensitive)
{

  if(case_sensitive==1)
	{
		return (!strcmp(a,b));
	}

#if(USE_DOS==1)

  return (!_stricmp(a,b));

#else

  return (!strcasecmp(a,b));

#endif

}


const char *MakeCaseString(int case_sensitive)
{

  if(case_sensitive==1)
  {
    return " ";
  }

  return " (case insensitive) ";

}



int TestMultibyte(int theattr, char *val1, const char *val2)
{
  int i,testval,insertindex=0;
  char newval1[1000];
  char newval2[1000];

  for(i=0;i<(int)strlen(val2);i++)
  {
    if(val2[i]=='?')
	{
      break;
	}
  }

  if(i==(int)strlen(val2))
  {
    
    return 0;
  }




  

  
  

  if(strlen(val1)!=strlen(val2))
  {
    if(strlen(val1)<strlen(val2))
	{
      fprintf(outerrfile,"Error:  %s:\n   codeListValue \"%s\" does not match Name \"%s\" (expected \"%s\")\n   Note that multi-byte UTF-8 characters may be shown above as \"?\" and are not checked.\n   There is an error because the lengths of the 2 strings are not the same (%d and %d)\n\n",
          ParsePath(0),attributeV[theattr],val1,val2,(int)strlen(val1),(int)strlen(val2));
      return 1;
	}
	else
	{
      
    
      insertindex = 0;
	  for(i=0;i<(int)strlen(val1);i++)
	  {
		testval = (int)val1[i]; 

        if((testval<0)||(testval>127))
		{
          continue;
		}
		else
		{
          newval1[insertindex] = val1[i];
		  insertindex = insertindex + 1;
		}
	  }
	  newval1[insertindex] = '\0';


      insertindex = 0;
	  for(i=0;i<(int)strlen(val2);i++)
	  {
        if(val2[i]=='?')
		{
          continue;
		}
		else
		{
          newval2[insertindex] = val2[i];
		  insertindex = insertindex + 1;
		}
	  }
	  newval2[insertindex] = '\0';
	}
  }


  if(strlen(newval1)!= strlen(newval2))
  {
    fprintf(outerrfile,"Error:  %s:\n   codeListValue \"%s\" does not match Name \"%s\" (expected \"%s\")\n   Note that multi-byte UTF-8 characters are removed and are not checked.\n   There is an error because the lengths of the 2 strings (with multi-byte UTF-8 characters removed) are not the same (%d and %d)\n\n",
            ParsePath(0),attributeV[theattr],newval1,newval2,(int)strlen(newval1),(int)strlen(newval2));
	return 1;
  }

  for(i=0;i<(int)strlen(newval1);i++)
  {
    if(newval2[i]=='?')
	{
      continue;
	}

	if(newval1[i]!=newval2[i])
	{
       fprintf(outerrfile,"Error:  %s:\n   codeListValue \"%s\" does not match Name \"%s\" (expected \"%s\")\n   Note that multi-byte UTF-8 characters are removed and are not checked.\n   There is an error because the 2 strings (with multi-byte UTF-8 characters removed) differ at position %d (%c and %c)\n\n",
            ParsePath(0),attributeV[theattr],newval1,newval2,i+1,newval1[i],newval2[i]);
       return 1;
	}
  }


  return 1;
}


extern "C" void CreateXMLTemplate(char *xmlloc)
{
  FILE *tempfile;
  char filename[1000];
  int i;

  printf("Create XML template file in %s\n",xmlloc);

  sprintf(filename,"%sRefinementTemplate.txt",xmlloc);
  tempfile = fopen(filename,"wt");
  if(tempfile==NULL)
  {
    printf("error: unable to create %s\n",filename);
	return;
  }


  fprintf(tempfile,"SSCALE \"NotSpecified\"  **** Change NotSpecified to 50000 or 100000 to specify 50000 or 100000 should always be used\n");
  fprintf(tempfile,"CLSTAT \"NotSpecified\"  **** Change NotSpecified to string required for CLSTAT\n");
  fprintf(tempfile,"CCPYRT \"NotSpecified\"  **** Change NotSpecified to string required for CCPYRT\n");
  fprintf(tempfile,"SCPYRT \"NotSpecified\"  **** Change NotSpecified to string required for all SCPYRT\n");


  for(i=0;i<ParticipantNationCodesMax2*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD2 \"%s\" \"%s\"\n",ParticipantNationCodes2[i],ParticipantNationCodes2[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax2*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD2 \"%s\" \"%s\"\n",ParticipantAgencyCodes2[i],ParticipantAgencyCodes2[i+1]);
  }

  for(i=0;i<AccuracyEvaluationMethodsMax23*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD2 \"%s\" \"%s\"\n",AccuracyEvaluationMethods23[i],AccuracyEvaluationMethods23[i+1]);
  }

  for(i=0;i<SourceTypeIdsMax2*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD2 \"%s\" \"%s\"\n",SourceTypeIds2[i],SourceTypeIds2[i+1]);
  }

  fprintf(tempfile,"\n\n");

  for(i=0;i<ParticipantNationCodesMax3*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD3 \"%s\" \"%s\"\n",ParticipantNationCodes3[i],ParticipantNationCodes3[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax3*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD3 \"%s\" \"%s\"\n",ParticipantAgencyCodes3[i],ParticipantAgencyCodes3[i+1]);
  }
  for(i=0;i<AccuracyEvaluationMethodsMax23*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD3 \"%s\" \"%s\"\n",AccuracyEvaluationMethods23[i],AccuracyEvaluationMethods23[i+1]);
  }
  for(i=0;i<SourceTypeIdsMax3*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD3 \"%s\" \"%s\"\n",SourceTypeIds3[i],SourceTypeIds3[i+1]);
  }

  fprintf(tempfile,"\n\n");

  for(i=0;i<ParticipantNationCodesMax4*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD4 \"%s\" \"%s\"\n",ParticipantNationCodes4[i],ParticipantNationCodes4[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax4*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD4 \"%s\" \"%s\"\n",ParticipantAgencyCodes4[i],ParticipantAgencyCodes4[i+1]);
  }
  for(i=0;i<AccuracyEvaluationMethodsMax4*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD4 \"%s\" \"%s\"\n",AccuracyEvaluationMethods4[i],AccuracyEvaluationMethods4[i+1]);
  }
  for(i=0;i<SourceTypeIdsMax4*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD4 \"%s\" \"%s\"\n",SourceTypeIds4[i],SourceTypeIds4[i+1]);
  }
  for(i=0;i<VerticalSourceTypeIdsMax4*3;i=i+3)
  {
    fprintf(tempfile,"VerticalSourceTypeId_TRD4 \"%s\" \"%s\"\n",VerticalSourceTypeIds4[i],VerticalSourceTypeIds4[i+1]);
  }


  for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD41 \"%s\" \"%s\"\n",ParticipantNationCodes41[i],ParticipantNationCodes41[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax41*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD41 \"%s\" \"%s\"\n",ParticipantAgencyCodes41[i],ParticipantAgencyCodes41[i+1]);
  }
  for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD41 \"%s\" \"%s\"\n",AccuracyEvaluationMethods41[i],AccuracyEvaluationMethods41[i+1]);
  }
  for(i=0;i<SourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD41 \"%s\" \"%s\"\n",SourceTypeIds41[i],SourceTypeIds41[i+1]);
  }
  for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"VerticalSourceTypeId_TRD41 \"%s\" \"%s\"\n",VerticalSourceTypeIds41[i],VerticalSourceTypeIds41[i+1]);
  }
  

  for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD42 \"%s\" \"%s\"\n",ParticipantNationCodes41[i],ParticipantNationCodes41[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax42*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD42 \"%s\" \"%s\"\n",ParticipantAgencyCodes42[i],ParticipantAgencyCodes42[i+1]);
  }
  for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD42 \"%s\" \"%s\"\n",AccuracyEvaluationMethods41[i],AccuracyEvaluationMethods41[i+1]);
  }
  for(i=0;i<SourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD42 \"%s\" \"%s\"\n",SourceTypeIds41[i],SourceTypeIds41[i+1]);
  }
  for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"VerticalSourceTypeId_TRD42 \"%s\" \"%s\"\n",VerticalSourceTypeIds41[i],VerticalSourceTypeIds41[i+1]);
  }



  for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD43 \"%s\" \"%s\"\n",ParticipantNationCodes41[i],ParticipantNationCodes41[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax43*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD43 \"%s\" \"%s\"\n",ParticipantAgencyCodes43[i],ParticipantAgencyCodes43[i+1]);
  }
  for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD43 \"%s\" \"%s\"\n",AccuracyEvaluationMethods41[i],AccuracyEvaluationMethods41[i+1]);
  }
  for(i=0;i<SourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD43 \"%s\" \"%s\"\n",SourceTypeIds41[i],SourceTypeIds41[i+1]);
  }
  for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"VerticalSourceTypeId_TRD43 \"%s\" \"%s\"\n",VerticalSourceTypeIds41[i],VerticalSourceTypeIds41[i+1]);
  }



  for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantNationCode_TRD44 \"%s\" \"%s\"\n",ParticipantNationCodes41[i],ParticipantNationCodes41[i+1]);
  }
  for(i=0;i<ParticipantAgencyCodesMax44*3;i=i+3)
  {
    fprintf(tempfile,"ParticipantAgencyCode_TRD44 \"%s\" \"%s\"\n",ParticipantAgencyCodes44[i],ParticipantAgencyCodes44[i+1]);
  }
  for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
  {
    fprintf(tempfile,"AccuracyEvaluationMethod_TRD44 \"%s\" \"%s\"\n",AccuracyEvaluationMethods41[i],AccuracyEvaluationMethods41[i+1]);
  }
  for(i=0;i<SourceTypeIdsMax44*3;i=i+3)
  {
    fprintf(tempfile,"SourceTypeId_TRD44 \"%s\" \"%s\"\n",SourceTypeIds44[i],SourceTypeIds44[i+1]);
  }
  for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
  {
    fprintf(tempfile,"VerticalSourceTypeId_TRD44 \"%s\" \"%s\"\n",VerticalSourceTypeIds41[i],VerticalSourceTypeIds41[i+1]);
  }




  fclose(tempfile);

}



void FillInAttributes(CMarkup xml)
{
	numattributes = 0;

	while(1)
	{
      sprintf(attributes[numattributes],"%s",GET_CSTR(xml.GetAttribName(numattributes),1));

      if(strlen(attributes[numattributes])>0)
	  {
		sprintf(attributeV[numattributes],"%s",GET_CSTR(xml.GetAttrib(attributes[numattributes]),1));
	  }
      else
	  { 
        break;
	  } 
      numattributes = numattributes + 1;
	  if(numattributes>10)
	  {
		printf("breaking at %d attributes for %s\n",numattributes,csTag);
		break;
	  }
	} 

}



extern "C" const char *GetACEALECharCode(int thisindex)
{

  

  

  if(thisindex==0)
  {
    return "UNK";  
  }
  if(thisindex==1)
  {
    return "ACA";  
  }
  if(thisindex==2)
  {
    return "ACS";  
  }
  if(thisindex==4)
  {
    return "ANA";  
  }
  if(thisindex==5)
  {
    return "ANS";  
  }
  if(thisindex==15)
  {
    return "EMC";  
  }
  if(thisindex==21)
  {
    return "FZD";  
  }
  if(thisindex==998)
  {
    return "Not Applicable"; 
  }
  if(thisindex==999)
  {
    return "Other"; 
  }

  printf("Error: GAACC for %d\n",thisindex);
  return "Internal Error";
}


extern "C" int GetRealCode(int thisindex)
{

  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    
    return -999;
  }

  

  if(!strcmp(thearray[thisindex],"UNK"))
  {
    return 0;  
  }
  if(!strcmp(thearray[thisindex],"ACA"))
  {
    return 1;  
  }
  if(!strcmp(thearray[thisindex],"ACS"))
  {
    return 2;  
  }
  if(!strcmp(thearray[thisindex],"ANA"))
  {
    return 4;  
  }
  if(!strcmp(thearray[thisindex],"ANS"))
  {
    return 5;  
  }
  if(!strcmp(thearray[thisindex],"EMC"))
  {
    return 15;  
  }
  if(!strcmp(thearray[thisindex],"FZD"))
  {
    return 21;  
  }
  if(!strcmp(thearray[thisindex],"NA"))
  {
    return 998;
  }

  printf("Error: GRC for %d = %s\n",thisindex,thearray[thisindex]);
  return -999;
}


void TestPickList(char *identifier, int limit)
{
  int theattr,thiscode,foundcodelist;
  int case_sensitive = 1,warn_multibyte=0,i,total,matched=0;
  unsigned char allsource[1000];
  unsigned char allaccuracy[1000];

  for(i=0;i<1000;i++)
  {
    allsource[i]   = 0;
    allaccuracy[i] = 0;
  }

  

  if((thearray==ParticipantAgencyCodes2) ||(thearray==ParticipantAgencyCodes3)||
	  (thearray==ParticipantAgencyCodes4)||(thearray==ParticipantAgencyCodes41)||
	  (thearray==ParticipantAgencyCodes42))
  {
    
    case_sensitive  = 0;
    warn_multibyte = 1;
  }
  else if((thearray==ParticipantAgencyCodes43)||(thearray==ParticipantAgencyCodes44))
  {
    warn_multibyte = 1; 
  }


  

  for(theattr=0;theattr<numattributes;theattr++)
  {
    if(!strcmp(attributes[theattr],"codeListValue"))
	{
      break;
	}
  }


 if(theattr>=numattributes)
  {
    

    fprintf(outerrfile,"Error:  %s\n  no codeListValue attribute\n\n",ParsePath(0));
  }
  else
  {
    

	foundcodelist = 0;



    for(thiscode=0;thiscode<limit*3;thiscode=thiscode+3)
	{

      if(!strcmp(thearray[thiscode+2],"0"))
	  {
        
        continue;
	  }

      if(StringMatch(attributeV[theattr],thearray[thiscode],case_sensitive)==1)
	  {
		 if(ACE_ALE>0)
		 {
           if(
			   (
			    ((version==4) &&(thearray==AccuracyEvaluationMethods4 )) || 
			    (  ((version==41)||(version==42)||(version==43)||(version==44))  &&(thearray==AccuracyEvaluationMethods41)) || 
			    (((version!=4)&&(version!=41)&&(version!=42)&&(version!=43)&&(version!=44))&&(thearray==AccuracyEvaluationMethods23))
			   )
			   &&
			   (matched==0) 
			   ) 
		   { 
		     NumSubregionData = NumSubregionData + 1;

		     if(SubregionData==NULL)
			 { 
		       SubregionData = (struct SubregionDataStruct *)malloc(sizeof(struct SubregionDataStruct));
			 } 
		     else
			 { 
               SubregionData = (struct SubregionDataStruct *)realloc(SubregionData,sizeof(struct SubregionDataStruct)*NumSubregionData);
			 } 

		     SubregionData[NumSubregionData-1].subregionid = foundsubregionid;
		     SubregionData[NumSubregionData-1].ace_index = -1;
		     SubregionData[NumSubregionData-1].ale_index = -1;
		     SubregionData[NumSubregionData-1].ace_value = FAKE_ACE_VALUE;
		     SubregionData[NumSubregionData-1].ale_value = FAKE_ACE_VALUE;
		     SubregionData[NumSubregionData-1].scpyrt = NULL;

		     if(ACE_ALE==1)
			 { 
               SubregionData[NumSubregionData-1].ace_index = GetRealCode(thiscode);
			 } 
		     else if(ACE_ALE==2)
			 { 
               SubregionData[NumSubregionData-1].ale_index = GetRealCode(thiscode);
			 } 

		     matched = 1;
		   } 
		 } 

		foundcodelist = 1;

        if(strcmp(thedata,thearray[thiscode+1]))
		{
          if((thearray==SourceTypeIds2)||(thearray==SourceTypeIds3))
		  {
            allsource[thiscode] = 1;
		  }
          else if ((version==4)&&(thearray==AccuracyEvaluationMethods4))
		  {
            allaccuracy[thiscode] = 1;			
		  }
          else if (  ((version==41)||(version==42)||(version==43)||(version==44))  &&(thearray==AccuracyEvaluationMethods41))
		  {
            allaccuracy[thiscode] = 1;			
		  }
          else if (((version!=4)&&(version!=41)&&(version!=42)&&(version!=43)&&(version!=44))&&(thearray==AccuracyEvaluationMethods23))
		  {
            allaccuracy[thiscode] = 1;			
		  }
		  else
		  {
			if(warn_multibyte==1)
			{
				
				if(TestMultibyte(theattr,thedata,thearray[thiscode+1])==1)
				{
				  
				  return;
				}
			}

			
            fprintf(outerrfile,"Error:  %s:\n   codeListValue \"%s\" does not match Name \"%s\" (expected \"%s\")\n\n",
              ParsePath(0),attributeV[theattr],thedata,thearray[thiscode+1]);

			return;
		  }
		}
		else
		{
		  
          return;
		}
	  }
    }


	if(foundcodelist==1)  
	{
      
      fprintf(outerrfile,"Error:  %s:\n   codeListValue \"%s\" does not match Name \"%s\"\n",
        ParsePath(0),attributeV[theattr],thedata);

	  total = 0;
	  for(i=0;i<1000;i++)
	  {
	    total = total + (int)allsource[i] + (int)allaccuracy[i];  
	  }
	  if(total==1)
	  {
	  	  fprintf(outerrfile,"   Expected:"); 
	  }
	  else
	  {
	  	  fprintf(outerrfile,"   Expected one of:\n");
	  }
	  for(i=0;i<1000;i++)
	  {
         if((allsource[i]==1)||(allaccuracy[i]==1))
		 {
		    fprintf(outerrfile,"    \"%s\"\n",thearray[i+1]);
		 }
	  }
      fprintf(outerrfile,"\n\n");
	  return;
	}


	if(foundcodelist==0)
	{
      fprintf(outerrfile,"Error:  %s:\n   unrecognized %s \"%s\"\n",
				ParsePath(0),identifier,attributeV[theattr]);

	  fprintf(outerrfile,"   Expected one of the%svalues: \n   ",MakeCaseString(case_sensitive));

      for(thiscode=0;thiscode<limit*3;thiscode=thiscode+3)
	  {

         if(!strcmp(thearray[thiscode+2],"0"))
		 {  
           
           continue;
		 }  


		 if(thiscode==(limit*3-3))
		 {
		   fprintf(outerrfile,"\"%s\"  ",thearray[thiscode]);
		 }
		 else
		 {
		   fprintf(outerrfile,"\"%s\",  ",thearray[thiscode]);
		 }
	  }
      fprintf(outerrfile,"\n\n");
	}
  }
}





void HandleCodeLists(char *lasttoken)
{
  int i;

  

  if(!strcmp(lasttoken,"MGCP_LanguageCode"))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_LanguageCode");
    thearray = LanguageCodes;
	TestPickList(lasttoken,LanguageCodesMax);
  }
  else if(!strcmp(lasttoken,"MD_CharacterSetCode"))
  {

    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_CharacterSetCode");

	
  }
  else if(!strcmp(lasttoken,"MD_ScopeCode"))
  {
	

	if(strstr(thepath,"subregion"))
	{
      TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_ScopeCode");
      thearray = ScopeCodes1;
	  TestPickList(lasttoken,ScopeCodes1Max);
	}
	else
	{
      TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_ScopeCode");
      thearray = ScopeCodes2;
	  TestPickList(lasttoken,ScopeCodes2Max);
	}
  }
  else if(!strcmp(lasttoken,"MGCP_ParticipantAgency"))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_ParticipantAgency");

	if(version==2)
	{
	  thearray = ParticipantAgencyCodes2;
      TestPickList(lasttoken,ParticipantAgencyCodesMax2);
	}
	else if(version==3)
	{
	  thearray = ParticipantAgencyCodes3;
      TestPickList(lasttoken,ParticipantAgencyCodesMax3);
	}
	else if(version==4)
	{
	  thearray = ParticipantAgencyCodes4;
      TestPickList(lasttoken,ParticipantAgencyCodesMax4);
	}
	else if(version==41)
	{
	  thearray = ParticipantAgencyCodes41;
      TestPickList(lasttoken,ParticipantAgencyCodesMax41);
	}
	else if(version==42)
	{
	  thearray = ParticipantAgencyCodes42;
      TestPickList(lasttoken,ParticipantAgencyCodesMax42);
	}
	else if(version==43)
	{
	  thearray = ParticipantAgencyCodes43;
      TestPickList(lasttoken,ParticipantAgencyCodesMax43);
	}
	else if(version==44)
	{
	  thearray = ParticipantAgencyCodes44;
      TestPickList(lasttoken,ParticipantAgencyCodesMax44);
	}
    else
	{
      printf("Error: BV1 %d\n",version);
	}

  }
  else if(!strcmp(lasttoken,"MGCP_ParticipantNation"))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_ParticipantNation");

	if(version==2)
	{
      thearray = ParticipantNationCodes2;
	  TestPickList(lasttoken,ParticipantNationCodesMax2);
	}
	else if(version==3)
	{
      thearray = ParticipantNationCodes3;
	  TestPickList(lasttoken,ParticipantNationCodesMax3);
	}
	else if(version==4)
	{
      thearray = ParticipantNationCodes4;
	  TestPickList(lasttoken,ParticipantNationCodesMax4);
	}
	else if  ((version==41)||(version==42)||(version==43)||(version==44))  
	{
      thearray = ParticipantNationCodes41;
	  TestPickList(lasttoken,ParticipantNationCodesMax41);
	}
  }
  else if(!strcmp(lasttoken,"CI_RoleCode"))
  {
    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#CI_RoleCode");

	if(version==2)
	{
      thearray = RoleCodesTRD2;
	  TestPickList(lasttoken,RoleCodes2Max);

	  if(strcmp(thedata,"originator"))
	  {
	    WriteExpected(1,"Expected RoleCode","\"originator\"",thedata);

		
	  }
	}
	else if(version==3)
	{
      thearray = RoleCodesTRD3;
	  TestPickList(lasttoken,RoleCodes3Max);

	  if( (strcmp(thedata,"originator")) && (strcmp(thedata,"processor")) )
	  {
	    WriteExpected(1,"Expected RoleCode","\"originator\" or \"processor\"",thedata);

		
	  }
	}
	else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	{
      thearray = RoleCodesTRD4;
	  TestPickList(lasttoken,RoleCodes4Max);

	  if( (strcmp(thedata,"originator")) && (strcmp(thedata,"processor")) )
	  {
	    WriteExpected(1,"Expected RoleCode","\"originator\" or \"processor\"",thedata);

		
	  }
	}
    else
	{
      printf("Error: BV2 %d\n",version);
	}


  }
  else if(!strcmp(lasttoken,"MD_GeometricObjectTypeCode"))
  {
    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_GeometricObjectTypeCode");

    thearray = ObjectTypeCodes;
	TestPickList(lasttoken,ObjectTypeCodesMax);
  }
  else if(!strcmp(lasttoken,"CI_DateTypeCode"))
  {
    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#CI_DateTypeCode");

    thearray = DateTypeCodes;
	TestPickList(lasttoken,DateTypeCodesMax);
  }
  else if(!strcmp(lasttoken,"MD_ClassificationCode"))
  {
    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_ClassificationCode");

    thearray = ClassificationCodes;
	TestPickList(lasttoken,ClassificationCodesMax);
  }
  else if(!strcmp(lasttoken,"MD_RestrictionCode"))
  {
    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_RestrictionCode");

    thearray = RestrictionCodes;
	TestPickList(lasttoken,RestrictionCodesMax);
  }
  else if(!strcmp(lasttoken,"MD_SpatialRepresentationTypeCode"))
  {
    TestAttributeList("codeList","resources/Codelist/gmxCodelists.xml#MD_SpatialRepresentationTypeCode");

    thearray = SpatialRepresentationTypeCodes;
	TestPickList(lasttoken,SpatialRepresentationTypeCodesMax);
  }
  else if(!strcmp(lasttoken,"MGCP_AccuracyEvaluationMethod"))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_AccuracyEvaluationMethod");

	if(version==4)
	{
      thearray = AccuracyEvaluationMethods4;
      TestPickList(lasttoken,AccuracyEvaluationMethodsMax4);
	}
	else if  ((version==41)||(version==42)||(version==43)||(version==44))  
	{
      thearray = AccuracyEvaluationMethods41;
      TestPickList(lasttoken,AccuracyEvaluationMethodsMax41);
	}
	else
	{
      thearray = AccuracyEvaluationMethods23;
      TestPickList(lasttoken,AccuracyEvaluationMethodsMax23);
	}
  }
  else if(!strcmp(lasttoken,"MGCP_SourceTypeId"))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_SourceTypeId");

	if(version==2)
	{
      thearray = SourceTypeIds2;
	  TestPickList(lasttoken,SourceTypeIdsMax2);
	}
	else if(version==3)
	{
      thearray = SourceTypeIds3;
	  TestPickList(lasttoken,SourceTypeIdsMax3);
	}
	else if(version==4)
	{
      thearray = SourceTypeIds4;
	  TestPickList(lasttoken,SourceTypeIdsMax4);
	}
	else if  ((version==41)||(version==42)||(version==43))
	{
      thearray = SourceTypeIds41;
	  TestPickList(lasttoken,SourceTypeIdsMax41);
	}
	else if  (version==44)
	{
      thearray = SourceTypeIds44;
	  TestPickList(lasttoken,SourceTypeIdsMax44);
	}
  }  
  else if(  ((version==3)||((version==4)||(version==41)||(version==42)||(version==43)||(version==44))) && (!strcmp(lasttoken,"MD_TopicCategoryCode"))  )
  {
	for(i=0;i<NumTopicCodes;i++)
	{
      if(!strcmp(thedata,TopicCodes[i]))
	  {
        TopicCodesFound[i] = 1;
		break;
	  }
	}
	if(i==NumTopicCodes)
	{
      fprintf(outerrfile,"Error:  %s:\n   Unrecognized TopicCode \"%s\" found, expected one of:\n  ",
          ParsePath(0),thedata);
      for(i=0;i<NumTopicCodes;i++)
	  {
		if(i==(NumTopicCodes-1))
		{
          fprintf(outerrfile," \"%s\"\n\n",TopicCodes[i]);
		}
		else
		{
          fprintf(outerrfile," \"%s\", ",TopicCodes[i]);
		}
	  }
	}
  }
  else if((version==4)&&(!strcmp(lasttoken,"MGCP_VerticalSourceTypeId")))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_VerticalSourceTypeId");

    thearray = VerticalSourceTypeIds4;
    TestPickList(lasttoken,VerticalSourceTypeIdsMax4);
  }  
  else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(!strcmp(lasttoken,"MGCP_VerticalSourceTypeId")))
  {
    TestAttributeList("codeList","mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_VerticalSourceTypeId");

    thearray = VerticalSourceTypeIds41;
    TestPickList(lasttoken,VerticalSourceTypeIdsMax41);
  }  

}




void CompareStrings(char *s1,char *s2,int type)
{
  int i,len,insertindex;
  char *news1,*news2;


  


  news1 = (char *)malloc(strlen(s1)+5);
  news2 = (char *)malloc(strlen(s2)+5);
  sprintf(news1,"%s",s1);
  sprintf(news2,"%s",s2);

  len = strlen(s1);
  insertindex = 0;
  for(i=0;i<len;i++)
  {
    if((s1[i]>=32)&&(s1[i]<=126))
	{
      if( ((s1[i]==' ')&&(s1[i+1]!=' '))  || (s1[i]!=' ') )
	  {
        news1[insertindex] = s1[i];
	    insertindex = insertindex + 1;
	  }
	}
  }
  news1[insertindex] = '\0';
  if(news1[insertindex-1]==' ')
  {
    news1[insertindex-1] = '\0';
  }


  len = strlen(s2);
  insertindex = 0;
  for(i=0;i<len;i++)
  {
    if((s2[i]>=32)&&(s2[i]<=126))
	{
      if( ((s2[i]==' ')&&(s2[i+1]!=' '))  || (s2[i]!=' ') )
	  {
        news2[insertindex] = s2[i];
	    insertindex = insertindex + 1;
	  }
	}
  }
  news2[insertindex] = '\0';
  if(news2[insertindex-1]==' ')
  {
    news2[insertindex-1] = '\0';
  }




  if(strcmp(news1,news2))
  {
    if(type==1)
	{ 
      WriteExpected(1,"Expected CLSTAT (from refinement file)",CLSTAT_OVERRIDE,thedata);
	} 
    else if(type==2)
	{ 
      WriteExpected(1,"Expected CCPYRT (from refinement file)",CCPYRT_OVERRIDE,thedata);
	} 
    else if(type==3)
	{ 
      WriteExpected(1,"Expected SCPYRT (from refinement file)",SCPYRT_OVERRIDE,thedata);
	} 
  } 

  free(news1);
  free(news2);

}



int IsValidReal(char *testval, double *retval)
{
  if(NUM_CHECK(testval)>0)
  {
    sscanf(testval,"%lf",retval);
	return 1;
  }
  return 0;
}



void Save_CCPYRT()
{
  CCPYRT_SAVE = (char *)malloc(strlen(thedata)+5);
  sprintf(CCPYRT_SAVE,"%s",thedata);
}


void Save_SCPYRT()
{
  NumSubregionData = NumSubregionData + 1;

  if(SubregionData==NULL)
  {
    SubregionData = (struct SubregionDataStruct *)malloc(sizeof(struct SubregionDataStruct));
  } 
  else
  { 
    SubregionData = (struct SubregionDataStruct *)realloc(SubregionData,sizeof(struct SubregionDataStruct)*NumSubregionData);
  } 

  SubregionData[NumSubregionData-1].subregionid = foundsubregionid;
  SubregionData[NumSubregionData-1].ace_index   = -1;
  SubregionData[NumSubregionData-1].ale_index   = -1;
  SubregionData[NumSubregionData-1].ace_value   = FAKE_ACE_VALUE;
  SubregionData[NumSubregionData-1].ale_value   = FAKE_ACE_VALUE;
  SubregionData[NumSubregionData-1].scpyrt      = (char *)malloc(strlen(thedata)+5);
  sprintf(SubregionData[NumSubregionData-1].scpyrt,"%s",thedata);
}



char *GetACEName()
{
  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    return (char *)"aha";
  }

  return (char *)"ace";
}

char *GetALEName()
{
  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    return (char *)"ava";
  }

  return (char *)"ale";
}



void HandleFixedStringValues()
{
  double thisrealval;
  int i;
  char consttemp[1000];


 if(version==2)
 {
   

   if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
   {
     FoundAdditionalStrings[0] = 1;

     if(trd1_override==1)
	 {
       if(
		   (strcmp("MGCP Technical Reference Document v1.0",thedata)) &&
           (strcmp("MGCP Technical Reference Document v1.1",thedata)) &&
           (strcmp("MGCP Technical Reference Document (TRD1v1.0)",thedata)) &&
           (strcmp("MGCP Technical Reference Document (TRD1v1.1)",thedata)) &&
           (strcmp("MGCP Technical Reference Documentation (TRD1v1.0)",thedata)) &&
           (strcmp("MGCP Technical Reference Documentation (TRD1v1.1)",thedata))
		  )
	   {
         WriteExpected(1,"Expected","\
\"MGCP Technical Reference Document v1.0\" or\n\
\"MGCP Technical Reference Document v1.1\" or\n\
\"MGCP Technical Reference Document (TRD1v1.0)\" or\n\
\"MGCP Technical Reference Document (TRD1v1.1)\" or\n\
\"MGCP Technical Reference Documentation (TRD1v1.0)\" or\n\
\"MGCP Technical Reference Documentation (TRD1v1.1)\"",thedata);
	   }
	 }
	 else
	 {
       if(
		   (strcmp("MGCP Technical Reference Document v2.0",thedata)) &&
           (strcmp("MGCP Technical Reference Document (TRD2v2.0)",thedata)) &&
           (strcmp("MGCP Technical Reference Documentation (TRD2v2.0)",thedata))
		  )
	   {
         WriteExpected(1,"Expected","\
\"MGCP Technical Reference Document v2.0\" or\n\
\"MGCP Technical Reference Document (TRD2v2.0)\" or\n\
\"MGCP Technical Reference Documentation (TRD2v2.0)\"",thedata);
	   }
	 }
   }
 }
 else if(version==3) 
  {
    if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
	{
      FoundAdditionalStrings[0] = 1;
      if(strcmp("MGCP Technical Reference Documentation (TRD3v3.0)",thedata))
	  {
	    WriteExpected(1,"Expected","MGCP Technical Reference Documentation (TRD3v3.0)",thedata);
	  }
	}
  }
 else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {

	if(version==4)
	{
      if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
	  { 
        FoundAdditionalStrings[0] = 1;
        if(strcmp("MGCP Technical Reference Documentation (TRD4v4.0)",thedata))
		{ 
	      WriteExpected(1,"Expected","MGCP Technical Reference Documentation (TRD4v4.0)",thedata);
		} 
	  } 
	}
	else if(version==41)
	{
      if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
	  { 
        FoundAdditionalStrings[0] = 1;
        if(strcmp("MGCP Technical Reference Documentation (TRD4v4.1)",thedata))
		{ 
	      WriteExpected(1,"Expected","MGCP Technical Reference Documentation (TRD4v4.1)",thedata);
		} 
	  } 
	}
	else if(version==42)
	{
      if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
	  { 
        FoundAdditionalStrings[0] = 1;
        if(strcmp("MGCP Technical Reference Documentation (TRD4v4.2)",thedata))
		{ 
	      WriteExpected(1,"Expected","MGCP Technical Reference Documentation (TRD4v4.2)",thedata);
		} 
	  } 
	}
	else if(version==43)
	{
      if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
	  { 
        FoundAdditionalStrings[0] = 1;
        if(strcmp("MGCP Technical Reference Documentation (TRD4v4.3)",thedata))
		{ 
	      WriteExpected(1,"Expected","MGCP Technical Reference Documentation (TRD4v4.3)",thedata);
		} 
	  } 
	}
	else if(version==44)
	{
      if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString"))
	  { 
        FoundAdditionalStrings[0] = 1;
        if(strcmp("MGCP Technical Reference Documentation (TRD4v4.4)",thedata))
		{ 
	      WriteExpected(1,"Expected","MGCP Technical Reference Documentation (TRD4v4.4)",thedata);
		} 
	  } 
	}
    else
	{
		printf("Internal Error: Bad TTN: %d\n",version);
	}




   
   
   if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString"))
   {
     if(!strcmp("Limited Distribution (limdis)",thedata))
	 { 
       foundCMSHND = 2;
	 }
	 else if(!strcmp("Unrestricted",thedata))
	 {
       foundCMSHND = 1;
	 }
	 else
	 { 
	   WriteExpected(1,"Expected","\"Limited Distribution (limdis)\" or \"Unrestricted\"",thedata);
       foundCMSHND = -1; 
	 } 
   }

   
   
   if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString"))
   {
    if(!strcmp("Limited Distribution (limdis)",thedata))
	{
       foundCSHNDI = 2;

	   if(foundCMSHND==1)
	   {
         fprintf(outerrfile,"Error:  %s:\n   Found CSHNDI \"Limited Distribution (limdis)\" but CMSHND \"Unrestricted\"\n\n",ParsePath(0));
	   }
	}
	else if(!strcmp("Unrestricted",thedata))
	{
       foundCSHNDI = 1;

	   if(foundCMSHND==2)
	   {
         fprintf(outerrfile,"Error:  %s:\n   Found CMSHND \"Limited Distribution (limdis)\" but CSHNDI \"Unrestricted\"\n\n",ParsePath(0));
	   }
	}
	else
	{
      WriteExpected(1,"Expected","\"Limited Distribution (limdis)\" or \"Unrestricted\"",thedata);
      foundCSHNDI = -1; 
	}
   }


   
   
   if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataConstraints *** MD_SecurityConstraints *** handlingDescription *** CharacterString"))
   {
    if(!strcmp("Limited Distribution (limdis)",thedata))
	{
      max_SMSHND_found = 2;  
      SAVE_SMSHND      = 2;  
	}
	else if(!strcmp("Unrestricted",thedata))
	{
	  if(max_SMSHND_found<1)
	  {
        max_SMSHND_found = 1;  
        SAVE_SMSHND      = 1;  
	  }
	}
	else
	{
	    WriteExpected(1,"Expected","\"Limited Distribution (limdis)\" or \"Unrestricted\"",thedata);
		if(max_SMSHND_found==0)
		{
          max_SMSHND_found = -1; 
		}
	}

   }

  }
 else
 {
   FoundAdditionalStrings[0] = 1;
 }


 if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_AbsoluteExternalPositionalAccuracy *** measureIdentification *** RS_Identifier *** code *** CharacterString"))
 {
   if(StringMatch(thedata,GetACEName(),1))
   {

	 ACE_ALE = 1;
   }
   else if(StringMatch(thedata,GetALEName(),1))
   {

	 ACE_ALE = 2;
   }
   else if(StringMatch(thedata,GetACEName(),0))
   {
     WriteExpected(1,"Expected",GetACEName(),thedata);

     printf("Error: Found %s = %s for subregion %d\n",GetACEName(),thedata,foundsubregionid);
	 ACE_ALE = 1;
   }
   else if(StringMatch(thedata,GetALEName(),0))
   {
     WriteExpected(1,"Expected",GetALEName(),thedata);

     printf("Error: Found %s = %s for subregion %d\n",GetALEName(),thedata,foundsubregionid);
	 ACE_ALE = 2;
   }
   else
   {
     char *tester;

	 tester = (char *)malloc(strlen(thedata)+5);

     sprintf(tester,"%s",thedata);
	 tester[3] = '\0';

     if(StringMatch(tester,GetACEName(),0))
	 { 
       WriteExpected(1,"Expected",GetACEName(),thedata);

       printf("Error: Found %s = %s for subregion %d\n",GetACEName(),thedata,foundsubregionid);
	   ACE_ALE = 1;
	 } 
     else if(StringMatch(tester,GetALEName(),0))
	 { 
       WriteExpected(1,"Expected",GetALEName(),thedata);

       printf("Error: Found %s = %s for subregion %d\n",GetALEName(),thedata,foundsubregionid);
	   ACE_ALE = 2;
	 }
	 else
	 {
       if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	   {
         WriteExpected(0,"Expected","\"aha\" or \"ava\"",thedata);
	   }
	   else
	   {
         WriteExpected(0,"Expected","\"ace\" or \"ale\"",thedata);
	   }
       ACE_ALE = 0;
	 }
	 free(tester);
   }
 }


 if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_AbsoluteExternalPositionalAccuracy *** result *** DQ_QuantitativeResult *** value *** Record"))
 {
   
   if((IsValidReal(thedata,&thisrealval))&&(ACE_ALE>0))
   {
     NumSubregionData = NumSubregionData + 1;

     if(SubregionData==NULL)
	 {
       SubregionData = (struct SubregionDataStruct *)malloc(sizeof(struct SubregionDataStruct));
	 } 
     else
	 { 
       SubregionData = (struct SubregionDataStruct *)realloc(SubregionData,sizeof(struct SubregionDataStruct)*NumSubregionData);
	 } 

     SubregionData[NumSubregionData-1].subregionid = foundsubregionid;
     SubregionData[NumSubregionData-1].ace_index   = -1;
     SubregionData[NumSubregionData-1].ale_index   = -1;
     SubregionData[NumSubregionData-1].ace_value   = FAKE_ACE_VALUE;
     SubregionData[NumSubregionData-1].ale_value   = FAKE_ACE_VALUE;
     SubregionData[NumSubregionData-1].scpyrt      = NULL;

	 if(ACE_ALE==1)
	 {
       SubregionData[NumSubregionData-1].ace_value = thisrealval;
	   if((thisrealval<1)||(thisrealval>25))
	   {
         WriteExpected(1,"Expected","SACEVL 1-25",thedata);
	   }
 	 }
	 else if(ACE_ALE==2)
	 {
       SubregionData[NumSubregionData-1].ale_value = thisrealval;
	   if((thisrealval<=0)||(thisrealval>=100))
	   {
         WriteExpected(1,"Expected","SALEVL >0...<100",thedata);
	   }
 	 }

   }
   else if(IsValidReal(thedata,&thisrealval)==0)
   {
	 if(ACE_ALE==1)
	 {
       WriteExpected(1,"Expected","Real value for SACEVL",thedata);
	 }
	 else
	 {
       WriteExpected(1,"Expected","Real value for SALEVL",thedata);
	 }
   }
 }


 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** referenceSystemInfo *** MD_ReferenceSystem *** referenceSystemIdentifier *** RS_Identifier *** code *** Anchor"))
 {
   if(
	   (strcmp(thedata,"WGS 84 2D-Geographic East-North"))  &&
	   (strcmp(thedata,"WGS 84 3D-Geographic East-North"))
	  )
   {
     WriteExpected(1,"Expected","\"WGS 84 2D-Geographic East-North\" or \"WGS 84 3D-Geographic East-North\"",thedata);

	 
   }
   else if(!strcmp(thedata,"WGS 84 2D-Geographic East-North"))
   {
      TestAttributeList("xlink:href","mgcp/crs/mgcp_gmxCrs.xml#WGS84E_2D");
   }
   else if(!strcmp(thedata,"WGS 84 3D-Geographic East-North"))
   {
      TestAttributeList("xlink:href","mgcp/crs/mgcp_gmxCrs.xml#WGS84E_3D");
   }

   FoundAdditionalStrings[2] = 1;
 }


 if(version==2)
 {
   for(i=0;i<NumFixedStringsTRD2*2;i=i+2)
   {
     if(!strcmp(FixedStringsTRD2[i],thepath))
	 {
       if(strcmp(FixedStringsTRD2[i+1],thedata))
	   {
         sprintf(consttemp,"%s",FixedStringsTRD2[i+1]);
         WriteExpected(1,"Expected",consttemp,thedata);

		 
	   }
       FoundFixedStringTRD2[i/2] = FoundFixedStringTRD2[i/2] + 1;
	 } 
   }
 }
 else if(version==3)
 {
   for(i=0;i<NumFixedStringsTRD3*2;i=i+2)
   {
     if(!strcmp(FixedStringsTRD3[i],thepath))
	 {
       if(strcmp(FixedStringsTRD3[i+1],thedata))
	   {
         sprintf(consttemp,"%s",FixedStringsTRD3[i+1]);
         WriteExpected(1,"Expected",consttemp,thedata);

		
	   }
       FoundFixedStringTRD3[i/2] = FoundFixedStringTRD3[i/2] + 1;
	 } 
   }
 }
 else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
 {
   for(i=0;i<NumFixedStringsTRD4*2;i=i+2)
   {
     if(!strcmp(FixedStringsTRD4[i],thepath))
	 {
       if(!strcmp(FixedStringsTRD4[i+1],"SENTINEL1"))  
	   {
          if((version==4)&&(strcmp(thedata,"Limited Distribution.  Official use only by MGCP members.")))
		  {
            WriteExpected(1,"Expected","Limited Distribution.  Official use only by MGCP members.",thedata);
		  }
		  else if((version==41)||(version==42)||(version==43))
		  {
            if(!strcmp(thedata,"Limited Distribution.  For Official Use Only by MGCP members."))
			{
              SAVE_STIERN = 2;
			}
			else if(!strcmp(thedata,"Unrestricted.  For Public Release."))
			{
              SAVE_STIERN = 1;
			}
			else
			{
              WriteExpected(1,"Expected",\
"\"Limited Distribution.  For Official Use Only by MGCP members.\" or \"Unrestricted.  For Public Release.\"",thedata);
			}
		  }
		  else if(version==44)
		  {
            if(!strcmp(thedata,"Limited Distribution. For Official Use Only by MGCP members."))
			{
              SAVE_STIERN = 2;
			}
			else if(!strcmp(thedata,"Unrestricted. For Public Release."))  
			{
              SAVE_STIERN = 1;
			}
			else
			{
              WriteExpected(1,"Expected",\
"\"Limited Distribution. For Official Use Only by MGCP members.\" or \"Unrestricted. For Public Release.\"",thedata);
			}
		  }
	   }
	   else
	   {
         if(strcmp(FixedStringsTRD4[i+1],thedata))
		 {
           sprintf(consttemp,"%s",FixedStringsTRD4[i+1]);
           WriteExpected(1,"Expected",consttemp,thedata);

		  
		 } 
	   }
       FoundFixedStringTRD4[i/2] = FoundFixedStringTRD4[i/2] + 1;  
	 }
   }
 }



 if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** supplementalInformation *** CharacterString"))
 {
   sprintf(saveSTYPEU,"%s",thedata);
   stypeu_found = 1;

   if((version==41)||(version==42)||(version==43)||(version==44))
   {
     
	 if(strcmp(thedata,"Complete Update"))
	 {
       WriteExpected(1,"Expected","STYPEU \"Complete Update\"",thedata);
	 }
   }
 }



 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** statement *** CharacterString"))
 {
   sprintf(saveCLSTAT,"%s",thedata);
   clstat_found = 1;

   if(use_clstat==1)
   {
     CompareStrings(thedata,CLSTAT_OVERRIDE,1);
   }
 }
 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_LegalConstraints *** useLimitation *** CharacterString"))
 {
   Save_CCPYRT();

   if(use_ccpyrt==1)
   {
     CompareStrings(thedata,CCPYRT_OVERRIDE,2);
   }
 }
 if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** resourceConstraints *** MD_LegalConstraints *** useLimitation *** CharacterString"))
 {

   Save_SCPYRT();

   if(use_scpyrt==1)
   {
     CompareStrings(thedata,SCPYRT_OVERRIDE,3);
   }
 }

 if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** spatialResolution *** MD_Resolution *** equivalentScale *** MD_RepresentativeFraction *** denominator *** Integer"))
 {
   if((use_sscale==1)&&(strcmp(thedata,SSCALE_OVERRIDE)))
   {
     
     WriteExpected(1,"Expected SSCALE (from refinement file)",SSCALE_OVERRIDE,thedata);
   }
   else if( (strcmp(thedata,"50000")) && (strcmp(thedata,"100000")) )
   {
     WriteExpected(1,"Expected","\"50000\" or \"100000\"",thedata);

	 
   }
 }
} 


void HandleGAITVersion(int expected_version)
{
  int found_version,len;
  static int curr_version = -1;
  char tempstr[100];


  if(curr_version==-1)
  {
    sscanf(VersionStr,"%d",&curr_version);
    printf("Got current GAIT version %d\n",curr_version);
 
    if(curr_version<17)
	{ 
      curr_version = 17;
	} 
  }


  if(
	  (((version==4)||(version==41)||(version==42)||(version==43)||(version==44)) && (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** measureDescription *** CharacterString"))) ||
	  ((version==3) && (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** measureDescription *** CharacterString"))) ||
	  ((version==2) && (!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** measureDescription *** CharacterString")))
	)
  {
	FoundAdditionalStrings[1] = 1;
    len = strlen(thedata);

	if(!strcmp(thedata,"11a"))
	{
      return;
	}

	if(len!=2)
	{
	  sprintf(tempstr,"10, 11a, 12, 13 or 15-%d",curr_version);

      WriteExpected(1,"Expected GAIT version",tempstr,thedata);

	  
	}
	else
	{
      if(
		  (thedata[0]<49) || 
		  (thedata[0]>57) || 
		  (thedata[1]<48) || 
		  (thedata[1]>57)
		)
	  {
	    sprintf(tempstr,"10-13 or 15-%d",curr_version);

        WriteExpected(1,"Expected GAIT version",tempstr,thedata);

	  
	  }
	  else
	  {
        sscanf(thedata,"%d",&found_version);
        if(
            (found_version<10)  ||
            (found_version==14) ||
            (found_version>curr_version)
		  )
		{
	      sprintf(tempstr,"10-13 or 15-%d",curr_version);

          WriteExpected(1,"Expected GAIT version (SVVERS)",tempstr,thedata);
	  
		  
		}

		if(expected_version>0)
		{
		  if(found_version!=expected_version)
		  {
			 sprintf(tempstr,"%d",expected_version);
             WriteExpected(0,"Expected GAIT version (SVVERS)",tempstr,thedata);
		  }
		}

	  }
	}
  }
}


int ValidateDate(int checklen,int *year,int *month,int *day)
{
  int i,len,yearint,monthint,dayint,maxdays,thisyear,thismonth,thisday;
  char yearchar[10],monthchar[5],daychar[5],message[20],tempstr[100];

  if(checklen==1)
  {
    sprintf(message,"Date");
  }
  else
  {
    sprintf(message,"DateTime");
  }

  len = strlen(thedata);

  if(checklen==1)
  {
    if(len!=10)
	{
	  sprintf(tempstr,"%s value \"%s\" (length %d)",message,thedata,len);

      WriteExpected(0,"Expected","format YYYY-MM-DD (length 10)",tempstr);

	  

	  return 0;
	}
  }
  else
  {
    len = 10;
  }

  for(i=0;i<len;i++)
  {
	if((i==4)||(i==7))
	{
	  if(thedata[i]!='-')
	  {
	    sprintf(tempstr,"%s value \"%s\" found (\"%c\" found in position %d.  expected '-')",
			message,thedata,thedata[i],i+1);

        WriteExpected(0,"Expected","format YYYY-MM-DD",tempstr);

		
	    return 0;
	  }
	}
	else
	{
      if((thedata[i]<48) || (thedata[i]>57) )
	  {
	    sprintf(tempstr,"%s value \"%s\" found (\"%c\" found in position %d.  expected a number)",
			message,thedata,thedata[i],i+1);

        WriteExpected(0,"Expected","format YYYY-MM-DD",tempstr);

		
	    return 0;
	  }
	}
  }

  yearchar[0] = thedata[0];
  yearchar[1] = thedata[1];
  yearchar[2] = thedata[2];
  yearchar[3] = thedata[3];
  yearchar[4] = '\0';

  monthchar[0] = thedata[5];
  monthchar[1] = thedata[6];
  monthchar[2] = '\0';

  daychar[0] = thedata[8];
  daychar[1] = thedata[9];
  daychar[2] = '\0';

  sscanf(yearchar, "%d",&yearint);
  sscanf(monthchar,"%d",&monthint);
  sscanf(daychar,  "%d",&dayint);


  *year  = yearint;
  *month = monthint;
  *day   = dayint;

  if(
	  (monthint==1) || (monthint==3) || (monthint==5) || (monthint==7) ||
	  (monthint==8) || (monthint==10)|| (monthint==12)
	)
  {
	  maxdays = 31;
  }
  else if((monthint==4) || (monthint==6) || (monthint==9) || (monthint==11))
  {
    maxdays = 30;
  }
  else if(monthint==2)
  {
	if((yearint%4==0)&&((yearint%100!=0)||(yearint%400==0))) 
	{
	  maxdays = 29;
	}
	else
	{
      maxdays = 28;
	}
  }
  else
  {    
    WriteExpected(1,"Expected","format YYYY-MM-DD (invalid month found)",thedata);

	
	return 0;
  }

  if((dayint>maxdays)||(dayint<1))
  {
    WriteExpected(1,"Expected","format YYYY-MM-DD (invalid day for the given month found)",thedata);

	
	return 0;
  }


  if(GetTimeValues(&thisyear,&thismonth,&thisday))
  {
    if( 
		(yearint>thisyear)                                                 ||
		((yearint==thisyear) && (monthint>thismonth))                      ||
        ((yearint==thisyear) && (monthint==thismonth) && (dayint>thisday))
	  )
	{
      fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" is a date after today's date (%d-%02d-%02d)\n\n",
        ParsePath(0),message,thedata,thisyear,thismonth,thisday);
	  return 0;
	}

  }


  if(yearint<1800)
  {
    fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (very old year %d)\n\n",
      ParsePath(0),message,thedata,yearint);
	return 0;
  }





  if(!strcmp(thepath,"path MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** date *** CI_Date *** date *** Date"))
  {
    

	if(trd1_override==1)
	{
      if(
		  (yearint<2006) ||
		  ((yearint==2006) && (monthint<5))   ||
		  ((yearint==2006) && (monthint==5) && (dayint<17))
		)
	  {
        fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (TRD1 creation date must be at least May 17, 2006)\n\n",
          ParsePath(0),message,thedata);
	    return 0;
	  }
	}
	else if((version==2)&&(yearint<2006))
	{
      fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (invalid year %d.  TRD2 creation date must be at least 2006)\n\n",
        ParsePath(0),message,thedata,yearint);
	  return 0;
	}
	else if((version==3)&&(yearint<2009))
	{
      fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (invalid year %d.  TRD3 creation date must be at least 2009)\n\n",
        ParsePath(0),message,thedata,yearint);
	  return 0;
	}
	else if(((version==4)||(version==41)||(version==42)||(version==43)||(version==44))&&(yearint<2012))
	{
      fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (invalid year %d.  TRD4 creation date must be at least 2012)\n\n",
        ParsePath(0),message,thedata,yearint);
	  return 0;
	}
  }


  if( (version==2) && (yearint<2006) && (!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** dateTime *** DateTime")) )
  {
    fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (invalid year %d.  TRD2 validation date stamp CVDATE must be at least 2006)\n\n",
      ParsePath(0),message,thedata,yearint);
    return 0;
  }
  else if( (version==3) && (yearint<2009) && (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** dateTime *** DateTime")) )
  {
    fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (invalid year %d.  TRD3 validation date stamp SVDATE must be at least 2009)\n\n",
      ParsePath(0),message,thedata,yearint);
    return 0;
  }
  else if( ((version==4)||(version==41)||(version==42)||(version==43)||(version==44)) && (yearint<2012) && (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** dateTime *** DateTime")) )
  {
    fprintf(outerrfile,"Error:  %s:\n   %s value \"%s\" found (invalid year %d.  TRD4 validation date stamp SVDATE must be at least 2012)\n\n",
      ParsePath(0),message,thedata,yearint);
    return 0;
  }

  return 1;
}


int ValidateDateTime(int *year,int *month,int *day,int *hour,int *minute,int *second)
{
  int i,len,hourint,minuteint,secondint,lyear,lmonth,lday;
  char hourchar[10],minutechar[5],secondchar[5],tempstr[1000];

  len = strlen(thedata);

  if(len!=20)
  {
	sprintf(tempstr,"\"%s\" (length %d)",thedata,len);
    WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ (length 20)",tempstr);

	
    return 0;
  }

  if(ValidateDate(0,&lyear,&lmonth,&lday)==1)
  {
    

    for(i=10;i<len;i++)
	{
 	  if(i==10)
	  { 
	    if(thedata[i]!='T')
		{
          sprintf(tempstr,"\"%s\" (\"%c\" found in position %d.  expected '-')",thedata,thedata[i],i+1);
          WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

		  
	      return 0;
		} 
	  }
	  else if((i==13)||(i==16))
	  { 
	    if(thedata[i]!=':')
		{
          sprintf(tempstr,"\"%s\" (\"%c\" found in position %d.  expected ':')",thedata,thedata[i],i+1);
          WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

		  
	      return 0;
		} 
	  }
	  else if(i==19)
	  { 
	    if(thedata[i]!='Z')
		{
          sprintf(tempstr,"\"%s\" (\"%c\" found in position %d.  expected 'Z')",thedata,thedata[i],i+1);
          WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

		  
	      return 0;
		} 
	  }
	  else
	  {
        if((thedata[i]<48) || (thedata[i]>57) )
		{
          sprintf(tempstr,"\"%s\" (\"%c\" found in position %d.  expected a number)",thedata,thedata[i],i+1);
          WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

		  

	      return 0;
		} 
	  } 
	} 
  }

  hourchar[0] = thedata[11];
  hourchar[1] = thedata[12];
  hourchar[2] = '\0';

  minutechar[0] = thedata[14];
  minutechar[1] = thedata[15];
  minutechar[2] = '\0';

  secondchar[0] = thedata[17];
  secondchar[1] = thedata[18];
  secondchar[2] = '\0';


  sscanf(hourchar,  "%d",&hourint);
  sscanf(minutechar,"%d",&minuteint);
  sscanf(secondchar,"%d",&secondint);
  
  if((hourint<0)||(hourint>23))
  {
    sprintf(tempstr,"DateTime value \"%s\" (invalid hour %d, valid are 00-23)",thedata,hourint);
    WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

	

	return 0;
  }
  if((minuteint<0)||(minuteint>59))
  {
    sprintf(tempstr,"DateTime value \"%s\" (invalid minute %d, valid are 00-59)",thedata,minuteint);
    WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

	

	return 0;
  }
  if((secondint<0)||(secondint>59))
  {
    sprintf(tempstr,"DateTime value \"%s\" (invalid second %d, valid are 00-59)",thedata,secondint);
    WriteExpected(0,"Expected","DateTime format YYYY-MM-DDThh:mm:ssZ",tempstr);

	

	return 0;
  }

  *year   = lyear;
  *month  = lmonth;
  *day    = lday;
  *hour   = hourint;
  *minute = minuteint;
  *second = secondint;

  return 1;
}




int CompareSVDates(int type)
{
  

  if(type==1)
  {
    if(SVDATyear<CCDATEyear)
	{ 
      return 0;
	} 
    else if(SVDATyear==CCDATEyear)
	{ 
      if(SVDATmonth<CCDATEmonth)
	  {  
        return 0;
	  } 
	  else if(SVDATmonth==CCDATEmonth)
	  { 
        if(SVDATday<CCDATEday)
		{  
          return 0;
		}	  
	  } 
	} 
  }
  else if(type==2)
  {
    if(SVDATyear<CMDATEyear)
	{ 
      return 0;
	} 
    else if(SVDATyear==CMDATEyear)
	{ 
      if(SVDATmonth<CMDATEmonth)
	  {  
        return 0;
	  } 
	  else if(SVDATmonth==CMDATEmonth)
	  { 
        if(SVDATday<CMDATEday)
		{  
          return 0;
		}	  
	  } 
	} 
  }
  else if(type==3)
  {
    if(SVDATyear<CEDDATyear)
	{ 
      return 0;
	} 
    else if(SVDATyear==CEDDATyear)
	{ 
      if(SVDATmonth<CEDDATmonth)
	  {  
        return 0;
	  } 
	  else if(SVDATmonth==CEDDATmonth)
	  { 
        if(SVDATday<CEDDATday)
		{  
          return 0;
		}	  
	  } 
	} 
  }
  else
  {
    printf("Internal error: CSDA called with %d\n",type);
  }


  return 1;
}



int CompareSCSEDates()
{
  if(SEDDATyear<SCDATEyear)
  {
    return 0;
  }
  else if(SEDDATyear==SCDATEyear)
  {
    if(SEDDATmonth<SCDATEmonth)
	{ 
      return 0;
	}
	else if(SEDDATmonth==SCDATEmonth)
	{
      if(SEDDATday<SCDATEday)
	  { 
        return 0;
	  }	 
	}
  }

  return 1;
}


int CompareSMSEDates()
{
  if(SEDDATyear<SMDATEyear)
  {
    return 0;
  }
  else if(SEDDATyear==SMDATEyear)
  {
    if(SEDDATmonth<SMDATEmonth)
	{ 
      return 0;
	}
	else if(SEDDATmonth==SMDATEmonth)
	{
      if(SEDDATday<SMDATEday)
	  { 
        return 0;
	  }	 
	}
  }
  return 1;
}


void HandleDates(char *lasttoken)
{
  char tempstr[100];
  int year,month,day,hour,minute,second;


  if(!strcmp(lasttoken,"DateTime"))
  {
    if(ValidateDateTime(&year,&month,&day,&hour,&minute,&second)==1)
	{

	  if((version==2)&&(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** dateTime *** DateTime")))
	  {
        SVDATyear   = year;
	    SVDATmonth  = month;
	    SVDATday    = day;
        foundSVDATE = 1;
		sprintf(SAVESVDAT,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVESVDAT))<950)
		{
		  sprintf(tempstr,"Subregion %d, SVDATE: %s\n",subregionfound,SAVESVDAT);
		  strcat(SavedSubregionDates,tempstr);
		}

        if(foundCCDATE==1)
		{ 
          if(CompareSVDates(1)==0)
		  { 
		    if(version==2)
			{ 
              fprintf(outerrfile,"Error:  %s\n   CVDATE \"%s\" earlier than CCDATE \"%s\"\n\n",ParsePath(0),SAVESVDAT,SAVECCDATE);
			} 
		  }  
		}
	   else
	   {
         printf("did not find CC date in time\n");
	   } 
	

	  }
	  else if((version==3)&&(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** dateTime *** DateTime")))
	  {
        SVDATyear   = year;
	    SVDATmonth  = month;
	    SVDATday    = day;
        foundSVDATE = 1;
		sprintf(SAVESVDAT,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVESVDAT))<950)
		{
		  sprintf(tempstr,"Subregion %d, SVDATE: %s\n",subregionfound,SAVESVDAT);
		  strcat(SavedSubregionDates,tempstr);
		}

        if(foundCCDATE==1)
		{ 
          if(CompareSVDates(1)==0)
		  { 
		    if(version==3)
			{ 
              fprintf(outerrfile,"Error:  %s\n   SVDATE \"%s\" earlier than CCDATE \"%s\"\n\n",ParsePath(0),SAVESVDAT,SAVECCDATE);
			} 
		  }  
		}
	  }
	  else if(((version==4)||(version==41)||(version==42)||(version==43)||(version==44))&&(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** dateTime *** DateTime")))
	  {
        SVDATyear   = year;
	    SVDATmonth  = month;
	    SVDATday    = day;
        foundSVDATE = 1;
		sprintf(SAVESVDAT,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVESVDAT))<950)
		{
		  sprintf(tempstr,"Subregion %d, SVDATE: %s\n",subregionfound,SAVESVDAT);
		  strcat(SavedSubregionDates,tempstr);
		}


		if((version==4)||(version==41)||(version==42)||(version==43))
		{
          if(foundCCDATE==1)
		  {  
            if(CompareSVDates(1)==0)
			{  
              fprintf(outerrfile,"Error:  %s\n   SVDATE \"%s\" earlier than CCDATE \"%s\"\n\n",ParsePath(0),SAVESVDAT,SAVECCDATE);
			} 
		  }  
		}
		else if(version==44)
		{
          if(foundCCDATE==1)
		  {  
            if(CompareSVDates(1)==0)
			{ 
              fprintf(outerrfile,"Error:  %s\n   SVDATE \"%s\" earlier than CCDATE \"%s\"\n\n",ParsePath(0),SAVESVDAT,SAVECCDATE);
			} 
		  }
          if(foundCMDATE==1)
		  {  
            if(CompareSVDates(2)==0)
			{ 
              fprintf(outerrfile,"Error:  %s\n   SVDATE \"%s\" earlier than CMDATE \"%s\"\n\n",ParsePath(0),SAVESVDAT,SAVECMDATE);
			} 
		  }
          if(foundCEDDAT==1)
		  {  
            if(CompareSVDates(3)==0)
			{ 
              fprintf(outerrfile,"Error:  %s\n   SVDATE \"%s\" earlier than CEDDAT \"%s\"\n\n",ParsePath(0),SAVESVDAT,SAVECEDDAT);
			} 
		  }  
		}
	  }

	 
      if (!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** date *** CI_Date *** date *** DateTime"))
	  {
        

	    if(version==2)
		{
		  if(trd1_override==1)
		  {
            if(
				(strcmp(thedata,"2005-12-31T00:00:00Z")) &&
				(strcmp(thedata,"2006-10-19T00:00:00Z"))
				)
			{
              WriteExpected(1,"Expected","Specification Date (CVSPCD) TRD 1.0 or 1.1 date \"2005-12-31T00:00:00Z\" or \"2006-10-19T00:00:00Z\"",thedata);
              return;
			} 
		  }
		  else
		  {
            if(
				(strncmp(thedata,"2007-03-28",10)) &&
				(strncmp(thedata,"2007-04-25",10))
			  )
			{  
              WriteExpected(1,"Expected","Specification Date (CVSPCD) TRD2 date \"2007-03-28T00:00:00Z\" or \"2007-04-25T00:00:00Z\"",thedata);

			  
              return;
			}  
            if(
				(strcmp(thedata,"2007-03-28T00:00:00Z")) &&
				(strcmp(thedata,"2007-04-25T00:00:00Z"))
			  )
			{
              WriteExpected(1,"Expected","Specification Date (CVSPCD) TRD2 date \"2007-03-28T00:00:00Z\" or \"2007-04-25T00:00:00Z\"",thedata);

			  
              return;
			} 
		  } 
		} 
	  }

      if(
		  (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** date *** CI_Date *** date *** DateTime")) ||
		  (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_AbsoluteExternalPositionalAccuracy *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** date *** CI_Date *** date *** DateTime"))
		)
	  {
        
	    if(version==3)
		{ 
          if(strncmp(thedata,"2009-11-03",10))
		  { 
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD3 date \"2009-11-03T00:00:00Z\"",thedata);

			
            return;
		  } 
          if(strcmp(thedata,"2009-11-03T00:00:00Z"))
		  { 
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD3 date \"2009-11-03T00:00:00Z\"",thedata);

			

            return;
		  }
		}

	    else if(version==4)
		{
          if(strncmp(thedata,"2012-12-31",10))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD4 date \"2012-12-31T00:00:00Z\"",thedata);
            return;
		  }
          if(strcmp(thedata,"2012-12-31T00:00:00Z"))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD4 date \"2012-12-31T00:00:00Z\"",thedata);
            return;
		  }
		}
	    else if(version==41)
		{
          if(strncmp(thedata,"2013-06-28",10))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD41 date \"2013-06-28T00:00:00Z\"",thedata);
            return;
		  }
          if(strcmp(thedata,"2013-06-28T00:00:00Z"))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD41 date \"2013-06-28T00:00:00Z\"",thedata);
            return;
		  }
		}
	    else if(version==42)
		{
          if(strncmp(thedata,"2014-06-20",10))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD42 date \"2014-06-20T00:00:00Z\"",thedata);
            return;
		  }
          if(strcmp(thedata,"2014-06-20T00:00:00Z"))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD42 date \"2014-06-20T00:00:00Z\"",thedata);
            return;
		  }
		}
	    else if(version==43)
		{
          if(strncmp(thedata,"2015-07-01",10))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD43 date \"2015-07-01T00:00:00Z\"",thedata);
            return;
		  }
          if(strcmp(thedata,"2015-07-01T00:00:00Z"))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD43 date \"2015-07-01T00:00:00Z\"",thedata);
            return;
		  }
		}
	    else if(version==44)
		{
          if(strncmp(thedata,"2016-12-31",10))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD44 date \"2016-12-31T00:00:00Z\"",thedata);
            return;
		  }
          if(strcmp(thedata,"2016-12-31T00:00:00Z"))
		  {
            WriteExpected(1,"Expected","Specification Date (SVSPCD) TRD44 date \"2016-12-31T00:00:00Z\"",thedata);
            return;
		  }
		}
	  }
	}
  }
  else if(!strcmp(lasttoken,"Date"))
  {
    if(ValidateDate(1,&year,&month,&day)==1)
	{
      if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** date *** CI_Date *** date *** Date"))
	  {
        if(trd1_override==1)
		{
          if(  (strcmp(thedata,"2005-12-31"))  &&  (strcmp(thedata,"2006-10-19"))  )
		  {
            fprintf(outerrfile,"Error:  %s:\n   Found: CFPDTD %s\n   Expected:\"2005-12-31\" or \"2006-10-19\"\n\n",
              ParsePath(0),thedata);
		  }
		}
		else if(version==2)
		{
          if(
			  (strcmp(thedata,"2007-03-28")) &&
			  (strcmp(thedata,"2007-04-25"))
			)
		  {
             fprintf(outerrfile,"Error:  %s:\n   Found: CFPDTD %s\n   Expected: \"2007-03-28\" or \"2007-04-25\"\n\n",
              ParsePath(0),thedata);
		  }
		}
	  }

      if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** date *** CI_Date *** date *** Date"))
	  {
        CCDATEyear   = year;
		CCDATEmonth  = month;
		CCDATEday    = day;
		foundCCDATE  = 1;
		sprintf(SAVECCDATE,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVECCDATE))<950)
		{
		  sprintf(tempstr,"CCDATE: %s\n",SAVECCDATE);
		  strcat(SavedSubregionDates,tempstr);
		}
	  }
      else if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** editionDate *** Date"))
	  {
        CEDDATyear   = year;
	    CEDDATmonth  = month;
	    CEDDATday    = day;
        foundCEDDAT = 1;
		sprintf(SAVECEDDAT,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVECEDDAT))<950)
		{
		  sprintf(tempstr,"CEDDAT: %s\n",SAVECEDDAT);
		  strcat(SavedSubregionDates,tempstr);
		}
	  }
	  else if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dateStamp *** Date"))
	  {
        CMDATEyear   = year;
	    CMDATEmonth  = month;
	    CMDATEday    = day;
        foundCMDATE = 1;
		sprintf(SAVECMDATE,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVECMDATE))<950)
		{
		  sprintf(tempstr,"CMDATE: %s\n",SAVECMDATE);
		  strcat(SavedSubregionDates,tempstr);
		}


	  }

  	  else if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dateStamp *** Date"))
	  {
        SMDATEyear   = year;
	    SMDATEmonth  = month;
	    SMDATEday    = day;
        SMDATE_found = 1;
		sprintf(SAVESMDATE,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVESMDATE))<950)
		{
		  sprintf(tempstr,"Subregion %d, SMDATE: %s\n",subregionfound,SAVESMDATE);
		  strcat(SavedSubregionDates,tempstr);
		}


		if(SEDDAT_found==1)
		{
          
          if(CompareSMSEDates()==0)
		  { 
            fprintf(outerrfile,"Error:  %s\n  SEDDAT \"%s\" earlier than SMDATE \"%s\" for subregion %d\n\n",ParsePath(0),SAVESEDDAT,SAVESMDATE,subregionfound);
		  } 
		}
	  }
	  else if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** date *** CI_Date *** date *** Date"))
	  {
        SCDATEyear   = year;
	    SCDATEmonth  = month;
	    SCDATEday    = day;
        SCDATE_found = 1;
		sprintf(SAVESCDATE,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVESCDATE))<950)
		{
		  sprintf(tempstr,"Subregion %d, SCDATE: %s\n",subregionfound,SAVESCDATE);
		  strcat(SavedSubregionDates,tempstr);
		}

		if(SEDDAT_found==1)
		{
          
          if(CompareSCSEDates()==0)
		  { 
            fprintf(outerrfile,"Error:  %s\n  SEDDAT \"%s\" earlier than SCDATE \"%s\" for subregion %d\n\n",ParsePath(0),SAVESEDDAT,SAVESCDATE,subregionfound);
		  } 
		}
	  }
	  else if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** editionDate *** Date"))
	  {
        SEDDATyear   = year;
	    SEDDATmonth  = month;
	    SEDDATday    = day;
        SEDDAT_found = 1;
		sprintf(SAVESEDDAT,"%s",thedata);

		if((strlen(SavedSubregionDates)+strlen(SAVESEDDAT))<950)
		{
		  sprintf(tempstr,"Subregion %d, SEDDAT: %s\n",subregionfound,SAVESEDDAT);
		  strcat(SavedSubregionDates,tempstr);
		}


		if(SCDATE_found==1)
		{
          
          if(CompareSCSEDates()==0)
		  { 
            fprintf(outerrfile,"Error:  %s\n  SEDDAT \"%s\" earlier than SCDATE \"%s\" for subregion %d\n\n",ParsePath(0),SAVESEDDAT,SAVESCDATE,subregionfound);
		  } 
		}

		if(SMDATE_found==1)
		{
          
          if(CompareSMSEDates()==0)
		  { 
            fprintf(outerrfile,"Error:  %s\n  SEDDAT \"%s\" earlier than SMDATE \"%s\" for subregion %d\n\n",ParsePath(0),SAVESEDDAT,SAVESMDATE,subregionfound);
		  } 
		}
	  }
	}
  }
}


int IsInt(char inchar)
{
  if(inchar<'0')
  {
    return 0;
  }
  if(inchar>'9')
  {
    return 0;
  }
  return 1;
}



void HandleCellIds()
{
  int len,intew,intns;
  char charew[10],charns[10];


  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** title *** CharacterString"))
  {
	

	sprintf(SAVECELLID,"%s",thedata);
	foundcellid = 1;

	len = strlen(thedata);
	if(len!=7)
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}
	if((thedata[0]!='E')&&(thedata[0]!='W'))
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}
	if((thedata[0]!='E')&&(thedata[0]!='W'))
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}

	if(IsInt(thedata[1])==0)
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}
	if(IsInt(thedata[2])==0)
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}
	if(IsInt(thedata[3])==0)
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}
	if(IsInt(thedata[5])==0)
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  
	  return;
	}
	if(IsInt(thedata[6])==0)
	{
      WriteExpected(1,"Expected","CELLID value with format <E/W><Longitude><N/S><Latitude>",thedata);

	  

	  return;
	}

	charew[0] = thedata[1];
	charew[1] = thedata[2];
	charew[2] = thedata[3];
	charew[3] = '\0';

	charns[0] = thedata[5];
	charns[1] = thedata[6];
	charns[2] = '\0';

	sscanf(charew,"%d",&intew);
	sscanf(charns,"%d",&intns);

	if(thedata[0]=='E')
	{
	  if((intew<0)||(intew>179))
	  {
        fprintf(outerrfile,"Error:  %s:\n   CELLID value \"%s\" found, East value must be 0-179, got %d\n\n",
          ParsePath(0),thedata,intew);
	    return;
	  }
	}
	else
	{
	  if((intew<1)||(intew>180))
	  {
        fprintf(outerrfile,"Error:  %s:\n   CELLID value \"%s\" found, West value must be 1-180, got %d\n\n",
          ParsePath(0),thedata,intew);
	    return;
	  }
	}

	if(thedata[4]=='N')
	{
	  if((intns<0)||(intns>89))
	  {
        fprintf(outerrfile,"Error:  %s:\n   CELLID value \"%s\" found, North value must be 0-89, got %d\n\n",
          ParsePath(0),thedata,intns);
	    return;
	  }
	}
	else
	{
	  if((intns<1)||(intns>90))
	  {
        fprintf(outerrfile,"Error:  %s:\n   CELLID value \"%s\" found, South value must be 1-90, got %d\n\n",
          ParsePath(0),thedata,intns);
	    return;
	  }
	}
  }
  else if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataSetURI *** CharacterString"))
  {
    

	if(
		(strncmp(thedata,"https://www.mgcp.ws/",20)) ||
		(strlen(thedata)!=27)
		)
	{
      fprintf(outerrfile,"Warning:  %s:\n   CURI value \"%s\" is not of the form \"https://www.mgcp.ws/<CELLID>\"\n\n",
        ParsePath(0),thedata);
	}

	sprintf(SAVECURI,"%s",thedata);
	sprintf(SAVECURIPATH,"%s",thepath);
	foundcuri = 1;
  }
}


void HandleFixedCatalogs(int close)
{
  int i;
  static int foundC1=0,foundC2=0,foundC3=0,foundC4=0,foundC5=0;

  if(version==3)
  {
    
    return;
  }
  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    
    return;
  }


  if(close==1)
  {
    if(foundC1==0)
	{
      fprintf(outerrfile,"Error:  Did not find MGCP_Cell *** datasetCatalogue with attribute \"%s\" = \"%s\"\n\n",
        "xlink:href","resources/Codelist/gmxCodelists.xml");
	}
    if(foundC2==0)
	{
      fprintf(outerrfile,"Error:  Did not find MGCP_Cell *** datasetCatalogue with attribute \"%s\" = \"%s\"\n\n",
        "xlink:href","mgcp/Codelist/mgcp_gmxCodelists.xml");
	}
    if(foundC3==0)
	{
      fprintf(outerrfile,"Error:  Did not find MGCP_Cell *** datasetCatalogue with attribute \"%s\" = \"%s\"\n\n",
        "xlink:href","mgcp/crs/mgcp_gmxCrs.xml");
	}
    if(foundC4==0)
	{
      fprintf(outerrfile,"Error:  Did not find MGCP_Cell *** datasetCatalogue with attribute \"%s\" = \"%s\"\n\n",
        "xlink:href","resources/uom/gmxUom.xml");
	}

	if(DO_C5==1)
	{
      if(foundC5==0)
	  {
        if(strlen(CellFCName)>0)
		{
          fprintf(outerrfile,"Error:  Did not find MGCP_Cell *** datasetCatalogue with attribute \"%s\" = \"%s\"\n\n",
            "xlink:href",CellFCName);
		} 
	  } 
	}

    foundC1 = 0;
    foundC2 = 0;
    foundC3 = 0;
    foundC4 = 0;
	foundC5 = 0;
  }


 if(!strcmp(thepath,"MGCP_Cell *** datasetCatalogue"))
  {
    for(i=0;i<numattributes;i++)
	{
      if(!strcmp(attributes[i],"xlink:href"))
	  {
        if(!strcmp(attributeV[i],"resources/Codelist/gmxCodelists.xml"))
		{
          foundC1 = 1;
		}
        if(!strcmp(attributeV[i],"mgcp/Codelist/mgcp_gmxCodelists.xml"))
		{
          foundC2 = 1;
		}
        if(!strcmp(attributeV[i],"mgcp/crs/mgcp_gmxCrs.xml"))
		{
          foundC3 = 1;
		}
        if(!strcmp(attributeV[i],"resources/uom/gmxUom.xml"))
		{
          foundC4 = 1;
		}
		if(!strcmp(attributeV[i],CellFCName))
		{
          foundC5 = 1;
		}
	  }
	}
  }
}


char * ListSubregionFCNames()
{
  static char retval[500];


  if(SubregionFCName[0]=='\0')
  {
    

    if(version==2)
	{
      if(trd1_override==1)
	  {
        sprintf(retval,"\n  \"MGCP_FeatureCatalogue_TRD1.0_20051231.xml\" or \n  \"MGCP_FeatureCatalogue_TRD1.1_20061019.xml\"");
	  }
	  else
	  {
        sprintf(retval,"\n  \"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or \n  \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\"");
	  }
	}
	else if(version==3)
	{
      sprintf(retval,"\n  \"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or \n  \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\" or \n  \"MGCP_FeatureCatalogue_TRD3.0_20091103.xml\"");
	}
	else if(version==4)
	{
      sprintf(retval,"\n  \"MGCP_FeatureCatalogue_TRD4.0_20121231.xml\" or \"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or \n  \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\" or \n  \"MGCP_FeatureCatalogue_TRD3.0_20091103.xml\"");
 	}
	else if((version==41)||(version==42)||(version==43)||(version==44))
	{
      sprintf(retval,"\n  \"MGCP_FeatureCatalogue_TRD4_v4.4_2016-12-31.xml\" or\n  \"MGCP_FeatureCatalogue_TRD4.1_20130628.xml\" or\n  \"MGCP_FeatureCatalogue_TRD4.0_20121231.xml\" or\n  \"MGCP_FeatureCatalogue_TRD2.0_20070328.xml\" or\n  \"MGCP_FeatureCatalogue_TRD2.0_20070425.xml\" or\n  \"MGCP_FeatureCatalogue_TRD3.0_20091103.xml\"");
 	}


    return retval;
  }
  else
  {
	sprintf(retval,"\"%s\"",SubregionFCName);
    return retval;
  }
}


int CheckSubregionFCName(char *name)
{

  if(SubregionFCName[0]=='\0')
  {
    

    if(version==2)
	{
      if(trd1_override==1)
	  {
        if(
		    (!strcmp(name,"MGCP_FeatureCatalogue_TRD1.0_20051231.xml")) ||
		    (!strcmp(name,"MGCP_FeatureCatalogue_TRD1.1_20061019.xml"))
		   )
		{
          return 1;
		}
	    return 0;
	  } 

      if(
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070328.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070425.xml"))
        )
	  {
        return 1;
	  }
	  return 0;
	}
	else if(version==3)
	{
      if(
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070328.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070425.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD3.0_20091103.xml"))
        )
	  {
        return 1;
	  }
	  return 0;
	}
	else if(version==4)
	{
      if(
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070328.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070425.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD3.0_20091103.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD4.0_20121231.xml"))
        )
	  {
        return 1;
	  }
	  return 0;
	}
	else if((version==41)||(version==42)||(version==43))
	{
      if(
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070328.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070425.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD3.0_20091103.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD4.0_20121231.xml")) ||
		 (!strcmp(name,"MGCP_FeatureCatalogue_TRD4.1_20130628.xml"))
        )
	  {
        return 1;
	  }
	  return 0;
	}
	else if(version==44)
	{
      if(
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070328.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD2.0_20070425.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD3.0_20091103.xml")) ||
	     (!strcmp(name,"MGCP_FeatureCatalogue_TRD4.0_20121231.xml")) ||
		 (!strcmp(name,"MGCP_FeatureCatalogue_TRD4.1_20130628.xml")) ||
		 (!strcmp(name,"MGCP_FeatureCatalogue_TRD4_v4.4_2016-12-31.xml"))
        )
	  {
        return 1;
	  }
	  return 0;
	}

  }
  else
  {
    
    if(!strcmp(SubregionFCName,name))
	{
      return 1;
	}
    return 0;
  }
  return 0;
}



void AddSLSTAT(char *theval)
{
  int oldlen;
  char tempstr[500];


  if(slstat_found==0)
  {
	saveSLSTAT = (char *)malloc(strlen(theval)+50);
	sprintf(saveSLSTAT," Subregion: %d  SLSTAT: %s\n",subregionfound,theval);
    slstat_found = 1;
  }
  else
  {
    oldlen = strlen(saveSLSTAT);
	saveSLSTAT = (char *)realloc(saveSLSTAT,oldlen+strlen(theval)+50);
	theval[499] = '\0';
	sprintf(tempstr," Subregion: %d  SLSTAT: %s\n",subregionfound,theval);
	strcat(saveSLSTAT,tempstr);
  }

}



void HandleInsideSubregions(int close)
{
  int i;
  static int foundT1=0,foundT2=0,foundT3=0,foundT4=0,foundT5=0,foundT6=0,foundT7=0;




  if(  (!strcmp(thepath,"MGCP_Cell *** subregion"))  || (close==1)  )
  {


	if(InSubregionFound>0)
	{

	   if((version==41)||(version==42)||(version==43)||(version==44))
	   { 
         if((SAVE_STIERN==1)&&(SAVE_SMSHND==2))
		 {
			if(version==44)
			{
		      fprintf(outerrfile,"Error:  STIERN \"Unrestricted. For Public Release.\" is not compatible with SMSHND \"Limited Distribution (limdis)\" for subregion %d\n\n",InSubregionFound);
			}
			else
			{
		      fprintf(outerrfile,"Error:  STIERN \"Unrestricted.  For Public Release.\" is not compatible with SMSHND \"Limited Distribution (limdis)\" for subregion %d\n\n",InSubregionFound);
			}
		 }
         else if((SAVE_STIERN==2)&&(SAVE_SMSHND==1))
		 {
			if(version==44)
			{
		      fprintf(outerrfile,"Error:  STIERN \"Limited Distribution. For Official Use Only by MGCP members.\" is not compatible with SMSHND \"Unrestricted\" for subregion %d\n\n",InSubregionFound);
			}
			else
			{
		      fprintf(outerrfile,"Error:  STIERN \"Limited Distribution.  For Official Use Only by MGCP members.\" is not compatible with SMSHND \"Unrestricted\" for subregion %d\n\n",InSubregionFound);
			}
		 }

         SAVE_STIERN = 0;
         SAVE_SMSHND = 0;
	   }


      if(foundT1==0)
	  {
        fprintf(outerrfile,"Error:  Did not find subregionCatalogue tag with attribute \"%s\" = \"%s\" for subregion %d\n\n",
          "xlink:href","resources/Codelist/gmxCodelists.xml",InSubregionFound);
 	  }
      if(foundT2==0)
	  {
        fprintf(outerrfile,"Error:  Did not find subregionCatalogue tag with attribute \"%s\" = \"%s\" for subregion %d\n\n",
          "xlink:href","mgcp/Codelist/mgcp_gmxCodelists.xml",InSubregionFound);
 	  }
      if(foundT3==0)
	  {
        fprintf(outerrfile,"Error:  Did not find subregionCatalogue tag with attribute \"%s\" = \"%s\" for subregion %d\n\n",
          "xlink:href","mgcp/crs/mgcp_gmxCrs.xml",InSubregionFound);
 	  }
      if(foundT4==0)
	  {
        fprintf(outerrfile,"Error:  Did not find subregionCatalogue tag with attribute \"%s\" = \"%s\" for subregion %d\n\n",
          "xlink:href","resources/uom/gmxUom.xml",InSubregionFound);
 	  }
      if(foundT5==0)
	  {
        fprintf(outerrfile,"Error:  Did not find subregionCatalogue tag with attribute \"%s\" = %s for subregion %d\n\n",
          "xlink:href",ListSubregionFCNames(),InSubregionFound);
 	  }

	  if((version==3)||((version==4)||(version==41)||(version==42)||(version==43)||(version==44)))
	  {
	    if(foundT6==0)
		{
          fprintf(outerrfile,"Error:  Did not find SORIGC for subregion %d\n\n",InSubregionFound);
		} 
	    if(foundT7==0)
		{ 
          fprintf(outerrfile,"Error:  Did not find SORIGA for subregion %d\n\n",InSubregionFound);
		} 
	  }
	}

    InSubregionFound = InSubregionFound + 1;
    foundT1 = 0;
    foundT2 = 0;
    foundT3 = 0;
    foundT4 = 0;
	foundT5 = 0;
	foundT6 = 0;
	foundT7 = 0;
  }



  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** statement *** CharacterString"))
  {
	AddSLSTAT(thedata);
  }


  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** pointOfContact *** CI_ResponsibleParty *** organisationName *** MGCP_ParticipantAgency"))
  {
    foundT7 = 1;
  }
  else if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** pointOfContact *** CI_ResponsibleParty *** contactInfo *** CI_Contact *** address *** CI_Address *** country *** MGCP_ParticipantNation"))
  {
    foundT6 = 1;
  }
  else if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionCatalogue"))
  {

    for(i=0;i<numattributes;i++)
	{
      if(!strcmp(attributes[i],"xlink:href"))
	  {
        if(!strcmp(attributeV[i],"resources/Codelist/gmxCodelists.xml"))
		{
          foundT1 = 1;
		}
        if(!strcmp(attributeV[i],"mgcp/Codelist/mgcp_gmxCodelists.xml"))
		{
          foundT2 = 1;
		}
        if(!strcmp(attributeV[i],"mgcp/crs/mgcp_gmxCrs.xml"))
		{
          foundT3 = 1;
		}
        if(!strcmp(attributeV[i],"resources/uom/gmxUom.xml"))
		{
          foundT4 = 1;
		}
        if(CheckSubregionFCName(attributeV[i]))
		{
          foundT5 = 1;
		}
	  }
	}
  }
}


void HandleSubregionISO()
{
  static int thissubregion = -1;
  static char saveSubCMSTDV[500];


  if(foundsubregionid!=thissubregion)
  {
    
	thissubregion    = foundsubregionid;
	foundSubCMSTDN   = 0;
    foundSubCMSTDV   = 0;
	saveSubCMSTDV[0] = '\0';
	ACE_ALE          = 0;
  }




  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** metadataStandardName *** CharacterString"))
  {
	if(!strcmp(thedata,"ISO 19115"))
	{
      foundSubCMSTDN = 1;
	}

	if((foundSubCMSTDN==1)&&(foundSubCMSTDV==1))
	{
	  if(strcmp(saveSubCMSTDV,"2003"))
	  {
        fprintf(outerrfile,"Error:  %s:\n   (Subregion %d) Subregion metadata standard name (SMSTDN) is \"ISO 19115\" but Subregion metadata standard version (SMSTDV) is \"%s\" (should be \"2003\")\n\n",
          ParsePath(0),foundsubregionid,saveSubCMSTDV);
        return;
	  }
	}
  }
  else if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** metadataStandardVersion *** CharacterString"))
  {
    foundSubCMSTDV = 1;
    sprintf(saveSubCMSTDV,"%s",thedata);

	if((foundSubCMSTDN==1)&&(foundSubCMSTDV==1))
	{
	  if(strcmp(saveSubCMSTDV,"2003"))
	  {
        fprintf(outerrfile,"Error:  %s:\n   (Subregion %d) Subregion metadata standard name (SMSTDN) is \"ISO 19115\" but Subregion metadata standard version (SMSTDV) is \"%s\" (should be \"2003\")\n\n",
          ParsePath(0),foundsubregionid,saveSubCMSTDV);
        return;
	  }
	}
  }
}



const char *GetPAL(char inchar)
{
  if(inchar=='P')
  {
    return "Point";
  }
  if(inchar=='A')
  {
    return "Area";
  }
  if(inchar=='L')
  {
    return "Line";
  }

  printf("Error in GPAL:  %c\n",inchar);
  return "Error";
}



int OnFCODEList(char *possiblefcode,int checkgeom)
{
  int i,max;
  char tester[10];

  if(version==2)
  {
    max = num_mgcp_trd2_codes;
	thearray = MGCP_TRD2_FCODES;
  }
  else if(version==3)
  {
    max = num_mgcp_trd3_codes;
	thearray = MGCP_TRD3_FCODES;
  }
  else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    max = num_mgcp_trd4_codes;
	thearray = MGCP_TRD4_FCODES;
  }

  for(i=0;i<max*3;i=i+3)
  {
	if(checkgeom==1)
	{
	  sprintf(tester,"%s%s",thearray[i],thearray[i+2]);
      if(!strcmp(possiblefcode,tester))
	  { 
        return i;
	  } 
	}
	else
	{
      if(!strcmp(&possiblefcode[1],thearray[i+2]))
	  { 
        return i;
	  } 
	}
  }
  return -1;
}



int OnFeatureList(const char *possiblefeaturestring)
{
  int i,max;
  char tester[200];

  if(version==2)
  {
    max = num_mgcp_trd2_codes;
	thearray = MGCP_TRD2_FCODES;
  }
  else if(version==3)
  {
    max = num_mgcp_trd3_codes;
	thearray = MGCP_TRD3_FCODES;
  }
  else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    max = num_mgcp_trd4_codes;
	thearray = MGCP_TRD4_FCODES;
  }

  for(i=0;i<max*3;i=i+3)
  {
     sprintf(tester,"%s %s Feature",thearray[i+1],GetPAL(thearray[i][0]));
     if(!strcmp(possiblefeaturestring,tester))
	 { 
        return i;
	 }
  }

  return -1;
}



void HandleSubregionFeatureTypes()
{
  char tester[10];
  int index,i;


  if(strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureTypes *** LocalName"))
  {
    return;
  }


  index = OnFeatureList(thedata);

 
  if(index<0)
  {
    printf("Subregion %d:  Did not find %s\n",foundsubregionid,thedata);

	if(!strcmp(thedata,"Non-Communication Tower Point Feature"))
	{
      index = OnFeatureList("Non-communication Tower Point Feature");

	  if(index>0)
	  {
        fprintf(outerrfile,"Error:  %s:\n   Subregion %d, found \"%s\", substituting \"%s\"\n\n",
          ParsePath(0),foundsubregionid,thedata,"Non-communication Tower Point Feature");
	  }
	}
	else
	{
      if(version<4)
	  {
        fprintf(outerrfile,"Warning:  %s:\n   Subregion %d, found unrecognized \"%s\" (specialized feature catalog?)\n\n",
          ParsePath(0),foundsubregionid,thedata);
	  }
	  else
	  {
        fprintf(outerrfile,"Error:  %s:\n   Subregion %d, found unrecognized \"%s\"\n\n",
          ParsePath(0),foundsubregionid,thedata);
	  }
	}
  }


  if(index>=0)
  {
	if(VERBOSE==1)
	{
      printf("Subregion %d:  Found %s\n",foundsubregionid,thedata);
	}
  }
  else
  {
    return;
  }

  

  for(i=0;i<NumSubregionPolys;i++)
  {
    if(SubregionPolys[i].subregionid==foundsubregionid)
	{
	  

      sprintf(tester,"%s%s",thearray[index],thearray[index+2]);

      SubregionPolys[i].num_ftype = SubregionPolys[i].num_ftype + 1;
	  if(SubregionPolys[i].num_ftype==1)
	  {
        SubregionPolys[i].ftypes = (char **)malloc(sizeof(char *));
		SubregionPolys[i].ftypes[0] = (char *)malloc(strlen(tester)+5);
		sprintf(SubregionPolys[i].ftypes[0],"%s",tester);
	  }
	  else
	  {
        SubregionPolys[i].ftypes = (char **)realloc(SubregionPolys[i].ftypes,sizeof(char *) * SubregionPolys[i].num_ftype);
		SubregionPolys[i].ftypes[SubregionPolys[i].num_ftype-1] = (char *)malloc(strlen(tester)+5);
		sprintf(SubregionPolys[i].ftypes[SubregionPolys[i].num_ftype-1],"%s",tester);
	  }
	}
  }
}


void Check2DDates(int type)
{
  char tempdate1[260],tempdate2[260];

  if((version==42)||(version==43)||(version==44))
  {
    if(SCDATE_found==0)
	{ 
      return;
	} 
    if(SSVCDT_found==0)
	{ 
      return;
	} 
    if(SSVRTI_found==0)
	{ 
      return;
	} 

    


    

    if(!strcmp(SSVRTI,"No Elevations"))
	{ 
      sprintf(tempdate1,"%s",SSVCDT);
      sprintf(tempdate2,"%s",SAVESCDATE);

      tempdate1[10] = '\0'; 
      tempdate2[10] = '\0';

	  if(strcmp(tempdate1,tempdate2))
	  { 
        fprintf(outerrfile,"Error: Subregion %d:\n   Found SSVRTI = \"No Elevations\" but SSVCDT %s not matching SCDATE %s\n\n",
		  foundsubregionid,SSVCDT,SAVESCDATE);
	  } 
	} 
  }
}

void HandleNewSubregion()
{

if(!strcmp(thepath,"MGCP_Cell *** subregion"))
  {
    subregion_version_found = -1;

  if(foundsubregionid>0)
  {

	Check2DDates(1);

	if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	{
      if(SSVRTI_found==0)
	  {
        fprintf(outerrfile,"Error: Did not find SSVRTI for subregion %d\n\n",foundsubregionid);
	  } 
      SSVRTI_found = 0;
      potential_SSVRTI_found = 0;

      if(SSVCDT_found==0)
	  { 
        fprintf(outerrfile,"Error: Did not find SSVCDT for subregion %d\n\n",foundsubregionid);
	  }
	  SSVCDT_found = 0;
	  potential_SSVCDT_found = 0;

      if(SSVCTY_found==0)
	  { 
        fprintf(outerrfile,"Error: Did not find SSVCTY for subregion %d\n\n",foundsubregionid);
	  } 
      SSVCTY_found = 0;
	}

    if(SDESCR_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SDESCR for subregion %d\n\n",foundsubregionid);
	}
    SDESCR_found = 0;

    if(SSCRTI_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SSCRTI for subregion %d\n\n",foundsubregionid);
	}
    SSCRTI_found = 0;

    if(SSRCDT_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SSRCDT for subregion %d\n\n",foundsubregionid);
	}
    SSRCDT_found = 0;

    if(SSRCTY_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SSRCTY for subregion %d\n\n",foundsubregionid);
	}
    SSRCTY_found = 0;

    
	if(SEDDAT_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SEDDAT for subregion %d\n\n",foundsubregionid);
	}
    SEDDAT_found = 0;

    if(SCDATE_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SCDATE for subregion %d\n\n",foundsubregionid);
	}
    SCDATE_found = 0;

    if(SMDATE_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SMDATE for subregion %d\n\n",foundsubregionid);
	}
    SMDATE_found = 0;

  }
  }

}



void HandleSubregionIds()
{
  char tempstr[500],idchar[10];
  int len,idint;



  if(strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** citation *** CI_Citation *** title *** CharacterString"))
  {
    return;
  }

  len = strlen(thedata);



  foundsubregionid = foundsubregionid + 1;





  if(len!=9)
  {
    fprintf(outerrfile,"Error:  %s:\n   Subregion ID \"%s\" found, not of correct form <E/W><Longitude><N/S><Latitude><01-99> (9 characters)\n\n",
        ParsePath(0),thedata);
    return;
  }

  if(foundcellid==1)
  {
	if(strncmp(SAVECELLID,thedata,7))
	{
      fprintf(outerrfile,"Error:  %s:\n   Subregion ID \"%s\" found that does not match CELLID previously found (%s)\n\n",
        ParsePath(0),thedata,SAVECELLID);
	}

	if(IsInt(thedata[7])==0)
	{
      fprintf(outerrfile,"Error:  %s:\n   Subregion ID value \"%s\" found, expected [01-99] as 8th and 9th characters, got \"%c\" as 8th character\n\n",
        ParsePath(0),thedata,thedata[7]);
	  return;
	}
	if(IsInt(thedata[8])==0)
	{
      fprintf(outerrfile,"Error:  %s:\n   Subregion ID value \"%s\" found, expected [01-99] as 8th and 9th characters, got \"%c\" as 9th character\n\n",
        ParsePath(0),thedata,thedata[8]);
	  return;
	}

	idchar[0] = thedata[7];
	idchar[1] = thedata[8];
	idchar[2] = '\0';
	sscanf(idchar,"%d",&idint);
	if(idint!=foundsubregionid)
	{
	  if(foundsubregionid<10)
	  {
		sprintf(tempstr,"Subregion ID value: \"%s0%d\"",SAVECELLID,foundsubregionid);
        WriteExpected(1,"Expected",tempstr,thedata);

		
	  }
	  else
	  {
		sprintf(tempstr,"Subregion ID value: \"%s%d\"",SAVECELLID,foundsubregionid);
        WriteExpected(1,"Expected",tempstr,thedata);

		
	  }
	}
  }
  else
  {
    fprintf(outerrfile,"Error:  %s:\n   Subregion ID \"%s\" found, but no valid CELLID previously found\n\n",
        ParsePath(0),thedata);
  }
}


void HandleCodeSpaces()
{

  if(strstr(thepath,"*** codeSpace *** CharacterString"))
  {
    if(version==2)
	{
	  if( (strcmp(thedata,"MGCP_v2_r0")) &&  (strcmp(thedata,"MGCP_V2_r0")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v2_r0 or MGCP_V2_r0",thedata);

		
	  }
	}
	else if(version==3)
	{
	  if( (strcmp(thedata,"MGCP_v3_r0")) && (strcmp(thedata,"MGCP_V3_r0")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v3_r0 or MGCP_V3_r0",thedata);

		
	  }
	}
	else if(version==4)
	{
	  if( (strcmp(thedata,"MGCP_v4_r0")) && (strcmp(thedata,"MGCP_V4_r0")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v4_r0 or MGCP_V4_r0",thedata);
	  }
	}
	else if(version==41)
	{
	  if( (strcmp(thedata,"MGCP_v4_r1")) && (strcmp(thedata,"MGCP_V4_r1")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v4_r1 or MGCP_V4_r1",thedata);
	  }
	}
	else if(version==42)
	{
	  if( (strcmp(thedata,"MGCP_v4_r2")) && (strcmp(thedata,"MGCP_V4_r2")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v4_r2 or MGCP_V4_r2",thedata);
	  }
	}
	else if(version==43)
	{
	  if( (strcmp(thedata,"MGCP_v4_r3")) && (strcmp(thedata,"MGCP_V4_r3")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v4_r3 or MGCP_V4_r3",thedata);
	  }
	}
	else if(version==44)
	{
	  if( (strcmp(thedata,"MGCP_v4_r4")) && (strcmp(thedata,"MGCP_V4_r4")) )
	  {
        WriteExpected(1,"Expected codeSpace","MGCP_v4_r4 or MGCP_V4_r4",thedata);
	  }
	}
  }
}




void CheckSpecializedName()
{
  int i,j,thisnum,len;
  char CC[10];


  

  for(i=0;i<numattributes;i++)
  {
	if(!strcmp(attributes[i],"xlink:href"))
	{

       sprintf(SubregionFCName,"%s",attributeV[i]);

	   len = strlen(attributeV[i]);
	   if(len==15)
	   {
         if(
			 (!strncmp(&attributeV[i][0],"FC_",3))    &&
			 (!strncmp(&attributeV[i][11],".xml",4))  &&
			 (attributeV[i][6]=='_')
			 )
		 {
			 CC[0] = attributeV[i][3];
			 CC[1] = attributeV[i][4];
			 CC[2] = attributeV[i][5];
			 CC[3] = '\0';

			 

			 for(j=0;j<ParticipantNationCodesMax2*3;j=j+3)  
			 {

				 if(!strcmp(CC,ParticipantNationCodes2[j]))
				 {
                   
				   if(
					   (IsInt(attributeV[i][7])) &&
					   (IsInt(attributeV[i][8])) &&
					   (IsInt(attributeV[i][9])) &&
					   (IsInt(attributeV[i][10]))
					 )
				   {
					   sprintf(CC,"%s",&attributeV[i][7]);
					   CC[4] = '\0';

                       sscanf(CC,"%d",&thisnum);
					   if(thisnum>10)
					   {
                         
					   }

					   
					   return;
				   }
				 }
			 }			 
		 }
	   }
	}
  }


  


  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"xlink:href\":<\"FC_<3 character ParticipantNationCode>_<4 digit number>.xml\"> }\n",
	  ParsePath(0));

  PrintAttributesToErrFile();

}


void HandleFCDates()
{
  static int looking_for_embedded_date = 0;
  static int found_special             = 0;
  static int foundtruefalse            = 0; 

  


  if(!strcmp(thepath,"MGCP_Cell *** subregion"))
  {
    
    if(looking_for_embedded_date==1)
	{ 
      
      printf("error:  did not find an SFCDTD for a subregion MGCP Feature Catalog\n");
	} 
    looking_for_embedded_date = 0;
	found_special             = 0;
	foundtruefalse            = 0;
	subregionfound            = subregionfound + 1;
	return;
  }

  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** includedWithDataset *** Boolean"))
  {
    if(((version==4)||(version==41)||(version==42)||(version==43)||(version==44))&&(strcmp(thedata,"false")))
	{
      fprintf(outerrfile,"Error:  %s:\n   Subregion (%d): (SFCINC) is \"%s\" (should be \"false\")\n\n",
        ParsePath(0),subregionfound,thedata);
      return;
	}
    else if(!strcmp(thedata,"true"))
	{
      foundtruefalse = 1;

	  

	  if(looking_for_embedded_date==1)
	  {
        fprintf(outerrfile,"Error:  %s:\n   Subregion (%d): Subregion Feature catalogue name (SFCATR) is \"MGCP Feature Catalogue\" but Subregion feature catalogue included (SFCINC) is true (should be false)\n\n",
          ParsePath(0),subregionfound);
        return;
	  }
	}
    else if(!strcmp(thedata,"false"))
	{
      foundtruefalse = 2;

	  

	  if(found_special==1)
	  {
        fprintf(outerrfile,"Error:  %s:\n   Subregion (%d): Subregion Feature catalogue name (SFCATR) is NOT \"MGCP Feature Catalogue\" (meaning a specialized feature catalog is being used) but Subregion feature catalogue included (SFCINC) is false (should be true)\n\n",
          ParsePath(0),subregionfound);
        return;
	  }
	}
  }


  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** title *** Anchor"))
  {

	if(!strcmp(thedata,"MGCP Feature Catalogue"))
	{
	  if(version==2)
	  {
        TestS1AttributeList("xlink:href");
	  }
	  else if(version==3)
	  {
        TestS2AttributeList("xlink:href");
	  }
	  else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	  {
        TestS3AttributeList("xlink:href");
	  }

      looking_for_embedded_date = 1;

	  

	  if(foundtruefalse==1)
	  {
        fprintf(outerrfile,"Error:  %s:\n   Subregion (%d): Subregion Feature catalogue name (SFCATR) is \"MGCP Feature Catalogue\" but Subregion feature catalogue included (SFCINC) is true (should be false)\n\n",
          ParsePath(0),subregionfound);
        return;
	  }

      SubregionFCName[0] = '\0'; 

	}
	else
	{

      if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	  {
        
        fprintf(outerrfile,"Error:  %s:\n   Subregion (%d): Subregion Feature catalogue name (SFCATR) is \"%s\", but should be \"MGCP Feature Catalogue\" \n\n",
           ParsePath(0),subregionfound,thedata);
	  }
	  else
	  {
	    
        looking_for_embedded_date = 0;
	    found_special = 1;

	    
  	    if(foundtruefalse==2)
		{
          fprintf(outerrfile,"Error:  %s:\n   Subregion (%d): Subregion Feature catalogue name (SFCATR) is \"%s\" (NOT \"MGCP Feature Catalogue\", meaning a specialized feature catalog is being used) but Subregion feature catalogue included (SFCINC) is false (should be true)\n\n",
           ParsePath(0),subregionfound,thedata);
          return;
		} 

        CheckSpecializedName();
	  }
	}
  }

  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** date *** CI_Date *** date *** Date"))
  {
    if(looking_for_embedded_date==1)
	{
      looking_for_embedded_date = 0;

      if(version==2)
	  {
        if(trd1_override==1)
		{
          if(
			  (strcmp(thedata,"2005-12-31"))  && 
			  (strcmp(thedata,"2006-10-19"))
			)
		  { 
            WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","TRD 1.0 or 1.1 date 2005-12-31 or 2006-10-19",thedata);

		    
            return;
		  }
		}
		else
		{
          if(
			  (strcmp(thedata,"2007-03-28")) &&
			  (strcmp(thedata,"2007-04-25"))
			)
		  {
            WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","TRD2 date \"2007-03-28\" or \"2007-04-25\"",thedata);

		    
            return;
		  }
		}
	  }
	  else if(version==3)
	  {
		
        if(
            (strcmp(thedata,"2007-03-28")) &&
            (strcmp(thedata,"2007-04-25")) &&
			(strcmp(thedata,"2009-11-03"))
		  )
		{
          WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","\"2007-03-28\" or \"2007-04-25\" or \"2009-11-03\"",thedata);

		  
          return;
		}
	  }
	  else if(version==4)
	  {
		
        if(
            (strcmp(thedata,"2007-03-28")) &&
            (strcmp(thedata,"2007-04-25")) &&
			(strcmp(thedata,"2009-11-03")) &&
			(strcmp(thedata,"2012-12-31"))
		  )
		{
          WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","\"2007-03-28\" or \"2007-04-25\" or \"2009-11-03\" or \"2012-12-31\"",thedata);
          return;
		}
	  }
	  else if((version==41)||(version==42)||(version==43)||(version==44))
	  {
		

		if((subregion_version_found==4)&&(strcmp(thedata,"2012-12-31")))
		{
          WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","2016-12-31",thedata);
          return;
		}
		else if((subregion_version_found==41)&&(strcmp(thedata,"2013-06-28")))
		{
          WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","2013-06-28",thedata);
          return;
		}
		else if((subregion_version_found==44)&&(strcmp(thedata,"2016-12-31")))
		{
          WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","2016-12-31",thedata);
          return;
		}
		else if(subregion_version_found<4)
		{
          if(
            (strcmp(thedata,"2007-03-28")) &&
            (strcmp(thedata,"2007-04-25")) &&
			(strcmp(thedata,"2009-11-03")) &&
			(strcmp(thedata,"2012-12-31")) &&
			(strcmp(thedata,"2013-06-28")) &&
			(strcmp(thedata,"2016-12-31"))
		    )
		  { 
            WriteExpected(1,"Expected Subregion feature catalogue date (SFCDTD)","\"2007-03-28\" or \"2007-04-25\" or \"2009-11-03\" or \"2012-12-31\" or \"2013-06-28\" or \"2016-12-31\"",thedata);
            return;
		  }
		}
	  }
	} 
  } 
}




void HandleReasonableValues(char *lasttoken)
{
  int i,len,numdecimal=0;

  len = strlen(thedata);

  if(!strcmp(lasttoken,"Boolean"))
  {
	if( (strcmp(thedata,"true")) && (strcmp(thedata,"false")) )
	{
      WriteExpected(1,"Expected Boolean value","\"true\" or \"false\"",thedata);

	  
	  return;
	}
  }
  else if(!strcmp(lasttoken,"Integer"))
  {
	for(i=0;i<len;i++)
	{
	  if(IsInt(thedata[i])==0)
	  {
		if((i!=0)||(thedata[i]!='-'))
		{
          WriteExpected(1,"Expected","Integer",thedata);

		  

	      return;
		}
	  }
	}
  }
  else if(
	       (!strcmp(lasttoken,"Real")) ||
 	       (!strcmp(lasttoken,"Float")) ||
 	       (!strcmp(lasttoken,"Decimal"))
		 )
  {
	for(i=0;i<len;i++)
	{
	  if(IsInt(thedata[i])==0)
	  {
	    if((thedata[i]=='.')&&(numdecimal==0))
		{
          numdecimal = 1;
		}
		else if( (i!=0) || (thedata[i]!='-') )
		{
          WriteExpected(1,"Expected","Real Number",thedata);

		  
	      return;
		}
	  }
	}
  }
}


void HandleISO()
{
  static char saveCMSTDV[500];

  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** metadataStandardName *** CharacterString"))
  {
	if(!strcmp(thedata,"ISO 19115"))
	{
      foundCMSTDN = 1;
	}

	if((foundCMSTDN==1)&&(foundCMSTDV==1))
	{
	  if(strcmp(saveCMSTDV,"2003"))
	  {
        fprintf(outerrfile,"Error:  %s:\n   Metadata standard name (CMSTDN) is \"ISO 19115\" but Metadata standard version (CMSTDV) is \"%s\" (should be \"2003\")\n\n",
          ParsePath(0),saveCMSTDV);
        return;
	  }
	}
  }
  else if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** metadataStandardVersion *** CharacterString"))
  {
    foundCMSTDV = 1;
    sprintf(saveCMSTDV,"%s",thedata);

	if((foundCMSTDN==1)&&(foundCMSTDV==1))
	{
	  if(strcmp(saveCMSTDV,"2003"))
	  {
        fprintf(outerrfile,"Error:  %s:\n   Metadata standard name (CMSTDN) is \"ISO 19115\" but Metadata standard version (CMSTDV) is \"%s\" (should be \"2003\")\n\n",
          ParsePath(0),saveCMSTDV);
        return;
	  }
	}
  }
}


void TestFixedAttributes()
{
  


  if(!strcmp(thepath,"MGCP_Cell *** dataFile *** MX_DataFile *** fileType *** MimeFileType"))
  {
    TestAttributeList("type","octet-stream");
  }

  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** title *** Anchor"))
  {
    if(version==2)
	{
      TestS1AttributeList("xlink:href");
	}
  }

}



void ProcessDataFile(int foundsrc)
{
  int i,len,index,index2;
  char tester[500];


  


  len = strlen(SaveLocalFilename);
  if(
	  (len!=6)                                                 ||
	  (IsInt(SaveLocalFilename[3])==0)                         ||
	  (IsInt(SaveLocalFilename[4])==0)                         ||
	  (IsInt(SaveLocalFilename[5])==0)                         ||
	  ((SaveLocalFilename[1]<65) || (SaveLocalFilename[1]>90)) ||
	  ((SaveLocalFilename[2]<65) || (SaveLocalFilename[2]>90)) ||
	  ((SaveLocalFilename[0]!='P') && (SaveLocalFilename[0]!='A') && (SaveLocalFilename[0]!='L'))
	  )
  {
    fprintf(outerrfile,"Error:  Line %s\nPath:  MGCP_Cell *** dataFile *** MX_DataFile *** fileName *** FileName:\n   Expected value of form <PAL><FCODE>, got \"%s\"\n\n",
	  SaveLocalFilenameLine,SaveLocalFilename);
  }
  else 
  {
	index = OnFCODEList(SaveLocalFilename,1);  

	if(index==-1)
	{
      if(version<4)
	  {
        fprintf(outerrfile,"Warning:  Line %s\nPath:  MGCP_Cell *** dataFile *** MX_DataFile *** fileName *** FileName:\n   \"%s\" is not a member of TRD%d (specialized feature catalog?)\n\n",
	     SaveLocalFilenameLine,SaveLocalFilename,version);
	  }
	  else
	  {
        fprintf(outerrfile,"Error:  Line %s\nPath:  MGCP_Cell *** dataFile *** MX_DataFile *** fileName *** FileName:\n   \"%s\" is not a member of TRD%d\n\n",
	     SaveLocalFilenameLine,SaveLocalFilename,version);
	  }
	}
	else
	{

      for(i=0;i<alldatafiles;i++)
	  {
        if(!strcmp(AllDataFiles[i],SaveLocalFilename))
		{
          fprintf(outerrfile,"Error:  %s:\n   Data file listed more than 1 time: %s\n\n",
            ParsePath(0),AllDataFiles[i]);
		  break;
		}
	  }


	  if(i>=alldatafiles)
	  {
        if(alldatafiles==0)
		{  
          AllDataFiles = (char **)malloc(sizeof(char *));
		}  
        else
		{  
          AllDataFiles = (char **) realloc(AllDataFiles,(alldatafiles+1)*sizeof(char *));
		}  

        AllDataFiles[alldatafiles] = (char *)malloc(strlen(SaveLocalFilename)+100);
        sprintf(AllDataFiles[alldatafiles],"%s",SaveLocalFilename);
        alldatafiles = alldatafiles + 1;	
	  }
	}


	index2 = OnFCODEList(SaveLocalFilename,0); 
	if(index2>=0)
	{
	  
      sprintf(tester,"%s %s Feature",thearray[index2+1],GetPAL(SaveLocalFilename[0]));

	  if(strcmp(SaveLocalName,tester))
	  {
        fprintf(outerrfile,"Error:  Line %s\nPath:  MGCP_Cell *** dataFile *** MX_DataFile *** featureTypes *** LocalName:\n   Expected: \"%s\"\n   Found: \"%s\"\n\n",
	      SaveLocalNameLine,tester,SaveLocalName);
	  }
	}
  }


  if(foundsrc==1)
  {
    sprintf(tester,"./%s.SHP",SaveLocalFilename);
	if(strcmp(tester,SaveLocalSrc))
	{
      sprintf(tester,"./%s_1.SHP",SaveLocalFilename);
      if(strcmp(tester,SaveLocalSrc))
	  {
        fprintf(outerrfile,"Error:  Line %s\nPath: MGCP_Cell *** dataFile *** MX_DataFile *** fileName *** FileName:\n   Expected: \"src\" attribute with value \"./%s.SHP\" or \"./%s_1.SHP\"\n   Found: \"src\" attribute with value \"%s\"\n\n",
	      SaveLocalFilenameLine,SaveLocalFilename,SaveLocalFilename,SaveLocalSrc);
	  }
	}
  }
}

void ProcessFeatureTotal(int geomtype, int total)
{
  if(geomtype==C_POFE)
  {
    pointtotal = total;
  }
  if(geomtype==C_LINE)
  {
    linetotal = total;
  }
  if(geomtype==C_AREA)
  {
    areatotal = total;
  }
}


void HandleCellFeatureCounts()
{
  int len,i;
  static int thistotal         = -1;

  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** spatialRepresentationInfo *** MD_VectorSpatialRepresentation *** geometricObjects *** MD_GeometricObjects"))
  {
    

    if(working_geom_type==C_AREA)
	{
      if(areatotal==-999)
	  {
        fprintf(outerrfile,"Error:  Found MD_GeometricObjects \"surface\" but did not find CAREAC\n\n");
	  }
	}
	else if(working_geom_type==C_LINE)
	{
      if(linetotal==-999)
	  {
        fprintf(outerrfile,"Error:  Found MD_GeometricObjects \"curve\" but did not find CLINEC\n\n");
	  }
	}
	else if(working_geom_type==C_POFE)
	{
      if(pointtotal==-999)
	  {
        fprintf(outerrfile,"Error:  Found MD_GeometricObjects \"point\" but did not find CPONTC\n\n");
	  }
	}
	else if(InsideDataTree==1)
	{
       fprintf(outerrfile,"Error:  %s:\n   Did not find MD_GeometricObjectTypeCode in previous MD_GeometricObjects tree\n\n",
         ParsePath(0));
	}
	working_geom_type = -1;
	thistotal         = -1;
    InsideDataTree    = 1;
  }
  else if(InsideDataTree==1)
  {
    if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** spatialRepresentationInfo *** MD_VectorSpatialRepresentation *** geometricObjects *** MD_GeometricObjects *** geometricObjectType *** MD_GeometricObjectTypeCode"))
	{
      if(!strcmp(thedata,"point"))
	  {
        working_geom_type = C_POFE;
	  }
      else if(!strcmp(thedata,"curve"))
	  {
        working_geom_type = C_LINE;
	  }
      else if(!strcmp(thedata,"surface"))
	  {
        working_geom_type = C_AREA;
	  }
	  else
	  {
	    WriteExpected(1,"Expected","\"point\", \"curve\", or \"surface\"",thedata);

		
	  }
	}

    if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** spatialRepresentationInfo *** MD_VectorSpatialRepresentation *** geometricObjects *** MD_GeometricObjects *** geometricObjectCount *** Integer"))
	{
	  len = strlen(thedata);
	  for(i=0;i<len;i++)
	  {
	    if(IsInt(thedata[i])==0)
		{ 
 	      return;
		} 
	  } 
      sscanf(thedata,"%d",&thistotal);
	}


	if((thistotal>=0) && (working_geom_type>=0))
	{
	  ProcessFeatureTotal(working_geom_type,thistotal);
      InsideDataTree = 0;
	}
  }

 
}


void HandleCellDataFiles()
{
  int i,len;
  static int found_desc       = 0; 
  static int found_local_name = 0; 
  static int found_filename   = 0; 
  static int found_srccode    = 0; 


  if(!strcmp(thepath,"MGCP_Cell *** dataFile"))
  {
    

	datafilenum = datafilenum + 1;

	if(InsideDataFile!=-1) 
	{
      if(found_desc==0)
	  {
        printf("Error: did not find description for dataFile #%d\n",datafilenum);
	  }
      if(found_local_name==0)
	  {
        printf("Error: did not find description for dataFile #%d\n",datafilenum);
	  }
      if(found_filename==0)
	  {
        printf("Error: did not find description for dataFile #%d\n",datafilenum);
	  }
	}
	InsideDataFile   = 1;
    found_desc       = 0;
    found_local_name = 0;
    found_filename   = 0;
	found_srccode    = 0;
  }
  else if(InsideDataFile==1)
  {

    if(!strcmp(thepath,"MGCP_Cell *** dataFile *** MX_DataFile *** fileName *** FileName"))
	{
      
      found_filename = 1;

	  if((int)strlen(thedata)>local_filename_max)
	  {
        free(SaveLocalFilename);
		SaveLocalFilename = (char *)malloc(strlen(thedata) + 505);
		local_filename_max = strlen(thedata) + 505;
	  }
      sprintf(SaveLocalFilename,"%s",thedata);

      sprintf(SaveLocalFilenameLine,"%s",GetLineNumber());


      for(i=0;i<numattributes;i++)
	  {
		if(!strcmp(attributes[i],"src"))
		{
          found_srccode = 1;
          len = strlen(attributeV[i]);

          if((int)strlen(attributeV[i])>local_src_max)
		  {
            free(SaveLocalSrc);
		    SaveLocalSrc = (char *)malloc(strlen(attributeV[i]) + 505);
		    local_src_max = strlen(attributeV[i]) + 505;
		  } 
          sprintf(SaveLocalSrc,"%s",attributeV[i]);
          break;
		}
	  }

	  if(found_srccode==0)
	  {
        
        fprintf(outerrfile,"Error:  %s:\n   Expected attribute \"src\"\n\n",
	      ParsePath(0));
	  }

	}
    else if(!strcmp(thepath,"MGCP_Cell *** dataFile *** MX_DataFile *** fileDescription *** CharacterString"))
	{
      
      found_desc = 1;

	  if((int)strlen(thedata)>local_desc_max)
	  {
        free(SaveLocalDesc);
		SaveLocalDesc = (char *)malloc(strlen(thedata) + 505);
		local_desc_max = strlen(thedata) + 505;
	  }
      sprintf(SaveLocalDesc,"%s",thedata);
	}
    else if(!strcmp(thepath,"MGCP_Cell *** dataFile *** MX_DataFile *** featureTypes *** LocalName"))
	{
      
      found_local_name = 1;

	  if((int)strlen(thedata)>local_name_max)
	  {
        free(SaveLocalName);
		SaveLocalName = (char *)malloc(strlen(thedata) + 505);
		local_name_max = strlen(thedata) + 505;
	  }
      sprintf(SaveLocalName,"%s",thedata);

      sprintf(SaveLocalNameLine,"%s",GetLineNumber());

	}

    if((found_filename==1)&&(found_desc==1)&&(found_local_name==1))
	{
      ProcessDataFile(found_srccode);
	  InsideDataFile = 0;
	} 
  }
}






int IsDecimal(char *ordinate)
{
  int len,i,numdecimal=0;
  
  len = strlen(ordinate);

  for(i=0;i<len;i++)
  { 
    if(IsInt(ordinate[i])==0)
	{
      if((ordinate[i]=='.')&&(numdecimal==0))
	  {
        numdecimal = 1;
	  }
      else if( (i!=0) || (ordinate[i]!='-') )
	  {
		if((ordinate[i]==10)||(ordinate[i]==13))
		{
          fprintf(outerrfile,"Error:  %s:\n   ordinate \"%s\" found in \"%s\", expected a decimal number (newlines are not allowed)\n\n",
           ParsePath(0),ordinate,thedata);	
		}
		else
		{
          fprintf(outerrfile,"Error:  %s:\n   ordinate \"%s\" found in \"%s\", expected a decimal number (the first illegal value is \"%c\" in position %d)\n\n",
           ParsePath(0),ordinate,thedata,ordinate[i],i+1);	
		}
		return 0;
	  }
	}
  } 
  return 1;
}



void MarkBad(int PolyID, char *thispiece)
{
  int i;


  


  for(i=0;i<NumSubregionPolys;i++)
  {
    if(
		(SubregionPolys[i].PolyID==PolyID) &&
		(SubregionPolys[i].subregionid==foundsubregionid)
		)
	{
      SubregionPolys[i].numverts = -1;
	  return;
	}
  }
}




int AddVertex(int xy,char *thispiece, int PolyID, int ishole)
{
  double thevalue;
  int i,j,foundit=0;


  sscanf(thispiece,"%lf",&thevalue);


  

  if(xy==1)
  {
    if((foundCellWest==1)&&(foundCellEast==1))
	{
      if((thevalue<cellWest)||(thevalue>cellEast))
	  {
		if(ishole==2)
		{
          fprintf(outerrfile,"Error:  %s:\n  Found longitude %lf outside cell longitude range [%lf to %lf]\n\n",
			ParsePath(0),thevalue,cellWest,cellEast);
		}
		else
		{
          fprintf(outerrfile,"Error:  %s:\n  Found subregion longitude %lf outside cell longitude range [%lf to %lf]\n\n",
			ParsePath(0),thevalue,cellWest,cellEast);
		}
      }
	}
  }
  else
  {
    if((foundCellNorth==1)&&(foundCellSouth==1))
	{
      if((thevalue<cellSouth)||(thevalue>cellNorth))
	  {
		if(ishole==2)
		{
          fprintf(outerrfile,"Error:  %s:\n  Found latitude %lf outside cell latitude range [%lf to %lf]\n\n",
			ParsePath(0),thevalue,cellSouth,cellNorth);
		}
		else
		{
          fprintf(outerrfile,"Error:  %s:\n  Found subregion latitude %lf outside cell latitude range [%lf to %lf]\n\n",
			ParsePath(0),thevalue,cellSouth,cellNorth);
		}
      }
	}
  }


  if(ishole==2) 
  {
     
	 return 1;
  }


  for(i=0;i<NumSubregionPolys;i++)
  {
    if(
		(SubregionPolys[i].PolyID==PolyID) &&
		(SubregionPolys[i].subregionid==foundsubregionid)
		)
	{
      if(SubregionPolys[i].numverts==-1)
	  {
		
        return 0;
	  }

	  foundit = 1;

      if(xy==1)
	  {

        

		if(ishole==1)
		{
          

		  foundit = 0;

		  if(SubregionPolys[i].numholes==0)
		  {
			foundit = 1;
            SubregionPolys[i].numholes = 1;
            SubregionPolys[i].holes = (struct SubregionHoleStruct *)malloc(sizeof(struct SubregionHoleStruct));
            SubregionPolys[i].holes[0].x = (double *) malloc(sizeof(double));
            SubregionPolys[i].holes[0].y = (double *) malloc(sizeof(double));
            SubregionPolys[i].holes[0].x[0] = thevalue;
            SubregionPolys[i].holes[0].HoleID = HoleID;
            SubregionPolys[i].holes[0].numverts = 1;
            SubregionPolys[i].holes[0].usethis = 1;
		  }
		  else if(SubregionPolys[i].numholes<HoleID)
		  {
            
            foundit = 1;
            SubregionPolys[i].numholes = SubregionPolys[i].numholes + 1;

            SubregionPolys[i].holes = (struct SubregionHoleStruct *)realloc(SubregionPolys[i].holes,sizeof(struct SubregionHoleStruct)*SubregionPolys[i].numholes);

            SubregionPolys[i].holes[SubregionPolys[i].numholes-1].x = (double *) malloc(sizeof(double));
            SubregionPolys[i].holes[SubregionPolys[i].numholes-1].y = (double *) malloc(sizeof(double));
            SubregionPolys[i].holes[SubregionPolys[i].numholes-1].x[0] = thevalue;
            SubregionPolys[i].holes[SubregionPolys[i].numholes-1].HoleID = HoleID;
            SubregionPolys[i].holes[SubregionPolys[i].numholes-1].numverts = 1;
            SubregionPolys[i].holes[SubregionPolys[i].numholes-1].usethis = 1;
		  }
		  else
		  {
		    for(j=0;j<SubregionPolys[i].numholes;j++)
			{
              if(SubregionPolys[i].holes[j].HoleID==HoleID)
			  {
				foundit = 1;
                SubregionPolys[i].holes[j].numverts = SubregionPolys[i].holes[j].numverts + 1;
                SubregionPolys[i].holes[j].x = (double *) realloc(SubregionPolys[i].holes[j].x,sizeof(double)*(SubregionPolys[i].holes[j].numverts));
                SubregionPolys[i].holes[j].y = (double *) realloc(SubregionPolys[i].holes[j].y,sizeof(double)*(SubregionPolys[i].holes[j].numverts));
                SubregionPolys[i].holes[j].x[SubregionPolys[i].holes[j].numverts-1] = thevalue;
				break;
			  }
			}
		  }
		} 
		else
		{
          SubregionPolys[i].numverts = SubregionPolys[i].numverts + 1;
          SubregionPolys[i].x = (double *) realloc(SubregionPolys[i].x,sizeof(double)*(SubregionPolys[i].numverts));
          SubregionPolys[i].y = (double *) realloc(SubregionPolys[i].y,sizeof(double)*(SubregionPolys[i].numverts));
          SubregionPolys[i].x[SubregionPolys[i].numverts-1] = thevalue;
		}
	  }
	  else
	  {
		if(ishole==1)
		{
          for(j=0;j<SubregionPolys[i].numholes;j++)
		  {
            if(SubregionPolys[i].holes[j].HoleID==HoleID)
			{
              SubregionPolys[i].holes[j].y[SubregionPolys[i].holes[j].numverts-1] = thevalue;
			}
		  }
		}
		else
		{
		  
          SubregionPolys[i].y[SubregionPolys[i].numverts-1] = thevalue;
		}
	  }

	  break;
	}
  }


  if(foundit==0)
  {

	if(ishole==1)
	{
		

        fprintf(outerrfile,"Error:  %s:\n  Found interior of polygon before exterior\n\n",ParsePath(0));
		return 0;
	}

	if(NumSubregionPolys==0)
	{
      SubregionPolys = (struct SubregionPolyStruct *)malloc(sizeof(struct SubregionPolyStruct));

      SubregionPolys[0].PolyID      = PolyID;
	  SubregionPolys[0].subregionid = foundsubregionid;
      SubregionPolys[0].numholes    = 0;
      SubregionPolys[0].numverts    = 1;
      SubregionPolys[0].num_ftype   = 0;
      SubregionPolys[0].ftypes      = NULL;
      SubregionPolys[0].x = (double *) malloc(sizeof(double));
      SubregionPolys[0].y = (double *) malloc(sizeof(double));
      SubregionPolys[0].x[0] = thevalue;
	  NumSubregionPolys = 1;
	}
	else
	{
	  NumSubregionPolys = NumSubregionPolys + 1;
      SubregionPolys = (struct SubregionPolyStruct *)realloc(SubregionPolys,sizeof(struct SubregionPolyStruct)*NumSubregionPolys);

      SubregionPolys[NumSubregionPolys-1].PolyID      = PolyID;
	  SubregionPolys[NumSubregionPolys-1].subregionid = foundsubregionid;
      SubregionPolys[NumSubregionPolys-1].numholes    = 0;
      SubregionPolys[NumSubregionPolys-1].numverts    = 1;
      SubregionPolys[NumSubregionPolys-1].num_ftype   = 0;
      SubregionPolys[NumSubregionPolys-1].ftypes      = NULL;
      SubregionPolys[NumSubregionPolys-1].x = (double *) malloc(sizeof(double));
      SubregionPolys[NumSubregionPolys-1].y = (double *) malloc(sizeof(double));
      SubregionPolys[NumSubregionPolys-1].x[0] = thevalue;
	}

  }


  

 return 1;

}


int AddVertices(int PolyID,int foundsubregionid,int ishole)
{
  int i,len,insertindex,numverts=0,xy=1,succ;
  char thispiece[1000];

  


  

  


  len = strlen(thedata);

  insertindex = 0;
  thispiece[0] = '\0';

  for(i=0;i<len;i++)
  {
    
	if((thedata[i]==' ')&&(insertindex!=0))
	{
	  thispiece[insertindex] = '\0';
	  if(IsDecimal(thispiece)==0)
	  {
		if((ishole==0)||(ishole==1))
		{
		  MarkBad(PolyID,thispiece);
		}
        return 0;
	  }
	  else
	  {
        numverts = numverts + 1;

		succ = AddVertex(xy,thispiece,PolyID,ishole);

		if(xy==0)
		{
          xy = 1;
		}
		else
		{
          xy = 0;
		}

		if(succ==0)
		{
          return 0;
		}
	  }
      insertindex = 0;
	}
    else if(i==len-1)  
	{
	  thispiece[insertindex]   = thedata[i];
	  thispiece[insertindex+1] = '\0';
	  if(IsDecimal(thispiece)==0)
	  {
        return 0;
	  }
	  else
	  {
        numverts = numverts + 1;

		succ = AddVertex(xy,thispiece,PolyID,ishole);
		if(succ==0)
		{
          return 0;
		}
		if(xy==0)
		{
          xy = 1;
		}
		else
		{
          xy = 0;
		}
	  }
      insertindex = 0;
	}
	else if(thedata[i]!=' ')
	{
      thispiece[insertindex] = thedata[i];
      insertindex = insertindex + 1;
	}
  }

  return numverts;
}


void HandleSubregionPolygons(char *lasttoken)
{

  

  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_BoundingPolygon *** polygon *** Polygon *** exterior"))
  {
	

	if((lastsubregionid==-1)||(lastsubregionid!=foundsubregionid))
	{
      PolyID = 1;
	}
	else
	{
      PolyID = PolyID + 1;
	}

	HoleID = -1;

	
  }


  else if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_BoundingPolygon *** polygon *** Polygon *** interior"))
  {
	if(HoleID==-1)
	{
      HoleID = 1;
	}
	else
	{
      HoleID = HoleID + 1;
	}
	
  }


  else if(
	  (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_BoundingPolygon *** polygon *** Polygon *** exterior *** LinearRing *** pos"))     ||
      (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_BoundingPolygon *** polygon *** Polygon *** exterior *** LinearRing *** posList"))
	  )
  {
	if(AddVertices(PolyID,foundsubregionid,0)==0)
	{
       printf("Not doing subregion %d\n",foundsubregionid);
	}
  }


  else if(
	  (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_BoundingPolygon *** polygon *** Polygon *** interior *** LinearRing *** pos"))     ||
      (!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_BoundingPolygon *** polygon *** Polygon *** interior *** LinearRing *** posList"))
	  )
  {
	if(AddVertices(PolyID,foundsubregionid,1)==0)
	{
       printf("Not doing subregion %d\n",foundsubregionid);
	}
  }

  else if( (!strcmp(lasttoken,"pos")) || (!strcmp(lasttoken,"posList")) )
  {
    
	if(AddVertices(PolyID,foundsubregionid,2)==0)
	{
       printf("Found bad vertex other than subregion extent in subregion %d\n",foundsubregionid);
	}
  }

}


int OnEarth(double test, int ord_num)
{
  int xy;


  if((ord_num%2)==0)
  {
    xy = 2;
  }
  else
  {
    xy = 1;
  }


  if(xy==2)
  {
    
	if( (test<-180.0001) || (test > 180.0001) )
	{
	  printf("...x error with ord_num %d and xy %d\n",ord_num,xy);
      fprintf(outerrfile,"Error:  %s:\n   posList ordinate %d (a longitude value of %lf) is not in range [-180,180]\n\n",
        ParsePath(0),ord_num,test);
	  return 0;
	}
  }
  else
  {
    
	if( (test<-90.0001) || (test > 90.0001) )
	{
	  printf("...y error with ord_num %d and xy %d\n",ord_num,xy);
      fprintf(outerrfile,"Error:  %s:\n   posList ordinate %d (a latitude value of %lf) is not in range [-90,90]\n\n",
        ParsePath(0),ord_num,test);
	  return 0;
	}
  }
  return 1;
}




void HandlePosLists(char *lasttoken)
{
  double test;
  int len,i,insertindex,numordinate=0,type=0;
  char thispiece[1000],firstC[100],secondC[100],thirdC[100],fourthC[100];

  

  

  if(!strcmp(lasttoken,"posList"))
  {
	type = 1;
  }
  else if(!strcmp(lasttoken,"pos"))
  {
	type = 2;
  }

  if(type>0)
  {
    len = strlen(thedata);

    for(i=0;i<len;i++)
	{
      if(
		  (IsInt(thedata[i])==0)      &&
		  (thedata[i]!='.')           &&
		  (thedata[i]!=' ')
		  )
	  {


		if(thedata[i]=='-')
		{
           
		   if(i==0)
		   {
             continue;
		   }
		   if(thedata[i-1]==' ')
		   {
             continue;
		   }
		}

        if(thedata[i]>=32)
		{
          fprintf(outerrfile,"Error:  %s:\n   posList value \"%s\" has invalid value \"%c\" at position %d\n\n",
            ParsePath(0),thedata,thedata[i],i+1);
		}
		else
		{
          fprintf(outerrfile,"Error:  %s:\n   posList value \"%s\" has invalid value at position %d (newlines are not allowed)\n\n",
            ParsePath(0),thedata,i+1);
		}
	    return;
	  }
	}


    insertindex  = 0;
	firstC[0]    = '\0';
	secondC[0]   = '\0';
	thirdC[0]    = '\0';
	fourthC[0]   = '\0';
    thispiece[0] = '\0';


    for(i=0;i<len;i++)
	{ 
      
	  if((thedata[i]==' ')&&(insertindex!=0))
	  { 
	    thispiece[insertindex] = '\0';
	    if(IsDecimal(thispiece)==0)
		{
          return;
		} 

		sscanf(thispiece,"%lf",&test);

		if(OnEarth(test,numordinate))
		{
          if(numordinate==0)
		  {
			sprintf(firstC,"%s",thispiece);
		  }
		  else if(numordinate==1)
		  {
			sprintf(secondC,"%s",thispiece);
		  }
		  else
		  {
			sprintf(thirdC,"%s",fourthC);
			sprintf(fourthC,"%s",thispiece);
		  }
		}

        numordinate = numordinate + 1;

        insertindex = 0;
	  } 
	  else if(i==len-1)  
	  { 
	    thispiece[insertindex]   = thedata[i];
	    thispiece[insertindex+1] = '\0';

	    if(IsDecimal(thispiece)==0)
		{ 
          return;
		}

		sscanf(thispiece,"%lf",&test);

		if(OnEarth(test,numordinate))
		{
          if(numordinate==0)
		  {
 			sprintf(firstC,"%s",thispiece);
		  }
		  else if(numordinate==1)
		  {
			sprintf(secondC,"%s",thispiece);
		  }
		  else
		  {
			sprintf(thirdC,"%s",fourthC);
			sprintf(fourthC,"%s",thispiece);
		  }
		}

        numordinate = numordinate + 1;

        insertindex = 0;
	  }
	  else if(thedata[i]!=' ')
	  { 
        thispiece[insertindex] = thedata[i];
        insertindex = insertindex + 1;
	  } 
	} 



  if((type==2)&&(numordinate!=2))
  {
    fprintf(outerrfile,"Error:  %s:\n   pos %s has %d ordinates, exactly 2 are required (1 longitude and 1 latitude)\n\n",
      ParsePath(0),thedata,numordinate);
	return;
  }
  else if((type==1)&&(numordinate<8))
  {
    fprintf(outerrfile,"Error:  %s:\n   posList %s has only %d ordinates, at least 8 are required (4 longitude and 4 latitude)\n\n",
      ParsePath(0),thedata,numordinate);
	return;
  }
  if(numordinate%2==1)
  {
    fprintf(outerrfile,"Error:  %s:\n   posList %s has %d ordinates, a multiple of 2 are required (1 longitude latitude per coordinate)\n\n",
      ParsePath(0),thedata,numordinate);
	return;
  }
  if(type==1)
  {
    if((strcmp(firstC,thirdC))||(strcmp(secondC,fourthC)))
	{
      fprintf(outerrfile,"Error:  %s:\n   posList %s does not have matching first (%s %s) and last (%s %s) coordinates\n\n",
        ParsePath(0),thedata,firstC,secondC,thirdC,fourthC);
	  return;
	} 
  }
 } 
}


extern "C" int ValidSubregionSource(int thisindex)
{
  int thismax;


  if(SubregionSources[thisindex].index<0)
  {
    return 0;
  }

  if(version==2)
  {
    thismax = SourceTypeIdsMax2*3;
  }
  else if(version==3)
  {
    thismax = SourceTypeIdsMax3*3;
  }
  else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    thismax = SourceTypeIdsMax4*3;
  }
		
  if(SubregionSources[thisindex].index>=thismax)
  {
    return 0;
  }

  return 1;
}


extern "C" const char *GetSourceType(int index)
{
  if(version==2)
  {
    if(index<=SourceTypeIdsMax2*3)
	{
      return SourceTypeIds2[index];
	}
	return "Bad Index2";
  }
  else if(version==3)
  {
    if(index<SourceTypeIdsMax3*3)  
	{
      return SourceTypeIds3[index];
	}
	return "Bad index3";
  }
  else if(version==4)
  {
    if(index<=SourceTypeIdsMax4*3)
	{
      return SourceTypeIds4[index];
	}
	return "Bad index4";
  }
  else if((version==41)||(version==42)||(version==43))
  {
    if(index<=SourceTypeIdsMax41*3)
	{
      return SourceTypeIds41[index];
	}
	return "Bad index4";
  }
  else if(version==44)
  {
    if(index<=SourceTypeIdsMax44*3)
	{
      return SourceTypeIds44[index];
	}
	return "Bad index4";
  }
  return "Not Found Yet";
}


int GetSourceIndex(char *sourcetype,int thismax)
{
  int i;

  

  for(i=0;i<thismax*3;i=i+3)
  {
	if( (!strcmp(sourcetype,thearray[i])) || (!strcmp(sourcetype,thearray[i+1])) )
	{
      return i;
	}
  }

  return -1;

}


int IsInteger(char *instr)
{
  int i,len;

  len = strlen(instr);

  for(i=0;i<len;i++)
  {
    if(instr[i]<'0')
	{ 
      return 0;
	} 
    if(instr[i]>'9')
	{ 
      return 0;
	} 
  }
  return 1;
}





void Handle44SourceScales(int havescale, char *thescale,char *scalelinenum)
{
  int i,j,index,thismax;
  int i_scale;
  char temp[1000];


  

  if(havescale<=0)
  {
	
	return;
  }


  if(IsInteger(thescale))
  {
    i_scale=atoi(thescale);
  }
  else
  {
	return;
  }


  if(version==44)
  {
    thearray = SourceTypeIds44;
    thismax = SourceTypeIdsMax44;
  }


  for(i=0;i<numattributes;i++)
  { 
	if(  
	  (!strcmp(attributes[i],"codeList")) &&
	  (!strcmp(attributeV[i],"mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_SourceTypeId"))
	 )
	{ 
      for(j=i+1;j<numattributes;j++)
	  {
        if(!strcmp(attributes[j],"codeListValue"))
		{  

		  index = GetSourceIndex(attributeV[j],thismax);

		  if(index>0)
		  {
			

            if(!strcmp(SourceScales44[index/3],"NoSpec"))
			{ 
			  return;
			} 
            else if(!strcmp(SourceScales44[index/3],"BlankOnly"))
			{ 
			  sprintf(temp,"Blank Scale for source %s (line %s)",thearray[index],scalelinenum);
		      WriteExpected(1,"Expected",temp,thescale);
			} 
	        else if(!strcmp(SourceScales44[index/3],"2,500 to 25,000"))
			{ 
			  if((i_scale<2500)||(i_scale>25000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"5,000 to 50,000"))
			{ 
			  if((i_scale<5000)||(i_scale>50000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"<=10,000"))
			{ 
			  if((i_scale<1)||(i_scale>10000))
			  {
			    sprintf(temp,"Scale \"1 to %s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"10,000"))
			{ 
			  if(i_scale!=10000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"10,001 to 50,000"))
			{ 
			  if((i_scale<10001)||(i_scale>50000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"25,000"))
			{ 
			  if(i_scale!=25000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"25,000 to 50,000"))
			{ 
			  if((i_scale<25000)||(i_scale>50000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"25,000 or 50,000"))
			{ 
			  if((i_scale!=25000)&&(i_scale!=50000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"50,000"))
			{ 
			  if(i_scale!=50000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"50,000 or 100,000"))
			{ 
			  if((i_scale!=50000)&&(i_scale!=100000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"50,000 to 250,000"))
			{ 
			  if((i_scale<50000)||(i_scale>250000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"50,000 to 500,000"))
			{ 
			  if((i_scale<50000)||(i_scale>500000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"50,001 to 150,000"))
			{ 
			  if((i_scale<50001)||(i_scale>150000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"100,000"))
			{ 
			  if(i_scale!=100000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],">150,000"))
			{ 
			  if(i_scale<=150000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"250,000"))
			{ 
			  if(i_scale!=250000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"250,000 to 120,000,000"))
			{ 
			  if((i_scale<250000)||(i_scale>120000000))
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],"1,000,000"))
			{ 
			  if(i_scale!=1000000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else if(!strcmp(SourceScales44[index/3],">=5,000,000"))
			{ 
			  if(i_scale<5000000)
			  {
			    sprintf(temp,"Scale \"%s\" for source %s (line %s)",SourceScales44[index/3],thearray[index],scalelinenum);
		        WriteExpected(1,"Expected",temp,thescale);
			  }
			} 
	        else
			{ 
	          printf("Internal error: CSS: unexpected %s\n",SourceScales44[index/3]);
			} 
		  }  
		}   
	  }  
	} 
  } 
}


void HandleSourceScales()
{
  static int havescale=0;
  static char thelinenum[100],thescale[500];
  int i,j;

  


  if(version<4)
  {
    return;
  }


  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source"))
  {
    
	havescale  = 0;
  }

  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** scaleDenominator *** MD_RepresentativeFraction *** denominator *** Integer"))
  {
    havescale = 1;
	if(DoLineNumbers>0)
	{
	  sprintf(thelinenum,"%s",GetLineNumber());
	  sprintf(thescale,"%s",thedata);
	}
  }




  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** series *** CI_Series *** name *** MGCP_SourceTypeId"))
  {

	if(version==44)
	{
	  Handle44SourceScales(havescale,thescale,thelinenum);
	  return;
	}

   if(havescale>0)
   {
    for(i=0;i<numattributes;i++)
	{ 
     if(  
	 	(!strcmp(attributes[i],"codeList")) &&
		(!strcmp(attributeV[i],"mgcp/Codelist/mgcp_gmxCodelists.xml#MGCP_SourceTypeId"))
	   )
	 {
      for(j=i+1;j<numattributes;j++)
	  {
       if(!strcmp(attributes[j],"codeListValue"))
	   { 

		if(
			(!strcmp(attributeV[j],"aafif")) ||
			(!strcmp(attributeV[j],"AAFIF")) ||
			(!strcmp(attributeV[j],"NGA Automated Aeronautical Facilities Information File")) ||
			(!strcmp(attributeV[j],"NGA Automated Aeronautical Facilities Information File.")) ||
			(!strcmp(attributeV[j],"ngaAutoAirFacInfoFile")) ||

			(!strcmp(attributeV[j],"dvof")) ||
			(!strcmp(attributeV[j],"DVOF")) ||
			(!strcmp(attributeV[j],"Digital Vertical Obstruction File")) ||
			(!strcmp(attributeV[j],"NGA Digital Vertical Obstruction File.")) ||
			(!strcmp(attributeV[j],"ngaDigitalVertObstruction")) ||

			(!strcmp(attributeV[j],"names")) ||
			(!strcmp(attributeV[j],"GeoNames")) ||
			(!strcmp(attributeV[j],"NGA GeoNames."))||
			(!strcmp(attributeV[j],"ngaGeoNames"))

		  )
		{
		  if(DoLineNumbers>0)
		  {
            fprintf(outerrfile,"Error:  %s:\n   SSRCSC (source scale) used (on line %s) with source type \"%s\".\n     Scale may not be used for AAFIF, DVOF, or GeoNames.\n\n",ParsePath(0),thelinenum,attributeV[j]);
		  }
		  else
		  {
            fprintf(outerrfile,"Error:  %s:\n   SSRCSC (source scale) used with source type \"%s\".\n     Scale may not be used for AAFIF, DVOF, or GeoNames.\n\n",ParsePath(0),attributeV[j]);
		  }
		}
	   } 
	  } 
	 } 
	} 
   }  
  }  
}



void HandleNewestOldestSub()
{
  int i,thismax,year,month,day,theindex,foundoldnew=0;
  char test[100];
  static int type=0,saveindex=-1,saveindex2=-1; 


  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source"))
  {
    
    potential_SSVRTI_found = 0;
    potential_SSVCDT_found = 0;
  }


  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** identifier *** RS_Identifier *** code *** CharacterString"))
	{ 
      

      

      if(saveindex>=0)
	  {
		
        
		if(strcmp(thedata,GetSourceType(saveindex)))
		{
		  WriteExpected(1,"Expected",GetSourceType(saveindex),thedata);
		}
	  }
      else
	  {
        if(saveindex2>0)
		{
	      if(version==4)
		  {
            

		    if(strcmp(thedata,VerticalSourceTypeIds4[saveindex2]))
			{
		      WriteExpected(1,"Expected",VerticalSourceTypeIds4[saveindex2],thedata);
			}
		  }
          if((version==41)||(version==42)||(version==43)||(version==44))
		  {  
            

		    if(strcmp(thedata,VerticalSourceTypeIds41[saveindex2]))
			{
		      WriteExpected(1,"Expected",VerticalSourceTypeIds41[saveindex2],thedata);
			}
		  } 
		} 
	    else
		{ 
	      printf("cannot validate %s because no saved source\n",thedata);
		}
	  }
	} 
  } 



  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** title *** CharacterString"))
  {
	
	potential_SSVRTI_found = 1;
	sprintf(potentialSSVRTI,"%s",thedata);
  }
  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** date *** CI_Date *** date *** Date"))
  {
	
	potential_SSVCDT_found = 1;
    sprintf(potentialSSVCDT,"%s",thedata);
  }
  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** series *** CI_Series *** name *** MGCP_VerticalSourceTypeId"))
  {
	SSVCTY_found = 1;
	if(potential_SSVRTI_found>0)
	{
      SSVRTI_found = 1;
	  sprintf(SSVRTI,"%s",potentialSSVRTI);
	}
	if(potential_SSVCDT_found>0)
	{
      SSVCDT_found = 1;
      sprintf(SSVCDT,"%s",potentialSSVCDT);
	}
  }

  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** series *** CI_Series *** name *** MGCP_SourceTypeId"))
  {
	SSRCTY_found = 1;

    if(version==2)
	{ 
      thearray = SourceTypeIds2;
      thismax = SourceTypeIdsMax2;
	} 
    else if(version==3)
	{
      thearray = SourceTypeIds3;
      thismax = SourceTypeIdsMax3;
	}
    else if(version==4)
	{
      thearray = SourceTypeIds4;
      thismax = SourceTypeIdsMax4;
	}
    else if((version==41)||(version==42)||(version==43))
	{
      thearray = SourceTypeIds41;
      thismax = SourceTypeIdsMax41;
	}
    else if(version==44)
	{
      thearray = SourceTypeIds44;
      thismax = SourceTypeIdsMax44;
	}

	theindex = -1;
	for(i=0;i<thismax*3;i=i+3)
	{
	  if( (!strcmp(thedata,thearray[i])) || (!strcmp(thedata,thearray[i+1])) )
	  {
        theindex = i;
        break;
	  }
	}

	if(theindex<0)
	{
      WriteExpected(1,"Expected","SSRCDT = valid <Source Type Identifier>",thedata);
	  saveindex  = -1;
	  saveindex2 = -1;
	}
	else
	{
      saveindex = theindex;
	}
  }



  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** date *** CI_Date *** date *** Date"))
  {
	SSRCDT_found = 1;

    if(type>0)  
	{

      

      if(ValidateDate(1,&year,&month,&day)==1)
	  {

        

        if(NumSubregionSources==0)
		{ 
          SubregionSources = (struct SubregionSourceStruct *)malloc(sizeof(struct SubregionSourceStruct));
          SubregionSources[0].index = saveindex;
          SubregionSources[0].subregionid = foundsubregionid;
          SubregionSources[0].type        = type;
          SubregionSources[0].year        = year;
          SubregionSources[0].month       = month;
          SubregionSources[0].day         = day;
          sprintf(SubregionSources[0].savedate,"%s",thedata);
          NumSubregionSources = 1;
		} 
	    else
		{ 
          NumSubregionSources = NumSubregionSources + 1;
          SubregionSources = (struct SubregionSourceStruct *)realloc(SubregionSources,sizeof(struct SubregionSourceStruct)*NumSubregionSources);
          SubregionSources[NumSubregionSources-1].index = saveindex;
          SubregionSources[NumSubregionSources-1].subregionid = foundsubregionid;
          SubregionSources[NumSubregionSources-1].type        = type;
          SubregionSources[NumSubregionSources-1].year        = year;
          SubregionSources[NumSubregionSources-1].month       = month;
          SubregionSources[NumSubregionSources-1].day         = day;
          sprintf(SubregionSources[NumSubregionSources-1].savedate,"%s",thedata);
		} 
	  } 
	} 
  }


  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** title *** CharacterString"))
  {

	SSCRTI_found = 1;

	type = 0;
	saveindex  = -1;
	saveindex2 = -1;


	if(version==4)
	{
      

	  for(i=0;i<VerticalSourceTypeIdsMax4*3;i=i+3)
	  { 
		if(!strcmp(thedata,VerticalSourceTypeIds4[i+1]))
		{
		  saveindex2 = i;
          return;
		} 
	  }
	}

	if((version==41)||(version==42)||(version==43)||(version==44))
	{
      

	  for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
	  { 
		if(!strcmp(thedata,VerticalSourceTypeIds41[i+1]))
		{
		  saveindex2 = i;
          return;
		} 
	  }
	}




	if(
		(!strncmp(thedata,"Newest",6))  ||
		(!strncmp(thedata,"newest",6))  ||
		(!strncmp(thedata,"NEWEST",6))  ||
		(!strncmp(thedata,"Oldest",6))  ||
		(!strncmp(thedata,"oldest",6))  ||
		(!strncmp(thedata,"OLDEST",6))
      )
	{
      
      foundoldnew = 1;
	}

    if(version==2)
	{ 
      thearray = SourceTypeIds2;
      thismax = SourceTypeIdsMax2;
	} 
    else if(version==3)
	{
      thearray = SourceTypeIds3;
      thismax = SourceTypeIdsMax3;
	}
    else if(version==4)
	{
      thearray = SourceTypeIds4;
      thismax = SourceTypeIdsMax4;
	}
    else if((version==41)||(version==42)||(version==43))
	{
      thearray = SourceTypeIds41;
      thismax = SourceTypeIdsMax41;
	}
    else if(version==44)
	{
      thearray = SourceTypeIds44;
      thismax = SourceTypeIdsMax44;
	}


	for(i=0;i<thismax*3;i=i+3)
	{

      if(version<4)
	  {
        sprintf(test,"%s",thearray[i]);
	  }
	  else
	  {
        sprintf(test,"%s",thearray[i+1]);
	  }
	  if(!strcmp(thedata,test))
	  {
        

        type = 3;
        break;
	  }




      if(version<4)
	  {
        sprintf(test,"Oldest %s",thearray[i]);    
	  }
	  else
	  {
        sprintf(test,"Oldest %s",thearray[i+1]);  
	  }

	  type = 1; 
	  if(!strcmp(thedata,test))
	  {
		if(!strcmp(test,"Oldest GeoNames"))
		{
          fprintf(outerrfile,"Error:  %s:\n   Subregion %d: Found \"%s\"...illegal to use Newest/Oldest for GeoNames\n\n",
			  ParsePath(0),foundsubregionid,thedata);
		}
        break;
	  }



	  type = 2; 
      if(version<4)
	  {
        sprintf(test,"Newest %s",thearray[i]);    
	  }
	  else
	  {
        sprintf(test,"Newest %s",thearray[i+1]);  
	  }



	  if(!strcmp(thedata,test))
	  {
		if(!strcmp(test,"Newest GeoNames"))
		{
          fprintf(outerrfile,"Error:  %s:\n   Subregion %d: Found \"%s\"...illegal to use Newest/Oldest for GeoNames\n\n",
			  ParsePath(0),foundsubregionid,thedata);
		}
        break;
	  }
	}


	if((i>=thismax*3) && (foundoldnew==1))     
	{
      WriteExpected(1,"Expected","\"Oldest <Source Type Identifier>\" or \"Newest <Source Type Identifier>\"",thedata);

      
      type = 0;
	}
	else if((i>=thismax*3)&&((version==4)||(version==41)||(version==42)||(version==43)||(version==44)))
	{
      WriteExpected(1,"Expected","\"Oldest <Source Type Identifier>\", \"Newest <Source Type Identifier>\", <Source Type Identifier>, or <Vertical Source Type Identifier>",thedata);
      type = 0;
	}
	else
	{
	  
	  if(saveindex<0) 
	  {
        saveindex = i;

		if(saveindex>=thismax*3)
		{
           printf("Was not able to map type \"%s\"\n",thedata);
		}
	  }
	}
  }
}

 

void HandleNewestOldest()
{
  int year,month,day;
  static int LookingForNewest=0,LookingForOldest = 0;

  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** title *** CharacterString"))
  { 
    LookingForNewest = 0;
    LookingForOldest = 0;

    if(!strcmp(thedata,"Newest source"))
	{ 
      LookingForNewest = 1;
	} 
    else if(!strcmp(thedata,"Oldest source"))
	{ 
      LookingForOldest = 1;
	} 
	else
	{
      WriteExpected(1,"Expected","\"Oldest source\" or \"Newest source\"",thedata);

	  
	}
  }


  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** lineage *** LI_Lineage *** source *** LI_Source *** sourceCitation *** CI_Citation *** date *** CI_Date *** date *** Date"))
  {
    if(LookingForNewest==1)  
	{
      if(ValidateDate(1,&year,&month,&day)==1)
	  { 
		FoundNewest = 1;
        NewestYear  = year;
        NewestMonth = month;
        NewestDay   = day;
		sprintf(SAVENEWEST,"%s",thedata);
	  }
	}
    else if(LookingForOldest==1)  
	{
      if(ValidateDate(1,&year,&month,&day)==1)
	  { 
		FoundOldest = 1;
        OldestYear  = year;
        OldestMonth = month;
        OldestDay   = day;
		sprintf(SAVEOLDEST,"%s",thedata);
	  }
	}
    LookingForNewest = 0;
    LookingForOldest = 0;
  }
}




void HandleSubregionDescriptions()
{
  char testchar[200],thenumber[50];

  if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** abstract *** CharacterString"))
  {

	SDESCR_found = 1;

	if((version==3)||((version==4)||(version==41)||(version==42)||(version==43)||(version==44)))
	{


      if((version==3)||(version==4))  
	  {
        if(!strcmp(thedata,"Update subregion"))
		{  
          return;
		}  
        if(!strcmp(thedata,"Producer subregion"))
		{  
          return;
		}  
	  }


	  if(foundsubregionid<10)
	  { 
        sprintf(thenumber,"Subregion0%d",foundsubregionid);
	  } 
	  else
	  { 
        sprintf(thenumber,"Subregion%d",foundsubregionid);
	  } 

      if(!strcmp(thedata,thenumber))
	  { 
        if(foundsubregionid==1)
		{
          NonSingleSR = 1;
		}
        return;
	  } 
	}

    if(!strcmp(thedata,"Single subregion"))
	{ 
	  SingleSR = 1;
      return;
	} 


	if((version==3)||(version==4))
	{
	  sprintf(testchar,"\"Single subregion\", \"Update subregion\", \"Producer subregion\", or \"%s\"",thenumber);
      WriteExpected(1,"Expected SDESCR value",testchar,thedata);
	}
	else if((version==41)||(version==42)||(version==43)||(version==44))
	{
	  sprintf(testchar,"\"Single subregion\" or \"%s\"",thenumber);
      WriteExpected(1,"Expected SDESCR value",testchar,thedata);
	}

  }
}


void HandleCellExtent()
{

 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_GeographicBoundingBox *** westBoundLongitude *** Decimal"))
 {
   if(IsDecimal(thedata)==1)
   {
     foundCellWest = 1;
	 sprintf(cellWestLine,"%s",GetLineNumber());
	 sscanf(thedata,"%lf",&cellWest);
	 sprintf(saveCellWest,"%s",thedata);
   }
 }
 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_GeographicBoundingBox *** eastBoundLongitude *** Decimal"))
 {
   if(IsDecimal(thedata)==1)
   {
     foundCellEast = 1;
	 sprintf(cellEastLine,"%s",GetLineNumber());
	 sscanf(thedata,"%lf",&cellEast);
	 sprintf(saveCellEast,"%s",thedata);
   }
 }
 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_GeographicBoundingBox *** southBoundLatitude *** Decimal"))
 {
   if(IsDecimal(thedata)==1)
   {
     foundCellSouth = 1;
	 sprintf(cellSouthLine,"%s",GetLineNumber());
	 sscanf(thedata,"%lf",&cellSouth);
	 sprintf(saveCellSouth,"%s",thedata);
   }
 }
 if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** extent *** EX_Extent *** geographicElement *** EX_GeographicBoundingBox *** northBoundLatitude *** Decimal"))
 {
   if(IsDecimal(thedata)==1)
   {
     foundCellNorth = 1;
	 sprintf(cellNorthLine,"%s",GetLineNumber());
	 sscanf(thedata,"%lf",&cellNorth);
	 sprintf(saveCellNorth,"%s",thedata);
   }
 }

}


void HandleCFCATR()
{
  int i;

  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** title *** Anchor"))
  {
    

    for(i=0;i<numattributes;i++)
	{
      if(!strcmp(attributes[i],"xlink:href"))
	  {
        sprintf(CellFCName,"%s",attributeV[i]);
	  }
	}
  }
}



void HandleCDESCR()
{
  char compareSTR[500];

  if(cdescr_found==2)
  {
    
    return;
  }

  if(!strcmp(thepath,"MGCP_Cell *** has *** MD_Metadata *** identificationInfo *** MD_DataIdentification *** abstract *** CharacterString"))
  {
    sprintf(saveCDESCR,"%s",thedata);
	cdescr_found = 1;
	sprintf(SaveCDESCRNumber,"%s",GetLineNumber());


	if(strlen(saveCDESCR)<142)
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR \"%s\" (length %d) is too short.\n\n",ParsePath(0),thedata,(int)strlen(saveCDESCR));
      cdescr_found = 2;
	}
  }

  if(
	  (foundCellWest==1)  &&
	  (foundCellEast==1)  &&
	  (foundCellNorth==1) &&
	  (foundCellSouth==1) &&
	  (cdescr_found==1)
	)
  {
	

	cdescr_found = 2;
    UseCDESCRNumber = 1;

	if(strncmp(saveCDESCR,"Multinational Geospatial Co-production Program (MGCP) dataset covering the ",75))
	{
	  sprintf(compareSTR,"%s",saveCDESCR);
	  compareSTR[75] = '\0';

      fprintf(outerrfile,"Error:  %s:\n   CDESCR: %s\n",ParsePath(0),saveCDESCR);
	  fprintf(outerrfile,"  Found   : \"%s\"\n",compareSTR);
	  fprintf(outerrfile,"  Expected: \"Multinational Geospatial Co-production Program (MGCP) dataset covering the \"\n\n");
      UseCDESCRNumber = 0;
	  return;
	}

	if(saveCDESCR[75]!='1')
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 76 should be '1' (hex 31) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[75]);
	}
	if((unsigned char)saveCDESCR[76]!=0xC2)
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 77 should be hex C2 (first UTF-8 byte of the degree symbol) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[76]);
	}
	if((unsigned char)saveCDESCR[77]!=0xB0)
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 78 should be hex B0 (second UTF-8 byte of the degree symbol) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[77]);
	}

	if(saveCDESCR[78]!='x')
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 79 should be 'x' (hex 78) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[78]);
	}

	if(saveCDESCR[79]!='1')
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 80 should be '1' (hex 31) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[79]);
	}
	if((unsigned char)saveCDESCR[80]!=0xC2)
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 81 should be hex C2 (first UTF-8 byte of the degree symbol) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[80]);
	}
	if((unsigned char)saveCDESCR[81]!=0xB0)
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR, character 82 should be hex B0 (second UTF-8 byte of the degree symbol) but is (hex) %02x\n\n",ParsePath(0),(unsigned char)saveCDESCR[81]);
	}


	sprintf(compareSTR," degree cell between %d and %d longitudes and %d and %d latitudes.",
		(int)cellWest,(int)cellEast,(int)cellSouth,(int)cellNorth);


	if(strcmp(compareSTR,&saveCDESCR[82]))
	{
      fprintf(outerrfile,"Error:  %s:\n   CDESCR: %s\n",ParsePath(0),saveCDESCR);
	  fprintf(outerrfile,"  Found    : \"%s\"\n",&saveCDESCR[82]);
	  fprintf(outerrfile,"  Expected: \"%s\"\n\n",compareSTR);
	}

	UseCDESCRNumber = 0;
  }
}


void HandleItem(char *lasttoken, int expected_version)
{
  int i,valuelen;

  valuelen = strlen(thedata);

  if(valuelen<1)
  {
    HandleBlankValues(lasttoken);
  }


  
  HandleFCDates();
  HandleCellDataFiles();
  HandleCellFeatureCounts();
  HandleSubregionPolygons(lasttoken);
  HandleInsideSubregions(0);
  HandleNewestOldestSub();
  HandleSourceScales();
  HandleFixedCatalogs(0);
  HandleNewSubregion();
  HandleCFCATR();
  HandleCDESCR();

  if((valuelen>0)||(numattributes>0))
  {
	HandleCellExtent();

	HandleCodeLists(lasttoken);

	HandleSubregionDescriptions();

	TestFixedAttributes();

	if(valuelen>0)
	{
      HandleFixedStringValues();
	  HandleGAITVersion(expected_version);
	  HandleDates(lasttoken);
	  HandleCellIds();
	  HandleSubregionIds();
	  HandleCodeSpaces();
	  HandleReasonableValues(lasttoken);
	  HandleISO();
	  HandleSubregionISO();
	  HandlePosLists(lasttoken);
      HandleSubregionFeatureTypes();
	  HandleNewestOldest();
	}

 
    if(DOOUTXML==1)
	{
      fprintf(outxmlfile,"\n\n");
      fprintf(outxmlfile,"path %s\n",thepath);
      fprintf(outxmlfile,"  value %s\n",thedata);




      fprintf(outxmlfile,"%d attributes:\n",numattributes);
      for(i=0;i<numattributes;i++)
	  {   
       fprintf(outxmlfile,"%s  :  %s\n",attributes[i],attributeV[i]);
	   

	  }  
	}
  }
}



void CompareCellidCuri()
{
  char tester[500];

  
  

  sprintf(tester,"%s",SAVECURI);
  sprintf(&tester[20],"%s",SAVECELLID);

  if(
      (!strncmp(SAVECURI,"https://www.mgcp.ws/",20)) &&
      (strlen(SAVECURI)==27)
	  )
  {
    if(strcmp(tester,SAVECURI))
	{
      fprintf(outerrfile,"Error:  %s:\n   CURI value \"%s\" does not have match dataset CELLID value %s (expected https://www.mgcp.ws/%s)\n\n",
        ParsePath(1),SAVECURI,SAVECELLID,SAVECELLID);
	}
  }
}



int CompareCCCEDates()
{
  

  if(CEDDATyear<CCDATEyear)
  {
    return 0;
  }
  else if(CEDDATyear==CCDATEyear)
  {
    if(CEDDATmonth<CCDATEmonth)
	{ 
      return 0;
	}
	else if(CEDDATmonth==CCDATEmonth)
	{
      if(CEDDATday<CCDATEday)
	  { 
        return 0;
	  }	 
	}
  }
  return 1;
}


int CompareCMCEDates()
{
  

  if(CEDDATyear<CMDATEyear)
  {
    return 0;
  }
  else if(CEDDATyear==CMDATEyear)
  {
    if(CEDDATmonth<CMDATEmonth)
	{ 
      return 0;
	}
	else if(CEDDATmonth==CMDATEmonth)
	{
      if(CEDDATday<CMDATEday)
	  { 
        return 0;
	  }	 
	}
  }
  return 1;
}


int CompareNewOldDates()
{
  

  if(NewestYear<OldestYear)
  {
    return 0;
  }
  else if(NewestYear==OldestYear)
  {
    if(NewestMonth<OldestMonth)
	{ 
      return 0;
	}
	else if(NewestMonth==OldestMonth)
	{
      if(NewestDay<OldestDay)
	  { 
        return 0;
	  }	 
	}
  }
  return 1;
}



int TestOpenShapefile(char *shapepath,char *shapename)
{
  FILE *testfile;
  int len,replposn;
  char tester[1000];


  if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
  {
    sprintf(tester,"%s\\%s.shp",shapepath,shapename);
  }
  else
  {
    sprintf(tester,"%s/%s.shp",shapepath,shapename);
  }

  len = strlen(tester);
  replposn = len-3;


  testfile = fopen(tester,"rb");
  if(testfile==NULL)
  {
    sprintf(&tester[replposn],"SHP");
    testfile = fopen(tester,"rb");
	if(testfile==NULL)
	{
      return 0;
	}
	else
	{
      fclose(testfile);
	}
  }
  else
  {
    fclose(testfile);
  }



  sprintf(&tester[replposn],"shx");
  testfile = fopen(tester,"rb");
  if(testfile==NULL)
  {
    sprintf(&tester[replposn],"SHX");
    testfile = fopen(tester,"rb");
	if(testfile==NULL)
	{
      return 0;
	}
	else
	{
      fclose(testfile);
	}
  }
  else
  {
    fclose(testfile);
  }


  sprintf(&tester[replposn],"dbf");
  testfile = fopen(tester,"rb");
  if(testfile==NULL)
  {
    sprintf(&tester[replposn],"DBF");
    testfile = fopen(tester,"rb");
	if(testfile==NULL)
	{
      return 0;
	}
	else
	{
      fclose(testfile);
	}
  }
  else
  {
    fclose(testfile);
  }

  return 1;
}


char *GetContainingFolder(char *pathname)
{
  char goodpath[1000],goodpath2[1000];
  int len,i,lastfound=-1,path1found=0,path2found=0,firstfound,secondfound;
  static char junkreturn[20];

  sprintf(junkreturn,"!!!");

  

  sprintf(goodpath,"%s",pathname);
  len = strlen(goodpath);

  if(
	 (goodpath[len-1]=='/')||
	 (goodpath[len-1]=='\\')
	 )
	{
	  goodpath[len-1]='\0';
	}

  len = strlen(goodpath);
  for(i=0;i<len;i++)
  {
    if(
       (goodpath[i]=='/')||
       (goodpath[i]=='\\')
	  )
	{
	  lastfound = i;
	}
  } 

  if(lastfound<0)
  {
    printf("Error parsing pathname %s  Unable to process shapefiles\n",goodpath);
    fprintf(outerrfile,"Error parsing pathname %s  Unable to process shapefiles\n\n",goodpath);
	return junkreturn;
  }


  goodpath[lastfound] = '\0';
  if(foundcellid==1)
  {
    if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
	{
      sprintf(goodpath2,"%s\\%s",goodpath,SAVECELLID);
	}
	else
	{
      sprintf(goodpath2,"%s/%s",goodpath,SAVECELLID);
	}
  }
  else
  {
    sprintf(goodpath2,"%s",goodpath);
  }


  for(i=0;i<alldatafiles;i++)
  {
	firstfound  = TestOpenShapefile(goodpath, AllDataFiles[i]);
	secondfound = TestOpenShapefile(goodpath2,AllDataFiles[i]);

	if(firstfound==1)
	{
      path1found = path1found + 1;
      AllDataFilesPresent[i] = AllDataFilesPresent[i] + 10;
	}
	if(secondfound==1)
	{
      path2found = path2found + 1;
      AllDataFilesPresent[i] = AllDataFilesPresent[i] + 1;
	}
  }

  if((path1found==0)&&(path2found==0))
  {
    fprintf(outerrfile,"Not processing shapefiles because none of the %d shapefiles were found in either:\n %s or\n %s\n\n",alldatafiles,goodpath,goodpath2);
    return junkreturn;
  }

  if(path1found>path2found)
  {
	ShapeFolder = (char *)malloc(strlen(goodpath)+10);
	sprintf(ShapeFolder,"%s",goodpath);

	for(i=0;i<alldatafiles;i++)
	{
      if(AllDataFilesPresent[i]<10)
	  {
        fprintf(outerrfile,"Error:  Did not find shapefile %s in %s\n  Unable to process this shapefile\n\n",
			AllDataFiles[i],ShapeFolder);
		AllDataFilesPresent[i] = 0;
	  }
	  else
	  {
		AllDataFilesPresent[i] = 1;
	  }
	}
  }
  else
  {
	ShapeFolder = (char *)malloc(strlen(goodpath2)+10);
	sprintf(ShapeFolder,"%s",goodpath2);

	for(i=0;i<alldatafiles;i++)
	{
      if((AllDataFilesPresent[i]!=1)&&(AllDataFilesPresent[i]!=11))
	  {
        fprintf(outerrfile,"Error:  Did not find shapefile %s in %s\n  Unable to process this shapefile\n\n",
			AllDataFiles[i],ShapeFolder);
		AllDataFilesPresent[i] = 0;
	  }
	  else
	  {
		AllDataFilesPresent[i] = 1;
	  }
	}
  }

  
  return ShapeFolder;
}


void FindExtraShapefiles(char *folderpath)
{
  FILE *infile;
  int i,len,foundit;
  char shapelist[1000],tester[1000],tester2[1000];


  printf("Looking in folder %s for extra shapefiles not referenced in XML\n",folderpath);


  for(i=0;i<alldatafiles;i++)
  {
    if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
	{ 
      sprintf(tester,"%s\\%s.shp",folderpath,AllDataFiles[i]);
	} 
    else
	{ 
      sprintf(tester,"%s/%s.shp",folderpath,AllDataFiles[i]);
	} 

  }




  if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
    {
      sprintf(shapelist,"%s\\shapelist.txt",folderpath);
      sprintf(tester,"dir /s /b \"%s\\*.shp\" > \"%s\"",folderpath,shapelist);
      system(tester);
    }
  else
    {
      sprintf(shapelist,"%s/shapelist.txt",folderpath);
      sprintf(tester, "find \"%s\" -name \"*.[sS][hH][pP]\" -print  > \"%s\"",folderpath,shapelist);
      system(tester);
    }


  infile = fopen(shapelist,"r");

  if(infile!=NULL)
  {
    fgets(tester,1000,infile);
    while(!feof(infile))
	{
	  len = strlen(tester);
	  if(tester[len-1]=='\n')
	  {
        tester[len-1] = '\0';
	  }

	  foundit = 0;
      for(i=0;i<alldatafiles;i++)
	  { 
        if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
		{  
          sprintf(tester2,"%s\\%s.shp",folderpath,AllDataFiles[i]);
		}  
        else
		{ 
          sprintf(tester2,"%s/%s.shp",folderpath,AllDataFiles[i]);
		}

		if(!strcmp(tester,tester2))
		{
          foundit = 1;
		  break;
		}
	  }

	  if(foundit==0)
	  {
         fprintf(outerrfile,"Error:  Found shapefile not referenced in XML:\n   %s\n\n",tester);
	  }

      fgets(tester,1000,infile);
    }
    fclose(infile);
  }
  else
  {
    printf("Error: could not open %s\n",shapelist);
  }

}



int PostProcessFeatures(char *folderpath)
{
  long int orig_posn,end_posn;
  int i,j,k,numpoint,numline,numarea,foundit;
  char thisshape[1000];

  if(!strcmp(folderpath,"!!!"))
  {
    return 0;
  }

  fprintf(outerrfile,"Shapefile errors:\n\n");

  printf("\nProcessing %d shapefiles, %d subregions...\n",alldatafiles,foundsubregionid);

  printf("  Expecting %d point features, %d line features, %d area features\n",pointtotal,linetotal,areatotal);


  





  numpoint = 0;
  numline  = 0;
  numarea  = 0;

  orig_posn = ftell(outerrfile);


  FindExtraShapefiles(folderpath);



  for(j=0;j<NumSubregionSources;j++)
  {
	SubregionSources[j].MatchedOldest = 0;
	SubregionSources[j].MatchedNewest = 0;
  }




  for(i=0;i<alldatafiles;i++)
  {
	if(AllDataFilesPresent[i]==0)
	{
      printf("...NOT processing %s\n",AllDataFiles[i]);
	  continue;
	}

  

    if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
	{ 
      sprintf(thisshape,"%s\\%s",folderpath,AllDataFiles[i]);
	}
	else
	{
      sprintf(thisshape,"%s/%s",folderpath,AllDataFiles[i]);
	}

	



    

    for(j=0;j<NumSubregionPolys;j++)
	{
	  SubregionPolys[j].FoundFeatureType = 0;
      SubregionPolys[j].foundx = -999999;
      SubregionPolys[j].foundy = -999999;
	}

    if(ProcessXMLShapes(outerrfile,thisshape,&numpoint,&numline,&numarea,i+1,AllDataFiles[i],outPOIfile,outerrfile,CCPYRT_SAVE,version)==0)
	{
       fprintf(outerrfile,"Error:  failed to process shapefile %s\n\n",thisshape);
	   
	}




    for(j=0;j<NumSubregionPolys;j++)
	{
	  if(SubregionPolys[j].FoundFeatureType==0)
	  {
		 
         for(k=0;k<SubregionPolys[j].num_ftype;k++)
		 {
		   if(!strcmp(SubregionPolys[j].ftypes[k],AllDataFiles[i]))
		   {
             fprintf(outerrfile,"Error:  Did not find expected feature type %s in subregion %d\n\n",AllDataFiles[i],SubregionPolys[j].subregionid);
             printf("Did not find expected feature type %s in subregion %d\n",AllDataFiles[i],SubregionPolys[j].subregionid);
		   }
		 }
	  }
	  else
	  {
		 
         foundit = 0;
         for(k=0;k<SubregionPolys[j].num_ftype;k++)
		 {
		   if(!strcmp(SubregionPolys[j].ftypes[k],AllDataFiles[i]))
		   {
             foundit = 1;
		   }
		 }
		 if(foundit==0)
		 {
           fprintf(outerrfile,"Found unexpected feature type %s in subregion %d\n",AllDataFiles[i],SubregionPolys[j].subregionid);
           printf("Found unexpected feature type %s in subregion %d\n",AllDataFiles[i],SubregionPolys[j].subregionid);
           if((SubregionPolys[j].foundx!=-999999)&&(SubregionPolys[j].foundy!=-999999))
		   {
             fprintf(outerrfile,"  Location: (%lf %lf)\n\n",SubregionPolys[j].foundx,SubregionPolys[j].foundy);


			 POIINDEX = POIINDEX + 1;
			 if(POIINDEX==15)
			 {
               POIINDEX = 1;
			 }

             fprintf(outPOIfile,"%d %.10lf %.10lf Subregion %d With Illegal Feature %s\n",
				 POIINDEX,
				 SubregionPolys[j].foundx,SubregionPolys[j].foundy,SubregionPolys[j].subregionid,AllDataFiles[i]);
		   }
		 }
	  }
	}
  }






  for(j=0;j<NumSubregionSources;j++)
  {

     

	 break;



     if(ValidSubregionSource(j))
	 {
 	   if(
           (SubregionSources[j].MatchedOldest==0)&&
		   (SubregionSources[j].type==1) 
	     )
	   {
         fprintf(outerrfile,"Error: subregion %d:  Did not find any feature with source type %s\n", 
		     SubregionSources[j].subregionid,GetSourceType(SubregionSources[j].index));
         fprintf(outerrfile,"   matching the subregion's oldest date %s\n\n",SubregionSources[j].savedate);
	   } 

 	   if(
		   (SubregionSources[j].MatchedNewest==0)&&
		   (SubregionSources[j].type==2) 
	     )
	   { 
         fprintf(outerrfile,"Error: subregion %d:  Did not find any feature with source type %s\n", 
		     SubregionSources[j].subregionid,GetSourceType(SubregionSources[j].index));
         fprintf(outerrfile,"   matching the subregion's newest date %s\n\n",SubregionSources[j].savedate);
	   } 
	 } 
  }


  if(numpoint!=pointtotal)
  {
     fprintf(outerrfile,"Error:  Expected %d point features from shapefiles (CPONTC), but read %d total\n\n",
       pointtotal,numpoint);
  }
  if(numline!=linetotal)
  {
     fprintf(outerrfile,"Error:  Expected %d line features from shapefiles (CLINEC), but read %d total\n\n",
       linetotal,numline);
  }
  if(numarea!=areatotal)
  {
     fprintf(outerrfile,"Error:  Expected %d area features from shapefiles (CAREAC), but read %d total\n\n",
       areatotal,numarea);
  }

  printf("counted %d point features %d line features %d area features\n",numpoint,numline,numarea);
  printf("  Expected %d point features, %d line features, %d area features\n",pointtotal,linetotal,areatotal);


  end_posn = ftell(outerrfile);

  if(orig_posn==end_posn)
  {
    fprintf(outerrfile," No Shapefile errors found.\n\n");
	return 0;
  }
  else
  {
	return 1;
  }

}


char *BuildLongName(char *inname)
{
  int index;
  static char retval[200];

  index = OnFCODEList(inname,1);

  if(index>=0)
  {
    sprintf(retval,"%s %s Feature",thearray[index+1],GetPAL(thearray[index][0]));
  }
  else
  {
    printf("Error in BLN: Could not find %s\n",inname);
    return inname;
  }

  return retval;
}



void TestSubregionFilesOnList()
{
  int i,j,k,foundit;

  

  for(i=0;i<NumSubregionPolys;i++)
  {
	if(VERBOSE==1)
	{
	  printf("Subregion %d, poly %d, numverts %d:\n",
		SubregionPolys[i].subregionid,SubregionPolys[i].PolyID,SubregionPolys[i].numverts);

	  for(j=0;j<SubregionPolys[i].numverts;j++)
	  { 
        printf("  %lf %lf\n",SubregionPolys[i].x[j],SubregionPolys[i].y[j]);
	  }

      for(j=0;j<SubregionPolys[i].numholes;j++)
	  { 
		printf("Hole %d for  Poly %d Numverts %d\n",SubregionPolys[i].holes[j].HoleID,SubregionPolys[i].PolyID,SubregionPolys[i].holes[j].numverts);

		for(k=0;k<SubregionPolys[i].holes[j].numverts;k++)
		{
          printf("  %lf %lf\n",SubregionPolys[i].holes[j].x[k],SubregionPolys[i].holes[j].y[k]);
		}
	  } 
	}

	for(j=0;j<SubregionPolys[i].num_ftype;j++)
	{
	  if(VERBOSE==1)
	  {
        printf("    Feature type %s\n",SubregionPolys[i].ftypes[j]);
	  }

      foundit = 0;
      for(k=0;k<alldatafiles;k++)
	  {
       if(!strcmp(AllDataFiles[k],SubregionPolys[i].ftypes[j]))
		{
          foundit = 1;
		  break;
		}
	  }


	  if(foundit==0)
	  {
        fprintf(outerrfile,"Error:  Subregion %d contains \"%s\", which is not on the cell data file list\n\n",
			SubregionPolys[i].subregionid,BuildLongName(SubregionPolys[i].ftypes[j]));
		sprintf(SubregionPolys[i].ftypes[j],"!!!");
	  }
	}
  }


  

  for(i=0;i<alldatafiles;i++)
  {
	foundit = 0;

    for(j=0;j<NumSubregionPolys;j++)
	{
	  for(k=0;k<SubregionPolys[j].num_ftype;k++)
	  {
        if(!strcmp(AllDataFiles[i],SubregionPolys[j].ftypes[k]))
		{
          foundit = 1;
		  break;
		} 
	  } 
	}
    if(foundit==0)
	{
      fprintf(outerrfile,"Error:  Cell data file \"%s\" was not found in any of the %d subregions\n\n",BuildLongName(AllDataFiles[i]),NumSubregionPolys);
	} 
  }
}


void WritePOIFile()
{
  int i,j;

  for(i=0;i<NumSubregionPolys;i++)
  {  
	POIINDEX = POIINDEX + 1;
	if(POIINDEX>=15)
	{
      POIINDEX = 1;
	}

	for(j=0;j<SubregionPolys[i].numverts;j++)
	{
	  fprintf(outPOIfile,"%d %.10lf %.10lf Subregion %d Vertex %d\n",POIINDEX,SubregionPolys[i].x[j],SubregionPolys[i].y[j],SubregionPolys[i].subregionid,j+1);
	}
  } 
}



void VerifySubregionSourceDates()
{
  int i,j,themax,adder,foundoldestmatch=0,foundnewestmatch=0;

  

  if(version==2)
  {
    thearray = SourceTypeIds2;
    themax = SourceTypeIdsMax2*3;
  }
  else if(version==3)
  {
    thearray = SourceTypeIds3;
    themax = SourceTypeIdsMax3*3;
  }
  else if(version==4)
  {
    thearray = SourceTypeIds4;
    themax = SourceTypeIdsMax4*3;
  }
  else if((version==41)||(version==42)||(version==43))
  {
    thearray = SourceTypeIds41;
    themax = SourceTypeIdsMax41*3;
  }
  else if(version==44)
  {
    thearray = SourceTypeIds44;
    themax = SourceTypeIdsMax44*3;
  }

  for(i=0;i<NumSubregionSources;i++)
  {
	if((SubregionSources[i].index>=0)&&(SubregionSources[i].index<themax))
	{

		if(version==2)
		{
	      if(
			(strcmp(thearray[SubregionSources[i].index],"cib1")) &&
			(strcmp(thearray[SubregionSources[i].index],"cib5")) &&
			(strcmp(thearray[SubregionSources[i].index],"CIB1")) &&
			(strcmp(thearray[SubregionSources[i].index],"CIB5")) &&
			(strcmp(thearray[SubregionSources[i].index],"qbrd")) &&
			(strcmp(thearray[SubregionSources[i].index],"QuickBird Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"field")) &&
			(strcmp(thearray[SubregionSources[i].index],"GPS")) &&
			(strcmp(thearray[SubregionSources[i].index],"ikonos")) &&
			(strcmp(thearray[SubregionSources[i].index],"IKONOS Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"ntm")) &&
			(strcmp(thearray[SubregionSources[i].index],"NTM Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"imag")) &&
			(strcmp(thearray[SubregionSources[i].index],"Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"ibd")) &&
			(strcmp(thearray[SubregionSources[i].index],"International Boundaries")) &&
			(strcmp(thearray[SubregionSources[i].index],"landsat")) &&
			(strcmp(thearray[SubregionSources[i].index],"Landsat Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"qbrdmm")) &&
			(strcmp(thearray[SubregionSources[i].index],"qbrdms")) &&
			(strcmp(thearray[SubregionSources[i].index],"qbrdpm")) &&
			(strcmp(thearray[SubregionSources[i].index],"qbrdps")) &&
			(strcmp(thearray[SubregionSources[i].index],"ikonmm")) &&
			(strcmp(thearray[SubregionSources[i].index],"ikonms")) &&
			(strcmp(thearray[SubregionSources[i].index],"ikonpm")) &&
			(strcmp(thearray[SubregionSources[i].index],"ikonps")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt10")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5mm")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5ms")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5pm")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5ps")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt2pm")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt2mm")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5m")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt5s")) &&
			(strcmp(thearray[SubregionSources[i].index],"spt10mm")) &&
			(strcmp(thearray[SubregionSources[i].index],"spthrg")) &&
			(strcmp(thearray[SubregionSources[i].index],"QuickBird Multispectral Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"QuickBird Multispectral Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"QuickBird Panchromatic Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"QuickBird Panchromatic Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Ikonos Multispectral Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Ikonos Multispectral Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Ikonos Panchromatic Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Ikonos Panchromatic Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 10m Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Multispectral Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Multispectral Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Panchromatic Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Panchromatic Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 2.5m Panchromatic Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 2.5m Multispectral Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 5m Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT 10m Multispectral Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"SPOT HRG Digital Terrain Model"))

		   )
		  {  
            continue;
		  } 
		} 
	  



		if(version==3)
		{
	      if(
			(strcmp(thearray[SubregionSources[i].index],"cib1")) &&
			(strcmp(thearray[SubregionSources[i].index],"cib5")) &&
			(strcmp(thearray[SubregionSources[i].index],"CIB1")) &&
			(strcmp(thearray[SubregionSources[i].index],"CIB5")) &&
			(strcmp(thearray[SubregionSources[i].index],"qbrd")) &&
			(strcmp(thearray[SubregionSources[i].index],"QuickBird Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"field")) &&
			(strcmp(thearray[SubregionSources[i].index],"GPS")) &&
			(strcmp(thearray[SubregionSources[i].index],"ikonos")) &&
			(strcmp(thearray[SubregionSources[i].index],"IKONOS Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"ntm")) &&
			(strcmp(thearray[SubregionSources[i].index],"NTM Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"imag")) &&
			(strcmp(thearray[SubregionSources[i].index],"Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"ibd")) &&
			(strcmp(thearray[SubregionSources[i].index],"International Boundaries")) &&
			(strcmp(thearray[SubregionSources[i].index],"VeryHighResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"VeryHighResCommStereoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"HighResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"HighResCommStereoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"MediumResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"MediumResCommStereoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"LowResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"LowResCommStereoImage")) &&

			(strcmp(thearray[SubregionSources[i].index],"Very High Resolution Commerical Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Very High Resolution Commerical Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"High Resolution Commerical Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"High Resolution Commerical Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Medium Resolution Commerical Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Medium Resolution Commerical Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Low Resolution Commerical Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Low Resolution Commerical Stereoscopic Imagery")) &&

			(strcmp(thearray[SubregionSources[i].index],"Very High Resolution Commercial Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Very High Resolution Commercial Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"High Resolution Commercial Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"High Resolution Commercial Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Medium Resolution Commercial Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Medium Resolution Commercial Stereoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Low Resolution Commercial Monoscopic Imagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"Low Resolution Commercial Stereoscopic Imagery")) 


		   )
		  {
            continue;
		  }
		}


        

		if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
		{
	      if(
			(strcmp(thearray[SubregionSources[i].index],"ngaControlledImageBase1")) &&
			(strcmp(thearray[SubregionSources[i].index],"ngaControlledImageBase5")) &&
			(strcmp(thearray[SubregionSources[i].index],"gpsBasedFieldCollect")) &&
			(strcmp(thearray[SubregionSources[i].index],"usNtmImagery")) &&
			(strcmp(thearray[SubregionSources[i].index],"imageryUnspecified")) &&
			(strcmp(thearray[SubregionSources[i].index],"ngaInternationalBoundary")) &&
			(strcmp(thearray[SubregionSources[i].index],"veryHighResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"veryHighResCommStereoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"highResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"highResCommStereoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"mediumResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"mediumResCommStereoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"lowResCommMonoImage")) &&
			(strcmp(thearray[SubregionSources[i].index],"lowResCommStereoImage"))
		   )
		  {
            continue;
		  }
		}



		


      if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	  {
        adder = 1; 
	  }
	  else
	  {
        adder = 0;
	  }

	  if(FoundOldest==1)
	  {
	    if(
		   (SubregionSources[i].year ==OldestYear)   &&
           (SubregionSources[i].month==OldestMonth)  &&
           (SubregionSources[i].day  ==OldestDay)
		  )
		{ 
          foundoldestmatch = 1;
		} 
	  }

	  if(FoundNewest==1)
	  {
	    if(
		    (SubregionSources[i].year ==NewestYear)   &&
            (SubregionSources[i].month==NewestMonth)  &&
            (SubregionSources[i].day  ==NewestDay)
		  )
		{ 
          foundnewestmatch = 1;
		} 
	  }

	  if(FoundOldest==1)
	  {
        if(SubregionSources[i].year<OldestYear)
		{ 
          fprintf(outerrfile,"Error:  Found SSRCDT date \"%s\" for subregion %d and source type \"%s\" earlier than Date of oldest source (COLDSD) \"%s\"\n\n",
            SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SAVEOLDEST);
		} 
        else if(SubregionSources[i].year==OldestYear)
		{ 
          if(SubregionSources[i].month<OldestMonth)
		  { 
            fprintf(outerrfile,"Error:  Found SSRCDT date \"%s\" for subregion %d and source type \"%s\" earlier than Date of oldest source (COLDSD) \"%s\"\n\n",
              SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SAVEOLDEST);
		  } 
          else if(SubregionSources[i].month==OldestMonth)
		  {  
            if(SubregionSources[i].day<OldestDay)
			{  
              fprintf(outerrfile,"Error:  Found SSRCDT date \"%s\" for subregion %d and source type \"%s\" earlier than Date of oldest source (COLDSD) \"%s\"\n\n",
                SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SAVEOLDEST);
			} 
		  } 
		} 
	  }

	  if(FoundNewest==1)
	  {
        if(SubregionSources[i].year>NewestYear)
		{ 
          fprintf(outerrfile,"Error:  Found SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than Date of newest source (CNEWSD) \"%s\"\n\n",
            SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SAVENEWEST);
		} 
        else if(SubregionSources[i].year==NewestYear)
		{ 
          if(SubregionSources[i].month>NewestMonth)
		  { 
            fprintf(outerrfile,"Error:  Found SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than Date of newest source (CNEWSD) \"%s\"\n\n",
              SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SAVENEWEST);
		  } 
          else if(SubregionSources[i].month==NewestMonth)
		  {  
            if(SubregionSources[i].day>NewestDay)
			{  
              fprintf(outerrfile,"Error:  Found SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than Date of newest source (CNEWSD) \"%s\"\n\n",
                SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SAVENEWEST);
			} 
		  } 
		} 
	  } 

	  


	  


      for(j=i+1;j<NumSubregionSources;j++)
	  {
	    if((SubregionSources[j].index>=0)&&(SubregionSources[j].index<themax))
		{
          if(SubregionSources[i].subregionid==SubregionSources[j].subregionid)
		  {
            if(SubregionSources[i].index==SubregionSources[j].index)
			{ 

              if((SubregionSources[i].type==1)&&(SubregionSources[j].type==2))
			  {
                if(SubregionSources[i].year>SubregionSources[j].year)
				{ 
                  fprintf(outerrfile,"Error:  Found \"Oldest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than \"Newest\" SSRCDT \"%s\" for subregion %d and source type \"%s\"\n\n",
                    SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SubregionSources[j].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder]);
				}   
                else if(SubregionSources[i].year==SubregionSources[j].year)
				{   
                  if(SubregionSources[i].month>SubregionSources[j].month)
				  {   
                    fprintf(outerrfile,"Error:  Found \"Oldest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than \"Newest\" SSRCDT \"%s\" for subregion %d and source type \"%s\"\n\n",
                      SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SubregionSources[j].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder]);
				  }
                  else if(SubregionSources[i].month==SubregionSources[j].month)
				  {    
                    if(SubregionSources[i].day>SubregionSources[j].day)
					{     
                      fprintf(outerrfile,"Error:  Found \"Oldest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than \"Newest\" SSRCDT \"%s\" for subregion %d and source type \"%s\"\n\n",
                        SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SubregionSources[j].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder]);
					}
				  }
				}
			  }

             else if((SubregionSources[i].type==2)&&(SubregionSources[j].type==1))
			  {
                if(SubregionSources[j].year>SubregionSources[i].year)
				{ 
                  fprintf(outerrfile,"Error:  Found \"Oldest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than \"Newest\" SSRCDT \"%s\" for subregion %d and source type \"%s\"\n\n",
                    SubregionSources[j].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder]);
				}   
                else if(SubregionSources[j].year==SubregionSources[i].year)
				{   
                  if(SubregionSources[j].month>SubregionSources[i].month)
				  {   
                    fprintf(outerrfile,"Error:  Found \"Oldest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than \"Newest\" SSRCDT \"%s\" for subregion %d and source type \"%s\"\n\n",
                      SubregionSources[j].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder]);
				  }
                  else if(SubregionSources[j].month==SubregionSources[i].month)
				  {    
                    if(SubregionSources[j].day>SubregionSources[i].day)
					{     
                      fprintf(outerrfile,"Error:  Found \"Oldest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\" later than \"Newest\" SSRCDT \"%s\" for subregion %d and source type \"%s\"\n\n",
                        SubregionSources[j].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder],SubregionSources[i].savedate,SubregionSources[i].subregionid,thearray[SubregionSources[i].index+adder]);
					}
				  }
				}
			  }
			}
		  }
		}
	  }
	}
	else
	{
      printf("Bad SRI %d\n",SubregionSources[i].index);
	}
  }



  if(FoundOldest==1)
  {
    if(foundoldestmatch==0)
	{ 
      fprintf(outerrfile,"Error:  Did not find any primary source with a date matching the cell oldest date COLDSD: %s\n\n",SAVEOLDEST);
	} 
  }

  if(FoundNewest==1)
  {
    if(foundnewestmatch==0)
	{ 
      fprintf(outerrfile,"Error:  Did not find any primary source with a date matching the cell newest date CNEWSD: %s\n\n",SAVENEWEST);
	} 
  }


  if(version>=4)
  {
    


    for(i=0;i<NumSubregionSources;i++)
	{ 
	  if((SubregionSources[i].index>=0)&&(SubregionSources[i].index<themax))
	  {
        for(j=i+1;j<NumSubregionSources;j++)
		{ 
	      if((SubregionSources[j].index>=0)&&(SubregionSources[j].index<themax))
		  { 

            if(SubregionSources[i].subregionid==SubregionSources[j].subregionid)
			{
              if(SubregionSources[i].index==SubregionSources[j].index)
			  {  

                if(
					((SubregionSources[i].type==1)&&(SubregionSources[j].type==2)) ||  
					((SubregionSources[i].type==2)&&(SubregionSources[j].type==1))     
					)
				{
                  if(
					  (SubregionSources[i].year ==SubregionSources[j].year)  &&
					  (SubregionSources[i].month==SubregionSources[j].month) &&
					  (SubregionSources[i].day  ==SubregionSources[j].day)
					)
				  {  
                      fprintf(outerrfile,"Error:  Found the same \"Oldest\" and \"Newest\" SSRCDT date \"%s\" for subregion %d and source type \"%s\"\n\n",
                        SubregionSources[i].savedate,
						SubregionSources[i].subregionid,
						thearray[SubregionSources[i].index+adder]);
				  }
				}
			  }
			}
		  }
		}
	  }
	}
  }
}  


void TestHolesInside()
{
  int i,j,k;

  for(i=0;i<NumSubregionPolys;i++)
  { 
    

    for(j=0;j<SubregionPolys[i].numholes;j++)
	{ 
      for(k=0;k<SubregionPolys[i].holes[j].numverts;k++)
	  { 
        if(SH_PointInAreal(SubregionPolys[i].holes[j].x[k],SubregionPolys[i].holes[j].y[k],
           SubregionPolys[i].x,SubregionPolys[i].y,0,SubregionPolys[i].numverts-1)!=1)
		{
          printf("Error:  Interior ring %d of exterior ring for subregion %d has vertex %lf %lf which is not inside the exterior ring\n",
            SubregionPolys[i].holes[j].HoleID,SubregionPolys[i].subregionid,SubregionPolys[i].holes[j].x[k],SubregionPolys[i].holes[j].y[k]);

          fprintf(outerrfile,"Error:  Interior ring %d of exterior ring for subregion %d has vertex %lf %lf\n  which is not inside the exterior ring...ignoring this interior ring\n\n",
            SubregionPolys[i].holes[j].HoleID,SubregionPolys[i].subregionid,SubregionPolys[i].holes[j].x[k],SubregionPolys[i].holes[j].y[k]);

            SubregionPolys[i].holes[j].usethis = 0;

			
		}
	  }
	}
  }
}


void SetUpLineNumbers(char *filename)
{
  FILE *thefile;
  char *aline;
  int line=0;

  aline = (char *)malloc(1000000);

  DoLineNumbers = 1;

  thefile = fopen(filename,"r");
  if(thefile==NULL)
  {
    printf("error: could not open %s for read\n",filename);
    DoLineNumbers = 0;
	free(aline);
	fclose(thefile);
    return;
  }

  fgets(aline,1000000,thefile);
  line = 1;

  if(strlen(aline)>990) 
  {
    DoLineNumbers = 0;
	free(aline);
	fclose(thefile);
    return;
  }


  while(!feof(thefile))
  {
    line = line + 1;
    fgets(aline,1000000,thefile);

	if((line<20)&&(strlen(aline)>990))
	{
      DoLineNumbers = 0;
      free(aline);
	  fclose(thefile);
      return;
	}

	if(strlen(aline)>999990)
	{ 
      printf("unable to report line numbers due to line %d having more than 1 million characters\n",line);
      DoLineNumbers = 0;
      free(aline);
	  fclose(thefile);
      return;
	} 

	if(strlen(aline)>990)
	{
       printf("big line %d\n",(int)strlen(aline));
	}
  }


  if(line<20)
  {
    printf("Not recording line numbers because strange number of lines: %d\n",line);
    DoLineNumbers = 0;
	free(aline);
	fclose(thefile);
    return;
  }


  if(FilePosns!=NULL)
  {
    free(FilePosns);
    FilePosns = NULL;
  }
  FilePosns = (long int *)malloc(sizeof(long int)*(line+10));
  rewind(thefile);


  line = 0;
  fgets(aline,1000000,thefile);
  FilePosns[line] = ftell(thefile);
  while(!feof(thefile))
  {
    line = line + 1;
    FilePosns[line] = ftell(thefile);
    fgets(aline,1000000,thefile);
  }

  free(aline);

  fclose(thefile);

  totallines = line;

}


char *GetCleanToken(char *intoken)
{
  int i,j,len;
  static char retval[200];

  len = strlen(intoken);

  for(i=len-1;i>=0;i--)
  {
    if(intoken[i]==':')
	{
      break;
	}
  }

  if(i<0)
  {
    sprintf(retval,"%s",intoken);
  }
  else
  {
    for(j=i+1;j<len;j++)
	{
      retval[j-(i+1)] = intoken[j];
      retval[j-(i+1)+1] = '\0';
	}
  }

  return retval;
}


void TestExtent()
{
  int bad=0;


  if(foundCellWest==0)
  {
    fprintf(outerrfile,"Error:  Did not find CEXTNT for westBoundLongitude\n\n");
    bad = 1;
  }
  if(foundCellEast==0)
  {
    fprintf(outerrfile,"Error:  Did not find CEXTNT for eastBoundLongitude\n\n");
    bad = 1;
  }
  if(foundCellSouth==0)
  {
    fprintf(outerrfile,"Error:  Did not find CEXTNT for southBoundLatitude\n\n");
    bad = 1;
  }
  if(foundCellNorth==0)
  {
    fprintf(outerrfile,"Error:  Did not find CEXTNT for northBoundLatitude\n\n");
    bad = 1;
  }

  if(bad==1)
  {
    return;
  }

  if((cellWest)!=((int)cellWest))
  {
    fprintf(outerrfile,"Error:  Line %s: Found non-integer CEXTNT (%s) for westBoundLongitude\n\n",cellWestLine,saveCellWest);
    bad = 1;
  }
  if((cellEast)!=((int)cellEast))
  {
    fprintf(outerrfile,"Error:  Line %s: Found non-integer CEXTNT (%s) for eastBoundLongitude\n\n",cellEastLine,saveCellEast);
    bad = 1;
  }
  if((cellSouth)!=((int)cellSouth))
  {
    fprintf(outerrfile,"Error:  Line %s: Found non-integer CEXTNT (%s) for southBoundLatitude\n\n",cellSouthLine,saveCellSouth);
    bad = 1;
  }
  if((cellNorth)!=((int)cellNorth))
  {
    fprintf(outerrfile,"Error:  Line %s: Found non-integer CEXTNT (%s) for northBoundLatitude\n\n",cellNorthLine,saveCellNorth);
    bad = 1;
  }

  if(bad==1)
  {
    return;
  }

  if((cellWest+1.0)!=(cellEast))
  {
    fprintf(outerrfile,"Error:  Found east CEXTNT %lf (Line %s) not one degree more than west CEXTNT %lf (Line %s)\n\n",cellEast,cellEastLine,cellWest,cellWestLine);
    bad = 1;
  }
  if((cellSouth+1.0)!=(cellNorth))
  {
    fprintf(outerrfile,"Error:  Found north CEXTNT %lf (Line %s) not one degree more than south CEXTNT %lf (Line %s)\n\n",cellNorth,cellNorthLine,cellSouth,cellSouthLine);
    bad = 1;
  }

  if(bad==1)
  {
    return;
  }

  if((cellWest<-180)||(cellWest>179))
  {
    fprintf(outerrfile,"Error:  Line %s: Found invalid CEXTNT (%lf) for westBoundLongitude (expected value -180 to 179)\n\n",cellWestLine,cellWest);
  }
  if((cellEast<-179)||(cellEast>180))
  {
    fprintf(outerrfile,"Error:  Line %s: Found invalid CEXTNT (%lf) for eastBoundLongitude (expected value -179 to 180)\n\n",cellEastLine,cellEast);
  }
  if((cellSouth<-90)||(cellSouth>89))
  {
    fprintf(outerrfile,"Error:  Line %s: Found invalid CEXTNT (%lf) for southBoundLatitude (expected value -90 to 89)\n\n",cellSouthLine,cellSouth);
  }
  if((cellNorth<-89)||(cellNorth>90))
  {
    fprintf(outerrfile,"Error:  Line %s: Found invalid CEXTNT (%lf) for northBoundLatitude (expected value -89 to 90)\n\n",cellNorthLine,cellNorth);
  }
}


extern "C" int IsOKVal(char *inval)
{
  int i,len,foundnewline=0;

  len = strlen(inval);

  if(len<1)
  {
	printf("0 length string!\n");
    return 0;
  }
  else
  {
    for(i=0;i<len;i++)
	{
      if(inval[i]>32)
	  {
        return 1;
	  }
	  if((inval[i]==10)||(inval[i]==13))
	  {
        foundnewline = 1;
	  }
	}
  }

  if(foundnewline==1)
  {
    return 0;
  }

  return 1;

}




void ZeroRefine(int onoff)
{
  int i,MaxNum,thiscode;
 
  for(i=0;i<5;i++)
  {

    if((i==4)&&((version!=4)&&(version!=41)&&(version!=42)&&(version!=43)&&(version!=44)))
	{
      
      continue;
	}

    if((version==2)&&(i==0))
	{   
      thearray = ParticipantAgencyCodes2;
      MaxNum = ParticipantAgencyCodesMax2;
	}
    else if((version==2)&&(i==1))
	{  
      thearray = ParticipantNationCodes2;
      MaxNum = ParticipantNationCodesMax2;
	}
    else if((version==2)&&(i==2))
	{  
      thearray = AccuracyEvaluationMethods23;
      MaxNum = AccuracyEvaluationMethodsMax23;
	}
    else if((version==2)&&(i==3))
	{  
      thearray = SourceTypeIds2;
      MaxNum = SourceTypeIdsMax2;
	}


    else if((version==3)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes3;
      MaxNum = ParticipantAgencyCodesMax3;
	}
    else if((version==3)&&(i==1))
	{
      thearray = ParticipantNationCodes3;
      MaxNum = ParticipantNationCodesMax3;
	}
    else if((version==3)&&(i==2))
	{  
      thearray = AccuracyEvaluationMethods23;
      MaxNum = AccuracyEvaluationMethodsMax23;
	}
    else if((version==3)&&(i==3))
	{  
      thearray = SourceTypeIds3;
      MaxNum = SourceTypeIdsMax3;
	}


    else if((version==4)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes4;
      MaxNum = ParticipantAgencyCodesMax4;
	}
    else if((version==41)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes41;
      MaxNum = ParticipantAgencyCodesMax41;
	}
    else if((version==42)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes42;
      MaxNum = ParticipantAgencyCodesMax42;
	}
    else if((version==43)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes43;
      MaxNum = ParticipantAgencyCodesMax43;
	}
    else if((version==44)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes44;
      MaxNum = ParticipantAgencyCodesMax44;
	}


    else if((version==4)&&(i==1))
	{
      thearray = ParticipantNationCodes4;
      MaxNum = ParticipantNationCodesMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(i==1))
	{
      thearray = ParticipantNationCodes41;
      MaxNum = ParticipantNationCodesMax41;
	}
    else if((version==4)&&(i==2))
	{  
      thearray = AccuracyEvaluationMethods4;
      MaxNum = AccuracyEvaluationMethodsMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(i==2))
	{
      thearray = AccuracyEvaluationMethods41;
      MaxNum = AccuracyEvaluationMethodsMax41;
	}
    else if((version==4)&&(i==3))
	{  
      thearray = SourceTypeIds4;
      MaxNum = SourceTypeIdsMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43))  &&(i==3))
	{
      thearray = SourceTypeIds41;
      MaxNum = SourceTypeIdsMax41;
	}
    else if((version==44)&&(i==3))
	{
      thearray = SourceTypeIds44;
      MaxNum = SourceTypeIdsMax44;
	}
    else if((version==4)&&(i==4))
	{  
      thearray = VerticalSourceTypeIds4;
      MaxNum = VerticalSourceTypeIdsMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(i==4))
	{  
      thearray = VerticalSourceTypeIds41;
      MaxNum = VerticalSourceTypeIdsMax41;
	}

    for(thiscode=0;thiscode<MaxNum*3;thiscode=thiscode+3)
	{
      if(onoff==1)
	  {
        thearray[thiscode+2] = onepointer;
	  }
	  else
	  {
        thearray[thiscode+2] = zeropointer;
	  }
	}
  }
}





void PrintRefine()
{
  int i,MaxNum,thiscode;
 
  for(i=0;i<5;i++)
  {

    if((i==4)&&((version!=4)&&(version!=41)&&(version!=42)&&(version!=43)&&(version!=44)))
	{
      
      continue;
	}

    if((version==2)&&(i==0))
	{   
      thearray = ParticipantAgencyCodes2;
      MaxNum = ParticipantAgencyCodesMax2;
	}
    else if((version==2)&&(i==1))
	{  
      thearray = ParticipantNationCodes2;
      MaxNum = ParticipantNationCodesMax2;
	}
    else if((version==2)&&(i==2))
	{  
      thearray = AccuracyEvaluationMethods23;
      MaxNum = AccuracyEvaluationMethodsMax23;
	}
    else if((version==2)&&(i==3))
	{  
      thearray = SourceTypeIds2;
      MaxNum = SourceTypeIdsMax2;
	}
    else if((version==3)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes3;
      MaxNum = ParticipantAgencyCodesMax3;
	}
    else if((version==3)&&(i==1))
	{
      thearray = ParticipantNationCodes3;
      MaxNum = ParticipantNationCodesMax3;
	}
    else if((version==3)&&(i==2))
	{  
      thearray = AccuracyEvaluationMethods23;
      MaxNum = AccuracyEvaluationMethodsMax23;
	}
    else if((version==3)&&(i==3))
	{  
      thearray = SourceTypeIds3;
      MaxNum = SourceTypeIdsMax3;
	}

    else if((version==4)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes4;
      MaxNum = ParticipantAgencyCodesMax4;
	}
    else if((version==41)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes41;
      MaxNum = ParticipantAgencyCodesMax41;
	}
    else if((version==42)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes42;
      MaxNum = ParticipantAgencyCodesMax42;
	}
    else if((version==43)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes43;
      MaxNum = ParticipantAgencyCodesMax43;
	}
    else if((version==44)&&(i==0))
	{  
      thearray = ParticipantAgencyCodes44;
      MaxNum = ParticipantAgencyCodesMax44;
	}


    else if((version==4)&&(i==1))
	{
      thearray = ParticipantNationCodes4;
      MaxNum = ParticipantNationCodesMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(i==1))
	{
      thearray = ParticipantNationCodes41;
      MaxNum = ParticipantNationCodesMax41;
	}
    else if((version==4)&&(i==2))
	{
      thearray = AccuracyEvaluationMethods4;
      MaxNum = AccuracyEvaluationMethodsMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(i==2))
	{
      thearray = AccuracyEvaluationMethods41;
      MaxNum = AccuracyEvaluationMethodsMax41;
	}
    else if((version==4)&&(i==3))
	{
      thearray = SourceTypeIds4;
      MaxNum = SourceTypeIdsMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43))  &&(i==3))
	{
      thearray = SourceTypeIds41;
      MaxNum = SourceTypeIdsMax41;
	}
    else if((version==44)&&(i==3))
	{
      thearray = SourceTypeIds44;
      MaxNum = SourceTypeIdsMax44;
	}
    else if((version==4)&&(i==4))
	{  
      thearray = VerticalSourceTypeIds4;
      MaxNum = VerticalSourceTypeIdsMax4;
	}
    else if(  ((version==41)||(version==42)||(version==43)||(version==44))  &&(i==4))
	{  
      thearray = VerticalSourceTypeIds41;
      MaxNum = VerticalSourceTypeIdsMax41;
	}


    for(thiscode=0;thiscode<MaxNum*3;thiscode=thiscode+3)
	{
if(!strcmp(thearray[thiscode+2],"0"))
{
      printf("%s %s is %s\n",thearray[thiscode],thearray[thiscode+1],thearray[thiscode+2]);
}
	}
  }



}




int SetRefine(char *refinefilename, char *theline, int line)
{
  int foundmatch,thiscode,MaxNum,case_sensitive=1;


  if(version==2)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD2"))
	{  
      thearray = ParticipantAgencyCodes2;
      MaxNum = ParticipantAgencyCodesMax2;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD2"))
	{  
      thearray = ParticipantNationCodes2;
      MaxNum = ParticipantNationCodesMax2;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD2"))
	{  
      thearray = AccuracyEvaluationMethods23;
      MaxNum = AccuracyEvaluationMethodsMax23;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD2"))
	{  
      thearray = SourceTypeIds2;
      MaxNum = SourceTypeIdsMax2;
	}
	else
	{
	  
      return 1;
	}
  }
  else if(version==3)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD3"))
	{  
      thearray = ParticipantAgencyCodes3;
      MaxNum = ParticipantAgencyCodesMax3;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD3"))
	{  
      thearray = ParticipantNationCodes3;
      MaxNum = ParticipantNationCodesMax3;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD3"))
	{  
      thearray = AccuracyEvaluationMethods23;
      MaxNum = AccuracyEvaluationMethodsMax23;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD3"))
	{  
      thearray = SourceTypeIds3;
      MaxNum = SourceTypeIdsMax3;
	}
	else
	{
	  
      return 1;
	}
  }
 

  else if(version==4)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD4"))
	{  
      thearray = ParticipantAgencyCodes4;
      MaxNum = ParticipantAgencyCodesMax4;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD4"))
	{  
      thearray = ParticipantNationCodes4;
      MaxNum = ParticipantNationCodesMax4;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD4"))
	{  
      thearray = AccuracyEvaluationMethods4;
      MaxNum = AccuracyEvaluationMethodsMax4;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD4"))
	{  
      thearray = SourceTypeIds4;
      MaxNum = SourceTypeIdsMax4;
	}
    else if(!strcmp(Ofirst,"VerticalSourceTypeId_TRD4"))
	{  
      thearray = VerticalSourceTypeIds4;
      MaxNum = VerticalSourceTypeIdsMax4;
	}
	else
	{
	  
      return 1;
	}
  }


  else if(version==41)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD41"))
	{  
      thearray = ParticipantAgencyCodes41;
      MaxNum = ParticipantAgencyCodesMax41;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD41"))
	{  
      thearray = ParticipantNationCodes41;
      MaxNum = ParticipantNationCodesMax41;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD41"))
	{  
      thearray = AccuracyEvaluationMethods41;
      MaxNum = AccuracyEvaluationMethodsMax41;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD41"))
	{  
      thearray = SourceTypeIds41;
      MaxNum = SourceTypeIdsMax41;
	}
    else if(!strcmp(Ofirst,"VerticalSourceTypeId_TRD41"))
	{  
      thearray = VerticalSourceTypeIds41;
      MaxNum = VerticalSourceTypeIdsMax41;
	}
	else
	{
	  
      return 1;
	}
  }
  else if(version==42)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD42"))
	{  
      thearray = ParticipantAgencyCodes42;
      MaxNum = ParticipantAgencyCodesMax42;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD42"))
	{  
      thearray = ParticipantNationCodes41;
      MaxNum = ParticipantNationCodesMax41;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD42"))
	{  
      thearray = AccuracyEvaluationMethods41;
      MaxNum = AccuracyEvaluationMethodsMax41;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD42"))
	{  
      thearray = SourceTypeIds41;
      MaxNum = SourceTypeIdsMax41;
	}
    else if(!strcmp(Ofirst,"VerticalSourceTypeId_TRD42"))
	{  
      thearray = VerticalSourceTypeIds41;
      MaxNum = VerticalSourceTypeIdsMax41;
	}
	else
	{
	  
      return 1;
	}
  }
  else if(version==43)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD43"))
	{  
      thearray = ParticipantAgencyCodes43;
      MaxNum = ParticipantAgencyCodesMax43;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD43"))
	{  
      thearray = ParticipantNationCodes41;
      MaxNum = ParticipantNationCodesMax41;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD43"))
	{  
      thearray = AccuracyEvaluationMethods41;
      MaxNum = AccuracyEvaluationMethodsMax41;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD43"))
	{  
      thearray = SourceTypeIds41;
      MaxNum = SourceTypeIdsMax41;
	}
    else if(!strcmp(Ofirst,"VerticalSourceTypeId_TRD43"))
	{  
      thearray = VerticalSourceTypeIds41;
      MaxNum = VerticalSourceTypeIdsMax41;
	}
	else
	{
	  
      return 1;
	}
  }
  else if(version==44)
  {
    if(!strcmp(Ofirst,"ParticipantAgencyCode_TRD44"))
	{  
      thearray = ParticipantAgencyCodes44;
      MaxNum = ParticipantAgencyCodesMax44;
      case_sensitive  = 0;
	}
    else if(!strcmp(Ofirst,"ParticipantNationCode_TRD44"))
	{  
      thearray = ParticipantNationCodes41;
      MaxNum = ParticipantNationCodesMax41;
	}
    else if(!strcmp(Ofirst,"AccuracyEvaluationMethod_TRD44"))
	{  
      thearray = AccuracyEvaluationMethods41;
      MaxNum = AccuracyEvaluationMethodsMax41;
	}
    else if(!strcmp(Ofirst,"SourceTypeId_TRD44"))
	{  
      thearray = SourceTypeIds44;
      MaxNum = SourceTypeIdsMax44;
	}
    else if(!strcmp(Ofirst,"VerticalSourceTypeId_TRD44"))
	{  
      thearray = VerticalSourceTypeIds41;
      MaxNum = VerticalSourceTypeIdsMax41;
	}
	else
	{
	  
      return 1;
	}
  }





  foundmatch = 0;
  for(thiscode=0;thiscode<MaxNum*3;thiscode=thiscode+3)
	{
      if(StringMatch(Osecond,thearray[thiscode],case_sensitive)==1)
	  { 
        if(!strcmp(Othird,thearray[thiscode+1]))
		{

          foundmatch = 1;
		  thearray[thiscode+2] = onepointer;
		  break;
		}
	  }
  }


  if(foundmatch==0)
  {
    sprintf(GErr,"Error in file %s\n line: %d:  %s Invalid entry\n",refinefilename,line,theline);
	return 0;
  }

  return 1;
}





int ProcessStringLine(char *theline,char *refinefilename,int type,int line)
{
  int i,len,start=-1,end;
  char buildstr[1000];

  

  len = strlen(theline);

  for(i=0;i<len;i++)
  {
    if(theline[i]=='"')
	{
      if(start<0)
	  {
        start = i;
	  }
	  end = i;
	}
  }

  if((start==end)||(start<0))
  {
    sprintf(GErr,"Error in file %s\n line: %d:  %s Invalid entry\nShould be of form <CLSTAT|SSCALE> \"<value>\" ",refinefilename,line,theline);
	return 0;
  }

  start = start + 1;
  for(i=start;i<end;i++)
  {
    buildstr[i-start] = theline[i];
  }
  buildstr[i-start] = '\0';


  if(!strcmp(buildstr,"NotSpecified"))
  {
    return 1;
  }


  if(type==1)
  {
    if( (strcmp(buildstr,"50000")) && (strcmp(buildstr,"100000")) )
	{
      sprintf(GErr,"Error in file %s\n line: %d:  %s\n\nInvalid entry:\n\n<SSCALE> must be 50000 or 100000, got %s ",
		  refinefilename,line,theline,buildstr);
      return 0;
	}

    use_sscale = 1;

	if(SSCALE_OVERRIDE!=NULL)
	{
      free(SSCALE_OVERRIDE);
	}
	SSCALE_OVERRIDE = (char *)malloc(strlen(buildstr)+5);

	sprintf(SSCALE_OVERRIDE,"%s",buildstr);
    printf("Got overriding SSCALE %s\n",SSCALE_OVERRIDE);
  }
  else if(type==2)
  {
    use_clstat = 1;

	if(CLSTAT_OVERRIDE!=NULL)
	{
      free(CLSTAT_OVERRIDE);
	}
	CLSTAT_OVERRIDE = (char *)malloc(strlen(buildstr)+5);

	sprintf(CLSTAT_OVERRIDE,"%s",buildstr);
    printf("Got overriding CLSTAT %s\n",CLSTAT_OVERRIDE);
  }
  else if(type==3)
  {
    use_ccpyrt = 1;

	if(CCPYRT_OVERRIDE!=NULL)
	{
      free(CCPYRT_OVERRIDE);
	}
	CCPYRT_OVERRIDE = (char *)malloc(strlen(buildstr)+5);

	sprintf(CCPYRT_OVERRIDE,"%s",buildstr);
    printf("Got overriding CCPYRT %s\n",CCPYRT_OVERRIDE);
  }
  else if(type==4)
  {
    use_scpyrt = 1;

	if(SCPYRT_OVERRIDE!=NULL)
	{
      free(SCPYRT_OVERRIDE);
	}
	SCPYRT_OVERRIDE = (char *)malloc(strlen(buildstr)+5);

	sprintf(SCPYRT_OVERRIDE,"%s",buildstr);
    printf("Got overriding SCPYRT %s\n",SCPYRT_OVERRIDE);
  }

  return 1;
}



int ProcessLine(char *refinefilename,char *theline, int line)
{
  int i,started,insertindex,len;

  



  Ofirst [0] = '\0';
  Osecond[0] = '\0';
  Othird [0] = '\0';

  sscanf(theline,"%s",Ofirst);

  if(!strcmp(Ofirst,"SSCALE"))
  {
    return ProcessStringLine(theline,refinefilename,1,line);
  }
  if(!strcmp(Ofirst,"CLSTAT"))
  {
    return ProcessStringLine(theline,refinefilename,2,line);
  }
  if(!strcmp(Ofirst,"CCPYRT"))
  {
    return ProcessStringLine(theline,refinefilename,3,line);
  }
  if(!strcmp(Ofirst,"SCPYRT"))
  {
    return ProcessStringLine(theline,refinefilename,4,line);
  }

  if(
	  (strcmp(Ofirst,"ParticipantNationCode_TRD2"))     &&
	  (strcmp(Ofirst,"ParticipantNationCode_TRD3"))     &&
	  (strcmp(Ofirst,"ParticipantNationCode_TRD4"))     &&
	  (strcmp(Ofirst,"ParticipantNationCode_TRD41"))    &&
	  (strcmp(Ofirst,"ParticipantNationCode_TRD42"))    &&
	  (strcmp(Ofirst,"ParticipantNationCode_TRD43"))    &&
	  (strcmp(Ofirst,"AccuracyEvaluationMethod_TRD2"))  &&
	  (strcmp(Ofirst,"AccuracyEvaluationMethod_TRD3"))  &&
	  (strcmp(Ofirst,"AccuracyEvaluationMethod_TRD4"))  &&
	  (strcmp(Ofirst,"AccuracyEvaluationMethod_TRD41")) &&
	  (strcmp(Ofirst,"AccuracyEvaluationMethod_TRD42")) &&
	  (strcmp(Ofirst,"AccuracyEvaluationMethod_TRD43")) &&
	  (strcmp(Ofirst,"ParticipantAgencyCode_TRD2"))     &&
	  (strcmp(Ofirst,"ParticipantAgencyCode_TRD3"))     &&
	  (strcmp(Ofirst,"ParticipantAgencyCode_TRD4"))     &&
	  (strcmp(Ofirst,"ParticipantAgencyCode_TRD41"))    &&
	  (strcmp(Ofirst,"ParticipantAgencyCode_TRD42"))    &&
	  (strcmp(Ofirst,"ParticipantAgencyCode_TRD43"))    &&
	  (strcmp(Ofirst,"SourceTypeId_TRD2"))              &&
	  (strcmp(Ofirst,"SourceTypeId_TRD3"))              &&
	  (strcmp(Ofirst,"SourceTypeId_TRD4"))              &&
	  (strcmp(Ofirst,"SourceTypeId_TRD41"))             &&
	  (strcmp(Ofirst,"SourceTypeId_TRD42"))             &&
	  (strcmp(Ofirst,"SourceTypeId_TRD43"))             &&
	  (strcmp(Ofirst,"VerticalSourceTypeId_TRD4"))      &&
	  (strcmp(Ofirst,"VerticalSourceTypeId_TRD41"))     &&
	  (strcmp(Ofirst,"VerticalSourceTypeId_TRD42"))     &&
	  (strcmp(Ofirst,"VerticalSourceTypeId_TRD43"))
	 )
  {
    sprintf(GErr,"Error in Refinement file:\n%s\nline %d:\nExpected one of:\n\
\"ParticipantNationCode_TRD2\",\"ParticipantNationCode_TRD3\",\"ParticipantNationCode_TRD{4,41,42}\",\n\
\"AccuracyEvaluationMethod_TRD2\",\"AccuracyEvaluationMethod_TRD3\",\"AccuracyEvaluationMethod_TRD{4,41,42}\",\n\
\"ParticipantAgencyCode_TRD2\",\"ParticipantAgencyCode_TRD3\",\"ParticipantAgencyCode_TRD{4,41,42}\",\n\
\"SourceTypeId_TRD2\", \"SourceTypeId_TRD3\", \"SourceTypeId_TRD{4,41,42}\", or \"VerticalSourceTypeId_TRD{4,41,42}\"\n\
  but found \"%s\"\n",refinefilename,line,Ofirst);
	return 0;
  }



  len = strlen(theline);
  started = 0;
  insertindex = 0;

  for(i=0;i<len;i++)
  {
    if(started==0) 
	{
      if(theline[i]=='"')
	  {
        started = 1;
	  }
	}
	else if(started==1) 
	{
      if(theline[i]=='"')
	  {
		started = 2;
		insertindex = 0;
	  }
	  else
	  {
        Osecond[insertindex] = theline[i];
		Osecond[insertindex+1] = '\0';
		insertindex = insertindex + 1;
	  }
	}
    else if(started==2) 
	{
      if(theline[i]=='"')
	  {
        started = 3;
	  }
	}
	else if(started==3) 
	{
      if(theline[i]=='"')
	  {
		break;
	  }
	  else
	  {
        Othird[insertindex] = theline[i];
		Othird[insertindex+1] = '\0';
		insertindex = insertindex + 1;
	  }
	}

	if(insertindex>500)
	{
      sprintf(GErr,"Error in Refinement file:\n%s\nline %d:\nExpected data of form <Identifier> \"code\" \"value\":\n  recieved: %s\n",refinefilename,line,theline);
	  return 0;
	}
  }


  

  if(SetRefine(refinefilename,theline,line)==0)
  {
    return 0;
  }

  return 1;
}


int ProcessRefineFile(char *refinefilename)
{
  FILE *rfile;
  char aline[1000];
  int i,foundit,lines = 0;

  if(refinefilename==NULL)
  {
    printf("Error: NULL refine filename\n");
	ZeroRefine(1);
	return 1;
  }

  if(strlen(refinefilename)<5)
  {

    
	ZeroRefine(1);
	return 1;
  }

  printf("processing refine file %s\n",refinefilename);

  rfile = fopen(refinefilename,"rt");
  if(rfile==NULL)
  {
    sprintf(GErr,"Error:  could not read file %s\n",refinefilename);
	return 0;
  }


  fgets(aline,1000,rfile);
  while(!feof(rfile))
  {
    lines = lines + 1;

	if(strlen(aline)>5)
	{
	  if(ProcessLine(refinefilename,aline,lines)==0)
	  { 
        return 0;
	  } 
	}


	aline[0] = '\0';
    fgets(aline,1000,rfile);

	if(feof(rfile))
	{
      if(strlen(aline)>0)
	  {
        
		lines = lines + 1;
	    if(strlen(aline)>5)
		{ 
	      if(ProcessLine(refinefilename,aline,lines)==0)
		  {  
            return 0;
		  }  
		}	   
	  } 
	}
  }

  fclose(rfile);




  if(version==2)
  {
    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax2*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes2[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD2\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax2*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes2[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD2\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax23*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods23[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD2\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax2*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds2[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD2\n",refinefilename);
	  return 0;
	}
  }
  else if(version==3)
  {

    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax3*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes3[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD3\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax3*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes3[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD3\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax23*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods23[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD3\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax3*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds3[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD3\n",refinefilename);
	  return 0;
	}
  }





  else if(version==4)
  {

    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax4*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes4[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD4\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax4*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes4[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD4\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax4*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods4[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD4\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax4*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds4[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD4\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<VerticalSourceTypeIdsMax4*3;i=i+3)
	{ 
      if(!strcmp(VerticalSourceTypeIds4[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for VerticalSourceTypeId_TRD4\n",refinefilename);
	  return 0;
	}
  }




  else if(version==41)
  {

    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD41\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax41*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD41\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD41\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD41\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(VerticalSourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for VerticalSourceTypeId_TRD41\n",refinefilename);
	  return 0;
	}
  }




  else if(version==42)
  {

    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD42\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax42*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes42[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD42\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD42\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD42\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(VerticalSourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for VerticalSourceTypeId_TRD42\n",refinefilename);
	  return 0;
	}
  }






  else if(version==43)
  {

    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD43\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax43*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes43[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD43\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD43\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD43\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(VerticalSourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for VerticalSourceTypeId_TRD43\n",refinefilename);
	  return 0;
	}
  }

  else if(version==44)
  {

    foundit = 0;
    for(i=0;i<ParticipantNationCodesMax41*3;i=i+3)
	{
      if(!strcmp(ParticipantNationCodes41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantNationCode_TRD44\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<ParticipantAgencyCodesMax44*3;i=i+3)
	{ 
      if(!strcmp(ParticipantAgencyCodes44[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for ParticipantAgencyCode_TRD44\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<AccuracyEvaluationMethodsMax41*3;i=i+3)
	{ 
      if(!strcmp(AccuracyEvaluationMethods41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for AccuracyEvaluationMethod_TRD44\n",refinefilename);
	  return 0;
	}

	foundit = 0;
    for(i=0;i<SourceTypeIdsMax44*3;i=i+3)
	{ 
      if(!strcmp(SourceTypeIds44[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for SourceTypeId_TRD44\n",refinefilename);
	  return 0;
	}


	foundit = 0;
    for(i=0;i<VerticalSourceTypeIdsMax41*3;i=i+3)
	{ 
      if(!strcmp(VerticalSourceTypeIds41[i+2],"1"))
	  {
        foundit = 1;
		break;
	  }
	}
	if(foundit==0)
	{
      sprintf(GErr,"Error in refinement file:\n%s\nDid not find any entries for VerticalSourceTypeId_TRD44\n",refinefilename);
	  return 0;
	}
  }





  return 1;
}


void ZeroOutTypes(int index,int type,int subid)
{
  int k;

  for(k=0;k<NumSubregionSources;k++)
  {
     if(
		 (SubregionSources[k].index      == index) &&
         (SubregionSources[k].type       == type) &&
         (SubregionSources[k].subregionid== subid)
	   )
	 {
       SubregionSources[k].type = 0;
	 }
  }
}


void Trex_err(char *t_id,char *path,char *expected,char *found,char *line, int type)
{
  if(type==1)
  {
    fprintf(outerrfile,"Error     : Line %s:\n",line);
    fprintf(outerrfile,"  Path    : %s\n",path);
    fprintf(outerrfile,"  Spec ID : %s\n",t_id);
    fprintf(outerrfile,"  Found   : \"%s\"\n",found);
    fprintf(outerrfile,"  Expected: \"%s\"\n\n",expected);
  }
  else if(type==2)
  {
    fprintf(outerrfile,"Error     : Line %s:\n",line);
    fprintf(outerrfile,"  Path    : %s\n",path);
    fprintf(outerrfile,"  Spec ID : %s\n",t_id);
    fprintf(outerrfile,"  Expected {attribute:value} pair {\"%s\":\"%s\"}\n",expected,found);

    PrintAttributesToErrFile();

  }
}






int HandleReasonableTrexValues(char *lasttoken,char *path)
{
  int i,len,numdecimal=0;

  len = strlen(thedata);

  if(!strcmp(lasttoken,"Boolean"))
  {
	if( (strcmp(thedata,"0")) && (strcmp(thedata,"1")) )
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",path);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected: \"Boolean\" value \"0\" or \"1\"\n\n");
	  return 1;
	}
  }
  else if(!strcmp(lasttoken,"Integer"))
  {
	for(i=0;i<len;i++)
	{
	  if(IsInt(thedata[i])==0)
	  {
		if((i!=0)||(thedata[i]!='-'))
		{
          fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
          fprintf(outerrfile,"  Path    : %s\n",path);
          fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
          fprintf(outerrfile,"  Expected: \"Integer\" value \n\n");
	      return 1;
		}
	  }
	}
  }
  else if(
	       (!strcmp(lasttoken,"Real"))    ||
 	       (!strcmp(lasttoken,"Float"))   ||
 	       (!strcmp(lasttoken,"Measure")) ||
 	       (!strcmp(lasttoken,"Decimal"))
		 )
  {
	for(i=0;i<len;i++)
	{
	  if(IsInt(thedata[i])==0)
	  {
	    if((thedata[i]=='.')&&(numdecimal==0))
		{
          numdecimal = 1;
		}
		else if( (i!=0) || (thedata[i]!='-') )
		{
          fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
          fprintf(outerrfile,"  Path    : %s\n",path);
          fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
          fprintf(outerrfile,"  Expected a real number (the first illegal value is \"%c\" in position %d)\n\n",thedata[i],i+1);
	      return 1;
		}
	  }
	}
  }
  return 0;
}





extern "C" int ProcessTrexItem2(char *rootname,char *t_path,char *t_domain,char *t_codelistval,char *t_id, double northing, double easting, int finished_raw)
{
  double test,lly,uly,xspacing_target;
  int i,j,len,xnum_target,insertindex;
  int thisyear,thismonth,thisday,foundit;
  char thevalue[1000],thevalue1[1000],yeartest[500],yeartest2[500],thevalue2[1000],charspacing[100];
  char beginexpect[300],beginreceived[300],endexpect[300],endreceived[300];
  extern FILE *badtrex;
  extern int fin_xml_date_found,raw_xml_date_found,global_trex_layer,raw_copyright,finished_copyright;
  extern char fin_xml_date[100],raw_xml_date[100];
  extern int GetTimeValues(int *year, int *month, int *day);


  extern int min_fin_xml_found0,
	         max_fin_xml_found0,
             min_fin_xml_found1,
	         max_fin_xml_found1,
			 min_fin_xml_found2,
	         max_fin_xml_found2,
			 min_fin_xml_found3,
	         max_fin_xml_found3,
			 min_raw_xml_found,
			 max_raw_xml_found;

  extern double min_xml_elev_finished0,
	            max_xml_elev_finished0,
				min_xml_elev_finished1,
	            max_xml_elev_finished1,
                min_xml_elev_finished2,
	            max_xml_elev_finished2,
				min_xml_elev_finished3,
	            max_xml_elev_finished3,
				min_xml_elev_raw,
				max_xml_elev_raw;

  extern char *GetTrexSpacings(double lly,double uly,double *xspacing_target,int *xnum_target, int finished_raw);


  if(finished_raw==1)
  {
	
    if(!strcmp(t_id,"7.1"))
	{
	  
	  len = strlen(thedata);
	  insertindex = 0;
	  for(i=0;i<len;i++)
	  {
		if(thedata[i]>=32)
		{
		  thevalue[insertindex] = thedata[i];
		  insertindex = insertindex + 1;
		}
	  }
	  thevalue[insertindex] = '\0';

      fprintf(badtrex,"LPC_SENTINEL %s\n",thevalue);
	} 
    else if(!strcmp(t_id,"147"))
	{ 
	  
	  len = strlen(thedata);
	  insertindex = 0;
	  for(i=0;i<len;i++)
	  {
		if(thedata[i]>=32)
		{
		  thevalue[insertindex] = thedata[i];
		  insertindex = insertindex + 1;
		}
	  }
	  thevalue[insertindex] = '\0';

      fprintf(badtrex,"ENV_SENTINEL %s\n",thevalue);
	} 
  }



  if(!strcmp(t_domain,"Free text character string"))
  {
    
    return 0;
  }
  if(!strcmp(t_domain,"Free text character string "))
  {
    
    return 0;
  }
  if(!strcmp(t_domain,"Free text character string (as defined in Annex C)"))
  {
    
    return 0;
  }
  if(!strcmp(t_domain,"Free text character string (e.g. DEMES 5.2.1.3185A, Parameters File Creation Key 1461734981)"))
  {
    
    return 0;
  }
  if(!strcmp(t_domain,"Integer value"))
  {
    
	return 0;
  }
  if(!strcmp(t_domain,"Integer Value"))
  {
    
	return 0;
  }
  if(
	  (!strcmp(t_domain,"Float value")) ||
      (!strcmp(t_domain,"Float Value"))
    )
  {
    

    if(!strcmp(t_id,"79"))
	{
	  if(IsDecimal(thedata)==1)
	  {
        sscanf(thedata,"%lf",&test);
		if(finished_raw==1)
		{
		  if(global_trex_layer==0)
		  {
            min_fin_xml_found0 = 1;
		    min_xml_elev_finished0 = test;
		  }
		  if(global_trex_layer==1)
		  {
            min_fin_xml_found1 = 1;
		    min_xml_elev_finished1 = test;
		  }
		  if(global_trex_layer==2)
		  {
            min_fin_xml_found2 = 1;
		    min_xml_elev_finished2 = test;
		  }
		  if(global_trex_layer==3)
		  {
            min_fin_xml_found3 = 1;
		    min_xml_elev_finished3 = test;
		  }

		}
		else
		{
          min_raw_xml_found = 1;
		  min_xml_elev_raw = test;
		}
	  }
	}
    if(!strcmp(t_id,"80"))
	{
	  if(IsDecimal(thedata)==1)
	  {
        sscanf(thedata,"%lf",&test);
		if(finished_raw==1)
		{
		  if(global_trex_layer==0)
		  {
            max_fin_xml_found0 = 1;
		    max_xml_elev_finished0 = test;
		  }
		  if(global_trex_layer==1)
		  {
            max_fin_xml_found1 = 1;
		    max_xml_elev_finished1 = test;
		  }
		  if(global_trex_layer==2)
		  {
            max_fin_xml_found2 = 1;
		    max_xml_elev_finished2 = test;
		  }
		  if(global_trex_layer==3)
		  {
            max_fin_xml_found3 = 1;
		    max_xml_elev_finished3 = test;
		  }

		}
		else
		{
          max_raw_xml_found = 1;
		  max_xml_elev_raw = test;
		}
	  }
	}
	return 0;
  }
  if(!strcmp(t_domain,"Boolean value"))
  {
    
	return 0;
  }
  if(!strcmp(t_domain,"Boolean value 1 = yes, 0 = no"))
  {
    
	return 0;
  }
  if(!strcmp(t_domain,"Real value"))
  {
    
	return 0;
  }
  if(!strcmp(t_domain,"Real Value"))
  {
    
	return 0;
  }
  if(!strcmp(t_domain,"YYYY-MM-DD"))
  {
    
	return 0;
  }
  if(
	  
	  (!strcmp(t_domain,"YYYY-MM-DDThh:mm:ssZ")) ||
	  (!strcmp(t_domain,"YYYY MM-DDThh:mm:ssZ"))
	)
  {

	if(!strcmp(t_id,"76"))
	{
	  if(strlen(thedata)<40)
	  { 
	    if(finished_raw==1)
		{ 
	      fin_xml_date_found = 1;
		  sprintf(fin_xml_date,"%s",thedata);
		} 
	    else
		{ 
	      raw_xml_date_found = 1;
		  sprintf(raw_xml_date,"%s",thedata);
		} 
	  } 
	}


    
	return 0;
  }



  if(!strcmp(t_id,"151.3"))
  {
	 
	 

	 if(t_domain[70]=='!') 
	 {

	   

	   if(strlen(thedata)!=77)
	   {
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
	     fprintf(outerrfile,"  Found   : String of length %d\n",(int)strlen(thedata));
		 fprintf(outerrfile,"  Expected: String of length 77 (note 3-byte UTF-8 \"less than or equal to\" symbol E2 89 A4)\n\n");
		 return 1;
	   }

	   sprintf(beginexpect,  "%s",t_domain);
	   sprintf(beginreceived,"%s",thedata);
       beginexpect  [70] = '\0';
       beginreceived[70] = '\0';

	   sprintf(endexpect,  "%s",&t_domain[73]);
	   sprintf(endreceived,"%s",&thedata[73]);
       endexpect  [4] = '\0';
       endreceived[4] = '\0';

	   if(strcmp(beginexpect,beginreceived))
	   {
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
		 fprintf(outerrfile,"  Found   : String beginning with: \"%s\"\n",beginreceived);
		 fprintf(outerrfile,"  Expected: String beginning with: \"%s\"\n\n",beginexpect);
		 return 1;
	   }

	   if(strcmp(endexpect,endreceived))
	   {
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
		 fprintf(outerrfile,"  Found   : String ending with: \"%s\"\n",endreceived);
		 fprintf(outerrfile,"  Expected: String ending with: \"%s\"\n\n",endexpect);
		 return 1;
	   }

	   if((unsigned char)thedata[70]!=0xE2)
	   {  
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 71 should be hex e2 (first UTF-8 byte of the <less than or equal to> symbol) but is (hex) %02x\n\n",(unsigned char)thedata[70]);
		 return 1;
	   }

	   if((unsigned char)thedata[71]!=0x89)
	   {  
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 72 should be hex 89 (second UTF-8 byte of the <less than or equal to> symbol) but is (hex) %02x\n\n",(unsigned char)thedata[71]);
		 return 1;
	   }

	   if((unsigned char)thedata[72]!=0xA4)
	   {  
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 73 should be hex a4 (third UTF-8 byte of the <less than or equal to> symbol) but is (hex) %02x\n\n",(unsigned char)thedata[72]);
		 return 1;
	   }

	 }
	 else
	 {
	   printf("Internal error: did not find ! for %s (%c:%d)\n",t_domain,t_domain[70],t_domain[70]);
	 }

	 return 0;
  }



  if(!strcmp(t_id,"65.1.1"))
  {
	 

	 if(t_domain[0]=='!')
	 {
       

	   if((unsigned char)thedata[0]!=0xC2)
	   {  
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 1 should be hex C2 (first UTF-8 byte of the copyright symbol) but is (hex) %02x\n\n",(unsigned char)thedata[0]);
		 return 1;
	   } 
	   if((unsigned char)thedata[1]!=0xA9)
	   {  
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 2 should be hex A9 (second UTF-8 byte of the copyright symbol) but is (hex) %02x\n\n",(unsigned char)thedata[1]);
		 return 1;
	   } 





	   if(GetTimeValues(&thisyear,&thismonth,&thisday))
	   {
		 if((thisyear<2030)&&(thisyear>2015))
		 {
		   sprintf(yeartest,"%s",&t_domain[2]);

		   foundit = 0;

		   for(i=2013;i<=thisyear;i++)
		   {
			 sprintf(yeartest2,"%d",i);

		     

			 yeartest[10] = yeartest2[0];
			 yeartest[11] = yeartest2[1];
			 yeartest[12] = yeartest2[2];
			 yeartest[13] = yeartest2[3];

	         if(!strcmp(&thedata[2],yeartest))
			 {
			   foundit = 1;

			   if(finished_raw==1)
			   {
				 finished_copyright = i;
			   }
			   else
			   {
				 raw_copyright = i;
			   }



			   break;
			 }
		   }

		   if(foundit==0)
		   {
             fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
             fprintf(outerrfile,"  Path    : %s\n",t_path);
             fprintf(outerrfile,"  Spec ID : %s\n",t_id);
	         fprintf(outerrfile,"  Found   : text after copyright symbol: \"%s\"\n",&thedata[2]);
	         fprintf(outerrfile,"  Expected: text after copyright symbol: \"%s\" (YYYY: 2013 to %d)\n\n",&t_domain[2],thisyear);
		     return 1;
		   }

		 }
		 else
		 {
	       fprintf(outerrfile,"Error     : Invalid current year %d found.  Cannot fully perform metadata analysis.\n\n",thisyear);
		 }
	   }










	 }



	 else if(t_domain[18]=='!')
	 {
	    char temptest[100];

	   

       

	   if(strncmp(thedata,t_domain,18))
	   {
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
		 sprintf(temptest,"%s",thedata);
		 temptest[18] = '\0';
		 fprintf(outerrfile,"  Found   : First 18 characters: %s\n",temptest);
		 sprintf(temptest,"%s",t_domain);
		 temptest[18] = '\0';
		 fprintf(outerrfile,"  Expected: First 18 characters: %s\n\n",temptest);
		 return 1;
	   }

	   if((unsigned char)thedata[18]!=0xC2)
	   {
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 19 should be hex C2 (first UTF-8 byte of the copyright symbol) but is (hex) %02x\n\n",(unsigned char)thedata[18]);
		 return 1;
	   }
	   if((unsigned char)thedata[19]!=0xA9)
	   {  
	     fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
         fprintf(outerrfile,"  Path    : %s\n",t_path);
         fprintf(outerrfile,"  Spec ID : %s\n",t_id);
         fprintf(outerrfile,"    character 20 should be hex A9 (second UTF-8 byte of the copyright symbol) but is (hex) %02x\n\n",(unsigned char)thedata[19]);
		 return 1;
	   }


	   if(GetTimeValues(&thisyear,&thismonth,&thisday))
	   {
		 if((thisyear<2030)&&(thisyear>2015))
		 {
		   sprintf(yeartest,"%s",&t_domain[20]);

		   foundit = 0;

		   for(i=2013;i<=thisyear;i++)
		   {
			 sprintf(yeartest2,"%d",i);

		     

			 yeartest[10] = yeartest2[0];
			 yeartest[11] = yeartest2[1];
			 yeartest[12] = yeartest2[2];
			 yeartest[13] = yeartest2[3];

	         if(!strcmp(&thedata[20],yeartest))
			 {
			   foundit = 1;

			   if(finished_raw==1)
			   {
				 finished_copyright = i;
			   }
			   else
			   {
				 raw_copyright = i;
			   }



			   break;
			 }
		   }

		   if(foundit==0)
		   {
             fprintf(outerrfile,"Error     : Line %s\n",GetLineNumber());
             fprintf(outerrfile,"  Path    : %s\n",t_path);
             fprintf(outerrfile,"  Spec ID : %s\n",t_id);
	         fprintf(outerrfile,"  Found   : text after copyright symbol: %s\n",&thedata[20]);
	         fprintf(outerrfile,"  Expected: text after copyright symbol: %s (YYYY: 2013 to %d)\n\n",&t_domain[20],thisyear);
		     return 1;
		   }

		 }
		 else
		 {
	       fprintf(outerrfile,"Error     : Invalid current year %d found.  Cannot fully perform metadata analysis.\n\n",thisyear);
		 }
	   }

	 }

	 return 0;
  }


  if(
	  (!strcmp(t_domain,"variable")) ||
	  (!strcmp(t_domain,"Variable"))
	)
  {
	lly = northing;
	uly = northing + 1.0;

	if(finished_raw==1) 
	{
      sprintf(charspacing,"%s",GetTrexSpacings(lly,uly,&xspacing_target,&xnum_target,1));
	}
	else 
	{
      sprintf(charspacing,"%s",GetTrexSpacings(lly,uly,&xspacing_target,&xnum_target,0));
	}

	if(!strcmp(t_id,"23.2"))
	{
	  sprintf(thevalue,"%d",xnum_target);

	  if(strcmp(thedata,thevalue))
	  {
        Trex_err(t_id,t_path,thevalue,thedata,GetLineNumber(),1);
		return 1;
	  }
	}
	if(!strcmp(t_id,"24.2"))
	{
	  if(strcmp(thedata,charspacing))
	  {
        Trex_err(t_id,t_path,charspacing,thedata,GetLineNumber(),1);
		return 1;
	  }
	}
    return 0;
  }



  if(!strcmp(t_domain,"Free text character string  (eng)"))
  {
	if(strcmp(thedata,"eng"))
	{
      Trex_err(t_id,t_path,(char *)"eng",thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;
  }



  if(!strcmp(t_domain,"Free text character string (TREx DEM Raw Specification or TREx DEM Finished Specification)"))
  {
	if(finished_raw==1)
	{
	  if(strcmp(thedata,"TREx DEM Finished Specification"))
	  {
        Trex_err(t_id,t_path,(char *)"TREx DEM Finished Specification",thedata,GetLineNumber(),1);
	    return 1; 
	  } 
	  return 0;
	}
	else if(finished_raw==0)
	{
	  if(strcmp(thedata,"TREx DEM Raw Specification"))
	  {
        Trex_err(t_id,t_path,(char *)"TREx DEM Raw Specification",thedata,GetLineNumber(),1);
	    return 1; 
	  } 
	  return 0;
	}
  }





  if(!strcmp(t_domain,"Free text character string (eng)"))
  {
	if(strcmp(thedata,"eng"))
	{
      Trex_err(t_id,t_path,(char *)"eng",thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;
  }


  if(!strcmp(t_domain,"Free text character string (Geotile or Geocell)"))
  {
	if( (strcmp(thedata,"Geotile")) && (strcmp(thedata,"Geocell")) )
	{
      Trex_err(t_id,t_path,(char *)"Geotile or Geocell",thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;
  }

  if(!strcmp(t_domain,"Free text character string (Geotiles or Geocells)"))
  {
	if( (strcmp(thedata,"Geotiles")) && (strcmp(thedata,"Geocells")) )
	{
      Trex_err(t_id,t_path,(char *)"Geotiles or Geocells",thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;
  }

  if(!strcmp(t_domain,"Free text character string (use value from codespace: 4326 for WGS84)"))
  {
	if(strcmp(thedata,"4326"))
	{
      Trex_err(t_id,t_path,(char *)"4326",thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;
  }





  if(!strcmp(t_domain,"Float value (format: -180.00000000 to +179.00000000, use 8 decimal places, zero filled as necessary)"))
  {

	

    if(IsDecimal(thedata)==0)
	{
      return 1;
	} 

	sscanf(thedata,"%lf",&test);

	if((test<-180)||(test>179))
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number between -180.0 and 179.0\n\n");
	  return 1;
	}

    len = strlen(thedata);
	if(len<10)
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

	if(thedata[len-9]!='.')
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

	if(strstr(t_path,"westBoundLongitude")) 
	{
	  

	  sprintf(charspacing,"%.8lf",easting);

	  if(strcmp(charspacing,thedata))
	  {
        fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
        fprintf(outerrfile,"  Path    : %s\n",t_path);
        fprintf(outerrfile,"  Spec ID : %s\n",t_id);
        fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
        fprintf(outerrfile,"  Expected: \"%s\"\n\n",charspacing);
	    return 1;
	  }
	}
	else
	{
	  printf("Internal error: expected westBoundLongitude, got %s for %s %s\n",t_path,t_id,GetLineNumber());
	}

    return 0;
  }




  if(!strcmp(t_domain,"Float value (format: -179.00000000 to +180.00000000, use 8 decimal places, zero filled as necessary)"))
  {

	

    if(IsDecimal(thedata)==0)
	{
      return 1;
	} 

	sscanf(thedata,"%lf",&test);

	if((test<-179)||(test>180))
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number between -179.0 and 180.0\n\n");
	  return 1;
	}

    len = strlen(thedata);
	if(len<10)
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

	if(thedata[len-9]!='.')
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

    if(strstr(t_path,"eastBoundLongitude"))
	{
	  

	  if(finished_raw==1)
	  {
	    sprintf(charspacing,"%.8lf",easting+1);
	  }
	  else
	  {
        if(northing<59.9) 
		{
	      sprintf(charspacing,"%.8lf",easting+1);
		}
        else if(northing<79.9) 
		{
	      sprintf(charspacing,"%.8lf",easting+2);
		}
        else if(northing<79.9) 
		{
	      sprintf(charspacing,"%.8lf",easting+4);
		}
	  }

	  if(strcmp(charspacing,thedata))
	  {
        fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
        fprintf(outerrfile,"  Path    : %s\n",t_path);
        fprintf(outerrfile,"  Spec ID : %s\n",t_id);
        fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
        fprintf(outerrfile,"  Expected: \"%s\"\n\n",charspacing);
	    return 1;
	  }
	}
	else
	{
	  printf("Internal error: expected eastBoundLongitude, got %s for %s %s\n",t_path,t_id,GetLineNumber());
	}

    return 0;
  }



  if(!strcmp(t_domain,"Float value (format: -90.00000000 to +89.00000000, use 8 decimal places, zero filled as necessary)"))
  {

	

    if(IsDecimal(thedata)==0)
	{
      return 1;
	} 

	sscanf(thedata,"%lf",&test);

	if((test<-90)||(test>89))
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number between -90.0 and 89.0\n\n");
	  return 1;
	}

    len = strlen(thedata);
	if(len<10)
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

	if(thedata[len-9]!='.')
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

	if(strstr(t_path,"southBoundLatitude"))
	{
	  

	  sprintf(charspacing,"%.8lf",northing);

	  if(strcmp(charspacing,thedata))
	  {
        fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
        fprintf(outerrfile,"  Path    : %s\n",t_path);
        fprintf(outerrfile,"  Spec ID : %s\n",t_id);
        fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
        fprintf(outerrfile,"  Expected: \"%s\"\n\n",charspacing);
	    return 1;
	  }
	}
	else
	{
	  printf("Internal error: expected southBoundLatitude, got %s for %s %s\n",t_path,t_id,GetLineNumber());
	}

	return 0;
  }



  if(!strcmp(t_domain,"Float value (format: -89.00000000 to +90.00000000, use 8 decimal places, zero filled as necessary)"))
  {

	

    if(IsDecimal(thedata)==0)
	{
      return 1;
	} 

	sscanf(thedata,"%lf",&test);

	if((test<-89)||(test>90))
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number between -89.0 and 90.0\n\n");
	  return 1;
	}

    len = strlen(thedata);
	if(len<10)
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

	if(thedata[len-9]!='.')
	{
      fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
      fprintf(outerrfile,"  Path    : %s\n",t_path);
      fprintf(outerrfile,"  Spec ID : %s\n",t_id);
      fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
      fprintf(outerrfile,"  Expected a number with 8 values after the decimal\n\n");
	  return 1;
	}

    if(strstr(t_path,"northBoundLatitude"))
	{
	  

	  sprintf(charspacing,"%.8lf",northing+1);

	  if(strcmp(charspacing,thedata))
	  {
        fprintf(outerrfile,"Error     :  Line %s:\n",GetLineNumber());
        fprintf(outerrfile,"  Path    : %s\n",t_path);
        fprintf(outerrfile,"  Spec ID : %s\n",t_id);
        fprintf(outerrfile,"  Found   : \"%s\"\n",thedata);
        fprintf(outerrfile,"  Expected: \"%s\"\n\n",charspacing);
	    return 1;
	  }
	}
	else
	{
	  printf("Internal error: expected northBoundLatitude, got %s for %s %s\n",t_path,t_id,GetLineNumber());
	}

	return 0;
  }







  if(!strcmp(t_domain,"Free text character string  Standard format : +# ### ### ####"))
  {
    return 0;
  }




  if(
	  (!strncmp(t_domain,"TDx_bDDcDDD_vv",14))||
	  (!strncmp(t_domain,"TDx_eDDfDDD_vv",14))
	)
  { 
   	sprintf(thevalue1,"%s",rootname);
   	sprintf(thevalue2,"%s",&t_domain[14]);

	sprintf(thevalue,"%s%s",thevalue1,thevalue2);

	if(strcmp(thedata,thevalue))
	{
      Trex_err(t_id,t_path,thevalue,thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;  
  }


  if(
     (!strncmp(t_domain,"./PREVIEW/TDx_bDDcDDD_vv",24)) ||
     (!strncmp(t_domain,"./PREVIEW/TDR_bDDcDDD_vv",24)) ||
     (!strncmp(t_domain,"./PREVIEW/TDF_bDDcDDD_vv",24)) ||
     (!strncmp(t_domain,"./PREVIEW/TDx_eDDfDDD_vv",24)) ||
     (!strncmp(t_domain,"./PREVIEW/TDR_eDDfDDD_vv",24)) ||
     (!strncmp(t_domain,"./PREVIEW/TDF_eDDfDDD_vv",24))
	)
  { 
   	sprintf(thevalue1,"./PREVIEW/%s",rootname);
   	sprintf(thevalue2,"%s",&t_domain[24]);

	sprintf(thevalue,"%s%s",thevalue1,thevalue2);

	if(strcmp(thedata,thevalue))
	{
      Trex_err(t_id,t_path,thevalue,thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;  
  }


  if(
	  (!strncmp(t_domain,"./DEM/TDx_bDDcDDD_vv",20)) ||
	  (!strncmp(t_domain,"./DEM/TDx_eDDfDDD_vv",20))
	  )
  { 
 	sprintf(thevalue1,"./DEM/%s",rootname);
   	sprintf(thevalue2,"%s",&t_domain[20]);

	sprintf(thevalue,"%s%s",thevalue1,thevalue2);

	if(strcmp(thedata,thevalue))
	{
      Trex_err(t_id,t_path,thevalue,thedata,GetLineNumber(),1);
	  return 1; 
	}
	return 0;  
  }





  

  if(!strncmp(t_domain,"Free text character string (",28))
  {
	sprintf(thevalue,"%s",&t_domain[28]);
    len = strlen(thevalue);
	if(thevalue[len-1]==')')
	{
	  thevalue[len-1] = '\0';
	}

	if(strcmp(thedata,thevalue))
	{
      Trex_err(t_id,t_path,thevalue,thedata,GetLineNumber(),1);
	  return 1;
	}
	return 0;
  }




  

  if(!strncmp(t_domain,"id = ",5))
  {
    sprintf(thevalue,"%s",&t_domain[5]);
    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"id")) &&
		   (!strcmp(attributeV[i],thevalue))
		 )
	   {
		 return 0;
	   }
	}
    Trex_err(t_id,t_path,(char *)"id",thevalue,GetLineNumber(),2);
	return 1;
  }

  if(!strncmp(t_domain,"uom = ",6))
  {
    sprintf(thevalue,"%s",&t_domain[6]);
    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"uom")) &&
		   (!strcmp(attributeV[i],thevalue))
		 )
	   {
		 return 0;
	   }
	}
    Trex_err(t_id,t_path,(char *)"uom",thevalue,GetLineNumber(),2);
	return 1;
  }



  if(!strncmp(t_domain,"frame = ",8))
  {
    sprintf(thevalue,"%s",&t_domain[8]);
    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"frame")) &&
		   (!strcmp(attributeV[i],thevalue))
		 )
	   {
		 return 0;
	   }
	}
    Trex_err(t_id,t_path,(char *)"frame",thevalue,GetLineNumber(),2);
	return 1;
  }
  if(!strncmp(t_domain,"gml:id = ",9))
  {
    sprintf(thevalue,"%s",&t_domain[9]);
    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"gml:id")) &&
		   (!strcmp(attributeV[i],thevalue))
		 )
	   {
		 return 0;
	   }
	}
    Trex_err(t_id,t_path,(char *)"gml:id",thevalue,GetLineNumber(),2);
	return 1;
  }
  if(!strncmp(t_domain,"codeSpace = ",12))
  {
    sprintf(thevalue,"%s",&t_domain[12]);
    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"codeSpace")) &&
		   (!strcmp(attributeV[i],thevalue))
		 )
	   {
		 return 0;
	   }
	}
    Trex_err(t_id,t_path,(char *)"codeSpace",thevalue,GetLineNumber(),2);
	return 1;
  }
  if(!strncmp(t_domain,"xlink:href = ",13))
  {
    sprintf(thevalue,"%s",&t_domain[13]);
    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"xlink:href")) &&
		   (!strcmp(attributeV[i],thevalue))
		 )
	   {
		 return 0;
	   }
	}
    Trex_err(t_id,t_path,(char *)"xlink:href",thevalue,GetLineNumber(),2);
	return 1;
  }




  if(!strncmp(t_domain,"codeList",8))
  {

    

    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"codeList")) &&
		   (!strcmp(attributeV[i],&t_domain[11])) 
	      )
	   { 
         for(j=i+1;j<numattributes;j++)
		 {  
           if(
		       (!strcmp(attributes[j],"codeListValue")) &&
		       (!strcmp(attributeV[j],&t_codelistval[16])) 
		     )
		   {   
             return 0;
		   }
		   

           if(!strcmp(t_codelistval,"codeListValue = onGoing (TDR) or codeListValue = completed (TDF)"))
		   {
             if(finished_raw==1)
			 {
			   if(!strcmp(attributeV[j],"completed"))
			   {
				 return 0;
			   }
			 }
             if(finished_raw==0)
			 {
			   if(!strcmp(attributeV[j],"onGoing"))
			   {
				 return 0;
			   }
			 }
		   }
		 }   
	   }  
	 }
    Trex_err(t_id,t_path,(char *)t_domain,t_codelistval,GetLineNumber(),2);
	return 1; 
  }





  

  if(!strcmp(t_domain,thedata))
  {
	return 0;
  }

  Trex_err(t_id,t_path,(char *)t_domain,thedata,GetLineNumber(),1);
  return 1;



}


int ProcessTrexItem(char *thispath,char *lasttoken, char *rootname, char *t_domain,char *t_codelistval,char *t_id, double northing, double easting, int finished_raw)
{
  int valuelen,year,month,day,hour,minute,second,numerrs=0;

  

  


  valuelen = strlen(thedata);

  if(valuelen>0)
  {
	numerrs = numerrs + HandleReasonableTrexValues(lasttoken,thispath);

	if(!strcmp(lasttoken,"Date"))
	{ 
      if(ValidateDate(1,&year,&month,&day)!=1)
	  {
		numerrs = numerrs + 1;
	  }
	} 
    if(!strcmp(lasttoken,"DateTime"))
	{
      if(ValidateDateTime(&year,&month,&day,&hour,&minute,&second)!=1)
	  {
		numerrs = numerrs + 1;
	  }
	}
    if(!strcmp(lasttoken,"timePosition"))
	{
      if(ValidateDateTime(&year,&month,&day,&hour,&minute,&second)!=1)
	  {
		numerrs = numerrs + 1;
	  }
	}
  }

  numerrs = numerrs + ProcessTrexItem2(rootname,thispath,t_domain,t_codelistval,t_id,northing,easting,finished_raw);

  return numerrs;

}




int ProcessSevenPaths(int type,CMarkup xml,
		               char *path1,char *path2,char *path3,char *path4,char *path5,char *path6,char *path7,
					   char *matchstring,char *matchstring2,char *matchstring3,
					   char *attrib1,char *attrib2,char *attrib3,char *attrib4,
					   char *additionalup2path1,char *additionalup2path2,char *additionalup2path3,char *additionalup2path4,char *additionalup2path5,
					   char *specid1,char *specid2,char *specid3,
					   char *specid4,char *specid5,char *specid6,
					   char *firstpath,char *secondpath,char *thirdpath,
                       char *fourthpath,char *fifthpath,char *sixthpath)
{
   int i,foundit=0,numerrs=0,foundfirst=0,foundsecond=0,foundseconda=0,foundthird=0,
       foundfourth=0,foundfifth=0,foundsixth=0;
   char buildpath[1000],buildpath1[1000];


   xml.ResetPos();


   while(xml.FindElem(path1))
   {
	if(foundfirst>0){break;}
	xml.IntoElem();

    while(xml.FindElem(path2))
	{ 
	 if(foundfirst>0){break;}
	 xml.IntoElem();

     while(xml.FindElem(path3))
	 {  
	  if(foundfirst>0){break;}
	  xml.IntoElem();

      while(xml.FindElem(path4))
	  { 
	   if(foundfirst>0){break;}
	   xml.IntoElem();

       while(xml.FindElem(path5))
	   {  
	    if(foundfirst>0){break;}
	    xml.IntoElem();

        while(xml.FindElem(path6))
		{  
	     if(foundfirst>0){break;}
	     xml.IntoElem();

         while(xml.FindElem(path7))
		 {   

	       if(foundfirst>0){break;}


		  if(!strcmp(GET_CSTR(xml.GetData(),1),matchstring))
		  {

			foundfirst = 1;
			

			
            xml.OutOfElem();
            xml.OutOfElem();

			sprintf(buildpath1,"%s",GET_CSTR(xml.GetElemPath(),1));






			
            
            

			if(type==1)
			{

			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path1);

			  if(xml.FindElem(buildpath))
			  { 

			    foundsecond = 1; 

			    FillInAttributes(xml);

                for(i=0;i<numattributes;i++)
				{   
	              if(!strcmp(attributes[i],attrib1))
				  {   
				    if(!strcmp(attributeV[i],attrib2))
					{ 
                      foundseconda = 1;
					} 
				  } 
				}  

			    if(foundseconda==0)
				{ 
                  sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));

                  SetUpLineOffsets(xml);

                  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"%s\"> }\n",
	                ParsePath(0),attrib1,attrib2);

                  PrintAttributesToErrFile();

				  numerrs = numerrs + 1;
				} 
			  } 


			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path2);

			  if(xml.FindElem(buildpath))
			  { 
			    

			    foundthird = 1;
			  } 

			  
			  break;
			}

			else if(type==2)
			{

               
			   

				



			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path1);

			  if(xml.FindElem(buildpath))
			  { 
                 foundsecond = 1;
		         if(strcmp(GET_CSTR(xml.GetData(),1),matchstring2))
				 {
			       SetUpLineOffsets(xml);
                   Trex_err(specid2,secondpath,matchstring2,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
				   numerrs = numerrs + 1;
				 }
			  } 





			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path2);

			  if(xml.FindElem(buildpath))
			  { 
                 foundthird = 1;
		         if(strcmp(GET_CSTR(xml.GetData(),1),matchstring3))
				 {
			       SetUpLineOffsets(xml);
                   Trex_err(specid3,thirdpath,matchstring3,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
				   numerrs = numerrs + 1;
				 }
			  } 





			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path3);

			  if(xml.FindElem(buildpath))
			  { 
                foundfourth = 1;

   			    FillInAttributes(xml);

				foundit = 0;
                for(i=0;i<numattributes;i++)
				{  
                  if(  
	                  (!strcmp(attributes[i],attrib1)) &&
		              (!strcmp(attributeV[i],attrib2))
	                ) 
				  {   
                    foundit = 1;
				  }
				}

				if(foundit==0)
				{
                  sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));

				  SetUpLineOffsets(xml);

                  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"%s\"> }\n",
	                ParsePath(0),attrib1,attrib2);

				  PrintAttributesToErrFile();

				  numerrs = numerrs + 1;
				 }
			  } 








			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path4);

			  if(xml.FindElem(buildpath))
			  { 
                foundfifth = 1;

   			    FillInAttributes(xml);

				foundit = 0;
                for(i=0;i<numattributes;i++)
				{  
                  if(  
	                  (!strcmp(attributes[i],attrib3)) &&
		              (!strcmp(attributeV[i],attrib4))
	                ) 
				  {   
                    foundit = 1;
				  }
				}

				if(foundit==0)
				{
                  sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));

				  SetUpLineOffsets(xml);

                  fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"%s\": <\"%s\"> }\n",
	                ParsePath(0),attrib3,attrib4);

				  PrintAttributesToErrFile();

				  numerrs = numerrs + 1;
				 }
			  } 






			  sprintf(buildpath,"%s/%s",buildpath1,additionalup2path5);

			  if(xml.FindElem(buildpath))
			  { 
				 
                 foundsixth = 1;
			  } 





			} 



			} 

		


	      xml.IntoElem();
          xml.OutOfElem();
		 }
         xml.OutOfElem();
		}  
        xml.OutOfElem();
	   }  
       xml.OutOfElem();
	  } 
      xml.OutOfElem();
	 }
     xml.OutOfElem();
	}
    xml.OutOfElem();
   }


   if((type==1)||(type==2))
   {
     if(foundfirst==0)
	 { 
       fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
       fprintf(outerrfile,"  Path    : %s\n",firstpath);
       fprintf(outerrfile,"  Spec ID : %s\n\n",specid1);
	   numerrs = numerrs + 1;
	 } 
     if(foundsecond==0)
	 { 
       fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
       fprintf(outerrfile,"  Path    : %s\n",secondpath);
       fprintf(outerrfile,"  Spec ID : %s\n\n",specid2);
	   numerrs = numerrs + 1;
	 } 
     if(foundthird==0)
	 {
       fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
       fprintf(outerrfile,"  Path    : %s\n",thirdpath);
       fprintf(outerrfile,"  Spec ID : %s\n\n",specid3);
	   numerrs = numerrs + 1;
	 } 
   }

   if(type==2)
   {
     if(foundfourth==0)
	 { 
       fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
       fprintf(outerrfile,"  Path    : %s\n",fourthpath);
       fprintf(outerrfile,"  Spec ID : %s\n\n",specid4);
	   numerrs = numerrs + 1;
	 } 
     if(foundfifth==0)
	 { 
       fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
       fprintf(outerrfile,"  Path    : %s\n",fifthpath);
       fprintf(outerrfile,"  Spec ID : %s\n\n",specid5);
	   numerrs = numerrs + 1;
	 } 
     if(foundsixth==0)
	 {
       fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
       fprintf(outerrfile,"  Path    : %s\n",sixthpath);
       fprintf(outerrfile,"  Spec ID : %s\n\n",specid6);
	   numerrs = numerrs + 1;
	 } 
   }


   return numerrs;
}



int CheckNeighbor(CMarkup xml,char *path1,char *path2,char *attrib1,char *attrib2,char *path3,char *specid1,
                    char *specid2,char *specpath2,char *specval2,
                    char *specid3,char *specpath3,char *specval3,
                    char *specid4,char *specpath4,char *specval4,
                    char *specid5,char *specpath5,char *specval5,
					char *specid6,char *specpath6 ,
					char *specid7,char *specpath7,char *attr1,char *attrval1,char *attr2,char *attrval2,
					char *specid8,char *specpath8,
					char *specid9,char *specpath9,char *attr3,char *attrval3,
					char *specid10,char *specpath10
					)
{
  int i,j,len,passval=0,numerrs=0;
  char predpath[1000],savepath[1000],newpath[1000],newverbosepath[1000];

  


  xml.ResetPos();

  if(xml.FindElem(path1))
  {
    xml.IntoElem();

    while(xml.FindElem("gmd:report"))
	 { 
	   xml.IntoElem();


	   sprintf(predpath,"%s[@%s='%s']",path2,attrib1,attrib2);  

	   if(xml.FindElem(predpath))
	   { 

	      sprintf(savepath,"%s",GET_CSTR(xml.GetElemPath(),1));


		  


          sprintf(newpath,"%s/%s",savepath,specpath2);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath2);

		   if(xml.FindElem(newpath))
		   {

		     if(strcmp(GET_CSTR(xml.GetData(),1),specval2))
			 {
			   SetUpLineOffsets(xml);
               Trex_err(specid2,newverbosepath,specval2,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
			   numerrs = numerrs + 1;
			 }
		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid2);
             numerrs = numerrs + 1;
		   }





		  


          sprintf(newpath,"%s/%s",savepath,specpath3);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath3);

		   
		   if(xml.FindElem(newpath))
		   {
		     if(strcmp(GET_CSTR(xml.GetData(),1),specval3))
			 {
			   SetUpLineOffsets(xml);
               Trex_err(specid3,newverbosepath,specval3,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
			   numerrs = numerrs + 1;
			 }
		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid3);
             numerrs = numerrs + 1;
		   }



		  


          sprintf(newpath,"%s/%s",savepath,specpath4);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath4);


		   if(xml.FindElem(newpath))
		   {
		     if(strcmp(GET_CSTR(xml.GetData(),1),specval4))
			 {
			   SetUpLineOffsets(xml);
               Trex_err(specid4,newverbosepath,specval4,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
			   numerrs = numerrs + 1;
			 }
		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid4);
             numerrs = numerrs + 1;
		   }




		  


          sprintf(newpath,"%s/%s",savepath,specpath6);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath6);

		   if(xml.FindElem(newpath))
		   {
             int year,month,day;

             sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

			 SetUpLineOffsets(xml);

			 savespecialpath = 1;
			 sprintf(specialpath,"%s",newverbosepath);

             if(ValidateDate(1,&year,&month,&day)!=1)
			 { 
		       numerrs = numerrs + 1;
			 } 
			 savespecialpath = 0;
		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid6);
             numerrs = numerrs + 1;
		   }



		  


          sprintf(newpath,"%s/%s",savepath,specpath7);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath7);

		   if(xml.FindElem(newpath))
		   {
             int foundit=0;
			 char temp1[1000],temp2[1000];

   			 FillInAttributes(xml);
			 SetUpLineOffsets(xml);


             for(i=0;i<numattributes;i++)
			 {  
               if(  
	               (!strcmp(attributes[i],attr1)) &&
		           (!strcmp(attributeV[i],attrval1))
	             ) 
			   {  
                 for(j=i+1;j<numattributes;j++)
				 {   
                   if(
		               (!strcmp(attributes[j],attr2)) &&
		               (!strcmp(attributeV[j],attrval2))
		             )
				   {    
                     foundit = 1;
				   }    
				 }    
			   }   
			 } 

			 if(foundit==0)
			 {
			   sprintf(temp1,"codeList = %s",attrval1);
			   sprintf(temp2,"codeListValue = %s",attrval2);

               Trex_err(specid7,newverbosepath,temp1,temp2,GetLineNumber(),2);
			   numerrs = numerrs + 1;
			 }

		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid7);
             numerrs = numerrs + 1;
		   }







		  


          sprintf(newpath,"%s/%s",savepath,specpath5);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath5);

		   if(xml.FindElem(newpath))
		   {
		     if(strcmp(GET_CSTR(xml.GetData(),1),specval5))
			 {
			   SetUpLineOffsets(xml);
               Trex_err(specid5,newverbosepath,specval5,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
			   numerrs = numerrs + 1;
			 }
		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid5);
             numerrs = numerrs + 1;
		   }






		  


          sprintf(newpath,"%s/%s",savepath,specpath8);
		  sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath8);
          passval = 0;

		  if(xml.FindElem(newpath))
		  {

			sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));
			 
		    if( (strcmp(thedata,"0")) && (strcmp(thedata,"1")) )
			{
			  SetUpLineOffsets(xml);
              Trex_err(specid8,newverbosepath,(char *)"Boolean value 0 or 1",GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
              numerrs = numerrs + 1;
			}

			if(!strcmp(thedata,"1"))
			{
			  passval = 1;
			}
		  } 
		  else
		  {
            fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
            fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
            fprintf(outerrfile,"  Spec ID : %s\n\n",specid8);
            numerrs = numerrs + 1;
		  }





		  if(passval==1)
		  {
			

			



			

            sprintf(newpath,"%s/%s",savepath,specpath9);
		    sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath9);

		    if(xml.FindElem(newpath))
			{ 
              int foundit=0;

   			  FillInAttributes(xml);
			  SetUpLineOffsets(xml);


              for(i=0;i<numattributes;i++)
			  {
                if(
	               (!strcmp(attributes[i],attr3))    &&
		           (!strcmp(attributeV[i],attrval3))
	              )
				{
                  foundit = 1;
				}
			  }

			 if(foundit==0)
			 {
               Trex_err(specid9,newverbosepath,attr3,attrval3,GetLineNumber(),2);
			   numerrs = numerrs + 1;
			 }
		   }
		   else
		   {
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid9);
             numerrs = numerrs + 1;
		   }



		   


           sprintf(newpath,"%s/%s",savepath,specpath10);
		   sprintf(newverbosepath,"%s[@%s='%s']/%s",savepath,attrib1,attrib2,specpath10);
           passval = 0;

		   if(xml.FindElem(newpath))
		   {

			 sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

			 len = strlen(thedata);

	         for(i=0;i<len;i++)
			 {  
	           if(IsInt(thedata[i])==0)
			   { 
		         if((i!=0)||(thedata[i]!='-'))
				 { 
			       SetUpLineOffsets(xml);
                   Trex_err(specid10,newverbosepath,(char *)"Integer value",thedata,GetLineNumber(),1);
				   numerrs = numerrs + 1;
				   break;
				 } 
			   } 
			 } 
		   }  
		   else
		   { 
             fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
             fprintf(outerrfile,"  Path    : %s\n",newverbosepath);
             fprintf(outerrfile,"  Spec ID : %s\n\n",specid10);
             numerrs = numerrs + 1;
		   } 

		  } 



		   xml.OutOfElem();

		  return numerrs;
	   } 
	   xml.OutOfElem();
	 } 

     xml.OutOfElem();


  }


  
  

  fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
  fprintf(outerrfile,"  Path    : %s\n",path3);
  fprintf(outerrfile,"  Spec ID : %s\n\n",specid1);
  numerrs = numerrs + 1;


  return numerrs;

}










int ProcessLongPaths(CMarkup xml,int finished_raw)
{
   double tempD;
   int i,numerrs=0,foundicesat=0,found1191=0,found1201=0,found1201a=0,found1211=0;
   char buildpath[1000],buildpath1[1000],tempchar[300];
   char path1[200];
   char path2[200];
   char path3[200];
   char path4[200];
   char path5[200];
   char path6[200];
   char path7[200];
   char matchstring[200],matchstring2[200],matchstring3[200],
	   attrib1[200],attrib2[200],attrib3[200],attrib4[200],
	   additionalup2path1[300],additionalup2path2[300],additionalup2path3[300],additionalup2path4[300],additionalup2path5[300];
   char path1191[]="/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"Number of ICESat Validation Points\"]";
   char path1201[]="/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"Number of ICESat Validation Points\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]";
   char path1211[]="/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"Number of ICESat Validation Points\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer";

   xml.ResetPos();


   



   while(xml.FindElem("gmd:MD_Metadata"))
   {
	if(found1191>0){break;}
	xml.IntoElem();

    while(xml.FindElem("gmd:dataQualityInfo"))
	{ 
	 if(found1191>0){break;}
	 xml.IntoElem();

     while(xml.FindElem("gmd:DQ_DataQuality"))
	 {  
	  if(found1191>0){break;}
	  xml.IntoElem();

      while(xml.FindElem("gmd:report"))
	  { 
	   if(found1191>0){break;}
	   xml.IntoElem();

       while(xml.FindElem("gmd:DQ_CompletenessCommission"))
	   {  
	    if(found1191>0){break;}
	    xml.IntoElem();

        while(xml.FindElem("gmd:nameOfMeasure"))
		{  
	     if(found1191>0){break;}
	     xml.IntoElem();

         while(xml.FindElem("gco:CharacterString"))
		 {   
	       if(found1191>0){break;}

		  if(!strcmp(GET_CSTR(xml.GetData(),1),"Number of ICESat Validation Points"))
		  {

			found1191 = 1;
			

			
            xml.OutOfElem();
            xml.OutOfElem();

			sprintf(buildpath1,"%s",GET_CSTR(xml.GetElemPath(),1));
			sprintf(buildpath,"%s/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",buildpath1);


			if(xml.FindElem(buildpath))
			{

			  found1201 = 1; 
              numattributes = 0;

			  FillInAttributes(xml);


              for(i=0;i<numattributes;i++)
			  {  
	            if(!strcmp(attributes[i],"xlink:href"))
				{  
				  if(!strcmp(attributeV[i],"http://www.opengis.net/def/uom/OGC/1.0/unity"))
				  {
                    found1201a = 1;
				  }
				}
			  } 

			  if(found1201a==0)
			  {
                sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));


				SetUpLineOffsets(xml);


                fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"xlink:href\": <\"http://www.opengis.net/def/uom/OGC/1.0/unity\"> }\n",
	              ParsePath(0));

                PrintAttributesToErrFile();

				numerrs = numerrs + 1;
			  }
			}


			sprintf(buildpath,"%s/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer",buildpath1);

			if(xml.FindElem(buildpath))
			{

              sprintf(tempchar,"%s",GET_CSTR(xml.GetData(),1));
              sscanf(tempchar,"%lf",&tempD);

			  if(tempD>30)
			  {
                printf("Found Number of ICESat Validation Points (121.1) %d\n",(int)tempD);
				foundicesat = 1;
			  }
			  found1211 = 1;
			}


			
			break;
		  }

	      xml.IntoElem();
          xml.OutOfElem();
		 }
         xml.OutOfElem();
		}  
        xml.OutOfElem();
	   }  
       xml.OutOfElem();
	  } 
      xml.OutOfElem();
	 }
     xml.OutOfElem();
	}
    xml.OutOfElem();
   }

   if(found1191==0)
   {
     fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
     fprintf(outerrfile,"  Path    : %s\n",path1191);
     fprintf(outerrfile,"  Spec ID : 119.1\n\n");
	 numerrs = numerrs + 1;
   }
   if(found1201==0)
   {
     fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
     fprintf(outerrfile,"  Path    : %s\n",path1201);
     fprintf(outerrfile,"  Spec ID : 120.1\n\n");
	 numerrs = numerrs + 1;
   }
   if(found1211==0)
   {
     fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
     fprintf(outerrfile,"  Path    : %s\n",path1211);
     fprintf(outerrfile,"  Spec ID : 121.1\n\n");
	 numerrs = numerrs + 1;
   }







   if(foundicesat>0)
   {
     


     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_AbsoluteExternalPositionalAccuracy");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Difference to ICESat LE90");
	 sprintf(attrib1,"xlink:href");
	 sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	 sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	 sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	 numerrs = numerrs +
		 ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.2",(char *)"120.2",(char *)"121.2",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat LE90\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat LE90\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat LE90\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");



     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_AbsoluteExternalPositionalAccuracy");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Difference to ICESat Vertical Mean");
	 sprintf(attrib1,"xlink:href");
	 sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	 sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	 sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	 numerrs = numerrs +
	     ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.3",(char *)"120.3",(char *)"121.3",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical Mean\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical Mean\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical Mean\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");


     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_RelativeInternalPositionalAccuracy");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Difference to ICESat Vertical Standard Deviation");
	 sprintf(attrib1,"xlink:href");
	 sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	 sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	 sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	 numerrs = numerrs +
	     ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.4",(char *)"120.4",(char *)"121.4",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical Standard Deviation\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical Standard Deviation\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical Standard Deviation\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");


	 if(finished_raw==1)
	 {
	   

       sprintf(path1,"gmd:MD_Metadata");
       sprintf(path2,"gmd:dataQualityInfo");
       sprintf(path3,"gmd:DQ_DataQuality");
       sprintf(path4,"gmd:report");
       sprintf(path5,"gmd:DQ_AbsoluteExternalPositionalAccuracy");
       sprintf(path6,"gmd:nameOfMeasure");
       sprintf(path7,"gco:CharacterString");
       sprintf(matchstring,"Difference to ICESat Vertical RMSE");
	   sprintf(attrib1,"xlink:href");
	   sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	   sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	   sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	   numerrs = numerrs +
	       ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.5",(char *)"120.5",(char *)"121.5",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical RMSE\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical RMSE\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to ICESat Vertical RMSE\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");

	 }

   } 





   



   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"north\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"north");

   
   


   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.1",
	   (char *)"123.1",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching North",
	   (char *)"124.1",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the northern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",
	   (char *)"125.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",
       (char *)"126.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.1",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.1",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
    }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.1",
	   (char *)"123.1",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching North",
	   (char *)"124.1",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the northern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",
	   (char *)"125.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",
       (char *)"126.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.1",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.1",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.1",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }


   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"northeast\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"northeast");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.2",
	   (char *)"123.2",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching North East",
	   (char *)"124.2",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the north eastern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",
	   (char *)"125.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",
       (char *)"126.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.2",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.2",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.2",
	   (char *)"123.2",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching North East",
	   (char *)"124.2",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the north eastern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",
	   (char *)"125.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",
       (char *)"126.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.2",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.2",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.2",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }


   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"east\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"east");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.3",
	   (char *)"123.3",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching East",
	   (char *)"124.3",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the eastern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",

       (char *)"126.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.3",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.3",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.3",
	   (char *)"123.3",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching East",
	   (char *)"124.3",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the eastern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",

       (char *)"126.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.3",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.3",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.3",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }



   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"southeast\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"southeast");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.4",
	   (char *)"123.4",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching South East",
	   (char *)"124.4",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the south eastern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",

       (char *)"126.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.4",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.4",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.4",
	   (char *)"123.4",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching South East",
	   (char *)"124.4",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the south eastern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",

       (char *)"126.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.4",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.4",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.4",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }


   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"south\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"south");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.5",
	   (char *)"123.5",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching South",
	   (char *)"124.5",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the southern neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",

       (char *)"126.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.5",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.5",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.5",
	   (char *)"123.5",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching South",
	   (char *)"124.5",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the southern neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",

       (char *)"126.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.5",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.5",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.5",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }




   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"southwest\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"southwest");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.6",
	   (char *)"123.6",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching South West",
	   (char *)"124.6",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the south western neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",

       (char *)"126.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.6",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.6",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.6",
	   (char *)"123.6",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching South West",
	   (char *)"124.6",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the south western neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",

       (char *)"126.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.6",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.6",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.6",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }




   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"west\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"west");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.7",
	   (char *)"123.7",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching West",
	   (char *)"124.7",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the western neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",

       (char *)"126.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.7",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.7",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.7",
	   (char *)"123.7",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching West",
	   (char *)"124.7",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the western neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",

       (char *)"126.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.7",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.7",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.7",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }


   

   sprintf(path1,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]");
   sprintf(path2,"gmd:DQ_TopologicalConsistency");
   sprintf(path3,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:report/gmd:DQ_TopologicalConsistency[@id=\"northwest\"]");
   sprintf(attrib1,"id");
   sprintf(attrib2,"northwest");

   
   

   if(finished_raw==1)
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.8",
	   (char *)"123.8",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching North West",
	   (char *)"124.8",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the north western neighbor. Element gmd:pass is set to 1 if the neighboring cell was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Finished Specification",
	   (char *)"128.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Finished Specification Document",

       (char *)"126.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.8",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.8",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }
   else
   {
     numerrs = numerrs + CheckNeighbor(xml,path1,path2,attrib1,attrib2,path3,(char *)"122.8",
	   (char *)"123.8",(char *)"gmd:nameOfMeasure[1]/gco:CharacterString[1]"     ,(char *)"Edge Matching North West",
	   (char *)"124.8",(char *)"gmd:measureDescription[1]/gco:CharacterString[1]",(char *)"Describes the edge matching result for the north western neighbor. Element gmd:pass is set to 1 if the neighboring tile was available for testing. Element gmd:value indicates the number of non matching posts.",

	   (char *)"125.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]",(char *)"TREx DEM Raw Specification",
	   (char *)"128.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:explanation[1]/gco:CharacterString[1]",(char *)"See chapter 2.10 Edge Matching of TREx DEM Raw Specification Document",

       (char *)"126.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]",
       (char *)"127.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:specification[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]",(char *)"codeList",(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#CI_DateTypeCode",(char *)"codeListValue",(char *)"publication",
	   (char *)"129.8",(char *)"gmd:result[1]/gmd:DQ_ConformanceResult[1]/gmd:pass[1]/gco:Boolean[1]",
	   (char *)"130.8",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",(char *)"xlink:href",(char *)"http://www.opengis.net/def/uom/OGC/1.0/unity",
	   (char *)"131.8",(char *)"gmd:result[2]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer[1]"
	   );
   }







    




   

   if(finished_raw==0)
   {
     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_ConceptualConsistency");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Quality Inspection");
     sprintf(matchstring2,"MCP processor internal quality control result.");
     sprintf(matchstring3,"Quality Inspection Result");
     sprintf(attrib1,"xlink:href");
     sprintf(attrib2,"urn:dgiwg:metadata:dmf:1.0:xmlns:isotc211:gmd:1.0:PT_FreeText_Type");
     sprintf(attrib3,"xlink:href");
     sprintf(attrib4,"http://www.opengis.net/def/uom/OGC/1.0/unity");

	 sprintf(additionalup2path1,"gmd:measureDescription/gco:CharacterString");
	 sprintf(additionalup2path2,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]");
	 sprintf(additionalup2path3,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]");
	 sprintf(additionalup2path4,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]");
	 sprintf(additionalup2path5,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:CharacterString[1]");

	 numerrs = numerrs +
	    ProcessSevenPaths(2,xml,
		           path1,path2,path3,path4,path5,path6,path7,
		           matchstring,matchstring2,matchstring3,
				   attrib1,attrib2,attrib3,attrib4,
			       additionalup2path1,additionalup2path2,additionalup2path3,additionalup2path4,additionalup2path5,
				   (char *)"132.1",(char *)"133.1",(char *)"134.1",(char *)"135.1",(char *)"136.1",(char *)"137.1",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Inspection\"]",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Inspection\"]/../../gmd:measureDescription/gco:CharacterString",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Inspection\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Inspection\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]/@xlink:href",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Inspection\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Inspection\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:CharacterString[1]");




     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_ConceptualConsistency");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Quality Remark");
     sprintf(matchstring2,"MCP processor quality remark.");
     sprintf(matchstring3,"Quality Remark Result");
     sprintf(attrib1,"xlink:href");
     sprintf(attrib2,"urn:dgiwg:metadata:dmf:1.0:xmlns:isotc211:gmd:1.0:PT_FreeText_Type");
     sprintf(attrib3,"xlink:href");
     sprintf(attrib4,"http://www.opengis.net/def/uom/OGC/1.0/unity");

	 sprintf(additionalup2path1,"gmd:measureDescription/gco:CharacterString");
	 sprintf(additionalup2path2,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]");
	 sprintf(additionalup2path3,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]");
	 sprintf(additionalup2path4,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]");
	 sprintf(additionalup2path5,"gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:CharacterString[1]");

	 numerrs = numerrs +
	    ProcessSevenPaths(2,xml,
		           path1,path2,path3,path4,path5,path6,path7,
		           matchstring,matchstring2,matchstring3,
				   attrib1,attrib2,attrib3,attrib4,
			       additionalup2path1,additionalup2path2,additionalup2path3,additionalup2path4,additionalup2path5,
				   (char *)"132.2",(char *)"133.2",(char *)"134.2",(char *)"135.2",(char *)"136.2",(char *)"137.2",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Remark\"]",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Remark\"]/../../gmd:measureDescription/gco:CharacterString",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Remark\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Remark\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueType[1]/gco:RecordType[1]/@xlink:href",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Remark\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]/@xlink:href",
                   (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_ConceptualConsistency/gmd:nameOfMeasure/gco:CharacterString[text()=\"Quality Remark\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:CharacterString[1]");

   }

   return numerrs;

}











int ProcessLongPaths2(CMarkup xml,int finished_raw)
{
   double tempD;
   int i,numerrs=0,foundgcp=0,found1196=0,found1206=0,found1206a=0,found1216=0;
   char buildpath[1000],buildpath1[1000],tempchar[300];
   char path1[200];
   char path2[200];
   char path3[200];
   char path4[200];
   char path5[200];
   char path6[200];
   char path7[200];
   char matchstring[200],attrib1[200],attrib2[200],additionalup2path1[300],additionalup2path2[300];
   char path1196[]="/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"Number of GCP Validation Points\"]";
   char path1206[]="/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"Number of GCP Validation Points\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]";
   char path1216[]="/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_CompletenessCommission/gmd:nameOfMeasure/gco:CharacterString[text()=\"Number of GCP Validation Points\"]/../../gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer";

   xml.ResetPos();


   



   while(xml.FindElem("gmd:MD_Metadata"))
   {
	if(found1196>0){break;}
	xml.IntoElem();

    while(xml.FindElem("gmd:dataQualityInfo"))
	{ 
	 if(found1196>0){break;}
	 xml.IntoElem();

     while(xml.FindElem("gmd:DQ_DataQuality"))
	 {  
	  if(found1196>0){break;}
	  xml.IntoElem();

      while(xml.FindElem("gmd:report"))
	  { 
	   if(found1196>0){break;}
	   xml.IntoElem();

       while(xml.FindElem("gmd:DQ_CompletenessCommission"))
	   {  
	    if(found1196>0){break;}
	    xml.IntoElem();

        while(xml.FindElem("gmd:nameOfMeasure"))
		{  
	     if(found1196>0){break;}
	     xml.IntoElem();

         while(xml.FindElem("gco:CharacterString"))
		 {   
	       if(found1196>0){break;}

		  if(!strcmp(GET_CSTR(xml.GetData(),1),"Number of GCP Validation Points"))
		  {

			found1196 = 1;
			

			
            xml.OutOfElem();
            xml.OutOfElem();

			sprintf(buildpath1,"%s",GET_CSTR(xml.GetElemPath(),1));
			sprintf(buildpath,"%s/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:valueUnit[1]",buildpath1);


			if(xml.FindElem(buildpath))
			{

			  found1206 = 1; 
              numattributes = 0;

			  FillInAttributes(xml);


              for(i=0;i<numattributes;i++)
			  {  
	            if(!strcmp(attributes[i],"xlink:href"))
				{  
				  if(!strcmp(attributeV[i],"http://www.opengis.net/def/uom/OGC/1.0/unity"))
				  {
                    found1206a = 1;
				  }
				}
			  } 

			  if(found1206a==0)
			  {
                sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));


				SetUpLineOffsets(xml);


                fprintf(outerrfile,"Error:  %s\n  Expected {attribute:value} pair {\"xlink:href\": <\"http://www.opengis.net/def/uom/OGC/1.0/unity\"> }\n",
	              ParsePath(0));

                PrintAttributesToErrFile();

				numerrs = numerrs + 1;
			  }
			}


			sprintf(buildpath,"%s/gmd:result[1]/gmd:DQ_QuantitativeResult[1]/gmd:value[1]/gco:Record[1]/gco:Integer",buildpath1);

			if(xml.FindElem(buildpath))
			{

              sprintf(tempchar,"%s",GET_CSTR(xml.GetData(),1));
              sscanf(tempchar,"%lf",&tempD);

			  if(tempD>30)
			  {
                printf("Found Number of GCP Validation Points (121.6) %d\n",(int)tempD);
				foundgcp = 1;
			  }
			  found1216 = 1;
			}


			
			break;
		  }

	      xml.IntoElem();
          xml.OutOfElem();
		 }
         xml.OutOfElem();
		}  
        xml.OutOfElem();
	   }  
       xml.OutOfElem();
	  } 
      xml.OutOfElem();
	 }
     xml.OutOfElem();
	}
    xml.OutOfElem();
   }

   if(found1196==0)
   {
     fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
     fprintf(outerrfile,"  Path    : %s\n",path1196);
     fprintf(outerrfile,"  Spec ID : 119.6\n\n");
	 numerrs = numerrs + 1;
   }
   if(found1206==0)
   {
     fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
     fprintf(outerrfile,"  Path    : %s\n",path1206);
     fprintf(outerrfile,"  Spec ID : 120.1\n\n");
	 numerrs = numerrs + 1;
   }
   if(found1216==0)
   {
     fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
     fprintf(outerrfile,"  Path    : %s\n",path1216);
     fprintf(outerrfile,"  Spec ID : 121.6\n\n");
	 numerrs = numerrs + 1;
   }







   if(foundgcp>0)
   {
     


     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_AbsoluteExternalPositionalAccuracy");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Difference to GCP LE90");
	 sprintf(attrib1,"xlink:href");
	 sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	 sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	 sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	 numerrs = numerrs +
		 ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.7",(char *)"120.7",(char *)"121.7",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP LE90\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP LE90\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP LE90\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");



     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_AbsoluteExternalPositionalAccuracy");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Difference to GCP Vertical Mean");
	 sprintf(attrib1,"xlink:href");
	 sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	 sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	 sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	 numerrs = numerrs +
	     ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.8",(char *)"120.8",(char *)"121.8",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical Mean\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical Mean\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical Mean\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");


     sprintf(path1,"gmd:MD_Metadata");
     sprintf(path2,"gmd:dataQualityInfo");
     sprintf(path3,"gmd:DQ_DataQuality");
     sprintf(path4,"gmd:report");
     sprintf(path5,"gmd:DQ_RelativeInternalPositionalAccuracy");
     sprintf(path6,"gmd:nameOfMeasure");
     sprintf(path7,"gco:CharacterString");
     sprintf(matchstring,"Difference to GCP Vertical Standard Deviation");
	 sprintf(attrib1,"xlink:href");
	 sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	 sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	 sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	 numerrs = numerrs +
	     ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.9",(char *)"120.9",(char *)"121.9",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical Standard Deviation\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical Standard Deviation\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_RelativeInternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical Standard Deviation\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");


	 if(finished_raw==1)
	 {
	   

       sprintf(path1,"gmd:MD_Metadata");
       sprintf(path2,"gmd:dataQualityInfo");
       sprintf(path3,"gmd:DQ_DataQuality");
       sprintf(path4,"gmd:report");
       sprintf(path5,"gmd:DQ_AbsoluteExternalPositionalAccuracy");
       sprintf(path6,"gmd:nameOfMeasure");
       sprintf(path7,"gco:CharacterString");
       sprintf(matchstring,"Difference to GCP Vertical RMSE");
	   sprintf(attrib1,"xlink:href");
	   sprintf(attrib2,"http://www.opengis.net/def/uom/OGC/1.0/metre");
	   sprintf(additionalup2path1,"gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit");
	   sprintf(additionalup2path2,"gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real");


	   numerrs = numerrs +
	       ProcessSevenPaths(1,xml,
		               path1,path2,path3,path4,path5,path6,path7,
					   matchstring,(char *)"unused",(char *)"unused",
					   attrib1,attrib2,(char *)"unused",(char *)"unused",
					   additionalup2path1,additionalup2path2,(char *)"unused",(char *)"unused",(char *)"unused",
					   (char *)"119.10",(char *)"120.10",(char *)"121.10",(char *)"unused",(char *)"unused",(char *)"unused",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical RMSE\"]",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical RMSE\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:valueUnit/@xlink:href",
                       (char *)"/gmd:MD_Metadata/gmd:dataQualityInfo/gmd:DQ_DataQuality/gmd:report/gmd:DQ_AbsoluteExternalPositionalAccuracy/gmd:nameOfMeasure/gco:CharacterString[text()=\"Difference to GCP Vertical RMSE\"]/../../gmd:result/gmd:DQ_QuantitativeResult/gmd:value/gco:Record/gco:Real",
					   (char *)"unused",(char *)"unused",(char *)"unused");

	 }

   } 


   return numerrs;

}



int IsNormalPath(CMarkup xml,char *testpath)
{

  if(strstr(testpath,".."))  
  {
    return 0;
  }

  if(strstr(testpath,"\""))  
  {
    return 0;
  }

  return 1;

}



int ProcessNewTrex(CMarkup xml, char *rootname, char *t_domain,char *t_codelistval,char *t_id,double northing,double easting,int finished_raw)
{
  int i,totalerrs=0;


  SetUpLineOffsets(xml);

  sprintf(csTag,"%s",GET_CSTR(xml.GetTagName(),1));

  sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

  
  sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));


  FillInAttributes(xml);






  totalerrs = ProcessTrexItem(theRealpath,GetCleanToken(csTag),rootname,t_domain,t_codelistval,t_id,northing,easting,finished_raw);

  for(i=0;i<10;i++)
  {
    attributes[i][0] = '\0';
  }

  return totalerrs;

}



int Compare1481(CMarkup xml,char *t_id,char *t_path,int finished_raw,char *targetval)
{
  int foundit=0,foundit2=0;
  char buildpath[1000];


  if(finished_raw==1)
  {
    

    while(xml.FindElem("gmd:MD_Metadata"))
	{ 
	  if(foundit>0){break;}
	  xml.IntoElem();

      while(xml.FindElem("gmd:dataQualityInfo"))
	  { 
	    if(foundit>0){break;}
	    xml.IntoElem();

        while(xml.FindElem("gmd:DQ_DataQuality"))
		{   
	      if(foundit>0){break;}
	      xml.IntoElem();

          while(xml.FindElem("gmd:report"))
		  {  
	        if(foundit>0){break;}
	        xml.IntoElem();

            while(xml.FindElem("gmd:DQ_CompletenessCommission"))
			{   
	          if(foundit>0){break;}
	          xml.IntoElem();

              while(xml.FindElem("gmd:nameOfMeasure"))
			  {   
	            if(foundit>0){break;}
	            xml.IntoElem();

                while(xml.FindElem("gco:CharacterString"))
				{    
	              if(foundit>0){break;}

		          if(!strcmp(GET_CSTR(xml.GetData(),1),"Number of ICESat Validation Points"))
				  {  
			        foundit  = 1;
			        foundit2 = 0;

			        
                    xml.OutOfElem();
                    xml.OutOfElem();

			        sprintf(buildpath,"%s/gmd:measureDescription/gco:CharacterString",GET_CSTR(xml.GetElemPath(),1));

			        if(xml.FindElem(buildpath))
					{ 
					  if(!strcmp(GET_CSTR(xml.GetData(),1),targetval))
					  {
						
                        xml.ResetPos();
						return 0;
					  }

					  
                      Trex_err(t_id,t_path,targetval,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
                      return 1;
					}  
                    break;
				  } 

	              xml.IntoElem();
                  xml.OutOfElem();
				}
                xml.OutOfElem();
			  }  
              xml.OutOfElem();
			}  
            xml.OutOfElem();
		  } 
          xml.OutOfElem();
		}
        xml.OutOfElem();
	  }
      xml.OutOfElem();
	} 

  } 


  fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
  fprintf(outerrfile,"  Path    : %s\n",t_path);
  fprintf(outerrfile,"  Spec ID : %s\n\n",t_id);
  return 1;


  xml.ResetPos();
}




int Compare1486(CMarkup xml,char *t_id,char *t_path,int finished_raw,char *targetval)
{
  int foundit=0,foundit2=0;
  char buildpath[1000];


  if(finished_raw==1)
  {
    

    while(xml.FindElem("gmd:MD_Metadata"))
	{ 
	  if(foundit>0){break;}
	  xml.IntoElem();

      while(xml.FindElem("gmd:dataQualityInfo"))
	  { 
	    if(foundit>0){break;}
	    xml.IntoElem();

        while(xml.FindElem("gmd:DQ_DataQuality"))
		{   
	      if(foundit>0){break;}
	      xml.IntoElem();

          while(xml.FindElem("gmd:report"))
		  {  
	        if(foundit>0){break;}
	        xml.IntoElem();

            while(xml.FindElem("gmd:DQ_CompletenessCommission"))
			{   
	          if(foundit>0){break;}
	          xml.IntoElem();

              while(xml.FindElem("gmd:nameOfMeasure"))
			  {   
	            if(foundit>0){break;}
	            xml.IntoElem();

                while(xml.FindElem("gco:CharacterString"))
				{    
	              if(foundit>0){break;}

		          if(!strcmp(GET_CSTR(xml.GetData(),1),"Number of GCP Validation Points"))
				  {  
			        foundit  = 1;
			        foundit2 = 0;

			        
                    xml.OutOfElem();
                    xml.OutOfElem();

			        sprintf(buildpath,"%s/gmd:measureDescription/gco:CharacterString",GET_CSTR(xml.GetElemPath(),1));

			        if(xml.FindElem(buildpath))
					{ 

					  if(!strcmp(GET_CSTR(xml.GetData(),1),targetval))
					  {
						
                        xml.ResetPos();
						return 0;
					  }

					  
                      Trex_err(t_id,t_path,targetval,GET_CSTR(xml.GetData(),1),GetLineNumber(),1);
                      return 1;
					}  
                    break;
				  } 

	              xml.IntoElem();
                  xml.OutOfElem();
				}
                xml.OutOfElem();
			  }  
              xml.OutOfElem();
			}  
            xml.OutOfElem();
		  } 
          xml.OutOfElem();
		}
        xml.OutOfElem();
	  }
      xml.OutOfElem();
	} 

  } 


  fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
  fprintf(outerrfile,"  Path    : %s\n",t_path);
  fprintf(outerrfile,"  Spec ID : %s\n\n",t_id);
  return 1;


  xml.ResetPos();
}









extern "C" int SetClassLevel(char columnname[],char Incolumnvalue[],char filename[],char tablename[]);

void AssessClassification(CMarkup xml,char *filename)
{
  int i,j;
  char *classpath=(char *)"/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[1]/gmd:MD_SecurityConstraints[1]/gmd:classification[1]/gmd:MD_ClassificationCode[1]";
  char *classpathvalue=(char *)"./RESOURCES/19139/20070417/resources/Codelist/gmxCodelists.xml#MD_ClassificationCode";


  

  if(xml.FindElem(classpath))
  {
	

    SetUpLineOffsets(xml);

    sprintf(csTag,"%s",GET_CSTR(xml.GetTagName(),1));

    sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

    
    sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));

    FillInAttributes(xml);


	


    for(i=0;i<numattributes;i++)
	{ 
       if(  
	       (!strcmp(attributes[i],"codeList")) &&
		   (!strcmp(attributeV[i],classpathvalue))
	      )
	   { 
         for(j=i+1;j<numattributes;j++)
		 {  
           if(
		       (!strcmp(attributes[j],"codeListValue")) &&
		       (!strcmp(attributeV[j],"unclassified"))
		     )
		   {
             SetClassLevel((char *)"TREX",(char *)"unclassified",filename,NULL);
		   }
           else if(
		       (!strcmp(attributes[j],"codeListValue")) &&
		       (!strcmp(attributeV[j],"restricted"))
		     )
		   {   
             SetClassLevel((char *)"TREX",(char *)"restricted",filename,NULL);
		   }
           else if(
		       (!strcmp(attributes[j],"codeListValue")) &&
		       (!strcmp(attributeV[j],"confidential"))
		     )
		   {   
             SetClassLevel((char *)"TREX",(char *)"confidential",filename,NULL);
		   }
           else if(
		       (!strcmp(attributes[j],"codeListValue")) &&
		       (!strcmp(attributeV[j],"secret"))
		     )
		   {   
             SetClassLevel((char *)"TREX",(char *)"secret",filename,NULL);
		   }
           else if(
		       (!strcmp(attributes[j],"codeListValue")) &&
		       (!strcmp(attributeV[j],"topSecret"))
		     )
		   {   
             SetClassLevel((char *)"TREX",(char *)"topSecret",filename,NULL);
		   }
		 }
	   }
	}


    for(i=0;i<10;i++)
	{ 
      attributes[i][0] = '\0';
	} 
  }


}


int FindReplacePos(char *thestr,char *replacestring,int verbose)
{
  char *secondpart;
  int totallen,secondlen;

  

  secondpart = strstr(thestr,replacestring);

  if(secondpart==NULL)
  {
	if(verbose==1)
	{
	  printf("Error: couldn't locate %s in %s\n",replacestring,thestr);
	}
	return -1;
  }

  totallen  = strlen(thestr);
  secondlen = strlen(secondpart);

  return totallen-secondlen+1;

}


void InitVars(CMarkup xml)
{

  SetUpLineOffsets(xml);

  sprintf(csTag,"%s",GET_CSTR(xml.GetTagName(),1));

  sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

  
  sprintf(theRealpath,"%s",GET_CSTR(xml.GetElemPath(),1));

  FillInAttributes(xml);

}


int ProcessNTypeSpecs(CMarkup xml)
{
  int i,j,k,replace1index,replace2index;
  int found138,found139,found140,found141,found142,found143,found144,found145,
	  numlong,num651,foundthisone,badstuff=0,isvalid=0;
  int goodsequence[MAX_LINEAGE];
  char seekpath[1000],temppath[1000];
  char *N138[MAX_LINEAGE];
  char *N139[MAX_LINEAGE];
  char *N140[MAX_LINEAGE];
  char *N141[MAX_LINEAGE]; 
  char *N142[MAX_LINEAGE];
  char *N143[MAX_LINEAGE];
  char *N144[MAX_LINEAGE];
  char *N145[MAX_LINEAGE]; 
  char *N651[MAX_LINEAGE];
  char n138path[1000];
  char n139path[1000];
  char n140path[1000];
  char n141path[1000];
  char n142path[1000];
  char n143path[1000];
  char n144path[1000];
  char n145path[1000];
  char n651path[1000];


  


  

#define NUM_SPECS 13

char Spec138[NUM_SPECS][500]=
{
"The Norsk Polarinstitutt DEM refers to a collection of the following datasets: Svalbard Digital Elevation Model (NP_S0)/Jan Mayen Digital Elevation Model (NP_J0)/Bear Island Digital Elevation Model (NP_B0)/Peter I Island Digital Elevation Model (NP_P50).",

"Active Earthscape is a reflective surface model derived from EO sources at 0.4 arc seconds sampling distance.",
"The SRT2f data is a void filled reflective surface model at 1 arc second post spacing derived from the interferometric c-band SAR data of the Shuttle Radar Topography Mission flown in 2000.",
"The ASTER GDEM v2 is a reflective surface model at 1 arc second sample spacing derived from data of ASTER (Advanced Spaceborne Thermal Emission and Reflection Radiometer) on board the Terra satellite launched into Earth orbit by NASA in 1999.",
"DTED1 is a coarse elevation dataset at 3 arc seconds sampling distance derived from various sources.",
"The Global Multi-resolution Terrain Elevation Data 2010 (GMTED2010) is a low resolution global elevation dataset compiled from various sources.",
"The ALOS World 3D 30m dataset released by the Japan Aerospace Exploration Agency (JAXA) is a digital surface model compiled with images acquired by the Advanced Land Observing Satellite \"DAICHI\" (ALOS).",
"The Polar Geospatial Center digital surface models are derived from optical stereo coverage of Digital Globe Inc. Worldview-1 and -2 satellites.",
"The Canadian Digital Elevation Model is a national dataset at 0.75 to 3 arc seconds ground sampling distance. It was compiled from various sources and represents ground as well as reflective surface elevations.",
"The Geografiska Sverigedata Elevation data grid is a SWE national dataset derived from airborne laser scanning. It is sampled to a grid of 10m post spacing specifically for the use as fill source within the TREx Program. The dataset represents the bare earth surface as derived by automated point classification.",
"The MGCP Nordic DEM is an elevation data set in DTED2 format provided by the Norwegian Military Geographic Service (FMGT).",
"The Norway Digital Elevation Model 10m is a NOR national digital terrain model. It is derived from map data and sampled at 10m grid spacing.",
"The Finland elevation model 10m depicts the elevation of the ground surface of the whole Finland. It is part of the open data of the National Land Survey of Finland and has in large parts been produced of contour lines."
};

char Spec139[NUM_SPECS][20]=
{
"NPDEM",

"AE",
"SRT2f",
"ASTGTM2",
"DTED1",
"GMTED",
"AW3D30",
"PGC",
"CDEM",
"GSDE",
"MGCPNDEM",
"NORDEM10",
"FINDEM10"
};
char Spec140[NUM_SPECS][20]=
{
"2014-12-04",

"2013-02-06",
"2010-02-25",
"2011-10-16",
"2000-05-19",
"2011-10-07",
"2015-05-18",
"2013-10-01",
"2012-11-06",
"2016-02-28",
"2007-09-01",
"2008-09-01",
"2010-04-01"
};
char Spec141[NUM_SPECS][20]=
{
"publication",

"publication",
"revision",
"publication",
"publication",
"publication",
"publication",
"publication",
"publication",
"publication",
"creation",
"creation",
"revision"
};
char Spec142[NUM_SPECS][20]=
{
"***",

"1",
"4.0",
"2",
"1",
"1",
"1",
"Beta",
"3.0",
"1",
"1",
"***",
"***"
};
char Spec143[NUM_SPECS][200]=
{
"Norsk Polarinstitutt",

"National Geospatial Intelligence Agency",
"National Geospatial Intelligence Agency",
"Ministry of Economy, Trade, and Industry (METI) of Japan and the National Aeronautics and Space Administration (NASA) of the United States of America",
"National Geospatial Intelligence Agency",
"U.S. Geological Survey",
"Japan Aerospace Exploration Agency",
"Polar Geospatial Center",
"Natural Resources Canada",
"Lantm~~teriet",
"Norwegian Military Geographic Service (FMGT)",
"Kartverket",
"National Land Survey of Finland"
};
char Spec144[NUM_SPECS][200]=
{
"https://data.npolar.no/geodata#basemap-data",

"https://www.nga.mil",
"https://www.nga.mil",
"http://asterweb.jpl.nasa.gov/gdem.asp",
"https://www.nga.mil",
"https://lta.cr.usgs.gov/GMTED2010",
"http://www.eorc.jaxa.jp/ALOS/en/aw3d30/",
"http://www.pgc.umn.edu/elevation/stereo",
"http://geogratis.gc.ca/api/en/nrcan-rncan/ess-sst/C40ACFBA-C722-4BE1-862E-146B80BE738E.html",
"https://www.lantmateriet.se/en/Maps-and-geographic-information/Elevation-data-",
"***",
"http://data.kartverket.no/download/content/geodataprodukter?korttype=3595&aktualitet=All&datastruktur=All&dataskema=All",
"http://www.maanmittauslaitos.fi/en/digituotteet/elevation-model-10-m"
};
char Spec145[NUM_SPECS][20]=
{
"publisher",

"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher",
"publisher"
};
char Spec651[NUM_SPECS][500]=
{
"Includes Svalbard Digital Elevation Model (NP_S0)/Jan Mayen Digital Elevation Model (NP_J0)/Bear Island Digital Elevation Model (NP_B0)/Peter I Island Digital Elevation Model (NP_P50) !! Norsk Polarinstitutt. Available under Creative Commons Attribution 4.0 International (CC BY 4.0) (http://creativecommons.org/licenses/by/4.0/legalcode)",

"Includes material !! COPYRIGHT 2013 BY THE UNITED STATES GOVERNMENT. NO COPYRIGHT CLAIMED UNDER TITLE 17 U.S.C.",
"Includes material !! COPYRIGHT 2010 BY THE UNITED STATES GOVERNMENT. NO COPYRIGHT CLAIMED UNDER TITLE 17 U.S.C.",
"Includes ASTER GDEM v2 data. ASTER GDEM is a product of METI and NASA.",
"Includes material !! COPYRIGHT 2000 BY THE UNITED STATES GOVERNMENT. NO COPYRIGHT CLAIMED UNDER TITLE 17 U.S.C.",
"Includes GMTED2010 available from the U.S. Geological Survey: Danielson, J.J., and Gesch, D.B., 2011, Global multi-resolution terrain elevation data 2010 (GMTED2010): U.S. Geological Survey Open-File Report 2011-1073, 26 p. http://pubs.usgs.gov/of/2011/1073/",
"Includes AW3D30 !! Japan Aerospace Exploration Agency.",
"Includes data !! Polar Geospatial Center: I. Howat, The Ohio State Univeristy.",
"Includes CDEM !! Department of Natural Resources Canada. All rights reserved. Available under the Open Government Licence - Canada (http://open.canada.ca/en/open-government-licence-canada)",
"Includes Geografiska Sverigedata elevation data, !! Lantm~~teriet.",
"Includes MGCP Nordic DEM, !! FMGT. All rights reserved.",
"Includes Norway Digital Elevation Model 10m, !! Kartverket. Available under Creative Commons Attribution 4.0 International (CC BY 4.0) (http://creativecommons.org/licenses/by/4.0/legalcode)",
"Includes data from the National Land Survey of Finland Topographic Database 04/2010. Available under Creative Commons Attribution 4.0 International (CC BY 4.0) (http://creativecommons.org/licenses/by/4.0/legalcode)"
};


  

  for(i=0;i<NUM_SPECS;i++)
  {
    replace1index = FindReplacePos(Spec138[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec138[i][replace1index]   = (unsigned char)0xC2;
      Spec138[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec138[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec138[i][replace1index]   = (unsigned char)0xC3;
      Spec138[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec139[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec139[i][replace1index]   = (unsigned char)0xC2;
      Spec139[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec139[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec139[i][replace1index]   = (unsigned char)0xC3;
      Spec139[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec140[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec140[i][replace1index]   = (unsigned char)0xC2;
      Spec140[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec140[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec140[i][replace1index]   = (unsigned char)0xC3;
      Spec140[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec141[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec141[i][replace1index]   = (unsigned char)0xC2;
      Spec141[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec141[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec141[i][replace1index]   = (unsigned char)0xC3;
      Spec141[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec142[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec142[i][replace1index]   = (unsigned char)0xC2;
      Spec142[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec142[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec142[i][replace1index]   = (unsigned char)0xC3;
      Spec142[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec143[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec143[i][replace1index]   = (unsigned char)0xC2;
      Spec143[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec143[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec143[i][replace1index]   = (unsigned char)0xC3;
      Spec143[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec144[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec144[i][replace1index]   = (unsigned char)0xC2;
      Spec144[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec144[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec144[i][replace1index]   = (unsigned char)0xC3;
      Spec144[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec145[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec145[i][replace1index]   = (unsigned char)0xC2;
      Spec145[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec145[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec145[i][replace1index]   = (unsigned char)0xC3;
      Spec145[i][replace1index+1] = (unsigned char)0xA4;
	}

    replace1index = FindReplacePos(Spec651[i],(char *)"!!",0) - 1;
    if(replace1index>=0)
	{ 
      Spec651[i][replace1index]   = (unsigned char)0xC2;
      Spec651[i][replace1index+1] = (unsigned char)0xA9;
	}
    replace1index = FindReplacePos(Spec651[i],(char *)"~~",0) - 1;
    if(replace1index>=0)
	{ 
      Spec651[i][replace1index]   = (unsigned char)0xC3;
      Spec651[i][replace1index+1] = (unsigned char)0xA4;
	}
  }




  
  sprintf(n138path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:description[1]/gco:CharacterString[1]");
  sprintf(n139path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:title[1]/gco:CharacterString[1]");
  sprintf(n140path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:date[1]/gco:Date[1]");
  sprintf(n141path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:date[1]/gmd:CI_Date[1]/gmd:dateType[1]/gmd:CI_DateTypeCode[1]");
  sprintf(n142path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:edition[1]/gco:CharacterString[1]");
  sprintf(n143path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:organisationName[1]/gco:CharacterString[1]");
  sprintf(n144path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:contactInfo[1]/gmd:CI_Contact[1]/gmd:onlineResource[1]/gmd:CI_OnlineResource[1]/gmd:linkage[1]/gmd:URL[1]");
  sprintf(n145path,"/gmd:MD_Metadata[1]/gmd:dataQualityInfo[1]/gmd:DQ_DataQuality[1]/gmd:lineage[1]/gmd:LI_Lineage[1]/gmd:source[N]/gmd:LI_Source[1]/gmd:sourceCitation[1]/gmd:CI_Citation[1]/gmd:citedResponsibleParty[1]/gmd:CI_ResponsibleParty[1]/gmd:role[1]/gmd:CI_RoleCode[1]");
  sprintf(n651path,"/gmd:MD_Metadata[1]/gmd:identificationInfo[1]/gmd:MD_DataIdentification[1]/gmd:resourceConstraints[2]/gmd:MD_LegalConstraints[1]/gmd:otherConstraints[N]/gco:CharacterString[1]");

  replace1index = FindReplacePos(n138path,(char *)"[N]",1);
  if(replace1index<=0)
  {
	return 0;
  }

  replace2index = FindReplacePos(n651path,(char *)"[N]",1);
  if(replace2index<=0)
  {
	return 0;
  }

  if(replace1index<=0)
  {
	return 0;
  }

  for(i=0;i<MAX_LINEAGE;i++)
  {
    N138[i] = NULL;
    N139[i] = NULL;
    N140[i] = NULL;
    N141[i] = NULL;
    N142[i] = NULL;
    N143[i] = NULL;
    N144[i] = NULL;
    N145[i] = NULL;
    N651[i] = NULL;
	goodsequence[i]   = -1;
  }

  numlong = 0;
  num651  = 0;

  for(i=1;i<=MAX_LINEAGE;i++)
  {
    found138 = 0;
    found139 = 0;
	found140 = 0;
	found141 = 0;
	found142 = 0;
	found143 = 0;
	found144 = 0;
	found145 = 0;

	

    xml.ResetPos();
	sprintf(temppath,"%s",n138path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n138path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found138 = 1;
	  InitVars(xml);
	  N138[i-1] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N138[i-1],"%s",thedata);
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n139path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n139path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found139 = 1;
	  InitVars(xml);
	  N139[i-1] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N139[i-1],"%s",thedata);
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n140path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n140path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found140 = 1;
	  InitVars(xml);
	  N140[i-1] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N140[i-1],"%s",thedata);
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n141path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n141path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found141 = 1;
	  InitVars(xml);


      for(j=0;j<numattributes;j++)
	  {  
		
        if(  
	 	   (!strcmp(attributes[j],"codeList")) &&
		   (!strcmp(attributeV[j],"./RESOURCES/19139/20070417/resources/codelist/gmxCodelists.xml#CI_DateTypeCode"))
	      )
		{ 
          for(k=j+1;k<numattributes;k++)
		  { 
            if(!strcmp(attributes[k],"codeListValue"))
			{  
              N141[i-1] = (char *)malloc(strlen(attributeV[k])+10);
	          sprintf(N141[i-1],"%s",attributeV[k]);
	          break;
			}
		  }
		}
	  }
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n142path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n142path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found142 = 1;
	  InitVars(xml);
	  N142[i-1] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N142[i-1],"%s",thedata);
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n143path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n143path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found143 = 1;
	  InitVars(xml);
	  N143[i-1] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N143[i-1],"%s",thedata);
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n144path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n144path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found144 = 1;
	  InitVars(xml);
	  N144[i-1] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N144[i-1],"%s",thedata);
	}

	

    xml.ResetPos();
	sprintf(temppath,"%s",n145path);
	temppath[replace1index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n145path[replace1index+1]);

    if(xml.FindElem(seekpath))
	{
	  found145 = 1;
	  InitVars(xml);

      for(j=0;j<numattributes;j++)
	  {  
		
        if(  
	 	   (!strcmp(attributes[j],"codeList")) &&
		   (!strcmp(attributeV[j],"./RESOURCES/19139/20070417/resources/codelist/gmxCodelists.xml#CI_RoleCode"))
	      )
		{ 
          for(k=j+1;k<numattributes;k++)
		  { 
            if(!strcmp(attributes[k],"codeListValue"))
			{  
              N145[i-1] = (char *)malloc(strlen(attributeV[k])+10);
	          sprintf(N145[i-1],"%s",attributeV[k]);
	          break;
			}
		  }
		}
	  }
	}


	if((found138+found139+found140+found141+found142+found143+found144+found145)==0)
	{
	  numlong = i-1;
	  break;
	}
  }


  for(i=3;i<=MAX_LINEAGE+2;i++)
  {
	

    xml.ResetPos();
	sprintf(temppath,"%s",n651path);
	temppath[replace2index] = '\0';
	sprintf(seekpath,"%s%d%s",temppath,i,&n651path[replace2index+1]);

    if(xml.FindElem(seekpath))
	{
	  InitVars(xml);
	  N651[i-3] = (char *)malloc(strlen(thedata)+10);
	  sprintf(N651[i-3],"%s",thedata);

      
	  isvalid = 0;
      for(j=0;j<NUM_SPECS;j++)
	  {  
	    if(!strcmp(N651[i-3],Spec651[j])) 
		{ 
	 	 isvalid = 1;
		 break;
		} 
	  }

	  if(isvalid==0)
	  { 
	    SetUpLineOffsets(xml);
        fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
        fprintf(outerrfile,"  Path    : %s\n",n651path);
        fprintf(outerrfile,"  Found   : Invalid lineage 65.1.N (otherConstraints)\n");
        fprintf(outerrfile,"   otherConstraints: \"%s\"\n",N651[i-3]);
        fprintf(outerrfile,"  Expected: Valid 65.1.N from Annex D\n\n");
	  }





	}
	else
	{
	  num651 = i-3;
	  break;
	}
  }





  
  for(i=0;i<numlong;i++)
  {
	

	foundthisone = 0;

	for(j=0;j<NUM_SPECS;j++)
	{
	  if(!strcmp(N138[i],Spec138[j]))
	  {
		
		

		foundthisone = 1;

		if( (strcmp(N139[i],Spec139[j]))  && (strcmp(Spec139[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n139path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n139path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N139[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec139[j]);
		  badstuff = badstuff + 1;
		  break;
		}
		if( (strcmp(N140[i],Spec140[j]))  && (strcmp(Spec140[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n140path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n140path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N140[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec140[j]);
		  badstuff = badstuff + 1;
		  break;
		}
		if( (strcmp(N141[i],Spec141[j]))  && (strcmp(Spec141[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n141path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n141path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N141[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec141[j]);
		  badstuff = badstuff + 1;
		  break;
		}
		if( (strcmp(N142[i],Spec142[j]))  && (strcmp(Spec142[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n142path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n142path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N142[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec142[j]);
		  badstuff = badstuff + 1;
		  break;
		}
		if( (strcmp(N143[i],Spec143[j]))  && (strcmp(Spec143[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n143path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n143path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N143[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec143[j]);
		  badstuff = badstuff + 1;
		  break;
		}
		if( (strcmp(N144[i],Spec144[j]))  && (strcmp(Spec144[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n144path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n144path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N144[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec144[j]);
		  badstuff = badstuff + 1;
		  break;
		}
		if( (strcmp(N145[i],Spec145[j]))  && (strcmp(Spec145[j],"***")) )
		{
          xml.ResetPos();
	      sprintf(temppath,"%s",n145path);
	      temppath[replace1index] = '\0';
	      sprintf(seekpath,"%s%d%s",temppath,i+1,&n145path[replace1index+1]);

          if(xml.FindElem(seekpath))
		  {
		    SetUpLineOffsets(xml);
            fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
		  }
		  else
		  {
            fprintf(outerrfile,"Error     :\n");
		  }
          fprintf(outerrfile,"  Path    : %s\n",seekpath);
          fprintf(outerrfile,"  Found   : \"%s\"\n",N145[i]);
          fprintf(outerrfile,"  Expected: \"%s\"\n\n",Spec145[j]);
		  badstuff = badstuff + 1;
		  break;
		}

		
		goodsequence[i] = j;
	  } 
	} 


	if(foundthisone==0)
	{
	  

      xml.ResetPos();
      sprintf(temppath,"%s",n138path);
	  temppath[replace1index] = '\0';
	  sprintf(seekpath,"%s%d%s",temppath,i+1,&n138path[replace1index+1]);

      if(xml.FindElem(seekpath))
	  {
		SetUpLineOffsets(xml);
        fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
	  }
	  else
	  {
        fprintf(outerrfile,"Error     :\n");
	  }
      fprintf(outerrfile,"  Path    : %s\n",seekpath);
      fprintf(outerrfile,"  Found   : \"%s\"\n",N138[i]);
      fprintf(outerrfile,"  Expected: Valid 138.N entry from Annex D\n\n");
	  badstuff = badstuff + 1;
	}


  } 







  
  for(i=0;i<numlong;i++)
  {
    foundthisone = 0;
	if(goodsequence[i]>=0)
	{
      

	  for(j=0;j<num651;j++)
	  {
		if(!strcmp(N651[j],Spec651[goodsequence[i]]))
		{
		  foundthisone = 1;
		  break;
		}
	  }

      if(foundthisone==0)
	  {
        fprintf(outerrfile,"Error     :\n");
        fprintf(outerrfile,"  Path    : %s\n",n651path);
        fprintf(outerrfile,"  Found   : Valid lineage sequence 138-145.N, but did not find matching 65.1.N (otherConstraints)\n");
        fprintf(outerrfile,"  Expected: \"%s\"\n",Spec651[goodsequence[i]]);
	    fprintf(outerrfile,"   List of 65.1.N (otherConstraints) found:\n");
		for(j=0;j<num651;j++)
		{
          fprintf(outerrfile,"            \"%s\"\n",N651[j]);
		}
	    badstuff = badstuff + 1;
	  }

	}
  }






  

  for(i=0;i<num651;i++)
  {
    foundthisone = 0;

	
	isvalid = 0;
    for(j=0;j<NUM_SPECS;j++)
	{ 

      if(!strcmp(N651[i],Spec651[j])) 
	  { 
		isvalid = 1;
		break;
	  }
	}

	if(isvalid==0)
	{
	  continue;
	}



	for(j=0;j<numlong;j++)
	{ 
	  if(goodsequence[j]>=0)
	  {
		if(!strcmp(N651[i],Spec651[goodsequence[j]])) 
		{
		  foundthisone = 1;
		  break;
		}
	  }
	}


    if(foundthisone==0)
	{

      xml.ResetPos();
	  sprintf(temppath,"%s",n651path);
	  temppath[replace2index] = '\0';
	  sprintf(seekpath,"%s%d%s",temppath,i+3,&n651path[replace2index+1]); 


      if(xml.FindElem(seekpath))
	  { 
		SetUpLineOffsets(xml);
        fprintf(outerrfile,"Error     : Line %s:\n",GetLineNumber());
        fprintf(outerrfile,"  Path    : %s\n",seekpath);
	  }
	  else
	  {
        fprintf(outerrfile,"Error     :\n");
        fprintf(outerrfile,"  Path    : %s\n",seekpath);
	  }

      fprintf(outerrfile,"  Found   : Valid lineage 65.1.N (otherConstraints), but did not find matching valid 138.N-145.N\n");
      fprintf(outerrfile,"   otherConstraints: \"%s\"\n",N651[i]);
      fprintf(outerrfile,"    At least one of the expected elements 138.N-145.N corresponding to this otherConstraints must be incorrect or missing\n\n");

	  badstuff = badstuff + 1;
	}
  }



  for(i=0;i<MAX_LINEAGE;i++)
  {
    if(N138[i]!=NULL){free(N138[i]);}
    if(N139[i]!=NULL){free(N139[i]);}
    if(N140[i]!=NULL){free(N140[i]);}
    if(N141[i]!=NULL){free(N141[i]);}
    if(N142[i]!=NULL){free(N142[i]);}
    if(N143[i]!=NULL){free(N143[i]);}
    if(N144[i]!=NULL){free(N144[i]);}
    if(N145[i]!=NULL){free(N145[i]);}
    if(N651[i]!=NULL){free(N651[i]);}
  }

  return badstuff;
}




extern "C" int ParseXML3(char *filename, char *outfilename, int *errors, char *rootname, int finished_raw, double northing,double easting)
{
  CMarkup xml;
  int i,len,pathtype,skipper=0,totalerrs=0;
  long int thistime=time(NULL);

  char t_id    [1000]; 
  char t_name  [1000]; 
  char t_defn  [1000]; 
  char t_dtype [1000]; 
  char t_domain[1000]; 
  char t_oblig [1000]; 
  char t_path  [1000]; 
 
  char t_codelistval[1000]; 

  extern int ReFormatTrexXML;
  extern int FileExists(char *filename);
  extern char * IGetNth(char aline[], int num, int silent);


  trex_type = 1;

  outerrfile = fopen(outfilename,"w");
  if(outerrfile==NULL)
  {
	printf("unable to open %s for write\n",outfilename);
	return 0;
  }

  if(FileExists(filename)==0)
  {
    fprintf(outerrfile,"Unable to validate TREx metadata.  Could not open file:\n  %s\n\n\n",filename);
	fclose(outerrfile);
    return 1;
  }

  fprintf(outerrfile,"Validation results for TREx metadata file:\n  %s\n\n",filename);
  fprintf(outerrfile,"Report Date: %s\n\n",ctime(&thistime));
  fprintf(outerrfile,"Errors found:\n\n");


  csTag                = (char *)malloc(1005);
  thedata              = (char *)malloc(1005);
  tempstring           = (char *)malloc(1005);
  local_tag_max        = 1000;
  local_thedata_max    = 1000;
  local_tempstring_max = 1000;


  len = strlen(outfilename);

  xml.Load(filename);

  SetUpLineNumbers(filename);

  if(ReFormatTrexXML==1)
  {
    char testname[1000];
    extern char importoutputdir      [1000];


    


    #if(USE_DOS==1)

    FILE *outPPfile;
    
      
    sprintf(testname,"%sinternal_data\\rf%d.bin",importoutputdir,finished_raw); 

    outPPfile = fopen(testname,"w");
      
    
      
    fprintf(outPPfile,"%s",xml.GetDocFormatted(4).c_str()     );
  
    fclose(outPPfile);
       

    #else

    FILE *outPPfile;
    std::string thestr;

	

    sprintf(testname,"%sinternal_data/rf%d.bin",importoutputdir,finished_raw); 

    outPPfile = fopen(testname,"w");
      
    thestr = xml.GetDocFormatted(4);
      
    fprintf(outPPfile,"%s",thestr.c_str());
      
    fclose(outPPfile);

    #endif

    xml.Close();
      
	
      
    xml.Load(testname);

    SetUpLineNumbers(testname);
  }


  xml.ResetPos();

  AssessClassification(xml,filename);

  xml.ResetPos();

  totalerrs = totalerrs + ProcessNTypeSpecs(xml);

  xml.ResetPos();



  for(i=1;i<num_trex_specs;i++)  
  {
    


	if(skipper==1)
	{
	  
	  skipper = 0;
	  continue;
	}



	skipper = 0;
	sprintf(t_id,    "%s",IGetNth(TREx_specs[i],0,0));
	sprintf(t_name,  "%s",IGetNth(TREx_specs[i],1,0));
	sprintf(t_defn,  "%s",IGetNth(TREx_specs[i],2,0));
	sprintf(t_dtype, "%s",IGetNth(TREx_specs[i],3,0));
	sprintf(t_domain,"%s",IGetNth(TREx_specs[i],4,0));
	sprintf(t_oblig, "%s",IGetNth(TREx_specs[i],5,0));
	sprintf(t_path,  "%s",IGetNth(TREx_specs[i],6,0));


	

	if(strstr(t_id,".N"))
	{
	  
	  if(
	      (!strncmp(t_domain,"codeList = ",11)) ||
		  (!strncmp(t_domain,"frame = ",8))
	    )
	  {
		skipper = 1;
	  }
	  continue;
	}




	if(
		(strcmp(t_oblig,"C / (TDR only)")) &&
		(strcmp(t_oblig,"C / (TDF only)")) &&
		(strcmp(t_oblig,"M / (TDR only)")) &&
		(strcmp(t_oblig,"M / (TDF only)")) &&
		(strcmp(t_oblig,"C (TDR only)")) &&
		(strcmp(t_oblig,"C (TDF only)")) &&
		(strcmp(t_oblig,"M (TDR only)")) &&
		(strcmp(t_oblig,"M (TDF only)")) &&
		(strcmp(t_oblig,"M")) &&
		(strcmp(t_oblig,"O"))
	  )
	{
      double thisid;

	  sscanf(t_id,"%lf",&thisid);

	  if((thisid<119)||(thisid>=132))
	  {
         printf("skipping spec %s because oblig is %s (line %d)\n",t_id,t_oblig,i);
		 continue;
	  }
	}

	if(finished_raw==1)
	{
	  
	  if(strstr(t_oblig,"TDR only"))

	  {
		


		

	    if(
		    (!strncmp(t_domain,"codeList = ",11)) ||
		    (!strncmp(t_domain,"frame = ",8))
	      )
		{
		  skipper = 1;
		}

        continue;
	  }
	}
	if(finished_raw==0)
	{
	  
	  if(strstr(t_oblig,"TDF only"))


	  {
		


		

	    if(
		    (!strncmp(t_domain,"codeList = ",11)) ||
		    (!strncmp(t_domain,"frame = ",8))
	      )
		{
		  skipper = 1;
		}

        continue;
	  }
	}


	if(
		(!strncmp(t_domain,"codeList = ",11)) ||
		(!strncmp(t_domain,"frame = ",8))
	  )
	{
      

	  sprintf(t_codelistval,"%s",IGetNth(TREx_specs[i+1],4,0));

	  skipper = 1;
	}
	else
	{
	  t_codelistval[0] = '\0';
	}




	






    xml.ResetPos();

	pathtype = IsNormalPath(xml,t_path);

	if(pathtype==1)
	{
      if(xml.FindElem(t_path))
	  {
		totalerrs = totalerrs + ProcessNewTrex(xml,rootname,t_domain,t_codelistval,t_id,northing,easting,finished_raw);
	  }
	  else
	  {
		if(strcmp(t_oblig,"O")) 
		{
          fprintf(outerrfile,"Error     : Failed to find required xpath:\n");
          fprintf(outerrfile,"  Path    : %s\n",t_path);
          fprintf(outerrfile,"  Spec ID : %s\n\n",t_id);
		  totalerrs = totalerrs + 1;
		}


		

	  }
	}
	else
	{
	  double thisid;

	  sscanf(t_id,"%lf",&thisid);

	  if(thisid==148.1)
	  {
		totalerrs = totalerrs + Compare1481(xml,t_id,t_path,finished_raw,t_domain);
	  }
	  else if(thisid==148.6)
	  {
		totalerrs = totalerrs + Compare1486(xml,t_id,t_path,finished_raw,t_domain);
	  }
	  else if((thisid<119)||(thisid>=138))
	  {
         printf("********Need to handle non-normal path %s\n",t_id);
	  }
	}
  }


  totalerrs = totalerrs + ProcessLongPaths(xml,finished_raw); 

  if(finished_raw==1)
  {
    totalerrs = totalerrs + ProcessLongPaths2(xml,finished_raw); 
  }


  xml.Close();


  if(totalerrs==0)
  {
    fprintf(outerrfile,"None.\n\n");
  }

  fclose(outerrfile);

  free(csTag);
  free(thedata);
  free(tempstring);
  local_tag_max        = 0;
  local_thedata_max    = 0;
  local_tempstring_max = 0;

  if(FilePosns!=NULL)
  {
    free(FilePosns);
    FilePosns = NULL;
  }

  return 1;
}


void WriteSummary()
{

  fprintf(outerrfile,"\n\nSummary:\n\n");

  if(cdescr_found>0)
  {
	fprintf(outerrfile,"CDESCR: %s\n\n",saveCDESCR);
  }
  else
  {
	fprintf(outerrfile,"CDESCR: Blank/Not Present\n\n");
  }

  if(stypeu_found>0)
  {
	fprintf(outerrfile,"STYPEU: %s\n\n",saveSTYPEU);
  }
  else
  {
	fprintf(outerrfile,"STYPEU: Blank/Not Present\n\n");
  }

  if(clstat_found>0)
  {
	fprintf(outerrfile,"CLSTAT: %s\n\n",saveCLSTAT);
  }
  else
  {
	fprintf(outerrfile,"CLSTAT: Blank/Not Present\n\n");
  }

  if(slstat_found>0)
  {
	fprintf(outerrfile,"SLSTAT:\n%s\n",saveSLSTAT); 
  }
  else
  {
	fprintf(outerrfile,"SLSTAT: Blank/Not Present\n\n");
  }


  if(sscrti_found>0)
  {
	fprintf(outerrfile,"SSCRTI:\n%s\n\n",saveSSCRTI);
  }
  else
  {
	fprintf(outerrfile,"SSCRTI: Blank/Not Present\n\n");
  }



  fprintf(outerrfile,"Dates:\n\n");
  
  fprintf(outerrfile,"%s",SavedSubregionDates);

}


void AddSSCRTI(int subregionid,char *sourcetype,int type)
{
  int oldlen;
  char tempstr[1000];

  


  if(sscrti_found==0)
  {
	saveSSCRTI = (char *)malloc(strlen(sourcetype)+100);
	sprintf(saveSSCRTI," Subregion: %d  Source Type: %s\n",subregionid,sourcetype);
    sscrti_found = 1;
  }
  else
  {
    oldlen = strlen(saveSSCRTI);

	saveSSCRTI = (char *)realloc(saveSSCRTI,oldlen+strlen(sourcetype)+100);
	sprintf(tempstr," Subregion: %d  Source Type: %s\n",subregionid,sourcetype);
	strcat(saveSSCRTI,tempstr);
  }

}



extern "C" int ParseXML2(int trd_version, int doxsd, char *filename, char *xsdname, char *refinefilename, char *outfilename,
						 int *schema_errors, int *nonschema_errors, int *shapefile_errors, int expected_version)
{
  CMarkup xml;
  int bFinished = 0;
  int i,j,k,len,lastsubregion,lastindex;
  long int orig_fileposn,end_fileposn,thistime=time(NULL);
  char testname[1000];

  trex_type = 0;

  if(trd_version==1)
  {
    version = 2;
	trd1_override = 1;
  }
  else
  {
    version = trd_version;
	trd1_override = 0;
  }

  if(SubregionACEI!=NULL)
  {
    free(SubregionACEI);
    SubregionACEI = NULL;
  }

  if(SubregionACEV!=NULL)
  {
    free(SubregionACEV);
    SubregionACEV = NULL;
  }

  if(SubregionALEI!=NULL)
  {
    free(SubregionALEI);
    SubregionALEI = NULL;
  }

  if(SubregionALEV!=NULL)
  {
    free(SubregionALEV);
    SubregionALEV = NULL;
  }

  if(SubregionSCPYRT!=NULL)
  {
    
    printf("Error: started with non-NULL SS\n");
    SubregionSCPYRT = NULL;
  }

  if(CCPYRT_SAVE!=NULL)
  {
    free(CCPYRT_SAVE);
	CCPYRT_SAVE = NULL;
  }




  

  GErr[0] = '\0';
  SSCALE_OVERRIDE     = NULL; 
  CLSTAT_OVERRIDE     = NULL; 
  CCPYRT_OVERRIDE     = NULL; 
  SCPYRT_OVERRIDE     = NULL; 
  use_sscale          = 0;
  use_clstat          = 0;
  use_ccpyrt          = 0;
  use_scpyrt          = 0;
  SingleSR            = 0;
  NonSingleSR         = 0;


  ZeroRefine(0);

  if(ProcessRefineFile(refinefilename)==0)
  {
    return 0;
  }


  


  SchemaErrorCount = 0;


  outerrfile = fopen(outfilename,"w");
  if(outerrfile==NULL)
  {
	printf("unable to open %s for write\n",outfilename);
	return 0;
  }


  len = strlen(outfilename);

  sprintf(testname,"%s",outfilename);
  sprintf(&testname[len-4],"TEMP.txt");
  if(DOOUTXML==1){outxmlfile = fopen(testname,"w");}


  sprintf(&testname[len-4],"POI.txt");
  outPOIfile = fopen(testname,"w");


  fprintf(outerrfile,"\n*********** UNCLASSIFIED ***********\n\n");

  fprintf(outerrfile,"Report Date: %s\n\n",ctime(&thistime));

  if(trd_version==1)
  {
    printf("Validate %s against version 1, save to %s\n",filename,outfilename);
    fprintf(outerrfile,"Validation results for %s against TRD version 1\n\n",filename);
  }
  else
  {
    printf("Validate %s against version %d, save to %s\n",filename,version,outfilename);
    fprintf(outerrfile,"Validation results for %s against TRD version %d\n\n",filename,version);
  }

  if(expected_version>0)
  {
    printf("...verify SVVERS is %d\n",expected_version);
  }

  printf("XSD: %s\n",xsdname);


  fprintf(outerrfile,"\nSchema (XSD) errors:\n\n");

  if(doxsd==1)
  {
    TestGrammar(xsdname,filename);

	if(SchemaErrorCount==0)
	{
      fprintf(outerrfile,"  No schema errors found.\n\n");
	}
	else
	{
      *schema_errors = 1;
	}
  }
  else
  {
    fprintf(outerrfile,"Not checking %s against schema (XSD) spec\n\n",filename);
  }

 
  fprintf(outerrfile,"\nNon-schema errors:\n\n");

  for(i=0;i<10;i++)
  {
    attributes[i][0] = '\0';
  }

  for(i=0;i<NumFixedStringsTRD2;i++)
  {
    FoundFixedStringTRD2[i] = 0;
  }

  for(i=0;i<NumFixedStringsTRD3;i++)
  {
    FoundFixedStringTRD3[i] = 0;
  }

  for(i=0;i<NumFixedStringsTRD4;i++)
  {
    FoundFixedStringTRD4[i] = 0;
  }

  for(i=0;i<NumAdditionalStrings;i++)
  {
    FoundAdditionalStrings[i] = 0;
  }

  for(i=0;i<NumTopicCodes;i++)
  {
    TopicCodesFound[i] = 0;
  }




  SaveLocalFilename    = (char *)malloc(1005);
  SaveLocalSrc         = (char *)malloc(1005);
  SaveLocalDesc        = (char *)malloc(1005);
  SaveLocalName        = (char *)malloc(1005);
  csTag                = (char *)malloc(1005);
  thedata              = (char *)malloc(1005);
  tempstring           = (char *)malloc(1005);
  local_filename_max   = 1000;
  local_src_max        = 1000;
  local_desc_max       = 1000;
  local_name_max       = 1000;
  local_tag_max        = 1000;
  local_thedata_max    = 1000;
  local_tempstring_max = 1000;

  InsideDataFile      = -1;
  InsideDataTree      = -1;
  PolyID              = -1;
  HoleID              = -1;
  lastsubregionid     = -1;
  working_geom_type   = -1;
  pointtotal          = -999;
  linetotal           = -999;
  areatotal           = -999;
  InSubregionFound    = 0;
  datafilenum         = 0;
  subregionfound      = 0;
  foundcellid         = 0;
  foundcuri           = 0;
  foundsubregionid    = 0;
  foundSVDATE         = 0;
  foundCCDATE         = 0;
  foundCEDDAT         = 0;
  foundCMDATE         = 0;
  foundCMSTDV         = 0;
  foundCMSTDN         = 0; 
  foundSubCMSTDV      = 0;
  foundSubCMSTDN      = 0; 
  foundCMSHND         = 0;
  foundCSHNDI         = 0;
  max_SMSHND_found    = 0;
  SDESCR_found        = 0;

  SSVRTI_found            = 0;
  SSVCDT_found            = 0;
  SSVCTY_found            = 0;
  potential_SSVRTI_found  = 0;
  potential_SSVCDT_found  = 0;



  SSCRTI_found           = 0;
  SEDDAT_found           = 0;
  SMDATE_found           = 0;
  SCDATE_found           = 0;
  SMDATE_found           = 0;
  SCDATE_found           = 0;
  SEDDAT_found           = 0;
  SAVE_STIERN            = 0;
  SAVE_SMSHND            = 0;
  FoundNewest            = 0;
  FoundOldest            = 0;
  alldatafiles           = 0;
  NumSubregionPolys      = 0;
  NumSubregionSources    = 0;
  ACE_ALE                = 0;
  POIINDEX               = 0;
  SingleSR               = 0;
  DoAce                  = 1;
  clstat_found           = 0;
  cdescr_found           = 0;
  slstat_found           = 0;
  sscrti_found           = 0;
  stypeu_found           = 0;
  foundCellWest          = 0;
  foundCellEast          = 0;
  foundCellSouth         = 0;
  foundCellNorth         = 0;
  cellWest               = 0;
  cellEast               = 0;
  cellSouth              = 0;
  cellNorth              = 0;
  cellWestLine [0]       = '\0';
  cellEastLine [0]       = '\0';
  cellSouthLine[0]       = '\0';
  cellNorthLine[0]       = '\0';
  SubregionFCName[0]     = '\0';
  CellFCName[0]          = '\0';
  saveCLSTAT[0]          = '\0';
  saveCDESCR[0]          = '\0';
  saveSTYPEU[0]          = '\0';
  SavedSubregionDates[0] = '\0';


  if(saveSLSTAT!=NULL)
  {
	free(saveSLSTAT);
    saveSLSTAT          = NULL;
  }
  if(saveSSCRTI!=NULL)
  {
	free(saveSSCRTI);
    saveSSCRTI          = NULL;
  }

  potentialSSVCDT[0]  = '\0';
  SSVCDT[0]           = '\0';
  potentialSSVRTI[0]  = '\0';
  SSVRTI[0]           = '\0';

  sprintf(SAVEOLDEST,"not found");
  sprintf(SAVENEWEST,"not found");



  xml.Load(filename);

  SetUpLineNumbers(filename);

  if(DoLineNumbers==0)
    {
      #if(USE_DOS==1)

       FILE *outPPfile;
       
      
       sprintf(&testname[len-11],"Formatted.xml");
       outPPfile = fopen(testname,"w");
      
       
      
       fprintf(outPPfile,"%s",xml.GetDocFormatted(4).c_str()     );
  
       fclose(outPPfile);
       

       #else

       FILE *outPPfile;
       std::string thestr;

	   
       sprintf(&testname[len-11],"Formatted.xml");
       outPPfile = fopen(testname,"w");
      
       thestr = xml.GetDocFormatted(4);
      
       fprintf(outPPfile,"%s",thestr.c_str());
      
       fclose(outPPfile);


       #endif




      xml.Close();
      
      fprintf(outerrfile,"Failed to find formatting in %s\n",filename);
      fprintf(outerrfile,"Re-wrote file with formatting to %s\n",testname);
      fprintf(outerrfile,"  Line numbers reported will be in this formatted file\n\n\n");
      
      xml.Load(testname);

      SetUpLineNumbers(testname);
    }


  xml.ResetPos();


  if(!xml.FindChildElem())
  {
    printf("empty!\n");
    bFinished = 1;
  }


  sprintf(theRealpath,"%s",GET_CSTR(xml.GetTagName(),1));
  thepath[0] = '\0';
  strcat(thepath,GetCleanToken(GET_CSTR(xml.GetTagName(),1)));

  


  orig_fileposn = ftell(outerrfile);


  while(bFinished==0)
  {

    xml.IntoElem();


    SetUpLineOffsets(xml);

    sprintf(csTag,"%s",GET_CSTR(xml.GetTagName(),1));


	


    strcat(theRealpath," *** ");
    strcat(theRealpath,csTag);

    strcat(thepath," *** ");
    strcat(thepath,GetCleanToken(csTag));


	

	sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

    FillInAttributes(xml);


	HandleItem(GetCleanToken(csTag),expected_version);


	for(i=0;i<10;i++)
	{
      attributes[i][0] = '\0';
	}

    int bFound = xml.FindChildElem();
    while((bFound==0) && (bFinished==0))
	{

	  if(!strcmp(csTag,GET_CSTR(xml.GetTagName(),1)))
	  {
		if((numattributes==0)&&(strlen(thedata)<1))
		{
           fprintf(outerrfile,"Error:  %s:\n   Empty (blank) tree or value\n\n",ParsePath(0));
		}
	  }

	  if(xml.OutOfElem())
	  {
		ScrubPath();
        bFound = xml.FindChildElem();

		
	  }
      else
	  {
        bFinished = 1;
	  }
	}  
  }

  HandleFixedCatalogs(1);

  HandleInsideSubregions(1);


  

  

  
  
  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    if(foundCSHNDI==0)
	{ 
	  printf("Never found CSHNDI\n");
      fprintf(outerrfile,"Error:  Never found CSHNDI\n\n");
	} 
    if(foundCMSHND==0)
	{ 
	  printf("Never found CMSHND\n");
      fprintf(outerrfile,"Error:  Never found CMSHND\n\n");
	} 
    if(max_SMSHND_found==0)
	{ 
	  printf("Never found SMSHND\n");
      fprintf(outerrfile,"Error:  Never found SMSHND\n\n");
	} 

    if((foundCSHNDI+foundCMSHND+max_SMSHND_found)>=3)
	{ 
       
	 
	   if(foundCSHNDI==foundCMSHND) 
	   { 
	     if((foundCSHNDI==1)&&(max_SMSHND_found==2))
		 { 
	       fprintf(outerrfile,"Error:  Found CSHNDI \"Unrestricted\" but SMSHND \"Limited Distribution (limdis)\"\n   Subregions may not have higher security handling levels than the cell\n\n");
		 } 
	     if((foundCSHNDI==2)&&(max_SMSHND_found==1))
		 { 
	       fprintf(outerrfile,"Error:  Found CSHNDI \"Limited Distribution (limdis)\" but did not find any SMSHND that were \"Limited Distribution (limdis)\"\n   All of the SMSHND found were \"Unrestricted\"\n   Some SMSHND must match the CSHNDI\n\n");
		 } 
	   } 
	} 
  }


  if(foundcellid==0)
  {
	printf("Never found CELLID\n");
    fprintf(outerrfile,"Error:  Never found CELLID\n\n");
  }
  else if(foundcuri==0)
  {
	printf("Never found CURI\n");
    fprintf(outerrfile,"Error:  Never found CURI\n\n");
  }
  else
  {
    CompareCellidCuri();
  }


  if(foundCCDATE==0)
  {
    fprintf(outerrfile,"Error:  Never found CCDATE\n\n");
  }
  if(foundCEDDAT==0)
  {
    fprintf(outerrfile,"Error:  Never found CEDATE\n\n");
  }
  if(foundCMDATE==0)
  {
    fprintf(outerrfile,"Error:  Never found CMDATE\n\n");
  }


  if((foundCCDATE>0)&&(foundCEDDAT>0))
  {
    if(CompareCCCEDates()==0)
	{
      fprintf(outerrfile,"Error:  CEDDAT \"%s\" earlier than CCDATE \"%s\"\n\n",SAVECEDDAT,SAVECCDATE);
	}
  }


  if((foundCMDATE>0)&&(foundCEDDAT>0))
  {
    if(CompareCMCEDates()==0)
	{
      fprintf(outerrfile,"Error:  CEDDAT \"%s\" earlier than CMDATE \"%s\"\n\n",SAVECEDDAT,SAVECMDATE);
	}
  }




  if(FoundNewest==0)
  {
    printf("Never found CNEWSD\n");
    fprintf(outerrfile,"Error:  Never found Date Of Newest Source (CNEWSD)\n\n");
  }
  else if(FoundOldest==0)
  {
    printf("Never found COLDSD\n");
    fprintf(outerrfile,"Error:  Never found Date Of Oldest Source (COLDSD)\n\n");
  }
  else
  {
    if(CompareNewOldDates()==0)
	{
      fprintf(outerrfile,"Error:  Date Of Newest Source (CNEWSD) \"%s\" earlier than Date Of Oldest Source (COLDSD) \"%s\"\n\n",SAVENEWEST,SAVEOLDEST);
	}
  }

  if((version==3)||((version==4)||(version==41)||(version==42)||(version==43)||(version==44)))
  {
    for(i=0;i<NumTopicCodes;i++)
	{ 
	  if(TopicCodesFound[i]==0)
	  { 
        fprintf(outerrfile,"Error:   Never found required TopicCode \"%s\"\n\n",TopicCodes[i]);
	  } 
	} 
  }


  
  VerifySubregionSourceDates();  


  if(version==2)
  {
    for(i=0;i<NumFixedStringsTRD2;i++)
	{
      if(FoundFixedStringTRD2[i]==0)
	  {
        printf("Never found %s\n",FixedStringsTRD2[i*2]);
        fprintf(outerrfile,"Error:  Never found %s\n  Did you select the right version of TRD to validate against?\n\n",FixedStringsTRD2[i*2]);
	  }
	}
  }
  else if(version==3)
  {
    for(i=0;i<NumFixedStringsTRD3;i++)
	{ 
      if(FoundFixedStringTRD3[i]==0)
	  { 
		
        fprintf(outerrfile,"Error:  Never found %s\n  Did you select the right version of TRD to validate against?\n\n",FixedStringsTRD3[i*2]);
	  } 
	} 
  }
  else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
  {
    for(i=0;i<NumFixedStringsTRD4;i++)
	{ 
      if(FoundFixedStringTRD4[i]==0)
	  { 
		
        fprintf(outerrfile,"Error:  Never found %s\n  Did you select the right version of TRD to validate against?\n\n",FixedStringsTRD4[i*2]);
	  }
	} 
  }


  for(i=0;i<NumAdditionalStrings;i++)
  {
    if(FoundAdditionalStrings[i]==0)
	{
      printf("Never found additional %d\n",i);

	  if(i==0)
	  {
        if(version==2)
		{ 
          fprintf(outerrfile,"Error:  Never found CVSPCN in %s\n  Did you select the right version of TRD to validate against?\n\n",
		   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString");
		}
		else if(version==3)
		{
          fprintf(outerrfile,"Error:  Never found SVSPCN in %s\n  Did you select the right version of TRD to validate against?\n\n",
		   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString");
		}
		else if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
		{
          fprintf(outerrfile,"Error:  Never found SVSPCN in %s\n  Did you select the right version of TRD to validate against?\n\n",
		   "MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** result *** DQ_ConformanceResult *** specification *** CI_Citation *** title *** CharacterString");
		}
	  }
      else if(i==1)
	  { 
	    if((version==3)||((version==4)||(version==41)||(version==42)||(version==43)||(version==44)))
		{ 
          fprintf(outerrfile,"Error:  Never found GAIT version in %s\n  Did you select the right version of TRD to validate against?\n\n",
		 	"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** measureDescription *** CharacterString");
		}
	    else if(version==2)
		{ 
          fprintf(outerrfile,"Error:  Never found GAIT version in %s\n  Did you select the right version of TRD to validate against?\n\n",
		   "MGCP_Cell *** has *** MD_Metadata *** dataQualityInfo *** DQ_DataQuality *** report *** DQ_ConceptualConsistency *** measureDescription *** CharacterString");
		} 
	  } 
	  else if(i==2)
	  { 
        fprintf(outerrfile,"Error:  Never found CCRSID in %s\n\n\n",
		 "MGCP_Cell *** has *** MD_Metadata *** referenceSystemInfo *** MD_ReferenceSystem *** referenceSystemIdentifier *** RS_Identifier *** code *** Anchor");
	  } 
	}
  }

  if(foundsubregionid==0)
  {
    fprintf(outerrfile,"Error:  Didn't find any subregions\n\n");
  }



  AllDataFilesPresent = (int *)malloc(sizeof(int)*(alldatafiles+5));

  for(i=0;i<alldatafiles;i++)
  {
    AllDataFilesPresent[i] = 0;
  }


  if((SingleSR==1)&&(foundsubregionid!=1))
  {
     fprintf(outerrfile,"Error:  Found a SDESCR \"Single subregion\", but found %d Subregions\n\n",foundsubregionid);
  }




  if(foundsubregionid==1)
  {
    if(NonSingleSR==1)  
	{
	  if((version==3)||(version==4))
	  {
        fprintf(outerrfile,"Error:  Found a SDESCR \"Subregion01\", but only 1 subregion...the SDESCR should be \"Single subregion\", \"Update subregion\", or \"Producer subregion\"   \n\n");
	  }
	  else
	  {
        fprintf(outerrfile,"Error:  Found a SDESCR \"Subregion01\", but only 1 subregion...the SDESCR should be \"Single subregion\"   \n\n");
	  }
	}
  }


  if(foundsubregionid>0)
  {
    if(SDESCR_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SDESCR for subregion %d\n\n",foundsubregionid);
	}
    SDESCR_found = 0;





    if((version==4)||(version==41)||(version==42)||(version==43)||(version==44))
	{
	  

	  Check2DDates(0);

      if(SSVRTI_found==0)
	  {
        fprintf(outerrfile,"Error: Did not find SSVRTI for subregion %d\n\n",foundsubregionid);
	  }
      SSVRTI_found = 0;
      potential_SSVRTI_found = 0;

      if(SSVCDT_found==0)
	  {
        fprintf(outerrfile,"Error: Did not find SSVCDT for subregion %d\n\n",foundsubregionid);
	  }
      SSVCDT_found = 0;
      potential_SSVCDT_found = 0;

      if(SSVCTY_found==0)
	  { 
        fprintf(outerrfile,"Error: Did not find SSVCTY for subregion %d\n\n",foundsubregionid);
	  }
      SSVCTY_found = 0;
	}




    if(SSCRTI_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SSCRTI for subregion %d\n\n",foundsubregionid);
	}
    SSCRTI_found = 0;

    if(SSRCDT_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SSRCDT for subregion %d\n\n",foundsubregionid);
	}
    SSRCDT_found = 0;


    if(SSRCTY_found==0)
	{
      fprintf(outerrfile,"Error: Did not find SSRCTY for subregion %d\n\n",foundsubregionid);
	}
    SSRCTY_found = 0;
  }

  TestExtent();

  TestSubregionFilesOnList();

  TestHolesInside();

  fprintf(outPOIfile,"1\n");

  SubregionACEI = (int *)   malloc(sizeof(int)*(foundsubregionid+5));
  SubregionACEV = (double *)malloc(sizeof(double)*(foundsubregionid+5));

  SubregionALEI = (int *)   malloc(sizeof(int)*(foundsubregionid+5));
  SubregionALEV = (double *)malloc(sizeof(double)*(foundsubregionid+5));

  SubregionSCPYRT = (char **)malloc(sizeof(char *)*(foundsubregionid+5));

  for(i=0;i<foundsubregionid+5;i++)
  {
    SubregionACEI[i]   = -1;
    SubregionACEV[i]   = FAKE_ACE_VALUE;

    SubregionALEI[i]   = -1;
    SubregionALEV[i]   = FAKE_ACE_VALUE;

    SubregionSCPYRT[i] = NULL;
  }

  for(i=0;i<NumSubregionData;i++)
  {
    if(SubregionData[i].ace_index>=0)
	{
      if(SubregionACEI[SubregionData[i].subregionid]<0)
	  {
        SubregionACEI[SubregionData[i].subregionid] = SubregionData[i].ace_index;
	  }
	  else if(SubregionACEI[SubregionData[i].subregionid]!=SubregionData[i].ace_index)
	  {
        fprintf(outerrfile,"Error: got 2 different SACEMT for subregion %d: %d and %d\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionACEI[SubregionData[i].subregionid],SubregionData[i].ace_index);

        printf("Error: got 2 different SACEMT for subregion %d: %d and %d\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionACEI[SubregionData[i].subregionid],SubregionData[i].ace_index);

		DoAce = 0;
	  }
	}


    if(SubregionData[i].ale_index>=0)
	{
      if(SubregionALEI[SubregionData[i].subregionid]<0)
	  {
        SubregionALEI[SubregionData[i].subregionid] = SubregionData[i].ale_index;
	  }
	  else if(SubregionALEI[SubregionData[i].subregionid]!=SubregionData[i].ale_index)
	  {
        fprintf(outerrfile,"Error: got 2 different SALEMT for subregion %d: %d and %d\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionALEI[SubregionData[i].subregionid],SubregionData[i].ale_index);

        printf("Error: got 2 different SALEMT for subregion %d: %d and %d\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionALEI[SubregionData[i].subregionid],SubregionData[i].ale_index);

		
        for(j=0;j<foundsubregionid+5;j++)
		{
          SubregionALEI[j] = -1;
		}
	  }
	}



    if(SubregionData[i].ace_value!=FAKE_ACE_VALUE)
	{
      if(SubregionACEV[SubregionData[i].subregionid]==FAKE_ACE_VALUE)
	  {
        SubregionACEV[SubregionData[i].subregionid] = SubregionData[i].ace_value;
	  }
	  else if(SubregionACEV[SubregionData[i].subregionid]!=SubregionData[i].ace_value)
	  {
        fprintf(outerrfile,"Error: got 2 different SACEVL for subregion %d: %lf and %lf\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionACEV[SubregionData[i].subregionid],SubregionData[i].ace_value);

        printf("Error: got 2 different SACEVL for subregion %d: %lf and %lf\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionACEV[SubregionData[i].subregionid],SubregionData[i].ace_value);

		DoAce = 0;
	  }
	}



    if(SubregionData[i].ale_value!=FAKE_ACE_VALUE)
	{
      if(SubregionALEV[SubregionData[i].subregionid]==FAKE_ACE_VALUE)
	  {
        SubregionALEV[SubregionData[i].subregionid] = SubregionData[i].ale_value;
	  }
	  else if(SubregionALEV[SubregionData[i].subregionid]!=SubregionData[i].ale_value)
	  {
        fprintf(outerrfile,"Error: got 2 different SALEVL for subregion %d: %lf and %lf\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionALEV[SubregionData[i].subregionid],SubregionData[i].ale_value);

        printf("Error: got 2 different SALEVL for subregion %d: %lf and %lf\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionALEV[SubregionData[i].subregionid],SubregionData[i].ale_value);

		
        for(j=0;j<foundsubregionid+5;j++)
		{
          SubregionALEI[j] = -1;
		}
	  }
	}

	if(SubregionData[i].scpyrt!=NULL)
	{
      if(SubregionSCPYRT[SubregionData[i].subregionid]==NULL)
	  {
        SubregionSCPYRT[SubregionData[i].subregionid] = (char *)malloc(strlen(SubregionData[i].scpyrt)+5);
        sprintf(SubregionSCPYRT[SubregionData[i].subregionid],"%s",SubregionData[i].scpyrt);
	  }
	  else if(strcmp(SubregionSCPYRT[SubregionData[i].subregionid],SubregionData[i].scpyrt))
	  {
        fprintf(outerrfile,"Error: got 2 different SCPYRT for subregion %d: %s and %s\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionSCPYRT[SubregionData[i].subregionid],SubregionData[i].scpyrt);

        printf("Error: got 2 different SCPYRT for subregion %d: %s and %s\n  Not doing feature level/cell level metadata checking\n\n",
			SubregionData[i].subregionid,SubregionSCPYRT[SubregionData[i].subregionid],SubregionData[i].scpyrt);

		DoAce = 0;
	  }
	}
  }


  for(j=0;j<NumSubregionPolys;j++)
  {
    SubregionPolys[j].ACE = FAKE_ACE_VALUE;
    SubregionPolys[j].ALE = FAKE_ACE_VALUE;
    SubregionPolys[j].ACE_pick = -1;
    SubregionPolys[j].ALE_pick = -1;
	SubregionPolys[j].SCPYRT = NULL;
  } 

  if(DoAce==1)
  {
    for(i=1;i<=foundsubregionid;i++)
	{
      if((version==2)||(version==3))  
	  {
        if(SubregionACEI[i]<0)
		{ 
          printf("Error: did not find SACEMT for subregion %d...cannot compare feature level metadata to cell level metadata\n",i);
		  DoAce = 0;
		} 
        if(SubregionACEV[i]==FAKE_ACE_VALUE)
		{ 
          printf("Error: did not find SACEVL for subregion %d...cannot compare feature level metadata to cell level metadata\n",i);
		  DoAce = 0;
		} 
	  }



	  

      for(j=0;j<NumSubregionPolys;j++)
	  {
        if(SubregionPolys[j].subregionid==i)
		{
          SubregionPolys[j].ACE      = SubregionACEV[i];
          SubregionPolys[j].ACE_pick = SubregionACEI[i];

          SubregionPolys[j].ALE      = SubregionALEV[i];
          SubregionPolys[j].ALE_pick = SubregionALEI[i];

		  if(SubregionSCPYRT[i]!=NULL)
		  {
            SubregionPolys[j].SCPYRT = (char *)malloc(strlen(SubregionSCPYRT[i])+5);
			sprintf(SubregionPolys[j].SCPYRT,"%s",SubregionSCPYRT[i]);
		  }
		}
	  }
	}


	


  }








  

  if(version==2)
  {
    thearray = SourceTypeIds2;
  }
  else if(version==3)
  {
    thearray = SourceTypeIds3;
  }
  else if(version==4)
  {
    thearray = SourceTypeIds4;
  }
  else if((version==41)||(version==42)||(version==43))
  {
    thearray = SourceTypeIds41;
  }
  else if(version==44)
  {
    thearray = SourceTypeIds44;
  }


  


  if((version==4)||(version==41)||(version==42)||(version==43)||(version==44)) 
  {
    for(j=0;j<NumSubregionSources;j++)
	{  
 	  if(ValidSubregionSource(j))
	  {  
        if(SubregionSources[j].type==3)
		{ 
          

		  for(k=j+1;k<NumSubregionSources;k++)
		  { 
		    if(
			    (SubregionSources[k].index      == SubregionSources[j].index) &&
		        (SubregionSources[k].type       == SubregionSources[j].type) &&
		        (SubregionSources[k].subregionid== SubregionSources[j].subregionid)
			  )
			{ 
			  

			  if((strcmp(thearray[SubregionSources[k].index],"OTH"))||(version<44))
			  {
                fprintf(outerrfile,"Error:  Source type %s, subregion %d\n  Multiple entries present without recognized Newest/Oldest designations\n\n",
					thearray[SubregionSources[k].index],SubregionSources[k].subregionid);
			  }
			} 
		  } 
		} 
	  } 
	} 
  }


  lastsubregion = -1;
  lastindex     = -1;

  for(j=0;j<NumSubregionSources;j++)
  { 
 	if(ValidSubregionSource(j))
	{ 

	  if(SubregionSources[j].type>0) 
	  {
	    
		if(
			(lastsubregion!=SubregionSources[j].subregionid) ||
			(lastindex!=SubregionSources[j].index)
		  )
		{
	      AddSSCRTI(SubregionSources[j].subregionid,(char *)thearray[SubregionSources[j].index],SubregionSources[j].type);
		  lastsubregion = SubregionSources[j].subregionid;
		  lastindex = SubregionSources[j].index;
		}
	  }


      if((SubregionSources[j].type==1)||(SubregionSources[j].type==2))
	  {
        


		for(k=j+1;k<NumSubregionSources;k++)
		{
		  if(
			   (SubregionSources[k].index      == SubregionSources[j].index) &&
		       (SubregionSources[k].type       == SubregionSources[j].type) &&
		       (SubregionSources[k].subregionid== SubregionSources[j].subregionid)
			)
		  {
			if(SubregionSources[j].type==1)
			{
              fprintf(outerrfile,"Error:  Source type %s has multiple Oldest dates for subregion %d\n  Terminating attempt to validate feature level dates for this source type and subregion\n\n",
					thearray[SubregionSources[k].index],SubregionSources[k].subregionid);

			  ZeroOutTypes(SubregionSources[k].index,SubregionSources[k].type,SubregionSources[k].subregionid);
			}
			if(SubregionSources[j].type==2)
			{
              fprintf(outerrfile,"Error:  Source type %s has multiple Newest dates for subregion %d\n  Terminating attempt to validate feature level dates for this source type and subregion\n\n",
					thearray[SubregionSources[k].index],SubregionSources[k].subregionid);

		      ZeroOutTypes(SubregionSources[k].index,SubregionSources[k].type,SubregionSources[k].subregionid);
			}
		  }
		}
	  }
	}
  } 


  end_fileposn = ftell(outerrfile);

  if(orig_fileposn==end_fileposn)
  {
    fprintf(outerrfile,"  No non-schema errors found.\n\n");
  }
  else
  {
    *nonschema_errors = 1;
  }


  if(PostProcessFeatures(GetContainingFolder(filename))>0)
  {
    *shapefile_errors = 1;
  }


  if(NumSubregionSources>0)
  {
    free(SubregionSources);
    SubregionSources = NULL;
    NumSubregionSources = 0;
  }


  WritePOIFile();

  WriteSummary();

  fprintf(outerrfile,"\n*********** UNCLASSIFIED ***********\n\n");


  


  for(i=0;i<NumSubregionPolys;i++)
  {

    for(j=0;j<SubregionPolys[i].num_ftype;j++)
	{
	  free(SubregionPolys[i].ftypes[j]);
	}

	free(SubregionPolys[i].ftypes);
	free(SubregionPolys[i].x);
	free(SubregionPolys[i].y);

	if(SubregionPolys[i].numholes>0)
	{
	  for(j=0;j<SubregionPolys[i].numholes;j++)
	  {
        free(SubregionPolys[i].holes[j].x);
        free(SubregionPolys[i].holes[j].y);
	  }
	  free(SubregionPolys[i].holes);
	}
	if(SubregionPolys[i].SCPYRT!=NULL)
	{
      free(SubregionPolys[i].SCPYRT);
	}
  }

  free(SubregionPolys);
  NumSubregionPolys = 0;
  SubregionPolys = NULL;

  if(NumSubregionData>0)
  {
    if(SubregionData!=NULL)
	{
      for(i=0;i<NumSubregionData;i++)
	  { 
        if(SubregionData[i].scpyrt!=NULL)
		{ 
          free(SubregionData[i].scpyrt);
		}
	  }
      free(SubregionData);
	}
  }
  NumSubregionData = 0;
  SubregionData = NULL;

  if(SubregionACEI!=NULL)
  {
    free(SubregionACEI);
    SubregionACEI = NULL;
  }

  if(SubregionALEI!=NULL)
  {
    free(SubregionALEI);
    SubregionALEI = NULL;
  }

  if(SubregionACEV!=NULL)
  {
    free(SubregionACEV);
    SubregionACEV = NULL;
  }

  if(SubregionALEV!=NULL)
  {
    free(SubregionALEV);
    SubregionALEV = NULL;
  }

  if(SubregionSCPYRT!=NULL)
  {
    for(i=0;i<foundsubregionid+5;i++)
	{
      if(SubregionSCPYRT[i]!=NULL)
	  {
		  free(SubregionSCPYRT[i]);
	  }
	}
    free(SubregionSCPYRT);
    SubregionSCPYRT = NULL;
  }
 



  


  if(DOOUTXML==1){fclose(outxmlfile);}
  fclose(outerrfile);
  fclose(outPOIfile);



  free(AllDataFilesPresent);

  if(ShapeFolder!=NULL)
  {
    free(ShapeFolder);
  }
  ShapeFolder = NULL;


  if(alldatafiles>0)
  {
    for(i=0;i<alldatafiles;i++)
	{
	  free(AllDataFiles[i]);
	} 
	free(AllDataFiles);
  }
  
  
  free(SaveLocalFilename);
  free(SaveLocalSrc);
  free(SaveLocalDesc);
  free(SaveLocalName);
  free(csTag);
  free(thedata);
  free(tempstring);
  local_filename_max   = 0;
  local_src_max        = 0;
  local_desc_max       = 0;
  local_name_max       = 0;
  local_tag_max        = 0;
  local_thedata_max    = 0;
  local_tempstring_max = 0;
  FreeShapeMemory();
  if(DoLineNumbers==1)
  {
	if(FilePosns!=NULL)
	{
      free(FilePosns);
	}
    FilePosns = NULL;
  }

  if(SSCALE_OVERRIDE!=NULL)
  {
    free(SSCALE_OVERRIDE);
	SSCALE_OVERRIDE = NULL;
  }

  if(CLSTAT_OVERRIDE!=NULL)
  {
    free(CLSTAT_OVERRIDE);
	CLSTAT_OVERRIDE = NULL;
  }

  if(CCPYRT_OVERRIDE!=NULL)
  {
    free(CCPYRT_OVERRIDE);
	CCPYRT_OVERRIDE = NULL;
  }
  if(SCPYRT_OVERRIDE!=NULL)
  {
    free(SCPYRT_OVERRIDE);
	SCPYRT_OVERRIDE = NULL;
  }

  return 1;
}












extern "C" char *PParseXML(char *filename)
{

  CMarkup xml;
  int bFinished = 0;
  int i;
  static char retval[500];


  

  retval[0] = '\0';

  xml.Load(filename);

  xml.ResetPos();

  csTag      = (char *)malloc(1005);
  thedata    = (char *)malloc(1005);
  tempstring = (char *)malloc(1005);

  local_tag_max        = 1000;
  local_thedata_max    = 1000;
  local_tempstring_max = 1000;


  if(!xml.FindChildElem())
  {
    printf("Could not open temporary XML file %s\n",filename);
	return 0;
  }


  sprintf(theRealpath,"%s",GET_CSTR(xml.GetTagName(),1));
  thepath[0] = '\0';
  strcat(thepath,GetCleanToken(GET_CSTR(xml.GetTagName(),1)));


  while(bFinished==0)
  {

    xml.IntoElem();

    SetUpLineOffsets(xml);

    sprintf(csTag,"%s",GET_CSTR(xml.GetTagName(),1));

    strcat(theRealpath," *** ");
    strcat(theRealpath,csTag);

    strcat(thepath," *** ");
    strcat(thepath,GetCleanToken(csTag));

	sprintf(thedata,"%s",GET_CSTR(xml.GetData(),1));

    FillInAttributes(xml);


    if(!strcmp(thepath,"MGCP_Cell *** subregion *** MGCP_Subregion *** subregionMetadata *** MD_Metadata *** contentInfo *** MD_FeatureCatalogueDescription *** featureCatalogueCitation *** CI_Citation *** title *** Anchor"))
	{ 

      for(i=0;i<numattributes;i++)
	  { 
	    if(!strcmp(attributes[i],"xlink:href"))
		{ 
          sprintf(retval,"%s",attributeV[i]);
		  break;
		}

		
	  }

	}

	if(retval[0]!='\0')
	{
      break;
	}

	for(i=0;i<10;i++)
	{
      attributes[i][0] = '\0';
	}

    int bFound = xml.FindChildElem();
    while((bFound==0) && (bFinished==0))
	{

	  

	  if(xml.OutOfElem())
	  {
		ScrubPath();
        bFound = xml.FindChildElem();

		
	  }
      else
	  {
        bFinished = 1;
	  }
	}  
  }

  free(csTag);
  free(thedata);
  free(tempstring);

  return retval;
}

