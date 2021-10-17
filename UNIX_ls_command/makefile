all: UnixLs

UnixLs:	UnixLs.o
	gcc -o UnixLs UnixLs.o
	
UnixLs.o: UnixLs.o
	gcc -c UnixLs.c

clean:
	rm -f *.o *.s UnixLs