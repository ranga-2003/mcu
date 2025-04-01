#include "stm32f4xx.h"

volatile uint32_t msTicks = 0;  // Millisecond counter

// SysTick Interrupt Handler (Called every 1ms)
void SysTick_Handler(void) {
    msTicks++;  // Increment the millisecond counter
}

// Initialize SysTick Timer for 1ms tick
void SysTick_Init(void) {
    SystemCoreClockUpdate(); // Ensure the system clock is updated

    // Configure SysTick: (SystemCoreClock / 1000) -> 1ms interrupt
    if (SysTick_Config(SystemCoreClock / 1000)) {
        while (1); // Error Handling: Infinite loop if configuration fails
    }
}

// Millisecond Delay Function
void SysTick_Delay_ms(uint32_t delay) {
    uint32_t start = msTicks;  // Capture the current tick count
    while ((msTicks - start) < delay); // Wait for the delay to complete
}

// GPIO Initialization (Example: Configure PA5 for LED)
void GPIO_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA clock
    GPIOA->MODER |= (1 << 10);  // Set PA5 as output
}

// Main Function
int main(void) {
    SysTick_Init();  // Initialize SysTick Timer
    GPIO_Init();     // Initialize GPIO (for LED blinking example)

    while (1) {
        GPIOA->ODR ^= (1 << 5); // Toggle LED (PA5)
        SysTick_Delay_ms(1000);  // 500ms delay
    }
}
