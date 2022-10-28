CC=gcc
CFLAGS=-Wall

SRCS := $(wildcard source/*.c)
HDRS := $(wildcard include/*.h)
S_OBJS = tree_server.o entry.o data.o tree.o tree_skel.o network_server.o network-private.o sdmessage.pb-c.o
S_HDRS = entry.h data.h tree.h tree_skel.h network_server.h network-private.h sdmessage.pb-c.h
C_OBJS = tree_client.o client_stub.o network_client.o sdmessage.pb-c.o network-private.o data.o entry.o
C_HDRS = client_stub.h network_client.h sdmessage.pb-c.h network-private.h data.h entry.h
LDFLAGS = /usr/lib/x86_64-linux-gnu/libprotobuf-c.a

all: tree-server tree-client

tree-server: $(foreach f, $(S_OBJS), ./object/$(f)) $(foreach f, $(S_HDRS), ./include/$(f))
	@mkdir -p binary
	$(CC) $(CFLAGS) $(foreach f, $(S_OBJS), ./object/$(f)) $(LDFLAGS) -o ./binary/tree-server

tree-client: $(foreach f, $(C_OBJS), ./object/$(f)) $(foreach f, $(C_HDRS), ./include/$(f))
	@mkdir -p binary
	$(CC) $(CFLAGS) $(foreach f, $(C_OBJS), ./object/$(f)) $(LDFLAGS) -o ./binary/tree-client

# TODO
#client-lib.o:

# compile an object based on source and headers
object/%.o: source/%.c $(HDRS) makefile
	$(CC) $(CFLAGS) -c $< -o $@

object/tree.o:
	cp tree.o ./object/

clean:
	rm -f ./object/*
	rm -f ./binary/*