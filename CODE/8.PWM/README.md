# Pulse Width Modulation (PWM) (Bare Metal)

This directory features a bare-metal implementation for generating PWM signals using STM32F4x Timers, specifically to control the brightness of an LED or speed of a motor.

## 1. `brighness`
This project demonstrates how to configure General-Purpose Timer 2 (TIM2) in Output Compare mode to generate a PWM signal on GPIO pin PA5.

### How it works:
1. **Clock & GPIO Configuration**:
   - `GPIOA` and `TIM2` peripheral clocks are enabled.
   - Pin PA5 is configured not as a standard output, but as Alternate Function 1 (`AF1`), effectively routing the `TIM2_CH1` (Timer 2 Channel 1) output directly to the pin.
2. **Timer Initialization**:
   - The Timer Prescaler (`TIM2->PSC`) is set to `84 - 1`, which divides the APB1 clock to tick at 1 MHz (assuming standard clock config).
   - The Auto-Reload Register (`TIM2->ARR`) is set to `20000 - 1`. This means the timer resets every 20,000 microseconds (20 ms), generating a fixed PWM frequency of **50 Hz**.
3. **Duty Cycle & Output Compare**:
   - The Capture/Compare Register 1 (`TIM2->CCR1`) sets the duty cycle. It is initialized to `1500`. Since the ARR is 20000, 1500 / 20000 creates a duty cycle of **7.5%** (often used as the neutral point for servo motors).
   - `TIM2->CCMR1` configures Channel 1 into **PWM Mode 1** (`OC1M = 110`), meaning the pin is active (high) while the counter is less than `CCR1`, and inactive (low) otherwise.
4. **Start Timer**:
   - The channel output is enabled (`CC1E` in `CCER`).
   - The counter is finally started by setting `CEN` in `TIM2->CR1`.
