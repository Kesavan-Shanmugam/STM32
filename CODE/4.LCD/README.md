# Parallel Character LCD (Bare Metal)

This directory provides a bare-metal implementation for standard (parallel interface) 16x2/20x4 alphanumeric LCDs (using the HD44780 controller) directly attached to the STM32F4x GPIOs.

## 1. `Name print`
This project demonstrates configuring basic GPIO output pins to interface with the LCD's 8-bit data bus (D0-D7) and control pins (RS, EN), along with initialization sequences and writing strings.

### How it works:
1. **GPIO Setup (`LCD_INIT`)**:
   - The `GPIOB` peripheral clock is enabled via `RCC->AHB1ENR`.
   - The lower 8 pins (`PB0` to `PB7`) are configured in general-purpose output mode to act as the 8-bit data bus.
   - Pins `PB8` and `PB9` are configured as output pins to act as the `RS` (Register Select) and `EN` (Enable) control lines, respectively. (RW is assumed to be hardwired to Ground).
2. **Command Function (`COMMAND()`)**:
   - The function masks the lower 8 bits of `GPIOB->ODR` and loads the 8-bit instruction byte.
   - The `RS` pin (PB8) is pulled low (`&= ~(1<<8)`) indicating an instruction.
   - The `EN` pin (PB9) is toggled high then low with delays in between, creating a pulse that tells the LCD to latch the data present on the data pins.
3. **Data Function (`DATA()`)**:
   - Similar to the Command function, the 8-bit character ASCII data is loaded into `GPIOB->ODR` (PB0-PB7).
   - The `RS` pin is pulled high (`|= (1<<8)`) to indicate character data.
   - The `EN` pin is toggled high then low to latch the character.
4. **Execution Flow**:
   - The initialization routine sends specific commands to set up the LCD (e.g., `0x38` for 8-bit/2-line mode, `0x0C` to turn the display on).
   - In the `while(1)` loop, the command `0x80` is used to move the cursor to the first position, and a `for` loop iterates through the string (e.g., "kesavan"), pushing each byte out using `DATA()`.
