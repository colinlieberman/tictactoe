CFLAGS=-Wall -g
CC=gcc
BIN_FILE=tictactoe.o
DSYM_DIR=tictactoe.dSYM

all: tictactoe.c menu.c play.c strategy.c
	$(CC) $(CFLAGS) -o $(BIN_FILE) tictactoe.c menu.c play.c strategy.c

clean:
	rm -dfr $(BIN_FILE) $(DSYM_DIR) 
