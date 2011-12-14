CFLAGS = -Wall -W
CFLAGS_lib = $(CFLAGS) -fPIC

OBJS_LIB = not24get.o
CC = gcc

all: $(OBJS_LIB)

not24get.o: not24get.c
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -rf *.o *.so
