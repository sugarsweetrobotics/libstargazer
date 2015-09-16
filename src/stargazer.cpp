#include "stargazer.h"
#include "StarGazer_impl.h"


static StarGazer_impl* g_pSG[MAX_SG];
static int g_counter = 0;

class InvalidHandleException : public StarGazerException {
public:
  InvalidHandleException() {}
  virtual ~InvalidHandleException() throw() {}
public:
  virtual const char* what() const throw() {
    return "Invalid Handle Exception";
  }
};


#define SG_TRY try {

#define SG_CATCH } catch (InvalidHandleException& ex) { \
  return SG_INVALID_HANDLE; \
} catch (std::exception& ex) {	\
  return SG_FAILED; \
}

inline StarGazer_impl *getSG(const SG_HANDLE SG) {
  if (SG >= MAX_SG || SG < 0) {
    throw InvalidHandleException();
  } else if (g_pSG[SG] == NULL) {
    throw InvalidHandleException();
  }
  return g_pSG[SG];
}

SG_RESULT stargazer_init(const char* filename, SG_HANDLE* pSG)
{
  if (g_counter == MAX_SG) {
    return SG_FAILED;
  }

SG_TRY
    g_pSG[g_counter] = new StarGazer_impl(filename);
SG_CATCH
  *pSG = g_counter;
  g_counter++;
  return SG_OK;
}

SG_RESULT stargazer_fini(const SG_HANDLE SG)
{
SG_TRY
  getSG(SG); // To generate invalid handle exception.
  delete g_pSG[SG];
  g_pSG[SG] = NULL;
SG_CATCH
  return SG_OK;
}


SG_RESULT stargazer_getVersion(const SG_HANDLE SG, char* version, uint32_t buffer_len) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  std::string version_str = sg->getVersion();
  strncpy(version, version_str.c_str(), buffer_len);
  SG_CATCH;
  return SG_OK;
}
