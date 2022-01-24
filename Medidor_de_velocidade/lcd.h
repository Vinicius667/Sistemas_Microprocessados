/*
 * lcd.h
 *
 *  Created on: 9 de nov de 2019
 *      Author: Vinicius Almeida
 */

#ifndef LCD_H_
#define LCD_H_


void lcdWriteNibble(uint8_t nibble, uint8_t isChar);
void lcdWriteByte(uint8_t byte, uint8_t isChar);
uint8_t lcdReadNibble();
uint8_t lcdReadbyte();
void lcdInit();
void lcdPrint(char *str);
void Hex8(unsigned char c);
void Hex16(unsigned int x);
void dec8(unsigned char c);
void dec3(float f);
//void dec8(unsigned char c);
//void dec16(unsigned int x);;
void lcd_limpar();
void lcdCursor(uint8_t lin, uint8_t col);
void lcdClr();



#endif /* LCD_H_ */
