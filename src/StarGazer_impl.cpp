#include <sstream>


#ifdef WIN32
#define _USE_MATH_DEFINES
#endif

#include <math.h>

#include "StarGazer_impl.h"
#define DEBUG


#ifdef DEBUG
#define dbg(fmt, ...) printf("%s:%d "fmt, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
    #else
#define dbg(fmt, ...) do {} while (0)
    #endif

static const char SYM_STX = '~';
static const char SYM_ETX = '`';
static const char SYM_READ = '@';
static const char SYM_WRITE = '#';
static const char SYM_RETURN = '$';
static const char SYM_ACK = '!';
static const char SYM_MESSAGE = '*';
static const char SYM_DELIM = '|';
static const char SYM_RESULT = '^';

static const char MSG_CALCSTART[] = "CalcStart";
static const char MSG_CALCSTOP[] = "CalcStop";
static const char MSG_SETEND[] = "SetEnd";
static const char MSG_PARAMETERUPDATE[] = "ParameterUpdate";
static const char MSG_VERSION[] = "Version";
static const char MSG_MARKTYPE[] = "MarkType";
static const char MSG_MARKMODE[] = "MarkMode";
static const char MSG_HEIGHTFIX[] = "HeightFix";
static const char MSG_REFID[] = "RefID";
static const char MSG_IDNUM[] = "IDNum";
static const char MSG_MARKHEIGHT[] = "MarkHeight";
static const char MSG_BAUDRATE[] = "Baudrate";
static const char MSG_CALCHEIGHT[] = "HeightCalc";
static const char MSG_MAPDATA[] = "I";
static const char MSG_CALCHEIGHTDATA[] = "Z";
static const char MSG_MAPBUILDDATA[] = "F";
static const char MSG_MAPID[] = "MAPID";

static const char MSG_MARK_HLD1S[] = "HLD1S";
static const char MSG_MARK_HLD2S[] = "HLD2S";
static const char MSG_MARK_HLD3S[] = "HLD3S";
static const char MSG_MARK_HLD1L[] = "HLD1L";
static const char MSG_MARK_HLD2L[] = "HLD2L";
static const char MSG_MARK_HLD3L[] = "HLD3L";

static const char MSG_YES[] = "Yes";
static const char MSG_NO[] = "No";
static const char MSG_START[] = "Start";
static const char MSG_ALONE[] = "Alone";
static const char MSG_MAP[] = "Map";

StarGazer_impl::StarGazer_impl(const char* filename, const int baudrate /* = 115200 */) {
  m_pSerialPort = new ssr::SerialPort(filename, baudrate);
  m_timeout.sec = 3;
  m_timeout.usec = 0;
}

StarGazer_impl::~StarGazer_impl() {
  delete m_pSerialPort;
}

void StarGazer_impl::_sendPacket(const char symbol, const char* message) {
  dbg("%c%s\n", symbol, message);
  m_pSerialPort->Write(&SYM_STX, 1);
  ssr::Thread::Sleep(10);
  m_pSerialPort->Write(&symbol, 1);
  ssr::Thread::Sleep(10);
  for (int i = 0;i < strlen(message);i++) {
    m_pSerialPort->Write(message+i, 1);
    ssr::Thread::Sleep(10);
  }
  m_pSerialPort->Write(&SYM_ETX, 1);
}

void StarGazer_impl::_receivePacket(char* message, const unsigned int buffer_len, unsigned int* read_len, ssr::TimeSpec& timeout)
{
  ssr::TimeSpec currentTime;
  m_receiveTimer.tick();
  dbg("called\n");
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
  } while ((msg-1)[0] != SYM_ETX);
  (msg-1)[0] = 0;
  dbg("return: %s\n", message);
}

void StarGazer_impl::_waitReply(const char symbol, const char* message, ssr::TimeSpec* pTimeout) {
  char buffer[BUFFER_LEN];
  _waitReply(symbol, message, buffer, BUFFER_LEN, pTimeout);
}

void StarGazer_impl::_waitReply(const char symbol, const char* message, char* buffer, const unsigned int buffer_len, ssr::TimeSpec* pTimeout) {
  dbg("called (%c, %s)\n", symbol, message);
  if (pTimeout == NULL) {
    pTimeout = &m_timeout;
  }
  do {
    unsigned int read_len = 0;
    _receivePacket(buffer, buffer_len, &read_len, *pTimeout);
  } while(buffer[0] != symbol || strncmp(buffer+1, message, strlen(message)) != 0);
  dbg("return : %s\n", buffer);
}

bool StarGazer_impl::_messagePop(const char* message, unsigned int* stopIndex, char* buffer)
{
  unsigned int i;
  for(i = 0;i < strlen(message);i++) {
    buffer[i] = message[i];
    if (buffer[i] == SYM_DELIM) {
      i++;
      *stopIndex = i;
      buffer[i-1] = 0;
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
  _messagePop(strbuf + strlen(message)+2, &stopIndex, buffer);
}

void StarGazer_impl::_write(const char* message, const char* value)
{
  char strbuf[BUFFER_LEN];
  sprintf(strbuf, "%s%c%s", message, SYM_DELIM, value);
  _sendPacket(SYM_WRITE, strbuf);
  _waitReply(SYM_ACK, message);
}

void StarGazer_impl::setEnd() {
  _sendPacket(SYM_WRITE, MSG_SETEND);
  ssr::TimeSpec timeout(10,0);
  _waitReply(SYM_ACK, MSG_PARAMETERUPDATE, &timeout);
}

std::string StarGazer_impl::getVersion() {
  char value[BUFFER_LEN];
  _read(MSG_VERSION, value, BUFFER_LEN);
  return std::string(value);
}

static SG_MARKTYPE __str_to_marktype(const char* value) {
  if (strcmp(value, MSG_MARK_HLD1S) == 0) {
    return SG_HLD1S;
  } else if (strcmp(value, MSG_MARK_HLD2S) == 0) {
    return SG_HLD2S;
  } else if (strcmp(value, MSG_MARK_HLD3S) == 0) {
    return SG_HLD3S;
  } else if (strcmp(value, MSG_MARK_HLD1L) == 0) {
    return SG_HLD1L;
  } else if (strcmp(value, MSG_MARK_HLD2L) == 0) {
    return SG_HLD2L;
  } else if (strcmp(value, MSG_MARK_HLD3L) == 0) {
    return SG_HLD3L;
  } 
  throw UnknownMarkTypeException(); 
}

static const char* __marktype_to_str(const SG_MARKTYPE type) {
  switch(type) {
  case SG_HLD1S: return MSG_MARK_HLD1S;
  case SG_HLD2S: return MSG_MARK_HLD2S;
  case SG_HLD3S: return MSG_MARK_HLD3S;
  case SG_HLD1L: return MSG_MARK_HLD1L;
  case SG_HLD2L: return MSG_MARK_HLD2L;
  case SG_HLD3L: return MSG_MARK_HLD3L;
  default: throw UnknownMarkTypeException();
  }
}

SG_MARKTYPE StarGazer_impl::getMarkType() {
  char value[BUFFER_LEN];
  _read(MSG_MARKTYPE, value, BUFFER_LEN);
  return __str_to_marktype(value);
}

void StarGazer_impl::setMarkType(const SG_MARKTYPE type) {
  _write(MSG_MARKTYPE, __marktype_to_str(type));
}

bool StarGazer_impl::isLandmarkHeightFix() {
  char value[BUFFER_LEN];
  _read(MSG_HEIGHTFIX, value, BUFFER_LEN);
  if (strcmp(value, MSG_YES) == 0) {
    return true;
  } 
  return false;
}

void StarGazer_impl::setLandmarkHeightFix(const bool yes) {
  if (yes) {
    _write(MSG_HEIGHTFIX, MSG_YES);
  } else {
    _write(MSG_HEIGHTFIX, MSG_NO);
  }
}

int StarGazer_impl::getNumOfLandmark() {
  char value[BUFFER_LEN];
  _read(MSG_IDNUM, value, BUFFER_LEN);
  std::istringstream iss(value);
  int d;
  iss >> d;
  return d;
}

void StarGazer_impl::setNumOfLandmark(const int num) {
  std::ostringstream oss;
  oss << num;
  _write(MSG_IDNUM, oss.str().c_str());
}

SG_ID StarGazer_impl::getReferenceID() {
  char value[BUFFER_LEN];
  _read(MSG_REFID, value, BUFFER_LEN);
  std::istringstream iss(value);
  int d;
  iss >> d;
  return (SG_ID)d;
}

void StarGazer_impl::setReferenceID(const SG_ID id) {
  std::ostringstream oss;
  oss << id;
  _write(MSG_REFID, oss.str().c_str());
}


double StarGazer_impl::getLandmarkHeight() {
  char value[BUFFER_LEN];
  _read(MSG_MARKHEIGHT, value, BUFFER_LEN);
  std::istringstream iss(value);
  double d;
  iss >> d;
  return d/1000.0;
}

void StarGazer_impl::setLandmarkHeight(const double height) {
  std::ostringstream oss;
  oss << ((int)(height * 1000));
  _write(MSG_MARKHEIGHT, oss.str().c_str());
}

SG_MODE StarGazer_impl::getMode() {
  char value[BUFFER_LEN];
  _read(MSG_MARKMODE, value, BUFFER_LEN);
  if (strcmp(value, MSG_ALONE) == 0) {
    return SG_ALONE;
  } else if (strcmp(value, MSG_MAP) == 0) {
    return SG_MAP;
  } 
  throw UnknownModeException();
}

void StarGazer_impl::setMode(const SG_MODE mode) {
  std::ostringstream oss;
  oss << mode;
  _write(MSG_MARKMODE, oss.str().c_str());
}

int StarGazer_impl::getBaudrate() {
  char value[BUFFER_LEN];
  _read(MSG_BAUDRATE, value, BUFFER_LEN);
  std::istringstream iss(value);
  int d;
  iss >> d;
  return d;
}

void StarGazer_impl::setBaudrate(const int baudrate) {
  std::ostringstream oss;
  oss << baudrate;
  _write(MSG_BAUDRATE, oss.str().c_str());
}

void StarGazer_impl::_extractPositionMessage(char* strbuf, SG_ID* id, double* x, double* y, double* z, double* a) {
  char value[BUFFER_LEN];
  unsigned int stopIndex = 0;
  char* ptr = strbuf + 2;
  _messagePop(ptr, &stopIndex, value);
  std::istringstream iss(value);
  iss >> *id;
  ptr += stopIndex;
  _messagePop(ptr, &stopIndex, value);
  iss.clear();
  iss.str(value);
  iss >> *a;
  *a = *a / 180.0 * M_PI;
  ptr += stopIndex;
  _messagePop(ptr, &stopIndex, value);
  iss.clear();
  iss.str(value);
  iss >> *x;
  *x /= 1000.0;
  ptr += stopIndex;
  _messagePop(ptr, &stopIndex, value);
  iss.clear();
  iss.str(value);
  iss >> *y;
  *y /= 1000.0;
  ptr += stopIndex;
  _messagePop(ptr, &stopIndex, value);
  iss.clear();
  iss.str(value);
  iss >> *z;
  *z /= 1000.0;
}
					    
void StarGazer_impl::getPosition(SG_ID* id, double* x, double* y, double* z, double* a) {
  char strbuf[BUFFER_LEN];
  _waitReply(SYM_RESULT, MSG_MAPDATA, strbuf, BUFFER_LEN);
  _extractPositionMessage(strbuf, id, x, y, z, a);
}

void StarGazer_impl::getPositionInCalcHeight(SG_ID* id, double* x, double* y, double* z, double* a) {
  char strbuf[BUFFER_LEN];
  _waitReply(SYM_RESULT, MSG_CALCHEIGHTDATA, strbuf, BUFFER_LEN);
  _extractPositionMessage(strbuf, id, x, y, z, a);
}

void StarGazer_impl::calcHeight() {
  _sendPacket(SYM_WRITE, MSG_CALCHEIGHT);
  ssr::TimeSpec timeout(10,0);
  _waitReply(SYM_ACK, MSG_PARAMETERUPDATE, &timeout);
}

void StarGazer_impl::startMapBuild( void* (cbPositionData)(SG_ID id, double x, double y, double z, double a),
				    void* (cbMapID)(SG_ID id),
				    void* (cbParameterUpdate)(void), 
				    ssr::TimeSpec *pTimeout) {

  char buffer[BUFFER_LEN];
  calcStop();
  setMode(SG_MAP);
  _write(MSG_MARKMODE, MSG_START);
  
  if (pTimeout == NULL) {
    pTimeout = &m_timeout;
  }
  while(1) {
    unsigned int read_len = 0;
    _receivePacket(buffer, BUFFER_LEN, &read_len, *pTimeout);

    if (buffer[0] == SYM_RESULT && strcmp(buffer+1, MSG_MAPBUILDDATA) == 0) {
      double x, y, z, a;
      SG_ID id;
      _extractPositionMessage(buffer, &id, &x, &y, &z, &a);
      cbPositionData(id, x, y, z, a);
    } else if (buffer[0] == SYM_MESSAGE && strcmp(buffer+1, MSG_MAPID) == 0) {
      SG_ID id;
      unsigned int stopIndex = 0;
      char value[BUFFER_LEN];
      _messagePop(buffer + strlen(MSG_MAPID) + 2, &stopIndex, value);
      std::istringstream iss(value);
      iss >> id;
      cbMapID(id);
    } else if (buffer[0] == SYM_ACK && strcmp(buffer+1, MSG_PARAMETERUPDATE) == 0) {
      cbParameterUpdate();
    }
    
  } 
}
