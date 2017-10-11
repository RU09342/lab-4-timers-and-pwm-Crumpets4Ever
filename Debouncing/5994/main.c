#include <msp430.h>
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//LED Jazz
            P1DIR |= BIT0; //sets the direction of pin 1 to output
            P1OUT &= ~BIT0; //sets output high

            P4DIR |= BIT7; //sets the direction of pin 1 to output
            P4OUT &= ~BIT7; //sets output high
//Button Jazz

            P1DIR &= ~BIT1; //Sets button2, pin1.2 as an input
            P1REN |=  BIT1; //Enables the pullup/down resistor
            P1OUT |=  BIT1; //Set the resistor to be a pullup resistor

            P2DIR &= ~BIT1; //Sets button2, pin1.2 as an input
            P2REN |=  BIT1; //Enables the pullup/down resistor
            P2OUT |=  BIT1; //Set the resistor to be a pullup resistor

//Timer Junk
            TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
            TA1CCR0 = 800;                          //Aclk runs at 10 hz maybe
            TA1CTL = TASSEL_1 + MC_1;

// Interrupt Enable Jawn
            P1IE |=BIT1;//enable the interrupt on Port 1.1
            P1IES |=BIT1;//set as falling edge
            P1IFG &=~(BIT1);//clear interrupt flag

            P2IE |=BIT1;//enable the interrupt on Port 2.1
            P2IES |=BIT1;//set as falling edge
            P2IFG &=~(BIT1);//clear interrupt flag

__bis_SR_register(LPM0_bits + GIE);             // Enter LPM0
__no_operation();                         // For debugger
}
    #pragma vector = PORT2_VECTOR               //Reset
    __interrupt void Port_2(void)
       {
        P1OUT ^= BIT0;
        P2IFG &= ~(BIT1);
        __bic_SR_register_on_exit(LPM0_bits + GIE);
       }
#pragma vector = PORT1_VECTOR               //Reset
__interrupt void Port_1(void)
   {
        P1OUT ^= BIT0;
        P1IFG &= ~(BIT1);
        __bic_SR_register_on_exit(LPM0_bits + GIE);
   }
#pragma vector = TIMER1_A0_VECTOR           //Timer counts
__interrupt void TA1_ISR(void)
    {
        P4OUT ^= BIT7;     //Test LED
       __bis_SR_register(LPM0_bits + GIE);
    }
