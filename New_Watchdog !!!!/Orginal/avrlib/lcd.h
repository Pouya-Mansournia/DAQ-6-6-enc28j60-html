#ifndef LCD_H
#define LCD_H

#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

#define		LCD_PORT	PORTC
#define		LCD_DDR		DDRC
#define		LCD_RS		PC5
#define		LCD_E		PC4
#define		LCD_DB4 	PC3
#define		LCD_DB5 	PC2
#define		LCD_DB6 	PC1
#define		LCD_DB7 	PC0

#define		LCD_CLEAR			0x01
#define		LCD_HOME			0x02
#define		LCD_CURSOR_OFF		0x0C
#define		LCD_CURSOR_ON		0x0E
#define		LCD_CURSOR_BNLINK	0x0F


#define	LCD_ROW		2
#define	LCD_COLUMN	16

void lcdReset(void);
void lcdInit(void);

void lcd_putchar(unsigned char c);

void lcd_home(void);
void lcd_clear(void);
void lcd_gotoXY(char x, char y);

void lcd_command(unsigned char command);

void lcdWriteByte(unsigned char byte);

#endif
