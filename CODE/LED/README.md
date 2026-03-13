# GPIO LED Control (Bare Metal)

This directory contains bare-metal implementations for controlling LEDs using the GPIO peripheral on the STM32F4x microcontroller. There are two primary examples provided in this folder:

## 1. `LED on` / `blink`
This example demonstrates the most fundamental GPIO output operation: blinking a single LED.
### How it works:
1. **Clock Enable**: The GPIOA peripheral clock is enabled by setting the corresponding bit (bit 0) in the `RCC->AHB1ENR` (AHB1 Peripheral Clock Enable Register).
2. **Pin Configuration**: GPIOA Pin 5 is configured as a general-purpose output mode by setting bits in the `GPIOA->MODER` register.
3. **Pin Toggling**: Inside the infinite `while(1)` loop, the code uses a bitwise XOR (`^=`) operation on the `GPIOA->ODR` (Output Data Register) to toggle the state of Pin 5.
4. **Delay Loop**: A simple software `for`-loop (`delay_ms`) using the `__NOP()` instruction is utilized to create a visible delay between state changes.

## 2. `one by one`
This example expands on the basic blinking by controlling a sequence of LEDs connected to multiple pins on GPIOA (Pins 0, 1, 4-11).
### How it works:
1. **Clock Enable**: Similar to the basic blink, the AHB1 clock for GPIOA is enabled.
2. **Bulk Configuration**: The `GPIOA->MODER` is configured to set multiple pins to output mode simultaneously. 
3. **Lookup Table**: An array `led[]` holds the specific hexadecimal pin masks (e.g., `0x0001` for PA0, `0x0010` for PA4) corresponding to the connected LEDs.
4. **Sequential Execution**: A `for` loop iterates through the array. It assigns the mask to `GPIOA->ODR` to light the specific LED, delays, clears the register (`0x0000`) to turn it off, and delays again before moving to the next LED.
