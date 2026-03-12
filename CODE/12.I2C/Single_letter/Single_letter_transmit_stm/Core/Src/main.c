#include "stm32f4xx.h"  

#define rs (1<<13)
#define en (1<<14)

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
    delay(5);
    GPIOB->ODR &= ~en;
    delay(5);
}

void DATA(uint8_t DA)
{
    GPIOB->ODR = (GPIOB->ODR & 0xFF00) | DA;
    GPIOB->ODR |= rs;
    GPIOB->ODR |= en;
    delay(5);
    GPIOB->ODR &= ~en;
    delay(5);
}

void LCD_INIT(void)
{
    RCC->AHB1ENR |= (1<<1);
    GPIOB->MODER &= ~0x3FFFF;
    GPIOB->MODER |=  0x15555;
    GPIOB->MODER |=  (1<<(13*2)) | (1<<(14*2));

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
        DATA(*str++);
}

void I2C1_Init(void)
{
    RCC->AHB1ENR |= (1<<1);
    RCC->APB1ENR |= (1<<21);

    GPIOB->MODER &= ~((3<<(8*2)) | (3<<(9*2)));
    GPIOB->MODER |=  ((2<<(8*2)) | (2<<(9*2)));
    GPIOB->OTYPER |= ((1<<8) | (1<<9));
    GPIOB->PUPDR &= ~((3<<(8*2)) | (3<<(9*2)));
    GPIOB->PUPDR |=  ((1<<(8*2)) | (1<<(9*2)));
    GPIOB->AFR[1] &= ~((0xF<<(0*4)) | (0xF<<(1*4)));
    GPIOB->AFR[1] |=  ((4<<(0*4)) | (4<<(1*4)));

    I2C1->CR1 &= ~(1<<0);
    I2C1->CR2 = 42;
    I2C1->CCR = 210;
    I2C1->TRISE = 43;
    I2C1->CR1 |= (1<<0);
}

void I2C1_Start(void)
{
    I2C1->CR1 |= (1<<8);
    while(!(I2C1->SR1 & (1<<0)));
}

void I2C1_Stop(void)
{
    I2C1->CR1 |= (1<<9);
}

void I2C1_WriteAddr(uint8_t addr)
{
    I2C1->DR = addr;
    while(!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;
}

void I2C1_WriteData(uint8_t data)
{
    while(!(I2C1->SR1 & (1<<7)));
    I2C1->DR = data;
    while(!(I2C1->SR1 & (1<<2)));
}

int main(void)
{
    uint8_t tx_data = 'A';

    LCD_INIT();
    I2C1_Init();

    COMMAND(0x01);
    COMMAND(0x80);
    LCD_String("TX: ");
    DATA(tx_data);

    // TRANSMIT
    I2C1_Start();
    I2C1_WriteAddr(0xA0);     // Slave address (Arduino)
    I2C1_WriteData(tx_data);
    I2C1_Stop();

    while(1);
}
