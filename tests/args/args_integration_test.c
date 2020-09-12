#include "../../xct.h"
#include "../../Unity/src/unity.h"
#include "../../args.h"

extern args* PRO_ARGS;

// Alloc more space than needed for modifications
char arg1[64] = "./build/xct.sh";
char arg2[64] = "-t testFile";
char arg3[64] = "-s Example";
char arg4[64] = "-P project.xcworkspace";
char arg5[64] = "-O os";
char arg6[64] = "-S scheme";
char arg7[64] = "-D device";
char * inputArgs[] = {arg1,arg2,arg3,arg4,arg5,arg6,arg7};

void setUp(void){
  init_args();
}

void test_args_parse_fullArgs(){
	parseArgs(7, inputArgs);
	TEST_ASSERT_EQUAL_STRING("testFile", PRO_ARGS->testTargetFile);
	TEST_ASSERT_EQUAL_STRING("Example", PRO_ARGS->matchingString);
	TEST_ASSERT_EQUAL_STRING("project.xcworkspace", PRO_ARGS->projectTarget);
	TEST_ASSERT_EQUAL_STRING("os", PRO_ARGS->os);
	TEST_ASSERT_EQUAL_STRING("scheme", PRO_ARGS->scheme);
	TEST_ASSERT_EQUAL_STRING("device", PRO_ARGS->device);
}

int main(void){
  UNITY_BEGIN();
	RUN_TEST(test_args_parse_fullArgs);
  return UNITY_END();
}

void tearDown(void){
  dealloc_args();
}

