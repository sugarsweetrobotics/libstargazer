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

int calc_start() {
  if ((result_t = stargazer_calcStart(stargazer)) != SG_OK) {
    printf("# ERROR: stargazer_calcStart failed returns (%d)\n", result_t);
    return -1;
  }
  printf("# stargazer_calcStart() Success\n");
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

void mode_to_map() {
  stargazer_setMode(stargazer, SG_MAP);
  stargazer_setEnd(stargazer);
  SG_MODE mode;
  stargazer_getMode(stargazer, &mode);

  if (mode != SG_MAP) {
    printf("# ERROR: StarGazer can not change the mode.\n");
  }
}

void mode_to_alone() {
  stargazer_setMode(stargazer, SG_ALONE);
  stargazer_setEnd(stargazer);
  SG_MODE mode;
  stargazer_getMode(stargazer, &mode);

  if (mode != SG_ALONE) {
    printf("# ERROR: StarGazer can not change the mode.\n");
  }
}

void print_help() {
  printf("Help:\n");
}

void view_position() {
  SG_ID id;
  double x, y, z, a;
  stargazer_calcStart(stargazer);
  while(1) {
    stargazer_getPosition(stargazer, &id, &x, &y, &z, &a);
    printf(" - POSITION(id=%d) (x,y,z,a)=(%4.3f %4.3f %4.3f %4.3f)\n", id, x, y, z, a); 
    if (myKbhit()) {
      int c = myGetch();
      if (c == 'q') {
	break;
      }
    }
  }
  stargazer_calcStop(stargazer);

}


int set_marktype() {
  printf(" ? SELECT MARK TYPE:\n"
	 " 1: HLD1S\n"
	 " 2: HLD1L\n"
	 " 3: HLD2S\n"
	 " 4: HLD2L\n"
	 " 5: HLD3S\n"
	 " 6: HLD3L\n");
  while(1) {
    if (myKbhit()) { 
      int c = myGetch();
      int d = c - '0' - 1;
      if (d < 0 || d >= 6) {
	printf(" # ERROR: Invalid Value.\n");
	return -1;
      }

      printf(" ? INPUT = %c. OK? (y/n):\n", c);
      while(1) {
	if (myKbhit()) {
	  int c2 = myGetch();
	  if (c2 != 'y') {
	    printf(" # CHANGE ABORTED.\n");
	    return 0;
	  }
	  if ((result_t = stargazer_setMarkType(stargazer, d)) != SG_OK) {
	    printf("# ERROR: stargazer_setMarkType failed. returns (%d)\n", result_t);
	    return -1;
	  }
	  if ((result_t = stargazer_setEnd(stargazer)) != SG_OK) {
	    printf("# ERROR: stargazer_setEnd failed. returns (%d)\n", result_t);	    
	    return -1;
	  }
	}
      }
    }
  }
  return -1;
}

int set_markmode() {
  printf(" ? SELECT MARK TYPE:\n"
	 " 1: MAP\n"
	 " 2: ALONE\n");
  while(1) {
    if (myKbhit()) { 
      int c = myGetch();
      int d = c - '0' - 1;
      if (d < 0 || d >= 2) {
	printf(" # ERROR: Invalid Value.\n");
	return -1;
      }

      printf(" ? INPUT = %c. OK? (y/n):\n", c);
      while(1) {
	if (myKbhit()) {
	  int c2 = myGetch();
	  if (c2 != 'y') {
	    printf(" # CHANGE ABORTED.\n");
	    return 0;
	  }
	  if ((result_t = stargazer_setMode(stargazer, d)) != SG_OK) {
	    printf("# ERROR: stargazer_setMode failed. returns (%d)\n", result_t);
	    return -1;
	  }
	  if ((result_t = stargazer_setEnd(stargazer)) != SG_OK) {
	    printf("# ERROR: stargazer_setEnd failed. returns (%d)\n", result_t);	    
	    return -1;
	  }
	}
      }
    }
  }
  return -1;
}

int set_refid() {
  int value;
  
  printf(" ? INPUT REFID:");
  scanf("%d", &value);

  printf(" ? INPUT = %d. OK? (y/n):\n", value);
  while(1) {
    if (myKbhit()) {
      int c2 = myGetch();
      if (c2 != 'y') {
	printf(" # CHANGE ABORTED.\n");
	return 0;
      }
      if ((result_t = stargazer_setReferenceID(stargazer, value)) != SG_OK) {
	printf("# ERROR: stargazer_setRefID failed. returns (%d)\n", result_t);
	return -1;
      }
      if ((result_t = stargazer_setEnd(stargazer)) != SG_OK) {
	printf("# ERROR: stargazer_setEnd failed. returns (%d)\n", result_t);	    
	return -1;
      }
    }
  }

  return -1;
}

int set_nummarker() {
  int value;
  
  printf(" ? INPUT REFID:");
  scanf("%d", &value);

  printf(" ? INPUT = %d. OK? (y/n):\n", value);
  while(1) {
    if (myKbhit()) {
      int c2 = myGetch();
      if (c2 != 'y') {
	printf(" # CHANGE ABORTED.\n");
	return 0;
      }
      if ((result_t = stargazer_setNumOfLandmark(stargazer, value)) != SG_OK) {
	printf("# ERROR: stargazer_setRefID failed. returns (%d)\n", result_t);
	return -1;
      }
      if ((result_t = stargazer_setEnd(stargazer)) != SG_OK) {
	printf("# ERROR: stargazer_setEnd failed. returns (%d)\n", result_t);	    
	return -1;
      }
    }
  }

  return -1;
}


void cbPosition(SG_ID id, double x, double y, double z, double a) {
  printf(" - POSITION(id=%d) (x,y,z,a)=(%4.3f %4.3f %4.3f %4.3f)\n", id, x, y, z, a); 
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
  return calc_stop();
}


int main_routine(const int argc, const char* argv[]);

int main(const int argc, const char* argv[]) {
  int ret;
  init_scr();
  ret = main_routine(argc, argv);
  exit_scr();
  return ret;
}
/**
 *
 */
int main_routine(const int argc, const char* argv[]) {
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
	case '1':
	  calc_start();
	  break;
	case '2':
	  calc_stop();
	  break;
	case '3':
	  print_status();
	  break;
	case '4':
	  view_position();
	  break;
	case '7':
	  mapbuild_mode();
	  break;
	case '8':
	  set_refid();
	  break;
	case '9':
	  set_nummarker();
	  break;
	case '0':
	  set_marktype();
	  break;
	case 'a':
	  set_markmode();
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
  return 0;
}
