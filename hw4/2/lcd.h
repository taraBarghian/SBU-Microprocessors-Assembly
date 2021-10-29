#ifndef _LCD_H
#define _LCD_H

//#include "stm32f4xx_hal.h"



void lcd_init(void);
void lcd_clear(void);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_puts(char *str);
void lcd_send_command(unsigned char command);

#endif
