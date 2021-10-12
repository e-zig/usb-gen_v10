/*
 * buttonsrv.cpp
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#include "lmx2594Lite.hpp"
#include "buttonsrv.h"
#include <stdio.h>
#include "pllsrv.h"
#include "lcdsrv.h"

extern bool printfToLCD;

void setPreDefFreq(uint64_t freq, DevStausStructType &devStatus) {
  lcdClear();
  lcdprintf(1, "Set PreDef F%lu\n", devStatus.preDefFreqCurIndex);
  if ((freq > LMX2594::fout_max) || (freq < LMX2594::fout_min)) {
    lcdWrite(2, (char*) ("F out of range!"));
    lcdWrite(3, (char*) ("Status: ERR"));
  } else {
    lcdprintf(2, "Set F%lu %.1f MHz\n", devStatus.preDefFreqCurIndex,
              freq / 1000000.0f);
    devStatus.Fgen = freq;
    if (SetF(devStatus))
      lcdWrite(3, (char*) ("Status: OK"));
    else
      lcdWrite(3, (char*) ("Status: ERR"));
  }
  lcdUpdate();
}

// !!! индекс меняется от 1 до preDefFreqCount
// значение по индексу 0 содержит сигнатуру записи

void setNextPreDefFreq(DevStausStructType &devStatus) {
  ++devStatus.preDefFreqCurIndex;
  if (devStatus.preDefFreqCurIndex > devStatus.preDefFreqCount)
    devStatus.preDefFreqCurIndex = 1;
  setPreDefFreq(devStatus.preDefFreq[devStatus.preDefFreqCurIndex] * 100000ull,
                devStatus);
}

void setPrevPreDefFreq(DevStausStructType &devStatus) {
  if (devStatus.preDefFreqCurIndex == 1)
    devStatus.preDefFreqCurIndex = devStatus.preDefFreqCount;
  else
    --devStatus.preDefFreqCurIndex;
  setPreDefFreq(devStatus.preDefFreq[devStatus.preDefFreqCurIndex] * 100000ull,
                devStatus);
}

