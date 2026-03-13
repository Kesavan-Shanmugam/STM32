
#include<stm32f4xx.h>
 void delay(uint32_t ms)
 {
    for(uint32_t i=1; i<ms*1600; i++)
    {
        __NOP();
    }
 }

 int main()
 {
    RCC->AHB1ENR |= (0X02);
    GPIOB->MODER &=~ (0xffff);
    GPIOB->MODER |= (0X5555);

    while(1)
    {
        int a[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7F, 0x6f};
        for(uint32_t i=0; i<10; i++)
        {
            GPIOB->ODR = a[i];
            delay(10000);
			
       }
    }
 }
