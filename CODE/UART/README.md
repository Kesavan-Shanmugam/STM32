# Universal Asynchronous Receiver-Transmitter (UART) (Bare Metal)

This directory features bare-metal code for configuring UART serial communication on the STM32F4x.

## 1. `Transmit_receive` / `full name`
This project demonstrates configuring `USART1` for bidirectional serial data transmission (TX/RX) and displaying the transferred data on an external parallel LCD attached to `GPIOB`.

### How it works:
1. **Clock & GPIO Configuration (`USART1_Init`)**:
   - The GPIOA clock (`AHB1ENR`) and USART1 clock (`APB2ENR`) are enabled.
   - Pins PA9 (TX) and PA10 (RX) are configured in alternate function mode (`AF7` for USART1).
2. **USART Configuration**:
   - **Baud Rate**: The Baud Rate Register (`USART1->BRR`) is configured to `0x0683`. This hexadecimal value sets the baud rate generator to exactly 9600 baud (assuming the APB2 clock is 16 MHz).
   - The Transmitter Enable (`TE`) and Receiver Enable (`RE`) bits are set in the `CR1` register.
   - The USART module is enabled globally (`UE` bit set).
3. **Data Handling**:
   - **Transmit**: The `USART1_Transmit(char c)` function waits for the Transmit Data Register Empty flag (`TXE` in `SR`) before loading the character `c` into `USART1->DR`. Strings are sent by looping through multiple characters.
   - **Receive**: The `USART1_Receive()` function blocks and waits for the Read Data Register Not Empty flag (`RXNE`). Once set, it reads the byte from the Data Register.
4. **Main Routine**:
   - In an infinite loop, the code continuously transmits a character (e.g., 'A'), polls the receive buffer to get an echo or reply, and pushes the text to the character LCD using specific Command (`0x80`, `0xC0`) and Data display functions.
