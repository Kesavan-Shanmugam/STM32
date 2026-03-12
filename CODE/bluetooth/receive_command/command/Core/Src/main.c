#include "stm32f4xx.h"

#define RS (1<<8)
#define EN (1<<9)

void delay(uint32_t ms)
{
    for(uint32_t i = 0; i < ms * 16000; i++)
        __NOP();
}

void command(char cmd)
{
    GPIOB->ODR = (GPIOB->ODR & 0XFF00)|cmd;
    GPIOB->ODR &=~ RS;
    GPIOB->ODR |= EN;
    delay(10);
    GPIOB->ODR &=~ EN;
    delay(10);
}

void data(char da)
{
    GPIOB->ODR = (GPIOB->ODR & 0XFF00)|da;
    GPIOB->ODR |= RS;
    GPIOB->ODR |= EN;
    delay(10);
    GPIOB->ODR &=~ EN;
    delay(10);
}

void LCD_INIT(void)
{

    RCC->AHB1ENR |= (1<<1);        // GPIOB clock
    GPIOB->MODER &= ~(0xFFFFF);    
    GPIOB->MODER |=  (0x55555);    // PB0-PB9 output

    delay(20);
    command(0x38);    // 8-bit, 2 line, 5x7 font
    delay(5);
    command(0x0C);    // Display ON, cursor OFF
    delay(5);
    command(0x06);    // Increment cursor
    delay(5);
    command(0x01);    // Clear display
    delay(10);
}

void LCD_SendString(char *str)
{
    while(*str)
        data(*str++);
}

void USART1_Init(void)
{
    RCC->AHB1ENR |= (1<<0);      // GPIOA clock
    RCC->APB2ENR |= (1<<4);      // USART1 clock

    // PA9=TX, PA10=RX ? AF7
    GPIOA->MODER &= ~((3<<(9*2)) | (3<<(10*2)));
    GPIOA->MODER |=  ((2<<(9*2)) | (2<<(10*2))); // AF mode

    GPIOA->AFR[1] &= ~((0xF<<(4*(9-8))) | (0xF<<(4*(10-8))));
    GPIOA->AFR[1] |=  ((7<<(4*(9-8))) | (7<<(4*(10-8)))); // AF7

    // USART1 config
    USART1->BRR = 0x0683;        // 9600 baud @16MHz
    USART1->CR1 |= (1<<3);       // TE
    USART1->CR1 |= (1<<2);       // RE
    USART1->CR1 |= (1<<13);      // UE
}

char USART1_Receive(void)
{
		
    while (!(USART1->SR & (1<<5)));  // Wait RXNE	
    return USART1->DR;
}


int main(void)
{
    char rx;
    LCD_INIT();
    USART1_Init();
	
   command(0x80);
    LCD_SendString("waiting...");
   
    while(1)
    {
			rx=USART1_Receive();
        if (rx=='0')
        {
            command(0x01);
            command(0x80);
            LCD_SendString("bluetooth off");
        }

        else if(rx=='1')
        {
            command(0x01);
            command(0x80);
            LCD_SendString("bluetooth on");
        }
        
        else if(rx=='2')
        {
            command(0x01);
            command(0x80);
            LCD_SendString("HELLO WOLRD");
        }

        else if(rx=='3')
        {
            command(0x01);
            command(0x80);
            LCD_SendString("HI KESAVAN");
        }

        else if(rx=='4')
        {
            command(0x01);
            command(0x80);
            LCD_SendString("IM WORKING");
        }
    }


}