
#include "gpio.h"
#include "stdio.h"
#include "string.h"

void LCD_PulseEnable(void);

void LCD_Send4bit(uint8_t data)
{
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_BD7, (data >> 3) & 0x01);
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_BD6, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_BD5, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_BD4, (data >> 0) & 0x01);

    LCD_PulseEnable();
}

void LCD_Writebit(char data)
{
    LCD_Send4bit((data >> 4) & 0x0F);
    LCD_Send4bit(data & 0x0F);
}

void LCD_Write(char* data)
{
    unsigned char i;
    char len = strlen(data);

    for (i = 0; i < len; i++)
    {
        LCD_Send4bit((data[i] >> 4) & 0x0F);
        LCD_Send4bit(data[i] & 0x0F);
    }
}

void Init_LC2041(void)
{
    HAL_Delay(30);    // Power on
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_RS, GPIO_PIN_RESET);

    // 8 bit mode
    LCD_Send4bit(0x03);     // 0 1 1 0
    LCD_Send4bit(0x03);     // 0 1 1 0
    LCD_Send4bit(0x03);     // 0 1 1 0

    // 4 bit mode
    LCD_Send4bit(0x02);     // 0 0 1 0
    LCD_Send4bit(0x02);     // 0 0 1 0
    LCD_Send4bit(0x08);     // 1 0 0 0

    HAL_Delay(1);

    // Display ON/OFF control 
    // D=1(display ON), C=0(cusor OFF), B=0(blinking OFF)
    LCD_Send4bit(0x00);     // 0 0 0 0
    LCD_Send4bit(0x0C);     // 1 1 0 0
    HAL_Delay(1);

    // Display Clear
    LCD_Send4bit(0x00);     // 0 0 0 0
    LCD_Send4bit(0x01);     // 0 0 0 1
    HAL_Delay(2);

    // Entry Mode Set (I/D, SH)
    // I/D=1(address increase), SH=0(none Shift)
    LCD_Send4bit(0x00);     // 0 0 0 0
    LCD_Send4bit(0x06);     // 0 1 1 0

    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_RS, GPIO_PIN_SET);
}

void LCD_Clear (void)
{
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_RS, GPIO_PIN_RESET);
                
    LCD_Send4bit(0x00);
    LCD_Send4bit(0x01);
    HAL_Delay(2);

    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_RS, GPIO_PIN_SET);
}

void LCD_Move_Cusor (void)
{
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_RS, GPIO_PIN_RESET);
                
    LCD_Send4bit(0x0C);
    LCD_Send4bit(0x00);
    HAL_Delay(2);

    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_RS, GPIO_PIN_SET);
}

void LCD_PulseEnable(void)
{
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_E, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LC2041_GPIO_Port, LC2041_E, GPIO_PIN_RESET);
    HAL_Delay(1);
}
