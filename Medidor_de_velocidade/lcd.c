/*
 * lcd.c
 *
 *  Created on: 9 de nov de 2019
 *      Author: Vinicius Almeida
 */


#include <msp430.h>
#include <stdint.h>
#include "i2c.h"
#include <math.h>

uint8_t lcdAddr = 0x27; //0x3F
const uint8_t lcdRS = 0x01;
const uint8_t lcdRW = 0x02;
const uint8_t lcdEN = 0x04;
const uint8_t lcdBT = 0x08;



//----------------------Escreve niblle no lcd----------------------

void lcdWriteNibble(uint8_t nibble, uint8_t isChar)
{
    nibble = nibble << 4;
    nibble |= lcdBT | isChar;

    i2cSendByte(lcdAddr, nibble| 0);
    i2cSendByte(lcdAddr, nibble | lcdEN);
    i2cSendByte(lcdAddr, nibble| 0);
}


//----------------------Escreve niblle no lcd----------------------

void lcdWriteByte(uint8_t byte, uint8_t isChar)
{
    lcdWriteNibble(byte >> 4, isChar);
    lcdWriteNibble(byte     , isChar);
}

//----------------------Lê niblle no lcd----------------------

uint8_t lcdReadNibble();


//----------------------lê byte no lcd----------------------

uint8_t lcdReadbyte();

//----------------------Escreve niblle no lcd----------------------

void lcdInit()
{
    if(i2cTst(lcdAddr))
    {
        lcdAddr = 0x27;
    }

    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x3, 0);
    lcdWriteNibble(0x2, 0);

    lcdWriteByte(0x28, 0);
    lcdWriteByte(0x0C, 0);
    lcdWriteByte(0x06, 0);

    lcdWriteByte(0x01, 0);
    //lcdBusy();
}

//----------------------Lcd Busy---------------------

void lcdBusy();
//{
    //while(lcdReadByte() & 0x80);
//}

//----------------------Lcd Print---------------------

void lcdPrint(char *str)
{
    while(*str){
        lcdWriteByte(*str++,1);
    }
}

void Hex8(unsigned char c){         // printa dois nibbles em hexadecimal
    char LSB, MSB;
    LSB = (c & 0x0f);
    MSB = (c & 0xf0) >> 4;
    if(MSB >0x09){
        MSB = 0x37 + MSB;}
    else{
        MSB = 0x30 + MSB;}
    if(LSB >0x09){
        LSB = 0x37 + LSB;}
    else{
        LSB = 0x30 + LSB;}
    lcdWriteByte(MSB, 1);
    lcdWriteByte(LSB, 1);

}

void Hex16(unsigned int x){     //printa 4 nibbles em hexadecimal
    int y = x>>8;
    x = x & 0x00ff;
    Hex8(y);
    Hex8(x);
}


void dec3(float f){         //recebe float e printa ele na tela com 3 casas decimais
    int cur,print,aux;
    cur = (int)f;
    if(cur > 100){}
    print = cur + 0x30;
    lcdWriteByte(print,1);
    lcdPrint(",");
    f = (f -cur)*10;
    cur = (int)f;
    print =print = cur + 0x30;
    lcdWriteByte(print,1);
    f = (f -cur)*10;
    cur = (int)f;
    print =print = cur + 0x30;
    lcdWriteByte(print,1);
    f = (f -cur)*10;
    cur = (int)f;
    print =print = cur + 0x30;
    lcdWriteByte(print,1);


}

void lcd_limpar(){
    PCF_write(0x08); // (D7,D6,D5,D4) = 0000b = 0x0,  (BL,E,RW,RS) = 1000b = 0x8
    PCF_write(0x0C); // (D7,D6,D5,D4) = 0000b = 0x0,  (BL,E,RW,RS) = 1100b = 0xC
    PCF_write(0x08); // (D7,D6,D5,D4) = 0000b = 0x0,  (BL,E,RW,RS) = 1000b = 0x8

    PCF_write(0x18); // (D3,D2,D1,D0) = 0001b = 0x1,  (BL,E,RW,RS) = 1000b = 0x8
    PCF_write(0x1C); // (D3,D2,D1,D0) = 0001b = 0x1,  (BL,E,RW,RS) = 1000b = 0x8
    PCF_write(0x18); // (D3,D2,D1,D0) = 0001b = 0x1,  (BL,E,RW,RS) = 1000b = 0x8
}


void lcdCursor(uint8_t lin, uint8_t col){
    uint8_t l=0x00;
    if(lin){
        l=0x40;
    }
    lcdWriteByte((0x80 | l | col), 0);
}

void lcdClr(){
    lcdPrint("                                                                                                ");
    lcdCursor(0,0);
}


