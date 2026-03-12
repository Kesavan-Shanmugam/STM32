# Inter-Integrated Circuit (I2C) (Bare Metal)

This directory provides a bare-metal I2C master implementation on the STM32F4x microcontroller to transmit data to an external slave device.

## 1. `full_word_trans` (and `Single_letter`)
The provided projects demonstrate how to configure the hardware I2C peripheral (`I2C1`) to act as a master transmitter, sending a string (e.g., "Hello") to a predefined slave address (like `0xA0`, such as an EEPROM), while simultaneously outputting the transmission status to a parallel LCD.

### How it works:
1. **Clock and GPIO Setup**:
   - `GPIOB` clock is enabled using `RCC->AHB1ENR`.
   - Pins PB8 (SCL) and PB9 (SDA) are configured to Alternate Function mode using `GPIOB->MODER`.
   - Their Alternate Function is set to `AF4` using the `AFR[1]` register to connect them to the `I2C1` peripheral.
   - Importantly, both pins are configured as Open-Drain (`OTYPER |= (1<<8) | (1<<9)`) with internal pull-up resistors enabled (`PUPDR`).
2. **I2C1 Initialization**:
   - `I2C1` is disabled first (`CR1 &= ~1`) to allow configuration.
   - The peripheral input clock frequency is set to 42 MHz (`CR2 = 42`).
   - The Clock Control Register (`CCR`) is set for Standard Mode (100kHz) by writing `210`. The rise time (`TRISE`) is configured as `43`.
   - The peripheral is then enabled (`CR1 |= 1`).
3. **Transmission Sequence**:
   - **Start**: Generates a START condition by setting the `START` bit in `CR1` and waiting for the `SB` flag in `SR1`.
   - **Address Phase**: Transmits the 8-bit slave address byte (`0xA0` - write mode) via the Data Register (`DR`) and waits for the Address sent acknowledged flag (`ADDR` in `SR1`).
   - **Data Phase**: A character string is transmitted byte-by-byte. The code waits for the `TXE` (Transmit Data Register Empty) flag before loading the next byte into `DR`, and waits for `BTF` (Byte Transfer Finished) at the very end.
   - **Stop**: Generates a STOP condition by setting the `STOP` bit in `CR1`.
