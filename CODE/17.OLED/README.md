# OLED Display Interface (Bare Metal)

This directory holds a bare-metal implementation for graphical OLED displays utilizing the SSD1306 controller via an I2C interface on the STM32F4x.

## 1. `OLED print`
This project demonstrates initializing the STM32's `I2C1` peripheral to communicate with the OLED's controller, sending a sequence of initialization commands, and directly writing a custom 8x8 font array into its screen buffer to display the string "HIKESAVAN[N]".

### How it works:
1. **I2C1 Initialization**:
   - The I2C interface is set up on pins PB6 (SCL) and PB7 (SDA) using Alternate Function 4 (`AF4`).
   - The clock control register (`CCR` = 210) sets standard mode 100kHz I2C communication.
2. **I2C Primitives**:
   - `I2C1_Start()`, `I2C1_Write()`, and `I2C1_Stop()` handle generating I2C bus conditions and data transfers to the display's `0x78` (shifted `0x3C`) slave address.
3. **OLED Control**:
   - `OLED_Command(cmd)` prepends the `0x00` control byte before the command, while `OLED_Data(data)` prepends `0x40` to indicate data bytes.
   - `OLED_Init()` sends the lengthy configuration sequence required by the SSD1306 (e.g., charge pump enable `0x8D 0x14`, setting display mode, pre-charge period, multiplex ratio, and finally display ON `0xAF`).
4. **Drawing to the Screen**:
   - A 1024-byte shadow buffer `buffer[]` represents the 128x8 byte-pages of the 128x64 display.
   - Hexadecimal byte arrays manually encoding pixel structures for specific letters (H, I, K, E, S, A, V, A, N) are written into the first page of the buffer.
   - The `OLED_Update()` function loops through all 8 pages (0-7), setting the column/page addresses, and dumping the data via I2C to refresh the physical screen.
