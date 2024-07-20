all:clean comp
	gcc Source/xct.c -o ./build/xct.o -c
	gcc -o ./build/xct.sh ./build/*.o
comp:
	gcc Source/Arguments/args.c -o ./build/args.o -c
	gcc Source/Arguments/arg_set.c -o ./build/arg_set.o -c
	gcc Source/Util/util.c -o ./build/util.o -c
	gcc Source/Util/string+util.c -o ./build/string+util.o -c
	gcc Source/TestSuiteParsing/fparse_testsuite.c -o ./build/fparse_testsuite.o -c
	gcc Source/TestCaseMatching/matcher.c -o ./build/matcher.o -c
	gcc Source/Exec/exec_command.c -o ./build/exec_command.o -c
	gcc Source/Exec/exec_runner.c -o ./build/exec_runner.o -c
	gcc Source/Environment/env_context.c -o ./build/env_context.o -c
	gcc Source/Environment/env_context_parser.c -o ./build/env_context_parser.o -c
	gcc Source/Environment/env_context_writer.c -o ./build/env_context_writer.o -c
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
