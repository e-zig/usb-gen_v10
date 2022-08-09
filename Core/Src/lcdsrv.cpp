/*
 * lcdsrv.cpp
 *
 *  Created on: 8 окт. 2021 г.
 *      Author: ziged
 */

#include "ssd1306.h"
#include <stdio.h>
#include <stdarg.h>

void lcdOnOff(const uint8_t on) {
  ssd1306_SetDisplayOn(on);
}

void lcdWrite(uint32_t line, char *str) {
  ssd1306_SetCursor(2, (line - 1) * 11);
  ssd1306_WriteString(str, Font_7x10, White);
}

void lcdWrite(char *str) {
  ssd1306_WriteString(str, Font_7x10, White);
}

extern bool printfToLCD;

// обязательно в конце \n
int lcdprintf(uint32_t line, const char *str, ...) {
  printfToLCD = true;
  ssd1306_SetCursor(2, (line - 1) * 11);
  va_list vargs;
  va_start(vargs, str);
  int res = vprintf(str, vargs);
  va_end(vargs);
  return res;
}

void lcdClear() {
  ssd1306_Fill(Black);
}

void lcdUpdate() {
  ssd1306_UpdateScreen();
}

void lcdInit(uint32_t firmware_version) {
  ssd1306_Init();
  lcdClear();
  lcdWrite(1, (char*) ("InSys USB Gen v1.0"));
  lcdprintf(2, "FW: %lX\n", firmware_version);
}
