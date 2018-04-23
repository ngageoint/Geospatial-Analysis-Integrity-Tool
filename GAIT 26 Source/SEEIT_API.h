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
/****
  This software is provided openly and freely for use in representing and
  interchanging environmental data & databases.
  
  This software was developed for use by the United States Government with
  unlimited rights.  The software was developed under IDA Contract Number
  DASW01-98-C-0067 by the Simulation Center, Institute for Defense Analyses,
  1801 N. Beauregard Street, Alexandria, VA, 22311-1772.  The software is
  unclassified and is deemed as Distribution A, approved for Public Release.
  
  Use by others is permitted only upon the ACCEPTANCE OF THE TERMS AND
  CONDITIONS, AS STIPULATED UNDER THE FOLLOWING PROVISIONS:
  
  1. Recipient may make unlimited copies of this software and give
  copies to other persons or entities as long as the copies contain
  this NOTICE, and as long as the same copyright notices that
  appear on, or in, this software remain.
  
  2. Trademarks. All trademarks belong to their respective trademark
  holders.  Third-Party applications/software/information are
  copyrighted by their respective owners.
  
  3. Recipient agrees to forfeit all intellectual property and
  ownership rights for any version created from the modification
  or adaptation of this software, including versions created from
  the translation and/or reverse engineering of the software design.
  
  4. Transfer.  Recipient may not sell, rent, lease, or sublicense
  this software or any derivative.  Recipient may, however,
  enable another person or entity the rights to use this software,
  provided that this AGREEMENT and NOTICE is furnished along with
  the software and/or software system utilizing this software.
  
  All derivatives, revisions, modifications, created by the
  Recipient, to this software and/or related technical data shall
  be forwarded by the Recipient to the Government at the following
  address:
  
  Engineering Directorate
  Attention SEDRIS COR
  STRICOM
  12350 Research Parkway
  Orlando, FL 32826
  
  or via electronic mail to:  se-mgmt@sedris.org
  
  5. No Warranty. This software is being delivered to you AS IS
  and there is no warranty, EXPRESS or IMPLIED, as to its use
  or performance.
  
  The RECIPIENT ASSUMES ALL RISKS, KNOWN AND UNKNOWN, OF USING
  THIS SOFTWARE.  The DEVELOPER EXPRESSLY DISCLAIMS, and the
  RECIPIENT WAIVES, ANY and ALL PERFORMANCE OR RESULTS YOU MAY
  OBTAIN BY USING THIS SOFTWARE OR DOCUMENTATION.  THERE IS
  NO WARRANTY, EXPRESS OR, IMPLIED, AS TO NON-INFRINGEMENT OF
  THIRD PARTY RIGHTS, MERCHANTABILITY, OR FITNESS FOR ANY
  PARTICULAR PURPOSE.  IN NO EVENT WILL THE DEVELOPER, THE
  UNITED STATES GOVERNMENT OR ANYONE ELSE ASSOCIATED WITH THE
  DEVELOPMENT OF THIS SOFTWARE BE HELD LIABLE FOR ANY CONSEQUENTIAL,
  INCIDENTAL OR SPECIAL DAMAGES, INCLUDING ANY LOST PROFITS
  OR LOST SAVINGS WHATSOEVER.
  ******/


/* Total number of condition types defined at this release 
***THIS IS ALSO CO_DEFINED IN SEEIT_API.h*****/



#define CONDITION_DEFINITIONS 245
#define CONDITION_ARRAY_SIZE  246 /*** CONDITION_DEFINITIONS + 1  **/


#define NUM_C 16  /* number of configuration classes */
#define NUM_S 6   /* number of stratum classes       */
#define NUM_D 19  /* number of domain classes        */

#define BIG 1     /* endians */
#define LITTLE 0

#define WGS84degreeLength 111319.5
#define MY2DSENTINEL  1.3070057 
#define SENTINEL_SMIDGE  .0000001

/*
  SEEIT_LoadConditionReport
  This function takes as it's arguments character strings naming:

  1) a directory that contains a saved condition report (dirname)

  This function is used to initialize the API for this database,
  and must be called before calling any other API function.
  */

void SEEIT_LoadConditionReport(char *dirname);




/* SEEIT_FreeConditionReportMemory:
   After calling SEEIT_LoadConditionReport, and using whatever API
   functions are desired, this function should be called*/

void SEEIT_FreeConditionReportMemory();




/* 
   SEEIT_GetConditionAnnotation:
   This function takes as an argument a condition and returns
   character string describing the annotation of the condition.
   */
char * SEEIT_GetConditionAnnotation(struct ConditionList *curr);




/* 
   SEEIT_GetNumConditions:
   This function tells the number of conditions located of type CONDITIONTYPE
   with additional instantion number additional_instance_num
   */
int SEEIT_GetNumConditions(int CONDITIONTYPE, int additional_instance_num);
     

/* 
   SEEIT_GetConditionName
   This function takes as an argument an integer [1..CONDITION_DEFINITIONS]
   (or an enumerated #define condition name from below), and returns a 
   character string naming the condition.
   */
char * SEEIT_GetConditionName(int CONDITIONTYPE);






struct AdditionalConditions
{
  int condition_number;
  int additional_instance_number;
  struct AdditionalConditions *next;
};


/*
  Add to list AdditionalConditionFlags a structure identifying
  condition number condnum and additional check number addnum
  (ie, to specify the 3rd additional check of type TVERT,
  condnum==TVERT, addnum==3)
*/
void SEEIT_AddAdditionalFlag(struct AdditionalConditions **AdditionalConditionFlags,
			     int condnum,int addnum);


/*
  free the list AdditionalConditionFlags, which was allocated using
  SEEIT_AddAdditionalFlag
*/
void SEEIT_FreeAdditionalFlags(struct AdditionalConditions *AdditionalConditionFlags);




/* 
   SEEIT_GetConditions
   
   This is the function used to actually retrieve stored 
   conditions from the condition report.
   
   This function takes as input the directory containing the
   SEE-IT database and condition report.

   This function also takes in 1 array used to filter its output:
   ConditionsToUse.  It must be filled by the user with 0s or 1s.  

   ConditionsToUse:  This array must be filled [1..CONDITION_DEFINITIONS] 
   with 0/1, describing whether or not the user wishes to retrieve 
   conditions of each type (1:retrieve 0:ignore).

   SpecificConditionBegin, SpecificConditionEnd:  Used to specify a range (ie, 5-8)
   of conditions that are desired.  Must both be 0 if specnum is used.

   NO_DRAW: If 1, the drawing functions will not be called.  otherwise, they will.

   NO_TEXT_MESSAGE: If 1, the individual text messages generated for each condition
   and stored in it's structure will not be generated.  otherwise, it will.

   DRAW_ONLY:  If 1, only the drawing functions are called.  A list of conditions is
   not returned.

   SpecArray: Array of Specific Conditions.  Can be used in place of many calls
   where SpecificConditionBegin and SpecificConditionEnd would have been used.

   specnum: Size of SpecArray.  Should be -1 when SpecArray is NULL
   Must be 0 when SpecificConditionBegin, SpecificConditionEnd are used.

   Example:
   If the user wanted to only retreive the VTEAR conditions, the user would set
        ConditionsToUse[VTEAR] = 1
   */

struct ConditionList * SEEIT_GetConditions(char *dirname,
					   int ConditionsToUse    [CONDITION_ARRAY_SIZE],
					   int SpecificConditionBegin,
					   int SpecificConditionEnd,
					   struct AdditionalConditions *AdditionalConditionFlags,
					   int NO_DRAW,
					   int NO_TEXT_MESSAGE,
					   int DRAW_ONLY,
					   int SpecArray[],
					   int specnum
					   );



/*
  SEEIT_PrintConditionByComponents
  This function takes as it's argument a single condition that 
  has been returned as part of a list by SEEIT_GetConditions.
  See read_conditions.c for an example of how to iterate through a
  list returned by SEEIT_GetConditions.

  This function prints information summarizing the condition

  However, in contrast to SEEIT_PrintConditionString, this 
  function uses the individual components of the condition to 
  print the information (this is indeed the function used to
  make the string printed by SEEIT_PrintConditionString).

  This function is intended to provide an example of how to 
  access the low-level information stored in the condition 
  structures returned by SEEIT_GetConditions.

  This function puts the generated string in the Message field 
  of the condition structure (condition->Message), replacing it if
  it already exists.
  */
void SEEIT_PrintConditionByComponents(struct ConditionList *condition,int begin,int end);



/*
  SEEIT_FreeConditions
  This function takes as it's argument a 
  pointer to a list of conditions that has been returned 
  by SEEIT_GetConditions.

  This function's job is to free the memory allocated by 
  SEEIT_GetConditions to build the list of conditions.

  This function must be called to free the memory allocated 
  by SEEIT_GetConditions.  
  */
void SEEIT_FreeConditions(struct ConditionList * ListOfConditions);



/* 
   SEEIT_ExportShapefile

This function takes as it's arguments a pointer to a list of conditions 
that has been returned by SEEIT_GetConditions and a character string 
representing a root file name, and then creates shapefiles <outname>LN
(non-point data) and/or <outname>PT (point data) from those conditions

*/
void SEEIT_ExportShapefile(struct ConditionList * conditions, 
		     char *outname);



int Ctype();


/* Defines for basic geometric form */
#define G_AREAL 1
#define G_CIRCLE 2
#define G_POLYGON 3
#define G_LINE 4
#define G_POINT 5
#define G_GRIDPT 6
#define G_CYLINDER 7
#define G_PIPED 8
#define G_SPHERE 9



/** defines for condition identification numbers **/
/**** feature size group ****/
#define SEGLEN                  1 /*** master check **/
#define PERIMLEN                2 /*** master check **/

#define SMALLAREA               3 /** manual load file for MGCP (2, by scale) ***/
#define SHORTFEAT               4 /** manual load file for MGCP (2, by scale) ***/

#define BIGAREA                 5
#define LONGSEG                 6
#define LONGFEAT                7

#define SMLCUTOUT              8

/******** feature composition group ***/
#define AREAUNCLOSED           9 /*** master check **/
#define MULTIPARTP             10 /*** master check **/
#define MULTIPARTL             11 /*** master check **/
#define MULTIPARTA             12 /*** master check **/
#define SLIVER                 13 /*** master check **/
#define CUTOUT                 14 /*** master check **/
#define CUT_INT                15 /*** master check **/

#define ENDPTINT               16
#define INCLSLIVER             17
#define FACESIZE               18
#define COLINEAR               19

/**** duplication group ***********/
#define V_DUPS                 20 /*** master check **/
#define C_DUPS                 21 /*** master check **/
#define ANY_SAMEID             22 /*** master check **/

#define G_DUPS                 23
#define SAMEID_GDUP            24
#define SAMEID_CDUP            25
#define SAMEID                 26

/*** undershoot / overshoot group ****/
#define AUNDERSHTA             27 /*** master check **/
#define AOVERSHTA              28 /*** master check **/
#define LUNDERSHTL             29 /*** master check **/
#define LOVERSHTL              30 /*** master check **/
#define LUSHTL_DF              31 /*** master check **/
#define LOSHTL_DF              32 /*** master check **/
#define LUNDERSHTA             33 /*** master check **/
#define PSHOOTL                34 /*** master check **/
#define LVUSHTL                35 /*** master check **/
#define LVOSHTL                36 /*** master check **/
#define LELINEPROX             37 /*** master check **/
#define LBNDUSHT              38
#define BNDRYUNDERSHT          39 /** added to master in 25B **/

#define LOVERSHTA              40
#define PUNDERSHTA             41
#define POVERSHTA              42

#define LUSHTL_CLEAN           43
#define VUSHTL_CLEAN           44



/****** connectivity group  *******/
#define FEATNOTCUT             45 /*** master check **/
#define FAILMERGEL             46 /*** master check **/
#define FAILMERGEL2            47 /** manual load file ***/
#define FAILMERGEA             48 /*** master check **/
#define FAILMERGEA2            49 /*** master check **/
#define EXTRA_NET              50 /*** master check **/
#define BADFEATCUT             51 /*** master check **/
#define NOENDCON               52 /*** master check **/

#define LLAINT                 53 /** manual load file for MGCP **/

#define NOCOINCIDE             54
#define ENCONFAIL              55
#define INTRA_NET              56
#define L_NOTL_AINT            57
#define ENCONNECT              58
#define LATTRCHNG              59
#define CONNECTFAIL            60
#define FEATBRIDGE             61
#define BADENCON               62
#define CREATENET              63


/***** containment group   ********/
#define PTINREGION             64 /*** master check **/
#define OBJECTWITHOUT          65 /*** master check **/
#define OBJ_WO_TWO            66 /** added to master in 25B **/
#define PTOSIDEREGION          67 /*** master check **/
#define LINSIDEA               68 /*** master check **/
#define AWITHOUTA              69 /*** master check **/
#define FEATOUTSIDE            70 /*** master check **/

#define PTINPROPER             71 /** master check ***/
#define ISOLINE                72 /** master check ***/

#define AINSIDEHOLE            73 /*** ilegal hole - no longer autogen, but used to be manual load file ***/

#define LOUTSIDEA              74
#define LEINSIDEA              75
#define LEAON_NOTIN            76
#define OSIDE_LAT              77
#define OSIDE_LON              78
#define ACOVERA                79
#define OVERUNDER             80

/******** z values group ***********/
#define SLOPEDIRCH             81 /*** master check **/
#define LEZ_PROX_3D            82 /*** master check **/
#define MULTIDFEAT             83 /*** master check **/
#define NOT_FLAT               84 /*** master check **/
#define FEATSPIKE              85 /*** master check **/
#define CNODE_ZBUST            86 /*** master check **/

#define MULTISENTINEL          87
#define ZUNCLOSED              88
#define L2D_L3D_MATCH          89
#define LJOINSLOPEDC           90
#define ELEVADJCHANGE          91
#define PC_SLOPE               92


/*** illogical feature separation group ***/
#define PLPFAIL                93 /*** master check **/
#define PLPROX                 94 /*** master check **/
#define PTPTPROX               95 /*** master check **/
#define PLLPROXFAIL           96 /*** master check **/
#define LAPROX                97 /*** manual load file only ***/

#define PLPROXEX              98
#define LASLIVER              99
#define LLSLIVER              100
#define LVPROX                101
#define EN_EN_PROX            102

/****** define kinks, kickbacks, loops group ***/
#define KINK                  103 /*** master check **/
#define Z_KINK                104 /*** master check **/
#define INTERNALKINK          105 /*** master check **/
#define CONTEXT_KINK          106 /*** master check **/
#define KICKBACK              107 /*** master check **/
#define LOOPS                 108 /*** master check **/
#define AREAKINK              109 /*** master check **/

#define ISOTURN               110
#define L_A_KINK              111
#define P_O_LOOP              112


/********* intersections group ****/
#define LLNONODEINT           113 /*** master check **/
#define LLMULTINT             114 /*** master check **/
#define LLNOINT               115 /*** master check **/
#define ISOLATEDA             116 /*** master check **/
#define LLIEX                 117 /*** master check **/
#define LLINT                 118 /*** master check **/
#define NETISOA               119 /** master check ***/
#define NETISOFEAT            120 /** added to master in GAIT24B **/
#define LACUTFAIL             121 /** added to master in 25B **/
#define ANETISOA              122
#define LLINTAWAY             123 /** added as manual load check in GAIT24 **/

#define LAINT                 124 /** removed as a master check file **/
#define LAIEX                 125
#define LFNOINT               126
#define LAINTNOEND            127
#define LLNOENDINT            128
#define AREAINTAREA           129
#define LLI_ANGLE             130
#define LSLICEA               131
#define LOC_MULTINT           132
#define LLINTNOEND            133

/***** overlap or covering relationship group ***/
#define LNOCOVERLA            134 /*** master check **/
#define SHARESEG              135 /*** master check **/
#define AOVERLAPA             136 /*** master check **/
#define ANOCOVERA             137 /*** master check **/
#define ANOCOVERLA            138 /*** master check **/
#define COVERFAIL             139 /*** master check **/
#define LSPANFAIL             140 /** new master check in GAIT21, replaced LNOSPANA **/
#define PNOCOVERLE            141 /*** master check **/
#define PSHAREFAIL            142 /*** master check **/
#define FSFAIL                143 /*** master check **/
#define LENOCOVERL            144 /*** master check **/
#define SHARE3SEG             145 /** added to master in GAIT 22 ***/
#define BOTHENDCON            146 /** added to master in GAIT24B ***/

#define PART_ISF              147 /** added to master in GAIT24  **/

#define AMCOVAFAIL            148

#define NOLCOVLE              149
#define LENOCOVERP            150
#define QUALANOCOVLA          151
#define COINCIDEFAIL          152
#define PNOCOV2LEA            153
#define LNOCOV2A              154
#define DUPLICATESEG          155
#define SHAREPERIM            156
#define PNOCOVERLV            157
#define LENOCOVERA            158
#define NONODEOVLP            159
#define LSEGCOVERA            160
#define CONFLATE              161
#define CONF_STATS            162


/**** define boundary matching group ***/
#define LRNGE_UNM_LAT         163 /** manual load boundary check file **/
#define LRNGE_UNM_LON         164 /** manual load boundary check file **/
#define ARNGE_UNM_LAT         165 /** manual load boundary check file **/
#define ARNGE_UNM_LON         166 /** manual load boundary check file **/
#define LGEOM_UNM_LAT         167 /** manual load boundary check file **/
#define LGEOM_UNM_LON         168 /** manual load boundary check file **/
#define AGEOM_UNM_LAT         169 /** manual load boundary check file **/
#define AGEOM_UNM_LON         170 /** manual load boundary check file **/
#define LHANG_LAT             171 /** manual load boundary check file **/
#define LHANG_LON             172 /** manual load boundary check file **/
#define AHANG_LAT             173 /** manual load boundary check file **/
#define AHANG_LON             174 /** manual load boundary check file **/

#define L_UNM_A               175
#define LSAME_UNM_A           176
#define LUNM_ACRS_A           177
#define AUNM_ACRS_A           178
#define AUNM_ATTR_A           179
#define LUNM_ATTR_A           180
#define LUNMA_ACRS_A          181

#define LE_A_UNM_LAT          182
#define LE_A_UNM_LON          183


/****** elevation range group *****/
#define ELEVGT                184 /*** master check **/
#define ELEVLT                185 /*** master check **/
#define ELEVEQOPEN            186 /*** master check **/

#define ELEVEQ                187

/**** DEM condition group *******/
#define CLAMP_DIF              188
#define MASKZERO              189  /*** master check ***/
#define MASKCONSTANT          190  /*** master check ***/
#define MASKMONO              191  /*** master check ***/
#define MASKSHOREL            192  /*** master check ***/
#define AVGSPIKE              193  /*** master check ***/
#define MASKEDIT_0            194  /*** master check ***/
#define MASKCONFLICT          195  /*** master check ***/
#define MASKCONF2             196  /*** master check ***/
#define GRIDEXACTDIF          197  /** added to master in 25B **/

#define LODELEVDIF            198
#define GSPIKE                199
#define CLAMP_SDC             200
#define CLAMP_NFLAT           201
#define CLAMP_JOINSDC         202
#define CLAMP_SEG             203
#define GRID_STD_DEV          204
#define MASKEDIT_1            205
#define WATERMMU              206
#define RAISEDPC              207
#define FLOWSTEP              208
#define KERNELSTATS           209
#define BILINSTATS            210
#define GSHELF                211
#define LOSMINHGT             212

/*** attribute value related ***/
#define VVTERR1WAY            213 /*** master check **/
#define VVTERR2WAY            214 /*** master check **/
#define VVTERR3WAY            215 /*** in master for NFI=NFN but NAM is different **/
#define RPTD_ATTR             216 /** manual load file **/
#define PORTRAYF              217 /** manual load file added in GAIT 24 **/
#define TPORTRAYF             218 /** manual load file added G25B ***/

#define CALC_AREA             219
#define ATTR_PICK             220
#define ATTR_PAIR             221
#define ATTR_RNG              222
#define ATTR_DT               223
#define ATTR_UNEXP            224
#define ATTR_MISSING          225
#define ATTR_META             226
#define ATTR_RNULL            227
#define ATTR_VVT              228
#define HIGHLIGHTED           229



/**** SEEIT only checks below, GAIT & SEEIT checks above ***/

#define OVERC             230
#define TVERT             231
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



struct Areal
{
  char *IDENT;
  char threeD;
  int objectid;
  int numverts;
  unsigned int Lindex;
  double FileAndPosn;
  double * x;
  double * y;
  double * z;
};

struct Poly
{
  char *IDENT;
  char threeD;
  int objectid;
  int numverts;
  unsigned int Lindex;
  double FileAndPosn;
  double * x;
  double * y;
  double * z;
} ;

struct Linear
{
  char *IDENT;
  char threeD;
  int objectid;
  int numverts;
  unsigned int Lindex;
  double FileAndPosn;
  double * x;
  double * y;
  double * z;
} ;

struct Point
{
  char *IDENT;
  char threeD;
  int objectid;
  unsigned int Lindex;
  double FileAndPosn;
  double x,y,z;
} ;

struct Circle
{
  char *IDENT;
  char threeD;
  int objectid;
  unsigned int Lindex;
  double FileAndPosn;
  double x,y,z,radius;
} ;

struct Cylinder
{
  char *IDENT;
  int objectid;
  unsigned int Lindex;
  double FileAndPosn;
  double x,y,z,radius,height;
} ;

struct Parallelpiped
{
  char * IDENT;
  int objectid;
  unsigned int Lindex;
  double FileAndPosn;
  double x[4], y[4], z[4];
  double height;
} ;

struct GridObject
{
  char *IDENT;
  char threeD;
  int objectid;
  int numverts;
  unsigned int Lindex;
  double FileAndPosn;
  double * x;
  double * y;
  double * z;
} ;




struct ConditionList
{
  /*
    this structure describes a single condition.
    A linked list of these structures is returned by SEEIT_GetConditions()
    */

  int CONDITION_TYPE;     /* which type of condition is this element                                    */
  int CONDITION_INSTANCE; /* which instance of this condition type                                      */
  int ordinal_num;        /* which condition in the list (ie, #10 of check instance 5                */
  int retainignore;       /* is this condition retained or ignored                                      */
  int num_areals;         /* how many areals   are involved in this condition                           */
  int num_polys;          /* how many polygons are involved in this condition                           */  
  int num_lines;          /* how many lines    are involved in this condition                           */
  int num_points;         /* how many points   are involved in this condition                           */
  int num_circles;        /* how many circles (could be 3D spheres) are involved in this condition      */
  int num_piped;          /* how many paralllelpipeds (bounding volumes) are involved in this condition */
  int num_cylinder;       /* how many stamps are involved in this condition                             */
  int num_gridpt;         /* how many points from grid objects                                          */
  int num_sphere;

  struct Areal areal1;
  struct Areal areal2;
  
  struct Circle circle1;
  struct Circle circle2;
 
  struct Poly poly1;
  struct Poly poly2;

  struct Linear linear1;
  struct Linear linear2;

  struct Point point1;
  struct Point point2;

  struct GridObject gridobj1;
  struct GridObject gridobj2;

  struct Cylinder cylinder1;
  struct Cylinder cylinder2;

  struct Circle sphere1;
  struct Circle sphere2;

  struct Parallelpiped piped1;
  struct Parallelpiped piped2;

  char AdditionalPointUsed;       /* 0 - 1: is a descriptive point, as below, recorded     */ 

  double pointx, pointy, pointz;  /* points used to descibe a condition and which are not  */
                                  /* actual data elements; and example would be an         */
                                  /* intersection point                                    */
  char *AdditionalIDENT;          /* points may correspond to vertices on an object which  */
                                  /* has an ID, such is case with T Vertices               */

  double magnitude;

  int ECC1, ECC2;                 /* up to two EC codes for objects involved               */

  char * AttrErrMsg;              /* describes, in text, an error in attribution           */

  char * Message;                 /* describes the entire condition                        */

  struct ConditionList * next;
  
};


struct ConditionTable
{
  int conditiontype;    /* correspinds to the anaytsis type, e,g, TVERT, G_DUPS, ...         */
  int instancenumber;   /* may have multiple instances of same analysis, which is this one   */
  int number;           /* number of conditions of this type                                 */
  int numthresholds;    /* how many sensitivity fields are used in this check?               */
  long int fileposn;    /* where the conditions of this type start in the condition file     */
  char *Annotation;     /* annotation for this check.  NULL if none.                         */
  double sensitivity;   /* primary sensitivity for this condition when it was written out    */
  double sensitivity2;  /* secondary sensitivity for this condition when it was written out  */
  char *units;          /* units of the sensitivity (meters, degrees, etc)                   */
  char *units2;         /* units of the second sensitivity (meters, degrees, etc)            */
  char *units3;         /* units of the third sensitivity (meters, degrees, etc)             */
  char *units4;         /* units of the fourth sensitivity (meters, degrees, etc)            */
  char *units5;         /* units of the fifth sensitivity (meters, degrees, etc)             */
  char *units6;         /* units of the sixth sensitivity (meters, degrees, etc)             */
  char *description;    /* text description of condition                                     */
  char *name;       /* name of condition                                                 */
  int selected;         /* whether or not this condition is being reported                   */
  unsigned char Config1[NUM_C];
  unsigned char Stratum1[NUM_S];   /* which primary actors were used to compute for (0/1/2)  */
  unsigned char Domain1[NUM_D];    /* 0=not used 1=used 2=not possible to have been used     */

  unsigned char Config2[NUM_C];
  unsigned char Stratum2[NUM_S]; /* which secondary actors were used to compute for (0/1/2)  */
  unsigned char Domain2[NUM_D];  /* 0=not used 1=used 2=not possible to have been used       */

  unsigned char Config3[NUM_C];
  unsigned char Stratum3[NUM_S]; /* which tertiary actors were used to compute for (0/1/2)   */
  unsigned char Domain3[NUM_D];  /* 0=not used 1=used 2=not possible to have been used       */
};


