#include <msp430.h>
#include <stdint.h>
#include "i2c.h"
#include "lcd.h"

void debounce(volatile uint16_t i);
void clock_config(void);
void io(void);




int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);
    i2cInit();
    lcdInit();
    delay(10000);


    UCA0CTLW0 = UCSWRST | UCSSEL__SMCLK;
    UCA0BRW = 6;
    UCA0MCTLW = 0x22D1;


    UCA0CTLW0 &=~(UCSWRST);

    io();
    clock_config();



    volatile uint16_t p,p2 = 0;     //valor passado do Echo
    volatile uint16_t a,a2 = 0;     //valor atual do Echo
    volatile uint32_t s1,s2;        //distancia do objeto para os sensores
    volatile long signed dist = 0;  //deslocamento do objeto
    volatile uint16_t t1,t2;        //tempos em que os sensores percebe um obejeto
    volatile float vel, tempo;      //velocidade


    while(1){

        //MEDINDO LARGURA DO PULSO 1
        if(TB3R >= 52) {P6OUT &= ~(BIT3);}              //PWM POR SOFTWARE
        if(TB3R >= 2000) {P6OUT |= (BIT3); TB3CTL |= TBCLR;}


        if(!(P5IN & BIT4)){a = 0;}   //PINO CONECTADO AO ECHO
        else{a = 1;}



        if(p ==0){
            if(a == 1) {TB2CTL |= TBCLR;}   //rising edge
        }

        if(p ==1){
            if(a == 0) {
                s1 = TB2R;
                if(s1<800){t1 = TB0R;}
            }   //falling edge
        }
        p = a;     //novo valor passado é atualizado



        //MEDINDO LARGURA DO PULSO 2

        if(!(P5IN & BIT0)){a2 = 0;}
        else{a2 = 1;}



        if(p2 ==0){
            if(a2 == 1) {TB2CTL |= TBCLR;}   //rising edge
        }

        if(p2 ==1){
            if(a2 == 0) {
                s2 = TB2R;
                if(s2<800){t2 = TB0R;}
            }   //falling edge
        }

        p2 = a2;     //novo valor passado é atualizado



        dist = s2 - s1;

        if((dist > 400) && (dist < 6000) && (t1) && (t2)){      //range de distancias permitidas 10cm a 60cm

            lcdCursor(0, 0);
            lcdPrint("Velocidade: ");
            lcdCursor(0, 11);
            vel = (dist/(s2-s1))*0.016;
            dec3(vel);

            t1 = 0;
            t2 = 0;

        }

    }
}


void clock_config(void){
    TB2CTL |= TBSSEL__SMCLK| MC__CONTINOUS | TBCLR|ID__4;

    TB3CTL |= TBSSEL__SMCLK| MC__CONTINUOUS | TBCLR;   //P6.3 TB3.4


    TB0CTL |= TBSSEL__ACLK| MC__CONTINOUS | TBCLR | ID__8;
}


void io(void){
    P1SEL0 |= BIT6 | BIT7;
    P1SEL1 &= ~(BIT6|BIT7);

    P2DIR &= ~(BIT3);
    P2REN |= BIT3;
    P2OUT |= BIT3;

    P6DIR |= BIT6;
    P6OUT &= ~(BIT6);

    P6DIR |= BIT3;

    P5DIR &= ~(BIT4);
}
