#include <STM32F4xx.h>

#define SSD1306_ADDR  (0x3C << 1)

void I2C1_Init(void)
{
    // enable PORTB clock
    RCC->AHB1ENR |= (1 << 1);

    // enable I2C1 clock
    RCC->APB1ENR |= (1 << 21);

    // PB6 = SCL, PB7 = SDA (AF mode)
    GPIOB->MODER &= ~((3 << 12) | (3 << 14));
    GPIOB->MODER |=  ((2 << 12) | (2 << 14));

    // open drain
    GPIOB->OTYPER |= (1 << 6) | (1 << 7);

    // pull up
    GPIOB->PUPDR &= ~((3 << 12) | (3 << 14));
    GPIOB->PUPDR |=  ((1 << 12) | (1 << 14));

    // High speed
    GPIOB->OSPEEDR |= (3 << 12) | (3 << 14);

    // AF4 for PB6 and PB7
    GPIOB->AFR[0] &= ~((0xF << 24) | (0xF << 28));
    GPIOB->AFR[0] |=  ((4 << 24) | (4 << 28));

    // RESET I2C
    I2C1->CR1 |= (1 << 15);
    I2C1->CR1 &= ~(1 << 15);

    I2C1->CR1 &= ~(1 << 0);  // disable PE
    I2C1->CR2 = 42;      // APB1 = 42MHz
    I2C1->CCR = 210;     // 100kHz mode
    I2C1->TRISE = 43;

    I2C1->CR1 |= (1 << 0);  // enable PE
}

void I2C1_Start(uint8_t addr)
{
    I2C1->CR1 |= (1 << 8);               // START
    while (!(I2C1->SR1 & (1 << 0)));     // SB = 1
    I2C1->DR = addr;
    while (!(I2C1->SR1 & (1 << 1)));     // ADDR = 1
    volatile uint32_t t = I2C1->SR2;     // clear
}

void I2C1_Write(uint8_t data)
{
    while (!(I2C1->SR1 & (1 << 7)));     // TXE
    I2C1->DR = data;
}

void I2C1_Stop(void)
{
    while (!(I2C1->SR1 & (1 << 2)));     // BTF
    I2C1->CR1 |= (1 << 9);               // STOP
}

void OLED_Command(uint8_t cmd)
{
    I2C1_Start(SSD1306_ADDR);
    I2C1_Write(0x00);
    I2C1_Write(cmd);
    I2C1_Stop();
}

void OLED_Data(uint8_t data)
{
    I2C1_Start(SSD1306_ADDR);
    I2C1_Write(0x40);
    I2C1_Write(data);
    I2C1_Stop();
}

void OLED_Init(void)
{
    OLED_Command(0xAE);
    OLED_Command(0x20); OLED_Command(0x00);
    OLED_Command(0x40);
    OLED_Command(0xA1);
    OLED_Command(0xC8);
    OLED_Command(0xA6);
    OLED_Command(0xA8); OLED_Command(0x3F);
    OLED_Command(0xD3); OLED_Command(0x00);
    OLED_Command(0xD5); OLED_Command(0x80);
    OLED_Command(0xD9); OLED_Command(0x22);
    OLED_Command(0xDA); OLED_Command(0x12);
    OLED_Command(0xDB); OLED_Command(0x20);
    OLED_Command(0x8D); OLED_Command(0x14);
    OLED_Command(0xAF);
}

uint8_t buffer[1024];

void OLED_Clear(void)
{
    for(int i = 0; i < 1024; i++)
        buffer[i] = 0;
}

void OLED_Update(void)
{
    for(uint8_t page = 0; page < 8; page++)
    {
        OLED_Command(0xB0 + page);
        OLED_Command(0x00);
        OLED_Command(0x10);

        I2C1_Start(SSD1306_ADDR);
        I2C1_Write(0x40);

        for(int i = 0; i < 128; i++)
            I2C1_Write(buffer[page * 128 + i]);

        I2C1_Stop();
    }
}

int main(void)
{
    I2C1_Init();
    OLED_Init();
    OLED_Clear();
    
	  //FOR H
    buffer[0] = 0xFF;  
    buffer[1] = 0x08;
    buffer[2] = 0x08;
    buffer[3] = 0x08;
    buffer[4] = 0xFF;
	  buffer[5] = 0x00;
	  buffer[6] = 0x00;
	  //FOR I
	  buffer[7] = 0xFF;
	  buffer[8] = 0x00;
	  buffer[9] = 0x00;
		buffer[10] = 0x00;
	  buffer[11] = 0x00;
	  //FOR K
	  buffer[12] = 0xFF;  
	  buffer[13] = 0x08;  
	  buffer[14] = 0x14; 
	  buffer[15] = 0xF7;
		buffer[16] = 0x00;
	  buffer[17] = 0x00;	
		//FOR E
	  buffer[18] = 0xFF; 
    buffer[19] = 0x89;
    buffer[20] = 0x89; 
    buffer[21] = 0x89; 
    buffer[22] = 0x00;
	  buffer[23] = 0x00;
		//FOR S
    buffer[24] = 0x8F;
	  buffer[25] = 0x89;
    buffer[26] = 0x89;
	  buffer[27] = 0x89;	
	  buffer[28] = 0xF9;
    buffer[29] = 0x00;
	  buffer[30] = 0x00;
		//FOR A
	  buffer[31] = 0xF8;	
	  buffer[32] = 0x0C;
    buffer[33] = 0x0A;
	  buffer[34] = 0x09;	
	  buffer[35] = 0x0A;	
	  buffer[36] = 0x0C;
    buffer[37] = 0xF8;
	  buffer[38] = 0x00;	
    buffer[39] = 0x00;
	  //FOR V
	  buffer[40] = 0x0F;	
	  buffer[41] = 0x10;
    buffer[42] = 0x20;
	  buffer[43] = 0x40;	
	  buffer[44] = 0x20;	
	  buffer[45] = 0x10;
    buffer[46] = 0x0F;
	  buffer[47] = 0x00;	
    buffer[48] = 0x00;
	  //FOR A
	  buffer[49] = 0xF8;	
	  buffer[50] = 0x0C;
    buffer[51] = 0x0A;
	  buffer[52] = 0x09;	
	  buffer[53] = 0x0A;	
	  buffer[54] = 0x0C;
    buffer[55] = 0xF8;
	  buffer[56] = 0x00;	
	  buffer[57] = 0x00;
		//FOR N
	  buffer[58] = 0xFF;
    buffer[59] = 0x01;
	  buffer[60] = 0x02;	
	  buffer[61] = 0x04;	
	  buffer[62] = 0x08;
    buffer[63] = 0x10;
	  buffer[64] = 0x20;	
	  buffer[65] = 0x40;	
	  buffer[66] = 0x80;
		buffer[67] = 0xFF;

    OLED_Update();

    while (1);
}
