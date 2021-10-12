/*
 * comproc.cpp
 *
 *  Created on: 8 окт. 2021 г.
 *      Author: ziged
 */

#include "usbd_cdc_if.h"
#include <stdio.h>
#include "RwCBuf.hpp"
#include <flashsrv.h>
#include "build_defs.h"
#include "adcsrv.h"
#include "statustype.h"
#include "buttonsrv.h"
#include "lcdsrv.h"
#include "lmx2594Lite.hpp"
#include "pllsrv.h"
#include <inttypes.h>

// максимальная частота внешнего опорного генератора, Гц
// можно до 400000000, но тогда надо включать PLL_R_PRE
// пока ограничим 300000000, вручную можно ставить PLL_R_PRE
constexpr uint32_t Frefmax = 300000000;

void comProc(char *buf, DevStausStructType &devStatus) {

  char errMsg[] = "ERR\r\n";
  char okMsg[] = "OK\r\n";
  char *msg = errMsg;

  lcdClear();

#ifdef DEBUG
  CDC_Transmit_FS((uint8_t*) buf, strlen(buf));
  HAL_Delay(10);
#endif

  switch (buf[0]) {
    case 'a': {
      lcdWrite(1, (char*) ("Read All PLL Regs"));
      lcdWrite(2, (char*) (">>...>>"));
      msg = okMsg;
      uint16_t val { };
      for (int regNum = LMX2594::RegsFullCount - 1; regNum >= 0; --regNum) {
        if (!readPllReg(regNum, &val))
          msg = errMsg;
        printf("R%d\t0x%06X\r\n", regNum, (regNum << 16) | val);
        HAL_Delay(10);
      }
      break;
    }
    case 'r': {
      uint32_t regNum = atol((const char*) &buf[1]);
      lcdprintf(1, "Read PLL Reg: %lu\n", regNum);
      if (regNum < LMX2594::RegsFullCount) {
        uint16_t val { };
        if (readPllReg(regNum, &val))
          msg = okMsg;
        printf("R%lu\t%#lX\r\n", regNum, (regNum << 16) | val);
        lcdprintf(2, "R%lu %#lX\n", regNum, (regNum << 16) | val);
      }
      break;
    }
    case 'F': {
      uint64_t f = atoll((const char*) &buf[1]);
      if ((f >= LMX2594::fout_min) && (f <= LMX2594::fout_max)) {
        devStatus.Fgen = f;
        if (SetF(devStatus))
          msg = okMsg;
      }
      lcdWrite(1, (char*) ("Set Frequency:"));
      lcdprintf(2, "%.6f MHz\n", f / 1000000.0);
      break;
    }
    case 'O': {
      uint32_t f = atol((const char*) &buf[1]);
      if ((f >= LMX2594::fosc_min) && (f <= Frefmax)) {
        devStatus.Fextref = f;
        msg = okMsg;
      }
      lcdWrite(1, (char*) ("Set Ref Freq:"));
      lcdprintf(2, "%lu Hz\n", f);
      break;
    }
    case 'P': {
      uint32_t p = atol((const char*) &buf[1]);
      bool saveP = devStatus.Poff;
      if (setPllPwr(p)) {
        devStatus.Poff = (p == LMX2594::PWR_Off);
        if (!devStatus.Poff)
          devStatus.Pgen = p;

        if (devStatus.Poff) {
          lcdWrite(1, (char*) ("Set Power Off"));
          uint64_t saveF = devStatus.Fgen;
          devStatus.Fgen = 15000000000ull;
          SetF(devStatus);
          devStatus.Fgen = saveF;
        } else {
          lcdprintf(1, "Set Power: %lu\n", p);
          if (saveP)
            SetF(devStatus);
        }
        msg = okMsg;
      }
      break;
    }
    case 'Z': {
      lcdWrite(1, (char*) ("Prepare Reset"));
      lcdWrite(2, (char*) ("for next set F"));
      pllReset();
      /*      if (SetF(devStatus))
       msg = okMsg;
       lcdprintf(2, "%.6f MHz\n", devStatus.Fgen / 1000000.0); */
      break;
    }
    case 'z': {
      lcdWrite(1, (char*) ("ReInit PLL regs"));
      lcdWrite(2, (char*) ("for next set F"));
      pllRegsReset();
      break;
    }
    case 'G': {
      uint32_t g = atol((const char*) &buf[1]);
      msg = okMsg;
      //bool saveGen = devStatus.EnGen;
      devStatus.EnGen = (g > 0);
      SetGen(devStatus.EnGen);
      if (devStatus.EnGen)
        lcdprintf(1, "Int Gen %.1f MHz\n", devStatus.Fintref / 1000000.0f);
      else
        lcdprintf(1, "Ext Gen %.1f MHz\n", devStatus.Fextref / 1000000.0f);
      /*
       if (devStatus.EnGen != saveGen) {
       pllReset();
       HAL_Delay(10);
       if (SetF(devStatus.struc))
       lcdprintf(2, "Set %.1f MHz\n", devStatus.Fgen / 1000000.0f);
       else
       lcdprintf(2, "Err %.1f MHz\n", devStatus.Fgen / 1000000.0f);
       } */
      break;
    }
    case 'S': {
      lcdWrite(1, (char*) ("Save Status"));
      if (writeStatus(devStatus.arrAddr, devStatus.arrSize))
        msg = okMsg;
      break;
    }
    case 's': {
      lcdWrite(1, (char*) ("Save PreDef freq"));
      if (writeFreq(devStatus.preDefFreq, devStatus.preDefFreqCount + 1))
        msg = okMsg;
      break;
    }
    case 'L': {
      if (GetLD())
        msg = okMsg;
      break;
    }
    case 'M': {
      lcdWrite(1, (char*) ("Read Status"));
      lcdWrite(2, (char*) (">>...>>"));
      printf("F %.6f\r\n", devStatus.Fgen / 1000000.0);
      HAL_Delay(5);
      printf("Fint %lu\r\n", devStatus.Fintref);
      HAL_Delay(5);
      printf("Fext %lu\r\n", devStatus.Fextref);
      HAL_Delay(5);
      printf("P %u\r\n", devStatus.Pgen);
      HAL_Delay(5);
      printf("OFF %u\r\n", devStatus.Poff);
      HAL_Delay(5);
      printf("REF %u\r\n", devStatus.EnGen);
      HAL_Delay(5);
      printf("FN %lu\r\n", devStatus.preDefFreqCount);
      HAL_Delay(5);
      for (uint32_t i = 1; i <= devStatus.preDefFreqCount; ++i) {
        printf("F%lu %.1f\r\n", i, devStatus.preDefFreq[i] / 10.0f);
        HAL_Delay(5);
      }
      printf("I %lu\r\n", devStatus.preDefFreqCurIndex);
      HAL_Delay(5);
      printf("SM %lu\r\n", devStatus.scanMode);
      HAL_Delay(5);
      printf("ST %.6f\r\n", devStatus.startFscan / 1000000.0);
      HAL_Delay(5);
      printf("SP %.6f\r\n", devStatus.stopFscan / 1000000.0);
      HAL_Delay(5);
      printf("SS %.6f\r\n", devStatus.stepFscan / 1000000.0);
      HAL_Delay(5);
      printf("FW %lX\r\n", devStatus.FWver);
      HAL_Delay(5);
      msg = okMsg;
      break;
    }
    case 'A': {
      lcdWrite(1, (char*) ("Monitors:"));
      ADCdataType ADCdata { };
      extern ADC_HandleTypeDef hadc;
      GetADC(&hadc, ADCdata);
      printf("%u\r\n", ADCdata.V33);
      HAL_Delay(1);
      printf("%u\r\n", ADCdata.Vusb);
      HAL_Delay(1);
      printf("%d\r\n", ADCdata.T);
      HAL_Delay(1);
      lcdprintf(2, "%.2fV %.2fV %dC\n", ADCdata.V33 / 1000.0f,
                ADCdata.Vusb / 1000.0f, ADCdata.T);
#ifdef DEBUG
      printf("3.3V mon: %u mV\r\n", ADCdata.V33);
      HAL_Delay(1);
      printf("Vusb mon: %u mV\r\n", ADCdata.Vusb);
      HAL_Delay(1);
      printf("T: %dC\r\n", ADCdata.T);
      HAL_Delay(1);
      printf("Vref mon: %u\r\n", ADCdata.Vref);
      HAL_Delay(1);
#endif
      if ((ADCdata.V33 > 3150) && (ADCdata.V33 < 3450) && (ADCdata.Vusb > 4500)
          && (ADCdata.Vusb < 5500) && (ADCdata.T < 85))
        msg = okMsg;
    }
      break;
    case 'W': {
      //поиск и замена пробела на 0, определение номера ячейки
      uint32_t ind { };
      uint32_t i { 1 };
      while (buf[i] != 0) {
        if (buf[i] == ' ') {
          buf[i] = 0;
          ind = (uint32_t) atoi(&buf[1]);
          break;
        } else
          ++i;
      }

      if (ind == 0) {  // если не нашли - входим
#ifdef DEBUG
        printf("Space not found\r\n");
#endif
        break;
      }
      lcdprintf(1, "Set PreDef F%lu\n", ind);
      if ((ind > 0) && (ind <= devStatus.preDefFreqCount)) {
        uint32_t val = atol((const char*) &buf[i + 1]);
        if ((val >= LMX2594::fout_min / 100000)
            && (val <= LMX2594::fout_max / 100000)) {
          devStatus.preDefFreq[ind] = val;
          lcdprintf(2, "%.1f MHz\n", val / 10.0f);
          msg = okMsg;
        }
      }
#ifdef DEBUG
      printf("Write to flash F%lu: %.1f MHz\r\n", ind,
             devStatus.preDefFreq[ind] / 10.0f);
#endif
      break;
    }
    case 'R': {
      //поиск и замена tab на 0, определение номера регистра
      uint8_t reg { 255 };
      int _si { };
      uint32_t i { 1 };
      while (buf[i] != 0) {
        if (buf[i] == '\t') {
          buf[i] = 0;
          _si = i + 4;
          reg = (uint8_t) atoi(&buf[1]);
          break;
        } else
          ++i;
      }
      // если не нашли - входим
      if (reg == 255) {
#ifdef DEBUG
        printf("Tab not found\r\n");
#endif
        break;
      }
      // определяем значение
      uint16_t val;
      val = (uint16_t) (strtol(&buf[_si], NULL, 16));
      lcdprintf(1, "Write to reg %d\n", reg);
      lcdprintf(2, "%#lX\n", (reg << 16) | val);
      if (writePllReg(reg, val))
        msg = okMsg;

#ifdef DEBUG
      printf("Write reg: %u val: %#X\r\n", reg, val);
#endif
    }
      break;
    default:
      lcdprintf(1, "Bad command: %c\n", buf[0]);
#ifdef DEBUG
      printf("Bad command: %c\r\n", buf[0]);
#endif
      break;
  }

  lcdprintf(3, (char*) ("Status: %s"), msg);
  lcdUpdate();
  CDC_Transmit_FS((uint8_t*) msg, strlen(msg));
}

