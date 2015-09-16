#include <stdio.h>

#include "stargazer.h"



int main(const int argc, const char* argv[]) {
  SG_HANDLE stargazer;
  SG_RESULT result_t;
  char* filename = "/dev/ttyUSB0";

  char strbuf[256];
  unsigned int buflen = 256;

  printf("StarGazer Library Demo.\n");


  if ((result_t = stargazer_init(filename, &stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_init failed with (%s) returns (%d)\n", filename, result_t);
    return -1;
  }
  printf("# stargazer_init(%s) Success.\n", filename);

  if ((result_t = stargazer_getVersion(stargazer, strbuf, buflen)) != SG_OK) {
    printf("# ERROR: stargazer_getVersion failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getVersion() Success: %s\n", strbuf);



  if ((result_t = stargazer_fini(stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_fini failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_fini() Success.\n");

  return 0;
}
