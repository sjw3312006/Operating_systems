main: process.o list.o main.o
	gcc -o main process.o list.o main.o

main.o: main.c list.h process.h
	gcc -c main.c

process.o: process.c list.h
	gcc -c process.c

list.o: list.c list.h 
	gcc -c list.c

clean:
	rm *.o main