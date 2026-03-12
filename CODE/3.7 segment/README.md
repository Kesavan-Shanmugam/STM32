# 7-Segment Display (Bare Metal)

This directory demonstrates interfacing bare-metal STM32F4x GPIOs with common-cathode 7-segment displays.

## 1. `number print` (Single Digit)
This project displays the numbers 0 through 9 sequentially on a single 7-segment display.
### How it works:
1. **Clock Enable**: The GPIOB peripheral clock is enabled via `RCC->AHB1ENR`.
2. **Pin Configuration**: The lower pins of `GPIOB` (pins 0 to 7) are configured as outputs using the `GPIOB->MODER` register. These pins connect to the a-g and DP segments of the display.
3. **Digit Encoding Table**: An array `a[]` contains the hexadecimal values required to light up the appropriate segments to form the digits 0 to 9 on a common-cathode display (e.g., `0x3f` for '0', `0x06` for '1').
4. **Output Generation**: A loop iterates from 0 to 9, outputting the respective array value directly to `GPIOB->ODR` (Output Data Register), updating the display. A software delay separates the numbers.

## 2. `2 digit display` (Multiplexing)
This intermediate project builds on the single-digit code to drive a 2-digit 7-segment module using multiplexing.
### How it works:
1. **Pin Configuration**: In addition to the segment lines on `GPIOB`, the module requires control lines to quickly switch between the two digits.
2. **Multiplexing Logic**: The code outputs the segment data for "Digit 1" (from `a[1]`) while simultaneously setting a high signal (e.g., `0x0200` or PB9) to enable that specific digit's common pin.
3. **Rapid Switching**: After a tiny delay, it zeroes the output, sets the data for "Digit 2" (from `a[2]`), and enables the other common pin (e.g., `0x0100` or PB8).
4. **Persistence of Vision**: By executing this loop very fast inside the `while(1)`, human eyes perceive both digits as being continuously on simultaneously.
