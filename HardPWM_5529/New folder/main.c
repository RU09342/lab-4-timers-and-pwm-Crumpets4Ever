/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//*******************************************************************************
//  MSP430F552x Demo - Timer0_A5, PWM TA1.1-2, Up Mode, DCO SMCLK
//
//  Description: This program generates two PWM outputs on P1.2,P1.3 using
//  Timer1_A configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using ~1.045MHz
//  SMCLK as TACLK, the timer period is ~500us with a 75% duty cycle on P1.2
//  and 25% on P1.3.
//  ACLK = n/a, SMCLK = MCLK = TACLK = default DCO ~1.045MHz.
//
//                MSP430F552x
//            -------------------
//        /|\|                   |
//         | |                   |
//         --|RST                |
//           |                   |
//           |         P1.2/TA0.1|--> CCR1 - 75% PWM
//           |         P1.3/TA0.2|--> CCR2 - 25% PWM
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>
int brightness[10] = {0, 10, 15, 25, 50, 100, 150, 250, 400, 511};
unsigned int i = 0;
int main(void)
{
            WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//LED lead
            P1DIR |= BIT2+BIT3;                       // P1.2 and P1.3 output
            P1SEL |= BIT2+BIT3;                       // P1.2 and P1.3 options select
//Button Jazz

            P1DIR &= ~BIT1; //Sets button2, pin1.2 as an input
            P1REN |=  BIT1; //Enables the pullup/down resistor
            P1OUT |=  BIT1; //Set the resistor to be a pullup resistor

            P2DIR &= ~BIT1; //Sets button2, pin1.2 as an input
            P2REN |=  BIT1; //Enables the pullup/down resistor
            P2OUT |=  BIT1; //Set the resistor to be a pullup resistor

//Timer Junk
//PWM Part
            TA0CCR0 = 512-1;                          // PWM Period
            TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
            TA0CCR1 = 0;                            // CCR1 PWM duty cycle
            TA0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
            TA0CCR2 = 511;                            // CCR2 PWM duty cycle
            TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR

//Timer Junk
//Timing/Debouncing Part
            TA1CCTL0 = CCIE;                        // CCR0 interrupt enabled
            TA1CCR0 = 10000;                          //Aclk runs at 10 hz maybe
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

    #pragma vector = PORT1_VECTOR               //Reset
    __interrupt void Port_1(void)
       {

           TA0CCR1 = brightness[i];
           TA0CCR2 = brightness[10-i];
           i++;
           if(i>10)
           {
               i=0;
           }
           P1IFG &= ~(BIT1);
           __bic_SR_register_on_exit(LPM0_bits + GIE);

        }
    #pragma vector = PORT2_VECTOR               //Reset
    __interrupt void Port_2(void)
        {
            TA0CCR1 = 511;
            TA0CCR2 = 256;
            i++;
            if(i>10)
            {
                i=0;
            }
            P2IFG &= ~(BIT1);
            __bic_SR_register_on_exit(LPM0_bits + GIE);

        }
    #pragma vector = TIMER1_A0_VECTOR           //Timer counts
    __interrupt void TA1_ISR(void)
        {
            P4OUT ^= BIT7;     //Test LED
           __bis_SR_register(LPM0_bits + GIE);
        }
