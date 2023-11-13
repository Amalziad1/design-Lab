#include "TM4C123.h" // Device header
#define DELAY 2000000
int main(void)
{
volatile unsigned long ulLoop ;
SYSCTL->RCGCGPIO |= (1<<5);
/* PORTF0 has special function, need to unlock to modify */
GPIOF->LOCK = 0x4C4F434B; /* unlock commit register */
GPIOF->CR = 0x01; /* make PORTF0 configurable */
GPIOF->LOCK = 0; /* lock commit register */
/*Initialize PF3 as a digital output, PF0 and PF4 as digital input pins */
GPIOF->DIR &= ~(1<<4)|~(1<<0); /* Set PF4 and PF0 as a digital input pins */
GPIOF->DIR |= (1<<3); /* Set PF3 as digital output to control green LED */
GPIOF->DIR |= (1<<2);//turn on blue led
GPIOF->DIR |= (1<<1); //turn on red led

GPIOF->DEN |= (1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0); /* make PORTF4-0 digital pins */
GPIOF->PUR |= (1<<4)|(1<<0); /* enable pull up for PORTF4, 0 */
/* configure PORTF4, 0 for falling edge trigger interrupt */
GPIOF->IS &= ~(1<<4)|~(1<<0); /* make bit 4, 0 edge sensitive */
GPIOF->IBE &=~(1<<4)|~(1<<0); /* trigger is controlled by IEV */
GPIOF->IEV &= ~(1<<4)|~(1<<0); /* falling edge trigger */
GPIOF->ICR |= (1<<4)|(1<<0); /* clear any prior interrupt */
GPIOF->IM |= (1<<4)|(1<<0); /* unmask interrupt */
/* enable interrupt in NVIC and set priority to 3 */
NVIC->IP[30] = 3 << 5; /* set interrupt priority to 3 */
NVIC->ISER[0] |= (1<<30); /* enable IRQ30 (D30 of ISER[0]) */
GPIOF->DATA = 0;
GPIOF->DATA |= (1<<1);//start with red before clicking the sw1 &sw2
GPIOF->ICR |= 0x10; /* clear the interrupt flag */

while(1)
{
}
}

void GPIOF_Handler(void)
{
volatile unsigned long ulLoop ;

while(1){
for (ulLoop = 0; ulLoop < DELAY; ulLoop++)
{
for (ulLoop = 0; ulLoop < DELAY; ulLoop++)
{
}
}
if (GPIOF->MIS & 0x10) /* check if interrupt causes by PF4/SW1*/
{

GPIOF->DATA = 0;
GPIOF->DATA |= (1<<2);//blue
GPIOF->ICR |= 0x10; /* clear the interrupt flag */

}

else if (GPIOF->MIS & 0x01) /* check if interrupt causes by PF0/SW2 */
{
GPIOF->DATA = 0;
GPIOF->DATA |= (1<<3);//green
GPIOF->ICR |= 0x10; /* clear the interrupt flag */

}

}

}
