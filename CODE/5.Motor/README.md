# Motor Control (Bare Metal)

This directory provides bare-metal implementation for controlling motors via simple digital logic output on the STM32F4x microcontroller.

## 1. `DC motor`
This straightforward project demonstrates how to control the direction of two DC motors (Motor A and Motor B) using standard GPIO output pins connected to a motor driver (such as the L298N dual H-bridge).

### How it works:
1. **GPIO Setup**:
   - `GPIOB` clock is enabled using `RCC->AHB1ENR`.
   - Four pins are assigned as general-purpose outputs: 
     - Motor A Control: PB12 and PB13
     - Motor B Control: PB14 and PB15
2. **Motor Direction Logic**:
   - A DC motor driver uses H-Bridge logic where two control pins determine the polarity applied to the motor coils.
   - **Forward**: The first pin is pulled High, and the second is pulled Low (` |= (1<<12)` and `&= ~(1<<13)`).
   - **Reverse**: The logic is explicitly flipped. The first pin is High, the second is Low. 
   - **Stop**: Both pins are pulled Low (`&= ~(1<<12)` and `&= ~(1<<13)`).
3. **Execution Routine**:
   - The main loop calls abstraction functions (e.g., `MotorA_Forward()`) to test the motor sequence. Both motors go forward for 500ms, stop for 1 second, go in reverse for 500ms, and stop for 2 seconds endlessly.
