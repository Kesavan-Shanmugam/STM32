# External Interrupts (EXTI) and NVIC (Bare Metal)

This directory features a bare-metal implementation demonstrating how to configure edge-triggered hardware interrupts on the STM32F4x using the External Interrupt/Event Controller (EXTI) and Nested Vectored Interrupt Controller (NVIC).

## 1. `led interrupt`
This project configures a user button (connected to PC13) to trigger a hardware interrupt on the falling edge (when pressed), which asynchronously toggles an LED on PA5.

### How it works:
1. **Clock Setup**:
   - `GPIOA` and `GPIOC` clocks are enabled in `AHB1ENR`.
   - Critically, the `SYSCFG` (System Configuration Controller) clock is enabled in `APB2ENR` to allow routing the EXTI lines.
2. **GPIO Configuration**:
   - `PA5` is set as a standard output (for the LED).
   - `PC13` is set as an input (for the Button).
3. **SYSCFG and EXTI Configuration**:
   - `SYSCFG->EXTICR[3]` (EXTI Control Register 4) is modified to connect EXTI line 13 to Port C. (`0x2` selects Port C).
   - The interrupt mask is disabled by setting `EXTI->IMR |= (1<<13)` to allow requests from line 13.
   - Falling trigger selection is enabled by setting `EXTI->FTSR |= (1<<13)`. This means pressing the active-low button triggers the interrupt.
4. **NVIC Enablement**:
   - The interrupt is globally enabled in the Cortex-M4 core vectors calling `NVIC_EnableIRQ(EXTI15_10_IRQn)` to handle lines 10 through 15.
5. **Interrupt Service Routine (ISR)**:
   - When the button is pressed, the hardware automatically jumps to the `EXTI15_10_IRQHandler(void)` function without polling.
   - It checks the Pending Register (`EXTI->PR`) for line 13. If set, it toggles the PA5 LED using a bitwise XOR, and immediately clears the pending flag by writing a `1` back to it (`EXTI->PR |= (1 << 13)`).
