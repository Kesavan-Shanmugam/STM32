#include "stm32f4xx.h"

#define rs (1<<8)
#define en (1<<9)

void delay(uint32_t ms)
{
    for(uint32_t i=0; i<ms*5000; i++)
        __NOP();
}


void COMMAND(uint8_t CMD)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | CMD;
    GPIOB->ODR &= ~rs;
    GPIOB->ODR |= en;
    delay(2);
    GPIOB->ODR &= ~en;
    delay(2);
}

void DATA(uint8_t DA)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | DA;
    GPIOB->ODR |= rs;
    GPIOB->ODR |= en;
    delay(2);
    GPIOB->ODR &= ~en;
    delay(2);
}

void LCD_INIT(void)
{
    RCC->AHB1ENR |= (1<<1);        // GPIOB clock
    GPIOB->MODER &= ~(0xFFFFF);    
    GPIOB->MODER |=  (0x55555);    // PB0-PB9 output

    delay(20);
    COMMAND(0x38);    // 8-bit, 2 line, 5x7 font
    delay(5);
    COMMAND(0x0C);    // Display ON, cursor OFF
    delay(5);
    COMMAND(0x06);    // Increment cursor
    delay(5);
    COMMAND(0x01);    // Clear display
    delay(10);
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

void USART1_Transmit(char c)
{
    while (!(USART1->SR & (1<<7)));  // Wait TXE
    USART1->DR = c;
}

char USART1_Receive(void)
{
    while (!(USART1->SR & (1<<5)));  // Wait RXNE
    return USART1->DR;
}

void USART1_SendString(char *str)
{
    while(*str)
        USART1_Transmit(*str++);
}


void LCD_SendString(char *str)
{
    while(*str)
        DATA(*str++);
}


int main(void)
{
    LCD_INIT();
    USART1_Init();

    char tx[] = "A";   // Transmit char
    char rx;

    while(1)
    {
        // 1. Transmit
        USART1_SendString(tx);

        // 2. Receive
        rx = USART1_Receive();

        // 3. Display on LCD
        COMMAND(0x01);     // Clear LCD
        delay(5);

        // Line 1: TX
        COMMAND(0x80);
        LCD_SendString("TX: ");
        DATA(tx[0]);

        // Line 2: RX
        COMMAND(0xC0);
        LCD_SendString("RX: ");
        DATA(rx);

        delay(500);
    }
}
