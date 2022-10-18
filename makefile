

CC=gcc
INCLUDE = ./include/
OBJ = ./object/
SRC = ./source/
BIN = ./binary/
FLAG = gcc -g -Wall -I include/ -c

all : data.o entry.o tree.o test_data.o test_entry.o test_tree.o test_data test_entry test_tree

data.o: $(INCLUDE)data.h
	$(FLAG) $(SRC)data.c -o $(OBJ)data.o

entry.o: $(INCLUDE)entry.h $(INCLUDE)data.h
	$(FLAG) $(SRC)entry.c -o $(OBJ)entry.o

test_data.o: $(INCLUDE)data.h
	$(FLAG) $(SRC)test_data.c -o $(OBJ)test_data.o

test_entry.o: $(INCLUDE)data.h $(INCLUDE)entry.h
	$(FLAG) $(SRC)test_entry.c -o $(OBJ)test_entry.o

test_data: $(OBJ)test_data.o $(OBJ)data.o
	$(CC) $(OBJ)test_data.o $(OBJ)data.o -o $(BIN)test_data

test_entry: $(OBJ)test_entry.o $(OBJ)data.o $(OBJ)entry.o
	$(CC) $(OBJ)test_entry.o $(OBJ)data.o $(OBJ)entry.o -o $(BIN)test_entry

tree.o: $(INCLUDE)tree-private.h $(INCLUDE)tree.h $(INCLUDE)entry.h $(INCLUDE)data.h
	$(FLAG) $(SRC)tree.c -o $(OBJ)tree.o

test_tree.o: $(INCLUDE)tree-private.h $(INCLUDE)tree.h $(INCLUDE)data.h $(INCLUDE)entry.h
	$(FLAG) $(SRC)test_tree.c -o $(OBJ)test_tree.o

test_tree: $(OBJ)test_tree.o $(OBJ)data.o $(OBJ)entry.o $(OBJ)tree.o
	$(CC) $(OBJ)test_tree.o $(OBJ)data.o $(OBJ)entry.o $(OBJ)tree.o -o $(BIN)test_tree

clean:
	rm $(OBJ)*.o