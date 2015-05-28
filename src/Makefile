#	POLYTECH Grenoble - RICM3 2015- PLS
#	David BUI
#	Yohann MATTEO
#	Mathias BRANGER

CC = gcc
SRCS = main.c compression.c decompression.c 	#Ajouter les fichiers
OBJS = $(SRCS:.c=.o)
CFLAGS = -W -Wall -g -I ../include

EXEC = main

main : main.c
	$(CC) $(CFLAGS) main.c 


%.o: %.c 
	$(CC) $(CFLAGS) -c $<


clean :
	 rm -f $(OBJS) *~ $(EXEC)
