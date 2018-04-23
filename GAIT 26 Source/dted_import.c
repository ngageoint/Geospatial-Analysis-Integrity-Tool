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
#include "share.h"
#include "shapefil.h"
#include <ctype.h>  


#define DTEDDEBUG 0
 
extern int SH_Points_Done;
extern int SH_Linears_Done;
extern int SH_Areals_Done;
extern int SH_bad_objects;
extern int SH_BadCoords;
extern int SH_Null_Shapes;
extern int SH_Zero_Shapes;
extern int SH_Bad_DBF;

extern unsigned char TIM_STRING;
extern unsigned char TIM_INTEGER;
extern unsigned char TIM_ENUMERANT;
extern unsigned char TIM_DOUBLE;
extern unsigned char TIM_UNI;
extern unsigned char TIM_METRE;
extern double maxgridspace;

extern unsigned long int gridpointsread;

typedef struct {
  unsigned char  uhl[3];
  unsigned char  one[1];
  unsigned char xdeg[3];
  unsigned char xmin[2];
  unsigned char xsec[2];
  unsigned char xhem[1];
  unsigned char ydeg[3];
  unsigned char ymin[2];
  unsigned char ysec[2];
  unsigned char yhem[1];
  unsigned char xspace[4];
  unsigned char yspace[4];
  unsigned char AVA[4];        
  unsigned char classif[3];
  unsigned char refnum[12];    
  unsigned char num_lines[4];  
  unsigned char num_points[4]; 
  unsigned char multacc[1];
  unsigned char resvd[24];
} user_header_label;



typedef struct {
  unsigned char DSI[3];  
  unsigned char dsi_junk1[56];   
  unsigned char dted_string[5];  

  unsigned char dsi_junk2[85];   
  unsigned char dted_type[10];   
  unsigned char dsi_junk3[489];  

} data_set_ident_rec;



typedef struct {
  unsigned char rec_sent[3];      
  unsigned char abs_hor_acc[4];   
  unsigned char abs_ver_acc[4];   
  unsigned char rel_hor_acc[4];   
  unsigned char rel_ver_acc[4];   
  unsigned char acc_stuff[2681];  
} accuracy_record;




double xstart,ystart,xspace,yspace;
int dted_xnum,dted_ynum;
int AbsVertAcc,RelVertAcc,AbsHorAcc,RelHorAcc;
char DTED_STRING[100];

extern void update_shape_import(char *allshapes,char *curr_file,int files_done, 
				int dbfdone, int dbftotal,
				int points_done,int linears_done,int areals_done,unsigned long int gptsdone,
				int DONE, int extra_type,int extra1, int extra2, int badstuff,
				int badstuff2, int nullshapes, int zeropart, int baddbf);


int ValidateACC(accuracy_record thisACC)
{
  char absvertacc[10];
  char relvertacc[10];
  char abshoracc[10];
  char relhoracc[10];


  if(strncmp((const char *)thisACC.rec_sent,"ACC",3))
    {
      printf("Did not get \"ACC\" as first block of Accuracy Record.\n"); 
      return 0;
    }
  
  strncpy(absvertacc,(const char *)thisACC.abs_ver_acc,4); absvertacc[4] = '\0';
  strncpy(relvertacc,(const char *)thisACC.rel_ver_acc,4); relvertacc[4] = '\0';
  strncpy(abshoracc ,(const char *)thisACC.abs_hor_acc,4); abshoracc [4] = '\0';
  strncpy(relhoracc ,(const char *)thisACC.rel_hor_acc,4); relhoracc [4] = '\0';

  sscanf(absvertacc,"%d",&AbsVertAcc);
  sscanf(relvertacc,"%d",&RelVertAcc);
  sscanf(abshoracc ,"%d",&AbsHorAcc );
  sscanf(relhoracc ,"%d",&RelHorAcc );

  if(DTEDDEBUG==1)
    {
      printf("Absolute Vertical Accuracy: %d\n"  ,AbsVertAcc);
      printf("Relative Vertical Accuracy: %d\n"  ,RelVertAcc);
      printf("Absolute Horizontal Accuracy: %d\n",AbsHorAcc);
      printf("Relative Horizontal Accuracy: %d\n",RelHorAcc);
    }
  return 1;
}




int ValidateDSI(data_set_ident_rec thisDSI)
{

  
  if(strncmp((const char *)thisDSI.DSI,"DSI",3))
    {
      printf("Did not get \"DSI\" as first block of Data Set Identification (DSI) Record.\n"); 
      return 0;
    }

  return 1;
}




int ValidateUHL(user_header_label thisUHL)
{
  int xdeg,xmin,xsec,Ixspace;
  int ydeg,ymin,ysec,Iyspace;
  char   cxdeg[5],cxmin[5],cxsec[5],cxspace[5], cxnum[5];
  char   cydeg[5],cymin[5],cysec[5],cyspace[5], cynum[5];
  char vertacc[5],classif[5];
  double xspace_sec,yspace_sec;


  if(strncmp((const char *)thisUHL.uhl,"UHL",3))
    {
      printf("Did not get \"UHL\" as first block of User Header Label.\n"); 
      return 0;
    }
  
  if(thisUHL.one[0]!='1')
    {
      printf("Did not get \"1\" as second block of User Header Label.\n"); 
      return 0;      
    }
  
  strncpy(cxdeg,(const char *)thisUHL.xdeg,3); cxdeg[3] = '\0';
  strncpy(cxmin,(const char *)thisUHL.xmin,2); cxmin[2] = '\0';
  strncpy(cxsec,(const char *)thisUHL.xsec,2); cxsec[2] = '\0';
  strncpy(cydeg,(const char *)thisUHL.ydeg,3); cydeg[3] = '\0';
  strncpy(cymin,(const char *)thisUHL.ymin,2); cymin[2] = '\0';
  strncpy(cysec,(const char *)thisUHL.ysec,2); cysec[2] = '\0';

  sscanf(cxdeg,"%d",&xdeg);
  sscanf(cxmin,"%d",&xmin);
  sscanf(cxsec,"%d",&xsec);
  sscanf(cydeg,"%d",&ydeg);
  sscanf(cymin,"%d",&ymin);
  sscanf(cysec,"%d",&ysec);


  strncpy(cxspace,(const char *)thisUHL.xspace,4); cxspace[4] = '\0';
  sscanf(cxspace,"%d",&Ixspace);
  xspace_sec = Ixspace/10.0;

  strncpy(cyspace,(const char *)thisUHL.yspace,4); cyspace[4] = '\0';
  sscanf(cyspace,"%d",&Iyspace);
  yspace_sec = Iyspace/10.0;

  strncpy(vertacc,(const char *)thisUHL.AVA,    4); vertacc[4] = '\0';
  strncpy(classif,(const char *)thisUHL.classif,3); classif[3] = '\0';



  strncpy(cxnum,(const char *)thisUHL.num_lines,4); cxnum[4] = '\0';
  sscanf(cxnum,"%d",&dted_xnum);

  strncpy(cynum,(const char *)thisUHL.num_points,4); cynum[4] = '\0';
  sscanf(cynum,"%d",&dted_ynum);


  if(DTEDDEBUG==1)
    {
      printf("Hemispheres: %c %c\n",thisUHL.xhem[0],thisUHL.yhem[0]);
    }

  xstart = (double)xdeg + (double)xmin/60.0 + (double)xsec/3600.0;
  ystart = (double)ydeg + (double)ymin/60.0 + (double)ysec/3600.0;
  xspace = xspace_sec/3600.0;
  yspace = yspace_sec/3600.0;


  if(thisUHL.yhem[0]=='S')
    {
      ystart = ystart * -1.0;
    }
  if(thisUHL.xhem[0]=='W')
    {
      xstart = xstart * -1.0;
    }


  if(DTEDDEBUG==1)
    {
      printf("X start point: %d %d %d = %lf\n",xdeg,xmin,xsec,xstart);
      printf("Y start point: %d %d %d = %lf\n",ydeg,ymin,ysec,ystart);
      printf("xspacing, yspacing: (arc second: %lf %lf)  (degree: %lf %lf)\n",
	     xspace_sec,yspace_sec,xspace,yspace);
      printf("xnum, ynum: %d %d\n",dted_xnum,dted_ynum);
      printf("Absolute (90%%) vertical accuracy: %s\n",vertacc);
      printf("Multiple Accuracy [0,1]: %c\n",thisUHL.multacc[0]);
      printf("Classification level [Secret,Confidential,Unclassified,Restricted]: %s\n",classif);
    }
  return 1;
}




void sh_update_shape_import(char *alldata, int files_done)
{
  update_shape_import(alldata,"", files_done,0,0,
		      SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
		      1,0,0,0,SH_bad_objects,SH_BadCoords,SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);
}





int FindClassCode(char *class_in)
{
  int i;
  extern int GIFD_CLASS_NUM;
  extern char **gifd_class_codes;


  if(!strcmp(class_in,"TIMGRID"))
    {
      return -1;
    }


  for(i=0;i<GIFD_CLASS_NUM;i++)
    {
      if(!strcmp(gifd_class_codes[i],class_in))
	{
	  return i;
	}
    }

  printf("Unable to find classification code %s\n",class_in);
  return 1;

}



int FindAttrCode(char *attr_in)
{
  int i;
  extern int GIFD_ATTR_NUM;
  extern char **gifd_attr_codes;


  for(i=0;i<GIFD_ATTR_NUM;i++)
    {
      if(!strcmp(gifd_attr_codes[i],attr_in))
	{
	  return i;
	}
    }

  printf("Unable to find attribute code %s\n",attr_in);
  return 1;

}



void tim_swap(char *junk)
{
  char c;
  c = junk[0];
  junk[0] = junk[1];
  junk[1] = c;
}


long int FindUHL(FILE *infile)
{
  

  char mychar1,mychar2,mychar3;
  long int saveposn1,saveposn2;
  int i;

  

  for(i=0;i<5000;i++)
    {
      saveposn1 = ftell(infile);

      fread(&mychar1,1,1,infile);
      if(mychar1=='U')
	{
	  saveposn2 = ftell(infile);

	  fread(&mychar2,1,1,infile);
	  fread(&mychar3,1,1,infile);

	  if((mychar2=='H')&&(mychar3=='L'))
	    {
	      return saveposn1;
	    }
	  else
	    {
	      fseek(infile,saveposn2,SEEK_SET);
	    }
	}
    }
  return -1;
}


int UseThisGrid(double x1,double y1, double x2, double y2)
{
  extern int CLIPEXTRACT;
  extern double CLIPLLX;
  extern double CLIPLLY;
  extern double CLIPURX;
  extern double CLIPURY;

  if(CLIPEXTRACT==1)
  { 
	if(
		(x1<CLIPLLX) &&
		(x2<CLIPLLX)
	  )
	{
      return 0;
	}
	if(
		(y1<CLIPLLY) &&
		(y2<CLIPLLY)
	  )
	{
      return 0;
	}
	if(
		(x1>CLIPURX) &&
		(x2>CLIPURX)
	  )
	{
      return 0;
	}
	if(
		(y1>CLIPURY) &&
		(y2>CLIPURY)
	  )
	{
      return 0;
	}
  }
  return 1;
}


int AddDtedFile(FILE *infile,char *dtedfile,char *lodoverride)
{
  FILE *dted_in;
  long int fileposn;
  user_header_label myUHL;
  data_set_ident_rec myDSI;
  char temp[1000];


  dted_in = fopen(dtedfile,"rb");
  if(dted_in==NULL)
    {
      printf("unable to open DTED file %s\n",dtedfile);
      return 0;
    }
  
  fileposn = FindUHL(dted_in);
  if(fileposn>=0)
    {
      fseek(dted_in,fileposn,SEEK_SET);
    }
  else
    {
      printf("Could not find UHL in file!\n");
      return 0;
    }
    
  
  fread(&myUHL,sizeof(user_header_label),1,dted_in);
  
  if(ValidateUHL(myUHL)==0)
    {
      return 0;
    }
  
  
  fread(&myDSI,sizeof(data_set_ident_rec),1,dted_in);
    
  if(ValidateDSI(myDSI)==0)
    {
      return 0;
    }
  


  if(!strcmp(lodoverride,"qq"))
  {
    if((dted_xnum==121)||(dted_ynum==121))
    {
      sprintf(temp,"\"DTED 0\"");
    }
    else if((dted_xnum==1201)||(dted_ynum==1201))
    {
      sprintf(temp,"\"DTED 1\"");
    }
    else if((dted_xnum==3601)||(dted_ynum==3601))
    {
      sprintf(temp,"\"DTED 2\"");
    }
    else
    {
      sprintf(temp,"\"%dx%d DTED format\"",dted_xnum,dted_ynum);
    }
  }
  else
  {
    sprintf(temp,"%s",lodoverride);
  }

  fprintf(infile,"DTED %s \"%s\"\n",temp,dtedfile);

  fclose(dted_in);


  return 1;
}


int import_dted(char *dtedfile, char *allshapes, int files_done, char *lod_override, int addtolong, int addtoshort)
{
  FILE *dted_in;
  user_header_label myUHL;
  data_set_ident_rec myDSI;
  accuracy_record myACC;
  unsigned char junk[80];
  short int **elev;
  int i,j, shortsize=sizeof(short int);
  int first_axis=2;  
  int index,index2;
  struct sh_long_attr_list  SH_SAC_LONG [SH_LISTLEN];   
  struct sh_short_attr_list SH_SAC_SHORT[SH_LISTLEN]; 
  int sh_sac_short_index=0, sh_sac_long_index=0;
  int short_style=1;
  int elev_index;
  int bad_type = 1;
  long int fileposn;
  int xindex,yindex;
  extern unsigned char OverLappingGrids[365][185];
  extern int num_grids;
  extern long int TestNumGridNodes;
  extern int SzD,SzI,SH_Grids_Prev;
  extern int EGRIDNODELIMIT;
  extern int USE_LITTLE;
  extern FILE *gridbin;
  extern void ReSetGridFile();
  extern void sh_testxyglobal(double x,double y,double z);
  extern int SH_FindSCC_LONG(int type, int scc, int numsac, 
			     struct sh_long_attr_list SAC_LONG[], int record_num);
  extern int SH_FindSCC_SHORT(int type, int scc, int numsac, 
			      struct sh_short_attr_list SAC_SHORT[], int record_num);

  extern void SH_AddShortAttribute(struct sh_short_attr_list SAC_SHORT[],int addindex,int attr_code);

  extern int SH_AddLongAttribute(struct sh_long_attr_list SAC_LONG[],int addindex,int attr_code, 
				 unsigned char unit_code, unsigned char scale_code, unsigned char SEDRIStype, 
				 int Ivalue, double Dvalue, char Cvalue[]);


  static int pass=0;

  sh_sac_short_index = 0;
  sh_sac_long_index  = 0;



  printf("Importing DTED file %s with %s\n",dtedfile,lod_override);


  for(i=0;i<SH_LISTLEN;i++)
    {
      SH_SAC_LONG [i].lod_stuff = NULL;
      SH_SAC_SHORT[i].lod_stuff = NULL;
      SH_SAC_LONG [i].nam_stuff = NULL;
      SH_SAC_SHORT[i].nam_stuff = NULL;
      SH_SAC_LONG [i].Cvalue    = NULL;
    }


  pass = pass + 1;


  elev_index         = -1; 



  
  dted_in = fopen(dtedfile,"rb");
  if(dted_in==NULL)
    {
      printf("unable to open DTED file %s\n",dtedfile);
      return 0;
    }
  


  if(bad_type==1)
    {
      fileposn = FindUHL(dted_in);
      if(fileposn>=0)
	{
	  fseek(dted_in,fileposn,SEEK_SET);
	}
      else
	{
	  printf("Could not find UHL in file!\n");
	  return 0;
	}
    }
  
  
  fread(&myUHL,sizeof(user_header_label),1,dted_in);
  
  if(ValidateUHL(myUHL)==0)
    {
	  fclose(dted_in);
      return 0;
    }
  

  if(UseThisGrid(
	              xstart,
	              ystart,
	              xstart+((dted_xnum-1)*xspace),
				  ystart+((dted_ynum-1)*yspace))==0)
  {
	
	printf("...skipping due to being outside import rectangle\n");
	fclose(dted_in);
    return 1; 
  }
  
  fread(&myDSI,sizeof(data_set_ident_rec),1,dted_in);
  
  
  if(ValidateDSI(myDSI)==0)
    {
      return 0;
    }
  
  
  fread(&myACC,sizeof(accuracy_record)   ,1,dted_in);
  
  if(ValidateACC(myACC)==0)
    {
      return 0;
    }



  




  sprintf(DTED_STRING,"-1 %d %s",C_GRID,lod_override);


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
  




  index  = SH_FindSCC_LONG (9,elev_index,sh_sac_long_index, SH_SAC_LONG,-1);
  index2 = SH_FindSCC_SHORT(9,elev_index,sh_sac_short_index,SH_SAC_SHORT,-1);

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
     

  


  
  if(TestNumGridNodes>EGRIDNODELIMIT)
    {
      
      ReSetGridFile();
      TestNumGridNodes = 0;
      SH_Grids_Prev = num_grids;
    }



  if((xstart>180)||(xstart<-180))
    {
      printf("bad DTED file with x-coord %lf\n",xstart);
      printf("should be -180 to 180\n");
    }

  if((ystart>90)||(ystart<-90))
    {
      printf("bad DTED file with y-coord %lf\n",ystart);
      printf("should be -90 to 90\n");
    }


  xindex = (int)(xstart + 180.2);
  yindex = (int)(ystart + 90.2);


  OverLappingGrids[xindex][yindex] = OverLappingGrids[xindex][yindex] + 1;


  



 if((yspace*100000)>maxgridspace)
    {
      maxgridspace = yspace*100000;
    }


  fwrite(&index ,SzI,1,gridbin);
  fwrite(&index2,SzI,1,gridbin);
  fwrite(&xspace,SzD,1,gridbin);
  fwrite(&yspace,SzD,1,gridbin);
  fwrite(&xstart,SzD,1,gridbin);
  fwrite(&ystart,SzD,1,gridbin);  
  fwrite(&first_axis,SzI,1,gridbin);
  fwrite(&dted_xnum,SzI,1,gridbin);
  fwrite(&dted_ynum,SzI,1,gridbin);



  if(short_style==1)
    {
      fwrite(&shortsize,SzI,1,gridbin);
    }




  elev = (short int **)malloc(sizeof(short int *)*dted_xnum);

  for(i=0;i<dted_xnum;i++)
  {
    elev[i] = (short int *)malloc(sizeof(short int)*dted_ynum);
    if(elev[i]==NULL)
    {
      printf("Import DTED: could not malloc %d\n",dted_ynum);
      exit(-1);
    }
  }


 

  for(i=0;i<dted_xnum;i++)
    {
      fread(&junk,8,1,dted_in);


      
      fread(&elev[i][0],2,dted_ynum,dted_in);

     
      
	  for(j=0;j<dted_ynum;j++)
	    {
		  if(USE_LITTLE==1)
		  {
            

			tim_swap((char *)&elev[i][j]);
		  }


	      

		  if((i==0)&&(j==0))
		  {
            sh_testxyglobal(xstart+((dted_xnum-1)*xspace),ystart+((dted_ynum-1)*yspace),elev[i][j]);
		  }

          sh_testxyglobal(xstart,ystart,elev[i][j]);
	    }
  
       fread(&junk,4,1,dted_in);
    }


  for(i=0;i<dted_ynum;i++)
  {
    if(i%10==0)
	{
	  update_shape_import(allshapes,dtedfile, files_done,0,0,
			      SH_Points_Done,SH_Linears_Done,SH_Areals_Done,gridpointsread,
			      0,0,0,0,SH_bad_objects,SH_BadCoords,SH_Null_Shapes,SH_Zero_Shapes,SH_Bad_DBF);
	}

    for(j=0;j<dted_xnum;j++)
	{ 
      fwrite(&elev[j][i],2,1,gridbin);
	}
    gridpointsread = gridpointsread + dted_xnum;
    TestNumGridNodes = TestNumGridNodes + dted_xnum;
  }
 

      

  for(i=0;i<dted_xnum;i++)
  {
    free(elev[i]);
  }
  free(elev);
  fclose(dted_in);
  num_grids++;

  return 1;
}




