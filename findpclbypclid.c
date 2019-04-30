#include "include.h"

struct pcl *findpclbypclid(int id)
{
  int tmpcntr;
  for (tmpcntr=0 ; tmpcntr < numpcl ; tmpcntr++)
    if(conf.pcl[tmpcntr]->id == id)
      return(conf.pcl[tmpcntr]);
  return('\0');
}
