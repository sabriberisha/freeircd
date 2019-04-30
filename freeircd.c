#include "include.h"
#include "config.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void read_config();		/* in read_config.c */
void show_running_config();	/* in show_running_config.c */

void usage(char *me)
{
  printf("Usage: %s [-f file] [-n]\n", me);
  printf("Options:\n");
  printf("  -f file\t Config file (default: %s).\n", CONFIGFILE);
  printf("  -n\t\tDo not fork.\n");
  exit(0);
}

int main(int argc, char *argv[])
{
  extern char *optarg;
  extern int optind;
  int opt;
  int want_fork=1;
//  int tmpcntr;
  char *file='\0';
 
  while ((opt = getopt(argc,argv,"nf:")) != -1)
  {
    switch(opt)
    {
	case 'n':
		    want_fork=0;
		    break;
	case 'f':
		    file=optarg;
		    break;
	default:
		    usage(argv[0]);
    }
  }
  argc -= optind;
  argv += optind;

  /* empty some variables */
  conf.myid=-1;                                 /* reset */
  bzero(conf.name, sizeof(conf.name));          /* reset */
  numclass=0;					/* reset */
  numacl=0;					/* reset */
  numlisten=0;					/* reset */

  if (file)		/* if -f option given, read that file */
    read_config(file);  /* if read_config() fails, it will exit itself */
  else			 
    read_config(CONFIGFILE);

  show_running_config();
  
  printf("Exiting from main()\n"); 
  exit(0);
}
