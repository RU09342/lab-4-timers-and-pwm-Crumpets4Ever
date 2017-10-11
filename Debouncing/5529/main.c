/* Author: Ben Jukus
 * Debounce 5529
*/
#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
//LED Jazz
            P1DIR |= BIT0;                          //sets the direction of pin 1 to output
            P1OUT &= ~BIT0;                         //sets LED as output

            P4DIR |= BIT7;                          //sets the direction of pin 1 to output
            P4OUT &= ~BIT7;                         //sets output high
//Button Jazz

            P1DIR &= ~BIT1;                         //Sets button2, pin1.2 as an input
            P1REN |=  BIT1;                         //Enables the pullup/down resistor
            P1OUT |=  BIT1;                         //Set the resistor to be a pullup resistor

            P2DIR &= ~BIT1;                         //Sets button2, pin1.2 as an input
            P2REN |=  BIT1;                         //Enables the pullup/down resistor
            P2OUT |=  BIT1;                         //Set the resistor to be a pullup resistor

//Timer Junk
            TA1CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TA1CCR0 = 800;                          //Aclk runs at 10 hz maybe
            TA1CTL = TASSEL_1 + MC_1;

// Interrupt Enable Jawn
            P1IE |=BIT1;                            //enable the interrupt on Port 1.1
            P1IES |=BIT1;                           //set as falling edge
            P1IFG &=~(BIT1);                        //clear interrupt flag

            P2IE |=BIT1;                            //enable the interrupt on Port 2.1
            P2IES |=BIT1;                           //set as falling edge
            P2IFG &=~(BIT1);                        //clear interrupt flag

__bis_SR_register(LPM0_bits + GIE);                 // Enter LPM0 and general interrupt enable
__no_operation();                                   // For debugger
}
//Buttton 1.1 Interrupt
    #pragma vector = PORT1_VECTOR
    __interrupt void Port_1(void)
       {
            P1IE|=BIT1;//Re-enable P1.3 interrupts
            TACTL&=~BIT4;//Stop timer
            TACTL|=BIT2;//Clear timer
            P1IFG&=~BIT3;//Clear P1.3 interrupt flag
       }

//Timer interrupt
#pragma vector = TIMER1_A0_VECTOR           //Timer counts
__interrupt void TA1_ISR(void)
    {
            P1OUT^=BIT0;//Toggle LED at P1.0
            P1IE&=~BIT3;//Disable P1.3 interrupt
            P1IFG&=~BIT3;//Clear P1.3 interrupt flag
            TACTL|=BIT4;//Turn timer to up mode
    }