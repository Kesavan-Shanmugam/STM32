# GPS Interfacing (Bare Metal)

This directory demonstrates interfacing a standard serial GPS receiver (e.g., NEO-6M) with the STM32F4x microcontroller using the USART peripheral.

## 1. `GPS address data receive`
This project receives NMEA sentences from the GPS module over USART1, parses the string to extract Latitude and Longitude directly, and displays them on a parallel 16x2 character LCD.

### How it works:
1. **USART1 Configuration**:
   - `USART1` uses pins `PA9` (TX) and `PA10` (RX) configured for Alternate Function 7 (`AF7`).
   - The baud rate divisor (`USART1->BRR = 0x0683`) is set for 9600 baud assuming a 16MHz system clock.
   - Transmission (`TE`), Reception (`RE`), and the USART module (`UE`) are enabled in the `CR1` register.
   - The `USART1_Receive()` function uses a polling method (`while (!(USART1->SR & (1<<5)))`) to wait for the Receive Data Register Not Empty (`RXNE`) flag before reading the byte.
2. **NMEA Parsing (`wait()` and loops)**:
   - The code specifically looks for the `$GPRMC` NMEA sentence. It sequentially waits for the characters `'G', 'P', 'R', 'M', 'C'` using the custom blocking `wait()` function.
   - After finding the header and skipping commas indicating time and status markers, it reads characters continuously into a `lat` string array until it hits the next comma.
   - It then waits for the North/South indicator (`'N'`) and skips to the next comma to extract the Longitude string (`lon`) in the same manner.
3. **LCD Display**:
   - `GPIOB` is initialized sequentially (using PB0-PB7 as Data, PB8 as RS, PB9 as EN).
   - Once the Latitude and Longitude strings are parsed, standard LCD commands (`0x80`, `0xC0`) position the cursor, and the strings are iteratively sent to the display.
