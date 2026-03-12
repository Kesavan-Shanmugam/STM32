#include <STM32f4xx.h>

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

void LCD_SendString(char *str)
{
    while(*str)
        DATA(*str++);
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
	  char r = USART1->DR;
    return r;
}

void wait(char w)
{
    char C;
    do{
			C= USART1_Receive();
    }while (C!= w);
}

int main(void)
{
    USART1_Init();
    LCD_INIT();
	
	  COMMAND(0x80);
    LCD_SendString("waiting GPS...");
    char lat[20];
    char lon[20];


        wait('G');
        wait('P');
        wait('R');
        wait('M');
        wait('C');
        wait(',');
        wait(',');
        wait('A');
        wait(',');

        int i=0;
        char c;
        for(i=0; ;i++)
        {
            char data = USART1_Receive();

            if(data == ',')
            {
                break;
            }
            
            lat[i] = data;
        }


        COMMAND(0x01);
        COMMAND(0x80);
        LCD_SendString("lat:");
        LCD_SendString(lat); 

        wait('N');
        wait(',');

        for(i=0; ;i++)
        {
            char data = USART1_Receive();

            if(data == ',')
            {
                break;
            }
            
            lon[i] = data;
        }
				
        COMMAND(0xC0);
        LCD_SendString("lon:");
        LCD_SendString(lon);
        
    
}