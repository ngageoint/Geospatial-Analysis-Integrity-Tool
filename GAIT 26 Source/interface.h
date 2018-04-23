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
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h> 
#include <time.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <Xm/MessageB.h>
#include <Xm/DialogS.h>
#include <Xm/Xm.h>              
#include <Xm/Text.h>             
#include <Xm/Form.h>
#include <Xm/FileSB.h>
#include <Xm/SelectioB.h>
#include <Xm/Frame.h>
#include <Xm/ScrollBar.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/ScrolledW.h>
#include <Xm/MainW.h>
#include <Xm/DrawingA.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/CascadeB.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>
#include <Xm/ArrowB.h>
#include <Xm/Separator.h>
#include <Xm/Protocols.h>
#include <Xm/PanedW.h>
#include <Xm/Scale.h>
#include <Xm/MwmUtil.h>
#include <Xm/TransltnsP.h>
#include <X11/cursorfont.h>
#include <X11/keysymdef.h>

#include "share.h"
#include "shapefil.h"
#include "LiteClue.h"
#include "OutlineP.h"
#include "HandleP.h"



#define TRS0   0
#define TRS1   1
#define TRS2   2
#define TRS4   3
#define TRS5   4
#define TRS7   5
#define TRS9   6
#define TRS10  7
#define TRS12  8
#define TRS13  9
#define TRS14  10
#define TRS15  11
#define TRS16  12
#define TRS20  13
#define TRS25  14
#define TRS998 15
#define TRS999 16
#define numTRS 17

#define PCF999999 0
#define PCF1      1
#define PCF2      2
#define PCF3      3
#define PCF4      4
#define PCF5      5
#define PCF6      6
#define numPCF 7



#define TRS0NAME   "Unknown"
#define TRS1NAME   "Aeronautical"
#define TRS2NAME   "Aqueduct"
#define TRS4NAME   "Bus"
#define TRS5NAME   "Canal"
#define TRS7NAME   "Maritime"
#define TRS9NAME   "Pedestrian"
#define TRS10NAME  "Pipeline"
#define TRS12NAME  "Railway"
#define TRS13NAME  "Road"
#define TRS14NAME  "Road and Railway"
#define TRS15NAME  "Truck"
#define TRS16NAME  "Underground Railway"
#define TRS20NAME  "Taxiway"
#define TRS25NAME  "Runway"
#define TRS998NAME "Not Applicable"
#define TRS999NAME "Other"

#define PCF999999NAME "No Information"
#define PCF1NAME      "Construction"
#define PCF2NAME      "Intact"
#define PCF3NAME      "Unmaintained"
#define PCF4NAME      "Damaged"
#define PCF5NAME      "Dismantled"
#define PCF6NAME      "Destroyed"



#if(USE_DOS==1)
#include <windows.h>
#include <direct.h>
#define POPEN _popen
#define PCLOSE _pclose
#define MYSLASH "\\"
#define MYCHARSLASH '\\'
#else
#include <sys/time.h>

#include <sys/stat.h>

#define POPEN popen
#define PCLOSE pclose
#define MYSLASH "/"
#define MYCHARSLASH '/'
#endif

#define UnicodeNum 10634


#if(NGA_TYPE==1)
#define TOOLNUM 11
#define TOOLBAR_CLASS 10
#else
#define TOOLNUM 10
#define TOOLBAR_CLASS 999
#endif

#define outfoldername "internal_data"


#define DOS_HEIGHT_MODIFIER (USE_DOS*60)
#define SHADES_PER_BIN 10
#define NUMBER_OF_BINS 5



#define USE_GGDM 1
#define USE_NFDD 1
#define USE_DFDD 1
#define USE_UFD  1
#define USE_TDS  1




#if(USE_DOS==0)
#include <X11/xpm.h>
#endif


#ifndef XmINDICATOR_CHECK_GLYPH 
#define TOGGLEINCLUDE XmNselectColor,   mycolors[COLOR_GREEN],                          \
                      XmNindicatorSize, 20
#else
  #ifndef XmINDICATOR_3D_BOX 
  #define TOGGLEINCLUDE XmNselectColor,   mycolors[COLOR_GREEN],                        \
                        XmNindicatorSize, 20
  #else
  #define TOGGLEINCLUDE XmNindicatorOn,   XmINDICATOR_CHECK_GLYPH + XmINDICATOR_3D_BOX, \
                        XmNfillOnSelect,  False,                                        \
                        XmNindicatorSize, 20
  #endif
#endif



#define TOGGLETRANSLATE XmNtranslations, XtParseTranslationTable ( translations_global )

#define LABELWTRANSLATE XmNtranslations, XtParseTranslationTable ( translations_global )


#define CHOOSEPROJ_BROWSE 12
#define CHOOSE_ES1_BROWSE 13
#define CHOOSE_ES2_BROWSE 14
#define CHOOSE_TXT_BROWSE 15
#define CHOOSESHAPE_BROWSE 16
#define CHOOSEDATA_BROWSE 26
#define CHOOSE_XML1_BROWSE 27
#define CHOOSE_XML2_BROWSE 28
#define CHOOSE_XML3_BROWSE 29
#define ATTRFOLDER_BROWSE 30
#define CHOOSE_DP_BROWSE 31
#define CHOOSEBROWSE_BROWSE 32

#define DRAWING_A_SCROLL     0
#define INFO_LEFT_SCROLL     1 
#define INFO_RIGHT_SCROLL    2 
#define LOD_SCROLL           3 
#define ATT1_SCROLL          4 
#define ATT2_SCROLL          5 
#define ATT3_SCROLL          6 
#define EXE_SCROLL           7 
#define CONDITION_SCROLL     8 
#define INDIV_SCROLL         9 
#define EXEHELP_SCROLL      10 
#define INFO_SCROLL         11 
#define FILE1_SCROLL        12 
#define INDBFILE_SCROLL     13 
#define FILEOTHER_SCROLL    14 
#define FILEOTHER1_SCROLL   15 
#define AOI_SCROLL          20
#define BO_SCROLL           23
#define BO_FILTER           25
#define NETWORK_SCROLL      28
#define INDB_SCROLL         29
#define EXEFILE_SCROLL      30
#define CONDPICK_SCROLL     31
#define PREVPICK_SCROLL     32
#define AOI_LOAD            33
#define LAYER_LOAD          34
#define SAVELAYER           35
#define CHOOSEPROJ_SCROLL   36
#define CHOOSESHAPE_SCROLL  37
#define CHOOSESHAPE_FILE    38
#define CHOOSE_ES1_SCROLL   39
#define CHOOSE_ES1_FILE     40
#define CHOOSE_ES2_SCROLL   41
#define CHOOSE_ES2_FILE     42
#define CHOOSE_TXT_SCROLL   43
#define CHOOSE_TXT_FILE     44
#define CHOOSEDATA_SCROLL   45
#define CHOOSEDATA_FILE     46
#define CHOOSE_XML1_SCROLL  47
#define CHOOSE_XML1_FILE    48
#define CHOOSE_XML2_SCROLL  49
#define CHOOSE_XML2_FILE    50
#define CHOOSE_XML3_SCROLL  51
#define CHOOSE_XML3_FILE    52
#define CHOOSEATTR_SCROLL   56
#define CHOOSEATTR_FILE     57
#define CHOOSE_DP_SCROLL    58
#define CHOOSE_DP_FILE      59
#define CHOOSEBROWSE_SCROLL 60
#define CHOOSEBROWSE_FILE   61
#define SAVEPREFS           63
#define NET_SCROLL          64
#define EXEFILE_NETSCROLL   65


#define SPECIAL_SCROLL    99 
#define DRIVE_MAX         30 

#define NETWORKCHECK CREATENET

#define XCG            1
#define DEBUG          0

#define FORMAT_POINT_NUM 1000000
#define FORMAT_LINE_NUM 1000000
#define FORMAT_AREA_NUM 1000000


#define CLIPDEBUG      0
#define SHIFTDEBUG     0
#define ERRORDEBUG     0
#define TOGGLEDEBUG    0
#define MODELDEBUG     0
#define OVERDEBUG      0
#define FANDEBUG       0
#define STRINGDEBUG    0
#define ACTORDEBUG     0
#define SENSDEBUG      0
#define SHORTDEBUG     0
#define ZOOMDEBUG      0
#define COLORDEBUG     0
#define LODDEBUG       0
#define EXCLUDEDEBUG   0
#define EDCSDEBUG      0
#define INTDEBUG       0
#define GRAPHDEBUG     0
#define EXEOPTIONDEBUG 0
#define BATCHDEBUG     0
#define ENDIANDEBUG    0
#define ATTRDEBUG      0
#define CLONEDEBUG     0
#define FIND_SAC_MAX   0
#define BODEBUG        0
#define POPDEBUG       0
#define DOSDEBUG       0
#define QACRDEBUG      0
#define MAX_SNGL_STACK 5
#define MAX_STACK      10


#define DOC_TRM          0
#define DOC_BASICS       1
#define DOC_INSPECTIONS  2
#define DOC_COOKBOOK     3
#define DOC_METADATA     5
#define DOC_NUNANPO      6
#define DOC_D4           7
#define DOC_VMAP         8
#define DOC_FACC         9
#define DOC_MGCP3       10
#define DOC_UFD1        14
#define DOC_UFD3        15
#define DOC_WHATSNEW    16
#define DOC_GGDM22      17
#define DOC_XWALK       20
#define DOC_UFD2        29
#define DOC_MGCPXML     30
#define DOC_MGCPXML2    31
#define DOC_SAC         36
#define DOC_SCHEMA      37
#define DOC_TDSISF4     38
#define DOC_TDSISF6     39
#define DOC_MGCP4       40
#define DOC_NGAQACR     41
#define DOC_MGCPQACR    42
#define DOC_MGCPISL3    43
#define DOC_MGCPISL4    44
#define DOC_MGCPRANGE3  45
#define DOC_MGCPISF3    46
#define DOC_MGCPISF4    47
#define DOC_TDS4        48
#define DOC_MGCPPPP3    50
#define DOC_TDS6        51
#define DOC_TDSISL4     52
#define DOC_MGCPRANGE4  53
#define DOC_UNIQUENAMES 54
#define DOC_MGCPPPP4    55
#define DOC_MGCPIAE     56
#define DOC_TDSISL6     57
#define DOC_PROJSUMM    58
#define DOC_CHECKSUMM   59
#define DOC_GGDM21      60
#define DOC_TDSIAE      61
#define DOC_MGCPIPA     62
#define DOC_MGCPIPL     63
#define DOC_MGCPILA     64
#define DOC_GGDM3       65
#define DOC_MGCPFTA     66


#define POLY_LIMIT   100
#define LINE_LIMIT   101
#define CIRCLE_LIMIT 102
#define GRID_LIMIT   103

#define NEW_BOTTOM 25
#define NUM_STIPMAPS 8

#define TRMNAME          "TechRefMan_GAIT26.pdf"
#define BASICSNAME       "Basics_GAIT26.pdf"
#define INSPECTIONSNAME  "Inspections_GAIT26.pdf"
#define COOKBOOKNAME     "Cookbook_GAIT26.pdf"
#define METADATANAME     "Metadata.pdf"
#define NUNANPONAME      "NUNANPO.pdf"
#define DOCD4NAME        "D4_SchemaInfo_GAIT26.pdf"
#define DOCVMAPNAME      "VMAP_SchemaInfo_GAIT26.pdf"
#define DOCFACCNAME      "FACC_SchemaInfo_GAIT26.pdf"
#define DOCMGCP3NAME     "MGCP3_SchemaInfo_GAIT26.pdf"
#define DOCMGCP4NAME     "MGCP4_SchemaInfo_GAIT26.pdf"
#define UFD1NAME         "UFD1_SchemaInfo_GAIT26.pdf"
#define UFD2NAME         "UFD2_SchemaInfo_GAIT26.pdf"
#define UFD3NAME         "UFD3_SchemaInfo_GAIT26.pdf"
#define DOCTDS4NAME      "TDS4_SchemaInfo_GAIT26.pdf"
#define DOCTDS6NAME      "TDS6_SchemaInfo_GAIT26.pdf"
#define DOCSACNAME       "AFD_SchemaInfo_GAIT26.pdf"
#define DOCSCHEMANAME    "DFDDNFDD_Info_GAIT26.pdf"
#define DOCGGDM21NAME    "GGDM21_SchemaInfo_GAIT26.pdf"
#define DOCGGDM22NAME    "GGDM22_SchemaInfo_GAIT26.pdf"
#define DOCGGDM3NAME     "GGDM3_SchemaInfo_GAIT26.pdf"
#define DOCXWALKNAME     "MasterCrosswalk_GAIT26.xls"
#define WHATSNEWNAME     "WhatsNew_GAIT26.pdf"
#define MGCPISF3NAME     "MGCP_IllegalSharedFaces_V10_TRD3.xlsx"
#define MGCPISF4NAME     "MGCP_IllegalSharedFaces_V15_TRD4.xlsx"
#define MGCPISL3NAME     "MGCP_IllogicalSharedLines_V13_TRD3.xls"
#define MGCPISL4NAME     "MGCP_IllogicalSharedLines_V13_TRD4.xls"
#define MGCPRANGE3NAME   "MGCP_NumericValueRange_V10_TRD3.xls"
#define MGCPRANGE4NAME   "MGCP_NumericValueRange_V7_TRD4.xls"
#define MGCPXMLNAME      "MGCP_XML_GAIT26.xls"
#define MGCPPPP3NAME     "MGCP_PointToPointProximity_V6_TRD3.xlsm"
#define MGCPPPP4NAME     "MGCP_PointToPointProximity_V8_TRD4.xlsm"
#define MGCPIAENAME      "MGCP_IllogicalAttributeEnumeration_V11_TRD4.xlsx"
#define TDSIAENAME       "TDS6_IllogicalAttributeEnumeration_V6.xlsx"
#define TDSISL4NAME      "TDS4_IllegalSharedCurves_V3.xls"
#define TDSISL6NAME      "TDS6_IllegalSharedCurves_V2.xls"
#define TDSISF4NAME      "TDS4_IllegalSharedFaces_V1.xls"
#define TDSISF6NAME      "TDS6_IllegalSharedFaces_V3.xls"
#define MGCPIPANAME      "MGCP_IPA_V6_TRD4.xlsx"
#define MGCPILANAME      "MGCP_ILA_V5_TRD4.xlsx"
#define MGCPIPLNAME      "MGCP_IPL_V3_TRD4.xlsx"
#define MGCPFTANAME      "MGCP_FTA_V2_TRD4.xlsx"


#define ZOOM_REMEMBER_NUM 10  

#define WRITETXT 1
#define WRITEBIN 1

#define INFO_BUFFER 8
#define HIGHLIGHTMAX 300

#define xlimit (990 + INFO_BUFFER)
#define ylimit (790 + INFO_BUFFER)

#define X_ZOOM_MAX 0.0003  
#define Y_ZOOM_MAX 0.0003  


#define NGAPREFIX  "DFEG_"
#define MGCPPREFIX "MGCP_"
#define UFD1PREFIX "UFD1_"
#define UFD3PREFIX "UFD3_"
#define TDSPREFIX  "TDS_"
#define TREXPREFIX "TREX_"
#define SACPREFIX  "AFD_"

#define deletesentinel     "::*??*??"  
#define deleteOpensentinel "::****??"  
#define errorsentinel "!!@@::**??"

#define DEFAULT_PAD_DIST 10.0
#define MIN_PAD_DIST 1.0
#define MAX_PAD_DIST 5000.0


#define IDSIZE 500

#define POLYCHUNK   15000
#define CIRCLECHUNK 50000  
#define LINECHUNK   15000 
#define GRIDCHUNK   50000 

#define MAX_GRAPH_NUM 250
#define NUM_DRAWING_PIXMAPS 6 
#define MAX_RENDER_PRI 5


#define MYPI 3.1415926535





#define STRING(a) XmStringCreateLocalized(a)
#define TOGGLESIZE 2

#define PIRATE 1
#define WATCH  2
#define GOOD   3
#define ARROW  4
#define WATCH2 5
#define GOOD2  6
#define SMALL_SHIFT_AMOUNT 1
#define SMALL 10.0
#define SHORTNUM 25000




struct IMarkList
{
  int origSCCindex;  
  struct IMarkList * next;
};

struct IMarkSACList
{
  int SCCindex ;   
  int SACindex ;   
  int EEindex  ;   
                   
  char *saveval;   
  struct IMarkSACList * next;
};



struct FilterList
{
  
  int index1, index2;
  struct FilterList * next;
} ;





typedef struct _menu_item {
  char        *label;          
  WidgetClass *cclass;         
  char         mnemonic;       
  char        *accelerator;    
  char        *accel_text;     
  void       (*callback)();    
  XtPointer    callback_data;  
  struct _menu_item *subitems; 
} MenuItem;

typedef struct _special_menu_item {
  char        label[50];     
  WidgetClass *cclass;       
  void       (*callback)();  
  int        callback_data;  
} SpecialMenuItem;



typedef struct my_box
{
  double x1,y1,x2,y2;
}Box; 


typedef struct my_data
{
  int check_num;
  int clonenum;
  int draw_sen;
  int csdstuff;
  int element;
  
  int axis;  
  int ADDER;
  int viewonly;
  int type;
  
  int group_num;
  int object_num;
  int level;  

  int geomtype;
  char *pathname;

  Widget Swid_to_pass[7],wid_to_pass,wid2_to_pass;
  XmOutlineWidget outlinewid;

}MyData; 


struct SACarray
{
  char *sac;         
  char *sacname;     
  char *desc;        
  char *lowdesc;     
  char *enums;       
  char *lowenumlab;  
  char *lowenumdef;  
};


struct SCCarray
{
  char *scc;      
  char *sccname;  
  char *desc;     
  char *lowdesc;  
  int marked;     
  int pos;        
};

struct CondHighlight
{
  
  unsigned short int errnum;
  unsigned short int instance;
  int condnum;
  long int fileposn;  
  struct CondHighlight * next;
};


struct EDCSLookup
{
  double value,value2;
  int type,intervaltype,scale,uom,count;
  char label[1000];
};



#if(USE_DOS==1)
typedef struct {
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
} stopWatch;

void startTimer( stopWatch *timer);
void stopTimer( stopWatch *timer);
double LIToSecs( LARGE_INTEGER * L);
double getElapsedTime( stopWatch *timer);
#endif


extern int machine_endianness;
extern int file_endianness;
extern int ConsultPreviouslyIgnored;

extern int SzShort; 
extern int SzI;  
extern int SzL;  
extern int SzD;  
extern int SzUC; 
extern int UseDMScoordinates;

extern void WriteMainOrIndexFileHeader(FILE *outfile, int type);
extern int determine_machine_endianness(void);
extern int LINETYPE ;
extern int POINTTYPE;
extern int mySHmax(int a, int b);
extern double EuclideanDistance(double x1,double y1,double x2,double y2);
extern double RegionSize;
extern struct CrossWalk *CrsWlk;  
extern struct CloneErrorTable *CloneErrorLookup;
extern struct ErrorTable ErrorLookup[];
extern struct ProfileList * ProfileRoot;
extern struct ProfileList * DrawProfileRoot;
extern struct FilterList  * FilterRoot;
extern struct ModelNames  * MdlNames;
extern int NumberOfModels;

extern char *NAMES[];
#if(NGA_TYPE==1)
extern unsigned char *DOMAINS;
extern unsigned char *STRATS;
#define INscc_array (10000) 
#define INsac_array (10000) 

#else
extern unsigned char DOMAINS[];
extern unsigned char STRATS[];
#define INscc_array (EDCS_TOTAL_EC+1)
#define INsac_array (EDCS_TOTAL_EA+1)
int INscc_loop = (EDCS_TOTAL_EC+1);
int INsac_loop = (EDCS_TOTAL_EA+1);
#endif





extern double Xtranslation;   
extern double Ytranslation;   
extern int CoordinateSystem;  

extern int FromI;
extern int ToI;
extern int FromJ;
extern int ToJ;
extern char indirectory[];
extern char outdirectory[];
extern int AbsoluteAccuracy;
extern double AllCells;

extern int iStart;
extern int iLimit;
extern int jStart;
extern int jLimit;
extern int MinXindex,MinYindex,MaxXindex,MaxYindex;
extern int NumberOfAttributesEntries;
extern struct AttrList *MdlNames2;
extern void DismissPreviouslyIgnored();
extern char IgnoredLineFile[];
extern char IgnoredPointFile[];
extern char errtypelog[];
extern char unsortlog[];
extern char *IllegalAreaCombos[];
extern int ILLEGALAREANUM;




extern char *MGCP3_IllegalAreaCombos[];
extern int MGCP3_ILLEGALAREANUM;
extern char *MGCP3_WarnAreaCombos[];
extern int MGCP3_WARNAREANUM;

extern char *MGCP4_IllegalAreaCombos[];
extern int MGCP4_ILLEGALAREANUM;
extern char *MGCP4_WarnAreaCombos[];
extern int MGCP4_WARNAREANUM;



extern int TDS4ISFNUM;
extern char TDS4_ISF[];
extern char *TDS4_ISF_Headers[];


extern int TDS6ISFNUM;
extern char TDS6_ISF[];
extern char *TDS6_ISF_Headers[];



extern char *IllegalHoleCombos[];
extern int ILLEGALHOLENUM;


extern void SetDefaultParticipants(int Check, int Clone, int Cloneindex);
extern void SetDefaultSensitivities(int Check, int Clone, int Cloneindex);
extern void FreeShapeMemory();
extern int ZeroNetworkFile(char filename[]);
extern int SilentGetCodeIndex(char *ecc);
extern int SilentGetAttrIndex(char *ecc);
extern int GetCodeIndex(char *ecc);
extern int GetAttrIndex(char *eac);
extern int GetNum(char *ecc);


extern unsigned char DrawContourLines;
extern double ContourInterval;



extern int NumLODbands; 
extern struct LODdisplay *LODindex; 








extern void TestSedNam(char *totest);
extern void TestVerts(int numverts, double Xverts[], double Yverts[]);
extern void TestVert(double Xvert, double Yvert);
extern double Gminx,Gminy,Gminz,Gmaxx,Gmaxy,Gmaxz;
extern int lines_found,points_found,lines_done,points_done,MaxSedLenFound;
extern struct SCCdata * SCCtable;
void non_fatal_error(Widget w,char message[],char title[]);
void los_info(Widget w,XtPointer data,XtPointer callData);
int realinfoprint(char msg[], int reallyprintit);    
void MakeGrid();
void read_logo();
void mapCallback(Widget w,XtPointer data,XtPointer callData);
void modifyCallback(Widget w, XtPointer userData, XtPointer callData);
void los_result(Widget w,XtPointer data,XtPointer callData);
void cleanse_events();
void refreshCallback(Widget w,XtPointer data,XtPointer callData);
void not_while_running(Widget w,char *message,int mode,char *title,int type);
Widget BuildMenu(Widget parent,int menu_type,char *menu_title,MenuItem *items);
Widget BuildImportMenu(Widget parent,int menu_type,char *menu_title,MenuItem *items);
void SortListbySCC(int p,int r);
void SortListbyDSCC(int p,int r);
void SortListbyOrder(int p,int r);
void SortListbyDOrder(int p,int r);

void ModelButton2Callback(Widget w,MyData  *mydata,XtPointer callData);
void ModelButton3Callback(Widget w,MyData  *mydata,XtPointer callData);
void ModelButton4Callback(Widget w,XtPointer clientData,XtPointer callData);


void ShellDeath(Widget widget, XtPointer userData, XtPointer callData);
void SetCSD(Widget w,int rebuild_win);
void savesettings(int batch,int type);
int fore_result(Widget w,XtPointer data,XtPointer callData);
void DrawTopGrid();
void set_cursor(Display *thedisplay,Window thewindow,int type);
void set_results(Widget w,XtPointer data,XtPointer callData);
void reporthandleCallback(Widget w, XtPointer userData, XtPointer callData);
void print_second_result(Widget text_w, MyData *userData, int *call_data);
void print_result(Widget text_w, MyData *userData, int *call_data, int tolnum);
void ActorCallback(Widget w, MyData *userData, XtPointer callData);
int closetogether(int System, double a, double b);
void set_color(Widget widget,char * client_data,XtPointer call_data);
void Iinvert(Widget w,XtPointer data,XtPointer callData);
void GenerateErrorEditor(int keepoldconfig);
void EnterFileCallback(Widget w,XtPointer data,XtPointer callData);
void BrowseCallback(Widget w,XtPointer data,XtPointer callData);
char * ParseAxisName(int axis);
void SetEndian(char testdirectory[]);
void shiftCallback(Widget w,XtPointer data,XtPointer callData);
void get_double(Widget w,XtPointer data,XtPointer callData);
int MY_ACTOR_UP(MyData *userData, int set);
int MY_MODEL_UP(MyData *mydata, int set);
int GetCloneIndex(int clonenum,int errnum);
double GetSens(int clonenum,int errnum, int primsecond);
void kill_all_model    (Widget w,MyData *mydata,XtPointer call_data);
void kill_attribute    (Widget w,MyData *mydata,XtPointer call_data);
void kill_model        (Widget w,MyData *mydata,XtPointer call_data);
void kill_all_attribute(Widget w,MyData *mydata,XtPointer call_data);
void real_kill_model   (Widget w,MyData *mydata,XtPointer call_data);
void CreateLongListEntries(int errnum,int axis, int ADDER, int batch, int clonenum, int cloneindex, int geomtype);
void ReallySaveDefaultsCallback2(Widget w,XtPointer data,XtPointer callData);
void CloneCallback(Widget w, XtPointer userData, XtPointer callData);
void ApplyExecutionOptions(char *exefile,int nettype);
int DoBatchEDCSStuff(Widget w,char * edcsfile,int lines_to_skip,int verbose);
char *GetADDType(int ADDER);
void GetEDCSInfo(int SCC, int SAC, int EEnum, int errnum, int axis, int ADDER);
void SaveAllSettingsFiles(FILE *savefile,int onlyifon,int nettype);
void loadExeOptionsfile(char *dir,int nettype);
void draw(Widget widget, XEvent *event, String *args, int *num_args);
void die_callback(Widget w,XtPointer client_data,XtPointer call_data);
void SetLabels();
void SetLabelsToDefault(int set);
void SAVE_ZOOM_STATE(double xscroll,double yscroll);
void PopZoomStack();
void TurnOffAllChecks(int nettype);
void RevertArrows();
void GetRidOfAllClones(int nettype);
void ScrubALLEDCSMarkerLists(int newdb, int nettype);
void ScrubModelIndices(int newdb, int nettype);
void ExecuteCallback(Widget w, XtPointer userData, XtPointer callData);
void ScrubEDCSMarkerList(int errnum, int axis, int ADDER, int clonenum, int cloneindex, int geomtype);
void ScrubEDCSMarkerList2(int errnum, int axis, int ADDER, int clonenum, int cloneindex, int geomtype);
void choose_specific_info(Widget unused,XtPointer client_data,XtPointer call_data);

void ImportGAITButtonCallback(Widget w,XtPointer data,XtPointer callData);
void ImportSEEITButtonCallback(Widget w,XtPointer data,XtPointer callData);
void Relief(int draw);
void BuildUNIQArray();
void con_info(Widget w,XtPointer data,XtPointer callData);
char *GetPwd();
void kill_run_format(Widget w,XtPointer data,XtPointer callData);
void ResetWidget(Widget w);
char *ParseAttributionType(int attr_type);
void MakeSen(Widget widget, XEvent *event, String *args, int *num_args);
char *GetShortGeomName(int geom);
void FillPage(int page_num);
void UpdateAllTotals();
void ConditionObjectCallback(Widget w, XtPointer userData, XtPointer callData);
void set_pad_distance(Widget w,XtPointer data,XtPointer callData);
void set_fcode(Widget w,XtPointer userData,XtPointer call_data);
void choose_specific_fcode(Widget w,XtPointer userData,XtPointer callData);
void makedisconnectCallback(Widget w,XtPointer data,XtPointer callData);
void ResetKeepDismiss(int value);
int GenerateExeFiles(int type);
void modeCallback(Widget w,XtPointer data,XtPointer callData);
Widget BuildPulldownMenu(Widget parent,char *menu_title,char menu_mnemonic, 
		  Boolean tear_off, MenuItem *items);
void LODCallback(Widget w, XtPointer userData, XtPointer callData);
void apply_layer_callback(Widget button, XtPointer client_data, XtPointer call_data);
void ClearBitmaps(int override);
void ClearDrawingPixmaps(int override);
void ZeroPixmapFlags();
int GetDefaultDrawPri(int ECC);
void FreeHighlighter();
int TestDB(char testdirectory[], int silent);
Widget MakePicker(Widget parent, int type, int num, int pass1, int pass2);
int GetFolders(char startlocation[],int type, int scrubextra);
void SortCondList(int p,int r, int type);
void SaveAOIFile();
void AOILoadsettingCallback(Widget w,Widget list_w,XtPointer unused);
void LoadLayerPrefCallback(Widget w,Widget list_w,XtPointer unused);

void draw_poly(Widget widget,XtPointer client_data,XtPointer call_data);
void all_toggle(Widget widget,XtPointer client_data,XtPointer call_data);
void Backcallback(Widget w,XtPointer data,XtPointer callData);
void Forecallback(Widget w,XtPointer data,XtPointer callData);
void AttrErr1Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr2Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr3Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr4Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr5Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr6Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr7Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr8Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr9Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr10Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr11Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr12Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr13Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr14Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr15Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr16Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr17Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr18Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr19Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr20Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr21Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr22Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr23Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr24Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr25Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr26Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr27Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr29Callback(Widget w,XtPointer data,XtPointer callData);
void AttrErr30Callback(Widget w,XtPointer data,XtPointer callData);
void ErrorSelectionCallback(Widget w,XtPointer data,XtPointer callData);
void AOIButtonCallback(Widget w,XtPointer data,XtPointer callData);
void SummaryCallback(Widget w,XtPointer data,XtPointer callData);
void grabCallback(Widget w,XtPointer data,XtPointer callData);
void LosParameterCallback(Widget w,XtPointer data,XtPointer callData);
void ShadeCallback(Widget w,XtPointer data,XtPointer callData);
void open_doc(Widget widget,XtPointer client_data,XtPointer call_data);
void sdcsCallback(Widget w,XtPointer data,XtPointer callData);




void ForeGridCallback(Widget w,XtPointer data,XtPointer callData);
int ParseSNumber(char *sname,int line, char *exefile);
int ParseDNumber(char *dname,int line, char *exefile);
void PrintUsage(char *progname);
void PrintHelp(char *progname);
void InitErrorStuff();
void InitOtherStuff();
void list_scroll_spec(Widget widget, XEvent *event, String *args, int *num_args);
void list_scroll(Widget widget, XEvent *event, String *args, int *num_args);
void list_scroll_bo(Widget widget, XEvent *event, String *args, int *num_args);
void list_scroll_vc(Widget widget, XEvent *event, String *args, int *num_args);
void list_scroll_net(Widget widget, XEvent *event, String *args, int *num_args);
void RetainError(Widget widget, XEvent *event, String *args, int *num_args);
Widget BuildPulldownMenu2(Widget parent, char *menu_title, char menu_mnemonic,Boolean tear_off, MenuItem *items);
void toolbarCallback(Widget w,XtPointer data,XtPointer callData);
void PutImageOnPixmap2(int image_num, int x, int y, Pixmap target_pixmap);
void PutImageOnPixmap(int image_num, int x, int y, Pixmap target_pixmap);
int tim_err_handle(Display *display, XErrorEvent *error);
void tim_xt_warn_handle(char *msg);
void cleantmp();
void DoBatchModeStuff(char *stf_locations,char *exop, int batchtype, char *ignoreproj);
void DoNGABAtchStuff(char *argv0, char *dirlistfile,char *projname,char *exop,char *ignoreproj,char *exefile);
void SetToggles();
void init_drawbytype();
void BuildDynamicArrayOfScc(char fname[]);
void set_cursor(Display *thedisplay,Window thewindow,int type);
void DynCCallback(Widget w,int data,XtPointer callData);
void DynSCallback(Widget w,int data,XtPointer callData);
void DynDCallback(Widget w,int data,XtPointer callData);
int BuildArrayOfScc(Widget w,char fname[]);


void FACCCallback2(Widget w,XtPointer data,XtPointer callData);
void SaveDefaultsCallback2(Widget w,XtPointer data,XtPointer callData);
void FormatHelpCallback2(Widget w,XtPointer data,XtPointer callData);
void myResizeHandler(Widget w, XtPointer userData, XEvent *event, Boolean *continue_to_dispatch);
void SetEndian(char testdirectory[]);
void BuildDynamicArrayOfScc(char fname[]);
int BuildArrayOfScc(Widget w,char fname[]);
void CCallback(Widget w,int data,XtPointer callData);
void SCallback(Widget w,int data,XtPointer callData);
void DCallback(Widget w,int data,XtPointer callData);

Widget SpecialBuildMenu(Widget parent,SpecialMenuItem *items,int numitems,unsigned char selection);
void FACCCallback(Widget w,XtPointer data,XtPointer callData);
void FormatHelpCallback(Widget w,XtPointer data,XtPointer callData);
void FormatCallback(Widget w,XtPointer data,XtPointer callData);
void SetESName();
void SetNSName();
void SetLKName();
void SetBOName();
void SetNTName();
void SetSMName();
void SetVRName();
void exitCallback(Widget w, XtPointer userData, XtPointer callData);
void InitializeAttribution();
int MakeOutStuff(char testdirectory[]);
int CheckDataContent(char *testdir, int silent);
void not_while_running(Widget w,char *message,int mode,char *title,int type);
void WriteDynamicDefs();
int ProjectContainsLNfile(char *value);
int ProjectContainsPTfile(char *value);

void clobberCallback(Widget w, XtPointer userData, XtPointer callData);
char timtoupper(char inchar);
void MakePixmap3();
void DrawMI();
void MakePixmap3();
void DrawAll();
void BuildSACArray();
void BuildSCCArray();
int UpdateProjectLoc(char *testloc);
void print_result2(Widget text_w,XtPointer client_data,XtPointer call_data);
char timtoupper2(char inchar);
void redraw_net(Widget graph, XtPointer client_data, XtPointer call_data);
void SetColorRight();
void info_sac_tog(Widget widget,XtPointer client_data,XtPointer call_data);
void toggled8(Widget widget,XtPointer client_data,XtPointer call_data);
void set_vpad_distance(Widget w,XtPointer data,XtPointer callData);
void DrawGrid();
double mydmax(double first, double second);
void SetSelected(int errnum, int cloneindex, int onoff);
void SortHighlighter(int p,int r, int pass);
char *GetCondDetails(int type, int len2, int ecc1, int ecc2, double magnitude, int errnum, int j,
		     int codelen, int codelen2, int labellen, int labellen2, int coverfailnum);
void FlushMainScreen(int clean);
void saveaoiCallback(Widget w, XtPointer userData, XtPointer callData);
void saveaoihelpCallback(Widget widget, XtPointer userData, XtPointer callData);
void MakeInfo(int type);
void WriteSummaryReport(int type);
void SortAOIList(int p,int r, int type);
void SortPrefList(int p,int r, int type);
void ModifyAOIList(int num);
void DrawInfoItem(struct InfoPrintContent *curr);
void UnderlineSpecialVertex();
void TransferCleanPixmap();
void DrawSpecialInfoVertex();

void FreeInfoList();
void ResetZoom();
void SetToggleVariables();
void SaveCleanPixmap();
char * MakeUString(double x,double y);
void TurnOnAll(int setval, int *sen1, int *sen2, int set1, int set2);
void MyDrawErrors();
void real_periodic_redraw();
int DeleteFromIMarkList(int index, int errnum, int axis, int ADDER, int clonenum, int cloneindex, int geomtype);
int DeleteFromIMarkSACList(int index, int errnum, int axis, int ADDER, int clonenum, int cloneindex, int geomtype);
int GetCode(int SCC, int SAC);
void searchEDCS_callback(Widget w, XtPointer client_data, XtPointer call_data);
void toggle_combo(Widget widget,MyData *mydata,XtPointer call_data);
void Help_modCallback(Widget w,XtPointer data,XtPointer callData);
void kill_model_callback(Widget button, MyData *mydata, XtPointer call_data);
void ModelDeath(Widget widget, MyData * userData, XtPointer callData);
void AddHighlightCondition(short int x,short int y, int errnum, int instance, int condnum, int fileposn);
void CleanScreen();
void STFFileCallback(Widget w,XtPointer data,XtPointer callData);
void PatchFileCallback(Widget w,XtPointer data,XtPointer callData);
char *GetVariousPeices(int type);
char * MakeCleanCode(char *oldcode);
char * MakeCleanCode2(char *oldcode);
char * MakeCleanCode3(char *oldcode);
int SetNUNANPO();
int TestOut(char *testdir);
int GetTotalIndiv(int *errindex, int *begin, int *end, int *cindex);
int GetTotalNonIndiv();
int mymax(int a, int b);
void show_vertices(Widget widget,XtPointer client_data,XtPointer call_data);
void ZoomStackCallback(Widget w, XtPointer userData, XtPointer callData);
int FindEDCSInFile(char *dir);
int ICheckRegion(int iStart,int iLimit,int jStart,int jLimit);
void BatchSaveResults();
void WriteMetaFile();
void SetIndex(int errnum, int axis, int ADDER, int Lindex, int clonenum, int cloneindex, int geomtype);
void CloseTopLevelStuff();
void RebuildTopImage(int type);
void infoprintbold(char msg[]);    
void infoprintunderline(char msg[]);    
char * ProcessGAITPrefFile(char path[]);
int testvalues(char line[], char geom[],char fcode[],int rendertype,int styletype,int red,int green,int blue);
int GetLayerNum(int geom, int ECC);
void FillInDefaults();
void save_layer_prefs(Widget w, XtPointer userData, XtPointer callData);
void ChoosePrefsCallback(Widget w,XtPointer data,XtPointer callData);
void SetLayerPrefCallback(Widget w, Widget list_w, XtPointer callData);
void InitLayerStuff();
void FillInRenderOrder();
void NGAMakeNewAttr(char *domainfile,char *combofile,char *classdeffile, char *attrdeffile,
		    char *newclassfile, char *newattrfile, char *newenumfile, 
		    char *newnunanpoenum, char *header, char *newnunanpoheader);
void NGAMakeAttrCallback(Widget w,XtPointer data,XtPointer callData);
void Record_SAVE_TOPCHANGE(int type);
char *ParseDName(int num);
void MakeAllShapefiles(Widget widget,XtPointer data,XtPointer callData);
void DeleteAOIList();
void ShowRegionMarkers();
void AddToAOIList(int type, char *annotation, double x, double y);
void DeleteSingleAOI(int num);
void DeleteAOIList();
void MAPdrawstring(double x1,double y1,char thestring[],int color, int yoff, int Lindex, int adjust);
void SaveAOIShapes();
int WriteAOIShapes(Widget w,char *outname);
char * MakeUString2();
void kill_model_callback2(Widget button, MyData *mydata, XtPointer call_data);
void kill_model_callback3(Widget button, MyData *mydata, XtPointer call_data);
int IS_MGCP_OTHER(int ECC);
int IS_MGCP_ACTIVITY(int ECC);
int IS_MGCP_LANDCOVER(int ECC);
void UpdateSummaryInfo(int type,char *inmessage,char *intime,char *title);
char *GetSWLat(double *djunk);
char *GetSWLong(double *djunk);
char * PrintTime(long int TheStartTime);
void zerooutCallback(Widget w, XtPointer userData, XtPointer callData);
void revertCallback(Widget widget, XtPointer client_data, XtPointer callData);
void ResetBOoptions();
void Read_SAVE_TOPCHANGE();
void BadDB(int silent,char tester[]);
void RemoveAnnotations(int nettype);
void SaveLODColors(Widget w);
void SortLayers(Widget w, XtPointer userData, XtPointer callData);
void ShellDeath2(Widget widget, XtPointer userData, XtPointer callData);
void preloadCRsettingCallback(Widget w,Widget list_w,char filename[]);
void prevsetCallback(Widget w,Widget list_w,char filename[]);
void preloadsettingCallback(Widget w,XtPointer intype,XtPointer unused);
void BuildNetworkViewer(Widget parent, XtPointer client_data, XtPointer call_data);
void FolderCallback(Widget w, XtPointer userData, XtPointer callData);
void SavereportCallback(Widget w, XtPointer userData, XtPointer callData);
void LoadreportCallback(Widget w, XtPointer userData, XtPointer callData);
void CompareReportCallback(Widget w, XtPointer userData, XtPointer callData);
void XMLCallback(Widget w, XtPointer userData, XtPointer callData);
void DPCallback(Widget w, XtPointer userData, XtPointer callData);
void quitCallback(Widget widget, XtPointer userData, XtPointer callData);
void helpCallback(Widget widget, XtPointer userData, XtPointer callData);
void creditCallback(Widget widget, XtPointer userData, XtPointer callData);
void FixDifferentDomains(int verbose);
void do_nothing(Widget widget, XEvent *event, String *args, int *num_args);
void do_nothing2(Widget widget, XEvent *event, String *args, int *num_args);
void remove_add_mode(Widget widget, XEvent *event, String *args, int *num_args);
void redraw(Widget drawing_a,XtPointer client_data,XtPointer call_data);
void SortProjList(int p,int r, int type);
void ModifyProjList(int num, int *foundfeat, int *foundgrid);
int TestShort(char testdirectory[]);
void myResizeWidthHandler(Widget w, XtPointer userData, XEvent *event, Boolean *continue_to_dispatch);
void Atom1Callback(Widget widget, XtPointer userData, XtPointer callData);
void Atom2Callback(Widget widget, XtPointer userData, XtPointer callData);
void Greyit();
void ClearErrorBitmap();
int find_col_index();
void do_logo();
void myMenuHandler(Widget w, XtPointer userData, XEvent *event, Boolean *continue_to_dispatch);
void GetDriveLetters();
char *ParseDriveType(int type);
void folderhelpCallback(Widget widget, XtPointer userData, XtPointer callData);
void FreeWidList(int type, int destroyalso);
void ResetMBWidget(Widget w);
void AddWidToList(int type, Widget wid, Widget subhandle, int level);
void toggle_dos(Widget widget,XtPointer client_data,XtPointer call_data);
void SortFolderList(int p,int r, int type);
void SortFileList(int p,int r, int type);
Widget DosBrowseCallback(Widget w,XtPointer data,XtPointer callData);
void importoutput2fileDialogCallback(Widget w, XtPointer userData,XmFileSelectionBoxCallbackStruct *data);
void ModifyCondList(int num);
void MyPopup(Widget wid);
void HandleSuccessDos(Widget w);
void NEWToggleButtonHandleCB(Widget w, MyData  *closure, XtPointer call_data);
void RefreshLocs();
void HandleSuccessDos(Widget w);
void SortNetList(int p,int r);
void SetZoomValues(double centerLLX,double centerLLY,double centerURX,double centerURY);
void SetScrollbars();
double AddVoidSpace(int xy);
double ScrollPercent(int xy);
int GetLongAttrVal(int type,int uniqueid,int *Ival,double *Dval,char *Cval);
int IsMetadata(char *attr_code);
void HandlePostDescribe();
FILE *GetLongAttrFilePtr(int InfoOrData,int AttrType);
void Begin_LongFiles(int type);
void End_LongFiles(int type);
int SpecialStrcmp(char ina[], char inb[], int type);
void BuildLLTable(int override, int size); 
void FreeLLTable(); 
void FreeMarkerList();
int GetLongAttrType(int code, int silent);
char *GetLongCode (int attr_type);
int GetGeom(char *inname);
char * ParseCName(int num);
double GetXFractionVisible();
double GetYFractionVisible();
void SetFidMarkTable(int attrtype, char *onval, int ecc, char *geom, int errnum, int axis, int ADDER, int clonenum, int cloneindex);
void myLogoHandler(Widget w, XtPointer userData, XEvent *event, Boolean *continue_to_dispatch);
int OneDegreeCheckOK(int *totalnum);
void ResetMasterRun();
void BuildMasterRun(double percentdone);
void BuildFIDTable(int override, int size);
void FreeFidTable();
void RemoveOldFiles();
void FillInCoverFail();
void StopDistanceGathering();
void CountIgnored(int *ignore_num, int *retain_num,char filename[], int silent);
void BuildEntries(Widget suboutline, int i, int clonenum, int nettype);
void AddSilentStuff(char *message, int type);
unsigned char AssignDrawingColor(int fid, unsigned char D, int crsindex);
void profileCallback(Widget w,XtPointer data,XtPointer callData);
char *GetCoordString(int type, double coord);
void Begin_FormatCache();
void End_FormatCache();
void GetShortName(char shortname[],char *shape_to_import);
int  OpenGeoDatabase(int AccessOrFile, char *filename, int MakeInputSpec,
							  int addtolong, int addtoshort,
		                      int *longitemsfound, int *shortitemsfound);
int BuildLongAttrValList(int SCC,int longcode,XmString new_items[], int doxmstring, int specialtype);
int SilentConvertClassCode(char *oldcode);
char *GetFrontPart();
char *MakeCleanString(char *oldcode);
char *MakeCleanString2(char *oldcode);
void ImportDestinationCallback(Widget w,XtPointer data,XtPointer callData);
void ImportHelpCallback(Widget w,XtPointer data,XtPointer callData);
void RectangleCallback(Widget w,XtPointer data,XtPointer callData);
void Import2Callback(Widget w,XtPointer data,XtPointer callData);
void ngainfileCallback(Widget w,XtPointer data,XtPointer callData);
void ExitWrapper(int flag);
int DoBlankAttr();
int AddGeoTiffFile(FILE *infile,char *GeoTiffFile, int type, char *annotation);
int import_geotiff(char *filename, char *allshapes, int files_done, char *lod_override, int addtolong, int addtoshort, int type);
void ImportFormatCallback(Widget w,XtPointer data,XtPointer callData);
void CacheGrid(FILE *gridfile,long int flag);
void AllocateExtraColors();
void AskZero(int type);
void DrawRealShadedGrid(char *filename,long int fileposn);
void DrawShadedGrid(char *filename,long int fileposn);
int AbortZoom();
void ResetContours();
void openProjectCallback(Widget w,Widget list_w,XtPointer unused);
void openProjectCallbackClean(Widget w,Widget list_w,XtPointer unused);
int BadConditions(int type, Widget w, Widget list_w, int flag);
void Import3Callback(Widget w,XtPointer data,XtPointer callData);
void Import3CallbackClean(Widget w,XtPointer data,XtPointer callData);
Widget GetImportShell();
void CombineExeOptionsFiles(char *file1,char *file2,char *newfilename);
int PossibleForNUNANPO(int test);
void RemoveOldExtractFiles();
void RemoveLayerFile();
int BadShapeInput();
int FileExists(char *filename);
void WritePOIDBHeader(FILE *outfile, int num_records,int maxlen);
void WritePOIDBEntry(FILE *outfile,char *annotation, int maxlen, int type,
					 double x, double y);
void MySetLineThickness(int thickness);
void SetShadedColor(int color);
void ReSetVertexList(int num);
void UnTranslateCoords(double *x, double *y);
int TotalGridLayers();
int NUM_CHECK(char *value);
void gridmenuCallback(Widget w,XtPointer data,XtPointer callData);
void InitGrid();
void BrowseFolderCallback(Widget w, XtPointer userData, XtPointer callData);
int IsValidDirectory(char *possiblefolder);
char *MakePlural(int num);
void SetDPFolderCallback(Widget w, XtPointer userData, XmFileSelectionBoxCallbackStruct *data);
void RememberLabelPixel(unsigned short int x,unsigned short y);
void ComputeOverAdjust(double x1,double y1,int *overadjustx,int *overadjusty);
void PrefsCallback(Widget w, XtPointer userData, XtPointer callData);
void RealSavePrefs();
void OverWritePrefs(Widget w,XtPointer data,XtPointer callData);
void LoadPrefCallback(Widget w, Widget list_w, XtPointer callData);
void savePrefCallback(Widget w, XtPointer userData, XtPointer callData);
void ProcessEnvVars(FILE *envfile, int flag);
int msz(unsigned char value);
char * CompareInspectionResults(char *f1name, char *f2name, char *f1type, char *f2type, int verbosity);
int ParseXML2(int trd_version, int doxsd, char *filename, char *xsdname, char *refinefilename, char *outfilename,
						 int *schema_errors, int *nonschema_errors, int *shapefile_errors, int expected_version);
int ParseErrNumberWrapper(char *errname,int line, char *exefile);
void kill_fcode(Widget w,XtPointer data,XtPointer call_data);
void kill_all_fcode(Widget w,XtPointer data,XtPointer call_data);
void Help_fcodeCallback(Widget w,XtPointer data,XtPointer callData);
void kill_fcode_callback(Widget button,XtPointer client_data, XtPointer call_data);
void SetNewPage();
void CheckFilterCallback(Widget w, XtPointer userData, XtPointer callData);
void borevertCallback(Widget w, XtPointer userData, XtPointer callData);
void bodeselectCallback(Widget w, XtPointer userData, XtPointer callData);
void GetGlobalTotals(int *totalactive,int *totalchecks);
void ToggleButtonHandleCB(Widget w,MyData  *closure, XtPointer call_data);
void GetTotals(int groupnum,int *totalactive,int *totalchecks);
int GetActiveNum(int checknum);
void toggled(Widget widget,XtPointer client_data,XtPointer call_data);
void SaveSettingsCallback(Widget w, XtPointer userData, XtPointer callData);
void LoadSettingsCallback(Widget w, XtPointer userData, XtPointer callData);
void InfoCallback(Widget widget, XtPointer client_data, XtPointer callData);
void new_value2(Widget w, XtPointer client_data, XtPointer call_data);
void SortPrevList(int p,int r, int type);
void reset_layer_color(Widget pb, XtPointer client_data, XtPointer call_data);
int MGCPFound(int thisdomain);
void layermgcp_onCallback(Widget w, XtPointer userData, XtPointer callData);
void layermgcp_offCallback(Widget w, XtPointer userData, XtPointer callData);
void layermgcphelpCallback(Widget w,XtPointer data,XtPointer callData);
void TogLODCallback(Widget widget,XtPointer client_data,XtPointer call_data);
void select_layers_by_mgcp(Widget w, XtPointer userData, XtPointer callData);
void PatchCreateCallback(Widget w,MyData *mydata,XtPointer callData);
void SelectRetainedOrIgnored(MyData *mydata, int retainignore);
void kill_pad_callback(Widget button, XtPointer client_data, XtPointer call_data);
void NETGetGlobalTotals(int *totalchecks,int *totalconds,int *totalhighlighted);
void NETGetTotals(int groupnum,int *totalchecks, int *totalconds, int *totalselected);
void NETGetTotal(int errnum, int *totalconds, int *totalchecks, int *totalhighlighted);
void VCToggleButtonHandleCB(Widget w, MyData  *closure, XtPointer call_data);
void toggle_view_some(Widget widget,MyData *mydata,XtPointer call_data);
void toggle_view_all(Widget widget,MyData *mydata,XtPointer call_data);
void Help_errCallback(Widget w,XtPointer data,XtPointer callData);
void DrawPolyWithHoles(int num_external_verts, double *externalX, double *externalY, int color, int numholes, struct AreaRing *holes, int Lindex);
void VCGetGlobalTotals(int *totalchecks,int *totalconds, int *totalhighlighted);
void VCGetTotals(int groupnum,int *totalchecks, int *totalconds, int *totalselected);
void VCGetTotal(int errnum, int *totalconds, int *totalchecks, int *totalhighlighted);
void BuildSpecificCond(Widget parent, int errnum, int clonenum, int scr_width);
void NETmodifyCallback(Widget w, XtPointer userData, XtPointer callData);
void GetNetGlobalTotals(int *totalactive,int *totalchecks);
void NetConfigToggleButtonHandleCB(Widget w,MyData  *closure,XtPointer call_data);
int IsNetCheck(int checknum);
void loadEXEsettingCallback(Widget w,Widget list_w,char filename[]);
void loadNETsettingCallback(Widget w,Widget list_w,char filename[]);
void preloadNETsettingCallback(Widget w,XtPointer intype,XtPointer unused);
void quick_network(Widget w,XtPointer userData,XtPointer call_data);
void quick_network2(Widget w,XtPointer userData,XtPointer call_data);
void DrawNetworksWrapper(int TgtCheck,int TgtCnumber, int TgtIndexStart, int TgtIndexStop);
void TogDiscreteCallback(Widget widget,XtPointer client_data,XtPointer call_data);
void TogOutlineCallback(Widget widget,XtPointer client_data,XtPointer call_data);
void DPwrapperCallback(Widget w, XtPointer userData, XtPointer callData);
void EstablishVVCaching(int type);
int AssignTREXColor(int layer_num);
int StringCaseInsensitiveMatch(char * a, char *b);
void AllocArealPixmaps();
void FreeArealPixmaps();
int ParseXML3(char *filename, char *outfilename, int *errors, char *rootname, int finished_raw, double northing,double easting);
int DO_FAN();
int FindAttr(double dval,int passtype,char *geometry,char *charSCC, char *charSAC, char *targetvalue);
char *GetUnderGroundName(int english);
char *GetLOCName(int english);
char *GetCDACode(int english);
char * ProcessGAITCondReport(char path[], int type);
int NUM_Getter(char *attrcode,char *geomstring,char *fcode,double *minVAL,double *maxVAL, 
			   double minaccept,double maxaccept, int filter_out_nulls, 
			   double *smallestOVER, double smalloverlimit,
               double *largestUNDER, double largeunderlimit);
int SingleInstance(int checktype);
char *GetAREAName();
char *GetHGTName();
char *GetZVName();
char *GetLENName();
char *GetNAMName();
char *GetSlash();
char *GetTRExSubName2(char *thepath, int id);
char *GetTRExType(char *projectpath);
