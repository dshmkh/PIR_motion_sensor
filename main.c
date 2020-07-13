
#include <msp430.h>

#define DELAY_05s 62500

unsigned char counter = 0;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;      // stop WDT

// P2.2 is PIR input
  P2REN |= 0x04;            // enable pull-up
  P2OUT &= 0x04;            // pull-up
  P2IE |= 0x04;            // interrupt enabled
  P2IES &= 0x04;            // hi/lo edge but might be changed depending on your PIR
              // IFG cleared

  P1DIR |= 0x02; // P1.1 is LED
  TACCR0 = DELAY_05s;
  TACTL = TASSEL_2 + MC_1 + ID_3;   // SMCLK, upmode , /8
  TACCTL0 = CCIE;

  __bis_SR_register(CPUOFF + GIE); // switch to LPM0 with interrupts
}

// Timer A interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
      if(counter == 10) {
       counter = 0;
  P1OUT &= ~0x02; // LED off
  __bis_SR_register_on_exit(LPM4_bits); // switch from LPM0 to LPM4
      } else {
        counter++;
      }
}

// Port 1 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)

{ 
  P1OUT |= 0x02; // LED on
  counter = 0;

  __bic_SR_register_on_exit(LPM4_bits-LPM0_bits); // switch from LPM4 to LPM0

    P2IFG &= ~0x04;              // IFG cleared
}