/*
 * adcsrv.cpp
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#ifndef SRC_ADCSRV_CPP_
#define SRC_ADCSRV_CPP_

#include "adcsrv.h"

#define VREFINT_CAL_ADDR 0x1FFFF7BA  /* datasheet p. 17 */
#define VREFINT_CAL (*(uint16_t*) VREFINT_CAL_ADDR)
#define TEMP30_CAL_ADDR 0x1FFFF7B8
#define TEMP30_CAL (*(uint16_t*) TEMP30_CAL_ADDR)

void GetADC(ADC_HandleTypeDef *hadc, ADCdataType &ADCdata) {

  HAL_ADC_Start(hadc);
  HAL_ADC_PollForConversion(hadc, 10);
  ADCdata.V33 = HAL_ADC_GetValue(hadc);
  HAL_ADC_Start(hadc);
  HAL_ADC_PollForConversion(hadc, 10);
  ADCdata.Vusb = HAL_ADC_GetValue(hadc);
  HAL_ADC_Start(hadc);
  HAL_ADC_PollForConversion(hadc, 10);
  ADCdata.T = HAL_ADC_GetValue(hadc);
  HAL_ADC_Start(hadc);
  HAL_ADC_PollForConversion(hadc, 10);
  ADCdata.Vref = HAL_ADC_GetValue(hadc);

  ADCdata.V33 = (uint16_t) ((6600ul * ADCdata.V33) / 4095);
  ADCdata.Vusb = (uint16_t) ((6600ul * ADCdata.Vusb) / 4095);

  static const uint32_t VDD_CALIB { 3300 };
  static const uint32_t AVG_SLOPE { 5336 };

  ADCdata.T = ((TEMP30_CAL - ADCdata.T * ADCdata.V33 / VDD_CALIB) * 1000)
      / AVG_SLOPE + 30;

}

#endif /* SRC_ADCSRV_CPP_ */
