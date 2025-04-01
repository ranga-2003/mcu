#include "stm32f4xx.h"

void TIM3_PWM_Init(void);
void GPIO_Init(void);
void delay_ms(uint32_t ms);

int main(void)
{
    GPIO_Init();       // Initialize PA6 for PWM output
    TIM3_PWM_Init();   // Configure TIM3 for PWM

    while (1)
    {
        // Gradually decrease brightness from 100% to 25% over 1 second
        for (int duty = 100; duty >= 25; duty--)
        {
            TIM3->CCR1 = (duty * TIM3->ARR) / 100;  // Adjust PWM duty cycle
            delay_ms(10);  // Smooth fading effect
        }

        while (1);  // Keep LED at 25% brightness
    }
}

/* Function to Initialize TIM3 in PWM Mode */
void TIM3_PWM_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable clock for TIM3

    TIM3->PSC = 84 - 1;    // Prescaler (1 MHz timer clock)
    TIM3->ARR = 999;       // Auto-reload value (PWM period = 1 kHz)
    TIM3->CCR1 = 999;      // Start with 100% brightness
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);  // PWM Mode 1 (OC1M = 110)
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;  // Enable output preload
    TIM3->CCER |= TIM_CCER_CC1E;  // Enable PWM output on CH1
    TIM3->CR1 |= TIM_CR1_CEN;  // Enable timer
}

/* Function to Initialize GPIOA Pin 6 as Alternate Function (TIM3 CH1) */
void GPIO_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable clock for GPIOA

    GPIOA->MODER &= ~(3 << (6 * 2));  // Clear mode bits
    GPIOA->MODER |= (2 << (6 * 2));   // Set PA6 as Alternate Function mode

    GPIOA->AFR[0] |= (2 << (6 * 4));  // Set PA6 to AF2 (TIM3_CH1)
}

/* Simple Delay Function */
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++)
    {
        __NOP();  // Do nothing (simple delay)
    }
}
