
#include <stm32f4xx.h>

void delay(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 1600; i++)
    {
        __NOP();   
    }
}

void EXTI15_10_IRQHandler(void)     // Interrupt handler for lines 10–15
{
    if (EXTI->PR & (1 << 13))       // Check pending flag for line 13
    {
        GPIOA->ODR ^= (1 << 5);     // Toggle LED on PA5
        EXTI->PR |= (1 << 13);      // Clear pending flag
    }
}

int main(void)
{
    // Enable clocks
    RCC->AHB1ENR |= (1 << 0);       // Enable GPIOA clock
    RCC->AHB1ENR |= (1 << 2);       // Enable GPIOC clock
    RCC->APB2ENR |= (1 << 14);      // Enable SYSCFG clock

    // Configure PA5 as output (LED)
    GPIOA->MODER &= ~(3 << 10);
    GPIOA->MODER |=  (1 << 10);
	
	  GPIOA->MODER &= ~(3 << 0);
    GPIOA->MODER |=  (1 << 0);
	
    // Configure PC13 as input (Button)
    GPIOC->MODER &= ~(3 << 26);          // PC13 input
 
    // Connect EXTI13 line to PC13
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |=  (0x2 << 4);   // 0x2 = Port C

    // Configure EXTI line 13
    EXTI->IMR  |= (1 << 13);            // Unmask EXTI13
    EXTI->FTSR |= (1 << 13);            // Trigger on falling edge

    // Enable interrupt in NVIC
    NVIC_EnableIRQ(EXTI15_10_IRQn);
			

    while (1)
    {
					GPIOA->ODR |= (1<<0);	
			    delay(10000);
	        GPIOA->ODR &=~ (1<<0);
    }
		
}
