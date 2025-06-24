/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_it.h"
#include "stdio.h"
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
extern volatile uint8_t Flag_dma1stream0IRQ;
extern volatile uint8_t Flag_dma1stream1IRQ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
//  __asm volatile
//  (
//    "TST LR, #4\n"
//    "ITE EQ\n"
//    "MRSEQ R0, MSP\n"
//    "MRSNE R0, PSP\n"
//    "B HardFault_Handler_C\n"
//  );
  /* USER CODE END HardFault_IRQn 0 */
	
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}


/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}
/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */
//  DMA_Check_Flags(&hdma_adc1);
	
	// 多种中断被置位了，这里选择TCIF作为区别标志，执行后续操作
	if(__HAL_DMA_GET_FLAG(&hdma_adc1, DMA_FLAG_TCIF0_4))  
	{
		HAL_ADC_Stop_DMA(&hadc1);
		__HAL_DMA_CLEAR_FLAG(&hdma_adc1, DMA_FLAG_TCIF0_4);
		/* 以下中断函数把所有flag置位为0 */
    HAL_DMA_IRQHandler(&hdma_adc1);

	  Flag_dma1stream0IRQ = 1;
	}
		  Flag_dma1stream0IRQ = 1;
  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
//	DMA_Check_Flags(&hdma_usart1_tx);
	if(__HAL_DMA_GET_FLAG(&hdma_usart1_tx, DMA_FLAG_TCIF1_5))  
	{
		__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx, DMA_FLAG_TCIF1_5);
//	  HAL_DMA_IRQHandler(&hdma_usart1_tx); 
		Flag_dma1stream1IRQ = 1;
//		hdma_usart1_tx.State = HAL_DMA_STATE_READY;
	}
	HAL_DMA_IRQHandler(&hdma_usart1_tx); 
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
/* USER CODE BEGIN 1 */
void DMA_Check_Flags(DMA_HandleTypeDef *hdma)
{
    printf("\r\nDMA1Stream1 Flags Status:\r\n");
    
    // 检查传输完成标志
    printf("TCIF: %d\r\n", 
           (DMA1->LISR & DMA_FLAG_TCIF1_5) ? 1 : 0);
    
    // 检查半传输标志
    printf("HTIF: %d\r\n", 
           (DMA1->LISR & DMA_FLAG_HTIF1_5) ? 1 : 0);
    
    // 检查传输错误标志
    printf("TEIF: %d\r\n", 
           (DMA1->LISR & DMA_FLAG_TEIF1_5) ? 1 : 0);
    
    // 检查直接模式错误标志
    printf("DMEIF: %d\r\n", 
           (DMA1->LISR & DMA_FLAG_DMEIF1_5) ? 1 : 0);
    
    // 检查FIFO错误标志
    printf("FEIF: %d\r\n", 
           (DMA1->LISR & DMA_FLAG_FEIF1_5) ? 1 : 0);
}

/* USER CODE END 1 */
