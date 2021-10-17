stalk: main.o list.o socket.o thread.o
	gcc -o stalk main.o list.o socket.o thread.o -pthread

list.o: list.c list.h
	gcc -c list.c

socket.o: socket.c socket.h
	gcc -c socket.c

thread.o: thread.c thread.h
	gcc -c thread.c

main.o: main.c list.h socket.h thread.h
	gcc -c main.c

clean:
	rm *.o socket