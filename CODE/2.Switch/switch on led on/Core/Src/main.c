
#include<stm32f4xx.h>
void delay(uint32_t count)
{
    for(uint32_t i = 0; i < count * 16000; i++) {
        __NOP();
    }
}
int main()
{
    RCC->AHB1ENR |= (1<<0);
	  RCC->AHB1ENR |= (1 << 2);
	
    GPIOA->MODER &= ~(3 << (5 * 2));  // Clear bits for PA5
    GPIOA->MODER |=  (1 << (5 * 2));  // Set as output


    GPIOC->MODER &= ~(3 << (13 * 2)); 

    GPIOC->PUPDR &= ~(3 << (13 * 2));
    GPIOC->PUPDR |=  (1 << (13 * 2)); 

    while(1)
    {
      
            if((GPIOC->IDR & (1<<13))==0)
            {
                GPIOA->ODR |= (1<<5);
            }
            else
            {
                GPIOA->ODR &=~ (1<<5);
						}
    }
}

