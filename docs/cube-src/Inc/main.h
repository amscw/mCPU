/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#include "stm32f7xx_ll_i2c.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_dma.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_tim.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define mCPU__5V_EN_Pin LL_GPIO_PIN_2
#define mCPU__5V_EN_GPIO_Port GPIOE
#define mCPU_USBHS_ENA_HS_Pin LL_GPIO_PIN_5
#define mCPU_USBHS_ENA_HS_GPIO_Port GPIOE
#define mCPU_USBHS_CD_Pin LL_GPIO_PIN_6
#define mCPU_USBHS_CD_GPIO_Port GPIOE
#define mCPU_TM_CONNECT_Pin LL_GPIO_PIN_8
#define mCPU_TM_CONNECT_GPIO_Port GPIOI
#define mCPU__5V_TM__EN_Pin LL_GPIO_PIN_9
#define mCPU__5V_TM__EN_GPIO_Port GPIOI
#define mCPU__5V_TM__PG_Pin LL_GPIO_PIN_10
#define mCPU__5V_TM__PG_GPIO_Port GPIOI
#define CPUh_INT_Pin LL_GPIO_PIN_6
#define CPUh_INT_GPIO_Port GPIOF
#define CPUg_INT_Pin LL_GPIO_PIN_7
#define CPUg_INT_GPIO_Port GPIOF
#define CPUf_INT_Pin LL_GPIO_PIN_8
#define CPUf_INT_GPIO_Port GPIOF
#define CPUe_INT_Pin LL_GPIO_PIN_9
#define CPUe_INT_GPIO_Port GPIOF
#define CPUd_INT_Pin LL_GPIO_PIN_0
#define CPUd_INT_GPIO_Port GPIOC
#define CPUc_INT_Pin LL_GPIO_PIN_1
#define CPUc_INT_GPIO_Port GPIOC
#define CPUb_INT_Pin LL_GPIO_PIN_2
#define CPUb_INT_GPIO_Port GPIOC
#define CPUa_INT_Pin LL_GPIO_PIN_3
#define CPUa_INT_GPIO_Port GPIOC
#define mCPU_EEPh_PROG_Pin LL_GPIO_PIN_0
#define mCPU_EEPh_PROG_GPIO_Port GPIOA
#define mCPU_EEPg_PROG_Pin LL_GPIO_PIN_1
#define mCPU_EEPg_PROG_GPIO_Port GPIOA
#define mCPU_EEPf_PROG_Pin LL_GPIO_PIN_2
#define mCPU_EEPf_PROG_GPIO_Port GPIOA
#define mCPU_EQ_SD_Pin LL_GPIO_PIN_2
#define mCPU_EQ_SD_GPIO_Port GPIOH
#define mCPU_SMBus__CS_Pin LL_GPIO_PIN_3
#define mCPU_SMBus__CS_GPIO_Port GPIOH
#define mCPU_EEPe_PROG_Pin LL_GPIO_PIN_3
#define mCPU_EEPe_PROG_GPIO_Port GPIOA
#define mCPU_EEPd_PROG_Pin LL_GPIO_PIN_4
#define mCPU_EEPd_PROG_GPIO_Port GPIOA
#define mCPU_EEPc_PROG_Pin LL_GPIO_PIN_5
#define mCPU_EEPc_PROG_GPIO_Port GPIOA
#define mCPU_EEPb_PROG_Pin LL_GPIO_PIN_6
#define mCPU_EEPb_PROG_GPIO_Port GPIOA
#define mCPU_EEPa_PROG_Pin LL_GPIO_PIN_7
#define mCPU_EEPa_PROG_GPIO_Port GPIOA
#define mCPU_USBLS_ENA_HS_Pin LL_GPIO_PIN_0
#define mCPU_USBLS_ENA_HS_GPIO_Port GPIOB
#define mCPU_USBLS_CD_Pin LL_GPIO_PIN_2
#define mCPU_USBLS_CD_GPIO_Port GPIOB
#define mCPU_LCD_OFF_Pin LL_GPIO_PIN_15
#define mCPU_LCD_OFF_GPIO_Port GPIOI
#define FPGA_INT_Pin LL_GPIO_PIN_11
#define FPGA_INT_GPIO_Port GPIOF
#define mCPU_DVI_PWR_DET_Pin LL_GPIO_PIN_10
#define mCPU_DVI_PWR_DET_GPIO_Port GPIOB
#define mCPU_DVI_HPD_EN_Pin LL_GPIO_PIN_11
#define mCPU_DVI_HPD_EN_GPIO_Port GPIOB
#define mCPU_CPUx_is_STBY_Pin LL_GPIO_PIN_8
#define mCPU_CPUx_is_STBY_GPIO_Port GPIOH
#define mCPU_xCPU_WAKEUP_Pin LL_GPIO_PIN_9
#define mCPU_xCPU_WAKEUP_GPIO_Port GPIOH
#define mCPU_xCPU_SEL_A2_Pin LL_GPIO_PIN_10
#define mCPU_xCPU_SEL_A2_GPIO_Port GPIOH
#define mCPU_xCPU_SEL_A1_Pin LL_GPIO_PIN_11
#define mCPU_xCPU_SEL_A1_GPIO_Port GPIOH
#define mCPU_xCPU_SEL_A0_Pin LL_GPIO_PIN_12
#define mCPU_xCPU_SEL_A0_GPIO_Port GPIOH
#define mCPU_DVI_PWR_EN_Pin LL_GPIO_PIN_12
#define mCPU_DVI_PWR_EN_GPIO_Port GPIOB
#define mCPU_CH_SEL_A2_Pin LL_GPIO_PIN_13
#define mCPU_CH_SEL_A2_GPIO_Port GPIOB
#define mCPU_CH_SEL_A1_Pin LL_GPIO_PIN_14
#define mCPU_CH_SEL_A1_GPIO_Port GPIOB
#define mCPU_CH_SEL_A0_Pin LL_GPIO_PIN_15
#define mCPU_CH_SEL_A0_GPIO_Port GPIOB
#define mCPU_USBHS_ON_Pin LL_GPIO_PIN_7
#define mCPU_USBHS_ON_GPIO_Port GPIOG
#define mCPU_USBHS_CONNECT_Pin LL_GPIO_PIN_8
#define mCPU_USBHS_CONNECT_GPIO_Port GPIOG
#define mCPU_LED_PWRON_Pin LL_GPIO_PIN_9
#define mCPU_LED_PWRON_GPIO_Port GPIOC
#define mCPU_DVIsw__UPDATE_Pin LL_GPIO_PIN_12
#define mCPU_DVIsw__UPDATE_GPIO_Port GPIOJ
#define mCPU_DVIsw__RESET_Pin LL_GPIO_PIN_13
#define mCPU_DVIsw__RESET_GPIO_Port GPIOJ
#define mCPU_NSS1_A0_Pin LL_GPIO_PIN_12
#define mCPU_NSS1_A0_GPIO_Port GPIOG
#define mCPU_NSS1_A1_Pin LL_GPIO_PIN_13
#define mCPU_NSS1_A1_GPIO_Port GPIOG
#define mCPU_NSS1_A2_Pin LL_GPIO_PIN_14
#define mCPU_NSS1_A2_GPIO_Port GPIOG
#define mCPU_PG_ADP_Pin LL_GPIO_PIN_7
#define mCPU_PG_ADP_GPIO_Port GPIOK
#define mCPU_NSS1_EN_Pin LL_GPIO_PIN_15
#define mCPU_NSS1_EN_GPIO_Port GPIOG
#define mCPU_I2CSW__RESET_Pin LL_GPIO_PIN_5
#define mCPU_I2CSW__RESET_GPIO_Port GPIOB
#define mCPU_INBUF__EN_Pin LL_GPIO_PIN_8
#define mCPU_INBUF__EN_GPIO_Port GPIOB
#define mCPU_OUTBUF__EN_Pin LL_GPIO_PIN_9
#define mCPU_OUTBUF__EN_GPIO_Port GPIOB
#define mCPU_NSS2_A0_Pin LL_GPIO_PIN_4
#define mCPU_NSS2_A0_GPIO_Port GPIOI
#define mCPU_NSS2_A1_Pin LL_GPIO_PIN_5
#define mCPU_NSS2_A1_GPIO_Port GPIOI
#define mCPU_NSS2_A2_Pin LL_GPIO_PIN_6
#define mCPU_NSS2_A2_GPIO_Port GPIOI
#define mCPU_NSS2_EN_Pin LL_GPIO_PIN_7
#define mCPU_NSS2_EN_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
