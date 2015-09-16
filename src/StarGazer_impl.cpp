#include "StarGazer_impl.h"


StarGazer_impl::StarGazer_impl(const char* filename, const int baudrate /* = 115200 */) {
  m_pSerialPort = new ssr::SerialPort(filename, baudrate);
}

StarGazer_impl::~StarGazer_impl() {
  delete m_pSerialPort;
}


#define STX '~'
#define STX_STR "~"
#define ETX '`'
#define ETX_STR "`"

void StarGazer_impl::_sendPacket(const char* message) {
  m_pSerialPort->Write(STX_STR, 1);
  m_pSerialPort->Write(message, strlen(message));
  m_pSerialPort->Write(ETX_STR, 1);
}

void StarGazer_impl::_receivePacket(char* message, const unsigned int buffer_len, unsigned int* read_len, ssr::TimeSpec& timeout)
{
  ssr::TimeSpec currentTime;
  m_receiveTimer.tick();
  char* msg = message;
  do {
    if (m_pSerialPort->ReadWithTimeout(msg, 1, timeout) < 0) {
      throw TimeoutException();
    }
    m_receiveTimer.tack(&currentTime);
    if (currentTime > timeout) {
      throw TimeoutException();
    }
  } while (msg[0] != STX);

  do {
    if (m_pSerialPort->ReadWithTimeout(msg, 1, timeout) < 0) {
      throw TimeoutException();
    }
    msg++;
    m_receiveTimer.tack(&currentTime);
    if (currentTime > timeout) {
      throw TimeoutException();
    }
  } while (msg[0] != ETX);
  msg[0] = 0;
}

void StarGazer_impl::_waitReply(const char* message) {
  char buffer[BUFFER_LEN];
  _waitReply(message, buffer, BUFFER_LEN);
}

void StarGazer_impl::_waitReply(const char* message, char* buffer, const unsigned int buffer_len) {
  do {
    unsigned int read_len = 0;
    _receivePacket(buffer, buffer_len, &read_len, m_timeout);
  } while(strncmp(buffer, message, strlen(message)) != 0);
}

bool StarGazer_impl::_messagePop(const char* message, const unsigned int startIndex, unsigned int* stopIndex, char* buffer)
{
  unsigned int len = strlen(message) - startIndex;
  unsigned int i;
  for(i = 0;i < len;i++) {
    buffer[i] = message[i];
    if (buffer[i] == '|') {
      i++;
      *stopIndex = i;
      buffer[i] = 0;
      return true;
    }
  }
  *stopIndex = i;
  buffer[i] = 0;
  return false;
}

void StarGazer_impl::calcStart() {
  _sendPacket("#CalcStart");
  _waitReply("!CalcStart");
}


void StarGazer_impl::calcStop() {
  _sendPacket("#CalcStop");
  _waitReply("!CalcStop");
}


std::string StarGazer_impl::getVersion() {
  char buffer[BUFFER_LEN];
  char value[BUFFER_LEN];
  unsigned int stopIndex = 0;
  _sendPacket("@Version");
  _waitReply("$Version", buffer, BUFFER_LEN);
  _messagePop(buffer, 8, &stopIndex, value);
  return std::string(value);
}
