CC=gcc
CFLAGS=-Wall

SRCS := $(wildcard source/*.c)
HDRS := $(wildcard include/*.h)
OBJS := $(patsubst source/%.c,object/%.o,$(SRCS)) object/tree.o
LDFLAGS = /usr/lib/x86_64-linux-gnu/libprotobuf-c.a

# link it all together
binary/tree_server: $(OBJS) $(HDRS) makefile
	@mkdir -p binary
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o ./binary/tree_server

# compile an object based on source and headers
object/%.o: source/%.c $(HDRS) makefile
	$(CC) $(CFLAGS) -c $< -o $@

object/tree.o:
	cp tree.o ./object/

clean:
	rm -f ./object/*
	rm -f ./binary/*