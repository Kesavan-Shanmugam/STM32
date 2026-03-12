#include <stm32f4xx.h>

#define rs (1<<8)
#define en (1<<9)

void delay(uint32_t ms)
{ 
    for(uint32_t i=0; i<ms*1600; i++)
        __NOP();
}

void COMMAND(uint8_t CMD)
{
    GPIOB->ODR = (GPIOB->ODR & (0xFF00)) | CMD;
    GPIOB->ODR &= ~rs;
    GPIOB->ODR |= en;
    delay(10);
    GPIOB->ODR &= ~en;
    delay(20);
}

void DATA(uint8_t DA)
{
    GPIOB->ODR = (GPIOB->ODR & (0xFF00)) | DA;
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

int main(void)
{
    
    LCD_INIT();
   char D[] = "kesavan";
    while(1)
    {
        COMMAND(0x80);           // move cursor to line 1
        for(int i=0; D[i]!='\0'; i++)
        {
            DATA(D[i]);
        }
        while(1); // stop after printing
    }
}

