//#ifndef _USART_H
//#define _USART_H

//#include "stdio.h"
//#include "main.h"

///*******************************************************************************************************/
///* ���� �� ���� ���� 
// * Ĭ�������USART1��.
// * ע��: ͨ���޸���12���궨��,����֧��USART1~UART7����һ������.
// */

//#define USART_TX_GPIO_PORT              GPIOA
//#define USART_TX_GPIO_PIN               GPIO_PIN_9
//#define USART_TX_GPIO_AF                GPIO_AF7_USART1
//#define USART_TX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

//#define USART_RX_GPIO_PORT              GPIOA
//#define USART_RX_GPIO_PIN               GPIO_PIN_10
//#define USART_RX_GPIO_AF                GPIO_AF7_USART1
//#define USART_RX_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* ��������ʱ��ʹ�� */

//#define USART_UX                        USART1
//#define USART_UX_IRQn                   USART1_IRQn
//#define USART_UX_IRQHandler             USART1_IRQHandler
//#define USART_UX_CLK_ENABLE()           do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */

///*******************************************************************************************************/

//#define USART_REC_LEN   200                     /* �����������ֽ��� 200 */
//#define USART_EN_RX     1                       /* ʹ�ܣ�1��/��ֹ��0������1���� */
//#define RXBUFFERSIZE    1                       /* �����С */

//extern UART_HandleTypeDef g_uart1_handle;       /* UART��� */

//extern uint8_t  g_usart_rx_buf[USART_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
//extern uint16_t g_usart_rx_sta;                 /* ����״̬��� */
//extern uint8_t g_rx_buffer[RXBUFFERSIZE];       /* HAL��USART����Buffer */


//void usart_init(uint32_t baudrate);             /* ���ڳ�ʼ������ */

//#endif









/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

void Usart1Printf(const char *format,...);

//void sendData(float Data,uint16_t ByteNum);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

