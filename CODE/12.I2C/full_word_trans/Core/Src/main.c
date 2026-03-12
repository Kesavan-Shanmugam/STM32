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
    RCC->AHB1ENR |= (1<<1);          // Enable GPIOB clock
    GPIOB->MODER &= ~0x3FFFF;        // Clear PB0–PB7
    GPIOB->MODER |=  0x15555;        // PB0–PB7 output
    GPIOB->MODER |=  (1<<(13*2)) | (1<<(14*2)); // PB13, PB14 output

    COMMAND(0x38); delay(5);   // 8-bit mode
    COMMAND(0x0C); delay(5);   // Display ON
    COMMAND(0x06); delay(5);   // Entry mode
    COMMAND(0x01); delay(5);   // Clear
}

void LCD_String(char *str)
{
    while(*str)
        DATA(*str++);
}

void I2C1_Init(void)
{
    RCC->AHB1ENR |= (1<<1);     // Enable GPIOB clock
    RCC->APB1ENR |= (1<<21);    // Enable I2C1 clock

    // PB8 -> SCL, PB9 -> SDA
    GPIOB->MODER &= ~((3<<(8*2)) | (3<<(9*2)));
    GPIOB->MODER |=  ((2<<(8*2)) | (2<<(9*2)));    // Alternate function
    GPIOB->OTYPER |= ((1<<8) | (1<<9));            // Open-drain
    GPIOB->PUPDR &= ~((3<<(8*2)) | (3<<(9*2)));
    GPIOB->PUPDR |=  ((1<<(8*2)) | (1<<(9*2)));    // Pull-up
    GPIOB->AFR[1] &= ~((0xF<<(0*4)) | (0xF<<(1*4)));
    GPIOB->AFR[1] |=  ((4<<(0*4)) | (4<<(1*4)));   // AF4 = I2C1

    I2C1->CR1 &= ~(1<<0);  // Disable I2C before config
    I2C1->CR2 = 42;        // APB1 = 42 MHz
    I2C1->CCR = 210;       // Standard mode 100kHz
    I2C1->TRISE = 43;      // Rise time
    I2C1->CR1 |= (1<<0);   // Enable I2C
}

void I2C1_Start(void)
{
    I2C1->CR1 |= (1<<8);               // START
    while(!(I2C1->SR1 & (1<<0)));      // Wait for SB bit
}

void I2C1_Stop(void)
{
    I2C1->CR1 |= (1<<9);               // STOP
}

void I2C1_WriteAddr(uint8_t addr)
{
    I2C1->DR = addr;                   // Send address
    while(!(I2C1->SR1 & (1<<1)));      // Wait for ADDR bit
    (void)I2C1->SR2;                   // Clear ADDR flag
}

void I2C1_WriteData(uint8_t data)
{
    while(!(I2C1->SR1 & (1<<7)));      // Wait for TXE
    I2C1->DR = data;                   // Send data
    while(!(I2C1->SR1 & (1<<2)));      // Wait for BTF
}

int main(void)
{
    char message[] = "Hello";

    LCD_INIT();
    I2C1_Init();

    // Display transmitted text on STM32 LCD
    COMMAND(0x01);
    COMMAND(0x80);
    LCD_String("TX: ");
    LCD_String(message);

    // I2C Transmit sequence
    I2C1_Start();
    I2C1_WriteAddr(0xA0);    // 0x50 << 1 (write mode)
    for (int i = 0; message[i] != '\0'; i++) {
        I2C1_WriteData(message[i]);
    }
    I2C1_Stop();

    while(1);
}
