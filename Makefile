CFLAGS = -Wall -W
CFLAGS_lib = $(CFLAGS) -fPIC
LFAGS=-lpasswdqc

OBJS_LIB = not24get_check not24get_test not24get.so
CC = gcc

all: $(OBJS_LIB)

not24get_test: not24get_check.c libnot24get.so
	$(CC) $(CFLAGS) -L. -lnot24get not24get_check.c -o not24get_test

not24get_check: not24get_check.c not24get.o
	$(CC) $(CFLAGS) $(LFAGS) not24get.o not24get_check.c -o not24get_check

not24get.o: not24get.c
	$(CC) -c -shared $(CFLAGS_lib) -DCONFIG_FILE=\"/etc/samba/not24get.conf\" not24get.c -o not24get.o

not24get.so: not24get.o
	$(CC) -shared $(CFLAGS_lib) $(LFAGS) not24get.o -o not24get.so

libnot24get.so: not24get.c
	$(CC) -shared $(CFLAGS_lib) $(LFAGS) -DCONFIG_FILE=\"./not24get.conf\" not24get.c -o libnot24get.so

clean:
	rm -rf *.o *.so $(OBJS_LIB)

test: not24get_check
	@./test.sh
