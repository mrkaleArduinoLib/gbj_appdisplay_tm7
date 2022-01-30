#include "gbj_appdisplay_tm7.h"
const String gbj_appdisplay_tm7::VERSION = "GBJ_APPDISPLAY_TM7 1.0.0";

gbj_appdisplay_tm7::ResultCodes gbj_appdisplay_tm7::errorHandler(
  gbj_tm1637::ResultCodes errDisplay)
{
  ResultCodes errApp = ResultCodes::ERROR_UNKNOWN;
  switch (errDisplay)
  {
    case gbj_tm1637::SUCCESS:
      errApp = ResultCodes::SUCCESS;
      break;

    case gbj_tm1637::ERROR_PINS:
      errApp = ResultCodes::ERROR_PINS;
      break;

    case gbj_tm1637::ERROR_ACK:
      errApp = ResultCodes::ERROR_ACK;
      break;
  }
  return errApp;
}
