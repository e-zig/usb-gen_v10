/*
 * pllsrv.cpp
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#include "lmx2594Lite.hpp"
#include "pllsrv.h"
#include "main.h"

// ожидание LD, мс, меньше 10 не надо - для первого включения
constexpr uint32_t timeoutLD = 10;

LMX2594 PLL;

void write_log_string(const char *str) {
  UNUSED(str);
}

extern SPI_HandleTypeDef hspi1;

HAL_StatusTypeDef SPI_Write(uint8_t *pData, uint16_t Size) {
  HAL_StatusTypeDef res { };
  HAL_GPIO_WritePin(PLL_CSn_GPIO_Port, PLL_CSn_Pin, GPIO_PIN_RESET);
  res = HAL_SPI_Transmit(&hspi1, pData, Size, 2);
  HAL_GPIO_WritePin(PLL_CSn_GPIO_Port, PLL_CSn_Pin, GPIO_PIN_SET);
  return res;
}

HAL_StatusTypeDef SPI_WriteRead(uint8_t *pTxData, uint8_t *pRxData,
                                uint16_t Size) {
  HAL_StatusTypeDef res { };
  HAL_GPIO_WritePin(PLL_CSn_GPIO_Port, PLL_CSn_Pin, GPIO_PIN_RESET);
  res = HAL_SPI_TransmitReceive(&hspi1, pTxData, pRxData, Size, 2);
  HAL_GPIO_WritePin(PLL_CSn_GPIO_Port, PLL_CSn_Pin, GPIO_PIN_SET);
  return res;
}

void SetGen(bool on) {
  if (on)
    HAL_GPIO_WritePin(GEN_EN_GPIO_Port, GEN_EN_Pin, GPIO_PIN_RESET);
  else
    HAL_GPIO_WritePin(GEN_EN_GPIO_Port, GEN_EN_Pin, GPIO_PIN_SET);
}

void pllReset() {
  PLL.first_init = true;
}

void pllRegsReset() {
  PLL.InitRegs();
}

bool readPllReg(uint8_t Num, uint16_t *Reg) {
  return PLL.ReadOneReg(Num, Reg) == HAL_OK;
}

bool writePllReg(uint8_t Num, uint16_t Reg) {
  return PLL.WriteOneReg(Num, Reg) == HAL_OK;
}

bool setPllPwr(uint8_t pwr) {
  return PLL.Set_PWR(pwr);
}

bool GetLD() {
  return GPIO_PIN_SET == HAL_GPIO_ReadPin(PLL_LD_GPIO_Port, PLL_LD_Pin);
}

bool SetF(DevStausStructType &devStatus) {
  uint32_t f_ref;

  if (devStatus.EnGen)
    f_ref = devStatus.Fintref;
  else
    f_ref = devStatus.Fextref;

  uint64_t f = devStatus.Fgen;

  bool res = PLL.Set_F(f, f_ref);

  if (!res)
    return false;

  PLL.WriteRegs();

  uint32_t st = HAL_GetTick();
  bool ld = GetLD();

  while (!ld && ((HAL_GetTick() - st) < timeoutLD))
    ld = GetLD();

  if (!ld && !PLL.first_init) {
    PLL.first_init = true;
    PLL.WriteRegs();
    st = HAL_GetTick();
    ld = GetLD();
    while (!ld && ((HAL_GetTick() - st) < timeoutLD))
      ld = GetLD();
  }

  PLL.SetCPG(LMX2594::CPG_type::ma_auto);
  PLL.stmOff();

  if (ld)
    PLL.first_init = false;

  return ld;
}
