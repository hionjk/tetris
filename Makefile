CC = gcc
FLAGS = -Wall
SRC = main
EXEC = tetris
LAUNCHER = launcher

H_DIR = ./headers

INCLUDES = -I/opt/homebrew/Cellar/raylib/5.0/include
LIBS = -L/opt/homebrew/Cellar/raylib/5.0/lib -lraylib

$(EXEC): $(SRC).o ui.o figure.o field.o
	$(CC) $(FLAGS) $(SRC).c ui.o figure.o field.o -o $(EXEC) $(INCLUDES) $(LIBS) 

$(LAUNCHER): $(LAUNCHER).o
	$(CC) $(FLAGS) $(LAUNCHER).c -o $(LAUNCHER) $(INCLUDES) $(LIBS) 

$(LAUNCHER).o: $(LAUNCHER).c
	$(CC) $(FLAGS) -c $(LAUNCHER).c $(INCLUDES)

$(SRC).o: $(SRC).c
	$(CC) $(FLAGS) -c $(SRC).c $(INCLUDES)

ui.o: ui.c $(H_DIR)/structs.h $(H_DIR)/constants.h
	$(CC) $(FLAGS) -c ui.c $(INCLUDES)

figure.o: figure.c $(H_DIR)/structs.h $(H_DIR)/constants.h $(H_DIR)/enums.h
	$(CC) $(FLAGS) -c figure.c $(INCLUDES)

field.o: field.c $(H_DIR)/structs.h $(H_DIR)/constants.h
	$(CC) $(FLAGS) -c field.c $(INCLUDES)

clean:
	rm -rf *.o
