#include "../../xct.h"
#include "../../Unity/src/unity.h"
#include "../../file_parse.h"

void setUp(void){
  fparse_init();
}

int main(void){
  UNITY_BEGIN();

  return UNITY_END();
}

void tearDown(void){
  fparse_dealloc();
}
