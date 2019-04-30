#include "include.h"
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *getlistenfamily(int type)
{
  if (type == 2)
    return("ipv4");
  else
    return("ipv6");
}

char *getlistentype(int type)
{
  if (type == 1)
    return("client");
  else
    return("server");
}

char *getobjecttype(int type)
{
  if (type == 1)
    return("network");
  else
    return("host");
}

void show_running_config()
{
  int tmpcntr;
  char tmpbuf[MAXWORDLEN];	/* needed for pcl's */
  bzero(tmpbuf,MAXWORDLEN);
  printf("!\n");
  printf("! Configuration type 1:\n");
  printf("!\n");
  printf("server name %s id %d;\n", conf.name, conf.myid);
  printf("!\n");
  printf("server description %s;\n", conf.desc);
  printf("!\n");
  for (tmpcntr=0 ; tmpcntr < numlisten ; tmpcntr++)
    printf("listen %s %s port %d type %s;\n",
		    getlistenfamily(conf.listenport[tmpcntr]->family),
		    conf.listenport[tmpcntr]->ip,
		    conf.listenport[tmpcntr]->port,
		    getlistentype(conf.listenport[tmpcntr]->type));
  printf("!\n");
  for (tmpcntr=0 ; tmpcntr < numclass ; tmpcntr++)
    if(conf.conclass[tmpcntr]->pingtime !=-1)
      printf("class id %d pingtime %d maxclients %d;\n",
		    conf.conclass[tmpcntr]->id,
		    conf.conclass[tmpcntr]->pingtime,
		    conf.conclass[tmpcntr]->maxclients);
    else
      printf("class id %d maxclients %d;\n",
		    conf.conclass[tmpcntr]->id,
		    conf.conclass[tmpcntr]->maxclients);
  printf("!\n");
  for (tmpcntr=0 ; tmpcntr < numacl ; tmpcntr++)
  {
    if (conf.acl[tmpcntr]->maxclients != -1 && conf.acl[tmpcntr]->password[0] != '\0')
	    printf("acl id %d type %s %s password %s maxclients %d class %d;\n", 
		    conf.acl[tmpcntr]->id,
		    getobjecttype(conf.acl[tmpcntr]->type),
		    conf.acl[tmpcntr]->object,
		    conf.acl[tmpcntr]->password,
		    conf.acl[tmpcntr]->maxclients,
		    conf.acl[tmpcntr]->classptr->id);
    if (conf.acl[tmpcntr]->maxclients != -1 && conf.acl[tmpcntr]->password[0] == '\0')
	    printf("acl id %d type %s %s maxclients %d class %d;\n", 
		    conf.acl[tmpcntr]->id,
		    getobjecttype(conf.acl[tmpcntr]->type),
		    conf.acl[tmpcntr]->object,
		    conf.acl[tmpcntr]->maxclients,
		    conf.acl[tmpcntr]->classptr->id);
    if (conf.acl[tmpcntr]->maxclients == -1 && conf.acl[tmpcntr]->password[0] != '\0')
	    printf("acl id %d type %s %s password %s class %d;\n", 
		    conf.acl[tmpcntr]->id,
		    getobjecttype(conf.acl[tmpcntr]->type),
		    conf.acl[tmpcntr]->object,
		    conf.acl[tmpcntr]->password,
		    conf.acl[tmpcntr]->classptr->id);
  }
  printf("!\n");
  for (tmpcntr=0 ; tmpcntr < numpcl ; tmpcntr++)
  {
    if(conf.pcl[tmpcntr]->permission & (CONNECT))
      strncat(tmpbuf,"connect ",(MAXWORDLEN-strlen(tmpbuf)));
    if(conf.pcl[tmpcntr]->permission & (WHO))
      strncat(tmpbuf,"who ",(MAXWORDLEN-strlen(tmpbuf)));
    if(conf.pcl[tmpcntr]->permission & (MAP))
      strncat(tmpbuf,"map ",(MAXWORDLEN-strlen(tmpbuf)));
    if(conf.pcl[tmpcntr]->permission & (LINKS))
      strncat(tmpbuf,"links ",(MAXWORDLEN-strlen(tmpbuf)));
#ifdef USEJUPE
    if(conf.pcl[tmpcntr]->permission & (JUPE))
      strncat(tmpbuf,"jupe ",(MAXWORDLEN-strlen(tmpbuf)));
#endif
#ifdef USEMODE
    if(conf.pcl[tmpcntr]->permission & (MODE))
      strncat(tmpbuf,"mode ",(MAXWORDLEN-strlen(tmpbuf)));
#endif
#ifdef USEKILL
    if(conf.pcl[tmpcntr]->permission & (KILL))
      strncat(tmpbuf,"kill ",(MAXWORDLEN-strlen(tmpbuf)));
#endif
#ifdef USEGBAN
    if(conf.pcl[tmpcntr]->permission & (GBAN))
      strncat(tmpbuf,"gban ",(MAXWORDLEN-strlen(tmpbuf)));
#endif
    if(conf.pcl[tmpcntr]->permission & (CONFIGURE))
      strncat(tmpbuf,"configure ",(MAXWORDLEN-strlen(tmpbuf)));
    if(conf.pcl[tmpcntr]->permission & (SUSPEND))
      strncat(tmpbuf,"suspend ",(MAXWORDLEN-strlen(tmpbuf)));
    if(conf.pcl[tmpcntr]->permission & (ADMIN))
      strncat(tmpbuf,"admin ",(MAXWORDLEN-strlen(tmpbuf)));
    tmpbuf[strlen(tmpbuf)-1]='\0';
    printf("pcl id %d %s;\n",conf.pcl[tmpcntr]->id,tmpbuf);
    bzero(tmpbuf,MAXWORDLEN);
  }
  printf("!\n");
  for (tmpcntr=0 ; tmpcntr < numops ; tmpcntr++)
    printf("operator user %s password %s acl %d pcl %d;\n", conf.oper[tmpcntr]->user,
		    conf.oper[tmpcntr]->password,
		    conf.oper[tmpcntr]->aclptr->id,
		    conf.oper[tmpcntr]->pclptr->id);
  printf("!\n");
  return;
}

