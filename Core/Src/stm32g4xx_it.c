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
	//#define DEBUG
	#define RELEASE

	static uint16_t bit_static;
	volatile static float SIN;
	static uint16_t open_loop_cnt = 0;
	static uint16_t duty_B1 = 0;
	static uint16_t duty_C1 = 0;
	static uint16_t duty_A1 = 0;
	
	/* DEBUG */
	/*70ns delay*/
	//GPIOC->BSRR = GPIO_PIN_1;	//GPIO_PIN_SET
	/* DEBUG */
	
	
/** MASTER_PERIOD 5440
	* TIM_PERIOD		2720
	* 
	*/
/** TimerA	A1:BOT1 A2:TOP1
	* CMP1xR   SET
	* CMP3xR RESET	
	* TA1+  TA2-
	* @NOTE CMPxxR  Min:	64		
	*								MAX:	TIM_PERIOD-64
	*/
	#ifdef DEBUG
		duty_A1 = SL_TABLE[open_loop_cnt];
//		if(duty_A1 == 1)				/* 100% */
//		{
//			hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 0;							
//			hhrtim1.Instance->sTimerxRegs[0].CMP3xR = TIM_PERIOD+1;   
//		}
//		else 									/* 0% */
//		{
			hhrtim1.Instance->sTimerxRegs[0].CMP1xR = TIM_PERIOD+1;		
			hhrtim1.Instance->sTimerxRegs[0].CMP3xR = 0;	
//		}
		
		/* 0% */
//		hhrtim1.Instance->sTimerxRegs[0].CMP1xR = TIM_PERIOD+1;		
//		hhrtim1.Instance->sTimerxRegs[0].CMP3xR = 0;							
		/* 5-95% */
	//	hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 0;								
	//	hhrtim1.Instance->sTimerxRegs[0].CMP3xR = TIM_PERIOD-64;  		//Min 64
		/* 100% */
//		hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 0;							
//		hhrtim1.Instance->sTimerxRegs[0].CMP3xR = TIM_PERIOD+1;   
	#else
		duty_A1 = SL_TABLE[open_loop_cnt];
		if(duty_A1 == 1)				/* 100% */
		{
			hhrtim1.Instance->sTimerxRegs[0].CMP1xR = 0;							
			hhrtim1.Instance->sTimerxRegs[0].CMP3xR = TIM_PERIOD+1;   
		}
		else 									/* 0% */
		{
			hhrtim1.Instance->sTimerxRegs[0].CMP1xR = TIM_PERIOD+1;		
			hhrtim1.Instance->sTimerxRegs[0].CMP3xR = 0;	
		}
	#endif
/* TimerA end */
	
	
	
/** TimerB	TOP 2
	* CMP1xR   SET
	* CMP3xR RESET
	* @NOTE CMPxxR  Min:	64		
	*								MAX:	TIM_PERIOD-64
	*/	
	#ifdef DEBUG
//		duty_B1 = FT_TABLE[open_loop_cnt];
//		if(duty_B1 > 64 && duty_B1 < TIM_PERIOD-64)
//		{
//			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;				//CMP1xR   SET
//			hhrtim1.Instance->sTimerxRegs[1].CMP3xR = duty_B1;  //CMP3xR RESET
//		}
//		else if(duty_B1 < 64)	/* 0% */
//		{
//			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = TIM_PERIOD+1;		
//			hhrtim1.Instance->sTimerxRegs[1].CMP3xR = 0;	
//		}
//		else									/* 100% */
//		{
			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;							
			hhrtim1.Instance->sTimerxRegs[1].CMP3xR = TIM_PERIOD+1;   
//		}
		
	/* 0% */
//	hhrtim1.Instance->sTimerxRegs[1].CMP1xR = TIM_PERIOD+1;		
//	hhrtim1.Instance->sTimerxRegs[1].CMP3xR = 0;	
	/* 5-95%*/
//	hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;								//CMP1xR   SET
//	hhrtim1.Instance->sTimerxRegs[1].CMP3xR = TIM_PERIOD*0.75;  //CMP3xR RESET
	/* 100% */
	hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;							
	hhrtim1.Instance->sTimerxRegs[1].CMP3xR = TIM_PERIOD+1;   
	#else
		duty_B1 = FB_TABLE[open_loop_cnt];
		if(duty_B1 > 64 && duty_B1 < TIM_PERIOD-64)
		{
			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;				//CMP1xR   SET
			hhrtim1.Instance->sTimerxRegs[1].CMP3xR = duty_B1;  //CMP3xR RESET
		}
		else if(duty_B1 < 64)	/* 0% */
		{
			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = TIM_PERIOD+1;		
			hhrtim1.Instance->sTimerxRegs[1].CMP3xR = 0;	
		}
		else									/* 100% */
		{
			hhrtim1.Instance->sTimerxRegs[1].CMP1xR = 0;							
			hhrtim1.Instance->sTimerxRegs[1].CMP3xR = TIM_PERIOD+1;   
		}
#endif
/* TimerB end */

/** TimerC	BOT2
	* CMP1xR   SET
	* CMP3xR RESET
	* @NOTE CMPxxR  Min:	64		
	*								MAX:	TIM_PERIOD-64
	*/	
	#ifdef DEBUG
	/* 0% */
	hhrtim1.Instance->sTimerxRegs[2].CMP1xR = TIM_PERIOD+1;		
	hhrtim1.Instance->sTimerxRegs[2].CMP3xR = 0;	
	/* 5-95%*/
//	hhrtim1.Instance->sTimerxRegs[2].CMP1xR = TIM_PERIOD*0.75;								//CMP1xR   SET
//	hhrtim1.Instance->sTimerxRegs[2].CMP3xR = TIM_PERIOD*0.75;  //CMP3xR RESET
	/* 100% */
//	hhrtim1.Instance->sTimerxRegs[2].CMP1xR = 0;							
//	hhrtim1.Instance->sTimerxRegs[2].CMP3xR = TIM_PERIOD+1;
	#else
		duty_C1 = FT_TABLE[open_loop_cnt];
		if(duty_C1 > 64 && duty_C1 < TIM_PERIOD-64)
		{
			hhrtim1.Instance->sTimerxRegs[2].CMP1xR = 0;				//CMP1xR   SET
			hhrtim1.Instance->sTimerxRegs[2].CMP3xR = duty_C1;  //CMP3xR RESET
		}
		else if(duty_C1 < 64)	/* 0% */
		{
			hhrtim1.Instance->sTimerxRegs[2].CMP1xR = TIM_PERIOD+1;		
			hhrtim1.Instance->sTimerxRegs[2].CMP3xR = 0;	
		}
		else									/* 100% */
		{
			hhrtim1.Instance->sTimerxRegs[2].CMP1xR = 0;							
			hhrtim1.Instance->sTimerxRegs[2].CMP3xR = TIM_PERIOD+1;   
		}
	#endif	
/* TimerC end */



	//sTimerxRegs[0]---->TimerA
	//sTimerxRegs[1]---->TimerB
	//sTimerxRegs[2]---->TimerC
	//sTimerxRegs[3]---->TimerD
	//sTimerxRegs[4]---->TimerE
	
	//	if(i>=999)i = 0;
	//SIN = sinf(0.01f);
	
  //__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, TIM_PERIOD);

  /* USER CODE END HRTIM1_Master_IRQn 0 */
  /* USER CODE BEGIN HRTIM1_Master_IRQn 1 */
	
	/*70ns delay*/
	__HAL_HRTIM_MASTER_CLEAR_IT(&hhrtim1, HRTIM_MASTER_IT_MUPD); //Clear IT
	if(open_loop_cnt < 999) open_loop_cnt++;
	else open_loop_cnt = 0;
	/* DEBUG */
	//GPIOC->BRR = GPIO_PIN_1;	//GPIO_PIN_RESET
	/* DEBUG */
	
	
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
