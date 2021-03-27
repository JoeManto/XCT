#include "xct.h"
#include "args.h"
#include "file_parse.h"
#include <regex.h>
#include "matcher.h"

int main(int argc, char** argv){

	init_args();
	fparse_init();
	fparse_start("test.swift");


	fparse_dealloc();
	dealloc_args();

  return 0;
}
