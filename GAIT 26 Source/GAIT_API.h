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
/* Total number of condition types defined at this release 
***THIS IS ALSO CO_DEFINED IN SEEIT_API.h*****/



#define CONDITION_DEFINITIONS 231 /** these are GAIT applicable conditions only - another define in SEE_IT_API.h ***/
#define CONDITION_ARRAY_SIZE  248 /*** CONDITION_DEFINITIONS + 17  **/



#define NUM_C 16  /* number of configuration classes */
#define NUM_S 6   /* number of stratum classes       */
#define NUM_D 19  /* number of domain classes        */

#define BIG 1     /* endians */
#define LITTLE 0

#define WGS84degreeLength 111319.5
#define MY2DSENTINEL  1.3070057 
#define SENTINEL_SMIDGE  .0000001


struct AdditionalConditions
{
  int condition_number;
  int additional_instance_number;
  struct AdditionalConditions *next;
};




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
   SEEIT_GetUnits:
   This function takes as an argument an integer [1..CONDITION_DEFINITIONS]
   (or an enumerated #define condition name from below), and a "unitnum",
   either 1 or 2, and returns a character string which is:
   the primary tolerance units if unitnum is 1
   the secondary tolerance units if unitnum is 2
*/
char * SEEIT_GetUnits(int CONDITIONTYPE, int unitnum);




/* 
   SEEIT_GetConditionAnnotation:
   This function takes as an argument a condition and returns
   character string describing the annotation of the condition.
   */
char * SEEIT_GetConditionAnnotation(struct ConditionList *curr);




/* 
   SEEIT_GetConditionDescription:
   This function takes as an argument an integer [1..CONDITION_DEFINITIONS]
   (or an enumerated #define condition name from below), and returns a 
   character string describing the definition of the condition.
   */
char * SEEIT_GetConditionDescription(int CONDITIONTYPE);


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


/* 
   SEEIT_GetNumberOfSensitivies 
   This function takes as an argument an integer [1..CONDITION_DEFINITIONS]
   (or an enumerated #define condition name from below), and returns an 
   integer which is the number of sensitivities defined for that condition.
   It will always be either 0,1, or 2.
   */
int SEEIT_GetNumberOfSensitivies(int CONDITIONTYPE);


/* 
   SEEIT_GetSensitivity
   This function takes as an argument an 
   integer [1..CONDITION_DEFINITIONS]
   (or an enumerated #define condition name from below),

   and an integer (1 or 2) which indicates which sensitivity 
   is being requested.
   
   The user should call SEEIT_GetNumberOfSensitivies(CONDITIONTYPE) 
   to determine how many sensitivities CONDITIONTYPE has before 
   calling this function.  THEN,

     If CONDITIONTYPE is found to have 1 sensitivity,
     the user can call SEEIT_GetSensitivity(CONDITIONTYPE,1) to 
     get the sensitivity for CONDITIONTYPE.
     
     If CONDITIONTYPE is found to have 2 sensitivities,
     the user can call:
     SEEIT_GetSensitivity(CONDITIONTYPE,1)  and  
     SEEIT_GetSensitivity(CONDITIONTYPE,1)
     to get the primary and secondary sensitivities for CONDITIONTYPE.
   */
double SEEIT_GetSensitivity(int CONDITIONTYPE,int SENSITIVITY);



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
   with 0/1, describing wether or not the user wishes to retrieve 
   conditions of each type (1:retrieve 0:ignore).

   SpecificCondition:  If nonzero, this option is used to specify that
   only a single condition from the only active condition type is to be
   returned.  (If you want only the 17th VTEAR, set this to 17, and set 
   ConditionsToUse[VTEAR] = 1 and all other ConditionsToUse[] = 0)

   NO_DRAW: If 1, the drawing functions will not be called.  otherwise, they will.

   NO_TEXT_MESSAGE: If 1, the individual text messages generated for each condition
   and stored in it's structure will not be generated.  otherwise, it will.

   DRAW_ONLY: If 1, the conditions will only be drawn.  A list of conditions will not be returned.

   SpecArray: Array of Specific Conditions.  Can be used in place of many calls
   where SpecificConditionBegin and SpecificConditionEnd would have been used.

   specnum: Size of SpecArray.  Should be -1 when SpecArray is NULL

   Example:
   If the user wanted to only retreive the VTEAR conditions, the user would set
        ConditionsToUse[VTEAR] = 1
   */




/*
  SEEIT_PrintConditionString
  This function takes as it's argument a single condition that has
  been returned as part of a list by SEEIT_GetConditions.
  See read_errors.c for an example of how to iterate through a
  list returned by SEEIT_GetConditions.

  This function prints a string summarizing the condition.
  This string was build by SEEIT_GetConditions (if NO_TEXT_MESSAGE was 0),
  and simply resides with the condition.
  */
void SEEIT_PrintConditionString(struct ConditionList *condition);



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
#define G_GRIDPT 9
#define G_CYLINDER 7
#define G_PIPED 8
#define G_SPHERE 6


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
#define CLAMP_DIF             188
#define MASKZERO              189  /*** master check ***/
#define MASKCONSTANT          190  /*** master check ***/
#define MASKMONO              191  /*** master check ***/
#define MASKSHOREL            192  /*** master check ***/
#define AVGSPIKE              193  /*** master check ***/
#define MASKEDIT_0            194  /*** master check ***/
#define MASKCONFLICT          195  /*** master check ***/
#define MASKCONF2             196  /*** master check ***/
#define GRIDEXACTDIF          197  /** added to master in 25B **/
#define PT_GRID_DIF           198  /** added to G26 master for TREX edge-matching **/

#define LODELEVDIF            199
#define GSPIKE                200
#define CLAMP_SDC             201
#define CLAMP_NFLAT           202
#define CLAMP_JOINSDC         203
#define CLAMP_SEG             204
#define GRID_STD_DEV          205
#define MASKEDIT_1            206
#define WATERMMU              207
#define RAISEDPC              208
#define FLOWSTEP              209
#define KERNELSTATS           210
#define BILINSTATS            211
#define GSHELF                212
#define LOSMINHGT             213
#define BREAKLINE             214

/*** attribute value related ***/
#define VVTERR1WAY            215 /*** master check **/
#define VVTERR2WAY            216 /*** master check **/
#define VVTERR3WAY            217 /*** in master for NFI=NFN but NAM is different **/
#define RPTD_ATTR             218 /** manual load file **/
#define PORTRAYF              219 /** manual load file added in GAIT 24 **/
#define TPORTRAYF             220 /** manual load file added G25B ***/

#define CALC_AREA             221
#define ATTR_PICK             222
#define ATTR_PAIR             223
#define ATTR_RNG              224
#define ATTR_DT               225
#define ATTR_UNEXP            226
#define ATTR_MISSING          227
#define ATTR_META             228
#define ATTR_RNULL            229
#define ATTR_VVT              230
#define HIGHLIGHTED           231


struct Areal
{
  char *IDENT;
  int objectid;
  char threeD;
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
  int objectid;
  char threeD;
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
  int objectid;
  char threeD;
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
  int objectid;
  char threeD;
  unsigned int Lindex;
  double FileAndPosn;
  double x,y,z;
} ;

struct Circle
{
  char *IDENT;
  int objectid;
  char threeD;
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
  int conditiontype;    /* corresponds to the anaytsis type, e,g, TVERT, G_DUPS, ...         */
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


