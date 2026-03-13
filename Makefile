CC           := cc
CFLAGS       := -ggdb -O2 -Wall -Wextra
BUILD_CFLAGS := -c -fPIC
GDB          := gf2

all: debug
.PHONY = clean

clean:
	rm blui.o demo

debug: blui.h example.c
	$(CC) $(CFLAGS) example.c -o demo
	gf2 ./demo

# Creates blui.o object file
blui.o: blui.c
	$(CC) $(BUILD_CFLAGS) $(CFLAGS) -c $< -o blui.o

# Creates libblui.so.1
libblui.so.1: blui.o
	$(CC) -shared -o libblui.so.1 $<

build: blui.c libblui.so.1
