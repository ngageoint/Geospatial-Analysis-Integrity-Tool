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
#include <time.h>

/***
#include <mcheck.h>
***/
/**********
#include "drm.h"
#include "se_read0.h"
#include "se_read1.h"
#include "se_read1_data_table.h"
******/
#include "share.h"
/*** #include "SEEIT_API.h" ***/
#include "TTshare.h"
/****
int maxverts = 1000;
*****/


/**** define constants ***/

#ifndef DATA_TYPE
#define DATA_TYPE void *
#endif
#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define DEBUG_ASSERT 0
/***#define EXPERIMENT_MODE 0 ****/

/*** type definitions ****/

extern struct PointAndID *PIroot, *pic, *pic2, *LAINTroot, *LSLICEroot;
/***extern struct GridLookup * GridTable; ***/
/***extern struct GlobalGridInfo *GridHeader; ****/
extern struct AttributionErrors *AttrErr; /** array of text messages on attribution errors **/
extern struct MoreAttrErrIndices *MAEc, *MAEn;
extern struct SCCdata * SCCtable;
extern struct ObjectLookup * ArealTable;
extern struct ObjectLookup * LinearTable;
extern struct ObjectLookup * PointTable;
extern struct InputFiles *ArealFiles;
extern struct InputFiles *LinearFiles;
extern struct RawLinear *TgtLroot;
extern struct ArealAsRead ArealFromFile;
extern struct RawAreal *TgtAroot;
extern struct PointAsRead PointFromFile;
extern struct RawPoint *TgtProot;
extern struct LinearAsRead LinearFromFile;
extern struct ErrorTable ErrorLookup[]; /** var declared and storage allocated in errors.c **/
extern struct CloneErrorTable *CloneErrorLookup;  /** declared in errors.c **/
extern struct LindexLookup * LindexTable;
extern struct LindexLookup * FIDindexTable;
extern struct CrossWalk  *CrsWlk;
extern struct CombinedChecks *ActiveChecks;  /*** should be an array of size CONDITION_DEFINITIONS + CLONE_DEFINITIONS ***/
extern struct PairsCheckedEntry *AreaPairRoot, *LinePairRoot;
extern struct PairsChecked *LPc, *LPp, *LPn, *APc, *APp, *APn;
extern struct pairedwith *PWc, *PWn, *PWp;
extern struct NetworkNode *BucketsOfTransNets, *NNc, *NNn, *NNt;
extern struct LIDlist *LIDc, *LIDp, *LIDn;
extern struct CoordList *CLc, *CLn;
extern struct CloseVertex *CVc, *CVn, *CVp;
extern struct NewGridLookup * NGT;
extern struct GlobalGridInfo *GridHeader;
extern rb_red_blk_node * RB_newNode;
extern rb_red_blk_tree * Trans_RB_Tree;
extern rb_red_blk_tree * Check_RB_Tree;
extern rb_red_blk_tree * Stats_RB_Tree;
extern struct CondToCollect *CTCroot, *CTCc, *CTCp;
struct ModelNames * MdlNames;
struct AttrList * MdlNames2;
extern struct MultiGrids *MG;

extern int NewNetNumber;
extern int SLASHTYPE;
extern int NumAttrErr;
extern int NumberOfModels;
extern int NumberOfGrids;
extern int GridFileInUse;
extern int GridNowLoaded;
extern int LoadedGridRows;
extern int LoadedGridCols;
extern int LoadedGridMres;
extern int IgnoreDrawingSettings;
extern int SzLAE;
extern char MessageLine[500];
extern char BndAttrMismatch[500];
extern char indirectory[500];
extern char outdirectory[500];
extern int TtlActiveChecks;
extern double Xtranslation ;
extern double Ytranslation ;
extern double MinXcoord;
extern double MaxXcoord;
extern double MinYcoord;
extern double MaxYcoord;
extern int MinXindex;
extern int MaxXindex;
extern int MinYindex;
extern int MaxYindex;
extern int UseBillBoards;
extern int file_endianness;
extern int machine_endianness;
extern double LAEminx,LAEminy,LAEmaxx,LAEmaxy;
extern double CF_LAEminx,CF_LAEminy,CF_LAEmaxx,CF_LAEmaxy;
extern double PI;
extern double HalfPI;
extern double DistTolerance, SqDistTolerance;
extern double PTL_Dist_Xi, PTL_Dist_Yi;  /*** these should really be parameters for function PointToLineDist2DwithFlag
                                       but using globals instead as a retrofit to avoid changing all the calls
                                       to that function - they hold the (X,Y) on line where distance was taken ***/

extern unsigned long int NUMBEROFCONDITIONS;
extern int UseNUNANPO;
extern char * linearsin;
extern char * arealsin;
extern char * ConflateSourceFiles;
extern int SzD;
extern int SzI;
extern int SzShort;
extern int SzF;
extern int FeatsInNet;
extern int FeatsAddedToNet;
extern int AbsoluteAccuracy;
extern int SignificantDecimals;
extern int NeedIntersectingArealCalc;
extern int NeedSliverGuide;
extern int NeedAreaCalculation;
extern int SACfull;
extern double RegionSize;
extern int RB_newKey;
extern double RB_newKey2;
extern int * RB_newInt;
extern double * RB_newDbl;
extern int CLAMPFEATURES;
extern int * Areal1ForIntersections;
extern int * Areal2ForIntersections;
extern int * Areal1Shared;
extern int * Areal2Shared;
extern int TtlCoverFailures;
extern double MinSurfXcoord;
extern double MaxSurfXcoord;
extern double MinSurfYcoord;
extern double MaxSurfYcoord;
extern FILE *unsortout;



extern struct CodesAndValue * GetAttrCodesAndValues(int Lindex, int idn, FILE * fileptr, int SzCV, int SzCVl, int DiscardLengthAttr);
extern int GetLENCode();
extern int GetZVCode();
extern int GetHGTCode();
extern int GetNAMCode();
extern int GetAREACode();
extern void GetAppropIDnumber(int index, char * answer);

extern int ThreePointsAreColinear(double *x, double *y, double *z);
extern void DrawNewCondition(int keyval, int Cnumber, double px, double py, double pz, int PtUsed,
             int Lindex1, int index1,  int gform1, char * IDStr1,
             double *x1, double *y1, double *z1, int numverts1,
             int Lindex2, int index2,  int gform2, char * IDStr2,
             double *x2, double *y2, double *z2, int numverts2);
extern double GetGridValue(FILE *gridfile,long int gridstart,int gridtypeflag,int xindex,int yindex,int xnum, int ynum); /** in interface.c ***/

extern int AddToSameAttrAndVals(char geom, int ECC, struct CodesAndValue * CV1, struct CodesAndValue * CV2);
extern void CreatePointInsideAreal(struct RawAreal *A, double *px, double *py);
extern FILE * OpenGridFile(int index, int findex);
extern rb_red_blk_node* RBExactQuery(rb_red_blk_tree*, void*);
extern struct RawAreal * FindEdgeClosestToPoint(double px, double py,struct RawAreal * Aroot, struct RawAreal * Neighbors, struct RawAreal *tgt, int *answer);

extern void FwriteMsgMagPointObjects(int keyval, int Cnumber, char *msg, double magnitude, double pointX, double pointY, double pointZ,                                  int index1, char gform1, int Lindex1, double localid1, int IDN1, double radius1, double height1,
                                  double *x1, double *y1, double *z1, int numverts1,
                                  int index2, char gform2, int Lindex2, double localid2, int IDN2, double radius2, double height2,
                                  double *x2, double *y2, double *z2, int numverts2);
extern void FwriteObject(int keyval, int Cnumber,
             int Lindex, int index, char gform, double LocalID, int IDN, double radius, double height,
             double *x, double *y, double *z, int numverts, int DrawNow);
extern void FwriteTreeInorder(rb_red_blk_tree* tree, rb_red_blk_node* x);
extern void FwriteMagnitudeAndTwoObjects(int keyval, int Cnumber, double magnitude,
                        int index1, char gform1, int Lindex1, double LocalID1, int IDN1, double radius1, double height1,
                        double *x1, double *y1, double *z1, int numverts1,
                        int index2, char gform2, int Lindex2, double LocalID2, int IDN2, double radius2, double height2,
                        double *x2, double *y2, double *z2, int numverts2);
extern void FwritePointAndObject(int keyval, int Cnumber, double pointX, double pointY, double pointZ, int pointIDN,
                                  int index1, char gform1, int Lindex, double localid, int IDN1, double radius1, double height1,
                                  double *x1, double *y1, double *z1, int numverts1);
extern FILE * GlobalUseOpenGridFile(int filenumber);
extern void FwritePointAndTwoObjects(int keyval, int Cnumber, double pointX, double pointY, double pointZ,
                                  int index1, char gform1, int Lindex1, double localid1, int IDN1, double radius1, double height1,
                                  double *x1, double *y1, double *z1, int numverts1,
                                  int index2, char gform2, int Lindex2, double localid2, int IDN2, double radius2, double height2,
                                  double *x2, double *y2, double *z2, int numverts2);
extern void FwriteTwoObjects(int keyval, int Cnumber,
                  int IDN1, char gform1, int Lindex1, double localid1, int index1, double radius1, double height1,
                  int numverts1, double *x1, double *y1,  double *z1,
                  int IDN2, char gform2, int Lindex2, double localid2, int index2, double radius2, double height2,
                  int numverts2, double *x2, double *y2,  double *z2);
extern void FwriteObjectAndMessage(int keyval, int Cnumber,
                            int IDN, char gform, int Lindex, double localid, int index, double radius, double height,
                            double *x, double *y, double *z, int numverts, char *msg);
extern void FwriteObjectAndMagnitude(int keyval, int Cnumber, int IDN,
                             int index, char gform, int Lindex, double localid,  double radius, double height,
                             double magnitude, double *x, double *y, double *z, int numverts, int DrawNow);
extern void FwritePointEdgeAndMagnitude(int keyval, int Cnumber,  double magnitude, int IDN1, char gform1,
                                 int Lindex1, double localid1,                                  int index1, double px, double py, double pz, int index2, int IDN2, char gform2,
                                 int Lindex2, double localid2,
                                 double ex1, double ey1, double ez1, double ex2, double ey2, double ez2,
                                 double *x, double *y, double *z);
extern void FwritePointObjectAndMagnitude(int keyval, int Cnumber,  double magnitude, int IDN1, char gform1,
                                 int Lindex1, double localid1,
                                 int index1, double px, double py, double pz, int index2, int IDN2, char gform2,
                                 int Lindex2, double localid2, int numverts, double *x, double *y, double *z);
extern double FindRelevantSensitivity(int checktype, int checknum, double *sensitivity2);
extern int FindSpecificSensitivity(int number, int checktype, int checknum, double *answer);
extern int CombinedCheckApplies(int IDlookupkey, int Instance, int TgtCheckType, int LongListIndx, int ShortListIndx,
                         int config, int stratum, int domain, int *p1, int *p2, int *p3);
extern int GetNextArealIndex(struct RawAreal *a, int lastusedindex, int firstuse, struct RB_Edgelist ** RE, int cindex, int *UsedTree);extern double TurnAngle(double x1,double y1,double x2,double y2,double x3,double y3);
extern int ClipLineToRegion(double lx, double ly, double ux, double uy,
     double *x1, double *y1, double *z1, double *x2, double *y2, double *z2, unsigned char *cflag, double decimalplaces);
extern int MatchLinearSegment2D(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4,
                         double *matchlen, double tolerance);
extern int Make_Correct(int keyval, int socalledinstance);
extern double PointToSmallArealDist2D(double px,double py,struct RawAreal *A, int *index, double *T);
extern double PointToLargeArealDist2D(double px,double py,struct RawAreal *A, int *index, double *T);
extern int SensitivityCheck(int GT_LT_EQ, int checktype, int checknum, double checkvalue);
extern double SquaredDistance_GAITcoords(double x1,double y1,double x2,double y2);
extern double PointToLineDist2DwithFlag(double xj,double yj,double xk,double yk,double xl,double yl, double *T);
extern int PointIsVertex(double px,double py,double *x,double *y,double *z,int vtxcnt,double *elev);
extern int PointOnEdge(double px,double py,double *x,double *y,double *z,int vtxcnt,double *elev,int *index);
extern int PointIsInside(double px,double py,double *x,double *y,double *z,int vtxcnt, unsigned char config, double *elev);
extern int PointIsLargeArealVertex(double px,double py, struct RawAreal *A, double *elev);
extern int PointOnLargeArealEdge(double px,double py,struct RawAreal *A,double *elev,int *index);
extern int PointInsideLargeAreal(double px, double py, struct RawAreal * A, int *OnEdge);
extern int LineSegmentsConnect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double Slack);
extern int LineSegmentsIntersect(double xl,double yl,double xk,double yk,double xm,double ym,double xn,double yn,double * xi,double * yi);
extern int MergeSeg_LineSegmentsOverlap(double xl,double yl,double xk,double yk, /***double Alk, double Blk, double Clk, ****/
                        double xm,double ym,double xn,double yn,double x1min, double x1max, double y1min, double y1max,
                        double * xi1,double * yi1,double * xi2,double * yi2, double tolerance);
extern int LineSegmentsOverlap(double xl,double yl,double xk,double yk,double xm,double ym,double xn,double yn,
                        double * xi1,double * yi1,double * xi2,double * yi2, double tolerance);
extern int PointWithinToleranceOfVertex(double px,double py,double *x,double *y,double *z,int vtxcnt,double *elev, double tolerance);
extern int PointWithinToleranceOfEdge(double px,double py,double *x,double *y,double *z,int vtxcnt,double *elev,int *index, double tolerance);
extern double PointToArealEdgeDist2D(double xp,double yp,struct RawAreal *Aroot, int *Index);
extern int PointWithinToleranceOfLargeArealVertex(double px,double py, struct RawAreal *A, double *elev,double tolerance);
extern int PointWithinToleranceOfLargeArealEdge(double px,double py,struct RawAreal *A,double *elev,int *index,double tolerance);
extern int ReadLinearFromFile(FILE * linearfile, double decimalplaces);
extern int ReadPointFromFile(FILE * pointfile, double decimalplaces);
extern int ReadArealFromFile(FILE * arealfile, double decimalplaces);
extern int ReadGridFromFile(int Gindex,FILE * gridfile, double decimalplaces, int index, int NGrids, int *Mcols, int *Mres);
extern void CloseGridFile(FILE * gridfile);
extern int IsSentinelZvalue(double v, int CountNUNANPOvaluesAs2D);
extern int TwoLinesIntersect(struct RawLinear *l1, int l1index, struct RawLinear *l2);
extern int PointInsidePoly(double px,double py,double * x,double * y,int numb);
extern double XCoordToProjectCoord(double x);
extern double YCoordToProjectCoord(double y);
extern double SegmentToSmallArealDist2D(double lx1, double ly1, double lx2, double ly2,
                                 double minx,double miny,double maxx,double maxy,struct RawAreal *A, int *Aindex1, int *Aindex2);
extern double SegmentToLargeArealDist2D(double lx1, double ly1, double lx2, double ly2,
                                 double minx,double miny,double maxx,double maxy,struct RawAreal *A, int *Aindex1, int *Aindex2);
extern double AvgSegmentToSmallArealDist2D(double lx1, double ly1, double lx2, double ly2,
                                 double minx,double miny,double maxx,double maxy,struct RawAreal *A, int *Aindex1, int *Aindex2);
extern double AvgSegmentToLargeArealDist2D(double lx1, double ly1, double lx2, double ly2,
                                 double minx,double miny,double maxx,double maxy,struct RawAreal *A, int *Aindex1, int *Aindex2);
extern int SegmentsOffsetOverlap(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
extern double AvgSegmentToLineDist2D(double tgtslope, double lx1, double ly1, double lx2, double ly2,
                                 double minx,double miny,double maxx,double maxy,struct RawLinear *L, int *Lindex1, int *Lindex2);
extern double RadiansToDegrees(double radianmeasure);
extern unsigned char AssignDrawingColor(int fid, unsigned char D, int crsindex);
extern double ApproxPointToLineDist2D(double xj,double yj,double xk,double yk,double xl,double yl);
extern int RemoveByContainmentOrCoincidence(double px, double py, struct RawLinear *Lroot, int checkinstance, int check,
    double minx,double miny,double maxx,double maxy, double tolerance,     struct RawAreal *Aroot, struct RawPoint *Proot);
extern int RemoveByContainmentOrCoincidence(double px, double py, struct RawLinear *Lroot, int checkinstance, int check,
    double minx,double miny,double maxx,double maxy, double tolerance,
    struct RawAreal *Aroot, struct RawPoint *Proot);
extern double Line_SmallArealAngle(double px,double py, double ex, double ey, struct RawAreal *A, int *Aindex);
extern double Line_LargeArealAngle(double px,double py,double ex, double ey, struct RawAreal *A, int *Aindex);
extern double PointToLineDist2D(double xj,double yj,double xk,double yk,double xl,double yl);
extern int PointInsideTriangle(double x,double y,double x1,double y1,double x2,double y2,double x3,double y3);
extern rb_red_blk_node * RBTreeInsert(rb_red_blk_tree* tree, void* key, void* info);
extern int StrictlyBetween(double x,double y,double x1,double y1,double x2,double y2);
extern int Between(double x,double y,double x1,double y1,double x2,double y2);
extern int EqualWithinTolerance(double x1, double y1, double x2, double y2,double tolerance);
extern void CountConnectingNodeInts(int ECC, int NodePresent);
extern int NearlyEqual(double v1, double v2);
extern double PointToLineDist(double xp, double yp, double zp, double xl1, double yl1, double zl1, double xl2, double yl2, double zl2);extern void FindAzimuthInteriorPoint(double xb,double yb,double xe,double ye,
   double tdist,double dist,double *px,double *py);
extern int PointOnQuarterDegreeBoundary(double px, double py, double tolerance);
extern int SameAttrAndVals(struct CodesAndValue * CV1, struct CodesAndValue * CV2, int MetaDataCounts);
extern void RBDelete(rb_red_blk_tree* tree, rb_red_blk_node* z);
extern void LineEquation(double x1,double y1,double x2,double y2,double * A,double * B,double * C);
extern void ImplicitLineToParametric(double A, double B, double C, double *X0, double *Y0, double *F, double *G);
extern int LineCircleIntersection(double XJ,double YJ,double RJ,double X0,double Y0,double F,double G,
          double  *X1,double *Y1,double *X2,double *Y2);
extern double FindXatYonSegment(double newy, double x1, double y1, double x2, double y2);
extern int PointIsAlmostVertex(double px,double py,double *x,double *y,double *z,int vtxcnt,double *elev);
extern int PointIsAlmostLargeArealVertex(double px,double py, struct RawAreal *A, double *elev);
extern int CheapLineSegmentsConnect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double Sl);extern double FindMaxSensitivities(int checktype, double *sensitivity2);
extern int PointInAreal(double px,double py,double * x,double * y,int numverts, int *OnEdge);
extern double PointToArealDist2D(double xp,double yp,struct RawAreal *Aroot);
extern double AngleBetweenLineSegments(double lx1,double ly1,double lx2,double ly2,double x1,double y1,double x2,double y2);
extern void FreeLinearsList(struct RawLinear *root);
extern double RoundToNearestPartialDegree(int XorY, double ActualCoord, double increment);
extern double PointToFullLineDist2D(double px, double py, double lx1, double ly1, double lx2, double ly2);
extern int EdgesIntersectAtVertex(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
extern int LineSegmentIntersectsLargeAreal(int tgtindex, double x1, double y1, double x2, double y2,
             struct RawAreal * A, int *OnEdge, double *xi, double *yi, int MallocSize);
extern void CreatePointInsideAreal(struct RawAreal *A, double *px, double *py);
extern int EdgesOverlap(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4,
                     double *cx1, double *cy1, double *cx2, double *cy2, int *bothmatch, double ptltolerance);
extern int TwoArealsIntersect(struct RawAreal *A, struct RawAreal *B,
                               double *xi, double *yi, int *Aindex, int *Bindex, int *ArealsTouch);
extern int TwoArealsShareEdge(struct RawAreal *A, struct RawAreal *B, int *Aindex, int *Bindex, double tolerance);
extern int GetAttrIndex(char *ecc);
extern int GetCodeIndex(char *ecc);
extern char * ParseGAITgeometry(unsigned char geom, int CaseFlag);
extern int IsJunkValue(int datatype,int Ivalue, double Fvalue, char *Svalue, int Evalue, char *Elabel);
extern int ConditionOnDataEdge(double testx1, double testy1, double testx2, double testy2,
                    struct RawAreal *Aroot, struct RawPoint *Proot, struct RawLinear *Lroot,
                    struct RawAreal *ThisA, struct RawPoint *ThisP, struct RawLinear *ThisL,
                    int Ncount, int Scount, int Ecount, int Wcount, int checkinstance);
/***extern int FindLengthLimitedPath(double fromx, double fromy, double fromz, double tgtx, double tgty, double tgtz,
                           double limit, double distsofar, int checkinstance); ****/
extern void fix_endian(char *data, int data_type);
extern int CalculateBytesPerGridValue(int flag);
char * FindCONFLATEsourceFile(int localid, int * entry);



struct ToCollect *TCroot, *TCn, *TCp, *TCc;
struct LM_ConflateStats *CSroot, *CSn, *CSp, *CSc;



void SummarizeRoutePlanningResults(void)
{
extern int RouteProblem1; /** route planner called with no POI loaded ***/
extern int RouteProblem2; /** route planner caller with no Start and Goal for Network **/
extern int RouteProblem3; /** route planner called with start / goal too far from specified network node **/
extern int RouteProblem4; /** no route exists in specified network from specified start to specified goal **/
extern int RoutePlanningAttempts; /** number of times attempted to plan a route **/
int i;

if(RoutePlanningAttempts > 0)
   {
   if(RoutePlanningAttempts > 1)
      printf("Route planning was attempted on %d individual networks\n",RoutePlanningAttempts);
   else
      printf("Route planning was attempted on %d network\n",RoutePlanningAttempts);

   if(RouteProblem1 > 1)
      printf("   On %d of the attempts, no Points of Interest were loaded, so no route planning was attempted\n",RouteProblem1);
   else if(RouteProblem1 > 0)
      printf("   On %d attempt, no Points of Interest were loaded, so no route planning was attempted\n",RouteProblem1);

   if(RouteProblem2 > 1)
      printf("   On %d of the attempts, no applicable Start and/or Goal was specified, so no route planning was attempted\n",
                     RouteProblem2);
   else if(RouteProblem2 > 0)
      printf("   On %d attempt, no applicable Start and/or Goal was specified, so no route planning was attempted\n",
                     RouteProblem2);

   if(RouteProblem3 > 1)
      printf("   On %d of the attempts, the specified Start and/or Goal were too far from\n      the specified network, so no route planning was attempted\n",
                     RouteProblem3);
   else if(RouteProblem3 > 0)
      printf("   On %d attempt, the specified Start and/or Goal were too far from\n      the specified network, so no route planning was attempted\n",
                     RouteProblem3);

   if(RouteProblem4 > 1)
      printf("   On %d of the attempts, no path from the specified Start to the specified Goal could be found\n",
                     RouteProblem4);
   else if(RouteProblem4 > 0)
      printf("   On %d attempt, no path from the specified Start to the specified Goal could be found\n",
                     RouteProblem4);

   i = RouteProblem1 + RouteProblem2 + RouteProblem3 + RouteProblem4;
   if(i > 0)
      printf("Route planning was successful on %d of the attempts\n\n",RoutePlanningAttempts - i);
   else if(RoutePlanningAttempts > 1)
      printf("All planning attempts were successful\n\n");
   else
      printf("This attempt was successful\n\n");
   }

RouteProblem1 = 0;
RouteProblem2 = 0;
RouteProblem3 = 0;
RouteProblem4 = 0;
RoutePlanningAttempts = 0;

return;
}




double DblSolveForY(double x,double A,double B,double C)
{
double y;
  if(B == 0)
     return((double) x);
  y = ((A*x) + C) / B;
  y = -1.0 * y;
  return(y);
}






void MassReadDoubleGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
                                   double *values, int NumToRead)
{
int seekmult;
int i;
int gridtypeflag = 9;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);

   fread(values, seekmult, NumToRead, gridfile);

   if (machine_endianness != file_endianness)
      {
      for(i=0; i < NumToRead; i++)
         fix_endian( (char *) &values[i], 2);   /** 2 is the flag for double data type ***/
      }

  return;
}

void MassReadFloatGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
                                   float *values, int NumToRead)
{
int seekmult;
int i;
int gridtypeflag = 6;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);  

   fread(values, seekmult, NumToRead, gridfile);

   if (machine_endianness != file_endianness)
      {
      for(i=0; i < NumToRead; i++)
         fix_endian( (char *) &values[i], 5);   /** 5 is the flag for float data type ***/
      }

  return;
}


void MassReadUnCharGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
         unsigned char *values, int NumToRead)
{
int seekmult;
int gridtypeflag = 7;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);  

   fread(values, seekmult, NumToRead, gridfile);

  return;
}


void MassReadCharGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
         char *values, int NumToRead)
{
int seekmult;
int gridtypeflag = 1;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);

   fread(values, seekmult, NumToRead, gridfile);

  return;
}


void MassReadShIntGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum, 
                                 short int *values, int NumToRead)
{
int seekmult;
int i;
int gridtypeflag = 2;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);

   fread(values, seekmult, NumToRead, gridfile);

   if (machine_endianness != file_endianness)
      {
      for(i=0; i < NumToRead; i++)
         fix_endian( (char *) &values[i], 4);   /** 4 is the flag for short int data type ***/
      }

  return;
}


void MassReadUnShIntGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
                                 unsigned short int *values, int NumToRead)
{
int seekmult;
int i;
int gridtypeflag = 3;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);

   fread(values, seekmult, NumToRead, gridfile);

   if (machine_endianness != file_endianness)
      {
      for(i=0; i < NumToRead; i++)
         fix_endian( (char *) &values[i], 4);   /** 4 is the flag for short int data type ***/
      }

  return;
}



void MassReadIntGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
                                 int *values, int NumToRead)
{
int seekmult;
int i;
int gridtypeflag = 4;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);

   fread(values, seekmult, NumToRead, gridfile);

   if (machine_endianness != file_endianness)
      {
      for(i=0; i < NumToRead; i++)
         fix_endian( (char *) &values[i], 0);   /** 0 is the flag for int data type ***/
      }

  return;
}


void MassReadUnIntGridValues(FILE *gridfile,long int gridstart,int xindex,int yindex,int xnum, int ynum,
                                 unsigned int *values, int NumToRead)
{
int seekmult;
int i;
int gridtypeflag = 5;

   seekmult = CalculateBytesPerGridValue(gridtypeflag);

   fseek(gridfile,gridstart + (yindex*xnum + xindex)*seekmult,SEEK_SET);

   fread(values, seekmult, NumToRead, gridfile);

   if (machine_endianness != file_endianness)
      {
      for(i=0; i < NumToRead; i++)
         fix_endian( (char *) &values[i], 0);   /** 0 is the flag for int data type ***/
      }

  return;
}



void BB_FwriteDynamicInfo(FILE * bbout, int keyval, int Cnumber, int cinstance, double Wflag, int PtUsed, int MagUsed, int NumObjects,
                                  char *msg, double magnitude, double pointX, double pointY, double pointZ,
                                  int index1, char gform1, int Lindex1, double localid1, int IDN1,
                                  double radius1, double height1,
                                  double *x1, double *y1, double *z1, int numverts1,
                                  int index2, char gform2, int Lindex2, double localid2, int IDN2,
                                  double radius2, double height2,
                                  double *x2, double *y2, double *z2, int numverts2)
{
int i,slen;
char ML2[300];
/**FILE *fout;**/
   if(bbout == NULL)
      return;


   SEEIT_fwrite_int(&keyval,bbout);
   SEEIT_fwrite_int(&Cnumber,bbout);
   SEEIT_fwrite_int(&cinstance,bbout);
   SEEIT_fwrite_double(&Wflag,bbout);
   SEEIT_fwrite_int(&PtUsed,bbout);
   SEEIT_fwrite_int(&MagUsed,bbout);
   SEEIT_fwrite_int(&NumObjects,bbout);
   if(msg != NULL)
      {
      slen = strlen(msg);
      SEEIT_fwrite_int(&slen,bbout);
      fwrite(&msg[0],1,slen,bbout);
      }
   else
      {
      slen = 0;
      SEEIT_fwrite_int(&slen,bbout);
      }

   if(MagUsed > 0)
      {
      SEEIT_fwrite_double(&magnitude,bbout);
      }

   pointX = pointY = pointZ = -1.0;
   if(PtUsed > 0)
      {
      SEEIT_fwrite_double(&pointX,bbout);
      SEEIT_fwrite_double(&pointY,bbout);
      SEEIT_fwrite_double(&pointZ,bbout);
      }

   if(NumObjects > 0)
      {
      SEEIT_fwrite_int(&IDN1,bbout);
      GetAppropIDnumber(IDN1, &MessageLine[0]);
      i = strlen(MessageLine);
      SEEIT_fwrite_int(&i,bbout);
      fwrite(&MessageLine[0],i,1,bbout); /** writing 'i' 1-byte chars, endian neutral, so just use a mass write **/
      SEEIT_fwrite_int(&SCCtable[index1].ECC,bbout);
      fwrite(&gform1,1,1,bbout);
      SEEIT_fwrite_int(&Lindex1,bbout);
      SEEIT_fwrite_double(&localid1, bbout);
      SEEIT_fwrite_double(&radius1, bbout);
      SEEIT_fwrite_double(&height1, bbout);
      SEEIT_fwrite_int(&numverts1,bbout);

      for(i=0; i<numverts1; i++)
         {
         SEEIT_fwrite_double(&x1[i],bbout);
         SEEIT_fwrite_double(&y1[i],bbout);
         SEEIT_fwrite_double(&z1[i],bbout);
         }
      }
   if(NumObjects > 1)
      {
      SEEIT_fwrite_int(&IDN2,bbout);
      GetAppropIDnumber(IDN2, &ML2[0]);
      i = strlen(ML2);
      SEEIT_fwrite_int(&i,bbout);
      fwrite(&ML2[0],i,1,bbout); /** writing 'i' 1-byte chars, endian neutral, so just use a mass write **/
      SEEIT_fwrite_int(&SCCtable[index2].ECC,bbout);
      fwrite(&gform2,1,1,bbout);
      SEEIT_fwrite_int(&Lindex2,bbout);
      SEEIT_fwrite_double(&localid2, bbout);
      SEEIT_fwrite_double(&radius2, bbout);
      SEEIT_fwrite_double(&height2, bbout);
      SEEIT_fwrite_int(&numverts2,bbout);

      for(i=0; i<numverts2; i++)
         {
         SEEIT_fwrite_double(&x2[i],bbout);
         SEEIT_fwrite_double(&y2[i],bbout);
         SEEIT_fwrite_double(&z2[i],bbout);
         }
      }

   if((keyval != RAISEDPC) && (keyval != WATERMMU) && (keyval != FLOWSTEP) &&
          (keyval != BREAKLINE) && (keyval != AVGSPIKE) && (keyval != GSHELF) )
      {
      DrawNewCondition(keyval, Cnumber, pointX,pointY,pointZ,PtUsed,  /**** px, py, pz, PtUsed ***/
             Lindex1, index1, gform1, MessageLine,
             x1, y1, z1, numverts1,
             Lindex2, index2, gform2,ML2, x2,y2,z2,numverts2);

      NUMBEROFCONDITIONS += 1;
      }
}








void FwriteDynamicInfo(int keyval, int Cnumber, int PtUsed, int MagUsed, int NumObjects,
                                  char *msg, double magnitude, double pointX, double pointY, double pointZ,
                                  int index1, char gform1, int Lindex1, double localid1, int IDN1,
                                  double radius1, double height1,
                                  double *x1, double *y1, double *z1, int numverts1,
                                  int index2, char gform2, int Lindex2, double localid2, int IDN2,
                                  double radius2, double height2,
                                  double *x2, double *y2, double *z2, int numverts2)
{
int i,slen;
char ML2[300];
/**FILE *fout;**/
   if(unsortout == NULL)
      return;


   SEEIT_fwrite_int(&keyval,unsortout);
   SEEIT_fwrite_int(&Cnumber,unsortout);
   SEEIT_fwrite_int(&PtUsed,unsortout);
   SEEIT_fwrite_int(&MagUsed,unsortout);
   SEEIT_fwrite_int(&NumObjects,unsortout);
   if(msg != NULL)
      {
      slen = strlen(msg);
      SEEIT_fwrite_int(&slen,unsortout);
      fwrite(&msg[0],1,slen,unsortout);
      }
   else
      {
      slen = 0;
      SEEIT_fwrite_int(&slen,unsortout);
      }

   if(MagUsed > 0)
      {
      SEEIT_fwrite_double(&magnitude,unsortout);
      }

   if(PtUsed > 0)
      {
      SEEIT_fwrite_double(&pointX,unsortout);
      SEEIT_fwrite_double(&pointY,unsortout);
      SEEIT_fwrite_double(&pointZ,unsortout);
      }

   if(NumObjects > 0)
      {
      SEEIT_fwrite_int(&IDN1,unsortout);
      GetAppropIDnumber(IDN1, &MessageLine[0]);
      i = strlen(MessageLine);
      SEEIT_fwrite_int(&i,unsortout);
      fwrite(&MessageLine[0],i,1,unsortout); /** writing 'i' 1-byte chars, endian neutral, so just use a mass write **/
      SEEIT_fwrite_int(&SCCtable[index1].ECC,unsortout);
      fwrite(&gform1,1,1,unsortout);
      SEEIT_fwrite_int(&Lindex1,unsortout);
      SEEIT_fwrite_double(&localid1, unsortout);
      SEEIT_fwrite_double(&radius1, unsortout);
      SEEIT_fwrite_double(&height1, unsortout);
      SEEIT_fwrite_int(&numverts1,unsortout);

      for(i=0; i<numverts1; i++)
         {
         SEEIT_fwrite_double(&x1[i],unsortout);
         SEEIT_fwrite_double(&y1[i],unsortout);
         SEEIT_fwrite_double(&z1[i],unsortout);
         }
      }

   if(NumObjects > 1)
      {
      SEEIT_fwrite_int(&IDN2,unsortout);
      GetAppropIDnumber(IDN2, &ML2[0]);
      i = strlen(ML2);
      SEEIT_fwrite_int(&i,unsortout);
      fwrite(&ML2[0],i,1,unsortout); /** writing 'i' 1-byte chars, endian neutral, so just use a mass write **/
      SEEIT_fwrite_int(&SCCtable[index2].ECC,unsortout);
      fwrite(&gform2,1,1,unsortout);
      SEEIT_fwrite_int(&Lindex2,unsortout);
      SEEIT_fwrite_double(&localid2, unsortout);
      SEEIT_fwrite_double(&radius2, unsortout);
      SEEIT_fwrite_double(&height2, unsortout);
      SEEIT_fwrite_int(&numverts2,unsortout);

      for(i=0; i<numverts2; i++)
         {
         SEEIT_fwrite_double(&x2[i],unsortout);
         SEEIT_fwrite_double(&y2[i],unsortout);
         SEEIT_fwrite_double(&z2[i],unsortout);
         }
      }

   DrawNewCondition(keyval, Cnumber, pointX,pointY,pointZ,PtUsed,  /**** px, py, pz, PtUsed ***/
             Lindex1, index1, gform1, MessageLine,
             x1, y1, z1, numverts1,
             Lindex2, index2, gform2,ML2, x2,y2,z2,numverts2);

   NUMBEROFCONDITIONS += 1;
}



void SkipDynamicInfo(int keyval, FILE *infile)
{
  int PtUsed; 
  int MagUsed; 
  int NumObjects;
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

  SEEIT_fread_int(&PtUsed,infile);
  SEEIT_fread_int(&MagUsed,infile);
  SEEIT_fread_int(&NumObjects,infile);
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
  if(MagUsed > 0)
     SEEIT_fread_double(&magnitude,infile);

  if(PtUsed > 0)
     {
     SEEIT_fread_double(&pointx, infile);
     SEEIT_fread_double(&pointy, infile);
     SEEIT_fread_double(&pointz, infile);
     }

  if(NumObjects > 0)
     {
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
     }

  /** second object ***/
  if(NumObjects > 1)
     {
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
     }

  if(slen >= 500)
    {
    free(bigmessage);
    bigmessage = NULL;
    }

  return;
}






void FreeConflateStatsCapture(int AllOrLMonly)
{
   CSc = CSroot;
   while(CSc != NULL)
      {
      TCc = CSc->TC;
      while(TCc != NULL)
         {
         TCp = TCc;
         TCc = TCc->next;
         free(TCp);
         }
      CSc->TC = NULL;
      CSp = CSc;
      CSc = CSc->next;
      if(AllOrLMonly == 1)
         free(CSp);
      }
   if(AllOrLMonly == 1)
      CSroot = NULL;
}

void InitConflateStatsCapture(void)
{
FILE *filesin;
char ECC[25], EAC[25],geomflag;
int i,j, mark,index;
int SzLMC = sizeof(struct LM_ConflateStats);
int SzTC = sizeof(struct ToCollect);
char junk[2000];

   CSroot = NULL;
   TCroot = NULL;
   if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
     {
     sprintf(junk,"%sinspection_files\\MarkersForAttributeStats.csv",indirectory);
     }
   else
     {
     sprintf(junk,"%sinspection_files/MarkersForAttributeStats.csv",indirectory);
     }
   filesin = fopen(junk,"rt");
   if(filesin == NULL)
      return;
   fgets(junk,1999,filesin); /*** skip the header ***/
   fgets(junk,1999,filesin);
   index = 1;
   while(feof(filesin) == 0)
      {
      mark = 0;
      if(junk[0] == '1') /*** a file has been turned on for stats capture ***/
         {
         if(strncmp(&junk[2],"Triangle",8) == 0)
            mark = 13;
         else if(strncmp(&junk[2],"Star",4) == 0)
            mark = 9;
         else if(strncmp(&junk[2],"Circle",6) == 0)
            mark = 5;
         else
            mark = 1; /** will default to an 'X' mark if all fails and can't parse ***/

         CSn = (struct LM_ConflateStats *) (malloc(SzLMC));
         CSn->marktype = mark;
         CSn->fileindex = index;
         CSn->TC = NULL;
         CSn->next = NULL;
         if(CSroot == NULL)
            CSroot = CSn;
         else
            CSp->next = CSn;

         CSp = CSn;
         }
      fgets(junk,1999,filesin);
      ++index;
      }
   fclose(filesin);

   if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
     {
     sprintf(junk,"%sinspection_files\\CollectAttrStats.csv",indirectory);
     }
   else
     {
     sprintf(junk,"%sinspection_files/CollectAttrStats.csv",indirectory);
     }

   filesin = fopen(junk,"rt");
   if(filesin == NULL)
      return;
   ECC[0] = EAC[0] = '\0';
   fgets(junk,1999,filesin);
   while(feof(filesin) == 0)
      {
      if(junk[0] == '1') /** something has been turned on for stat collection ****/
         {
      j = 0;
      while(junk[j] != ',')
         ++j;
      ++j;
      while(junk[j] != ',')
         ++j;
      ++j;
      i = 0;
      while(junk[j] != ',')
         {
         EAC[i] = junk[j];
         ++j;
         ++i;
         EAC[i] = '\0';
         }
      ++j;
      while(junk[j] != ',')
         ++j;
      ++j;
      i = 0;
      while(junk[j] != ',')
         {
         ECC[i] = junk[j];
         ++j;
         ++i;
         ECC[i] = '\0';
         }
      ++j;
      geomflag = junk[j];


      TCn = (struct ToCollect *) (malloc(SzTC));
      if(TCn == NULL)
         break;
      TCn->EAC = GetAttrIndex(EAC);
      TCn->ECC = GetCodeIndex(ECC);
      if(geomflag == 'P')
         TCn->geom = 5;
      else if(geomflag == 'A')
         TCn->geom = 1;
      else if(geomflag == 'L')
         TCn->geom = 4;
      else
         TCn->geom = 0;
      TCn->good = 0;
      TCn->bad = 0;
      TCn->next = NULL;

      if(TCroot == NULL)
          TCroot = TCn;
      else
          TCp->next = TCn;
      TCp = TCn;
         }

      fgets(junk,1999,filesin);
      }
   fclose(filesin);

}



char * BLN_FindCONFLATEsourceFile(int line_number, FILE * sfn_in)
{
int i,entrynumber;
static char answer[500];
char junk[1050];

   strcpy(answer,"Unable to identify source file");
   rewind(sfn_in);
   fgets(junk,1049,sfn_in);
   entrynumber = 1;
   while(feof(sfn_in) == 0)
      {
      if(entrynumber == line_number)
         {
         if(strstr(junk," BadFile") == NULL)
            {
            i = 0;
            while(junk[i] != ' ') /** skip the 'from' index number **/
               ++i;
            while(junk[i] != ',')
               ++i;
            while(junk[i] != ' ')
               ++i;
            ++i;
            while(junk[i] != ' ') /*** skip the 'to' index number ****/
               ++i;
            ++i;

            strcpy(answer,&junk[i]);
            i = strlen(answer) - 1;
            while((i > 0) && (answer[i] < ' '))
               {
               answer[i] = '\0';
               --i;
               }
            break;
            }
         }

      fgets(junk,1049,sfn_in);
      ++entrynumber;
      }

   return(answer);
}




void FwriteStatsTreeInorder(rb_red_blk_tree* tree, rb_red_blk_node* x, FILE *fout, FILE *filesin)
{
struct ConflateStats *CS;

   if (x != tree->nil)
      {
      FwriteStatsTreeInorder(tree,x->left, fout, filesin);

      CS = (struct ConflateStats *) (x->info);

      if((CS != NULL) && (CS->goodval > 0))
         {
         fprintf(fout,"%s,", BLN_FindCONFLATEsourceFile(CS->line_number, filesin));
         //fprintf(fout,"%d,",CS->line_number);
         fprintf(fout,"%s,%s,",GetECCCode(CS->ECC),GetECCLabel(CS->ECC));
         fprintf(fout,"%s,%s,",GetEACCode(CS->EAC),GetEACLabel(CS->EAC));
         fprintf(fout,"%d,%d\n",CS->goodval,CS->junkval);
         }
      FwriteStatsTreeInorder(tree,x->right, fout, filesin);
      }

}




void CaptureInformativeValueStats(struct RawLinear *rlc, int Lindex, int localid, int loadmodule)
{
char EElabel[500];
char tempkey[500];
char *rb_key;
char svalue[500];
double dvalue;
int EACcode, ECCcode, tgttype;
int i,j,k,file_line_number, goodval, junkval;
int longattrs, geomindex,datatype;
int SzCS = sizeof(struct ConflateStats);
struct ConflateStats *RB_stats;

   j = Lindex;
   FindCONFLATEsourceFile(localid, &file_line_number);

   j = CrsWlk[Lindex].crossindex; 
   geomindex = CrsWlk[Lindex].geomtype; 

   ECCcode = MdlNames[j].code;

   for(k=0; k<MdlNames[j].numattributes; ++k)
      {
      for(i=0; i<MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].LLindexcount; i++)
         {
         if(MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].LongListIndices[i] == Lindex)
            {
            EACcode = MdlNames2[MdlNames[j].AttrIndices[k]].code;
            tgttype = MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].type;

            goodval = junkval = 0;
            switch(tgttype)
               {
               case 0:  /** an int **/
                  if(IsJunkValue(tgttype,
                          (int) MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].value,
                                 -1312452,NULL,-1312452,NULL) == 0)
                     {
                     goodval = 1;
                     }
                  else
                     {
                     junkval = 1;
                     }
                  break;
               case 1: /** double **/
                  if(IsJunkValue(tgttype,-1312452,
                      MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].value,
                             NULL,-1312452,NULL) == 0)
                     {
                     goodval = 1;
                     }
                  else
                     {
                     junkval = 1;
                     }
                  break;
               case 2: /** string value **/
                   if(IsJunkValue(tgttype,-1312452,-1312452,
                     MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].label,
                       -1312452,NULL) == 0)
                     {
                     goodval = 1;
                     }
                  else
                     {
                     junkval = 1;
                     }
                  break;
               case 3: /** enum - print label **/
                  strcpy(EElabel,GetEELabel(EACcode,
                    (int) (MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].value)));
                  if(IsJunkValue(tgttype,-1312452,-1312452,NULL,
                     (int) (MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].value),
                           EElabel) == 0)
                     {
                     goodval = 1;
                     }
                  else
                     {
                     junkval = 1;
                     }
                  break;
               default:
                  printf("  unknown type %d\n",
                           MdlNames2[MdlNames[j].AttrIndices[k]].values[MdlNames[j].ValueIndices[k]].type);
                  break;
               } /*** end switch ***/
            if((goodval > 0) || (junkval > 0))
               {
               sprintf(tempkey,"%d_%d_%d_%d",file_line_number,/** entry line # in input files list ***/
                                             ECCcode, /*** the ECC code ***/
                                             geomindex,  /** geom flag **/
                                             EACcode); /** EAC code **/
               if(( RB_newNode = RBExactQuery(Stats_RB_Tree,tempkey)))
                  {
                  RB_stats = (struct ConflateStats *) RB_newNode->info;
                  RB_stats->goodval += goodval;
                  RB_stats->junkval += junkval;
                  }
               else /***if(goodval > 0)***  add them all in so can get % counts ****/
                  {
                  rb_key = (char *) (malloc(strlen(tempkey) + 1));
                  strcpy(rb_key,tempkey);
                  RB_stats = (struct ConflateStats *) (malloc(SzCS));
                  if(RB_stats == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree information alloction for PERIMLEN analysis\n");
                     exit(-1);
                     }
                  RB_stats->ECC = ECCcode;
                  RB_stats->EAC = EACcode;
                  RB_stats->geom = geomindex;
                  RB_stats->line_number = file_line_number;
                  RB_stats->keyval = CONF_STATS;
                  RB_stats->goodval = goodval;
                  RB_stats->junkval = junkval;
                  RBTreeInsert(Stats_RB_Tree,rb_key,RB_stats);
                  }
               CSc = CSroot;
               while(CSc != NULL)
                  {
                  if(CSc->fileindex == file_line_number)
                     break;
                  CSc = CSc->next;
                  }
               if(CSc != NULL) /*** this file is subject to stats collection for POI marks ***/
                  {
                  TCc = TCroot;
                  while(TCc != NULL)
                     {
                     if((TCc->ECC == ECCcode) && (TCc->EAC == EACcode) &&
                                 (TCc->geom == geomindex))
                        break;
                     TCc = TCc->next;
                     }
                  if(TCc != NULL) /** this ECC, EAC && geom are subject to collection as well ***/
                     {
                     TCp = CSc->TC;
                     while(TCp != NULL)
                        {
                        if((TCp->ECC == TCc->ECC) && (TCp->EAC == TCc->EAC) && (TCp->geom == TCc->geom))
                           {
                           TCp->good += goodval;
                           TCp->bad += junkval;
                           break;
                           }
                        TCp = TCp->next;
                        }
                     if(TCp == NULL)
                        {
                        TCn = (struct ToCollect *) (malloc(sizeof(struct ToCollect)));
                        TCn->ECC = TCc->ECC;
                        TCn->EAC = TCc->EAC;
                        TCn->geom = TCc->geom;
                        TCn->good = goodval;
                        TCn->bad = junkval;
                        TCn->next = CSc->TC;
                        CSc->TC = TCn;
                        }
                     }
                  }
               } /*** end make or update an RB Tree entry ***/

            break;

            } /*** end if(MdlNames2[ . . .].LongListIndices[i] == Lindex) ***/
         } /*** end for(i=0; i<MdlNames2[ . . . ].LLindexcount; ***/
      } /*** k=0; k<MdlNames[j].numattributes ***/

   for(longattrs = 0; longattrs < 5; longattrs++)
      {
      goodval = 0;
      junkval = 0;
      dvalue = -1312452;
      svalue[0] = '\0';
      datatype = -1;
      switch(longattrs)
         {
         case 0:
            EACcode = GetLENCode();
            if(EACcode >= 0)
               {
               if(rlc->lenattr != MY2DSENTINEL)
                  {
                  dvalue = rlc->lenattr;
                  datatype = 1;
                  }
               else if(rlc->lenstr != NULL)
                  {
                  datatype = 2;
                  strcpy(svalue,rlc->lenstr);
                  }
               }
            break;
         case 1:
            EACcode = GetZVCode();
            if(EACcode >= 0)
               {
               if(rlc->zv2attr != MY2DSENTINEL)
                  {
                  dvalue = rlc->zv2attr;
                  datatype = 1;
                  }
               else if(rlc->zv2str != NULL)
                  {
                  datatype = 2;
                  strcpy(svalue,rlc->zv2str);
                  }
               }
            break;
         case 2:
            EACcode = GetHGTCode();
            if(EACcode >= 0)
               {
               if(rlc->hgtattr != MY2DSENTINEL)
                  {
                  dvalue = rlc->hgtattr;
                  datatype = 1;
                  }
               else if(rlc->hgtstr != NULL)
                  {
                  datatype = 2;
                  strcpy(svalue,rlc->hgtstr);
                  }
               }
            break;
         case 3:
            EACcode = GetNAMCode();
            if(EACcode >= 0)
               {
               if(rlc->namattr != MY2DSENTINEL)
                  {
                  dvalue = rlc->namattr;
                  datatype = 1;
                  }
               else if(rlc->namstr != NULL)
                  {
                  datatype = 2;
                  strcpy(svalue,rlc->namstr);
                  }
               }
            break;
         case 4:
            EACcode = GetAREACode();
            if(EACcode >= 0)
               {
               if(rlc->areaattr != MY2DSENTINEL)
                  {
                  dvalue = rlc->areaattr;
                  datatype = 1;
                  }
               else if(rlc->areastr != NULL)
                  {
                  datatype = 2;
                  strcpy(svalue,rlc->areastr);
                  }
               }
            break;
         } /*** end switch ***/
      if(datatype >= 0)
         {
         if(IsJunkValue(datatype,-1312452,dvalue,svalue,-1312452,NULL) == 0)
            goodval = 1;
         else
            junkval = 1;
         }
      if((goodval > 0) || (junkval > 0))
         {
         sprintf(tempkey,"%d_%d_%d_%d",file_line_number,/** entry line # in input files list ***/
                              ECCcode, /*** the ECC code ***/
                              tgttype, EACcode);
         if(( RB_newNode = RBExactQuery(Stats_RB_Tree,tempkey)))
            {
            RB_stats = (struct ConflateStats *) RB_newNode->info;
            RB_stats->goodval += goodval;
            RB_stats->junkval += junkval;
            }
         else /***if(goodval > 0)***  add them all in so can get % counts ****/
            {
            rb_key = (char *) (malloc(strlen(tempkey) + 1));
            strcpy(rb_key,tempkey);
            RB_stats = (struct ConflateStats *) (malloc(SzCS));
            if(RB_stats == NULL)
      {
               printf("memory has been exhausted during RB-Tree information alloction for PERIMLEN analysis\n");
               exit(-1);
               }
            RB_stats->ECC = ECCcode;
            RB_stats->EAC = EACcode;
                     RB_stats->geom = geomindex;
            RB_stats->line_number = file_line_number;
            RB_stats->keyval = CONF_STATS;
            RB_stats->goodval = goodval;
            RB_stats->junkval = junkval;
            RBTreeInsert(Stats_RB_Tree,rb_key,RB_stats);
            }
         CSc = CSroot;
         while(CSc != NULL)
            {
            if(CSc->fileindex == file_line_number)
               break;
            CSc = CSc->next;
            }
         if(CSc != NULL) /*** this file is subject to stats collection for POI marks ***/
            {
            TCc = TCroot;
            while(TCc != NULL)
               {
               if((TCc->ECC == RB_stats->ECC) && (TCc->EAC == EACcode) &&
                           (TCc->geom == geomindex))
                  break;
               TCc = TCc->next;
               }
            if(TCc != NULL) /** this ECC, EAC && geom are subject to collection as well ***/
               {
               TCp = CSc->TC;
               while(TCp != NULL)
                  {
                  if((TCp->ECC == TCc->ECC) && (TCp->EAC == TCc->EAC) && (TCp->geom == TCc->geom))
                     {
                     TCp->good += goodval;
                     TCp->bad += junkval;
                     break;
                     }
                  TCp = TCp->next;
                  }
               if(TCp == NULL)
                  {
                  TCn = (struct ToCollect *) (malloc(sizeof(struct ToCollect)));
                  TCn->ECC = TCc->ECC;
                  TCn->EAC = TCc->EAC;
                  TCn->geom = TCc->geom;
                  TCn->good = goodval;
                  TCn->bad = junkval;
                  TCn->next = CSc->TC;
                  CSc->TC = TCn;
                  }
               }
            }
         } /*** end make or update a long attr RB Tree entry ***/
      } /**** end dealing with long attributes ***/

}  /** end CaptureInformativeValueStats  ***/





char * FindCONFLATEsourceFile(int localid, int * entry)
{
FILE * sfn_in;
int i,negsign,entrynumber;
int from, to;
static char answer[500];
char junk[1050];

   strcpy(answer,"Unable to identify source file");
   sprintf(answer,"Unable to identify source file: %d",localid);
   sfn_in = fopen(ConflateSourceFiles,"rt");
   fgets(junk,1049,sfn_in);
   entrynumber = 1;
   while(feof(sfn_in) == 0)
      {
      if(strstr(junk," BadFile") == NULL)
         {
         i = 0;
         from = 0;
         negsign = 0;
         while(junk[i] != ' ')
            {
            if(junk[i] == '-')
               negsign = 1;
            else if((junk[i] >= '0') && (junk[i] <= '9'))
               from = (from * 10) + (int) junk[i] - 48;
            ++i;
            }
         if(negsign > 0)
            from = 0 - from;
         while(junk[i] != ',')
            ++i;
         while(junk[i] != ' ')
            ++i;
         while(junk[i] == ' ')
            ++i;
         to = 0;
         negsign = 0;
         while(junk[i] != ' ')
            {
            if(junk[i] == '-')
               negsign = 1;
            else if((junk[i] >= '0') && (junk[i] <= '9'))
               to = (to * 10) + (int) junk[i] - 48;
            ++i;
            }
         if(negsign > 0)
            to = 0 - to;
      
         if((from <= localid) && (to >= localid))
            {
            strcpy(answer,&junk[i]);
            i = strlen(answer) - 1;
            while((i > 0) && (answer[i] < ' '))
               {
               answer[i] = '\0';
               --i;
               }
            break;
            }
         }

      fgets(junk,1049,sfn_in);
      ++entrynumber;
      }

   fclose(sfn_in);
   *entry = entrynumber;
   return(answer);
}

double FindAcuteAngleBetweenSegments(double x1, double y1, double x2, double y2,
                                           double x3, double y3, double x4, double y4)
{
double A1, B1, C1;
double A2, B2, C2;
double answer;

   LineEquation(x1,y1,x2,y2,&A1, &B1, &C1);
   LineEquation(x3,y3,x4,y4,&A2, &B2, &C2);

   answer = (A1 * A2) + (B1 * B2);
   if(answer >= 0.9999999)
      {
      answer = 0.0;
      }
   else if(answer < -0.9999999)
      answer = PIvalue;
   else
      answer = acos(answer);

   if(answer > 1.57079631345) /*** if angle > 90 degrees in radians, then subtract it from 180 ***/
      answer = PIvalue - answer;

   return(answer);
}

int Get_LOC_AttrValues(int Lindex, int idn, FILE * fileptr, int * LOCvalue)
{
int ECC, seeitgeom,numchar,numattr,NumAttrToRead,answer;
int uom, uomscale, SEDRIStype, datatype, Idtvalue, IdtVal2;
int IntervalType;
int i,j, Lcode, thiscode;
double Fdtvalue, FdtVal2;
char longtitle[500];
char tc;

   Lcode = GetAttrIndex("LOC");
   answer = 0;

   fseek(fileptr,CrsWlk[Lindex].FullFilePosn,SEEK_SET);

   SEEIT_fread_int(&ECC,fileptr);

   if(feof(fileptr) == 0)
      {
      SEEIT_fread_int(&seeitgeom,fileptr);
      SEEIT_fread_int(&NumAttrToRead,fileptr);
      numattr = NumAttrToRead;
      if(numattr > 0)
         {
         for(i=0; i<numattr; i++)
            {
            SEEIT_fread_int(&thiscode,fileptr);

            if(thiscode < 0)
               {
               SEEIT_fread_int(&numchar,fileptr);

               if(numchar < 499)
                  fread(&longtitle[0],1,numchar,fileptr);
               longtitle[numchar] = '\0';
               }
            else
               {
               SEEIT_fread_int(&uom,fileptr);
               SEEIT_fread_int(&uomscale,fileptr);
               SEEIT_fread_int(&SEDRIStype,fileptr);
               SEEIT_fread_int(&datatype,fileptr);
               switch(datatype)
                  {
                  case 0: /** regular integer **/
                     SEEIT_fread_int(&Idtvalue,fileptr);
                     break;
                  case 1: /** a double **/
                     SEEIT_fread_double(&Fdtvalue,fileptr);
                     break;
                  case 2: /** a string **/
                     SEEIT_fread_int(&numchar,fileptr);
                     for(j=0; j<numchar; j++)
                        {
                        fread(&tc,1,1,fileptr);
                        MessageLine[j] = tc;
                        }
                     MessageLine[j] = '\0';
                     break;
                  case 3: /** enum as int **/
                     SEEIT_fread_int(&Idtvalue,fileptr);
                     if(thiscode == Lcode)
                        {
                        answer = 1;
                        *LOCvalue = Idtvalue;
                        }
                     break;
                  case 4: /** metadata as int **/
                     SEEIT_fread_int(&Idtvalue,fileptr);
                     break;
                  case 5: /** a float interval for SEDRIS only **/
                     SEEIT_fread_double(&Fdtvalue,fileptr);
                     SEEIT_fread_double(&FdtVal2,fileptr);
                     SEEIT_fread_int(&IntervalType,fileptr);
                     break;
                  case 6: /** int interval - for SEDRIS, not NGA ***/
                    SEEIT_fread_int(&Idtvalue,fileptr);
                    SEEIT_fread_int(&IdtVal2,fileptr);
                    SEEIT_fread_int(&IntervalType,fileptr);
                    break;
                  case 7:
                 /* this case is not possible at this time */
                    SEEIT_fread_int(&Idtvalue,fileptr);
                    SEEIT_fread_int(&IdtVal2,fileptr);
                    SEEIT_fread_int(&IntervalType,fileptr);
                    sprintf(MessageLine,"\n interval stuff need updating: (%d to %d)\n",Idtvalue,IdtVal2);
                    infoprint(MessageLine);
                    break;
                  }
               if(answer > 0)
                  break;
               }
            }
         }
      }

   return(answer);
}




void FreeAndWriteTransNetStructs(void)
{
int listlength;
int i,Sindex;
int part1, part2, part3;
int vtxcnt;
int PotentialToCheck, PotentialChecked;
int CountOfNets;
int NetTypeCount;
//long int seekposn;
int Afiledecimal, Lfiledecimal;
long int Afileposn, Lfileposn;
int Afileopen, Lfileopen;
FILE *lfilein, *afilein;
double dbltemp;
char Gfname[900];
char suffix[100];
struct diffnets
   {
   int number;
   int count;
   int numfcodes;
   int samplefcode;
   double mix, max, miy, may;
   struct LIDlist *features;
   struct NetworkNode *net;
   struct diffnets * next;
   } *NetRootUnsorted, *NetRoot, *nc, *np, *nn;
struct ParticipantData
   {
   struct NetworkNode *net;
   struct LIDlist *lid;
   struct ParticipantData *next;
   } *Root1, *Root3, *PDc, *PDn, *PDp;
struct LIDlist *LIDr;
int SzPD = sizeof(struct ParticipantData);
int Szdiff = sizeof(struct diffnets);

   NetRoot = NULL;
   NetRootUnsorted = NULL;
   CountOfNets = 0;
   NetTypeCount = 0;
   Afiledecimal = Lfiledecimal = -1;
   Afileopen = Lfileopen = -1;
   lfilein = afilein = NULL;

   Root1 = Root3 = NULL;

   listlength = (MaxXindex + 1) * (MaxYindex + 1);
   for(i=0; i<listlength; i++)
      {
      if(BucketsOfTransNets[i].next != NULL)
         {
         NNc = BucketsOfTransNets[i].next;
         while(NNc != NULL)
            {
            NNc = NNc->next;
            }
         }
      }


   PotentialToCheck = PotentialChecked = 0;
   for(i=0; i<listlength; i++)
      {
      if(BucketsOfTransNets[i].next != NULL)
         {
         NNc = BucketsOfTransNets[i].next;

         while(NNc != NULL)
            {

            if(NNc->lid != NULL)
               {
               if(NetRootUnsorted == NULL)
                  {
                  nc = (struct diffnets *) (malloc(Szdiff));
                  if(nc == NULL)
                     {
                     printf("memory has been exhausted during final network analysis\n");
                     exit(-1);
                     }
                  nc->number = NNc->netnumb;
                  nc->features = NNc->lid;
                  nc->count = 0;
                  nc->mix = nc->max = nc->miy = nc->may = 0;
                  CountOfNets += 1;
                  NetTypeCount += 1;
                  nc->net = NNc;
                  nc->next = NULL;
                  NetRootUnsorted = nc;
                  }
               else
                  {
                  nc = NetRootUnsorted;
                  while(nc != NULL)
                     {
                     if(nc->number == NNc->netnumb)
                        {
                        if(nc->features == NULL)
                           nc->features = NNc->lid;
                        else
                           {
                           LIDc = nc->features;
                           while(LIDc->next != NULL)
                              LIDc = LIDc->next;
                           LIDc->next = NNc->lid;
                           }
                        break;
                        }
                     nc = nc->next;
                     }


                  if(nc == NULL)
                     {
                     nn = (struct diffnets *) (malloc(Szdiff));
                     if(nn == NULL)
                        {
                        printf("memory has been exhausted during final network analysis\n");
                        exit(-1);
                        }
                     nn->number = NNc->netnumb;
                     nn->features = NNc->lid;
                     nn->count = 0;
                     nn->net = NNc;
                     nn->mix = nn->max = nn->miy = nn->may = 0;
                     CountOfNets += 1;
                     nc = np = NetRootUnsorted;
                     while(nn->features->checkinstance > nc->features->checkinstance)
                        {
                        np = nc;
                        nc = nc->next;
                        if(nc == NULL)
                           break;
                        }
                     if(nc == NetRootUnsorted)
                        {
                        nn->next = NetRootUnsorted;
                        NetRootUnsorted = nn;
                        if(nn->features->checkinstance != nc->features->checkinstance)
                           NetTypeCount += 1;
                        }
                     else
                        {
                        nn->next = nc;
                        np->next = nn;
                        if(nc == NULL)
                           NetTypeCount += 1;
                        else if(nn->features->checkinstance != nc->features->checkinstance)
                           NetTypeCount += 1;
                        }
                     }
                  }
                }
             NNc = NNc->next;
             }
          }
       }

   if(NetRootUnsorted != NULL)
      {
      nc = NetRootUnsorted;
      while(nc != NULL)
         {
         //k = 0;
         LIDc = nc->features;
         Root1 = Root3 = NULL;
         while(LIDc != NULL)
            {
            Sindex = CrsWlk[LIDc->Lindex].LtoSmapping;
            if(CombinedCheckApplies(LIDc->idn,LIDc->checkinstance,NETISOFEAT,LIDc->Lindex,Sindex,
                SCCtable[Sindex].C, SCCtable[Sindex].S, SCCtable[Sindex].D,&part1,&part2,&part3) > 0)
               {
               if(part1 > 0)
                  {
                  if(Root1 == NULL)
                     {
                     PDn = (struct ParticipantData *) (malloc(SzPD));
                     if(PDn == NULL)
                        {
                        printf("memory has been exhausted during NETISOFEAT inspection - must terminate now\n");
                        exit(-1);
                        }
                     PDn->net = nc->net;
                     PDn->lid = LIDc;
                     PDn->next = NULL;
                     Root1 = PDn;
                     }
                  else
                     {
                     PDc = Root1;
                     while(PDc != NULL)
                        {
                        if(PDc->lid->localid == LIDc->localid)
                           break;
                        PDc = PDc->next;
                        }
                     if(PDc == NULL)
                        {
                        PDn = (struct ParticipantData *) (malloc(SzPD));
                        if(PDn == NULL)
                           {
                           printf("memory has been exhausted during NETISOFEAT inspection - must terminate now\n");
                           exit(-1);
                           }
                        PDn->net = nc->net;
                        PDn->lid = LIDc;
                        PDn->next = Root1;
                        Root1 = PDn;
                        }
                     }
                  }
   
               if(part3 > 0)
                  {
                  if(Root3 == NULL)
                     {
                     PDn = (struct ParticipantData *) (malloc(SzPD));
                     if(PDn == NULL)
                        {
                        printf("memory has been exhausted during NETISOFEAT inspection - must terminate now\n");
                        exit(-1);
                        }
                     PDn->net = nc->net;
                     PDn->lid = LIDc;
                     PDn->next = NULL;
                     Root3 = PDn;
                     }
                  else
                     {
                     PDc = Root3;
                     while(PDc != NULL)
                        {
                        if(PDc->lid->localid == LIDc->localid)
                           break;
                        PDc = PDc->next;
                        }
                     if(PDc == NULL)
                        {
                        PDn = (struct ParticipantData *) (malloc(SzPD));
                        if(PDn == NULL)
                           {
                           printf("memory has been exhausted during NETISOFEAT inspection - must terminate now\n");
                           exit(-1);
                           }
                        PDn->net = nc->net;
                        PDn->lid = LIDc;
                        PDn->next = Root3;
                           Root3 = PDn;
                        }
                     }
                  }
               }
            nc->count += 1;
            LIDc = LIDc->next;
            }
         if(Root1 != NULL)
            {
            PDp = Root1;
            while(PDp != NULL)
               {
               PDc = Root3;
               while(PDc != NULL)
                  {
                  if(PDp->lid->idn != PDc->lid->idn)
                     {
                     break;
                     }
                  PDc = PDc->next;
                  }
               LIDr = NULL;
               if(PDc == NULL) /** should we remove condition because some network feature is on the data edge? ****/
                  {
                  if(ActiveChecks[PDp->lid->checkinstance].UseBorderConds == 0)
                     {
                     LIDr = nc->features;
                     while(LIDr != NULL)
                        {
                        if(LIDr->OnDataEdge == 0)
                           break;
                        LIDr = LIDr->next;
                        }
                     }
                  }
               if((PDc == NULL) && (LIDr == NULL)) /** then found a part1 but could not find a part3 with different local id ***/
                  {
                  LIDn = PDp->lid;
                  Sindex = CrsWlk[LIDn->Lindex].LtoSmapping;

                  if(LIDn->geom == G_AREAL)
                     {
                     Afileposn = (long int) LIDn->localid;
                     dbltemp = LIDn->localid - ((double) Afileposn);
                     dbltemp = dbltemp * 10;
                     dbltemp += 0.1;
                     Afiledecimal = (int) dbltemp;
                     if(Afiledecimal != Afileopen)
                        {
                        if(afilein != NULL)
                           {
                           fclose(afilein);
                           afilein = NULL;
                           }
                        strcpy(Gfname,arealsin);
                        if(Afiledecimal > 1)
                           {
                           sprintf(suffix,"%d",Afiledecimal-1);
                           strcat(Gfname,suffix);
                           }
                        afilein = fopen(Gfname,"rb");
                        if(afilein == NULL)
                           {
                           printf("areals in string is: %s\n",arealsin);
                           printf("9 area feature data input file %s could not be opened for read\n",Gfname);
                           exit(-1);
                           }
                        Afileopen = Afiledecimal;
                        }

                     fseek(afilein,Afileposn,SEEK_SET);
                     vtxcnt = ReadArealFromFile(afilein, SignificantDecimals);

                     if((vtxcnt > 0) && (ArealFromFile.ishole <= 0))
                        FwriteObject(NETISOFEAT,Make_Correct(NETISOFEAT,PDp->lid->checkinstance),
                                     PDp->lid->Lindex,Sindex,G_AREAL,PDp->lid->localid,PDp->lid->idn,
                                    -1.0, -1.0, ArealFromFile.x, ArealFromFile.y, ArealFromFile.z, ArealFromFile.numverts,1);
                     }
                  else if(LIDn->geom == G_LINE)
                     {
                     Lfileposn = (long int) LIDn->localid;
                     dbltemp = LIDn->localid - ((double) Lfileposn);
                     dbltemp = dbltemp * 10;
                     dbltemp += 0.1;
                     Lfiledecimal = (int) dbltemp;
                     if(Lfiledecimal != Lfileopen)
                        {
                        if(lfilein != NULL)
                           {
                           fclose(lfilein);
                           lfilein = NULL;
                           }
                        strcpy(Gfname,linearsin);
                        if(Lfiledecimal > 1)
                           {
                           sprintf(suffix,"%d",Lfiledecimal-1);
                           strcat(Gfname,suffix);
                           }
                        lfilein = fopen(Gfname,"rb");
                        if(lfilein == NULL)
                           {
                           printf("linears in string is: %s\n",linearsin);
                           printf("9 line feature data input file %s could not be opened for read\n",Gfname);
                           exit(-1);
                           }
                        Lfileopen = Lfiledecimal;
                        }

                     fseek(lfilein,Lfileposn,SEEK_SET);
                     vtxcnt = ReadLinearFromFile(lfilein, SignificantDecimals);

                     if(vtxcnt > 0)
                        FwriteObject(NETISOFEAT,Make_Correct(NETISOFEAT,PDp->lid->checkinstance),
                                     PDp->lid->Lindex,Sindex,G_LINE,PDp->lid->localid,PDp->lid->idn,
                                    -1.0, -1.0, LinearFromFile.x, LinearFromFile.y, LinearFromFile.z, LinearFromFile.numnodes,1);
                     }
                  }
               PDp = PDp->next;
               }
 
            }
         PDc = Root1;
         while(PDc != NULL)
            {
            PDp = PDc;
            PDc = PDc->next;
            free(PDp);
            }
         Root1 = NULL;
         PDc = Root3;
         while(PDc != NULL)
            {
            PDp = PDc;
            PDc = PDc->next;
            free(PDp);
            }
         Root3 = NULL;
         nc = nc->next;
         }

      } /** end if abortnow == 0) ***/

   if(afilein != NULL)
      fclose(afilein);
   afilein = NULL;
   if(lfilein != NULL)
      fclose(lfilein);
   lfilein = NULL;

   PDc = Root1;
   while(PDc != NULL)
      {
      PDp = PDc;
      PDc = PDc->next;
      free(PDp);
      }
   PDc = Root3;
   while(PDc != NULL)
      {
      PDp = PDc;
      PDc = PDc->next;
      free(PDp);
      }

   nc = NetRoot;
   while(nc != NULL)
      {
      np = nc;

      LIDc = nc->features;
      while(LIDc != NULL)
         {
         LIDp = LIDc;
         LIDc = LIDc->next;
         free(LIDp);
         }

      nc = nc->next;
      free(np);
      }

   nc = NetRootUnsorted;
   while(nc != NULL)
      {
      np = nc;

      LIDc = nc->features;
      while(LIDc != NULL)
         {
         LIDp = LIDc;
         LIDc = LIDc->next;
         free(LIDp);
         }

      nc = nc->next;
      free(np);
      }

   for(i=0; i<listlength; i++)
      {
      if(BucketsOfTransNets[i].next != NULL)
         {
         NNc = BucketsOfTransNets[i].next;
         while(NNc != NULL)
            {
            NNn = NNc;
            NNc = NNc->next;
            CLc = NNn->CL;
            while(CLc != NULL)
               {
               CLn = CLc;
               CLc = CLc->next;
               free(CLn);
               }
            free(NNn);
            }
         }
      BucketsOfTransNets[i].netnumb = -1;
      BucketsOfTransNets[i].CL = NULL;
      BucketsOfTransNets[i].next = NULL;
      }

   NewNetNumber = 0;
}




void CheckForBndryRemoval(int cindex, struct RawLinear *Lroot, struct RawAreal *Aroot, struct RawPoint *Proot,
                              int  NorthCount, int SouthCount, int EastCount, int WestCount)
{
int keepit;

   if(BucketsOfTransNets[cindex].next != NULL)
      {
      NNc = BucketsOfTransNets[cindex].next;
      while(NNc != NULL)
         {
         CLc = NNc->CL;
         while(CLc != NULL)
            {
            keepit = ConditionOnDataEdge(CLc->x,CLc->y,CLc->x,CLc->y,Aroot,Proot,Lroot,
                                             NULL, NULL, NULL,    // is area, point, line
                                             NorthCount,SouthCount,EastCount,WestCount,CLc->checkinstance);
             if(keepit <= 0)
                CLc->lid->OnDataEdge = 0;

            CLc = CLc->next;
            }
         NNc = NNc->next;
         }
      }
   return;
}



void AddEdgesToTransNetwork(struct RawLinear *Lroot, struct RawAreal *Aroot, struct RawPoint *Proot,
                        int compositeindex, int checkinstance,
                        int  NorthCount, int SouthCount, int EastCount, int WestCount,
                        double MIX, double MIY, double MAX, double MAY, int PseudoAdded)
{
int cindex;
int i,j,jj,maxindex, checkapplies,i_next;
int part1, part2, part3;
int index1, index2;
int nettouse,listlength,LastAreaVertex, LastLineNode;
struct RawLinear *rlc;
struct RawAreal *rac;
int *foundnets;
int *BucketIndices;
int SzNN = sizeof(struct NetworkNode);
int SzCL = sizeof(struct CoordList);
int SzLID = sizeof(struct LIDlist);
double mix, miy, max, may;
double NodeToNodeDist;

   listlength = (MaxXindex + 1) * (MaxYindex + 1);
   maxindex = 100;
   BucketIndices = (int *) (malloc(SzI * maxindex));
   foundnets = (int *) (malloc(SzI * maxindex));
   if(foundnets == NULL) 
      {
      printf("all memory has been allocated - exiting now!\n");
      exit(-1);
      }


   rlc = Lroot;
   while(rlc != NULL)
      {
      checkapplies = CombinedCheckApplies(rlc->idn,checkinstance,NETISOFEAT,rlc->Lindex,rlc->Sindex,
                                          SCCtable[rlc->Sindex].C, SCCtable[rlc->Sindex].S, SCCtable[rlc->Sindex].D,
                                          &part1, &part2, &part3);
      if(checkapplies > 0)
         {
         FeatsAddedToNet += 1;
         if((rlc->firstencounter != 1) && (PseudoAdded == 0))
            checkapplies = -1;
         }

      if(checkapplies > 0)
         {
LastLineNode = rlc->numnodes;
         LIDc = (struct LIDlist *) (malloc(SzLID));
         if(LIDc == NULL)
            {
            printf("all memory has been consumed while adding feature identifcations to assembled networks\n");
            printf("terminating now\n");
            exit(-1);
            }
         LIDc->OnDataEdge = 1;
         LIDc->localid = rlc->localID;
         LIDc->Lindex = rlc->Lindex;
         LIDc->idn = rlc->idn;
         LIDc->checkinstance = checkinstance;
         LIDc->geom = (char) (G_LINE);

         if(rlc->numnodes > maxindex)
            {
            BucketIndices = (int *) (realloc(BucketIndices,SzI * rlc->numnodes));
            foundnets = (int *) (realloc(foundnets,SzI * rlc->numnodes));
            maxindex = rlc->numnodes;
            if(foundnets == NULL)
               {
               printf("all memory has been allocated - exiting now!\n");
               exit(-1);
               }
            }
         for(i=0; i<rlc->numnodes; i++)
            {
            foundnets[i] = -1;
            }
         nettouse = -1;

         for(i=0; i < LastLineNode; i++)
            {
            j = i + 1;

            foundnets[i] = -1;
            index1 = (int) (rlc->x[i] / RegionSize);
            index2 = (int) (rlc->y[i] / RegionSize);

            cindex = (index1 * (MaxYindex + 1)) + index2;
            BucketIndices[i] = cindex;

            if(BucketsOfTransNets[cindex].next == NULL) /*** start off the net stucture in this bucket **/
               {
               NewNetNumber += 1;
               nettouse = NewNetNumber;
               foundnets[i] = nettouse;

               NNc = (struct NetworkNode *) (malloc(SzNN));
               if(NNc == NULL)
                  {
                  printf("all memory has been allocated - exiting now!\n");
                  exit(-1);
                  }
               NNc->next = NULL;
               NNc->lid = NULL;
               NNc->netnumb = nettouse;
               NNc->CL = NULL;
               BucketsOfTransNets[cindex].next = NNc;

               CLc = (struct CoordList *) (malloc(SzCL));
               if(CLc == NULL)
                  {
                  printf("all memory has been allocated - exiting now!\n");
                  exit(-1);
                  }
               CLc->x = rlc->x[i];
               CLc->y = rlc->y[i];
               CLc->z = rlc->z[i];
               CLc->lid = LIDc;
               CLc->checkinstance = checkinstance;
/******************
               CLc->FD = FDc;
               CLc->nearby = NULL; **/
               CLc->next = NNc->CL;
               NNc->CL = CLc;
               }
            else
               {
               NNc = BucketsOfTransNets[cindex].next;
               while(NNc != NULL)
                  {
                  CLc = NNc->CL;
                  while(CLc != NULL)
                     {
                     if(CLc->checkinstance == checkinstance)
                        NodeToNodeDist = Distance(CLc->x,CLc->y,rlc->x[i],rlc->y[i]);
                     else
                        NodeToNodeDist = ActiveChecks[checkinstance].sensitivity + 100;

                     if(NodeToNodeDist <= ActiveChecks[checkinstance].sensitivity)
                        {
                        if(foundnets[i] >= 0)  /** already put this node into a network somewhere - join nets ***/
                           {
                           for(jj=0; jj<listlength; jj++)
                              {
                              NNt = BucketsOfTransNets[jj].next;
                              while(NNt != NULL)
                                 {
                                 if(NNt->netnumb == foundnets[i])
                                    {
                                    NNt->netnumb = NNc->netnumb;
                                    }
                                 NNt = NNt->next;
                                 }
                              }
                           }
                        foundnets[i] = NNc->netnumb;
                        if(nettouse < 0)
                           nettouse = NNc->netnumb;

      
                        break;
                        }
                     CLc = CLc->next;
                     }
                  NNc = NNc->next;
                  }
               }
            }
            if(nettouse < 0) /*** add nodes with a new net number of NewNetNumber + 1 ***/
               {
               NewNetNumber += 1;
               nettouse = NewNetNumber;
               }
         for(i=0; i<LastLineNode; i++)
            {
            j = i + 1;

            if(foundnets[i] < 0) /** means there is no connection to this node ***/
               {
               foundnets[i] = nettouse;
               cindex = BucketIndices[i];

            BucketIndices[i] = cindex;
               NNc = BucketsOfTransNets[cindex].next;
               while(NNc != NULL)
                  {
                  if(NNc->netnumb == nettouse)
                     break;
                  NNc = NNc->next;
                  }
               if(NNc == NULL) /*** have to add a new network ***/
                  {
                  NNc = (struct NetworkNode *) (malloc(SzNN));
                  if(NNc == NULL)
                     {
                     printf("all memory has been allocated - exiting now!\n");
                     exit(-1);
                     }
                  NNc->next = BucketsOfTransNets[cindex].next;
                  NNc->netnumb = nettouse;
                  NNc->lid = NULL;
                  NNc->CL = NULL;
                  BucketsOfTransNets[cindex].next = NNc;
                  }

               CLc = (struct CoordList *) (malloc(SzCL));
               if(CLc == NULL)
                  {
                  printf("all memory has been allocated - exiting now!\n");
                  exit(-1);
                  }
               CLc->x = rlc->x[i];
               CLc->y = rlc->y[i];
               CLc->z = rlc->z[i];
               CLc->lid = LIDc;
               CLc->checkinstance = checkinstance;
               CLc->next = NNc->CL;
               NNc->CL = CLc;
               }
            }

         mix = max = rlc->x[0];
         miy = may = rlc->y[0];
         for(i=0; i<LastLineNode; i++)
            {
            if(rlc->x[i] > max)  max = rlc->x[i];
            if(rlc->x[i] < mix)  mix = rlc->x[i];
            if(rlc->y[i] > may)  may = rlc->y[i];
            if(rlc->y[i] < miy)  miy = rlc->y[i];

            if(foundnets[i] != nettouse) /** means this node exists in another network - need to change it ***/
               {
               for(j=0; j<listlength; j++)
                  {
                  NNc = BucketsOfTransNets[j].next;
                  while(NNc != NULL) 
                     {
                     if(NNc->netnumb == foundnets[i])
                        {
                        NNc->netnumb = nettouse;
                        }
                     NNc = NNc->next;
                     }
                  }
               }
            }
         NNc = BucketsOfTransNets[BucketIndices[0]].next;
         while(NNc != NULL)
            {
            if(NNc->netnumb == nettouse)
               {
               LIDc->next = NNc->lid;
               NNc->lid = LIDc;
               break;
               }
            NNc = NNc->next;
            }
         if(NNc == NULL)
            {
            printf("fatal error in assigning feaure id to network - terminating now\n");
            exit(-1);
            }
         }
      rlc = rlc->next;
      }

   rac = Aroot;
   while(rac != NULL)
      {
      
      checkapplies = CombinedCheckApplies(rac->idn,checkinstance,NETISOFEAT,rac->Lindex,rac->Sindex,
                                       SCCtable[rac->Sindex].C, SCCtable[rac->Sindex].S, SCCtable[rac->Sindex].D,
                                       &part1, &part2, &part3);
      if(checkapplies > 0)
         {
         FeatsAddedToNet += 1;
         if((rac->firstindex != compositeindex) && (rac->firstindex >= -90))
            checkapplies = -1;
         }

      if(checkapplies > 0)
         {
         if(ActiveChecks[checkinstance].sensitivity2 > 0)
            LastAreaVertex = rac->numverts;
         else
            LastAreaVertex = rac->coversLM;  /** stored the original numnodes here, before added
                                                  pseudo nodes from intersections ***/
         if(rac->numverts > maxindex)
            {
            BucketIndices = (int *) (realloc(BucketIndices,SzI * rac->numverts));
            foundnets = (int *) (realloc(foundnets,SzI * rac->numverts));
            maxindex = rac->numverts;
            if(foundnets == NULL)
               {
               printf("all memory has been allocated - exiting now!\n");
               exit(-1);
               }
            }
         LIDc = (struct LIDlist *) (malloc(SzLID));
         if(LIDc == NULL)
            {
            printf("all memory has been consumed while adding feature identifcations to assembled networks\n");
            printf("terminating now\n");
            exit(-1);
            }
         LIDc->OnDataEdge = 1;

         for(i=0; i<rac->numverts; i++)
            {
            foundnets[i] = -1;
            }


         nettouse = -1;
         for(i=0; i < LastAreaVertex; i++)
            {
            i_next = i + 1;
            if(i_next >= LastAreaVertex)
               i_next = 0;
   
            foundnets[i] = -1;
            index1 = (int) (rac->x[i] / RegionSize);
            index2 = (int) (rac->y[i] / RegionSize);

            cindex = (index1 * (MaxYindex + 1)) + index2;
            BucketIndices[i] = cindex;

            if(BucketsOfTransNets[cindex].next == NULL) /*** then need to start off the net stucture in this bucket **/
               {
               NewNetNumber += 1;
               nettouse = NewNetNumber;
               foundnets[i] = nettouse;

               NNc = (struct NetworkNode *) (malloc(SzNN));
               if(NNc == NULL)
                  {
                  printf("all memory has been allocated - exiting now!\n");
                  exit(-1);
                  }
               NNc->next = NULL;
               NNc->netnumb = nettouse;
               NNc->lid = NULL;
               NNc->CL = NULL;
               BucketsOfTransNets[cindex].next = NNc;

               CLc = (struct CoordList *) (malloc(SzCL));
               if(CLc == NULL)
                  {
                  printf("all memory has been allocated - exiting now!\n");
                  exit(-1);
                  }
               CLc->x = rac->x[i];
               CLc->y = rac->y[i];
               CLc->z = rac->z[i];
               /**CLc->neighbor = NULL; **/
               /**CLc->potential = NULL;**/
               /**CLc->sibling = NULL;**/
               CLc->lid = LIDc;

               CLc->checkinstance = checkinstance;
               CLc->next = NNc->CL;
               NNc->CL = CLc;
               }
            else
               {
               NNc = BucketsOfTransNets[cindex].next;
               while(NNc != NULL)
                  {
                  CLc = NNc->CL;
                  while(CLc != NULL)
                     {
                     if(CLc->checkinstance == checkinstance)
                        NodeToNodeDist = Distance(CLc->x,CLc->y,rac->x[i],rac->y[i]);
                     else
                        NodeToNodeDist = ActiveChecks[checkinstance].sensitivity * 100 + 100;
                     if(NodeToNodeDist <= ActiveChecks[checkinstance].sensitivity)
                        {
                        if(foundnets[i] >= 0)
                           {
                           for(jj=0; jj<listlength; jj++)
                              {
                              NNt = BucketsOfTransNets[jj].next;
                              while(NNt != NULL)
                                 {
                                 if(NNt->netnumb == foundnets[i])
                                    {

                                    NNt->netnumb = NNc->netnumb;
                                    }
                                 NNt = NNt->next;
                                 }
                              }
                           }
                        foundnets[i] = NNc->netnumb;
                        if(nettouse < 0)
                           nettouse = NNc->netnumb;
                        break;
                        }
                     CLc = CLc->next;
                     }
                  NNc = NNc->next;
                  }
               }
            }
         if(nettouse < 0) /*** add nodes with a new net number of NewNetNumber + 1 ***/
            {
            NewNetNumber += 1;
            nettouse = NewNetNumber;
            }

         //for(i=0; i<rac->numverts; i++)
         for(i=0; i<LastAreaVertex; i++)
            {
            i_next = i + 1;
            if(i_next >= LastAreaVertex)
               i_next = 0;


            if(foundnets[i] < 0) /** means there is no connection to this node ***/
               {
               foundnets[i] = nettouse;
               cindex = BucketIndices[i];

               NNc = BucketsOfTransNets[cindex].next;
               while(NNc != NULL)
                  {
                  if(NNc->netnumb == nettouse)
                     break;
                  NNc = NNc->next;
                  }
               if(NNc == NULL) /*** have to add a new network ***/
                  {
                  NNc = (struct NetworkNode *) (malloc(SzNN));
                  if(NNc == NULL)
                     {
                     printf("all memory has been allocated - exiting now!\n");
                     exit(-1);
                     }
                  NNc->next = BucketsOfTransNets[cindex].next;
                  NNc->lid = NULL;
                  NNc->netnumb = nettouse;
                  NNc->CL = NULL;
                  BucketsOfTransNets[cindex].next = NNc;
                  }

               CLc = (struct CoordList *) (malloc(SzCL));
               if(CLc == NULL)
                  {
                  printf("all memory has been allocated - exiting now!\n");
                  exit(-1);
                  }
               CLc->x = rac->x[i];
               CLc->y = rac->y[i];
               CLc->z = rac->z[i];
               CLc->lid = LIDc;
               CLc->checkinstance = checkinstance;
               /**CLc->potential = NULL;**/
               /**CLc->neighbor = NULL; **/
               /**CLc->sibling = NULL;**/
               CLc->next = NNc->CL;
               NNc->CL = CLc;
               }
            }

         mix = max = rac->x[0];
         miy = may = rac->y[0];
         for(i=0; i<LastAreaVertex; i++)
            {
            if(rac->x[i] > max)  max = rac->x[i];
            if(rac->x[i] < mix)  mix = rac->x[i];
            if(rac->y[i] > may)  may = rac->y[i];
            if(rac->y[i] < miy)  miy = rac->y[i];

            if(foundnets[i] != nettouse)
               {
               for(j=0; j<listlength; j++)
                  {
                  NNc = BucketsOfTransNets[j].next;
                  while(NNc != NULL)
                     {
                     if(NNc->netnumb == foundnets[i])
                        {
                        NNc->netnumb = nettouse;
                        }
                     NNc = NNc->next;
                     }
                  }
               }
            }
         NNc = BucketsOfTransNets[BucketIndices[0]].next;
         while(NNc != NULL)
            {
            if(NNc->netnumb == nettouse)
               {
               LIDc->localid = rac->localID;
               LIDc->Lindex = rac->Lindex;
               LIDc->idn = rac->idn;
               LIDc->checkinstance = checkinstance;
               LIDc->geom = (char) (G_AREAL);
               LIDc->next = NNc->lid;
               NNc->lid = LIDc;
               break;
               }
            NNc = NNc->next;
            }
         if(NNc == NULL)
            {
            printf("fatal error in assigning feature id to network - terminating now\n");
            exit(-1);
            }

         }
      rac = rac->next;
      }
   i = compositeindex;

   free(BucketIndices);
   free(foundnets);

}




void InitializeBucketsOfTransNets(void)
{
int i, listlength;

   listlength = (MaxXindex + 1) * (MaxYindex + 1);
   BucketsOfTransNets = (struct NetworkNode *) (malloc(sizeof(struct NetworkNode) * listlength));
   if(BucketsOfTransNets == NULL)
      {
      printf("allocation memory exhausted during network indexing initialization\n");
      exit(-1);
      }
   for(i=0; i<listlength; i++)
      {
      BucketsOfTransNets[i].netnumb = 0;
      BucketsOfTransNets[i].CL = NULL;
      BucketsOfTransNets[i].next = NULL;
      }
   return;
}






void PropagateTransitiveCounters(rb_red_blk_tree* tree, rb_red_blk_node* x)
{
struct TransitiveCollection *TC,*TC2;
double RB_newKey3;
struct DirectConnect *DCc, *DCp;
struct TransValueUpdate
   {
   struct DirectConnect *dc;
   struct TransValueUpdate * next;
   }  *UpdateRoot, *UDR2, *UDc, *UDp, *UDn;
int foundobject;
int SzTVU = sizeof(struct TransValueUpdate);

   if (x != tree->nil)
      {
      PropagateTransitiveCounters(tree,x->left);

      TC = (struct TransitiveCollection *) (x->info);

      if(((TC->keyval == NETISOA) || (TC->keyval == ANETISOA)) && (TC->counter > 0) && (TC->magnitude < 1.0))
         {
         foundobject = TC->counter;
         TC->magnitude = 100.0; /** mark this node as visited ***/
         UDR2 = NULL;
         DCc = TC->connect;
         while(DCc != NULL)
            {
            UDc = (struct TransValueUpdate *) (malloc(SzTVU));
            UDc->dc = DCc;
            UDc->next = UDR2;
            UDR2 = UDc;
            DCc = DCc->next;
            }
         while(UDR2 != NULL)
            {
            UpdateRoot = UDR2;
            UDR2 = NULL; /** this is for pointer from pointers to add to update list ***/
            UDc = UpdateRoot;
            while(UDc != NULL)
               {
               UDp = UDc;
               UDc = UDc->next;
               DCp = UDp->dc;
               RB_newKey3 = DCp->TreeKey;
               if(( RB_newNode = RBExactQuery(Trans_RB_Tree,&RB_newKey3)))
                  {
                  TC2 = (struct TransitiveCollection *) RB_newNode->info;
                  if(TC2->magnitude < 1.0)
                     {
                     TC2->magnitude = 100.0; /** mark this node as visited ***/
                     if(TC2->counter == 0)
                        TC2->counter += foundobject;
                     DCc = TC2->connect;
                     while(DCc != NULL)
                        {
                        UDn = (struct TransValueUpdate *) (malloc(SzTVU));
                        UDn->dc = DCc;
                        UDn->next = UDR2;
                        UDR2 = UDn;

                        DCc = DCc->next;
                        }
                     }
                  }
               free(UDp);
               }
            }


         }
      PropagateTransitiveCounters(tree,x->right);
      }

}






void PerformTransitiveChecks(char * prompt, int cindex,struct RawAreal *root,double minx,double miny,double maxx,double maxy, double multiplier,
    int  NorthCount, int SouthCount, int EastCount, int WestCount,
    struct RawLinear *Lroot, struct RawBridge *Broot, struct RawPoint *Proot, struct ListOfEdges *PEroot, struct RawAreal * NeighborARoot, int IndexLimit, int SzPC, int SzPW)
{

struct RawAreal *rac, *ran;
struct RawLinear *rlc;
struct TransitiveCollection *RB_cond;

unsigned char clipflag;
int CheckEligible;
int checkit, unusedint;
int i,kk,ii,ii3,jj,savei,savej;
int part1, part2, part3;
int Config1, Strat1, Domain1;
int PassCond;
int checkinstance;
int foundobject, found_two;
int LastI;
int abortNow = 0;
int maxXindex, minXindex, maxYindex, minYindex;
double x1, y1, z1, x2, y2, z2, unuseddbl;
double L1minx, L1miny, L1maxx, L1maxy;
double xint, yint;
double checktolerance, tolerance2;
struct RB_Edgelist * LastEdge;
struct DirectConnect *DCc, *DCp, *DCroot;
struct collection
   {
   int keyval;
   int Cnumber;
   double px_int, py_int, pz_int;
   double x[4];
   double y[4];
   double z[4];
   double magnitude;
   int IDN1;
   int IDN2;
   unsigned int Sindex1,Sindex2;
   int vertexindex1, vertexindex2;
   int numverts1, numverts2;
   /***double *x1, *y1, *z1;
   double *x2, *y2, *z2; ***/
   struct RawAreal *aa, *aa2;
   struct RawBridge *bb;
   struct RawPoint *pp;
   struct ListOfEdges *le;
   struct RawLinear *ll;
   struct ThePolys *poly;
   struct collection * next;
   } *ERroot, *ERc, *ERn;
int SzTC = sizeof(struct TransitiveCollection);

int SzC = sizeof(struct collection);
int SzDC = sizeof(struct DirectConnect);





   rac = root;
   ERroot = NULL;

   while(rac != NULL)
      {
      Config1 = SCCtable[rac->Sindex].C;
      Strat1 = SCCtable[rac->Sindex].S;
      Domain1 = SCCtable[rac->Sindex].D;

      checkinstance = -1;

      for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++)
         {

          if( (CombinedCheckApplies(rac->idn,checkinstance,NETISOA,rac->Lindex,rac->Sindex,
                Config1,Strat1,Domain1, &part1,&part2,&part3) > 0) && (part1 > 0))
            {
            CheckEligible = 1;
             if((NorthCount <= 0) || (SouthCount <= 0) || (EastCount <= 0) || (WestCount <= 0))
                {
                maxXindex = minXindex = maxYindex = minYindex = -1;
                for(i=0; i<rac->numverts; i++)
                   {
                   if((rac->x[i] >= minx) && (rac->x[i] <= maxx) && (rac->y[i] >= miny) && (rac->y[i] <= maxy))
                      {
                      if((maxXindex < 0) || (rac->x[i] > rac->x[maxXindex]))
                         maxXindex = i;
                      if((minXindex < 0) || (rac->x[i] < rac->x[minXindex]))
                         minXindex = i;
                      if((maxYindex < 0) || (rac->y[i] > rac->y[maxYindex]))
                         maxYindex = i;
                      if((minYindex < 0) || (rac->y[i] < rac->y[minYindex]))
                         minYindex = i;
                      }
                   }
                CheckEligible = 1;

                if((maxXindex >= 0) && (EastCount <= 0))
                   {
                   if(maxXindex == 0)
                      i = 1;
                   else
                      i = maxXindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[maxXindex],rac->y[maxXindex],root,Proot,Lroot,
                                              rac, NULL, NULL,100,100,EastCount,100,checkinstance);
                   }
                if((minXindex >= 0) && (CheckEligible > 0) && (WestCount <= 0))
                   {
                   if(minXindex == 0)
                      i = 1;
                   else
                      i = minXindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[minXindex],rac->y[minXindex],root,Proot,Lroot,
                                              rac, NULL, NULL,100,100,100,WestCount,checkinstance);
                   }
                if((minYindex >= 0) && (CheckEligible > 0) && (SouthCount <= 0))
                   {
                   if(minYindex == 0)
                      i = 1;
                   else
                      i = minYindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[minYindex],rac->y[minYindex],root,Proot,Lroot,
                                              rac, NULL, NULL,100,SouthCount,100,100,checkinstance);
                   }
                if((maxYindex >= 0) && (CheckEligible > 0) && (NorthCount <= 0))
                   {
                   if(maxYindex == 0)
                      i = 1;
                   else
                      i = maxYindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[maxYindex],rac->y[maxYindex],root,Proot,Lroot,
                                              rac, NULL, NULL, NorthCount,100,100,100,checkinstance);
                   }
                }

            checktolerance = FindRelevantSensitivity(NETISOA,checkinstance,&tolerance2);
            if(CheckEligible > 0)
               foundobject = 0;
            else
               foundobject = 1;
            DCroot = NULL;
            if((rac->RB_Tree_Edgelist == NULL) && (rac->ishole <= 0)) /** don't care about line ints with cut-outs **/
               {
               for(ii=0; ii<rac->numverts; ii++)
                  {
                  if(ii == 0)
                     jj = rac->numverts - 1;
                  else
                     jj = ii - 1;
                  checkit = 0;
                  if(rac->ismulti == 0)
                     checkit = 1;
                  else
                     {
                     x1 = rac->x[ii];
                     y1 = rac->y[ii];
                     z1 = rac->z[ii];
                     x2 = rac->x[jj];
                     y2 = rac->y[jj];
                     z2 = rac->z[jj];
                     if(ClipLineToRegion(minx,miny,maxx,maxy,&x1,&y1,&z1,&x2,&y2,&z2,&clipflag,multiplier) > 0)
                        checkit = 1;
                     }
                  if(checkit > 0)
                     {
                     if(rac->x[jj] < rac->x[ii])
                        {
                        L1minx = rac->x[jj];
                        L1maxx = rac->x[ii];
                        }
                     else
                        {
                        L1maxx = rac->x[jj];
                        L1minx = rac->x[ii];
                        }
                     if(rac->y[jj] < rac->y[ii])
                        {
                        L1miny = rac->y[jj];
                        L1maxy = rac->y[ii];
                        }
                     else
                        {
                        L1maxy = rac->y[jj];
                        L1miny = rac->y[ii];
                        }

                     rlc = Lroot;
                     while(rlc != NULL)
                        {
                        if((CombinedCheckApplies(rlc->idn,checkinstance,NETISOA,
                               rlc->Lindex,rlc->Sindex,SCCtable[rlc->Sindex].C,SCCtable[rlc->Sindex].S,SCCtable[rlc->Sindex].D,
                                &part1,&part2,&part3) > 0) && (part2 > 0))
                           {
                           for(kk=1; kk<rlc->numnodes; kk++)
                              {
                              if(LineSegmentsIntersect(rlc->x[kk-1],rlc->y[kk-1],rlc->x[kk],rlc->y[kk],
                               rac->x[ii],rac->y[ii],rac->x[jj],rac->y[jj], &xint, &yint) > 0)
                                 {
                                 foundobject = 1;
                                 break;
                                 }
                              else if(MergeSeg_LineSegmentsOverlap(rlc->x[kk-1],rlc->y[kk-1],rlc->x[kk],rlc->y[kk],
                                    rac->x[ii],rac->y[ii],rac->x[jj],rac->y[jj], L1minx,L1maxx,L1miny,L1maxy,
                                    &x1, &y1, &x2, &y2, 0.0001) > 0)
                                 {
                                 foundobject = 1;
                                 break;
                                 }
                              else
                                 {
                                 unuseddbl = PointToLineDist(rlc->x[kk-1],rlc->y[kk-1],rlc->z[kk-1],
                                              rac->x[ii],rac->y[ii],rac->z[ii],rac->x[jj],rac->y[jj],rac->z[jj]);
                                ii3 = 99;
                                if(unuseddbl < ActiveChecks[checkinstance].sensitivity)
                                   {
                                   ii3 = PointIsInside(rlc->x[kk],rlc->y[kk],
                                             rac->x,rac->y,rac->x,rac->numverts,C_AREA,&unuseddbl);
                                   if(ii3 == 0)
                                      {
                                      foundobject = 1;
                                      break;
                                      }
                                   }
                                else
                                   {
                                   unuseddbl = PointToLineDist(rlc->x[kk],rlc->y[kk],rlc->z[kk],
                                           rac->x[ii],rac->y[ii],rac->z[ii],rac->x[jj],rac->y[jj],rac->z[jj]);
                                   if(unuseddbl < ActiveChecks[checkinstance].sensitivity)
                                      {
                                      if((ii3 == 0) ||
                                          (PointIsInside(rlc->x[kk-1],rlc->y[kk-1],
                                             rac->x,rac->y,rac->x,rac->numverts,C_AREA,&unuseddbl) == 0))
                                         {
                                         foundobject = 1;
                                         break;
                                         }
                                      }
                                   }
                                 }
                              }
                           }
                        if(foundobject > 0)
                           break;
                        rlc = rlc->next;
                        }
                     }
                  if(foundobject > 0)
                     break;
                  }
               }
            else if(rac->ishole <= 0) /**** a rb-tree, large areal, but still don't care about line ints w/ cut-out ***/
               {
               LastEdge = NULL;
               LastI = -1;
               ii = GetNextArealIndex(rac, LastI, 1, &LastEdge, cindex, &unusedint);
               LastI = ii;
               while(ii >= 0)
                  {
                  if(ii == 0)
                     jj = rac->numverts - 1;
                  else
                     jj = ii - 1;
                  checkit = 0;
                  if(rac->ismulti == 0)
                     checkit = 1;
                  else
                     {
                     x1 = rac->x[ii];
                     y1 = rac->y[ii];
                     z1 = rac->z[ii];
                     x2 = rac->x[jj];
                     y2 = rac->y[jj];
                     z2 = rac->z[jj];
                     if(ClipLineToRegion(minx,miny,maxx,maxy,&x1,&y1,&z1,&x2,&y2,&z2,&clipflag,multiplier) > 0)
                        checkit = 1;
                     }
                  if(checkit > 0)
                    {
                     if(rac->x[jj] < rac->x[ii])
                        {
                        L1minx = rac->x[jj];
                        L1maxx = rac->x[ii];
                        }
                     else
                        {
                        L1maxx = rac->x[jj];
                        L1minx = rac->x[ii];
                        }
                     if(rac->y[jj] < rac->y[ii])
                        {
                        L1miny = rac->y[jj];
                        L1maxy = rac->y[ii];
                        }
                     else
                        {
                        L1maxy = rac->y[jj];
                        L1miny = rac->y[ii];
                        }

                     rlc = Lroot;
                     while(rlc != NULL)
                        {
                        if((CombinedCheckApplies(rlc->idn,checkinstance,NETISOA,
                               rlc->Lindex,rlc->Sindex,SCCtable[rlc->Sindex].C,SCCtable[rlc->Sindex].S,SCCtable[rlc->Sindex].D,
                                &part1,&part2,&part3) > 0) && (part2 > 0))
                           {
                           for(kk=1; kk<rlc->numnodes; kk++)
                              {
                              if(LineSegmentsIntersect(rlc->x[kk-1],rlc->y[kk-1],rlc->x[kk],rlc->y[kk],
                               rac->x[ii],rac->y[ii],rac->x[jj],rac->y[jj], &xint, &yint) > 0)
                                 {
                                 foundobject = 1;
                                 break;
                                 }
                              else if(MergeSeg_LineSegmentsOverlap(rlc->x[kk-1],rlc->y[kk-1],rlc->x[kk],rlc->y[kk],
                                    rac->x[ii],rac->y[ii],rac->x[jj],rac->y[jj], L1minx,L1maxx,L1miny,L1maxy,
                                    &x1, &y1, &x2, &y2, 0.0001) > 0)
                                 {
                                 foundobject = 1;
                                 break;
                                 }
                              else
                                 {
                                 unuseddbl = PointToLineDist(rlc->x[kk-1],rlc->y[kk-1],rlc->z[kk-1],
                                              rac->x[ii],rac->y[ii],rac->z[ii],rac->x[jj],rac->y[jj],rac->z[jj]);
                                ii3 = 99;
                                if(unuseddbl < ActiveChecks[checkinstance].sensitivity)
                                   {
                                   ii3 = PointInsideLargeAreal(rlc->x[kk],rlc->y[kk],
                                             rac,&unusedint);
                                   if(ii3 == 0)
                                      {
                                      foundobject = 1;
                                      break;
                                      }
                                   }
                                else
                                   {
                                   unuseddbl = PointToLineDist(rlc->x[kk],rlc->y[kk],rlc->z[kk],
                                           rac->x[ii],rac->y[ii],rac->z[ii],rac->x[jj],rac->y[jj],rac->z[jj]);
                                   if(unuseddbl < ActiveChecks[checkinstance].sensitivity)
                                      {
                                      if((ii3 == 0) ||
                                          (PointInsideLargeAreal(rlc->x[kk-1],rlc->y[kk-1],
                                             rac,&unusedint) == 0))
                                         {
                                         foundobject = 1;
                                         break;
                                         }
                                      }
                                   }
                                 }
                              }
                           }
                        if(foundobject > 0)
                           break;
                        rlc = rlc->next;
                        }
                     }
                  if(foundobject > 0)
                      break;
                  ii = GetNextArealIndex(rac, LastI, 1, &LastEdge, cindex, &unusedint);
                  LastI = ii;
                  }
               }
            ran = root;
            found_two = 0;
            savei = savej = 1;
            while(ran != NULL)
               {
               if(ran != rac)
                  {
                  PassCond = 0;
                  savei = savej = 1;
                  if((ran->ishole < 0) && (rac->ishole > 0) && (ran->idn == rac->idn))
                     PassCond = 1;  /** a hole 'shares edges' with its parent ... ***/
                  else if((ran->ishole > 0) && (rac->ishole < 0) && (ran->idn == rac->idn))
                     PassCond = 1;  /** a hole 'shares edges' with its parent ... ***/
                  else
                     PassCond = TwoArealsShareEdge(rac, ran, &savei, &savej, tolerance2);
                  if(PassCond > 0)
                     {
                     ++found_two;
                     DCc = (struct DirectConnect *) (malloc(SzDC));
                     DCc->TreeKey = ran->localID + (checkinstance * 0.0001);
                     DCc->localID = ran->localID;
                     DCc->vertex2 = savej;
                     if(savej == 0)
                        {
                        DCc->vertex1 = ran->numverts - 1;
                        }
                     else
                        {
                        DCc->vertex1 = savej - 1;
                        }
                     DCc->next = NULL;
                     if(DCroot == NULL)
                        DCroot = DCc;
                     else
                        DCp->next = DCc;
                     DCp = DCc;
                     }
                  }
               ran = ran->next;
               }
            if((rac->ismulti == 0) && (rac->ishole == 0) && (foundobject == 0) && (found_two == 0))
               {
               if(CheckEligible == 1)
                  {
                  ERc = (struct collection *) (malloc(SzC));
                  if(ERc == NULL)
                     {
                     printf("allocation memory exhausted during area feature condition analysis\n");
                     printf("processing cannot continue\n");
                     exit(-1);
                     }
                  ERc->magnitude = 0;
                  ERc->keyval = NETISOA;
                  ERc->Cnumber = Make_Correct(ERc->keyval,checkinstance);
                  ERc->Sindex1 = rac->Sindex;
                  ERc->aa = rac;
                  ERc->next = NULL;
                  if(ERroot == NULL)
                     {
                     ERroot = ERc;
                     }
                  else
                     {
                     ERn = ERroot;
                     while(ERn != NULL)
                        {
                        if((ERn->Cnumber == ERc->Cnumber) && (ERn->keyval == ERc->keyval) && (ERn->aa == ERc->aa))
                           break;
                        ERn = ERn->next;
                        }
                     if(ERn == NULL)
                        {
                        ERc->next = ERroot;
                        ERroot = ERc;
                        }
                     else
                        free(ERc);
                     }
                  }
               }
            else  /*** must be a multi-region area feature or shares edges, or one having holes, etc... ****/
               {
               RB_newKey2 = rac->localID + (checkinstance * 0.0001); /*** was  rac->featureid ***/
               if(( RB_newNode = RBExactQuery(Trans_RB_Tree,&RB_newKey2)))
                  {
                  RB_cond = (struct TransitiveCollection *) RB_newNode->info;
                  RB_cond->counter += foundobject;

                  if((DCroot != NULL) && (found_two > 0))
                     {
                     DCp = DCroot;
                     while(DCp->next != NULL)
                        DCp = DCp->next;
                     
                     DCp->next = RB_cond->connect;
                     RB_cond->connect = DCroot;
                     }
                  }
               else
                  {
                  RB_cond = (struct TransitiveCollection *) (malloc(SzTC));
                  if(RB_cond == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree information alloction for NETISOA analysis\n");
                     exit(-1);
                     }
                  RB_cond->keyval = NETISOA;
                  RB_cond->Cnumber =  Make_Correct(NETISOA,checkinstance);
                  RB_cond->connect = DCroot;
                  RB_cond->gform = G_AREAL;
                  RB_cond->featureidn = rac->featureid;
                  RB_cond->localID = rac->localID;
                  RB_cond->counter = foundobject;
                  /**RB_cond->counter = foundobject; **/
                  RB_cond->radius = -1;
                  if(rac->ishole > 0) /*** remember the 'ishole' status in the unused height field ***/
                     RB_cond->height = rac->ishole;
                  else
                     RB_cond->height = -1;
                  RB_cond->magnitude = 0.0;
                  RB_cond->IDN = rac->idn;
                  RB_cond->Sindex = rac->Sindex;
                  RB_cond->Lindex = rac->Lindex;

                  RB_cond->numnodes = 2;
                  RB_cond->x = (double *) (malloc(RB_cond->numnodes * SzD));
                  RB_cond->y = (double *) (malloc(RB_cond->numnodes * SzD));
                  RB_cond->z = (double *) (malloc(RB_cond->numnodes * SzD));
                  if(RB_cond->z == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree element vertex alloction for NETISOA analysis\n");
                     exit(-1);
                     }
                  if((savei > rac->numverts) || (savei < 0))
                    savei = 1;
                  RB_cond->x[0] = rac->x[savei];
                  RB_cond->y[0] = rac->y[savei];
                  RB_cond->z[0] = rac->z[savei];
                  if(savei == 0)
                     {
                     RB_cond->x[1] = rac->x[rac->numverts-1];
                     RB_cond->y[1] = rac->y[rac->numverts-1];
                     RB_cond->z[1] = rac->z[rac->numverts-1];
                     }
                  else
                     {
                     RB_cond->x[1] = rac->x[savei-1];
                     RB_cond->y[1] = rac->y[savei-1];
                     RB_cond->z[1] = rac->z[savei-1];
                     }

                  RB_newDbl = (double *) (malloc(SzD));
                  if(RB_newDbl == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree alloction for NETISOA analysis\n");
                     exit(-1);
                     }
                  *RB_newDbl = RB_newKey2; /**rac->localID; **/
                  RBTreeInsert(Trans_RB_Tree,RB_newDbl, RB_cond);
                  } /*** end treating multi-region area for NETISOA ***/
               }
            } /*** end NETISOA check for areals **/


          if( (CombinedCheckApplies(rac->idn,checkinstance,ANETISOA,rac->Lindex,rac->Sindex,
                Config1,Strat1,Domain1, &part1,&part2,&part3) > 0) && (part1 > 0))
            {
            CheckEligible = 1;
            savei = savej = 1;
             if((NorthCount <= 0) || (SouthCount <= 0) || (EastCount <= 0) || (WestCount <= 0))
                {
                maxXindex = minXindex = maxYindex = minYindex = -1;
                for(i=0; i<rac->numverts; i++)
                   {
                   if((rac->x[i] >= minx) && (rac->x[i] <= maxx) && (rac->y[i] >= miny) && (rac->y[i] <= maxy))
                      {
                      if((maxXindex < 0) || (rac->x[i] > rac->x[maxXindex]))
                         maxXindex = i;
                      if((minXindex < 0) || (rac->x[i] < rac->x[minXindex]))
                         minXindex = i;
                      if((maxYindex < 0) || (rac->y[i] > rac->y[maxYindex]))
                         maxYindex = i;
                      if((minYindex < 0) || (rac->y[i] < rac->y[minYindex]))
                         minYindex = i;
                      }
                   }
                CheckEligible = 1;

                if((maxXindex >= 0) && (EastCount <= 0))
                   {
                   if(maxXindex == 0)
                      i = 1;
                   else
                      i = maxXindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[maxXindex],rac->y[maxXindex],root,Proot,Lroot,
                                              rac, NULL, NULL,100,100,EastCount,100,checkinstance);
                   }
                if((minXindex >= 0) && (CheckEligible > 0) && (WestCount <= 0))
                   {
                   if(minXindex == 0)
                      i = 1;
                   else
                      i = minXindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[minXindex],rac->y[minXindex],root,Proot,Lroot,
                                              rac, NULL, NULL,100,100,100,WestCount,checkinstance);
                   }
                if((minYindex >= 0) && (CheckEligible > 0) && (SouthCount <= 0))
                   {
                   if(minYindex == 0)
                      i = 1;
                   else
                      i = minYindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[minYindex],rac->y[minYindex],root,Proot,Lroot,
                                              rac, NULL, NULL,100,SouthCount,100,100,checkinstance);
                   }
                if((maxYindex >= 0) && (CheckEligible > 0) && (NorthCount <= 0))
                   {
                   if(maxYindex == 0)
                      i = 1;
                   else
                      i = maxYindex - 1;
                   CheckEligible = ConditionOnDataEdge(rac->x[i],rac->y[i],rac->x[maxYindex],rac->y[maxYindex],root,Proot,Lroot,
                                              rac, NULL, NULL, NorthCount,100,100,100,checkinstance);
                   }
                }

            tolerance2 = FindRelevantSensitivity(ANETISOA,checkinstance,&checktolerance);
            if(CheckEligible > 0)
               foundobject = 0;
            else
               foundobject = 1;
            DCroot = NULL;
            found_two = 0;
            if(foundobject == 0)
               {
               ran = root;
               while(ran != NULL)
                  {
                  if(((rac->ishole > 0) && (ran->ishole <= 0) && (rac->idn == ran->idn)) ||
                       ((ran->ishole > 0) && (rac->ishole <= 0) && (rac->idn == ran->idn)))
                     {
                     ++found_two;
                     DCc = (struct DirectConnect *) (malloc(SzDC));
                     DCc->TreeKey = ran->localID + (checkinstance * 0.0001);
                     DCc->localID = ran->localID;
                     DCc->vertex2 = savej;
                     if(savej == 0)
                        {
                        DCc->vertex1 = ran->numverts - 1;
                        }
                     else
                        {
                        DCc->vertex1 = savej - 1;
                        }
                     DCc->next = NULL;
                     if(DCroot == NULL)
                        DCroot = DCc;
                     else
                        DCp->next = DCc;
                     DCp = DCc;
                     }
                  else if((ran != rac) &&  (CombinedCheckApplies(ran->idn,checkinstance,ANETISOA,ran->Lindex,ran->Sindex,
                   Config1,Strat1,Domain1, &part1,&part2,&part3) > 0) && (part3 > 0))
                     {
                     if(TwoArealsShareEdge(rac, ran, &savei, &savej, tolerance2) > 0)
                        {
                        foundobject = 1;
                        break;
                        }
                     }

                  else if((ran != rac) &&
                              (TwoArealsShareEdge(rac, ran, &savei, &savej, tolerance2) > 0))
                     {
                     ++found_two;
                     DCc = (struct DirectConnect *) (malloc(SzDC));
                     DCc->TreeKey = ran->localID + (checkinstance * 0.0001);
                     DCc->localID = ran->localID;
                     DCc->vertex2 = savej;
                     if(savej == 0)
                        {
                        DCc->vertex1 = ran->numverts - 1;
                        }
                     else
                        {
                        DCc->vertex1 = savej - 1;
                        }
                     DCc->next = NULL;
                     if(DCroot == NULL)
                        DCroot = DCc;
                     else
                        DCp->next = DCc;
                     DCp = DCc;
                     }


                  ran = ran->next;
                  }
               }


            if((rac->ismulti == 0) && (rac->ishole == 0) && (foundobject == 0) && (found_two == 0))
               {
               if(CheckEligible == 1)
                  {
                  ERc = (struct collection *) (malloc(SzC));
                  if(ERc == NULL)
                     {
                     printf("allocation memory exhausted during area feature condition analysis\n");
                     printf("processing cannot continue\n");
                     exit(-1);
                     }
                  ERc->magnitude = 0;
                  ERc->keyval = ANETISOA;
                  ERc->Cnumber = Make_Correct(ERc->keyval,checkinstance);
                  ERc->Sindex1 = rac->Sindex;
                  ERc->aa = rac;
                  ERc->next = NULL;
                  if(ERroot == NULL)
                     {
                     ERroot = ERc;
                     }
                  else
                     {
                     ERn = ERroot;
                     while(ERn != NULL)
                        {
                        if((ERn->Cnumber == ERc->Cnumber) && (ERn->keyval == ERc->keyval) && (ERn->aa == ERc->aa))
                           break;
                        ERn = ERn->next;
                        }
                     if(ERn == NULL)
                        {
                        ERc->next = ERroot;
                        ERroot = ERc;
                        }
                     else
                        free(ERc);
                     }
                  }
               }
            else  /*** must be a multi-region area feature or shares edges, or one having holes, etc... ****/
               {
               RB_newKey2 = rac->localID + (checkinstance * 0.0001); /*** was  rac->featureid ***/
               if(( RB_newNode = RBExactQuery(Trans_RB_Tree,&RB_newKey2)))
                  {
                  RB_cond = (struct TransitiveCollection *) RB_newNode->info;
                  RB_cond->counter += foundobject;

                  if((DCroot != NULL) && (found_two > 0))
                     {
                     DCp = DCroot;
                     while(DCp->next != NULL)
                        DCp = DCp->next;

                     DCp->next = RB_cond->connect;
                     RB_cond->connect = DCroot;
                     }
                  }
               else
                  {
                  RB_cond = (struct TransitiveCollection *) (malloc(SzTC));
                  if(RB_cond == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree information alloction for ANETISOA analysis\n");
                     exit(-1);
                     }
                  RB_cond->keyval = ANETISOA;
                  RB_cond->Cnumber =  Make_Correct(ANETISOA,checkinstance);
                  RB_cond->connect = DCroot;
                  RB_cond->gform = G_AREAL;
                  RB_cond->featureidn = rac->featureid;
                  RB_cond->localID = rac->localID;
                  RB_cond->counter = foundobject;
                  /**RB_cond->counter = foundobject; **/
                  RB_cond->radius = -1;
                  if(rac->ishole > 0) /*** remember the 'ishole' status in the unused height field ***/
                     RB_cond->height = rac->ishole;
                  else
                     RB_cond->height = -1;
                  RB_cond->magnitude = 0.0;
                  RB_cond->IDN = rac->idn;
                  RB_cond->Sindex = rac->Sindex;
                  RB_cond->Lindex = rac->Lindex;

                  RB_cond->numnodes = 2;
                  RB_cond->x = (double *) (malloc(RB_cond->numnodes * SzD));
                  RB_cond->y = (double *) (malloc(RB_cond->numnodes * SzD));
                  RB_cond->z = (double *) (malloc(RB_cond->numnodes * SzD));
                  if(RB_cond->z == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree element vertex alloction for ANETISOA analysis\n");
                     exit(-1);
                     }
                  if((savei > rac->numverts) || (savei < 0))
                    savei = 1;
                  RB_cond->x[0] = rac->x[savei];
                  RB_cond->y[0] = rac->y[savei];
                  RB_cond->z[0] = rac->z[savei];
                  if(savei == 0)
                     {
                     RB_cond->x[1] = rac->x[rac->numverts-1];
                     RB_cond->y[1] = rac->y[rac->numverts-1];
                     RB_cond->z[1] = rac->z[rac->numverts-1];
                     }
                  else
                     {
                     RB_cond->x[1] = rac->x[savei-1];
                     RB_cond->y[1] = rac->y[savei-1];
                     RB_cond->z[1] = rac->z[savei-1];
                     }
                  RB_newDbl = (double *) (malloc(SzD));
                  if(RB_newDbl == NULL)
                     {
                     printf("memory has been exhausted during RB-Tree alloction for ANETISOA analysis\n");
                     exit(-1);
                     }
                  *RB_newDbl = RB_newKey2; /**rac->localID; **/
                  RBTreeInsert(Trans_RB_Tree,RB_newDbl, RB_cond);
                  } /*** end treating multi-region area for ANETISOA ***/
               }
            } /*** end ANETISOA check for areals **/



         } /**** end for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++) ***/

      rac = rac->next;

      if(abortNow > 0)
         break;
      }  /*** end while rac != NULL ****/

   ERc = ERroot;
   while(ERc != NULL)
      {
      switch(ERc->keyval)
         {
         case ANETISOA:
         case NETISOA: 
             FwriteObject(ERc->keyval,ERc->Cnumber,ERc->aa->Lindex, ERc->Sindex1,
                                   G_AREAL,ERc->aa->localID,ERc->aa->idn,-1.0, -1.0,ERc->aa->x,
                                   ERc->aa->y,ERc->aa->z,2, 1);
             break;

         default:
            printf("unrecognized key condition flag received during area feature analysis\n");
            break;
         }
      ERn = ERc;
      ERc = ERc->next;
      free(ERn);
      }
   return;


}  /*** end PerformTransitiveChecks ***/


void PerformConflationAnalyses(char * prompt, int cindex,struct RawLinear *root,
    double minx,double miny,double maxx,double maxy, double multiplier,
    int NorthCount, int SouthCount, int EastCount, int WestCount,
    struct RawAreal *Aroot, struct RawBridge *Broot, struct RawPoint *Proot,
    struct ListOfEdges *PEroot, struct ListOfEdges *VPEroot,
    struct RawLinear *NLroot, struct ThePolys * Polyroot, int SzPC, int SzPW, int IndexLimit)
{

struct RawLinear *rlc, *rln;
struct RawAreal *rac;
int Config1, Strat1, Domain1, Config2, Strat2, Domain2;
int part1, part2, part3;
int found_one, ii2, ii3,IsCutout;
int i, ii, j, jj, checkinstance;
int inside1, inside2,closeEdge1, closeEdge2;
int unusedint;

double x1,y1,z1,x2,y2,z2;
double fromx, fromy, xi, yi;
double LenToCover, CoveredSoFar;
double MatchLen, unuseddbl;

unsigned char clipflag;

struct IntersectionList
   {
   double x;
   double y;
   double dist;
   struct IntersectionList *next;
   } *IntRoot, *IRn, *IRc, *IRp;
struct ConditionCollection *RB_cond;
struct EdgeIntervals *FTc, *FTp, *FTa;
struct collection
   {
   int keyval;
   int Cnumber;
   double x[5];
   double y[5];
   double z[5];
   double magnitude;
   int IDN;
   unsigned int Sindex;
   int vertexindex1, vertexindex2;
   struct RawAreal *aa;
   struct RawBridge *bb;
   struct RawPoint *pp;
   struct ListOfEdges *le;
   struct RawLinear *ll1,*ll2;
   struct ThePolys *poly;
   struct collection * next;
   } *ERroot, *ERc, *ERn;
int SzCC = sizeof(struct ConditionCollection);
int SzC = sizeof(struct collection);
int SzIL = sizeof(struct IntersectionList);


   ERroot = NULL;
   rlc = root;
   while(rlc != NULL)
      {
      Config1 = SCCtable[rlc->Sindex].C;
      Strat1 = SCCtable[rlc->Sindex].S;
      Domain1 = SCCtable[rlc->Sindex].D;

      for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++)
         {

         if((rlc->badform != 2) && /*** if ->badform set to 2, then this is a complete duplicate with another feature **/
              (CombinedCheckApplies(rlc->idn,checkinstance,CONFLATE,
                      rlc->Lindex,rlc->Sindex,Config1,Strat1,Domain1,&part1,&part2,&part3) > 0))
            {
            if(part1 > 0)
               {
               found_one = 0;
               ii2 = ii3 = -99;  /*** holds indices of verts of consecutive uncovered edges ***/
               for(i=1; i<rlc->numnodes; i++)
                  {
                  ii = i - 1;

                  x1 = rlc->x[ii];
                  y1 = rlc->y[ii];
                  z1 = rlc->z[ii];
                  x2 = rlc->x[i];
                  y2 = rlc->y[i];
                  z2 = rlc->z[i];
                  if(ClipLineToRegion(minx,miny,maxx,maxy,&x1,&y1,&z1,&x2,&y2,&z2,&clipflag,multiplier) > 0)
                     {
                     LenToCover = Distance(x1,y1,x2,y2);
                     LenToCover *= 0.99;
                     CoveredSoFar = 0.0;
                     rln = root;
                     while((rln != NULL) && (CoveredSoFar < LenToCover))
                        {
                        if(rln != rlc)
                           {
                           Config2 = SCCtable[rln->Sindex].C;
                           Strat2 = SCCtable[rln->Sindex].S;
                           Domain2 = SCCtable[rln->Sindex].D;
                           if(CombinedCheckApplies(rln->idn,checkinstance,CONFLATE,rln->Lindex,rln->Sindex,
                                     Config2,Strat2,Domain2,&part1,&part2,&part3) > 0)
                              {
                              if(part2 > 0)
                                 {
                                 for(j=1; j<rln->numnodes; j++)
                                    {
                                    if(MatchLinearSegment2D(x1,y1,x2,y2,
                                                            rln->x[j-1],rln->y[j-1],rln->x[j],rln->y[j],&MatchLen,
                                                            ActiveChecks[checkinstance].sensitivity) > 0)
                                       {
                                       CoveredSoFar += MatchLen;
                                       if(CoveredSoFar >= LenToCover)
                                          break;
                                       }
                                    }
                                 }
                              }
                           }
                        rln = rln->next;
                        }
                     if(CoveredSoFar < LenToCover) /*** checktosee if an area feature edge will cover the linear ***/
                        {
                        rac = Aroot;
                        while((rac != NULL) && (CoveredSoFar < LenToCover))
                           {
                           Config2 = SCCtable[rac->Sindex].C;
                           Strat2 = SCCtable[rac->Sindex].S;
                           Domain2 = SCCtable[rac->Sindex].D;
                           if(CombinedCheckApplies(rac->idn,checkinstance,CONFLATE,rac->Lindex,rac->Sindex,
                                     Config2,Strat2,Domain2,&part1,&part2,&part3) > 0)
                              {
                              if(part2 > 0)
                                 {
                                 for(j=1; j<rac->numverts; j++)
                                    {
                                    if(MatchLinearSegment2D(x1,y1,x2,y2,
                                                            rac->x[j-1],rac->y[j-1],rac->x[j],rac->y[j],&MatchLen,
                                                            1.09 * ActiveChecks[checkinstance].sensitivity) > 0)
                                       {
                                       CoveredSoFar += MatchLen;
                                       if(CoveredSoFar >= LenToCover)
                                             break;
                                       }
                                    }
                                 if(j == rac->numverts) /*** check last edge of area feature ***/
                                    {
                                    if(MatchLinearSegment2D(x1,y1,x2,y2,
                                                         rac->x[0],rac->y[0],rac->x[j-1],rac->y[j-1],&MatchLen,
                                                            1.09 * ActiveChecks[checkinstance].sensitivity) > 0)
                                       {
                                       CoveredSoFar += MatchLen;
                                       }
                                    }
                                 }
                              }
                           rac = rac->next;
                           }
                        }



                     if(CoveredSoFar < LenToCover) /*** check to see if an area feature face will cover the linear ***/
                        {
                        rac = Aroot;
                        while(rac != NULL)
                           {
                           if(rac->ishole <= 0)
                              IsCutout = 0;
                           else
                              IsCutout = 1;
                           if(1)
                              {
                              Config2 = SCCtable[rac->Sindex].C;
                              Strat2 = SCCtable[rac->Sindex].S;
                              Domain2 = SCCtable[rac->Sindex].D;
                              if(CombinedCheckApplies(rac->idn,checkinstance,CONFLATE,rac->Lindex,rac->Sindex,
                                     Config2,Strat2,Domain2,&part1,&part2,&part3) > 0)
                                 {
                                 if(part3 > 0)
                                    {
                                    //if(rac->RB_Tree_Edgelist == NULL)
if((rac->RB_Tree_Edgelist == NULL) || (rac->RB_Tree_Edgelist != NULL))  /*** remove this ***/
                                       {
                                       inside1 = inside2 = 0;
                                       closeEdge1 = closeEdge2 = -100;
                                       inside1 = PointInsidePoly(x1,y1,rac->x, rac->y, rac->numverts);
                                       if(inside1 == 0)
                                          {
                                          if(PointToSmallArealDist2D(x1, y1,rac,&closeEdge1,&unuseddbl) <
                                                   ActiveChecks[checkinstance].sensitivity)
                                             inside1 = 1;
                                          }
                                       inside2 = PointInsidePoly(x2,y2,rac->x, rac->y, rac->numverts);
                                       if(inside2 == 0)
                                          {
                                          if(PointToSmallArealDist2D(x2, y2,rac,&closeEdge2,&unuseddbl) <
                                                   ActiveChecks[checkinstance].sensitivity)
                                             inside2 = 1;
                                          }
                                       IntRoot = NULL;
                                       jj = rac->numverts - 1;
                                       for(j=0; j<rac->numverts; j++)
                                          {
                                          //if(LineSegmentsIntersect(x1,y1,x2,y2,
                                                    //rac->x[jj],rac->y[jj],rac->x[j],rac->y[j], &xi, &yi) > 0)
                                       if((LineSegmentsIntersect(x1,y1,x2,y2,
                                                 rac->x[jj],rac->y[jj],rac->x[j],rac->y[j], &xi, &yi) > 0) &&
                                           (EqualWithinTolerance(x1,y1,xi,yi,ActiveChecks[checkinstance].sensitivity) <= 0) &&
                                           (EqualWithinTolerance(x2,y2,xi,yi,ActiveChecks[checkinstance].sensitivity) <= 0))
                                             {
                                             IRn = (struct IntersectionList *) (malloc(SzIL));
                                             IRn->x = xi;
                                             IRn->y = yi;
                                             IRn->dist = ((xi - x1) * (xi - x1)) + ((yi - y1) * (yi - y1));
                                             if(IntRoot == NULL)
                                                {
                                                IntRoot = IRn;
                                                IRn->next = NULL;
                                                }
                                             else
                                                {
                                                IRc = IRp = IntRoot;
                                                while(IRc != NULL)
                                                   {
                                                   if(IRn->dist < IRc->dist)
                                                      break;
                                                   IRp = IRc;
                                                   IRc = IRc->next;
                                                   }
                                                if(IRc == NULL)
                                                   {
                                                   IRp->next = IRn;
                                                   IRn->next = NULL;
                                                   }
                                                else if(IRc == IntRoot)
                                                   {
                                                   IRn->next = IntRoot;
                                                   IntRoot = IRn;
                                                   }
                                                else
                                                   {
                                                   IRn->next = IRc;
                                                   IRp->next = IRn;
                                                   }
                                                }
                                             }
                                          jj = j;
                                          }
                                       if((inside1 > 0) && (inside2 > 0) && (IntRoot == NULL))
                                          {
                                          if(IsCutout == 0)
                                             CoveredSoFar += LenToCover;
                                          else
                                             CoveredSoFar -= LenToCover;
                                          }
                                       else if(IntRoot != NULL)  /*** must be some intersections - what parts are inside? ****/
                                          {
                                          fromx = x1;
                                          fromy = y1;
                                          IRc = IntRoot;
                                          if(inside1 > 0)
                                             {
                                             jj = 1;
                                             }
                                          else
                                             {
                                             jj = 0;
                                             }
                                          while(IRc != NULL)
                                             {
                                             if(jj == 1) /** add in the distance here ***/
                                                {
                                                //CoveredSoFar += Distance(fromx,fromy,IRc->x,IRc->y);
                                                if(IsCutout == 0)
                                                   CoveredSoFar += Distance(fromx,fromy,IRc->x,IRc->y);
                                                else
                                                   CoveredSoFar -= Distance(fromx,fromy,IRc->x,IRc->y);
                                                jj = 0;
                                                }
                                             else
                                                jj = 1;
                                             fromx = IRc->x;
                                             fromy = IRc->y;
                                             IRc = IRc->next;
                                             }
                                          if(jj > 0)
                                             {
                                             //CoveredSoFar += Distance(fromx,fromy,x2,y2);
                                             if(IsCutout == 0)
                                                CoveredSoFar += Distance(fromx,fromy,x2,y2);
                                             else
                                                CoveredSoFar -= Distance(fromx,fromy,x2,y2);
                                             }
                                          IRc = IntRoot;
                                          while(IRc != NULL)
                                             {
                                             IRp = IRc;
                                             IRc = IRc->next;
                                             free(IRp);
                                             }
                                          IntRoot = NULL;
                                          }

                                       }  /**** end is a small area feature ***/

                                    } /**** end if(part3 > 0) ****/
                                 } /*** end combined check applies as tertiary for the area feature **/
                              }  /** end ishole indicates rac is an outer edge ***/
                           rac = rac->next;
                           } /** end while rac ...   ***/
                        } /** end covered so far is not enough ***/

                     if(rlc->IsMulti >= 1)
                        {
                        RB_newKey2 = rlc->localID + (checkinstance * 0.0001);
                        if(( RB_newNode = RBExactQuery(Check_RB_Tree,&RB_newKey2)))
                           {
                           RB_cond = (struct ConditionCollection *) RB_newNode->info;
                           RB_cond->magnitude += LenToCover - CoveredSoFar; /** this requires handshake with condition
                                                                           writing test in TT.c  ***/
                           FTc = RB_cond->index_from_to;
                           FTp = FTc;
                           while(FTc != NULL)
                              {
                              if(FTc->start == i)
                                 {
                                 FTc->start = i - 1;
                                 break;
                                 }
                              else if(FTc->stop == (i - 1))
                                 {
                                 FTc->stop = i;
                                 break;
                                 }
                              else if((FTc->start <= i) && (FTc->stop >= i))
                                 break;  /** this can happen as cross region boundaries and check same edge again ***/
                              FTp = FTc;
                              FTc = FTc->next;
                              }
                           if(FTc == NULL)
                              {
                              FTc = (struct EdgeIntervals *) (malloc(sizeof(struct EdgeIntervals)));
                              if(FTc == NULL)
                                 {
                                 printf("memory has been exhausted during RB-Tree alloction for CONFLATE (edges1) analysis\n");                                 exit(-1);
                                 }
                              FTc->start = i - 1;
                              FTc->stop = i;
                              FTc->next = NULL;
                              FTp->next = FTc;
                              }
                           else
                              {
                              FTa = RB_cond->index_from_to;
                              while(FTa != NULL)
                                 {
                                 if(FTa->start == FTc->stop)
                                    {
                                    FTa->start = FTc->start;
                                    if(FTc == RB_cond->index_from_to)
                                       RB_cond->index_from_to = FTc->next;
                                    else
                                       FTp->next = FTc->next;
                                    free(FTc);
                                    break;
                                    }
                                 else if(FTa->stop == FTc->start)
                                    {
                                    FTa->stop = FTc->stop;
                                    if(FTc == RB_cond->index_from_to)
                                       RB_cond->index_from_to = FTc->next;
                                    else
                                       FTp->next = FTc->next;
                                    free(FTc);
                                    break;
                                    }
                                 FTa = FTa->next;
                                 }
                              }
                           }
                        else /*** must not have found any uncovered edges for this line feature before now ***/
                           {
                           RB_cond = (struct ConditionCollection *) (malloc(SzCC));
                           if(RB_cond == NULL)
                              {
                              printf("memory has been exhausted during RB-Tree information alloction for CONFLATE analysis\n");                              exit(-1);
                              }
                           RB_cond->keyval = CONFLATE;
                           FTc = (struct EdgeIntervals *) (malloc(sizeof(struct EdgeIntervals)));
                           if(FTc == NULL)
                              {
                              printf("memory has been exhausted during RB-Tree alloction for CONFLATE (edges2) analysis\n");
                              exit(-1);
                              }

                           FTc->start = i - 1;
                           FTc->stop = i;
                           FTc->next = NULL;
                           RB_cond->index_from_to = FTc;
                           RB_cond->Cnumber =  Make_Correct(CONFLATE,checkinstance);
                           RB_cond->gform = G_LINE;
                           RB_cond->featureidn = rlc->featureid;
                           RB_cond->localID = rlc->localID;
                           RB_cond->counter = 2;
                           RB_cond->radius = -1;
                           RB_cond->height = -1;
                           RB_cond->magnitude = 0.0;
                           RB_cond->IDN = rlc->idn;
                           RB_cond->Sindex = rlc->Sindex;
                           RB_cond->Lindex = rlc->Lindex;

                           RB_cond->numnodes = rlc->numnodes;
                           RB_cond->x = (double *) (malloc(RB_cond->numnodes * SzD));
                           RB_cond->y = (double *) (malloc(RB_cond->numnodes * SzD));
                           RB_cond->z = (double *) (malloc(RB_cond->numnodes * SzD));
                           if(RB_cond->z == NULL)
                              {
                              printf("memory has been exhausted during RB-Tree element vertex alloction for NOCOINCIDE analysis\n");
                              exit(-1);
                              }
                           for(jj=0; jj< rlc->numnodes; jj++)
                              {
                              RB_cond->x[jj] = rlc->x[jj];
                              RB_cond->y[jj] = rlc->y[jj];
                              RB_cond->z[jj] = rlc->z[jj];
                              }

                           RB_newDbl = (double *) (malloc(SzD));
                           if(RB_newDbl == NULL)
                              {
                              printf("memory has been exhausted during RB-Tree alloction for NOCOINCIDE analysis\n");
                              exit(-1);
                              }
                           *RB_newDbl = RB_newKey2;
                           RBTreeInsert(Check_RB_Tree,RB_newDbl, RB_cond);
                           } /*** end add a new tree node ***/
                        } /*** end multi-region process ***/

                     //else if(CoveredSoFar < LenToCover) /*** and is  a single region feature ***/
                     else if((rlc->IsMulti < 1) && ((LenToCover - CoveredSoFar) > 0.1))
                        {
                        ++found_one;
                        if(ii2 < 0)
                           {
                           ii2 = i - 1;
                           ii3 = i;
                           }
                        else
                           {
                           ii3 = i;
                           }
                        if(found_one == 1)
                           {
                           ERc = (struct collection *) (malloc(SzC));
                           if(ERc == NULL)
                              {
                              printf("allocation memory exhausted during linear feature condition analysis\n");                               printf("processing cannot continue\n");
                              exit(-1);
                              }
                           ERc->x[0] = rlc->x[i-1];
                           ERc->y[0] = rlc->y[i-1];
                           ERc->z[0] = rlc->z[i-1];
                           ERc->x[1] = rlc->x[i];
                           ERc->y[1] = rlc->y[i];
                           ERc->z[1] = rlc->z[i];
                           ERc->magnitude = 0.0;
                           ERc->keyval = CONFLATE;
                           ERc->Cnumber = Make_Correct(ERc->keyval,checkinstance);
                           ERc->IDN = rlc->idn;
                           ERc->Sindex = rlc->Sindex;
                           ERc->ll1 = rlc;
                           ERc->vertexindex1 = i-1;
                           ERc->ll2 = NULL;
                           ERc->vertexindex2 = i;
                           ERc->next = NULL;
                           if(ERroot == NULL)
                              {
                              ERroot = ERc;
                              }
                           else
                              {
                              ERn = ERroot;
                              while(ERn != NULL)
                                 {
                                 if((ERn->Cnumber == ERc->Cnumber) && (ERn->keyval == ERc->keyval) && (ERn->ll1 == ERc->ll1) &&
                                    (ERn->x[0] == ERc->x[0]) && (ERn->y[0] == ERc->y[0]) &&
                                      (ERn->x[1] == ERc->x[1]) && (ERn->y[1] == ERc->y[1]))
                                    break;

                                 ERn = ERn->next;
                                 }
                              if(ERn == NULL)
                                 {
                                 ERc->next = ERroot;
                                 ERroot = ERc;
                                 }
                              else
                                free(ERc);
                              }
                           }
                        else
                           {
                           ERn = ERroot;
                           while(ERn != NULL)
                              {
                              if((ERn->Cnumber == ERc->Cnumber) && (ERn->keyval == ERc->keyval) && (ERn->ll1 == ERc->ll1) &&
                                (ERn->vertexindex1 == ii2) && (ERn->vertexindex2 < ii3))
                                 {
                                 ERn->vertexindex2 = ii3;
                                 break;
                                 }
                              ERn = ERn->next;
                              }
                           if(ERn == NULL)
                              {
                              printf("could not find CONFLATE failure for second segment\n");
                              exit(-1);
                              }
                           }
                        }
                     else
                        {
                        ii2 = ii3 = -99;  /**reset the vertex number pointers ***/
                        found_one = 0;
                        }
                     } /*** end if(ClipLineToRegion(minx ***/
                  } /*** end for(i=1; i<rlc->numnodes;   ****/
               } /** end part1 > 0 **/
            } /*** end CONFLATE applies to rlc ***/


         if((rlc->badform != 2) && /*** if ->badform set to 2, then this is a complete duplicate with another feature **/
                (rlc->firstencounter == 1) && (ATTRTYPE_TDS6() > 0) &&
                   (CombinedCheckApplies(rlc->idn,checkinstance,CONF_STATS,
                      rlc->Lindex,rlc->Sindex,Config1,Strat1,Domain1,&part1,&part2,&part3) > 0))
            {
            if(part1 > 0)
               {
               CaptureInformativeValueStats(rlc, rlc->Lindex, rlc->idn, cindex);
               } /** end part1 for CONF_STATS is true **/

            } /** end CONF_STATS applies ***/

         } /** end for(checkinstance = ***/
      rlc = rlc->next;
      } /*** end while(rlc != NULL) ***/

   ERc = ERroot;
   while(ERc != NULL)
      {
      switch(ERc->keyval)
         {
         case CONFLATE:
            FwriteObjectAndMessage(ERc->keyval,ERc->Cnumber,
                       ERc->ll1->idn,G_LINE,ERc->ll1->Lindex,ERc->ll1->localID,ERc->Sindex,-1.0,-1.0,
                       &ERc->ll1->x[ERc->vertexindex1], &ERc->ll1->y[ERc->vertexindex1], &ERc->ll1->z[ERc->vertexindex1],
                       (ERc->vertexindex2 - ERc->vertexindex1 + 1),FindCONFLATEsourceFile((int) ERc->ll1->idn, &unusedint) );
            break;

         default:
            printf("unrecognized key condition flag received during linear feature analysis\n");
            break;
         }
      ERn = ERc;
      ERc = ERc->next;
      free(ERn);
      }
   return;

}


void AddPseudoNodeFromOuterToInnerRings(struct RawAreal *Aroot, int compositeindex)
{
struct RawAreal *rap, *rah;
int k1, k2;
double *xcoords, *ycoords, *zcoords;

   rah = Aroot;
   while(rah != NULL)
      {
      if(rah->ishole > 0)
         {
         rap = Aroot;
         while(rap != NULL)
            {
            if((rap->ishole < 0) && (rap->idn == rah->idn)) /** then this is the parent for rah ***/
               {
               xcoords = (double *) (malloc(SzD * (rah->numverts + 1)));
               ycoords = (double *) (malloc(SzD * (rah->numverts + 1)));
               zcoords = (double *) (malloc(SzD * (rah->numverts + 1)));

               xcoords[0] = rap->x[0];
               ycoords[0] = rap->y[0];
               zcoords[0] = rap->z[0];

               k1 = 1;
               for(k2 = 0; k2 < rah->numverts; k2++)
                  {
                  xcoords[k1] = rah->x[k2];
                  ycoords[k1] = rah->y[k2];
                  zcoords[k1] = rah->z[k2];
                  ++k1;
                  }
               free(rah->x);
               free(rah->y);
               free(rah->z);
               rah->x = xcoords;
               rah->y = ycoords;
               rah->z = zcoords;
               rah->numverts += 1;

               break;
               }
            rap = rap->next;
            }
         }
      rah = rah->next;
      }
   return;
}



void RemoveInnerRingPseudoNode(struct RawAreal *Aroot, int compositeindex)
{
struct RawAreal *rap, *rah;
int k1,k2;
double *xcoords, *ycoords, *zcoords;

   rah = Aroot;
   while(rah != NULL)
      {
      if(rah->ishole > 0)
         {
         rap = Aroot;
         while(rap != NULL)
            {
            if((rap->ishole < 0) && (rap->idn == rah->idn)) /** then this is the parent for rah ***/
               {
               if((rap->x[0] == rah->x[0]) && (rap->y[0] == rah->y[0]) && (rap->z[0] == rah->z[0]))
                  {
                  xcoords = (double *) (malloc(SzD * (rah->numverts - 1)));
                  ycoords = (double *) (malloc(SzD * (rah->numverts - 1)));
                  zcoords = (double *) (malloc(SzD * (rah->numverts - 1)));

                  k1 = 0;
                  for(k2 = 1; k2 < rah->numverts; k2++)
                     {
                     xcoords[k1] = rah->x[k2];
                     ycoords[k1] = rah->y[k2];
                     zcoords[k1] = rah->z[k2];
                     ++k1;
                     }
                  free(rah->x);
                  free(rah->y);
                  free(rah->z);
                  rah->x = xcoords;
                  rah->y = ycoords;
                  rah->z = zcoords;
                  rah->numverts -= 1;

                  break;
                  }
               }
            rap = rap->next;
            }
         }
      rah = rah->next;
      }
   return;
}




void AddPseudoNodesAtIntersections(struct RawLinear **Lroot, struct RawAreal **Aroot, struct RawPoint *Proot,
                        int compositeindex, int checkinstance,
                        double MIX, double MIY, double MAX, double MAY)
{
struct RawLinear *rlc, *rln2, *rlp, *rlpprev;
/**struct RawLinear *rlctest;**/
struct RawAreal *rac, *rap, *raToAdd;
struct RawPoint *rpc;
struct RB_Edgelist * LastIEdge, *LastJEdge;
double *xcoords, *ycoords, *zcoords;
int i,j,i0,j0,k1,k2,k3,addnewpoint,climit,plimit;
int checkapplies,part1,part2,part3;
int capplies2,p1,p2,p3,priorV,nextV;
int LastI, UsedTree, LastJ;
double xint,yint,equalitylimit;
double d1,d2,d3,d4;
/****double IntersectionEquality = 0.000000001; ***/
double unuseddbl;
int SzRL = sizeof(struct RawLinear);


   equalitylimit = ActiveChecks[checkinstance].sensitivity;
   rlc = *Lroot;
   while(rlc != NULL)
      {
      checkapplies = CombinedCheckApplies(rlc->idn,checkinstance,CREATENET,rlc->Lindex,rlc->Sindex,
                                          SCCtable[rlc->Sindex].C, SCCtable[rlc->Sindex].S, SCCtable[rlc->Sindex].D,
                                          &part1, &part2, &part3);
if(checkapplies <= 0)
checkapplies = CombinedCheckApplies(rlc->idn,checkinstance,NETISOFEAT,rlc->Lindex,rlc->Sindex,
                                          SCCtable[rlc->Sindex].C, SCCtable[rlc->Sindex].S, SCCtable[rlc->Sindex].D,
                                          &part1, &part2, &part3);
      if(checkapplies > 0)
         {
         rlp = rlc->next; 
         rlpprev = rlc;

         while(rlp != NULL)
            {
            if(rlp == rlc)
               capplies2 = 0;
            else
               {
               capplies2 = CombinedCheckApplies(rlp->idn,checkinstance,CREATENET,rlp->Lindex,rlp->Sindex,
                                          SCCtable[rlp->Sindex].C, SCCtable[rlp->Sindex].S, SCCtable[rlp->Sindex].D,
                                          &p1, &p2, &p3);
if(capplies2 <= 0)
capplies2 = CombinedCheckApplies(rlp->idn,checkinstance,NETISOFEAT,rlp->Lindex,rlp->Sindex,
                                          SCCtable[rlp->Sindex].C, SCCtable[rlp->Sindex].S, SCCtable[rlp->Sindex].D,
                                          &p1, &p2, &p3);
               }
            if(capplies2 > 0)
               {

               i0 = 0;
               climit = rlc->NumIntersections;
               i = 1;

               while(i < climit)
                  {
                  j0 = 0;
                  j = 1;
                  plimit = rlp->NumIntersections;
                  while(j < plimit)
                     {
                     d1 = 1000.0;
                     if(i0 == 0)
                        {
                        d1 = PointToLineDist2DwithFlag(rlc->x[i0],rlc->y[i0],
                                                                rlp->x[j0],rlp->y[j0],rlp->x[j],rlp->y[j],&unuseddbl);
                        }
                     if(d1 <= equalitylimit)
                        {
                        xint = rlc->x[i0];
                        yint = rlc->y[i0];
                        }
                     else
                        {
                        d1 = PointToLineDist2DwithFlag(rlc->x[i],rlc->y[i],
                                                                rlp->x[j0],rlp->y[j0],rlp->x[j],rlp->y[j],&unuseddbl);
                        if(d1 <= equalitylimit)
                           {
                           xint = rlc->x[i];
                           yint = rlc->y[i];
                           }
                        else
                           {
                           if(j0 == 0)
                              {
                              d1 = PointToLineDist2DwithFlag(rlp->x[j0],rlp->y[j0],
                                                                rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],&unuseddbl);
                              }
                           if(d1 <= equalitylimit)
                              {
                              xint = rlp->x[j0];
                              yint = rlp->y[j0];
                              }
                           else
                              {
                              d1 = PointToLineDist2DwithFlag(rlp->x[j],rlp->y[j],
                                                                rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],&unuseddbl);
                              if(d1 <= equalitylimit)
                                 {
                                 xint = rlp->x[j];
                                 yint = rlp->y[j];
                                 }
                              }
                           }
                        }
                     k3 = 0;
                     if(d1 > equalitylimit)
                        k3 = LineSegmentsIntersect(rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],
                                              rlp->x[j0],rlp->y[j0],rlp->x[j],rlp->y[j],&xint, &yint);
                     //if((d1 <= equalitylimit) || (LineSegmentsIntersect(rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],
                                              //rlp->x[j0],rlp->y[j0],rlp->x[j],rlp->y[j],&xint, &yint) > 0))
                     if((d1 <= equalitylimit) || (k3 > 0))
                        {
                        addnewpoint = 0;
                        d1 = Distance(xint,yint,rlc->x[i0],rlc->y[i0]);
                        if(d1 <= equalitylimit)
                           {
                           d3 = Distance(xint,yint,rlp->x[j0],rlp->y[j0]);
                           if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                              {
                              d4 = Distance(xint,yint,rlp->x[j],rlp->y[j]);
                              if(d4 > equalitylimit)
                                 addnewpoint = 1;
                              }
                           }
                        else
                           {
                           d2 = Distance(xint,yint,rlc->x[i],rlc->y[i]);
                           if(d2 <= equalitylimit)
                              {
                              d3 = Distance(xint,yint,rlp->x[j0],rlp->y[j0]);
                              if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                                 {
                                 d4 = Distance(xint,yint,rlp->x[j],rlp->y[j]);
                                 if(d4 > equalitylimit)
                                    addnewpoint = 1;
                                 }
                              }
                           else
                              addnewpoint = 1;
                           }
                        if(addnewpoint > 0)
                           {
                           xcoords = (double *) (malloc(SzD * (rlc->numnodes + 1)));
                           ycoords = (double *) (malloc(SzD * (rlc->numnodes + 1)));
                           zcoords = (double *) (malloc(SzD * (rlc->numnodes + 1)));

                           k1 = 0;
                           for(k2 = 0; k2 <= i0; k2++)
                              {
                              xcoords[k1] = rlc->x[k2];
                              ycoords[k1] = rlc->y[k2];
                              zcoords[k1] = rlc->z[k2];
                              ++k1;
                              }
                           xcoords[k1] = xint;
                           ycoords[k1] = yint;
                           zcoords[k1] = InterpLinearElev(rlc->x[i0],rlc->y[i0],rlc->z[i0],
                                           rlc->x[i],rlc->y[i],rlc->z[i],xint,yint);
                           ++k1;
                           for(k2 = i; k2 < rlc->numnodes; k2++)
                              {
                              xcoords[k1] = rlc->x[k2];
                              ycoords[k1] = rlc->y[k2];
                              zcoords[k1] = rlc->z[k2];
                              ++k1;
                              }
                           free(rlc->x);
                           free(rlc->y);
                           free(rlc->z);
                           rlc->x = xcoords;
                           rlc->y = ycoords;
                           rlc->z = zcoords;
                           rlc->numnodes += 1;


                           xcoords = (double *) (malloc(SzD * (rlp->numnodes + 1)));
                           ycoords = (double *) (malloc(SzD * (rlp->numnodes + 1)));
                           zcoords = (double *) (malloc(SzD * (rlp->numnodes + 1)));

                           k1 = 0;
                           for(k2 = 0; k2 <= j0; k2++)
                              {
                              xcoords[k1] = rlp->x[k2];
                              ycoords[k1] = rlp->y[k2];
                              zcoords[k1] = rlp->z[k2];
                              ++k1;
                              }
                           xcoords[k1] = xint;
                           ycoords[k1] = yint;
                           zcoords[k1] = InterpLinearElev(rlp->x[j0],rlp->y[j0],rlp->z[j0],
                                           rlp->x[j],rlp->y[j],rlp->z[j],xint,yint);
                           ++k1;
                           for(k2 = j; k2 < rlp->numnodes; k2++)
                              {
                              xcoords[k1] = rlp->x[k2];
                              ycoords[k1] = rlp->y[k2];
                              zcoords[k1] = rlp->z[k2];
                              ++k1;
                              }
                           free(rlp->x);
                           free(rlp->y);
                           free(rlp->z);
                           rlp->x = xcoords;
                           rlp->y = ycoords;
                           rlp->z = zcoords;
                           rlp->numnodes += 1;
                           }
                        }
                     j0 = j;
                     j += 1;
                     }
                  i0 = i;
                  i += 1;
                  }
               }
            rlpprev = rlp;
            rlp = rlp->next;
            }




         rap = *Aroot;

         while(rap != NULL)
            {
            capplies2 = CombinedCheckApplies(rap->idn,checkinstance,CREATENET,rap->Lindex,rap->Sindex,
                                          SCCtable[rap->Sindex].C, SCCtable[rap->Sindex].S, SCCtable[rap->Sindex].D,
                                          &p1, &p2, &p3);
if(capplies2 <= 0)
capplies2 = CombinedCheckApplies(rap->idn,checkinstance,NETISOFEAT,rap->Lindex,rap->Sindex,
                                          SCCtable[rap->Sindex].C, SCCtable[rap->Sindex].S, SCCtable[rap->Sindex].D,
                                          &p1, &p2, &p3);
            if(capplies2 > 0)
               {
//printf("  secondary area %d\n",rap->featureid);
               i0 = 0;
               climit = rlc->NumIntersections;
               i = 1;
               plimit = rap->coversLM;
               while(i < climit)
                  {
                  if(rap->RB_Tree_Edgelist != NULL)
                     {
                     LastJEdge = NULL;
                     LastJ = -1;
                     j = GetNextArealIndex(rap, LastJ, 1, &LastJEdge, compositeindex, &UsedTree);
                     LastJ = j;
                     }
                  else
                     j = 0;
                  while(j >= 0)
                     {
                     if(j == 0)
                        j0 = plimit - 1;
                     else
                        j0 = j - 1;
                     d1 = 1000.0;
                     if(i0 == 0)
                        {
                        d1 = PointToLineDist2DwithFlag(rlc->x[i0],rlc->y[i0],
                                                                rap->x[j0],rap->y[j0],rap->x[j],rap->y[j],&unuseddbl);
                        }
                     if(d1 <= equalitylimit)
                        {
                        xint = rlc->x[i0];
                        yint = rlc->y[i0];
                        }
                     else
                        {
                        d1 = PointToLineDist2DwithFlag(rlc->x[i],rlc->y[i],
                                                                rap->x[j0],rap->y[j0],rap->x[j],rap->y[j],&unuseddbl);
                        if(d1 <= equalitylimit)
                           {
                           xint = rlc->x[i];
                           yint = rlc->y[i];
                           }
                        else
                           {
                           if(j0 == 0)
                              {
                              d1 = PointToLineDist2DwithFlag(rap->x[j0],rap->y[j0],
                                                                rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],&unuseddbl);
                              }
                           if(d1 <= equalitylimit)
                              {
                              xint = rap->x[j0];
                              yint = rap->y[j0];
                              }
                           else
                              {
                              d1 = PointToLineDist2DwithFlag(rap->x[j],rap->y[j],
                                                                rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],&unuseddbl);
                              if(d1 <= equalitylimit)
                                 {
                                 xint = rap->x[j];
                                 yint = rap->y[j];
                                 }
                              }
                           }
                        }

                     if((d1 <= equalitylimit) || (LineSegmentsIntersect(rlc->x[i0],rlc->y[i0],rlc->x[i],rlc->y[i],
                                              rap->x[j0],rap->y[j0],rap->x[j],rap->y[j],&xint, &yint) > 0))
                        {
                        addnewpoint = 0;
                        d1 = Distance(xint,yint,rlc->x[i0],rlc->y[i0]);
                        if(d1 <= equalitylimit)
                           {
                           d3 = Distance(xint,yint,rap->x[j0],rap->y[j0]);
                           if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                              {
                              d4 = Distance(xint,yint,rap->x[j],rap->y[j]);
                              if(d4 > equalitylimit)
                                 addnewpoint = 1;
                              }
                           }
                        else
                           {
                           d2 = Distance(xint,yint,rlc->x[i],rlc->y[i]);
                           if(d2 <= equalitylimit)
                              {
                              d3 = Distance(xint,yint,rap->x[j0],rap->y[j0]);
                              if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                                 {
                                 d4 = Distance(xint,yint,rap->x[j],rap->y[j]);
                                 if(d4 > equalitylimit)
                                    addnewpoint = 1;
                                 }
                              }
                           else
                              addnewpoint = 1;
                           }
                        if(addnewpoint > 0)
                           {
                           k1 = rlc->numnodes;
                           rlc->numnodes += 1;
                           k2 = rlc->numnodes;
                           rlc->x = (double *) (realloc(rlc->x,k2 * SzD));
                           rlc->x[k1] = xint;
                           rlc->y = (double *) (realloc(rlc->y,k2 * SzD));
                           rlc->y[k1] = yint;
                           rlc->z = (double *) (realloc(rlc->z,k2 * SzD));
                           rlc->z[k1] = InterpLinearElev(rlc->x[i0],rlc->y[i0],rlc->z[i0],
                                           rlc->x[i],rlc->y[i],rlc->z[i],xint,yint);

                           rap->firstindex = -99;
                           k1 = rap->numverts;
                           rap->numverts += 1;
                           k2 = rap->numverts;
                           rap->x = (double *) (realloc(rap->x,k2 * SzD));
                           rap->x[k1] = xint;
                           rap->y = (double *) (realloc(rap->y,k2 * SzD));
                           rap->y[k1] = yint;
                           rap->z = (double *) (realloc(rap->z,k2 * SzD));
                           rap->z[k1] = InterpLinearElev(rap->x[j0],rap->y[j0],rap->z[j0],
                                           rap->x[j],rap->y[j],rap->z[j],xint,yint);

                           }
                        }
                     if(rap->RB_Tree_Edgelist != NULL)
                        {
                        j = GetNextArealIndex(rap, LastJ, 1, &LastJEdge, compositeindex, &UsedTree);
                        LastJ = j;
                        }
                     else
                        {
                        ++j;
                        if(j >= plimit)
                           j = -1;
                        }
                     }
                  i0 = i;
                  i += 1;
                  }
               }
            rap = rap->next;
            }

         }
      rlc = rlc->next;
      }


         
   rac = *Aroot;
   while(rac != NULL)
      {
      checkapplies = CombinedCheckApplies(rac->idn,checkinstance,CREATENET,rac->Lindex,rac->Sindex,
                                          SCCtable[rac->Sindex].C, SCCtable[rac->Sindex].S, SCCtable[rac->Sindex].D,
                                          &part1, &part2, &part3);
if(checkapplies <= 0)
checkapplies = CombinedCheckApplies(rac->idn,checkinstance,NETISOFEAT,rac->Lindex,rac->Sindex,
                                          SCCtable[rac->Sindex].C, SCCtable[rac->Sindex].S, SCCtable[rac->Sindex].D,
                                          &part1, &part2, &part3);
      if(checkapplies > 0)
         {
         climit = rac->coversLM;
         rap = rac->next;
         while(rap != NULL)
            {
            plimit = rap->coversLM;
            capplies2 = CombinedCheckApplies(rap->idn,checkinstance,CREATENET,rap->Lindex,rap->Sindex,
                                          SCCtable[rap->Sindex].C, SCCtable[rap->Sindex].S, SCCtable[rap->Sindex].D,
                                          &p1, &p2, &p3);
if(capplies2 <= 0)
capplies2 = CombinedCheckApplies(rap->idn,checkinstance,NETISOFEAT,rap->Lindex,rap->Sindex,
                                          SCCtable[rap->Sindex].C, SCCtable[rap->Sindex].S, SCCtable[rap->Sindex].D,
                                          &p1, &p2, &p3);
            if(capplies2 > 0)
               {
               if(rac->RB_Tree_Edgelist != NULL)
                  {
                  LastIEdge = NULL;
                  LastI = -1;
                  i = GetNextArealIndex(rac, LastI, 1, &LastIEdge, compositeindex, &UsedTree);
                  LastI = i;
                  }
               else
                  i = 0;
               while(i >= 0)
                  {
                  if(i == 0)
                     i0 = climit - 1;
                  else
                     i0 = i - 1;
                  addnewpoint = 0;
                  if(rap->RB_Tree_Edgelist != NULL)
                     {
                     LastJEdge = NULL;
                     LastJ = -1;
                     j = GetNextArealIndex(rap, LastJ, 1, &LastJEdge, compositeindex, &UsedTree);
                     LastJ = j;
                     }
                  else
                     j = 0;
                  while(j >= 0)
                     {
                     if(j == 0)
                        j0 = plimit - 1;
                     else
                        j0 = j - 1;
                     if(LineSegmentsIntersect(rac->x[i0],rac->y[i0],rac->x[i],rac->y[i],
                                              rap->x[j0],rap->y[j0],rap->x[j],rap->y[j],&xint, &yint) > 0)
                        {
                        addnewpoint = 0;
                        d1 = Distance(xint,yint,rac->x[i0],rac->y[i0]);
                        if(d1 <= equalitylimit)
                           {
                           d3 = Distance(xint,yint,rap->x[j0],rap->y[j0]);
                           if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                              {
                              d4 = Distance(xint,yint,rap->x[j],rap->y[j]);
                              if(d4 > equalitylimit)
                                 addnewpoint = 1;
                              }
                           }
                        else
                           {
                           d2 = Distance(xint,yint,rac->x[i],rac->y[i]);
                           if(d2 <= equalitylimit)
                              {
                              d3 = Distance(xint,yint,rap->x[j0],rap->y[j0]);
                              if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                                 {
                                 d4 = Distance(xint,yint,rap->x[j],rap->y[j]);
                                 if(d4 > equalitylimit)
                                    addnewpoint = 1;
                                 }
                              }
                           else
                              addnewpoint = 1;
                           }
                        }
                        if(addnewpoint == 0)
                           {
                           d1 = d2 = d3 = d4 = equalitylimit * 1000;
                           raToAdd = NULL;

                           d1 = PointToLineDist2DwithFlag(rac->x[i],rac->y[i],
                                                               rap->x[j0],rap->y[j0],rap->x[j],rap->y[j],&unuseddbl);
                           if(d1 <= equalitylimit)
                              {
                              xint = rac->x[i];
                              yint = rac->y[i];
                              raToAdd = rap;
                              priorV = j0;
                              nextV = j;
                              }
                           else
                              {
                              d2 = PointToLineDist2DwithFlag(rac->x[i0],rac->y[i0],
                                                               rap->x[j0],rap->y[j0],rap->x[j],rap->y[j],&unuseddbl);
                              if(d2 <= equalitylimit)
                                 {
                                 xint = rac->x[i0];
                                 yint = rac->y[i0];
                                 raToAdd = rap;
                                 priorV = j0;
                                 nextV = j;
                                 }
                              else
                                 {
                                 d3 = PointToLineDist2DwithFlag(rap->x[j0],rap->y[j0],
                                                               rac->x[i0],rac->y[i0],rac->x[i],rac->y[i],&unuseddbl);
                                 if(d3 <= equalitylimit)
                                    {
                                    xint = rap->x[j0];
                                    yint = rap->y[j0];
                                    raToAdd = rac;
                                    priorV = i0;
                                    nextV = i;
                                    }
                                 else
                                    {
                                    d4 = PointToLineDist2DwithFlag(rap->x[j],rap->y[j],
                                                               rac->x[i0],rac->y[i0],rac->x[i],rac->y[i],&unuseddbl);
                                    if(d4 <= equalitylimit)
                                       {
                                       xint = rap->x[j];
                                       yint = rap->y[j];
                                       raToAdd = rac;
                                       priorV = i0;
                                       nextV = i;
                                       }
                                    }
                                 }
                              }
                           if((raToAdd != NULL) && ((d1 <= equalitylimit) || (d2 <= equalitylimit) || (d3 <= equalitylimit) ||
                                       (d4 <= equalitylimit)))
                              {
                              xcoords = (double *) (malloc(SzD * (raToAdd->numverts + 1)));
                              ycoords = (double *) (malloc(SzD * (raToAdd->numverts + 1)));
                              zcoords = (double *) (malloc(SzD * (raToAdd->numverts + 1)));

                              for(k1 = 0; k1 < raToAdd->numverts; k1++)
                                 {
                                 xcoords[k1] = raToAdd->x[k1];
                                 ycoords[k1] = raToAdd->y[k1];
                                 zcoords[k1] = raToAdd->z[k1];
                                 }
                              xcoords[k1] = xint;
                              ycoords[k1] = yint;
                              zcoords[k1] = InterpLinearElev(raToAdd->x[priorV],raToAdd->y[priorV],raToAdd->z[priorV],
                                           raToAdd->x[nextV],raToAdd->y[nextV],raToAdd->z[nextV],xint,yint);

                              free(raToAdd->x);
                              free(raToAdd->y);
                              free(raToAdd->z);
                              raToAdd->x = xcoords;
                              raToAdd->y = ycoords;
                              raToAdd->z = zcoords;
                              raToAdd->firstindex = -99;
                              raToAdd->numverts += 1;
                              }
                           }
                        if(addnewpoint > 0)
                           {
                           xcoords = (double *) (malloc(SzD * (rac->numverts + 1)));
                           ycoords = (double *) (malloc(SzD * (rac->numverts + 1)));
                           zcoords = (double *) (malloc(SzD * (rac->numverts + 1)));
                           for(k1 = 0; k1 < rac->numverts; k1++)
                              {
                              xcoords[k1] = rac->x[k1];
                              ycoords[k1] = rac->y[k1];
                              zcoords[k1] = rac->z[k1];
                              }
                           xcoords[k1] = xint;
                           ycoords[k1] = yint;
                           zcoords[k1] = InterpLinearElev(rac->x[i0],rac->y[i0],rac->z[i0],
                                           rac->x[i],rac->y[i],rac->z[i],xint,yint);

                           free(rac->x);
                           free(rac->y);
                           free(rac->z);
                           rac->x = xcoords;
                           rac->y = ycoords;
                           rac->z = zcoords;
                           rac->firstindex = -99;
                           rac->numverts += 1;


                           xcoords = (double *) (malloc(SzD * (rap->numverts + 1)));
                           ycoords = (double *) (malloc(SzD * (rap->numverts + 1)));
                           zcoords = (double *) (malloc(SzD * (rap->numverts + 1)));
                           for(k1 = 0; k1 < rap->numverts; k1++)
                              {
                              xcoords[k1] = rap->x[k1];
                              ycoords[k1] = rap->y[k1];
                              zcoords[k1] = rap->z[k1];
                              }
                           xcoords[k1] = xint;
                           ycoords[k1] = yint;
                           zcoords[k1] = InterpLinearElev(rap->x[j0],rap->y[j0],rap->z[j0],
                                           rap->x[j],rap->y[j],rap->z[j],xint,yint);

                           free(rap->x);
                           free(rap->y);
                           free(rap->z);
                           rap->x = xcoords;
                           rap->y = ycoords;
                           rap->z = zcoords;
                           rap->firstindex = -99;
                           rap->numverts += 1;

                           }
                        
                     if(rap->RB_Tree_Edgelist != NULL)
                        {
                        j = GetNextArealIndex(rap, LastJ, 1, &LastJEdge, compositeindex, &UsedTree);
                        LastJ = j;
                        }
                     else
                        {
                        ++j;
                        if(j >= plimit)
                           j = -1;
                        }
                     }
                  if(rac->RB_Tree_Edgelist != NULL)
                     {
                     i = GetNextArealIndex(rac, LastI, 1, &LastIEdge, compositeindex, &UsedTree);
                     LastI = i;
                     }
                  else
                     {
                     ++i;
                     if(i >= climit)
                        i = -1;
                     }
                  }
               }
            rap = rap->next;
            }

         }
      rac = rac->next;
      }



   rpc = Proot;
   while(rpc != NULL)
      {
      if(rpc->numverts == -1)
         checkapplies = CombinedCheckApplies(rpc->idn,checkinstance,CREATENET,rpc->Lindex,rpc->Sindex,
                                          SCCtable[rpc->Sindex].C, SCCtable[rpc->Sindex].S, SCCtable[rpc->Sindex].D,
                                          &part1, &part2, &part3);
      else
         checkapplies = 0;

      if(checkapplies > 0)
         {

         rlp = *Lroot;
         rlpprev = *Lroot;

         while(rlp != NULL)
            {
            capplies2 = CombinedCheckApplies(rlp->idn,checkinstance,CREATENET,rlp->Lindex,rlp->Sindex,
                                          SCCtable[rlp->Sindex].C, SCCtable[rlp->Sindex].S, SCCtable[rlp->Sindex].D,
                                          &p1, &p2, &p3);
            if(capplies2 > 0)
               {
                  j0 = 0;
                  j = 1;
                  plimit = rlp->NumIntersections;
                  while(j < plimit)
                     {
                     d1 = PointToLineDist2D(rpc->x[0],rpc->y[0],rlp->x[j0],rlp->y[j0],rlp->x[j],rlp->y[j]);
                     if(d1 <= equalitylimit) /** then the point is on the edge ***/
                        {
                        addnewpoint = 0;
                        d2 = Distance(rpc->x[0],rpc->y[0],rlp->x[j0],rlp->y[j0]);
                        if(d2 > equalitylimit)
                           {
                           d3 = Distance(rpc->x[0],rpc->y[0],rlp->x[j],rlp->y[j]);
                          if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                              {
                              addnewpoint = 1;
                              }
                           }
                        if(addnewpoint > 0)
                           {
                           rln2 = (struct RawLinear *) (malloc(SzRL));
                           if(rln2 == NULL)
                              {
                              printf("allocation memory exhausted during linear network pseudo node creation\n");
                              printf("processing cannot continue\n");
                              exit(-1);
                              }
                           rln2->length = rlp->length;
                           rln2->badform = rlp->badform;
                           rln2->featureid = rlp->featureid;
                           rln2->numnodes = rlp->numnodes + 1;
                           rln2->Lindex = rlp->Lindex;
                           rln2->Sindex = rlp->Sindex;
                           rln2->localID = rlp->localID;
                           rln2->IsMulti = rlp->IsMulti;
                           rln2->firstencounter = 1;
                           rln2->idn = rlp->idn;
                           rln2->areaattr = rlp->areaattr;
                           rln2->hgtattr = rlp->hgtattr;
                           rln2->zv2attr = rlp->zv2attr;
                           rln2->areastr = NULL;
                           rln2->hgtstr = NULL;
                           rln2->zv2str = NULL;

                           rln2->x = (double *) malloc(SzD * (rlp->numnodes + 1));
                           rln2->y = (double *) malloc(SzD * (rlp->numnodes + 1));
                           rln2->z = (double *) malloc(SzD * (rlp->numnodes + 1));
                           k1 = 0;
                           for(k2 = 0; k2 <= j0; k2++)
                              {
                              rln2->x[k1] = rlp->x[k2];
                              rln2->y[k1] = rlp->y[k2];
                              rln2->z[k1] = rlp->z[k2];
                              ++k1;
                              }
                           rln2->x[k1] = rpc->x[0];
                           rln2->y[k1] = rpc->y[0];
                           rln2->z[k1] = InterpLinearElev(rlp->x[j0],rlp->y[j0],rlp->z[j0],
                                           rlp->x[j],rlp->y[j],rlp->z[j],rpc->x[0],rpc->y[0]);
                           ++k1;
                           for(k2 = j; k2 < rlp->numnodes; k2++)
                              {
                              rln2->x[k1] = rlp->x[k2];
                              rln2->y[k1] = rlp->y[k2];
                              rln2->z[k1] = rlp->z[k2];
                              ++k1;
                              }

                           if(rlp == *Lroot)
                              *Lroot = rln2;
                           else
                               rlpprev->next = rln2;
                           rln2->next = rlp->next;
                           rlp->next = NULL;
                           FreeLinearsList(rlp);
                           rlp = rln2;
                           }
                        }
                     j0 = j;
                     j += 1;
                     }
               }
            rlpprev = rlp;
            rlp = rlp->next;
            }




         rap = *Aroot;

         while(rap != NULL)
            {
            capplies2 = CombinedCheckApplies(rap->idn,checkinstance,CREATENET,rap->Lindex,rap->Sindex,
                                          SCCtable[rap->Sindex].C, SCCtable[rap->Sindex].S, SCCtable[rap->Sindex].D,
                                          &p1, &p2, &p3);
            if(capplies2 > 0)
               {
                  j0 = rap->coversLM-1;
                  j = 0;
                  plimit = rap->coversLM;
                  while(j < plimit)
                     {
                     d1 = PointToLineDist2D(rpc->x[0],rpc->y[0],rap->x[j0],rap->y[j0],rap->x[j],rap->y[j]);
                     if(d1 <= equalitylimit) /** then the point is on the edge ***/
                        {
                        addnewpoint = 0;
                        d2 = Distance(rpc->x[0],rpc->y[0],rap->x[j0],rap->y[j0]);
                        if(d2 > equalitylimit)
                           {
                           d3 = Distance(rpc->x[0],rpc->y[0],rap->x[j],rap->y[j]);
                           if(d3 > equalitylimit) /** check to see if [j] is a connecting node ***/
                              {
                              addnewpoint = 1;
                              }
                           }



                        if(addnewpoint > 0)
                           {
                           xcoords = (double *) (malloc(SzD * (rap->numverts + 1)));
                           ycoords = (double *) (malloc(SzD * (rap->numverts + 1)));
                           zcoords = (double *) (malloc(SzD * (rap->numverts + 1)));
                           if(j == 0) /** add node between first and closing vertices ***/
                              {
                              for(k1 = 0; k1 < rap->numverts; k1++)
                                 {
                                 xcoords[k1] = rap->x[k1];
                                 ycoords[k1] = rap->y[k1];
                                 zcoords[k1] = rap->z[k1];
                                 }
                              xcoords[k1] = rpc->x[0];
                              ycoords[k1] = rpc->y[0];
                              zcoords[k1] = InterpLinearElev(rap->x[j0],rap->y[j0],rap->z[j0],
                                           rap->x[j],rap->y[j],rap->z[j],xint,yint);
                              }
                           else
                              {
                              k1 = 0;
                              for(k2 = 0; k2 <= j0; k2++)
                                 {
                                 xcoords[k1] = rap->x[k2];
                                 ycoords[k1] = rap->y[k2];
                                 zcoords[k1] = rap->z[k2];
                                 ++k1;
                                 }
                              xcoords[k1] = rpc->x[0];
                              ycoords[k1] = rpc->y[0];
                              zcoords[k1] = InterpLinearElev(rap->x[j0],rap->y[j0],rap->z[j0],
                                           rap->x[j],rap->y[j],rap->z[j],rpc->x[0],rpc->y[0]);
                              ++k1;
                              for(k2 = j; k2 < rap->numverts; k2++)
                                 {
                                 xcoords[k1] = rap->x[k2];
                                 ycoords[k1] = rap->y[k2];
                                 zcoords[k1] = rap->z[k2];
                                 ++k1;
                                 }
                              }

                           free(rap->x);
                           free(rap->y);
                           free(rap->z);
                           rap->x = xcoords;
                           rap->y = ycoords;
                           rap->z = zcoords;
                           rap->firstindex = -99;
                           rap->numverts += 1;

                           }
                        }
                     j0 = j;
                     j += 1;
                     }
               }
            rap = rap->next;
            }

         }
      rpc = rpc->next;
      }
   return;
}

double PseudoAngle(double refx, double refy, double x, double y)
{
double answer;
double dx, dy, ax, ay;

   dx = x - refx;
   if(dx < 0)
      ax = 0 - dx;
   else
      ax = dx;

   dy = y - refy;
   if(dy < 0)
      ay = 0 - dy;
   else
      ay = dy;

   if((dx == 0) && (dy == 0))
      answer = 0;
   else
      answer = dy / (ax + ay);

   if(dx < 0)
      answer = 2.0 - answer;
   else  if(dy < 0)
      answer = 4.0 + answer;

   answer = answer * 90.0;

   return(answer);
}


int TwoPointsOnSameSideOfLine(double px1, double py1, double px2, double py2,
        double lx1, double ly1, double lx2, double ly2)
{
int answer;
double dx, dy, dx1, dy1, dx2, dy2, temp;

   dx = lx2 - lx1;
   dy = ly2 - ly1;
   dx1 = px1 - lx1;
   dy1 = py1 - ly1;
   dx2 = px2 - lx2;
   dy2 = py2 - ly2;

   temp = ((dx * dy1) - (dy * dx1)) * ((dx * dy2) - (dy * dx2));
   if(temp <= 0)
      answer = 0;
   else //if(temp >= 0)
      answer = 1;

   return(answer);
}


int IntersectAcrossConnectingNode(double x1, double y1, double x2, double y2, double x3, double y3,
                double x4, double y4, double x5, double y5, double x6, double y6)
{
double A, B, C;
double calcy;
double xint, yint, ptpdist;
int answer = 0;
int p1,p2;
   
   ptpdist = Distance(x1,y1,x4,y4);
   if(ptpdist < 0.0001)
      return(answer);
   ptpdist = Distance(x1,y1,x6,y6);
   if(ptpdist < 0.0001)
      return(answer);
   ptpdist = Distance(x3,y3,x4,y4);
   if(ptpdist < 0.0001)
      return(answer);
   ptpdist = Distance(x3,y3,x6,y6);
   if(ptpdist < 0.0001)
      return(answer);

   LineEquation(x1,y1,x3,y3,&A,&B,&C);
   calcy = DblSolveForY(x2,A,B,C);
   if(calcy > y2)
      calcy = calcy - y2;
   else
      calcy = y2 - calcy;
   if(calcy < 0.1) /** consider the 3 points to be colinear ***/
      {
      if(TwoPointsOnSameSideOfLine(x4,y4,x6,y6,  x1,y1,x3,y3) == 0)
         answer = 1;
      }
   else
      {
      p1 = PointInsideTriangle(x4,y4,  x1,y1,x2,y2,x3,y3);
      if(p1 == 0)
         {
         if(LineSegmentsIntersect(x4,y4,x5,y5,x1,y1,x3,y3,&xint, &yint) > 0)
            p1 = 1;
         }
      p2 = PointInsideTriangle(x6,y6,  x1,y1,x2,y2,x3,y3);
      if(p2 == 0)
         {
         if(LineSegmentsIntersect(x6,y6,x5,y5,x1,y1,x3,y3,&xint, &yint) > 0)
            p2 = 1;
         }
      p1 = p1 + p2;
      if(p1 == 1)
         answer = 1;
      }
   return(answer);
}


int AllCaseLineSegmentsIntersect(double x1, double y1, double x2, double y2,
                               double x3, double y3, double x4, double y4, double tolerance, double *xi, double *yi)
{
int answer = 0;
int on1,on2,on3,on4;
int con13, con14, con23, con24;
double x12,y12,x43,y43,x32,y32,s,t,det;

   on1 = on2 = on3 = on4 = 0;
   con13 = con14 = con23 = con24 = 0;
   if(EqualWithinTolerance(x1,y1,x3,y3,tolerance) > 0)
      {
      con13 = 1;
      }
   else if(EqualWithinTolerance(x1,y1,x4,y4,tolerance) > 0)
      {
      con14 = 1;
      }
   else if(EqualWithinTolerance(x2,y2,x3,y3,tolerance) > 0)
      {
      con23 = 1;
      }
   else if(EqualWithinTolerance(x2,y2,x4,y4,tolerance) > 0)
      {
      con24 = 1;
      }

   if(PointOnLineSegment(x1,y1,x3,y3,x4,y4,tolerance) > 0)
      on1 = 1;
   if(PointOnLineSegment(x2,y2,x3,y3,x4,y4,tolerance) > 0)
      on2 = 1;
   if((on1 > 0) && (on2 > 0)) 
      return(0);
   if(PointOnLineSegment(x3,y3,x1,y1,x2,y2,tolerance) > 0) 
      on3 = 1;
   if((on3 > 0) && (on1 > 0) && (con13 == 0))
      return(0);
   if((on3 > 0) && (on2 > 0) && (con23 == 0)) 
      return(0);
   if(PointOnLineSegment(x4,y4,x1,y1,x2,y2,tolerance) > 0)
      on4 = 1;
   if((on4 > 0) && (on1 > 0) && (con14 == 0))
      return(0);
   if((on4 > 0) && (on2 > 0) && (con24 == 0))
      return(0);

   if(con13 > 0)
      {
      answer = 2;
      *xi = x1;
      *yi = y1;
      }
   else if(con14 > 0)
      {
      answer = 3;
      *xi = x1;
      *yi = y1;
      }
   else if(con23 > 0)
      {
      answer = 4;
      *xi = x2;
      *yi = y2;
      }
   else if(con24 > 0)
      {
      answer = 5;
      *xi = x2;
      *yi = y2;
      }
   else if(on1 > 0)
      {
      answer = 6;
      *xi = x1;
      *yi = y1;
      }
   else if(on2 > 0)
      {
      answer = 7;
      *xi = x2;
      *yi = y2;
      }
   else if(on3 > 0)
      {
      answer = 8;
      *xi = x3;
      *yi = y3;
      }
   else if(on4 > 0)
      {
      answer = 9;
      *xi = x4;
      *yi = y4;
      }
   else 
      {
      x12 = x1 - x2;
      y12 = y1 - y2;
      x43 = x4 - x3;
      y43 = y4 - y3;
      x32 = x3 - x2;
      y32 = y3 - y2;

      det = (x43 * y12) - (y43 * x12);
      if((-0.00001 < det) && (det < 0.00001))  /** parallel lines **/
         {
         answer = 0;
         }
      else
         {
         det = 1.0 / det;
         s = ((x43 * y32) - (y43 * x32)) * det;
         if((s < 0.0) || (s > 1.0))
            {
            answer = 0;
            }
         else
            {
            t = (x12 * y32 - y12 * x32) * det;
            if((t < 0.0) || (t > 1.0))
               {
               answer = 0;
               }
            else
               {
               *xi = x2 + x12 * s;
               *yi = y2 + y12 * s;
               answer = 1;
               }
            }
         }
      }
   return(answer);
}


int ReCheckAtJoin(struct RawLinear *root, int Check, int checkinstance,struct RawLinear *rlc,
             int ii,int i,struct RawLinear *rln,int jj2,int jj,int j)
{
struct RawLinear * rl;
int k, k2, on;
int answer = 0;
int part1, part2, part3;

   if(jj2 < 0)
      jj2 = j;
   if(jj2 >= rln->numnodes)
      jj2 = j;

   if(j < 0)
      j = jj2;
   if(j >= rln->numnodes)
      j = jj2;

   rl = root;
   while(rl != NULL)
      {
      if((rl != rlc) && (rl != rln))
         {
         if(CombinedCheckApplies(rl->idn,checkinstance,Check,rl->Lindex,rl->Sindex,
             SCCtable[rl->Sindex].C, SCCtable[rl->Sindex].S, SCCtable[rl->Sindex].D, &part1,&part2,&part3) > 0)
            {
            if(part1 > 0)
               {
               for(k=0; k<rl->numnodes; k++)
                  {
                  if(EqualWithinTolerance(rl->x[k],rl->y[k],rlc->x[ii],rlc->y[ii],0.0001) > 0)
                     {
                     if((k == 0) || (k == (rl->numnodes - 1)))
                        {
                        if(k == 0)
                           k2 = 1;
                        else
                           k2 = k - 1;
                        on = 0;
                        on = TwoPointsOnSameSideOfLine(rlc->x[i],rlc->y[i],rl->x[k2],rl->y[k2],
                                                             rln->x[jj],rln->y[jj],rln->x[j],rln->y[j]);
                        if(on == 0)
                           {
                           on = TwoPointsOnSameSideOfLine(rlc->x[i],rlc->y[i],rl->x[k2],rl->y[k2],
                                                             rln->x[jj],rln->y[jj],rln->x[jj2],rln->y[jj2]);
                           if(on == 0)
                              {
                              if(jj2 != j)
                                 on = TwoPointsOnSameSideOfLine(rln->x[j],rln->y[j],rln->x[jj2],rln->y[jj2],
                                                                rlc->x[ii],rlc->y[ii],rl->x[k2],rl->y[k2]);
                              if(on == 0)
                                 {
                                 if(jj2 != j)
                                    on = TwoPointsOnSameSideOfLine(rln->x[j],rln->y[j],rln->x[jj2],rln->y[jj2],
                                                                rlc->x[ii],rlc->y[ii],rl->x[i],rl->y[i]);
                                 if(on == 0)
                                    answer = 1;
                                 }
                              }
                           }
                        }
                     else 
                        {
                        k2 = k - 1;
                        on = 0;
                        on = TwoPointsOnSameSideOfLine(rlc->x[i],rlc->y[i],rl->x[k2],rl->y[k2],
                                                             rln->x[jj],rln->y[jj],rln->x[j],rln->y[j]);
                        if(on == 0)
                           {
                           on = TwoPointsOnSameSideOfLine(rlc->x[i],rlc->y[i],rl->x[k2],rl->y[k2],
                                                             rln->x[jj],rln->y[jj],rln->x[jj2],rln->y[jj2]);
                           if(on == 0)
                              {
                              if(jj2 != j)
                                 on = TwoPointsOnSameSideOfLine(rln->x[j],rln->y[j],rln->x[jj2],rln->y[jj2],
                                                                rlc->x[ii],rlc->y[ii],rl->x[k2],rl->y[k2]);
                              if(on == 0)
                                 {
                                 if(jj2 != j)
                                    on = TwoPointsOnSameSideOfLine(rln->x[j],rln->y[j],rln->x[jj2],rln->y[jj2],
                                                                rlc->x[ii],rlc->y[ii],rl->x[i],rl->y[i]);
                                 if(on == 0)
                                    answer = 1;
                                 }
                              }
                           }
                        if(on > 0)
                           {
                           k2 = k + 1;
                           on = 0;
                           on = TwoPointsOnSameSideOfLine(rlc->x[i],rlc->y[i],rl->x[k2],rl->y[k2],
                                                             rln->x[jj],rln->y[jj],rln->x[j],rln->y[j]);
                           if(on == 0)
                              {
                              on = TwoPointsOnSameSideOfLine(rlc->x[i],rlc->y[i],rl->x[k2],rl->y[k2],
                                                             rln->x[jj],rln->y[jj],rln->x[jj2],rln->y[jj2]);
                              if(on == 0)
                                 {
                                 if(jj2 != j)
                                    on = TwoPointsOnSameSideOfLine(rln->x[j],rln->y[j],rln->x[jj2],rln->y[jj2],
                                                                rlc->x[ii],rlc->y[ii],rl->x[k2],rl->y[k2]);
                                 if(on == 0)
                                    {
                                    if(jj2 != j)
                                       on = TwoPointsOnSameSideOfLine(rln->x[j],rln->y[j],rln->x[jj2],rln->y[jj2],
                                                                rlc->x[ii],rlc->y[ii],rl->x[i],rl->y[i]);
                                    if(on == 0)
                                       answer = 1;
                                    }
                                 }
                              }
                           }
                        }
                     }
                  if(answer > 0)
                     break;
                  }
               }
            }
         }
      if(answer > 0)
         break;
      rl = rl->next;
      }

   return(answer);
}



int GrahamScan(double *x, double *y, double *z, int numpoints)
{
int i, oi, j, k, minindex, M, sameside, *OnHull;
int maxXindex,minXindex,maxYindex;
int numonhull;
double unuseddbl, ud2, ud1;
double MinMaxX[4], MinMaxY[4];
struct HullSort
   {
   double x, y, z, temp;
   struct HullSort * next;
   } *Hroot, *Hc, *Hp, *Hn;
int SzH = sizeof(struct HullSort);

   if(numpoints < 4)  /** a triangle is already the convex hull of itself ***/
      return(3);

   minindex = 0;
   minXindex = maxXindex = maxYindex = 0;
   for(i=0; i<numpoints; i++)
      {
      if(y[i] < y[minindex])
         minindex = i;
      else if(y[i] == y[minindex])
         {
         if(x[i] >= x[minindex])
            minindex = i;
         }
      if(y[i] > y[maxYindex])
         maxYindex = i;
      else if(y[i] == y[maxYindex])
         {
         if(x[i] >= x[maxYindex])
            maxYindex = i;
         }
      if(x[i] < x[minXindex])
         minXindex = i;
      else if(x[i] == x[minXindex])
         {
         if(y[i] <= y[minXindex])
            minXindex = i;
         }
      if(x[i] > x[maxXindex])
         maxXindex = i;
      else if(x[i] == x[maxXindex])
         {
         if(y[i] <= y[maxXindex])
            maxXindex = i;
         }
      }

   MinMaxX[0] = x[minindex];  MinMaxY[0] = y[minindex];
   MinMaxX[1] = x[maxXindex];  MinMaxY[1] = y[maxXindex];
   MinMaxX[2] = x[maxYindex];  MinMaxY[2] = y[maxYindex];
   MinMaxX[3] = x[minXindex];  MinMaxY[3] = y[minXindex];

   OnHull = NULL;

   Hroot = NULL;
      Hn = (struct HullSort *) (malloc(SzH));
      Hn->temp = 0.0;
      Hn->x = x[minindex];
      Hn->y = y[minindex];
      Hn->z = z[minindex];
      Hn->next = NULL;
   Hroot = Hn;

   for(i=0; i<numpoints; i++)
      {
      if(i != minindex)
         {
         Hn = (struct HullSort *) (malloc(SzH));
         unuseddbl = PseudoAngle(x[minindex],y[minindex],x[i],y[i]);
         Hn->temp = unuseddbl;
         Hn->x = x[i];
         Hn->y = y[i]; 
         Hn->z = z[i];
         Hn->next = NULL;
         if(Hroot == NULL)
            Hroot = Hn;
         else
            {
            Hp = Hroot;
            Hc = Hroot->next;
            while((Hc != NULL) && (Hc->temp < Hn->temp))
               {
               Hp = Hc;
               Hc = Hc->next;
               if(Hc == NULL)
                  break;
               }
            if(Hc == NULL)
               Hp->next = Hn;
            else if(Hc == Hroot)
               {
               Hn->next = Hroot;
               Hroot = Hn;
               }
            else
               {
               if((Hc != NULL) && (Hn->temp > 179.999) && (Hn->temp < 180.001) &&
                           (Hc->temp > 179.999) && (Hc->temp < 180.001))
                  {
                  ud1 = Hn->x - x[minindex];
                  if(ud1 < 0)
                     ud1 = 0 - ud1;
                  ud2 = Hc->x - x[minindex];
                  if(ud2 < 0)
                     ud2 = 0 - ud2;
                  if(ud1 > ud2)
                     {
                     Hc->x = Hn->x;
                     Hc->temp = Hn->temp;
                     free(Hn);
                     }
                  else
                     {
                     free(Hn);
                     }
                  }
               else if((Hc != NULL) && (Hn->temp > 89.999) && (Hn->temp < 90.001) &&
                           (Hc->temp > 89.999) && (Hc->temp < 90.001))
                  {
                  ud1 = Hn->y - y[minindex];
                  if(ud1 < 0)
                     ud1 = 0 - ud1;
                  ud2 = Hc->y - y[minindex];
                  if(ud2 < 0)
                     ud2 = 0 - ud2;
                  if(ud1 > ud2)
                     {
                     Hc->y = Hn->y;
                     Hc->temp = Hn->temp;
                     free(Hn);
                     }
                  else
                     {
                     free(Hn);
                     }
                  }
               else
                  {
                  Hp->next = Hn;
                  Hn->next = Hc;
                  }
               }
            }
         }
      }

   Hp = Hroot;
   Hc = Hroot->next;
   while(Hc->next != NULL)
      {
      Hn = Hc->next;
      if((Hp->x == Hc->x) && (Hn->x == Hc->x))
         {
         Hp->next = Hn;
         free(Hc);
         Hc = Hn;
         }
      else if((Hp->y == Hc->y) && (Hn->y == Hc->y))
         {
         Hp->next = Hn;
         free(Hc);
         Hc = Hn;
         }
      else
         {
         Hp = Hc;
         Hc = Hc->next;
         }
      }


/** now eliminate points that can't be on the hull - to start, copy ordered pts back into original x,y,z arrays***/
   Hc = Hroot;
   i = 0;
   while(Hc != NULL)
      {
      x[i] = Hc->x;
      y[i] = Hc->y;
      z[i] = Hc->z;
      ++i;
      Hp = Hc;
      Hc = Hc->next;
      free(Hp);
      }
   Hroot = NULL;

   numpoints = i;

   minindex = 0;
   minXindex = maxXindex = maxYindex = 0;
   for(i=0; i<numpoints; i++)
      {
      if(y[i] < y[minindex])
         minindex = i;
      else if(y[i] == y[minindex])
         {
         if(x[i] >= x[minindex])
            minindex = i;
         }
      if(y[i] > y[maxYindex])
         maxYindex = i;
      else if(y[i] == y[maxYindex])
         {
         if(x[i] >= x[maxYindex])
            maxYindex = i;
         }
      if(x[i] < x[minXindex])
         minXindex = i;
      else if(x[i] == x[minXindex])
         {
         if(y[i] <= y[minXindex])
            minXindex = i;
         }
      if(x[i] > x[maxXindex])
         maxXindex = i;
      else if(x[i] == x[maxXindex])
         {
         if(y[i] <= y[maxXindex])
            maxXindex = i;
         }
      }


   Hroot = NULL;
   OnHull = (int *) (malloc(SzI * numpoints));
   for(i=0; i<numpoints; i++)
      OnHull[i] = 1; /** initialize so all pts considered to be on the hull **/
   for(i=3; i< numpoints; i++)
      {
      M = i - 1;
      while(M > 1)
         {
         sameside = TwoPointsOnSameSideOfLine(x[0],y[0],x[i],y[i], /* these are the points */
                        x[M],y[M],x[M-1],y[M-1]); /* these define the line **/
         if(sameside < 1)
            OnHull[M] = 0;
         --M;
         }
      }

   for(i=0; i<numpoints; i++)
      {
      if((i == minindex) || (i == maxYindex) || (i == minXindex) || (i == maxXindex))
         {
         OnHull[i] = 1;
         }
      else if(OnHull[i] > 0)
         {
         M = PointInsidePoly(x[i],y[i],MinMaxX,MinMaxY,4);
         if(M > 0)
            OnHull[i] = 0;
         }
      }

   numonhull = 1;
   i = 1;
   oi = 1;
   while(i < numpoints)
      {
      if(OnHull[i] > 0)
         {
         ++numonhull;
         x[oi] = x[i];
         y[oi] = y[i];
         z[oi] = z[i];
         ++oi;
         ++i;
         }
      else
         {
         j = i;
         while(OnHull[j] < 1)
            {
            ++j;
            if(j >= numpoints)
               break;
            }
         if(j < numpoints)
            {
            x[oi] = x[j];
            y[oi] = y[j];
            z[oi] = z[j];
            ++numonhull;
            ++oi;
            i = j + 1;
            }
         else
            break;
         }
      }

/** now one last check to clean up any numerical issues given the high symmetry of grid points ***/
   for(i=0; i<numonhull; i++)
      OnHull[i] = 1;
   for(i=0; i< numonhull; i++)
      {
      j = i + 1;
      k = i + 2;
      oi = j;
      if(j < numonhull)
         ud1 = PseudoAngle(x[i],y[i],x[j],y[j]);
      while(k < numonhull)
         {
         ud2 = PseudoAngle(x[i],y[i],x[k],y[k]);
         if(ud2 < ud1)
            {
            ud1 = ud2;
            oi = k;
            }
         ++k;
         }
      if(j != oi)
         {
         for(k = j; k<oi; k++)
            OnHull[k] = 0;
         }
      }
   numpoints = numonhull;
   numonhull = 1;
   i = 1;
   oi = 1;
   while(i < numpoints)
      {
      if(OnHull[i] > 0)
         {
         ++numonhull;
         x[oi] = x[i];
         y[oi] = y[i];
         z[oi] = z[i];
         ++oi;
         ++i;
         }
      else
         {
         j = i;
         while(OnHull[j] < 1)
            {
            ++j;
            if(j >= numpoints)
               break;
            }
         if(j < numpoints)
            {
            x[oi] = x[j];
            y[oi] = y[j];
            z[oi] = z[j];
            ++numonhull;
            ++oi;
            i = j + 1;
            }
         else
            break;
         }
      }


      
   if(OnHull != NULL)
      free(OnHull);
   return(numonhull);
}




int BetterBreaklineExists(double * x, double * y, int CheckInstance)
{
int GI;
int index1, index2, compositeindex, GridFileInUse;
int i, j, kx, ky, kk, ii2, part1, part2, part3, iX, iY, retval, bestX, bestY,Xindex, Yindex;
int rows, cols, Xextent, Yextent;
double MinMX,MinMY,MaxMX,MaxMY, tx,ty, td, BaseDist, NewDist;
/***/double A, B, C, BaseXCoord, BaseYCoord, tdx; /***/
double AltX, AltY;
char readC[1000];
char tgt, ntgt;
int WaveX[2000];
int WaveY[2000];
int NowAt,LastAdd, xincr1,xincr2,yincr1,yincr2;
FILE * Mask;
struct Mvalues
   {
   char MaskCol[305];
   } MaskRow[305];



   for(i=0; i< 2000; i++)
      {
      WaveX[i] = WaveY[i] = -1000;
      }
   for(i=0; i<305; i++)
      {
      for(j=0; j<305; j++)
         {
         MaskRow[i].MaskCol[j] = 10;
         }
      }
   index1 = (int) (x[0] / RegionSize);
   index2 = (int) (y[0] / RegionSize);
   compositeindex = (index1 * (MaxYindex + 1)) + index2;

   GridFileInUse = -10;
   Mask = NULL;
   retval = 0;
   tx = x[1] - x[0];
   ty = y[1] - y[0];
   NewDist = BaseDist = (tx * tx) + (ty * ty);
   AltX = x[0];
   AltY = y[0];
   for(GI=0; GI < NGT[compositeindex].NumGrids; GI++)
      {
      ii2 = NGT[compositeindex].GridIndex[GI];
      if((CombinedCheckApplies(GridHeader[ii2].idn,CheckInstance,BREAKLINE,GridHeader[ii2].Lindex,
            GridHeader[ii2].Sindex,C_GRID /** config must be grid **/,
            SCCtable[GridHeader[ii2].Sindex].S,SCCtable[GridHeader[ii2].Sindex].D,
            &part1,&part2,&part3)) && (part1 > 0))
         {
         if(GridHeader[ii2].BPGV != 1)
            {
            printf("\nbad file type in WBM file - expected single byte value per post\n");
            printf("analysis of River elevation break line  orientation will not be applied\n\n");
            return(0);
            }
         Mask = GlobalUseOpenGridFile(NGT[compositeindex].FileNumber[GI]);
         if(Mask == NULL)
            return(0);
         MinMX = x[0] - (100.0 * GridHeader[ii2].Xspacing);
         if(MinMX < GridHeader[ii2].BaseX)
            MinMX = GridHeader[ii2].BaseX;
         Xindex = (int) ((x[0] - MinMX) / GridHeader[ii2].Xspacing);
         MaxMX = x[0] + (100.0 * GridHeader[ii2].Xspacing);
         if(MaxMX > (GridHeader[ii2].BaseX + (GridHeader[ii2].Xspacing * (double) GridHeader[ii2].Xpts)))
            MaxMX = GridHeader[ii2].BaseX + (GridHeader[ii2].Xspacing * (double) GridHeader[ii2].Xpts);
         MinMY = y[0] - (100.0 * GridHeader[ii2].Yspacing);
         if(MinMY < GridHeader[ii2].BaseY)
            MinMY = GridHeader[ii2].BaseY;
         Yindex = (int) ((y[0] - MinMY) / GridHeader[ii2].Yspacing);
         MaxMY = y[0] + (100.0 * GridHeader[ii2].Yspacing);
         if(MaxMY > (GridHeader[ii2].BaseY + (GridHeader[ii2].Yspacing * (double) GridHeader[ii2].Ypts)))
            MaxMY = GridHeader[ii2].BaseY + (GridHeader[ii2].Yspacing * (double) GridHeader[ii2].Ypts);

         Xextent = (int) ((MaxMX - MinMX) / GridHeader[ii2].Xspacing);
         Yextent = (int) ((MaxMY - MinMY) / GridHeader[ii2].Yspacing);
         if((Xextent > 200) || (Yextent > 200))
            {
            printf("\n\nbad extents for mass read %d %d\n",Xextent,Yextent);
            if(Mask != NULL)
               fclose(Mask);
            Mask = NULL;
            return(0);
            }

         iX = (int) ((MinMX / GridHeader[ii2].Xspacing) + 0.5);
         iY = (int) ((MaxMY / GridHeader[ii2].Yspacing) + 0.5);

         for(rows = 0; rows < Yextent; rows++)
            {
            MassReadCharGridValues(Mask,NGT[0].fileptr[GI], iX, iY - rows,
                        GridHeader[ii2].Xpts,GridHeader[ii2].Ypts,readC,Xextent);
            for(cols = 0; cols < Xextent; cols++)
               MaskRow[rows].MaskCol[cols] = readC[cols];
            }
         bestX = bestY = 1500;
         if(MaskRow[Yindex].MaskCol[Xindex] == 0)
            tgt = 3;
         else
            tgt = 0;
         AltX = x[0];
         AltY = y[0];
         for(kk = 1; kk < 50; kk++)
            {
            if((kk > Yindex) || (kk > Xindex))
               break;
            if(((Xindex + kk) > Xextent) || ((Yindex + kk) > Yextent))
               break;
            if(MaskRow[Yindex].MaskCol[Xindex + kk] == tgt)
               {
               bestX = Xindex + kk;
               bestY = Yindex;
               break;
               }
            if(MaskRow[Yindex].MaskCol[Xindex - kk] == tgt)
               {
               bestX = Xindex - kk;
               bestY = Yindex;
               break;
               }
            if(MaskRow[Yindex + kk].MaskCol[Xindex] == tgt)
               {
               bestX = Xindex;
               bestY = Yindex + kk;
               break;
               }
            if(MaskRow[Yindex - kk].MaskCol[Xindex] == tgt)
               {
               bestX = Xindex;
               bestY = Yindex - kk;
               break;
               }
            if(MaskRow[Yindex + kk].MaskCol[Xindex + kk] == tgt)
               {
               bestX = Xindex + kk;
               bestY = Yindex + kk;
               break;
               }
            if(MaskRow[Yindex + kk].MaskCol[Xindex - kk] == tgt)
               {
               bestX = Xindex - kk;
               bestY = Yindex + kk;
               break;
               }
            if(MaskRow[Yindex - kk].MaskCol[Xindex + kk] == tgt)
               {
               bestX = Xindex + kk;
               bestY = Yindex - kk;
               break;
               }
            if(MaskRow[Yindex - kk].MaskCol[Xindex - kk] == tgt)
               {
               bestX = Xindex - kk;
               bestY = Yindex - kk;
               break;
               }
            }

         if((bestX > 200) || (bestY > 200))
            {
            printf("\n\navoiding out of bounds array assignment\n\n");
            if(Mask != NULL)
               fclose(Mask);
            Mask = NULL;
            return(0);
            }
         MaskRow[Yindex].MaskCol[Xindex] = 10;
         if(MaskRow[bestY].MaskCol[bestX] == 3)
            ntgt = 0;
         else
            ntgt = 3;

         WaveX[0] = bestX;
         WaveY[0] = bestY;
         NowAt = 0;
         LastAdd = 1;
         do
            {
            for(i=1; i<9; i++)
               {
               switch(i)
                  {
                  case 1: xincr1 = 0; yincr1 = 1; break;
                  case 2: xincr1 = 1; yincr1 = 1; break;
                  case 3: xincr1 = 1; yincr1 = 0; break;
                  case 4: xincr1 = 1; yincr1 = -1; break;
                  case 5: xincr1 = 0; yincr1 = -1; break;
                  case 6: xincr1 = -1; yincr1 = -1; break;
                  case 7: xincr1 = -1; yincr1 = 0; break;
                  case 8: xincr1 = -1; yincr1 = 1; break;
                  default: break;
                  }
               if(((WaveY[NowAt] + yincr1) < 200) && ((WaveY[NowAt] +yincr1) >= 0) &&
                  ((WaveX[NowAt] + xincr1) < 200) && ((WaveX[NowAt] +xincr1) >= 0) &&
                       (MaskRow[WaveY[NowAt] + yincr1].MaskCol[WaveX[NowAt] + xincr1] == tgt))
                  {
                  for(kk = 1; kk < 9; kk++)
                     {
                     switch(kk)
                         {
                         case 1: xincr2 = 0; yincr2 = 1; break;
                         case 2: xincr2 = 1; yincr2 = 1; break;
                         case 3: xincr2 = 1; yincr2 = 0; break;
                         case 4: xincr2 = 1; yincr2 = -1; break;
                         case 5: xincr2 = 0; yincr2 = -1; break;
                         case 6: xincr2 = -1; yincr2 = -1; break;
                         case 7: xincr2 = -1; yincr2 = 0; break;
                         case 8: xincr2 = -1; yincr2 = 1; break;
                         default: break;
                         }
                     kx = WaveX[NowAt] + xincr1;
                     ky = WaveY[NowAt] + yincr1;
                     if(((ky + yincr2) < 200) && ((ky + yincr2) >= 0) &&
                         ((kx + xincr2) < 200) && ((kx + xincr2) >= 0) &&
                           (MaskRow[WaveY[NowAt] + yincr1 + yincr2].MaskCol[WaveX[NowAt] + xincr1 + xincr2]== ntgt))
                         
                        {
                        WaveX[LastAdd] = WaveX[NowAt] + xincr1;
                        WaveY[LastAdd] = WaveY[NowAt] + yincr1;
                        tx = MinMX + (WaveX[LastAdd] * GridHeader[ii2].Xspacing);
                        ty = MaxMY - (WaveY[LastAdd] * GridHeader[ii2].Yspacing);
                        tx = x[1] - tx;
                        ty = y[1] - ty;
                        td = (tx * tx) + (ty * ty);
                        if(td < NewDist)
                           {
                           NewDist = td;
                           AltX = MinMX + (WaveX[LastAdd] * GridHeader[ii2].Xspacing);
                           AltY = MaxMY - (WaveY[LastAdd] * GridHeader[ii2].Yspacing);
                           }
                        MaskRow[WaveY[NowAt] + yincr1].MaskCol[WaveX[NowAt] + xincr1] = 10;
                        if(LastAdd < 2000)
                           ++LastAdd;
                        break;
                        }
                     }
                  }
               }
            ++NowAt;
            } while((LastAdd < 2000) && (NowAt < LastAdd));
         break;
         }
      }
   if(Mask != NULL)
      fclose(Mask);
   Mask = NULL;
   td = sqrt(BaseDist) - sqrt(NewDist);
   if(td > 10)
      {
      retval = 1;
      x[2] = AltX;
      y[2] = AltY;

/*** could an island be in the way of a proposed alternate solution ? ***/
      LineEquation(x[1],y[1],x[2],y[2],&A,&B,&C);
      if(x[2] > x[1])
         {
         tdx = x[2] - x[1];
         }
      else
         {
         tdx = x[1] - x[2];
         }
      if(y[2] > y[1])
         {
         td = y[2] - y[1];
         }
      else
         {
         td = y[1] - y[2];
         }
      if(tdx > td)
         {
         BaseXCoord = x[1]; BaseYCoord = y[1];
         if(x[2] > x[1])
            {
            for(tx = (4.0 * GridHeader[ii2].Xspacing); tx < tdx; tx += GridHeader[ii2].Xspacing)
               {
               ty = DblSolveForY(BaseXCoord + tx,A,B,C);
               Yindex = (int) ((MaxMY - ty) / GridHeader[ii2].Yspacing);
               Xindex = (int) ((BaseXCoord + tx - MinMX) / GridHeader[ii2].Xspacing);
               if(MaskRow[Yindex].MaskCol[Xindex] == 0)
                  {
                  AltX = BaseXCoord + tx;
                  AltY = ty;
                  break;
                  }
               }
            }
         else
            {
            for(tx = (4.0 * GridHeader[ii2].Xspacing); tx < tdx; tx += GridHeader[ii2].Xspacing)
               {
               ty = DblSolveForY(BaseXCoord - tx,A,B,C);
               Yindex = (int) ((MaxMY - ty) / GridHeader[ii2].Yspacing);
               Xindex = (int) ((BaseXCoord - tx - MinMX) / GridHeader[ii2].Xspacing);
               if(MaskRow[Yindex].MaskCol[Xindex] == 0)
                  {
                  AltX = BaseXCoord - tx;
                  AltY = ty;
                  break;
                  }
               }
            }
         }
      else
         {
         BaseXCoord = x[1]; BaseYCoord = y[1];
         if(y[2] > y[1])
            {
            for(ty = (4.0 * GridHeader[ii2].Yspacing); ty < td; ty += GridHeader[ii2].Yspacing)
               {
               tx = DblSolveForY(BaseYCoord + ty,B,A,C);
               Yindex = (int) ((MaxMY - BaseYCoord - ty) / GridHeader[ii2].Yspacing);
               Xindex = (int) ((tx - MinMX) / GridHeader[ii2].Xspacing);
               if(MaskRow[Yindex].MaskCol[Xindex] == 0)
                  {
                  AltX = tx;
                  AltY = BaseYCoord + ty;
                  break;
                  }
               }
            }
         else
            {
            for(ty = (4.0 * GridHeader[ii2].Yspacing); ty < td; ty += GridHeader[ii2].Yspacing)
               {
               tx = DblSolveForY(BaseYCoord - ty,B,A,C);
               Yindex = (int) ((MaxMY - BaseYCoord + ty) / GridHeader[ii2].Yspacing);
               Xindex = (int) ((tx - MinMX) / GridHeader[ii2].Xspacing);
               if(MaskRow[Yindex].MaskCol[Xindex] == 0)
                  {
                  AltX = tx;
                  AltY = BaseYCoord - ty;
                  break;
                  }
               }
            }
         }
      x[2] = AltX;
      y[2] = AltY;
      }

   return(retval);
}


int LeastSquaresFit(double *x, double * y, int numverts, double xspace, double yspace, int CheckInstance, double *mag)
{
double xsum, xsquaresum, ysquaresum, ysum, xysum;
double common, nv, A, B, angle;
double x1, x2, y1, y2, xdif, ydif, dist1, dist2;
double altx[15];
double alty[15];
int i;

   xsum = xsquaresum = ysquaresum = ysum = xysum = 0.0;
   x1 = x2 = x[0];
   y1 = y2 = y[0];
   for(i=0; i< numverts; i++)
      {
      if(x[i] < x1)
         x1 = x[i];
      if(x[i] > x2)
         x2 = x[i];

      if(y[i] < y1)
         y1 = y[i];
      if(y[i] > y2)
         y2 = y[i];

      xsum += x[i];
      ysum += y[i];
      xsquaresum += (x[i] * x[i]);
      ysquaresum += (y[i] * y[i]);
      xysum += (x[i] * y[i]);
      }

   xdif = (x2 - x1) / xspace;
   ydif = (y2 - y1) / yspace;

   if((ydif < 10) && (xdif < 10))
      return(0);

   if(xdif > ydif)
      {
      common = xsum / ((double) numverts);
      nv = (double) numverts;

      B = xysum - (ysum * common);
      B = B / (xsquaresum - (xsum * common));

      A = ysum - (xsum * B);
      A = A / nv;

      y1 = A + (B * x1);
      y2 = A + (B * x2);
      }
   else 
      {
/***  then flip the roles played by x and y ***/
      common = ysum / ((double) numverts);
      nv = (double) numverts;

      B = xysum - (xsum * common);
      B = B / (ysquaresum - (ysum * common));

      A = xsum - (ysum * B);
      A = A / nv;

      x1 = A + (B * y1);
      x2 = A + (B * y2);
      }

   x[0] = x1; y[0] = y1;
   x[1] = x2; y[1] = y2;
   altx[0] = x[1];  altx[1] = x[0];
   alty[0] = y[1];  alty[1] = y[0];

   if(BetterBreaklineExists(x, y, CheckInstance) > 0)
      {
      dist1 = Distance(x[0],y[0],x[2],y[2]);
      if(BetterBreaklineExists(altx, alty, CheckInstance) > 0)
         {
         dist2 = Distance(altx[0], alty[0], altx[2], alty[2]);
         if(dist2 > dist1)
            {
            x[0] = altx[0]; y[0] = alty[0];
            x[1] = altx[1]; y[1] = alty[1];
            x[2] = altx[2]; y[2] = alty[2];
            }
         }
      angle = AngleBetweenLineSegments(x[0],y[0],x[1],y[1],x[1],y[1],x[2],y[2]);
      angle = RadiansToDegrees(acos(angle));
      if(angle > 90.0)
         angle = 180.0 - angle;
      *mag = angle;
      if(angle >= ActiveChecks[CheckInstance].sensitivity2)
         return(1);
      else
         return(0);
      }
   else
      {
      if(BetterBreaklineExists(altx, alty, CheckInstance) > 0)
         {
         angle = AngleBetweenLineSegments(altx[0],alty[0],altx[1],alty[1],altx[1],alty[1],altx[2],alty[2]);
         angle = RadiansToDegrees(acos(angle));
         if(angle > 90.0)
            angle = 180.0 - angle;
         *mag = angle;
         if(angle >= ActiveChecks[CheckInstance].sensitivity2)
            {
            x[0] = altx[0]; y[0] = alty[0];
            x[1] = altx[1]; y[1] = alty[1];
            x[2] = altx[2]; y[2] = alty[2];
            return(1);
            }
         else
            return(0);
         }
      else
         return(0);
      }
}



int RemoveColinearVertices(double *x, double * y, double *z, int numverts)
{
int i,j,i2;
double *xn, *yn, *zn;

  xn = (double *) malloc(SzD * numverts);
  yn = (double *) malloc(SzD * numverts);
  zn = (double *) malloc(SzD * numverts);

  xn[0] = x[0];
  yn[0] = y[0];
  zn[0] = z[0];
  xn[1] = x[1]; 
  yn[1] = y[1]; 
  zn[1] = z[1];

  j = 2;
  i2 = 0;
  for(i = 2; i < numverts; i++)
     {
     if(ThreePointsAreColinear(&x[i2], &y[i2], &z[i2]) == 0) 
        {
        xn[j] = x[i];
        yn[j] = y[i];
        zn[j] = z[i];
        ++j;
        }
     else /** over write the last point which is the middle point of the 3 colinear points ***/
        {
        --j;
        xn[j] = x[i];
        yn[j] = y[i];
        zn[j] = z[i];
        ++j;
        }
     ++i2;
     }

   if(j != i)
      {
      for(i=0; i < j; i++)
        {
        x[i] = xn[i];
        y[i] = yn[i];
        z[i] = zn[i];
        }
      numverts = j;
      }

   free(xn);
   free(yn);
   free(zn);

   return(numverts);

}






void MakeBillBoardFromPoints_DynamicInfo(FILE *infile, int TargetCheck, int SortOrder)
{
#define WaveSize 10000
extern struct GlobalGridInfo *GridHeader;

  int i,j,ki,kj,kk,XI,YI,jj,ii, kk2, txi, tyi, savei, AnyToProcess;
  int minii,maxii,minjj,maxjj, unusedint, WriteTheCondition;
  int idn1,SIDlen1;
  int idn2,SIDlen2;
  int keyval,Cnumber, CheckInstance, LastCN, CurSIDlen1, CurSIDlen2;
  char *SID1;
  char *SID2;
  char gform1, gform2;
  double localid1, localid2;
  double radius1, radius2;
  double height1, height2;
  double HowToWrite;
  double ScaleBy,unuseddbl;
  double testx,testy;
  double pointX, pointY, pointZ;
  int ECC1, ECC2;
  int numverts1, numverts2;
  int MaxNUMX, MaxNUMY, GridHeadIndex, LastGHI;
  int WaveX[WaveSize], WaveY[WaveSize], wi, onwave, madechange;
  int TwaveX[WaveSize], TwaveY[WaveSize];
  double DblWaveX[WaveSize];
  double DblWaveY[WaveSize];
  double DblWaveZ[WaveSize];
  long int fileposition;
  double X,Y,Z;
  double X2, Y2, Z2;
  double magnitude, MaxX, MaxY, raisedttl, MaxMag, MinMag;
  int WaveLimit = WaveSize - 10;
  int Lindex1, Lindex2;
  int PtUsed;
  int MagUsed;
  int NumObjects;
  int msglen = 0;
  char *message;
  struct IndexToConds
     {
     long int fileposn;
     int numC;
     int Cindex;
     int Gindex;
     struct IndexToConds * next;
     } *CLroot, *CLn, *CLc, *CLp;
  struct GridV
     {
     double * maxmag;
     char * iscond;
     } *AltRep;

  if((SortOrder < 0) || (SortOrder > 1))
     {
     printf("Unexpected flag %d sent to MakeBillBoardsFromPoints_DynamicInfo\n",SortOrder);
     exit(-1);
     }
  if(infile == NULL)
    return;
  CLroot = NULL;
  SID1 = (char *) malloc(100);
  CurSIDlen1 = 98;
  SID2 = (char *) malloc(100);
  CurSIDlen2 = 98;


  ScaleBy = 1.0;
/** idea for scale by is to lump points into a lower res grid to save time, space ***/

  AnyToProcess = 0;

  LastCN = -99848019;
  LastGHI = -90832980;
  MaxNUMX = MaxNUMY = 0;
  rewind(infile);
  fileposition = ftell(infile);


  SEEIT_fread_int(&keyval, infile);

  txi = 0;

  while(feof(infile) == 0)
     {
     ++txi;
     if((txi % 500) == 0)
        {
        periodic_checking_redraw(0,"Condition Report Progress","Preparing condition report\n     Making BillBoards\n");
        txi = 0;
        }
     SEEIT_fread_int(&Cnumber, infile);

     SEEIT_fread_int(&CheckInstance, infile);

     if(CheckInstance == TargetCheck)
        ++AnyToProcess;

     SEEIT_fread_double(&HowToWrite,infile);

     SEEIT_fread_int(&PtUsed, infile);
     SEEIT_fread_int(&MagUsed, infile);
     SEEIT_fread_int(&NumObjects, infile);

     SEEIT_fread_int(&msglen, infile);
     if(msglen > 0)
        {
        message = (char *) (malloc(msglen + 2));
        fread(&message[0],1,msglen,infile);
        }

     magnitude = 0;
     if(MagUsed > 0)
        SEEIT_fread_double(&magnitude,infile);

     pointX = pointY = pointZ = -1.0;
     if(PtUsed > 0)
        {
        SEEIT_fread_double(&pointX,infile);
        SEEIT_fread_double(&pointY,infile);
        SEEIT_fread_double(&pointZ,infile);
        }
     if(NumObjects > 0)
        {
        SEEIT_fread_int(&idn1,infile);
        if(MagUsed < 1)
           magnitude = idn1;
        SEEIT_fread_int(&SIDlen1,infile);
        if(SIDlen1 > CurSIDlen1);
           {
           free(SID1);
           CurSIDlen1 = SIDlen1 + 10;
           SID1 = (char *) (malloc(CurSIDlen1));
           CurSIDlen1 -= 2;
           }
        fread(&SID1[0],1,SIDlen1,infile);
        SID1[SIDlen1] = '\0';
        SEEIT_fread_int(&ECC1,infile);
        fread(&gform1,1,1,infile);
        SEEIT_fread_int(&Lindex1,infile);
        SEEIT_fread_double(&localid1, infile);
        GridHeadIndex = (int) (0 - localid1); /** the convention when passing in the arg to write ***/
        if(MaxNUMX < GridHeader[GridHeadIndex].Xpts)
           MaxNUMX = GridHeader[GridHeadIndex].Xpts;
        if(MaxNUMY < GridHeader[GridHeadIndex].Ypts)
           MaxNUMY = GridHeader[GridHeadIndex].Ypts;
        SEEIT_fread_double(&radius1,infile);
        SEEIT_fread_double(&height1,infile);
        SEEIT_fread_int(&numverts1, infile);
        if(numverts1 != 1)
           {
           printf("1 bad number of feature vertices in BBread %d\n",numverts1);
           exit(-1);
           }
        SEEIT_fread_double(&X, infile);
        SEEIT_fread_double(&Y, infile);
        SEEIT_fread_double(&Z, infile);
        }

     if(NumObjects > 1)
        {
        SEEIT_fread_int(&idn2,infile);
        SEEIT_fread_int(&SIDlen2,infile);
        if(SIDlen2 > CurSIDlen2);
           {
           free(SID2);
           CurSIDlen2 = SIDlen2 + 10;
           SID2 = (char *) (malloc(CurSIDlen2));
           CurSIDlen2 -= 2;
           }
        fread(&SID2[0],1,SIDlen2,infile);
        SID2[SIDlen2] = '\0';
        SEEIT_fread_int(&ECC2,infile);
        fread(&gform2,1,1,infile);
        SEEIT_fread_int(&Lindex2,infile);
        SEEIT_fread_double(&localid2, infile);
        if(keyval == MASKCONSTANT) /** with this check, the finished DEM should be participant 2 ***/
           {
           GridHeadIndex = (int) (0 - localid2); /** the convention when passing in the arg to write ***/
           if(MaxNUMX < GridHeader[GridHeadIndex].Xpts)
              MaxNUMX = GridHeader[GridHeadIndex].Xpts;
           if(MaxNUMY < GridHeader[GridHeadIndex].Ypts)
              MaxNUMY = GridHeader[GridHeadIndex].Ypts;
           }
        SEEIT_fread_double(&radius2,infile);
        SEEIT_fread_double(&height2,infile);
        SEEIT_fread_int(&numverts2, infile);
        if(numverts2 != 1)
           {
           printf("2 bad number of feature vertices in BBread %d\n",numverts1);
           exit(-1);
           }
        SEEIT_fread_double(&X2, infile);
        SEEIT_fread_double(&Y2, infile);
        SEEIT_fread_double(&Z2, infile);
        }

     if(CheckInstance == TargetCheck)
        {
     if((HowToWrite > 1.5) && (ActiveChecks[CheckInstance].number != RAISEDPC) &&
              (ActiveChecks[CheckInstance].number != FLOWSTEP) &&
               (ActiveChecks[CheckInstance].number != BREAKLINE) &&
               (ActiveChecks[CheckInstance].number != WATERMMU) )
        {
        if(msglen > 0)
           {
           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,Cnumber,PtUsed,MagUsed,NumObjects, message,
                    magnitude, pointX, pointY, pointZ,
                    CrsWlk[Lindex1].LtoSmapping,gform1,Lindex1,localid1,
                    idn1,radius1,height1, &X, &Y, &Z, numverts1,
                    CrsWlk[Lindex2].LtoSmapping,gform2,Lindex2,localid2,
                    idn2,radius2,height2, &X2, &Y2, &Z2, numverts2);
           }
        else
           {
           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,Cnumber,PtUsed,MagUsed,NumObjects, NULL,
                    magnitude, pointX, pointY, pointZ,
                    CrsWlk[Lindex1].LtoSmapping,gform1,Lindex1,localid1,
                    idn1,radius1,height1, &X, &Y, &Z, numverts1,
                    CrsWlk[Lindex2].LtoSmapping,gform2,Lindex2,localid2,
                    idn2,radius2,height2, &X2, &Y2, &Z2, numverts2);
           }
        }
     if((Cnumber != LastCN) || (GridHeadIndex != LastGHI))
        {
        LastCN = Cnumber;
        LastGHI = GridHeadIndex;
        CLn = (struct IndexToConds *) (malloc(sizeof(struct IndexToConds)));
        CLn->fileposn = fileposition;
        CLn->numC = 1;
        CLn->Cindex = Cnumber;
        CLn->Gindex = GridHeadIndex;
        CLn->next = NULL;
        if(CLroot == NULL)
           CLroot = CLn;
        else
           {
           CLc = CLp = CLroot;
           while(CLc != NULL)
              {
              if((CLc->Cindex == CLn->Cindex) && (CLc->Gindex == CLn->Gindex))
                 break;
              CLp = CLc;
              CLc = CLc->next;
              }
           if(CLc == NULL)
              CLp->next = CLn;
           else
              {
              CLn->next = CLc->next;
              CLc->next = CLn;
              }
           }
        }
     else
        CLn->numC += 1;
        }

   fileposition = ftell(infile);
   SEEIT_fread_int(&keyval, infile);
   }

   MaxNUMX = (int) (((double) (MaxNUMX) / ScaleBy) + 1.0);
   MaxNUMY = (int) (((double) (MaxNUMY) / ScaleBy) + 1.0);

   AltRep = (struct GridV *) (malloc(sizeof(struct GridV) * MaxNUMX));
   for(i=0; i<MaxNUMX; i++)
      {
      AltRep[i].maxmag = (double *) (malloc(sizeof(double) * MaxNUMY));
      AltRep[i].iscond = (char *) (malloc(MaxNUMY));
      }

   for(i=0; i<MaxNUMX; i++)
      {
      for(j=0; j<MaxNUMY; j++)
         {
         AltRep[i].maxmag[j] = 0.0;
         AltRep[i].iscond[j] = 0;
         }
      }

   rewind(infile);
   CLc = CLroot;
   LastCN = CLc->Cindex;
   LastGHI = CLc->Gindex;

   if(AnyToProcess > 0)
      CLc = CLroot;
   else
      CLc = NULL;
   txi = 0;
   while(CLc != NULL)
      {

      fseek(infile,CLc->fileposn,SEEK_SET);
      i = 0;
      SEEIT_fread_int(&keyval, infile);
      while(i < CLc->numC)
         {
         if(feof(infile) != 0)
           {
           break;
           }
     ++txi;
     if((txi % 500) == 0)
        {
        periodic_checking_redraw(0,"Condition Report Progress","Preparing condition report\n     Making BillBoards\n");
        txi = 0;
        }
         SEEIT_fread_int(&Cnumber, infile);
         SEEIT_fread_int(&CheckInstance, infile);
         if(CheckInstance == TargetCheck)
            ++AnyToProcess;
         SEEIT_fread_double(&HowToWrite,infile);
         SEEIT_fread_int(&PtUsed, infile);
         SEEIT_fread_int(&MagUsed, infile);
         SEEIT_fread_int(&NumObjects, infile);
         SEEIT_fread_int(&msglen, infile);
         if(msglen > 0)
            {
            message = (char *) (malloc(msglen + 2));
            fread(&message[0],1,msglen,infile);
            }
         magnitude = 0;
         if(MagUsed > 0)
            SEEIT_fread_double(&magnitude,infile);
         pointX = pointY = pointZ = -1.0;
         if(PtUsed > 0)
            {
            SEEIT_fread_double(&pointX,infile);
            SEEIT_fread_double(&pointY,infile);
            SEEIT_fread_double(&pointZ,infile);
            }
         if(NumObjects > 0)
            {
            SEEIT_fread_int(&idn1,infile);
            SEEIT_fread_int(&SIDlen1,infile);
            if(SIDlen1 > CurSIDlen1);
               {
               free(SID1);
               CurSIDlen1 = SIDlen1 + 10;
               SID1 = (char *) (malloc(CurSIDlen1));
               CurSIDlen1 -= 2;
               }
            fread(&SID1[0],1,SIDlen1,infile);
            SID1[SIDlen1] = '\0';
            SEEIT_fread_int(&ECC1,infile);
            fread(&gform1,1,1,infile);
            SEEIT_fread_int(&Lindex1,infile);
            SEEIT_fread_double(&localid1, infile);
            GridHeadIndex = (int) (0 - localid1); /** the convention when passing in the arg to write ***/
            if(MaxNUMX < GridHeader[GridHeadIndex].Xpts)
               MaxNUMX = GridHeader[GridHeadIndex].Xpts;
            if(MaxNUMY < GridHeader[GridHeadIndex].Ypts)
               MaxNUMY = GridHeader[GridHeadIndex].Ypts;
            SEEIT_fread_double(&radius1,infile);
            SEEIT_fread_double(&height1,infile);
            SEEIT_fread_int(&numverts1, infile);
            if(numverts1 != 1)
               {
               printf("3 bad number of feature vertices in BBread %d\n",numverts1);
               exit(-1);
               }
            SEEIT_fread_double(&X, infile);
            SEEIT_fread_double(&Y, infile);
            SEEIT_fread_double(&Z, infile);
            }
         if(NumObjects > 1)
            {
            SEEIT_fread_int(&idn2,infile);
            SEEIT_fread_int(&SIDlen2,infile);
            if(SIDlen2 > CurSIDlen2);
               {
               free(SID2);
               CurSIDlen2 = SIDlen2 + 10;
               SID2 = (char *) (malloc(CurSIDlen2));
               CurSIDlen2 -= 2;
               }
            fread(&SID2[0],1,SIDlen2,infile);
            SID2[SIDlen2] = '\0';
            SEEIT_fread_int(&ECC2,infile);
            fread(&gform2,1,1,infile);
            SEEIT_fread_int(&Lindex2,infile);
            SEEIT_fread_double(&localid2, infile);
            SEEIT_fread_double(&radius2,infile);
            SEEIT_fread_double(&height2,infile);
            SEEIT_fread_int(&numverts2, infile);
            if(numverts2 != 1)
               {
               printf("4 bad number of feature vertices in BBread %d\n",numverts1);
               exit(-1);
               }
            SEEIT_fread_double(&X2, infile);
            SEEIT_fread_double(&Y2, infile);
            SEEIT_fread_double(&Z2, infile);
            }

         if(TargetCheck == CheckInstance)
            {

            unuseddbl = ((X - GridHeader[LastGHI].BaseX) / GridHeader[LastGHI].Xspacing) / ScaleBy; 
            XI = (int) (unuseddbl + 0.5);

            if(XI < MaxNUMX)
               {
               unuseddbl = ((Y - GridHeader[LastGHI].BaseY) / GridHeader[LastGHI].Yspacing) / ScaleBy;
               YI = (int) (unuseddbl + 0.5);
               if(YI < MaxNUMY)
                  {
                  AltRep[XI].iscond[YI] = 1;
                  AltRep[XI].maxmag[YI] = magnitude;
                  ++i;
                  }
               }
            }

         SEEIT_fread_int(&keyval, infile);
         }
      CLc = CLc->next;
      ii = MaxNUMX - 1;
      jj = MaxNUMY - 1;

      if((CLc == NULL) || (CLc->Cindex != LastCN) || (CLc->Gindex != LastGHI))
         {
         for(i=0; i<WaveSize; i++)
            DblWaveZ[i] = 0.0;
         for(XI = 0; XI < MaxNUMX; XI++)
            {
            periodic_checking_redraw(0,"Condition Report Progress","Preparing condition report\n     Making BillBoards\n");
            for(YI = 0; YI < MaxNUMY; YI++)
               {
               if(AltRep[XI].iscond[YI] == 1)
                  {
                  AltRep[XI].iscond[YI] = 3;
                  WaveX[0] = XI;
                  WaveY[0] = YI;
                  minii = maxii = XI;
                  minjj = maxjj = YI;
                  wi = 1;
                  onwave = 1;
                  i = 0;
                  magnitude = AltRep[XI].maxmag[YI];
                  MaxMag = MinMag = AltRep[XI].maxmag[YI];
                  if(magnitude > 0)
                     raisedttl = 1.0;
                  else
                     raisedttl = 0.0;

                  /**MaxX = WaveX[0] * ScaleBy * GridHeader[LastGHI].Xspacing;
                  MaxY = WaveY[0] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                  MaxX = (WaveX[0] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                  MaxY = (WaveY[0] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;

                  while(i < wi)
                     {
                     ii = WaveX[i];
                     jj = WaveY[i] + 1;
                     if((jj < MaxNUMY) && (AltRep[ii].iscond[jj] == 1))
                        {
                        if(AltRep[ii].maxmag[jj] < MinMag)
                           MinMag = AltRep[ii].maxmag[jj];
                        if(AltRep[ii].maxmag[jj] > MaxMag)
                           MaxMag = AltRep[ii].maxmag[jj];
                        AltRep[ii].iscond[jj] = 3;
                        WaveX[wi] = ii;
                        WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                        if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                              ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                           {
                           magnitude = AltRep[ii].maxmag[jj];
                           /***MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                           MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing;***/
                           MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                           MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                           }
                        ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                        }
                     jj = WaveY[i] - 1;
                     if((jj >= 0) && (AltRep[ii].iscond[jj] == 1))
                        {
                        if(AltRep[ii].maxmag[jj] < MinMag)
                           MinMag = AltRep[ii].maxmag[jj];
                        if(AltRep[ii].maxmag[jj] > MaxMag)
                           MaxMag = AltRep[ii].maxmag[jj];
                        AltRep[ii].iscond[jj] = 3;
                        WaveX[wi] = ii;
                        WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                        if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                              ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                           {
                           magnitude = AltRep[ii].maxmag[jj];
                           /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                           MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                           MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                           MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                           }
                        ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                        }
                     ii = WaveX[i] + 1;
                     jj = WaveY[i];
                     if(ii < MaxNUMX)
                        {
                        if(AltRep[ii].iscond[jj] == 1)
                           {
                           if(AltRep[ii].maxmag[jj] < MinMag)
                              MinMag = AltRep[ii].maxmag[jj];
                           if(AltRep[ii].maxmag[jj] > MaxMag)
                              MaxMag = AltRep[ii].maxmag[jj];
                           AltRep[ii].iscond[jj] = 3;
                           WaveX[wi] = ii;
                           WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                           if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                                 ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                              {
                              magnitude = AltRep[ii].maxmag[jj];
                              /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                              MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing;**/
                              MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                              MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                              }
                           ++wi;
                           ++onwave;
                           if(AltRep[ii].maxmag[jj] > 0)
                              raisedttl = raisedttl + 1.0;
                           }
                        jj = WaveY[i] + 1;
                        if((jj < MaxNUMY) && (AltRep[ii].iscond[jj] == 1))
                           {
                           if(AltRep[ii].maxmag[jj] < MinMag)
                              MinMag = AltRep[ii].maxmag[jj];
                           if(AltRep[ii].maxmag[jj] > MaxMag)
                              MaxMag = AltRep[ii].maxmag[jj];
                           AltRep[ii].iscond[jj] = 3;
                           WaveX[wi] = ii;
                           WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                           if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                                 ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                              {
                              magnitude = AltRep[ii].maxmag[jj];
                              /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                              MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing;**/
                              MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                              MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                              }
                           ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                           }
                        jj = WaveY[i] - 1;
                        if((jj >= 0) && (AltRep[ii].iscond[jj] == 1))
                           {
                           if(AltRep[ii].maxmag[jj] < MinMag)
                              MinMag = AltRep[ii].maxmag[jj];
                           if(AltRep[ii].maxmag[jj] > MaxMag)
                              MaxMag = AltRep[ii].maxmag[jj];
                           AltRep[ii].iscond[jj] = 3;
                           WaveX[wi] = ii;
                           WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                           if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                                 ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                              {
                              magnitude = AltRep[ii].maxmag[jj];
                              /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                              MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                              MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                              MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                              }
                           ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                           }
                        }
                     ii = WaveX[i] - 1;
                     jj = WaveY[i];
                     if(ii >= 0)
                        {
                        if(AltRep[ii].iscond[jj] == 1)
                           {
                           if(AltRep[ii].maxmag[jj] < MinMag)
                              MinMag = AltRep[ii].maxmag[jj];
                           if(AltRep[ii].maxmag[jj] > MaxMag)
                              MaxMag = AltRep[ii].maxmag[jj];
                           AltRep[ii].iscond[jj] = 3;
                           WaveX[wi] = ii;
                           WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                           if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                                 ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                              {
                              magnitude = AltRep[ii].maxmag[jj];
                              /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                              MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                              MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                              MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                              }
                           ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                           }
                        jj = WaveY[i] + 1;
                        if((jj < MaxNUMY) && (AltRep[ii].iscond[jj] == 1))
                           {
                           if(AltRep[ii].maxmag[jj] < MinMag)
                              MinMag = AltRep[ii].maxmag[jj];
                           if(AltRep[ii].maxmag[jj] > MaxMag)
                              MaxMag = AltRep[ii].maxmag[jj];
                           AltRep[ii].iscond[jj] = 3;
                           WaveX[wi] = ii;
                           WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                           if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                                 ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                              {
                              magnitude = AltRep[ii].maxmag[jj];
                              /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                              MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                              MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                              MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                              }
                           ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                           }
                        jj = WaveY[i] - 1;
                        if((jj >= 0) && (AltRep[ii].iscond[jj] == 1))
                           {
                           if(AltRep[ii].maxmag[jj] < MinMag)
                              MinMag = AltRep[ii].maxmag[jj];
                           if(AltRep[ii].maxmag[jj] > MaxMag)
                              MaxMag = AltRep[ii].maxmag[jj];
                           AltRep[ii].iscond[jj] = 3;
                           WaveX[wi] = ii;
                           WaveY[wi] = jj;
if(ii < minii)
   minii = ii;
 if(ii > maxii)
  maxii = ii;
if(jj < minjj)
   minjj = jj;
if(jj > maxjj)
   maxjj = jj;
                           if(((SortOrder == 0) && (AltRep[ii].maxmag[jj] < magnitude)) ||
                                 ((SortOrder == 1) && (AltRep[ii].maxmag[jj] > magnitude)))
                              {
                              magnitude = AltRep[ii].maxmag[jj];
                              /**MaxX = WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing;
                              MaxY = WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                              MaxX = (WaveX[wi] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                              MaxY = (WaveY[wi] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                              }
                           ++wi;
                        ++onwave;
                        if(AltRep[ii].maxmag[jj] > 0)
                           raisedttl = raisedttl + 1.0;
                           }
                        }
                     ++i; /** move to next unexpanded element on the Wavefront ***/
                     if(wi > WaveLimit)
                        {
                        kk = 0;
                        for(kj = 0; kj < i; kj++) /** kj should loop through the expanded posts **/
                           {
                           ki = 0;
                           for(txi = WaveX[kj] - 1; txi < (WaveX[kj] + 2); txi++) /** look in the 3x3 centered on kj ***/
                              {
                              if(txi >= 0)
                                 {
                                 if(txi >= MaxNUMX)
                                    break;
                                 for(tyi = (WaveY[kj] - 1); tyi < (WaveY[kj] + 2); tyi++)
                                    {
                                    if(tyi >= 0)
                                       {
                                       if(tyi >= MaxNUMY)
                                          break;
                                       if((AltRep[txi].iscond[tyi] == 1) || (AltRep[txi].iscond[tyi] == 3))
                                          {
                                          ++ki;
                                          }
                                       }
                                    }
                                 }
                              }
                           if(ki < 9) /** if kj is surrounded by condition posts, it can't be on hull, so discard ***/
                              {
                              TwaveX[kk] = WaveX[kj];
                              TwaveY[kk] = WaveY[kj];
                              ++kk;
                              }
                           }
                        ki = kk + 500;
                        if(ki < i) /** is it really worth doing the rerack here? ***/
                           {
                           for(ki=0; ki<kk; ki++) /** copy the non-surrounded post into already expanded slots **/
                              {
                              WaveX[ki] = TwaveX[ki];
                              WaveY[ki] = TwaveY[ki];
                              }
                           savei = ki; /** this should be next unexpanded post **/
                           for(kj = i; kj <wi; kj++) /** copy unexpanded posts down in the array ***/
                              {
                              WaveX[ki] = WaveX[kj];
                              WaveY[ki] = WaveY[kj];
                              ++ki;
                              }
                           i = savei;
                           wi = ki; /** set the new 'last post added **/
                           }
                        if(wi > WaveLimit)
                           {
                           break;
                           }
                        }
                     }
                  for(ii=0; ii<wi; ii++)
                     {
                     /**DblWaveX[ii] = WaveX[ii] * ScaleBy * GridHeader[LastGHI].Xspacing;
                     DblWaveY[ii] = WaveY[ii] * ScaleBy * GridHeader[LastGHI].Yspacing; **/
                     DblWaveX[ii] = (WaveX[ii] * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                     DblWaveY[ii] = (WaveY[ii] * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                     }

                  WriteTheCondition = 1;
                  if(ActiveChecks[CheckInstance].number == RAISEDPC)
                     {
                     if(onwave > 10)
                        magnitude = (100.0 * raisedttl) / ((double) onwave);
                     else
                        magnitude = 0.0;
                     if(magnitude < ActiveChecks[CheckInstance].sensitivity4)
                        WriteTheCondition = 0;
                     if((WriteTheCondition > 0) && ((ActiveChecks[CheckInstance].sensitivity5 < 1.0) ||
                                (ActiveChecks[CheckInstance].sensitivity5 > 1.5)))
                        {
                        if(ActiveChecks[CheckInstance].sensitivity5 < 1.0)
                           WriteTheCondition = 0;
                        for(ii=0; ii<wi; ii++)
                           {
                           txi = WaveX[ii];
                           tyi = WaveY[ii];
                           if((AltRep[txi].iscond[tyi] >= 1) && (AltRep[txi].maxmag[tyi] > 0))
                              {
                              FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,0,1,1,NULL,magnitude,-1.0,-1.0,-1.0,
                               CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_POINT,GridHeader[LastGHI].Lindex,
                               (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                               &DblWaveX[ii], &DblWaveY[ii], &DblWaveZ[ii], 1,
                               0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                              }
                           }
                        }
                     }
                  else if(ActiveChecks[CheckInstance].number == GSHELF)
                     {
                     if(onwave < ActiveChecks[CheckInstance].sensitivity5)
                        WriteTheCondition = 0;
magnitude = onwave;
                     }
                  else if(ActiveChecks[CheckInstance].number == FLOWSTEP)
                     {
                     magnitude = MaxMag - MinMag;
                     if(magnitude < ActiveChecks[CheckInstance].sensitivity3)
                        WriteTheCondition = 0;
                     }
/****************************/
                  else if(ActiveChecks[CheckInstance].number == BREAKLINE)
                     {
                     magnitude = MaxMag - MinMag;
                     if(magnitude < 0.1)
                        WriteTheCondition = 0;
                     }
/******************************/
                  else if(ActiveChecks[CheckInstance].number == WATERMMU)
                     {
                     unuseddbl = (GridHeader[LastGHI].Xspacing * GridHeader[LastGHI].Yspacing) * ((double) (onwave));
                     if(unuseddbl > ActiveChecks[CheckInstance].sensitivity3)
                        WriteTheCondition = 0;
                     else
                        magnitude = unuseddbl;
                     if(wi > WaveLimit) /** then we want to preclude writing conditions for slivery remainders of things ***/
                        {

      madechange = 1;
      while(madechange > 0)
         {
         madechange = 0;
         for(txi = 0; txi < MaxNUMX; txi++)
            {
            periodic_checking_redraw(0,"Condition Report Progress","Preparing condition report\n     Making BillBoards\n");
            for(tyi = 0; tyi < MaxNUMY; tyi++)
               {
               if(AltRep[txi].iscond[tyi] == 3) /** then this node has already been expanded, so mark any unexpanded neighbors **/
                  {
                  for(ii = txi - 1; ii < txi + 2; ii++)
                     {
                     if((ii >= 0) && (ii < MaxNUMX))
                        {
                        for(jj = tyi - 1; jj < tyi + 2; jj++)
                           {
                           if((jj >= 0) && (jj < MaxNUMY))
                              {
                              if(AltRep[ii].iscond[jj] == 1)
                                 {
                                 AltRep[ii].iscond[jj] = 3;
                                 ++madechange;
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         for(txi = MaxNUMX - 1; txi >= 0; txi--)
            {
            for(tyi = MaxNUMY - 1; tyi >= 0; tyi--)
               {
               if(AltRep[txi].iscond[tyi] == 3) /** then this node has already been expanded, so mark any unexpanded neighbors **/
                  {
                  for(ii = txi - 1; ii < txi + 2; ii++)
                     {
                     if((ii >= 0) && (ii < MaxNUMX))
                        {
                        for(jj = tyi - 1; jj < tyi + 2; jj++)
                           {
                           if((jj >= 0) && (jj < MaxNUMY))
                              {
                              if(AltRep[ii].iscond[jj] == 1)
                                 {
                                 AltRep[ii].iscond[jj] = 3;
                                 ++madechange;
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         } /** end while madechange **/
                        }
                     }


                  if(WriteTheCondition > 0)
                     {
                     if(wi == 1)
                        {
                        if(MagUsed > 0)
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,0,1,1,NULL,magnitude,-1.0,-1.0,-1.0,
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_POINT,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, 1,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                        else
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,0,0,1,NULL,magnitude,-1.0,-1.0,-1.0,
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_POINT,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, 1,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                        }
                     else if(wi == 2)
                        {
                        if(MagUsed > 0)
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,1,1,1,NULL,magnitude, MaxX, MaxY, magnitude,
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_LINE,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, 2,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                        else
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,1,0,1,NULL,
                            magnitude, DblWaveX[0], DblWaveY[0], DblWaveZ[0],
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_LINE,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, 2,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                        }
                     else
                        {
                        ii = GrahamScan(DblWaveX,DblWaveY,DblWaveZ,wi);
                        for(ki = minii; ki <= maxii; ki++)
                           {
                           for(kj=minjj; kj <= maxjj; kj++)
                              {
                              if(AltRep[ki].iscond[kj] == 1)
                                 {
                                 testx = ((double) ki * ScaleBy * GridHeader[LastGHI].Xspacing) + GridHeader[LastGHI].BaseX;
                                 testy = ((double) kj * ScaleBy * GridHeader[LastGHI].Yspacing) + GridHeader[LastGHI].BaseY;
                                 if(PointInAreal(testx,testy,DblWaveX,DblWaveY,ii,&unusedint) > 0)
                                    {
                                    AltRep[ki].iscond[kj] = 0;
                                    }
                                 }
                              }
                           }
                        if((ActiveChecks[CheckInstance].number == RAISEDPC) || (ActiveChecks[CheckInstance].number == FLOWSTEP) ||
                               (ActiveChecks[CheckInstance].number == WATERMMU) || (ActiveChecks[CheckInstance].number == GSHELF) ||
                               (ActiveChecks[CheckInstance].number ==  BREAKLINE) || (ActiveChecks[CheckInstance].number == AVGSPIKE) )
                           {
                           ii = RemoveColinearVertices(DblWaveX, DblWaveY, DblWaveZ, ii);
                           MaxX = 0.0; //DblWaveX[0];
                           MaxY = 0.0; //DblWaveY[0];
                           for(kk2=0; kk2<ii; kk2++)
                              {
                              MaxX = MaxX + DblWaveX[kk2];
                              MaxY = MaxY + DblWaveY[kk2];
                              }
                           MaxX = MaxX / ((double) kk2);
                           MaxY = MaxY / ((double) kk2);
                           if(ActiveChecks[CheckInstance].number ==  BREAKLINE)
                              {
                              if(LeastSquaresFit(DblWaveX, DblWaveY, ii,GridHeader[LastGHI].Xspacing,
                                        GridHeader[LastGHI].Yspacing, CheckInstance, &magnitude) > 0)
                                 {
                                 FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,1,1,1,NULL,magnitude, MaxX, MaxY, magnitude,
                                  CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_LINE,GridHeader[LastGHI].Lindex,
                                  (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                                  DblWaveX, DblWaveY, DblWaveZ, 3,
                                  0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                                 }
                              }
                           else
                              {
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,1,1,1,NULL,magnitude, MaxX, MaxY, magnitude,
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_AREAL,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, ii,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                              }
                           }
                        else if(MagUsed > 0)
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,1,1,1,NULL,magnitude, MaxX, MaxY, magnitude,
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_AREAL,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, ii,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                        else
                           {
                           ii = RemoveColinearVertices(DblWaveX, DblWaveY, DblWaveZ, ii);
                           MaxX = 0.0; //DblWaveX[0];
                           MaxY = 0.0; //DblWaveY[0];
                           for(kk2=0; kk2<ii; kk2++)
                              {
                              MaxX = MaxX + DblWaveX[kk2];
                              MaxY = MaxY + DblWaveY[kk2];
                              }
                           MaxX = MaxX / ((double) kk2);
                           MaxY = MaxY / ((double) kk2);
                           FwriteDynamicInfo(ActiveChecks[CheckInstance].number,LastCN,1,0,1,NULL,magnitude, MaxX, MaxY, magnitude,
                            CrsWlk[GridHeader[LastGHI].Lindex].LtoSmapping,G_AREAL,GridHeader[LastGHI].Lindex,
                            (double) (0 - LastGHI),GridHeader[LastGHI].idn,radius1,height1,
                            DblWaveX, DblWaveY, DblWaveZ, ii,
                            0,G_POINT,0,0,0,-1.0,-1.0,&X,&Y,&Z,0);
                           }
                        }
                     }
                  } /** end found a seed point ***/
               } /** end for YI **/
            } /** end for XI **/
         if(CLc == NULL)
            break;
         else
            {
            LastCN = CLc->Cindex;
            LastGHI = CLc->Gindex;
            }
         }
      }

   free(SID1);
   free(SID2);

   CLc = CLroot;
   while(CLc != NULL)
      {
      CLp = CLc;
      CLc = CLc->next;
      free(CLp);
      }
   for(i=0; i<MaxNUMX; i++)
      {
      free(AltRep[i].maxmag);
      free(AltRep[i].iscond);
      }
   free(AltRep);
   return;
}


int MatchAndModifyLinearSegment(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4,
                         double *matchlen, double *xa1, double *ya1, double *xa2, double *ya2,
                           int *on0, int *on1, int *on2, int *on3, double tolerance)
{
int answer = 0;
int OnLine[4];
int i;
double ptldist;


   for(i=0; i<4; i++)
      OnLine[i] = 0;

   ptldist = PointToLineDist2D(x1,y1,x3,y3,x4,y4);
   if(ptldist <= tolerance)
      OnLine[0] = 1;

   ptldist = PointToLineDist2D(x2,y2,x3,y3,x4,y4);
   if(ptldist <= tolerance)
      OnLine[1] = 1;

   ptldist = PointToLineDist2D(x3,y3,x1,y1,x2,y2);
   if(ptldist <= tolerance)
      OnLine[2] = 1;

   ptldist = PointToLineDist2D(x4,y4,x1,y1,x2,y2);
   if(ptldist <= tolerance)
      OnLine[3] = 1;

   *on0 = OnLine[0];
   *on1 = OnLine[1];
   *on2 = OnLine[2];
   *on3 = OnLine[3];

   i = 0;
   if(OnLine[0] > 0)
      ++i;
   if(OnLine[1] > 0)
      ++i;
   if(OnLine[2] > 0)
      ++i;
   if(OnLine[3] > 0)
      ++i;

   if(i < 1) /** no shared points for these edges **/
      {
      answer = 0;
      *matchlen = 0.0;
      }
   else if (i == 1)
      {
      answer = 0;
      *matchlen = 0.0;
      }
   else if(i == 4) /** two edges match exactly (share same two vertices) ****/
      {
      answer = 1;
      *matchlen = Distance(x1,y1,x2,y2);
      *xa1 = x1;  *ya1 = y1;
      *xa2 = x2;  *ya2 = y2;
      }
   else if(i == 3)  /** one endpt shared **/
      {
      if(OnLine[0] == 0) /** then either 3 or 4 should be a T **/
         {
         *matchlen = Distance(x3,y3,x4,y4);
         answer = 1;
         *xa1 = x3;  *ya1 = y3;
         *xa2 = x4;  *ya2 = y4;
         }
      else if(OnLine[1] == 0) /** again, either 3 or 4 should be a T **/
         {
         *matchlen = Distance(x3,y3,x4,y4);
         answer = 1;
         *xa1 = x3;  *ya1 = y3;
         *xa2 = x4;  *ya2 = y4;
         }
      else if(OnLine[2] == 0) /** then either 1 or 2 should be a T **/
         {
         *matchlen = Distance(x1,y1,x2,y2);
         answer = 1;
         *xa1 = x1;  *ya1 = y1;
         *xa2 = x2;  *ya2 = y2;
         }
      else if(OnLine[3] == 0) /** again, either 1 or 2 should be a T **/
         {
         *matchlen = Distance(x1,y1,x2,y2);
         answer = 1;
         *xa1 = x1;  *ya1 = y1;
         *xa2 = x2;  *ya2 = y2;
         }
      }
   else if(i == 2)  /** one line 'inside' the other or a skew overlap, must have 2 'T's **/
                   /** could also have edges sharing a vertex, but no other points in common **/
      {
      if(OnLine[0] == 1) /** then pt (x1,y1) could be a 'T' ***/
         {
         if(OnLine[1] == 1) /** then pt (x2,y2) is a 'T' ***/
            {
            answer = 1;
            *matchlen = Distance(x1,y1,x2,y2);
            *xa1 = x1;  *ya1 = y1;
            *xa2 = x2;  *ya2 = y2;
            }
         else if(OnLine[2] == 1) /** then pt (x3,y3) could be a 'T' ***/
            {
            answer = 1;
            *matchlen = Distance(x1,y1,x3,y3);
            *xa1 = x1;  *ya1 = y1;
            *xa2 = x3;  *ya2 = y3;
            }
         else if(OnLine[3] == 1) /** then pt (x4,y4) could be a 'T' ***/
            {
            answer = 1;
            *matchlen = Distance(x1,y1,x4,y4);
            *xa1 = x1;  *ya1 = y1;
            *xa2 = x4;  *ya2 = y4;
            }
         }
      else if(OnLine[1] == 1)  /** then pt (x2,y2) could be a 'T' ***/
         {
         if(OnLine[2] == 1) /** then pt (x3,y3) could be a 'T' ***/
            {
            answer = 1;
            *matchlen = Distance(x2,y2,x3,y3);
            *xa1 = x2;  *ya1 = y2;
            *xa2 = x3;  *ya2 = y3;
            }
         else if(OnLine[3] == 1) /** then pt (x4,y4) could be a 'T' ***/
            {
            answer = 1;
            *matchlen = Distance(x2,y2,x4,y4);
            *xa1 = x2;  *ya1 = y2;
            *xa2 = x4;  *ya2 = y4;
            }
         }
      else if(OnLine[2] == 1) /** then pt (x3,y3) is a 'T' ***/
         {
         if(OnLine[3] == 1) /** then pt (x4,y4) is a 'T' ***/
            {
            answer = 1;
            *matchlen = Distance(x3,y3,x4,y4);
            *xa1 = x3;  *ya1 = y3;
            *xa2 = x4;  *ya2 = y4;
            }
         }
      }


   return(answer);
}


double GenerateKernelStats(int PassNumber, int Lindex1,int Uindex1, int Lindex2, int Uindex2, double AvgElevErr, 
                             double *RMSE, long int *NZpoints, double *MaxDiff, double *MaxDiffX, double *MaxDiffY,
                                int *SrcLindex, int *RefLindex, int *MLindex)
{
FILE * Source;
FILE * Reference;
FILE * Mask;
double *ref_row1, *ref_row2, *ref_row3, *ref_row4, *ref_row5;
double *src_row;
double *mrow1, *mrow2, *mrow3, *mrow4, *mrow5;
char *readC = NULL;
unsigned char *readUC = NULL;
short int *readSI = NULL;
unsigned short int *readUSI = NULL;
int *readI = NULL;
unsigned int *readUI = NULL;
float *readF = NULL;
double *readD = NULL;
int checkinstance, SrcNGT, RefNGT, MNGT,compositeindex;
int i,j,k, jj1, jj2, useit, part1, part2, part3, LI1,SI1, Sxi,Syi,EndIndex;
int GridIndex, SrcIndex, RefIndex, MIndex, MaxAllocIndex;
int DoTheCheckNow, HaveNULLtertiary;
int listlength, TgtRX, TgtRY, TgtMX, TgtMY, REndIndex,MEndIndex;
long int SrcSeek, RefSeek, MSeek, NumPtsUsed;
long int NonZeroPoints = 0;
double MaxError = 0;
double MinSrcX,MinSrcY,MaxSrcX,MaxSrcY;
double MinRefX,MinRefY,MaxRefX,MaxRefY;
double MinMX,MinMY,MaxMX,MaxMY,CX,CY,XCoord, YCoord,RXCoord,RYCoord,MYCoord;
double CheckMinX,CheckMinY,CheckMaxX,CheckMaxY;
double unuseddbl, s1, s2,weighted_avg_ele, mean, variance, RMSEsum;
double returnvalue,tmpdbl;
double ActualSY, ActualRYlow, ActualRYhigh;
char portiondone[200];

   Source = Reference = Mask = NULL;
   SrcIndex = RefIndex = MIndex = -1;
   src_row = NULL;
   ref_row1 = ref_row2 = ref_row3 = ref_row4 = ref_row5 = NULL;
   mrow1 = mrow2 = mrow3 = mrow4 = mrow5 = NULL;
   variance = 0.0;
   RMSEsum = 0.0;
   DoTheCheckNow = 0;
   *MLindex = -1;

   for(GridIndex=0; GridIndex < NumberOfGrids; GridIndex++)
      {
      LI1 = GridHeader[GridIndex].Lindex;
      SI1 = CrsWlk[LI1].LtoSmapping;
      for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++)
         {
         HaveNULLtertiary = 0;
         if(ActiveChecks[checkinstance].tertiaryEDCSstuff == NULL)
            HaveNULLtertiary = 1;
         else
            {
            for(i=0; i<SACfull; i++)
               {
               if(ActiveChecks[checkinstance].tertiaryEDCSstuff[i] == 1)
                  break;
               }
            if(i >= SACfull)
               HaveNULLtertiary = 1;
            }

         if((CombinedCheckApplies(GridHeader[GridIndex].idn,checkinstance,KERNELSTATS,LI1,SI1,
                     SCCtable[SI1].C, SCCtable[SI1].S, SCCtable[SI1].D,
                     &part1,&part2,&part3) >  0) || (HaveNULLtertiary > 0))
            {
            if(part1 > 0)
               {
               SrcIndex = GridIndex;
               *SrcLindex = LI1;
               }
            if(part2 > 0)
               {
               RefIndex = GridIndex;
               *RefLindex = LI1;
               }
            if(part3 > 0) 
               {
               MIndex = GridIndex;
               *MLindex = LI1;
               }

            if((HaveNULLtertiary > 0) && (SrcIndex >= 0) && (RefIndex >= 0) && (MIndex < 0))
               DoTheCheckNow = 1;
            else if((SrcIndex >= 0) && (RefIndex >= 0) && (MIndex >= 0))
               DoTheCheckNow = 1;

            if(DoTheCheckNow > 0)
               {
               MinSrcX = GridHeader[SrcIndex].BaseX;   MinSrcY = GridHeader[SrcIndex].BaseY;
               MaxSrcX = GridHeader[SrcIndex].Xspacing * (double) GridHeader[SrcIndex].Xpts;
               MaxSrcX += MinSrcX;
               MaxSrcY = GridHeader[SrcIndex].Yspacing * (double) GridHeader[SrcIndex].Ypts;
               MaxSrcY += MinSrcY;

               MinRefX = GridHeader[RefIndex].BaseX;   MinRefY = GridHeader[RefIndex].BaseY;
               MaxRefX = GridHeader[RefIndex].Xspacing * (double) GridHeader[RefIndex].Xpts;
               MaxRefX += MinRefX;
               MaxRefY = GridHeader[RefIndex].Yspacing * (double) GridHeader[RefIndex].Ypts;
               MaxRefY += MinRefY;
               
               if(MIndex >= 0)
                  {
                  MinMX = GridHeader[MIndex].BaseX;   MinMY = GridHeader[MIndex].BaseY;
                  MaxMX = GridHeader[MIndex].Xspacing * (double) GridHeader[MIndex].Xpts;
                  MaxMX += MinMX;
                  MaxMY = GridHeader[MIndex].Yspacing * (double) GridHeader[MIndex].Ypts;
                  MaxMY += MinMY;
                  MaxMY = GridHeader[MIndex].Yspacing * (double) GridHeader[MIndex].Ypts;
                  }
               else
                  {
                  MinMX = MinSrcX;
                  MaxMX = MaxSrcX;
                  MinMY = MinSrcY;
                  MaxMY = MaxSrcY;
                  }

               CheckMinX = MinSrcX;
               if(MinRefX > CheckMinX)
                  CheckMinX = MinRefX;
               if((MIndex >= 0) && (MinMX > CheckMinX))
                  CheckMinX = MinMX;
               CheckMaxX = MaxSrcX;
               if(MaxRefX < CheckMaxX)
                  CheckMaxX = MaxRefX;
               if((MIndex >= 0) && (MaxMX < CheckMaxX))
                  CheckMaxX = MaxMX;

               CheckMinY = MinSrcY;
               if(MinRefY > CheckMinY)
                  CheckMinY = MinRefY;
               if((MIndex >= 0) && (MinMY > CheckMinY))
                  CheckMinY = MinMY;
               CheckMaxY = MaxSrcY;
               if(MaxRefY < CheckMaxY)
                  CheckMaxY = MaxRefY;
               if((MIndex >= 0) && (MaxMY < CheckMaxY))
                  CheckMaxY = MaxMY;

               if((CheckMinX < CheckMaxX) && (CheckMinY < CheckMaxY))
                  {
                  unuseddbl = (double) Lindex1 * RegionSize;
                  if(unuseddbl > CheckMinX)
                     CheckMinX = unuseddbl;
                  unuseddbl = (double) Uindex1 * RegionSize + RegionSize;
                  if(unuseddbl < CheckMaxX)
                     CheckMaxX = unuseddbl;
                  unuseddbl = (double) Lindex2 * RegionSize;
                  if(unuseddbl > CheckMinY)
                     CheckMinY = unuseddbl;
                  unuseddbl = (double) Uindex2 * RegionSize + RegionSize;
                  if(unuseddbl < CheckMaxY)
                     CheckMaxY = unuseddbl;

                  if((CheckMinX <= CheckMaxX) && (CheckMinY <= CheckMaxY))
                     {
/*** now we know that all three grids overlap in the part of the project where the inspection has been commanded ***/
                     listlength = (MaxXindex + 1) * (MaxYindex + 1);
                     i = (int) (CheckMinX / RegionSize);
                     j = (int) (CheckMinY / RegionSize);
                     compositeindex = (i * (MaxYindex + 1)) + j; /** LM at this index should have all 3 grids ***/
                     SrcNGT = RefNGT = MNGT = -1;
                     for(i=0; i<listlength; i++)
                        {
                        for(j=0; j< NGT[i].NumGrids; j++)
                           {
                           if((SrcNGT < 0) && (NGT[i].GridIndex[j] == SrcIndex))
                              {
                              SrcNGT = i;
                              SrcSeek = NGT[i].fileptr[j];
                              if(Source != NULL)
                                 fclose(Source);
                              Source = GlobalUseOpenGridFile(NGT[i].FileNumber[j]);
                              if(Source == NULL)
                                 return(0);
                              }
                           if((RefNGT < 0) && (NGT[i].GridIndex[j] == RefIndex))
                              {
                              RefNGT = i;
                              RefSeek = NGT[i].fileptr[j];
                              if(Reference != NULL)
                                 fclose(Reference);
                              Reference = GlobalUseOpenGridFile(NGT[i].FileNumber[j]);
                              if(Reference == NULL)
                                 {
                                 fclose(Source);
                                 return(0);
                                 }
                              }
                           if((MIndex >= 0) && (MNGT < 0) && (NGT[i].GridIndex[j] == MIndex))
                              {
                              MNGT = i;
                              MSeek = NGT[i].fileptr[j];
                              if(Mask != NULL)
                                 fclose(Mask);
                              Mask = GlobalUseOpenGridFile(NGT[i].FileNumber[j]);
                              if(Mask == NULL)
                                 {
                                 fclose(Source);
                                 fclose(Reference);
                                 return(0);
                                 }
                              }

                           if((SrcNGT >= 0) && (RefNGT >= 0) && ((MNGT >= 0) || (MIndex < 0)))
                              break;
                           }
                        if((SrcNGT >= 0) && (RefNGT >= 0) && ((MNGT >= 0) || (MIndex < 0)))
                           break;
                        }

                     if((SrcNGT >= 0) && (RefNGT >= 0) && ((MNGT >= 0) || (MIndex < 0)))
                        {
                        NumPtsUsed = 0;
                        mean = 0;
Sxi = (int) ((CheckMinX - GridHeader[SrcIndex].BaseX) / GridHeader[SrcIndex].Xspacing);
if(Sxi < 0)
   Sxi = 0;
EndIndex = (int) ((CheckMaxY - GridHeader[SrcIndex].BaseX) / GridHeader[SrcIndex].Xspacing);
EndIndex = EndIndex - Sxi;
if(EndIndex >= GridHeader[SrcIndex].Xpts)
EndIndex = GridHeader[SrcIndex].Xpts - 1;
                        ++EndIndex;
                        MaxAllocIndex = EndIndex;
                        TgtRX = (int) ((CheckMinX / GridHeader[RefIndex].Xspacing) + 0.5);
                        REndIndex = (int) ((CheckMaxY / GridHeader[RefIndex].Xspacing) + 0.5);
                        REndIndex = REndIndex - TgtRX;
TgtRX = (int) ((CheckMinX - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
TgtRX -= 2;
if(TgtRX < 0)
   TgtRX = 0;
REndIndex = (int) ((CheckMaxY - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
REndIndex = REndIndex - TgtRX;
if(REndIndex >= GridHeader[RefIndex].Xpts)
   REndIndex = GridHeader[RefIndex].Xpts - 1;
                        ++REndIndex;
                        if(REndIndex > EndIndex)
                           MaxAllocIndex = REndIndex;
                        if(MIndex >= 0)
                           {
TgtMX = (int) ((CheckMinX - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
TgtMX -= 2;
if(TgtMX < 0)
   TgtMX = 0;
MEndIndex = (int) ((CheckMaxY - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
MEndIndex = MEndIndex - TgtRX;
if(MEndIndex >= GridHeader[MIndex].Xpts)
   MEndIndex = GridHeader[MIndex].Xpts - 1;
                           ++MEndIndex;
                           if(MEndIndex > EndIndex)
                              MaxAllocIndex = MEndIndex;
                           }


                        src_row = (double *) (malloc(SzD * (EndIndex + 1)));
                        ref_row1 = (double *) (malloc(SzD * (REndIndex + 1)));
                        ref_row2 = (double *) (malloc(SzD * (REndIndex + 1)));
                        ref_row3 = (double *) (malloc(SzD * (REndIndex + 1)));
                        ref_row4 = (double *) (malloc(SzD * (REndIndex + 1)));
                        ref_row5 = (double *) (malloc(SzD * (REndIndex + 1)));
                        if(MIndex >= 0)
                           {
                           mrow1 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           mrow2 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           mrow3 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           mrow4 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           mrow5 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           }
                        else
                           {
                           mrow1 = mrow2 = mrow3 = mrow4 = mrow5 = NULL; 
                           }

                        switch(GridHeader[SrcIndex].BPGV)
                           {
                           case 1:  readC = (char *) (malloc(MaxAllocIndex + 1)); break;
                           case 2:  readSI = (short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 3:  readUSI = (unsigned short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 4:  readI = (int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 5:  readUI = (unsigned int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 6:  readF = (float *) (malloc(SzF * (MaxAllocIndex + 1))); break;
                           case 7:  readUC = (unsigned char *) (malloc(MaxAllocIndex + 1)); break;
                           case 9:  readD = (double *) (malloc(SzD * (MaxAllocIndex + 1))); break;
                           default: printf("Bad Value in BPGV %d\n",GridHeader[SrcIndex].BPGV);  exit(-1);
                           }

                        switch(GridHeader[RefIndex].BPGV)
                           {
                           case 1:  if(readC == NULL)
                                       readC = (char *) (malloc(MaxAllocIndex + 1)); break;
                           case 2:  if(readSI == NULL)
                                       readSI = (short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 3:  if(readUSI == NULL)
                                       readUSI = (unsigned short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 4:  if(readI == NULL)
                                       readI = (int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 5:  if(readUI == NULL)
                                       readUI = (unsigned int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 6:  if(readF == NULL)
                                       readF = (float *) (malloc(SzF * (MaxAllocIndex + 1))); break;
                           case 7:  if(readUC == NULL)
                                       readUC = (unsigned char *) (malloc(MaxAllocIndex + 1)); break;
                           case 9:  if(readD == NULL)
                                       readD = (double *) (malloc(SzD * (MaxAllocIndex + 1))); break;
                           default: printf("2: Bad Value in BPGV %d\n",GridHeader[RefIndex].BPGV);  exit(-1);
                           }

                        if(MIndex >= 0)
                           {
                           switch(GridHeader[MIndex].BPGV)
                              {
                              case 1:  if(readC == NULL)
                                       readC = (char *) (malloc(MaxAllocIndex + 1)); break;
                              case 2:  if(readSI == NULL)
                                       readSI = (short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                              case 3:  if(readUSI == NULL)
                                       readUSI = (unsigned short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                              case 4:  if(readI == NULL)
                                       readI = (int *) (malloc(SzI * (MaxAllocIndex + 1))); break; 
                              case 5:  if(readUI == NULL)
                                       readUI = (unsigned int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                              case 6:  if(readF == NULL) 
                                       readF = (float *) (malloc(SzF * (MaxAllocIndex + 1))); break;
                              case 7:  if(readUC == NULL)
                                       readUC = (unsigned char *) (malloc(MaxAllocIndex + 1)); break;
                              case 9:  if(readD == NULL)
                                       readD = (double *) (malloc(SzD * (MaxAllocIndex + 1))); break; 
                              default: printf("3: Bad Value in BPGV %d\n",GridHeader[MIndex].BPGV);  exit(-1);
                              }
                           }

                        for(CY = CheckMinY; CY <= CheckMaxY; CY += GridHeader[SrcIndex].Yspacing)
                           {
                           YCoord = CY - CheckMinY;
                           if(PassNumber == 1)
                              sprintf(portiondone,"DEM Surface Comparison Statistics Generation:\n %.1lf%% complete\n",
                                  50.0 * ((CY - CheckMinY) / ((CheckMaxY - CheckMinY))));
                           else if(PassNumber == 2)
                              sprintf(portiondone,"DEM Surface Comparison Statistics Generation:\n %.1lf%% complete\n",
                                  50.0 + (50.0 * ((CY - CheckMinY) / ((CheckMaxY - CheckMinY)))));
                           periodic_checking_redraw(0,"Nearest Neighbor Statistics Generation Progress",portiondone);
                           Syi = (int) ((CY / GridHeader[SrcIndex].Yspacing) + 0.5);
Syi = (int) ((CY - GridHeader[SrcIndex].BaseY) / GridHeader[SrcIndex].Yspacing);
if(Syi < 0)
   Syi = 0;
YCoord = Syi * GridHeader[SrcIndex].Yspacing;
ActualSY = Syi * GridHeader[SrcIndex].Yspacing + GridHeader[SrcIndex].BaseY;
                           switch(GridHeader[SrcIndex].BPGV)
                              {
                              case 1:  
                                  MassReadCharGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readC, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readC[i];
                                  break;
                              case 2:
                                  MassReadShIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readSI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readSI[i];
                                  break;
                              case 3:
                                  MassReadUnShIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readUSI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readUSI[i];
                                  break;
                              case 4:
                                  MassReadIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readI[i];
                                  break;
                               case 5:
                                  MassReadUnIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readUI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readUI[i];
                                  break;
                               case 6:
                                  MassReadFloatGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readF, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readF[i];
                                  break;
                              case 7:
                                  MassReadUnCharGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readUC, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readUC[i];
                                  break;
                               case 9:
                                  MassReadDoubleGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, src_row, EndIndex);
                                  break;
                              default: printf("Bad Value in BPGV %d\n",GridHeader[SrcIndex].BPGV);  exit(-1);
                              }
                           TgtRY = (int) ((CY / GridHeader[RefIndex].Yspacing));
                           RYCoord = TgtRY * GridHeader[RefIndex].Yspacing;
                           while((RYCoord + GridHeader[RefIndex].Yspacing) < YCoord)
                              {
                              ++TgtRY;
                              RYCoord = TgtRY * GridHeader[RefIndex].Yspacing;
                              if(RYCoord > CheckMaxY)
                                 break;
                              }
tmpdbl = (((double) (Syi)) * GridHeader[SrcIndex].Yspacing) + GridHeader[SrcIndex].BaseY;
tmpdbl -= GridHeader[RefIndex].BaseY;
TgtRY = (int) (tmpdbl / GridHeader[RefIndex].Yspacing);
ActualRYlow = TgtRY * GridHeader[RefIndex].Yspacing + GridHeader[RefIndex].BaseY;
ActualRYhigh = ActualRYlow + GridHeader[RefIndex].Yspacing;
RYCoord = TgtRY * GridHeader[RefIndex].Yspacing;
TgtRY -= 2;
TgtRX = (int) ((CheckMinX - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
TgtRX -= 2;
if(TgtRX < 0)
   TgtRX = 0;
REndIndex = (int) ((CheckMaxY - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
REndIndex = REndIndex - TgtRX;
if(REndIndex >= GridHeader[RefIndex].Xpts)
REndIndex = GridHeader[RefIndex].Xpts - 1;
                           switch(GridHeader[RefIndex].BPGV)
                              {
                              case 1:
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readC[i];
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readC[i];
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readC[i];
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readC[i];
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readC[i];
                                  break;
                              case 2:
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readSI[i];
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readSI[i];
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readSI[i];
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readSI[i];
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readSI[i];
                                  break;
                              case 3:
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readUSI[i];
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readUSI[i];
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readUSI[i];
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readUSI[i];
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readUSI[i];
                                  break;
                              case 4:
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readI[i];
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readI[i];
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readI[i];
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readI[i];
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readI[i];
                                  break;
                               case 5:
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readUI[i];
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readUI[i];
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readUI[i];
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readUI[i];
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readUI[i];
                                  break;
                               case 6:
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readF[i];
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readF[i];
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readF[i];
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readF[i];
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readF[i];
                                  break;
                              case 7:
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readUC[i];
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readUC[i];
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row3[i] = (double) readUC[i];
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row4[i] = (double) readUC[i];
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row5[i] = (double) readUC[i];
                                  break;
                               case 9:
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row1, REndIndex);
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row2, REndIndex);
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY+2,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row3, REndIndex);
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY+3,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row4, REndIndex);
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY+4,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row5, REndIndex);
                                  break;
                              default: printf("Bad Value in BPGV %d\n",GridHeader[RefIndex].BPGV);  exit(-1);
                              }
                           if(MIndex >= 0)
                              {
tmpdbl = (((double) (Syi)) * GridHeader[SrcIndex].Yspacing) + GridHeader[SrcIndex].BaseY;
tmpdbl -= GridHeader[MIndex].BaseY;
TgtMY = (int) (tmpdbl / GridHeader[MIndex].Yspacing);
MYCoord = TgtMY * GridHeader[MIndex].Yspacing;
TgtMY -= 2;
TgtMX = (int) ((CheckMinX - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
TgtMX -= 2;
if(TgtMX < 0)
   TgtMX = 0;
MEndIndex = (int) ((CheckMaxY - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
MEndIndex = MEndIndex - TgtMX;
if(MEndIndex >= GridHeader[MIndex].Xpts)
MEndIndex = GridHeader[MIndex].Xpts - 1;

                              switch(GridHeader[MIndex].BPGV)
                                 {
                                 case 1:
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readC[i];
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readC[i];
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readC[i];
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readC[i];
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readC[i];
                                     break;
                                 case 2:
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readSI[i];
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readSI[i];
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readSI[i];
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readSI[i];
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readSI[i];
                                     break;
                                 case 3:
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                       GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readUSI[i];
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readUSI[i];
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readUSI[i];
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readUSI[i];
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readUSI[i];
                                     break;
                                 case 4:
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readI[i];
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readI[i];
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readI[i];
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readI[i];
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readI[i];
                                     break;
                                  case 5:
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readUI[i];
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readUI[i];
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readUI[i];
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readUI[i];
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readUI[i];
                                     break;
                                  case 6:
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readF[i];
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readF[i];
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readF[i];
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readF[i];
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readF[i];
                                     break;
                                 case 7:
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readUC[i];
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readUC[i];
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow3[i] = (double) readUC[i];
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow4[i] = (double) readUC[i];
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow5[i] = (double) readUC[i];
                                     break;
                                  case 9:
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow1, MEndIndex);
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow2, MEndIndex);
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY+2,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow2, MEndIndex);
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY+3,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow2, MEndIndex);
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY+4,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow2, MEndIndex);
                                     break;
                                 default: printf("Bad Value in BPGV %d\n",GridHeader[MIndex].BPGV);  exit(-1);
                                 }
                              }

                           for(i=0; i<EndIndex; i++)
                              {
                              XCoord = ((double) i * GridHeader[SrcIndex].Xspacing);
                              CX = CheckMinX + XCoord;

                              j = (int) (XCoord / GridHeader[RefIndex].Xspacing)  - 1;
                              if(j < 0)
                                 j = 0;
                              unuseddbl = j * GridHeader[RefIndex].Xspacing;
                              jj1 = 0;
                              jj2 = 1;
                              while(XCoord > unuseddbl)
                                 {
                                 jj1 = j;
                                 ++j;
                                 jj2 = j;
                                 unuseddbl = j * GridHeader[RefIndex].Xspacing;
                                 if(j >= REndIndex)
                                    break;
                                 }
                              useit = 1;
                              if((j < 0) || (j >= REndIndex))
                                 useit = 0;
                              if(useit > 0)
                                 {
                                 unuseddbl = ActiveChecks[checkinstance].sensitivity;
                                 if(src_row[i] <= unuseddbl)
                                    useit = 0;
                                 else if((ref_row5[j-1] <= unuseddbl) || (ref_row5[j] <= unuseddbl) ||
                                       (ref_row5[j+1] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row1[j-1] <= unuseddbl) || (ref_row1[j] <= unuseddbl) ||
                                       (ref_row1[j+1] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row2[j-1] <= unuseddbl) || (ref_row2[j] <= unuseddbl) ||
                                       (ref_row2[j+1] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row3[j-1] <= unuseddbl) || (ref_row3[j] <= unuseddbl) ||
                                       (ref_row3[j+1] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row4[j-1] <= unuseddbl) || (ref_row4[j] <= unuseddbl) ||
                                       (ref_row4[j+1] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row4[j-2] <= unuseddbl) || (ref_row4[j+2] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row3[j-2] <= unuseddbl) || (ref_row3[j+2] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row2[j-2] <= unuseddbl) || (ref_row2[j+2] <= unuseddbl))
                                    useit = 0;
                                 }
                              if((useit > 0) && (MIndex >= 0))
                                 {
                                 s1 = ActiveChecks[checkinstance].sensitivity2;
                                 s2 = ActiveChecks[checkinstance].sensitivity3;
                                 for(k=j-2; k<=j+2; k++)
                                    {
                                    if((k >= (j-1)) && (k <= (j+1)))
                                       {
                                       if((mrow1[k] < s1) || (mrow1[k] > s2) || (mrow5[k] < s1) || (mrow5[k] > s2))
                                          {
                                          useit = 0;
                                          break;
                                          }
                                       }
                                    else
                                       {
                                       if((mrow2[k] < s1) || (mrow2[k] > s2) || (mrow3[k] < s1) || (mrow3[k] > s2) ||
                                                  (mrow4[k] < s1) || (mrow4[k] > s2))
                                          {
                                          useit = 0;
                                          break;
                                          }
                                       }
                                    }
                                 }
                              if(useit > 0)
                                 {
                                 RXCoord = ((double) jj1 * GridHeader[RefIndex].Xspacing);
                                 weighted_avg_ele = (ref_row5[j-1] + ref_row5[j] + ref_row5[j+1]) / 36.0;
                                 weighted_avg_ele += (ref_row4[j-2] + ref_row4[j+2]) / 36.0;
                                 weighted_avg_ele += (ref_row4[j-1] + ref_row4[j] + ref_row4[j+1]) / 24.0;
                                 weighted_avg_ele += (ref_row3[j-2] + ref_row3[j+2]) / 36.0;
                                 weighted_avg_ele += (ref_row3[j-1] + ref_row3[j+1]) / 24.0;
                                 weighted_avg_ele += ref_row3[j] / 3.0;
                                 weighted_avg_ele += (ref_row2[j-2] + ref_row2[j+2]) / 36.0;
                                 weighted_avg_ele += (ref_row2[j-1] + ref_row2[j] + ref_row2[j+1]) / 24.0;
                                 weighted_avg_ele += (ref_row1[j-1] + ref_row1[j] + ref_row1[j+1]) / 36.0;

                                 NumPtsUsed += 1;
                                 if(PassNumber == 1)
                                    {
                                       unuseddbl = src_row[i] - weighted_avg_ele;
                                       if(unuseddbl <= ActiveChecks[checkinstance].sensitivity4)
                                          {
                                          mean = mean + unuseddbl;
                                          if(unuseddbl > 0.001)
                                             {
                                             NonZeroPoints += 1;
                                             if(unuseddbl > MaxError)
                                               {
                                               MaxError = unuseddbl;
                                               *MaxDiffX = GridHeader[SrcIndex].BaseX + (i * GridHeader[SrcIndex].Xspacing);
                                               *MaxDiffX += (CheckMinX - MinSrcX);
*MaxDiffY = ActualSY;
                                               }
                                            }
                                         else if(unuseddbl < -0.001)
                                            {
                                            NonZeroPoints += 1;
                                            if((0 - unuseddbl) > MaxError)
                                               {
                                               MaxError = 0 - unuseddbl;
                                               *MaxDiffX = GridHeader[SrcIndex].BaseX + (i * GridHeader[SrcIndex].Xspacing);
                                               *MaxDiffX += (CheckMinX - MinSrcX);
*MaxDiffY = ActualSY;
                                               }
                                            }
                                          }
                                       /**} **/
                                    }
                                 else if(PassNumber == 2)
                                    {
                                    if(weighted_avg_ele > src_row[i])
                                       unuseddbl = weighted_avg_ele - src_row[i];
                                    else
                                       unuseddbl = src_row[i] - weighted_avg_ele;
                                    if(unuseddbl <= ActiveChecks[checkinstance].sensitivity4)
                                       {
                                       variance = variance + ((unuseddbl - AvgElevErr) * ((unuseddbl - AvgElevErr)));
                                       RMSEsum = RMSEsum + (unuseddbl * unuseddbl);
                                       }
                                    }
                                 }
                              }
                           } /*** end for CY ***/
                        
                        free(src_row);   src_row = NULL;
                        free(ref_row1);  ref_row1 = NULL;
                        free(ref_row2);  ref_row2 = NULL;
                        free(ref_row3);  ref_row3 = NULL;
                        free(ref_row4);  ref_row4 = NULL;
                        free(ref_row5);  ref_row5 = NULL;

                        if(MIndex >= 0)
                           {
                           free(mrow1);     mrow1 = NULL;
                           free(mrow2);     mrow2 = NULL;
                           free(mrow3);     mrow3 = NULL;
                           free(mrow4);     mrow4 = NULL;
                           free(mrow5);     mrow5 = NULL;
                           }

                        if(readC != NULL)
                           {  free(readC);     readC = NULL;  }
                        if(readSI != NULL)
                           {  free(readSI);    readSI = NULL;  }
                        if(readUSI != NULL)
                           {  free(readUSI);   readUSI = NULL;  }
                        if(readI != NULL)
                           {  free(readI);     readI = NULL;  }
                        if(readUI != NULL)
                           {  free(readUI);    readUI = NULL;  }
                        if(readF != NULL)
                           {  free(readF);     readF = NULL;  }
                        if(readUC != NULL)
                           {  free(readUC);    readUC = NULL;  }
                        if(readD != NULL)
                           {  free(readD);     readD = NULL;  }

                        *MaxDiff = MaxError;
                        *NZpoints = NonZeroPoints;
                        }  /*** end if((SrcNGT >= 0) && (RefNGT >= 0) && (MNGT >= 0))  ***/
                     }
                  }


               if(Source != NULL)
                  { fclose(Source);  Source = NULL; }
               if(Reference != NULL)
                  { fclose(Reference);  Reference = NULL; }
               if((MIndex >= 0) && (Mask != NULL))
                  { fclose(Mask);  Mask = NULL; }
               SrcIndex = RefIndex = MIndex = -1; /** reset in case there is another BILINSTATS instance ***/
               }
            }
         }
      }

   if(PassNumber == 1)
      returnvalue = mean / ((double) NumPtsUsed);
   else if(PassNumber == 2)
      {
      *RMSE = sqrt(RMSEsum / NumPtsUsed);
      returnvalue = sqrt(variance / ((double) (NumPtsUsed - 1)));
      }
   return(returnvalue);
}



double GenerateNearestNeighborStats(int PassNumber, int Lindex1,int Uindex1, int Lindex2, int Uindex2, double AvgElevErr, 
                             double *RMSE, long int *NZpoints, double *MaxDiff, double *MaxDiffX, double *MaxDiffY,
                                int *SrcLindex, int *RefLindex, int *MLindex)
{
FILE * Source;
FILE * Reference;
FILE * Mask;
/*************/
double *ref_row1, *ref_row2;
double *src_row;
double *mrow1, *mrow2;
char *readC = NULL;
unsigned char *readUC = NULL;
short int *readSI = NULL;
unsigned short int *readUSI = NULL;
int *readI = NULL;
unsigned int *readUI = NULL;
float *readF = NULL;
double *readD = NULL;
/***************/

int checkinstance, SrcNGT, RefNGT, MNGT,compositeindex;
int i,j,k, jj1, jj2, kk1, kk2, useit, part1, part2, part3, LI1,SI1, Sxi,Syi,EndIndex;
int GridIndex, SrcIndex, RefIndex, MIndex, MaxAllocIndex;
int DoTheCheckNow, HaveNULLtertiary;
int listlength, TgtRX, TgtRY, TgtMX, TgtMY, REndIndex,MEndIndex;
long int SrcSeek, RefSeek, MSeek, NumPtsUsed;
long int NonZeroPoints = 0;
double MaxError = 0;
double MinSrcX,MinSrcY,MaxSrcX,MaxSrcY;
double MinRefX,MinRefY,MaxRefX,MaxRefY;
double MinMX,MinMY,MaxMX,MaxMY,CX,CY,XCoord, YCoord,RXCoord,RYCoord,MYCoord;
double CheckMinX,CheckMinY,CheckMaxX,CheckMaxY;
double unuseddbl, s1, s2,weighted_avg_ele, mean, variance, RMSEsum, t,u,t1,u1, Ex_y1, Ex_y2;
double returnvalue, tmpdbl;
double ActualSY, ActualRYlow, ActualRYhigh;
char portiondone[200];

   Source = Reference = Mask = NULL;
   SrcIndex = RefIndex = MIndex = -1;
   src_row = NULL;
   ref_row1 = ref_row2 = NULL;
   mrow1 = mrow2 = NULL;
   variance = 0.0;
   RMSEsum = 0.0;
   DoTheCheckNow = 0;
   *MLindex = -1;

   for(GridIndex=0; GridIndex < NumberOfGrids; GridIndex++)
      {
      LI1 = GridHeader[GridIndex].Lindex;
      SI1 = CrsWlk[LI1].LtoSmapping;
      for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++)
         {
         HaveNULLtertiary = 0;
         if(ActiveChecks[checkinstance].tertiaryEDCSstuff == NULL)
            HaveNULLtertiary = 1;
         else
            {
            for(i=0; i<SACfull; i++)
               {
               if(ActiveChecks[checkinstance].tertiaryEDCSstuff[i] == 1)
                  break;
               }
            if(i >= SACfull)
               HaveNULLtertiary = 1;
            }

         if((CombinedCheckApplies(GridHeader[GridIndex].idn,checkinstance,BILINSTATS,LI1,SI1,
                     SCCtable[SI1].C, SCCtable[SI1].S, SCCtable[SI1].D,
                     &part1,&part2,&part3) >  0) || (HaveNULLtertiary > 0))
            {
            if(part1 > 0)
               {
               SrcIndex = GridIndex;
               *SrcLindex = LI1;
               }
            if(part2 > 0)
               {
               RefIndex = GridIndex;
               *RefLindex = LI1;
               }
            if(part3 > 0) 
               {
               MIndex = GridIndex;
               *MLindex = LI1;
               }

            if((HaveNULLtertiary > 0) && (SrcIndex >= 0) && (RefIndex >= 0) && (MIndex < 0))
               DoTheCheckNow = 1;
            else if((SrcIndex >= 0) && (RefIndex >= 0) && (MIndex >= 0))
               DoTheCheckNow = 1;

            if(DoTheCheckNow > 0)
               {
               MinSrcX = GridHeader[SrcIndex].BaseX;   MinSrcY = GridHeader[SrcIndex].BaseY;
               MaxSrcX = GridHeader[SrcIndex].Xspacing * (double) GridHeader[SrcIndex].Xpts;
               MaxSrcX += MinSrcX;
               MaxSrcY = GridHeader[SrcIndex].Yspacing * (double) GridHeader[SrcIndex].Ypts;
               MaxSrcY += MinSrcY;

               MinRefX = GridHeader[RefIndex].BaseX;   MinRefY = GridHeader[RefIndex].BaseY;
               MaxRefX = GridHeader[RefIndex].Xspacing * (double) GridHeader[RefIndex].Xpts;
               MaxRefX += MinRefX;
               MaxRefY = GridHeader[RefIndex].Yspacing * (double) GridHeader[RefIndex].Ypts;
               MaxRefY += MinRefY;
               
               if(MIndex >= 0)
                  {
                  MinMX = GridHeader[MIndex].BaseX;   MinMY = GridHeader[MIndex].BaseY;
                  MaxMX = GridHeader[MIndex].Xspacing * (double) GridHeader[MIndex].Xpts;
                  MaxMX += MinMX;
                  MaxMY = GridHeader[MIndex].Yspacing * (double) GridHeader[MIndex].Ypts;
                  MaxMY += MinMY;
                  MaxMY = GridHeader[MIndex].Yspacing * (double) GridHeader[MIndex].Ypts;
                  }
               else
                  {
                  MinMX = MinSrcX;
                  MaxMX = MaxSrcX;
                  MinMY = MinSrcY;
                  MaxMY = MaxSrcY;
                  }

               CheckMinX = MinSrcX;
               if(MinRefX > CheckMinX)
                  CheckMinX = MinRefX;
               if((MIndex >= 0) && (MinMX > CheckMinX))
                  CheckMinX = MinMX;
               CheckMaxX = MaxSrcX;
               if(MaxRefX < CheckMaxX)
                  CheckMaxX = MaxRefX;
               if((MIndex >= 0) && (MaxMX < CheckMaxX))
                  CheckMaxX = MaxMX;

               CheckMinY = MinSrcY;
               if(MinRefY > CheckMinY)
                  CheckMinY = MinRefY;
               if((MIndex >= 0) && (MinMY > CheckMinY))
                  CheckMinY = MinMY;
               CheckMaxY = MaxSrcY;
               if(MaxRefY < CheckMaxY)
                  CheckMaxY = MaxRefY;
               if((MIndex >= 0) && (MaxMY < CheckMaxY))
                  CheckMaxY = MaxMY;

               if((CheckMinX < CheckMaxX) && (CheckMinY < CheckMaxY))
                  {
                  unuseddbl = (double) Lindex1 * RegionSize;
                  if(unuseddbl > CheckMinX)
                     CheckMinX = unuseddbl;
                  unuseddbl = (double) Uindex1 * RegionSize + RegionSize;
                  if(unuseddbl < CheckMaxX)
                     CheckMaxX = unuseddbl;
                  unuseddbl = (double) Lindex2 * RegionSize;
                  if(unuseddbl > CheckMinY)
                     CheckMinY = unuseddbl;
                  unuseddbl = (double) Uindex2 * RegionSize + RegionSize;
                  if(unuseddbl < CheckMaxY)
                     CheckMaxY = unuseddbl;

                  if((CheckMinX <= CheckMaxX) && (CheckMinY <= CheckMaxY))
                     {
/*** now we know that all three grids overlap in the part of the project where the inspection has been commanded ***/
                     listlength = (MaxXindex + 1) * (MaxYindex + 1);
                     i = (int) (CheckMinX / RegionSize);
                     j = (int) (CheckMinY / RegionSize);
                     compositeindex = (i * (MaxYindex + 1)) + j; /** LM at this index should have all 3 grids ***/
                     SrcNGT = RefNGT = MNGT = -1;
                     for(i=0; i<listlength; i++)
                        {
                        for(j=0; j< NGT[i].NumGrids; j++)
                           {
                           if((SrcNGT < 0) && (NGT[i].GridIndex[j] == SrcIndex))
                              {
                              SrcNGT = i;
                              SrcSeek = NGT[i].fileptr[j];
                              if(Source != NULL)
                                 fclose(Source);
                              Source = GlobalUseOpenGridFile(NGT[i].FileNumber[j]);
                              if(Source == NULL)
                                 return(0);
                              }
                           if((RefNGT < 0) && (NGT[i].GridIndex[j] == RefIndex))
                              {
                              RefNGT = i;
                              RefSeek = NGT[i].fileptr[j];
                              if(Reference != NULL)
                                 fclose(Reference);
                              Reference = GlobalUseOpenGridFile(NGT[i].FileNumber[j]);
                              if(Reference == NULL)
                                 {
                                 fclose(Source);
                                 return(0);
                                 }
                              }
                           if((MIndex >= 0) && (MNGT < 0) && (NGT[i].GridIndex[j] == MIndex))
                              {
                              MNGT = i;
                              MSeek = NGT[i].fileptr[j];
                              if(Mask != NULL)
                                 fclose(Mask);
                              Mask = GlobalUseOpenGridFile(NGT[i].FileNumber[j]);
                              if(Mask == NULL)
                                 {
                                 fclose(Source);
                                 fclose(Reference);
                                 return(0);
                                 }
                              }

                           if((SrcNGT >= 0) && (RefNGT >= 0) && ((MNGT >= 0) || (MIndex < 0)))
                              break;
                           }
                        if((SrcNGT >= 0) && (RefNGT >= 0) && ((MNGT >= 0) || (MIndex < 0)))
                           break;
                        }

                     if((SrcNGT >= 0) && (RefNGT >= 0) && ((MNGT >= 0) || (MIndex < 0)))
                        {
                        NumPtsUsed = 0;
                        mean = 0;
Sxi = (int) ((CheckMinX - GridHeader[SrcIndex].BaseX) / GridHeader[SrcIndex].Xspacing);
if(Sxi < 0)
   Sxi = 0;
EndIndex = (int) ((CheckMaxY - GridHeader[SrcIndex].BaseX) / GridHeader[SrcIndex].Xspacing);
EndIndex = EndIndex - Sxi;
if(EndIndex >= GridHeader[SrcIndex].Xpts)
EndIndex = GridHeader[SrcIndex].Xpts - 1;
                        ++EndIndex;
                        MaxAllocIndex = EndIndex;
                        TgtRX = (int) ((CheckMinX / GridHeader[RefIndex].Xspacing) + 0.5);
                        REndIndex = (int) ((CheckMaxY / GridHeader[RefIndex].Xspacing) + 0.5);
                        REndIndex = REndIndex - TgtRX;
TgtRX = (int) ((CheckMinX - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
if(TgtRX < 0)
   TgtRX = 0;
REndIndex = (int) ((CheckMaxY - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
REndIndex = REndIndex - TgtRX;
if(REndIndex >= GridHeader[RefIndex].Xpts)
   REndIndex = GridHeader[RefIndex].Xpts - 1;
                        ++REndIndex;
                        if(REndIndex > EndIndex)
                           MaxAllocIndex = REndIndex;
                        if(MIndex >= 0)
                           {
TgtMX = (int) ((CheckMinX - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
if(TgtMX < 0)
   TgtMX = 0;
MEndIndex = (int) ((CheckMaxY - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
MEndIndex = MEndIndex - TgtRX;
if(MEndIndex >= GridHeader[MIndex].Xpts)
   MEndIndex = GridHeader[MIndex].Xpts - 1;
                           ++MEndIndex;
                           if(MEndIndex > EndIndex)
                              MaxAllocIndex = MEndIndex;
                           }


                        src_row = (double *) (malloc(SzD * (EndIndex + 1)));
                        ref_row1 = (double *) (malloc(SzD * (REndIndex + 1)));
                        ref_row2 = (double *) (malloc(SzD * (REndIndex + 1)));
                        if(MIndex >= 0)
                           {
                           mrow1 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           mrow2 = (double *) (malloc(SzD * (MEndIndex + 1)));
                           }
                        else
                           {
                           mrow1 = mrow2 = NULL; 
                           }

                        switch(GridHeader[SrcIndex].BPGV)
                           {
                           case 1:  readC = (char *) (malloc(MaxAllocIndex + 1)); break;
                           case 2:  readSI = (short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 3:  readUSI = (unsigned short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 4:  readI = (int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 5:  readUI = (unsigned int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 6:  readF = (float *) (malloc(SzF * (MaxAllocIndex + 1))); break;
                           case 7:  readUC = (unsigned char *) (malloc(MaxAllocIndex + 1)); break;
                           case 9:  readD = (double *) (malloc(SzD * (MaxAllocIndex + 1))); break;
                           default: printf("Bad Value in BPGV %d\n",GridHeader[SrcIndex].BPGV);  exit(-1);
                           }

                        switch(GridHeader[RefIndex].BPGV)
                           {
                           case 1:  if(readC == NULL)
                                       readC = (char *) (malloc(MaxAllocIndex + 1)); break;
                           case 2:  if(readSI == NULL)
                                       readSI = (short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 3:  if(readUSI == NULL)
                                       readUSI = (unsigned short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                           case 4:  if(readI == NULL)
                                       readI = (int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 5:  if(readUI == NULL)
                                       readUI = (unsigned int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                           case 6:  if(readF == NULL)
                                       readF = (float *) (malloc(SzF * (MaxAllocIndex + 1))); break;
                           case 7:  if(readUC == NULL)
                                       readUC = (unsigned char *) (malloc(MaxAllocIndex + 1)); break;
                           case 9:  if(readD == NULL)
                                       readD = (double *) (malloc(SzD * (MaxAllocIndex + 1))); break;
                           default: printf("2: Bad Value in BPGV %d\n",GridHeader[RefIndex].BPGV);  exit(-1);
                           }

                        if(MIndex >= 0)
                           {
                           switch(GridHeader[MIndex].BPGV)
                              {
                              case 1:  if(readC == NULL)
                                       readC = (char *) (malloc(MaxAllocIndex + 1)); break;
                              case 2:  if(readSI == NULL)
                                       readSI = (short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                              case 3:  if(readUSI == NULL)
                                       readUSI = (unsigned short int *) (malloc(SzShort * (MaxAllocIndex + 1))); break;
                              case 4:  if(readI == NULL)
                                       readI = (int *) (malloc(SzI * (MaxAllocIndex + 1))); break; 
                              case 5:  if(readUI == NULL)
                                       readUI = (unsigned int *) (malloc(SzI * (MaxAllocIndex + 1))); break;
                              case 6:  if(readF == NULL) 
                                       readF = (float *) (malloc(SzF * (MaxAllocIndex + 1))); break;
                              case 7:  if(readUC == NULL)
                                       readUC = (unsigned char *) (malloc(MaxAllocIndex + 1)); break;
                              case 9:  if(readD == NULL)
                                       readD = (double *) (malloc(SzD * (MaxAllocIndex + 1))); break; 
                              default: printf("3: Bad Value in BPGV %d\n",GridHeader[MIndex].BPGV);  exit(-1);
                              }
                           }

                        for(CY = CheckMinY; CY <= CheckMaxY; CY += GridHeader[SrcIndex].Yspacing)
                           {
                           YCoord = CY - CheckMinY;
                           if(PassNumber == 1)
                              sprintf(portiondone,"DEM Surface Comparison Statistics Generation:\n %.1lf%% complete\n",
                                  50.0 * ((CY - CheckMinY) / ((CheckMaxY - CheckMinY))));
                           else if(PassNumber == 2)
                              sprintf(portiondone,"DEM Surface Comparison Statistics Generation:\n %.1lf%% complete\n",
                                  50.0 + (50.0 * ((CY - CheckMinY) / ((CheckMaxY - CheckMinY)))));
                           periodic_checking_redraw(0,"Nearest Neighbor Statistics Generation Progress",portiondone);
                           Syi = (int) ((CY / GridHeader[SrcIndex].Yspacing) + 0.5);
Syi = (int) ((CY - GridHeader[SrcIndex].BaseY) / GridHeader[SrcIndex].Yspacing);
if(Syi < 0)
   Syi = 0;
YCoord = Syi * GridHeader[SrcIndex].Yspacing;
ActualSY = Syi * GridHeader[SrcIndex].Yspacing + GridHeader[SrcIndex].BaseY;
                           switch(GridHeader[SrcIndex].BPGV)
                              {
                              case 1:  
                                  MassReadCharGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readC, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readC[i];
                                  break;
                              case 2:
                                  MassReadShIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readSI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readSI[i];
                                  break;
                              case 3:
                                  MassReadUnShIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readUSI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readUSI[i];
                                  break;
                              case 4:
                                  MassReadIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readI[i];
                                  break;
                               case 5:
                                  MassReadUnIntGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readUI, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readUI[i];
                                  break;
                               case 6:
                                  MassReadFloatGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readF, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readF[i];
                                  break;
                              case 7:
                                  MassReadUnCharGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, readUC, EndIndex);
                                  for(i=0; i<EndIndex; i++)
                                     src_row[i] = (double) readUC[i];
                                  break;
                               case 9:
                                  MassReadDoubleGridValues(Source,SrcSeek,Sxi,Syi,
                                       GridHeader[SrcIndex].Xpts,GridHeader[SrcIndex].Ypts, src_row, EndIndex);
                                  break;
                              default: printf("Bad Value in BPGV %d\n",GridHeader[SrcIndex].BPGV);  exit(-1);
                              }
                           TgtRY = (int) ((CY / GridHeader[RefIndex].Yspacing));
                           RYCoord = TgtRY * GridHeader[RefIndex].Yspacing;
                           while((RYCoord + GridHeader[RefIndex].Yspacing) < YCoord)
                              {
                              ++TgtRY;
                              RYCoord = TgtRY * GridHeader[RefIndex].Yspacing;
                              if(RYCoord > CheckMaxY)
                                 break;
                              }
tmpdbl = (((double) (Syi)) * GridHeader[SrcIndex].Yspacing) + GridHeader[SrcIndex].BaseY;
tmpdbl -= GridHeader[RefIndex].BaseY;
TgtRY = (int) (tmpdbl / GridHeader[RefIndex].Yspacing);
ActualRYlow = TgtRY * GridHeader[RefIndex].Yspacing + GridHeader[RefIndex].BaseY;
ActualRYhigh = ActualRYlow + GridHeader[RefIndex].Yspacing;
RYCoord = TgtRY * GridHeader[RefIndex].Yspacing;
TgtRX = (int) ((CheckMinX - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
if(TgtRX < 0)
   TgtRX = 0;
REndIndex = (int) ((CheckMaxY - GridHeader[RefIndex].BaseX) / GridHeader[RefIndex].Xspacing);
REndIndex = REndIndex - TgtRX;
if(REndIndex >= GridHeader[RefIndex].Xpts)
REndIndex = GridHeader[RefIndex].Xpts - 1;
                           switch(GridHeader[RefIndex].BPGV)
                              {
                              case 1:
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readC[i];
                                  MassReadCharGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readC[i];
                                  break;
                              case 2:
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readSI[i];
                                  MassReadShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readSI[i];
                                  break;
                              case 3:
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readUSI[i];
                                  MassReadUnShIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUSI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readUSI[i];
                                  break;
                              case 4:
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readI[i];
                                  MassReadIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readI[i];
                                  break;
                               case 5:
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readUI[i];
                                  MassReadUnIntGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUI, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readUI[i];
                                  break;
                               case 6:
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readF[i];
                                  MassReadFloatGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readF, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readF[i];
                                  break;
                              case 7:
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row1[i] = (double) readUC[i];
                                  MassReadUnCharGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, readUC, REndIndex);
                                  for(i=0; i<REndIndex; i++)
                                     ref_row2[i] = (double) readUC[i];
                                  break;
                               case 9:
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row1, REndIndex);
                                  MassReadDoubleGridValues(Reference,RefSeek,TgtRX,TgtRY+1,
                                       GridHeader[RefIndex].Xpts,GridHeader[RefIndex].Ypts, ref_row2, REndIndex);
                                  break;
                              default: printf("Bad Value in BPGV %d\n",GridHeader[RefIndex].BPGV);  exit(-1);
                              }
                           if(MIndex >= 0)
                              {
tmpdbl = (((double) (Syi)) * GridHeader[SrcIndex].Yspacing) + GridHeader[SrcIndex].BaseY;
tmpdbl -= GridHeader[MIndex].BaseY;
TgtMY = (int) (tmpdbl / GridHeader[MIndex].Yspacing);
MYCoord = TgtMY * GridHeader[MIndex].Yspacing;
TgtMX = (int) ((CheckMinX - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
if(TgtMX < 0)
   TgtMX = 0;
MEndIndex = (int) ((CheckMaxY - GridHeader[MIndex].BaseX) / GridHeader[MIndex].Xspacing);
MEndIndex = MEndIndex - TgtMX;
if(MEndIndex >= GridHeader[MIndex].Xpts)
MEndIndex = GridHeader[MIndex].Xpts - 1;

                              switch(GridHeader[MIndex].BPGV)
                                 {
                                 case 1:
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readC[i];
                                     MassReadCharGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readC[i];
                                     break;
                                 case 2:
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readSI[i];
                                     MassReadShIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readSI[i];
                                     break;
                                 case 3:
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                       GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readUSI[i];
                                     MassReadUnShIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUSI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readUSI[i];
                                     break;
                                 case 4:
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readI[i];
                                     MassReadIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readI[i];
                                     break;
                                  case 5:
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readUI[i];
                                     MassReadUnIntGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUI, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readUI[i];
                                     break;
                                  case 6:
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readF[i];
                                     MassReadFloatGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readF, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readF[i];
                                     break;
                                 case 7:
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow1[i] = (double) readUC[i];
                                     MassReadUnCharGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, readUC, MEndIndex);
                                     for(i=0; i<MEndIndex; i++)
                                        mrow2[i] = (double) readUC[i];
                                     break;
                                  case 9:
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow1, MEndIndex);
                                     MassReadDoubleGridValues(Mask,MSeek,TgtMX,TgtMY+1,
                                          GridHeader[MIndex].Xpts,GridHeader[MIndex].Ypts, mrow2, MEndIndex);
                                     break;
                                 default: printf("Bad Value in BPGV %d\n",GridHeader[MIndex].BPGV);  exit(-1);
                                 }
                              }

                           for(i=0; i<EndIndex; i++)
                              {
                              XCoord = ((double) i * GridHeader[SrcIndex].Xspacing);
                              CX = CheckMinX + XCoord;

                              j = (int) (XCoord / GridHeader[RefIndex].Xspacing)  - 1;
                              if(j < 0)
                                 j = 0;
                              unuseddbl = j * GridHeader[RefIndex].Xspacing;
                              jj1 = 0;
                              jj2 = 1;
                              while(XCoord > unuseddbl)
                                 {
                                 jj1 = j;
                                 ++j;
                                 jj2 = j;
                                 unuseddbl = j * GridHeader[RefIndex].Xspacing;
                                 if(j >= REndIndex)
                                    break;
                                 }
                              useit = 1;
                              if((j < 0) || (j >= REndIndex))
                                 useit = 0;
                              if(useit > 0)
                                 {
                                 unuseddbl = ActiveChecks[checkinstance].sensitivity;
                                 if(src_row[i] <= unuseddbl)
                                    useit = 0;
                                 else if((ref_row1[jj1] <= unuseddbl) || (ref_row1[jj2] <= unuseddbl))
                                    useit = 0;
                                 else if((ref_row2[jj1] <= unuseddbl) || (ref_row2[jj2] <= unuseddbl))
                                    useit = 0;
                                 }
                              if((useit > 0) && (MIndex >= 0))
                                 {
                                 s1 = ActiveChecks[checkinstance].sensitivity2;
                                 s2 = ActiveChecks[checkinstance].sensitivity3;
                                 k = (int) (XCoord / GridHeader[MIndex].Xspacing)  - 1;
                                 if(k < 0)
                                    k = 0;
                                 unuseddbl = k * GridHeader[MIndex].Xspacing;
                                 kk1 = 0;
                                 kk2 = 1;
                                 while(XCoord > unuseddbl)
                                    {
                                    kk1 = k;
                                    ++k;
                                    kk2 = k;
                                    unuseddbl = k * GridHeader[MIndex].Xspacing;
                                    if(k >= MEndIndex)
                                       break;
                                    }
                                 if(useit > 0)
                                    {
                                    if((mrow1[kk1] < s1) || (mrow1[kk1] > s2))
                                       useit = 0;
                                    else if((mrow1[kk2] < s1) || (mrow1[kk2] > s2))
                                       useit = 0;
                                    else if((mrow2[kk1] < s1) || (mrow2[kk1] > s2))
                                       useit = 0;
                                    else if((mrow2[kk2] < s1) || (mrow2[kk2] > s2))
                                       useit = 0;
                                    }
                                 }
                              if(useit > 0)
                                 {
                                 RXCoord = ((double) jj1 * GridHeader[RefIndex].Xspacing);
                                 t = ((jj2 * GridHeader[RefIndex].Xspacing) - XCoord) / GridHeader[RefIndex].Xspacing;
                                 t1 = 1.0 - t;
u = (ActualSY - ActualRYlow) / (GridHeader[RefIndex].Yspacing);
u1 = 1.0 - u;


Ex_y1 = (t * ref_row1[jj1]) + (t1 * ref_row1[jj2]);
Ex_y2 = (t * ref_row2[jj1]) + (t1 * ref_row2[jj2]);
weighted_avg_ele = (u * Ex_y2) + (u1 * Ex_y1);

                                 NumPtsUsed += 1;
                                 if(PassNumber == 1)
                                    {
                                       unuseddbl = src_row[i] - weighted_avg_ele;
                                       if(unuseddbl <= ActiveChecks[checkinstance].sensitivity4)
                                          {
                                          mean = mean + unuseddbl;
                                          if(unuseddbl > 0.001)
                                             {
                                             NonZeroPoints += 1;
                                             if(unuseddbl > MaxError)
                                               {
                                               MaxError = unuseddbl;
                                               *MaxDiffX = GridHeader[SrcIndex].BaseX + (i * GridHeader[SrcIndex].Xspacing);
                                               *MaxDiffX += (CheckMinX - MinSrcX);
*MaxDiffY = ActualSY;
                                               }
                                            }
                                          else if(unuseddbl < -0.001)
                                            {
                                             NonZeroPoints += 1;
                                             if((0 - unuseddbl) > MaxError)
                                               {
                                               MaxError = 0 - unuseddbl;
                                               *MaxDiffX = GridHeader[SrcIndex].BaseX + (i * GridHeader[SrcIndex].Xspacing);
                                               *MaxDiffX += (CheckMinX - MinSrcX);
                                               *MaxDiffY = ActualSY;
                                               }
                                            }
                                          }
                                    }
                                 else if(PassNumber == 2)
                                    {
                                    if(weighted_avg_ele > src_row[i])
                                       unuseddbl = weighted_avg_ele - src_row[i];
                                    else
                                       unuseddbl = src_row[i] - weighted_avg_ele;
                                    if(unuseddbl <= ActiveChecks[checkinstance].sensitivity4)
                                       {
                                       variance = variance + ((unuseddbl - AvgElevErr) * ((unuseddbl - AvgElevErr)));
                                       RMSEsum = RMSEsum + (unuseddbl * unuseddbl);
                                       }
                                    }
                                 }
                              }
                           } /*** end for CY ***/
                        
                        free(src_row);   src_row = NULL;
                        free(ref_row1);  ref_row1 = NULL;
                        free(ref_row2);  ref_row2 = NULL;

                        if(MIndex >= 0)
                           {
                           free(mrow1);     mrow1 = NULL;
                           free(mrow2);     mrow2 = NULL;
                           }

                        if(readC != NULL)
                           {  free(readC);     readC = NULL;  }
                        if(readSI != NULL)
                           {  free(readSI);    readSI = NULL;  }
                        if(readUSI != NULL)
                           {  free(readUSI);   readUSI = NULL;  }
                        if(readI != NULL)
                           {  free(readI);     readI = NULL;  }
                        if(readUI != NULL)
                           {  free(readUI);    readUI = NULL;  }
                        if(readF != NULL)
                           {  free(readF);     readF = NULL;  }
                        if(readUC != NULL)
                           {  free(readUC);    readUC = NULL;  }
                        if(readD != NULL)
                           {  free(readD);     readD = NULL;  }

                        *MaxDiff = MaxError;
                        *NZpoints = NonZeroPoints;
                        }  /*** end if((SrcNGT >= 0) && (RefNGT >= 0) && (MNGT >= 0))  ***/
                     }
                  }


               if(Source != NULL)
                  { fclose(Source);  Source = NULL; }
               if(Reference != NULL)
                  { fclose(Reference);  Reference = NULL; }
               if((MIndex >= 0) && (Mask != NULL))
                  { fclose(Mask);  Mask = NULL; }
               SrcIndex = RefIndex = MIndex = -1; /** reset in case there is another BILINSTATS instance ***/
               }
            }
         }
      }

   if(PassNumber == 1)
      returnvalue = mean / ((double) NumPtsUsed);
   else if(PassNumber == 2)
      {
      *RMSE = sqrt(RMSEsum / NumPtsUsed);
      returnvalue = sqrt(variance / ((double) (NumPtsUsed - 1)));
      }
   return(returnvalue);
}


void PerformMoreLinearRelatedChecks(char * prompt, int cindex,struct RawLinear *root,double minx,double miny,double maxx,double maxy, double multiplier,
    int NorthCount, int SouthCount, int EastCount, int WestCount,
    struct RawAreal *Aroot, struct RawBridge *Broot, struct RawPoint *Proot, struct ListOfEdges *PEroot, struct ListOfEdges *VPEroot,
    struct RawLinear *NLroot, struct RawAreal *NAroot, struct ThePolys * Polyroot, int SzPC, int SzPW, int IndexLimit)
{
struct RawLinear *rlc, *rln;
struct RawAreal *rac;
struct RawPoint *rpc;
struct RB_Edgelist * LastEdge;
int Config1, Strat1, Domain1, Config2, Strat2, Domain2;
int checkinstance,i,ii,j,jj,jj2,ii2,i2,k,km1,tgtedge;
int LastJ, UsedTree, FoundCondition;
int part1, part2, part3, foundpair,keepit, conditionfound;
double x1,y1,x2,y2,x3,y3,x4,y4,z1,z2,z3,z4;
double xi, yi, segmentlength, ptpdist, tolerancepad, tol2, ptldist, ptldist2, ptpdist1, T;
double VX[2], VY[2], VZ[2];
unsigned char clipflag;
struct collection
   {
   int keyval;
   int Cnumber;
   double x[5];
   double y[5];
   double z[5];
   double magnitude;
   int IDN;
   unsigned int Sindex;
   int vertexindex1, vertexindex2;
   struct RawAreal *aa;
   struct RawBridge *bb;
   struct RawPoint *pp;
   struct ListOfEdges *le;
   struct RawLinear *ll1,*ll2;
   struct ThePolys *poly;
   struct collection * next;
   } *ERroot, *ERc, *ERn;
int SzC = sizeof(struct collection);


   ERroot = NULL;
   rlc = root;

   while(rlc != NULL)
      {
      Config1 = SCCtable[rlc->Sindex].C;
      Strat1 = SCCtable[rlc->Sindex].S;
      Domain1 = SCCtable[rlc->Sindex].D;
      for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++)
         {
         if((CombinedCheckApplies(rlc->idn,checkinstance,LACUTFAIL,rlc->Lindex,rlc->Sindex,
                                    Config1,Strat1,Domain1,
                                    &part1,&part2,&part3)) && (part1 > 0))
            {
            for(i=1; i<rlc->numnodes; i++)
               {
               ii = i - 1;
               x1 = rlc->x[ii];
               y1 = rlc->y[ii];
               z1 = rlc->z[ii];
               x2 = rlc->x[i];
               y2 = rlc->y[i];
               z2 = rlc->z[i];
               if(ClipLineToRegion(minx,miny,maxx,maxy,&x1,&y1,&z1,&x2,&y2,&z2,&clipflag,multiplier) > 0)
                  {
                  rac = Aroot;
                  conditionfound = 0;
                  while(rac != NULL)
                     {
                     Config2 = SCCtable[rac->Sindex].C;
                     Strat2 = SCCtable[rac->Sindex].S;
                     Domain2 = SCCtable[rac->Sindex].D;
                     if((CombinedCheckApplies(rac->idn,checkinstance,LACUTFAIL,rac->Lindex,rac->Sindex,
                                    Config2,Strat2,Domain2,
                                    &part1,&part2,&part3)) && (part2 > 0))
                        {

                        if(rac->RB_Tree_Edgelist != NULL)
                           {
                           LastEdge = NULL;
                           LastJ = -1;
                           j = GetNextArealIndex(rac, LastJ, 1, &LastEdge, cindex, &UsedTree);
                           LastJ = j;
                           }
                        else
                           {
                           j = 0;
                           UsedTree = 0;
                           }
                        while(j >= 0)
                           {
                           if(j == 0)
                              jj = rac->numverts - 1;
                           else
                              jj = j - 1;

                           x3 = rac->x[jj];
                           y3 = rac->y[jj];
                           z3 = rac->z[jj];
                           x4 = rac->x[j];
                           y4 = rac->y[j];
                           z4 = rac->z[j];
                           if(ClipLineToRegion(minx,miny,maxx,maxy,&x3,&y3,&z3,&x4,&y4,&z4,&clipflag,multiplier) > 0)
                              {
                              foundpair = AllCaseLineSegmentsIntersect(x1,y1,x2,y2,x3,y3,x4,y4,
                                             0.0001,&xi, &yi);
                              if(foundpair > 0)
                                 {
                                 ptpdist = Distance(xi,yi,rlc->x[0],rlc->y[0]);
                                 if(ptpdist > ActiveChecks[checkinstance].sensitivity)
                                    {
                                    ptpdist = Distance(xi,yi,rlc->x[rlc->numnodes-1],rlc->y[rlc->numnodes-1]);
                                    if(ptpdist > ActiveChecks[checkinstance].sensitivity)
                                       {
                                       switch(foundpair)
                                          {
                                          case 0: /** no intersection of any flavor ***/
                                             keepit = 0; break;
                                          case 1: /** an intersection at (xi,yi), away from any vertex of either feature ***/
                                             keepit = 1; break;
                                          case 2: /** (rlc->x[ii],rlc->y[ii]) and (rac->x[jj],rac->y[jj]) are connecting nodes **/
                                             ii2 = ii - 1;
                                             if(jj == 0)
                                                jj2 = rac->numverts - 1;
                                             else
                                                jj2 = jj - 1;
                                             keepit = 0;
                                             if(IntersectAcrossConnectingNode(rlc->x[ii2],rlc->y[ii2],
                                                      rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i],
                                                      rac->x[jj2],rac->y[jj2],rac->x[jj],rac->y[jj],rac->x[j],rac->y[j]) > 0)
                                                keepit = 1;
                                             break;
                                          case 3: /** (rlc->x[ii],rlc->y[ii]) and (rac->x[j],rac->y[j]) are connecting nodes **/
                                             ii2 = ii - 1;
                                             if(j == (rac->numverts - 1))
                                                jj2 = 0;
                                             else
                                                jj2 = j + 1;
                                             keepit = 0;
                                             if(IntersectAcrossConnectingNode(rlc->x[ii2],rlc->y[ii2],
                                                   rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i],
                                                   rac->x[jj],rac->y[jj],rac->x[j],rac->y[j],rac->x[jj2],rac->y[jj2]) > 0)
                                                keepit = 1;
                                             break;
                                          case 4: /** (rlc->x[i],rlc->y[i]) and (rac->x[jj],rac->y[jj]) are connecting nodes **/
                                             ii2 = i + 1;
                                             if(jj == 0)
                                                jj2 = rac->numverts - 1;
                                             else
                                                jj2 = jj - 1;
                                             keepit = 0;
                                             if(IntersectAcrossConnectingNode(rlc->x[ii],rlc->y[ii],
                                                   rlc->x[i],rlc->y[i],rlc->x[ii2],rlc->y[ii2],
                                                   rac->x[jj2],rac->y[jj2],rac->x[jj],rac->y[jj],rac->x[j],rac->y[j]) > 0)
                                                keepit = 1;
                                             break;
                                          case 5: /** (rlc->x[i],rlc->y[i]) and (rac->x[j],rac->y[j]) are connecting nodes **/
                                             ii2 = i + 1;
                                             if(j == (rac->numverts - 1))
                                                jj2 = 0;
                                             else
                                                jj2 = j + 1;
                                             keepit = 0;
                                             if(IntersectAcrossConnectingNode(rlc->x[ii],rlc->y[ii],
                                                   rlc->x[i],rlc->y[i],rlc->x[ii2],rlc->y[ii2],
                                                   rac->x[jj],rac->y[jj],rac->x[j],rac->y[j],rac->x[jj2],rac->y[jj2]) > 0)
                                                keepit = 1;
                                             break;
                                          case 6: /** (rlc->x[ii],rlc->y[ii]) is on rac, no connecting node **/
                                             ii2 = ii - 1;
                                             i2 = 1;
                                             if(PointOnLineSegment(rlc->x[ii2],rlc->y[ii2],
                                                   rac->x[jj],rac->y[jj],rac->x[j],rac->y[j], 0.001) == 0)
                                                {
                                                i2 = TwoPointsOnSameSideOfLine(rlc->x[ii2],rlc->y[ii2],rlc->x[i],rlc->y[i],
                                                       rac->x[jj],rac->y[jj],rac->x[j],rac->y[j]);
                                                }
                                             if(i2 == 0)
                                                keepit = 1;
                                             else
                                                keepit = 0;
                                             break;
                                          case 7: /** (rlc->x[i],rlc->y[i]) is on rac, no connecting node **/
                                             ii2 = i + 1;
                                             i2 = 1;
                                             if(PointOnLineSegment(rlc->x[ii2],rlc->y[ii2],
                                                      rac->x[jj],rac->y[jj],rac->x[j],rac->y[j], 0.001) == 0)
                                                {
                                                i2 = TwoPointsOnSameSideOfLine(rlc->x[ii2],rlc->y[ii2],rlc->x[ii],rlc->y[ii],
                                                       rac->x[jj],rac->y[jj],rac->x[j],rac->y[j]);
                                                }
                                             if(i2 == 0)
                                                keepit = 1;
                                             else
                                                keepit = 0;
                                             break;
                                          case 8: /** (rac->x[jj],rac->y[jj]) is on rlc, no connecting node **/
                                             if(jj == 0)
                                                jj2 = rac->numverts - 1;
                                             else
                                                jj2 = jj - 1;
                                             i2 = 1;
                                             if(PointOnLineSegment(rac->x[jj2],rac->y[jj2],
                                                   rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i], 0.001) == 0)
                                                {
                                                i2 = TwoPointsOnSameSideOfLine(rac->x[jj2],rac->y[jj2],rac->x[j],rac->y[j],
                                                          rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i]);
                                                }
                                             if(i2 == 0)
                                                keepit = 1;
                                             else
                                                keepit = 0;
                                             break;
                                          case 9: /** (rac->x[j],rac->y[j]) is on rlc, no connecting node **/
                                             if(j == (rac->numverts - 1))
                                                jj2 = 0;
                                             else
                                                jj2 = j + 1;
                                             i2 = 1;
                                             if(PointOnLineSegment(rac->x[jj2],rac->y[jj2],
                                                   rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i], 0.001) == 0)
                                                {
                                                i2 = TwoPointsOnSameSideOfLine(rac->x[jj2],rac->y[jj2],rac->x[j],rac->y[j],
                                                       rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i]);
                                                }
                                             if(i2 == 0)
                                                keepit = 1;
                                             else
                                                keepit = 0;
                                             break;
                                          }

                                       if(keepit > 0)
                                          {
                                          keepit = 0;
                                          ERc = (struct collection *) (malloc(SzC));
                                          if(ERc == NULL)
                                             {
                                             printf("allocation memory exhausted during linear feature condition analysis\n");
                                             printf("processing cannot continue\n");
                                             exit(-1);
                                             }
                                          ERc->x[0] = xi;
                                          ERc->y[0] = yi;
                                          ERc->x[1] = rac->x[jj];
                                          ERc->y[1] = rac->y[jj];
                                          ERc->z[1] = rac->x[jj];
                                          ERc->x[2] = rac->x[j];
                                          ERc->y[2] = rac->y[j];
                                          ERc->z[2] = rac->x[j];
                                          ERc->magnitude = 0.0;
                                          ERc->keyval = LACUTFAIL;
                                          ERc->Cnumber = Make_Correct(ERc->keyval,checkinstance);
                                          ERc->IDN = rlc->idn;
                                          ERc->Sindex = rlc->Sindex;
                                          ERc->ll1 = rlc;
                                          ERc->vertexindex1 = ii;
                                          ERc->aa = rac;
                                          ERc->vertexindex2 = jj;
                                          ERc->next = NULL;
                                          if(ERroot == NULL)
                                             {
                                             ERc->z[0] = InterpLinearElev(x1,y1,z1,x2,y2,z2,xi,yi);
                                             ERroot = ERc;
                                             conditionfound = 1;
                                             }
                                          else
                                             {
                                             ERn = ERroot;
                                             while(ERn != NULL)
                                                {
                                                  if((ERn->Cnumber == ERc->Cnumber) && (ERn->keyval == ERc->keyval) &&
                                                     (ERn->x[0] == ERc->x[0]) && (ERn->y[0] == ERc->y[0]) &&
                                                     (ERn->ll1 == ERc->ll1) && (ERn->aa == ERc->aa))
                                                   break;
   
                                                ERn = ERn->next;
                                                }
                                             if(ERn == NULL)
                                                {
                                                ERc->z[0] = InterpLinearElev(x1,y1,z1,x2,y2,z2,xi,yi);
                                                ERc->next = ERroot;
                                                ERroot = ERc;
                                                conditionfound = 1;
                                                }
                                             else
                                               free(ERc);
                                            }
                                         }

                                       }
                                    }
                                 }

                              }

                           if(conditionfound > 0)
                              {
                              conditionfound = 0;
                              j = -1;
                              }
                           else
                              {
                              if(rac->RB_Tree_Edgelist != NULL)
                                 {
                                 j = GetNextArealIndex(rac, LastJ, 1, &LastEdge, cindex, &UsedTree);
                                 LastJ = j;
                                 }
                              else
                                 {
                                 ++j;
                                 if(j >= rac->numverts)
                                    j = -1;
                                 }
                              }
                           } /*** while j >= 0 ***/
                        } /** end check applies to rac as part 2 ***/

                     rac = rac->next;
                     }
                  }
               }

            }


          if((CombinedCheckApplies(rlc->idn,checkinstance,LBNDUSHT,rlc->Lindex,rlc->Sindex,
                             Config1,Strat1,Domain1,&part1,&part2,&part3) > 0) &&
                        (part1 > 0))
             {
             j = 0;
             FoundCondition = 0;
             tolerancepad = FindRelevantSensitivity(LBNDUSHT,checkinstance,&tol2);
             while((j < rlc->numnodes) && 
                   (rlc->x[j] >= minx) && (rlc->x[j] <= maxx) && (rlc->y[j] >= miny) && (rlc->y[j] <= maxy))
                {
                tgtedge = 0;
                if(rlc->x[j] < LAEmaxx)
                    {
                    ptldist = Distance(rlc->x[j],rlc->y[j],LAEmaxx,rlc->y[j]);
                    if((ptldist > tolerancepad) && (ptldist < tol2))
                       {
                       tgtedge = 1;
                       FoundCondition = 1;
                       }
                    }
                if((FoundCondition  == 0) && (rlc->x[j] > LAEminx))
                    {
                    ptldist = Distance(rlc->x[j],rlc->y[j],LAEminx,rlc->y[j]);
                    if((ptldist > tolerancepad) && (ptldist < tol2))
                       {
                       tgtedge = 2;
                       FoundCondition = 1;
                       }
                    }
                if((FoundCondition  == 0) && (rlc->y[j] < LAEmaxy))
                    {
                    ptldist = Distance(rlc->x[j],rlc->y[j],rlc->x[j],LAEmaxy);
                    if((ptldist > tolerancepad) && (ptldist < tol2))
                       {
                       tgtedge = 3;
                       FoundCondition = 1;
                       }
                    }
                if((FoundCondition  == 0) && (rlc->y[j] > LAEminy))
                    {
                    ptldist = Distance(rlc->x[j],rlc->y[j],rlc->x[j],LAEminy);
                    if((ptldist > tolerancepad) && (ptldist < tol2))
                       {
                       tgtedge = 4;
                       FoundCondition = 1;
                       }
                    }
                if((FoundCondition > 0) && (tgtedge > 0))
                   {
                   if(j == 0)
                      jj = rlc->numnodes - 1;
                   else
                      jj = 0;
                   switch(tgtedge)
                      {
                      case 1:
                         ptldist2 = Distance(rlc->x[jj],rlc->y[jj],LAEmaxx,rlc->y[jj]);
                         if(ptldist2 < ptldist)
                            FoundCondition = 0;
                         break;
                      case 2:
                         ptldist2 = Distance(rlc->x[jj],rlc->y[jj],LAEminx,rlc->y[jj]);
                         if(ptldist2 < ptldist)
                            FoundCondition = 0;
                         break;
                      case 3:
                         ptldist2 = Distance(rlc->x[jj],rlc->y[jj],rlc->x[jj],LAEmaxy);
                         if(ptldist2 < ptldist)
                            FoundCondition = 0;
                         break;
                      case 4:
                         ptldist2 = Distance(rlc->x[jj],rlc->y[jj],rlc->x[jj],LAEminy);
                         if(ptldist2 < ptldist)
                            FoundCondition = 0;
                         break;
                      }
                   }
                if(FoundCondition > 0)
                   {
                   rpc = Proot;
                   while(rpc != NULL)
                      {
                      ptpdist1 = tolerancepad * 2.0;
                      if((rpc->numverts == -1) &&
                       (CombinedCheckApplies(rpc->idn,checkinstance,LBNDUSHT,rpc->Lindex,rpc->Sindex,
                            SCCtable[rpc->Sindex].C,SCCtable[rpc->Sindex].S,SCCtable[rpc->Sindex].D,
                              &part1,&part2,&part3) > 0) &&
                                  (part2 > 0))
                         {
                         ptpdist1 = Distance(rlc->x[j], rlc->y[j],rpc->x[0],rpc->y[0]);

                         if(ptpdist1 <= tolerancepad)
                            {
                            FoundCondition = 0;
                            break;
                            }
                         }
                      rpc = rpc->next;
                      }
                   }
                if(FoundCondition > 0)
                   {
                   rln = root;
                   while((rln != NULL) && (FoundCondition > 0))
                      {
                      ptpdist1 = tolerancepad * 2.0;
                      if((rln != rlc) &&
                          (CombinedCheckApplies(rln->idn,checkinstance,LBNDUSHT,rln->Lindex,rln->Sindex,
                               SCCtable[rln->Sindex].C,SCCtable[rln->Sindex].S,SCCtable[rln->Sindex].D,
                                    &part1,&part2,&part3) > 0) && (part2 > 0))
                         {
                         k = 1;
                         while(k < rln->numnodes )
                            {
                            km1 = k - 1;
                            ptpdist1 = PointToLineDist2DwithFlag(rlc->x[j], rlc->y[j],
                                     rln->x[km1], rln->y[km1], rln->x[k], rln->y[k],&T);
                            if(ptpdist1 <= tolerancepad) /** call a match witl rlc end node ***/
                               {
                               FoundCondition = 0;
                               break; /** from while rln loop ***/
                               }
                            ++k;
                            }
                         } /** end if check applies as part3 ***/
                      rln = rln->next;
                      } /** end while rln ***/
                   }
                if(FoundCondition > 0)
                   {
                   rac = Aroot;
                   while((rac != NULL) && (FoundCondition > 0))
                      {
                      ptpdist1 = tolerancepad * 2.0;
                      if((CombinedCheckApplies(rac->idn,checkinstance,LBNDUSHT,rac->Lindex,rac->Sindex,
                               SCCtable[rac->Sindex].C,SCCtable[rac->Sindex].S,SCCtable[rac->Sindex].D,
                                  &part1,&part2,&part3) > 0) && (part2 > 0))
                         {
                         if(rac->RB_Tree_Edgelist == NULL)
                            ptpdist1 = PointToSmallArealDist2D(rlc->x[j], rlc->y[j],rac,&k,&T);
                         else
                            ptpdist1 = PointToLargeArealDist2D(rlc->x[j], rlc->y[j],rac,&k,&T);

                         if((ptpdist1 > -1.0) && (ptpdist1 <= tolerancepad)) 
                            {
                            FoundCondition = 0;
                            break; /** break from the while rac loop ***/
                            }
                         } /* end if combined check applies ***/
                      rac = rac->next;
                      }
                   }
                if(FoundCondition > 0)
                   {
                   FoundCondition = 0;
                   ERc = (struct collection *) (malloc(SzC));
                   if(ERc == NULL)
                     {
                     printf("allocation memory exhausted during linear feature condition analysis (Z-shear analysis)\n");
                     printf("processing cannot continue\n");
                     exit(-1);
                     }
                   ERc->magnitude = ptldist;
                   ERc->x[0] = rlc->x[j];
                   ERc->y[0] = rlc->y[j];
                   ERc->z[0] = rlc->z[j];
                   ERc->IDN = rlc->idn;
                   ERc->Sindex = rlc->Sindex;
                   ERc->aa = NULL; /** RawAreal **/
                   ERc->bb = NULL; /** RawBridge **/
                   ERc->pp = NULL; /** RawPoint **/
                   ERc->le = NULL; /** ListOfEdges **/
                   ERc->poly = NULL; /** ThePolys **/
                   ERc->ll1 = rlc;
                   ERc->ll2 = NULL;
                   ERc->keyval = LBNDUSHT;
                   ERc->Cnumber = Make_Correct(LBNDUSHT,checkinstance);

                   if(ERroot == NULL)
                      {
                      ERroot = ERc;
                      ERroot->next = NULL;                                }
                   else
                      {
                      ERn = ERroot;
                      while(ERn != NULL)
                         {
                         if((ERn->keyval == ERc->keyval) && (ERc->Cnumber == ERn->Cnumber) && (ERn->ll1 == ERc->ll1))
                            {
                            if((ERn->x[0] == ERc->x[0]) && (ERn->y[0] == ERc->y[0]))
                               break;
                            }
                         ERn = ERn->next;
                         }
                      if(ERn == NULL)
                         {
                         ERc->next = ERroot;
                         ERroot = ERc;
                         }
                      else
                         {
                         free(ERc);
                         }
                      }
                   }
                j += rlc->numnodes - 1;
                }
             } /*** end looking for LBNDUSHT ***/



         if(CombinedCheckApplies(rlc->idn,checkinstance,LLINTNOEND,rlc->Lindex,rlc->Sindex,Config1,Strat1,Domain1,&part1,&part2,&part3) > 0)
            {
            if(part1 > 0)
               {
               rln = root;
               while(rln != NULL)
                  {
                  if(rln == rlc)
                     {
                     if(rlc->next == NULL)
                        break;
                     else
                        rln = rlc->next;
                     }
                  Config2 = SCCtable[rln->Sindex].C;
                  Strat2 = SCCtable[rln->Sindex].S;
                  Domain2 = SCCtable[rln->Sindex].D;
                  if(CombinedCheckApplies(rln->idn,checkinstance,LLINTNOEND,rln->Lindex,rln->Sindex,Config2,Strat2,Domain2,&part1,&part2,&part3) > 0)
                     {
                     if(part2 > 0)
                        {
                        keepit = 0;
                        for(i=1; i<rlc->numnodes; i++)
                           {
                           ii = i - 1;
                           x1 = rlc->x[ii];
                           y1 = rlc->y[ii];
                           z1 = rlc->z[ii];
                           x2 = rlc->x[i];
                           y2 = rlc->y[i];
                           z2 = rlc->z[i];
                           if(ClipLineToRegion(minx,miny,maxx,maxy,&x1,&y1,&z1,&x2,&y2,&z2,&clipflag,multiplier) > 0)
                              {
                              for(j=1; j<rln->numnodes; j++)
                                 {
                                 jj = j - 1;
                                 x3 = rln->x[jj];
                                 y3 = rln->y[jj];
                                 z3 = rln->z[jj];
                                 x4 = rln->x[j];
                                 y4 = rln->y[j];
                                 z4 = rln->z[j];
                                 if(ClipLineToRegion(minx,miny,maxx,maxy,&x3,&y3,&z3,&x4,&y4,&z4,&clipflag,multiplier) > 0)
                                    {
                                    /***if(LineSegmentsIntersect(x1,y1,x2,y2,x3,y3,x4,y4, &xi, &yi) > 0) ***/
                                    if(AllCaseLineSegmentsIntersect(rlc->x[ii],rlc->y[ii],rlc->x[i],rlc->y[i],
                                                         rln->x[jj],rln->y[jj],rln->x[j],rln->y[j],0.001,&xi,&yi) > 0)
                                       {
                                       ptpdist1 = Distance(xi,yi,rlc->x[0],rlc->y[0]);
                                       if(ptpdist1 > ActiveChecks[checkinstance].sensitivity)
                                          {
                                          ptpdist1 = Distance(xi,yi,rlc->x[rlc->numnodes-1],rlc->y[rlc->numnodes-1]);
                                          if(ptpdist1 > ActiveChecks[checkinstance].sensitivity)
                                             {
                                             keepit = 1;
                                             }
                                          }
                                       if(keepit > 0)
                                          {
                                          keepit = 0;
                                          ERc = (struct collection *) (malloc(SzC));
                                          if(ERc == NULL)
                                             {
                                             printf("allocation memory exhausted during linear feature condition analysis\n");
                                             printf("processing cannot continue\n");
                                             exit(-1);
                                             }
                                          ERc->x[0] = xi;
                                          ERc->y[0] = yi;
                                          ERc->magnitude = 0.0;
                                          ERc->keyval = LLINTNOEND;
                                          ERc->Cnumber = Make_Correct(ERc->keyval,checkinstance);
                                          ERc->IDN = rlc->idn;
                                          ERc->Sindex = rlc->Sindex;
                                          ERc->ll1 = rlc;
                                          ERc->vertexindex1 = ii;
                                          ERc->vertexindex1 = ii;
                                          ERc->ll2 = rln;
                                          ERc->vertexindex2 = jj;
                                          ERc->next = NULL;
                                          if(ERroot == NULL)
                                             {
                                             ERc->z[0] = InterpLinearElev(x1,y1,z1,x2,y2,z2,xi,yi);
                                             ERroot = ERc;
                                             }
                                          else
                                             {
                                             ERn = ERroot;
                                             while(ERn != NULL)
                                                {
                                                  if((ERn->Cnumber == ERc->Cnumber) && (ERn->keyval == ERc->keyval) &&
                                                     (ERn->x[0] == ERc->x[0]) && (ERn->y[0] == ERc->y[0]) &&
                                                     (( (ERn->ll1 == ERc->ll1) && (ERn->ll2 == ERc->ll2)) ||
                                                      ((ERn->ll1 == ERc->ll2) && (ERn->ll2 == ERc->ll1))))
                                                   break;
                                                ERn = ERn->next;
                                                }
                                             if(ERn == NULL)
                                                {
                                                ERc->z[0] = InterpLinearElev(x1,y1,z1,x2,y2,z2,xi,yi);
                                                ERc->next = ERroot;
                                                ERroot = ERc;
                                                }
                                             else
                                               free(ERc);
                                            }
                                          break;
                                          }
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                  rln = rln->next;
                  }
               }
            }  /*** end LLINTNOEND check ***/

         if((rlc->numnodes > 1) &&
            (CombinedCheckApplies(rlc->idn,checkinstance,LONGSEG,rlc->Lindex,rlc->Sindex,
                                 Config1, Strat1, Domain1,
                                 &part1,&part2,&part3)))
           {
             if(part1 > 0)
                {
                for(jj=1; jj<rlc->numnodes; jj++)
                   {
                   if((rlc->x[jj] >= minx) && (rlc->x[jj] <= maxx) && (rlc->y[jj] >= miny) && (rlc->y[jj] <= maxy))
                      {
                      if((rlc->x[jj] != rlc->x[jj-1]) ||
                          (rlc->y[jj] != rlc->y[jj-1]) ||
                          (rlc->z[jj] != rlc->z[jj-1]))
                         {
                         segmentlength = Distance3D(rlc->x[jj-1],rlc->y[jj-1],rlc->z[jj-1],
                                           rlc->x[jj],rlc->y[jj],rlc->z[jj]);
                         if(SensitivityCheck(GTEQ, LONGSEG, checkinstance, segmentlength) > 0)
                            {
                            keepit = ConditionOnDataEdge(rlc->x[jj],rlc->y[jj],rlc->x[jj-1],rlc->y[jj-1],NAroot,Proot,NLroot,
                                                NULL, NULL, rlc,   // is area, point, line
                                                   NorthCount,SouthCount,EastCount,WestCount,checkinstance);
                            if(keepit > 0)
                               {
                               FwriteObjectAndMagnitude(LONGSEG,Make_Correct(LONGSEG,checkinstance),
                                     rlc->idn,
                                     rlc->Sindex,G_LINE,rlc->Lindex,rlc->localID,-1.0,-1.0,segmentlength,
                                     &rlc->x[jj-1], &rlc->y[jj-1],&rlc->z[jj-1],2, 1);
                               }
                            }
                         }
                      }
                   }
                }
           }





         }  /**** for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++) ****/

      rlc = rlc->next;
      }  /*** end while rlc != NULL ****/


   rac = Aroot;
   while(rac != NULL)
      {
      Config1 = SCCtable[rac->Sindex].C;
      Strat1 = SCCtable[rac->Sindex].S;
      Domain1 = SCCtable[rac->Sindex].D;
      for(checkinstance = 0; checkinstance < TtlActiveChecks; checkinstance++)
         {
         if((rac->numverts > 1) &&
            (CombinedCheckApplies(rac->idn,checkinstance,LONGSEG,rac->Lindex,rac->Sindex,
                                 Config1, Strat1, Domain1,
                                 &part1,&part2,&part3)))
             {
             if(part1 > 0)
                {
                if(rac->RB_Tree_Edgelist != NULL)
                   {
                   LastEdge = NULL;
                   LastJ = -1;
                   jj = GetNextArealIndex(rac, LastJ, 1, &LastEdge, cindex, &UsedTree);
                   LastJ = jj;
                   }
                else
                   {
                   jj = 0;
                   UsedTree = 0;
                   }
                while(jj >= 0)
                   {
                   if((rac->x[jj] >= minx) && (rac->x[jj] <= maxx) && (rac->y[jj] >= miny) && (rac->y[jj] <= maxy))
                      {
                      if(jj == 0)
                         j = rac->numverts -1;
                      else
                         j = jj - 1;
                      if((rac->x[jj] != rac->x[j]) ||
                          (rac->y[jj] != rac->y[j]) ||
                          (rac->z[jj] != rac->z[j]))
                         {
                         segmentlength = Distance3D(rac->x[j],rac->y[j],rac->z[j],
                                           rac->x[jj],rac->y[jj],rac->z[jj]);
                         if(SensitivityCheck(GTEQ, LONGSEG, checkinstance, segmentlength) > 0)
                            {
                            keepit = ConditionOnDataEdge(rac->x[jj],rac->y[jj],rac->x[j],rac->y[j],Aroot,Proot,root,
                                                rac, NULL, NULL,   // is area, point, line
                                                   NorthCount,SouthCount,EastCount,WestCount,checkinstance);
                            if(keepit > 0)
                               {
                               VX[0] = rac->x[j];
                               VY[0] = rac->y[j];
                               VZ[0] = rac->z[j];
                               VX[1] = rac->x[jj];
                               VY[1] = rac->y[jj];
                               VZ[1] = rac->z[jj];

                               FwriteObjectAndMagnitude(LONGSEG,Make_Correct(LONGSEG,checkinstance),
                                     rac->idn,
                                     rac->Sindex,G_LINE,rac->Lindex,rac->localID,-1.0,-1.0,segmentlength,
                                     VX,VY,VZ,2, 1);
                               }
                            }
                         }
                      }
                   if(rac->RB_Tree_Edgelist != NULL)
                      {
                      jj = GetNextArealIndex(rac, LastJ, 1, &LastEdge, cindex, &UsedTree);
                      LastJ = jj;
                      }
                   else
                      {
                      ++jj;
                      if(jj >= rac->numverts)
                         jj = -1;
                      }
                   }
                }
           }

         }
      rac = rac->next;
      }

   ERc = ERroot;
   while(ERc != NULL)
      {
      switch(ERc->keyval)
         {
         case LACUTFAIL:
            FwritePointAndTwoObjects(ERc->keyval,ERc->Cnumber,ERc->x[0],ERc->y[0],ERc->z[0],
                               ERc->ll1->Sindex,G_LINE,ERc->ll1->Lindex,ERc->ll1->localID,
                               ERc->ll1->idn,-1.0,-1.0,&ERc->ll1->x[ERc->vertexindex1],
                               &ERc->ll1->y[ERc->vertexindex1],&ERc->ll1->z[ERc->vertexindex1],2,
                               ERc->aa->Sindex,G_AREAL,ERc->aa->Lindex,ERc->aa->localID,
                               ERc->aa->idn,-1.0,-1.0,&ERc->x[1],&ERc->y[1],&ERc->z[1],2);
            break;

         case LLINTNOEND:
            FwritePointAndTwoObjects(ERc->keyval,ERc->Cnumber,ERc->x[0],ERc->y[0],ERc->z[0],
                               ERc->ll1->Sindex,G_LINE,ERc->ll1->Lindex,ERc->ll1->localID,
                               ERc->ll1->idn,-1.0,-1.0,&ERc->ll1->x[ERc->vertexindex1],
                               &ERc->ll1->y[ERc->vertexindex1],&ERc->ll1->z[ERc->vertexindex1],2,
                               ERc->ll2->Sindex,G_LINE,ERc->ll2->Lindex,ERc->ll2->localID,
                               ERc->ll2->idn,-1.0,-1.0,&ERc->ll2->x[ERc->vertexindex2],
                               &ERc->ll2->y[ERc->vertexindex2],&ERc->ll2->z[ERc->vertexindex2],2);
            break;

         case LBNDUSHT:
            FwriteObjectAndMagnitude(ERc->keyval,ERc->Cnumber,
                        ERc->ll1->idn, ERc->Sindex,G_LINE,ERc->ll1->Lindex,ERc->ll1->localID,
                           -1.0,-1.0,ERc->magnitude,ERc->x, ERc->y, ERc->z,1, 1);
            break;

         }

      ERn = ERc;
      ERc = ERc->next;
      free(ERn);
      }
   return;

}



