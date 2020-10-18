#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "stm32f10x.h"

#define USART1_TX_PORT          GPIOA
#define USART1_TX_PIN           GPIO_Pin_9

#define USART1_RX_PORT          GPIOA
#define USART1_RX_PIN           GPIO_Pin_10

#define USART2_TX_PORT          GPIOA
#define USART2_TX_PIN           GPIO_Pin_2

#define USART2_RX_PORT          GPIOA
#define USART2_RX_PIN           GPIO_Pin_3

#define USART1_IRQPPriority     5
#define USART1_IRQSPriority     0

#define USART1_BAUDRATE         115200
#define USART1_BUFFER_SIZE      4096

#define USART2_BAUDRATE         115200


void bsp_InitUart1(void);
void bsp_InitUart2(void);
void UART1_DMA_Handle(void);

#endif

