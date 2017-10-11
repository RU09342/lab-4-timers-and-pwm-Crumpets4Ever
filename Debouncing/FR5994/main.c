#include <msp430.h>
//Author: Ben Jukus
//Debounce 5994
//which interrupt to use based on Chris Iapicco's code.
// I had the wrong type of interrupt disable. It happens

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;
//LED Jazz
    P1DIR |= BIT0;                              //sets the direction of pin 1 to output
    P1OUT &= ~BIT0;                             //sets output high

//Button Jazz
    P5DIR &= ~BIT5;                             //Sets button2, pin1.2 as an input
    P5REN |=  BIT5;                             //Enables the pullup/down resistor
    P5OUT |=  BIT5;                             //Set the resistor to be a pullup resistor

//Timer Junk
    TA0CCTL0 = CCIE;                            // CCR0 interrupt enabled
    TA0CCR0 = 10000;                            //Aclk runs at 10 hz maybe
    TA0CTL = TASSEL_2 + MC_0 +TA0CLR+ID_3;

// Interrupt Enable Jawn
    P5IE |=BIT5;                                //enable the interrupt on Port 1.1
    P5IES |=BIT5;                               //set interrupt to catch as falling edge
    P5IFG &=~(BIT5);                            //clear interrupt flag



__bis_SR_register(LPM0_bits + GIE);             // Enter LPM0
__no_operation();                               // For debugger

}

// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR               //Timer counts
__interrupt void TA0_ISR(void)
{
   P5IE|=BIT5;                                  //Re-enable P1.1 interrupts
   TA0CTL&=~BIT4;                               //Stop timer
   TA0CTL|=BIT2;                                //Clear timer
   P5IFG&=~BIT5;                                //Clear P1.1 interrupt flag
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT^=BIT0;                                //Toggle LED at P1.0
    P5IE&=~BIT5;                                //Disable P1.1 interrupt
    P5IFG&=~BIT5;                               //Clear P1.1 interrupt flag
    TA0CTL|=BIT4;                               //Turn timer to up mode

}
