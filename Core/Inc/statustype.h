/*
 * statustype.h
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#ifndef INC_STATUSTYPE_H_
#define INC_STATUSTYPE_H_

//#include "stddef.h"

#define ALIGN_STATUS_STRUCT 4

struct DevStausStructType {
  uint32_t signature;  ///< Сигнатура записи
  uint64_t Fgen;  // частота PLL, Гц
  uint32_t Fintref;  // частота внутреннего опорного генератора, Гц
  uint32_t Fextref;  // частота внешнего опорного генератора, Гц
  uint8_t Pgen;  // мощность на выходе А
  bool Poff;  // флаг включения выхода OUTA
  bool EnGen;  // флаг включенного внутреннего опорного генератора
  uint32_t preDefFreqCurIndex;  // текущая частота из списка
  uint32_t preDefFreqCount;  // размер списка частот
  uint32_t *preDefFreq;  // указатель на список частот
  uint32_t scanMode;  //
  uint64_t startFscan;  //
  uint64_t stopFscan;  //
  uint64_t stepFscan;  //
  uint32_t FWver;  // версия прошивки
  uint32_t *arrAddr;  // адрес массива union с этой структурой
  uint32_t arrSize;  // размер массива union с этой структурой
} __attribute__((packed, aligned(ALIGN_STATUS_STRUCT)));

constexpr uint32_t DevStausStructSize = sizeof(DevStausStructType)
    / ALIGN_STATUS_STRUCT;

constexpr uint32_t wrSignature = 0xAD1FB3F5 ^ sizeof(DevStausStructType);  ///< Сигнатура записи

#endif /* INC_STATUSTYPE_H_ */
