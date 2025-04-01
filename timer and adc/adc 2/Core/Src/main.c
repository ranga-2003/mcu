#include "stm32f4xx.h"
#include <stdio.h>

#define V25        0.76   // Voltage at 25°C (in Volts)
#define AVG_SLOPE  0.0025 // 2.5mV/°C slope

void UART2_Init(void);
void UART2_SendChar(char c);
void UART2_SendString(char *str);
void ADC1_Init(void);
uint16_t ADC1_Read(void);
float Get_Temperature(void);

void delay(uint32_t time) {
    for (uint32_t i = 0; i < time * 4000; i++); // Approximate delay
}

/* Initialize ADC1 for Internal Temperature Sensor */
void ADC1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  // Enable ADC1 clock
    ADC->CCR |= ADC_CCR_TSVREFE;         // ✅ Enable temperature sensor
    ADC1->CR2 = ADC_CR2_ADON;            // Enable ADC1
    ADC1->SMPR1 |= ADC_SMPR1_SMP16;      // Set sampling time (480 cycles)
    ADC1->SQR3 = (16 & ADC_SQR3_SQ1);    // Select Channel 16 (IN16)
}



/* Read ADC Value from Internal Temperature Sensor */
uint16_t ADC1_Read(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART; // Start conversion
    while (!(ADC1->SR & ADC_SR_EOC)); // Wait for conversion to finish
    return ADC1->DR; // Read ADC value
}

/* Convert ADC Value to Temperature (°C) */

float Get_Temperature(void) {
    uint16_t adc_value = ADC1_Read();
    float V_sense = (adc_value * 3.3) / 4095.0;  // Convert ADC to voltage
    float temperature = ((V_sense - 0.76) / 0.0025) + 25.0;  // Convert to Celsius
    return temperature;
}




/* Initialize UART2 (PA2=TX, PA3=RX) */
void UART2_Init(void) {
    /* Enable Clocks for GPIOA and USART2 */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Configure PA2 (TX) as Alternate Function (AF7) */
    GPIOA->MODER |= (2 << (2 * 2));  // PA2 as AF mode
    GPIOA->AFR[0] |= (7 << (4 * 2)); // AF7 for USART2

    /* Configure USART2 */
    USART2->BRR = 0x683; // Baud rate 9600 (assuming 16MHz clock)
    USART2->CR1 = USART_CR1_TE | USART_CR1_UE; // Enable UART, TX mode
}

/* Send a character via UART2 */
void UART2_SendChar(char c) {
    while (!(USART2->SR & USART_SR_TXE)); // Wait for TX buffer empty
    USART2->DR = c; // Send character
}

/* Send a string via UART2 */
void UART2_SendString(char *str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}

int  main(void) {
    char msg[50];

    UART2_Init();
    ADC1_Init();

    while (1) {
        uint16_t adc_value = ADC1_Read();
        float V_sense = (adc_value * 3.3) / 4095.0; // Convert ADC to voltage
        sprintf(msg, "ADC: %d, V_sense: %.3fV\r\n", adc_value, V_sense);
        UART2_SendString(msg);
        delay(1000);
    }
}



