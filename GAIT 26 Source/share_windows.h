
#define NGA_TYPE 1   

#define USE_DOS 1
#define USE_MAC 0
#define USE_LINUX 0

#define MakeAttr 0

#define DRAW_COLOR_BLACK 51      
#define DRAW_COLOR_WHITE 255     
#define DRAW_COLOR_YELLOW 28     
#define DRAW_COLOR_MEDGREEN 50   
#define DRAW_COLOR_MEDGREEN2 200 
#define DRAW_COLOR_BROWN 53      
#define DRAW_COLOR_LAVENDER 52   
#define DRAW_COLOR_LTGREEN 27    
#define DRAW_COLOR_DKCYAN 29     
#define DRAW_COLOR_PURPLE 40     
#define DRAW_COLOR_LTCYAN 30     
#define DRAW_COLOR_BLUE 55       
#define DRAW_COLOR_TAN   54      
#define DRAW_COLOR_GRAY64  64    
#define DRAW_COLOR_GRAY128  128  
#define DRAW_COLOR_GRAY192  192  
#define DRAW_COLOR_GRAY4 57      
#define DRAW_COLOR_MAGENTA 59    
#define DRAW_COLOR_SALMON 60     
#define DRAW_COLOR_RED 1         
#define DRAW_COLOR_ORANGE 31     
#define DRAW_COLOR_SP_RED 299   
#define TREX_EDGE_POINT_CODE "ZB060"


#define num_meta 32
#define num_MGCP_meta 15
#define num_TDS_meta 22

#define DOS_TYPE 1     
#define NONDOS_TYPE 0

#define BLACK_R 0
#define BLACK_G 0
#define BLACK_B 0

#define WHITE_R 65535
#define WHITE_G 65535
#define WHITE_B 65535

#define RED_R 65535
#define RED_G 0
#define RED_B 0

#define GRAY64_R 16384
#define GRAY64_G 16384
#define GRAY64_B 16384

#define GRAY128_R 32768
#define GRAY128_G 32768
#define GRAY128_B 32768

#define GRAY192_R 49152
#define GRAY192_G 49152
#define GRAY192_B 49152

#define GRAY4_R 24480
#define GRAY4_G 24480
#define GRAY4_B 24480

#define MAGENTA_R 65535
#define MAGENTA_G 0
#define MAGENTA_B 65535

#define SALMON_R 65535
#define SALMON_G 46592
#define SALMON_B 49408

#define YELLOW_R 65535
#define YELLOW_G 55040
#define YELLOW_B 0

#define BLUE_R 0
#define BLUE_G 0
#define BLUE_B 65535

#define TAN_R 62464
#define TAN_G 41984
#define TAN_B 24576


#define LTGREEN_R 0
#define LTGREEN_G 65535
#define LTGREEN_B 0

#define PURPLE_R 32768
#define PURPLE_G 0
#define PURPLE_B 65535

#define LTCYAN_R 0
#define LTCYAN_G 65535
#define LTCYAN_B 65535

#define DKCYAN_R 34560
#define DKCYAN_G 52736
#define DKCYAN_B 60160


#define MEDGREEN_R 0
#define MEDGREEN_G 39680
#define MEDGREEN_B 0

#define MEDGREEN2_R 0
#define MEDGREEN2_G 25600
#define MEDGREEN2_B 0

#define BROWN_R 35584
#define BROWN_G 17664
#define BROWN_B 4864

#define LAVENDER_R 32512
#define LAVENDER_G 53248
#define LAVENDER_B 65280

#define ORANGE_R 65535
#define ORANGE_G 42240
#define ORANGE_B 0



#define NUMCHECKGROUPS 15
#define ModelNameMaxLength 90


#define SHAPE_IGNORE_LIMIT 100000
#define DEM_GROUP        -999
#define SIZE_GROUP          0
#define COMPO_GROUP         1
#define DUP_GROUP           2
#define UNDERSHOOT_GROUP    3
#define CONNECT_GROUP       4
#define CONTAIN_GROUP       5
#define ZVAL_GROUP          6
#define SEPDIST_GROUP       7
#define KINK_GROUP          8
#define INTERSECT_GROUP     9
#define OVERLAP_GROUP       10
#define MATCH_GROUP         11
#define RANGE_GROUP         12
#define NEW_DEM_GROUP       13
#define ATTR_GROUP          14

#define GAITcheck 1
#define SEEITcheck 2
#define GAITandSEEITcheck 3
#define BUSTEDcheck       -1



#if(NGA_TYPE==1)
#define VersionStr "26"

#include "GAIT_API.h"

#define OVERC             246
#define TVERT             247
#define LMINT             232
#define LSPINT            233
#define LSPIEXP           234
#define POLYINAREA        235
#define POLYOSIDEAREA     236
#define POLYINTPOLY       237
#define POLYINTAREA       238
#define HSLOPE            239
#define VERTSLOPE         240
#define VTEAR             241
#define NARROW            242
#define HTEAR             243


#define SMALLOBJ          244
#define ATTRERR           245

#else
#include "SEEIT_API.h"

#define VersionStr "2.2"

#endif


#include <Xm/Xm.h>

#include <stdio.h>






#define NUM_NGA_C 4 
#define NGA_AREA 0
#define NGA_LINE 1
#define NGA_POFE 2
#define NGA_GRID 3


#ifndef DATA_TYPE
#define DATA_TYPE void *
#endif
#ifdef DMALLOC
#include <dmalloc.h>
#endif

typedef struct stk_stack_node {
  DATA_TYPE info;
  struct stk_stack_node * next;
} stk_stack_node;

typedef struct stk_stack {
  stk_stack_node * top;
  stk_stack_node * tail;
} stk_stack ;

typedef struct rb_red_blk_node {
  void* key;
  void* info; 
  int red; 
  struct rb_red_blk_node* left;
  struct rb_red_blk_node* right;
  struct rb_red_blk_node* parent;
} rb_red_blk_node;


typedef struct rb_red_blk_tree {
  int (*Compare)(const void* a, const void* b);
  void (*DestroyKey)(void* a);
  void (*DestroyInfo)(void* a);
  void (*PrintKey)(const void* a);
  void (*PrintInfo)(void* a);
  rb_red_blk_node* root;
  rb_red_blk_node* nil;
} rb_red_blk_tree;







#define FAKE_ACE_VALUE -438971

struct SubregionHoleStruct
{
   int numverts;
   int usethis;
   int HoleID;
   int subregionid;
   double *x;
   double *y;
};


struct SubregionPolyStruct
{
   double foundx;
   double foundy;
   int numverts;
   int PolyID;
   int subregionid;
   int num_ftype;
   int FoundFeatureType;
   int numholes;
   double ACE;
   double ALE;
   int ACE_pick; 
   int ALE_pick; 
   char *SCPYRT;
   struct SubregionHoleStruct *holes;
   char **ftypes;
   double *x;
   double *y;
};


struct SubregionSourceStruct
{
   int type;  
   int index; 
   int subregionid;
   int year;
   int month;
   int day;
   int attr_enum_code;
   int MatchedOldest;
   int MatchedNewest;
   char savedate[100];
};

struct SubregionDataStruct
{
   int subregionid;
   int ace_index;
   int ale_index;
   double ace_value;
   double ale_value;
   char *scpyrt;
};


struct AreaRing
{
   long int seekposn;
   int numverts;
   double *x;
   double *y;
   double *z;
   struct AreaRing *next;
};


struct AreaAndHoles
{
   int idn,Lindex;
   int maxXindex;
   int numholes;
   unsigned char drawcolor;
   struct AreaRing *outer;
   struct AreaRing *inner;
   struct AreaAndHoles * next;
};

struct MultiGrids
   {
   int idn;
   int localident;
   int numX;
   int numY;
   int actualindex;
   char OnEdge;
   double Xspace;
   double Yspace;
   double SouthWestX;
   double SouthWestY;
   double *Elevs;
   };

struct POIList
{
  
  int type;
  double x,y;
  char annotation[255];
  struct POIList * next;
} ;




struct FirstList
{
  char *first;
  struct FirstList * next;
};

struct AStrings
{
  char *code;
  char *Label;
  char *value;
  char *valueLabel;
  char * SEDRISxtra;
} ;
struct InfoPrintContent
{
  int longindex;
  int ECC;
  int geom;
  int FID;
  int ishole;
  int numcoords;
  int numattr;
  int numholes;
  unsigned char domain;
  char * IDstr;
  char * ID2str;
  double length;
  double area;
  double radius;
  double *X;
  double *Y;
  double *Z;
  struct AreaRing * innerings;
  struct AStrings * A;
  struct InfoPrintContent *next;
};









#define FLAGSIZE 30
#define SH_LISTLEN 1000 














#define COORD_SYSTEMS 13


#define COLOR_TAN     54
#define COLOR_BROWN   53
#define COLOR_GREEN   27
#define COLOR_BLUE    55
#define COLOR_PURPLE  40
#define COLOR_BLACK   51
#define COLOR_RED      1
#define COLOR_WHITE  255




#define NUM_OBS 12
#define E_POLY     0   
#define E_LINEAR   1   
#define E_AREAL    2   
#define E_POINT    3   
#define E_POMO     4
#define E_GRID     5   
#define E_COVOS    6
#define E_COVOP    7
#define E_MOLI     8
#define E_FMPF     9   
#define E_FMLF     10   
#define E_FMAF     11  



#define C_NONE  0
#define C_AREA  1
#define C_DILI  2
#define C_FOMO  3
#define C_LINE  4
#define C_POFE  5
#define C_POMO  6
#define C_POLY  7
#define C_DYMO  8
#define C_GRID  9
#define C_COVS  10
#define C_COVP  11
#define C_MOLI  12
#define C_FMPF  13
#define C_FMLF  14
#define C_FMAF  15



#define S_NONE 0
#define S_APPL 1
#define S_COVE 2
#define S_INAC 3
#define S_SUBS 4
#define S_SURF 5





#if(NGA_TYPE==1)



#define D_NONE 0
#define D_AIMO 0
#define D_AIOB 0

#define D_BRID 0
#define D_CULT 0

#define D_LAMO 0
#define D_LAOB 0
#define D_MIUS 0
#define D_OPWA 0
#define D_TERR 0

#define D_MAMO 0
#define D_MAOB 0
#define D_MACU 0
#define D_WEAT 0
#define D_SPAC 0
#define D_DESC 0




#define D_AERO 0
#define D_AITO 1
#define D_BOUN 2
#define D_DAQU 3
#define D_DEST 4
#define D_ELEV 5
#define D_GRTR 6
#define D_INWA 7
#define D_MALI 8
#define D_OCEN 9
#define D_PHYS 10
#define D_POPU 11
#define D_POAN 12
#define D_SLPO 13
#define D_SUMA 14
#define D_UTAN 15
#define D_VEGE 16
#define D_GENE 17
#define D_GREL 18



#else




#define D_NONE 0
#define D_AIMO 1
#define D_AIOB 2
#define D_BOUN 3
#define D_BRID 4
#define D_CULT 5
#define D_INWA 6
#define D_LAMO 7
#define D_LAOB 8
#define D_MIUS 9
#define D_OPWA 10
#define D_TERR 11
#define D_VEGE 12
#define D_MAMO 13
#define D_MAOB 14
#define D_MACU 15
#define D_WEAT 16
#define D_SPAC 17
#define D_DESC 18



#define D_AERO 0
#define D_AITO 0

#define D_DAQU 0
#define D_DEST 0
#define D_ELEV 0
#define D_GRTR 0

#define D_MALI 0
#define D_OCEN 0
#define D_PHYS 0
#define D_POPU 0
#define D_POAN 0
#define D_SLPO 0
#define D_SUMA 0
#define D_UTAN 0

#define D_GENE 0
#define D_GREL 0



#endif


#define SAC_MAX 357000 



#define EARTH_RAD 6378145





struct ArealAsRead
{
  int Lindex;
  int Sindex;
  int idn;
  int featureid;
  int ishole;
  double areaattr;
  double hgtattr;
  double zv2attr;
  double namattr;
  double lenattr;
  double minxvtx;
  double area;
  double perimlength;
  unsigned char config;
  unsigned char stratum;
  unsigned char domain;
  char areastr[50];
  char hgtstr[50];
  char zv2str[50];
  char lenstr[50];
  char namstr[500];
  int numverts;
  int MaxVerts;
  double * x;
  double * y;
  double * z;
} ;

struct LinearAsRead
{
  int featureid;
  int Lindex;
  int Sindex;
  int idn;
  char badform; 
  unsigned char config;
  unsigned char stratum;
  unsigned char domain;
  char areastr[50];
  char hgtstr[50];
  char zv2str[50];
  char lenstr[50];
  char namstr[500];
  int numnodes;
  int MaxNodes;
  
  
  double areaattr;
  double hgtattr;
  double zv2attr;
  double lenattr;
  double namattr;
  double length;
  double * x;
  double * y;
  double * z;
} ;


struct BridgeAsRead
{
  int Bindex;
  int Lindex;
  int Sindex;
  int idn;
  unsigned char config;
  unsigned char stratum;
  unsigned char domain;
  int numverts;
  double * x;
  double * y;
  double * z;
  double * h;
} ;


struct PointAsRead
{
  
  int Lindex;
  int Sindex;
  int idn;
  int featureid;
  unsigned char config;
  unsigned char stratum;
  unsigned char domain;
  char areastr[50];
  char hgtstr[50];
  char zv2str[50];
  char lenstr[50];
  char namstr[500];
  int numverts;
  int MaxVerts;
  double areaattr;
  double hgtattr;
  double zv2attr;
  double lenattr;
  double namattr;
  double rad;
  double * x;
  double * y;
  double * z;
  double * h;
} ;

struct PolyAsRead
{
  int SACindex;
  int SCCindex;
  int idn;
  int TblIdn;
  int numverts;
  double *x;
  double *y;
  double *z;
  double minxvtx,maxxvtx,minyvtx,maxyvtx;
  unsigned char config;
  unsigned char stratum;
  unsigned char domain;
} ;



struct RawAreal
{
  int Lindex;
  int Sindex;
  int idn;
  int featureid;
  int ishole;
  int notclosed;
  int firstindex;
  int numverts;
  int ismulti;
  int coversLM;
  rb_red_blk_tree * RB_Tree_Edgelist;
  double localID;
  double minxvtx;
  double areaattr;
  double hgtattr;
  double zv2attr;
  double namattr;
  double lenattr;
  char * areastr;
  char * hgtstr;
  char * zv2str;
  char * lenstr;
  char * namstr;
  double area;
  double perimlength;
  double * x;
  double * y;
  double * z;
  struct RawAreal * next;
} ;

struct RawLinear
{
  int featureid;
  int Lindex;
  int Sindex;
  int idn;
  int NumIntersections;
  int numnodes;
  int IsMulti;
  char badform; 
  char firstencounter; 
  
  
  double areaattr;
  double hgtattr;
  double zv2attr;
  double lenattr;
  double namattr;
  char * areastr;
  char * hgtstr;
  char * zv2str;
  char * lenstr;
  char * namstr;
  double localID;
  double length;
  double * x;
  double * y;
  double * z;
  struct RawLinear * next;
} ;

struct RawPoint
{
  
  int Lindex;
  int Sindex;
  int idn;
  int featureid;
  int numverts;
  double areaattr;
  double hgtattr;
  double zv2attr;
  double lenattr;
  double namattr;
  char * areastr;
  char * hgtstr;
  char * zv2str;
  char * lenstr;
  char * namstr;
  double localID;
  double rad;
  double * x;
  double * y;
  double * z;
  double * h;
  struct RawPoint * next;
} ;
struct RawBridge
{
  int Bindex;
  int Lindex;
  int Sindex;
  int idn;
  int numverts;
  double localID;
  double * x;
  double * y;
  double * z;
  double * h;
  struct RawBridge * next;
} ;


struct ThePolys
{
  int SACindex;
  int SCCindex;
  int idn;
  int numverts;
  double localID;
  double  * x;
  double  * y;
  double  * z;
  double minxvtx,maxxvtx,minyvtx,maxyvtx;
  double slope;
  unsigned char config;
  unsigned char stratum;
  unsigned char domain;
  unsigned char insertOK;
  struct PolyExtra * Xtra;
  struct ThePolys * next;
};




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




struct SccNode
{
  int scc;
  int numsac;
  int type;  
  int numflags;
  int flagsfound[FLAGSIZE];
  struct long_attr_list *sac;
  struct SccNode * prev;
  struct SccNode * next;
};

struct SccNodeSH
{
  int scc;
  int numsac;
  int type;  
  int numflags;
  int flagsfound[FLAGSIZE];
  struct short_attr_list *sac;
  struct SccNodeSH * next;
};



struct long_attr_list
{
  int attr_code;     
  int unit_code;     
  int scale_code;    
  int SEDRIStype;    
  
  int geomtype;      
  
  
  int    Ivalue;   
  int    Ivalue2;  
  char  *Cvalue;   
  
  int boundtype;    
  
  char *lod_stuff;
  char *nam_stuff;
  char *string_stuff;
  double Dvalue;     
  double Dvalue2;     
};


struct short_attr_list
{
  int attr_code;
  char *lod_stuff;
};



struct ECCtgtList
{
  int tag;
  int count;
  struct ECCtgtList * next;
};

struct EACtgtList
{
  int tag;
  int count;
  double lowDBLval;
  double highDBLval;
  int lowINTval;
  int highINTval;
  int lowENUMval;
  int highENUMval;
  char datatype;  
  
  char *StringTgt;
  struct EACtgtList * next;
};








void MakeElevationDataGrid(void);
void FreeModelAllocations();
void ReadInModels(int index1,int index2);
double Distance(double x1, double y1, double x2, double y2);
double Distance3D(double x1, double y1, double z1, double x2, double y2, double z2);
int GetPointElevation(double pntx,double pnty,double * NewZ);
void CreateDrawProfile(double x1,double y1,double x2,double y2,
		       double * minElev, double * maxElev);
void FreeModelNames(void);
void FreeMemoryAsExit(int flag);

void DescribeRegionContents(double x1,double y1,double x2,double y2, int IsPoint);


void SetMacroCheckFlags(void);
void InitializeInStreams();
void InitializeOutStreams();
void SetUpTable(int flag);
void DrawRegionContent(int Lindex1, int Uindex1, int Lindex2, int Uindex2);
int LineSegmentsIntersect(double xl,double yl,double xk,double yk,
			  double xm,double ym,double xn,double yn,
			  double *xi,double *yi);
int SegmentIntersectX(double xl,double yl,double xk,double yk,
		      double xm,double ym,double xn,double yn, int maxval);
int SegmentIntersectY(double xl,double yl,double xk,double yk,
		      double xm,double ym,double xn,double yn, int maxval);
void CalcLOSfan(double xo,double yo,double xt,double yt);
char * Coordinate3DtoString(double x,double y,double z);
char * Coordinate2DtoString(double x,double y);
void CheckRegion(int Lindex1,int Uindex1, int Lindex2, int Uindex2);
void ReadDynamicDefsFile(void);
char * ParseConfigurationClass(unsigned char keyval);
char * ParseStratumClass(unsigned char keyval);
char * ParseDomainClass(unsigned char keyval);
char * ProvideNameForSCC(char  *SCC);
char * ProvideNameForSAC(char  *SAC);
void FullSACDescription(char *SACin, int ShowDefinition, int ShowEnums);
void FullSCCDescription(char *SCC,   int ShowDefinition);
double InterpLinearElev(double X1,double Y1,double Z1,double X2,double Y2,double Z2,double x,double y);
int PointOnLineSegment(double px,double py,double x1,double y1,double x2,double y2,double slack);
int SummarizeRegionContents(int verbose, double x1,double y1,double x2,double y2,
			    struct ECCtgtList *CtgtRoot, struct EACtgtList *AtgtRoot);
void GetSedrisIDnumber(int index, char * answer);


void DeleteFromFilterList(int type, int index1, int index2, double Dindex1, double Dindex2);
void AddToFilterList(int type,int index1,int index2, double Dindex1, double Dindex2);
void AddToMarkerList();
void CheckAssignedSAC(char * filename, int LLnumber);


int DoTheFixing(char *stfname,char *patchfilename);


void DrawErrors(int DoAll, int ErrorIndex, int SingleBegin, int SingleEnd, int CloneIndex, int overridetxt);
void DoErrorSpecificInitialization(void);
char * ParseErrType(int i);
int ParseErrNumber(char *errname,int line, char *exefile);
char * ParseErrName(int errname);



void line(int xs,int ys,int xe,int ye);
void colorpixLine(int xs,int ys,int xe,int ye,unsigned long color);
void SEEIT_fread_int(int *i, FILE *fp);
void SEEIT_fread_uint(unsigned int *i, FILE *fp);
void SEEIT_fread_float(float *i, FILE *fp);
void SEEIT_fread_long(long *li, FILE *fp);
void SEEIT_fread_double(double *d, FILE *fp);
void SEEIT_fread_short(short int *si, FILE *fp);
void SEEIT_fread_ushort(unsigned short int *si, FILE *fp);
void SEEIT_fread_char(char *c, FILE *FP);
void SEEIT_fwrite_long(long *li, FILE *fp);
void SEEIT_fwrite_short(short int *si, FILE *fp);
void SEEIT_fwrite_double(double *d, FILE *fp);
void SEEIT_fwrite_int(int *i, FILE *fp);
void SEEIT_fwrite_char(char *c, FILE *fp);
void write_endian(FILE *f);
void read_endian(FILE *f);
int  read_endian_return(FILE *f);
double TruncateToNdigits( double x, int digits );

#define TruncNdigs(x, d) ( TruncateToNdigits(x, (int) d) )



int import_shape(char *shape_to_import,int files_done, char *allshapes, 
		 char *fcode,char *shortname,
		 int *longitemsfound, int *shortitemsfound, int addtolong, int addtoshort, char *override_curr_file);
int import_dted(char *dtedfile, char *allshapes, int files_done, char *lod_override, int addtolong, int addtoshort);
int import_gridascii(char *dtedfile, char *allshapes, int files_done, char *lod_override, int addtolong, int addtoshort);




void FormatTheTerrain(char *dirname);



int Do_Glue(char filename[]);



int tempis2D(double val);
int tempis3D(double val);

char *GetEACLabel(int code) ;
char *GetLongECCLabel(int code);
char *GetECCLabel(int code);
char *GetScaleLabel(int code);
char *GetUnitsLabel(int code);
char *GetSedrisTypeLabel(int code);
char *GetSedrisRangeLabel(int code);
char *GetEEDefinition(int attr_code, int enum_code);
char *GetECCDefinition(int code);
char *GetEACDefinition(int code);
char *GetEACEnumDefinition(int code);
char *GetEELabel(int attr_code, int enum_code);
char *GetAllEELab(int code);
char *GetAllEEDef(int code);
char *GetEAEQclass(int code);
char *GetEAValueType(int code);
char *GetEMLabel(int attr_code);
char *GetEMDefinition(int attr_code);
char *GetECCCode(int code);
char *GetEACCode(int code);



void PrepareForGridMasking(int gridnum,long int fileposn, int LODband, int reset, int setmask);
void EndGridMasking();
void short_non_fatal_error(char message[],char title[]);
void update_extract(int DO_FMIS,
		    int num_fmareal, int num_fmpoint, int num_fmlinear,
		    int firstmodel , int secondmodel, int thirdmodel  ,
		    int num_coll   , int num_par    , int num_stamp   , int num_foot,
		    int num_polys  , int num_areals , int num_points  , int num_modelpolys,
		    int num_linears, int num_gmi    , int num_fmi     , long int gridpointsdone,
		    int done       , int longnum);

void update_concat(int shortindices, int longindices, int totalpolysdone, int totalpointssdone, 
		   int totallinearsdone, int totalarealsdone, int totalbridgesdone, 
		   int totalgridsdone,char *currdir,
		   int FILES_DONE, int DONE);
void infoprint(char msg[]); 
void infoprintbold(char msg[]);    
void MAPdrawTri(double x1,double y1,int color, int Lindex);
void MAPdrawcircle(double x,double y,double diameter,int color, int Lindex);
void MAPdrawpoly(int num, double *X, double *Y,
		 int color,int convex,int clip,int thickness, int Lindex);
void MAPdrawline(int flag,double x1,double y1,double x2,double y2, int color, int Lindex);
void MAPdrawfullline(double X[],double Y[],int numnodes,int color, int Lindex);
void MAPdrawX(double x1,double y1,int color, int Lindex);
void MAPdrawStar(double x1,double y1,double z1,int color,int Lindex,int type);
void MAPdrawContourLine(int ContourMultiple, double X, double Y, double X2, double Y2, int lindex);
void periodic_redraw(void);
int periodic_grid_redraw(char title[],char message[]);
int periodic_checking_redraw(int UseButton, char title[],char message[]);
void set_shift(int on);
void BuildOpenMenu(double AttrChk,int ECCdone, int ECCttl,int polys_read, int polys_done,int model_polys_done,int areals_done,int linears_done,int points_done,
                   int s_coll_done, int p_coll_done, int stamps_done, int foot_done, int bridges_done,
                   unsigned int Gptread, unsigned int GptDone,int FMpoint,int  FMlinear,int  FMareal,
                   double PCacount, double IndexPrep, double PCattr, double PCattrval,int DON);
void DO_NAV(double x,double y);
void make_los_message(char ff[],int bb,char *cc,int dd);
void periodic_fixing_redraw(int done);
char * ParseCoordSystem(int coordflag);
int GetCloneNumber(int cloneindex,int errnum);
int GetCloneIndex(int clonenum,int errnum);
void InitializationAdvice(double PCacount, double IndexPrep, double PCattr, double PCattrval, double ECCdone, int DON);
void CombIntDrawingLayers(int geom);
int DrawAreaFilled(int Lindex);
int DrawGridShaded(int Lindex);








int ATTRTYPE_D4();
int ATTRTYPE_FACC();
int ATTRTYPE_VMAP();
int ATTRTYPE_MGCP3();
int ATTRTYPE_MGCP4();
int ATTRTYPE_UFD2();
int ATTRTYPE_UFD1();
int ATTRTYPE_UFD3();
int ATTRTYPE_LTDS4();
int ATTRTYPE_LTDS5();
int ATTRTYPE_UTDS4();
int ATTRTYPE_UTDS5();
int ATTRTYPE_RTDS4();
int ATTRTYPE_RTDS5();
int ATTRTYPE_GTDS4();
int ATTRTYPE_GTDS5();
int ATTRTYPE_NFDD();
int ATTRTYPE_DFDD();
int ATTRTYPE_SAC();
int ATTRTYPE_GGDM22();
int ATTRTYPE_GGDML22();
int ATTRTYPE_GGDMS22();
int ATTRTYPE_GGDMR22();
int ATTRTYPE_GGDMG22();
int ATTRTYPE_GGDM21();
int ATTRTYPE_GGDML21();
int ATTRTYPE_GGDMS21();
int ATTRTYPE_GGDMR21();
int ATTRTYPE_GGDMG21();
int ATTRTYPE_GGDM3();
int ATTRTYPE_ANY_MGCP();
int ATTRTYPE_ANY_UFD();
int ATTRTYPE_ANY_TDS4();
int ATTRTYPE_ANY_TDS5();
int ATTRTYPE_ANY_TDS6();
int ATTRTYPE_ANY_TDS();
int ATTRTYPE_ANY_GGDM();
int ATTRTYPE_ANY_GGDM21();
int ATTRTYPE_ANY_GGDM22();
int ATTRTYPE_TDS4();
int ATTRTYPE_TDS5();
int ATTRTYPE_TDS6();
int ATTRTYPE_TREX();






struct LindexLookup
{
  int inside;
  int * indices;
};

struct ProfileList
{
  double dist;
  double elev;
  int ptype;
  int LLindex;
  unsigned char stratum;
  struct ProfileList * next;
};

struct SCCdata
{
  int ECC;
  char * pname;
  unsigned char C, S, D;
};




struct CheckGroupNames
{
  char name[100];
  Widget topwid;
  Widget NETgrouplevel;
  Widget VCgrouplevel;
  Widget FCgrouplevel;
};



struct ErrorTable
{
  int DO_EDCS_COMBO[3+1][2][NUM_C];       
  Widget list_w    [3+1][2][NUM_C];
  Widget list2_w   [3+1][2][NUM_C];
  Widget EAlist    [3+1][2][NUM_C];
  Widget EAVlist   [3+1][2][NUM_C];
  Widget EASlist   [3+1][2][NUM_C];
  Widget specwidget[3+1][2][NUM_C];
  Widget CSD;
  Widget allconfig       [3+1][NUM_C];  
  Widget specifiedconfig [3+1][NUM_C];  
  Widget allconfiginclude[3+1][NUM_C];  
  Widget toggle;
  Widget sens_head;
  Widget annot_head;
  Widget annot_label;
  Widget topwid;                        
  Widget scrollwid,vertwid,scrollwidNET,vertwidNET;
  Widget viewallwid,viewsomewid,viewallwidNET,viewsomewidNET,filterwid,FCselectallwid,FCdeselectallwid;
  Widget checktype,checktypeNET,FCchecktype;         
  Widget EAstring1,EAstring2,EAstring3;
  int scrollwidmade,scrollwidmadeNET,viewall,viewallNET,viewsome,viewsomeNET,maxscrollsize,maxscrollsizeNET;

  int pos_list_made,NETpos_list_made;
  int *pos_list,*NETpos_list;
  unsigned char *keepdismiss;

  
  struct IMarkList     * IMarkRoot   [3+1][2][NUM_C];
  struct IMarkSACList  * IMarkSACRoot[3+1][2][NUM_C];
  int model_index[3+1][2][NUM_C];
  int sac_index  [3+1][2][NUM_C];
  
  char *Annotation;                
  int exportnumber;                


  
  int UseBorderConds;              
  int UseBorderCondsDefault;       


  int number,numberNets;           
  int num_clones;                  
  int maxgroupid;                  
  int *groupnums;
  long int fileposn;
  int numthresholds;               
  int usemagnitude;                
  double sensitivity;              
  double sensitivity2;             
  double sensitivity3;             
  double sensitivity4;             
  double sensitivity5;             
  double sensitivity6;             
  double lowrange;                 
  double highrange;                
  double lowrange2;                
  double highrange2;               
  double lowrange3;                
  double highrange3;               
  double lowrange4;                
  double highrange4;               
  double lowrange5;                
  double highrange5;               
  double lowrange6;                
  double highrange6;               
  char range;                      
  char checkapplies;               
  char use_sen1;
  char use_sen2;
  char use_sen3;
  char use_sen4;
  char use_sen5;
  char use_sen6;
  int mygroup;                     
  char *units;                     
  char *units2;                    
  char *units3;                    
  char *units4;                    
  char *units5;                    
  char *units6;                    
  char *tol_desc1;                 
  char *tol_desc2;                 
  char *tol_desc3;                 
  char *tol_desc4;                 
  char *tol_desc5;                 
  char *tol_desc6;                 
  char *description;               
  char *name;                      
  char *magdescribe;               
  int selected;                    
  int active;                      
  int anyactive;                   
  int draw_wholeareal;             
  int participants;                  
  int bigworse;                    
  int onoff,NETonoff;              
  int masteronoff;                 
  int VConoff;                     
  int filterout;                   
  int totalBO;                     
  int wroteout;                    
  
  Widget WidStratum1[NUM_S+5],WidDomain1[NUM_D+5];
  Widget WidStratum2[NUM_S+5],WidDomain2[NUM_D+5];
  Widget WidStratum3[NUM_S+5],WidDomain3[NUM_D+5];
  
  
  unsigned char AllConfigOn[3+1][NUM_C];  
  
  unsigned char SpecifiedConfigOn[3+1][NUM_C];  
  
  unsigned char Config1 [NUM_C];
  unsigned char Stratum1[NUM_S];   
  unsigned char Domain1 [NUM_D];
  unsigned char *primaryEDCSstuff; 
  unsigned char *primaryFIDstuff; 
  
  
  unsigned char Config2[NUM_C];
  unsigned char Stratum2[NUM_S]; 
  unsigned char Domain2[NUM_D];
  unsigned char *secondaryEDCSstuff; 
   unsigned char *secondaryFIDstuff; 
  
  unsigned char Config3[NUM_C];
  unsigned char Stratum3[NUM_S]; 
  unsigned char Domain3[NUM_D];
  unsigned char *tertiaryEDCSstuff; 
  unsigned char *tertiaryFIDstuff; 
};



struct CloneErrorTable
{
  int DO_EDCS_COMBO[3+1][2][NUM_C];       
  Widget list_w    [3+1][2][NUM_C];
  Widget list2_w   [3+1][2][NUM_C];
  Widget EAlist    [3+1][2][NUM_C];
  Widget EAVlist   [3+1][2][NUM_C];
  Widget EASlist   [3+1][2][NUM_C];
  Widget specwidget[3+1][2][NUM_C];
  Widget CSD;
  Widget allconfig       [3+1][NUM_C];  
  Widget specifiedconfig [3+1][NUM_C];  
  Widget allconfiginclude[3+1][NUM_C];  
  Widget toggle;
  Widget sens_head;
  Widget annot_head;
  Widget annot_label;
  Widget scrollwid,vertwid,scrollwidNET,vertwidNET;
  Widget viewallwid,viewsomewid,viewallwidNET,viewsomewidNET,filterwid;
  Widget EAstring1,EAstring2,EAstring3;
  int scrollwidmade,scrollwidmadeNET,viewall,viewallNET,viewsome,viewsomeNET,maxscrollsize,maxscrollsizeNET,UseBorderConds;

  int pos_list_made,NETpos_list_made;
  int *pos_list,*NETpos_list;
  unsigned char *keepdismiss;

  struct IMarkList     * IMarkRoot   [3+1][2][NUM_C];
  struct IMarkSACList  * IMarkSACRoot[3+1][2][NUM_C];
  int model_index[3+1][2][NUM_C];
  int sac_index  [3+1][2][NUM_C];
  
  
  char *Annotation;                  
  int CloneNumber;                 
  int maxgroupid;                  
  int *groupnums;
  long int fileposn;
  double sensitivity;              
  double sensitivity2;             
  double sensitivity3;             
  double sensitivity4;             
  double sensitivity5;             
  double sensitivity6;             
  int selected;                    
  int exportnumber;                
  int number,numberNets;           
  int active;                      
  int onoff,NETonoff;              
  int VConoff;                     
  int filterout;                   
  int totalBO;                     
  int wroteout;                    
  char use_sen1;
  char use_sen2;
  char use_sen3;
  char use_sen4;
  char use_sen5;
  char use_sen6;

  
  Widget WidStratum1[NUM_S+5],WidDomain1[NUM_D+5];
  Widget WidStratum2[NUM_S+5],WidDomain2[NUM_D+5];
  Widget WidStratum3[NUM_S+5],WidDomain3[NUM_D+5];
  
  unsigned char AllConfigOn[3+1][NUM_C];  
  
  unsigned char SpecifiedConfigOn[3+1][NUM_C];  
 
  unsigned char Config1[NUM_C];
  unsigned char Stratum1[NUM_S];   
  unsigned char Domain1[NUM_D];
  unsigned char *primaryEDCSstuff; 
  unsigned char *primaryFIDstuff; 
  
  
  unsigned char Config2[NUM_C];
  unsigned char Stratum2[NUM_S]; 
  unsigned char Domain2[NUM_D];
  unsigned char *secondaryEDCSstuff; 
  unsigned char *secondaryFIDstuff; 
  
  unsigned char Config3[NUM_C];
  unsigned char Stratum3[NUM_S]; 
  unsigned char Domain3[NUM_D];
  unsigned char *tertiaryEDCSstuff; 
  unsigned char *tertiaryFIDstuff; 
};







struct AttributionErrors
{
  char *description;
};
struct reflist
{
  int index; 
  struct reflist * next;
};
struct AttrValList
{
  double occurrences; 
  double uniqueoccurrences;
  double value; 
  double value2; 
  int type; 
  int intervaltype; 
  int uom; 
  int scale; 
  char * label; 
  int LLindexcount; 
  int * LongListIndices; 
  int UniqueLLindexcount;
  int * UniqueLLindices;
};


struct AttrList
{
  double occurrences; 
  int code;        
  int count;  
  
  struct AttrValList * values;
};


struct ModelNames
{
  int code;             
  int inout;            
  int inout2;           
  int origindex;        
  char name[ModelNameMaxLength]; 
  double occurrences;      
  int references;       
  int * indices;        
  int numattributes;      
  int *AttrIndices;     
  int *ValueIndices;    
  int *LongListIndices; 
  double *ValueCounts;     
  int numuniqueAttr;
  int * UniqueAttr;
  double * UniqueCounts;   
  
} ;


struct MoreAttrErrIndices
{
   int index;
   struct MoreAttrErrIndices * next;
};


struct CrossWalk
{
  unsigned char toggle;
  long int FullFilePosn;
  int LODband;
  int AttrErr; 
  int geomtype;
  int crossindex;
  int LtoSmapping;
  struct MoreAttrErrIndices * MAE; 
};

struct LODdisplay
{
  unsigned char active;
  unsigned char c1active;
  unsigned char c2active;
  unsigned char sp1active;
  char *filename;
  char *title;
  int color;
  int firsttime;
  int temp_color;
  int rcolor,gcolor,bcolor;
  int temp_rcolor,temp_gcolor,temp_bcolor;
  int geomtype[NUM_C];
  int geomtype_single;
  Widget color_wid;
  int other_index;  
};


struct LODdisplaySH
{
  
  Widget discrete_value_wid;
  int other_index;  
  int draw_priority;
  int draw_style;
  int temp_style;
  int ECC;
  int geom;
  int do_discrete; 
  int do_outline;
  int equal_type;
  double discrete_ignore_value;
};


struct DomainColors
{
  int toggleonoff;
  int color;
  int firsttime;
  int temp_color;
  int rcolor,gcolor,bcolor;
  int temp_rcolor,temp_gcolor,temp_bcolor;
  Widget color_wid;
};

struct StaticModel
{
  int refnumber;
  int idn;
  int numverts;
  int Lindex;
  double *x;
  double *y;
  double *z;
  double *h;
  struct StaticModel * next;
};

struct StampModel
{
  int refnumber;
  int idn;
  int Lindex;
  double x;
  double y;
  double z;
  double radius;
  double height;
  struct StampModel * next;
};




struct SCCstoreint
{
  char SCC[500]; 
  int sccint;   
  unsigned char config;
  unsigned char strat;
  unsigned char domain;
  
  int orig_position;
  long int listposn;
};


#if (NGA_TYPE == 1)
#define MaximumECCcount (857+1)   
#define MaximumEACcount (2126+1)  

#else

#define MaximumECCcount (EDCS_TOTAL_EC+1)
#define MaximumEACcount (EDCS_TOTAL_EA+1)
#endif

#define NUM_ATTR_SCHEMA 47




typedef enum {
  STATUS_OK,
  STATUS_KEY_FOUND,
  STATUS_MEM_EXHAUSTED,
  STATUS_DUPLICATE_KEY,
  STATUS_KEY_NOT_FOUND
} statusEnum;

#define compLT(a,b) ( strcmp(a,b) < 0)
#define compEQ(a,b) ( strcmp(a,b) == 0)



typedef enum { BLACK, RED } nodeColor;


struct recType{
  int stuff;             
  struct SccNode *node;  
};



typedef struct nodeTag {
  struct nodeTag *left;       
  struct nodeTag *right;      
  struct nodeTag *parent;     
  nodeColor color;            
  char *key;                  
} nodeType;


typedef struct L_nodeTag {
  struct L_nodeTag *left;       
  struct L_nodeTag *right;      
  struct L_nodeTag *parent;     
  nodeColor color;            
  char *key;                  
  struct recType rec;         
} nodeType_LIST;


#define NIL  &sentinel           
#define NILL &L_sentinel         


#define EDCS_NVT_OPEN_INTERVAL 2
#define EDCS_NVT_CLOSED_INTERVAL 5
#define EDCS_NVT_GE_LT_INTERVAL 3
#define EDCS_NVT_GT_LE_INTERVAL 4
#define EDCS_NVT_GT_SEMI_INTERVAL 6
#define EDCS_NVT_GE_SEMI_INTERVAL 7
#define EDCS_NVT_LT_SEMI_INTERVAL 8
#define EDCS_NVT_LE_SEMI_INTERVAL 9
#define EUC_UNITLESS 221
#define EUC_METRE 142
#define ESC_UNI 11
#define EDCS_AVT_INTEGER 2
#define EDCS_AVT_REAL 1
#define EDCS_AVT_ENUMERATION 8
#define EDCS_AVT_STRING 5

