CC           := cc
CFLAGS       := -ggdb -O2 -Wall -Wextra
GDB          := gf2

all: run_demo
.PHONY = clean

clean:
	rm blui.o demo

demo: example.c
	$(CC) $(CFLAGS) $< -o $@

run_demo: demo
	./$<

debug: demo
	gf2 ./$<

# BUILD_CFLAGS := -c -fPIC
# Creates blui.o object file
# blui.o: blui.c
# 	$(CC) $(BUILD_CFLAGS) $(CFLAGS) -c $< -o $@

# Creates libblui.so
# libblui.so: blui.o
# 	$(CC) -shared -o $@ $<
