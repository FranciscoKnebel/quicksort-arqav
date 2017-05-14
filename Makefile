CC=gcc
CFLAGS=-Wall -g -Wextra
MAINFLAGS=-O3 -fopenmp -Wall -Wextra
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src
DST_DIR=./dist

## Style ##
BOLD=\033[1m
RED=\033[1;31m
END=\033[0m

all: $(LIB_DIR)/libquicksort.a
	$(CC) $(MAINFLAGS) -o $(DST_DIR)/main $(SRC_DIR)/main.c -L$(LIB_DIR) -lquicksort
	@echo "\nExecute '$(BOLD)./dist/main $(RED)<NumberOfElements> <NumberOfThreads>$(END)' to run the simulation."

## Library generation ##
$(LIB_DIR)/libquicksort.a: buildall
	ar rcs $(LIB_DIR)/libquicksort.a $(BIN_DIR)/parallel.o $(BIN_DIR)/sequential.o $(BIN_DIR)/sequential_std.o

## Quicksort implementations ##
buildall: $(BIN_DIR)/parallel.o $(BIN_DIR)/sequential.o $(BIN_DIR)/sequential_std.o

$(BIN_DIR)/parallel.o: $(SRC_DIR)/parallel.c
	$(CC) $(CFLAGS) -O3 -fopenmp -c -o $(BIN_DIR)/parallel.o -I$(INC_DIR) $(SRC_DIR)/parallel.c

$(BIN_DIR)/sequential.o: $(SRC_DIR)/sequential.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)/sequential.o -I$(INC_DIR) $(SRC_DIR)/sequential.c

$(BIN_DIR)/sequential_std.o: $(SRC_DIR)/sequential_std.c
	$(CC) $(CFLAGS) -c -o $(BIN_DIR)/sequential_std.o -I$(INC_DIR) $(SRC_DIR)/sequential_std.c

## Delete generated files ##
clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
