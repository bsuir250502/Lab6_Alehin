OBJ = main.o
CFLAGS = -Wall -std=c99 -ggdb

all: bin

bin: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main

clean:
	rm main $(OBJ)