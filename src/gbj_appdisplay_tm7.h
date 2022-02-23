/*
  NAME:
  gbj_appdisplay_tm7

  DESCRIPTION:
  Application library for managing TM1637 display.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
 */
#ifndef GBJ_APPDISPLAY_TM7_H
#define GBJ_APPDISPLAY_TM7_H

#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "font7seg_decnums.h"
#include "gbj_appcore.h"
#include "gbj_serial_debug.h"
#include "gbj_timer.h"
#include "gbj_tm1637.h"

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "gbj_appdisplay_tm7"

class gbj_appdisplay_tm7 : gbj_appcore
{
public:
  const char *VERSION = "GBJ_APPDISPLAY_TM7 1.0.0";

  /*
    Constructor.

    DESCRIPTION:
    Constructor creates the class instance object for managing TM1637
    display.

    PARAMETERS:
    pinClk - Number of a GPIO pin of the microcontroller for the serial clock.
      - Data type: non-negative integer
      - Default value: none
      - Limited range: 0 ~ 255
    pinDio - Number of a GPIO pin of the microcontroller fo the serial data.
      - Data type: non-negative integer
      - Default value: none
      - Limited range: 0 ~ 255
    digits - Number of 7-segment digits of the TM1637 to be utilized.
      - Data type: non-negative integer
      - Default value: 4
      - Limited range: 0 ~ 6

    RETURN: object
  */
  inline gbj_appdisplay_tm7(byte pinClk, byte pinDio, byte digits = 4)
  {
    display_ = new gbj_tm1637(pinClk, pinDio, digits);
    timer_ = new gbj_timer(Timing::PERIOD_BLINK);
    timer_->halt();
    // Calculate maximal error code
    byte errDigits = display_->getDigits() - 2;
    errMax_ = 1;
    while (errDigits--)
    {
      errMax_ *= 10;
    }
    errMax_ -= 1;
  }

  /*
    Initialization.

    DESCRIPTION:
    The method should be called in the SETUP section of a sketch. It references
    instance objects of related devices, especially for display device.

    PARAMETERS: none

    RETURN: none
  */
  inline void begin()
  {
    SERIAL_TITLE("begin")
    if (display_->begin() || display_->setContrastMin())
    {
      printError(errorHandler(display_->getLastResult()));
    }
    display_->setFont(gbjFont7segTable, sizeof(gbjFont7segTable));
    printInit();
    setDataPrintOn();
  }

  inline void run()
  {
    if (timer_->run())
    {
      if (flBlink_)
      {
        display_->displayToggle();
      }
      if (flBreath_)
      {
        display_->displayBreath();
      }
    }
  }

  inline void blinkStart()
  {
    flBlink_ = true;
    timeStart();
  }

  inline void blinkStop()
  {
    flBlink_ = false;
    timeStop();
  }

  inline void breathStart()
  {
    flBreath_ = true;
    timeStart();
  }

  inline void breathStop()
  {
    flBreath_ = false;
    timeStop();
  }

  inline void printText(String msg, byte digit = 0)
  {
    display_->displayOn();
    display_->printText(msg, digit);
    display_->display();
  }

  inline void printTextBlink(String msg, byte digit = 0)
  {
    blinkStart();
    display_->printText(msg, digit);
    display_->display();
  }

  inline void printTextBreath(String msg, byte digit = 0)
  {
    breathStart();
    display_->printText(msg, digit);
    display_->display();
  }

  inline void printError(unsigned int err)
  {
    err = min(err, errMax_);
    String errMsg = String(err);
    while ((byte)errMsg.length() < display_->getDigits() - 2)
    {
      errMsg = "0" + errMsg;
    }
    errMsg = "-" + errMsg + "-";
    printText(errMsg);
  }

  inline void printInit()
  {
    String msg;
    byte dashes = display_->getDigits();
    msg.reserve(dashes);
    while (dashes--)
    {
      msg += "-";
    }
    breathStop();
    blinkStop();
    printText(msg);
  }
  inline void printData() { printText(String(data_, 1)); }

  // Setters
  inline void setDataPrintOn()
  {
    breathStop();
    blinkStop();
    flData_ = true;
  };
  inline void setDataPrintOff() { flData_ = false; };
  inline void setData(float data)
  {
    data_ = data;
    if (flData_)
    {
      printData();
    }
  }

  // Getters
  inline gbj_tm1637 *getDisplayPtr() { return display_; }

private:
  enum Timing : unsigned int
  {
    PERIOD_BLINK = 500,
  };
  gbj_timer *timer_;
  gbj_tm1637 *display_;
  bool flData_, flBlink_, flBreath_;
  float data_;
  unsigned int errMax_;
  ResultCodes errorHandler(gbj_tm1637::ResultCodes errDisplay);
  void timeStart()
  {
    if (!timer_->isActive())
    {
      timer_->resume();
      display_->setContrastMax();
    }
  }
  void timeStop()
  {
    if (timer_->isActive() && !flBlink_ && !flBreath_)
    {
      timer_->halt();
      display_->setContrastMin();
    }
  }
};

#endif
