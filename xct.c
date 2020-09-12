#include "xct.h"
#include "args.h"
#include "file_parse.h"

int main(int argc, char** argv){

	char arg1[] = "./build";
	char arg2[] = "test";

	char * args[] = {arg1,arg2};

	printf("%s\n",args[1]);

  return 0;
}
