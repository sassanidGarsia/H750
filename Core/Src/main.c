/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "dma.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUF_LEN 256
#define TX_BUF_SIZE 2053  // 2053可根据需要扩大
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t adcv;
float adcv_f;
__attribute__((section(".RAM_D1"))) uint16_t adc_buf[ADC_BUF_LEN];

volatile  uint8_t Flag_dma1stream0IRQ = 0;
volatile  uint8_t Flag_dma1stream1IRQ = 0;
uint8_t Flag_usart1TxIRQ = 0;

uint16_t i_buff = 0;

__attribute__((section(".RAM_D1"))) uint8_t uart_tx_buf[TX_BUF_SIZE];
uint32_t uart_tx_len = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();
	SCB_EnableICache();  // 只对 Cacheable 区域有效
	SCB_EnableDCache();  // 启用数据 Cache


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	PeriphCommonClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  MX_DMA_Init();
  MX_TIM15_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
	MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
	// TIM15��TIM2��TIM3��������TIM1�������ȴ�TIM1���ź�
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);  // PA3
	HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2);    // PA1
	HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_2);    // PA7
		// TIM8����ADC����
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);    // PC6

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   // PE9
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   // PE11
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);   // PE13
	HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1);    // PD12
	HAL_TIM_OC_Start(&htim5, TIM_CHANNEL_1);    // PA0

	if(HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}
	// 启动 ADC + DMA 采样 256 次
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADC_BUF_LEN);
	
//	char str[] = "HelloWorld\r\n";
//  HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);


  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
		if (Flag_dma1stream0IRQ == 1)
		{
			Flag_dma1stream0IRQ = 0;

			uart_tx_len = 0;

			// 加入数据包起始标志位 "3.5\r\n"
      uart_tx_len += snprintf((char*)&uart_tx_buf[uart_tx_len], TX_BUF_SIZE - uart_tx_len, "3.5\r\n");
			// 将所有采样值格式化进 uart_tx_buf
			for (i_buff = 0; i_buff < ADC_BUF_LEN; i_buff++)
			{
					uart_tx_len += snprintf((char*)&uart_tx_buf[uart_tx_len], TX_BUF_SIZE - uart_tx_len,
																	"%.4f\r\n", adc_buf[i_buff] * 3.3f / 65535.0f);
			}

			// 启动 DMA 传输（只有等上次传输完成后再传）
			if (huart1.gState == HAL_UART_STATE_READY )
			{
				Flag_usart1TxIRQ = 0;
				HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart_tx_buf, uart_tx_len);
//				HAL_UART_TxCpltCallback(&huart1);
			}
			
			// 重启 ADC DMA
			HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, ADC_BUF_LEN);
    }


  }

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_IRQHandler(&huart1);
	if (huart->Instance == USART1)
  {
//    HAL_UART_DMAStop(huart);

		Flag_usart1TxIRQ = 1;  // 标志发送完成
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
	__HAL_RCC_SYSCFG_CLK_ENABLE();
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
	__HAL_RCC_VREF_CLK_ENABLE();
	
	HAL_SYSCFG_DisableVREFBUF();
	HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_ENABLE);
	
//	HAL_SYSCFG_EnableVREFBUF();
//	HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE);
	
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
	
	
}

/* USER CODE BEGIN 4 */
/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE END 4 */

 /* MPU Configuration */

//void MPU_Config(void)
//{
//  MPU_Region_InitTypeDef MPU_InitStruct = {0};

//  /* Disables the MPU */
//  HAL_MPU_Disable();

//  /** Initializes and configures the Region and the memory to be protected
//  */
//  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
//  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
//  MPU_InitStruct.BaseAddress = 0x0;
//  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
//  MPU_InitStruct.SubRegionDisable = 0x87;
//  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
//  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
//  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
//  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
//  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
//  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

//  HAL_MPU_ConfigRegion(&MPU_InitStruct);
//  /* Enables the MPU */
//  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

//}

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  // 1️⃣ 关闭 MPU
  HAL_MPU_Disable();

  // 2️⃣ 配置默认禁止区域（可选）
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x00000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  // 3️⃣ 设置 RAM D1 区域（0x24000000 - 0x2407FFFF）为非 Cacheable
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;  // 区域编号可自定义
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;  // 非缓存建议使用 TEX1
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  // 4️⃣ 启用 MPU
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
