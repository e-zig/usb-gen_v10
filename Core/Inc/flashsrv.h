/*
 * flashsrv.h
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#ifndef INC_FLASHSRV_H_
#define INC_FLASHSRV_H_

#include "stm32f0xx_hal.h"

bool readStatus(uint32_t *arr, uint32_t size);
bool readFreq(uint32_t *arr, uint32_t size);
bool writeStatus(uint32_t *arr, uint32_t size);
bool writeFreq(uint32_t *arr, uint32_t size);
#endif /* INC_FLASHSRV_H_ */
