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


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef int32_t SG_HANDLE;

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
  
  SG_RESULT stargazer_getVersion(const SG_HANDLE SG, char* version, uint32_t buffer_len);

  SG_RESULT stargazer_getMarkType(const SG_HANDLE SG, SG_MARKTYPE* markType);

  SG_RESULT stargazer_fini(const SG_HANDLE SG);



#ifdef __cplusplus
}
#endif
