all:clean comp
	./build/xct.sh
comp:
	gcc xct.c -o ./build/xct.o -c
	gcc args.c -o ./build/args.o -c
	gcc util.c -o ./build/util.o -c
	gcc file_parse.c -o ./build/file_parse.o -c
	gcc matcher.c -o ./build/matcher.o -c
	gcc -o ./build/xct.sh ./build/*.o
clean:
	rm ./build/*.o ./build/*.sh
test:
	gcc file_parse.c -o ./build/file_parse.o -c
	gcc args.c -o ./build/args.o -c
	gcc util.c -o ./build/util.o -c
	gcc matcher.c -o ./build/matcher.o -c
	gcc ./Unity/src/unity.c -o ./build/unity.o -c
argstest: test
	gcc ./tests/args/args_test.c -o ./build/test_args.o -c
	gcc -o ./build/argstest.sh ./build/*.o
	./build/argstest.sh
fparsetest: test
	gcc ./tests/fparse_test.c -o ./build/fparse_test.o -c
	gcc -o ./build/fparsetest.sh ./build/*.o
	./build/fparsetest.sh
