/*
 * adcsrv.h
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#ifndef INC_ADCSRV_H_
#define INC_ADCSRV_H_

#include "stm32f0xx_hal.h"

struct ADCdataType {
  uint16_t V33;
  uint16_t Vusb;
  uint16_t Vref;
  int16_t T;
};

void GetADC(ADC_HandleTypeDef *hadc, ADCdataType &ADCdata);

#endif /* INC_ADCSRV_H_ */
