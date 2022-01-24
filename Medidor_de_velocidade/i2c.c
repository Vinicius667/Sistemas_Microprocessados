/*
 * i2c.c
 *
 *  Created on: 9 de nov de 2019
 *      Author: Vinicius Almeida
 */




#include <msp430.h>
#include <stdint.h>


//------------Inicia comunicação i2c--------------------------------
void i2cInit()
{
    UCB0CTLW0 = UCSSEL__SMCLK |
                UCMODE_3      |
                UCSWRST       |
                UCMST;
    UCB0BRW = 10;

    P1SEL0 |=  (BIT2 | BIT3);
    P1SEL1 &= ~(BIT2 | BIT3);
    P1DIR  &= ~(BIT2 | BIT3);
    P1REN  |=  (BIT2 | BIT3);
    P1OUT  |=  (BIT2 | BIT3);

    UCB0CTLW0 &= ~UCSWRST;
}


//-------------------Testa o endereço do escravo-----------------------
uint8_t i2cTst(uint8_t addr)
{
    UCB0IFG = 0;                        //Zera as flags pois o nack nao é
                                        //zerado no start

    UCB0I2CSA = addr;                   //config. end. do escravo
    UCB0CTLW0 |= UCTR + UCTXSTT;        //pede um start

    while(!(UCB0IFG & UCTXIFG));        //Escreve um bye dummy para destravar
    UCB0TXBUF == 0x00;                  // o ciclo de ACK

    while(!(UCB0IFG & UCNACKIFG) &&     //Espera ou NACK ou TXIFG
            !(UCB0IFG & UCTXIFG) );

    UCB0CTLW0 |= UCTXSTP;               //Pede o stop e
    while(UCB0CTLW0 & UCTXSTP);         //aguarda ele ser enviado

    return (UCB0IFG & UCNACKIFG);       //Retorna ack ou nack
}

//---------------------------Envia dados---------------------------- -

uint8_t i2cSend(uint8_t addr, uint8_t *data, uint8_t count)
{
    UCB0IFG = 0;

    UCB0I2CSA = addr;
    UCB0CTLW0 |= UCTR + UCTXSTT;

    while(!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = *data++;
    count--;

    while(!(UCB0IFG & UCNACKIFG) &&
          !(UCB0IFG & UCTXIFG) );

    if(UCB0IFG & UCNACKIFG)
    {
        UCB0CTLW0 |= UCTXSTP;
        while(UCB0CTLW0 & UCTXSTP);
        return 1;
    }
    while(count--)
    {
        while(!(UCB0IFG & UCTXIFG));
        UCB0TXBUF = *data++;
    }

    UCB0CTLW0 |= UCTXSTP;
    while(UCB0CTLW0 & UCTXSTP);
    return 0;
}


//------------------------Coleta dados------------------------------

uint8_t i2cGet(uint8_t addr, uint8_t *data, uint8_t count)
{
    UCB0IFG = 0;

    UCB0I2CSA = addr;
    UCB0CTLW0 &= ~UCTR;
    UCB0CTLW0 |= UCTXSTT;

    while(UCB0IFG & UCTXSTT);

    if(UCB0IFG & UCNACKIFG)
    {
        UCB0CTLW0 |= UCTXSTP;
        while(UCB0CTLW0 & UCTXSTP);
        return 1;
    }

    while(--count)
    {
        while(!(UCB0IFG & UCRXIFG));
        *data++ = UCB0RXBUF;
    }

    UCB0CTLW0 |= UCTXSTP;

    while(!(UCB0IFG & UCRXIFG));
    *data++ = UCB0RXBUF;

    while(UCB0CTLW0 & UCTXSTP);

    return 0;
}


//------------------------Envia bytes-------------------------------------

uint8_t i2cSendByte(uint8_t addr, uint8_t byte)
{
    return i2cSend(addr, &byte, 1);
}


//-----------------------------------Coleta bytes------------------------

uint8_t i2cGetByte(uint8_t addr)
{
    uint8_t byte;
    i2cGet(addr, &byte, 1);
    return byte;
}

uint16_t convADinit(uint8_t pin)
 {
    PM5CTL0 &= ~(LOCKLPM5);
    ADCCTL0 = ADCON  |
              ADCSHT2;

    ADCCTL1 = ADCSHS_0|
              ADCSHP |
              ADCCONSEQ_0;
    ADCCTL2 = ADCRES_2;
    ADCMCTL0 = pin;

    P1SEL0 |= (1 << pin);
    P10SEL1|= (1 << pin);

    ADCCTL0 |= ADCENC;

    ADCCTL0 &= ~ADCSC;
    ADCCTL0 |= ADCSC;

    while(!(ADCIFG & ADCIFG0));
    return ADCMEM0;
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}

void PCF_write(char dado){
    UCB0IFG = 0;

    UCB0I2CSA = 0x27;                   //Endereço do escravo
    UCB0CTL1 |= UCTR   |                   //Mestre transmissor
                UCTXSTT;                   //Gerar START
    while((UCB0IFG & UCTXIFG) == 0);       //Esperar TXIFG = 1
    UCB0TXBUF = dado;


 while(!(UCB0IFG & UCNACKIFG) &&
          !(UCB0IFG & UCTXIFG) );

    if(UCB0IFG & UCNACKIFG)
    {
        UCB0CTLW0 |= UCTXSTP;
        while(UCB0CTLW0 & UCTXSTP);
    delay(50);
}}








