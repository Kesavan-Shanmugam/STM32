#include "stm32f4xx.h"

/* ---------------- DELAY ---------------- */
void delay(uint32_t ms)
{
    for(uint32_t i=0;i<ms*3000;i++)
        __NOP();
}

/* ------------ MOTOR A --------------- */

void MotorA_Forward(void)
{
    GPIOB->ODR |= (1<<12);
    GPIOB->ODR &= ~(1<<13);
}

void MotorA_Reverse(void)
{
    GPIOB->ODR &= ~(1<<12);
    GPIOB->ODR |= (1<<13);
}

void MotorA_Stop(void)
{
    GPIOB->ODR &= ~(1<<12);
    GPIOB->ODR &= ~(1<<13);
}

/* ------------ MOTOR B --------------- */

void MotorB_Forward(void)
{
    GPIOB->ODR |= (1<<14);
    GPIOB->ODR &= ~(1<<15);
}

void MotorB_Reverse(void)
{
    GPIOB->ODR &= ~(1<<14);
    GPIOB->ODR |= (1<<15);
}

void MotorB_Stop(void)
{
    GPIOB->ODR &= ~(1<<14);
    GPIOB->ODR &= ~(1<<15);
}

/* ------------ GPIO INIT -------------- */

void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1<<1);   // GPIOB clock

    GPIOB->MODER &= ~((3<<(12*2)) |(3<<(13*2)) |(3<<(14*2)) |(3<<(15*2)));

    GPIOB->MODER |= ((1<<(12*2)) |(1<<(13*2)) |(1<<(14*2)) |(1<<(15*2)));
}

/* -------------- MAIN ----------------- */

int main(void)
{
    GPIO_Init();

    while(1)
    {
        // Both motors forward
        MotorA_Forward();
        MotorB_Forward();
        delay(500);

        // Stop
        MotorA_Stop();
        MotorB_Stop();
        delay(1000);

        // Both motors reverse
        MotorA_Reverse();
        MotorB_Reverse();
        delay(500);

        // Stop
        MotorA_Stop();
        MotorB_Stop();
        delay(2000);
    }
}
