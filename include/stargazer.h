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
} SG_RESULT;

SG_RESULT stargazer_init(const char* filename, SG_HANDLE* pSG);

SG_RESULT stargazer_getVersion(const SG_HANDLE SG, char* version, uint32_t buffer_len);

SG_RESULT stargazer_fini(const SG_HANDLE SG);



#ifdef __cplusplus
}
#endif
