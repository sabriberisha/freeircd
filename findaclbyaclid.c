#include "include.h"

struct acl *findaclbyaclid(int id)
{
  int tmpcntr;
  for (tmpcntr=0 ; tmpcntr < numacl ; tmpcntr++)
    if(conf.acl[tmpcntr]->id == id)
      return(conf.acl[tmpcntr]);
  return('\0');
}
