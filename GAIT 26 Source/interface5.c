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
#include "interface.h"
#include "TTshare.h"


double *global_road_wd1=NULL,*global_cart_wd1=NULL,*global_bridge_wid=NULL,*global_bridge_wd1=NULL;
int road_wd1_val_found   = 0;
int cart_wd1_val_found   = 0;
int bridge_wid_val_found = 0;
int bridge_wd1_val_found = 0;

double *global_bridge_len=NULL;
double *global_span_len  =NULL;
double *global_bridge_hgt=NULL;
double *global_span_hgt  =NULL;
double *global_bridge_zvh=NULL;
double *global_span_zvh  =NULL;

int bridge_len_val_found = 0;
int span_len_val_found   = 0;
int bridge_hgt_val_found = 0;
int span_hgt_val_found   = 0;
int bridge_zvh_val_found = 0;
int span_zvh_val_found   = 0;




#define TDS_badsingleNum 9

int *TDS_badvals2=NULL;

char *TDS_badvals[TDS_badsingleNum*5] = 
{
            

   "AL025",        "P",      "LMC",          "1",                  "True",
   "AI020",        "A",      "FFN",          "1",                  "Recreation",
   "BB090",        "A",      "ZVH",          "0",                  "0",
   "EA055",        "A",      "ZI013_CSP",    "1",                  "Hop",
   "BC070",        "P",      "LMC",          "1",                  "True",
   "AI021",        "A",      "FFN",          "1",                  "Residence",
   "BH135",        "A",      "ZI013_CSP",    "1",                  "Rice",
   "AL250",        "P",      "LMC",          "1",                  "True",
   "EA050",        "A",      "ZI013_CSP",    "1",                  "Grape"
};



#define MGCP4_badsingleNum 87

int *MGCP4_badvals2=NULL;

char *MGCP4_badvals[MGCP4_badsingleNum*5] = 
{
            

   "BB190",        "A",      "MCC",        "1",            "Asphalt",                
   "BB190",        "A",      "MCC",        "1",            "Clay",                   
   "BB190",        "A",      "MCC",        "1",            "Gravel",                 
   "BB190",        "A",      "MCC",        "1",            "Macadam",                
   "BB190",        "A",      "MCC",        "1",            "Pebbles",                
   "BB190",        "A",      "MCC",        "1",            "Rubble",                 
   "BB190",        "A",      "MCC",        "1",            "Sand",                   
   "BB190",        "A",      "MCC",        "1",            "Soil",                   

   "BB190",        "L",      "MCC",        "1",            "Asphalt",                
   "BB190",        "L",      "MCC",        "1",            "Clay",                   
   "BB190",        "L",      "MCC",        "1",            "Gravel",                 
   "BB190",        "L",      "MCC",        "1",            "Macadam",                
   "BB190",        "L",      "MCC",        "1",            "Pebbles",                
   "BB190",        "L",      "MCC",        "1",            "Rubble",                 
   "BB190",        "L",      "MCC",        "1",            "Sand",                   
   "BB190",        "L",      "MCC",        "1",            "Soil",                   

   "BB090",        "A",      "MCC",        "1",            "Asphalt",                
   "BB090",        "A",      "MCC",        "1",            "Clay",                   
   "BB090",        "A",      "MCC",        "1",            "Gravel",                 
   "BB090",        "A",      "MCC",        "1",            "Macadam",                
   "BB090",        "A",      "MCC",        "1",            "Pebbles",                
   "BB090",        "A",      "MCC",        "1",            "Rubble",                 
   "BB090",        "A",      "MCC",        "1",            "Sand",                   
   "BB090",        "A",      "MCC",        "1",            "Soil",                   

   
   
   
   
   
   "AL010",        "A",      "FFN",        "1",            "Transport",              

   
   
   
   "AL010",        "A",      "FFN",        "1",            "Defence Activities",     

   "BB043",        "A",      "MCC",        "1",            "Asphalt",                
   "BB043",        "A",      "MCC",        "1",            "Clay",                   
   "BB043",        "A",      "MCC",        "1",            "Gravel",                 
   "BB043",        "A",      "MCC",        "1",            "Macadam",                
   "BB043",        "A",      "MCC",        "1",            "Pebbles",                
   "BB043",        "A",      "MCC",        "1",            "Rubble",                 
   "BB043",        "A",      "MCC",        "1",            "Sand",                   
   "BB043",        "A",      "MCC",        "1",            "Soil",                   

   "BB043",        "L",      "MCC",        "1",            "Asphalt",                
   "BB043",        "L",      "MCC",        "1",            "Clay",                   
   "BB043",        "L",      "MCC",        "1",            "Gravel",                 
   "BB043",        "L",      "MCC",        "1",            "Macadam",                
   "BB043",        "L",      "MCC",        "1",            "Pebbles",                
   "BB043",        "L",      "MCC",        "1",            "Rubble",                 
   "BB043",        "L",      "MCC",        "1",            "Sand",                   
   "BB043",        "L",      "MCC",        "1",            "Soil",                   

   "AN010",        "L",      "GAW",        "0",            "-32764",                 
   "AN010",        "L",      "GAW",        "0",            "-32768",                 
   "AN010",        "L",      "LTN",        "0",            "-32768",                 

   "AN050",        "L",      "GAW",        "0",            "-32764",                 
   "AN050",        "L",      "GAW",        "0",            "-32768",                 

   "AK190",        "A",      "MCC",        "1",            "Asphalt",                
   "AK190",        "A",      "MCC",        "1",            "Clay",                   
   "AK190",        "A",      "MCC",        "1",            "Gravel",                 
   "AK190",        "A",      "MCC",        "1",            "Macadam",                
   "AK190",        "A",      "MCC",        "1",            "Pebbles",                
   "AK190",        "A",      "MCC",        "1",            "Rubble",                 
   "AK190",        "A",      "MCC",        "1",            "Sand",                   
   "AK190",        "A",      "MCC",        "1",            "Soil",                   

   "AK190",        "L",      "MCC",        "1",            "Asphalt",                
   "AK190",        "L",      "MCC",        "1",            "Clay",                   
   "AK190",        "L",      "MCC",        "1",            "Gravel",                 
   "AK190",        "L",      "MCC",        "1",            "Macadam",                
   "AK190",        "L",      "MCC",        "1",            "Pebbles",                
   "AK190",        "L",      "MCC",        "1",            "Rubble",                 
   "AK190",        "L",      "MCC",        "1",            "Sand",                   
   "AK190",        "L",      "MCC",        "1",            "Soil",                   

   "BD120",        "A",      "WLE",        "1",            "Always Dry",             
   "BD120",        "A",      "WLE",        "1",            "Floating",               
   "BD120",        "A",      "WLE",        "1",            "Not Applicable",         
   "BD120",        "A",      "WLE",        "1",            "Other",                  

   "BD120",        "L",      "WLE",        "1",            "Always Dry",             
   "BD120",        "L",      "WLE",        "1",            "Floating",               
   "BD120",        "L",      "WLE",        "1",            "Not Applicable",         
   "BD120",        "L",      "WLE",        "1",            "Other",                  


   "BH140",        "L",      "WID",        "0",            "-32764",                 
   "BH140",        "L",      "WID",        "0",            "-32765",                 
   "BH140",        "L",      "WID",        "0",            "-32768",                 

   "AP030",        "L",      "LTN",        "0",            "-32768",                 
   "AP030",        "L",      "WD1",        "0",            "-32768",                 


   "BB230",        "L",      "MCC",        "1",            "Asphalt",                
   "BB230",        "L",      "MCC",        "1",            "Clay",                   
   "BB230",        "L",      "MCC",        "1",            "Gravel",                 
   "BB230",        "L",      "MCC",        "1",            "Macadam",                
   "BB230",        "L",      "MCC",        "1",            "Pebbles",                
   
   "BB230",        "L",      "MCC",        "1",            "Sand",                   
   "BB230",        "L",      "MCC",        "1",            "Soil",                   

   "DA010",        "A",      "SMC",        "1",            "Salt",
   "DA010",        "A",      "SMC",        "1",            "Evaporite",

   "AQ130",        "L",      "ACC",        "1",            "Accurate",               
   "AQ130",        "A",      "ACC",        "1",            "Accurate"

};






#define MGCP_RANGE_SPECS_MAX 295  
int MGCP_RANGE_SPECS = 0;         
char **MGCP_ranges;               
#define MGCP_RANGE_MULTIPLIER 9


int MGCP_RANGE_FCODES[MGCP_RANGE_SPECS_MAX*MGCP_RANGE_MULTIPLIER]; 


#define MGCP_RANGE_SPECS3 295

char *MGCP_ranges3[MGCP_RANGE_SPECS3*MGCP_RANGE_MULTIPLIER] = 
{
   "metad","!",      "ACE","Y",">0",   "25",     "E", "unused","unused",
   "metad","!",      "ALE","Y",">0",   "25",     "E", "unused","unused",
   "AL015","(AREA)", "AOO","Y",">=0",  "360",    "E", "unused","unused",
   "AL015","(POINT)","AOO","Y",">=0",  "360",    "E", "unused","unused",
   "BD110","(POINT)","AOO","Y",">=0",  "360",    "E", "unused","unused",
   "BI020","(AREA)", "AOO","Y",">=0",  "360",    "E", "unused","unused",
   "BI020","(LINE)", "AOO","Y",">=0",  "360",    "E", "unused","unused",
   "BI020","(POINT)","AOO","Y",">=0",  "360",    "E", "unused","unused",
   "AA010","(POINT)","ARA","Y",">0",   "15625",  "E", "unused","unused",
   "AA012","(POINT)","ARA","Y",">0",   "15625",  "E", "unused","unused",
   "AC000","(POINT)","ARA","Y",">0",   "15625",  "E", "unused","unused",
   "AD010","(POINT)","ARA","Y",">0",   "15625",  "E", "unused","unused",
   "AD030","(POINT)","ARA","Y",">0",   "MAX",    "E", "unused","unused",
   "AK040","(POINT)","ARA","Y",">0",   "15625",  "E", "unused","unused",
   "AL015","(POINT)","ARA","Y",">0",   "625",    "E", "unused","unused",
   "AM070","(POINT)","ARA","Y",">0",   "MAX",    "E", "unused","unused",
   "BD100","(POINT)","ARA","Y",">0",   "15625",  "E", "unused","unused",
   "GA035","(POINT)","BRF","Y",">0",   "MAX",    "E", "unused","unused",
   "AN050","(LINE)", "CTL","Y",">0",   "10000",  "W", "unused","unused",
   "AN060","(AREA)", "CTL","Y",">0",   "10000",  "W", "unused","unused",
   "AF070","(POINT)","DMF","Y",">0",   "50",     "W", "unused","unused",
   "AQ070","(LINE)", "FCL","Y",">0",   "5000",   "W", "unused","unused",
   "AQ070","(POINT)","FCL","Y",">0",   "25",     "E", "unused","unused",
   "AN010","(LINE)", "GAW","Y",">0",   "2",      "E", "unused","unused",
   "AN050","(LINE)", "GAW","Y",">0",   "2",      "E", "unused","unused",
   "AQ040","(LINE)", "HCA","Y",">0",   "25",     "W", "unused","unused",
   "AA040","(POINT)","HGT","Y",">0",   "200",    "W", "unused","unused",
   "AB000","(AREA)", "HGT","Y",">0",   "20",     "W", "unused","unused",
   "AB010","(AREA)", "HGT","Y",">0",   "20",     "W", "unused","unused",
   "AC000","(AREA)", "HGT","Y",">0",   "380",    "W", "unused","unused",
   "AC000","(POINT)","HGT","Y",">0",   "380",    "W", "unused","unused",
   "AC020","(POINT)","HGT","Y",">0",   "100",    "W", "unused","unused",
   "AD010","(AREA)", "HGT","Y",">0",   "200",    "W", "unused","unused",
   "AD010","(POINT)","HGT","Y",">0",   "200",    "W", "unused","unused",
   "AD020","(POINT)","HGT","Y",">0",   "100",    "W", "unused","unused",
   "AD030","(AREA)", "HGT","Y",">0",   "400",    "W", "unused","unused",
   "AD030","(POINT)","HGT","Y",">0",   "400",    "W", "unused","unused",
   "AF010","(POINT)","HGT","Y",">0",   "380",    "W", "unused","unused",
   "AF020","(LINE)", "HGT","Y",">0",   "100",    "W", "unused","unused",
   "AF030","(POINT)","HGT","Y",">0",   "200",    "W", "unused","unused",
   "AF040","(POINT)","HGT","Y",">0",   "120",    "W", "unused","unused",
   "AF070","(POINT)","HGT","Y",">0",   "220",    "W", "unused","unused",
   "AH050","(AREA)", "HGT","Y",">0",   "100",    "W", "unused","unused",
   "AH050","(POINT)","HGT","Y",">0",   "100",    "W", "unused","unused",
   "AJ050","(POINT)","HGT","Y",">0",   "50",     "W", "unused","unused",
   "AJ051","(POINT)","HGT","Y",">0",   "220",    "W", "unused","unused",
   "AK030","(AREA)", "HGT","Y",">0",   "330",    "W", "unused","unused",
   "AK160","(AREA)", "HGT","Y",">0",   "150",    "W", "unused","unused",
   "AK160","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "AL015","(AREA)", "HGT","Y",">0",   "850",    "W", "unused","unused",
   "AL015","(POINT)","HGT","Y",">0",   "850",    "W", "unused","unused",
   "AL020","(AREA)", "HGT","Y",">0",   "850",    "W", "unused","unused",
   "AL020","(POINT)","HGT","Y",">0",   "850",    "W", "unused","unused",
   "AL080","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "AL130","(POINT)","HGT","Y",">0",   "160",    "W", "unused","unused",
   "AL200","(AREA)", "HGT","Y",">0",   "200",    "W", "unused","unused",
   "AL240","(POINT)","HGT","Y",">0",   "500",    "W", "unused","unused",
   "AM010","(AREA)", "HGT","Y",">0",   "50",     "W", "unused","unused",
   "AM020","(AREA)", "HGT","Y",">0",   "150",    "W", "unused","unused",
   "AM020","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "AM030","(AREA)", "HGT","Y",">0",   "150",    "W", "unused","unused",
   "AM030","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "AM040","(AREA)", "HGT","Y",">0",   "50",     "W", "unused","unused",
   "AM040","(POINT)","HGT","Y",">0",   "50",     "W", "unused","unused",
   "AM070","(AREA)", "HGT","Y",">0",   "75",     "W", "unused","unused",
   "AM070","(POINT)","HGT","Y",">0",   "50",     "W", "unused","unused",
   "AM080","(POINT)","HGT","Y",">0",   "67",     "W", "unused","unused",
   "AQ060","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "AQ064","(LINE)", "HGT","Y",">0",   "50",     "W", "unused","unused",
   "AQ110","(POINT)","HGT","Y",">0",   "100",    "W", "unused","unused",
   "AQ113","(LINE)", "HGT","Y",">0",   "20",     "W", "unused","unused",
   "AQ116","(AREA)", "HGT","Y",">0",   "40",     "W", "unused","unused",
   "AQ116","(POINT)","HGT","Y",">0",   "40",     "W", "unused","unused",
   "AT010","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "AT030","(LINE)", "HGT","Y",">0",   "350",    "W", "unused","unused",
   "AT041","(LINE)", "HGT","Y",">0",   "150",    "W", "unused","unused",
   "AT042","(POINT)","HGT","Y",">0",   "400",    "W", "unused","unused",
   "AT045","(POINT)","HGT","Y",">0",   "MAX",    "W", "unused","unused",
   "AT050","(AREA)", "HGT","Y",">0",   "MAX",    "W", "unused","unused",
   "AT060","(LINE)", "HGT","Y",">0",   "MAX",    "W", "unused","unused",
   "AT080","(POINT)","HGT","Y",">0",   "650",    "W", "unused","unused",
   "BC050","(POINT)","HGT","Y",">0",   "150",    "W", "unused","unused",
   "BD110","(POINT)","HGT","Y",">0",   "200",    "W", "unused","unused",
   "BI020","(AREA)", "HGT","Y",">0",   "350",    "W", "unused","unused",
   "BI020","(LINE)", "HGT","Y",">0",   "100",    "W", "unused","unused",
   "BI020","(POINT)","HGT","Y",">0",   "100",    "W", "unused","unused",
   "BI050","(POINT)","HGT","Y",">0",   "100",    "W", "unused","unused",
   "BJ060","(POINT)","HGT","Y",">0",   "MAX",    "W", "unused","unused",
   "DB160","(AREA)", "HGT","Y",">0",   "400",    "W", "unused","unused",
   "DB160","(POINT)","HGT","Y",">0",   "400",    "W", "unused","unused",
   "DB180","(AREA)", "HGT","Y",">0",   "3000",   "W", "unused","unused",
   "DB180","(POINT)","HGT","Y",">0",   "3000",   "W", "unused","unused",
   "GB030","(POINT)","HGT","Y",">0",   "350",    "W", "unused","unused",
   "GB040","(POINT)","HGT","Y",">0",   "MAX",    "W", "unused","unused",
   "GB050","(LINE)", "HGT","Y",">0",   "20",     "W", "unused","unused",
   "GB220","(POINT)","HGT","Y",">0",   "650",    "W", "unused","unused",
   "AT030","(LINE)", "KVA","Y",">0",   "1000",   "W", "unused","unused",
   "BH020","(AREA)", "LBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH020","(LINE)", "LBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH030","(AREA)", "LBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH030","(LINE)", "LBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH140","(AREA)", "LBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH140","(LINE)", "LBV","Y",">=0",  "100",    "E", "unused","unused",
   "AQ040","(LINE)", "LC1","Y",">0",   "150",    "W", "unused","unused",
   "AQ040","(LINE)", "LC2","Y",">0",   "150",    "W", "unused","unused",
   "AQ040","(LINE)", "LC3","Y",">0",   "150",    "W", "unused","unused",
   "AQ040","(LINE)", "LC4","Y",">0",   "150",    "W", "unused","unused",
   "BH010","(POINT)","LEN","Y",">0",   "1200000","W", "unused","unused",
   "BH120","(AREA)", "LEN","Y",">0",   "10000",  "W", "unused","unused",
   "GB030","(POINT)","LEN","Y",">0",   "200",    "W", "unused","unused",
   "GB055","(AREA)", "LEN","Y",">0",   "12500",  "W", "unused","unused",
   "AA040","(POINT)","LEN","Y",">0",   "150",    "W", "unused","unused",
   "AC000","(AREA)", "LEN","Y",">0",   "10000",  "W", "unused","unused",
   "AC000","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AC030","(AREA)", "LEN","Y",">0",   "2000",   "W", "unused","unused",
   "AD010","(AREA)", "LEN","Y",">0",   "10000",  "W", "unused","unused",
   "AD010","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AD020","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AD030","(AREA)", "LEN","Y",">0",   "1000",   "W", "unused","unused",
   "AD030","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AF010","(POINT)","LEN","Y",">0",   "420",    "W", "unused","unused",
   "AF030","(POINT)","LEN","Y",">0",   "200",    "W", "unused","unused",
   "AF070","(POINT)","LEN","Y",">0",   "100",    "W", "unused","unused",
   "AH050","(POINT)","LEN","Y",">0",   "1000",   "W", "unused","unused",
   "AJ010","(AREA)", "LEN","Y",">0",   "2000",   "W", "unused","unused",
   "AK040","(POINT)","LEN","Y",">0",   "2000",   "W", "unused","unused",
   "AK160","(POINT)","LEN","Y",">0",   "3000",   "W", "unused","unused",
   "AL015","(POINT)","LEN","Y",">0",   "625",    "W", "unused","unused",
   "AL080","(POINT)","LEN","Y",">0",   "400",    "W", "unused","unused",
   "AL130","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AL140","(AREA)", "LEN","Y",">0",   "28000",  "W", "unused","unused",
   "AL210","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AL240","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "AM010","(AREA)", "LEN","Y",">0",   "2000",   "W", "unused","unused",
   "AM020","(AREA)", "LEN","Y",">0",   "2000",   "W", "unused","unused",
   "AM020","(POINT)","LEN","Y",">0",   "40",     "E", "unused","unused",
   "AM030","(AREA)", "LEN","Y",">0",   "830",    "W", "unused","unused",
   "AM030","(POINT)","LEN","Y",">0",   "40",     "E", "unused","unused",
   "AM040","(AREA)", "LEN","Y",">0",   "500",    "W", "unused","unused",
   "AM040","(POINT)","LEN","Y",">0",   "40",     "E", "unused","unused",
   "AM060","(AREA)", "LEN","Y",">0",   "500",    "W", "unused","unused",
   "AM060","(POINT)","LEN","Y",">0",   "<40",    "E", "unused","unused",
   "AM070","(AREA)", "LEN","Y",">0",   "200",    "W", "unused","unused",
   "AM070","(POINT)","LEN","Y",">0",   "40",     "E", "unused","unused",
   "AM080","(POINT)","LEN","Y",">0",   "100",    "W", "unused","unused",
   "AQ116","(AREA)", "LEN","Y",">125", "500",    "W", "unused","unused",
   "AQ116","(POINT)","LEN","Y",">0",   "125",    "E", "unused","unused",
   "AQ130","(LINE)", "LEN","Y",">0",   "75000",  "W", "unused","unused",
   "AT010","(POINT)","LEN","Y",">0",   "350",    "W", "unused","unused",
   "AT045","(POINT)","LEN","Y",">0",   "1000",   "W", "unused","unused",
   "AT050","(AREA)", "LEN","Y",">0",   "1000",   "W", "unused","unused",
   "BB041","(AREA)", "LEN","Y",">0",   "11000",  "W", "unused","unused",
   "BB041","(LINE)", "LEN","Y",">0",   "11000",  "W", "unused","unused",
   "BB043","(AREA)", "LEN","Y",">0",   "500",    "W", "unused","unused",
   "BB043","(LINE)", "LEN","Y",">0",   "500",    "W", "unused","unused",
   "BB090","(AREA)", "LEN","Y",">0",   "550",    "W", "unused","unused",
   "BB140","(AREA)", "LEN","Y",">0",   "35000",  "W", "unused","unused",
   "BB140","(LINE)", "LEN","Y",">0",   "35000",  "W", "unused","unused",
   "BB190","(AREA)", "LEN","Y",">0",   "5000",   "W", "unused","unused",
   "BB190","(LINE)", "LEN","Y",">0",   "5000",   "W", "unused","unused",
   "BB240","(AREA)", "LEN","Y",">0",   "550",    "W", "unused","unused",
   "BD130","(POINT)","LEN","Y",">0",   "500",    "W", "unused","unused",
   "BH120","(LINE)", "LEN","Y",">=25", "20000",  "W", "unused","unused",
   "BH120","(POINT)","LEN","Y",">0",   "25",     "E", "unused","unused",
   "BI020","(AREA)", "LEN","Y",">0",   "30000",  "W", "unused","unused",
   "BI020","(POINT)","LEN","Y",">0",   "25",     "E", "unused","unused",
   "BI030","(AREA)", "LEN","Y",">0",   "500",    "W", "unused","unused",
   "BI030","(POINT)","LEN","Y",">0",   "25",     "E", "unused","unused",
   "BI040","(POINT)","LEN","Y",">0",   "25",     "E", "unused","unused",
   "BI041","(POINT)","LEN","Y",">0",   "25",     "E", "unused","unused",
   "BI050","(POINT)","LEN","Y",">0",   "100",    "W", "unused","unused",
   "BJ020","(AREA)", "LEN","Y",">0",   "10000",  "W", "unused","unused",
   "DB090","(AREA)", "LEN","Y",">0",   "800000", "W", "unused","unused",
   "EC040","(AREA)", "LEN","Y",">0",   "100000", "W", "unused","unused",
   "GB045","(AREA)", "LEN","Y",">0",   "2000",   "W", "unused","unused",
   "GB075","(AREA)", "LEN","Y",">0",   "10000",  "W", "unused","unused",
   "GB050","(LINE)", "LNU","Y",">0",   "50",     "W", "unused","unused",
   "AN010","(LINE)", "LTN","Y",">0",   "20",     "W", "unused","unused",
   "AP030","(LINE)", "LTN","Y",">0",   "20",     "W", "unused","unused",
   "AL070","(LINE)", "MOH","Y",">0",   "50",     "W", "unused","unused",
   "AL260","(LINE)", "MOH","Y",">0",   "50",     "W", "unused","unused",
   "AQ040","(LINE)", "MVC","Y",">0",   "300",    "W", "unused","unused",
   "AQ040","(LINE)", "NOS","Y",">0",   "20",     "W", "unused","unused",
   "AT030","(LINE)", "NPL","Y",">0",   "25",     "W", "unused","unused",
   "AQ040","(LINE)", "OHB","Y",">0",   "400",    "W", "unused","unused",
   "AC000","(AREA)", "PFH","Y",">0",   "425",    "W", "unused","unused",
   "AC000","(POINT)","PFH","Y",">0",   "425",    "W", "unused","unused",
   "AD010","(AREA)", "PFH","Y",">0",   "200",    "W", "unused","unused",
   "AD010","(POINT)","PFH","Y",">0",   "200",    "W", "unused","unused",
   "AL070","(LINE)", "PFH","Y",">0",   "50",     "W", "unused","unused",
   "AL105","(AREA)", "PFH","Y",">0",   "MAX",    "W", "unused","unused",
   "AL105","(POINT)","PFH","Y",">0",   "MAX",    "W", "unused","unused",
   "AL260","(LINE)", "PFH","Y",">0",   "50",     "W", "unused","unused",
   "AM020","(AREA)", "PFH","Y",">0",   "150",    "W", "unused","unused",
   "AM020","(POINT)","PFH","Y",">0",   "150",    "W", "unused","unused",
   "AT030","(LINE)", "PFH","Y",">0",   "350",    "W", "unused","unused",
   "DB010","(LINE)", "PFH","Y",">0",   "2000",   "W", "unused","unused",
   "DB070","(LINE)", "PFH","Y",">0",   "MAX",    "W", "unused","unused",
   "DB071","(LINE)", "PFH","Y",">0",   "MAX",    "W", "unused","unused",
   "DB090","(AREA)", "PFH","Y",">0",   "200",    "W", "unused","unused",
   "DB090","(LINE)", "PFH","Y",">0",   "200",    "W", "unused","unused",
   "EA010","(AREA)", "PFH","Y",">0",   "MAX",    "W", "unused","unused",
   "EB020","(AREA)", "PFH","Y",">0",   "10",     "W", "unused","unused",
   "EC030","(AREA)", "PFH","Y",">0",   "150",    "W", "unused","unused",
   "EC030","(LINE)", "PFH","Y",">0",   "150",    "W", "unused","unused",
   "EC030","(POINT)","PFH","Y",">0",   "150",    "W", "unused","unused",
   "ED020","(AREA)", "PFH","Y",">0",   "100",    "W", "unused","unused",
   "GB050","(LINE)", "PFH","Y",">0",   "50",     "W", "unused","unused",
   "BH020","(AREA)", "RBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH020","(LINE)", "RBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH030","(AREA)", "RBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH030","(LINE)", "RBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH140","(AREA)", "RBV","Y",">=0",  "100",    "E", "unused","unused",
   "BH140","(LINE)", "RBV","Y",">=0",  "100",    "E", "unused","unused",
   "DB170","(AREA)", "SDO","Y",">=0",  "359",    "W", "unused","unused",
   "AP030","(LINE)", "WD1","Y",">0",   "120",    "W", "unused","unused",
   "AP010","(LINE)", "WD1","Y",">0",   "30",     "W", "unused","unused",
   "AQ040","(LINE)", "WD1","Y",">0",   "60",     "W", "unused","unused",
   "BH020","(LINE)", "WID","Y",">0",   "180",    "W", "unused","unused",
   "BH140","(LINE)", "WID","Y",">0",   "25",     "E", "unused","unused",
   "BI020","(LINE)", "WID","Y",">0",   "25",     "E", "unused","unused",
   "BI020","(POINT)","WID","Y",">0",   "25",     "E", "unused","unused",
   "BI030","(LINE)", "WID","Y",">0",   "25",     "E", "unused","unused",
   "BI030","(POINT)","WID","Y",">0",   "25",     "E", "unused","unused",
   "DB110","(LINE)", "WID","Y",">0",   "200",    "W", "unused","unused",
   "AC030","(AREA)", "WID","Y",">=125","2000",   "W", "unused","unused",
   "AD010","(AREA)", "WID","Y",">25",  "10000",  "W", "unused","unused",
   "AD010","(POINT)","WID","Y",">0",   "125",    "E", "unused","unused",
   "AD020","(POINT)","WID","Y",">0",   "100",    "W", "unused","unused",
   "AD030","(AREA)", "WID","Y",">0",   "1000",   "W", "unused","unused",
   "AD030","(POINT)","WID","Y",">0",   "<40",    "E", "unused","unused",
   "AF010","(POINT)","WID","Y",">0",   "50",     "W", "unused","unused",
   "AH050","(AREA)", "WID","Y",">0",   "500",    "W", "unused","unused",
   "AH050","(POINT)","WID","Y",">0",   "40",     "E", "unused","unused",
   "AK040","(POINT)","WID","Y",">0",   "250",    "W", "unused","unused",
   "AK160","(POINT)","WID","Y",">0",   "250",    "W", "unused","unused",
   "AL015","(AREA)", "WID","Y",">0",   "800",    "W", "unused","unused",
   "AL015","(POINT)","WID","Y",">0",   "25",     "W", "unused","unused",
   "AL080","(POINT)","WID","Y",">0",   "100",    "W", "unused","unused",
   "AL130","(POINT)","WID","Y",">0",   "30",     "W", "unused","unused",
   "AL210","(LINE)", "WID","Y",">0",   "50",     "W", "unused","unused",
   "AL260","(LINE)", "WID","Y",">0",   "30",     "W", "unused","unused",
   "AM010","(AREA)", "WID","Y",">0",   "2000",   "W", "unused","unused",
   "AM020","(AREA)", "WID","Y",">0",   "400",    "W", "unused","unused",
   "AM020","(POINT)","WID","Y",">0",   "40",     "E", "unused","unused",
   "AM030","(AREA)", "WID","Y",">0",   "100",    "W", "unused","unused",
   "AM030","(POINT)","WID","Y",">0",   "100",    "W", "unused","unused",
   "AM060","(AREA)", "WID","Y",">0",   "500",    "W", "unused","unused",
   "AM060","(POINT)","WID","Y",">0",   "<40",    "E", "unused","unused",
   "AM070","(AREA)", "WID","Y",">0",   "60",     "W", "unused","unused",
   "AM070","(POINT)","WID","Y",">0",   "60",     "W", "unused","unused",
   "AM080","(POINT)","WID","Y",">0",   "50",     "W", "unused","unused",
   "AQ040","(LINE)", "WID","Y",">0",   "75",     "W", "unused","unused",
   "AQ065","(POINT)","WID","Y",">0",   "20",     "W", "unused","unused",
   "AQ116","(AREA)", "WID","Y",">125", "500",    "W", "unused","unused",
   "AQ116","(POINT)","WID","Y",">0",   "125",    "W", "unused","unused",
   "AQ130","(LINE)", "WID","Y",">0",   "30",     "W", "unused","unused",
   "BB041","(AREA)", "WID","Y",">=20", "200",    "W", "unused","unused",
   "BB041","(LINE)", "WID","Y",">0",   "20",     "E", "unused","unused",
   "BB043","(AREA)", "WID","Y",">0",   "50",     "W", "unused","unused",
   "BB043","(LINE)", "WID","Y",">0",   "20",     "E", "unused","unused",
   "BB090","(AREA)", "WID","Y",">=20", "150",    "W", "unused","unused",
   "BB140","(AREA)", "WID","Y",">=20", "200",    "W", "unused","unused",
   "BB140","(LINE)", "WID","Y",">0",   "20",     "E", "unused","unused",
   "BB190","(LINE)", "WID","Y",">0",   "20",     "E", "unused","unused",
   "BB230","(LINE)", "WID","Y",">1",   "100",    "W", "unused","unused",
   "BB240","(AREA)", "WID","Y",">2",   "100",    "W", "unused","unused",
   "BH020","(AREA)", "WID","Y",">3",   "200",    "W", "unused","unused",
   "BH030","(AREA)", "WID","Y",">=25", "100",    "W", "unused","unused",
   "BH030","(LINE)", "WID","Y",">0",   "25",     "E", "unused","unused",
   "BH040","(AREA)", "WID","Y",">0",   "1000",   "W", "unused","unused",
   "BH140","(AREA)", "WID","Y",">=25", "12000",  "W", "unused","unused",
   "BH160","(AREA)", "WID","Y",">0",   "30000",  "W", "unused","unused",
   "BI020","(AREA)", "WID","Y",">=25", "200",    "W", "unused","unused",
   "BI030","(AREA)", "WID","Y",">=25", "100",    "W", "unused","unused",
   "BI041","(LINE)", "WID","Y",">0",   "50",     "W", "unused","unused",
   "BI041","(POINT)","WID","Y",">0",   "25",     "W", "unused","unused",
   "BJ031","(LINE)", "WID","Y",">0",   "1000",   "W", "unused","unused",
   "DB061","(LINE)", "WID","Y",">0",   "30",     "W", "unused","unused",
   "DB070","(LINE)", "WID","Y",">0",   "<60",    "W", "unused","unused",
   "DB090","(LINE)", "WID","Y",">0",   "100",    "W", "unused","unused",
   "EC040","(AREA)", "WID","Y",">0",   "300",    "W", "unused","unused",
   "FA090","(LINE)", "WID","Y",">0",   "30",     "W", "unused","unused",
   "GB015","(AREA)", "WID","Y",">0",   "1000",   "W", "unused","unused",
   "GB030","(POINT)","WID","Y",">0",   "200",    "W", "unused","unused",
   "GB045","(AREA)", "WID","Y",">0",   "100",    "W", "unused","unused",
   "GB050","(LINE)", "WID","Y",">0",   "50",     "W", "unused","unused",
   "GB075","(AREA)", "WID","Y",">0",   "50",     "W", "unused","unused",
   "GB005","(AREA)", "ZVA","Y",">-378","4300",   "W", "unused","unused",
   "GB030","(POINT)","ZVA","Y",">-378","4300",   "W", "unused","unused",
   "GB035","(AREA)", "ZVA","Y",">-378","4300",   "W", "unused","unused",
   "GB040","(POINT)","ZVA","Y",">-378","4300",   "W", "unused","unused",
   "GB055","(AREA)", "ZVA","Y",">-378","4300",   "W", "unused","unused",
   "GB065","(POINT)","ZVA","Y",">-378","4300",   "W", "unused","unused",
   "GB055","(AREA)", "AOO","Y",">=0",  "359.99", "E", "unused","unused"
    
   

  };




#define MGCP_RANGE_SPECS4 103

char *MGCP_ranges4[MGCP_RANGE_SPECS4*MGCP_RANGE_MULTIPLIER] = 
{
   "metad","!",      "ACE","Y",">0",   "25",    "E","none","none",
   "metad","!",      "ALE","Y",">0",   "25",    "E","none","none",
   "GB220","(POINT)","HGT","Y",">=46", "650",   "W","none","none",
   "BH010","(POINT)","LEN","Y",">0",   "3.00001",  "W","none","none",
   "BH010","(LINE)", "WID","Y",">0",   "<25",   "W","none","CDA=1001,LOC=40,FUN=1-4",
   "BB190","(LINE)", "WID","Y",">0",   "<25",   "E","none","FUN=1-4",
   "BB041","(AREA)", "WID","Y",">25",  "50",    "W","none","FUN=1-4",
   "BB041","(LINE)", "WID","Y",">0",   "<25",   "E","none","FUN=1-4",
   "AQ040","(AREA)", "NOS","Y",">0",   "20",    "W","-32767","none",
   "AQ040","(LINE)", "NOS","Y",">0",   "20",    "W","-32767","none",
   "AQ040","(AREA)", "OHB","Y",">0",   "50",    "W","-32767 and -32765","none",
   "AQ040","(LINE)", "OHB","Y",">0",   "345",   "W","-32767 and -32765","none",
   "AQ040","(LINE)", "WD1","Y",">0",   "60",    "W","-32767 and -32765","FUN=1-4,TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999",
   "AQ040","(LINE)", "WID","Y",">0",   "60",    "W","-32767","FUN=1-4",
   "AT041","(LINE)", "HGT","Y",">0",   "150",   "W","-32767 and -32765","none",
   "BH020","(LINE)", "WID","Y",">0",   "<25",   "W","-32767","LOC=40,CDA=1001,FUN=1-4",
   "AP010","(LINE)", "WD1","Y",">0",   "5",     "W","-32767","none",
   "AC020","(POINT)","HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "AT060","(LINE)", "HGT","Y",">0",   "150",   "W","-32767 and -32765","none",
   "AT050","(AREA)", "HGT","Y",">0",   "150",   "W","-32767 and -32765","none",
   "AF020","(LINE)", "HGT","Y",">0",   "90",    "W","-32767 and -32765","none",
   "AF040","(POINT)","HGT","Y",">0",   "135",   "W","-32767 and -32765","none",
   "BJ031","(LINE)", "WID","Y",">0",   "<50",   "W","-32767","none",
   "DB061","(LINE)", "WID","Y",">0",   "<50",   "W","-32767","none",
   "EA010","(AREA)", "DMT","Y","0",    "100",   "E","-32767","none",
   "AQ065","(POINT)","AOO","Y",">=0",  "360",   "E","-32767","none",
   "AQ065","(POINT)","WID","Y",">0",   "20",    "W","-32767","none",
   "DB070","(LINE)", "WID","Y",">0",   "30",    "W","-32767","none",
   "BI020","(AREA)", "HGT","Y",">0",   "350",   "W","-32767 and -32765","none",
   "BI020","(LINE)", "HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "BI020","(POINT)","HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "BI020","(LINE)", "WID","Y",">0",   "<125",  "E","-32767","FUN=1-4",
   "AT010","(POINT)","HGT","Y",">=46", "150",   "W","-32767 and -32765","none",
   "AB000","(AREA)", "HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "BH030","(LINE)", "WID","Y",">0",   "<25",   "E","-32767","CDA=1001,FUN=1-4",
   "AL060","(AREA)", "WID","Y",">25",  "50",    "W","-32767","FUN=1-4",
   "AL060","(LINE)", "WID","Y",">0",   "25",    "W","-32767","FUN=1-4",
   "AD010","(AREA)", "HGT","Y",">0",   "175",   "W","-32767 and -32765","none",
   "AD010","(POINT)","HGT","Y",">0",   "175",   "W","-32767 and -32765","none",
   "DB090","(LINE)", "WID","Y",">0",   "<50",   "W","-32767","none",
   "AH025","(LINE)", "WID","Y",">0",   "25",    "W","-32767","none",
   "AF070","(POINT)","HGT","Y",">0",   "210",   "W","-32767 and -32765","none",
   "AH050","(AREA)", "HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "AH050","(POINT)","HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "AL015","(AREA)", "HGT","Y",">0",   "830",   "W","-32767 and -32765","none",
   "AL015","(POINT)","HGT","Y",">0",   "830",   "W","-32767 and -32765","none",
   "DB110","(LINE)", "WID","Y",">0",   "200",   "W","-32767","none",
   "AM030","(AREA)", "HGT","Y",">0",   "150",   "W","-32767 and -32765","none",
   "AM030","(POINT)","HGT","Y",">0",   "150",   "W","-32767 and -32765","none",
   "AM020","(AREA)", "HGT","Y",">0",   "120",   "W","-32767 and -32765","none",
   "AM020","(POINT)","HGT","Y",">0",   "120",   "W","-32767 and -32765","none",
   "BB043","(LINE)", "WID","Y",">0",   "<25",   "E","-32767","FUN=1-4",
   "GB030","(POINT)","ZVA","Y",">-378","6500",  "W","-32767","none",
   "GB035","(AREA)", "ZVA","Y",">-378","6500",  "W","-32767","none",
   "GB005","(AREA)", "ZVA","Y",">-378","6500",  "W","-32767","none",
   "BI030","(LINE)", "WID","Y",">0",   "<25",   "E","-32767","FUN=1-4",
   "ED030","(AREA)", "DMT","Y",">0",   "100",   "E","-32767","none",
   "AL130","(POINT)","HGT","Y",">0",   "210",   "W","-32767 and -32765","none",
   "AM040","(AREA)", "HGT","Y",">0",   "50",    "W","-32767 and -32765","none",
   "AM040","(POINT)","HGT","Y",">0",   "50",    "W","-32767 and -32765","none",
   "AQ110","(POINT)","HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "BD110","(POINT)","HGT","Y",">0",   "170",   "W","-32767 and -32765","none",
   "EA040","(AREA)", "DMT","Y",">0",   "100",   "E","-32767","none",
   "AT030","(LINE)", "HGT","Y",">0",   "350",   "W","-32767 and -32765","none",
   "AC000","(AREA)", "HGT","Y",">0",   "380",   "W","-32767 and -32765","none",
   "AC000","(POINT)","HGT","Y",">0",   "380",   "W","-32767 and -32765","none",
   "AL210","(LINE)", "WID","Y",">0",   "50",    "W","-32767","FUN=1-4",
   "AT042","(POINT)","HGT","Y",">0",   "400",   "W","-32767 and -32765","none",
   "AN010","(LINE)", "GAW","Y",">=0.6","1.676", "E","-32767","FUN=1-4,LOC=40",
   "AN010","(LINE)", "LTN","Y",">0",   "20",    "W","-32767","FUN=1-4,LOC=40",
   "AN050","(LINE)", "GAW","Y",">=0.6","1.676", "E","-32767","FUN=1-4,LOC=40",
   "AA040","(POINT)","HGT","Y",">0",   "200",   "W","-32767 and -32765","none",
   "BH140","(LINE)", "WID","Y",">0",   "<25",   "E","-32767","CDA=1001",
   "AP030","(LINE)", "LTN","Y",">0",   "20",    "W","-32767","FUN=1-4,LOC=40",
   "AP030","(LINE)", "WD1","Y",">0",   "110",   "W","-32767","FUN=1-4,LOC=40",
   "GB055","(AREA)", "AOO","Y",">=0",  "179.9", "E","-32767","none",
   "GB055","(AREA)", "LEN","Y",">0",   "6000",  "W","-32767","none",
   "GB055","(AREA)", "ZVA","Y",">-378","4300",  "W","-32767","none",
   "DB170","(AREA)", "SDO","Y",">=0",  "359",   "W","-32767 and -32765","none",
   "BB230","(LINE)", "WID","Y",">0",   "100",   "W","-32767","FUN=1-4",
   "AK150","(POINT)","AOO","Y",">=0",  "<360",  "E","-32767","none",
   "AK150","(LINE)", "HGT","Y",">0",   "135",   "W","-32767 and -32765","none",
   "AK150","(POINT)","HGT","Y",">0",   "135",   "W","-32767 and -32765","none",
   "AK150","(LINE)", "WID","Y",">0",   "40",    "W","-32767","FUN=1-4",
   "BI040","(LINE)", "WID","Y",">0",   "25",    "E","-32767","FUN=1-4",
   "AF010","(POINT)","HGT","Y",">0",   "380",   "W","-32767 and -32765","none",
   "AK160","(AREA)", "HGT","Y",">0",   "135",   "W","-32767 and -32765","none",
   "AK160","(POINT)","HGT","Y",">0",   "135",   "W","-32767 and -32765","none",
   "AM070","(AREA)", "HGT","Y",">0",   "50",    "W","-32767 and -32765","none",
   "AM070","(POINT)","HGT","Y",">0",   "50",    "W","-32767 and -32765","none",
   "ED020","(AREA)", "DMT","Y",">0",   "100",   "E","-32767","none",
   "EB020","(AREA)", "DMB","Y",">0",   "100",   "E","-32767","none",
   "AL241","(POINT)","HGT","Y",">0",   "635",   "W","-32767 and -32765","none",
   "BB140","(LINE)", "WID","Y",">0",   "<25",   "E","-32767","FUN=1-4",
   "AQ130","(LINE)", "WID","Y",">0",   "20",    "W","-32767","none",
   "AL260","(LINE)", "WID","Y",">0",   "20",    "W","-32767","none",
   "GB065","(AREA)", "ZVA","Y",">-378","4300",  "W","-32767","none",
   "GB065","(POINT)","ZVA","Y",">-378","4300",  "W","-32767","none",
   "BI041","(LINE)", "WID","Y",">0",   "<25",   "W","-32767","FUN=1-4",
   "BI050","(POINT)","HGT","Y",">0",   "100",   "W","-32767 and -32765","none",
   "AJ051","(POINT)","HGT","Y",">0",   "205",   "W","-32767 and -32765","none",
   "AJ050","(POINT)","HGT","Y",">0",   "45",    "W","-32767 and -32765","none",
   "EC030","(AREA)", "DMT","Y",">0",   "100",   "E","-32767","none"
};


extern double NO_U_MIN,NO_U_MAX,smalljunkflag;

extern int LEN_Getter(char *attrcode,char *geomstring,char *fcode,double *minVAL,double *maxVAL, 
			   double minaccept,double maxaccept);
extern char *GetBUAName(int english);
extern char *GetQanatName(int english);
extern char *GetRockName(int english);
extern char *GetOrchardName(int english);
extern char *GetTreesName(int english);
extern char *GetScrubName(int english);
extern char *GetATCName(int english);
extern char *GetRKFName(int english);
extern char *GetDMTName(int english);
extern char *GetDMBName(int english);
extern char *GetCoastName(int english);
extern char *GetNMName(int english);

extern unsigned char area_bridgeTRS        [numTRS];
extern unsigned char line_bridgeTRS        [numTRS];
extern unsigned char line_causewaystrTRS   [numTRS];
extern unsigned char line_ferrycrossingTRS [numTRS];
extern unsigned char point_ferrycrossingTRS[numTRS];
extern unsigned char area_transStationTRS  [numTRS];
extern unsigned char point_transStationTRS [numTRS];
extern unsigned char area_tunnelTRS        [numTRS];
extern unsigned char line_tunnelTRS        [numTRS];
extern unsigned char area_damTRS           [numTRS];
extern unsigned char line_damTRS           [numTRS];
extern unsigned char point_damTRS          [numTRS];
extern unsigned char area_embankmentTRS    [numTRS];
extern unsigned char line_embankmentTRS    [numTRS];
extern unsigned char line_cutTRS           [numTRS];



extern int    *LLFidTable;
extern int LLFidTableCount,INscc_loop;

extern int NOTYPE;
extern int C_TYPE;
extern int D_TYPE;
extern int I_TYPE;

extern int LONG_ATTR_HGT;
extern int LONG_ATTR_NAM;
extern int LONG_ATTR_ZV2;
extern int LONG_ATTR_LEN;

extern int FIND_ENUM;
extern int FIND_STRING;
extern int FIND_REAL;

extern int SLASHTYPE;




void FreeShareSegData()
{

  if(global_bridge_wid!=NULL)
  {
	free(global_bridge_wid);
	global_bridge_wid = NULL;
  }

  if(global_bridge_wd1!=NULL)
  {
	free(global_bridge_wd1);
	global_bridge_wd1 = NULL;
  }

  if(global_road_wd1!=NULL)
  {
    free(global_road_wd1);
	global_road_wd1 = NULL;
  }

  if(global_cart_wd1!=NULL)
  {
    free(global_cart_wd1);
	global_cart_wd1 = NULL;
  }


  road_wd1_val_found   = 0;
  cart_wd1_val_found   = 0;
  bridge_wid_val_found = 0;
  bridge_wd1_val_found = 0;
}


int WD1IsLarger(double wd1_num,double wid_num)
{
  if(
	 (ATTRTYPE_D4()) ||  
	 (ATTRTYPE_FACC()) ||  
	 (ATTRTYPE_VMAP()) ||  
	 (ATTRTYPE_UFD2())||  
	 (ATTRTYPE_UFD1())||  
	 (ATTRTYPE_UFD3())    
	 )
	 {
	   if ( (wd1_num/10.0) > wid_num )  
	   {
		 return 1;
	   } 
	   return 0;
	 }

  
  return (wd1_num>wid_num);
}

char *GetSEPName(int english)
{

	if((ATTRTYPE_D4())||(ATTRTYPE_FACC()))
  {
    if(english==1)
	{
	  return "Road Separation Category";
	}

	return "RoadSeparationCategory";
  }

  return "Divided";
}


int GenerateShareSegInfo(int *retval1, int *retval2, int *retval3, int *retval4)
{
  int i,j,thisindex,thatindex,insertindex1=0,insertindex2=0,insertindex3=0,insertindex4=0;
  double tempval,LOW_LIMIT=0.0,HIGH_LIMIT=900.0,WD1multiplier=1.0;
  extern char *GetWD1Name();
  extern void SortReals(double sortarray[],int p,int r);

  cart_wd1_val_found   = 0;
  road_wd1_val_found   = 0;
  bridge_wid_val_found = 0;
  bridge_wd1_val_found = 0;


  if(
	 (ATTRTYPE_D4()) ||  
	 (ATTRTYPE_FACC()) ||  
	 (ATTRTYPE_VMAP()) ||  
	 (ATTRTYPE_UFD2())||  
	 (ATTRTYPE_UFD1())||  
	 (ATTRTYPE_UFD3())    
	)
  {
	
	WD1multiplier = 10.0;
  }


  

  

  for(i=0;i<NumberOfModels;i++)
  {
    if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),"AP030")))
	{ 
	  

	  for(j=0;j<MdlNames[i].numattributes;j++)
	  { 	    
	    thisindex = MdlNames[i].AttrIndices[j];
	    thatindex = MdlNames[i].ValueIndices[j];

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),GetWD1Name()))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<(HIGH_LIMIT*WD1multiplier)))
			{
			  road_wd1_val_found = road_wd1_val_found + 1;
			}
		  }
		}
	  }
	}

    else if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),"AP010")))
	{ 
	  

	  for(j=0;j<MdlNames[i].numattributes;j++)
	  { 	    
	    thisindex = MdlNames[i].AttrIndices[j];
	    thatindex = MdlNames[i].ValueIndices[j];

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),GetWD1Name()))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<(HIGH_LIMIT*WD1multiplier)))
			{
			  cart_wd1_val_found = cart_wd1_val_found + 1;
			}
		  }
		}
	  }
	}
   else if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),"AQ040")))
	{
	  
	  for(j=0;j<MdlNames[i].numattributes;j++)
	  {
	    thisindex = MdlNames[i].AttrIndices[j];
	    thatindex = MdlNames[i].ValueIndices[j];

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"WID"))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<HIGH_LIMIT))
			{
			  bridge_wid_val_found = bridge_wid_val_found + 1;
			}
		  }
		}

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"WD1"))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<(HIGH_LIMIT*WD1multiplier)))
			{
			  bridge_wd1_val_found = bridge_wd1_val_found + 1;
			}
		  }
		}
	  }
	}
  }


  


  


  global_bridge_wid = (double *)malloc(SzD*(bridge_wid_val_found+5));
  global_bridge_wd1 = (double *)malloc(SzD*(bridge_wd1_val_found+5));
  global_road_wd1   = (double *)malloc(SzD*(road_wd1_val_found  +5));
  global_cart_wd1   = (double *)malloc(SzD*(cart_wd1_val_found  +5));

  

  insertindex1 = 0;
  insertindex2 = 0;
  insertindex3 = 0;
  insertindex4 = 0;

  for(i=0;i<NumberOfModels;i++)
  {
    if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),"AP030")))
	{ 
	  

	  for(j=0;j<MdlNames[i].numattributes;j++)
	  { 	    
	    thisindex = MdlNames[i].AttrIndices[j];
	    thatindex = MdlNames[i].ValueIndices[j];

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),GetWD1Name()))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<(HIGH_LIMIT*WD1multiplier)))
			{
			  global_road_wd1[insertindex1] = tempval;
			  insertindex1 = insertindex1 + 1;
			}
		  }
		}
	  }
	}
   else if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),"AP010")))
	{ 
	  

	  for(j=0;j<MdlNames[i].numattributes;j++)
	  { 	    
	    thisindex = MdlNames[i].AttrIndices[j];
	    thatindex = MdlNames[i].ValueIndices[j];

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),GetWD1Name()))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<(HIGH_LIMIT*WD1multiplier)))
			{
			  global_cart_wd1[insertindex3] = tempval;
			  insertindex3 = insertindex3 + 1;
			}
		  }
		}
	  }
	}
   else if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),"AQ040")))
	{
	  
	  for(j=0;j<MdlNames[i].numattributes;j++)
	  {
	    thisindex = MdlNames[i].AttrIndices[j];
	    thatindex = MdlNames[i].ValueIndices[j];

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"WID"))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<HIGH_LIMIT))
			{
			  global_bridge_wid[insertindex2] = tempval;
			  insertindex2 = insertindex2 + 1;
			}
		  }
		}

	    if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"WD1"))
		{
		  
		  if(
			  (MdlNames2[thisindex].values[thatindex].type==0) ||
			  (MdlNames2[thisindex].values[thatindex].type==1)
			)
		  {
			tempval = (double)MdlNames2[thisindex].values[thatindex].value;
			if((tempval>LOW_LIMIT)&&(tempval<(HIGH_LIMIT*WD1multiplier)))
			{
			  global_bridge_wd1[insertindex4] = tempval;
			  insertindex4 = insertindex4 + 1;
			}
		  }
		}
	  }
	}
  }

  if(insertindex1!=road_wd1_val_found)
  {
	
	printf("error: insertindex1!=road_wd1_val_found %d %d\n",insertindex1,road_wd1_val_found);
    FreeShareSegData();
    return 0;
  }
  if(insertindex2!=bridge_wid_val_found)
  {
	
	printf("error: insertindex2!=bridge_wid_val_found %d %d\n",insertindex2,bridge_wid_val_found);
    FreeShareSegData();
    return 0;
  }
  if(insertindex3!=cart_wd1_val_found)
  {
	
	printf("error: insertindex3!=cart_wd1_val_found %d %d\n",insertindex2,cart_wd1_val_found);
    FreeShareSegData();
    return 0;
  }
  if(insertindex4!=bridge_wd1_val_found)
  {
	
	printf("error: insertindex4!=bridge_wd1_val_found %d %d\n",insertindex4,bridge_wd1_val_found);
    FreeShareSegData();
    return 0;
  }


  

  SortReals(global_road_wd1,  0,road_wd1_val_found  -1);
  SortReals(global_cart_wd1,  0,cart_wd1_val_found  -1);
  SortReals(global_bridge_wid,0,bridge_wid_val_found-1);
  SortReals(global_bridge_wd1,0,bridge_wd1_val_found-1);


  



  

  for(i=0;i<bridge_wid_val_found;i++)
  {
	if(road_wd1_val_found>0)
	{
	  if(WD1IsLarger(global_road_wd1[road_wd1_val_found-1],global_bridge_wid[i]))
	  { 
	    *retval1 = *retval1 + 1; 
	  } 
	}
  }

  for(i=0;i<bridge_wid_val_found;i++)
  {
	if(cart_wd1_val_found>0)
	{
	  if(WD1IsLarger(global_cart_wd1[cart_wd1_val_found-1],global_bridge_wid[i]))
	  { 
	    *retval2 = *retval2 + 1; 
	  } 
	}
  }

  

  if(insertindex4 < (insertindex1+insertindex3) )
  {
    *retval3 = insertindex4;
  }
  else
  {
    *retval3 = insertindex1 + insertindex3;
  }

  if(insertindex4 < insertindex1)
  {
    *retval4 = insertindex4;
  }
  else
  {
    *retval4 = insertindex1;
  }



  

  return 1;
}




void FreeOtherShareSegData()
{
  if(global_bridge_len!=NULL)
  {
	free(global_bridge_len);
	global_bridge_len = NULL;
  }

  if(global_span_len!=NULL)
  {
	free(global_span_len);
	global_span_len = NULL;
  }

  if(global_bridge_zvh!=NULL)
  {
	free(global_bridge_zvh);
	global_bridge_zvh = NULL;
  }

  if(global_span_zvh!=NULL)
  {
	free(global_span_zvh);
	global_span_zvh = NULL;
  }

  if(global_bridge_hgt!=NULL)
  {
	free(global_bridge_hgt);
	global_bridge_hgt = NULL;
  }

  if(global_span_hgt!=NULL)
  {
	free(global_span_hgt);
	global_span_hgt = NULL;
  }

  bridge_len_val_found  = 0;
  span_len_val_found    = 0;
  bridge_hgt_val_found  = 0;
  span_hgt_val_found    = 0;
  bridge_zvh_val_found  = 0;
  span_zvh_val_found    = 0;
}



int GenerateOtherShareSegInfo(int *retval1, int *retval2, int *retval3, 
							  int *retval1_bridgefirst, int *retval2_bridgefirst, int *retval3_bridgefirst)
{
  double LOW_LIMIT=-900.0,HIGH_LIMIT=10000000.0,Dval,oldval;
  int i,thistype,thisecc,thisgeom,Ival,insertindex,
	  insertindex1=0,insertindex2=0,insertindex3=0,insertindex4=0,
	  insertindex5=0,insertindex6=0,bridge_index,span_index,
	  retval1a=0,retval2a=0,retval3a=0,retval1b=0,retval2b=0,retval3b=0;
  char cleancode[1000];
  extern void SortReals(double sortarray[],int p,int r);


  

  *retval1 = 0;
  *retval2 = 0;
  *retval3 = 0;

  *retval1_bridgefirst = 0;
  *retval2_bridgefirst = 0;
  *retval3_bridgefirst = 0;

  insertindex1 = 0;
  insertindex2 = 0;
  insertindex3 = 0;
  insertindex4 = 0;
  insertindex5 = 0;
  insertindex6 = 0;



  


  LOW_LIMIT = 0; 


  bridge_index = GetCodeIndex("AQ040");
  span_index   = GetCodeIndex("AQ045");


  BuildLLTable(1,LLFidTableCount);
  Begin_LongFiles(LONG_ATTR_LEN);

  for(i=0;i<LLFidTableCount;i++)
    { 
      if(LLFidTable[i]<0)
	{
	  continue;
	} 
      
      
      thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
      thisgeom =          CrsWlk[LLFidTable[i]].geomtype;
      
      if(thisgeom!=C_LINE)
	{
	  continue;
	}
      if((thisecc!=bridge_index)&&(thisecc!=span_index))
	{
	  continue;
	}
      
      thistype = GetLongAttrVal(LONG_ATTR_LEN,i,&Ival,&Dval,&cleancode[0]);
      

      if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
	{
      if(thistype==I_TYPE)
	  { 
        Dval = (double)Ival;
	  }  

	  

      if((Dval>LOW_LIMIT)&&(Dval<HIGH_LIMIT))
	  {
	    if(thisecc==bridge_index)
		{ 
          bridge_len_val_found = bridge_len_val_found + 1;
		} 
	    else if(thisecc==span_index)
		{ 
          span_len_val_found = span_len_val_found + 1;
		} 
	  }
	}
  }


  global_bridge_len = (double *)malloc(SzD*(bridge_len_val_found+5));
  global_span_len   = (double *)malloc(SzD*(span_len_val_found+5));


  for(i=0;i<LLFidTableCount;i++)
    { 
      if(LLFidTable[i]<0)
	{
	  continue;
	} 
      
      thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
      thisgeom =          CrsWlk[LLFidTable[i]].geomtype;
      
      if(thisgeom!=C_LINE)
	{
	  continue;
	}
      if((thisecc!=bridge_index)&&(thisecc!=span_index))
	{
	  continue;
	}
      
      thistype = GetLongAttrVal(LONG_ATTR_LEN,i,&Ival,&Dval,&cleancode[0]);


    if(thistype==I_TYPE)
	{ 
      Dval = (double)Ival;
	} 
	if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
	{
	  

      if((Dval>LOW_LIMIT)&&(Dval<HIGH_LIMIT))
	  {
	    if(thisecc==bridge_index)
		{ 
          global_bridge_len[insertindex1] = Dval;
          insertindex1 = insertindex1 + 1;
		} 
	    else if(thisecc==span_index)
		{ 
          global_span_len[insertindex2] = Dval;
          insertindex2 = insertindex2 + 1;
		} 
	  }
	}
  }


  FreeLLTable(); 
  End_LongFiles(LONG_ATTR_LEN);














  

  LOW_LIMIT = 0; 


  bridge_index = GetCodeIndex("AQ040");
  span_index   = GetCodeIndex("AQ045");


  BuildLLTable(1,LLFidTableCount);
  Begin_LongFiles(LONG_ATTR_HGT);

  for(i=0;i<LLFidTableCount;i++)
    { 
      if(LLFidTable[i]<0)
	{
	  continue;
	} 
      
      
      thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
      thisgeom =          CrsWlk[LLFidTable[i]].geomtype;
      
      if(thisgeom!=C_LINE)
	{
	  continue;
	}
      if((thisecc!=bridge_index)&&(thisecc!=span_index))
	{
	  continue;
	}
      
      thistype = GetLongAttrVal(LONG_ATTR_HGT,i,&Ival,&Dval,&cleancode[0]);
      

      if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
	{
      if(thistype==I_TYPE)
	  { 
        Dval = (double)Ival;
	  }  

	  

      if((Dval>LOW_LIMIT)&&(Dval<HIGH_LIMIT))
	  {
	    if(thisecc==bridge_index)
		{ 
          bridge_hgt_val_found = bridge_hgt_val_found + 1;
		} 
	    else if(thisecc==span_index)
		{ 
          span_hgt_val_found = span_hgt_val_found + 1;
		} 
	  }
	}
  }


  global_bridge_hgt = (double *)malloc(SzD*(bridge_hgt_val_found+5));
  global_span_hgt   = (double *)malloc(SzD*(span_hgt_val_found+5));


  for(i=0;i<LLFidTableCount;i++)
    { 
      if(LLFidTable[i]<0)
	{
	  continue;
	} 
      
      thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
      thisgeom =          CrsWlk[LLFidTable[i]].geomtype;
      
      if(thisgeom!=C_LINE)
	{
	  continue;
	}
      if((thisecc!=bridge_index)&&(thisecc!=span_index))
	{
	  continue;
	}
      
      thistype = GetLongAttrVal(LONG_ATTR_HGT,i,&Ival,&Dval,&cleancode[0]);


    if(thistype==I_TYPE)
	{ 
      Dval = (double)Ival;
	} 
	if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
	{
	  

      if((Dval>LOW_LIMIT)&&(Dval<HIGH_LIMIT))
	  {
	    if(thisecc==bridge_index)
		{ 
          global_bridge_hgt[insertindex3] = Dval;
          insertindex3 = insertindex3 + 1;
		} 
	    else if(thisecc==span_index)
		{ 
          global_span_hgt[insertindex4] = Dval;
          insertindex4 = insertindex4 + 1;
		} 
	  }
	}
  }


  FreeLLTable(); 
  End_LongFiles(LONG_ATTR_HGT);



  


  LOW_LIMIT = -400; 

  Begin_LongFiles(LONG_ATTR_ZV2);

  for(i=0;i<LLFidTableCount;i++)
    { 
      if(LLFidTable[i]<0)
	{
	  continue;
	} 
      
      
      thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
      thisgeom =          CrsWlk[LLFidTable[i]].geomtype;
      
      if(thisgeom!=C_LINE)
	{
	  continue;
	}
      if((thisecc!=bridge_index)&&(thisecc!=span_index))
	{
	  continue;
	}

      thistype = GetLongAttrVal(LONG_ATTR_ZV2,i,&Ival,&Dval,&cleancode[0]);


	if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
	{
      if(thistype==I_TYPE)
	  {
        Dval = (double)Ival;
	  }

	  

      if((Dval>=LOW_LIMIT)&&(Dval<HIGH_LIMIT))
	  {
	    if(thisecc==bridge_index)
		{
          bridge_zvh_val_found = bridge_zvh_val_found + 1;
		}
	    else if(thisecc==span_index)
		{
          span_zvh_val_found = span_zvh_val_found + 1;
		}
	  }
	}
  }


  global_bridge_zvh = (double *)malloc(SzD*(bridge_zvh_val_found+5));
  global_span_zvh   = (double *)malloc(SzD*(span_zvh_val_found+5));


  for(i=0;i<LLFidTableCount;i++)
    { 
      if(LLFidTable[i]<0)
	{
	  continue;
	} 
      
      
      thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
      thisgeom =          CrsWlk[LLFidTable[i]].geomtype;
      
      if(thisgeom!=C_LINE)
	{
	  continue;
	}
      if((thisecc!=bridge_index)&&(thisecc!=span_index))
	{
	  continue;
	}
      
      thistype = GetLongAttrVal(LONG_ATTR_ZV2,i,&Ival,&Dval,&cleancode[0]);
      

    if(thistype==I_TYPE)
	{ 
      Dval = (double)Ival;
	} 
	if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
	{
	  

      if((Dval>=LOW_LIMIT)&&(Dval<HIGH_LIMIT))
	  {
	    if(thisecc==bridge_index)
		{ 
          global_bridge_zvh[insertindex5] = Dval;
          insertindex5 = insertindex5 + 1;
		} 
	    else if(thisecc==span_index)
		{ 
          global_span_zvh[insertindex6] = Dval;
          insertindex6 = insertindex6 + 1;
		} 
	  }
	}
  }


  FreeLLTable();
  End_LongFiles(LONG_ATTR_ZV2);




  SortReals(global_bridge_len,  0,bridge_len_val_found -1);
  SortReals(global_span_len,    0,span_len_val_found   -1);
  SortReals(global_bridge_hgt,  0,bridge_hgt_val_found -1);
  SortReals(global_span_hgt,    0,span_hgt_val_found   -1);
  SortReals(global_bridge_zvh,  0,bridge_zvh_val_found -1);
  SortReals(global_span_zvh,    0,span_zvh_val_found   -1);


  


  if(bridge_len_val_found>0)
  {
    oldval = global_bridge_len[0];
	insertindex = 1;

    for(i=0;i<bridge_len_val_found;i++)
	{
      if(global_bridge_len[i]!=oldval)
	  {
        global_bridge_len[insertindex] = global_bridge_len[i];
		insertindex = insertindex + 1;
		oldval = global_bridge_len[i];
	  }
	} 
    bridge_len_val_found = insertindex;
  }

  if(span_len_val_found>0)
  {
    oldval = global_span_len[0];
	insertindex = 1;

    for(i=0;i<span_len_val_found;i++)
	{
      if(global_span_len[i]!=oldval)
	  {
        global_span_len[insertindex] = global_span_len[i];
		insertindex = insertindex + 1;
		oldval = global_span_len[i];
	  }
	} 
    span_len_val_found = insertindex;
  }






  if(bridge_hgt_val_found>0)
  {
    oldval = global_bridge_hgt[0];
	insertindex = 1;

    for(i=0;i<bridge_hgt_val_found;i++)
	{
      if(global_bridge_hgt[i]!=oldval)
	  {
        global_bridge_hgt[insertindex] = global_bridge_hgt[i];
		insertindex = insertindex + 1;
		oldval = global_bridge_hgt[i];
	  }
	} 
    bridge_hgt_val_found = insertindex;
  }

  if(span_hgt_val_found>0)
  {
    oldval = global_span_hgt[0];
	insertindex = 1;

    for(i=0;i<span_hgt_val_found;i++)
	{
      if(global_span_hgt[i]!=oldval)
	  {
        global_span_hgt[insertindex] = global_span_hgt[i];
		insertindex = insertindex + 1;
		oldval = global_span_hgt[i];
	  }
	} 
    span_hgt_val_found = insertindex;
  }



  if(bridge_zvh_val_found>0)
  {
    oldval = global_bridge_zvh[0];
	insertindex = 1;

    for(i=0;i<bridge_zvh_val_found;i++)
	{
      if(global_bridge_zvh[i]!=oldval)
	  {
        global_bridge_zvh[insertindex] = global_bridge_zvh[i];
		insertindex = insertindex + 1;
		oldval = global_bridge_zvh[i];
	  }
	} 
    bridge_zvh_val_found = insertindex;
  }

  if(span_zvh_val_found>0)
  {
    oldval = global_span_zvh[0];
	insertindex = 1;

    for(i=0;i<span_zvh_val_found;i++)
	{
      if(global_span_zvh[i]!=oldval)
	  {
        global_span_zvh[insertindex] = global_span_zvh[i];
		insertindex = insertindex + 1;
		oldval = global_span_zvh[i];
	  }
	} 
    span_zvh_val_found = insertindex;
  }





  

  for(i=0;i<bridge_hgt_val_found;i++)
  {
	if(span_hgt_val_found>0)
	{
	  if(global_span_hgt[span_hgt_val_found-1]>global_bridge_hgt[i])
	  {
	    retval1a = retval1a + 1;  
	  }
	}
  }


  

  for(i=0;i<span_hgt_val_found;i++)
  {
	if(bridge_hgt_val_found>0)
	{
	  if(global_span_hgt[i]>global_bridge_hgt[0])
	  {
	    retval1b = retval1b + 1;  
	  }
	}
  }


  if(retval1a<=retval1b)
  {
    *retval1_bridgefirst = 1;  
	*retval1 = retval1a;
  }
  else
  {
    *retval1_bridgefirst = 0;  
	*retval1 = retval1b;
  }



  

  for(i=0;i<bridge_zvh_val_found;i++)
  {
	if(span_zvh_val_found>0)
	{
	  if(global_span_zvh[span_zvh_val_found-1]>global_bridge_zvh[i])
	  {
	    retval2a = retval2a + 1;  
	  }
	}
  }


  

  for(i=0;i<span_zvh_val_found;i++)
  {
	if(bridge_zvh_val_found>0)
	{
	  if(global_span_zvh[i]>global_bridge_zvh[0])
	  {
	    retval2b = retval2b + 1;  
	  }
	}
  }


  if(retval2a<=retval2b)
  {
    *retval2_bridgefirst = 1;  
	*retval2 = retval2a;
  }
  else
  {
    *retval2_bridgefirst = 0;  
	*retval2 = retval2b;
  }







  

  for(i=0;i<bridge_len_val_found;i++)
  {
	if(span_len_val_found>0)
	{
	  if(global_span_len[span_len_val_found-1]>global_bridge_len[i])
	  {
	    retval3a = retval3a + 1;  
	  }
	}
  }


  

  for(i=0;i<span_len_val_found;i++)
  {
	if(bridge_len_val_found>0)
	{
	  if(global_span_len[i]>global_bridge_len[0])
	  {
	    retval3b = retval3b + 1;  
	  }
	}
  }


  if(retval3a<=retval3b)
  {
    *retval3_bridgefirst = 1;  
	*retval3 = retval3a;
  }
  else
  {
    *retval3_bridgefirst = 0;  
	*retval3 = retval3b;
  }




  return 1;
}


char *GetPCFName(int i)
{
  if(i==0) {return PCF999999NAME;}
  if(i==1) {return PCF1NAME;}
  if(i==2) {return PCF2NAME;}
  if(i==3) {return PCF3NAME;}
  if(i==4) {return PCF4NAME;}
  if(i==5) {return PCF5NAME;}
  if(i==6) {return PCF6NAME;}

  printf("Error: GPCN %d\n",i);
  return "ErrorSeeConsole";

}



void DoSHARESEGother(
		int type,char *prefix,
		int *codespresent,
        int *line_water,
        int *linespresent,
	    int lines_found,
		int line_road_found,
		int line_bridge_found,
		int line_span_found,
		int line_rail_found,
		int line_armistice_found,
		int line_admin_found,
		int line_footbridge_found,
		int line_trail_found,
		int line_ctrack_found,
		int line_ford_found,
		int line_tunnel_found,
		int line_sidetrack_found,
		int line_canal_found,
		int line_aqueduct_found,
		int line_pipe_found,
		int line_interchange_found,
		int line_embankment_found,
		int line_loc40road,
		int line_loc40rail,
		int line_loc40sidetrack,
		int line_loc25road,
		int line_loc25rail,
		int line_rst1road,
		int line_wtc1road,
		int line_loc25ctrack,
		int line_loc25trail,
		int line_loc40ctrack,
		int line_loc40trail,
		int line_rle998ctrack,
		int line_rle998aqueduct,
		int line_rle998canal,
		int line_rle998pipe,
		int line_rle998road,
		int line_rle998rail,
		int line_rle998sidetrack,
		int line_cwt1001aqueduct,
		int line_cwt1001canal,
		int line_cwt1001pipe,
		int line_cwt1001road,
		int line_cwt1001rail,
		int line_cwt1001sidetrack,
		int line_sbb1000aqueduct,
		int line_sbb1000canal,
		int line_sbb1000pipe,
		int line_sbb1000road,
		int line_sbb1000rail,
		int line_sbb1000sidetrack,
		int line_loc44pipe,
		int line_loc44road,
		int line_loc44rail,
		int line_loc44sidetrack,
		int line_rle1aqueduct,
		int line_rle1canal,
		int line_rle1pipe,
		int line_rle1road,
		int line_rle1rail,
		int line_rle1sidetrack,
		int line_cwt1000aqueduct,
		int line_cwt1000canal,
		int line_cwt1000pipe,
		int line_cwt1000road,
		int line_cwt1000rail,
		int line_cwt1000sidetrack,
		int line_sbb1001aqueduct,
		int line_sbb1001canal,
		int line_sbb1001pipe,
		int line_sbb1001road,
		int line_sbb1001rail,
		int line_sbb1001sidetrack,
		int line_loc17road,
		int line_loc17cart,
		int line_loc40pipe,
		int line_loc25sidetrack,
		int line_loc25canal,
		int line_loc25pipe,
		int line_loc25aqueduct,
		int line_loc25interchange,
		int line_loc40canal,
		int line_loc40aqueduct,
		int line_loc40interchange,
		int line_cwt1001ctrack,
		int line_sbb1000ctrack,
		int line_loc44ctrack,
		int line_rle1ctrack,
		int line_cwt1000ctrack,
		int line_sbb1001ctrack,
        int line_cet2cut_found,
        int line_cet3cut_found,
        int line_rle3ctrack,
	    int line_rle3aqueduct,
        int line_rle3pipe,
        int line_rle3canal,
        int line_rle3road,
        int line_rle3rail,
        int line_rle3sidetrack,
        int line_rle3trail,
		int line_rle1trail,
		int line_causeway_struct_found,
    	int line_cwt1000trail,
    	int line_sbb1000trail,
    	int line_loc44trail,
		int line_rle998trail,
		int line_cwt1001trail,
		int line_sbb1001trail,
		int line_loc17trail,
		int line_cable_found,
		int line_overheadOHC999999,
		int line_rle2road,
		int line_rle2ctrack,
		int line_rle2trail
		)
{
  FILE *newfile;
  char message[1000],tempchar[1000],cleancode[1000],pcfname[200];
  int docheck1,docheck2,docheck3,docheck4,docheck5,docheck6,docheck7,docheck8,
    docheck9,docheck10,docheck11,docheck12,  docheck13, docheck14,docheck15,
    docheck16,docheck17,docheck18,docheck19, docheck20, docheck21,docheck22,
    docheck23,docheck24,docheck25,docheck26, docheck27, docheck28,docheck29,
    docheck30,docheck31,docheck32,docheck33, docheck34, docheck35,docheck36,
	docheck37,docheck38,docheck39,docheck40a,docheck40b,docheck41,docheck42,
	docheck43,docheck44,totalchecks,i,j,k,l,jtemp,savej,
	total_tds_isl_checks,doit=0;
  
  int num_b1,num_b2,num_b3,num_b4,num_b5,num_b6,num_b7,
	  b5_bridgefirst,b6_bridgefirst,b7_bridgefirst;


int TDS_num_ISL;
int *TDS_ISL_CODES;
char **TDS_ISL;
char *TDS_ISL_USE;


#define TDS4_num_ISL 87
char  *TDS4_ISL[TDS4_num_ISL] = {"AA011","AF020","AF021","AF050","AH025","AK020","AK080","AK130","AK150","AK155","AL018","AL060","AL070","AL080","AL130","AL140","AL155","AL195","AL211","AL260","AN010","AN050","AP010","AP030","AP040","AP041","AP050","AQ035","AQ040","AQ045","AQ050","AQ056","AQ059","AQ063","AQ065","AQ070","AQ075","AQ111","AQ113","AQ120","AQ130","AQ150","AQ151","AT005","AT041","BA010","BB081","BB082","BH010","BH020","BH030","BH065","BH070","BH100","BH110","BH120","BH140","BH165","BH180","BI006","BI020","BI030","BI040","BI044","BI045","BI060","BJ031","BJ040","CA010","DB010","DB061","DB070","DB071","DB090","DB100","DB110","DB190","EA020","EC015","EC040","FA000","FA110","GB050","GB055","GB075","ZD040","ZD045"};
int TDS4_ISL_CODES[TDS4_num_ISL];
char TDS4_ISL_USE [TDS4_num_ISL];

extern int GetZVCode();
extern char *GetZVCode2();
extern char *GetWD1Name();

char TDS4_ISL_VALS[TDS4_num_ISL][TDS4_num_ISL]=
{
 {'e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','e','e','e','e','e','w','e','e','e','e','e','e','e','e'},
 {'n','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','w','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','w'},
 {'n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','w','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','g','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','w','e','e','e','e','e','e','e','w','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','w','w','e','e'},
 {'n','n','n','n','n','n','n','n','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','g','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','e','e','e','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','g','g','e','e','g','g','g','g','g','g','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','g','g','g','e','e','g','g','e','e','e','e','g','g','e','e','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','e','g','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','w','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','e','g','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','w','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','w','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','w','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','e','g','g','e','g','e','e','e','g','g','e','e','e','e','g','e','e','e','w','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','w','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','g','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','g','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','w','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','g','g','e','e','g','e','e','e','e','e','e','g','g','g','e','e','e','g','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','g','g','e','e','e','e','g','g','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','g','g','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e'}
 };



#define TDS6_num_ISL 83
char  *TDS6_ISL[TDS6_num_ISL] = {"AA011","AF020","AF050","AH025","AK020","AK080","AK130","AK150","AK155","AL018","AL060","AL070","AL080","AL130","AL140","AL155","AL195","AL211","AL260","AN010","AN050","AP010","AP030","AP040","AP041","AP050","AP055","AQ035","AQ040","AQ045","AQ050","AQ056","AQ059","AQ063","AQ065","AQ070","AQ075","AQ113","AQ120","AQ130","AQ150","AQ151","AT005","AT041","BA010","BB081","BB082","BH010","BH020","BH030","BH065","BH070","BH100","BH110","BH120","BH140","BH165","BH180","BI006","BI020","BI030","BI040","BI044","BI045","BI060","BJ031","BJ040","CA010","DB010","DB061","DB070","DB071","DB090","DB100","DB110","DB190","EA020","EC015","EC040","GB050","GB075","ZD040","ZD045"};
int TDS6_ISL_CODES[TDS6_num_ISL];
char TDS6_ISL_USE [TDS6_num_ISL];


char TDS6_ISL_VALS[TDS6_num_ISL][TDS6_num_ISL]=
{
 {'e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','e','e','e','e','e','w','e','e','e','e','e'},
 {'n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','w','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','w'},
 {'n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','w','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','w','e','e','e','e','e','e','e','w','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','w','e','e'},
 {'n','n','n','n','n','n','n','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','e','e','e','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','g','g','e','e','g','g','g','g','g','g','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','g','g','g','e','e','g','g','g','e','e','e','e','g','g','e','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','g','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','g','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','w','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','g','g','g','e','g','g','e','e','e','e','g','g','e','g','e','e','e','g','g','e','e','e','e','g','e','e','e','w','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','g','g','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','g','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','g','g','e','g','g','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','w','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','g','g','e','e','g','e','e','e','e','e','g','g','g','e','e','e','g','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','g','e','e','e','e','e','e','e','e','e','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','g','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','g','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','g','e','g','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','g','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','g','e','e','g','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','g','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','g','g','e','e','g','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','g','g','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e','e'},
 {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','e'}
};

 extern unsigned char line_bridgePCF    [numPCF];
 extern unsigned char line_tunnelPCF    [numPCF];
 extern unsigned char line_aqueductPCF  [numPCF];
 extern unsigned char line_canalPCF     [numPCF];
 extern unsigned char line_pipePCF      [numPCF];
 extern unsigned char line_railPCF      [numPCF];
 extern unsigned char line_roadPCF      [numPCF];
 extern unsigned char line_ctrackPCF    [numPCF];
 extern unsigned char line_trailPCF     [numPCF];
 extern unsigned char line_sidetrackPCF [numPCF];
 extern unsigned char line_riverPCF     [numPCF];
 extern unsigned char line_ditchPCF     [numPCF];
 extern unsigned char line_taxiwayPCF   [numPCF];

 extern void ExeUpdate(char message[]);
 extern char *GetRailName(int english);
 extern char *GetBridgeName(int english);
 extern char *GetRoadName(int english);
 extern char *GetSideTrackName(int english);
 extern char *GetPipelineName(int english);
 extern char *GetInterchangeName(int english);
 extern char *GetEmbankmentName(int english);
 extern char *GetWD1Name2();
 extern char *GetContourLineName(int english);
 extern char *GetLengthCode();
 extern char *GetLOCName(int english);
 extern char *GetUnderGroundName(int english);
 extern char *GetRSTName();
 extern char *GetCETName(int english);
 extern char *GetRiverName(int english);
 extern char *GetOverheadName(int english);

  if(ATTRTYPE_TDS4())
  {
    TDS_num_ISL   = TDS4_num_ISL;
    TDS_ISL       = TDS4_ISL;
    TDS_ISL_CODES = TDS4_ISL_CODES;
	TDS_ISL_USE   = TDS4_ISL_USE;
  }
  else
  {
    TDS_num_ISL   = TDS6_num_ISL;
    TDS_ISL       = TDS6_ISL;
    TDS_ISL_CODES = TDS6_ISL_CODES;
	TDS_ISL_USE   = TDS6_ISL_USE;
  }


  pcfname[0] = '\0';

  if((type==1)||(type==3)||(type==4)||(type==5)||(type==6)||(type==7)||(type==8)||(type==11))
    {
      
      
      sprintf(message,"%sSHARESEG.txt",prefix);
      newfile = fopen(message,"w");
      ExeUpdate(message);

      docheck1   = 0;
      docheck2   = 0;
      docheck3   = 0;
      docheck4   = 0;
      docheck5   = 0;
      docheck6   = 0;
      docheck7   = 0;
      docheck8   = 0;
      docheck9   = 0;
      docheck10  = 0;
      docheck11  = 0;
      docheck12  = 0;
      docheck13  = 0;
      docheck14  = 0;
      docheck15  = 0;
      docheck16  = 0;
      docheck17  = 0;
      docheck18  = 0;
      docheck19  = 0;
      docheck20  = 0;
      docheck21  = 0;
      docheck22  = 0;
      docheck23  = 0;
      docheck24  = 0;
      docheck25  = 0; 
      docheck26  = 0;
      docheck27  = 0;
      docheck28  = 0;
      docheck29  = 0;
      docheck30  = 0;
      docheck31  = 0;
      docheck32  = 0;
      docheck33  = 0;
      docheck34  = 0;
      docheck35  = 0;
      docheck36  = 0;
      docheck37  = 0;
      docheck38  = 0;
      docheck39  = 0;
      docheck40a = 0;
      docheck40b = 0;
      docheck41  = 0;
      docheck42  = 0;
	  docheck43  = 0;
	  docheck44  = 0;

      total_tds_isl_checks = 0;
      
      num_b1 = 0;
      num_b2 = 0;
      num_b3 = 0;
      num_b4 = 0;
      num_b5 = 0; 
      num_b6 = 0; 
      num_b7 = 0;   
      b5_bridgefirst = 0;
	  b6_bridgefirst = 0;
	  b7_bridgefirst = 0;

      if(  ((type==1)||(type==5)||(type==6)||(type==7)||(type==8)||(type==11))  &&  (line_road_found>0)  ) 
	{
	  GenerateShareSegInfo(&num_b1,&num_b2,&num_b3,&num_b4);
	}


	  if((line_bridge_found>0)&&(line_span_found>0))
	  {
	    if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
		{ 
		  
          GenerateOtherShareSegInfo(&num_b5,&num_b6,&num_b7,&b5_bridgefirst,&b6_bridgefirst,&b7_bridgefirst);


		  

		  if( (LLFidTableCount*2.0*num_b5)>33000000 )
		  {
            printf("*** Not performing Bridge / Bridge Span HGT inspection due to %d instantiations needed and %d (%lf)\n",num_b5,LLFidTableCount,LLFidTableCount*2.0*num_b5);
			num_b5 = 0;
		  }
		  if( (LLFidTableCount*2.0*num_b6)>33000000 )
		  {
            printf("*** Not performing Bridge / Bridge Span ZVH inspection due to %d instantiations needed and %d (%lf)\n",num_b6,LLFidTableCount,LLFidTableCount*2.0*num_b6);
			num_b6 = 0;
		  }
		  if( (LLFidTableCount*2.0*num_b7)>33000000 )
		  {
            printf("*** Not performing Bridge / Bridge Span LEN inspection due to %d instantiations needed and %d (%lf)\n",num_b7,LLFidTableCount,LLFidTableCount*2.0*num_b7);
			num_b7 = 0;
		  }
		} 
	  }

      if((line_road_found>0)||(line_rail_found>0))
	  {
        if((type!=5)&&(type!=6)&&(type!=7)&&(type!=8)&&(type!=11))
		{
	      for(i=0; i<INscc_loop;i++)
		  {
	        if((codespresent[i]==1)&&(line_water[i]==1))
			{
		      if(line_road_found>0)
			  {
		        docheck1 = 1;
			  }
		      if(line_rail_found>0)
			  {
		        docheck2 = 1;
			  }
			}
		  }
		}
      }
 
      
      
      if(type==3)
	{
 	  if((line_armistice_found>0)&&(line_admin_found>0))
	    {
	      
	      docheck3 = 1;
	    }
	  
	  if((line_footbridge_found>0)&&((line_road_found+line_trail_found+line_ctrack_found)>0))
	    {
	      
	      docheck4 = 1;
	    }
	  
	  if((line_ford_found==1)&& ((line_ctrack_found>0)||(line_trail_found>0)) )
	    {
	      docheck5 = 1;
	    }
	  
	  if(
	      (ATTRTYPE_ANY_TDS()==0) &&
          (ATTRTYPE_ANY_GGDM()==0)  
	     )
	    {
	      if((line_road_found>0)&&(line_bridge_found>0))
		  { 
		    
		    
		    docheck10 = 1;
		  } 
	      if((line_road_found>0)&&(line_bridge_found>0))
		  { 
		    
		    
		    docheck11 = 1;
		  }  
	    }
	}
      
      
      
      if((type==3)||(type==4))
	{
	  
	  if((line_road_found>0)&&(line_tunnel_found>0))
	    {
	      docheck6 = 1;
	    }
	  if((line_rail_found>0)&&(line_tunnel_found>0))
	    {
	      docheck7 = 1;
	    }
	 
	      if(
			  (ATTRTYPE_ANY_MGCP()==0) &&
			  (ATTRTYPE_ANY_TDS() ==0) &&
		      (ATTRTYPE_DFDD()    ==0) &&
		      (ATTRTYPE_NFDD()    ==0) &&
              (ATTRTYPE_ANY_GGDM()==0)
		 )
		{ 
		  
		  if((line_road_found>0)&&(line_bridge_found>0))
		    {
		      docheck8 = 1;
		    }
		  if((line_rail_found>0)&&(line_bridge_found>0))
		    {
		      docheck9 = 1;
		    }
	    }
	  
	  
	  
	  if(type==4)
	    {
	      if(
			  (ATTRTYPE_ANY_MGCP()==0) &&
			  (ATTRTYPE_ANY_TDS() ==0) &&
		      (ATTRTYPE_DFDD()    ==0) &&
		      (ATTRTYPE_NFDD()    ==0) &&
              (ATTRTYPE_ANY_GGDM()==0)
		 )
		{
		  
		  if((line_ctrack_found>0)&&(line_bridge_found>0))
		    {
		      docheck12 = 1;
		    }
		  if((line_trail_found>0)&&(line_bridge_found>0))
		    {
		      docheck13 = 1;
		    }
		}
	      
	      if((line_ctrack_found>0)&&(line_tunnel_found>0))
		{
		  docheck14 = 1;
		}
	      if((line_trail_found>0)&&(line_tunnel_found>0))
		{
		  docheck15 = 1;
		} 
	    }	  
	}


    if((line_ctrack_found>0)&&(line_bridge_found>0))
	{
	  if(type==1)
	  {
        docheck16 = 1;
	  }
	  else if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
	  {
         
	  }
	}



	if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))  
	{
      if(
         (ATTRTYPE_ANY_TDS()) ||
	     (ATTRTYPE_ANY_GGDM())
		)
	  {

        
	    if(line_tunnel_found>0)
		{
	      if(
			   ((line_road_found>0)||(line_rail_found>0)||(line_sidetrack_found>0)||(line_canal_found>0)||(line_aqueduct_found>0)||(line_pipe_found>0))
			   ||
               (   (line_ctrack_found>0)&&
			       (
                     (ATTRTYPE_ANY_TDS()) ||
					 (ATTRTYPE_ANY_GGDM())
				   )
			   )
             )
		  {
	        docheck17 = 1;  
	        docheck18 = 1;  
	        docheck19 = 1;  
		  }


	      if(
			  ((line_road_found>0)||(line_rail_found>0)||(line_sidetrack_found>0)||(line_pipe_found>0))
			  ||
               (   (line_ctrack_found>0)&&
			       (
                     (ATTRTYPE_ANY_TDS()) ||
					 (ATTRTYPE_ANY_GGDM())
				   )
			   )
             )
		  {
	        docheck27 = 1;  
		  }

		}

		if(line_bridge_found>0)
		{
	      if(
			  ((line_road_found>0)||(line_rail_found>0)||(line_sidetrack_found>0)||(line_pipe_found>0))
			  ||
               (   (line_ctrack_found>0)&&
			       (
                     (ATTRTYPE_ANY_TDS()) ||
					 (ATTRTYPE_ANY_GGDM())
				   )
			   )
             )
		  {
	        docheck20 = 1;  
		  }

  	      if(
			  ((line_road_found>0)||(line_rail_found>0)||(line_sidetrack_found>0)||(line_canal_found>0)||(line_aqueduct_found>0)||(line_pipe_found>0))
			  ||
               (   (line_ctrack_found>0)&&
			       (
                     (ATTRTYPE_ANY_TDS()) ||
					 (ATTRTYPE_ANY_GGDM())
				   )
			   )
             )
		  {
	        docheck21 = 1;  
	        docheck22 = 1;  
	        docheck23 = 1;  
		  }
		}

		if( (line_cet2cut_found+line_cet3cut_found) > 0 )
		{
          if( 
			  (line_ctrack_found+line_aqueduct_found+line_pipe_found+
			   line_rail_found+line_sidetrack_found+line_canal_found+
               line_trail_found+line_road_found) > 0
			)
		  {
             docheck24 = 1;
		  }
		}


        if(line_ford_found>0)
		{
		  if((line_road_found+line_ctrack_found+line_trail_found)>0)
		  { 
		    docheck26 = 1;  
		  } 
		}

	  }
	}


	if(type==1)
	{
	  if( ((line_road_found+line_rail_found+line_sidetrack_found+line_canal_found+line_aqueduct_found+line_interchange_found+line_pipe_found)>0)   && (line_bridge_found>0) )
	    {
	      docheck28 = 1;
	    }
	  if( ((line_road_found+line_rail_found+line_sidetrack_found+line_canal_found+line_aqueduct_found+line_interchange_found+line_pipe_found)>0)   && (line_tunnel_found>0) )
	    {
	      docheck29 = 1;
	    }
	}

	if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
	{
      if( (line_embankment_found+line_causeway_struct_found) >0)
	  {
        if( 
			(line_ctrack_found+line_aqueduct_found+line_pipe_found+
			 line_rail_found+line_sidetrack_found+line_canal_found+
             line_trail_found+line_road_found) > 0)
		{
          docheck30 = 1;
		}
        if( 
			(line_ctrack_found+
			 line_rail_found+line_sidetrack_found+
             line_trail_found+line_road_found) > 0)
		{
          docheck31 = 1;
          docheck32 = 1;
          docheck33 = 1;
		}
	  }

	  if((line_road_found>0)||(line_ctrack_found>0))
	  {
		if(line_ford_found>0)
		{ 
		  docheck34 = 1;  
		  docheck35 = 1;  
		  docheck36 = 1;  
		} 
	  }

	  if((line_bridgePCF[PCF999999]+line_tunnelPCF[PCF999999])>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==0){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck37 = 1;
		  }
		}
	  }

	  if((line_bridgePCF[PCF1]+line_tunnelPCF[PCF1])>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==1){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck38 = 1;
		  }
		}
	  }
	  if((line_bridgePCF[PCF2]+line_tunnelPCF[PCF2])>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==2){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck39 = 1;
		  }
		}
	  }
	  if(line_bridgePCF[PCF3]>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==3){continue;}

		  if(i!=5)
		  {
			
		    if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
			{ 
			  docheck40a = 1;
			} 
		  }
		  else
		  {
			
		    if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
			{ 
			  docheck40a = 1;
			} 
		  }
		}
	  }
	  if(line_tunnelPCF[PCF3]>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==3){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck40b = 1;
		  }
		}
	  }
	  if((line_bridgePCF[PCF4]+line_tunnelPCF[PCF4])>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==4){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck41 = 1;
		  }
		}
	  }
	  if((line_bridgePCF[PCF5]+line_tunnelPCF[PCF5])>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==5){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck42 = 1;
		  }
		}
	  }
	  if((line_bridgePCF[PCF6]+line_tunnelPCF[PCF6])>0)
	  {
		
		for(i=0;i<numPCF;i++)
		{
		  if(i==6){continue;}
		  if(
			  (line_aqueductPCF[i]+line_canalPCF[i]+line_pipePCF[i]+
			  line_railPCF[i]+line_roadPCF[i]+line_ctrackPCF[i]+line_trailPCF[i]+
              line_sidetrackPCF[i]+line_riverPCF[i]+line_ditchPCF[i]+line_taxiwayPCF[i])>0)
		  {
			docheck43 = 1;
		  }
		}

	    if((line_cable_found>0)&&(line_overheadOHC999999>0))
		{ 
          docheck44 = 1;
		}

	  }

	  if((docheck37+docheck38+docheck39+docheck40a+docheck40b+docheck41+docheck42+docheck43)>0)
	  {
		sprintf(pcfname,"%s",MakeCleanCode(GetEACLabel(GetAttrIndex("PCF"))));
	  }




	}

	if((type==1)||(type==5)||(type==6)||(type==7)||(type==8)||(type==11))
	{
      

	  if(type==1)
	  {
        num_b2 = 0;
	  }
      if(
		  (ATTRTYPE_ANY_TDS() ==0) &&
		  (ATTRTYPE_DFDD()    ==0) &&
		  (ATTRTYPE_NFDD()    ==0) &&
          (ATTRTYPE_ANY_GGDM()==0)
		 )
	  {
	    num_b2 = 0;
	  }
	}




    if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
	{
      for(i=0;i<TDS_num_ISL;i++)
	  { 
        TDS_ISL_CODES[i] = SilentConvertClassCode(TDS_ISL[i]);
	    if(TDS_ISL_CODES[i]>=0)
		{ 
          TDS_ISL_CODES[i] = TDS_ISL_CODES[i] + 1;
		} 

        TDS_ISL_USE[i] = 0;
	  } 

	  for(i=0;i<TDS_num_ISL;i++)
	  { 
        if(TDS_ISL_CODES[i]<0)
		{ 
          
          continue;
		} 

	    if(linespresent[TDS_ISL_CODES[i]]==0)
		{ 
		  
          continue;
		} 

		if(!strcmp(GetECCLabel(TDS_ISL_CODES[i]),GetContourLineName(1)))
		{
		  

		  continue;
		}



	    for(j=0;j<TDS_num_ISL;j++)
		{ 
          if(TDS_ISL_CODES[j]<0)
		  { 
            continue;
		  }  

		  if(TDS_ISL_USE[i]==3)
		  { 
		    continue;
		  } 


		  

		  doit = 0;
		  if(ATTRTYPE_TDS4()==1)
		  {
			
		    if(TDS4_ISL_VALS[i][j]=='e')
			{
			  doit = 1;
			}
		  }
		  else 
		  {
		    if(TDS6_ISL_VALS[i][j]=='e')
			{
			   
			   doit = 1;
			}
		  }

		  if(doit==1)
		  { 
		    if(linespresent[TDS_ISL_CODES[j]]==1)
			{ 
              

			  if(TDS_ISL_USE[i]==0)
			  { 
			    TDS_ISL_USE[i] = 1;
                total_tds_isl_checks = total_tds_isl_checks + 1;
			  } 
			  if(TDS_ISL_USE[i]==2)
			  { 
			    TDS_ISL_USE[i] = 3;
                total_tds_isl_checks = total_tds_isl_checks + 1;
			  } 
			} 
		  } 


		  doit = 0;
		  if(ATTRTYPE_TDS4()==1)
		  {
			
		    if(TDS4_ISL_VALS[i][j]=='w')
			{
			  doit = 1;
			}
		  }
		  else 
		  {
		    if(TDS6_ISL_VALS[i][j]=='w')
			{
			   
			   doit = 1;
			}
		  }


		  if(doit==1)
		  { 
		    if(linespresent[TDS_ISL_CODES[j]]==1)
			{ 
              

			  if(TDS_ISL_USE[i]==0)
			  { 
			    TDS_ISL_USE[i] = 2;
                total_tds_isl_checks = total_tds_isl_checks + 1;
			  } 
			  if(TDS_ISL_USE[i]==1)
			  { 
			    TDS_ISL_USE[i] = 3;
                total_tds_isl_checks = total_tds_isl_checks + 1;
			  } 
			} 
		  } 
		} 
	  } 
	  
	}




    totalchecks = 
	 docheck1    + docheck2  + docheck3  + docheck4   + docheck5   + docheck6  +
	 docheck7    + docheck8  + docheck9  + docheck10  + docheck11  + docheck12 +
	 docheck13   + docheck14 + docheck15 + docheck16  + docheck17  + docheck18 +
	 docheck19   + docheck20 + docheck21 + docheck22  + docheck23  + docheck24 +
	 docheck25   + docheck26 + docheck27 + docheck28  + docheck29  + docheck30 +
	 docheck31   + docheck32 + docheck33 + docheck34  + docheck35  + docheck36 +
	 docheck37   + docheck38 + docheck39 + docheck40a + docheck40b + docheck41 + 
	 docheck42   + docheck43 + docheck44 + 
	 num_b1      + num_b2    + num_b5    + num_b6    + num_b7    +
	 lines_found + total_tds_isl_checks;


	if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
	{
      

	  

      num_b4 = 0;  
	  totalchecks = totalchecks - lines_found;
	}


      if(totalchecks>0)
	{
	  
	  fprintf(newfile,"UNIQUE_CHECKS 1\n");
	  fprintf(newfile,"SHARESEG %d\n",totalchecks);
	  
	  j = 1;
	  
	  if(docheck1==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);

	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road sharing segment with line water\"\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      j = j + 1;
	    }
	  
	  if(docheck2==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s sharing segment with line water\"\n",j,GetRailName(1));
		
	      j = j + 1;
	    }
	  
	  if(docheck3==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Administrative Boundary sharing segment with line Armistice Line\"\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  
	  if(docheck4==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);


		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with TUC:Pedestrian sharing segment with Road, Cart Track, or Trail\"\n",
			  j,GetBridgeName(1));

	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  
	  if(docheck5==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Ford sharing segment with line Cart Track or Trail\"\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  
	  
	  if(docheck6==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road without LOC=40 sharing segment with line Tunnel\"\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  j = j + 1;
	    }
	  
	  if(docheck7==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or %s without LOC=40 sharing segment with line Tunnel\"\n",
			  j,GetRailName(1),GetSideTrackName(1));
		  }
		  else
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s without LOC=40 sharing segment with line Tunnel\"\n",
			  j,GetRailName(1));
		  }
		
	      j = j + 1;
	    }
	  
	  if(docheck8==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road without LOC=25 sharing segment with line %s\"\n",j,GetBridgeName(1));
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  j = j + 1;
	    }
	  
	  if(docheck9==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
 
		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s without LOC=25 sharing segment with line %s\"\n",
			  j,GetRailName(1),GetBridgeName(1));

	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  j = j + 1;
	    }
	  
	  if(docheck10==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      
		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road without RST=1 sharing segment with line %s\"\n",j,GetBridgeName(1));
	
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  j = j + 1;
	    }
	  
	  if(docheck11==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road without WTC=1 sharing segment with line %s\"\n",j,GetBridgeName(1));
		
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  
	  if(docheck12==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		  if(
			  (ATTRTYPE_UFD2())||
			  (ATTRTYPE_UFD1())||
			  (ATTRTYPE_UFD3())
			)
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track sharing segment with line %s\"\n",j,GetBridgeName(1));
		  }
		  else
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track without LOC=25 sharing segment with line %s\"\n",j,GetBridgeName(1));
		  }

	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  if(docheck13==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		  
		  if(
			  (ATTRTYPE_UFD2())||
			  (ATTRTYPE_UFD1())||
			  (ATTRTYPE_UFD3())
			)
		  {
            fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail sharing segment with line %s\"\n",j,GetBridgeName(1));
		  }
		  else
		  {
            fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail without LOC=25 sharing segment with line %s\"\n",j,GetBridgeName(1));
		  }
		  
		  

		  fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  if(docheck14==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);


		  if(
			  (ATTRTYPE_UFD2())||
			  (ATTRTYPE_UFD1())||
			  (ATTRTYPE_UFD3())
			)
		  {
	        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track sharing segment with Line Tunnel\"\n",j);
		  }
		  else
		  {
	        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track without LOC=40 sharing segment with Line Tunnel\"\n",j);
		  }

	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  if(docheck15==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		  if(
			  (ATTRTYPE_UFD2())||
			  (ATTRTYPE_UFD1())||
			  (ATTRTYPE_UFD3())
			)
		  {
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail sharing segment with Line Tunnel\"\n",j);
		  }
		  else
		  {
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail without LOC=40 sharing segment with Line Tunnel\"\n",j);
		  }

	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }

	  if(docheck16==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track sharing segment with Line %s\"\n",j,GetBridgeName(1));
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }

	  if(docheck17==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, Aqueduct, Canal, %s, %s, %s or %s without RLE = \"Not Applicable\" sharing segment with line Tunnel\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1));


	      j = j + 1;
	    }


	  if(docheck18==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, Aqueduct, Canal, %s, %s, %s or %s without CWT = \"True\" sharing segment with line Tunnel\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }

	  if(docheck19==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);


          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, Aqueduct, Canal, %s, %s, %s or %s without SBB = \"False\" sharing segment with line Tunnel\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }



	  if(docheck20==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, %s, %s, %s or %s without LOC = \"On Surface\" sharing segment with line %s\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetBridgeName(1));

	      j = j + 1;
	    }

	  if(docheck21==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, Aqueduct, Canal, %s, %s, %s or %s without RLE = \"Raised\" sharing segment with line %s\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetBridgeName(1));

	      j = j + 1;
	    }

	  if(docheck22==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, Aqueduct, Canal, %s, %s, %s or %s without CWT = \"False\" sharing segment with line %s\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetBridgeName(1));

	      j = j + 1;
	    }

	  if(docheck23==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, Aqueduct, Canal, %s, %s, %s or %s without SBB = \"True\" sharing segment with line %s\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetBridgeName(1));

	      j = j + 1;
	    }


	  if(docheck24==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);


          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cut with CET \"Two Sides\" or \"Many Sides\" coincident with line Trail, Cart Track, Aqueduct, Canal, %s, %s, %s or %s without RLE = \"Depressed\"\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }




	  if(docheck26==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s, Cart Track, or Trail without LOC = \"On Waterbody Bottom\" sharing segment with line Ford\"\n",
			  j,GetRoadName(1));

	      j = j + 1;
	    }


        if(docheck27==1)
	    {
		  

	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track, Trail, %s, %s, %s or %s without LOC = \"Below Ground Surface\" sharing segment with line Tunnel\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }


	  if(docheck28==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road, %s, %s, %s, Aqueduct, Canal, or %s without LOC=25 sharing segment with line %s\"\n",
			  j,GetRailName(1),GetSideTrackName(1),GetPipelineName(1),GetInterchangeName(1),GetBridgeName(1));

	      j = j + 1;
	    }
	  

	  if(docheck29==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road, %s, %s, %s, Aqueduct, Canal, or %s without LOC=40 sharing segment with line Tunnel\"\n",
			  j,GetRailName(1),GetSideTrackName(1),GetPipelineName(1),GetInterchangeName(1));

	      j = j + 1;
	    }
	  
	  if(docheck30==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail, Cart Track, Aqueduct, Canal, %s, %s, %s or %s without RLE = \"Raised\" sharing segment with line %s or Causeway Structure\"\n",
			  j,GetPipelineName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetEmbankmentName(1));

	      j = j + 1;
	    }


	  if(docheck31==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail, Cart Track, %s, %s or %s without SBB = \"False\" sharing segment with line %s or Causeway Structure\"\n",
			  j,GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetEmbankmentName(1));

	      j = j + 1;
	    }

	  if(docheck32==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail, Cart Track, %s, %s or %s without CWT = \"False\" sharing segment with line %s or Causeway Structure\"\n",
			  j,GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetEmbankmentName(1));

	      j = j + 1;
	    }

	  if(docheck33==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

          fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail, Cart Track, %s, %s or %s without LOC = \"On Surface\" sharing segment with line %s or Causeway Structure\"\n",
			  j,GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetEmbankmentName(1));

	      j = j + 1;
	    }



	  if(docheck34==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s, Cart Track, or Trail without SBB = \"False\" sharing segment with line Ford\"\n",
			  j,GetRoadName(1));

	      j = j + 1;
	    }

	  if(docheck35==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s, Cart Track, or Trail without CWT = \"False\" sharing segment with line Ford\"\n",
			  j,GetRoadName(1));

	      j = j + 1;
	    }

	  if(docheck36==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s, Cart Track, or Trail without RLE = \"Level\" sharing segment with line Ford\"\n",
			  j,GetRoadName(1));

	      j = j + 1;
	    }

	  if(docheck37==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,GetBridgeName(1),PCF999999NAME);

	      j = j + 1;
	    }

	  if(docheck38==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,GetBridgeName(1),PCF1NAME);

	      j = j + 1;
	    }
	  if(docheck39==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,GetBridgeName(1),PCF2NAME);

	      j = j + 1;
	    }

	  if(docheck40a==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with PCF = \"%s\" coincident with carried line feature with different PCF (except %s or %s with PCF = \"Dismantled\")\"\n",
			  j,GetBridgeName(1),PCF3NAME,GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }
	  if(docheck40b==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,PCF3NAME);

	      j = j + 1;
	    }


	  if(docheck41==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,GetBridgeName(1),PCF4NAME);

	      j = j + 1;
	    }
	  if(docheck42==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,GetBridgeName(1),PCF5NAME);

	      j = j + 1;
	    }
	  if(docheck43==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Tunnel with PCF = \"%s\" coincident with carried line feature with different PCF\"\n",
			  j,GetBridgeName(1),PCF6NAME);

	      j = j + 1;
	    }

	  if(docheck44==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with OHC=-999999 within .1m of line Cable\"\n",j,GetOverheadName(1)); 

	      j = j + 1;
	    }

	  for(k=0;k<num_b1;k++)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (WID=%.3lf) overlapping line Road with larger %s\"\n",
			  j,GetBridgeName(1),global_bridge_wid[k],GetWD1Name());
	      j = j + 1;
	    }

	  for(k=0;k<num_b2;k++)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (WID=%.3lf) overlapping line Cart Track with larger %s\"\n",
			  j,GetBridgeName(1),global_bridge_wid[k],GetWD1Name());
	      j = j + 1;
	  }




	  if( (num_b4>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {
         

	    if(bridge_wd1_val_found < road_wd1_val_found)
		{
          for(k=0;k<bridge_wd1_val_found;k++)
		  {
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (%s=%.3lf) overlapping line Road with different %s\"\n",
			    j,GetBridgeName(1),GetWD1Name(),global_bridge_wd1[k],GetWD1Name2());
	        j = j + 1;
		  }
		}
	    else
		{
          for(k=0;k<road_wd1_val_found;k++)
		  {
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road (%s=%.3lf) overlapping line %s with different %s\"\n",
			    j,GetWD1Name(),global_road_wd1[k],GetBridgeName(1),GetWD1Name2());
	        j = j + 1;
		  }
		}
	  }




	  if( (num_b5>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {

        

		if(b5_bridgefirst>0)
		{
		  

          for(i=0;i<bridge_hgt_val_found;i++)
		  {
	        if(span_hgt_val_found>0)
			{
	          if(global_span_hgt[span_hgt_val_found-1]>global_bridge_hgt[i])
			  { 
			    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (HGT=%.3lf) overlapping line Bridge Span with larger HGT\"\n",
			    j,GetBridgeName(1),global_bridge_hgt[i]);
	            j = j + 1;
			  } 
			} 
		  } 
		} 
		else
		{
		  

          for(i=0;i<span_hgt_val_found;i++)
		  {
	        if(bridge_hgt_val_found>0)
			{ 
	          if(global_span_hgt[i]>global_bridge_hgt[0])
			  { 
			    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Bridge Span (HGT=%.3lf) overlapping line %s with larger HGT\"\n",
			    j,global_span_hgt[i],GetBridgeName(1));
	            j = j + 1;
			  } 
			} 
		  } 
		}
	  }



	  if( (num_b6>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {

        

		if(b6_bridgefirst>0)
		{
		  

          for(i=0;i<bridge_zvh_val_found;i++)
		  {
	        if(span_zvh_val_found>0)
			{
	          if(global_span_zvh[span_zvh_val_found-1]>global_bridge_zvh[i])
			  {
			    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (%s=%.3lf) overlapping line Bridge Span with larger ZVH\"\n",
			    j,GetBridgeName(1),GetZVCode2(),global_bridge_zvh[i]);
	            j = j + 1;
			  } 
			}  
		  } 
		}
		else
		{
		  

          for(i=0;i<span_zvh_val_found;i++)
		  {
	        if(bridge_zvh_val_found>0)
			{ 
	          if(global_span_zvh[i]>global_bridge_zvh[0])
			  { 
			    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Bridge Span (ZVH=%.3lf) overlapping line %s with larger ZVH\"\n",
			    j,global_span_zvh[i],GetBridgeName(1));
	            j = j + 1;
			  } 
			} 
		  }
		}
	  }




	  if( (num_b7>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {

        

		if(b7_bridgefirst>0)
		{
		  

          for(i=0;i<bridge_len_val_found;i++)
		  { 
	        if(span_len_val_found>0)
			{ 
	          if(global_span_len[span_len_val_found-1]>global_bridge_len[i])
			  { 
			    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (%s=%.3lf) overlapping line Bridge Span with larger %s\"\n",
			    j,GetBridgeName(1),GetLengthCode(),global_bridge_len[i],GetLengthCode());
	            j = j + 1;
			  } 
			} 
		  } 
		} 
		else
		{
		  

          for(i=0;i<span_len_val_found;i++)
		  {
	        if(bridge_len_val_found>0)
			{ 
	          if(global_span_len[i]>global_bridge_len[0])
			  { 
			    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Bridge Span (%s=%.3lf) overlapping line %s with larger %s\"\n",
			    j,GetLengthCode(),global_span_len[i],GetBridgeName(1),GetLengthCode());
	            j = j + 1;
			  } 
			} 
		  }
		}
	  }


      

      if(total_tds_isl_checks>0)
	  {
        for(i=0;i<TDS_num_ISL;i++)
		{  
          if(TDS_ISL_CODES[i]<0)
		  { 
            continue;
		  }  

          if((TDS_ISL_USE[i]==1)||(TDS_ISL_USE[i]==3))
		  { 
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	        fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Illegal Shared Curve error: %s\"\n",j,GetECCLabel(TDS_ISL_CODES[i]));
	        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    j = j + 1;
		  } 

          if((TDS_ISL_USE[i]==2)||(TDS_ISL_USE[i]==3))
		  { 
            fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"SHARESEG %d ANNOTATION \"Illegal Shared Curve warning: %s\"\n",j,GetECCLabel(TDS_ISL_CODES[i]));
            fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
            fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    j = j + 1;
		  } 
		} 
	  }



      


	  if((type!=5)&&(type!=6)&&(type!=7)&&(type!=8)&&(type!=11))
	  {
	    jtemp = j;
	  
	    for(k=0;k<lines_found;k++)
		{
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  
	  
	    j = jtemp;
	  
	    for(i=0;i<INscc_loop;i++)
	    {
	      if(linespresent[i]==1)
		  {
		    sprintf(tempchar,"%s",GetECCLabel(i));
		  
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s sharing segments with line %s\"\n",
			  j,tempchar,tempchar);
		    j = j + 1;
		  } 
		} 
	  }
	  
	  
	  fprintf(newfile,"ATTRATTRATTR\n");
	  
	  
	  j = 1;
	  
	  
	  if(docheck1==1)
	    {

		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));

	      
	      for(i=0;i<INscc_loop;i++)
		{
		  if((codespresent[i]==1)&&(line_water[i]==1))
		    {
		      sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(i)));
		      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,cleancode);
		    }
		}
	      j = j + 1;
	    }
	  
	  if(docheck2==1)
	    {
	      
		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		
	      
	      for(i=0;i<INscc_loop;i++)
		{
		  if((codespresent[i]==1)&&(line_water[i]==1))
		    {
		      sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(i)));
		      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,cleancode);
		    }
		}
	      j = j + 1;
	    }
	  
	  
	  if(docheck3==1)
	    {
	      fprintf(newfile,"SHARESEG %d PRIMARY AdministrativeBoundary ANY on C_LINE\n",j);
	      fprintf(newfile,"SHARESEG %d SECONDARY ArmisticeLine ANY on C_LINE\n",j);
	      j = j + 1;
	    }
	  
	  
	  if(docheck4==1)
	    {
	      
	      
	      if(
			  (ATTRTYPE_MGCP3())||(ATTRTYPE_MGCP4()) ||
			  (ATTRTYPE_NFDD())||
			  (ATTRTYPE_DFDD())
			  )
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Pedestrian on C_LINE\n",j,GetBridgeName(0));
		}
	      else
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationUseCategory LABEL Pedestrian on C_LINE\n",j,GetBridgeName(0));
		}
	      
	      
	      if(line_road_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));
		}
	      
	      if(line_ctrack_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);
		}
	      if(line_trail_found==1)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);
		}
	      
	      
	      j = j + 1;
	    }
	  
	  
	  if(docheck5==1)
	    {
	      fprintf(newfile,"SHARESEG %d PRIMARY Ford ANY on C_LINE\n",j);
	      
	      if(line_ctrack_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);
		}
	      if(line_trail_found==1)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);
		}
	      j = j + 1;
	    }
	  
	  
	  if(docheck6==1)
	    {

		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));

	      
	      if(line_loc40road>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRoadName(0),GetLOCName(0),GetUnderGroundName(0));
		}
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   
	      
	      j = j + 1;
	    }
	  
	  
	  
	  if(docheck7==1)
	    {

		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));


	      if(line_loc40rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRailName(0),GetLOCName(0),GetUnderGroundName(0));
		  }


		  if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
		  {
			if(line_sidetrack_found>0)
			{
		      fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
			}


	        if(line_loc40sidetrack>0)
			{ 
              fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetSideTrackName(0),GetLOCName(0),GetUnderGroundName(0));
			} 
		  }


	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   
	      
	      j = j + 1;
	    }
	  
	  
	  
	  
	  
	  
	  if(docheck8==1)
	    {
	      
	      
		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
	      
	      if(line_loc25road>0)
		{
	      fprintf(newfile,"SHARESEG %d PRIMARY %s LocationCategory LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetRoadName(0));
		}
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
	      
	      j = j + 1;
	    }
	  
	  
	  
	  if(docheck9==1)
	    {
	      
	      

		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));

	      if(line_loc25rail>0)
		{
		  if((ATTRTYPE_VMAP()))
		    {
		      fprintf(newfile,"SHARESEG %d PRIMARY RRTrack LocationCategory LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j);
		    }
		  else if(
		          (ATTRTYPE_MGCP3())||(ATTRTYPE_MGCP4()) ||
                  (ATTRTYPE_ANY_TDS()) ||
		          (ATTRTYPE_NFDD())||
		          (ATTRTYPE_DFDD())||
		          (ATTRTYPE_ANY_GGDM())
			  )
		    {
		      
		    }
		  else
		    {
		      fprintf(newfile,"SHARESEG %d PRIMARY Railroad LocationCategory LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j);
		    }
		}
	      
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
	      
	      j = j + 1;
	    }
	  
	  
	  
	  if(docheck10==1)
	    {
	      
	      
		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
	      
	      if(line_rst1road>0)
		  { 
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL HardPaved off C_LINE\n",j,GetRoadName(0),GetRSTName());
		  } 


		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		
	      
	      j = j + 1;
	    }
	  
	  
	  if(docheck11==1)
	    {
	      
	      
		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
	      
	      
	      if(line_wtc1road>0)
		{
		  if(
			  (ATTRTYPE_MGCP3())||(ATTRTYPE_MGCP4()) ||
			  (ATTRTYPE_NFDD())||
			  (ATTRTYPE_DFDD())
			  )
		    {
		      fprintf(newfile,"SHARESEG %d PRIMARY %s RoadWeatherRestriction LABEL Allweather off C_LINE\n",j,GetRoadName(0));
		    }
		  else
		    {
		      fprintf(newfile,"SHARESEG %d PRIMARY %s WeatherTypeCategory LABEL AllWeather off C_LINE\n",j,GetRoadName(0));
		    }
		}

		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
	      
	      j = j + 1;
	    }
	  
	  
	  
	  
	  if(docheck12==1)
	    { 
	      
	      
	      fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
	      
	      if(line_loc25ctrack>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY CartTrack LocationCategory LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j);
		}
	      
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
	      
	      j = j + 1;
	    }
	  
	  if(docheck13==1)
	    {
	      
	      
	      fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
	      
	      if(line_loc25trail>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Trail LocationCategory LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j);
		}
	      
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
	      
	      j = j + 1;
	    }
	  
	  
	  if(docheck14==1)
	    {
	      fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
	      
	      if(line_loc40ctrack>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s LABEL %s off C_LINE\n",j,GetLOCName(0),GetUnderGroundName(0));
		}
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   
	      
	      j = j + 1;
	    }
	  
	  if(docheck15==1)
	    {
	      fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
	      
	      if(line_loc40trail>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY Trail %s LABEL %s off C_LINE\n",j,GetLOCName(0),GetUnderGroundName(0));
		}
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   
	      
	      j = j + 1;
	    }
	  


	  	  if(docheck16==1)
	    { 
	      fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
	      
	      j = j + 1;
	    }




        if(docheck17==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  }
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }


	      if(line_rle998trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail RelativeLevel LABEL NotApplicable off C_LINE\n",j);
		  } 
          if(line_rle998ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack RelativeLevel LABEL NotApplicable off C_LINE\n",j);
		  } 
	      if(line_rle998aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct RelativeLevel LABEL NotApplicable off C_LINE\n",j);
		  }
	      if(line_rle998canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal RelativeLevel LABEL NotApplicable off C_LINE\n",j);
		  }
	      if(line_rle998pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL NotApplicable off C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_rle998road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL NotApplicable off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_rle998rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL NotApplicable off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_rle998sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL NotApplicable off C_LINE\n",j,GetSideTrackName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   

	      j = j + 1;
	    }

       if(docheck18==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }


          if(line_cwt1001trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ContainedWithinTunnel LABEL True off C_LINE\n",j);
		  } 
          if(line_cwt1001ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ContainedWithinTunnel LABEL True off C_LINE\n",j);
		  }
	      if(line_cwt1001aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ContainedWithinTunnel LABEL True off C_LINE\n",j);
		  }
	      if(line_cwt1001canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal ContainedWithinTunnel LABEL True off C_LINE\n",j);
		  }
	      if(line_cwt1001pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL True off C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_cwt1001road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL True off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_cwt1001rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL True off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_cwt1001sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL True off C_LINE\n",j,GetSideTrackName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   

	      j = j + 1;
	    }

       if(docheck19==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }




          if(line_sbb1000trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  } 
          if(line_sbb1000ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  } 
	      if(line_sbb1000aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  }
	      if(line_sbb1000canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  }
	      if(line_sbb1000pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_sbb1000road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_sbb1000rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_sbb1000sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetSideTrackName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);	   

	      j = j + 1;
	    }



       if(docheck20==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }



          if(line_loc44trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0)); 
		  }
          if(line_loc44ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0)); 
		  }
	      if(line_loc44pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetPipelineName(0),GetLOCName(0));
		  }
	      if(line_loc44road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
	      if(line_loc44rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
	      if(line_loc44sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetSideTrackName(0),GetLOCName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

	      j = j + 1;
	    }


      if(docheck21==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }



          if(line_rle1trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail RelativeLevel LABEL Raised off C_LINE\n",j);
		  } 
          if(line_rle1ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack RelativeLevel LABEL Raised off C_LINE\n",j);
		  } 
		  if(line_rle1aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct RelativeLevel LABEL Raised off C_LINE\n",j);
		  }
	      if(line_rle1canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal RelativeLevel LABEL Raised off C_LINE\n",j);
		  }
	      if(line_rle1pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_rle1road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_rle1rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_rle1sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetSideTrackName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

	      j = j + 1;
	    }

     if(docheck22==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }



          if(line_cwt1000trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  } 
          if(line_cwt1000ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  } 
		  if(line_cwt1000aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  }
	      if(line_cwt1000canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  }
	      if(line_cwt1000pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetPipelineName(0));
		  }		  
		  if(line_cwt1000road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_cwt1000rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_cwt1000sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetSideTrackName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

	      j = j + 1;
	    }


	     if(docheck23==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }		  
		  if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }



          if(line_sbb1001trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail SupportedByBridgeSpan LABEL True off C_LINE\n",j);
		  } 
          if(line_sbb1001ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack SupportedByBridgeSpan LABEL True off C_LINE\n",j);
		  } 
		  if(line_sbb1001aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct SupportedByBridgeSpan LABEL True off C_LINE\n",j);
		  }
	      if(line_sbb1001canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal SupportedByBridgeSpan LABEL True off C_LINE\n",j);
		  }
	      if(line_sbb1001pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL True off C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_sbb1001road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL True off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_sbb1001rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL True off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_sbb1001sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL True off C_LINE\n",j,GetSideTrackName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

	      j = j + 1;
	    }



	     if(docheck24==1)
	    {

          if(line_cet2cut_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Cut %s LABEL TwoSides on C_LINE\n",j,GetCETName(0));
		  }

		  if(line_cet3cut_found>0)
		  {
           fprintf(newfile,"SHARESEG %d PRIMARY Cut %s LABEL ManySides on C_LINE\n",j,GetCETName(0));
		  }

		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
          if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);
		  } 
          if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);
		  } 


		  if(line_rle3ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY CartTrack RelativeLevel LABEL Depressed off C_LINE\n",j);
		  }
		  if(line_rle3trail>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Trail RelativeLevel LABEL Depressed off C_LINE\n",j);
		  }
		  if(line_rle3aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct RelativeLevel LABEL Depressed off C_LINE\n",j);
		  }
	      if(line_rle3canal>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Canal RelativeLevel LABEL Depressed off C_LINE\n",j);
		  }
	      if(line_rle3pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s RelativeLevel LABEL Depressed off C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_rle3road>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s RelativeLevel LABEL Depressed off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_rle3rail>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s RelativeLevel LABEL Depressed off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_rle3sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s RelativeLevel LABEL Depressed off C_LINE\n",j,GetSideTrackName(0));
		  }

	      j = j + 1;
	    }



      if(docheck26==1)
	    {
		  if(line_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_ctrack_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  }
		  if(line_trail_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }

	      if(line_loc17road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnWaterbodyBottom off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
	      if(line_loc17cart>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s LABEL OnWaterbodyBottom off C_LINE\n",j,GetLOCName(0));
		  }
	      if(line_loc17trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail %s LABEL OnWaterbodyBottom off C_LINE\n",j,GetLOCName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Ford ANY on C_LINE\n",j);

	      j = j + 1;
	    }



       if(docheck27==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }



          if(line_loc40trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail %s LABEL %s off C_LINE\n",j,GetLOCName(0),GetUnderGroundName(0));
		  } 
          if(line_loc40ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s LABEL %s off C_LINE\n",j,GetLOCName(0),GetUnderGroundName(0));
		  } 
	      if(line_loc40pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetPipelineName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRoadName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRailName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetSideTrackName(0),GetLOCName(0),GetUnderGroundName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);

	      j = j + 1;
	    }


	  
	  

       if(docheck28==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_interchange_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetInterchangeName(0));
		  }



	      if(line_loc25road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
	      if(line_loc25rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
	      if(line_loc25sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetSideTrackName(0),GetLOCName(0));
		  }
	      if(line_loc25canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetLOCName(0));
		  }
	      if(line_loc25pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetPipelineName(0),GetLOCName(0));
		  }
	      if(line_loc25aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetLOCName(0));
		  }
	      if(line_loc25interchange>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL SuspendedorElevatedAboveGroundorWaterSurface off C_LINE\n",j,GetInterchangeName(0),GetLOCName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

	      j = j + 1;
	    }



       if(docheck29==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_interchange_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetInterchangeName(0));
		  }



	      if(line_loc40road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRoadName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRailName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetSideTrackName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal %s LABEL %s off C_LINE\n",j,GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetPipelineName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct %s LABEL %s off C_LINE\n",j,GetLOCName(0),GetUnderGroundName(0));
		  }
	      if(line_loc40interchange>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetInterchangeName(0),GetLOCName(0),GetUnderGroundName(0));
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);

	      j = j + 1;
	    }



       if(docheck30==1)
	    {

		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_aqueduct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct ANY on C_LINE\n",j);
		  }
		  if(line_canal_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }
          if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
          if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  } 


		  if(line_rle1ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack RelativeLevel LABEL Raised off C_LINE\n",j);
		  }
		  if(line_rle1trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail RelativeLevel LABEL Raised off C_LINE\n",j);
		  }
		  if(line_rle1aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Aqueduct RelativeLevel LABEL Raised off C_LINE\n",j);
		  }
	      if(line_rle1canal>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Canal RelativeLevel LABEL Raised off C_LINE\n",j);
		  }
	      if(line_rle1pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetPipelineName(0));
		  }
	      if(line_rle1road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_rle1rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_rle1sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Raised off C_LINE\n",j,GetSideTrackName(0));
		  }

		  if(line_embankment_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  }
		  if(line_causeway_struct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY CausewayStructure ANY on C_LINE\n",j);
		  }

	      j = j + 1;
	    }



       if(docheck31==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
          if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
          if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  } 


          if(line_sbb1000ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  } 
	      if(line_sbb1000road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_sbb1000rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_sbb1000sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetSideTrackName(0));
		  }
	      if(line_sbb1000trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  }


		  if(line_embankment_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  }
		  if(line_causeway_struct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY CausewayStructure ANY on C_LINE\n",j);
		  }

	      j = j + 1;
	    }


       if(docheck32==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
          if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
          if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  } 


          if(line_cwt1000ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  } 
		  if(line_cwt1000road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetRoadName(0));
		  }
	      if(line_cwt1000rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetRailName(0));
		  }
	      if(line_cwt1000sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetSideTrackName(0));
		  }
	      if(line_cwt1000trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  }

		  if(line_embankment_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  }
		  if(line_causeway_struct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY CausewayStructure ANY on C_LINE\n",j);
		  }

	      j = j + 1;
	    }


       if(docheck33==1)
	    {
		  if(line_road_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
          if(line_ctrack_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  } 
          if(line_trail_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  } 


          if(line_loc44ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0));
		  } 
	      if(line_loc44road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
	      if(line_loc44rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
	      if(line_loc44sidetrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,GetSideTrackName(0),GetLOCName(0));
		  }
	      if(line_loc44trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0));
		  }

		  if(line_embankment_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  }
		  if(line_causeway_struct_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY CausewayStructure ANY on C_LINE\n",j);
		  }

	      j = j + 1;
	    }



      if(docheck34==1)
	    {
		  if(line_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_ctrack_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  }
		  if(line_trail_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }


	      if(line_sbb1000road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s SupportedByBridgeSpan LABEL False off C_LINE\n",j,GetRoadName(0));
		  }
          if(line_sbb1000ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  } 
          if(line_sbb1000trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail SupportedByBridgeSpan LABEL False off C_LINE\n",j);
		  } 

	      fprintf(newfile,"SHARESEG %d SECONDARY Ford ANY on C_LINE\n",j);

	      j = j + 1;
	    }



      if(docheck35==1)
	    {
		  if(line_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_ctrack_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  }
		  if(line_trail_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }

		  if(line_cwt1000road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ContainedWithinTunnel LABEL False off C_LINE\n",j,GetRoadName(0));
		  }
          if(line_cwt1000ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  } 
          if(line_cwt1000trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ContainedWithinTunnel LABEL False off C_LINE\n",j);
		  } 

	      fprintf(newfile,"SHARESEG %d SECONDARY Ford ANY on C_LINE\n",j);

	      j = j + 1;
	    }


      if(docheck36==1)
	    {
		  if(line_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_ctrack_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);
		  }
		  if(line_trail_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);
		  }

		  if(line_rle2road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s RelativeLevel LABEL Level off C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rle2ctrack>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack RelativeLevel LABEL Level off C_LINE\n",j);
		  }
		  if(line_rle2trail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Trail RelativeLevel LABEL Level off C_LINE\n",j);
		  }

	      fprintf(newfile,"SHARESEG %d SECONDARY Ford ANY on C_LINE\n",j);

	      j = j + 1;
	    }


      if(docheck37==1)
	  {
	    if(line_bridgePCF[PCF999999]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF999999NAME));
		}
	    if(line_tunnelPCF[PCF999999]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF999999NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==0) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }

      if(docheck38==1)
	  {
	    if(line_bridgePCF[PCF1]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF1NAME));
		}
	    if(line_tunnelPCF[PCF1]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF1NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==1) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }

      if(docheck39==1)
	  {
	    if(line_bridgePCF[PCF2]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF2NAME));
		}
	    if(line_tunnelPCF[PCF2]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF2NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==2) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }

      if(docheck40a==1)
	  {
	    if(line_bridgePCF[PCF3]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF3NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==3) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }
		  

		  if(i!=5)
		  {
		    if(line_railPCF[i]>0)
			{  
		      fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
			}  
            if(line_sidetrackPCF[i]>0)
			{  
		      fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
			}  
		  }


		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }



      if(docheck40b==1)
	  {
	    if(line_tunnelPCF[PCF3]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF3NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==3) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }




      if(docheck41==1)
	  {
	    if(line_bridgePCF[PCF4]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF4NAME));
		}
	    if(line_tunnelPCF[PCF4]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF4NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==4) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }

      if(docheck42==1)
	  {
	    if(line_bridgePCF[PCF5]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF5NAME));
		}
	    if(line_tunnelPCF[PCF5]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF5NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==5) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }

      if(docheck43==1)
	  {
	    if(line_bridgePCF[PCF6]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,GetBridgeName(0),pcfname,MakeCleanCode(PCF6NAME));
		}
	    if(line_tunnelPCF[PCF6]>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Tunnel %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(PCF6NAME));
		}
			 
		for(i=0;i<numPCF;i++)
		{
		  if(i==6) {continue;}

		  if(line_aqueductPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_canalPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_pipePCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetPipelineName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  }	 
		  if(line_railPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRailName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_roadPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRoadName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ctrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_trailPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Trail %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
          if(line_sidetrackPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetSideTrackName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_riverPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,GetRiverName(0),pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_ditchPCF[i]>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Ditch %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		  if(line_taxiwayPCF[i])
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Taxiway %s LABEL %s on C_LINE\n",j,pcfname,MakeCleanCode(GetPCFName(i)));
		  } 
		}

	    j = j + 1;
	  }


	  if(docheck44==1)
	  { 
	    if(line_overheadOHC999999>0)
		{ 
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE -999999 -999999 on C_LINE\n",j,
			   GetOverheadName(0),
		       MakeCleanCode2(GetEACLabel(GetAttrIndex("OHC"))));
		}  

	    if(line_cable_found>0)
		{
          fprintf(newfile,"SHARESEG %d SECONDARY Cable ANY on C_LINE\n",j);
		} 

	    j = j + 1;
	  }



	  if((num_b1+num_b2)>0)
	  {
	    for(i=0;i<bridge_wid_val_found;i++)
		{
		  if(road_wd1_val_found<=0)
		  {
             break;
		  }

	      if(WD1IsLarger(global_road_wd1[road_wd1_val_found-1],global_bridge_wid[i]))
		  {
			

			for(k=0;k<road_wd1_val_found;k++)
			{
			  if(WD1IsLarger(global_road_wd1[k],global_bridge_wid[i]))
			  {
				break;
			  }
			}

			if(k>=road_wd1_val_found)
			{
			   
			   printf("error: k %d/%d for road\n",k,road_wd1_val_found);
			   k = road_wd1_val_found - 1;
			}

			fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AQ040"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex("WID"))),
			  global_bridge_wid[i],global_bridge_wid[i]);


			fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AP030"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			  global_road_wd1[k],global_road_wd1[road_wd1_val_found-1]);

	        j = j + 1;
		  }
		}


		

		if(num_b2>0)
		{
	    for(i=0;i<bridge_wid_val_found;i++)
		{
		  if(cart_wd1_val_found<=0)
		  {
            break;
		  }

	      if(WD1IsLarger(global_cart_wd1[cart_wd1_val_found-1],global_bridge_wid[i]))
		  {
			

			for(k=0;k<cart_wd1_val_found;k++)
			{
			  if(WD1IsLarger(global_cart_wd1[k],global_bridge_wid[i]))
			  {
				break;
			  }
			}

			if(k>=cart_wd1_val_found)
			{
			   
			   printf("error: k %d/%d for road\n",k,cart_wd1_val_found);
			   k = cart_wd1_val_found - 1;
			}


			fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AQ040"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex("WID"))),
			  global_bridge_wid[i],global_bridge_wid[i]);


			fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AP010"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			  global_cart_wd1[k],global_cart_wd1[cart_wd1_val_found-1]);

	        j = j + 1;
		  }
		}
	  }
	  }



	  if((num_b4>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11))  )
	  {
	    

        if(bridge_wd1_val_found < road_wd1_val_found)
		{
          for(k=0;k<bridge_wd1_val_found;k++)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			   j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))),
			   global_bridge_wd1[k],global_bridge_wd1[k]);

            for(l=0;l<road_wd1_val_found;l++)
			{  
			  if(global_road_wd1[l]!=global_bridge_wd1[k])
			  {
		         fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			       j,GetRoadName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			       global_road_wd1[l],global_road_wd1[l]);
			  } 
			} 
	        j = j + 1;
		  } 
		} 
	    else
		{ 
          for(k=0;k<road_wd1_val_found;k++)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			    j,GetRoadName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			    global_road_wd1[k],global_road_wd1[k]);

            for(l=0;l<bridge_wd1_val_found;l++)
			{
			  if(global_road_wd1[k]!=global_bridge_wd1[l])
			  {
		         fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			        j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))),
			        global_bridge_wd1[l],global_bridge_wd1[l]);
			  }
			}
	        j = j + 1;
		  }
		} 
	  } 



  	  if( (num_b5>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {
        


        if(b5_bridgefirst>0)
		{
		  

		  savej = j;

          for(i=0;i<bridge_hgt_val_found;i++)
		  { 
	        if(span_hgt_val_found>0)
			{ 
	          if(global_span_hgt[span_hgt_val_found-1]>global_bridge_hgt[i])
			  { 
		        fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			      j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
			      global_bridge_hgt[i],global_bridge_hgt[i]);

			    j = j + 1;
			  }   
			}   
		  }   

          j = savej;

          for(i=0;i<bridge_hgt_val_found;i++)
		  { 
	        if(span_hgt_val_found>0)
			{ 
	          if(global_span_hgt[span_hgt_val_found-1]>global_bridge_hgt[i])
			  { 
			    for(l=0;l<span_hgt_val_found;l++)
				{   
			      if(global_span_hgt[l]>global_bridge_hgt[i])
				  {   
		             fprintf(newfile,"SHARESEG %d SECONDARY BridgeSpan %s RANGE %lf %lf on C_LINE\n",
			          j,MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
			          global_span_hgt[l],global_span_hgt[l]);
				  }   
				}  
			    j = j + 1;
			  }   
			}   
		  }   
		} 
		else
		{

		  

		  savej = j;

          for(i=0;i<span_hgt_val_found;i++)
		  { 
	        if(bridge_hgt_val_found>0)
			{ 
	          if(global_span_hgt[i]>global_bridge_hgt[0])
			  { 
		        fprintf(newfile,"SHARESEG %d PRIMARY BridgeSpan %s RANGE %lf %lf on C_LINE\n",
			      j,MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
			      global_span_hgt[i],global_span_hgt[i]);

			    j = j + 1;
			  }   
			}   
		  }   

          j = savej;

          for(i=0;i<span_hgt_val_found;i++)
		  { 
	        if(bridge_hgt_val_found>0)
			{ 
	          if(global_span_hgt[i]>global_bridge_hgt[0])
			  { 
			    for(l=0;l<bridge_hgt_val_found;l++)
				{   
			      if(global_span_hgt[i]>global_bridge_hgt[l])
				  {   
		             fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			          j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
			          global_bridge_hgt[l],global_bridge_hgt[l]);
				  }   
				}  
			    j = j + 1;
			  }   
			}   
		  }   
		}
	  } 


     
  	  if( (num_b6>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {
        

        if(b6_bridgefirst>0)
		{
		  

		  savej = j;

          for(i=0;i<bridge_zvh_val_found;i++)
		  {
	        if(span_zvh_val_found>0)
			{
	          if(global_span_zvh[span_zvh_val_found-1]>global_bridge_zvh[i])
			  {
		        fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			      j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetZVCode())),
			      global_bridge_zvh[i],global_bridge_zvh[i]);
			    j = j + 1;
			  } 
			} 
		  } 

		  j = savej;

          for(i=0;i<bridge_zvh_val_found;i++)
		  { 
	        if(span_zvh_val_found>0)
			{ 
	          if(global_span_zvh[span_zvh_val_found-1]>global_bridge_zvh[i])
			  { 
			    for(l=0;l<span_zvh_val_found;l++)
				{ 
			      if(global_span_zvh[l]>global_bridge_zvh[i])
				  { 
		             fprintf(newfile,"SHARESEG %d SECONDARY BridgeSpan %s RANGE %lf %lf on C_LINE\n",
			          j,MakeCleanCode2(GetEACLabel(GetZVCode())),
			          global_span_zvh[l],global_span_zvh[l]);
				  } 
				} 
			    j = j + 1;
			  } 
			} 
		  } 
		} 
		else
		{

		  

		  savej = j;

          for(i=0;i<span_zvh_val_found;i++)
		  { 
	        if(bridge_zvh_val_found>0)
			{ 
	          if(global_span_zvh[i]>global_bridge_zvh[0])
			  { 
		        fprintf(newfile,"SHARESEG %d PRIMARY BridgeSpan %s RANGE %lf %lf on C_LINE\n",
			      j,MakeCleanCode2(GetEACLabel(GetZVCode())),
			      global_span_zvh[i],global_span_zvh[i]);
			    j = j + 1;
			  }   
			}   
		  }   

          j = savej;

          for(i=0;i<span_zvh_val_found;i++)
		  { 
	        if(bridge_zvh_val_found>0)
			{ 
	          if(global_span_zvh[i]>global_bridge_zvh[0])
			  { 
			    for(l=0;l<bridge_zvh_val_found;l++)
				{   
			      if(global_span_zvh[i]>global_bridge_zvh[l])
				  {   
		             fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			          j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetZVCode())),
			          global_bridge_zvh[l],global_bridge_zvh[l]);
				  }   
				}  
			    j = j + 1;
			  }   
			}   
		  }   

		}
	  } 



  	  if( (num_b7>0) && ((type==5)||(type==6)||(type==7)||(type==8)||(type==11)) )
	  {
        

        if(b7_bridgefirst>0)
		{

		  savej = j;

          for(i=0;i<bridge_len_val_found;i++)
		  { 
	        if(span_len_val_found>0)
			{ 
	          if(global_span_len[span_len_val_found-1]>global_bridge_len[i])
			  { 
		        fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			      j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex(GetLengthCode()))),
			      global_bridge_len[i],global_bridge_len[i]);

			    j = j + 1;
			  } 
			} 
		  } 

          j = savej;

          for(i=0;i<bridge_len_val_found;i++)
		  { 
	        if(span_len_val_found>0)
			{ 
	          if(global_span_len[span_len_val_found-1]>global_bridge_len[i])
			  { 
			    for(l=0;l<span_len_val_found;l++)
				{ 
			      if(global_span_len[l]>global_bridge_len[i])
				  { 
		             fprintf(newfile,"SHARESEG %d SECONDARY BridgeSpan %s RANGE %lf %lf on C_LINE\n",
			          j,MakeCleanCode2(GetEACLabel(GetAttrIndex(GetLengthCode()))),
			          global_span_len[l],global_span_len[l]);
				  } 
				} 
			    j = j + 1;
			  } 
			} 
		  } 
		} 
		else
		{

		  

		  savej = j;

          for(i=0;i<span_len_val_found;i++)
		  { 
	        if(bridge_len_val_found>0)
			{ 
	          if(global_span_len[i]>global_bridge_len[0])
			  { 
		        fprintf(newfile,"SHARESEG %d PRIMARY BridgeSpan %s RANGE %lf %lf on C_LINE\n",
			      j,MakeCleanCode2(GetEACLabel(GetAttrIndex(GetLengthCode()))),
			      global_span_len[i],global_span_len[i]);

			    j = j + 1;
			  }   
			}   
		  }   

          j = savej;

          for(i=0;i<span_len_val_found;i++)
		  { 
	        if(bridge_len_val_found>0)
			{ 
	          if(global_span_len[i]>global_bridge_len[0])
			  { 
			    for(l=0;l<bridge_len_val_found;l++)
				{   
			      if(global_span_len[i]>global_bridge_len[l])
				  {   
		             fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			          j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex(GetLengthCode()))),
			          global_bridge_len[l],global_bridge_len[l]);
				  }   
				}  
			    j = j + 1;
			  }   
			}   
		  }   

		} 
	  } 


	  if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
	  {
        FreeOtherShareSegData();
	  }



      if(total_tds_isl_checks>0)
	  {
        for(i=0;i<TDS_num_ISL;i++)
		{ 
          if(TDS_ISL_CODES[i]<0)
		  { 
            continue;
		  }  

          if((TDS_ISL_USE[i]==1)||(TDS_ISL_USE[i]==3))
		  { 
            sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(TDS_ISL_CODES[i])));
            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,cleancode);
		    for(k=0;k<TDS_num_ISL;k++)
			{ 
              if(TDS_ISL_CODES[k]<0)
			  { 
                continue;
			  }   


		      doit = 0;
		      if(ATTRTYPE_TDS4()==1)
			  {  
			    
		        if((TDS4_ISL_VALS[i][k]=='e')&&(linespresent[TDS_ISL_CODES[k]]==1))
				{  
			      doit = 1;
				} 
			  } 
		      else 
			  {  
		        if((TDS6_ISL_VALS[i][k]=='e')&&(linespresent[TDS_ISL_CODES[k]]==1))
				{  
			       
			       doit = 1;
				}  
			  }  
              
			  
			  if(doit==1)
			  { 
                sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(TDS_ISL_CODES[k])));
                fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,cleancode);
			  } 
			} 
		    j = j + 1;
		  } 

          if((TDS_ISL_USE[i]==2)||(TDS_ISL_USE[i]==3))
		  { 
            sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(TDS_ISL_CODES[i])));
            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,cleancode);
		    for(k=0;k<TDS_num_ISL;k++)
			{ 
              if(TDS_ISL_CODES[k]<0)
			  { 
                continue;
			  }   


		      doit = 0;
		      if(ATTRTYPE_TDS4()==1)
			  {  
			    
		        if((TDS4_ISL_VALS[i][k]=='w')&&(linespresent[TDS_ISL_CODES[k]]==1))
				{  
			      doit = 1;
				} 
			  } 
		      else 
			  {  
		        if((TDS6_ISL_VALS[i][k]=='w')&&(linespresent[TDS_ISL_CODES[k]]==1))
				{  
			       
			       doit = 1;
				}  
			  }  


              if(doit==1)
			  { 
                sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(TDS_ISL_CODES[k])));
                fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,cleancode);
			  } 
			}
		    j = j + 1;
		  } 
		} 
	  } 


      if((type!=5)&&(type!=6)&&(type!=7)&&(type!=8)&&(type!=11))
	  {
	    for(i=0;i<INscc_loop;i++)
	    {
	      if(linespresent[i]==1)
		  {
		    sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(i)));
		  
		    fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",
			  j,cleancode);
		    fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",
			  j,cleancode);
		    j = j + 1;
		  } 
		} 
	  }
	  }
      else
	{
	  fprintf(newfile,"Data not available to specify this check\n");
	}
      fclose(newfile);
    }


   else if(type==9)
  {
    
      
    sprintf(message,"%sSHARESEG.txt",prefix);
    newfile = fopen(message,"w");
    ExeUpdate(message);

    docheck1  = 0;

	if(lines_found>0)
	{
      docheck1 = 1;
	}

    totalchecks = docheck1;

    if(totalchecks>0)
	{
	  fprintf(newfile,"UNIQUE_CHECKS 1\n");
	  fprintf(newfile,"SHARESEG %d\n",totalchecks);
	  
	  j = 1;
      
	  if(docheck1>0)
	  {
	    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE off\n",j);
	    fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE on\n",j);
	    fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE off\n",j);
	    fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE on\n",j);
	    fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	    fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	    fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	    fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		fprintf(newfile,"SHARESEG %d ANNOTATION \"Overlapping line features\"\n",j);

        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

        fprintf(newfile,"ATTRATTRATTR\n");
        j = j + 1;
	  }
	}
    else
	{
	  fprintf(newfile,"Data not available to specify this check\n");
	}
    fclose(newfile);
   }

}







void DoSHARESEGtype2(
		int type,char *prefix,
		int *linespresent,
		int line_road_found,
		int line_bridge_found,
		int line_rail_found,
		int line_trail_found,
		int line_ctrack_found,
		int line_ford_found,
		int line_tunnel_found,
		int line_sidetrack_found,
		int line_canal_found,
		int line_aqueduct_found,
		int line_embankment_found,
		int line_pipe_found,
		int line_loc40road,
		int line_loc40rail,
		int line_5bridgefound,
		int line_9bridgefound,
		int line_12bridgefound,
		int line_999bridgefound,
		int line_5cutfound,
		int line_9cutfound,
		int line_12cutfound,
		int line_999cutfound,
		int line_12causewayfound,
		int line_999causewayfound,
		int line_5damfound,
		int line_9damfound,
		int line_12damfound,
		int line_999damfound,
		int line_5embankmentfound,
		int line_9embankmentfound,
		int line_12embankmentfound,
		int line_999embankmentfound,
		int line_5tunnelfound,		
		int line_9tunnelfound,
		int line_12tunnelfound,
		int line_999tunnelfound,
		int line_13bridgefound,
		int line_13cutfound,
		int line_13causewayfound,
		int line_13damfound,
		int line_13embankmentfound,
		int line_13tunnelfound,
		int line_14bridgefound,
		int line_14cutfound,
		int line_14causewayfound,
		int line_14damfound,
		int line_14embankmentfound,
		int line_14tunnelfound,
		int line_roadACC1found,
		int line_ctrackACC1found,
		int line_river_found,
		int line_fun0_bridge_found,
		int line_fun1_road_found,
		int line_fun6_road_found,
		int line_fun13_road_found,
		int line_fun1_bridge_found,
		int line_fun0_road_found,
		int line_fun6_bridge_found,
		int line_fun13_bridge_found,
		int area_building_found,
		int line_cut_found,
		int line_causeway_found,
		int line_dam_found,
        int line_bridge_wd1_32767,
		int line_road_wd1_32767,
		int line_ctrack_wd1_32767,
		int lnfordTRS_AUTO_found,
	    int lnfordTRS_OTHER_found,
	    int lnfordTRS_NULL_found,
		int line_loc45road,
		int line_loc45rail,
		int line_loc45pipe,
		int line_wtc2track,
		int line_wtc2trail,
        int line_998bridgefound,
        int line_998cutfound,
        int line_998tunnelfound,
        int line_998damfound,
		int area_canal_found,
		int area_aqueduct_found,
		int line_causeway_struct_found,
		int line_loc44aqueduct,
        int line_loc44canal,
        int line_loc44road,
        int line_loc44rail,
        int line_loc44pipe
		)
{
  FILE *newfile;
  char message[1000],tempchar[1000],cleancode[1000],cleancode2[1000];
  int docheck1,docheck2,docheck3,docheck4,docheck5,docheck6,docheck7,docheck8,
    docheck9,docheck10,docheck11,docheck12,docheck13,docheck14,docheck15,
    docheck16,docheck17,docheck18,docheck19,docheck20,docheck21,docheck22,
	docheck23,docheck24,docheck25,docheck26,docheck27,docheck28,docheck29,
	docheck30,docheck31,docheck32,docheck33,
	totalchecks,i,j,jj,k,l,thisindex,thatindex,
	locval,thiscomplex,thiscomplex2,primaryadded,instancemade,thispass,
	numlocinstances,numloc2instances,numloc3instances,numloc4instances,
	numloc5instances,numloc6instances,numloc7instances,numloc8instances,
	numloc9instances,numloc10instances,numloc11instances,numloc12instances,
	numloc13instances,numloc14instances,numloc15instances,numloc16instances,
	numloc17instances,numloc18instances;
  int num_b1,num_b2,num_b3,num_b4;
  int num_railway_gaw=0,num_sidetrack_gaw=0;

  double dval;

  int MGCP_num_ISL;
  char **MGCP_ISL;
  int *MGCP_ISL_CODES;
  char *MGCP_ISL_USE;
  char thisval;

  char **MGCP_C_ISL;
  int MGCP_num_c_ISL;



#define MGCP3_num_ISL 54
char  *MGCP3_ISL[MGCP3_num_ISL] = 
{"AF020","AK130","AK190","AL070","AL210","AL260","AN010","AN050","AP010","AP030","AP050","AQ040","AQ045","AQ064","AQ070","AQ075","AQ113","AQ130","AT030","AT041","AT060","BA010","BB041","BB043","BB140","BB190","BB230","BH010","BH020","BH030","BH060","BH070","BH110","BH120","BH140","BH180","BI020","BI030","BI040","BI041","BJ031","BJ040","DB010","DB061","DB070","DB071","DB090","DB100","DB110","DB200","EA020","EC030","FA090","GB050"};

int MGCP3_ISL_CODES[MGCP3_num_ISL];
char MGCP3_ISL_USE [MGCP3_num_ISL];

char MGCP3_ISL_VALS[MGCP3_num_ISL][MGCP3_num_ISL]=
 {
  {'x','x','x','x','x','x','c','x','x','c','x','x','x','x','x','x','c','g','x','x','x','x','x','x','x','g','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','x','x','x','x','x','c','x','x','c','x','g','x','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','x','x','x','x','c','x','x','g','x','x','x','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','x','x','x','g','x','x','c','x','x','x','x','x','x','c','g','g','x','x','x','x','x','x','x','x','c','c','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','g','x','x','x','x','x','g','x'},
  {'n','n','n','n','x','x','g','g','g','g','g','x','x','x','x','x','c','x','g','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','x','g','x','x','x','x','x','x','x','x','x','c','g','g','x','x','x','x','x','x','x','x','c','c','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','c','c','c','g','c','g','c','g','c','c','c','g','g','c','c','x','g','c','c','c','c','c','c','c','c','c','x','c','x','c','g','c','g','c','c','c','c','c','g','c','g','c','c','c','x','c','x','c'},
  {'n','n','n','n','n','n','n','x','c','g','c','g','x','g','x','x','c','g','x','x','x','x','g','x','x','x','x','x','c','x','x','x','x','x','x','x','g','x','g','x','x','x','x','x','g','x','g','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','x','x','x','g','x','g','x','x','c','g','g','x','x','x','x','x','x','x','x','c','c','c','c','g','x','x','c','x','g','x','g','g','x','x','x','x','g','x','c','g','x','g','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','c','c','g','c','g','c','c','c','g','x','x','x','x','g','c','x','x','c','c','c','c','c','g','x','c','c','c','g','c','g','g','c','c','c','c','g','c','g','g','c','c','c','c','c','c'},
  {'n','n','n','n','n','n','n','n','n','n','x','g','x','g','x','x','c','g','g','x','x','x','x','x','x','x','x','c','c','c','c','g','x','x','c','x','g','x','g','g','x','x','x','g','g','x','c','g','x','g','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','x','c','x','x','x','g','g','g','x','x','x','x','x','x','x','x','g','g','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','g','g','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','g','g','g','x','x','x','x','x','x','x','x','g','g','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','x','g','g','x','x','x','x','x','x','x','x','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','g','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','c','c','c','c','c','c','c','c','c','c','c','c','c','g','x','c','c','c','g','c','c','c','c','c','c','c','g','c','c','c','c','c','g','g','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','g','g','x','x','x','x','x','x','g','g','g','g','g','x','g','g','g','g','g','g','g','x','g','g','x','g','x','g','g','g','g','g','g','g','g'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','g','g','g','x','g','x','g','x','g','g','g','g','g','x','x','x','x','x','x','g','x','x','x','g','g','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','x','x','c','x','c','c','c','c','x','x','x','x','x','x','x','g','c','c','x','x','x','c','c','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','c','c','x','c','c','c','c','g','x','x','x','x','x','x','g','c','g','c','g','x','c','c','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','g','x','x','x','x','x','x','x','x','x','x','g','x','c','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','x','c','c','c','c','x','x','x','x','x','x','x','c','c','c','c','x','x','c','c','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','c','x','c','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','x','x','x','x','x','x','x','x','g','x','x','x','x','g','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','g','x','x','x','x','x','g','x','x','x','x','g','c','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x'}
 };



#define MGCP4_num_ISL 59

char  *MGCP4_ISL[MGCP4_num_ISL] = {"AF020","AH025","AK130","AK150","AK190","AL060","AL070","AL210","AL260","AN010","AN050","AP010","AP030","AP050","AQ040","AQ063","AQ070","AQ075","AQ113","AQ130","AT030","AT041","AT060","BA010","BB041","BB043","BB140","BB190","BB230","BD120","BH010","BH020","BH030","BH060","BH070","BH110","BH120","BH140","BH165","BH180","BI020","BI030","BI040","BI041","BJ031","BJ040","DB010","DB061","DB070","DB071","DB090","DB100","DB110","DB160","DB200","EA020","EC030","FA090","GB050"};

int MGCP4_ISL_CODES[MGCP4_num_ISL];
char MGCP4_ISL_USE [MGCP4_num_ISL];

char MGCP4_ISL_VALS[MGCP4_num_ISL][MGCP4_num_ISL]=
{
  {'x','x','x','x','x','x','x','x','x','c','x','x','c','x','x','x','x','x','c','g','x','x','x','x','x','x','x','g','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','x','x','x','x','x','x','x','x','c','x','x','c','g','x','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x'},
  {'n','n','x','x','x','x','x','x','x','c','x','x','c','x','g','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','x','x','x','x','x','x','c','x','x','c','x','x','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','x','x','x','x','x','c','x','x','g','x','x','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','x','x','x','x','c','x','x','c','x','x','x','x','x','c','g','x','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','x','x','x','g','x','x','c','x','x','x','x','x','c','g','g','x','x','x','x','x','x','x','x','x','c','c','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','g','x','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','n','x','x','g','g','g','g','g','x','x','x','x','c','x','g','x','x','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','x','g','x','x','c','x','x','x','x','x','c','g','g','x','x','x','x','x','x','x','x','x','c','c','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','c','c','c','g','c','g','g','c','c','c','g','g','c','c','x','g','c','c','c','c','x','c','c','c','c','c','x','c','x','g','c','g','c','g','c','c','c','c','c','g','c','g','c','c','c','c','x','c','x','c'},
  {'n','n','n','n','n','n','n','n','n','n','x','c','g','c','g','g','x','x','c','g','x','x','x','x','g','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','g','x','g','x','x','x','x','x','g','x','g','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','x','x','x','g','g','x','x','c','g','g','x','x','x','x','x','x','x','x','x','c','c','c','c','g','x','x','c','x','x','g','x','g','g','x','x','x','x','g','x','g','g','x','x','g','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','c','c','g','g','c','c','g','g','c','x','c','x','g','c','x','x','c','x','c','c','c','c','g','x','c','g','c','c','g','c','g','g','c','c','c','c','g','c','g','g','c','c','c','c','c','c','c'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','g','x','x','c','g','g','x','x','x','x','x','x','x','x','x','c','c','c','c','g','x','x','c','x','x','g','x','g','g','x','x','x','g','g','x','g','g','x','x','g','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','g','x','g','x','x','x','x','x','x','x','x','x','c','g','c','g','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','g','g','g','x','x','x','x','x','x','x','x','x','g','g','x','g','x','g','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','c','g','g','x','x','x','x','x','x','x','x','x','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','g','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','c','c','c','c','c','c','c','c','c','x','c','c','c','c','g','x','c','c','c','c','g','c','c','c','c','c','c','c','g','c','g','c','c','c','c','g','g','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','g','g','x','x','x','x','x','x','x','g','g','g','g','g','x','g','g','g','g','g','g','g','g','x','g','g','x','g','x','g','g','g','g','g','g','g','g','g'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','g','g','g','x','g','x','g','x','x','g','g','g','g','g','x','x','x','x','x','x','g','x','x','x','x','g','g','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','c','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','x','g','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','c','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','x','x','c','x','c','c','x','c','c','x','x','x','x','x','x','x','g','c','g','x','x','x','x','c','c','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','c','c','x','c','c','x','c','g','g','x','x','x','x','x','x','g','c','g','c','g','x','x','c','c','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','g','x','x','x','x','x','x','x','x','x','x','g','x','x','c','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','c','x','c','c','x','c','g','x','x','x','x','x','x','x','g','c','c','c','x','x','x','c','c','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','g','x','g','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','x','x','x','x','x','x','x','x','x','g','x','x','x','x','g','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','g','x','x','g','x','x','x','x','x','g','x','x','x','x','g','x','c','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x','x'},
  {'n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','n','x'}
};


#define MGCP3_num_c_ISL 181
char  *MGCP3_C_ISL[MGCP3_num_c_ISL*3] =
{
  "BH010","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "BH010","BH070","FEATURE1 NOT LOC=40,45",
  "BH010","BH120","FEATURE1 NOT LOC=40,45",
  "BH010","BH140","FEATURE1 NOT LOC=40,45",
  "BH010","BH180","FEATURE1 NOT LOC=40,45",
  "BH010","BI020","FEATURE1 NOT LOC=40,45",
  "BH010","DB071","FEATURE1 NOT LOC=40,45",
  "BH010","DB090","FEATURE1 NOT LOC=44 OR FEATURE2 NOT TRS=2",
  "BH010","EA020","FEATURE1 NOT LOC=40,45",
  "BH010","EC030","FEATURE1 NOT LOC=40,45",
  "BB190","BH020","FEATURE2 NOT LOC=40",
  "BB041","BH020","FEATURE2 NOT LOC=40",
  "AQ040","AQ045","FEATURE1 NOT FUN=3",
  "AQ040","BH030","FEATURE2 NOT CDA=1001",
  "AQ040","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AT041","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AT041","BH020","FEATURE2 NOT LOC=40",
  "BH020","BH030","FEATURE1 NOT LOC=40,45",
  "BH020","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "BH020","BH070","FEATURE1 NOT LOC=40,45",
  "BH020","BH120","FEATURE1 NOT LOC=40,45",
  "BH020","BH140","FEATURE1 NOT LOC=40,45",
  "BH020","BH180","FEATURE1 NOT LOC=40,45",
  "BH020","BI020","FEATURE1 NOT LOC=40,45",
  "BH020","DB071","FEATURE1 NOT LOC=40,45",
  "BH020","DB100","FEATURE1 NOT LOC=40,45",
  "BH020","EA020","FEATURE1 NOT LOC=40,45",
  "BH020","EC030","FEATURE1 NOT LOC=40,45",
  "AP010","AQ113","FEATURE2 NOT LOC=40",
  "AP010","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AP010","BH020","FEATURE2 NOT LOC=40,45",
  "AP010","BH030","FEATURE2 NOT CDA=1001",
  "AP010","BH060","FEATURE2 NOT LOC=40",
  "AP010","BH140","FEATURE2 NOT HYP=2,4",
  "AP010","DB090","FEATURE2 NOT TRS=13",
  "AQ064","BH060","FEATURE2 NOT LOC=40",
  "AT060","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AT060","BH020","FEATURE2 NOT LOC=40",
  "AF020","AN010","FEATURE2 NOT LOC=40,45",
  "AF020","AP030","FEATURE2 NOT LOC=40,45",
  "AF020","AQ113","FEATURE2 NOT LOC=40,45",
  "AF020","BH020","FEATURE2 NOT LOC=40,45",
  "BH030","EA020","FEATURE1 NOT CDA=1001",
  "AL070","AP030","FEATURE2 NOT LOC=40,45",
  "AL070","AQ113","FEATURE2 NOT LOC=40,45",
  "AL070","BH010","FEATURE2 NOT LOC=40,45",
  "AL070","BH020","FEATURE2 NOT LOC=40,45",
  "AL070","BH060","FEATURE2 NOT LOC=40",
  "AQ070","BH010","FEATURE2 NOT LOC=40",
  "AQ070","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "BH060","BH070","FEATURE1 NOT LOC=40",
  "BH060","BH120","FEATURE1 NOT LOC=40",
  "BH060","BH140","FEATURE1 NOT LOC=40",
  "BH060","BH180","FEATURE1 NOT LOC=40",
  "BH060","BI020","FEATURE1 NOT LOC=40",
  "BH060","DB070","FEATURE1 NOT LOC=40",
  "BH060","DB071","FEATURE1 NOT LOC=40",
  "BH060","DB090","FEATURE1 NOT LOC=40",
  "BH060","DB100","FEATURE1 NOT LOC=40",
  "BH060","EA020","FEATURE1 NOT LOC=40",
  "BH060","EC030","FEATURE1 NOT LOC=40",
  "BB043","BH020","FEATURE2 NOT LOC=40",
  "BA010","BH020","FEATURE2 NOT LOC=40",
  "BH110","DB070","FEATURE1 NOT LOC=44",
  "BH110","DB090","FEATURE1 NOT LOC=44",
  "AQ113","AQ130","FEATURE2 NOT TRS=10",
  "AQ113","AT030","FEATURE1 NOT LOC=40,45",
  "AQ113","AT041","FEATURE1 NOT LOC=40",
  "AQ113","AT060","FEATURE1 NOT LOC=40",
  "AQ113","BA010","FEATURE1 NOT LOC=40",
  "AQ113","BB041","FEATURE1 NOT LOC=40",
  "AQ113","BB043","FEATURE1 NOT LOC=40",
  "AQ113","BB140","FEATURE1 NOT LOC=40",
  "AQ113","BB190","FEATURE1 NOT LOC=17,23,46",
  "AQ113","BB230","FEATURE1 NOT LOC=40,45",
  "AQ113","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AQ113","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "AQ113","BH030","FEATURE1 NOT LOC=40,45",
  "AQ113","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AQ113","BH120","FEATURE1 NOT LOC=23,46",
  "AQ113","BH140","FEATURE1 NOT LOC=17,23,46",
  "AQ113","BH180","FEATURE1 NOT LOC=40,45",
  "AQ113","BI030","FEATURE1 NOT LOC=23",
  "AQ113","BI040","FEATURE1 NOT LOC=23,46",
  "AQ113","BI041","FEATURE1 NOT LOC=23,46",
  "AQ113","BJ031","FEATURE1 NOT LOC=40",
  "AQ113","BJ040","FEATURE1 NOT LOC=40",
  "AQ113","DB010","FEATURE1 NOT LOC=40",
  "AQ113","DB061","FEATURE1 NOT LOC=40",
  "AQ113","DB071","FEATURE1 NOT LOC=40,45",
  "AQ113","DB090","FEATURE2 NOT TRS=10",
  "AQ113","DB100","FEATURE1 NOT LOC=40",
  "AQ113","DB110","FEATURE1 NOT LOC=40",
  "AQ113","DB200","FEATURE1 NOT LOC=40",
  "AL210","AQ113","FEATURE2 NOT LOC=40",
  "AL210","BH020","FEATURE2 NOT LOC=40,45",
  "AK130","AN010","FEATURE2 NOT LOC=40,45",
  "AK130","AP030","FEATURE2 NOT LOC=40,45",
  "AK130","AQ113","FEATURE2 NOT LOC=40,45",
  "AK130","BH020","FEATURE2 NOT LOC=40,45",
  "AN010","AN010","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","AN050","FEATURE1 NOT LOC=40",
  "AN010","AP010","FEATURE1 NOT FUN=4",
  "AN010","AP050","FEATURE1 NOT FUN=4",
  "AN010","AQ045","FEATURE1 NOT FUN=3 OR FEATURE2 NOT FUN=3",
  "AN010","AQ070","FEATURE1 NOT LOC=40",
  "AN010","AQ075","FEATURE1 NOT LOC=40",
  "AN010","AQ113","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","AT041","FEATURE1 NOT LOC=40",
  "AN010","AT060","FEATURE1 NOT LOC=40",
  "AN010","BB043","FEATURE1 NOT LOC=40,45",
  "AN010","BB140","FEATURE1 NOT LOC=40,45",
  "AN010","BB190","FEATURE1 NOT LOC=40,45",
  "AN010","BB230","FEATURE1 NOT LOC=40",
  "AN010","BH010","FEATURE1 NOT LOC=40",
  "AN010","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","BH030","FEATURE1 NOT LOC=40,45",
  "AN010","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","BH070","FEATURE1 NOT LOC=40,45",
  "AN010","BH120","FEATURE1 NOT LOC=40,45",
  "AN010","BH180","FEATURE1 NOT LOC=40",
  "AN010","BI030","FEATURE1 NOT LOC=40,45",
  "AN010","BI041","FEATURE1 NOT LOC=40,45",
  "AN010","BJ031","FEATURE1 NOT LOC=40",
  "AN010","BJ040","FEATURE1 NOT LOC=40",
  "AN010","DB010","FEATURE1 NOT LOC=40",
  "AN010","DB061","FEATURE1 NOT LOC=40",
  "AN010","DB071","FEATURE1 NOT LOC=40",
  "AN010","DB100","FEATURE1 NOT LOC=40",
  "AN010","DB110","FEATURE1 NOT LOC=40,45",
  "AN010","DB200","FEATURE1 NOT LOC=40",
  "AN010","EC030","FEATURE1 NOT LOC=40",
  "AN010","GB050","FEATURE1 NOT LOC=40",
  "AN050","AP010","FEATURE1 NOT FUN=4",
  "AN050","AP050","FEATURE1 NOT FUN=4",
  "AN050","AQ113","FEATURE2 NOT LOC=40",
  "AN050","BH020","FEATURE2 NOT LOC=40,45",
  "AK190","AN010","FEATURE2 NOT LOC=40,45",
  "AK190","AQ113","FEATURE2 NOT LOC=17,23,46",
  "AK190","BH020","FEATURE2 NOT LOC=40,45",
  "BH140","DB200","FEATURE1 NOT WCC=3",
  "AP030","AP030","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","AP050","FEATURE1 NOT LOC=40,45",
  "AP030","AQ045","FEATURE1 NOT FUN=3 OR FEATURE2 NOT FUN=3",
  "AP030","AQ070","FEATURE1 NOT LOC=40",
  "AP030","AQ075","FEATURE1 NOT LOC=40",
  "AP030","AQ113","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","BB043","FEATURE1 NOT LOC=40,45",
  "AP030","BB230","FEATURE1 NOT LOC=40",
  "AP030","BH010","FEATURE1 NOT LOC=40",
  "AP030","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","BH030","FEATURE2 NOT CDA=1001",
  "AP030","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","BH120","FEATURE1 NOT LOC=40,45",
  "AP030","BH140","FEATURE1 NOT LOC=40,45",
  "AP030","BH180","FEATURE1 NOT LOC=40,45",
  "AP030","BI030","FEATURE1 NOT LOC=40,45",
  "AP030","BJ031","FEATURE1 NOT LOC=40,45",
  "AP030","BJ040","FEATURE1 NOT LOC=40,45",
  "AP030","DB010","FEATURE1 NOT LOC=40,45",
  "AP030","DB061","FEATURE1 NOT LOC=40,45",
  "AP030","DB071","FEATURE1 NOT LOC=40,45",
  "AP030","DB110","FEATURE1 NOT LOC=40,45",
  "AP030","DB200","FEATURE1 NOT LOC=40,45",
  "AP030","EA020","FEATURE1 NOT LOC=40,45",
  "AP030","EC030","FEATURE1 NOT LOC=40,45",
  "AP030","FA090","FEATURE1 NOT LOC=40,45",
  "AP030","GB050","FEATURE1 NOT LOC=40,45",
  "BB230","BH020","FEATURE2 NOT LOC=40",
  "AP050","AQ113","FEATURE2 NOT LOC=40",
  "AP050","BH010","FEATURE2 NOT LOC=40,45",
  "AP050","BH020","FEATURE2 NOT LOC=40,45",
  "AP050","BH030","FEATURE2 NOT CDA=1001",
  "AP050","BH060","FEATURE2 NOT LOC=40",
  "AP050","BH140","FEATURE2 NOT HYP=2,4",
  "AP050","DB090","FEATURE2 NOT TRS=9",
  "BB140","BH020","FEATURE2 NOT LOC=40",
  "AL260","AQ113","FEATURE2 NOT LOC=40,45",
  "AL260","BH010","FEATURE2 NOT LOC=40,45",
  "AL260","BH020","FEATURE2 NOT LOC=40,45",
  "AL260","BH060","FEATURE2 NOT LOC=40"
};





#define MGCP4_num_c_ISL 187
char *MGCP4_C_ISL[MGCP4_num_c_ISL*3] =
{
  "BH010","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "BH010","BH070","FEATURE1 NOT LOC=40,45",
  "BH010","BH120","FEATURE1 NOT LOC=40,45",
  "BH010","BH140","FEATURE1 NOT LOC=40,45",
  "BH010","BH180","FEATURE1 NOT LOC=40,45",
  "BH010","DB071","FEATURE1 NOT LOC=40,45",
  "BH010","EA020","FEATURE1 NOT LOC=40,45",
  "BH010","EC030","FEATURE1 NOT LOC=40,45",
  "BH010","BI020","FEATURE1 LOC NOT=44,45",
  "BH010","AQ040","FEATURE1 NOT LOC=45",
  "BB190","BH020","FEATURE2 NOT LOC=40",
  "BB041","BH020","FEATURE2 NOT LOC=40",
  "AQ040","BH030","FEATURE2 NOT CDA=1001",
  "AT041","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AT041","BH020","FEATURE2 NOT LOC=40",
  "BH020","BH030","FEATURE1 NOT LOC=40,45",
  "BH020","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "BH020","BH070","FEATURE1 NOT LOC=40,45",
  "BH020","BH120","FEATURE1 NOT LOC=40,45",
  "BH020","BH140","FEATURE1 NOT LOC=40,45",
  "BH020","BH180","FEATURE1 NOT LOC=40,45",
  "BH020","DB071","FEATURE1 NOT LOC=40,45",
  "BH020","DB100","FEATURE1 NOT LOC=40,45",
  "BH020","EA020","FEATURE1 NOT LOC=40,45",
  "BH020","EC030","FEATURE1 NOT LOC=40,45",
  "AP010","AQ113","FEATURE2 NOT LOC=40",
  "AP010","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AP010","BH020","FEATURE2 NOT LOC=40,45",
  "AP010","BH030","FEATURE2 NOT CDA=1001",
  "AP010","BH060","FEATURE2 NOT LOC=40",
  "AP010","BH140","FEATURE2 NOT HYP=2,4",
  "AT060","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AT060","BH020","FEATURE2 NOT LOC=40",
  "AF020","AN010","FEATURE2 NOT LOC=40,45",
  "AF020","AP030","FEATURE2 NOT LOC=40,45",
  "AF020","AQ113","FEATURE2 NOT LOC=40,45",
  "AF020","BH020","FEATURE2 NOT LOC=40,45",
  "BH030","EA020","FEATURE1 NOT CDA=1001",
  "AL060","AN010","FEATURE2 NOT LOC=40,45",
  "AL060","AP030","FEATURE2 NOT LOC=40",
  "AL060","AQ113","FEATURE2 NOT LOC=40,45",
  "AL060","BH020","FEATURE2 NOT LOC=40,45",
  "AH025","AN010","FEATURE2 NOT LOC=40,45",
  "AH025","AP030","FEATURE2 NOT LOC=40,45",
  "AH025","AQ113","FEATURE2 NOT LOC=40,45",
  "AH025","BH020","FEATURE2 NOT LOC=40,45",
  "AL070","AP030","FEATURE2 NOT SEP=1001 OR MES=1001",
  "AL070","AQ113","FEATURE2 NOT LOC=40,45",
  "AL070","BH010","FEATURE2 NOT LOC=40,45",
  "AL070","BH020","FEATURE2 NOT LOC=40,45",
  "AL070","BH060","FEATURE2 NOT LOC=40",
  "AQ070","BH010","FEATURE2 NOT LOC=40",
  "AQ070","BH020","FEATURE2 NOT LOC=40,45",
  "AQ070","AQ113","FEATURE2 NOT LOC=17,23,46",
  "BH060","BH070","FEATURE1 NOT LOC=40",
  "BH060","BH120","FEATURE1 NOT LOC=40",
  "BH060","BH140","FEATURE1 NOT LOC=40",
  "BH060","BH180","FEATURE1 NOT LOC=40",
  "BH060","DB071","FEATURE1 NOT LOC=40",
  "BH060","DB090","FEATURE1 NOT LOC=40",
  "BH060","DB100","FEATURE1 NOT LOC=40",
  "BH060","EA020","FEATURE1 NOT LOC=40",
  "BH060","EC030","FEATURE1 NOT LOC=40",
  "BB043","BH020","FEATURE2 NOT LOC=40",
  "BA010","BH020","FEATURE2 NOT LOC=40",
  "AQ113","AT030","FEATURE1 NOT LOC=40,45",
  "AQ113","AT041","FEATURE1 NOT LOC=40",
  "AQ113","AT060","FEATURE1 NOT LOC=40",
  "AQ113","BA010","FEATURE1 NOT LOC=40",
  "AQ113","BB041","FEATURE1 NOT LOC=40",
  "AQ113","BB043","FEATURE1 NOT LOC=40",
  "AQ113","BB140","FEATURE1 NOT LOC=40",
  "AQ113","BB190","FEATURE1 NOT LOC=17,23,46",
  "AQ113","BB230","FEATURE1 NOT LOC=40,45",
  "AQ113","BH010","FEATURE1 LOC = FEATURE2 LOC",
  "AQ113","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "AQ113","BH030","FEATURE1 NOT LOC=40,45",
  "AQ113","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AQ113","BH120","FEATURE1 NOT LOC=23,46",
  "AQ113","BH140","FEATURE1 NOT LOC=17,23,46",
  "AQ113","BH165","FEATURE1 NOT LOC=40,45",
  "AQ113","BH180","FEATURE1 NOT LOC=40,45",
  "AQ113","BI030","FEATURE1 NOT LOC=23",
  "AQ113","BI040","FEATURE1 NOT LOC=23,46",
  "AQ113","BI041","FEATURE1 NOT LOC=23,46",
  "AQ113","BJ031","FEATURE1 NOT LOC=40",
  "AQ113","BJ040","FEATURE1 NOT LOC=40",
  "AQ113","DB010","FEATURE1 NOT LOC=40",
  "AQ113","DB061","FEATURE1 NOT LOC=40",
  "AQ113","DB071","FEATURE1 NOT LOC=40,45",
  "AQ113","DB100","FEATURE1 NOT LOC=40",
  "AQ113","DB110","FEATURE1 NOT LOC=40",
  "AQ113","DB160","FEATURE1 NOT LOC=40",
  "AQ113","DB200","FEATURE1 NOT LOC=40",
  "AL210","AQ113","FEATURE2 NOT LOC=40",
  "AL210","BH020","FEATURE2 NOT LOC=40,45",
  "AK130","AN010","FEATURE2 NOT LOC=40,45",
  "AK130","AP030","FEATURE2 NOT LOC=40,45",
  "AK130","AQ113","FEATURE2 NOT LOC=40,45",
  "AK130","BH020","FEATURE2 NOT LOC=40,45",
  "AN010","AN010","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","AN050","FEATURE1 NOT LOC=40",
  "AN010","AP010","FEATURE1 NOT FUN=4",
  "AN010","AP050","FEATURE1 NOT FUN=4",
  "AN010","AQ070","FEATURE1 NOT LOC=40",
  "AN010","AQ075","FEATURE1 NOT LOC=40",
  "AN010","AQ113","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","AT041","FEATURE1 NOT LOC=40",
  "AN010","AT060","FEATURE1 NOT LOC=40",
  "AN010","BB043","FEATURE1 NOT LOC=40,45",
  "AN010","BB140","FEATURE1 NOT LOC=40,45",
  "AN010","BB190","FEATURE1 NOT LOC=40,45",
  "AN010","BB230","FEATURE1 NOT LOC=40",
  "AN010","BH010","FEATURE1 NOT LOC=40",
  "AN010","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","BH030","FEATURE1 NOT LOC=40,45",
  "AN010","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AN010","BH070","FEATURE1 NOT LOC=40,45",
  "AN010","BH120","FEATURE1 NOT LOC=40,45",
  "AN010","BH180","FEATURE1 NOT LOC=40",
  "AN010","BI030","FEATURE1 NOT LOC=40,45",
  "AN010","BI041","FEATURE1 NOT LOC=40,45",
  "AN010","BJ031","FEATURE1 NOT LOC=40",
  "AN010","BJ040","FEATURE1 NOT LOC=40",
  "AN010","DB010","FEATURE1 NOT LOC=40",
  "AN010","DB061","FEATURE1 NOT LOC=40",
  "AN010","DB071","FEATURE1 NOT LOC=40",
  "AN010","DB100","FEATURE1 NOT LOC=40",
  "AN010","DB110","FEATURE1 NOT LOC=40,45",
  "AN010","DB160","FEATURE1 NOT LOC=40",
  "AN010","DB200","FEATURE1 NOT LOC=40",
  "AN010","EC030","FEATURE1 NOT LOC=40",
  "AN010","GB050","FEATURE1 NOT LOC=40",
  "AN050","AQ113","FEATURE2 NOT LOC=40",
  "AN050","BH020","FEATURE2 NOT LOC=40,45",
  "AN050","AP010","FEATURE1 NOT FUN=4",
  "AN050","AP050","FEATURE1 NOT FUN=4",
  "AK190","AN010","FEATURE2 NOT LOC=40,45",
  "AK190","AQ113","FEATURE2 NOT LOC=17,23,46",
  "AK190","BH020","FEATURE2 NOT LOC=40,45",
  "BD120","BH020","FEATURE2 NOT LOC=40",
  "BH140","DB200","FEATURE1 NOT WCC=3",
  "AP030","AP030","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","AP050","FEATURE1 NOT LOC=40,45",
  "AP030","AQ070","FEATURE1 NOT LOC=40",
  "AP030","AQ075","FEATURE1 NOT LOC=40",
  "AP030","BB043","FEATURE1 NOT LOC=40,45",
  "AP030","BB230","FEATURE1 NOT LOC=40",
  "AP030","BH010","FEATURE1 NOT LOC=40",
  "AP030","BH020","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","BH030","FEATURE2 NOT CDA=1001 OR FEATURE1 NOT SEP=1001 OR MES=1001",
  "AP030","BH060","FEATURE1 LOC = FEATURE2 LOC",
  "AP030","BH120","FEATURE1 NOT LOC=40,45",
  "AP030","BH165","FEATURE1 NOT LOC=40,45",
  "AP030","BH180","FEATURE1 NOT LOC=40,45",
  "AP030","BI030","FEATURE1 NOT LOC=40,45",
  "AP030","BJ031","FEATURE1 NOT LOC=40,45",
  "AP030","BJ040","FEATURE1 NOT LOC=40,45",
  "AP030","DB010","FEATURE1 NOT LOC=40,45",
  "AP030","DB061","FEATURE1 NOT LOC=40,45",
  "AP030","DB071","FEATURE1 NOT SEP=1001 or MES=1001",
  "AP030","DB110","FEATURE1 NOT LOC=40,45",
  "AP030","DB160","FEATURE1 NOT LOC=40,45",
  "AP030","DB200","FEATURE1 NOT LOC=40,45",
  "AP030","EA020","FEATURE1 NOT SEP=1001 or MES=1001",
  "AP030","EC030","FEATURE1 NOT SEP=1001 or MES=1001",
  "AP030","FA090","FEATURE1 NOT LOC=40,45",
  "AP030","GB050","FEATURE1 NOT LOC=40,45",
  "AP030","AT060","FEATURE1 NOT SEP=1001 or MES=1001",
  "AP030","AL260","FEATURE1 NOT SEP=1001 or MES=1001",
  "AP030","AQ113","FEATURE1 NOT SEP=1001 or MES=1001",
  "BB230","BH020","FEATURE2 NOT LOC=40",
  "AK150","AN010","FEATURE2 NOT LOC=40,45",
  "AK150","AP030","FEATURE2 NOT LOC=40",
  "AK150","AQ113","FEATURE2 NOT LOC=40,45",
  "AK150","BH020","FEATURE2 NOT LOC=40,45",
  "AP050","AQ113","FEATURE2 NOT LOC=40",
  "AP050","BH010","FEATURE2 NOT LOC=40,45",
  "AP050","BH020","FEATURE2 NOT LOC=40,45",
  "AP050","BH030","FEATURE2 NOT CDA=1001",
  "AP050","BH060","FEATURE2 NOT LOC=40",
  "AP050","BH140","FEATURE2 NOT HYP=2,4",
  "BB140","BH020","FEATURE2 NOT LOC=40",
  "AL260","AQ113","FEATURE2 NOT LOC=40,45",
  "AL260","BH010","FEATURE2 NOT LOC=40,45",
  "AL260","BH020","FEATURE2 NOT LOC=40,45",
  "AL260","BH060","FEATURE2 NOT LOC=40"
};



short int *primaryCcodes=NULL;
short int *secondaryCcodes=NULL;







 extern void ExeUpdate(char message[]);
 extern char *GetRailName(int english);
 extern char *GetBridgeName(int english);
 extern char *GetRoadName(int english);
 extern char *GetSideTrackName(int english);
 extern char *GetPipelineName(int english);
 extern char *GetInterchangeName(int english);
 extern char *GetEmbankmentName(int english);
 extern char *GetWD1Name();
 extern char *GetWD1Name2();
 extern char *GetContourLineName(int english);
 extern char *GetLengthCode();
 extern char *GetLOCName(int english);
 extern char *GetUnderGroundName(int english);
 extern char *GetRSTName();
 extern char *GetCETName(int english);
 extern int WD1IsLarger(double wd1_num,double wid_num);
 extern int FIND_REAL;
 extern double GetGAW(char *charSCC,char *geometry,char *charSAC, int flag);
 extern char *GetRiverName(int english);
 extern char *GetBuildingName(int english);
 extern char *GetDamName(int english);
 extern char *GetACCCode(int english);
 extern int FindAttr(double dval,int passtype,char *geometry,char *charSCC, char *charSAC, char *targetvalue);
 extern char *GetUndergroundName(int english);
 extern char *GetAboveName(int english);
 extern char *GetCDACode(int english);
 extern char *GetHYCCode(int english);
 extern char *GetTRS1Name();
 extern char *GetLOC17Name(int english);
 extern char *GetLOC23Name(int english);









  if(ATTRTYPE_MGCP3())
  { 
    MGCP_num_ISL = MGCP3_num_ISL;
	MGCP_ISL = MGCP3_ISL;
	MGCP_ISL_CODES = MGCP3_ISL_CODES;
	MGCP_ISL_USE = MGCP3_ISL_USE;


    MGCP_num_c_ISL = MGCP3_num_c_ISL;
	MGCP_C_ISL = MGCP3_C_ISL;

  }
  else
  {
    MGCP_num_ISL = MGCP4_num_ISL;
	MGCP_ISL = MGCP4_ISL;
	MGCP_ISL_CODES = MGCP4_ISL_CODES;
	MGCP_ISL_USE = MGCP4_ISL_USE;

    MGCP_num_c_ISL = MGCP4_num_c_ISL;
	MGCP_C_ISL = MGCP4_C_ISL;
  }


  if(type==2)
    {
      
      
      sprintf(message,"%sSHARESEG.txt",prefix);
      newfile = fopen(message,"w");
      ExeUpdate(message);


      

	  totalchecks = 0;


      for(i=0;i<MGCP_num_ISL;i++)
	  { 
        MGCP_ISL_CODES[i] = SilentConvertClassCode(MGCP_ISL[i]);
	    if(MGCP_ISL_CODES[i]>=0)
		{ 
          MGCP_ISL_CODES[i] = MGCP_ISL_CODES[i] + 1;
		} 

        MGCP_ISL_USE[i] = 0;
	  } 

	  for(i=0;i<MGCP_num_ISL;i++)
	  { 
        if(MGCP_ISL_CODES[i]<0)
		{ 
          
          continue;
		} 

	    if(linespresent[MGCP_ISL_CODES[i]]==0)
		{ 
		  
          continue;
		} 


	    for(j=0;j<MGCP_num_ISL;j++)
		{ 
          if(MGCP_ISL_CODES[j]<0)
		  { 
            continue;
		  }  

		  if(MGCP_ISL_USE[i]==3)
		  { 
		    continue;
		  } 

		  

		  if(ATTRTYPE_MGCP3())
		  {
            thisval = MGCP3_ISL_VALS[i][j];
		  }
		  else
		  {
            thisval = MGCP4_ISL_VALS[i][j];
		  }

		  if(thisval=='e')  
		  { 
		    if(linespresent[MGCP_ISL_CODES[j]]==1)
			{ 
              

			  if(MGCP_ISL_USE[i]==0)
			  { 
			    MGCP_ISL_USE[i] = 1;
                totalchecks = totalchecks + 1;
			  } 
			  if(MGCP_ISL_USE[i]==2)
			  { 
			    MGCP_ISL_USE[i] = 3;
                totalchecks = totalchecks + 1;
			  } 
			} 
		  } 
		  if(thisval=='x') 
		  { 
		    if(linespresent[MGCP_ISL_CODES[j]]==1)
			{ 
              
			  if(MGCP_ISL_USE[i]==0)
			  { 
			    MGCP_ISL_USE[i] = 2;
                totalchecks = totalchecks + 1;
			  } 
			  if(MGCP_ISL_USE[i]==1)
			  { 
			    MGCP_ISL_USE[i] = 3;
                totalchecks = totalchecks + 1;
			  } 
			} 
		  } 
		} 
	  } 


      docheck1  = 0;
      docheck2  = 0;
      docheck3  = 0;
      docheck4  = 0;
      docheck5  = 0;
      docheck6  = 0;
	  docheck7  = 0;
	  docheck8  = 0;
	  docheck9  = 0;
	  docheck10 = 0;
	  docheck11 = 0;
	  docheck12 = 0;
	  docheck13 = 0;
	  docheck14 = 0;
	  docheck15 = 0;
	  docheck16 = 0;
	  docheck17 = 0;
	  docheck18 = 0;

	  docheck19 = 0;
	  docheck20 = 0;
	  docheck21 = 0;  
	  docheck22 = 0;
	  docheck23 = 0;
	  docheck24 = 0;

	  docheck25 = 0;
	  docheck26 = 0;
	  docheck27 = 0;
	  docheck28 = 0; 
	  docheck29 = 0;
	  docheck30 = 0;

	  docheck31 = 0; 
	  docheck32 = 0; 
	  docheck33 = 0; 



	  num_b1 = 0;
	  num_b2 = 0;
	  num_b3 = 0;

	  if(line_road_found>0) 
	  {
	    GenerateShareSegInfo(&num_b1,&num_b2,&num_b3,&num_b4);
	  }
      

	  if((line_ford_found>0)&&(ATTRTYPE_MGCP3()))
	  {
        if((line_road_found+line_ctrack_found)>0)
		{
	      docheck1 = 1;
		}
        if(line_trail_found>0)
		{
	      docheck2 = 1;
		}
	  }
	   

      if( 
		  (line_bridge_found>0)     ||
		  (line_cut_found>0)        ||
		  (line_causeway_found>0)   ||
		  (line_dam_found>0)        ||
		  (line_embankment_found>0) ||
		  (line_tunnel_found>0)
		)
	  {

		if(
			(ATTRTYPE_MGCP3())||  
			(ATTRTYPE_NFDD()) ||
			(ATTRTYPE_DFDD())
		  )
		{
          if((line_road_found>0)||(line_ctrack_found>0))
		  { 
	        docheck3 = 1;
		  } 

          if((line_sidetrack_found>0)||(line_rail_found>0))
		  { 
	        docheck4 = 1;
		  } 

          if(line_canal_found>0)
		  { 
	        docheck5 = 1;
		  } 

          if(line_trail_found>0)
		  { 
	        docheck6 = 1;
		  } 
		} 
	  }


	if((line_bridge_found>0)&&((line_road_found+line_ctrack_found)>0))
	{
      if(line_bridge_wd1_32767>0)  
	  {
		if((road_wd1_val_found+cart_wd1_val_found)>0)
		{
          
          docheck7 = 1;
		}
	  }
      if((line_road_wd1_32767+line_ctrack_wd1_32767)>0)
	  {
		if(bridge_wd1_val_found>0)
		{
          
          docheck8 = 1;
		}
	  }
	}



    if((line_road_found+line_rail_found+line_pipe_found)>0)
	{
	  if(line_bridge_found>0)
	  {
	    docheck9 = 1;
	  }
	}


	if((type==2)&&(line_tunnel_found>0))
	{
      if((line_roadACC1found+line_ctrackACC1found)>0)
	  {
         docheck10 = 1;
	  }
	}


	if(line_ford_found>0)
	{
       docheck11 = 1;
	}

    if(((line_ctrack_found+line_trail_found)>0)&&(line_river_found>0))
	{
       docheck12 = 1;
	}


    if((line_rail_found+line_road_found)>0)
	{
      if(line_tunnel_found>0)
	  {
	    docheck13 = 1;
	  }
	}


    if(line_fun0_bridge_found>0)
	{
      if((line_fun1_road_found + line_fun6_road_found + line_fun13_road_found)>0)
	  {
        docheck14 = 1;
	  }
	}
    if(line_fun1_bridge_found>0)
	{
      if((line_fun0_road_found + line_fun6_road_found + line_fun13_road_found)>0)
	  {
        docheck15 = 1;
	  }
	}
    if(line_fun6_bridge_found>0)
	{
      if((line_fun0_road_found + line_fun1_road_found + line_fun13_road_found)>0)
	  {
        docheck16 = 1;
	  }
	}
    if(line_fun13_bridge_found>0)
	{
      if((line_fun0_road_found + line_fun1_road_found + line_fun6_road_found)>0)
	  {
        docheck17 = 1;
	  }
	}


	if(ATTRTYPE_MGCP3()==0)
	{
      if((line_998bridgefound         +
		  line_998cutfound            +
		  line_998tunnelfound         +
		  line_998damfound            +
		  line_causewaystrTRS[TRS998] +
          line_embankmentTRS[TRS998]
		  )>0)
	  { 
        docheck18 = 1;
	  } 
	}


	if(ATTRTYPE_MGCP4())
	{
	   
       

		if( 
			(
			  line_bridgeTRS     [TRS13]+
			  line_causewaystrTRS[TRS13]+
			  line_tunnelTRS     [TRS13]+
			  line_damTRS        [TRS13]+
			  line_embankmentTRS [TRS13]+
			  line_cutTRS        [TRS13]
			)>0
		  )
		{
		  docheck19 = 1;
		}

		if( 
			(
			  line_bridgeTRS     [TRS12]+
			  line_causewaystrTRS[TRS12]+
			  line_tunnelTRS     [TRS12]+
			  line_damTRS        [TRS12]+
			  line_embankmentTRS [TRS12]+
			  line_cutTRS        [TRS12]
			)>0
		  )
		{
		  docheck20 = 1;
		}

		if( 
			(
			  line_bridgeTRS     [TRS9]+
			  line_causewaystrTRS[TRS9]+
			  line_tunnelTRS     [TRS9]+
			  line_damTRS        [TRS9]+
			  line_embankmentTRS [TRS9]+
			  line_cutTRS        [TRS9]
			)>0
		  )
		{
		  docheck21 = 1;
		}

		if( 
			(
			  line_bridgeTRS     [TRS10]+
			  line_causewaystrTRS[TRS10]+
			  line_tunnelTRS     [TRS10]+
			  line_damTRS        [TRS10]+
			  line_embankmentTRS [TRS10]+
			  line_cutTRS        [TRS10]
			)>0
		  )
		{
		  docheck22 = 1;
		}

		if( 
			(
			  line_bridgeTRS     [TRS5]+
			  line_causewaystrTRS[TRS5]+
			  line_tunnelTRS     [TRS5]+
			  line_damTRS        [TRS5]+
			  line_embankmentTRS [TRS5]+
			  line_cutTRS        [TRS5]
			)>0
		  )
		{
		  docheck23 = 1;
		}

		if( 
			(
			  line_bridgeTRS     [TRS2]+
			  line_causewaystrTRS[TRS2]+
			  line_tunnelTRS     [TRS2]+
			  line_damTRS        [TRS2]+
			  line_embankmentTRS [TRS2]+
			  line_cutTRS        [TRS2]
			)>0
		  )
		{
		  docheck24 = 1;
		}


		if( 
			(
			  line_bridgeTRS     [TRS999]+
			  line_causewaystrTRS[TRS999]+
			  line_tunnelTRS     [TRS999]+
			  line_damTRS        [TRS999]+
			  line_embankmentTRS [TRS999]+
			  line_cutTRS        [TRS999]
			)>0
		  )
		{
		  if((line_road_found+line_ctrack_found)>0)
		  {
		    docheck25 = 1;
		  }
		  if((line_rail_found+line_sidetrack_found)>0)
		  {
		    docheck26 = 1;
		  }
		  if(line_trail_found>0)
		  {
		    docheck27 = 1;
		  }
		  if(line_pipe_found>0)
		  {
		    docheck28 = 1;
		  }
		  if(line_canal_found>0)
		  {
		    docheck29 = 1;
		  }
		  if(line_aqueduct_found>0)
		  {
		    docheck30 = 1;
		  }
		}
	}


	if(ATTRTYPE_MGCP3()==0)
	{
      
	}



    if(ATTRTYPE_ANY_MGCP())
	{ 
	  num_railway_gaw   = (int)GetGAW(GetRailName(1),"(LINE)","GAW",-1);
	  num_sidetrack_gaw = (int)GetGAW(GetSideTrackName(1),"(LINE)","GAW",-1);
	}



	

	for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	{
      if(
		  (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 LOC = FEATURE2 LOC"))                             &&
          (strncmp(MGCP_C_ISL[thiscomplex+2],"FEATURE2 NOT ",13))                                        &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=40,45"))                                  &&


          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=17,23,46"))                               &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=23,46"))                                  &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=23"))                                     &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 LOC NOT=44,45"))                                  &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT FUN=4"))                                      &&

          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=44"))                                     &&

          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=40"))                                     &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT FUN=3"))                                      &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT CDA=1001"))                                   &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT WCC=3"))                                      &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT FUN=3 OR FEATURE2 NOT FUN=3"))                &&

          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=44 OR FEATURE2 NOT TRS=2"))               &&


          (strcmp (MGCP_C_ISL[thiscomplex+2],"NOT COINCIDENT WITH FEATURE1 RIR=1001 OR FEATURE1 FUN=4")) &&

          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT LOC=45"))                                     &&
          (strcmp (MGCP_C_ISL[thiscomplex+2],"FEATURE1 NOT SEP=1001 or MES=1001"))
		)
	  {
         printf("Unhandled ISLC case specification #%d: %s %s %s\n",
			 thiscomplex/3+1,
			 MGCP_C_ISL[thiscomplex],
			 MGCP_C_ISL[thiscomplex+1],
			 MGCP_C_ISL[thiscomplex+2]);
	  }
	}


	



	

	primaryCcodes   = (short int *)malloc(sizeof(short int)*MGCP_num_c_ISL);
	secondaryCcodes = (short int *)malloc(sizeof(short int)*MGCP_num_c_ISL);

	for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	{
	  if(thiscomplex>0)
	  {
		if(strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		{
          primaryCcodes[thiscomplex/3] = SilentGetCodeIndex(MGCP_C_ISL[thiscomplex]);
		}
		else
		{
		  primaryCcodes[thiscomplex/3] = primaryCcodes[thiscomplex/3-1];
		}
	  }
	  else
	  {
        primaryCcodes[thiscomplex/3]   = SilentGetCodeIndex(MGCP_C_ISL[thiscomplex]);
	  }

      secondaryCcodes[thiscomplex/3] = SilentGetCodeIndex(MGCP_C_ISL[thiscomplex+1]);
	}

	

	numlocinstances = 0;

	for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	{

	  if(primaryCcodes[thiscomplex/3]<0)
	  {
		

		
        continue;
	  }


	  if(thiscomplex>0)
	  {
		if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		{
		  
		  continue;
		}
	  }


      


	  

	  

      for(i=0;i<NumberOfModels;i++)
	  {  
        if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),MGCP_C_ISL[thiscomplex])))
		{   
	      

	      for(j=0;j<MdlNames[i].numattributes;j++)
		  {   	    
	        thisindex = MdlNames[i].AttrIndices[j];
	        thatindex = MdlNames[i].ValueIndices[j];

	        if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"LOC"))
			{  
		      
		      if(MdlNames2[thisindex].values[thatindex].type==3)
			  {  
			    locval = (int)MdlNames2[thisindex].values[thatindex].value;

			    

                

                for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
				{
                  if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
				  {
					
					break;
				  }

				  if(secondaryCcodes[thiscomplex2/3]<0)
				  {
					 
					 continue;
				  }

                  if(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 LOC = FEATURE2 LOC"))
				  {  
		             
		             continue;
				  }   


				  

				  

	              if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",
					  GetEELabel(MdlNames2[thisindex].code,(int)MdlNames2[thisindex].values[thatindex].value)))
				  {
					 
					 numlocinstances = numlocinstances + 1;
					 break;
				  }
				}
			  }  
			}  
		  }  
		}
	  } 
	} 









    






	



	numloc2instances = 0;

	for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	{

	  if(primaryCcodes[thiscomplex/3]<0)
	  { 
		
        continue;
	  } 

	  if(thiscomplex>0)
	  { 
		if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		{ 
		  
		  continue;
		} 
	  }  


	  if(linespresent[primaryCcodes[thiscomplex/3]]==1)
	  { 
        for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		{
          if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
		  {
			
			break;
		  } 

		  if(secondaryCcodes[thiscomplex2/3]<0)
		  {
			
			continue;
		  }

          if(strncmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT ",13))
		  {   
		    continue;
		  }    

		  if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
		  {
            numloc2instances = numloc2instances + 1;
			break;
		  }
		} 
	  } 
	} 









    



	



	numloc3instances = 0;

	for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	{

	  if(primaryCcodes[thiscomplex/3]<0)
	  { 
		
        continue;
	  } 

	  if(thiscomplex>0)
	  { 
		if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		{ 
		  
		  continue;
		} 
	  }  


	  if(linespresent[primaryCcodes[thiscomplex/3]]==1)
	  { 
        for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		{
          if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
		  {
			
			break;
		  } 

		  if(secondaryCcodes[thiscomplex2/3]<0)
		  {
			
			continue;
		  }

          if(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=40,45"))
		  {   
		    continue;
		  }    

		  if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
		  {
            numloc3instances = numloc3instances + 1;
			break;
		  }
		} 
	  } 
	} 






    



	



	numloc4instances  = 0; 
	numloc5instances  = 0; 
	numloc6instances  = 0; 
	numloc7instances  = 0; 
	numloc8instances  = 0; 
    numloc9instances  = 0; 

    numloc10instances = 0; 
    numloc11instances = 0; 
    numloc12instances = 0; 
	numloc13instances = 0; 
	numloc14instances = 0; 
	numloc15instances = 0; 

	numloc16instances = 0; 
	numloc17instances = 0; 

	numloc18instances = 0; 


	for(thispass=0;thispass<15;thispass++)
	{
	  for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	  { 

	    if(primaryCcodes[thiscomplex/3]<0)
		{  
		  
          continue;
		}  

	    if(thiscomplex>0)
		{  
		  if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		  {  
		    
		    continue;
		  }  
		}   


	    if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		{  
          for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		  { 
            if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			{ 
			  
			  break;
			}  

		    if(secondaryCcodes[thiscomplex2/3]<0)
			{ 
			  
			  continue;
			} 

            if((thispass==0)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=40")))
			{    
		      continue;
			}     
            if((thispass==1)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=3")))
			{    
		      continue;
			}     
            if((thispass==2)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT CDA=1001")))
			{    
		      continue;
			}     
            if((thispass==3)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT WCC=3")))
			{    
		      continue;
			}     
            if((thispass==4)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=3 OR FEATURE2 NOT FUN=3")))
			{    
		      continue;
			}     
            if((thispass==5)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"NOT COINCIDENT WITH FEATURE1 RIR=1001 OR FEATURE1 FUN=4")))
			{
		      continue;
			}

            if((thispass==6)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=17,23,46")))
			{
		      continue;
			}
            if((thispass==7)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=23,46")))
			{
		      continue;
			}
            if((thispass==8)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=23")))
			{
		      continue;
			}
            if((thispass==9)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 LOC NOT=44,45")))
			{
		      continue;
			}
            if((thispass==10)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=4")))
			{    
		      continue;
			}     
            if((thispass==11)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=44 OR FEATURE2 NOT TRS=2")))
			{    
		      continue;
			}     
            if((thispass==12)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=45")))
			{    
		      continue;
			}     
            if((thispass==13)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT SEP=1001 or MES=1001")))
			{    
		      continue;
			}     
            if((thispass==14)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=44")))
			{    
		      continue;
			}     




		    if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			{
			  if(thispass==0)  {numloc4instances  = numloc4instances  + 1;  break;}
			  if(thispass==1)  {numloc5instances  = numloc5instances  + 1;  break;}
			  if(thispass==2)  {numloc6instances  = numloc6instances  + 1;  break;}
			  if(thispass==3)  {numloc7instances  = numloc7instances  + 1;  break;}
			  if(thispass==4)  {numloc8instances  = numloc8instances  + 1;  break;}
			  if(thispass==5)  {numloc9instances  = numloc9instances  + 1;  break;}
			  if(thispass==6)  {numloc10instances = numloc10instances + 1;  break;}
			  if(thispass==7)  {numloc11instances = numloc11instances + 1;  break;}
			  if(thispass==8)  {numloc12instances = numloc12instances + 1;  break;}
			  if(thispass==9)  {numloc13instances = numloc13instances + 1;  break;}
			  if(thispass==10) {numloc14instances = numloc14instances + 1;  break;}
			  if(thispass==11) {numloc15instances = numloc15instances + 1;  break;}
			  if(thispass==12) {numloc16instances = numloc16instances + 1;  break;}
			  if(thispass==13) {numloc17instances = numloc17instances + 1;  break;}
			  if(thispass==14) {numloc18instances = numloc18instances + 1;  break;}
			} 
		  } 
		} 
	  } 
	} 



    totalchecks = totalchecks + 
		          docheck1  + docheck2  + docheck3  + docheck4  + docheck5  + docheck6  +
		          docheck7  + docheck8  + docheck9  + docheck10 + docheck11 + docheck12 +
		          docheck13 + docheck14 + docheck15 + docheck16 + docheck17 + docheck18 +
		          docheck19 + docheck20 + docheck21 + docheck22 + docheck23 + docheck24 +
		          docheck25 + docheck26 + docheck27 + docheck28 + docheck29 + docheck30 +
				  docheck31 + docheck32 + docheck33 +
				  numlocinstances       + numloc2instances      + numloc3instances      +
				  numloc4instances      + numloc5instances      + numloc6instances      +
				  numloc7instances      + numloc8instances      + numloc9instances      +
				  numloc10instances     + numloc11instances     + numloc12instances     +
				  num_railway_gaw       + num_sidetrack_gaw     + numloc13instances     +
				  numloc14instances     + numloc15instances     + numloc16instances     +
				  numloc17instances     + numloc18instances     +
				  num_b1    + num_b2    + num_b3;


     if(totalchecks>0)
	{
	  
	  fprintf(newfile,"UNIQUE_CHECKS 1\n");
	  fprintf(newfile,"SHARESEG %d\n",totalchecks);
	  
	  j = 1;
	  
	  if(docheck1==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road or Cart Track sharing segments with line Ford that does not have TRS = \"Automotive\" or \"Null\"\"\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      j = j + 1;
	    }
	  
	  if(docheck2==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Trail sharing segments with line Ford that does not have TRS = \"Other\"\"\n",j);

	      j = j + 1;
	    }



	  if(docheck3==1)
	    {
	      
	      
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
	      if(ATTRTYPE_MGCP4())
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Road\" or \"Other\" but has some other TRS\"\n",j);
		  }
	      if(ATTRTYPE_MGCP3())
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Road\" or \"Road and Railway\" but has some other TRS\"\n",j);
		  }
	      else if(
			  (ATTRTYPE_NFDD())||
			  (ATTRTYPE_DFDD())
			  )
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Road\" but has some other TRS\"\n",j);
		  }

	      j = j + 1;
	    }
	  
	  
	  if(docheck4==1)
	    {
	      
	      
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
	      if(ATTRTYPE_MGCP4())
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Railway\" or \"Other\" but has some other TRS\"\n",j);
		  }
	      if(ATTRTYPE_MGCP3())
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Railway\" or \"Road and Railway\" but has some other TRS\"\n",j);
		  }
	      else if(
			  (ATTRTYPE_NFDD())||
			  (ATTRTYPE_DFDD())
			  )
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Railway\" but has some other TRS\"\n",j);
		  }
	      
	      j = j + 1;
	    }
	  
	  
	  if(docheck5==1)
	    {
	      
	      
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Canal\" but has some other TRS\"\n",j);
	      
	      j = j + 1;
	    }
	  
	  
	  if(docheck6==1)
	    {
	      
	      
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line TransportationSupportFeature which should have TRS \"Pedestrian\" but has some other TRS\"\n",j);
	      
	      j = j + 1;
	    }

	  if(docheck7==1)
	    {
	      
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);


	       

	       fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with WD1=-32767 sharing segments with line %s or Cart Track without WD1=-32767\"\n",
			  j,GetBridgeName(1),GetRoadName(1));

	      j = j + 1;
	    }


 	  if(docheck8==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	       

	       fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s or Cart Track with WD1=-32767 sharing segments with line %s without WD1=-32767\"\n",
			  j,GetRoadName(1),GetBridgeName(1));


	      j = j + 1;
	    }



	  if(docheck9==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
		  if((ATTRTYPE_MGCP3())||(ATTRTYPE_MGCP4()))
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road, %s, or %s without LOC \"Above Surface\" coincident with line %s\"\n",j,GetRailName(1),GetPipelineName(1),GetBridgeName(1));
		  }
	      else
		  {
		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road, %s, %s, or %s without LOC \"Above Surface\" coincident with line %s\"\n",j,GetRailName(1),GetSideTrackName(1),GetPipelineName(1),GetBridgeName(1));
		  }
	      j = j + 1;
	    }
	  


 	  if(docheck10==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road or Cart Track attributed ACC=\"Accurate\" sharing segment with line Tunnel\"\n",j);

	      j = j + 1;
	    }


 	  if(docheck11==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Ford sharing segment with line %s or %s\"\n",j,GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }


 	  if(docheck12==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track or Trail not assigned WTC = \"Fair-weather\" and not coincident with line Ford or Bridge sharing segments with line %s\"\n",j,GetRiverName(1));

	      j = j + 1;
	    }



	  if(docheck13==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",   j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",   j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n", j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n", j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);
	      
	      if(
             (ATTRTYPE_ANY_TDS()) ||
	         (ATTRTYPE_NFDD())||
	         (ATTRTYPE_DFDD())||
	         (ATTRTYPE_ANY_GGDM())
		    )
		{
		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road, %s, or %s without LOC \"%s\" and not inside area %s coincident with line Tunnel\"\n",j,GetRailName(1),GetSideTrackName(1),GetUnderGroundName(1),GetBuildingName(1));
		}
		else if((ATTRTYPE_MGCP3())||(ATTRTYPE_MGCP4()))
		{
		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road or %s without LOC \"%s\" and not inside area %s coincident with line Tunnel\"\n",j,GetRailName(1),GetUnderGroundName(1),GetBuildingName(1));
		}
	      else
		{
		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road, %s, or %s without LOC \"%s\" and not inside area %s coincident with line Tunnel\"\n",j,GetRailName(1),GetSideTrackName(1),GetUnderGroundName(1),GetBuildingName(1));
		}
	      j = j + 1;
	    }


 	  if(docheck14==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with FUN = \"Unknown\" sharing segments with %s different FUN\"\n",
			  j,GetBridgeName(1),GetRoadName(1));

	      j = j + 1;
	    }

 	  if(docheck15==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with FUN = \"Under Construction\" sharing segments with %s different FUN\"\n",
			  j,GetBridgeName(1),GetRoadName(1));

	      j = j + 1;
	    }

   	  if(docheck16==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with FUN = \"Fully Functional\" sharing segments with %s different FUN\"\n",
			  j,GetBridgeName(1),GetRoadName(1));

	      j = j + 1;
	    }

   	  if(docheck17==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s with FUN = \"Damaged\" sharing segments with %s different FUN\"\n",
			  j,GetBridgeName(1),GetRoadName(1));

	      j = j + 1;
	    }


   	  if(docheck18==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s, %s, %s, Causeway Structure, Cut, or Tunnel with TRS = \"Not Applicable\" coincident with a line %s, Cart Track, %s, %s, Aqueduct, Canal, Trail, or %s\"\n",
			  j,GetBridgeName(1),GetDamName(1),GetEmbankmentName(1),GetRoadName(1),GetRailName(1),GetSideTrackName(1),GetPipelineName(1));

	      j = j + 1;
	    }


   	  if(docheck19==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Road\" coincident with carried feature other than Road and Cart Track\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck20==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Railway\" coincident with carried feature other than Railway and Railway Sidetrack\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck21==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Pedestrian\" coincident with non-Pedestrian carried feature\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck22==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Pipeline\" coincident with non-Pipeline carried feature\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck23==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Canal\" coincident with non-Canal carried feature\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck24==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Aqueduct\" coincident with non-Aqueduct carried feature\"\n",j);

	      j = j + 1;
	    }


   	  if(docheck25==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Other\" sharing with %s or Cart Track but no other carried feature type\"\n",j,GetRoadName(1));

	      j = j + 1;
	    }

   	  if(docheck26==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Other\" sharing with %s or %s but no other carried feature type\"\n",j,GetRailName(1),GetSideTrackName(1));

	      j = j + 1;
	    }

   	  if(docheck27==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Other\" sharing with Trail but no other carried feature type\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck28==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Other\" sharing with %s but no other carried feature type\"\n",j,GetPipelineName(1));

	      j = j + 1;
	    }

   	  if(docheck29==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Other\" sharing with Canal but no other carried feature type\"\n",j);

	      j = j + 1;
	    }

   	  if(docheck30==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line transportation support feature with TRS = \"Other\" sharing with Aqueduct but no other carried feature type\"\n",j);

	      j = j + 1;
	    }


   	  if(docheck31==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Causeway Structure or Cut sharing with line Aqueduct, Canal, %s, %s, or %s without %s = \"On Surface\"\"\n",
			  j,GetRoadName(1),GetRailName(1),GetPipelineName(1),GetLOCName(1));

	      j = j + 1;
	    }


   	  if(docheck32==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s sharing with line Aqueduct, %s, or %s without %s = \"On Surface\"\"\n",
			  j,GetEmbankmentName(1),GetRoadName(1),GetRailName(1),GetLOCName(1));

	      j = j + 1;
	    }

   	  if(docheck33==1)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

	      fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s sharing with line %s or %s without %s = \"On Surface\"\"\n",
			  j,GetDamName(1),GetRoadName(1),GetRailName(1),GetLOCName(1));

	      j = j + 1;
	    }


	  sprintf(cleancode,"%s",GetRailName(1));

	  for(k=0;k<num_railway_gaw;k++)
	  {
        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (GAW=%lf) overlapping line railway Bridge with different WD1\"\n",
		  j,cleancode,GetGAW(cleancode,"(LINE)","GAW",k));
        j = j + 1;
	  }

  	  sprintf(cleancode,"%s",GetSideTrackName(1));

	  for(k=0;k<num_sidetrack_gaw;k++)
	  {
        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
        fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

        fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (GAW=%lf) overlapping line railway Bridge with different WD1\"\n",
		  j,cleancode,GetGAW(cleancode,"(LINE)","GAW",k));
        j = j + 1;
	  }



	  for(k=0;k<num_b1;k++)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (WID=%.3lf) overlapping line Road with larger %s\"\n",
			  j,GetBridgeName(1),global_bridge_wid[k],GetWD1Name());
	      j = j + 1;
	    }

	  for(k=0;k<num_b2;k++)
	    {
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (WID=%.3lf) overlapping line Cart Track with larger %s\"\n",
			  j,GetBridgeName(1),global_bridge_wid[k],GetWD1Name());
	      j = j + 1;
	    }



	  if(num_b3>0)  
	  {
         

	    if(bridge_wd1_val_found < (road_wd1_val_found+cart_wd1_val_found) )
		{
          for(k=0;k<bridge_wd1_val_found;k++)
		  {
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line %s (%s=%.3lf) overlapping line Road or Cart Track with different %s\"\n",
			    j,GetBridgeName(1),GetWD1Name(),global_bridge_wd1[k],GetWD1Name());
	        j = j + 1;
		  }
		}
	    else
		{
          for(k=0;k<road_wd1_val_found;k++)
		  {
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
            fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Road (%s=%.3lf) overlapping line %s with different %s\"\n",
			    j,GetWD1Name(),global_road_wd1[k],GetBridgeName(1),GetWD1Name());
	        j = j + 1;
		  }


          for(k=0;k<cart_wd1_val_found;k++)
		  {
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
            fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	        fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		    fprintf(newfile,"SHARESEG %d ANNOTATION \"Line Cart Track (%s=%.3lf) overlapping line %s with different WD1\"\n",
                j,GetWD1Name(),global_cart_wd1[k],GetBridgeName(1));
	        j = j + 1;
		  }
		}
	  }



      for(i=0;i<MGCP_num_ISL;i++)
	  {

        if(MGCP_ISL_CODES[i]<0)
		{
          continue;
		} 

        if((MGCP_ISL_USE[i]==1)||(MGCP_ISL_USE[i]==3))
		{
		  

		  fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line error: %s\"\n",j,GetECCLabel(MGCP_ISL_CODES[i]));

	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  j = j + 1;
		}


        if((MGCP_ISL_USE[i]==2)||(MGCP_ISL_USE[i]==3))
		{
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	      fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		  fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line: %s\"\n",j,GetECCLabel(MGCP_ISL_CODES[i]));
		
	      fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	      fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		  j = j + 1;
		}
	  }


	  if(numlocinstances>0)
	  {

		

	    for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
		{ 

	      if(primaryCcodes[thiscomplex/3]<0)
		  { 
		    

		    
            continue;
		  } 


	      if(thiscomplex>0)
		  { 
		    if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
			{ 
		      
		      continue;
			} 
		  } 


          


	      

	      

          for(i=0;i<NumberOfModels;i++)
		  {   
            if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),MGCP_C_ISL[thiscomplex])))
			{    
	          

	          for(jj=0;jj<MdlNames[i].numattributes;jj++)
			  {    	    
	            thisindex = MdlNames[i].AttrIndices[jj];
	            thatindex = MdlNames[i].ValueIndices[jj];

	            if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"LOC"))
				{   
		          
		          if(MdlNames2[thisindex].values[thatindex].type==3)
				  {   
			        locval = (int)MdlNames2[thisindex].values[thatindex].value;

			        

					

                    for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
					{ 
                      if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
					  { 
					    
					    break;
					  } 

				      if(secondaryCcodes[thiscomplex2/3]<0)
					  { 
					     
					     continue;
					  } 

                      if(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 LOC = FEATURE2 LOC"))
					  {   
		                 
		                 continue;
					  }    


				      


				      

				      

	                  if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",
					      GetEELabel(MdlNames2[thisindex].code,(int)MdlNames2[thisindex].values[thatindex].value)))
					  { 
	                    fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
	                    fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
	                    fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
	                    fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
	                    fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
	                    fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
	                    fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
	                    fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

		                fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s with same LOC (%s) as other feature type(s)\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetEELabel(MdlNames2[thisindex].code,(int)MdlNames2[thisindex].values[thatindex].value));
		
	                    fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
	                    fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
	                    fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		                j = j + 1;

						break;  
					  } 
					} 
				  }   
				}   
			  }   
			} 
		  } 
		} 
	  } 






	if(numloc2instances>0)
	{
	  for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	  { 
	    if(primaryCcodes[thiscomplex/3]<0)
		{  
		  
          continue;
		}  

	    if(thiscomplex>0)
		{  
		  if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		  {  
		    
		    continue;
		  }  
		}   

	    if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		{  
          for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		  { 
            if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			{ 
			  
			  break;
			}  

		    if(secondaryCcodes[thiscomplex2/3]<0)
			{ 
			  
			  continue;
			} 

            if(strncmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT ",13))
			{    
		      continue;
			}     

		    if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			{ 

               fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
               fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
               fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
               fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

	           fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s sharing with other feature type(s) without specified attribute values\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
		
               fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
               fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
               fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		       j = j + 1;
			   break;  

			}
		  } 
		} 
	  } 
	} 








	if(numloc3instances>0)
	{
	  for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	  { 
	    if(primaryCcodes[thiscomplex/3]<0)
		{  
		  
          continue;
		}  

	    if(thiscomplex>0)
		{  
		  if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		  {  
		    
		    continue;
		  }  
		}   

	    if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		{  
          for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		  { 
            if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			{ 
			  
			  break;
			}  

		    if(secondaryCcodes[thiscomplex2/3]<0)
			{ 
			  
			  continue;
			} 

            if(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=40,45"))
			{    
		      continue;
			}     

		    if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			{ 

               fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
               fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
               fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
               fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
               fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

	           fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"%s\" or \"%s\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetUnderGroundName(1),GetAboveName(1));
		
               fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
               fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
               fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		       j = j + 1;
			   break;  

			}
		  } 
		} 
	  } 
	} 






    if((numloc4instances+numloc5instances+numloc6instances+numloc7instances+numloc8instances+
		numloc9instances+numloc10instances+numloc11instances+numloc12instances+
		numloc13instances+numloc14instances+numloc15instances+numloc16instances+
		numloc17instances+numloc18instances)>0)
	{
	  for(thispass=0;thispass<15;thispass++)
	  { 
	    for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
		{  
	      if(primaryCcodes[thiscomplex/3]<0)
		  {   
		    
            continue;
		  }   

	      if(thiscomplex>0)
		  {   
		    if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
			{   
		      
		      continue;
			}   
		  }    


	      if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		  {   
            for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
			{  
              if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			  {  
			    
			    break;
			  }   

		      if(secondaryCcodes[thiscomplex2/3]<0)
			  {  
			    
			    continue;
			  }  

              if((thispass==0)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=40")))
			  {     
		        continue;
			  }      
              if((thispass==1)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=3")))
			  {     
		        continue;
			  }      
              if((thispass==2)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT CDA=1001")))
			  {     
		        continue;
			  }      
              if((thispass==3)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT WCC=3")))
			  {     
		        continue;
			  }      
              if((thispass==4)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=3 OR FEATURE2 NOT FUN=3")))
			  {     
		        continue;
			  }      
              if((thispass==5)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"NOT COINCIDENT WITH FEATURE1 RIR=1001 OR FEATURE1 FUN=4")))
			  {     
		        continue;
			  }    
			  
              if((thispass==6)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=17,23,46")))
			  { 
		        continue;
			  } 
              if((thispass==7)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=23,46")))
			  { 
		        continue;
			  } 
              if((thispass==8)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=23")))
			  { 
		        continue;
			  } 
              if((thispass==9)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 LOC NOT=44,45")))
			  { 
		        continue;
			  } 
              if((thispass==10)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=4")))
			  {     
		        continue;
			  }      
              if((thispass==11)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=44 OR FEATURE2 NOT TRS=2")))
			  {    
		        continue;
			  }      
              if((thispass==12)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=45")))
			  {    
		        continue;
			  }      
              if((thispass==13)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT SEP=1001 or MES=1001")))
			  {    
		        continue;
			  }      
              if((thispass==14)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=44")))
			  {    
		        continue;
			  }      









		      if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			  {  

                fprintf(newfile,"SHARESEG %d C_PRIMARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_PRIMARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_SECONDARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_SECONDARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_LINE on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_LINE off\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_SOME_AREA on\n",j);
                fprintf(newfile,"SHARESEG %d C_TERTIARY C_ALL_AREA off\n",j);

				if(thispass==0)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"%s\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetUnderGroundName(1));
				}
				else if(thispass==1)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with FUN \"Destroyed\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==2)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with CDA \"1001\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==3)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with WCC \"Gorge\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==4)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with FUN \"Destroyed\" with Bridge Span except with FUN \"Destroyed\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==5)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s with other features except when coincident with AN010 RIR=1001 OR AN010 FUN=4\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}

				if(thispass==6)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"%s\", \"%s\", or \"Above Waterbody Bottom\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetLOC17Name(1),GetLOC23Name(1));
				}
				if(thispass==7)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"%s\" or \"Above Waterbody Bottom\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetLOC23Name(1));
				}
				if(thispass==8)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"%s\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetLOC23Name(1));
				}
				if(thispass==9)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"On Surface\" or \"%s\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]),
							GetAboveName(1));
				}
				else if(thispass==10)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with FUN \"Dismantled\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==11)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"On Surface\" with Embankment except with TRS \"Aqueduct\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==12)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"Above Surface\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==13)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with SEP \"Present\" or MES \"With Median\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}
				else if(thispass==14)
				{
	              fprintf(newfile,"SHARESEG %d ANNOTATION \"Illogical Shared Line (complex): %s except with LOC \"On Surface\"\"\n",
							j,GetECCLabel(primaryCcodes[thiscomplex/3]));
				}

                fprintf(newfile,"SHARESEG %d TOLERANCE_PRIMARY 0.001\n",j);
                fprintf(newfile,"SHARESEG %d TOLERANCE_SECONDARY 0.1\n",j);
                fprintf(newfile,"SHARESEG %d TOLERANCE_TERTIARY -1.0\n",j);

		        j = j + 1;
			    break;  
			  }  
			} 
		  } 
		} 
	  } 
	} 











	  fprintf(newfile,"ATTRATTRATTR\n");
	  	  
	  j = 1;
	  
	  
	  if(docheck1==1)
	    {

          fprintf(newfile,"SHARESEG %d PRIMARY Ford ANY on C_LINE\n",j);

	      if(lnfordTRS_AUTO_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Ford TransportationSystemType LABEL Automotive off C_LINE\n",j);
		  }
	      if(lnfordTRS_NULL_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Ford TransportationSystemType LABEL NullReserved off C_LINE\n",j);
		  }
 
		  if(line_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_ctrack_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);
		  }

	      j = j + 1;
	    }

	  
	  if(docheck2==1)
	    {

          fprintf(newfile,"SHARESEG %d PRIMARY Ford ANY on C_LINE\n",j);

	      if(lnfordTRS_OTHER_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Ford TransportationSystemType LABEL Other off C_LINE\n",j);
		  }
	      if(lnfordTRS_NULL_found>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY Ford TransportationSystemType LABEL NullReserved off C_LINE\n",j);
		  }
		  if(line_trail_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);
		  }

	      j = j + 1;
	    }
	  
	  
	  
	  if(docheck3==1)
	    {
	      if(line_bridge_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		  } 
	      if(line_cut_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Cut ANY on C_LINE\n",j);
		  } 
	      if(line_causeway_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Causeway ANY on C_LINE\n",j);
		  } 
	      if(line_dam_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetDamName(0));
		  } 
	      if(line_embankment_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  } 
	      if(line_tunnel_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Tunnel ANY on C_LINE\n",j);
		  } 


   	      if(line_13bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Road off C_LINE\n",j,GetBridgeName(0));}
      
	      if(line_13cutfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL Road off C_LINE\n",j);}
	      
	      if(line_13causewayfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Causeway TransportationSystemType LABEL Road off C_LINE\n",j);}
	      
	      if(line_13damfound>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Road off C_LINE\n",      j,GetDamName(0));}

	      if(line_13embankmentfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Road off C_LINE\n",j,GetEmbankmentName(0));}

	      if(line_13tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL Road off C_LINE\n",j);}


   	      if(line_14bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL RoadandRailway off C_LINE\n",j,GetBridgeName(0));}
      
	      if(line_14cutfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL RoadandRailway off C_LINE\n",j);}
	      
	      if(line_14causewayfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Causeway TransportationSystemType LABEL RoadandRailway off C_LINE\n",j);}
	      
	      if(line_14damfound>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL RoadandRailway off C_LINE\n",      j,GetDamName(0));}

	      if(line_14embankmentfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL RoadandRailway off C_LINE\n",j,GetEmbankmentName(0));}

	      if(line_14tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL RoadandRailway off C_LINE\n",j);}


		  if(ATTRTYPE_MGCP4())
		  {
   	        if(line_999bridgefound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Other off C_LINE\n",j,GetBridgeName(0));}
      
	        if(line_999cutfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL Other off C_LINE\n",j);}
	      
	        if(line_999causewayfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Causeway TransportationSystemType LABEL Other off C_LINE\n",j);}
	      
	        if(line_999damfound>0)
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Other off C_LINE\n",      j,GetDamName(0));}

	        if(line_999embankmentfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Other off C_LINE\n",j,GetEmbankmentName(0));}

	        if(line_999tunnelfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL Other off C_LINE\n",j);}
		  }



	      if(line_ctrack_found>0)
		  {
		   fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);
		  } 
	      if(line_road_found>0)
		  { 
	       fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  } 
	      
	      j = j + 1;
	    }
	  
	  
	  
	  
	  if(docheck4==1)
	    {
	      if(line_bridge_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		  } 
	      if(line_cut_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Cut ANY on C_LINE\n",j);
		  } 
	      if(line_causeway_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Causeway ANY on C_LINE\n",j);
		  } 
	      if(line_dam_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetDamName(0));
		  } 
	      if(line_embankment_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  } 
	      if(line_tunnel_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Tunnel ANY on C_LINE\n",j);
		  } 
	      	      
   	      if(line_12bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Railway off C_LINE\n",j,GetBridgeName(0));}
      
	      if(line_12cutfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL Railway off C_LINE\n",j);}
	      
	      if(line_12causewayfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Causeway TransportationSystemType LABEL Railway off C_LINE\n",j);}
	      
	      if(line_12damfound>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Railway off C_LINE\n",      j,GetDamName(0));}

	      if(line_12embankmentfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Railway off C_LINE\n",j,GetEmbankmentName(0));}

	      if(line_12tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL Railway off C_LINE\n",j);}
	      	      


   	      if(line_14bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL RoadandRailway off C_LINE\n",j,GetBridgeName(0));}
      
	      if(line_14cutfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL RoadandRailway off C_LINE\n",j);}
	      
	      if(line_14causewayfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Causeway TransportationSystemType LABEL RoadandRailway off C_LINE\n",j);}
	      
	      if(line_14damfound>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL RoadandRailway off C_LINE\n",      j,GetDamName(0));}

	      if(line_14embankmentfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL RoadandRailway off C_LINE\n",j,GetEmbankmentName(0));}

	      if(line_14tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL RoadandRailway off C_LINE\n",j);}


		  if(ATTRTYPE_MGCP4())
		  {
   	        if(line_999bridgefound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Other off C_LINE\n",j,GetBridgeName(0));}
      
	        if(line_999cutfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL Other off C_LINE\n",j);}
	      
	        if(line_999causewayfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Causeway TransportationSystemType LABEL Other off C_LINE\n",j);}
	      
	        if(line_999damfound>0)
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Other off C_LINE\n",      j,GetDamName(0));}

	        if(line_999embankmentfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Other off C_LINE\n",j,GetEmbankmentName(0));}

	        if(line_999tunnelfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL Other off C_LINE\n",j);}
		  }


	      if(line_sidetrack_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		}
	      
	      if(line_rail_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));
		}
	      
	      j = j + 1;
	    }
	  

	  
	  
	  if(docheck5==1)
	    {
	      
	      if(line_bridge_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		  } 
	      if(line_cut_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Cut ANY on C_LINE\n",j);
		  } 
	      if(line_dam_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetDamName(0));
		  } 
	      if(line_embankment_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		  } 
	      if(line_tunnel_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Tunnel ANY on C_LINE\n",j);
		  } 


   	      if(line_5bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Canal off C_LINE\n",j,GetBridgeName(0));}
      
	      if(line_5cutfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL Canal off C_LINE\n",j);}
	      	      
	      if(line_5damfound>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Canal off C_LINE\n",      j,GetDamName(0));}

	      if(line_5embankmentfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Canal off C_LINE\n",j,GetEmbankmentName(0));}

	      if(line_5tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL Canal off C_LINE\n",j);}

	      
	      
	      fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);
	      
	      
	      j = j + 1;
	    }
	  
	  
	  
	  
	  if(docheck6==1)
	    {

		  if(ATTRTYPE_MGCP4())
		  {
	        if(line_cut_found==1)
			{ 
	          fprintf(newfile,"SHARESEG %d PRIMARY Cut ANY on C_LINE\n",j);
			}  
	        if(line_dam_found==1)
			{ 
	          fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetDamName(0));
			}  
	        if(line_embankment_found==1)
			{ 
	          fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
			}  

	        if(line_9cutfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL Pedestrian off C_LINE\n",j);}

		    if(line_9damfound>0)
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Pedestrian off C_LINE\n",      j,GetDamName(0));}

	        if(line_9embankmentfound>0)     
			{fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Pedestrian off C_LINE\n",j,GetEmbankmentName(0));}

		  }


	      if(line_bridge_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		  } 
	      if(line_tunnel_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY Tunnel ANY on C_LINE\n",j);
		  } 

   	      if(line_9bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL Pedestrian off C_LINE\n",j,GetBridgeName(0));}
	      	      
	      if(line_9tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL Pedestrian off C_LINE\n",j);}

	      
	      fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);
	      
	      j = j + 1;
	    }


	  if(docheck7>0)
	  {

		if(line_bridge_wd1_32767>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE -32767.0 -32767.0 on C_LINE\n",
			  j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))));
		}

		if(line_road_found>0)
		{
		  if(road_wd1_val_found>0)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE 0.0 900.0 on C_LINE\n",
			  j,GetRoadName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))));
		  } 
		}

		if(line_ctrack_found>0)
		{
          if(cart_wd1_val_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s RANGE 0.0 900.0 on C_LINE\n",
			  j,MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))));
		  } 
		}

		j = j + 1;
	  }



      if(docheck8==1)
	  {

		if(line_road_wd1_32767>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE -32767.0 -32767.0 on C_LINE\n",
			  j,GetRoadName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))));
		}

		if(line_ctrack_wd1_32767>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s RANGE -32767.0 -32767.0 on C_LINE\n",
			  j,MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))));
		}

		if(line_bridge_found>0)
		{
          if(bridge_wd1_val_found>0)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE 0.0 900.0 on C_LINE\n",
			  j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))));
		  } 
		}

		j = j + 1;
	  }



	  if(docheck9==1)
	    {
	      if(line_road_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
          if(line_rail_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
          if(line_pipe_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  }

	      if(line_bridge_found==1)
		  { 
	        fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		  } 

          if(line_loc45road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL AboveSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
          if(line_loc45rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL AboveSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
          if(line_loc45pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL AboveSurface off C_LINE\n",j,GetPipelineName(0),GetLOCName(0));
		  }

	      j = j + 1;
	    }



      if(docheck10==1)
	  {
        if(line_roadACC1found>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL Accurate on C_LINE\n",j,GetRoadName(0),GetACCCode(0));
		}
        if(line_ctrackACC1found>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s LABEL Accurate on C_LINE\n",j,GetACCCode(0));
		}

         fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);
         j = j + 1;
	  }


      if(docheck11==1)
	  {
        fprintf(newfile,"SHARESEG %d PRIMARY Ford ANY on C_LINE\n",j);

 		if(line_rail_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));
		}
		if(line_sidetrack_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		}
        j = j + 1;
	  }

	  if(docheck12==1)
	  {
		
		if(line_ctrack_found>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY CartTrack ANY on C_LINE\n",j);

          if(line_wtc2track>0)
		  { 
            fprintf(newfile,"SHARESEG %d PRIMARY CartTrack RoadWeatherRestriction LABEL Fairweather off C_LINE\n",j);
		  } 
		}

		if(line_trail_found>0)
		{
          fprintf(newfile,"SHARESEG %d PRIMARY Trail ANY on C_LINE\n",j);

          if(line_wtc2trail>0)
		  { 
            fprintf(newfile,"SHARESEG %d PRIMARY Trail RoadWeatherRestriction LABEL Fairweather off C_LINE\n",j);
		  } 
		}



        fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRiverName(0));

        if(line_ford_found>0)
		{
          fprintf(newfile,"SHARESEG %d TERTIARY Ford ANY on C_LINE\n",j);
		}
        if(line_bridge_found>0)
		{
          fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetBridgeName(0));
		}
        j = j + 1;
	  } 


	  if(docheck13==1)
	    {
	      if(line_road_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
          if(line_rail_found==1)
		  {
	        fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }

	      if(line_tunnel_found==1)
		  { 
	        fprintf(newfile,"SHARESEG %d SECONDARY Tunnel ANY on C_LINE\n",j);
		  } 

	      
	      if(line_loc40road>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRoadName(0),GetLOCName(0),GetUnderGroundName(0));
		  }
          if(line_loc40rail>0)
		  {
            fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,GetRailName(0),GetLOCName(0),GetUnderGroundName(0));
		  }

		  if(area_building_found>0)
		  {
            fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_AREA\n",j,GetBuildingName(0));
		  }

	      j = j + 1;
	    }


      if(docheck14==1)
	  {
        fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL Unknown on C_LINE\n",j,GetBridgeName(0));

		if(line_road_found>0)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

          if(line_fun0_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s ConditionofFacility LABEL Unknown off C_LINE\n",j,GetRoadName(0));
		  }
		}
        j = j + 1;
	  }

      if(docheck15==1)
	  {
        fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL UnderConstruction on C_LINE\n",j,GetBridgeName(0));

		if(line_road_found>0)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

          if(line_fun1_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s ConditionofFacility LABEL UnderConstruction off C_LINE\n",j,GetRoadName(0));
		  }
		}
        j = j + 1;
	  }

      if(docheck16==1)
	  {
        fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL FullyFunctional on C_LINE\n",j,GetBridgeName(0));

		if(line_road_found>0)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

          if(line_fun6_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s ConditionofFacility LABEL FullyFunctional off C_LINE\n",j,GetRoadName(0));
		  }
		}
        j = j + 1;
	  }

      if(docheck17==1)
	  {
        fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL Damaged on C_LINE\n",j,GetBridgeName(0));

		if(line_road_found>0)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

          if(line_fun13_road_found>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s ConditionofFacility LABEL Damaged off C_LINE\n",j,GetRoadName(0));
		  }
		}
        j = j + 1;
	  }


	  if(docheck18==1)
	    {

   	      if(line_998bridgefound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL NotApplicable on C_LINE\n",j,GetBridgeName(0));}
      
	      if(line_998cutfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL NotApplicable on C_LINE\n",j);}
	      	      
	      if(line_998damfound>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL NotApplicable on C_LINE\n",j,GetDamName(0));}

	      if(line_998tunnelfound>0)     
		  {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL NotApplicable on C_LINE\n",j);}

          if(line_causewaystrTRS[TRS998]>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL NotApplicable on C_LINE\n",j);}

          if(line_embankmentTRS[TRS998]>0)
		  {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL NotApplicable on C_LINE\n",j,GetEmbankmentName(0));}


   	      if(line_road_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));
		  }
		  if(line_rail_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));
		  }
		  if(line_sidetrack_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));
		  }
		  if(line_ctrack_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);
		  }
		  if(line_trail_found>0)
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);
		  }
	      if(line_aqueduct_found==1)
		  {
		    fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);
		  } 
	      if(line_canal_found==1)
		  { 
		    fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);
		  }
		  if(line_pipe_found>0)
		  { 
		   fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		  } 

	      j = j + 1;
	    }
	  
	  

	  if(docheck19==1)
	    {
		  if(line_bridgeTRS     [TRS13] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS13NAME));}
          if(line_causewaystrTRS[TRS13] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS13NAME));}
          if(line_tunnelTRS     [TRS13] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS13NAME));}
          if(line_damTRS        [TRS13] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS13NAME));}
          if(line_embankmentTRS [TRS13] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS13NAME));}
          if(line_cutTRS        [TRS13] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS13NAME));}

	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck20==1)
	    {
		  if(line_bridgeTRS     [TRS12] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS12NAME));}
          if(line_causewaystrTRS[TRS12] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS12NAME));}
          if(line_tunnelTRS     [TRS12] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS12NAME));}
          if(line_damTRS        [TRS12] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS12NAME));}
          if(line_embankmentTRS [TRS12] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS12NAME));}
          if(line_cutTRS        [TRS12] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS12NAME));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck21==1)
	    {
		  if(line_bridgeTRS     [TRS9] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS9NAME));}
          if(line_causewaystrTRS[TRS9] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS9NAME));}
          if(line_tunnelTRS     [TRS9] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS9NAME));}
          if(line_damTRS        [TRS9] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS9NAME));}
          if(line_embankmentTRS [TRS9] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS9NAME));}
          if(line_cutTRS        [TRS9] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS9NAME));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck22==1)
	    {
		  if(line_bridgeTRS     [TRS10] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS10NAME));}
          if(line_causewaystrTRS[TRS10] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS10NAME));}
          if(line_tunnelTRS     [TRS10] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS10NAME));}
          if(line_damTRS        [TRS10] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS10NAME));}
          if(line_embankmentTRS [TRS10] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS10NAME));}
          if(line_cutTRS        [TRS10] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS10NAME));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck23==1)
	    {
		  if(line_bridgeTRS     [TRS5] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS5NAME));}
          if(line_causewaystrTRS[TRS5] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS5NAME));}
          if(line_tunnelTRS     [TRS5] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS5NAME));}
          if(line_damTRS        [TRS5] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS5NAME));}
          if(line_embankmentTRS [TRS5] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS5NAME));}
          if(line_cutTRS        [TRS5] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS5NAME));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck24==1)
	    {
		  if(line_bridgeTRS     [TRS2] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS2NAME));}
          if(line_causewaystrTRS[TRS2] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS2NAME));}
          if(line_tunnelTRS     [TRS2] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS2NAME));}
          if(line_damTRS        [TRS2] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS2NAME));}
          if(line_embankmentTRS [TRS2] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS2NAME));}
          if(line_cutTRS        [TRS2] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS2NAME));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);}

	      j = j + 1;
	    }



	  if(docheck25==1)
	    {
		  if(line_bridgeTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS999NAME));}
          if(line_causewaystrTRS[TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_tunnelTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_damTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS999NAME));}
          if(line_embankmentTRS [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS999NAME));}
          if(line_cutTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d SECONDARY CartTrack ANY on C_LINE\n",j);}

	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck26==1)
	    {
		  if(line_bridgeTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS999NAME));}
          if(line_causewaystrTRS[TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_tunnelTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_damTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS999NAME));}
          if(line_embankmentTRS [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS999NAME));}
          if(line_cutTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}

		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck27==1)
	    {
		  if(line_bridgeTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS999NAME));}
          if(line_causewaystrTRS[TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_tunnelTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_damTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS999NAME));}
          if(line_embankmentTRS [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS999NAME));}
          if(line_cutTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}

	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Trail ANY on C_LINE\n",j);}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck28==1)
	    {
		  if(line_bridgeTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS999NAME));}
          if(line_causewaystrTRS[TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_tunnelTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_damTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS999NAME));}
          if(line_embankmentTRS [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS999NAME));}
          if(line_cutTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}

		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }

	  if(docheck29==1)
	    {
		  if(line_bridgeTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS999NAME));}
          if(line_causewaystrTRS[TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_tunnelTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_damTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS999NAME));}
          if(line_embankmentTRS [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS999NAME));}
          if(line_cutTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}

	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);}

	      j = j + 1;
	    }


	  if(docheck30==1)
	    {
		  if(line_bridgeTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS999NAME));}
          if(line_causewaystrTRS[TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_tunnelTRS     [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}
          if(line_damTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS999NAME));}
          if(line_embankmentTRS [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS999NAME));}
          if(line_cutTRS        [TRS999] >0) {fprintf(newfile,"SHARESEG %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS999NAME));}

		  if(line_aqueduct_found==1){fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);}

          if(line_road_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));}
          if(line_ctrack_found>0)   {fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);}
	      if(line_trail_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);}
		  if(line_rail_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRailName(0));}
		  if(line_sidetrack_found>0){fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetSideTrackName(0));}
		  if(line_pipe_found>0)     {fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));}
	      if(line_canal_found==1)   {fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);}

	      j = j + 1;
	    }




	  
	  
	  


	  if(docheck31==1)
	  {
		if(line_causeway_struct_found>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY CausewayStructure ANY on C_LINE\n",j);
		}
		if(line_cut_found>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY Cut ANY on C_LINE\n",j);
		}

	    if(line_canal_found==1)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY Canal ANY on C_LINE\n",j);

	      if(line_loc44canal>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Canal %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0));
		  }
		}

		if(line_aqueduct_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);

	      if(line_loc44aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0));
		  }
		}

	    if(line_road_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

	      if(line_loc44road>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
		}

		if(line_rail_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));

	      if(line_loc44rail>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
		}

		if(line_pipe_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetPipelineName(0));

	      if(line_loc44pipe>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetPipelineName(0),GetLOCName(0));
		  }
		}

	    j = j + 1;
	  }



	  if(docheck32==1)
	  {
		if(line_causeway_struct_found>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetEmbankmentName(0));
		}

		if(line_aqueduct_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct ANY on C_LINE\n",j);

	      if(line_loc44aqueduct>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY Aqueduct %s LABEL OnSurface off C_LINE\n",j,GetLOCName(0));
		  }
		}

	    if(line_road_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

	      if(line_loc44road>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
		}

		if(line_rail_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));

	      if(line_loc44rail>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
		}

	    j = j + 1;
	  }


	  if(docheck33==1)
	  {
		if(line_causeway_struct_found>0)
		{
		  fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,GetDamName(0));
		}


	    if(line_road_found==1)
		{
	      fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRoadName(0));

	      if(line_loc44road>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetRoadName(0),GetLOCName(0));
		  }
		}

		if(line_rail_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetRailName(0));

	      if(line_loc44rail>0)
		  {
            fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL OnSurface off C_LINE\n",j,GetRailName(0),GetLOCName(0));
		  }
		}
	    j = j + 1;
	  }








          if(num_railway_gaw>0)
          {
	    sprintf(cleancode,"%s",GetRailName(0));
	    sprintf(tempchar,"%s",MakeCleanCode(GetEACLabel(GetAttrIndex("GAW"))));
	    sprintf(cleancode2,"%s",MakeCleanCode(GetEACLabel(GetAttrIndex("WD1"))));
          }

	  for(k=0;k<num_railway_gaw;k++)
	  {
		dval = GetGAW(cleancode,"(LINE)","GAW",k);

	    fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  cleancode,
			  tempchar,
			  dval,dval);

		if(line_bridge_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

		  if(FindAttr(dval,FIND_REAL,"(LINE)",GetBridgeName(1),"WD1", "No Information"))
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf off C_LINE\n",
			  j,
			  GetBridgeName(0),
			  cleancode2,
			  dval,dval);
		  }
		  if(FindAttr(-32768.0,FIND_REAL,"(LINE)",GetBridgeName(1),"WD1", "No Information"))
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE -32768.0 -32768.0 off C_LINE\n",
			  j,
			  GetBridgeName(0),
			  cleancode2);
		  }
		}

	    if(line_road_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));
		}
	    if(line_ctrack_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);
		}
	    if(line_trail_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);
		}
		if(line_pipe_found>0)
		{
		  fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		}
		if(line_aqueduct_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);
		}
	    if(line_canal_found==1)
		{
		  fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);
		}
	    if(line_river_found==1)
		{ 
	      fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRiverName(0));
		} 

        j = j + 1;
	  }



          if(num_sidetrack_gaw>0)
          {
	    sprintf(cleancode,"%s",GetSideTrackName(0));
	    sprintf(tempchar,"%s",MakeCleanCode(GetEACLabel(GetAttrIndex("GAW"))));
	    sprintf(cleancode2,"%s",MakeCleanCode(GetEACLabel(GetAttrIndex("WD1"))));
          }

	  for(k=0;k<num_sidetrack_gaw;k++)
	  {
		dval = GetGAW(GetSideTrackName(1),"(LINE)","GAW",k);

	    fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  cleancode,
			  tempchar,
			  dval,dval);

		if(line_bridge_found>0)
		{
		  fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,GetBridgeName(0));

		  if(FindAttr(dval,FIND_REAL,"(LINE)",GetBridgeName(1),"WD1", "No Information"))
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf off C_LINE\n",
			  j,
			  GetBridgeName(0),
			  cleancode2,
			  dval,dval);
		  }
		  if(FindAttr(-32768.0,FIND_REAL,"(LINE)",GetBridgeName(1),"WD1", "No Information"))
		  {
	        fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE -32768.0 -32768.0 off C_LINE\n",
			  j,
			  GetBridgeName(0),
			  cleancode2);
		  }
		}

	    if(line_road_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRoadName(0));
		}
	    if(line_ctrack_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY CartTrack ANY on C_LINE\n",j);
		}
	    if(line_trail_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY Trail ANY on C_LINE\n",j);
		}
		if(line_pipe_found>0)
		{
		  fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetPipelineName(0));
		}
		if(line_aqueduct_found==1)
		{
	      fprintf(newfile,"SHARESEG %d TERTIARY Aqueduct ANY on C_LINE\n",j);
		}
	    if(line_canal_found==1)
		{
		  fprintf(newfile,"SHARESEG %d TERTIARY Canal ANY on C_LINE\n",j);
		}
	    if(line_river_found==1)
		{ 
	      fprintf(newfile,"SHARESEG %d TERTIARY %s ANY on C_LINE\n",j,GetRiverName(0));
		} 

        j = j + 1;
	  }













      if((num_b1+num_b2)>0)
	  {
	    for(i=0;i<bridge_wid_val_found;i++)
		{
		  if(road_wd1_val_found<=0)
		  {
            break;
		  }

	      if(WD1IsLarger(global_road_wd1[road_wd1_val_found-1],global_bridge_wid[i]))
		  {
			

			for(k=0;k<road_wd1_val_found;k++)
			{
			  if(WD1IsLarger(global_road_wd1[k],global_bridge_wid[i]))
			  {
				break;
			  }
			}

			if(k>=road_wd1_val_found)
			{
			   
			   printf("error: k %d/%d for road\n",k,road_wd1_val_found);
			   k = road_wd1_val_found - 1;
			}

			fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AQ040"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex("WID"))),
			  global_bridge_wid[i],global_bridge_wid[i]);


			fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AP030"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			  global_road_wd1[k],global_road_wd1[road_wd1_val_found-1]);

	        j = j + 1;
		  }
		}


		

	    for(i=0;i<bridge_wid_val_found;i++)
		{
		  if(cart_wd1_val_found<=0)
		  {
            break;
		  }

	      if(WD1IsLarger(global_cart_wd1[cart_wd1_val_found-1],global_bridge_wid[i]))
		  {
			

			for(k=0;k<cart_wd1_val_found;k++)
			{
			  if(WD1IsLarger(global_cart_wd1[k],global_bridge_wid[i]))
			  {
				break;
			  }
			}

			if(k>=cart_wd1_val_found)
			{
			   
			   printf("error: k %d/%d for road\n",k,cart_wd1_val_found);
			   k = cart_wd1_val_found - 1;
			}


			fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AQ040"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex("WID"))),
			  global_bridge_wid[i],global_bridge_wid[i]);


			fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			  j,
			  MakeCleanCode (GetECCLabel(GetCodeIndex("AP010"))),
			  MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			  global_cart_wd1[k],global_cart_wd1[cart_wd1_val_found-1]);

	        j = j + 1;
		  }
		}
	  }



	  if(num_b3>0) 
	  {
	    

        if(bridge_wd1_val_found < (road_wd1_val_found+cart_wd1_val_found) )
		{
          for(k=0;k<bridge_wd1_val_found;k++)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			   j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))),
			   global_bridge_wd1[k],global_bridge_wd1[k]);

            for(l=0;l<road_wd1_val_found;l++)
			{  
			  if(global_road_wd1[l]!=global_bridge_wd1[k])
			  {
		         fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			       j,GetRoadName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			       global_road_wd1[l],global_road_wd1[l]);
			  } 
			} 

            for(l=0;l<cart_wd1_val_found;l++)
			{ 
			  if(global_cart_wd1[l]!=global_bridge_wd1[k])
			  {
		        fprintf(newfile,"SHARESEG %d SECONDARY CartTrack %s RANGE %lf %lf on C_LINE\n",
			       j,MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			       global_cart_wd1[l],global_cart_wd1[l]);
			  } 
			}
	        j = j + 1;
		  } 
		} 
	    else
		{ 
          for(k=0;k<road_wd1_val_found;k++)
		  { 
		    fprintf(newfile,"SHARESEG %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",
			    j,GetRoadName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			    global_road_wd1[k],global_road_wd1[k]);

            for(l=0;l<bridge_wd1_val_found;l++)
			{ 
			  if(global_road_wd1[k]!=global_bridge_wd1[l])
			  { 
		         fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			        j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))),
			        global_bridge_wd1[l],global_bridge_wd1[l]);
			  } 
			}
	        j = j + 1;
		  } 

          for(k=0;k<cart_wd1_val_found;k++)
		  {
		    fprintf(newfile,"SHARESEG %d PRIMARY CartTrack %s RANGE %lf %lf on C_LINE\n",
			    j,MakeCleanCode2(GetEACLabel(GetAttrIndex(GetWD1Name()))),
			    global_cart_wd1[k],global_cart_wd1[k]);

            for(l=0;l<bridge_wd1_val_found;l++)
			{ 
			  if(global_cart_wd1[k]!=global_bridge_wd1[l])
			  { 
		        fprintf(newfile,"SHARESEG %d SECONDARY %s %s RANGE %lf %lf on C_LINE\n",
			       j,GetBridgeName(0),MakeCleanCode2(GetEACLabel(GetAttrIndex("WD1"))),
			       global_bridge_wd1[l],global_bridge_wd1[l]);
			  }
			} 
	        j = j + 1;
		  }
		} 
	  } 




      for(i=0;i<MGCP_num_ISL;i++)
	  {
        if(MGCP_ISL_CODES[i]<0)
		{
          continue;
		} 

        if((MGCP_ISL_USE[i]==1)||(MGCP_ISL_USE[i]==3))
		{
          

          sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(MGCP_ISL_CODES[i])));
          fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,cleancode);
		  for(k=0;k<MGCP_num_ISL;k++)
		  {
            if(MGCP_ISL_CODES[k]<0)
			{
              continue;
			}  

		    if(ATTRTYPE_MGCP3())
			{ 
              thisval = MGCP3_ISL_VALS[i][k];
			} 
		    else
			{ 
              thisval = MGCP4_ISL_VALS[i][k];
			} 

            if((thisval=='e')&&(linespresent[MGCP_ISL_CODES[k]]==1))  
			{
              sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(MGCP_ISL_CODES[k])));
              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,cleancode);
			}
		  }
		  j = j + 1;
		}

        if((MGCP_ISL_USE[i]==2)||(MGCP_ISL_USE[i]==3))
		{
          sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(MGCP_ISL_CODES[i])));
          fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,cleancode);
		  for(k=0;k<MGCP_num_ISL;k++)
		  {
            if(MGCP_ISL_CODES[k]<0)
			{
              continue;
			}  

		    if(ATTRTYPE_MGCP3())
			{ 
              thisval = MGCP3_ISL_VALS[i][k];
			} 
		    else
			{ 
              thisval = MGCP4_ISL_VALS[i][k];
			} 

            if((thisval=='x')&&(linespresent[MGCP_ISL_CODES[k]]==1)) 
			{
              sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(MGCP_ISL_CODES[k])));
              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,cleancode);
			}
		  }
		  j = j + 1;
		}
	  }






	  if(numlocinstances>0)
	  {

		

	    for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
		{ 

	      if(primaryCcodes[thiscomplex/3]<0)
		  { 
		    

		    
            continue;
		  } 


	      if(thiscomplex>0)
		  { 
		    if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
			{ 
		      
		      continue;
			} 
		  } 


          


	      

	      

          for(i=0;i<NumberOfModels;i++)
		  {   
            if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCCode(MdlNames[i].code),MGCP_C_ISL[thiscomplex])))
			{    
	          

	          for(jj=0;jj<MdlNames[i].numattributes;jj++)
			  {    	    
	            thisindex = MdlNames[i].AttrIndices[jj];
	            thatindex = MdlNames[i].ValueIndices[jj];

	            if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"LOC"))
				{   
		          
		          if(MdlNames2[thisindex].values[thatindex].type==3)
				  {   
			        locval = (int)MdlNames2[thisindex].values[thatindex].value;

			        

					

					primaryadded = 0;
					instancemade = 0;

                    for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
					{ 
                      if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
					  { 
					    
					    break;
					  } 

				      if(secondaryCcodes[thiscomplex2/3]<0)
					  { 
					     
					     continue;
					  } 

                      if(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 LOC = FEATURE2 LOC"))
					  {   
		                 
		                 continue;
					  }    


				      


				      

				      

	                  if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",
					      GetEELabel(MdlNames2[thisindex].code,(int)MdlNames2[thisindex].values[thatindex].value)))
					  { 

						

						if(primaryadded==0)
						{
                          fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s on C_LINE\n",j,
							MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
							GetLOCName(0),
                            MakeCleanCode2(GetEELabel(MdlNames2[thisindex].code,(int)MdlNames2[thisindex].values[thatindex].value)));

						  primaryadded = 1;
						  instancemade = 1;
						}

                        fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s on C_LINE\n",j,
					      MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
						  GetLOCName(0),
                          MakeCleanCode2(GetEELabel(MdlNames2[thisindex].code,(int)MdlNames2[thisindex].values[thatindex].value)));
					  } 

					} 

					if(instancemade==1)
					{
					  
					  j = j + 1;
					}

				  } 
				} 
			  } 
			} 
		  } 
		} 
	  } 








	if(numloc2instances>0)
	{
	  for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	  { 
	    if(primaryCcodes[thiscomplex/3]<0)
		{  
		  
          continue;
		}  

	    if(thiscomplex>0)
		{  
		  if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		  {  
		    
		    continue;
		  }  
		}
		
	    primaryadded = 0;
		instancemade = 0;

	    if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		{  
          for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		  { 
            if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			{ 
			  
			  break;
			}  

		    if(secondaryCcodes[thiscomplex2/3]<0)
			{ 
			  
			  continue;
			} 

            if(strncmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT ",13))
			{    
		      continue;
			}     

		    if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			{ 
              if(primaryadded==0)
			  {
	            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
				primaryadded = 1;
				instancemade = 1;
			  }

	          fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));



              if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT LOC=40,45"))
			  {
			    

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",GetUnderGroundName(1)))
				{ 
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetLOCName(0),
                    GetUnderGroundName(0));
				} 

			    

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",GetAboveName(1)))
				{ 
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetLOCName(0),
                    GetAboveName(0));
				}  

			  }

              else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT LOC=40"))
			  {
			    

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",GetUnderGroundName(1)))
				{ 
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetLOCName(0),
                    GetUnderGroundName(0));
				} 
			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT CDA=1001"))
			  {
				

                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"CDA","Covered"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Covered off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetCDACode(0));
				}
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"CDA","True"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL True off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetCDACode(0));
				}
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"CDA","TRUE"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL TRUE off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetCDACode(0));
				}

			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT HYP=2,4"))
			  {

                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),GetHYCCode(1),"Intermittent"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Intermittent off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetHYCCode(0));
				}

                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),GetHYCCode(1),"Dry"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Dry off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetHYCCode(0));
				}

			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT TRS=10"))
			  {
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"TRS","Pipeline"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Pipeline off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetTRS1Name(0));
				}
			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT TRS=13"))
			  {
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"TRS","Road"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Road off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetTRS1Name(0));
				}
			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT TRS=9"))
			  {
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"TRS","Pedestrian"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Pedestrian off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetTRS1Name(0));
				}
			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT TRS=999"))
			  {
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"TRS","Other"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Other off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetTRS1Name(0));
				}
			  }
              else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT LOC=17,23,46"))
			  {
			    

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",GetLOC17Name(1)))
				{  
                      fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				        GetLOCName(0),
                        GetLOC17Name(0));
				}  

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC",GetLOC23Name(1)))
				{  
                      fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				        GetLOCName(0),
                        GetLOC23Name(0));
				}  

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "LOC","Above Waterbody Bottom"))
				{  
                      fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL AboveWaterbodyBottom off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				        GetLOCName(0));
				}  

			  }
			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT CDA=1001 OR FEATURE1 NOT SEP=1001 OR MES=1001"))
			  {
				

                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"CDA","Covered"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Covered off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetCDACode(0));
				}
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"CDA","True"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL True off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetCDACode(0));
				}
                if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]),"CDA","TRUE"))
				{
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL TRUE off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
				    GetCDACode(0));
				}


			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "SEP","Present"))
				{  
                  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL Present off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),GetSEPName(0));
				}  

			   if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "MES","With Median"))
			   {  
                  fprintf(newfile,"SHARESEG %d PRIMARY %s MedianPresent LABEL WithMedian off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
			   }  

			  }

			  else if(!strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE2 NOT SEP=1001 OR MES=1001"))
			  {
			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "SEP","Present"))
				{  
                  fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Present off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),GetSEPName(0));
				}  

			   if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "MES","With Median"))
			   {  
                  fprintf(newfile,"SHARESEG %d SECONDARY %s MedianPresent LABEL WithMedian off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
			   }  
			  }

			  else
			  {
				 printf("Unhandled case (b) in ISL: %s\n",MGCP_C_ISL[thiscomplex2+2]);
			  }

			} 
		  } 

		  if(instancemade==1)
		  {
		    
		    j = j + 1;
		  }

		} 
	  } 
	} 










	if(numloc3instances>0)
	{
	  for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
	  { 
	    if(primaryCcodes[thiscomplex/3]<0)
		{  
		  
          continue;
		}  

	    if(thiscomplex>0)
		{  
		  if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
		  {  
		    
		    continue;
		  }  
		}
		
	    primaryadded = 0;
		instancemade = 0;

	    if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		{  
          for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
		  { 
            if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			{ 
			  
			  break;
			}  

		    if(secondaryCcodes[thiscomplex2/3]<0)
			{ 
			  
			  continue;
			} 

            if(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=40,45"))
			{    
		      continue;
			}     

		    if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			{ 
              if(primaryadded==0)
			  {
	            fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));


			    

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetUnderGroundName(1)))
				{ 
                  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				    GetLOCName(0),
                    GetUnderGroundName(0));
				} 

			    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetAboveName(1)))
				{  
                  fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				    MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				    GetLOCName(0),
                    GetAboveName(0));  
				}

				primaryadded = 1;
				instancemade = 1;
			  }

	          fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));

			} 
		  } 

		  if(instancemade==1)
		  {
		    
		    j = j + 1;
		  }

		} 
	  } 
	} 

















    if((numloc4instances+numloc5instances+numloc6instances+numloc7instances+numloc8instances+
		numloc9instances+numloc10instances+numloc11instances+numloc12instances+
		numloc13instances+numloc14instances+numloc15instances+numloc16instances+numloc17instances+numloc18instances)>0)
	{
	  for(thispass=0;thispass<15;thispass++)
	  { 
	    for(thiscomplex=0;thiscomplex<MGCP_num_c_ISL*3;thiscomplex=thiscomplex+3)
		{  
	      if(primaryCcodes[thiscomplex/3]<0)
		  {   
		    
            continue;
		  }   

	      if(thiscomplex>0)
		  {   
		    if(!strcmp(MGCP_C_ISL[thiscomplex],MGCP_C_ISL[thiscomplex-3]))
			{   
		      
		      continue;
			}   
		  }    

	      primaryadded = 0;
		  instancemade = 0;

	      if(linespresent[primaryCcodes[thiscomplex/3]]==1)
		  {   
            for(thiscomplex2=thiscomplex;thiscomplex2<MGCP_num_c_ISL*3;thiscomplex2=thiscomplex2+3)
			{  
              if(primaryCcodes[thiscomplex2/3]!=primaryCcodes[thiscomplex/3])
			  {  
			    
			    break;
			  }   

		      if(secondaryCcodes[thiscomplex2/3]<0)
			  {  
			    
			    continue;
			  }  

              if((thispass==0)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=40")))
			  {     
		        continue;
			  }      
              if((thispass==1)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=3")))
			  {     
		        continue;
			  }      
              if((thispass==2)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT CDA=1001")))
			  {     
		        continue;
			  }      
              if((thispass==3)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT WCC=3")))
			  {     
		        continue;
			  }      
              if((thispass==4)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=3 OR FEATURE2 NOT FUN=3")))
			  {     
		        continue;
			  }      
              if((thispass==5)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"NOT COINCIDENT WITH FEATURE1 RIR=1001 OR FEATURE1 FUN=4")))
			  {
		        continue;
			  }

              if((thispass==6)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=17,23,46")))
			  { 
		        continue;
			  } 
              if((thispass==7)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=23,46")))
			  { 
		        continue;
			  } 
              if((thispass==8)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=23")))
			  { 
		        continue;
			  } 
              if((thispass==9)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 LOC NOT=44,45")))
			  { 
		        continue;
			  }
              if((thispass==10)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT FUN=4")))
			  { 
		        continue;
			  }
              if((thispass==11)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=44 OR FEATURE2 NOT TRS=2")))
			  {     
		        continue;
			  }
              if((thispass==12)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=45")))
			  {    
		        continue;
			  }      
              if((thispass==13)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT SEP=1001 or MES=1001")))
			  {    
		        continue;
			  }      
              if((thispass==14)&&(strcmp(MGCP_C_ISL[thiscomplex2+2],"FEATURE1 NOT LOC=44")))
			  {    
		        continue;
			  }      



		      if(linespresent[secondaryCcodes[thiscomplex2/3]]==1)
			  {  

				if(thispass==0)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetUnderGroundName(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetUnderGroundName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				}
				else if(thispass==1)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "FUN","Destroyed"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL Destroyed off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				}
				else if(thispass==2)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

                    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]),"CDA","Covered"))
					{ 
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL Covered off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetCDACode(0));
					} 
                    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]),"CDA","True"))
					{ 
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL True off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetCDACode(0));
					} 
                    if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]),"CDA","TRUE"))
					{ 
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL TRUE off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetCDACode(0));
					} 

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));

				}
				else if(thispass==3)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "WCC","Gorge"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s WatercourseChannelType LABEL Gorge off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				}
				else if(thispass==4)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "FUN","Destroyed"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL Destroyed off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));

			      if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "FUN","Destroyed"))
				  {  
                    fprintf(newfile,"SHARESEG %d SECONDARY %s ConditionofFacility LABEL Destroyed off C_LINE\n",j,
				      MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				  }  
				} 



				else if(thispass==5)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetRailName(1), "RIR","In road"))
					{  
                      fprintf(newfile,"SHARESEG %d TERTIARY %s RailwayinRoad LABEL Inroad on C_LINE\n",j,GetRailName(1));
					}  

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetRailName(1), "FUN","Dismantled"))
					{  
                      fprintf(newfile,"SHARESEG %d TERTIARY %s ConditionofFacility LABEL Dismantled on C_LINE\n",j,GetRailName(1));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));

				} 


				else if(thispass==6)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetLOC17Name(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetLOC17Name(0));
					}  

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetLOC23Name(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetLOC23Name(0));
					}  

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC","Above Waterbody Bottom"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL AboveWaterbodyBottom off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 
				

				else if(thispass==7)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetLOC23Name(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetLOC23Name(0));
					}  

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC","Above Waterbody Bottom"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL AboveWaterbodyBottom off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 

				else if(thispass==8)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetLOC23Name(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetLOC23Name(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 

				else if(thispass==9)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC","On Surface"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0));
					}  

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetAboveName(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetAboveName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 

				else if(thispass==10)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "FUN","Dismantled"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s ConditionofFacility LABEL Dismantled off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 


				else if(thispass==11)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC","On Surface"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));

			      if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(secondaryCcodes[thiscomplex2/3]), "TRS","Aqueduct"))
				  {  
                    fprintf(newfile,"SHARESEG %d SECONDARY %s %s LABEL Aqueduct off C_LINE\n",j,
				      MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])),
					  GetTRS1Name(0));
				  }  
				} 



			  


				else if(thispass==12)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC",GetAboveName(1)))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL %s off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0),
                        GetAboveName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 

				else if(thispass==13)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "SEP","Present"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL Present off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),GetSEPName(0));
					}  

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "MES","With Median"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s MedianPresent LABEL WithMedian off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));
				} 

				else if(thispass==14)
				{
                  if(primaryadded==0)
				  { 
	                fprintf(newfile,"SHARESEG %d PRIMARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])));

			        

			        if(FindAttr(-1,FIND_ENUM,"(LINE)", GetECCLabel(primaryCcodes[thiscomplex/3]), "LOC","On Surface"))
					{  
                      fprintf(newfile,"SHARESEG %d PRIMARY %s %s LABEL OnSurface off C_LINE\n",j,
				        MakeCleanCode(GetECCLabel(primaryCcodes[thiscomplex/3])),
				        GetLOCName(0));
					}  

			        primaryadded = 1;
				    instancemade = 1;
				  } 

	              fprintf(newfile,"SHARESEG %d SECONDARY %s ANY on C_LINE\n",j,MakeCleanCode(GetECCLabel(secondaryCcodes[thiscomplex2/3])));

				} 



			  }  
			} 

		    if(instancemade==1)
			{ 
		      
		      j = j + 1;
			} 

		  } 
		} 
	  } 
	} 

	}
      else
	{
	  fprintf(newfile,"Data not available to specify this check\n");
	}
      fclose(newfile);
    }

	if(primaryCcodes!=NULL)
	{
      free(primaryCcodes);
	}
	if(secondaryCcodes!=NULL)
	{
      free(secondaryCcodes);
	}
}





int ExcludeRES(int rangeindex,char *value,int type)
{
  char theattr[100];

  


  


  if(type==1)
  {
    return 1;
  }


  

  if(  (!strcmp(value,"LOC=40"))   && (strstr(MGCP_ranges[rangeindex+8],"LOC=40")) ) 
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"LOC",GetUnderGroundName(1));
  }
  if(  (!strcmp(value,"CDA=1001")) && (strstr(MGCP_ranges[rangeindex+8],"CDA=1001")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"CDA","Covered");
  }
  if(  (!strcmp(value,"FUN=1"))    && (strstr(MGCP_ranges[rangeindex+8],"FUN=1-4")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"FUN","Under Construction");
  }
  if(  (!strcmp(value,"FUN=2"))    && (strstr(MGCP_ranges[rangeindex+8],"FUN=1-4")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"FUN","Abandoned");
  }
  if(  (!strcmp(value,"FUN=3"))    && (strstr(MGCP_ranges[rangeindex+8],"FUN=1-4")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"FUN","Destroyed");
  }
  if(  (!strcmp(value,"FUN=4"))    && (strstr(MGCP_ranges[rangeindex+8],"FUN=1-4")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"FUN","Dismantled");
  }

  if(  (!strcmp(value,"TRS=0"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Unknown");
  }
  if(  (!strcmp(value,"TRS=1"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Aeronautical");
  }
  if(  (!strcmp(value,"TRS=2"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Aqueduct");
  }
  if(  (!strcmp(value,"TRS=5"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Canal");
  }
  if(  (!strcmp(value,"TRS=7"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Maritime");
  }
  if(  (!strcmp(value,"TRS=9"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Pedestrian");
  }
  if(  (!strcmp(value,"TRS=10"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Pipeline");
  }
  if(  (!strcmp(value,"TRS=998"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Not Applicable");
  }
  if(  (!strcmp(value,"TRS=999"))    && (strstr(MGCP_ranges[rangeindex+8],"TRS = 0, 1, 2, 5, 7, 9, 10, 998, or 999")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"TRS","Other");
  }
  if(  (!strcmp(value,"SDT=0"))    && (strstr(MGCP_ranges[rangeindex+8],"SDT=0,2,7")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"SDT","Unknown");
  }
  if(  (!strcmp(value,"SDT=2"))    && (strstr(MGCP_ranges[rangeindex+8],"SDT=0,2,7")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"SDT","Dome");
  }
  if(  (!strcmp(value,"SDT=7"))    && (strstr(MGCP_ranges[rangeindex+8],"SDT=0,2,7")) )
  {
	return FindAttr(-1,FIND_ENUM,MGCP_ranges[rangeindex+1],GetECCLabel(MGCP_RANGE_FCODES[rangeindex]),"SDT","Star");
  }


  

  sprintf(theattr,"%s",value);
  theattr[3] = '\0';

  if(strstr(MGCP_ranges[rangeindex+8],theattr))
  {
    printf("Unhandled value=%s   with %s\n",value,MGCP_ranges[rangeindex+8]);
  }



  return 0;


  
}


double GetHighVal(char *val)
{
  double retval;
  char newval[100];

  if(!strcmp(val,"MAX"))
  {
    return 2147483647;
  }
  else
  {
    if(val[0]=='<')
	{
      sprintf(newval,"%s",&val[1]);
      sscanf(newval,"%lf",&retval);
	  retval = retval - 0.00000001;
	  
	}
	else
	{
      sscanf(val,"%lf",&retval);
	}
  }

  if((retval<1)||(retval>1200000))
  {
    printf("bad high val %lf from %s\n",retval,val);
  }

  return retval;
}


double GetLowVal(char *val, int *include_value)
{
  

  double retval;

  if(!strcmp(val,">0"))         {retval = 0.0;    *include_value = 1;}
  else if(!strcmp(val,"0"))     {retval = 0.0;    *include_value = 0;}
  else if(!strcmp(val,">=0"))   {retval = 0.0;    *include_value = 0;}
  else if(!strcmp(val,">125"))  {retval = 125.0;  *include_value = 1;}
  else if(!strcmp(val,">=125")) {retval = 125.0;  *include_value = 0;}
  else if(!strcmp(val,">25"))   {retval = 25.0;   *include_value = 1;}
  else if(!strcmp(val,">=25"))  {retval = 25.0;   *include_value = 0;}
  else if(!strcmp(val,">=20"))  {retval = 20.0;   *include_value = 0;}
  else if(!strcmp(val,">1"))    {retval = 1.0;    *include_value = 1;}
  else if(!strcmp(val,">2"))    {retval = 2.0;    *include_value = 1;}
  else if(!strcmp(val,">3"))    {retval = 3.0;    *include_value = 1;}
  else if(!strcmp(val,">-378")) {retval = -378.0; *include_value = 1;}
  else if(!strcmp(val,">46"))   {retval = 46.0;   *include_value = 1;}
  else if(!strcmp(val,">=46"))  {retval = 46.0;   *include_value = 1;}
  else if(!strcmp(val,">=0.6")) {retval = 0.6;    *include_value = 1;}
  else if(!strcmp(val,">0.6"))  {retval = 0.6;    *include_value = 1;}
  else if(!strcmp(val,"0.6"))   {retval = 0.6;    *include_value = 1;}
  else
  {
    printf("Error in GLV: %s\n",val);
  }

  return retval;
}


char *GetRangeGeomType(int type,char *gtype)
{
  if(!strcmp(gtype,"(POINT)"))
  {
    if(type==1)
	{
      return "C_POFE";
	}
	return "Point";
  }
  if(!strcmp(gtype,"(LINE)"))
  {
    if(type==1)
	{
      return "C_LINE";
	}
	return "Line";
  }
  if(!strcmp(gtype,"(AREA)"))
  {
    if(type==1)
	{
      return "C_AREA";
	}
    return "Area";
  }
  else
  {
    printf("Error in GRGT: %d %s\n",type,gtype);
	return "Error--See Console";
  }
}


int AddResValue(int oldvalue,int reserved_val)
{
  char old_result[10];
  int new_result,madechange=0;

	


  if(oldvalue==0)
  {
    sprintf(old_result,"00000");
  }
  else if(oldvalue<10)
  {
    
    sprintf(old_result,"0000%d",oldvalue);
  }
  else if(oldvalue<100)
  {
    
    sprintf(old_result,"000%d",oldvalue);
  }
  else if(oldvalue<1000)
  {
    
    sprintf(old_result,"00%d",oldvalue);
  }
  else if(oldvalue<10000)
  {
    
    sprintf(old_result,"0%d",oldvalue);
  }
  else
  {
    sprintf(old_result,"%d",oldvalue);
  }


  if(reserved_val==-32764)
  {
    if(old_result[0]=='0')
	{
      old_result[0] = '1';
	  madechange = 1;
	}
  }
  if(reserved_val==-32765)
  {
    if(old_result[1]=='0')
	{
      old_result[1] = '1';
	  madechange = 1;
	}
  }
  if(reserved_val==-32767)
  {
    if(old_result[2]=='0')
	{
      old_result[2] = '1';
	  madechange = 1;
	}
  }
  if(reserved_val==-32768)
  {
    if(old_result[3]=='0')
	{
      old_result[3] = '1';
	  madechange = 1;
	}
  }
  if(reserved_val==0)
  {
    if(old_result[4]=='0')
	{
      old_result[4] = '1';
	  madechange = 1;
	}
  }

  sscanf(old_result,"%d",&new_result);

  

  return new_result;

}


int GetReservedBit(int oldvalue,int type)
{
  char old_result[10];

  if(oldvalue==0)
  {
    sprintf(old_result,"00000");
  }
  else if(oldvalue<10)
  {
    
    sprintf(old_result,"0000%d",oldvalue);
  }
  else if(oldvalue<100)
  {
    
    sprintf(old_result,"000%d",oldvalue);
  }
  else if(oldvalue<1000)
  {
    
    sprintf(old_result,"00%d",oldvalue);
  }
  else if(oldvalue<10000)
  {
    
    sprintf(old_result,"0%d",oldvalue);
  }
  else
  {
    sprintf(old_result,"%d",oldvalue);
  }

  if(type==1)
  {
    if(old_result[0]=='1')
	{
      return 1;
	}
  }
  if(type==2)
  {
    if(old_result[1]=='1')
	{
      return 1;
	}
  }
  if(type==3)
  {
    if(old_result[2]=='1')
	{
      return 1;
	}
  }
  if(type==4)
  {
    if(old_result[3]=='1')
	{
      return 1;
	}
  }
  if(type==5)
  {
    if(old_result[4]=='1')
	{
      return 1;
	}
  }
  return 0;
}



void CacheMGCPFCODES()
{
  int i;

  for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
    {
      MGCP_RANGE_FCODES[i] = SilentGetCodeIndex(MGCP_ranges[i]);
    }
}


int FilterOutNulls(int thisecc,int thisgeom,double Dval)
{
  int i,fcode_index;
  extern int ATTRIBUTION_TYPE;
  extern int RealClose(double a, double b);

  

  



  if(ATTRIBUTION_TYPE!=7)
  {
    return 1;
  }



  


  for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
  {
    if(!strcmp(MGCP_ranges[i],"metad"))
	{
      continue;
	}

    

    fcode_index = MGCP_RANGE_FCODES[i];

	if(fcode_index==thisecc)
	{
      if(!strcmp(MGCP_ranges[i+2],"HGT"))
	  {
        if(
		    ((thisgeom==C_POFE)&&(!strcmp(MGCP_ranges[i+1],"(POINT)"))) ||
		    ((thisgeom==C_LINE)&&(!strcmp(MGCP_ranges[i+1],"(LINE)")))  ||
		    ((thisgeom==C_AREA)&&(!strcmp(MGCP_ranges[i+1],"(AREA)")))
		  )
		{ 
           

           if(MGCP_ranges[i+3][0]=='N')
		   {
             return 1;
		   }
           else if(MGCP_ranges[i+3][0]=='Y')
		   {
			 if(
                (RealClose(Dval,-32764.0)) ||
                (RealClose(Dval,-32765.0)) ||
                (RealClose(Dval,-32767.0)) ||
                (RealClose(Dval,-32768.0))
			   )
			 { 
               return 0;
			 }
			 return 1;
		   }
		   else
		   {
		     printf("Error in FON...should not be here with (%d) %s = %c\n",i,MGCP_ranges[i+3],MGCP_ranges[i+3][0]);
			 return 1;
		   }
		} 
	  } 
	}
  }

  return 1;

}




int ComputeAdditionalVV1_TDS(	
		     int *pointspresent,
			 int *areaspresent
          )
{
  int i,ecc_index,retval=0;


  if(ATTRTYPE_TDS6()==0)
  {
    return 0;
  }


  TDS_badvals2 = (int *)malloc(SzI * (TDS_badsingleNum*5 + 10));

  for(i=0;i<TDS_badsingleNum*5;i=i+5)
  {

    TDS_badvals2[i] = 0;


    ecc_index = SilentGetCodeIndex(TDS_badvals[i]);

	if(ecc_index<0)
	{
	  printf("bad ecc %d for %s\n",ecc_index,TDS_badvals[i]);
	  continue;
	}

	if(TDS_badvals[i+1][0]=='P')
	{
	  
	  if(pointspresent[ecc_index]>0)
	  {
	    TDS_badvals2[i] = 1;
	  }
	}
	else if(TDS_badvals[i+1][0]=='A')
	{
	  
      if(areaspresent[ecc_index]>0)
	  {
	    TDS_badvals2[i] = 1;
	  }
	}
	else
	{
	  printf("Bad value when processing TMBV: %c\n",TDS_badvals[i+1][0]);
	  return 0;
	}


    if(TDS_badvals2[i]>0)
	{ 
 	  retval = retval + 1;
	}

  }
  
  return retval;
}








int ComputeAdditionalVV1_MGCP()
{
  char cleancode[1000];
  int i,thiscodeindex,retval=0,lastcode=-1;
  double Dval;
  extern int FindAttr(double dval,int passtype,char *geometry,char *charSCC, char *charSAC, char *targetvalue);


  if(ATTRTYPE_MGCP4()==0)
  {
    return 0;
  }



  MGCP4_badvals2 = (int *)malloc(SzI * (MGCP4_badsingleNum*5 + 10));

  for(i=0;i<MGCP4_badsingleNum*5;i=i+5)
  {

    MGCP4_badvals2[i] = 0;

	if((lastcode>=0)&&(i>0))
	{
	  if(!strcmp(MGCP4_badvals[i],MGCP4_badvals[i-5]))
	  {
        thiscodeindex = lastcode;
	  }
	  else
	  {
        thiscodeindex = GetCodeIndex(MGCP4_badvals[i]);
	    lastcode = thiscodeindex;
	  }
	}
	else
	{
      thiscodeindex = GetCodeIndex(MGCP4_badvals[i]);
	  lastcode = thiscodeindex;
	}

	if(MGCP4_badvals[i+3][0]=='1')
	{
      sprintf(cleancode,"%s",GetECCLabel(thiscodeindex));

	  if(MGCP4_badvals[i+1][0]=='L')
	  {
	    MGCP4_badvals2[i] = FindAttr(-1,FIND_ENUM,"(LINE)",cleancode,MGCP4_badvals[i+2], MGCP4_badvals[i+4]);
	  }
	  else if(MGCP4_badvals[i+1][0]=='A')
	  {
	    MGCP4_badvals2[i] = FindAttr(-1,FIND_ENUM,"(AREA)",cleancode,MGCP4_badvals[i+2], MGCP4_badvals[i+4]);
	  }
	  else
	  {
		 printf("Bad value when processing MBV: %c\n",MGCP4_badvals[i+1][0]);
		 return 0;
	  }

	  
     
	}
	else
	{
      sprintf(cleancode,"%s",GetECCLabel(thiscodeindex));

	  sscanf(MGCP4_badvals[i+4],"%lf",&Dval);

	  if(MGCP4_badvals[i+1][0]=='L')
	  {
	    MGCP4_badvals2[i] = FindAttr(Dval,FIND_REAL,"(LINE)",cleancode,MGCP4_badvals[i+2], "unused");
	  }
	  else if(MGCP4_badvals[i+1][0]=='A')
	  {
	    MGCP4_badvals2[i] = FindAttr(Dval,FIND_REAL,"(AREA)",cleancode,MGCP4_badvals[i+2], "unused");
	  }
	  else
	  {
		 printf("Bad value when processing MBV2: %c\n",MGCP4_badvals[i+1][0]);
		 return 0;
	  }

	  
	}


    if(MGCP4_badvals2[i]>0)
	{ 
 	  retval = retval + 1;
	} 
  }
  
  return retval;
}



char *GetEqualString(char *inval)
{
  if(inval[0]=='<')
  {
    return " or equal to ";
  }
  return " ";
}



int BadTDSDate(char *thedate)
{
  char temp[50];
  int year,month,day,thisyear,thismonth,thisday,maxdays;
  extern int GetTimeValues(int *year, int *month, int *day);


  

  if(strlen(thedate)!=12)
  {
    return 1;
  }

  if(thedate[5]!='-')
  {
    return 1;
  }
  if(thedate[8]!='-')
  {
    return 1;
  }

  if(
	  ((thedate[1] <'0')||(thedate[1] >'9')) ||
	  ((thedate[2] <'0')||(thedate[2] >'9')) ||
	  ((thedate[3] <'0')||(thedate[3] >'9')) ||
	  ((thedate[4] <'0')||(thedate[4] >'9')) ||
	  ((thedate[6] <'0')||(thedate[6] >'9')) ||
	  ((thedate[7] <'0')||(thedate[7] >'9')) ||
	  ((thedate[9] <'0')||(thedate[9] >'9')) ||
	  ((thedate[10]<'0')||(thedate[10]>'9'))
	)
  {
    return 1;
  }


  temp[0] = thedate[1];
  temp[1] = thedate[2];
  temp[2] = thedate[3];
  temp[3] = thedate[4];
  temp[4] = '\0';
  sscanf(temp,"%d",&year);

  temp[0] = thedate[6];
  temp[1] = thedate[7];
  temp[2] = '\0';
  sscanf(temp,"%d",&month);

  temp[0] = thedate[9];
  temp[1] = thedate[10];
  temp[2] = '\0';
  sscanf(temp,"%d",&day);


  if((month<1)||(month>12))
  {
    return 1;
  }
  if(day<1)
  {
    return 1;
  }


  if(
	  (month==1) || (month==3) || (month==5) || (month==7) ||
	  (month==8) || (month==10)|| (month==12)
	)
  {
    maxdays = 31;
  }
  else if((month==4) || (month==6) || (month==9) || (month==11))
  {
    maxdays = 30;
  }
  else if(month==2)
  {
	if((year%4==0)&&((year%100!=0)||(year%400==0))) 
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
    return 1;
  }

  if(day>maxdays)
  {
    return 1;
  }


  if(GetTimeValues(&thisyear,&thismonth,&thisday))
  {
    if( 
		(year>thisyear)                                            ||
		((year==thisyear) && (month>thismonth))                    ||
        ((year==thisyear) && (month==thismonth) && (day>thisday))
	  )
	{
	  return 1;
	}
  }



  return 0;

}




int BadMGCPDate(char *thedate)
{
  char temp[50];
  int year,month,day,thisyear,thismonth,thisday,maxdays;
  extern int GetTimeValues(int *year, int *month, int *day);


  

  if(strlen(thedate)!=12)
  {
    return 1;
  }

  if(thedate[5]!='-')
  {
    return 1;
  }
  if(thedate[8]!='-')
  {
    return 1;
  }

  if(
	  ((thedate[1] <'0')||(thedate[1] >'9')) ||
	  ((thedate[2] <'0')||(thedate[2] >'9')) ||
	  ((thedate[3] <'0')||(thedate[3] >'9')) ||
	  ((thedate[4] <'0')||(thedate[4] >'9')) ||
	  ((thedate[6] <'0')||(thedate[6] >'9')) ||
	  ((thedate[7] <'0')||(thedate[7] >'9')) ||
	  ((thedate[9] <'0')||(thedate[9] >'9')) ||
	  ((thedate[10]<'0')||(thedate[10]>'9'))
	)
  {
    return 1;
  }


  temp[0] = thedate[1];
  temp[1] = thedate[2];
  temp[2] = thedate[3];
  temp[3] = thedate[4];
  temp[4] = '\0';
  sscanf(temp,"%d",&year);
  if(year<1900)
  {
	return 1;
  }

  temp[0] = thedate[6];
  temp[1] = thedate[7];
  temp[2] = '\0';
  sscanf(temp,"%d",&month);

  temp[0] = thedate[9];
  temp[1] = thedate[10];
  temp[2] = '\0';
  sscanf(temp,"%d",&day);


  if((month<1)||(month>12))
  {
    return 1;
  }
  if(day<1)
  {
    return 1;
  }


  if(
	  (month==1) || (month==3) || (month==5) || (month==7) ||
	  (month==8) || (month==10)|| (month==12)
	)
  {
    maxdays = 31;
  }
  else if((month==4) || (month==6) || (month==9) || (month==11))
  {
    maxdays = 30;
  }
  else if(month==2)
  {
	if((year%4==0)&&((year%100!=0)||(year%400==0))) 
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
    return 1;
  }

  if(day>maxdays)
  {
    return 1;
  }


  if(GetTimeValues(&thisyear,&thismonth,&thisday))
  {
    if( 
		(year>thisyear)                                            ||
		((year==thisyear) && (month>thismonth))                    ||
        ((year==thisyear) && (month==thismonth) && (day>thisday))
	  )
	{
	  return 1;
	}
  }

  return 0;

}



void DoVVTERR1WAY_TYPE12(
			 int type,char *prefix,
			 int *pointspresent,
			 int *linespresent,
			 int *areaspresent,
			 int point_aqueduct_found,
			 int line_tunnelACC1found,
             int point_bua_nam_unk,
             int point_bua_nam_na,
             int point_sett_nam_unk,
             int point_sett_nam_na,
			 int point_bua_found,
			 int area_mangrove_found,
			 int area_mangroveTID1001_found,
			 int point_bua_nam_unk1,
			 int point_bua_nam_unk2,
			 int point_bua_nam_unk3,
			 int point_bua_nam_unk4,
			 int point_bua_nam_na1,
			 int point_bua_nam_na2,
			 int point_bua_nam_na3,
			 int point_bua_nam_na4,
			 int point_bua_nam_na5,
			 int point_sett_nam_unk1,
			 int point_sett_nam_unk2,
			 int point_sett_nam_unk3,
			 int point_sett_nam_unk4,
			 int point_sett_nam_na1,
			 int point_sett_nam_na2,
			 int point_sett_nam_na3,
			 int point_sett_nam_na4,
			 int point_sett_nam_na5,
			 int area_bridge_found,
			 int area_1bridgefound,
			 int area_tunnel_found,
             int area_1tunnelfound,
             int point_atc1aqueduct,
			 int area_rkf3rock,
			 int area_orchard_found,
			 int area_trees_found,
			 int area_swamp_found,
			 int area_scrub_found,
			 int point_named_nam_unk,
			 int point_named_nam_unk1,
			 int point_named_nam_unk2,
			 int point_named_nam_unk3,
			 int point_named_nam_unk4,
			 int point_named_nam_na,
			 int point_named_nam_na1,
			 int point_named_nam_na2,
			 int point_named_nam_na3,
			 int point_named_nam_na4,
			 int point_named_nam_na5,
			 int area_tidal_nam_sentinel1,
			 int area_tidal_nam_sentinel2,
			 int area_tidal_nam_sentinel3,
			 int area_tidal_nam_sentinel4,
			 int area_tidal_nam_sentinel5,
			 int area_tidal_nam_sentinel6,
			 int area_tidal_nam_sentinel7,
			 int area_tidal_nam_sentinel8,
			 int area_tidal_nam_sentinel9,
			 int area_tidal_nam_sentinel10,
			 int area_tidal_nam_sentinel11,
			 int area_tidal_nam_sentinel12,
			 int area_tidal_nam_sentinel13,
			 int area_tidal_nam_sentinel14,
			 int area_tidal_nam_sentinel15,
			 int area_tidal_nam_sentinel16,
			 int area_tidal_nam_sentinel17,
			 int area_tidal_nam_sentinel18,
			 int area_tidal_nam_sentinel19,
			 int area_tidal_nam_sentinel20
			 )
{
  FILE *newfile;
  char message[1000],cleancode[1000];
  int docheck1,docheck1a,docheck2,docheck3,docheck4,
	  docheck5,docheck6,docheck7,docheck8,docheck9,
	  docheck10,docheck11,docheck12,docheck13,
	  docheck14,docheck15,docheck16,docheck17,
	  docheck18,docheck19,docheck20,docheck21,
	  docheck22,docheck23,docheck24,docheck25,
	  docheck26,docheck27,include_end_value,i,j,k,
	  totalchecks,thisecc,thisgeom,fcode_index,
	  thistype,do_lower,do_higher,filter_out_nulls,
	  thisindex,thatindex,thiscodeindex=-1,Ival;
  double maxHGT,Dval,lowval,highval,thevalue,minWID,maxWID,
	  smallestOVER,thishighval,junk1,junk2,saveAQwid,djunk;



  int *pointHGTres,*lineHGTres,*areaHGTres;
  double *pointHGTmax,*lineHGTmax,*areaHGTmax,*pointHGTmin,*lineHGTmin,*areaHGTmin;

  int *pointLENres,*lineLENres,*areaLENres;
  double *pointLENmax,*lineLENmax,*areaLENmax,*pointLENmin,*lineLENmin,*areaLENmin;

  double MGCP_range_low [MGCP_RANGE_SPECS_MAX];
  double MGCP_range_high[MGCP_RANGE_SPECS_MAX];
  double MGCP_range_high2[MGCP_RANGE_SPECS_MAX];
  int    MGCP_range_res[MGCP_RANGE_SPECS_MAX];

  extern int GIFD_CLASS_NUM;
  extern int ATTRIBUTION_TYPE;
  extern void ExeUpdate(char message[]);
  extern int RealClose(double a, double b);
  extern char *GetLengthCode();
  extern char *GetBridgeName(int english);
  extern char *GetTRS1Name();
  extern char *GetDamName(int english);
  extern char *GetEmbankmentName(int english);
  extern char *GetTidalWaterName(int english);






  pointHGTres    = (int *)malloc(SzI * (GIFD_CLASS_NUM+5));
  lineHGTres     = (int *)malloc(SzI * (GIFD_CLASS_NUM+5));
  areaHGTres     = (int *)malloc(SzI * (GIFD_CLASS_NUM+5));
  pointHGTmax    = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  lineHGTmax     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  areaHGTmax     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  pointHGTmin    = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  lineHGTmin     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  areaHGTmin     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));


  pointLENres    = (int *)malloc(SzI * (GIFD_CLASS_NUM+5));
  lineLENres     = (int *)malloc(SzI * (GIFD_CLASS_NUM+5));
  areaLENres     = (int *)malloc(SzI * (GIFD_CLASS_NUM+5));
  pointLENmax    = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  lineLENmax     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  areaLENmax     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  pointLENmin    = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  lineLENmin     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));
  areaLENmin     = (double *)malloc(SzD * (GIFD_CLASS_NUM+5));

  maxHGT = 45.999;  







   if(type==2)
   {

     if(ATTRTYPE_MGCP3())
	 { 
       MGCP_RANGE_SPECS = MGCP_RANGE_SPECS3;
       MGCP_ranges = MGCP_ranges3;
	 }
     else
	 { 
       MGCP_RANGE_SPECS = MGCP_RANGE_SPECS4;
       MGCP_ranges = MGCP_ranges4;
	 } 


      CacheMGCPFCODES();

      sprintf(message,"%sVVTERR1WAY.txt",prefix);

      newfile = fopen(message,"w");
      ExeUpdate(message);
	  
	  
	  


	 for(i=0;i<MGCP_RANGE_SPECS;i++)
	 {
       MGCP_range_low [i]  =  199999991;
       MGCP_range_high[i]  = -199999993;
       MGCP_range_high2[i] = -199999993;
	   MGCP_range_res [i]  = 0;
	 }










	 

     for(i=1;i<=GIFD_CLASS_NUM;i++)
	 {
       pointHGTres   [i] = 0;
       lineHGTres    [i] = 0;
       areaHGTres    [i] = 0;

       pointHGTmax   [i] = -1999999999;
       lineHGTmax    [i] = -1999999999;
       areaHGTmax    [i] = -1999999999;

       pointHGTmin   [i] = 1999999999;
       lineHGTmin    [i] = 1999999999;
       areaHGTmin    [i] = 1999999999;
	 }


	 if(SilentGetAttrIndex("HGT")>=0)  
	 {
       BuildLLTable(1,LLFidTableCount);

       Begin_LongFiles(LONG_ATTR_HGT);

       for(i=0;i<LLFidTableCount;i++)
	   {  
  	     if(LLFidTable[i]<0)
		 { 
           continue;
		 }  

         thistype = GetLongAttrVal(LONG_ATTR_HGT,i,&Ival,&Dval,&cleancode[0]);

         thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
         thisgeom =          CrsWlk[LLFidTable[i]].geomtype;

         if(thistype==I_TYPE)
		 {  
           Dval = (double)Ival;
		 }  

         if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
		 {  
           
           if(thisgeom==C_POFE)
		   {  
             if(Dval>pointHGTmax[thisecc])
			 {  
               pointHGTmax[thisecc] = Dval;
			 } 
             if(Dval<pointHGTmin[thisecc])
			 {  
			   if(FilterOutNulls(thisecc,C_POFE,Dval)==1)
			   { 
                 pointHGTmin[thisecc] = Dval;
			   } 
			 } 
		     if(RealClose(Dval,-32764.0))
			 { 
               pointHGTres[thisecc] = AddResValue(pointHGTres[thisecc],-32764);
			 } 
		     if(RealClose(Dval,-32765.0))
			 { 
               pointHGTres[thisecc] = AddResValue(pointHGTres[thisecc],-32765);
			 } 
		     if(RealClose(Dval,-32767.0))
			 { 
               pointHGTres[thisecc] = AddResValue(pointHGTres[thisecc],-32767);
			 } 
		     if(RealClose(Dval,-32768.0))
			 { 
               pointHGTres[thisecc] = AddResValue(pointHGTres[thisecc],-32768);
			 } 
		     if(RealClose(Dval,0.0))
			 { 
               pointHGTres[thisecc] = AddResValue(pointHGTres[thisecc],0);
			 } 
		   } 
           else if(thisgeom==C_LINE)
		   {  
             if(Dval>lineHGTmax[thisecc])
			 {  
               lineHGTmax[thisecc] = Dval;
			 } 
             if(Dval<lineHGTmin[thisecc])
			 {  
			   if(FilterOutNulls(thisecc,C_LINE,Dval)==1)
			   { 
                 lineHGTmin[thisecc] = Dval;
			   } 
			 } 
		     if(RealClose(Dval,-32764.0))
			 { 
               lineHGTres[thisecc] = AddResValue(lineHGTres[thisecc],-32764);
			 } 
		     if(RealClose(Dval,-32765.0))
			 {
               lineHGTres[thisecc] = AddResValue(lineHGTres[thisecc],-32765);
			 }
		     if(RealClose(Dval,-32767.0))
			 { 
               lineHGTres[thisecc] = AddResValue(lineHGTres[thisecc],-32767);
			 } 
		     if(RealClose(Dval,-32768.0))
			 {
               lineHGTres[thisecc] = AddResValue(lineHGTres[thisecc],-32768);
			 } 
		     if(RealClose(Dval,0.0))
			 { 
               lineHGTres[thisecc] = AddResValue(lineHGTres[thisecc],0);
			 } 
		   }  
           else if(thisgeom==C_AREA)
		   {  
             if(Dval>areaHGTmax[thisecc])
			 {  
               areaHGTmax[thisecc] = Dval;
			 }  
             if(Dval<areaHGTmin[thisecc])
			 {  
			   if(FilterOutNulls(thisecc,C_AREA,Dval)==1)
			   { 
                 areaHGTmin[thisecc] = Dval;
			   } 
			 } 
		     if(RealClose(Dval,-32764.0))
			 { 
               areaHGTres[thisecc] = AddResValue(areaHGTres[thisecc],-32764);
			 } 
		     if(RealClose(Dval,-32765.0))
			 { 
               areaHGTres[thisecc] = AddResValue(areaHGTres[thisecc],-32765);
			 }
		     if(RealClose(Dval,-32767.0))
			 { 
               areaHGTres[thisecc] = AddResValue(areaHGTres[thisecc],-32767);
			 } 
		     if(RealClose(Dval,-32768.0))
			 { 
               areaHGTres[thisecc] = AddResValue(areaHGTres[thisecc],-32768);
			 } 
		     if(RealClose(Dval,0.0))
			 { 
               areaHGTres[thisecc] = AddResValue(areaHGTres[thisecc],0);
			 } 
		   } 
		 } 
	   } 

       FreeLLTable(); 
       End_LongFiles(LONG_ATTR_HGT);
	  } 


	 



	 

     for(i=1;i<=GIFD_CLASS_NUM;i++)
	 {
       pointLENres   [i] = 0;
       lineLENres    [i] = 0;
       areaLENres    [i] = 0;

       pointLENmax   [i] = -1999999999;
       lineLENmax    [i] = -1999999999;
       areaLENmax    [i] = -1999999999;

       pointLENmin   [i] = 1999999999;
       lineLENmin    [i] = 1999999999;
       areaLENmin    [i] = 1999999999;
	 }


	 

	 if(SilentGetAttrIndex("LEN")>=0)  
	 {
       BuildLLTable(1,LLFidTableCount);

       Begin_LongFiles(LONG_ATTR_LEN);

       for(i=0;i<LLFidTableCount;i++)
	   {  
  	     if(LLFidTable[i]<0)
		 { 
           continue;
		 }  

         thistype = GetLongAttrVal(LONG_ATTR_LEN,i,&Ival,&Dval,&cleancode[0]);

         thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
         thisgeom =          CrsWlk[LLFidTable[i]].geomtype;

         if(thistype==I_TYPE)
		 {  
           Dval = (double)Ival;
		 }  

         if( ((thistype==I_TYPE)||(thistype==D_TYPE)) )
		 {  
           
           if(thisgeom==C_POFE)
		   {  
             if(Dval>pointLENmax[thisecc])
			 {  
               pointLENmax[thisecc] = Dval;
			 } 
             if(Dval<pointLENmin[thisecc])
			 {  
			   if(FilterOutNulls(thisecc,C_POFE,Dval)==1)
			   { 
                 pointLENmin[thisecc] = Dval;
			   } 
			 } 
		     if(RealClose(Dval,-32764.0))
			 { 
               pointLENres[thisecc] = AddResValue(pointLENres[thisecc],-32764);
			 } 
		     if(RealClose(Dval,-32765.0))
			 { 
               pointLENres[thisecc] = AddResValue(pointLENres[thisecc],-32765);
			 } 
		     if(RealClose(Dval,-32767.0))
			 { 
               pointLENres[thisecc] = AddResValue(pointLENres[thisecc],-32767);
			 } 
		     if(RealClose(Dval,-32768.0))
			 { 
               pointLENres[thisecc] = AddResValue(pointLENres[thisecc],-32768);
			 } 
		     if(RealClose(Dval,0.0))
			 { 
               pointLENres[thisecc] = AddResValue(pointLENres[thisecc],0);
			 } 
		   } 
           else if(thisgeom==C_LINE)
		   {  
             if(Dval>lineLENmax[thisecc])
			 {  
               lineLENmax[thisecc] = Dval;
			 } 
             if(Dval<lineLENmin[thisecc])
			 {  
			   if(FilterOutNulls(thisecc,C_LINE,Dval)==1)
			   { 
                 lineLENmin[thisecc] = Dval;
			   } 
			 } 
		     if(RealClose(Dval,-32764.0))
			 { 
               lineLENres[thisecc] = AddResValue(lineLENres[thisecc],-32764);
			 } 
		     if(RealClose(Dval,-32765.0))
			 { 
               lineLENres[thisecc] = AddResValue(lineLENres[thisecc],-32765);
			 } 
		     if(RealClose(Dval,-32767.0))
			 { 
               lineLENres[thisecc] = AddResValue(lineLENres[thisecc],-32767);
			 } 
		     if(RealClose(Dval,-32768.0))
			 { 
               lineLENres[thisecc] = AddResValue(lineLENres[thisecc],-32768);
			 } 
		     if(RealClose(Dval,0.0))
			 { 
               lineLENres[thisecc] = AddResValue(lineLENres[thisecc],0);
			 } 
		   }  
           else if(thisgeom==C_AREA)
		   {  
             if(Dval>areaLENmax[thisecc])
			 {  
               areaLENmax[thisecc] = Dval;
			 }  
             if(Dval<areaLENmin[thisecc])
			 {  
			   if(FilterOutNulls(thisecc,C_AREA,Dval)==1)
			   { 
                 areaLENmin[thisecc] = Dval;
			   } 
			 } 
		     if(RealClose(Dval,-32764.0))
			 { 
               areaLENres[thisecc] = AddResValue(areaLENres[thisecc],-32764);
			 }
		     if(RealClose(Dval,-32765.0))
			 { 
               areaLENres[thisecc] = AddResValue(areaLENres[thisecc],-32765);
			 } 
		     if(RealClose(Dval,-32767.0))
			 {
               areaLENres[thisecc] = AddResValue(areaLENres[thisecc],-32767);
			 } 
		     if(RealClose(Dval,-32768.0))
			 { 
               areaLENres[thisecc] = AddResValue(areaLENres[thisecc],-32768);
			 } 
		     if(RealClose(Dval,0.0))
			 { 
               areaLENres[thisecc] = AddResValue(areaLENres[thisecc],0);
			 } 
		   } 
		} 
	  } 

      FreeLLTable(); 
      End_LongFiles(LONG_ATTR_LEN);

     } 


	 




	 for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
	 {

	   

       fcode_index = MGCP_RANGE_FCODES[i];


	   thishighval = GetHighVal(MGCP_ranges[i+5]);

       if(fcode_index>=0)
	   {

		 if(!strcmp(MGCP_ranges[i+2],"HGT"))
		 {
		    if(!strcmp(MGCP_ranges[i+1],"(POINT)"))
			{
              MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]  = pointHGTmin[fcode_index];
              MGCP_range_high[i/MGCP_RANGE_MULTIPLIER] = pointHGTmax[fcode_index];
			  MGCP_range_res[i/MGCP_RANGE_MULTIPLIER]  = pointHGTres[fcode_index];
			} 
		    else if(!strcmp(MGCP_ranges[i+1],"(LINE)"))
			{ 
              MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]  = lineHGTmin[fcode_index];
              MGCP_range_high[i/MGCP_RANGE_MULTIPLIER] = lineHGTmax[fcode_index];
			  MGCP_range_res[i/MGCP_RANGE_MULTIPLIER]  = lineHGTres[fcode_index];
			} 
		    else if(!strcmp(MGCP_ranges[i+1],"(AREA)"))
			{ 
              MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]  = areaHGTmin[fcode_index];
              MGCP_range_high[i/MGCP_RANGE_MULTIPLIER] = areaHGTmax[fcode_index];
			  MGCP_range_res[i/MGCP_RANGE_MULTIPLIER]  = areaHGTres[fcode_index];
			} 
		    else
			{ 
              printf("Bad ftrg: %s\n",MGCP_ranges[i+1]);
			} 

			
		    MGCP_range_high2[i/MGCP_RANGE_MULTIPLIER] = GetHighVal(MGCP_ranges[i+5]);
		 }
		 else if(!strcmp(MGCP_ranges[i+2],"LEN"))
		 {
		    if(!strcmp(MGCP_ranges[i+1],"(POINT)"))
			{
              MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]  = pointLENmin[fcode_index];
              MGCP_range_high[i/MGCP_RANGE_MULTIPLIER] = pointLENmax[fcode_index];
			  MGCP_range_res[i/MGCP_RANGE_MULTIPLIER]  = pointLENres[fcode_index];
			} 
		    else if(!strcmp(MGCP_ranges[i+1],"(LINE)"))
			{ 
              MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]  = lineLENmin[fcode_index];
              MGCP_range_high[i/MGCP_RANGE_MULTIPLIER] = lineLENmax[fcode_index];
			  MGCP_range_res[i/MGCP_RANGE_MULTIPLIER]  = lineLENres[fcode_index];
			} 
		    else if(!strcmp(MGCP_ranges[i+1],"(AREA)"))
			{ 
              MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]  = areaLENmin[fcode_index];
              MGCP_range_high[i/MGCP_RANGE_MULTIPLIER] = areaLENmax[fcode_index];
			  MGCP_range_res[i/MGCP_RANGE_MULTIPLIER]  = areaLENres[fcode_index];
			} 
		    else
			{ 
              printf("Bad ftrg: %s\n",MGCP_ranges[i+1]);
			} 

			
		    MGCP_range_high2[i/MGCP_RANGE_MULTIPLIER] = GetHighVal(MGCP_ranges[i+5]);
		 }
		 else
		 { 

           

		   if(MGCP_ranges[i+3][0]=='Y')
		   {
             filter_out_nulls = 1;
		   }
		   else
		   {
             filter_out_nulls = 0;
		   }


		   if(ExcludeRES(i,"-1",1)==0)
		   {

			   


			  

              

			  filter_out_nulls = 0;
		   }


if(ATTRIBUTION_TYPE!=7)
{
  

	filter_out_nulls = 0;  
}


           smallestOVER = 12345669;

           if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,
			   NO_U_MIN,NO_U_MAX,filter_out_nulls,  &smallestOVER,thishighval,&djunk,-999       )>0)
		   {
             MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]   = minWID;
             MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]  = maxWID;
			 MGCP_range_high2[i/MGCP_RANGE_MULTIPLIER] = smallestOVER;

             

		   }


           if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-32768.0,-32768.0,0,&smalljunkflag,-999,&djunk,-999)>0)
		   {
		     MGCP_range_res[i/MGCP_RANGE_MULTIPLIER] = AddResValue(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],-32768);
		   }
           if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-32767.0,-32767.0,0,&smalljunkflag,-999,&djunk,-999)>0)
		   {
		     MGCP_range_res[i/MGCP_RANGE_MULTIPLIER] = AddResValue(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],-32767);
		   }
           if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-32765.0,-32765.0,0,&smalljunkflag,-999,&djunk,-999)>0)
		   {
		     MGCP_range_res[i/MGCP_RANGE_MULTIPLIER] = AddResValue(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],-32765);
		   }
           if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-32764.0,-32764.0,0,&smalljunkflag,-999,&djunk,-999)>0)
		   {
		     MGCP_range_res[i/MGCP_RANGE_MULTIPLIER] = AddResValue(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],-32764);
		   }
           if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,0.0,0.0,0,&smalljunkflag,-999,&djunk,-999)>0)
		   {
		     MGCP_range_res[i/MGCP_RANGE_MULTIPLIER] = AddResValue(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],0);
		   }
		 }
	   }
	 }


	 docheck1  = 0; 
	 docheck1a = 0; 
	 docheck2  = 0; 
	 docheck3  = 0; 
	 docheck4  = 0;  
	 docheck5  = 0; 
	 docheck6  = 0; 
     docheck7  = 0; 
     docheck8  = 0; 
	 docheck9  = 0; 
	 docheck10 = 0; 
	 docheck11 = 0; 
	 docheck12 = 0; 
	 docheck13 = 0; 
	 docheck14 = 0; 
	 docheck15 = 0; 
	 docheck16 = 0; 
	 docheck17 = 0; 
	 docheck18 = 0; 
	 docheck19 = 0; 
	 docheck20 = 0; 
	 docheck21 = 0; 
	 docheck22 = 0; 
	 docheck23 = 0; 
     docheck24 = 0; 
	 docheck25 = 0; 
	 docheck26 = 0; 
	 docheck27 = 0; 

	 do_lower  = 1;
	 do_higher = 1;


	 for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
	 {
	   if(do_lower==0)
	   {
         break;
	   }

	   

       if(ATTRTYPE_MGCP3()==0)
	   {
         if(
			 (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1) || 
             (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1) || 
             (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)    
           )
		 {
		   docheck1a = docheck1a + 1;
		 }
	   }




       if(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]!=199999991)
	   {
         

		 lowval = GetLowVal(MGCP_ranges[i+4],&include_end_value);

		 if((include_end_value==0)&&(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]<lowval))
		 {
           docheck1 = docheck1 + 1;

		   
		 }
		 else if((include_end_value==1)&&(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]<=lowval))
		 {
           docheck1 = docheck1 + 1;

		   
		 }
	   }
	 }


	 for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
	 {
	   if(do_higher==0)
	   {
         break;
	   }


       if(MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]!=-199999993)
	   {
         

		 highval = GetHighVal(MGCP_ranges[i+5]);

		 if(MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]>highval)
		 {
           docheck2 = docheck2 + 1;

		   
		 }
	   }
	 }


	 
	 
	 
	 


     for(i=0;i<NumberOfAttributesEntries;i++)
	 { 
	    thistype = 0;
		
        if(!strcmp(GetEACCode(MdlNames2[i].code),"ACE"))
		{
		  thistype = 1;
		}
        if(!strcmp(GetEACCode(MdlNames2[i].code),"ALE"))
		{
		  thistype = 2;
		}

		if(thistype>0)
		{
          for(j=0;j<MdlNames2[i].count;j++)
		  {
	        if(MdlNames2[i].values[j].uniqueoccurrences>0)
			{
		      if((MdlNames2[i].values[j].type==0)||(MdlNames2[i].values[j].type==1))
			  {
				thevalue = (double) MdlNames2[i].values[j].value;

				if(thistype==1)
				{
                  if(thevalue<=0)
				  {
                   if( 
					   (RealClose(thevalue,-32764.0)==0) &&
					   (RealClose(thevalue,-32765.0)==0) &&
					   (RealClose(thevalue,-32767.0)==0) &&
					   (RealClose(thevalue,-32768.0)==0)
                      )
				   {  
                     docheck3 = 1;
				   } 
				  }
                  if(thevalue>25.1)
				  {
                    docheck4 = 1;
				  }
				}
				else if(thistype==2)
				{
                  if(thevalue<=0)
				  {
                    if( 
					   (RealClose(thevalue,-32764.0)==0) &&
					   (RealClose(thevalue,-32765.0)==0) &&
					   (RealClose(thevalue,-32767.0)==0) &&
					   (RealClose(thevalue,-32768.0)==0)
                      )
					{ 
                      docheck5 = 1;
					}
				  }
                  if(thevalue>25.1)
				  {
                    docheck6 = 1;
				  }
				}

				

			  } 
			} 
		  } 
		} 
	 } 



    if(point_aqueduct_found>0)
	{
	  if(ATTRTYPE_MGCP3()==1)
	  {
		

        if(LEN_Getter(GetLengthCode(),"(POINT)","Aqueduct",&minWID,&maxWID,-9999999,9999999)>0)
		{
		  saveAQwid = maxWID;
	      if(maxWID>=3.0)
		  {  
            docheck7 = 1;
		  }  
	      
		}  
	  } 
	}

    if(line_tunnelACC1found>0)
	{
      docheck8 = 1;

	  docheck8 = 0;  
	}


	if((type==2)&&(ATTRTYPE_MGCP4()))
	{
	  if((point_bua_nam_unk +
		  point_bua_nam_unk1+
		  point_bua_nam_unk2+
		  point_bua_nam_unk3+
		  point_bua_nam_unk4)>0)
	  { 
        docheck9 = 1;
	  } 
	  if((point_bua_nam_na +
		  point_bua_nam_na1+
		  point_bua_nam_na2+
		  point_bua_nam_na3+
		  point_bua_nam_na4+
		  point_bua_nam_na5)>0)
	  { 
        docheck10 = 1;
	  } 
	  if((point_sett_nam_unk +
		  point_sett_nam_unk1+
		  point_sett_nam_unk2+
		  point_sett_nam_unk3+
		  point_sett_nam_unk4+

		  point_named_nam_unk +
		  point_named_nam_unk1+
		  point_named_nam_unk2+
		  point_named_nam_unk3+
		  point_named_nam_unk4
		  )>0)
	  { 
        docheck11 = 1;
	  } 
	  if((point_sett_nam_na +
		  point_sett_nam_na1+
		  point_sett_nam_na2+
		  point_sett_nam_na3+
		  point_sett_nam_na4+
		  point_sett_nam_na5+

          point_named_nam_na +
		  point_named_nam_na1+
		  point_named_nam_na2+
		  point_named_nam_na3+
		  point_named_nam_na4+
		  point_named_nam_na5		  
		  )>0)
	  { 
        docheck12 = 1;
	  }
	  if(area_mangrove_found>0)
	  {
        docheck13 = 1;
	  }
	  if((area_bridge_found+area_tunnel_found)>0)
	  {
        docheck14 = 1;
        docheck14 = 0;
	  }
	}

    if(point_aqueduct_found>0)
	{
      docheck15 = 1;
	}


	if(area_rkf3rock>0)
	{
      if(ATTRTYPE_MGCP3()==0)
	  {
        docheck17 = 1;
	  }
	}


	if(ATTRTYPE_MGCP3()==0)
	{
	  if(area_orchard_found>0)
	  { 
        docheck18 = 1;
	  } 
	  if(area_trees_found>0)
	  { 
        docheck19 = 1;
	  } 
	  if(area_swamp_found>0)
	  { 
        docheck20 = 1;
	  } 
	  if(area_mangrove_found>0)
	  { 
        docheck21 = 1;
	  } 
	  if(area_scrub_found>0)
	  { 
        docheck22 = 1;
	  } 
	}


    if(ATTRTYPE_MGCP4())
	{
	   docheck23 = ComputeAdditionalVV1_MGCP();
	   



	   

	   if(
           (area_bridgeTRS[TRS0]  >0) ||
           (area_bridgeTRS[TRS2]  >0) ||
           (area_bridgeTRS[TRS4]  >0) ||
           (area_bridgeTRS[TRS5]  >0) ||
           (area_bridgeTRS[TRS7]  >0) ||
           (area_bridgeTRS[TRS9]  >0) ||
           (area_bridgeTRS[TRS10] >0) ||
           (area_bridgeTRS[TRS12] >0) ||
           (area_bridgeTRS[TRS13] >0) ||
           (area_bridgeTRS[TRS998]>0) ||

           (area_tunnelTRS[TRS0]  >0) ||
           (area_tunnelTRS[TRS2]  >0) ||
           (area_tunnelTRS[TRS4]  >0) ||
           (area_tunnelTRS[TRS5]  >0) ||
           (area_tunnelTRS[TRS7]  >0) ||
           (area_tunnelTRS[TRS9]  >0) ||
           (area_tunnelTRS[TRS10] >0) ||
           (area_tunnelTRS[TRS12] >0) ||
           (area_tunnelTRS[TRS13] >0) ||
           (area_tunnelTRS[TRS998]>0) ||
		   
		   (line_bridgeTRS[TRS1] >0) ||
           (line_bridgeTRS[TRS4] >0) ||
           (line_bridgeTRS[TRS7] >0) ||

           (line_causewaystrTRS[TRS1] >0) ||
           (line_causewaystrTRS[TRS4] >0) ||
           (line_causewaystrTRS[TRS7] >0) ||

           (line_tunnelTRS[TRS1] >0) ||
           (line_tunnelTRS[TRS4] >0) ||
           (line_tunnelTRS[TRS7] >0) ||

           (area_damTRS[TRS1] >0) ||
           (area_damTRS[TRS4] >0) ||
           (area_damTRS[TRS7] >0) ||

           (line_damTRS[TRS1] >0) ||
           (line_damTRS[TRS4] >0) ||
           (line_damTRS[TRS7] >0) ||

           (point_damTRS[TRS1] >0) ||
           (point_damTRS[TRS4] >0) ||
           (point_damTRS[TRS7] >0) ||

           (area_embankmentTRS[TRS1] >0) ||
           (area_embankmentTRS[TRS4] >0) ||
           (area_embankmentTRS[TRS7] >0) ||

           (line_embankmentTRS[TRS1] >0) ||
           (line_embankmentTRS[TRS4] >0) ||
           (line_embankmentTRS[TRS7] >0) ||

           (line_cutTRS[TRS1] >0) ||
           (line_cutTRS[TRS4] >0) ||
           (line_cutTRS[TRS7] >0) ||

		   (line_ferrycrossingTRS[TRS1]  >0) ||
		   (line_ferrycrossingTRS[TRS2]  >0) ||
           (line_ferrycrossingTRS[TRS4]  >0) ||
           (line_ferrycrossingTRS[TRS5]  >0) ||
           (line_ferrycrossingTRS[TRS10] >0) ||
           (line_ferrycrossingTRS[TRS12] >0) ||
           (line_ferrycrossingTRS[TRS13] >0) ||

		   (point_ferrycrossingTRS[TRS1]  >0) ||
		   (point_ferrycrossingTRS[TRS2]  >0) ||
           (point_ferrycrossingTRS[TRS4]  >0) ||
           (point_ferrycrossingTRS[TRS5]  >0) ||
           (point_ferrycrossingTRS[TRS7]  >0) ||
           (point_ferrycrossingTRS[TRS10] >0) ||
           (point_ferrycrossingTRS[TRS12] >0) ||
           (point_ferrycrossingTRS[TRS13] >0) ||

		   (area_transStationTRS[TRS1]  >0) ||
		   (area_transStationTRS[TRS2]  >0) ||
           (area_transStationTRS[TRS4]  >0) ||
           (area_transStationTRS[TRS5]  >0) ||
           (area_transStationTRS[TRS7]  >0) ||
           (area_transStationTRS[TRS9]  >0) ||
           (area_transStationTRS[TRS10] >0) ||

		   (point_transStationTRS[TRS1]  >0) ||
		   (point_transStationTRS[TRS2]  >0) ||
           (point_transStationTRS[TRS4]  >0) ||
           (point_transStationTRS[TRS5]  >0) ||
           (point_transStationTRS[TRS7]  >0) ||
           (point_transStationTRS[TRS9]  >0) ||
           (point_transStationTRS[TRS10] >0)
		 )
	   {
		  docheck24 = 1;
	   }
	}


   
   for(i=0;i<NumberOfModels;i++)
   {
	 if(docheck25>0)
	 {
       break;
	 }

     for(k=0;k<MdlNames[i].numattributes;k++)
	 { 	   
	   thisindex = MdlNames[i].AttrIndices[k];
	   thatindex = MdlNames[i].ValueIndices[k];

       
	     
	   if(
		  (strcmp(GetEACCode(MdlNames2[thisindex].code),"SDV")) &&
		  (strcmp(GetEACCode(MdlNames2[thisindex].code),"SRC_DATE"))
		 )
	   {
         continue;
	   }

       if(MdlNames2[thisindex].values[thatindex].type==2) 
	   { 
         sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

		 cleancode[11] =  '"'; 
		 cleancode[12] = '\0';

 	     if(BadMGCPDate(cleancode))
		 {
           docheck25 = 1;
		   break;
		 }
	   } 
       else
	   { 
         continue;
	   } 			
	 } 
   } 




   if(ATTRTYPE_MGCP3()==0)
   {
     for(i=0;i<NumberOfModels;i++)
	 { 
	   if(docheck26>0)
	   { 
         break;
	   } 

       for(k=0;k<MdlNames[i].numattributes;k++)
	   {  	   
	     thisindex = MdlNames[i].AttrIndices[k];
	     thatindex = MdlNames[i].ValueIndices[k];

         

	     if(strcmp(GetEACCode(MdlNames2[thisindex].code),"VOI"))
		 {
           continue;
		 }

         if(MdlNames2[thisindex].values[thatindex].type==2) 
		 {  
           sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

 	       if(
		 	   (!strcmp(cleancode,"\"N_A\"")) ||
               (!strcmp(cleancode,"\"N/A\""))
		     )
		   {
             docheck26 = 1;
		     break;
		   } 
		 }  
         else
		 {  
           continue;
		 }  			
	   } 
	 } 
   }


   if(ATTRTYPE_MGCP3()==0)
   {
	 if(
		 (
		   area_tidal_nam_sentinel1  +
		   area_tidal_nam_sentinel2  +
		   area_tidal_nam_sentinel3  +
		   area_tidal_nam_sentinel4  +
		   area_tidal_nam_sentinel5  +
		   area_tidal_nam_sentinel6  +
		   area_tidal_nam_sentinel7  +
		   area_tidal_nam_sentinel8  +
		   area_tidal_nam_sentinel9  +
		   area_tidal_nam_sentinel10 +
		   area_tidal_nam_sentinel11 +
		   area_tidal_nam_sentinel12 +
		   area_tidal_nam_sentinel13 +
		   area_tidal_nam_sentinel14 +
		   area_tidal_nam_sentinel15 +
		   area_tidal_nam_sentinel16 +
		   area_tidal_nam_sentinel17 +
		   area_tidal_nam_sentinel18 +
		   area_tidal_nam_sentinel19 +
		   area_tidal_nam_sentinel20
		 )>0
       )
	 {
       docheck27 = 1;
	 }
   }


    totalchecks = docheck1  + docheck1a + docheck2  + docheck3  + docheck4  + docheck5  + 
		          docheck6  + docheck7  + docheck8  + docheck9  + docheck10 + docheck11 + 
				  docheck12 + docheck13 + docheck14 + docheck15 + docheck16 + docheck17 + 
				  docheck18 + docheck19 + docheck20 + docheck21 + docheck22 + docheck23 +
				  docheck24 + docheck25 + docheck26 + docheck27;

    if(totalchecks>0)
	  { 
	    fprintf(newfile,"UNIQUE_CHECKS 1\n");
	    fprintf(newfile,"VVTERR1WAY %d\n",totalchecks);

		j = 1;

	    for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
		{ 

          if(do_lower==0)
		  { 
            break;
		  }


          if(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]!=199999991)
		  { 
            


 

		    lowval = GetLowVal(MGCP_ranges[i+4],&include_end_value);

		    if((include_end_value==1)&&(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]<=lowval))
			{ 
		      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

	          
              fcode_index = MGCP_RANGE_FCODES[i];

			  if(MGCP_ranges[i+6][0]=='E')
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): %s %s with %s less than or equal to %.4lf\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2],lowval);
			  }
			  else
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Warning (NVR): %s %s with %s less than or equal to %.4lf\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2],lowval);
			  }
			  j = j + 1;
			}
		    else if((include_end_value==0)&&(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]<lowval))
			{
		      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

 	          
              fcode_index = MGCP_RANGE_FCODES[i];

			  if(MGCP_ranges[i+6][0]=='E')
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): %s %s with %s less than %.4lf\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2],lowval);
			  }
			  else
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Warning (NVR): %s %s with %s less than %.4lf\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2],lowval);
			  }
			  j = j + 1;
			} 
		  } 
		}




        

		









	    for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
		{ 

          if(do_lower==0)
		  { 
            break;
		  }

		  if(docheck1a<=0) 
		  {
			break;
		  }

          if(
			 (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1) || 
             (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1) || 
             (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)    
            )
		  {

		    fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
	        fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
	        fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
	        fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
	        fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
	        fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

            fcode_index = MGCP_RANGE_FCODES[i];

		    if(MGCP_ranges[i+6][0]=='E')
			{  
              fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): %s %s with invalid sentinel value for %s\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2]);
			} 
		    else
			{ 
              fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Warning (NVR): %s %s with invalid sentinel value for %s\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2]);
			} 
			j = j + 1;
		  }
		}






		


        










	    for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
		{

          if(do_higher==0)
		  { 
            break;
		  }


          if(MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]!=-199999993)
		  { 
            


 

		    highval = GetHighVal(MGCP_ranges[i+5]);

		    if(MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]>highval)
			{ 
		      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
	          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

	          
              fcode_index = MGCP_RANGE_FCODES[i];

			  if(MGCP_ranges[i+6][0]=='E')
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): %s %s with %s greater than%s%.4lf\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2],GetEqualString(MGCP_ranges[i+5]),highval);
			  }
			  else
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Warning (NVR): %s %s with %s greater than%s%.4lf\"\n",j,
				  GetRangeGeomType(2,MGCP_ranges[i+1]),GetECCLabel(fcode_index),MGCP_ranges[i+2],GetEqualString(MGCP_ranges[i+5]),highval);
			  }
			  j = j + 1;
			}
		  } 
		}

		if(docheck3>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): Features with ACE less than or equal to 0\"\n",j);
          j = j + 1;
		}

		if(docheck4>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): Features with ACE greater than 25\"\n",j);
          j = j + 1;
		}

		if(docheck5>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): Features with ALE less than or equal to 0\"\n",j);
          j = j + 1;
		}

		if(docheck6>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Error (NVR): Features with ALE greater than 25\"\n",j);
          j = j + 1;
		}


        if(docheck7>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point Aqueduct with %s = [3.0-%lf]\"\n",j,GetLengthCode(),maxWID);
          j = j + 1;
		}


        if(docheck8>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Line Tunnel with ACC = Accurate\"\n",j);
          j = j + 1;
		}

        if(docheck9>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point %s with NAM = \"Unknown\"\"\n",j,GetBUAName(1));
          j = j + 1;
		}
        if(docheck10>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point %s with NAM = \"Not Applicable\"\"\n",j,GetBUAName(1));
          j = j + 1;
		}
        if(docheck11>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point Settlement or Named Location with NAM = \"Unknown\"\"\n",j);
          j = j + 1;
		}
        if(docheck12>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point Settlement or Named Location with NAM = \"Not Applicable\"\"\n",j);
          j = j + 1;
		}

        if(docheck13>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area Mangrove Swamp without TID = \"Tidal/Tidal Fluctuating\"\"\n",j);
          j = j + 1;
		}

        if(docheck14>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area %s or Tunnel without TRS = \"Aeronautical\"\"\n",j,GetBridgeName(1));
          j = j + 1;
		}

        if(docheck15>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point Aqueduct without ATC = \"%s\"\"\n",j,GetQanatName(1));
          j = j + 1;
		}

        if(docheck17>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area %s with RKF = \"Pinnacle\"\"\n",j,GetRockName(1));
          j = j + 1;
		}


        if(docheck18>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area %s with DMT other than 25 or 51\"\n",j,GetOrchardName(1));
          j = j + 1;
		}
        if(docheck19>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area %s with DMT other than 25 or 51\"\n",j,GetTreesName(1));
          j = j + 1;
		}
        if(docheck20>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area Swamp with DMT other than 25 or 51\"\n",j);
          j = j + 1;
		}
        if(docheck21>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area Mangrove Swamp with DMT other than 25 or 51\"\n",j);
          j = j + 1;
		}
        if(docheck22>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area %s with DMB other than 25 or 51\"\n",j,GetScrubName(1));
          j = j + 1;
		}

		if(docheck23>0)
		{
          for(i=0;i<MGCP4_badsingleNum*5;i=i+5)
		  { 
            if(MGCP4_badvals2[i]>0)
			{

              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

	          thiscodeindex = GetCodeIndex(MGCP4_badvals[i]);

              sprintf(cleancode,"%s",GetECCLabel(thiscodeindex));


	          if(MGCP4_badvals[i+1][0]=='L')
			  { 
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Line %s with %s = \"%s\"\"\n",j,
					cleancode,MGCP4_badvals[i+2],MGCP4_badvals[i+4]);
			  }
			  else
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area %s with %s = \"%s\"\"\n",j,
					cleancode,MGCP4_badvals[i+2],MGCP4_badvals[i+4]);
			  }

              j = j + 1;
			}
		  }
		}

        if(docheck24>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features with invalid TRS values\"\n",j);
          j = j + 1;
		}

        if(docheck25>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
		  if(ATTRTYPE_MGCP3())
		  {
            fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features with invalid \"SRC_DATE\" (invalid CCYY-MM-DD format or year < 1900)\"\n",j);
		  }
		  else
		  {
            fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features with invalid \"SDV\" (invalid CCYY-MM-DD format or year < 1900)\"\n",j);
		  }
          j = j + 1;
		}

        if(docheck26>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features with VOI = \"N_A\" or \"N/A\"\"\n",j);
          j = j + 1;
		}

        if(docheck27>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area %s with invalid (sentinel) NAM value\"\n",j,GetTidalWaterName(1));
          j = j + 1;
		}

	    fprintf(newfile,"ATTRATTRATTR\n");
	    
	    j = 1;

	    for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
		{ 

          if(do_lower==0)
		  { 
            break;
		  }


          if(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]!=199999991)
		  { 
            

 

		    lowval = GetLowVal(MGCP_ranges[i+4],&include_end_value);


		    if((include_end_value==0)&&(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]<lowval))
			{ 
	          
              fcode_index = MGCP_RANGE_FCODES[i];

              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  MGCP_range_low[i/MGCP_RANGE_MULTIPLIER],lowval,GetRangeGeomType(1,MGCP_ranges[i+1]));



			  if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],5)==1)
			  {
				if(RealClose(lowval,0))
				{
			      


				    if(ExcludeRES(i,"0",1)==1)
					{
					  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  lowval,lowval,GetRangeGeomType(1,MGCP_ranges[i+1]));
					}
				}
			  }

			  if(MGCP_ranges[i+3][0]=='Y')
			  {
                 if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1)
				 {
                   

				   if(ExcludeRES(i,"-32764",1)==1)
				   {
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32764 -32764 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   }
				 }
                 if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1)
				 {
                   
				   if(ExcludeRES(i,"-32765",1)==1)
				   {
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32765 -32765 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   }
				 }
                 if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)
				 {
                   
				   if(ExcludeRES(i,"-32767",1)==1)
				   {
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32767 -32767 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   }
				 }



                 if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],4)==1)
				 {

                   if(ATTRIBUTION_TYPE==7)
				   { 
                     
				     if(ExcludeRES(i,"-32768",1)==1)
					 { 
                       fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32768 -32768 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
					 } 
				   }
				 }
			  } 

			  j = j + 1;
			} 

		    else if((include_end_value==1)&&(MGCP_range_low[i/MGCP_RANGE_MULTIPLIER]<=lowval))
			{
 	          
              fcode_index = MGCP_RANGE_FCODES[i];

              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  MGCP_range_low[i/MGCP_RANGE_MULTIPLIER],lowval,GetRangeGeomType(1,MGCP_ranges[i+1]));

			  if(MGCP_ranges[i+3][0]=='Y')
			  {

				 if(!strcmp(MGCP_ranges[i+2],"BRF"))
				 {

                   
				   


				   if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-2147483648.0,-2147483648.0,0,&smalljunkflag,-999,&djunk,-999)>0)
				   { 
                     
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -2147483648 -2147483648 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   } 
				   if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-2147483647.0,-2147483647.0,0,&smalljunkflag,-999,&djunk,-999)>0)
				   { 
                     
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -2147483647 -2147483647 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   } 
				   if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-2147483645.0,-2147483645.0,0,&smalljunkflag,-999,&djunk,-999)>0)
				   { 
                     
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -2147483645 -2147483645 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   } 
				   if(NUM_Getter(MGCP_ranges[i+2],MGCP_ranges[i+1],GetECCLabel(fcode_index),&minWID,&maxWID,-2147483644.0,-2147483644.0,0,&smalljunkflag,-999,&djunk,-999)>0)
				   { 
                     
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -2147483644 -2147483644 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
				   } 


				 }
				 else
				 {
                   if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1)
				   {
                     
				     if(ExcludeRES(i,"-32764",1)==1)
					 {
                       fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32764 -32764 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
					 }
				   } 
                   if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1)
				   { 
                     
				     if(ExcludeRES(i,"-32765",1)==1)
					 {
                       fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32765 -32765 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
					 }
				   } 
                   if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)
				   { 
                     
				     if(ExcludeRES(i,"-32767",1)==1)
					 {
                       fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32767 -32767 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
					 }
				   } 



                   if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],4)==1)
				   { 

                     if(ATTRIBUTION_TYPE==7)
					 {   
                       
				       if(ExcludeRES(i,"-32768",1)==1)
					   { 
                         fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32768 -32768 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
					   }
					 }
				   } 
				 }
			  } 

			  j = j + 1;

			} 
		  } 
		} 









        

		





	    for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
		{ 

          if(do_lower==0)
		  { 
            break;
		  }

		  if(docheck1a<=0) 
		  {
			break;
		  }

		  
          if(
			 (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1) || 
             (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1) || 
             (GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)    
            )
		  {
            fcode_index = MGCP_RANGE_FCODES[i];

            if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1)
			{ 
              
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32764 -32764 on %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
			}  
            if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1)
			{  
             
             fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32765 -32765 on %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
			} 
            if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)
			{  
             
             fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32767 -32767 on %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
  							  GetRangeGeomType(1,MGCP_ranges[i+1]));
			} 


			

		    if(ExcludeRES(i,"LOC=40",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
                       GetLOCName(0),GetUnderGroundName(0),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"FUN=1",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ConditionofFacility LABEL UnderConstruction off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"FUN=2",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ConditionofFacility LABEL Abandoned off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"FUN=3",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ConditionofFacility LABEL Destroyed off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"FUN=4",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ConditionofFacility LABEL Dismantled off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"CDA=1001",2)==1)
			{ 
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL Covered off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetCDACode(0),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}

		    if(ExcludeRES(i,"TRS=0",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Unknown off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=1",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Aeronautical off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=2",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Aqueduct off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=5",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Canal off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=7",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Maritime off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=9",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Pedestrian off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=10",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Pipeline off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=998",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL NotApplicable off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"TRS=999",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL Other off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}


		    if(ExcludeRES(i,"SDT=0",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s SandDuneType LABEL Unknown off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"SDT=2",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s SandDuneType LABEL Dome off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}
		    if(ExcludeRES(i,"SDT=7",2)==1)
			{
               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s SandDuneType LABEL Star off %s\n",j,
					   MakeCleanCode (GetECCLabel(fcode_index)),
					   GetRangeGeomType(1,MGCP_ranges[i+1]));
			}




            if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],1)==1)
			{
			  
              if(strstr(MGCP_ranges[i+7],"-32764"))
			  {
				
                fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32764 -32764 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
			  }
			}
            if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],2)==1)
			{
			  
              if(strstr(MGCP_ranges[i+7],"-32765"))
			  {
				
                fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32765 -32765 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
			  }
			}
            if(GetReservedBit(MGCP_range_res[i/MGCP_RANGE_MULTIPLIER],3)==1)
			{
			  
              if(strstr(MGCP_ranges[i+7],"-32767"))
			  {
				
                fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE -32767 -32767 off %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  GetRangeGeomType(1,MGCP_ranges[i+1]));
			  }
			}



			j = j + 1;
		  }


		}




        

		







	    for(i=0;i<MGCP_RANGE_SPECS*MGCP_RANGE_MULTIPLIER;i=i+MGCP_RANGE_MULTIPLIER)
		{ 

          if(do_higher==0)
		  { 
            break;
		  }

          if(MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]!=-199999993)
		  { 
            

		    highval = GetHighVal(MGCP_ranges[i+5]);

		    if(MGCP_range_high[i/MGCP_RANGE_MULTIPLIER]>highval)
			{
	          
              fcode_index = MGCP_RANGE_FCODES[i];

              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on %s\n",j,
				  			  MakeCleanCode (GetECCLabel(fcode_index)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex(MGCP_ranges[i+2]))),
							  MGCP_range_high2[i/MGCP_RANGE_MULTIPLIER],MGCP_range_high[i/MGCP_RANGE_MULTIPLIER],GetRangeGeomType(1,MGCP_ranges[i+1]));

			  j = j + 1;

			} 
		  } 
		} 


		if((docheck3+docheck4+docheck5+docheck6)>0)
		{

          
          
          
          
          

          for(i=0;i<INscc_loop;i++)
		  { 
	        if(pointspresent[i]==1)
			{ 
              if((docheck3+docheck4)>0)
			  {
                if(NUM_Getter("ACE","(POINT)",GetECCLabel(i),&minWID,&maxWID,NO_U_MIN,NO_U_MAX,1,&smalljunkflag,-999,&djunk,-999)>0) 
				{ 
                  if(minWID<=0)
				  {  
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_POFE\n",j,
                       MakeCleanCode (GetECCLabel(i)),
                       MakeCleanCode2(GetEACLabel(GetAttrIndex("ACE"))),
                       minWID,0.0);
				  } 
                  if(maxWID>=25.1)
				  { 
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_POFE\n",j+docheck3,
                       MakeCleanCode (GetECCLabel(i)),
                       MakeCleanCode2(GetEACLabel(GetAttrIndex("ACE"))),
                       25.1,maxWID);
				  } 
				} 
			  }

              if((docheck5+docheck6)>0)
			  {
                if(NUM_Getter("ALE","(POINT)",GetECCLabel(i),&minWID,&maxWID,NO_U_MIN,NO_U_MAX,1,&smalljunkflag,-999,&djunk,-999)>0) 
				{ 
                  if(minWID<=0)
				  { 
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_POFE\n",j+docheck3+docheck4,
                      MakeCleanCode (GetECCLabel(i)),
                      MakeCleanCode2(GetEACLabel(GetAttrIndex("ALE"))),
                      minWID,0.0);
				  } 
                  if(maxWID>=25.1)
				  {  
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_POFE\n",j+docheck3+docheck4+docheck5,
                      MakeCleanCode (GetECCLabel(i)),
                      MakeCleanCode2(GetEACLabel(GetAttrIndex("ALE"))),
                      25.1,maxWID);
				  } 
				} 
			  }
			} 



	        if(linespresent[i]==1)
			{ 
              if((docheck3+docheck4)>0)
			  {
                if(NUM_Getter("ACE","(LINE)",GetECCLabel(i),&minWID,&maxWID,NO_U_MIN,NO_U_MAX,1,&smalljunkflag,-999,&djunk,-999)>0) 
				{ 
                  if(minWID<=0)
				  {  
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",j,
                       MakeCleanCode (GetECCLabel(i)),
                       MakeCleanCode2(GetEACLabel(GetAttrIndex("ACE"))),
                       minWID,0.0);
				  } 
                  if(maxWID>=25.1)
				  { 
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",j+docheck3,
                       MakeCleanCode (GetECCLabel(i)),
                       MakeCleanCode2(GetEACLabel(GetAttrIndex("ACE"))),
                       25.1,maxWID);
				  } 
				} 
			  }

              if((docheck5+docheck6)>0)
			  {
                if(NUM_Getter("ALE","(LINE)",GetECCLabel(i),&minWID,&maxWID,NO_U_MIN,NO_U_MAX,1,&smalljunkflag,-999,&djunk,-999)>0) 
				{ 
                  if(minWID<=0)
				  { 
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",j+docheck3+docheck4,
                      MakeCleanCode (GetECCLabel(i)),
                      MakeCleanCode2(GetEACLabel(GetAttrIndex("ALE"))),
                      minWID,0.0);
				  } 
                  if(maxWID>=25.1)
				  {  
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",j+docheck3+docheck4+docheck5,
                      MakeCleanCode (GetECCLabel(i)),
                      MakeCleanCode2(GetEACLabel(GetAttrIndex("ALE"))),
                      25.1,maxWID);
				  } 
				} 
			  }
			} 



	        if(areaspresent[i]==1)
			{ 
              if((docheck3+docheck4)>0)
			  {
                if(NUM_Getter("ACE","(AREA)",GetECCLabel(i),&minWID,&maxWID,NO_U_MIN,NO_U_MAX,1,&smalljunkflag,-999,&djunk,-999)>0) 
				{ 
                  if(minWID<=0)
				  {  
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_AREA\n",j,
                       MakeCleanCode (GetECCLabel(i)),
                       MakeCleanCode2(GetEACLabel(GetAttrIndex("ACE"))),
                       minWID,0.0);
				  } 
                  if(maxWID>=25.1)
				  { 
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_AREA\n",j+docheck3,
                       MakeCleanCode (GetECCLabel(i)),
                       MakeCleanCode2(GetEACLabel(GetAttrIndex("ACE"))),
                       25.1,maxWID);
				  } 
				} 
			  }

              if((docheck5+docheck6)>0)
			  {
                if(NUM_Getter("ALE","(AREA)",GetECCLabel(i),&minWID,&maxWID,NO_U_MIN,NO_U_MAX,1,&smalljunkflag,-999,&djunk,-999)>0) 
				{ 
                  if(minWID<=0)
				  { 
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_AREA\n",j+docheck3+docheck4,
                      MakeCleanCode (GetECCLabel(i)),
                      MakeCleanCode2(GetEACLabel(GetAttrIndex("ALE"))),
                      minWID,0.0);
				  } 
                  if(maxWID>=25.1)
				  {  
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_AREA\n",j+docheck3+docheck4+docheck5,
                      MakeCleanCode (GetECCLabel(i)),
                      MakeCleanCode2(GetEACLabel(GetAttrIndex("ALE"))),
                      25.1,maxWID);
				  } 
				} 
			  }
			} 

		  } 

          j = j + docheck3 + docheck4 + docheck5 + docheck6;

		} 


		if(docheck7>0)
		{
          fprintf(newfile,"VVTERR1WAY %d PRIMARY Aqueduct %s RANGE 3.0 %lf on C_POFE\n",j,
			                  MakeCleanCode(GetEACLabel(GetAttrIndex(GetLengthCode()))),
							  saveAQwid);
		  j = j + 1;
		}


		if(docheck8>0)
		{
          fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel %s LABEL Accurate on C_LINE\n",
				  j,MakeCleanCode(GetEACLabel(GetAttrIndex("ACC"))));
		  j = j + 1;
		}



		if(docheck9>0)
		{
		  if(point_bua_nam_unk>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Unknown\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
		  if(point_bua_nam_unk1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Unk\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
		  if(point_bua_nam_unk2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"UNK\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
		  if(point_bua_nam_unk3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"unk\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
		  if(point_bua_nam_unk4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"UNKNOWN\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }

		  j = j + 1;
		}

		if(docheck10>0)
		{
          if(point_bua_nam_na>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Not Applicable\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
          if(point_bua_nam_na1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NA\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
          if(point_bua_nam_na2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"N_A\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
          if(point_bua_nam_na3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"n_a\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
          if(point_bua_nam_na4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"na\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }
          if(point_bua_nam_na5>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NOT APPLICABLE\" on C_POFE\n",
				  j,GetBUAName(0),MakeCleanCode(GetNAMName()));
		  }

		  j = j + 1;
		}

		if(docheck11>0)
		{
		  if(point_sett_nam_unk>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"Unknown\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_sett_nam_unk1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"Unk\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_sett_nam_unk2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"UNK\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_sett_nam_unk3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"unk\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_sett_nam_unk4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"UNKNOWN\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }


		  if(point_named_nam_unk>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"Unknown\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_named_nam_unk1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"Unk\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_named_nam_unk2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"UNK\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_named_nam_unk3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"unk\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
		  if(point_named_nam_unk4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"UNKNOWN\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }



		  j = j + 1;
		}

		if(docheck12>0)
		{
          if(point_sett_nam_na>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"Not Applicable\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_sett_nam_na1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"NA\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_sett_nam_na2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"N_A\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_sett_nam_na3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"n_a\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_sett_nam_na4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"na\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_sett_nam_na5>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Settlement %s STRING \"NOT APPLICABLE\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }


          if(point_named_nam_na>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"Not Applicable\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_named_nam_na1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"NA\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_named_nam_na2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"N_A\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_named_nam_na3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"n_a\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_named_nam_na4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"na\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }
          if(point_named_nam_na5>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING \"NOT APPLICABLE\" on C_POFE\n",j,MakeCleanCode(GetNAMName()));
		  }

		  j = j + 1;
		}

		if(docheck13>0)
		{
          if(area_mangrove_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY MangroveSwamp ANY on C_AREA\n",j);

		    if(area_mangroveTID1001_found>0)
			{
	          fprintf(newfile,"VVTERR1WAY %d PRIMARY MangroveSwamp %s LABEL TidalTidalFluctuating off C_AREA\n",j,MakeCleanCode(GetEACLabel(GetAttrIndex("TID"))));
			} 
		  }
		  j = j + 1;
		}

		if(docheck14>0)
		{
          if(area_bridge_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_AREA\n",j,GetBridgeName(0));

		    if(area_1bridgefound>0)
			{
	          fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL Aeronautical off C_AREA\n",j,GetBridgeName(0),GetTRS1Name());
			} 
		  }

          if(area_tunnel_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel ANY on C_AREA\n",j);

		    if(area_1tunnelfound>0)
			{
	          fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel %s LABEL Aeronautical off C_AREA\n",j,GetTRS1Name());
			} 
		  }
		  j = j + 1;
		}

		if(docheck15>0)
		{
          if(point_aqueduct_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Aqueduct ANY on C_POFE\n",j);

		    if(point_atc1aqueduct>0)
			{
	          fprintf(newfile,"VVTERR1WAY %d PRIMARY Aqueduct %s LABEL %s off C_POFE\n",j,GetATCName(0),GetQanatName(0));
			} 
		  }
		  j = j + 1;
		}


		if(docheck17>0)
		{
          if(area_rkf3rock>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL Pinnacle on C_AREA\n",j,GetRockName(0),GetRKFName(0));
		  }
		  j = j + 1;
		}


		if(docheck18>0)
		{
          if(area_orchard_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_AREA\n",j,GetOrchardName(0));

            if(NUM_Getter("DMT","(AREA)",GetOrchardName(1),&junk1,&junk2,25.0,25.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 25.0 25.0 off C_AREA\n",j,GetOrchardName(0),GetDMTName(0));
			}
            if(NUM_Getter("DMT","(AREA)",GetOrchardName(1),&junk1,&junk2,51.0,51.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 51.0 51.0 off C_AREA\n",j,GetOrchardName(0),GetDMTName(0));
			}
		  }
		  j = j + 1;
		}

		if(docheck19>0)
		{
          if(area_trees_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_AREA\n",j,GetTreesName(0));

            if(NUM_Getter("DMT","(AREA)",GetTreesName(1),&junk1,&junk2,25.0,25.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 25.0 25.0 off C_AREA\n",j,GetTreesName(0),GetDMTName(0));
			}
            if(NUM_Getter("DMT","(AREA)",GetTreesName(1),&junk1,&junk2,51.0,51.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 51.0 51.0 off C_AREA\n",j,GetTreesName(0),GetDMTName(0));
			}
		  }
		  j = j + 1;
		}

		if(docheck20>0)
		{
          if(area_swamp_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY Swamp ANY on C_AREA\n",j);

            if(NUM_Getter("DMT","(AREA)","Swamp",&junk1,&junk2,25.0,25.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY Swamp %s RANGE 25.0 25.0 off C_AREA\n",j,GetDMTName(0));
			}
            if(NUM_Getter("DMT","(AREA)","Swamp",&junk1,&junk2,51.0,51.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY Swamp %s RANGE 51.0 51.0 off C_AREA\n",j,GetDMTName(0));
			}
		  }
		  j = j + 1;
		}

		if(docheck21>0)
		{
          if(area_mangrove_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY MangroveSwamp ANY on C_AREA\n",j);

            if(NUM_Getter("DMT","(AREA)","Mangrove Swamp",&junk1,&junk2,25.0,25.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY MangroveSwamp %s RANGE 25.0 25.0 off C_AREA\n",j,GetDMTName(0));
			}
            if(NUM_Getter("DMT","(AREA)","Mangrove Swamp",&junk1,&junk2,51.0,51.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY MangroveSwamp %s RANGE 51.0 51.0 off C_AREA\n",j,GetDMTName(0));
			}
		  }
		  j = j + 1;
		}

		if(docheck22>0)
		{
          if(area_scrub_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_AREA\n",j,GetScrubName(0));

            if(NUM_Getter("DMB","(AREA)",GetScrubName(1),&junk1,&junk2,25.0,25.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 25.0 25.0 off C_AREA\n",j,GetScrubName(0),GetDMBName(0));
			}
            if(NUM_Getter("DMB","(AREA)",GetScrubName(1),&junk1,&junk2,51.0,51.0,0,&smalljunkflag,-999,&djunk,-999)>0)
			{
              fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 51.0 51.0 off C_AREA\n",j,GetScrubName(0),GetDMBName(0));
			}
		  }
		  j = j + 1;
		}



		if(docheck23>0)
		{

          for(i=0;i<MGCP4_badsingleNum*5;i=i+5)
		  { 
            if(MGCP4_badvals2[i]>0)
			{

              thiscodeindex = GetCodeIndex(MGCP4_badvals[i]);

              sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(thiscodeindex)));


	          if(MGCP4_badvals[i+1][0]=='L')
			  { 
	            if(MGCP4_badvals[i+3][0]=='1')
				{ 
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on C_LINE\n",
					  j,
					  cleancode,
					  MakeCleanCode(GetEACLabel(GetAttrIndex(MGCP4_badvals[i+2]))),
					  MakeCleanCode2(MGCP4_badvals[i+4]));
				}
				else
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %s %s on C_LINE\n",
					  j,
					  cleancode,
					  MakeCleanCode(GetEACLabel(GetAttrIndex(MGCP4_badvals[i+2]))),
					  MGCP4_badvals[i+4],MGCP4_badvals[i+4]);
				}
			  }
			  else
			  {
	            if(MGCP4_badvals[i+3][0]=='1')
				{ 
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on C_AREA\n",
					  j,
					  cleancode,
					  MakeCleanCode(GetEACLabel(GetAttrIndex(MGCP4_badvals[i+2]))),
					  MakeCleanCode2(MGCP4_badvals[i+4]));
				}
				else
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %s %s on C_AREA\n",
					  j,
					  cleancode,
					  MakeCleanCode(GetEACLabel(GetAttrIndex(MGCP4_badvals[i+2]))),
					  MGCP4_badvals[i+4],MGCP4_badvals[i+4]);
				}
			  }

              j = j + 1;
			}
		  }
		}


		if(docheck24>0)
		{

		   if(area_bridgeTRS[TRS0]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS0NAME));}
		   if(area_bridgeTRS[TRS2]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS2NAME));}
           if(area_bridgeTRS[TRS4]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS4NAME));}
           if(area_bridgeTRS[TRS5]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS5NAME));}
           if(area_bridgeTRS[TRS7]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS7NAME));}
           if(area_bridgeTRS[TRS9]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS9NAME));}
           if(area_bridgeTRS[TRS10] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS10NAME));}
           if(area_bridgeTRS[TRS12] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS12NAME));}
           if(area_bridgeTRS[TRS13] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS13NAME));}
           if(area_bridgeTRS[TRS998]>0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetBridgeName(0),MakeCleanCode(TRS998NAME));}

           if(area_tunnelTRS[TRS0]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS0NAME));}
           if(area_tunnelTRS[TRS2]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS2NAME));}
           if(area_tunnelTRS[TRS4]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS4NAME));}
           if(area_tunnelTRS[TRS5]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS5NAME));}
           if(area_tunnelTRS[TRS7]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS7NAME));}
           if(area_tunnelTRS[TRS9]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS9NAME));}
           if(area_tunnelTRS[TRS10] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS10NAME));}
           if(area_tunnelTRS[TRS12] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS12NAME));}
           if(area_tunnelTRS[TRS13] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS13NAME));}
           if(area_tunnelTRS[TRS998]>0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS998NAME));}
		   
		   if(line_bridgeTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS1NAME));}
           if(line_bridgeTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS4NAME));}
           if(line_bridgeTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetBridgeName(0),MakeCleanCode(TRS7NAME));}

           if(line_causewaystrTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS1NAME));}
           if(line_causewaystrTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS4NAME));}
           if(line_causewaystrTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY CausewayStructure TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS7NAME));}

           if(line_tunnelTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS1NAME));}
           if(line_tunnelTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS4NAME));}
           if(line_tunnelTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Tunnel TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS7NAME));}

           if(area_damTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetDamName(0),MakeCleanCode(TRS1NAME));}
           if(area_damTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetDamName(0),MakeCleanCode(TRS4NAME));}
           if(area_damTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetDamName(0),MakeCleanCode(TRS7NAME));}

           if(line_damTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS1NAME));}
           if(line_damTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS4NAME));}
           if(line_damTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetDamName(0),MakeCleanCode(TRS7NAME));}

           if(point_damTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_POFE\n",j,GetDamName(0),MakeCleanCode(TRS1NAME));}
           if(point_damTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_POFE\n",j,GetDamName(0),MakeCleanCode(TRS4NAME));}
           if(point_damTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_POFE\n",j,GetDamName(0),MakeCleanCode(TRS7NAME));}

           if(area_embankmentTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetEmbankmentName(0),MakeCleanCode(TRS1NAME));}
           if(area_embankmentTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetEmbankmentName(0),MakeCleanCode(TRS4NAME));}
           if(area_embankmentTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_AREA\n",j,GetEmbankmentName(0),MakeCleanCode(TRS7NAME));}

           if(line_embankmentTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS1NAME));}
           if(line_embankmentTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS4NAME));}
           if(line_embankmentTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY %s TransportationSystemType LABEL %s on C_LINE\n",j,GetEmbankmentName(0),MakeCleanCode(TRS7NAME));}

           if(line_cutTRS[TRS1] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS1NAME));}
           if(line_cutTRS[TRS4] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS4NAME));}
           if(line_cutTRS[TRS7] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY Cut TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS7NAME));}

		   if(line_ferrycrossingTRS[TRS1]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS1NAME));}
		   if(line_ferrycrossingTRS[TRS2]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS2NAME));}
           if(line_ferrycrossingTRS[TRS4]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS4NAME));}
           if(line_ferrycrossingTRS[TRS5]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS5NAME));}
           if(line_ferrycrossingTRS[TRS10] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS10NAME));}
           if(line_ferrycrossingTRS[TRS12] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS12NAME));}
           if(line_ferrycrossingTRS[TRS13] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS13NAME));}
           if(line_ferrycrossingTRS[TRS998]>0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_LINE\n",j,MakeCleanCode(TRS998NAME));}

		   if(point_ferrycrossingTRS[TRS1]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS1NAME));}
		   if(point_ferrycrossingTRS[TRS2]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS2NAME));}
           if(point_ferrycrossingTRS[TRS4]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS4NAME));}
           if(point_ferrycrossingTRS[TRS5]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS5NAME));}
           if(point_ferrycrossingTRS[TRS7]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS7NAME));}
           if(point_ferrycrossingTRS[TRS10] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS10NAME));}
           if(point_ferrycrossingTRS[TRS12] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS12NAME));}
           if(point_ferrycrossingTRS[TRS13] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS13NAME));}
           if(point_ferrycrossingTRS[TRS998]>0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY FerryCrossing TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS998NAME));}

		   if(area_transStationTRS[TRS1]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS1NAME));}
		   if(area_transStationTRS[TRS2]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS2NAME));}
           if(area_transStationTRS[TRS4]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS4NAME));}
           if(area_transStationTRS[TRS5]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS5NAME));}
           if(area_transStationTRS[TRS7]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS7NAME));}
           if(area_transStationTRS[TRS9]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS9NAME));}
           if(area_transStationTRS[TRS10] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_AREA\n",j,MakeCleanCode(TRS10NAME));}

		   if(point_transStationTRS[TRS1]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS1NAME));}
		   if(point_transStationTRS[TRS2]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS2NAME));}
           if(point_transStationTRS[TRS4]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS4NAME));}
           if(point_transStationTRS[TRS5]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS5NAME));}
           if(point_transStationTRS[TRS7]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS7NAME));}
           if(point_transStationTRS[TRS9]  >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS9NAME));}
           if(point_transStationTRS[TRS10] >0) {fprintf(newfile,"VVTERR1WAY %d PRIMARY TransportationStation TransportationSystemType LABEL %s on C_POFE\n",j,MakeCleanCode(TRS10NAME));}

		   j = j + 1;
		}


        if(docheck25>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

			  if(
				  (!strcmp(GetEACCode(MdlNames2[thisindex].code),"SDV")) ||
				  (!strcmp(GetEACCode(MdlNames2[thisindex].code),"SRC_DATE"))
				)
			  {
                if(MdlNames2[thisindex].values[thatindex].type==2) 
				{
                  sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

		          cleancode[11] =  '"';  
		          cleancode[12] = '\0';

			      if(BadMGCPDate(cleancode))
				  { 
					
                    sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                        MakeCleanCode (GetECCLabel(MdlNames[i].code)),
						MakeCleanCode2(GetEACLabel(GetAttrIndex(GetEACCode(MdlNames2[thisindex].code)))),
                        cleancode,GetRangeGeomType(1,MdlNames[i].name));
				  } 
				} 
			  }
			}  
		  }  
		  j = j + 1;
		}  



        if(docheck26>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

			  if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"VOI"))
			  {
                if(MdlNames2[thisindex].values[thatindex].type==2) 
				{
                  sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			      if(
					  (!strcmp(cleancode,"\"N_A\"")) ||
					  (!strcmp(cleancode,"\"N/A\""))
					)
				  {
					
                    sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                        MakeCleanCode (GetECCLabel(MdlNames[i].code)),
						MakeCleanCode2(GetEACLabel(GetAttrIndex(GetEACCode(MdlNames2[thisindex].code)))),
                        cleancode,GetRangeGeomType(1,MdlNames[i].name));
				  } 
				} 
			  }
			}  
		  }  
		  j = j + 1;
		}  




		if(docheck27>0)
		{
          if(area_tidal_nam_sentinel1>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Unknown\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel2>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Unk\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel3>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"UNK\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel4>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"unk\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel5>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"UNKNOWN\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel6>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Not Applicable\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel7>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NA\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel8>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"N_A\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel9>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"n_a\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel10>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"na\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel11>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NOT APPLICABLE\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel12>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Not Populated\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel13>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NP\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel14>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"N_P\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel15>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"n_p\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel16>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"np\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel17>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NOT POPULATED\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel18>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"Null\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel19>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"N/A\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }
          if(area_tidal_nam_sentinel20>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING \"NULL\" on C_AREA\n",j,GetTidalWaterName(0),MakeCleanCode(GetNAMName()));
		  }

		  j = j + 1;
		}







	  } 
      else
	  {
	  fprintf(newfile,"Data not available to specify this check\n");
	  }
      fclose(newfile);

	  if(MGCP4_badvals2!=NULL)
	  {
        free(MGCP4_badvals2);
		MGCP4_badvals2 = NULL;
	  }

    } 	  





   if(type==1)
    {
      

	  if((USE_DOS==1)&&(SLASHTYPE==DOS_TYPE))
	  {
	    sprintf(message,"%sinspection_files\\HGT_Over_46m.txt",indirectory);
	  }
      else
	  {
	    sprintf(message,"%sinspection_files/HGT_Over_46m.txt",indirectory);
	  }

      newfile = fopen(message,"w");
      ExeUpdate(message);


	  


      for(i=1;i<=GIFD_CLASS_NUM;i++)
	  {
	    pointHGTmax[i] = -9999999;
	    lineHGTmax[i]  = -9999999;
	    areaHGTmax[i]  = -9999999;
	  }

	  docheck1 = 0;

      BuildLLTable(1,LLFidTableCount);

      Begin_LongFiles(LONG_ATTR_HGT);

      for(i=0;i<LLFidTableCount;i++)
	  {
  	    if(LLFidTable[i]<0)
		{
		  continue;
		}


        thistype = GetLongAttrVal(LONG_ATTR_HGT,i,&Ival,&Dval,&cleancode[0]);

		thisecc  = SCCtable[CrsWlk[LLFidTable[i]].LtoSmapping].ECC;
	    thisgeom =          CrsWlk[LLFidTable[i]].geomtype;

		if(thistype==I_TYPE)
        {
		  Dval = (double)Ival;
        }

		if( (Dval>maxHGT)&& ((thistype==I_TYPE)||(thistype==D_TYPE)) )
		{
		  
		  if(thisgeom==C_POFE)
		  {
			if(Dval>pointHGTmax[thisecc])
			 {
			   pointHGTmax[thisecc] = Dval;
			 }
		  }
		  else if(thisgeom==C_LINE)
		  {
			if(Dval>lineHGTmax[thisecc])
			 {
			   lineHGTmax[thisecc] = Dval;
			 }
		  }
		  else if(thisgeom==C_AREA)
		  {
			if(Dval>areaHGTmax[thisecc])
			 {
			   areaHGTmax[thisecc] = Dval;
			 }
		  }
		}
	  }

      FreeLLTable(); 
      End_LongFiles(LONG_ATTR_HGT);


      for(i=1;i<=GIFD_CLASS_NUM;i++)
	  {
		if(pointHGTmax[i]>maxHGT)
		{
		  
		  docheck1 = docheck1 + 1;
		}


		if(lineHGTmax[i]>maxHGT)
		{
		  
		  docheck1 = docheck1 + 1;
		}
		if(areaHGTmax[i]>maxHGT)
		{
		  
		  docheck1 = docheck1 + 1;
		}
	  }


      totalchecks = docheck1;
      
      if(totalchecks>0)
	  { 
	    fprintf(newfile,"UNIQUE_CHECKS 1\n");
	    fprintf(newfile,"VVTERR1WAY %d\n",totalchecks);

		j = 1;

		for(i=1;i<=GIFD_CLASS_NUM;i++)
		{
	 	  if(pointHGTmax[i]>maxHGT)
		  {
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
		    fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point %s with HGT 46-%lf\"\n",j,GetECCLabel(i),pointHGTmax[i]);
			j = j + 1;
		  }
	 	  if(lineHGTmax[i]>maxHGT)
		  {
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
		    fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Line %s with HGT 46-%lf\"\n",j,GetECCLabel(i),lineHGTmax[i]);
			j = j + 1;
		  }
	 	  if(areaHGTmax[i]>maxHGT)
		  {
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
            fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
		    fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area %s with HGT 46-%lf\"\n",j,GetECCLabel(i),areaHGTmax[i]);
			j = j + 1;
		  }
	  }

	  fprintf(newfile,"ATTRATTRATTR\n");
	  
	  
	  j = 1;

 	  for(i=1;i<=GIFD_CLASS_NUM;i++)
		{
	 	  if(pointHGTmax[i]>maxHGT)
		  {
	        fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_POFE\n",j,
				  			  MakeCleanCode (GetECCLabel(i)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
							  maxHGT,pointHGTmax[i]);

			j = j + 1;
		  }
	 	  if(lineHGTmax[i]>maxHGT)
		  {
	        fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_LINE\n",j,
				  			  MakeCleanCode (GetECCLabel(i)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
							  maxHGT,lineHGTmax[i]);
			j = j + 1;
		  }
	 	  if(areaHGTmax[i]>maxHGT)
		  {
	        fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on C_AREA\n",j,
				  			  MakeCleanCode (GetECCLabel(i)),
			                  MakeCleanCode2(GetEACLabel(GetAttrIndex("HGT"))),
							  maxHGT,areaHGTmax[i]);
			j = j + 1;
		  }
	  } 
	 
	  }
     else
	{
	  fprintf(newfile,"Data not available to specify this check\n");
	}
      fclose(newfile);
    }	  


    

    free(pointHGTres);
    free(lineHGTres);
    free(areaHGTres);
    free(pointHGTmax);
    free(lineHGTmax);
    free(areaHGTmax);
    free(pointHGTmin);
    free(lineHGTmin);
    free(areaHGTmin);

    free(pointLENres);
    free(lineLENres);
    free(areaLENres);
    free(pointLENmax);
    free(lineLENmax);
    free(areaLENmax);
    free(pointLENmin);
    free(lineLENmin);
    free(areaLENmin);

}






void DoVVTERR1WAY_TYPE5678(
		  int type,char *prefix,
		  int *pointspresent,
		  int *areaspresent,
		  int point_named_found,
		  int line_coast_found,
		  int area_dataset_found,
		  int line_coast_SHD1000,
		  int line_coast_BA000VDC999,
		  int area_dataset_HZD84_found,
		  int area_dataset_VDT84_found,
          int area_ent_col_meta_found,
          int area_ecm_zi001_sdp_noinfo1,
          int area_ecm_zi001_sdp_noinfo2,
          int area_ecm_zi001_sps_false,
		  int line_cut_found,
		  int point_building_found,
		  int area_building_found
)
{
  FILE *newfile;
  char message[1000],cleancode[1000],uncleancode[1000],tempcode[500];
  int i,j,k,totalchecks,thiscodeindex,
	  docheck1,docheck2,docheck3,docheck4,docheck5,
      docheck6,docheck7,docheck8,docheck9,docheck10,
      docheck11,docheck12,docheck13,docheck14,docheck15,
	  docheck16,docheck17,docheck18,docheck19,docheck20,
      docheck20a,docheck21,thisindex,thatindex,foundit,temp=0;
  
  double Dval,djunk,min=999,max=-999,minCutPFD,maxCutPFD;
  double cutlow=-999990.0,cuthigh=1.99999;

  int thistype,thisgeom,thisecc,Ival,tgtgeom,ecc_index=-1;

  extern void ExeUpdate(char message[]);
  extern int ZV2_Getter(char *attrcode,char *geomstring,char *fcode,double *minVAL,double *maxVAL, 
			   double minaccept,double maxaccept);
 
  extern int SPECIAL_NAME_ECC;
  extern int SPECIAL_NAME_LEN;
  extern int free_global_char_type;



#define num_bad_names 50
  int bad_names_found[num_bad_names];
  




  char  *BadNames[num_bad_names] =
  {
    "\" \"",
    "\"  \"",
    "\"No Information\"",
    "\"-999999\"",
     "\"-99999\"",
    "\"-9999\"",
    "\"-999\"",
    "\"-99\"",
    "\"0\"",
    "\"-32764\"",
    "\"-32765\"",
    "\"-32766\"",
    "\"-32767\"",
    "\"-32768\"",
    "\"996\"",
    "\"997\"",
    "\"998\"",
    "\"999\"",
    "\"-999999.0\"",
    "\"-99999.0\"",
    "\"-9999.0\"",
    "\"-999.0\"",
    "\"-99.0\"",
    "\"0.0\"",
    "\"-32764.0\"",
    "\"-32765.0\"",
    "\"-32766.0\"",
    "\"-32767.0\"",
    "\"-32768.0\"",
    "\"996.0\"",
    "\"997.0\"",
    "\"998.0\"",
    "\"999.0\"",
    "\"Not Applicable\"",
    "\"NA\"",
    "\"N_A\"",
    "\"UNK\"",
    "\"UNKNOWN\"",
    "\"Unknown\"",
    "\"Not Populated\"",
    "\"NP\"",
    "\"N_P\"",
    "\"NOT POPULATED\"",
    "\"Null\"",
    "\"NULL\"",
    "\"Other\"",
    "\"OTHER\"",
    "\"Oth\"",
    "\"OTH\"",
    "\"N/A\""
  };


  char  *BadNamesNoQuote[num_bad_names] =
  {
    " ",
    "  ",
    "No Information",
    "-999999",
     "-99999",
    "-9999",
    "-999",
    "-99",
    "0",
    "-32764",
    "-32765",
    "-32766",
    "-32767",
    "-32768",
    "996",
    "997",
    "998",
    "999",
    "-999999.0",
    "-99999.0",
    "-9999.0",
    "-999.0",
    "-99.0",
    "0.0",
    "-32764.0",
    "-32765.0",
    "-32766.0",
    "-32767.0",
    "-32768.0",
    "996.0",
    "997.0",
    "998.0",
    "999.0",
    "Not Applicable",
    "NA",
    "N_A",
    "UNK",
    "UNKNOWN",
    "Unknown",
    "Not Populated",
    "NP",
    "N_P",
    "NOT POPULATED",
    "Null",
    "NULL",
    "Other",
    "OTHER",
    "Oth",
    "OTH",
    "N/A"
  };










  if((type==5)||(type==6)||(type==7)||(type==8)||(type==11))
    {
      

      sprintf(message,"%sVVTERR1WAY.txt",prefix);
      newfile = fopen(message,"w");
      ExeUpdate(message);


SPECIAL_NAME_ECC = SilentGetCodeIndex("ZD040");
SPECIAL_NAME_LEN = 15;

Begin_LongFiles(LONG_ATTR_NAM);

SPECIAL_NAME_ECC = -1;
SPECIAL_NAME_LEN = -1;

temp = free_global_char_type;
free_global_char_type = 0;








      ecc_index = SilentGetCodeIndex("ZD040");

      for(i=0;i<num_bad_names;i++)
	{
	  bad_names_found[i] = 0;
	}
      
	  tgtgeom = C_POFE; 

      
      if((point_named_found>0)&&(ecc_index>=0))
	{
	  for(j=0;j<LLFidTableCount;j++)
	    {

		  

	      if(LLFidTable[j]<0)
		{
		  continue;
		}
	      
	      
	      thisecc  = SCCtable[CrsWlk[LLFidTable[j]].LtoSmapping].ECC;
	      thisgeom =          CrsWlk[LLFidTable[j]].geomtype;
	      
	      if(thisgeom!=tgtgeom)
		{
		  continue;
		}
	      
	      if(thisecc!=ecc_index)
		{
		  continue;
		}


	    thistype = GetLongAttrVal(LONG_ATTR_NAM,j,&Ival,&Dval,&cleancode[0]);

		  
	      if(thistype==C_TYPE)
		{
		  for(i=0;i<num_bad_names;i++)
		    {
		      if(!strcmp(cleancode,BadNamesNoQuote[i]))
			{
			  bad_names_found[i] = 1;
			}
		    }
		}
	    }
	}
    


      docheck1 = 0; 
      docheck2 = 0; 
      docheck3 = 0; 
      docheck4 = 0; 
      docheck5 = 0; 
      docheck6 = 0; 
      docheck7 = 0; 
      docheck8 = 0; 


	  
	  docheck9   = 0; 
	  docheck10  = 0; 
	  docheck11  = 0; 
	  docheck12  = 0; 
	  docheck13  = 0; 
	  docheck14  = 0; 
	  docheck15  = 0; 
	  docheck16  = 0; 
	  docheck17  = 0; 
	  docheck18  = 0; 
      docheck19  = 0; 
	  docheck20  = 0; 
      docheck20a = 0; 
	  docheck21 = 0; 

	  djunk = 123;
      smalljunkflag = -999;


      if(line_coast_found>0)
	{
	  docheck1 = 1;
	  docheck2 = 1;
	  docheck3 = 1;
	}

      if(point_named_found>0)
	  {
        for(i=0;i<num_bad_names;i++)
		{  
          if(bad_names_found[i]>0)
		  {  
            docheck4 = 1;
		    break;
		  }  
		} 
	  } 

	  if(area_dataset_found>0)
	  {
        docheck5 = 1;
        docheck6 = 1;
	  }




      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck7>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZSAX_RS0"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));


			if( (strcmp(cleancode,"U")) && (strcmp(cleancode,"UNCLASSIFIED")) )
			{
              docheck7 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 



      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck8>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZSAX_RX4"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==2) 
		  { 
            sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			if(strcmp(cleancode,"\"USA\""))
			{
              docheck8 = 1;
			  break;
			}
		  } 
          else
		  { 
            continue;
		  } 			
		} 
	  } 






      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck9>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZSAX_RX3"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==2) 
		  { 
            sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			if(strcmp(cleancode,"\"DS\""))
			{
              docheck9 = 1;
			  break;
			}
		  } 
          else
		  { 
            continue;
		  } 			
		} 
	  } 





      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck10>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CCN"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==2) 
		  { 
            sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			if(
				(!strcmp(cleancode,"\"No Information\"")) ||
				(!strcmp(cleancode,"\"noInformation\""))
			  )
			{
              docheck10 = 1;
			  break;
			}
		  } 
          else
		  { 
            continue;
		  } 			
		} 
	  } 




      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck11>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI004_RCG"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));


			if(!strcmp(cleancode,"No Information"))
			{
              docheck11 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 



     for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck12>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI001_SRT"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));


			if(!strcmp(cleancode,"No Information"))
			{
              docheck12 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 




 
     for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck13>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(
			  (strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI001_SDV")) &&
			  (strcmp(GetEACCode(MdlNames2[thisindex].code),"MDE"))
			)
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==2) 
		  { 
            sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			if(BadTDSDate(cleancode))
			{
              docheck13 = 1;
			  break;
			}
		  } 
          else
		  { 
            continue;
		  } 			
		} 
	  } 




     for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck14>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI001_SPS"))
		  {
            continue;
		  }

		  if(!strcmp(GetECCCode(MdlNames[i].code),"ZI039"))
		  {
			
            continue;
		  }



          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));


			if(strcmp(cleancode,"True"))
			{
              docheck14 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 






     for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck15>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI020_GE4"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));


			if(!strcmp(cleancode,"No Information"))
			{
              docheck15 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 




     for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck16>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CPS"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));


			if(!strcmp(cleancode,"No Information"))
			{
              docheck16 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 




      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck17>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CID"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==2) 
		  {
            sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			if(
				(!strcmp(cleancode,"\"No Information\"")) ||
				(!strcmp(cleancode,"\"noInformation\""))
			  )
			{
              docheck17 = 1;
			  break;
			}
		  } 
          else
		  { 
            continue;
		  } 			
		} 
	  } 




     for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck18>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     

		  if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI026_CTUL"))
		  {
            if(MdlNames2[thisindex].values[thatindex].type==0) 
			{  
			  if((int)MdlNames2[thisindex].values[thatindex].value!=1)
			  { 
                docheck18 = 1;
			    break;
			  } 
			} 
		  }
		  if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI026_CTUU"))
		  {
            if(MdlNames2[thisindex].values[thatindex].type==0) 
			{  
			  if((int)MdlNames2[thisindex].values[thatindex].value<5000)
			  { 
                docheck18 = 1;
			    break;
			  } 
			} 
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 


	  if(area_ent_col_meta_found>0)
	  {
		if( (area_ecm_zi001_sdp_noinfo1+area_ecm_zi001_sdp_noinfo2) > 0)
		{
          docheck19 = 1;
		}
        docheck20 = 1;
	  }

	  if(line_cut_found>0)
	  {
	    if(NUM_Getter("PFD","(LINE)","Cut",&minCutPFD,&maxCutPFD,cutlow,cuthigh,0,&smalljunkflag,-999,&djunk,-999)>0)
		{
          
          docheck20a = 1;
		}
	  }











End_LongFiles(LONG_ATTR_NAM);
free_global_char_type = temp;





	  if(ATTRTYPE_TDS6()==1)
	  {
	    docheck21 = ComputeAdditionalVV1_TDS(pointspresent,areaspresent);
	  }


	  if(ATTRTYPE_TDS4()==1)
	  {
        docheck9  = 0;
		docheck10 = 0;
		docheck11 = 0;
		docheck12 = 0;
		docheck13 = 0;
		docheck14 = 0;
		docheck15 = 0;
		docheck16 = 0;
		docheck17 = 0;
		docheck18 = 0;
		docheck19 = 0;
		docheck20 = 0;
	  }



      totalchecks = docheck1   + docheck2   + docheck3  + docheck4  + docheck5  + 
		            docheck6   + docheck7   + docheck8  + docheck9  + docheck10 + 
					docheck11  + docheck12  + docheck13 + docheck14 + docheck15 +
					docheck16  + docheck17  + docheck18 + docheck19 + docheck20 +
					docheck20a + docheck21;


      if(totalchecks>0)
	  {
	    fprintf(newfile,"UNIQUE_CHECKS 1\n");
	    fprintf(newfile,"VVTERR1WAY %d\n",totalchecks);

		j = 1;


        if(docheck1>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Line %s features that are not assigned SHD=False\"\n",j,GetCoastName(1));
          j = j + 1;
		}

        if(docheck2>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Line %s features that are not assigned BA000_VDC=Other\"\n",j,GetCoastName(1));
          j = j + 1;
		}

        if(docheck3>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Line %s features that are not assigned BA000_VDR=\"debris line\"\"\n",j,GetCoastName(1));
          j = j + 1;
		}

	    if(docheck4>0)
		{ 
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Point Named Location features with \"%s\" = ",j,GetNAMName());

		  foundit = 0;
          for(i=0;i<num_bad_names;i++)
		  {  
            if(bad_names_found[i]>0)
			{
              if(foundit>0)
			  {
                fprintf(newfile," or ");
			  }
              fprintf(newfile,"%s",BadNames[i]);
			  foundit = foundit + strlen(BadNames[i]);
			  if(foundit>500)
			  {
                break;
			  }
			} 
		  } 

          fprintf(newfile,"\"\n");

          j = j + 1;
		} 

        if(docheck5>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area Dataset features that are not assigned \"HZD\" = \"World Geodetic System 1984\"\"\n",j);
          j = j + 1;
		}

        if(docheck6>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area Dataset features that are not assigned \"VDT\" = \"WGS 84 Ellipsoid\"\"\n",j);
          j = j + 1;
		}

        if(docheck7>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZSAX_RS0\" other than 'U'\"\n",j);
          j = j + 1;
		}

        if(docheck8>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZSAX_RX4\" other than 'USA'\"\n",j);
          j = j + 1;
		}

        if(docheck9>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZSAX_RX3\" other than 'DS'\"\n",j);
          j = j + 1;
		}

        if(docheck10>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"CCN\" = \"No Information\"\"\n",j);
          j = j + 1;
		}

        if(docheck11>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZI004_RCG\" = \"No Information\"\"\n",j);
          j = j + 1;
		}
        if(docheck12>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZI001_SRT\" = \"No Information\"\"\n",j);
          j = j + 1;
		}
        if(docheck13>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features with \"ZI001_SDV\" or \"MDE\" not in CCYY-MM-DD format\"\n",j);
          j = j + 1;
		}
        if(docheck14>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZI001_SPS\" other than \"True\"\"\n",j);
          j = j + 1;
		}
        if(docheck15>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZI020_GE4\" = \"No Information\"\"\n",j);
          j = j + 1;
		}
        if(docheck16>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"CPS\" = \"No Information\"\"\n",j);
          j = j + 1;
		}
        if(docheck17>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"CID\" = \"No Information\"\"\n",j);
          j = j + 1;
		}
        if(docheck18>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"ZI026_CTUL\" other than 1 or \"ZI026_CTUU\" less than 5000\"\n",j);
          j = j + 1;
		}
        if(docheck19>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area Entity Collection Metadata features with ZI001_SDP = No Information\"\n",j);
          j = j + 1;
		}
        if(docheck20>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Area Entity Collection Metadata features with ZI001_SPS other than \"False\"\"\n",j);
          j = j + 1;
		}
        if(docheck20a>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Line Cut features with PFD<2\"\n",j);
          j = j + 1;
		}




		if(docheck21>0)
		{
          for(i=0;i<TDS_badsingleNum*5;i=i+5)
		  { 
            if(TDS_badvals2[i]>0)
			{

              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
              fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);

	          thiscodeindex = GetCodeIndex(TDS_badvals[i]);

              sprintf(cleancode,"%s",GetECCLabel(thiscodeindex));


	          if(TDS_badvals[i+1][0]=='P')
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Point %s without %s = \"%s\"\"\n",j,
					cleancode,TDS_badvals[i+2],TDS_badvals[i+4]);
			  }
			  else if(TDS_badvals[i+1][0]=='A')
			  {
                fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"IAE violation:  Area %s without %s = \"%s\"\"\n",j,
					cleancode,TDS_badvals[i+2],TDS_badvals[i+4]);
			  }
			  else
			  {
				printf("bad tbvi1 %s (%d)\n",TDS_badvals[i+1],i);
			  }

              j = j + 1;
			}
		  }
		}


	    fprintf(newfile,"ATTRATTRATTR\n");

	    j = 1;

		if(docheck1>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_LINE\n",j,GetCoastName(0));

		  if(line_coast_SHD1000==1)
		  {
 		    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ShorelineDelineated LABEL False off C_LINE\n",j,GetCoastName(0));
		  }

		  j = j + 1;
		}

		if(docheck2>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_LINE\n",j,GetCoastName(0));

		  if(line_coast_BA000VDC999==1)
		  {
 		    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s WaterLineSoundingDatum LABEL Other off C_LINE\n",j,GetCoastName(0));
		  }

		  j = j + 1;
		}

		if(docheck3>0)
		{
	      fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_LINE\n",j,GetCoastName(0));

		  

          for(i=0;i<NumberOfModels;i++)
		  { 
             if((!strcmp(MdlNames[i].name,"(LINE)")) && (!strcmp(GetECCLabel(MdlNames[i].code),GetCoastName(1))))
			 {	 
	           for(k=0;k<MdlNames[i].numattributes;k++)
			   {	   
	             thisindex = MdlNames[i].AttrIndices[k];
	             thatindex = MdlNames[i].ValueIndices[k];

	             if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"BA000_VDR"))
				 { 
		           if(MdlNames2[thisindex].values[thatindex].type==2) 
				   { 
					 if(StringCaseInsensitiveMatch(MdlNames2[thisindex].values[thatindex].label,"\"debris line\""))
					 {
                        
                        fprintf(newfile,"VVTERR1WAY %d PRIMARY %s WaterLineSoundingDatumName STRING %s off C_LINE\n",j,GetCoastName(0),MdlNames2[thisindex].values[thatindex].label);
					 }
					 else
					 {
                        
					 }
				   } 
		           else
				   { 
                      continue;
				   } 
				 }
			   }
			 }
		  }
		  j = j + 1;
		}


	    if(docheck4>0)
		{  
          if(point_named_found>0)
		  {   
            for(i=0;i<num_bad_names;i++)
			{   
	          if(bad_names_found[i]!=0)
			  {   
		        fprintf(newfile,"VVTERR1WAY %d PRIMARY NamedLocation %s STRING %s on C_POFE\n",j,MakeCleanCode(GetNAMName()),BadNames[i]);
			  }   
			}   
		  }   
		  j = j + 1;
		} 	  

		if(docheck5>0)
		{
 	      fprintf(newfile,"VVTERR1WAY %d PRIMARY Dataset ANY on C_AREA\n",j);
		  if(area_dataset_HZD84_found>0)
		  {
		    fprintf(newfile,"VVTERR1WAY %d PRIMARY Dataset GeodeticDatum LABEL WorldGeodeticSystem1984 off C_AREA\n",j);
		  }
		  j = j + 1;
		}

		if(docheck6>0)
		{
 	      fprintf(newfile,"VVTERR1WAY %d PRIMARY Dataset ANY on C_AREA\n",j);
		  if(area_dataset_VDT84_found>0)
		  {
		    fprintf(newfile,"VVTERR1WAY %d PRIMARY Dataset VerticalDatum LABEL WGS84Ellipsoid off C_AREA\n",j);
		  }

		  

		  j = j + 1;
		}


        if(docheck7>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZSAX_RS0"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 MakeCleanCode(GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value)));

			    if( (strcmp(cleancode,"U")) && (strcmp(cleancode,"UNCLASSIFIED")) )
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZSAX_RS0"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  




        if(docheck8>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZSAX_RX4"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==2) 
			  {
                sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			    if(strcmp(cleancode,"\"USA\""))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZSAX_RX4"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  





        if(docheck9>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZSAX_RX3"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==2) 
			  {
                sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			    if(strcmp(cleancode,"\"DS\""))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZSAX_RX3"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  





        if(docheck10>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CCN"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==2) 
			  {
                sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			    if(
					(!strcmp(cleancode,"\"No Information\"")) ||
					(!strcmp(cleancode,"\"noInformation\""))
				  )
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("CCN"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  



        if(docheck11>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI004_RCG"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 MakeCleanCode(GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value)));


			    if(!strcmp(cleancode,"No_Information"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI004_RCG"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  



        if(docheck12>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI001_SRT"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 MakeCleanCode(GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value)));

			    if(!strcmp(cleancode,"No_Information"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI001_SRT"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  



        if(docheck13>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

			  if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI001_SDV"))
			  {
                if(MdlNames2[thisindex].values[thatindex].type==2) 
				{
                  sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			      if(BadTDSDate(cleancode))
				  { 
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                        MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI001_SDV"))),
                        cleancode,GetRangeGeomType(1,MdlNames[i].name));
				  } 
				} 
			  }
			  else if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"MDE"))
			  {
                if(MdlNames2[thisindex].values[thatindex].type==2) 
				{
                  sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			      if(BadTDSDate(cleancode))
				  { 
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                        MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("MDE"))),
                        cleancode,GetRangeGeomType(1,MdlNames[i].name));
				  } 
				} 
			  }
			}  
		  }  
		  j = j + 1;
		}  








        if(docheck14>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI001_SPS"))
			  {
                continue;
			  }

		      if(!strcmp(GetECCCode(MdlNames[i].code),"ZI039"))
			  { 
			    
                continue;
			  } 

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 MakeCleanCode(GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value)));

			    if(strcmp(cleancode,"True"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI001_SPS"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
              else
			  {
                continue;
			  }
			}  
		  }  
		  j = j + 1;
		}  


        if(docheck15>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI020_GE4"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 MakeCleanCode(GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value)));

			    if(!strcmp(cleancode,"No_Information"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI020_GE4"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
              else
			  {
                continue;
			  }
			}  
		  }  
		  j = j + 1;
		}  




        if(docheck16>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CPS"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 MakeCleanCode(GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value)));

			    if(!strcmp(cleancode,"No_Information"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("CPS"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
              else
			  {
                continue;
			  }
			}  
		  }  
		  j = j + 1;
		}  





        if(docheck17>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CID"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==2) 
			  {
                sprintf(cleancode,"%s",MdlNames2[thisindex].values[thatindex].label);

			    if(
					(!strcmp(cleancode,"\"No Information\"")) ||
					(!strcmp(cleancode,"\"noInformation\""))
				  )
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s STRING %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("CID"))),
                      cleancode,GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
			}  
		  }  
		  j = j + 1;
		}  



        if(docheck18>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              



		      if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI026_CTUL"))
			  {
                if(MdlNames2[thisindex].values[thatindex].type==0) 
				{   
			      if(
					  (MdlNames2[thisindex].values[thatindex].value<0.9999) ||
					  (MdlNames2[thisindex].values[thatindex].value>1.0001)
					)
				  {  
                     fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI026_CTUL"))),
                      MdlNames2[thisindex].values[thatindex].value,MdlNames2[thisindex].values[thatindex].value,
					  GetRangeGeomType(1,MdlNames[i].name));
				  }
				}
			  }
		      else if(!strcmp(GetEACCode(MdlNames2[thisindex].code),"ZI026_CTUU"))
			  { 
                if(MdlNames2[thisindex].values[thatindex].type==0) 
				{   
			      if(MdlNames2[thisindex].values[thatindex].value<4999.999)
				  { 
                    fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE %lf %lf on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI026_CTUU"))),
                      MdlNames2[thisindex].values[thatindex].value,MdlNames2[thisindex].values[thatindex].value,
					  GetRangeGeomType(1,MdlNames[i].name));
				  }  
				}  
			  } 
			}  
		  }  
		  j = j + 1;
		}  



		if(docheck19>0)
		{
		  if(area_ecm_zi001_sdp_noinfo1==1)
		  {
 		    fprintf(newfile,"VVTERR1WAY %d PRIMARY EntityCollectionMetadata %s STRING \"No Information\" on C_AREA\n",j,MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI001_SDP"))));
		  }
		  if(area_ecm_zi001_sdp_noinfo2==1)
		  {
 		    fprintf(newfile,"VVTERR1WAY %d PRIMARY EntityCollectionMetadata %s STRING \"noInformation\" on C_AREA\n",j,MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI001_SDP"))));
		  }
		  j = j + 1;
		}


		if(docheck20>0)
		{
		  if(area_ent_col_meta_found>0)
		  {
            fprintf(newfile,"VVTERR1WAY %d PRIMARY EntityCollectionMetadata ANY on C_AREA\n",j);
		  }

		  if(area_ecm_zi001_sps_false==1)
		  {
 		    fprintf(newfile,"VVTERR1WAY %d PRIMARY EntityCollectionMetadata %s LABEL False off C_AREA\n",j,MakeCleanCode2(GetEACLabel(GetAttrIndex("ZI001_SPS"))));
		  }
		  j = j + 1;
		}

		if(docheck20a>0)
		{
          fprintf(newfile,"VVTERR1WAY %d PRIMARY Cut %s RANGE %lf %lf off C_LINE\n",j,
		    	 	     MakeCleanCode2(GetEACLabel(GetAttrIndex("PFD"))),
						 minCutPFD,maxCutPFD);
		  j = j + 1;
		}



		if(docheck21>0)
		{

          for(i=0;i<TDS_badsingleNum*5;i=i+5)
		  { 
            if(TDS_badvals2[i]>0)
			{
	          thiscodeindex = GetCodeIndex(TDS_badvals[i]);

              sprintf(uncleancode,"%s",GetECCLabel(thiscodeindex));
              sprintf(cleancode,"%s",MakeCleanCode(GetECCLabel(thiscodeindex)));


	          if(TDS_badvals[i+3][0]=='1')
			  {  
				

	            if(TDS_badvals[i+1][0]=='P')
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_POFE\n",j,cleancode);

				  sprintf(tempcode,"%s",TDS_badvals[i+2]); 

	              if(FindAttr(-1,FIND_ENUM,"(POINT)",uncleancode,tempcode, TDS_badvals[i+4]))
				  {
 		            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off C_POFE\n",j,
					  cleancode,
					  MakeCleanCode2(GetEACLabel(GetAttrIndex(tempcode))),
                      MakeCleanCode3(TDS_badvals[i+4]));
				  }


				  sprintf(tempcode,"%s2",TDS_badvals[i+2]); 

	              if(FindAttr(-1,FIND_ENUM,"(POINT)",uncleancode,tempcode, TDS_badvals[i+4]))
				  {
 		            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off C_POFE\n",j,
					  cleancode,
					  MakeCleanCode2(GetEACLabel(GetAttrIndex(tempcode))),
                      MakeCleanCode3(TDS_badvals[i+4]));
				  }


				  sprintf(tempcode,"%s3",TDS_badvals[i+2]); 

	              if(FindAttr(-1,FIND_ENUM,"(POINT)",uncleancode,tempcode, TDS_badvals[i+4]))
				  {
 		            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off C_POFE\n",j,
					  cleancode,
					  MakeCleanCode2(GetEACLabel(GetAttrIndex(tempcode))),
                      MakeCleanCode3(TDS_badvals[i+4]));
				  }


				} 
	            else if(TDS_badvals[i+1][0]=='A')
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_AREA\n",j,cleancode);

				  sprintf(tempcode,"%s",TDS_badvals[i+2]); 

	              if(FindAttr(-1,FIND_ENUM,"(AREA)",uncleancode,tempcode, TDS_badvals[i+4]))
				  {
 		            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off C_AREA\n",j,
					  cleancode,
					  MakeCleanCode2(GetEACLabel(GetAttrIndex(tempcode))),
                      MakeCleanCode3(TDS_badvals[i+4]));
				  }

				  sprintf(tempcode,"%s2",TDS_badvals[i+2]); 

	              if(FindAttr(-1,FIND_ENUM,"(AREA)",uncleancode,tempcode, TDS_badvals[i+4]))
				  {
 		            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off C_AREA\n",j,
					  cleancode,
					  MakeCleanCode2(GetEACLabel(GetAttrIndex(tempcode))),
                      MakeCleanCode3(TDS_badvals[i+4]));
				  }

				  sprintf(tempcode,"%s3",TDS_badvals[i+2]); 

	              if(FindAttr(-1,FIND_ENUM,"(AREA)",uncleancode,tempcode, TDS_badvals[i+4]))
				  {
 		            fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s off C_AREA\n",j,
					  cleancode,
					  MakeCleanCode2(GetEACLabel(GetAttrIndex(tempcode))),
                      MakeCleanCode3(TDS_badvals[i+4]));
				  }


				}
	            else
				{ 
		          printf("Bad value when processing TMBV: %c\n",TDS_badvals[i+1][0]);
				}
			  }
	          else
			  {
				 

	             
                 if(TDS_badvals[i+1][0]=='P')
				 { 
                   fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_POFE\n",j,cleancode);

				   if(!strcmp(TDS_badvals[i+2],"ZVH"))
				   {
					 
                     if(ZV2_Getter("ZVH","(POINT)",uncleancode,&min,&max, 0,0)>0)
					 { 
 		               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 0 0 off C_POFE\n",j,
			  		     cleancode,
					     MakeCleanCode2(GetEACLabel(GetAttrIndex(TDS_badvals[i+2]))));
					 } 
				   }


				   
				 }
                 else if(TDS_badvals[i+1][0]=='A')
				 {
                   fprintf(newfile,"VVTERR1WAY %d PRIMARY %s ANY on C_AREA\n",j,cleancode);


				   if(!strcmp(TDS_badvals[i+2],"ZVH"))
				   {
                     if(ZV2_Getter("ZVH","(AREA)",uncleancode,&min,&max, 0,0)>0)
					 { 
 		               fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s RANGE 0 0 off C_AREA\n",j,
			  		     cleancode,
					     MakeCleanCode2(GetEACLabel(GetAttrIndex(TDS_badvals[i+2]))));
					 } 
				   }

				   
				 }
	             else
				 {
		           printf("Bad value when processing TMBV2: %c\n",TDS_badvals[i+1][0]);
				 }
			  }

              j = j + 1;
			} 
		  } 
		} 



	  }
      else
	  {
	    fprintf(newfile,"Data not available to specify this check\n");
	  }

      fclose(newfile);
  }
}


void DoVVTERR1WAY_TYPE9(
		  int type,char *prefix)
{
  FILE *newfile;
  char message[1000],cleancode[1000];
  int docheck7,docheck8,i,j,k,totalchecks,thisindex,thatindex;
  extern void ExeUpdate(char message[]);


  if(type==9)
    {
      

      sprintf(message,"%sVVTERR1WAY.txt",prefix);
      newfile = fopen(message,"w");
      ExeUpdate(message);


      docheck7 = 0; 
      docheck8 = 0; 


      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck7>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CLASS"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));

			if(strcmp(cleancode,"U (UNCLASS)"))
			{
              docheck7 = 1;
			  break;
			}
		  }
          else
		  { 
            continue;
		  } 			
		} 
	  } 



      for(i=0;i<NumberOfModels;i++)
	  {
		if(docheck8>0)
		{
          break;
		}

        for(k=0;k<MdlNames[i].numattributes;k++)
		{ 	   
	      thisindex = MdlNames[i].AttrIndices[k];
	      thatindex = MdlNames[i].ValueIndices[k];

          
	     
		  if(strcmp(GetEACCode(MdlNames2[thisindex].code),"REL"))
		  {
            continue;
		  }

          if(MdlNames2[thisindex].values[thatindex].type==3) 
		  { 
            sprintf(cleancode,"%s", 
		             GetEELabel(
				         MdlNames2[thisindex].code,
				         (int)MdlNames2[thisindex].values[thatindex].value));

			if(strcmp(cleancode,"LIMDIS"))
			{
              docheck8 = 1;
			  break;
			}
		  }
         else
		  { 
            continue;
		  } 			
		} 
	  } 




      totalchecks = docheck7 + docheck8;


      if(totalchecks>0)
	  {
	    fprintf(newfile,"UNIQUE_CHECKS 1\n");
	    fprintf(newfile,"VVTERR1WAY %d\n",totalchecks);

		j = 1;

        if(docheck7>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"CLASS\" other than \"U (UNCLASS)\"\"\n",j);
          j = j + 1;
		}

        if(docheck8>0)
		{  
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_POFE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_POFE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_LINE off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_LINE on\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_ALL_AREA off\n",j);
          fprintf(newfile,"VVTERR1WAY %d C_PRIMARY C_SOME_AREA on\n",j);
          fprintf(newfile,"VVTERR1WAY %d ANNOTATION \"Features that are assigned \"REL\" other than \"LIMDIS\"\"\n",j);
          j = j + 1;
		}


	    fprintf(newfile,"ATTRATTRATTR\n");

	    j = 1;

        if(docheck7>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"CLASS"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value));

			    if(strcmp(cleancode,"U (UNCLASS)"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("CLASS"))),
                      MakeCleanCode3(cleancode),GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
              else
			  {
                continue;
			  }
			}  
		  }  
		  j = j + 1;
		}  




        if(docheck8>0)
		{
          for(i=0;i<NumberOfModels;i++)
		  {
            for(k=0;k<MdlNames[i].numattributes;k++)
			{
	          thisindex = MdlNames[i].AttrIndices[k];
	          thatindex = MdlNames[i].ValueIndices[k];

              

		      if(strcmp(GetEACCode(MdlNames2[thisindex].code),"REL"))
			  {
                continue;
			  }

              if(MdlNames2[thisindex].values[thatindex].type==3) 
			  {
                sprintf(cleancode,"%s",
		                 GetEELabel(
				             MdlNames2[thisindex].code,
				             (int)MdlNames2[thisindex].values[thatindex].value));

			    if(strcmp(cleancode,"LIMDIS"))
				{
                  fprintf(newfile,"VVTERR1WAY %d PRIMARY %s %s LABEL %s on %s\n",j,
                      MakeCleanCode(GetECCLabel(MdlNames[i].code)),MakeCleanCode2(GetEACLabel(GetAttrIndex("REL"))),
                      MakeCleanCode3(cleancode),GetRangeGeomType(1,MdlNames[i].name));
				}
			  }
              else
			  {
                continue;
			  }
			}  
		  }  
		  j = j + 1;
		}  
	  }
      else
	  {
	    fprintf(newfile,"Data not available to specify this check\n");
	  }

      fclose(newfile);
  }
}



