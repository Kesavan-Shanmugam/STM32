#include "stm32f4xx.h"
#include <stdio.h>

void delay(uint32_t ms)
{
    for(uint32_t i = 0; i < ms * 1600; i++)
        __NOP();
}

/* LCD command */
void command(uint8_t cmd)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | cmd;  // D0–D7
    GPIOB->ODR &= ~(1<<8);   // RS = 0
    GPIOB->ODR |=  (1<<9);   // EN = 1
    delay(1);
    GPIOB->ODR &= ~(1<<9);   // EN = 0
    delay(2);
}

/* LCD data */
void data(uint8_t d)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | d;    // D0–D7
    GPIOB->ODR |=  (1<<8);   // RS = 1
    GPIOB->ODR |=  (1<<9);   // EN = 1
    delay(1);
    GPIOB->ODR &= ~(1<<9);   // EN = 0
    delay(2);
}

/* ADC1 initialization */
void ADC_INIT(void)
{
    // GPIOA clock enable
    RCC->AHB1ENR |= (1<<0);

    // PA0 analog mode
    GPIOA->MODER |= (3<<(0*2));   // 11 -> analog
    GPIOA->PUPDR &= ~(3<<(0*2));  // no pull

    // ADC1 clock enable
    RCC->APB2ENR |= (1<<8);

    // ADC config
    ADC1->CR2 = 0;           // clear CR2
    ADC1->SMPR2 |= (7<<0);   // sample time for channel 0 = 480 cycles
    ADC1->SQR3  = 0;         // channel 0 first
    ADC1->CR2  |= (1<<0);    // ADON = 1
}

/* Read ADC value */
uint16_t ADC_Read(void)
{
    uint16_t val;
    ADC1->CR2 |= (1<<30);           // SWSTART
    while(!(ADC1->SR & (1<<1)));    // Wait for EOC
    val = ADC1->DR;
    ADC1->SR &= ~(1<<1);            // Clear EOC
    return val;
}

int main(void)
{
    char value[20];
    uint16_t ADC_VALUE;

    // LCD GPIO setup
    RCC->AHB1ENR |= (1<<1);              // GPIOB clock
    GPIOB->MODER &= ~(0xFFFF);
    GPIOB->MODER |=  (0x5555);           // PB0–PB7 output
    GPIOB->MODER |=  (1<<(8*2)) | (1<<(9*2)); // PB8, PB9 output

    // LCD initialize
    command(0x38); // 8-bit, 2 line
    command(0x0C); // Display ON
    command(0x06); // Entry mode
    command(0x01); // Clear display

    ADC_INIT();

    while(1)
    {
        ADC_VALUE = ADC_Read();
        sprintf(value, "ADC: %d", ADC_VALUE);

        command(0x80); // Move cursor to first line start

        for(uint8_t i = 0; value[i] != '\0'; i++)
            data(value[i]);

        delay(1000);
    }
}

