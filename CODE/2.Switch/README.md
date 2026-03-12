# GPIO Switch/Button Input (Bare Metal)

This directory demonstrates reading digital input states from push buttons using the STM32F4x's bare-metal environment via software polling.

## 1. `switch on led on`
This highly fundamental project configures an input pin (connected to a switch) and continuously polls its state inside the main loop. When the switch is pressed, an LED is turned on; when released, it turns off.

### How it works:
1. **GPIO Initialization**:
   - Clocks for both `GPIOA` and `GPIOC` are activated in `RCC->AHB1ENR`.
   - **LED Setup**: Pin `PA5` is configured as a standard output mode (`GPIOA->MODER |= (1 << 10)`).
   - **Button Setup**: Pin `PC13` is configured as an input mode by clearing both mode bits (`GPIOC->MODER &= ~(3 << 26)`). 
2. **Internal Pull-Up Resistor**:
   - To prevent the `PC13` input from "floating" unpredictably when the switch is unpressed, its internal Pull-Up resistor is enabled (`GPIOC->PUPDR |= (1 << 26)`). This ensures the pin naturally reads `HIGH` (1).
3. **Continuous Polling Loop**:
   - Inside the main `while(1)` loop, the Input Data Register (`GPIOC->IDR`) is constantly read and masked (`& (1<<13)`) to check the state of Pin 13.
   - **If Pressed**: The switch connects `PC13` to Ground, pulling it `LOW` (0). The `if ((GPIOC->IDR & (1<<13)) == 0)` condition checks this, and if true, pulls the `PA5` output HIGH (`|=`) to light the LED.
   - **If Released**: The pull-up resistor overcomes the floating state, `PC13` becomes `HIGH` (1), execution falls into the `else` block, and `PA5` is pulled LOW (`&=~`) to disable the LED.
