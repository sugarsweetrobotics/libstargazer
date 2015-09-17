/**
 * StarGazer_impl.h
 * author: Yuki Suga (SUGAR SWEET ROBOTICS)
 * copyright: 2015, Yuki Suga, Sugar Sweet Robotics all rights reserved.
 * license: GPL
 */

#pragma once

#include "stargazer.h"
#include <string>
#include "SerialPort.h"
#include "Thread.h"

class StarGazerException : public std::exception {
 public:
  StarGazerException() {}
  virtual ~StarGazerException() throw() {}

 public:
  virtual const char* what() const throw() {
    return "Star Gazer Exception";
  }
};

class TimeoutException : public std::exception {
 public:
  TimeoutException() {}
  virtual ~TimeoutException() throw() {}

 public:
  virtual const char* what() const throw() {
    return "Timeout";
  }
};

class UnknownMarkTypeException : public std::exception {
 public:
  UnknownMarkTypeException() {}
  virtual ~UnknownMarkTypeException() throw() {}

 public:
  virtual const char* what() const throw() {
    return "Unknown Mark Type Detected";
  }
};

class UnknownModeException : public std::exception {
 public:
  UnknownModeException() {}
  virtual ~UnknownModeException() throw() {}

 public:
  virtual const char* what() const throw() {
    return "Unknown Mark Mode Detected";
  }
};

class StarGazer_impl {
 private:
  ssr::SerialPort* m_pSerialPort;
  ssr::Mutex m_Mutex;
 public:
  /**
   * Constructor
   * @param filename File Name of Serial Port like COM1, COM2, /dev/ttyUSB0,..
   * @param baudrate Baudrate of Serial Port.
   * @throws ssr::COMException
   */
  StarGazer_impl(const char* filename, const int baudrate=115200);

  /**
   * Destructor
   */
  ~StarGazer_impl();

 private:
  void _sendPacket(const char symbol, const char* message);
  void _receivePacket(char* message, const unsigned int buffer_len, unsigned int* read_len, ssr::TimeSpec& timeout);

  void _waitReply(const char symbol, const char* message, ssr::TimeSpec* pTimeout =nullptr);
  void _waitReply(const char symbol, const char* message, char* buffer, const unsigned int buffer_len, ssr::TimeSpec* pTimeout=nullptr);

  void _read(const char* message, char* buffer, const unsigned int buffer_len);
  void _write(const char* message, const char* value);

  ssr::Timer m_receiveTimer;

  ssr::TimeSpec m_timeout;

  /**
   * @return If false if message ends.
   */
  bool _messagePop(const char* message, unsigned int* stopIndex, char* buffer);
 public:
  /**
   * Send a command to start calculation. After the command, the output of data including position and angle is obtained continuously.
   */
  void calcStart();

  /**
   * Send a command to stop calculating.
   */
  void calcStop();

  /**
   * Update Paramter
   */
  void setEnd();

 public:  

  /**
   * Firmware version.
   * @return version of firmware.
   */
  std::string getVersion();

  /**
   * Mark Type
   * @return MarkType number.
   * @see SG_MARKTYPE
   */
  SG_MARKTYPE getMarkType();

  /**
   * Set Mark Type
   * @param type SG_MARKTYPE
   * @see SG_MARKTYPE
   */
  void setMarkType(const SG_MARKTYPE type);

  /**
   * Land Mark Height Fixed?
   * @return true if yes.
   */
  bool isLandmarkHeightFix();

  /**
   * Land Mark Height fix set.
   * @param yes Fix when true.
   */
  void setLandmarkHeightFix(const bool yes);

  /**
   * Num of Landmark
   */
  int getNumOfLandmark();

  /**
   * Set Num of Landmark
   * @param num Num of Landmark
   */
  void setNumOfLandmark(const int num);

  /**
   * Reference ID
   */
  SG_ID getReferenceID();

  /**
   * Set Reference ID
   * @param id Reference ID
   */
  void setReferenceID(const SG_ID id);

  /**
   * Land Mark Height
   * @return Land Mark Height [m]
   */
  double getLandmarkHeight();

  /**
   * Set Landmark Height
   * @param height height [m]
   */
  void setLandmarkHeight(const double height);

  /**
   * Mode Of Mapping
   * @return SG_MAP or SG_ALONE
   */
  SG_MODE getMode();

  /**
   * set Mode of Mapping
   * @param mode SG_MAP or SG_ALONE
   */
  void setMode(const SG_MODE mode);

  /**
   * Baudrate of StarGazer
   * @return baudrate
   */
  int getBaudrate();

  /**
   * Baudrate of StarGazer
   * @param baudrate Baudrate
   */
  void setBaudrate(const int baudrate);
  
  /**
   * Get Position Of Marker
   * @param id [out] ID of marker
   * @param x [out] position.x [m]
   * @param y [out] position.y [m]
   * @param z [out] position.z [m]
   * @param a [out] angular posture in x-y plane (z-axis) [rad]
   */
  void getPosition(SG_ID* id, double* x, double* y, double* z, double* a);

  void calcHeight();
};


#define BUFFER_LEN 256
