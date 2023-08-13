#include "lcd.h"

void lcdInit(void){
	LCD_DDR |= (1<<LCD_E);
	LCD_DDR |= (1<<LCD_RS);
	LCD_DDR |= (1<<LCD_DB4);
	LCD_DDR |= (1<<LCD_DB5);
	LCD_DDR |= (1<<LCD_DB6);
	LCD_DDR |= (1<<LCD_DB7);
	
	lcdReset();
	
	lcd_command(0x28);
	lcd_command(0x0c);
	lcd_command(0x01);
}

void lcdReset(void){
	LCD_PORT &= ~(1<<LCD_E);
	LCD_PORT &= ~(1<<LCD_RS);
	LCD_PORT |= (1<<LCD_DB4);
	LCD_PORT |= (1<<LCD_DB5);
	LCD_PORT &= ~(1<<LCD_DB7);
	LCD_PORT &= ~(1<<LCD_DB6);
	
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	

	_delay_ms(4);
	
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	
	_delay_ms(1);
	
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	
	_delay_ms(1);
	
	LCD_PORT &= ~(1<<LCD_DB4);
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_ms(1);
}


void lcd_putchar(unsigned char c){
	_delay_ms(10);
	LCD_PORT &= ~(1<<LCD_E);
	LCD_PORT |= (1<<LCD_RS);
	lcdWriteByte(c);
}

void lcd_home(void){
	lcd_command(LCD_HOME);
}

void lcd_clear(void){
	lcd_command(LCD_CLEAR);
}

void lcd_gotoXY(char x, char y){
	switch( y ){
		case 0:
			lcd_command( 0x80 + x );
			break;
		
		case 1:
			lcd_command( 0xC0 + x );
			break;
	}
}

void lcd_command(unsigned char command){
	_delay_ms(10);
	LCD_PORT &= ~(1<<LCD_E);
	LCD_PORT &= ~(1<<LCD_RS);
	lcdWriteByte(command);
}

void lcdWriteByte(unsigned char byte){
	// send HIGH nibble ------------------
	LCD_PORT &= ~(1<<LCD_DB4);
	LCD_PORT &= ~(1<<LCD_DB5);
	LCD_PORT &= ~(1<<LCD_DB6);
	LCD_PORT &= ~(1<<LCD_DB7);
	
	if(byte & 1<<4)
		LCD_PORT |= (1<<LCD_DB4);
	
	if(byte & 1<<5)
		LCD_PORT |= (1<<LCD_DB5);
	
	if(byte & 1<<6)
		LCD_PORT |= (1<<LCD_DB6);
	
	if(byte & 1<<7)
		LCD_PORT |= (1<<LCD_DB7);
	
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_ms(1);
	
	// send LOW nibble ------------------
	LCD_PORT &= ~(1<<LCD_DB4);
	LCD_PORT &= ~(1<<LCD_DB5);
	LCD_PORT &= ~(1<<LCD_DB6);
	LCD_PORT &= ~(1<<LCD_DB7);
	
	if(byte & 1<<0)
		LCD_PORT |= (1<<LCD_DB4);
	
	if(byte & 1<<1)
		LCD_PORT |= (1<<LCD_DB5);
	
	if(byte & 1<<2)
		LCD_PORT |= (1<<LCD_DB6);
	
	if(byte & 1<<3)
		LCD_PORT |= (1<<LCD_DB7);
	
	LCD_PORT |= (1<<LCD_E);
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_ms(1);
}

