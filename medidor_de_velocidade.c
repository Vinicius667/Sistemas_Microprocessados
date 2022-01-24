// Medidor de velocidade utilizando dois sensores ultrassonicos 
// conectar no 5V
// Trigger no P6.3
// Echo no P5.4
// ground no ground
// echo fica em nivel alto esperando receber o sinal de volta
// distancia proprocional ao clk

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



     volatile uint16_t p,p2 = 0;    //valor passado do acho
     volatile uint16_t a,a2 = 0;    //valor atual do echo
     volatile uint32_t clk,clk2;
     volatile  float cm;    //distancia em cm
     static uint16_t limite = 1600;
     volatile uint32_t vetor[100];
     volatile uint32_t clocks[100];
     volatile uint16_t count = 0;
     volatile uint16_t s_a = 0;
     volatile uint16_t s_p = 0;
     volatile long signed dist = 0;
     volatile uint16_t v1,v2;
     volatile float vel;
     v1 = 1000;
     v2 = 1000;



     while(1){


         if(!(P5IN & BIT4)){a = 0;}
         else{a = 1;}



         if(p ==0){
             if(a == 1) {TB2CTL |= TBCLR;}   //rising edge
         }

         if(p ==1){
                      if(a == 0) {
                          clk = TB2R;
                          if(clk<800){v1 = TB0R;}
                      }   //falling edge
                  }
         p = a;     //novo valor passado é atualizado




         if(!(P5IN & BIT0)){a2 = 0;}
         else{a2 = 1;}



         if(p2 ==0){
             if(a2 == 1) {TB2CTL |= TBCLR;}   //rising edge
         }

         if(p2 ==1){
                      if(a2 == 0) {
                          clk2 = TB2R;
                          if(clk2<800){v2 = TB0R;}
                      }   //falling edge
                  }
         p2 = a2;     //novo valor passado é atualizado

         /////////////////////////////////////////////////////////////////////////////////////


             dist = v1 - v2;

             if((dist > 400) && (dist < 6000) && (v1) && (v2)){
                 P6OUT |= (BIT6);
                 lcdCursor(0, 0);
                 lcdPrint("Velocidade: ");
                 lcdCursor(0, 11);
                 vel = 4000/dist;
                 dec3(vel);
                 while((P2IN&BIT3));
                 debounce(15000);
                 P6OUT &= ~(BIT6);
                 v1 = 0;
                 v2 = 0;
             }





         /////////////////////////////////////////////////////////////////////////////////////




/*         if(!(P2IN & BIT3)){s_a = 0;}
         else{s_a = 1;}

         if((P2IN & BIT3)){
             if(s_p == 0){debounce(15000); }  //rising edge
         }

         if(!(P2IN & BIT3)){
             if(s_p == 1){debounce(15000);  count = 0; TB3CTL |= TBCLR; }   //falling edge
         }*/
         s_p = s_a;



     }
}

void debounce(volatile uint16_t i) {
    while (i--);
}

void clock_config(void){
    TB2CTL |= TBSSEL_SMCLK| MCCONTINOUS | TBCLR|ID_4;

         TB3CTL |= TBSSEL_SMCLK| MC_UP | TBCLR;   //P6.3 TB3.4
         TB3CCR0 = 20000;
         TB3CCR4 = 52;
         TB3CCTL4 = OUTMOD_6;

         TB0CTL |= TBSSEL_ACLK| MC_CONTINOUS | TBCLR;
}


void io(void){

    P1SEL0 |= BIT6 | BIT7;
    P1SEL1 &= ~(BIT6|BIT7);

       P2DIR &= ~(BIT3);
       P2REN |= BIT3;
       P2OUT |= BIT3;

     P6DIR |= BIT6;
     P6OUT &= ~(BIT6);

}
