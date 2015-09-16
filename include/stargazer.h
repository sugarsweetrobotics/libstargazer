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

enum SG_RESULT {
  SG_OK = 0,
  SG_FAILED = -1,
};

SG_RESULT stargazer_init(const char* filename, SG_HANDLE* pSG);

SG_RESULT stargazer_fini(const SG_HANDLE SG);




#ifdef __cplusplus
}
#endif
