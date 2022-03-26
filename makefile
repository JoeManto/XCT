all:clean comp
	gcc src/xct.c -o ./build/xct.o -c
	gcc -o ./build/xct.sh ./build/*.o
	./build/xct.sh
comp:
	gcc src/args/args.c -o ./build/args.o -c
	gcc src/util/util.c -o ./build/util.o -c
	gcc src/fparse/fparse_testsuite.c -o ./build/file_parse.o -c
	gcc src/fparse/matcher.c -o ./build/matcher.o -c
clean:
	rm ./build/*.o ./build/*.sh &

testcomp:comp
	gcc ./Unity/src/unity.c -o ./build/unity.o -c
argstest:clean testcomp
	gcc ./tests/args/args_test.c -o ./build/test_args.o -c
	gcc -o ./build/argstest.sh ./build/*.o
	./build/argstest.sh
argsinttest: clean testcomp
	gcc ./tests/args/args_integration_test.c -o ./build/test_args.o -c
	gcc -o ./build/argstest.sh ./build/*.o
	./build/argstest.sh
fparsetest:clean testcomp
	gcc ./tests/fparse/fparse_test.c -o ./build/fparse_test.o -c
	gcc -o ./build/fparsetest.sh ./build/*.o
	./build/fparsetest.sh
