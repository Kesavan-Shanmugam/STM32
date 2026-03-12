#include "stm32f4xx.h"

void delay_ms(uint32_t ms)
{
    for(uint32_t i=0;i<ms*4000;i++)
        __NOP();
}


void I2C1_Init(void)
{
    RCC->AHB1ENR |= (1<<1);    // GPIOB clock
    RCC->APB1ENR |= (1<<21);   // I2C1 clock

    // PB8 & PB9 AF mode
    GPIOB->MODER |= (2<<(8*2)) | (2<<(9*2));
    GPIOB->AFR[1] |= (4<<(0*4)) | (4<<(1*4));   // AF4
    GPIOB->OTYPER |= (1<<8) | (1<<9);
    GPIOB->OSPEEDR |= (3<<(8*2)) | (3<<(9*2));

    I2C1->CR2 = 16;     //APB clock = 16MHz
    I2C1->CCR = 80;     //100KHz I2C speed set
    I2C1->TRISE = 17;   //Rise time setting
    I2C1->CR1 |= 1;     //I2C enable
}


void I2C_Write(uint8_t addr, uint8_t data)
{
    I2C1->CR1 |= (1<<8);          //START condition        
    while(!(I2C1->SR1 & 1));      //Start bit sent nu wait       

    I2C1->DR = addr<<1;           //Slave address send         
    while(!(I2C1->SR1 & (1<<1))); //Address acknowledged nu wait
    (void)I2C1->SR2;              //Clear flag

    I2C1->DR = data;              //Data send      
    while(!(I2C1->SR1 & (1<<7))); //Data transmitted nu wait

    I2C1->CR1 |= (1<<9);          //STOP condition      
} 



#define LCD_ADDR 0x27 //LCD I2C address
#define EN  0x04      //enable
#define RS  0x01      //register select
#define BL  0x08      // Backlight

void LCD_Send4Bit(uint8_t data)           //4 bit LCD data send
{
    I2C_Write(LCD_ADDR, data | BL);       //Backlight ON with data
    I2C_Write(LCD_ADDR, data | EN | BL);  //Enable HIGH
    delay_ms(1);
    I2C_Write(LCD_ADDR, data | BL);       //Enable LOW
}

void LCD_SendCmd(uint8_t cmd)
{
    LCD_Send4Bit(cmd & 0xF0);             //Upper nibble
    LCD_Send4Bit((cmd<<4) & 0xF0);        //Lower nibble
}

void LCD_SendData(uint8_t data)
{
    LCD_Send4Bit((data & 0xF0) | RS);      //Same logic but with RS = 1
    LCD_Send4Bit(((data<<4)&0xF0) | RS);
}

void LCD_Init(void)
{
    delay_ms(50);

    LCD_Send4Bit(0x30);
    delay_ms(5);
    LCD_Send4Bit(0x30);
    delay_ms(5);
    LCD_Send4Bit(0x30);
    delay_ms(5);

    LCD_Send4Bit(0x20);   // 4-bit mode

    LCD_SendCmd(0x28);   // 4bit,2line
    LCD_SendCmd(0x0C);   // display ON
    LCD_SendCmd(0x01);   // clear
    delay_ms(5);
}

void LCD_Print(char *str)
{
    while(*str)
        LCD_SendData(*str++);
}



int main(void)
{
    I2C1_Init();
    LCD_Init();

    LCD_Print("HELLO");

    while(1);
}
