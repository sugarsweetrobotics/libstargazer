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
 } catch (TimeoutException& ex) { \
  return SG_TIMEOUT; \
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

SG_RESULT stargazer_calcStart(const SG_HANDLE SG) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->calcStart();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_calcStop(const SG_HANDLE SG) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->calcStop();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setEnd(const SG_HANDLE SG) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setEnd();
  SG_CATCH;
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

SG_RESULT stargazer_getMarkType(const SG_HANDLE SG, SG_MARKTYPE* markType) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *markType = sg->getMarkType();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setMarkType(const SG_HANDLE SG, const SG_MARKTYPE markType) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setMarkType(markType);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_isLandmarkHeightFix(const SG_HANDLE SG, int* yes) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *yes = sg->isLandmarkHeightFix();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setLandmarkHeightFix(const SG_HANDLE SG, const int yes) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setLandmarkHeightFix(yes);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_getNumOfLandmark(const SG_HANDLE SG, int* num) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *num = sg->getNumOfLandmark();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setNumOfLandmark(const SG_HANDLE SG, const int num) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setNumOfLandmark(num);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_getReferenceID(const SG_HANDLE SG, SG_ID* id) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *id = sg->getReferenceID();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setReferenceID(const SG_HANDLE SG, const SG_ID id) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setReferenceID(id);
  SG_CATCH;
  return SG_OK;
}


SG_RESULT stargazer_getLandmarkHeight(const SG_HANDLE SG, double* height) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *height = sg->getLandmarkHeight();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setLandmarkHeight(const SG_HANDLE SG, const double height) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setLandmarkHeight(height);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_getMode(const SG_HANDLE SG, SG_MODE* mode) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *mode = sg->getMode();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setMode(const SG_HANDLE SG, const SG_MODE mode) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setMode(mode);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_getBaudrate(const SG_HANDLE SG, int* baudrate) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  *baudrate = sg->getBaudrate();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_setBaudrate(const SG_HANDLE SG, const int baudrate) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->setBaudrate(baudrate);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_getPosition(const SG_HANDLE SG, SG_ID* id, double *x, double *y, double *z, double *a) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->getPosition(id, x, y, z, a);
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_calcHeight(const SG_HANDLE SG) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  sg->calcHeight();
  SG_CATCH;
  return SG_OK;
}

SG_RESULT stargazer_startMapBuild(const SG_HANDLE SG,
				  CB_FUNCTYPE_POSITION cbPositionData,
				  CB_FUNCTYPE_MAPID cbMapID,
				  CB_FUNCTYPE_PARAMETERUPDATE cbParameterUpdate) {
  SG_TRY;
  StarGazer_impl *sg = getSG(SG);
  ssr::TimeSpec timeout(15, 0);
  sg->startMapBuild(cbPositionData, cbMapID, cbParameterUpdate, &timeout);
  SG_CATCH;
  return SG_OK;
} 


