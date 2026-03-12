#include "stm32f4xx.h"

void delay(uint32_t ms)
{
    for(uint32_t i=0;i<ms*3000;i++)
        __NOP();
}

uint8_t seq[4]={0x09,0x03,0x06,0x0C};

/* Stepper motor function */
void stepper(uint8_t d)
{
    GPIOC->ODR &= ~(0x0F);   // Clear PC0-PC3
    GPIOC->ODR |= d;         // Send sequence
}

int main()
{
    /* Enable GPIOC clock */
    RCC->AHB1ENR |= (1<<2);

    /* PC0-PC3 output mode */
    GPIOC->MODER &= ~(0x000000FF);  
    GPIOC->MODER |=  (0x00000055);

    while(1)
    {
        for(int i=0;i<4;i++)
        {
					GPIOC->ODR &= ~(0x0F);   // Clear PC0-PC3
           GPIOC->ODR |=seq[i];  
            //stepper(seq[i]);
            delay(10);
        }
    }
}
