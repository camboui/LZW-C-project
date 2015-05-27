#	POLYTECH Grenoble - RICM3 2015- PLS
#	David BUI
#	Yohann MATTEO
#	Mathias BRANGER

CC = gcc
SRCS = main.c 	#Ajouter les fichiers
OBJS = $(SRCS:.c=.o)
CFLAGS = -W -Wall -g -I ../include

EXEC = main

main : main.c
	$(CC) $(CFLAGS)  main.c 


%.o: %.c 
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<


clean :
	 rm -f $(OBJS) *~ $(EXEC)
