#include "stm32f4xx.h"

#define LCD_ADDR 0x27
#define EN  0x04
#define RS  0x01
#define BL  0x08


void delay_ms(uint32_t ms)
{
    for(uint32_t i=0;i<ms*3000;i++) __NOP();
}


void GPIO_INIT(void)
{
    RCC->AHB1ENR |= (1<<0) | (1<<1);
    RCC->APB1ENR |= (1<<21) | (1<<25);

    /* I2C1 PB8 PB9 */
    GPIOB->MODER &= ~((3<<(8*2)) | (3<<(9*2)));
    GPIOB->MODER |=  ((2<<(8*2)) | (2<<(9*2)));

    GPIOB->AFR[1] &= ~((0xF<<0) | (0xF<<4));
    GPIOB->AFR[1] |=  ((4<<0) | (4<<4));

    GPIOB->OTYPER |= (1<<8) | (1<<9);
    GPIOB->OSPEEDR |= (3<<(8*2)) | (3<<(9*2));

    /* CAN PA11 PA12 */
    GPIOA->MODER &= ~((3<<(11*2)) | (3<<(12*2)));
    GPIOA->MODER |=  ((2<<(11*2)) | (2<<(12*2)));

    GPIOA->AFR[1] &= ~((0xF<<12) | (0xF<<16));
    GPIOA->AFR[1] |=  ((9<<12) | (9<<16));
}


void I2C1_Init(void)
{
    I2C1->CR1 &= ~1;
    I2C1->CR2 = 16;
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= 1;
}


void I2C_Write(uint8_t addr, uint8_t data)
{
    I2C1->CR1 |= (1<<8);
    while(!(I2C1->SR1 & 1));

    I2C1->DR = addr<<1;
    while(!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;

    I2C1->DR = data;
    while(!(I2C1->SR1 & (1<<7)));

    I2C1->CR1 |= (1<<9);
}


void LCD_Send4Bit(uint8_t data)
{
    I2C_Write(LCD_ADDR, data | BL);
    I2C_Write(LCD_ADDR, data | EN | BL);
    delay_ms(1);
    I2C_Write(LCD_ADDR, data | BL);
}

void LCD_SendCmd(uint8_t cmd)
{
    LCD_Send4Bit(cmd & 0xF0);
    LCD_Send4Bit((cmd<<4) & 0xF0);
}

void LCD_SendData(uint8_t data)
{
    LCD_Send4Bit((data & 0xF0) | RS);
    LCD_Send4Bit(((data<<4)&0xF0) | RS);
}

void LCD_Print(char *s)
{
    while(*s) LCD_SendData(*s++);
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
    LCD_Send4Bit(0x20);

    LCD_SendCmd(0x28);
    LCD_SendCmd(0x0C);
    LCD_SendCmd(0x01);
    delay_ms(5);
}


void CAN_Init(void)
{
    CAN1->MCR |= 1;
    while(!(CAN1->MSR & 1));

    CAN1->BTR = (2<<24) | (12<<16) | (1<<20) | 3;

    CAN1->FMR |= 1;
    CAN1->FA1R |= 1;
    CAN1->FS1R |= 1;
    CAN1->sFilterRegister[0].FR1 = 0;
    CAN1->sFilterRegister[0].FR2 = 0;
    CAN1->FMR &= ~1;

    CAN1->MCR &= ~1;
    while(CAN1->MSR & 1);
}


void CAN_Send_Byte(char c)
{
    while(!(CAN1->TSR & (1<<26)));

    CAN1->sTxMailBox[0].TIR = (0x123<<21);
    CAN1->sTxMailBox[0].TDTR = 1;
    CAN1->sTxMailBox[0].TDLR = c;
    CAN1->sTxMailBox[0].TIR |= 1;
}


char CAN_Receive_Byte(void)
{
    while(!(CAN1->RF0R & 3));
    char d = CAN1->sFIFOMailBox[0].RDLR & 0xFF;
    CAN1->RF0R |= (1<<5);
    return d;
}


int main(void)
{
    char rx;
    char tx[] = "HELLO";

    GPIO_INIT();
    I2C1_Init();
    LCD_Init();
    CAN_Init();

    LCD_SendCmd(0x80);
    LCD_Print("Sent:");
    LCD_Print(tx);

    LCD_SendCmd(0xC0);
    LCD_Print("Recv:");

    while(1)
    {
        CAN_Send_Byte(tx[0]);
        rx = CAN_Receive_Byte();
        LCD_SendData(rx);
        delay_ms(500);
    }
}
