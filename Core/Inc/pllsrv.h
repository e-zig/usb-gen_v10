/*
 * pllsrv.h
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#ifndef INC_PLLSRV_H_
#define INC_PLLSRV_H_

#include "statustype.h"
#include "stm32f0xx_hal.h"

bool GetLD();
void SetGen(bool on);
bool setPllPwr(uint8_t pwr);
bool SetF(DevStausStructType &devStatus);
bool readPllReg(uint8_t Num, uint16_t *Reg);
bool writePllReg(uint8_t Num, uint16_t Reg);
void pllReset();
void pllRegsReset();
HAL_StatusTypeDef SPI_Write(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef SPI_WriteRead(uint8_t *pTxData, uint8_t *pRxData,
                                uint16_t Size);

#endif /* INC_PLLSRV_H_ */
