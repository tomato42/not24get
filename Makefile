CFLAGS = -Wall -W -lpasswdqc
CFLAGS_lib = $(CFLAGS) -fPIC

OBJS_LIB = libnot24get.so not24get_check
CC = gcc

all: $(OBJS_LIB)

not24get_check: not24get_check.c libnot24get.so
	$(CC) $(CFLAGS) -L. not24get_check.c -lnot24get -o not24get_check

libnot24get.so: not24get.c
	$(CC) -shared $(CFLAGS_lib) -DCONFIG_FILE=\"./not24get.conf\" not24get.c -o libnot24get.so
	cp libnot24get.so not24get.so

clean:
	rm -rf *.o *.so not24get_check

test: not24get_check
	@./test.sh
