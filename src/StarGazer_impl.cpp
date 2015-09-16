#include "StarGazer_impl.h"

static const char SYM_STX = '~';
static const char SYM_ETX = '`';
static const char SYM_READ = '@';
static const char SYM_WRITE = '#';
static const char SYM_RETURN = '$';
static const char SYM_ACK = '!';
static const char SYM_MESSAGE = '*';
static const char SYM_DELIM = '|';

static const char MSG_CALCSTART[] = "CalcStart";
static const char MSG_CALCSTOP[] = "CalcStop";
static const char MSG_VERSION[] = "Version";

StarGazer_impl::StarGazer_impl(const char* filename, const int baudrate /* = 115200 */) {
  m_pSerialPort = new ssr::SerialPort(filename, baudrate);
}

StarGazer_impl::~StarGazer_impl() {
  delete m_pSerialPort;
}

void StarGazer_impl::_sendPacket(const char symbol, const char* message) {
  m_pSerialPort->Write(&SYM_STX, 1);
  m_pSerialPort->Write(&symbol, 1);
  m_pSerialPort->Write(message, strlen(message));
  m_pSerialPort->Write(&SYM_ETX, 1);
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
  } while (msg[0] != SYM_STX);

  do {
    if (m_pSerialPort->ReadWithTimeout(msg, 1, timeout) < 0) {
      throw TimeoutException();
    }
    msg++;
    m_receiveTimer.tack(&currentTime);
    if (currentTime > timeout) {
      throw TimeoutException();
    }
  } while (msg[0] != SYM_ETX);
  msg[0] = 0;
}

void StarGazer_impl::_waitReply(const char symbol, const char* message) {
  char buffer[BUFFER_LEN];
  _waitReply(symbol, message, buffer, BUFFER_LEN);
}

void StarGazer_impl::_waitReply(const char symbol, const char* message, char* buffer, const unsigned int buffer_len) {
  do {
    unsigned int read_len = 0;
    _receivePacket(buffer, buffer_len, &read_len, m_timeout);
    if (buffer[0] != symbol) continue;
  } while(strncmp(buffer, message, strlen(message)) != 0);
}

bool StarGazer_impl::_messagePop(const char* message, const unsigned int startIndex, unsigned int* stopIndex, char* buffer)
{
  unsigned int len = strlen(message) - startIndex;
  unsigned int i;
  for(i = 0;i < len;i++) {
    buffer[i] = message[i];
    if (buffer[i] == SYM_DELIM) {
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
  _sendPacket(SYM_WRITE, MSG_CALCSTART);
  _waitReply(SYM_ACK, MSG_CALCSTART);
}


void StarGazer_impl::calcStop() {
  _sendPacket(SYM_WRITE, MSG_CALCSTOP);
  _waitReply(SYM_ACK, MSG_CALCSTOP);
}

void StarGazer_impl::_read(const char* message, char* buffer, const unsigned int buffer_len)
{
  char strbuf[BUFFER_LEN];
  char value[BUFFER_LEN];
  unsigned int stopIndex = 0;
  _sendPacket(SYM_READ, message);
  _waitReply(SYM_RETURN, message, strbuf, BUFFER_LEN);
  _messagePop(strbuf, strlen(message)+1, &stopIndex, buffer);
}

void StarGazer_impl::_write(const char* message, const char* value)
{
  char strbuf[BUFFER_LEN];
  sprintf(strbuf, "%s%c%s", message, SYM_DELIM, value);
  _sendPacket(SYM_WRITE, strbuf);
  _waitReply(SYM_ACK, message);
}

std::string StarGazer_impl::getVersion() {
  char value[BUFFER_LEN];
  _read(MSG_VERSION, value, BUFFER_LEN);
  return std::string(value);
}
