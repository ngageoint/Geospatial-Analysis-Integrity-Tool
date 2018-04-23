#include "share.h"

char **gifd_attr_defns;
char **gifd_class_defns;
char **gifd_enum_names;
char **gifd_attr_types;
char **gifd_enum_codes;
int   *gifd_attr_enum_start;
int   *gifd_attr_enum_count;
char **gifd_attr_names;
char **gifd_attr_codes;
char **gifd_class_names;
char **gifd_class_codes;
int GIFD_CLASS_NUM;
int GIFD_ATTR_NUM;

#if(NGA_TYPE==1)
char **STRATS;
char **DOMAINS;
int DOMAINSLEN;
int STRATSLEN;
#endif
