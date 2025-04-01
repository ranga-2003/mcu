#include "stm32f4xx.h"
 int main(void) {
 // configure PA5 output for the LED
 RCC->AHB1ENR |= 1; /* enable GPIOA clock */
 GPIOA->MODER &= ~0x00000C00; /* clear pin mode */
 GPIOA->MODER |= 0x00000400; /* set pin to output mode */
 // configure PB8 as input of TIM2 ETR
 RCC->AHB1ENR |= 2; /* enable GPIOB clock */ GPIOB->MODER &=
 ~0x00030000; /* clear pin mode */
 GPIOB->MODER |= 0x00020000; /* set pin to alternate function */
 GPIOB->AFR[1] &= ~0x0000000F; /* clear pin AF bits */ GPIOB->AFR[1]
 |= 0x00000001; /* set pin to AF1 for TIM2 ETR */
 // configure TIM2 to use external input as counter clock source RCC
 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; /* Enable TIM2 clock */

 TIM2->SMCR = 0x4377; /* use ETR input as clock, no prescaler */
 TIM2->CNT = 0; /* clear counter */
 TIM2->CR1 = 1; /* enable TIM2 */
 while (1) {
 // use bit 0 to turn LED on/off
 if (TIM2->CNT & 1)
GPIOA->ODR |= 0x20; /* turn on LED */ else
 GPIOA->ODR &= ~0x20; /* turn off LED */ }
 }
