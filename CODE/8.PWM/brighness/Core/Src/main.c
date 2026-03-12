#include "stm32f4xx.h"

void TIM2_PWM_Init(void)
{
    // 1) Enable TIM2 & GPIOA clocks
    RCC->APB1ENR |= (1 << 0);   // TIM2EN = bit0
    RCC->AHB1ENR |= (1 << 0);   // GPIOAEN = bit0

    // 2) Configure PA5 as Alternate Function (AF1 = TIM2_CH1)
    GPIOA->MODER &= ~(3 << (5 * 2));   // Clear mode for PA5
    GPIOA->MODER |=  (2 << (5 * 2));   // AF mode (10)

    GPIOA->AFR[0] &= ~(0xF << (5 * 4)); // Clear AF bits (bits 23:20)
    GPIOA->AFR[0] |=  (0x1 << (5 * 4)); // AF1 ? TIM2_CH1

    // 3) Timer configuration ? 1 MHz tick, 20 ms period (50 Hz)
    TIM2->PSC = 84 - 1;          // Prescaler
    TIM2->ARR = 20000 - 1;       // Auto-reload

    // 4) Duty cycle (initial = 7.5%)
    TIM2->CCR1 = 1500;

    // 5) PWM Mode 1 on CH1 ? OC1M = 110, OC1PE = 1
    TIM2->CCMR1 &= ~(7 << 4);    // Clear OC1M bits (6:4)
    TIM2->CCMR1 |=  (6 << 4);    // PWM mode 1
    TIM2->CCMR1 |=  (1 << 3);    // OC1PE = 1

    // 6) Enable CH1 output
    TIM2->CCER |=  (1 << 0);     // CC1E = 1

    // 7) Enable auto-reload preload
    TIM2->CR1 |=  (1 << 7);      // ARPE = 1

    // 8) Update registers
    TIM2->EGR |=  (1 << 0);      // UG = 1

    // 9) Start timer
    TIM2->CR1 |=  (1 << 0);      // CEN = 1
}

int main(void)
{
    TIM2_PWM_Init();

    while (1)
    {
    }
}
