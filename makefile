all:clean comp
	gcc Source/xct.c -o ./build/xct.o -c
	gcc -o ./build/xct.sh ./build/*.o
comp:
	gcc Source/Args/args.c -o ./build/args.o -c
	gcc Source/Args/arg_set.c -o ./build/arg_set.o -c
	gcc Source/Util/util.c -o ./build/util.o -c
	gcc Source/Util/device_parsing.c -o ./build/device_parsing.o -c
	gcc Source/TestSuiteParser/fparse_testsuite.c -o ./build/file_parse.o -c
	gcc Source/TestCase/matcher.c -o ./build/matcher.o -c
	gcc Source/CommandExec/exec_runner.c -o ./build/exec_runner.o -c
	gcc Source/CommandExec/exec_command.c -o ./build/exec_command.o -c
clean:
	rm ./build/*.o ./build/*.sh &

testcomp:comp
	gcc ./Frameworks/Unity/src/unity.c -o ./build/unity.o -c
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
