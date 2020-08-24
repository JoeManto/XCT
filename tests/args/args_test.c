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
  deinit_args();
  TEST_ASSERT_NULL(PRO_ARGS);
}

/*
 *  args_setMatchingProps Tests 
 */

void test_args_setMatchingProps_NULLMatchingString_Fail() {
  int error = args_setMatchingProps(NULL, 'e');
  TEST_ASSERT_TRUE(error);
}

void test_args_setMatchingProps_ValidMatchingString_Success(){
  int error = args_setMatchingProps("test", 'e');
  TEST_ASSERT_FALSE(error);
  TEST_ASSERT_NOT_NULL(PRO_ARGS->matchingString);
  TEST_ASSERT_NOT_NULL(PRO_ARGS->matchingType);
}

void test_args_setMatchingProps_NULLMatchingType_DefaultsToRegex(){
  int error = args_setMatchingProps("test", -1);
  enum Matcher regexMatcher = regex;
  TEST_ASSERT_FALSE(error);
  TEST_ASSERT_EQUAL_INT(regex, PRO_ARGS->matchingType);
}

int main(void){
  UNITY_BEGIN();
  RUN_TEST(test_initArgs_AbleToInit);
  RUN_TEST(test_deinitArgs_AbleToDeInit);
  RUN_TEST(test_args_setMatchingProps_NULLMatchingString_Fail);
  RUN_TEST(test_args_setMatchingProps_ValidMatchingString_Success);
  RUN_TEST(test_args_setMatchingProps_NULLMatchingType_DefaultsToRegex);
  return UNITY_END();
}

void tearDown(void){
  deinit_args();
}
