/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint16_t adc1_val_buf[];
/* VOFA */
#define CH_COUNT 1
struct FOFA_Frame {
    float fdata[CH_COUNT];		//float 32  4byte 
    uint8_t tail[4];
};
struct FOFA_Frame UART_FRAME = {		
	0,0x00,0x00,0x80,0x7f
};	
uint8_t UART_FLAG = 0;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
//	static uint16_t i;
//	if(i)GPIOC->BSRR = GPIO_PIN_1;	//GPIO_PIN_SET
//  else GPIOC->BRR = GPIO_PIN_1;	//GPIO_PIN_RESET
//	i = 1-i;
	//GPIOC->BSRR = GPIO_PIN_1;	//GPIO_PIN_SET
	/* x ns delay*/

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

	/* Clear the transfer complete flag */
	hdma_adc1.DmaBaseAddress->IFCR = ((uint32_t)DMA_ISR_TCIF1 << (hdma_adc1.ChannelIndex & 0x1FU));
	/* Process Unlocked */
	__HAL_UNLOCK(&hdma_adc1);
	if (hdma_adc1.XferCpltCallback != NULL)
	{
		/* Transfer complete callback */
		hdma_adc1.XferCpltCallback(&hdma_adc1);
	}
	/*284ns delay*/
	//GPIOC->BRR = GPIO_PIN_2;	//GPIO_PIN_RESET
	//HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc1_val_buf, (3*1));
	//GPIOC->BRR = GPIO_PIN_1;	//GPIO_PIN_RESET
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */
	/* parameter */
	//CNT
	static uint8_t i = 0;
	
	//OPEN LOOP
	static uint16_t OpenLoop_SIN_CNT = 0;
	
//	//ADC
	static float V_Load = 0;
	static float I_Load = 0;
	static float I_Indu = 0;
	
	//UART

	/* parameter end */
	
	if((TIM1->CR1 & 0x10) !=0)
	{
		
		/* ADC */
		V_Load = ((adc1_val_buf[0]-1986)*0.0008056640625f)*50.0f;
		I_Load = (adc1_val_buf[1]*0.0008056640625f-1.588f)*2.0f;
		I_Indu = (adc1_val_buf[2]*0.0008056640625f-1.588f)*10.0f;
//		V_Load = (adc1_val_buf[0]-1986);
//		I_Load = (adc1_val_buf[1]);
//		I_Indu = (adc1_val_buf[2]);

//		if(i)
//			GPIOC->BSRR = GPIO_PIN_1;	//  SET
//		else 
//			GPIOC->BRR = GPIO_PIN_1;		//RESET
//		i = 1-i;
		
//		TIM1->CCR1 = 100;	//A1 A2		PA8	PA11
//		TIM1->CCR2 = 100;	//B1      PA9
//		TIM1->CCR3 = 100;	//C1      PA10
		
		
		/* OPEN LOOP*/
		
		TIM1->CCR1 = SL_TABLE[OpenLoop_SIN_CNT];
		TIM1->CCR2 = FB_TABLE[OpenLoop_SIN_CNT];	
		TIM1->CCR3 = FT_TABLE[OpenLoop_SIN_CNT];	
			/* DEBUG */
//			TIM1->CCR1 = 85;
//			TIM1->CCR2 = 170;	
//			TIM1->CCR3 = 0;	
			/* DEBUG */
		
		if(OpenLoop_SIN_CNT < 499)
			OpenLoop_SIN_CNT++;
		else
			OpenLoop_SIN_CNT = 0;
		/* OPEN LOOP END*/
		
		/* UART */
		UART_FRAME.fdata[0] = V_Load;//V_Load//I_Load //I_Indu
//	UART_FRAME.fdata[1] = 1;
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&UART_FRAME, (4*CH_COUNT+4));
		/* UART END */
		
		/**/
  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */
		
		/* TIM Update event */
		if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE) != RESET)
		{
			if (__HAL_TIM_GET_IT_SOURCE(&htim1, TIM_IT_UPDATE) != RESET)
			{
				__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
				HAL_TIM_PeriodElapsedCallback(&htim1);
			}
		}
	}
  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
