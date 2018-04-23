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
#include <string.h>

#include "share.h"

extern char **gifd_class_names;
extern char **gifd_class_codes;

extern char **gifd_attr_names;
extern char **gifd_attr_codes;

extern char **gifd_enum_names;
extern char **gifd_enum_codes;

extern int *gifd_attr_enum_count;
extern int *gifd_attr_enum_start;


extern char **gifd_attr_defns;
extern char **gifd_class_defns;


extern int GIFD_ATTR_NUM;
extern int GIFD_CLASS_NUM;



char *GetECCCode(int code)
{
  if(code==-100)
    {
      return "No Classifier Found";
    }

  if(code==0)
    {
      return "*GRID";
    }

  if((code<1)||(code>GIFD_CLASS_NUM))
    {
      printf("Error getting class code: %d/%d\n",code,GIFD_CLASS_NUM);
      return "Error";
    }

  return gifd_class_codes[code-1];
}


char *GetEACCode(int code)
{

  if((code<1)||(code>GIFD_ATTR_NUM))
    {
      printf("Error getting attr code: %d/%d\n",code,GIFD_ATTR_NUM);
      return "Error";
    }

  return gifd_attr_codes[code-1];
}





char *GetECCLabel(int code)
{
  if(code==-100)
    {
      return "No Classifier Found";
    }
  if(code==0)
    {
      return "Post";
    }


  if((code<1)||(code>GIFD_CLASS_NUM))
    {
      printf("Error getting class name: %d/%d\n",code,GIFD_CLASS_NUM);
      return "Error";
    }
  
  
  return gifd_class_names[code-1];
}



char *GetEACLabel(int code)
{
  
  if((code<1)||(code>GIFD_ATTR_NUM))
    {
      printf("Error getting attr name: %d/%d\n",code,GIFD_ATTR_NUM);
      return "Error";
    }
  
  return gifd_attr_names[code-1];
}




char *GetEELabel(int attr_code, int enum_num)
{
  int index;

  attr_code = attr_code - 1;
  

  /* our enums start counting at 0 */
  
  
  if(gifd_attr_enum_count[attr_code]<(enum_num+1))
    {
      printf("Error:  Tried to get enum_num %d for attribute %d (%s)\n",
	     enum_num,attr_code,GetEACLabel(attr_code+1));

      printf("This attribute has only %d enums defined\n",
	     gifd_attr_enum_count[attr_code]);
      return "Error - See output window";
    }

  index = gifd_attr_enum_start[attr_code] + enum_num;
  

  if(index<0)
    {
      /*
	printf("Error - Should not call GEEL with %d for %d\n",enum_num,attr_code);
      */
      return "Error - See output window";
    }


  return gifd_enum_names[index];
}



char *GetEECode(int attr_code, int enum_num)
{
  int index;

  attr_code = attr_code - 1;
  

  /* our enums start counting at 0 */
  
  
  if(gifd_attr_enum_count[attr_code]<(enum_num+1))
    {
      printf("Error:  Tried to get enum_num %d for attribute %d (%s)\n",
	     enum_num,attr_code,GetEACLabel(attr_code+1));

      printf("This attribute has only %d enums defined\n",
	     gifd_attr_enum_count[attr_code]);
      return "Error - See output window";
    }

  index = gifd_attr_enum_start[attr_code] + enum_num;
  
  return gifd_enum_codes[index];
}



char *GetEEDefinition(int attr_code, int enum_code)
{
  return "";
}



char *GetEACEnumDefinition(int attr_code)
{
  static char fulldef[SAC_MAX];
  char temp[1000];
  int i;


  fulldef[0] = '\0';
  
  if(gifd_attr_enum_count[attr_code-1]==0)
    {
      return fulldef;
    }
  

  strcat(fulldef,"\n\nEnumerants:\n\n");
  for(i=0;i<gifd_attr_enum_count[attr_code-1];i++)
    {
      /*
      sprintf(temp,"%s (%s): %s\n",
	      GetEECode      (attr_code, i),
	      GetEELabel     (attr_code, i),
	      GetEEDefinition(attr_code, i)
	      );
      */

      sprintf(temp,"%s: %s\n",
	      GetEECode      (attr_code, i),
	      GetEELabel     (attr_code, i)
	      );

      strcat(fulldef,temp);
    }

  return fulldef;
}



char *GetECCDefinition(int code)
{ 
  if(code==-100)
    {
      return "No Definition";
    }
  if(code==0)
    {
      return "Gridded elevation data such as DTED or SRTM.";
    }

  if((code<1)||(code>GIFD_CLASS_NUM))
    {
      printf("Error getting class def: %d/%d\n",code,GIFD_CLASS_NUM);
      return "Error";
    }

  return gifd_class_defns[code-1];
}



char *GetEACDefinition(int code)
{
  if((code<1)||(code>GIFD_ATTR_NUM))
    {
      printf("Error getting attr def: %d/%d\n",code,GIFD_ATTR_NUM);
      return "Error";
    }
  
  return gifd_attr_defns[code-1];
}




char *GetAllEEDef(int attr_code)
{
  static char fulldef[SAC_MAX];
  int i;
  
  
  fulldef[0] = '\0';
  
  if(gifd_attr_enum_count[attr_code-1]==0)
    {
      return fulldef;
    }
  
  for(i=0;i<gifd_attr_enum_count[attr_code-1];i++)
    {
      strcat(fulldef,GetEEDefinition(attr_code, i));
    }
  return fulldef;
}




char *GetAllEELab(int attr_code)
{
  static char fulldef[SAC_MAX];
  int i;

  
  fulldef[0] = '\0';
  
  if(gifd_attr_enum_count[attr_code-1]==0)
    {
      return fulldef;
    }
  
  for(i=0;i<gifd_attr_enum_count[attr_code-1];i++)
    {
      strcat(fulldef,GetEELabel(attr_code, i));
    }
  return fulldef;
}



int SilentGetCodeIndex(char *ecc)
{
  int i;

  for(i=0;i<GIFD_CLASS_NUM;i++)
    {
      if(!strcmp(ecc,gifd_class_codes[i]))
	{
	  return (i+1);
	}
    }
  
  return -1;
}



int GetCodeIndex(char *ecc)
{
  int i;

  for(i=0;i<GIFD_CLASS_NUM;i++)
    {
      if(!strcmp(ecc,gifd_class_codes[i]))
	{
	  return (i+1);
	}
    }
  
  printf("GetCodeIndex:  Did not find code %s\n",ecc);
  return 1;
}


int GetNum(char *ecc)
{
  int i;

  for(i=0;i<GIFD_CLASS_NUM;i++)
    {
      if(!strcmp(ecc,gifd_class_names[i]))
	{
	  return (i+1);
	}
    }
  
  printf("GetNum:  Did not find code %s\n",ecc);
  return 1;
}




int SilentGetAttrIndex(char *eac)
{
  int i;
  
  for(i=0;i<GIFD_ATTR_NUM;i++)
    {
      if(!strcmp(eac,gifd_attr_codes[i]))
	{
	  return (i+1);
	}
    }
  
  return -1;
}


int GetAttrIndex(char *eac)
{
  int i;

  for(i=0;i<GIFD_ATTR_NUM;i++)
    {
      if(!strcmp(eac,gifd_attr_codes[i]))
	{
	  return (i+1);
	}
    }

  printf("GetAttrIndex:  Did not find attribute with code %s\n",eac);

  return 1;
}


int GetNum2(char *eac)
{
  int i;

  for(i=0;i<GIFD_ATTR_NUM;i++)
    {
      if(!strcmp(eac,gifd_attr_names[i]))
	{
	  return (i+1);
	}
    }
  
  printf("GetNum2:  Did not find attribute with code %s\n",eac);
  return 1;
}
