/* FreeIRCD
 *
 * This software is written by Sabri Berisha <sabri@freeircd.net>
 *
 * See the file LICENCE for information on licencing
 *
 */

#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <sys/types.h>
#include <netinet/in.h>

/* Users
 */

#define MAXUSERS 256
#define MAXNICKLEN 10
#define MAXUSERLEN 8
#define MAXINFOLEN 32
#define MAXUSERHOSTLEN 32
#define MAXUSERIDLEN 32
#define MAXIPLEN 32
#define MAXUSERCHAN 10
#define MAXINVITES 10

struct user 
{
  char nick[MAXNICKLEN];	/* nickname */
  char user[MAXUSERLEN];	/* username */
  char info[MAXINFOLEN];	/* userinfo */
  char host[MAXUSERHOSTLEN];	/* hostname */
  char id[MAXUSERIDLEN];	/* unique id */
  char strip[MAXIPLEN];		/* ip in string */
  char *channel[MAXUSERCHAN];	/* pointers to channels */
  char *invites[MAXINVITES];	/* pointers to channels where the user
				   has been invited to */
  int mode;			/* mode, see status.h for more info */
  int signon;			/* timestamp of signon */ 
  int lastaction;		/* time of last action (used for idle) */
  int lastping;			/* time of last ping sent */
  int dueping;			/* time when new ping is sent */
  int fd;			/* socket (-1 when not directly connected) */
  struct in_addr ip;		/* ip info */
  struct server *server;	/* server */
} *user[MAXUSERS];


/* Servers
 */

#define MAXSERVERS 256
#define MAXSERVERNAMELEN 128
#define MAXSERVERDESCLEN 256

struct server
{
  char name[MAXSERVERNAMELEN];	/* name of the server */
  char desc[MAXSERVERDESCLEN];	/* description */
  int id;			/* server ID */
  int fd;			/* socket (-1 when not directly connected)*/
  int rfd;			/* routed socket */
  int mode;			/* server mode */
} server[MAXSERVERS];


/* Channels 
 */

#define MAXCHANNELS 32768
#define MAXCHANNAMELEN 20
#define MAXTOPICLEN 128
#define MAXKEYLEN 128
#define MAXUSERSPERCHAN 512
#define MAXBANSPERCHAN 30

struct channel
{
  char name[MAXCHANNAMELEN];	/* name of the channel */
  char topic[MAXTOPICLEN];	/* topic */
  char key[MAXKEYLEN];		/* key to enter */
  int mode;			/* channel mode */
  int model;			/* max users allowed */
  int fd[MAXSERVERS];		/* outgoing sockets */
  struct member
  {
    char *user;			/* ptr to user */
    int flags;			/* flags of user */
  } *member[MAXUSERSPERCHAN];
  struct ban
  {
    char *hostmask;		/* nick!user@host */
    char nick[MAXNICKLEN];	/* whodunnit */
    int set;			/* time of ban */
  } *ban[MAXBANSPERCHAN];
} *channel[MAXCHANNELS];

/* Configuration
 */

#define MAXCONFIGFILESIZE 200000
#define MAXCONFIGLINES 1024
#define MAXCONFLINELENGTH 256
#define MAXOPTIONSPERLINE 32
#define MAXSERVNAMELEN 80
#define MAXSERVDESCLEN 80
#define MAXLISTENPORT 16
#define NUMADMIN 3
#define MAXADMINLEN 80
#define MAXCLASS 10
#define MAXLINEPASSWORDLEN 16
#define MAXACLLEN 128
#define MAXACLS 128
#define MAXPCLS 128
#define MAXOPS 4
#define ACLTYPENETWORK 1
#define ACLTYPEHOST 2
#define SERVERTYPECLIENT 1
#define SERVERTYPESERVER 2
#define MAXCRYPTLEN 14
#define MAXOPERS 16

/* operator powers */

#define USEMODE		/* are you sure?? */
#define USEGBAN		/* are you sure?? */
#define USEKILL		/* are you sure?? */
#define USEJUPE		/* are you sure?? */

#define CONNECT 1
#define WHO 2
#define MAP 4
#define LINKS 8
#ifdef USEJUPE
#define JUPE 16
#endif
#ifdef USEMODE
#define MODE 32
#endif
#ifdef USEKILL
#define KILL 64
#endif
#ifdef USEGBAN
#define GBAN 128
#endif
#define CONFIGURE 256
#define SUSPEND 512
#define ADMIN 1024

#define MAXWORDLEN 256	/* maximum length of all powers written 
			 * needed for pcl's in show_running_config()
			 */

int numclass;
int numacl;
int numpcl;
int numops;
int numlisten;

struct conf
{
  char name[MAXSERVNAMELEN];		/* name of server */
  char desc[MAXSERVDESCLEN];		/* description */
  char myadmin[NUMADMIN][MAXADMINLEN];	/* number of admin lines and lenght */
  int myid;				/* server id on network */
  struct conclass
  {
    int id;				/* class id */
    int pingtime;			/* ping times */
    int maxclients;			/* maximum number of clients */
  } *conclass[MAXCLASS];
  struct acl
  {
    int id;				/* acl id */
    char object[MAXACLLEN];		/* object (host or network */
    char password[MAXLINEPASSWORDLEN];	/* password to connect */
    int type;				/* type ACLTYPENETWORK or ACLTYPEHOST */
    int maxclients;			/* maximum clients for this netline */
    struct conclass *classptr;		/* ptr to class */
  } *acl[MAXACLS];
  struct listenport
  {
    int family;				/* AF_INET or AF_INET6 */
    char ip[MAXIPLEN];			/* ip in string */
    int port;				/* port */
    int type;				/* 1 = client, 2 = server */
  } *listenport[MAXLISTENPORT];
  struct pcl
  {
    int id;				/* pcl id */
    int permission;			/* permissions */
  } *pcl[MAXPCLS];
  struct oper
  {
    char user[MAXUSERLEN];		/* username */
    char password[MAXCRYPTLEN];		/* password in crypt() */
    struct acl *aclptr;			/* acl */
    struct pcl *pclptr;			/* pcl */
  } *oper[MAXOPERS];
} conf;
  
  

#endif /* _INCLUDE_H_ */
