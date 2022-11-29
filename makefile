all:clean comp
	gcc src/xct.c -o ./build/xct.o -c
	gcc -o ./build/xct.sh ./build/*.o
comp:
	gcc src/args/args.c -o ./build/args.o -c
	gcc src/args/arg_set.c -o ./build/arg_set.o -c
	gcc src/util/util.c -o ./build/util.o -c
	gcc src/util/device_parsing.c -o ./build/device_parsing.o -c
	gcc src/fparse/fparse_testsuite.c -o ./build/file_parse.o -c
	gcc src/matching/matcher.c -o ./build/matcher.o -c
	gcc src/exec/exec_runner.c -o ./build/exec_runner.o -c
	gcc src/exec/exec_command.c -o ./build/exec_command.o -c
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
