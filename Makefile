CC		= gcc
CFLAGS		= -Wall -DDEBUG -g
OBJS		= freeircd.o read_config.o parse_config.o \
		  findclassbyclassid.o show_running_config.o findaclbyaclid.o \
		  findpclbypclid.o

freeircd:	$(OBJS)
		$(CC) -g -o freeircd $(CFLAGS) $(OBJS)

clean:		
		rm -f freeircd
		rm -f *.o
		rm -f *.core

freeircd.o:		freeircd.c
read_config.o:		read_config.c
parse_config.o:		parse_config.c
findclassbyclassid.o:	findclassbyclassid.c
show_running_config.o:	show_running_config.c
findaclbyaclid.o:	findaclbyaclid.c
findpclbypclid.o:	findpclbypclid.c

