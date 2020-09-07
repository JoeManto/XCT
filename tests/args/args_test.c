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
* args_setProjectProps tests
*/

void test_args_setProjectProps_NULLProject_Fail() {
	int error = args_setProjectProps(NULL);
	TEST_ASSERT_TRUE(error);
	TEST_ASSERT_NULL(PRO_ARGS->projectTarget);
}

void test_args_setProjectProps_WrongExtension_Fail() {
	int error = args_setProjectProps("test.notValid");
	TEST_ASSERT_TRUE(error);
	TEST_ASSERT_NULL(PRO_ARGS->projectTarget);
}

void test_args_setProjectProps_NoExtension_Fail() {
	int error = args_setProjectProps("test");
	TEST_ASSERT_TRUE(error);
	TEST_ASSERT_NULL(PRO_ARGS->projectTarget);
}

void test_args_setProjectProps_projectExtension_Success() {
	int error = args_setProjectProps("test.xcodeproj");
	enum Project projectType = project;
	TEST_ASSERT_FALSE(error);
	TEST_ASSERT_NOT_NULL(PRO_ARGS->projectTarget);
	TEST_ASSERT_EQUAL_INT(PRO_ARGS->projectType, projectType);
}

void test_args_setProjectProps_workspaceExtension_Success() {
	int error = args_setProjectProps("test.xcworkspace");
	enum Project projectType = workspace;
	TEST_ASSERT_FALSE(error);
	TEST_ASSERT_NOT_NULL(PRO_ARGS->projectTarget);
	TEST_ASSERT_EQUAL_INT(PRO_ARGS->projectType, projectType);
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
	RUN_TEST(test_args_clearPropIfNeeded_NULLValue_KeepNull);

	RUN_TEST(test_args_setProjectProps_NULLProject_Fail);
	RUN_TEST(test_args_setProjectProps_WrongExtension_Fail);
	RUN_TEST(test_args_setProjectProps_NoExtension_Fail);
	RUN_TEST(test_args_setProjectProps_projectExtension_Success);
	RUN_TEST(test_args_setProjectProps_workspaceExtension_Success);
  return UNITY_END();
}

void tearDown(void){
  dealloc_args();
}
