#include <stm32f4xx.h>

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms*1600; i++)
    {
        __NOP();
    }
}
int main(void)
{

    RCC->AHB1ENR |= (0X01);
    GPIOA->MODER |= (1<<10);
    GPIOA->MODER &= ~(1<<11);

  while (1)
  {
        GPIOA->ODR ^= (1<<5);
        delay_ms(1000);
        GPIOA->ODR &= ~(1<<5);
       delay_ms(1000);
  }
}

