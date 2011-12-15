CFLAGS = -Wall -W
CFLAGS_lib = $(CFLAGS) -fPIC

OBJS_LIB = libnot24get.so not24get_check
CC = gcc

all: $(OBJS_LIB)

not24get_check: not24get_check.c libnot24get.so
	$(CC) $(CFLAGS) -L. not24get_check.c -lnot24get -o not24get_check

libnot24get.so: not24get.c
	$(CC) -shared $(CFLAGS_lib) not24get.c -o libnot24get.so
	cp libnot24get.so not24get.so

clean:
	rm -rf *.o *.so not24get_check

test: not24get_check
	@echo "horse battery staple correct" > password.txt
	@LD_LIBRARY_PATH=. ./not24get_check < password.txt && echo "test OK"
	@rm password.txt
