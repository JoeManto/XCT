#include "xct.h"
#include "args/args.h"
#include "fparse/fparse_testsuite.h"
#include <regex.h>
#include "fparse/matcher.h"

extern args* PRO_ARGS;

int main(int argc, char** argv){

	init_args();

	args_setMatchingProps("Example", 's');

	fparse_init();
	fparse_start("tests/supporting_files/test.swift");

	fparse_dealloc();
	dealloc_args();

  return 0;
}
