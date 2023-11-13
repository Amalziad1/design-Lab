//يكون الاحمر ضاوي او لم تشتغل اذا ضغطنا عالسويتش اللي عالشمال يضوي اخضر واليمين يضوي ازرق 
/*PORTF PF0 and PF4 fall edge interrupt example*/
/*This GPIO interrupt example code controls green LED with switches SW1 and SW2 external interrupts */
#define DELAY 5000000
#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))


#include "TM4C123.h" // Device header
int main(void)
{

SYSCTL_RCGCGPIO_R = 0x20;
	
/* PORTF0 has special function, need to unlock to modify */
	GPIOF->LOCK = 0x4C4F434B; /* unlock commit register */
	GPIOF->CR = 0x01; /* make PORTF0 configurable */
	GPIOF->LOCK = 0; /* lock commit register */
/*Initialize PF3 as a digital output, PF0 and PF4 as digital input pins */
	GPIOF->DIR &= ~(1<<4)|~(1<<0); /* Set PF4 and PF0 as a digital input pins */
	GPIOF->DIR |= (1<<3)|(1<<2)|(1<<1); /* Set PF3 as digital output to control green LED */
	GPIOF->DEN |= (1<<4)|(1<<3)|(1<<0)|(1<<1) |(1<<2) ; /* make PORTF4-0 digital pins */
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

 while(1)
 {
GPIOF->DATA |= (1<<1);
 }
}
/* SW1 is connected to PF4 pin, SW2 is connected to PF0. */
/* Both of them trigger PORTF falling edge interrupt */
void GPIOF_Handler(void)
{
	
	volatile unsigned long ulLoop ;
	
	SYSCTL->RCGCGPIO |= (1<<5); /* Set bit5 of RCGCGPIO to enable clock to PORTF*/

	
 if (GPIOF->MIS & 0x10) /* check if interrupt causes by PF4/SW1*/
 {while(1){
	 
		  GPIOF->DATA &= ~(1<<1);//clear red
			GPIOF->DATA |= (1<<3);//green
 for ( ulLoop = 0; ulLoop < DELAY; ulLoop++)
{

 
 }
 GPIOF->DATA &= ~(1<<3);// clear green
 GPIOF->DATA &= ~(1<<1);//clear red
GPIOF->DATA |= (1<<1);//red
 for ( ulLoop = 0; ulLoop < DELAY; ulLoop++)
{

 }
 GPIOF->DATA &= ~(1<<3);// clear green
GPIOF->DATA &= ~(1<<1);//clear red
 GPIOF->ICR |= 0x10; /* clear the interrupt flag */
 if (GPIOF->MIS & 0x01) break;
 }}
 

 else if (GPIOF->MIS & 0x01) /* check if interrupt causes by PF0/SW2 */
 {	while(1){
	 	  GPIOF->DATA &= ~(1<<1);
			GPIOF->DATA &= ~(1<<3);// clear green
			GPIOF->DATA &= ~(1<<2);//clear red
			GPIOF->DATA |= (1<<2);//blue
	 
	  for ( ulLoop = 0; ulLoop < DELAY; ulLoop++)
	{
		}
	GPIOF->DATA &= ~(1<<1);
			GPIOF->DATA &= ~(1<<3);// clear green
			GPIOF->DATA &= ~(1<<2);//clear red
			GPIOF->DATA |= (1<<3);//blue
	  for ( ulLoop = 0; ulLoop < DELAY; ulLoop++)
	{
		}
	
 GPIOF->DATA &= ~(1<<2);
 GPIOF->ICR |= 0x01; /* clear the interrupt flag */
		if (GPIOF->MIS & 0x10) break;
 }
 
 }
}

