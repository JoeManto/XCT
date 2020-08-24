
all:clean comp
	./build/xct.sh
comp:
	gcc xct.c -o ./build/xct.o -c
	gcc args.c -o ./build/args.o -c
	gcc util.c -o ./build/util.o -c
	gcc -o ./build/xct.sh ./build/*.o

clean:
	rm ./build/*.o ./build/*.sh

argstest:
	gcc args.c -o ./build/args.o -c
	gcc util.c -o ./build/util.o -c
	gcc ./tests/args/args_test.c -o ./build/test_args.o -c
	gcc ./Unity/src/unity.c -o ./build/unity.o -c
	gcc -o ./build/argstest.sh ./build/*.o
	./build/argstest.sh
