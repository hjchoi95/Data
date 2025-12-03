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
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "gpio.h"
#include "key.h"
#include "lcd.h"

#define MAX_ROCK_NUM        (4)
#define MAX_BRIGHTNESS      (1000)
#define DEFAULT_BRIGHTNESS  (100)
#define BRIGHTNESS_VALUE    (20)
#define CHECK_DELAY         (1500)

#define DEFAULT_PASSWORD    "1111"

void SetBrightness(uint16_t duty);

int main(void)
{
    bool setting = false;
    bool lock = true;
    int key;
    uint16_t brt = DEFAULT_BRIGHTNESS;
    uint8_t num = 0;
    uint8_t password[] = DEFAULT_PASSWORD;
    uint8_t input_password[MAX_ROCK_NUM+1];
    uint8_t change_password[MAX_ROCK_NUM+1];

    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    
    MX_USART2_UART_Init();

    MX_TIM_Init();

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    Init_LC2041();

    // End of initialization
    printf("LC2041 initialized\r\n");

    LCD_Write("INPUT PASSWORD");
    LCD_Move_Cusor();

    SetBrightness(brt);

    while (1)
    {
        key = ScanKeypad();

        if (key >= 0)
        {
            // Input password
            // While setting password, this is not working
            if (keymap[key] <= '9' && !setting)
                if ('0' <= keymap[key])
                {
                    LCD_Writebit(keymap[key]);
                    input_password[num] = keymap[key];
                    num++;
                }
            
            // Lock the doorlock
            // If doorlock is already locked, this is not working
            if (keymap[key] == '*')
            {
                if(!lock)
                {
                    lock = true;
                    LCD_Clear();
                    LCD_Write("LOCKED");
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    num=0;
                }
            }

            // Unlock
            // If this is already unlocked, not working
            if (keymap[key] == '#')
            {
                if (!lock)
                {
                    LCD_Clear();
                    LCD_Write("ALREADY UNLOCKED");
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    num=0;
                }
                // Correct
                else if (memcmp(input_password, password, sizeof(password)) == 0 && lock)
                {
                    LCD_Clear();
                    LCD_Write("UNLOCK PASSWORD");
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    memset(input_password, 0, sizeof(input_password));
                    num=0;
                    lock = false;
                }
                // Wrong
                else
                {
                    LCD_Clear();
                    LCD_Write("WRONG PASSWORD");
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    memset(input_password, 0, sizeof(input_password));
                    num=0;
                    lock = true;
                }
            }

            if (keymap[key] == 'A')
            {
                if (brt < MAX_BRIGHTNESS)
                {
                    brt += BRIGHTNESS_VALUE;
                    SetBrightness(brt);
                }
                else if (brt == MAX_BRIGHTNESS)
                {
                    LCD_Clear();
                    LCD_Write("BRIGHTNESS MAX");
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    LCD_Write("INPUT PASSWORD");
                }
            }
            if (keymap[key] == 'B')
            {
                if (brt > 0)
                {
                    brt -= BRIGHTNESS_VALUE;
                    SetBrightness(brt);
                }
                else if (brt == 0)
                {
                    LCD_Clear();
                    LCD_Write("BRIGHTNESS MIN");
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    LCD_Write("INPUT PASSWORD");
                }
            }

            // Set doorlock password mode
            if (keymap[key] == 'D' && num == 0 && !setting && !lock)
            {
                LCD_Clear();
                LCD_Write("SET PASSWORD");
                LCD_Move_Cusor();
                setting = true;
                memset(change_password, 0, sizeof(change_password));
                num=0;
            }

            // Set password
            else if (setting)
            {
                LCD_Writebit(keymap[key]);
                change_password[num] = keymap[key];
                num++;

                if (num == MAX_ROCK_NUM)
                {
                    change_password[num] = '\0';
                    printf("\r\n");
                    LCD_Clear();
                    LCD_Write("SETTING COMPLETE");
                    LCD_Move_Cusor();
                    LCD_Write("PASSWORD : ");
                    LCD_Write((char*)change_password);
                    HAL_Delay(CHECK_DELAY);
                    LCD_Clear();
                    memcpy(password, change_password, sizeof(change_password));
                    setting = false;
                    num=0;
                }
            }

            // Over MAX_ROCK_NUM
            if (num > MAX_ROCK_NUM)
            {
                LCD_Clear();
                LCD_Write("OVER MAX NUMBER");
                HAL_Delay(CHECK_DELAY);
                LCD_Clear();
                memset(input_password, 0, sizeof(input_password));
                num=0;
            }
            HAL_Delay(200);

            if (num == 0 && !setting && keymap[key] <= '9' && keymap[key] != '*')
            {
                LCD_Write("INPUT PASSWORD");
                LCD_Move_Cusor();
            }
        }

        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) 
        {
            printf("on\r\n");
            printf("off\r\n");
            // 센서 HIGH
        } 
        else 
        {
            // 센서 LOW
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

void SetBrightness(uint16_t duty)   // 0 ~ MAX_BRIGHTNESS
{
    printf("brt = %d\r\n", duty);
    if (duty > MAX_BRIGHTNESS) duty = MAX_BRIGHTNESS;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
}
