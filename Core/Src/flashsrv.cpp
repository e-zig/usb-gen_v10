/*
 * flashsrv.cpp
 *
 *  Created on: 7 окт. 2021 г.
 *      Author: ziged
 */

#include <flashsrv.h>
#include "statustype.h"

#define STATUS_FLASH_ADDR 0x0801F800
#define FREQ_FLASH_ADDR 0x0801F000

// поправленная функция стирания памяти из stm32f0xx_hal_flash_ex.h
void FLASH_PageErase(uint32_t PageAddress) {
  /* Clean the error context */
  extern FLASH_ProcessTypeDef pFlash;
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR);

  /* Proceed to erase the page */
  SET_BIT(FLASH->CR, FLASH_CR_PER);
  WRITE_REG(FLASH->AR, PageAddress);
  SET_BIT(FLASH->CR, FLASH_CR_STRT);

  FLASH_WaitForLastOperation((uint32_t) HAL_FLASH_TIMEOUT_VALUE);
  CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
}

uint32_t FLASH_Read(uint32_t address) {
  return (*(__IO uint32_t*) address);
}

void readWords(uint32_t address, uint32_t *arr, uint32_t size) {
  for (size_t i = 0; i < size; ++i)
    arr[i] = FLASH_Read(address + i * (1 << FLASH_TYPEPROGRAM_WORD));
}

bool writeWords(uint32_t address, uint32_t *arr, uint32_t size) {
  HAL_FLASH_Unlock();
  FLASH_PageErase(address);
  for (size_t i = 0; i < size; ++i) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                          address + i * (1 << FLASH_TYPEPROGRAM_WORD), arr[i])
        != HAL_OK)
      return false;
  }
  HAL_FLASH_Lock();
  return true;
}

bool readStatus(uint32_t *arr, uint32_t size) {
  if ((arr == nullptr) || (size < 1))
    return false;
  if (wrSignature == FLASH_Read(STATUS_FLASH_ADDR))
    readWords(STATUS_FLASH_ADDR, arr, size);
  return wrSignature == arr[0];
}

bool readFreq(uint32_t *arr, uint32_t size) {
  if ((arr == nullptr) || (size < 1))
    return false;
  if (wrSignature == FLASH_Read(FREQ_FLASH_ADDR))
    readWords(FREQ_FLASH_ADDR, arr, size);
  return wrSignature == arr[0];
}

bool writeStatus(uint32_t *arr, uint32_t size) {
  if ((arr == nullptr) || (size < 1))
    return false;
  return writeWords(STATUS_FLASH_ADDR, arr, size);
}

bool writeFreq(uint32_t *arr, uint32_t size) {
  if ((arr == nullptr) || (size < 1))
    return false;
  return writeWords(FREQ_FLASH_ADDR, arr, size);
}
