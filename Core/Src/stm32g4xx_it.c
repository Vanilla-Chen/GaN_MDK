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

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern HRTIM_HandleTypeDef hhrtim1;
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
//	if(i)GPIOC->BSRR = GPIO_PIN_2;	//GPIO_PIN_SET
//  else GPIOC->BRR = GPIO_PIN_2;	//GPIO_PIN_RESET
	/*252ns delay*/
	//GPIOC->BSRR = GPIO_PIN_2;	//GPIO_PIN_SET
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
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles HRTIM master timer global interrupt.
  */
void HRTIM1_Master_IRQHandler(void)
{
  /* USER CODE BEGIN HRTIM1_Master_IRQn 0 */
	volatile static float SIN;
	static uint16_t i;
	/*70ns delay*/
	GPIOC->BSRR = GPIO_PIN_1;	//GPIO_PIN_SET
	
/** TimerA
	* CMP1xR   SET
	* CMP3xR RESET	
	* TA1+  TA2-
	* @NOTE CMPxxR  Min:	64		
	*								MAX:	TIM_PERIOD-64
	*/
	/* 0% */
	hhrtim1.Instance->sTimerxRegs[0].CMP1xR = TIM_PERIOD+1;		
	hhrtim1.Instance->sTimerxRegs[0].CMP3xR = 0;							
	/* 5-95% */
//	hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 0;								
//	hhrtim1.Instance->sTimerxRegs[0].CMP3xR = TIM_PERIOD-64;  							//Min 64
	/* 100% */
//	hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 0;							
//	hhrtim1.Instance->sTimerxRegs[0].CMP3xR = TIM_PERIOD+1;   
/* TimerA */
	
/** TimerB
	* CMP1xR   SET
	* CMP3xR RESET
	*/	
	/* 5-95%*/
//	hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;								//CMP1xR   SET
//	hhrtim1.Instance->sTimerxRegs[1].CMP3xR = TIM_PERIOD*0.75;  //CMP3xR RESET
/*TimerB*/

/** TimerC
	* CMP1xR   SET
	* CMP3xR RESET
	*/	
	/* 5-95%*/
	hhrtim1.Instance->sTimerxRegs[2].CMP1xR = 0;								//CMP1xR   SET
	hhrtim1.Instance->sTimerxRegs[2].CMP3xR = TIM_PERIOD*0.75;  //CMP3xR RESET
/*TimerC*/

	//sTimerxRegs[0]---->TimerA
	//sTimerxRegs[1]---->TimerB
	//sTimerxRegs[2]---->TimerC
	//sTimerxRegs[3]---->TimerD
	//sTimerxRegs[4]---->TimerE
	
	//hhrtim1.Instance->sTimerxRegs[2].CMP1xR = TIM_PERIOD*0.5f;
	//	if(i>=999)i = 0;
	//SIN = sinf(0.01f);
	
  //__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, TIM_PERIOD);

  /* USER CODE END HRTIM1_Master_IRQn 0 */
  /* USER CODE BEGIN HRTIM1_Master_IRQn 1 */
	
	/*70ns delay*/
	__HAL_HRTIM_MASTER_CLEAR_IT(&hhrtim1, HRTIM_MASTER_IT_MUPD); //Clear IT
	GPIOC->BRR = GPIO_PIN_1;	//GPIO_PIN_RESET
	
  /* USER CODE END HRTIM1_Master_IRQn 1 */
}

/* USER CODE BEGIN 1 */
//void DMA1_Channel1_IRQHandler(void)
//{
//  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
////	static uint16_t i;
////	if(i)GPIOC->BSRR = GPIO_PIN_2;	//GPIO_PIN_SET
////  else GPIOC->BRR = GPIO_PIN_2;	//GPIO_PIN_RESET
//	/*252ns delay*/
//	GPIOC->BSRR = GPIO_PIN_2;	//GPIO_PIN_SET
//  /* USER CODE END DMA1_Channel1_IRQn 0 */
//  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
//	
//	/* Clear the transfer complete flag */
//	hdma_adc1.DmaBaseAddress->IFCR = ((uint32_t)DMA_ISR_TCIF1 << (hdma_adc1.ChannelIndex & 0x1FU));
//	/* Process Unlocked */
//	__HAL_UNLOCK(&hdma_adc1);
//	if (hdma_adc1.XferCpltCallback != NULL)
//	{
//		/* Transfer complete callback */
//		hdma_adc1.XferCpltCallback(&hdma_adc1);
//	}
//	/*284ns delay*/
//	GPIOC->BRR = GPIO_PIN_2;	//GPIO_PIN_RESET
//	//HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc1_val_buf, (3*1));
//  /* USER CODE END DMA1_Channel1_IRQn 1 */
//}
/* USER CODE END 1 */
