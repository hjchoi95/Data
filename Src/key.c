
#include "gpio.h"

int ScanKeypad(void)
{
    for (int row = 0; row < 4; row++)
    {
        HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R3, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R4, GPIO_PIN_SET);

        switch (row)
        {
            case 0: HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R1, GPIO_PIN_RESET); break;
            case 1: HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R2, GPIO_PIN_RESET); break;
            case 2: HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R3, GPIO_PIN_RESET); break;
            case 3: HAL_GPIO_WritePin(BTN_MAT_R_Port, MAT_R4, GPIO_PIN_RESET); break;
        }

        HAL_Delay(1);

        if (HAL_GPIO_ReadPin(BTN_MAT_C_Port, MAT_C1) == GPIO_PIN_RESET) return row * 4 + 0;
        if (HAL_GPIO_ReadPin(BTN_MAT_C_Port, MAT_C2) == GPIO_PIN_RESET) return row * 4 + 1;
        if (HAL_GPIO_ReadPin(BTN_MAT_C_Port, MAT_C3) == GPIO_PIN_RESET) return row * 4 + 2;
        if (HAL_GPIO_ReadPin(BTN_MAT_C_Port, MAT_C4) == GPIO_PIN_RESET) return row * 4 + 3;
    }

    return -1;  // No key pressed
}