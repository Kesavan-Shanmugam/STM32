# Serial Peripheral Interface (SPI) (Bare Metal)

This directory provides a bare-metal SPI master implementation on the STM32F4x. 

## 1. `FULL_LETTER` / `SINGLE LETTER`
This project demonstrates configuring the `SPI1` peripheral in Master mode to transmit and receive string data (e.g., "HELLO") in a loop, while verifying the transaction by printing the sent and received data on a parallel character LCD.

### How it works:
1. **Clock & GPIO Configuration (`SPI1_Init`)**:
   - The GPIOA and SPI1 clocks are enabled.
   - Pins PA5 (SCK), PA6 (MISO), and PA7 (MOSI) are configured as Alternate Function 5 (`AF5`) which corresponds to SPI1.
2. **SPI Peripheral Configuration**:
   - `SPI1->CR1` is initialized.
   - The STM32 is configured as the SPI **Master** (`MSTR` bit set).
   - The **Baud Rate** is defined by setting the prescaler (e.g., `Fpclk/8`).
   - Software Slave Management (`SSM` and `SSI` bits) is enabled to bypass relying on the hardware NSS pin.
   - The peripheral is finally turned on (`SPE` bit set).
3. **Transmission & Reception (`SPI1_transmit`, `SPI1_Receive`)**:
   - To send a byte, the code waits for the Transmit Buffer Empty flag (`TXE`). It writes data to `SPI1->DR`, then blocks until the Busy flag (`BSY`) is cleared indicating the physical transmission is done.
   - To read a byte (because SPI is full-duplex), the code sends a dummy byte (`0xFF`) which forces the slave device to clock data back entirely. It then awaits the Receive Buffer Not Empty (`RXNE`) flag and reads `SPI1->DR`.
4. **Application Logic**:
   - In the `while(1)` loop, the program iterates over a string array, pushing bytes out via `SPI1_transmit()` and capturing the MISO reply via `SPI1_Receive()` into an `rx` buffer.
   - Standard LCD data/command functions (`LCD_String`, `COMMAND`) output the results to verify communication.
