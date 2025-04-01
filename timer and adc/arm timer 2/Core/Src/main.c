
 #include "stm32f4xx.h"
 int main(void) {
 RCC->AHB1ENR |= 1; /* enable GPIOA clock */
 GPIOA->MODER &= ~0x00000C00; /* clear pin mode */
 GPIOA->MODER |= 0x00000400; /* set pin to output mode */ /*
 Configure SysTick */
 SysTick->LOAD = 3200000 - 1; /* reload with number of clocks per
 second */ SysTick->VAL = 0;
 SysTick->CTRL = 5; /* enable it, no interrupt, use system clock */
 while (1) {
 if (SysTick->CTRL & 0x10000) { /* if COUNT flag is set */ GPIOA->ODR
 ^= 0x00000020; /* toggle green LED */
 }
 }
 }
