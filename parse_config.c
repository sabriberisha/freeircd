#include "include.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

struct conclass *findclassbyclassid();	/* in findclassbyclassid.c */
struct acl *findaclbyaclid();		/* in findaclbyaclid.c */
struct pcl *findpclbypclid();		/* in findpclbypclid.c */

static void out(char *message, int num)
{
  printf("Error in line %d of configuration: %s\n", num+1, message);
  exit(1);
}

void p_listen(char *option[], int no, int num)
{
  int tmpcntr;
  int sane; /* for sanitiy checking */
  struct listenport *this;
  if (no <= 5)
    out("listem needs at least 6 parameters",num);
  if(numlisten >=MAXLISTENPORT)
    out("to many listenports",num);
  if(!(conf.listenport[numlisten]=malloc(sizeof(struct listenport))))
    out("could not malloc() listenport",num);
  this=conf.listenport[numlisten];
  this->family=-1;
  bzero(this->ip,MAXIPLEN);
  this->port=-1;
  this->type=-1;
  numlisten++;
  
  for (tmpcntr=1 ; tmpcntr < no ; tmpcntr++)
  {
    sane=0;	/* if a usable option is found, sane will be 1 */
    if (!strcasecmp(option[tmpcntr],"ipv4") && tmpcntr != num && sane==0)
    {
      if(this->family != -1)
	out("only 1 family permitted",num);
      this->family=AF_INET;
      strlcpy(this->ip,option[tmpcntr+1],MAXIPLEN);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"ipv6") && tmpcntr != num && sane==0)
    {
      if(this->type != -1)
	out("only 1 family permitted",num);
      sane=1;
      out("ipv6 is currently not supported",num);
    }
    if (!strcasecmp(option[tmpcntr],"port") && tmpcntr != num && sane==0)
    {
      if(this->port != -1)
	out("multiple ports in 1 statement is not allowed",num);
      this->port=atoi(option[tmpcntr+1]);
      if(this->port==0)
	 out("port 0 is illegal",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"type") && tmpcntr != num && sane==0)
    {
      if(!strcasecmp(option[tmpcntr+1],"client"))
	this->type=SERVERTYPECLIENT;
      if(!strcasecmp(option[tmpcntr+1],"server"))
	this->type=SERVERTYPESERVER;
      if(this->type==-1)
	out("listen needs servertype client or server",num);
      sane=1;
      tmpcntr++;
    }
    if(sane==0)
    {
      printf("Sorry, never heard of option %s for listen\n", option[tmpcntr]);
      out("unknown statement", num);
    }
  }
  if (this->port == -1)
    out("obgligatory port negated",num);
  if (this->type == -1)
    out("obgligatory type negated",num);
  if (this->family == -1)
    out("obgligatory ip negated",num);
  for (tmpcntr=0 ; tmpcntr < numlisten ; tmpcntr++)
    if (conf.listenport[tmpcntr]->family == this->family && 
		    !strcasecmp(conf.listenport[tmpcntr]->ip,this->ip) && 
		    conf.listenport[tmpcntr]->port == this->port && 
		    conf.listenport[tmpcntr]->type == this->type && 
		    tmpcntr != numlisten-1)
      out("duplicate line",num);
  return;
}

void p_operator(char *option[], int no, int num)
{
  int tmpcntr;
  int sane; /* for sanity checking */
  struct oper *this;

  if (no <= 7)
    out("operator needs at least 8 parameters",num);
  if(numops >=MAXOPS)
    out("to many irc operators",num);
  if(!(conf.oper[numops]=malloc(sizeof(struct oper))))
    out("could not malloc() oper",num);
  this=conf.oper[numops];
  this->aclptr='\0';
  this->pclptr='\0';
  bzero(this->user,MAXUSERLEN);
  bzero(this->password,MAXLINEPASSWORDLEN);
  numops++;

  for (tmpcntr=1 ; tmpcntr < no ; tmpcntr++)
  {
    sane=0;	/* if a usable option is found, sane will be 1 */
    if (!strcasecmp(option[tmpcntr],"user") && tmpcntr != num && sane==0)
    {
      if(this->user[0] !='\0')
	out("cannot have multiple username in 1 operstatement", num);
      strlcpy(this->user,option[tmpcntr+1],MAXUSERLEN);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"password") && tmpcntr != num && sane==0)
    {
      if(this->password[0] !='\0')
	out("cannot have multiple passwords in 1 operstatement", num);
      strlcpy(this->password,option[tmpcntr+1],MAXLINEPASSWORDLEN);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"acl") && tmpcntr != num && sane==0)
    {
      int tmp;
      if(this->aclptr != '\0')
	out("cannot have multiple acl's in 1 operstatement",num);
      tmp=atoi(option[tmpcntr+1]);
      if(tmp==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("acl is not numeric",num);
      this->aclptr=findaclbyaclid(tmp);
      if (this->aclptr=='\0')
	out("could not find acl",num);
      sane=1;
      tmpcntr++;
    } 
    if (!strcasecmp(option[tmpcntr],"pcl") && tmpcntr != num && sane==0)
    {
      int tmp;
      if(this->pclptr != '\0')
	out("cannot have multiple pcl's in 1 operstatement",num);
      tmp=atoi(option[tmpcntr+1]);
      if(tmp==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("pcl is not numeric",num);
      this->pclptr=findpclbypclid(tmp);
      if (this->pclptr=='\0')
	out("could not find pcl",num);
      sane=1;
      tmpcntr++;
    } 
    if(sane==0)
    {
      printf("Sorry, never heard of option %s for oper\n", option[tmpcntr]);
      out("unknown statement", num);
    }
  }
  if (this->user[0] == '\0')
    out("obgligatory user negated",num);
  if (this->password[0] == '\0')
    out("obgligatory password negated",num);
  if (this->aclptr == '\0')
    out("obgligatory acl negated",num);
  if (this->pclptr == '\0')
    out("obgligatory pcl negated",num);
  if(numops==1)
    return;
  for (tmpcntr=0 ; tmpcntr < numops-1 ; tmpcntr++)
  {
    if((!(strcasecmp(this->user,conf.oper[tmpcntr]->user))) &&
		    (!(strcasecmp(this->password,conf.oper[tmpcntr]->password))) &&
		     this->aclptr == conf.oper[tmpcntr]->aclptr)
      out("duplicate oper statements",num);
  }
  return;
}

void p_pcl(char *option[], int no, int num)
{
  int tmpcntr;
  int sane; /* for sanity checking */
  struct pcl *this;

  if (no <= 3)
    out("pcl needs at least 4 parameters",num);
  if(numpcl >=MAXPCLS)
    out("to many pcl's",num);
  if(!(conf.pcl[numpcl]=malloc(sizeof(struct pcl))))
    out("could not malloc() pcl",num);
  this=conf.pcl[numpcl];
  this->id=-1;
  this->permission=0; /* -1 would make 'connect' to true */
  numpcl++;

  for (tmpcntr=1 ; tmpcntr <= no ; tmpcntr++)
  {
    sane=0;	/* if a usable option is found, sane will be 1 */
    if (!strcasecmp(option[tmpcntr],"id") && tmpcntr != num && sane==0)
    {
      if(this->id != -1)
	out("multiple id's are not allowed",num);
      this->id=atoi(option[tmpcntr+1]);
      if(this->id==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("id is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"connect") && sane==0)
    {
      if (this->permission & (CONNECT))
	out("duplicate connect statement",num);
      this->permission |= CONNECT;
      sane=1;
    }
    if (!strcasecmp(option[tmpcntr],"who") && sane==0)
    {
      if (this->permission & (WHO))
	out("duplicate who statement",num);
      this->permission |= WHO;
      sane=1;
    }
    if (!strcasecmp(option[tmpcntr],"map") && sane==0)
    {
      if (this->permission & (MAP))
	out("duplicate map statement",num);
      this->permission |= MAP;
      sane=1;
    }
    if (!strcasecmp(option[tmpcntr],"links") && sane==0)
    {
      if (this->permission & (LINKS))
	out("duplicate links statement",num);
      this->permission |= LINKS;
      sane=1;
    }
#ifdef USEJUPE
    if (!strcasecmp(option[tmpcntr],"jupe") && sane==0)
    {
      if (this->permission & (JUPE))
	out("duplicate jupe statement",num);
      this->permission |= JUPE;
      sane=1;
    }
#endif
#ifdef USEMODE
    if (!strcasecmp(option[tmpcntr],"mode") && sane==0)
    {
      if (this->permission & (MODE))
	out("duplicate mode statement",num);
      this->permission |= MODE;
      sane=1;
    }
#endif
#ifdef USEKILL
    if (!strcasecmp(option[tmpcntr],"kill") && sane==0)
    {
      if (this->permission & (KILL))
	out("duplicate kill statement",num);
      this->permission |= KILL;
      sane=1;
    }
#endif
#ifdef USEGBAN
    if (!strcasecmp(option[tmpcntr],"gban") && sane==0)
    {
      if (this->permission & (GBAN))
	out("duplicate gban statement",num);
      this->permission |= GBAN;
      sane=1;
    }
#endif
    if (!strcasecmp(option[tmpcntr],"configure") && sane==0)
    {
      if (this->permission & (CONFIGURE))
	out("duplicate configure statement",num);
      this->permission |= CONFIGURE;
      sane=1;
    }
    if (!strcasecmp(option[tmpcntr],"suspend") && sane==0)
    {
      if (this->permission & (SUSPEND))
	out("duplicate suspend statement",num);
      this->permission |= SUSPEND;
      sane=1;
    }
    if (!strcasecmp(option[tmpcntr],"admin") && sane==0)
    {
      if (this->permission & (ADMIN))
	out("duplicate admin statement",num);
      this->permission |= ADMIN;
      sane=1;
    }
    if(sane==0)
    {
      printf("Sorry, never heard of option %s for pcl\n", option[tmpcntr]);
      out("unknown statement", num);
    }
  }
  for (tmpcntr=0 ; tmpcntr < numpcl-1 ; tmpcntr++)
    if(conf.pcl[tmpcntr]->id == this->id)
      out("duplicate id's not permitted",num);
  if (this->id == -1)
    out("obgligatory id negated",num);
  if (this->permission == 0)
    out("will code for permissions",num);
}
      

void p_acl(char *option[], int no, int num)
{
  int tmpcntr;
  int sane; /* for sanity checking */
  struct acl *this;

  if (no <= 3)
    out("acl needs at least 4 parameters",num);
  if(numacl >=MAXACLS)
    out("to many acl's",num);
  if(!(conf.acl[numacl]=malloc(sizeof(struct acl))))
    out("could not malloc() acl",num);
  this=conf.acl[numacl];
  this->id=-1;
  this->type=-1;
  this->maxclients=-1;
  this->classptr='\0';
  bzero(this->object,MAXACLLEN);
  bzero(this->password,MAXLINEPASSWORDLEN);
  numacl++;

  for (tmpcntr=1 ; tmpcntr < no ; tmpcntr++)
  {
    sane=0;	/* if a usable option is found, sane will be 1 */
    if (!strcasecmp(option[tmpcntr],"id") && tmpcntr != num && sane==0)
    {
      if(this->id != -1)
	out("multiple id's are not allowed",num);
      this->id=atoi(option[tmpcntr+1]);
      if(this->id==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("id is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"host") && tmpcntr != num && sane==0)
    {
      if(this->type != -1)
	out("cannot have network and host in 1 acl",num);
      if(this->object[0] !='\0')
	out("cannot have multiple hosts in 1 acl", num);
      this->type=ACLTYPEHOST;
      strlcpy(this->object,option[tmpcntr+1],MAXACLLEN);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"network") && tmpcntr != num && sane==0)
    {
      if(this->type != -1)
	out("cannot have network and host in 1 acl",num);
      if(this->object[0] !='\0')
	out("cannot have multiple networks in 1 acl", num);
      this->type=ACLTYPENETWORK;
      strlcpy(this->object,option[tmpcntr+1],MAXACLLEN);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"password") && tmpcntr != num && sane==0)
    {
      if(this->password[0] !='\0')
	out("cannot have multiple passwords in 1 acl", num);
      strlcpy(this->password,option[tmpcntr+1],MAXLINEPASSWORDLEN);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"maxclients") && tmpcntr != num && sane==0)
    {
      if(this->maxclients != -1)
	out("multiple maxclients are not allowed",num);
      this->maxclients=atoi(option[tmpcntr+1]);
      if(this->maxclients==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("maxclients is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"class") && tmpcntr != num && sane==0)
    {
      int tmp;
      if(this->classptr != '\0')
	out("cannot have multiple classes in 1 acl",num);
      tmp=atoi(option[tmpcntr+1]);
      if(tmp==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("class is not numeric",num);
      this->classptr=findclassbyclassid(tmp);
      if (this->classptr=='\0')
	out("could not find class",num);
      sane=1;
    }
    if(sane==0)
    {
      printf("Sorry, never heard of option %s for acl\n", option[tmpcntr]);
      out("unknown statement", num);
    }
  }
  if (this->id == -1)
    out("obgligatory id negated",num);
  if (this->type == -1)
    out("obgligatory network or host negated",num);
  if (this->classptr == '\0')
    out("obgligatory class negated",num);
  for (tmpcntr=0 ; tmpcntr < numacl-1 ; tmpcntr++)
    if(conf.acl[tmpcntr]->id == this->id)
      out("duplicate id's not permitted",num);
  return;
}

void p_class(char *option[], int no, int num)
{
  int tmpcntr;		
  int sane; /* for sanity checking */
  struct conclass *this;
  
  if (no <= 3)
    out("class needs at least 4 parameters",num);
  if(numclass >= MAXCLASS)
    out("to many classes",num);
  if(!(conf.conclass[numclass]=malloc(sizeof(struct conclass))))
    out("could not malloc() class",num);
  this=conf.conclass[numclass];
  this->id=-1;
  this->pingtime=-1;
  this->maxclients=-1;
  numclass++;
  
  for (tmpcntr=1 ; tmpcntr < no ; tmpcntr++)
  {
    sane=0;	/* if a usable option is found, sane will be 1 */
    if (!strcasecmp(option[tmpcntr],"id") && tmpcntr != num && sane==0)
    {
      if(this->id != -1)
	out("multiple id's are not allowed",num);
      this->id=atoi(option[tmpcntr+1]);
      if(this->id==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("id is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"maxclients") && tmpcntr != num && sane==0)
    {
      if(this->maxclients != -1)
	out("multiple maxclients are not allowed",num);
      this->maxclients=atoi(option[tmpcntr+1]);
      if(this->maxclients==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("maxclients is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"pingtime") && tmpcntr != num && sane==0)
    {
      if(this->pingtime != -1)
	out("multiple pingtime's are not allowed",num);
      this->pingtime=atoi(option[tmpcntr+1]);
      if(this->pingtime==0)
	if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	  out("pingtime is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if(sane==0)
    {
      printf("Sorry, never heard of option %s for class\n", option[tmpcntr]);
      out("unknown statement", num);
    }
  }
  if (this->id == -1)
    out("obgligatory id negotiated",num);
  if (this->maxclients == -1)
    out("obgligatory maxclients negotiated",num);
  for (tmpcntr=0 ; tmpcntr < numclass-1 ; tmpcntr++)
    if(conf.conclass[tmpcntr]->id == this->id)
      out("duplicate id's not permitted",num);
  return;
}

void p_description(char *option[], int no, int num)
{
  int tmpcntr;
  for (tmpcntr=2 ; tmpcntr <= no ; tmpcntr++)
  {
    strncat(conf.desc,option[tmpcntr],(MAXSERVDESCLEN-strlen(conf.desc)));
    if (strlen(conf.desc) < MAXSERVDESCLEN)
    {
      conf.desc[strlen(conf.desc)]=' ';
      conf.desc[strlen(conf.desc)+1]='\0';
    }
  }
  return;
}

void p_server(char *option[], int no, int num)
{
  int tmpcntr;		
  int sane; /* for sanity checking */
  if (no != 4)
    out("server needs 4 parameters",num);

  if (conf.myid != -1)
    out("only 1 servername statement permitted",num);
  
  for (tmpcntr=1 ; tmpcntr < no ; tmpcntr++)
  {
    sane=0;	/* if a usable option is found, sane will be 1 */
    if (!strcasecmp(option[tmpcntr],"id") && tmpcntr != num && sane==0)
    {
      if(conf.myid != -1)
	out("cannot have 2 id's",num);
      conf.myid=atoi(option[tmpcntr+1]);
      if(conf.myid == 0)
       if (option[tmpcntr+1][0] != '0' || strlen(option[tmpcntr+1]) >1)
	out("id is not numeric",num);
      sane=1;
      tmpcntr++;
    }
    if (!strcasecmp(option[tmpcntr],"name") && tmpcntr != num && sane==0)
    {
      if(conf.name[0] != '\0')
	out("cannot have 2 names",num);
      strlcpy(conf.name, option[tmpcntr+1], MAXSERVNAMELEN);
      sane=1;
      tmpcntr++;
    } 
    if (sane==0)	/* uh oh, sane is still 0, someone fscked up */
    {
      printf("Sorry, never heard of option %s for server\n", option[tmpcntr]);
      out("unknown statement", num);
    }
  }
  return;
}

void parse_config(char *line,int num)
{
  char *option[MAXOPTIONSPERLINE];	/* store options */
  char *tmpptr;				/* temp pointer */
  int no;				/* number of options extracted */
  int tmpcntr;				/* temp counter */
  int sane;				/* sanity */

  if (line[0]=='#' || line[0]=='!' || line[0]=='\n')	/* don't read */
    return;						/* commented lines */

  if (strlen(line) > MAXCONFLINELENGTH)
    out("line way too long!",num);
 
  line[strlen(line)-1]='\0';	/* strip the ; from the line */ 
  tmpptr=line;			/* we might need line later on */
  tmpcntr=0; 			/* start with 0 options */
  option[tmpcntr]=tmpptr;		/* first option */
  while (*tmpptr && tmpcntr <= MAXOPTIONSPERLINE) /* options from line */
  {
    if (*tmpptr==' ')
    {
      *tmpptr='\0';	/* change space to null */
      tmpptr++;
      if(*tmpptr!='\0')
      {
	tmpcntr++;
	option[tmpcntr]=tmpptr;
      }
    }
    tmpptr++;
  }
  no=tmpcntr;

/*  for (tmpcntr=0 ; tmpcntr <= no ; tmpcntr++)
  {
    printf("option: %s\n", option[tmpcntr]);
  } */  

  if (no <= 1)			/* need at least 2 options */
    out("need more arguments",num);
  
  sane=0;
  if(!strcasecmp(option[0],"server") && !strcasecmp(option[1],"description"))
  {
    p_description(option,no,num);
    sane=1;
  }
  if(!strcasecmp(option[0],"server") && sane==0)
  {
    p_server(option,no,num);
    sane=1;
  }
  if(!strcasecmp(option[0],"class") && sane==0)
  {
    p_class(option,no,num);
    sane=1;
  }
  if(!strcasecmp(option[0],"acl") && sane==0)
  {
    p_acl(option,no,num);
    sane=1;
  }
  if(!strcasecmp(option[0],"listen") && sane==0)
  {
    p_listen(option,no,num);
    sane=1;
  }
  if(!strcasecmp(option[0],"pcl") && sane==0)
  {
    p_pcl(option,no,num);
    sane=1;
  }
  if(!strcasecmp(option[0],"operator") && sane==0)
  {
    p_operator(option,no,num);
    sane=1;
  }

  if (sane==0)	/* uh oh, sane is still 0, someone fscked up */
  {
    printf("Sorry, never heard of the line %s\n", option[0]);
    out("unknown statement", num);
  }

  return;
}

