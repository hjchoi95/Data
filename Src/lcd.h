
#ifndef __LCD_H
#define __LCD_H

void LCD_Writebit(char data);
void LCD_Write(char* data);
void Init_LC2041(void);
void LCD_Clear (void);
void LCD_Move_Cusor (void);

#endif  //__LCD_H
