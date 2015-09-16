/**
 * StarGazer_impl.h
 * author: Yuki Suga (SUGAR SWEET ROBOTICS)
 * copyright: 2015, Yuki Suga, Sugar Sweet Robotics all rights reserved.
 * license: GPL
 */

#pragma once

#include <string>
#include "SerialPort.h"

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

class StarGazer_impl {
 private:
  ssr::SerialPort* m_pSerialPort;
  
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
  void _sendPacket(const char* message);
  void _receivePacket(char* message, const unsigned int buffer_len, unsigned int* read_len, ssr::TimeSpec& timeout);
  void _waitReply(const char* message);

  ssr::Timer m_receiveTimer;
  ssr::TimeSpec m_timeout;
 public:
  void calcStart();
  void calcStop();

  
  std::string getVersion();
};


#define BUFFER_LEN 256
