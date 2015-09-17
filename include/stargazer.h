/**
 * stargazer.h
 * libstargazer header file
 * date: 2015/09/16
 * author: Yuki Suga (SUGAR SWEET ROBOTICS)
 * email: ysuga@sugarsweetrobotics.com
 * url: https://github.com/sugarsweetrobotics/libstargazer.git
 * copyright: 2015, Yuki Suga, Sugar Sweet Robotics all rights reserved.
 * license: GPL
 */

#pragma once 
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
  

  typedef int32_t SG_HANDLE;
  typedef int32_t SG_ID;

  typedef void (*CB_FUNCTYPE_POSITION)(SG_ID id, double x, double y, double z, double a); 
  typedef void (*CB_FUNCTYPE_MAPID)(SG_ID id);
  typedef void (*CB_FUNCTYPE_PARAMETERUPDATE)(void);
  



  typedef enum _SG_MODE_ENUM {
    SG_ALONE,
    SG_MAP,
  } SG_MODE;

#define MAX_SG 16

  typedef enum _SG_RESULT_ENUM {
    SG_OK = 0,
    SG_FAILED = -1,
    SG_INVALID_HANDLE = -2,
    SG_TIMEOUT = -3,
  } SG_RESULT;
  
  typedef enum _SG_MARK_TYPE {
    SG_HLD1S,
    SG_HLD2S,
    SG_HLD3S,
    SG_HLD1L,
    SG_HLD2L,
    SG_HLD3L,
    SG_UNKNOWN,
  } SG_MARKTYPE;

  SG_RESULT stargazer_init(const char* filename, SG_HANDLE* pSG);

  SG_RESULT stargazer_fini(const SG_HANDLE SG);

  SG_RESULT stargazer_calcStart(const SG_HANDLE SG);

  SG_RESULT stargazer_calcStop(const SG_HANDLE SG);

  SG_RESULT stargazer_setEnd(const SG_HANDLE SG);
  
  SG_RESULT stargazer_getVersion(const SG_HANDLE SG, char* version, uint32_t buffer_len);

  SG_RESULT stargazer_getMarkType(const SG_HANDLE SG, SG_MARKTYPE* markType);

  SG_RESULT stargazer_setMarkType(const SG_HANDLE SG, const SG_MARKTYPE markType);

  SG_RESULT stargazer_isLandmarkHeightFix(const SG_HANDLE SG, int* yes);

  SG_RESULT stargazer_setLandmarkHeightFix(const SG_HANDLE SG, const int yes);

  SG_RESULT stargazer_getNumOfLandmark(const SG_HANDLE SG, int* num);

  SG_RESULT stargazer_setNumOfLandmark(const SG_HANDLE SG, const int num);

  SG_RESULT stargazer_getReferenceID(const SG_HANDLE SG, SG_ID* id);

  SG_RESULT stargazer_setReferenceID(const SG_HANDLE SG, const SG_ID id);

  SG_RESULT stargazer_getLandmarkHeight(const SG_HANDLE SG, double* haight);

  SG_RESULT stargazer_setLandmarkHeight(const SG_HANDLE SG, const double height);

  SG_RESULT stargazer_getMode(const SG_HANDLE SG, SG_MODE* mode);

  SG_RESULT stargazer_setMode(const SG_HANDLE SG, const SG_MODE mode);

  SG_RESULT stargazer_getBaudrate(const SG_HANDLE SG, int* baudrate);

  SG_RESULT stargazer_setBaudrate(const SG_HANDLE SG, const int baudrate);

  SG_RESULT stargazer_getPosition(const SG_HANDLE SG, SG_ID *id, double *x, double *y, double *z, double *a);

  SG_RESULT stargazer_calcHeight(const SG_HANDLE SG);

  SG_RESULT stargazer_startMapBuild( const SG_HANDLE SG, 
				  CB_FUNCTYPE_POSITION cbPositionData,
				  CB_FUNCTYPE_MAPID cbMapID,
				     CB_FUNCTYPE_PARAMETERUPDATE cbParameterUpdate);





#ifdef __cplusplus
}
#endif
