#include <msp430.h>


/**
 * main.c
 */

void io(void)
{
    P2DIR &= ~(BIT0);           // entrada conectada ao sensor

     P6DIR |= (BIT0);           //s0
     P6OUT | (BIT0);            // s0 = 1

     P6DIR |= (BIT1);           //s1
     P6OUT &= ~(BIT0);          // s1 = 0

     P6DIR |= (BIT2);           //s2
     P6DIR |= (BIT3);           //s3
     P6DIR |= (BIT6);           //led verde
     P1DIR |= (BIT0);           //led azul
    }


void timer(void)                            // configura timers
{

    TB0CTL = TBSSEL__ACLK |                 //seleciona clock
    ID__8|                                  //selciona divisor 8
    MC__CONTINOUS |                         // seleciona modo continuo
    TBCLR;                                  // inicia clock do 0
   // TB0EX0 = TBIDEX__4;                   //selciona outro divisor pra 4
}


int periodo(void)                           //caclula periodo, presume-se que o filtro correspondente a cor ja foi selecionado
{
    volatile unsigned int cv,pv = 0;
    volatile long unsigned  soma = 0;            //maior valor ffff
    volatile unsigned int dif = 0;
    volatile int i;
    //volatile unsigned int media = 0;

    for(i =1 ; i < 11; i = i + 1 )          //dez interações
    {
    while(!(P2IN&BIT0));                  // garante que as análises começam na sabida do clock

    pv = cv;                                  // valor passado do clock
    cv =  TB0R;                              // valor atual do clock


    if (i>2)                                // salva 8 iterações
    {
        if (pv > cv)
       {dif = 0xffff +cv - pv;}         //corrige caso valores de tempo estejam em rampas diferentes

        else
       {dif = cv - pv;}

        soma += dif;
        while ((P2IN&BIT0));                    // espera descer
    }

  }
     volatile unsigned int media = soma/8;
    return media;

}


void led(int r, int g,int b)             //acende os leds de acordo com o perido de cada cor
    {
            if((r <b) &  (r < g) )
            {
                P1OUT |= (BIT0);
                P6OUT &= ~(BIT6);
            }

            if((g < b) & (g < r) )
            {
                P6OUT |= (BIT6);
                P1OUT &= ~(BIT0);
            }

            if( (b < r) & (b < g) )
            {
                P1OUT |= (BIT0);
                P6OUT |= (BIT6);
            }

    }



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer
    PM5CTL0 &= ~(LOCKLPM5);

    int r,g,b;
    timer();
    io();

    while(1)
    {
        //testa vermelho
        P6OUT &= ~(BIT2);     //seleciona filtro no sensor (chaves s2 e s3)
        P6OUT &= ~(BIT3);
        r = periodo();

        //testa azul
        P6OUT &= ~(BIT2);
        P6OUT |= ~(BIT3);
        b = periodo();


        //testa verde
        P6OUT |= (BIT2);
        P6OUT |= (BIT3);
        g = periodo();

        led(r,g,b);

    }
    return 0;
}
