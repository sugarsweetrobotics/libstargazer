#include <stdio.h>

#include "stargazer.h"



int main(const int argc, const char* argv[]) {
  SG_HANDLE stargazer;
  SG_RESULT result_t;
  char strbuf[256];
  unsigned int buflen = 256;
  SG_MARKTYPE markType;

  printf("StarGazer Library Demo.\n");

  if (argc == 1) {
    printf("# USAGE: stagazer_demo $SERIAL_DEVICE\n");
    return -1;
  }
  
  if ((result_t = stargazer_init(argv[1], &stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_init failed with (%s) returns (%d)\n", argv[1], result_t);
    return -1;
  }
  printf("# stargazer_init(%s) Success.\n", argv[1]);

  if ((result_t = stargazer_getVersion(stargazer, strbuf, buflen)) != SG_OK) {
    printf("# ERROR: stargazer_getVersion failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getVersion() Success: %s\n", strbuf);


  if ((result_t = stargazer_getMarkType(stargazer, &markType)) != SG_OK) {
    printf("# ERROR: stargazer_getMarkType failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getMarkType() Success: %d\n", markType);


  if ((result_t = stargazer_fini(stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_fini failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_fini() Success.\n");

  return 0;
}
