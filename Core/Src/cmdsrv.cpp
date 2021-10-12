/*
 * cmdsrv.cpp
 *
 *  Created on: 8 окт. 2021 г.
 *      Author: ziged
 */

#ifndef SRC_CMDSRV_CPP_
#define SRC_CMDSRV_CPP_

#include "stdint.h"
#include "RwCBuf.hpp"

#define cBufLength 512

using RwCBufType = RwCBuf<cBufLength, char>;

RwCBufType cBuf(RwCBufType::rwType::nonRwBuf);

void takeUsbBuf(uint8_t *buf, uint32_t len) {
  if ((buf != nullptr) && (len > 0)) {
    for (uint32_t i = 0; i < len; ++i) {
      if (cBuf.isFull())
        break;
      cBuf << static_cast<RwCBufType::value_type>(buf[i]);
    }
  }
}

bool isAbcDig(char c) {
  return (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z'))
      || ((c >= 'a') && (c <= 'z')));
}

void clearHeadcBuf() {
  char c;
  while (!cBuf.isEmpty())
    if (!isAbcDig(cBuf[0]))
      cBuf >> c;
    else
      break;
}

bool takeCmd(char *buf, uint32_t bufLength) {
// результат
  bool res = false;
// если буфер пуст
  if (cBuf.isEmpty())
    return res;
// очищаем начало буфера от неправильных символов
  clearHeadcBuf();
// индекс конца команды
  uint32_t endi { };
// ищем конец команды
  for (RwCBufType::size_type i = 0; i < cBuf.count(); ++i) {
    if (((cBuf[i] == '\r') || (cBuf[i] == '\n')) && (i > 0)) {
      endi = i;
      res = true;
      break;
    }
  }
// если не нашли
  if (!res) {
    if (cBuf.isFull())
      cBuf.clear();
    return res;
  }

// если длина строки с командой большая - удаляем ее
  if (endi >= bufLength) {
    for (RwCBufType::size_type i = 0; i < endi; ++i)
      cBuf >> buf[0];
    res = false;
  } else {  // иначе вычитываем в буфер команды
    for (RwCBufType::size_type i = 0; i < endi; ++i)
      cBuf >> buf[i];
    buf[endi] = 0;  // обозначаем конец строки
  }

  return res;
}

#endif /* SRC_CMDSRV_CPP_ */
