#include "stm32f4xx.h"

#define R1 (1<<0)   // PC0 - Row1
#define R2 (1<<1)   // PC1 - Row2
#define R3 (1<<2)   // PC2 - ROW3
#define R4 (1<<3)   // PC3 - ROW4
#define C1 (1<<4)   // PC0 - COL1
#define C2 (1<<5)   // PC1 - COL2
#define C3 (1<<6)   // PC2 - COL3
#define C4 (1<<7)   // PC3 - COL4

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

void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1<<1) | (1<<2);      // Enable GPIOB, GPIOC clock

    GPIOB->MODER &= ~(0xFFFFF);    
    GPIOB->MODER |=  (0x55555);           // PB0-PB9 output

    GPIOC->MODER &= ~(0xFFFF);            // Clear PC0–PC7 mode
    GPIOC->MODER |=  (0x0055);            // PC0 TO PC3 output; PC4 TO PC7 input
    GPIOC->PUPDR &= ~(0xFFFF);           
    GPIOC->PUPDR |=  (0x5555);            // Pull-up on inputs

}

int main(void)
{
    GPIO_Init(); 
    LCD_INIT(); 	

	  command(0x01);
    command(0x80);
    data('H');
    data('I');

    while(1)
    {

			  //FIRST ROW 
        GPIOC->ODR &= ~R1;  
        GPIOC->ODR |=  R2;
		   	GPIOC->ODR |=  R3;
			  GPIOC->ODR |=  R4;

        if(!(GPIOC->IDR & C1))               //1 ROW, 1 COLUMN 
        {    
					  command(0x01);
            command(0x80);
            data('1');
        }
        else if(!(GPIOC->IDR & C2))           //1 ROW, 2 COLUMN 
        {
            command(0x80);
            data('2');
        }
				else if(!(GPIOC->IDR & C3))           //1 ROW, 3 COLUMN 
        {
            command(0x80);
            data('3');
        }
				else if(!(GPIOC->IDR & C4))           //1 ROW, 4 COLUMN 
        {
            command(0x80);
            data('A');
        }
        delay(50);
				
				
				//SECOND ROW 
				GPIOC->ODR &= ~R2;  
        GPIOC->ODR |=  R1;
		   	GPIOC->ODR |=  R3;
			  GPIOC->ODR |=  R4;

        if(!(GPIOC->IDR & C1))                 //2 ROW, 1 COLUMN 
        {    
           command(0x80);
            data('4');
        }
        else if(!(GPIOC->IDR & C2))            //2 ROW, 2 COLUMN 
        {
            command(0x80);
            data('5');
        }
				else if(!(GPIOC->IDR & C3))            //2 ROW, 3 COLUMN 
        {
            command(0x80);
            data('6');
        }
				else if(!(GPIOC->IDR & C4))            //2 ROW, 4 COLUMN 
        {
            command(0x80);
            data('B');
        }
        delay(50);
				
				
				//THIRD ROW
				GPIOC->ODR &= ~R3;  
        GPIOC->ODR |=  R1;
		   	GPIOC->ODR |=  R2;
			  GPIOC->ODR |=  R4;

        if(!(GPIOC->IDR & C1))                 //3 ROW, 1 COLUMN 
        {    
           command(0x80);
            data('7');
        }
        else if(!(GPIOC->IDR & C2))            //3 ROW, 2 COLUMN 
        {
            command(0x80);
            data('8');
        }
				else if(!(GPIOC->IDR & C3))            //3 ROW, 3 COLUMN 
        {
            command(0x80);
            data('9');
        }
				else if(!(GPIOC->IDR & C4))            //3 ROW, 4 COLUMN 
        {
            command(0x80);
            data('C');
        }
        delay(50);
				
				
				//FOUTH ROW
				GPIOC->ODR &= ~R4;  
        GPIOC->ODR |=  R1;
		   	GPIOC->ODR |=  R2;
			  GPIOC->ODR |=  R3;

        if(!(GPIOC->IDR & C1))                 //4 ROW, 1 COLUMN 
        {    
           command(0x80);
            data('*');
        }
        else if(!(GPIOC->IDR & C2))            //4 ROW, 2 COLUMN 
        {
            command(0x80);
            data('0');
        }
				else if(!(GPIOC->IDR & C3))            //4 ROW, 3 COLUMN 
        {
            command(0x80);
            data('#');
        }
				else if(!(GPIOC->IDR & C4))            //4 ROW, 4 COLUMN 
        {
            command(0x80);
            data('D');
        }
        delay(50);
    }
}
