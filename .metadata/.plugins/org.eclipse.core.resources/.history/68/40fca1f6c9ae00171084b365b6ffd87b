/* Author: Ben Jukus
 * Software PWM 5529
 */

#include <msp430.h>
int count = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // disable watchdog timer
    //PM5CTL0 &= ~LOCKLPM5;                     //disables high impedence mode
//LED lead
    P1DIR |= BIT0;                              //sets the direction of pin 1 to output
    P1OUT &= ~(BIT0);                           //sets output high
    P4DIR |= BIT7;                              //sets the direction of pin 1 to output
    P4OUT &= ~(BIT7);                           //sets output high

//Button Jazz
    P1DIR &= ~BIT1;                             //Sets button2, pin1.2 as an input
    P1REN |=  BIT1;                             //Enables the pullup/down resistor
    P1OUT |=  BIT1;                             //Set the resistor to be a pullup resistor

    P2DIR &= ~BIT1;                             //Sets button2, pin1.2 as an input
    P2OUT |=  BIT1;                             //Set the resistor to be a pullup resistor
    P2REN |=  BIT1;                             //Enables the pullup/down resistor

//Timer Jingle
    TA0CTL = TASSEL_2 + MC_1;                   //Sets Timer A0 to SMCLK, Upmode

    TA0CCTL0 = CCIE;                            //Enables CCR0 interrupt
    TA0CCR0 = 3200;                             //Sets CCR0 to 1 kHz

    TA0CCTL1 = CCIE;                            //Enables CCR1 interrupt
    TA0CCR1 = 1600;                             //Sets CCR1 to .5 kHz

// Interrupt Enable Jawn
    P1IE |=BIT1;                                //enable the interrupt on Port 1.1
    P1IES |=BIT1;                               //set as falling edge
    P1IFG &=~(BIT1);                            //clear interrupt flag

    P2IE |=BIT1;                                //enable the interrupt on Port 2.1
    P2IES |=BIT1;                               //set as falling edge
    P2IFG &=~(BIT1);                            //clear interrupt flag


    _BIS_SR(LPM0_bits+GIE);                     //Enter low power mode, general interrupt enable

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

        if(count==10)
        {
            count=0;                            //resets count
            TA0CCR1=0;                          //resets capture compare for Timer A0 (timer counter)
        }
        else
        {
            count++;
            TA0CCR1 = count*420;                //increments but jubluh
        }

    P4OUT ^= BIT7;                              //toggle led
    P2IFG &=~(BIT1);                            //clear interrupt flag

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR (void)
{
    if(count!=0)                                //LED is on while
    {
        P4OUT |= (BIT7);                        //sets output high

    }
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TA1_ISR (void)
{
    if(count!=10)
    {
        P4OUT &= ~(BIT7);                           //sets output high
    }
    TA0IV=0;
}
