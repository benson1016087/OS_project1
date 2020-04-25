
all: main

main:
	gcc -O2 process_util.c scheduler.c project1.c -o project1.exe

clean: 
	rm -f *.o
	rm -f *.exe


