#	POLYTECH Grenoble - RICM3 2015- PLS
#	David BUI
#	Yohann MATTEO
#	Mathias BRANGER
# 	Florian PIERRE-LOUIS

CC = gcc
SRCS = main.c compression.c decompression.c gestion_dico.c RLE.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -W -Wall -g -I ../include

EXEC = lzw

all: $(EXEC)

lzw : $(OBJS)
	$(CC) $(CFLAGS) $^ -o lzw -lm


%.o : %.c %.h

clean :
	 rm -f $(OBJS) *~ $(EXEC)
