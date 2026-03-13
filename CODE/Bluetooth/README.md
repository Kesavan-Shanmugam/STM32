# Bluetooth Module Interfacing (Bare Metal)

This directory focuses on interfacing a UART-based Bluetooth module (e.g., HC-05) with the STM32F4x using bare-metal register-level programming.

## 1. `receive_command`
This project utilizes the `USART1` peripheral to receive specific ASCII control characters wirelessly from a Bluetooth-connected device (like a smartphone terminal app), parsing these commands to update text on a parallel LCD.

### How it works:
1. **USART1 and LCD Initialization**:
   - `GPIOB` is initialized for 8-bit parallel control of a 16x2/20x4 LCD display to show system status.
   - `GPIOA` pins PA9 (TX) and PA10 (RX) are configured as Alternate Function 7 (`AF7`).
   - `USART1` is configured with a baud rate of 9600 (`USART1->BRR = 0x0683`), matching the default baud rate of modules like the HC-05. Both RX and TX (`RE` and `TE`) are enabled.
2. **Receiving Data (`USART1_Receive`)**:
   - A blocking polling routine `while (!(USART1->SR & (1<<5)))` loops infinitely until the Bluetooth module pushes data resulting in the `RXNE` flag going high. It returns the captured byte.
3. **Command Processing**:
   - Inside the main `while(1)` loop, the program waits for a character to arrive via Bluetooth.
   - Series of `if...else if` statements act as a command interpreter:
     - If `'0'` is received: The LCD clears and displays "bluetooth off".
     - If `'1'` is received: The LCD clears and displays "bluetooth on".
     - If `'2'` is received: The LCD output updates to "HELLO WORLD".
     - Depending on the received ascii character, predefined string actions are executed to modify hardware outputs (in this case, the LCD screen). This forms the basis for wireless home automation or robot control.
