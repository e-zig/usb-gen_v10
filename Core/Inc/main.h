/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);
void usbCallBack(uint8_t *buf, uint32_t len);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN2_Pin GPIO_PIN_13
#define BTN2_GPIO_Port GPIOC
#define BTN2_EXTI_IRQn EXTI4_15_IRQn
#define Mon_3V3_Pin GPIO_PIN_0
#define Mon_3V3_GPIO_Port GPIOA
#define Mon_5Vusb_Pin GPIO_PIN_1
#define Mon_5Vusb_GPIO_Port GPIOA
#define PLL_LD_Pin GPIO_PIN_2
#define PLL_LD_GPIO_Port GPIOA
#define PLL_ON_Pin GPIO_PIN_3
#define PLL_ON_GPIO_Port GPIOA
#define PLL_CSn_Pin GPIO_PIN_4
#define PLL_CSn_GPIO_Port GPIOA
#define PLL_SCK_Pin GPIO_PIN_5
#define PLL_SCK_GPIO_Port GPIOA
#define PLL_MISO_Pin GPIO_PIN_6
#define PLL_MISO_GPIO_Port GPIOA
#define PLL_MOSI_Pin GPIO_PIN_7
#define PLL_MOSI_GPIO_Port GPIOA
#define PLL_RampCLK_Pin GPIO_PIN_0
#define PLL_RampCLK_GPIO_Port GPIOB
#define PLL_RampOUT_Pin GPIO_PIN_1
#define PLL_RampOUT_GPIO_Port GPIOB
#define LED_Yel_Pin GPIO_PIN_2
#define LED_Yel_GPIO_Port GPIOB
#define GEN_EN_Pin GPIO_PIN_11
#define GEN_EN_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_13
#define OLED_SCL_GPIO_Port GPIOB
#define MCO_Pin GPIO_PIN_8
#define MCO_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_5
#define BTN1_GPIO_Port GPIOB
#define BTN1_EXTI_IRQn EXTI4_15_IRQn
#define PLL_SYSREF_Pin GPIO_PIN_8
#define PLL_SYSREF_GPIO_Port GPIOB
#define PLL_SYNC_Pin GPIO_PIN_9
#define PLL_SYNC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
