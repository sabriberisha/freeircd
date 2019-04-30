#include "include.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

void parse_config();		/* in parse_config.c */

static void out(char *message)
{
  printf("%s\n",message);
  exit(1);
}

void read_config(char *file)
{
  char *content; 			/* ptr to loaded file */
  char *configline[MAXCONFIGLINES+1];	/* exstracted lines from config */
  char *tmpptr;				/* temp pointer */
  int fd;				/* filehandle for config file */
  int nl;				/* number of extracted lines */
  int tmpcntr;				/* temp counter*/ 
  struct stat st;			/* used for stat() */

  if (stat(file, &st) == -1) /* stat the file */
  {
    if (errno==ENOENT)
      out("Fatal error: configuration file does not exist");
    if (errno==EACCES)
      out("Fatal error: access denied to configuration file");
   out("Unknown fatal error during stat() of configuration file");
  }

  if (st.st_size > MAXCONFIGFILESIZE)
    out("Fatal error: configuration file way too big");
  
  if (!(content = malloc(sizeof(char) *(st.st_size)))) /* file too big? */
    out("Fatal error: configuration file too big for memory");

  fd=open(file,O_RDONLY); /* open the file */
  if (fd == -1)
  {
    if (errno==EISDIR)
      out("Fatal error: given configuration file is directory");
    if (errno==ENFILE)
      out("Fatal error: could not open configuration file (file table full)");
    if (errno==EIO)
      out("Fatal error: I/O error while opening configuration file");
    if (errno==EINVAL)
      out("Fatal error: invalid argument while opening configuration file");
    out("Unknown fatal error while opening configuration file");
  }

  if (read(fd,content,st.st_size) != st.st_size) /* read the file to mem */
    out("Fatal error: read() failed");
   
  tmpptr=content;	/* we don't wanna mess with *content as we will need
			 * it for free();
			 */
  tmpcntr=0;		/* start with 0 lines */
  configline[tmpcntr]=tmpptr; /* first line */
  while (*tmpptr && tmpcntr <= MAXCONFIGLINES) /* extract lines from config */
  {
    if(*tmpptr=='\n')	
    {
      *tmpptr='\0';	/* change \n to null */
      tmpptr++;
      if (*tmpptr!='\0')
      {
	tmpcntr++;
	configline[tmpcntr]=tmpptr;
      }
    }
    tmpptr++;
  }
  nl=tmpcntr;

  for (tmpcntr=0 ; tmpcntr <= nl ; tmpcntr++)
  {
    parse_config(configline[tmpcntr], tmpcntr);
  }
  
  free(content);
  return;
}
