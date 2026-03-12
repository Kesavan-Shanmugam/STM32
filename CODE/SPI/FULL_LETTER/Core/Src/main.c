#include "stm32f4xx.h"

#define rs (1<<8)
#define en (1<<9)

void delay(uint32_t ms)
{
    for(uint32_t i=0; i<ms*1600; i++)
        __NOP();
}

void COMMAND(uint8_t CMD)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | CMD;
    GPIOB->ODR &= ~rs;
    GPIOB->ODR |= en;
    delay(10);
    GPIOB->ODR &= ~en;
    delay(20);
}

void DATA(uint8_t DA)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | DA;
    GPIOB->ODR |= rs;
    GPIOB->ODR |= en;
    delay(10);
    GPIOB->ODR &= ~en;
    delay(20);
}

void LCD_INIT(void)
{
    RCC->AHB1ENR |= (1<<1);          // Enable GPIOB clock
    GPIOB->MODER &= ~(0xFFFFF);      // Clear PB0–PB9
    GPIOB->MODER |=  (0x55555);      // Set PB0–PB9 as output

    COMMAND(0x38);
    delay(5);
    COMMAND(0x0C);
    delay(5);
    COMMAND(0x06);
    delay(5);
    COMMAND(0x01);
    delay(5);
}

void LCD_String(char *str)
{
    while(*str)
    {
        DATA(*str++);
    }
}

void SPI1_Init(void)
{
    RCC->AHB1ENR |= (1<<0);                            // Enable GPIOA clock
    RCC->APB2ENR |= (1<<12);                           // Enable SPI1 clock

    // PA5=SCK, PA6=MISO, PA7=MOSI ? AF5
    GPIOA->MODER &= ~((3<<(5*2)) | (3<<(6*2)) | (3<<(7*2)));
    GPIOA->MODER |=  (2<<(5*2)) | (2<<(6*2)) | (2<<(7*2));
    GPIOA->AFR[0]  |= (5<<(5*4)) | (5<<(6*4)) | (5<<(7*4));

    SPI1->CR1 = 0;
    SPI1->CR1 |= (1<<2);                               // Master mode
    SPI1->CR1 |= (3<<3);                               // Baud rate = Fpclk/8
    SPI1->CR1 |= (1<<8) | (1<<9);                      // Software slave management
    SPI1->CR1 |= (1<<6);                               // Enable SPI1
}

void SPI1_transmit(uint8_t data)
{
    while(!(SPI1->SR & (1<<1)));                       // Wait until TXE = 1
    SPI1->DR = data;
    while(SPI1->SR & (1<<7));                          // Wait while BSY = 1
}

uint8_t SPI1_Receive(void)
{
    SPI1_transmit(0xFF);                                   // Dummy write
    while(!(SPI1->SR & (1<<0)));                       // Wait RXNE = 1
    return SPI1->DR;
}


int main(void)
{
    LCD_INIT();
    SPI1_Init();

    char tx[] = "HELLO";
    char rx[10];
    int i;

    // --- Print TX: and RX: headers ---
    
    COMMAND(0x80);   // Line 1
    LCD_String("TX: ");
    COMMAND(0xC0);   // Line 2
    LCD_String("RX: ");

    while(1)
    {
        // Transmit “HELLO” and receive it back
        for(i=0; tx[i] != '\0'; i++)
        {
            SPI1_transmit(tx[i]);      // Send byte
            rx[i] = SPI1_Receive();    // Receive byte
            delay(10);
        }
        rx[i] = '\0'; // Null-terminate

				delay(3000);
        // Display TX string
        COMMAND(0x83);
        LCD_String(tx);
				delay(3000);

        // Display RX string
        COMMAND(0xC3);
        LCD_String(rx);

        delay(3000);
        COMMAND(0x01);
        delay(10);
        COMMAND(0x80);
        LCD_String("TX: ");
        COMMAND(0xC0);
        LCD_String("RX: ");
    }
}