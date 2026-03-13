# Analog-to-Digital Converter (ADC)

This directory features bare-metal code for reading analog voltage values and displaying them on an LCD.

## 1. `display the ADC value`
This project configures the STM32F4x's ADC1 peripheral to sample an analog signal on pin PA0 and displays the converted numeric value on a 16x2 / 20x4 parallel character LCD.

### How it works:
1. **ADC Initialization Phase**: 
   - **GPIO Setup**: The clock for `GPIOA` is enabled. Pin PA0 is put into Analog mode (`GPIOA->MODER |= 3`), and its internal pull-up/pull-down resistors are explicitly disabled.
   - **ADC Config**: The clock for `ADC1` is enabled on the APB2 bus. `ADC1->SMPR2` is configured to set the sample time for channel 0 to 480 clock cycles. `ADC1->SQR3` sets channel 0 as the single conversion target. `ADC1->CR2` turns on the ADC (ADON bit).
2. **Reading the ADC**:
   - The `ADC_Read()` function triggers a software conversion by setting the SWSTART bit in `ADC1->CR2`.
   - It employs a polling loop `while(!(ADC1->SR & (1<<1)))` to wait until the End Of Conversion (EOC) flag is set.
   - The digital value is read from `ADC1->DR` (Data Register), and the EOC flag is cleared.
3. **LCD Display**:
   - `GPIOB` is initialized for 8-bit parallel LCD communication (PB0-PB7 as data lines, PB8 as RS, PB9 as EN).
   - Standard LCD commands are sent to initialize it in 8-bit, 2-line mode.
   - In the infinite loop, the ADC integer is converted to a string using `sprintf` and printed out character-by-character to the LCD using the `data()` function.
