#include "include.h"

struct conclass *findclassbyclassid(int id)
{
  int tmpcntr;
  for (tmpcntr=0 ; tmpcntr < numclass ; tmpcntr++)
    if(conf.conclass[tmpcntr]->id == id)
      return(conf.conclass[tmpcntr]);
  return('\0');
}
