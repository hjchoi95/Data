#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f0xx_hal.h"

extern TIM_HandleTypeDef htim3;

#define BTN_GPIO_Port       GPIOA
#define LC2041_GPIO_Port    GPIOB
#define LED_GPIO_Port       GPIOC
#define BTN_MAT_R_Port      GPIOB
#define BTN_MAT_C_Port      GPIOC

#define LED_Pin             GPIO_PIN_8

#define BTN_Pin_1           GPIO_PIN_0

#define LC2041_RS           GPIO_PIN_3
#define LC2041_E            GPIO_PIN_4
#define LC2041_BD4          GPIO_PIN_5
#define LC2041_BD5          GPIO_PIN_6
#define LC2041_BD6          GPIO_PIN_7
#define LC2041_BD7          GPIO_PIN_8

#define MAT_R1              GPIO_PIN_2
#define MAT_R2              GPIO_PIN_10
#define MAT_R3              GPIO_PIN_11
#define MAT_R4              GPIO_PIN_12

#define MAT_C1              GPIO_PIN_4
#define MAT_C2              GPIO_PIN_5
#define MAT_C3              GPIO_PIN_13
#define MAT_C4              GPIO_PIN_14

void MX_GPIO_Init(void);
void SystemClock_Config(void);
void MX_USART2_UART_Init(void);
void MX_TIM_Init(void);

void Error_Handler(void);

#endif  //__GPIO_H
