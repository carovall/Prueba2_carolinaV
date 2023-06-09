CC = gcc
CFLAGS = -Wall -Wextra -lm

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c -lm

run: main
	./main > datos.dat

plot:
	gnuplot graficar.gp

clean:
	rm -f main
