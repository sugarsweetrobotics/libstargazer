#include <stdio.h>

#include "stargazer.h"
#include "Console.h"

SG_HANDLE stargazer;
SG_RESULT result_t;
char strbuf[256];
unsigned int buflen = 256;
SG_MARKTYPE markType;
int fix;
int num;
SG_ID id;
double height;
SG_MODE mode;

/**
 *
 */
int calc_stop() {
  if ((result_t = stargazer_calcStop(stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_calcStop failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_calcStop() Success\n");
  return 0;
}

/**
 *
 */
int print_status() {
  if ((result_t = stargazer_getVersion(stargazer, strbuf, buflen)) != SG_OK) {
    printf("# ERROR: stargazer_getVersion failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getVersion() Success: %s\n", strbuf);

  if ((result_t = stargazer_getReferenceID(stargazer, &id)) != SG_OK) {
    printf("# ERROR: stargazer_getReferenceID failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getReferenceID() Success: %d\n", id);

  if ((result_t = stargazer_getMarkType(stargazer, &markType)) != SG_OK) {
    printf("# ERROR: stargazer_getMarkType failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getMarkType() Success: %d\n", markType);

  if ((result_t = stargazer_isLandmarkHeightFix(stargazer, &fix)) != SG_OK) {
    printf("# ERROR: stargazer_isLandmarkHeightFix failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_isLandmarkHeightFix() Success: %d\n", fix);

  if ((result_t = stargazer_getLandmarkHeight(stargazer, &height)) != SG_OK) {
    printf("# ERROR: stargazer_getLandmarkHeight failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getLandmarkHeight() Success: %f\n", height);

  if ((result_t = stargazer_getNumOfLandmark(stargazer, &num)) != SG_OK) {
    printf("# ERROR: stargazer_getNumOfLandmark failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getNumOfLandmark Success: %d\n", num);
  
  if ((result_t = stargazer_getMode(stargazer, &mode)) != SG_OK) {
    printf("# ERROR: stargazer_getMode failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_getMode Success: %d\n", mode);

  return 0;
}

void print_help() {
  printf("Help:\n");
}

void view_mode() {
  SG_ID id;
  double x, y, z, a;
  stargazer_calcStart(stargazer);
  while(1) {
    stargazer_getPosition(stargazer, &id, &x, &y, &z, &a);
    printf(" - %d %4.2f %4.2f %4.2f %4.2f\n", id, x, y, z, a); 
    if (myKbhit()) {
      int c = myGetch();
      if (c == 'q') {
	break;
      }
    }
  }
  stargazer_calcStop(stargazer);

}

void cbPosition(SG_ID id, double x, double y, double z, double a) {
  printf(" - POSITION: %d %4.2f %4.2f %4.2f %4.2f\n", id, x, y, z, a); 
}

void cbMapID(SG_ID id) {
  printf(" # MAPID %d REGISTERED!\n", id);
}

void cbParameterUpdate() {
  printf(" # PARAMETERUPDATE\n");
}

int mapbuild_mode() {

  if ((result_t = stargazer_startMapBuild(stargazer, cbPosition, cbMapID, cbParameterUpdate)) != SG_OK) {
    printf("# ERROR: stargazer_startMapBuild failed. returns (%d)\n", result_t);
    return -1;
  }
    
  return 0;
}

/**
 *
 */
int main(const int argc, const char* argv[]) {
  int endflag = 0;
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

  init_scr();
  if (calc_stop() != 0)  return -1;
  if (print_status() != 0) return -1;

  while(!endflag) {
    printf("Waiting Command... (h for help):\n");
    while(1) {
      if (myKbhit()) {
	int c = myGetch();
	switch(c) {
	case 'q':
	case ESCAPE:
	  printf("Exiting...\n");
	  endflag = 1;
	  break;
	case 'p':
	  print_status();
	  break;
	case 'v':
	  view_mode();
	  break;
	case 'm':
	  mapbuild_mode();
	  break;
	case 'h': 
	default:
	  print_help();
	}
	break;
      }
    }	  
  }
  
  if ((result_t = stargazer_fini(stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_fini failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_fini() Success.\n");
  
  exit_scr();
  return 0;
}
