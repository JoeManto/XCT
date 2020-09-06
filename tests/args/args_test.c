#include "../../xct.h"
#include "../../Unity/src/unity.h"
#include "../../args.h"

extern args* PRO_ARGS;

void setUp(void){
  init_args();
}

void test_initArgs_AbleToInit() {
  TEST_ASSERT_NOT_NULL(PRO_ARGS);
}

void test_deinitArgs_AbleToDeInit() {
  dealloc_args();
  TEST_ASSERT_NULL(PRO_ARGS);
}

/*
*  args_setMatchingProps tests
*/

void test_args_setMatchingProps_NULLMatchingString_Fail() {
  int error = args_setMatchingProps(NULL, 'e');
  TEST_ASSERT_TRUE(error);
}

void test_args_setMatchingProps_ValidMatchingString_Success(){
  int error = args_setMatchingProps("test", 'e');
  TEST_ASSERT_FALSE(error);
  TEST_ASSERT_NOT_NULL(PRO_ARGS->matchingString);
  TEST_ASSERT_EQUAL_INT(0, PRO_ARGS->matchingType);
}

void test_args_setMatchingProps_NULLMatchingType_DefaultsToRegex(){
  int error = args_setMatchingProps("test", -1);
  enum Matcher regexMatcher = regex;
  TEST_ASSERT_FALSE(error);
  TEST_ASSERT_EQUAL_INT(regex, PRO_ARGS->matchingType);
}

/*
* args_clearPropIfNeeded tests
*/

void test_args_clearPropIfNeeded_NONNULLValue_Nullifies(){
	//set struct value for matchingString
	args_setMatchingProps("test", 'e');
	TEST_ASSERT_NOT_NULL(PRO_ARGS->matchingString);
	args_clearPropIfNeeded(&PRO_ARGS->matchingString);
	TEST_ASSERT_NULL(PRO_ARGS->matchingString);
}

void test_args_clearPropIfNeeded_NULLValue_KeepNull(){
	PRO_ARGS->matchingString = NULL;
	TEST_ASSERT_NULL(PRO_ARGS->matchingString);
	args_clearPropIfNeeded(&PRO_ARGS->matchingString);
	TEST_ASSERT_NULL(PRO_ARGS->matchingString);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_initArgs_AbleToInit);
  RUN_TEST(test_deinitArgs_AbleToDeInit);
  RUN_TEST(test_args_setMatchingProps_NULLMatchingString_Fail);
  RUN_TEST(test_args_setMatchingProps_ValidMatchingString_Success);
  RUN_TEST(test_args_setMatchingProps_NULLMatchingType_DefaultsToRegex);
	RUN_TEST(test_args_clearPropIfNeeded_NONNULLValue_Nullifies);
  return UNITY_END();
}

void tearDown(void){
  dealloc_args();
}
