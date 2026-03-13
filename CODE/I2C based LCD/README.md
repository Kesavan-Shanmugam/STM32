# I2C-Based Character LCD (Bare Metal)

This directory provides a bare-metal driver for alphanumeric LCD modules that utilize an I2C backpack (typically featuring a PCF8574 I/O expander). 

## 1. `I2C_LCD`
By using the STM32's `I2C1` peripheral to communicate with the backpack, this implementation severely reduces the number of pins required to control standard 16x2 or 20x4 LCDs down to just SCL (PB8) and SDA (PB9).

### How it works:
1. **I2C1 Initialization**:
   - Pins PB8 and PB9 are configured for `AF4` (Alternate Function, I2C1), Open-Drain mode.
   - The I2C Clock Control Register (`CCR`) and Rise Time (`TRISE`) are set for Standard Mode (100 kHz) assuming an APB1 clock of 16 MHz.
2. **I2C Write Abstraction (`I2C_Write`)**:
   - This function abstracts a complete I2C frame transmission.
   - It generates a START condition, transmits the I2C Slave Address (e.g., `0x27 << 1`), waits for ACK, transmits the 8-bit data payload containing the display commands/data, and finally generates a STOP condition.
3. **4-Bit LCD Control Over I2C**:
   - An I2C backpack maps its 8 output pins to the LCD's RS, RW, EN, Backlight, and Data (D4-D7) pins.
   - `LCD_Send4Bit()` takes a nibble (4 bits) of data. It performs three I2C writes to strobe the LCD's `EN` pin:
     - 1st Write: Data + Backlight Bit
     - 2nd Write: Data + Enable Bit + Backlight Bit
     - 3rd Write: Data + Backlight Bit (Enable pulled low again)
4. **Initialization and Printing**:
   - The `LCD_Init()` function sends the standard Hitachi HD44780 initialization sequence (`0x30`, `0x20` to enter 4-bit mode, then `0x28` for 2 lines, `0x0C` for display ON, `0x01` to clear).
   - `LCD_Print()` pushes strings character-by-character, asserting the RS bit high during the I2C transfers so the LCD interprets incoming nibbles as characters instead of commands.
