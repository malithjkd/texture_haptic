CC=gcc
CFLAGS+=-D_LINUX -fPIE

LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
s826demo: s826_example.o client_c/client_connect.o -ljson-c lib826_64.a
	$(CC) $(CFLAGS) s826_example.o client_c/client_connect.o -ljson-c -o s826demo -lm -L./ -l826_64
else
s826demo: s826_example.o client_c/client_connect.o -ljson-c lib826.a
	$(CC) $(CFLAGS) s826_example.o client_c/client_connect.o -ljson-c -o s826demo -lm -L./ -l826
endif

all: s826demo






client_c/json/ %.o: %.c 
	$(CC) $(CFLAGS) -Wall -Wextra -DOSTYPE_LINUX -c $^ 

clean:
	rm -rf s826_example.o client_c/client_connect.o s826demo
