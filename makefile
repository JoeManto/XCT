
all:clean comp
	./build/xct.sh
comp:
	gcc xct.c -o ./build/xct.o -c
	gcc args.c -o ./build/args.o -c
	gcc -o ./build/xct.sh ./build/xct.o ./build/args.o

clean:
	rm ./build/*.o ./build/*.sh
