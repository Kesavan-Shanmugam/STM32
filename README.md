# STM32 Bare Metal Programming

Welcome to the **STM32 Bare-Metal Programming** repository.

This repository is dedicated to advanced embedded systems engineering. It contains educational, register-level projects for programming **STM32 Microcontrollers (STM32F4x series - ARM Cortex-M4)** entirely without the abstraction of HAL (Hardware Abstraction Layer) or Standard Peripheral Libraries.

By altering hardware memory-mapped registers directly from the CMSIS headers, the code remains brutally fast, highly efficient, and deeply educational regarding how ARM microprocessors operate internally.

## 📂 Repository Structure

- **[CODE](./CODE/)**: 
  - The absolute core of the repository. Contains **17 highly detailed, sequentially ordered projects**. 
  - Ranges from fundamental GPIO blink routines to advanced protocols like CAN, SPI, I2C, and GPS parsing.
  - *Make sure to read the comprehensive `README.md` inside the `CODE` folder for the complete breakdown.*
- **[DESIGN](./DESIGN/)**: 
  - Contains all the `image/` and `video/` assets.
  - Refer to these to see the breadboard circuitry, LCD readouts, and operational proof-of-work.
- **[MANUALS](./MANUALS/)**: 
  - Bare-metal coding is impossible without documentation. This folder contains the **Reference Manuals** (explaining all register bits), the **Datasheet** (for electrical characteristics), and the pinout diagram (`STM32F446RE-pinout.png`) necessary to map functionalities to physical legs on the IC.

## 🚀 Getting Started

If you are new to bare-metal programming on STM32:
1. Open up the `STM32F446RE-pinout` in `MANUALS/` and keep it on your second monitor. You will need it to find the Alternate Functions.
2. Read the `CODE/README.md` to understand the standard 5-step process of hardware initialization (RCC -> GPIO Mod -> Alternate Function -> Config -> Enable).
3. Start small: Begin with `CODE/1.LED` and `CODE/2.Switch` to understand polling. Then move up to `CODE/7.Interrupt` to appreciate the NVIC cortex handler.
4. Move onto standard protocols like `10.UART` and `12.I2C` before attempting advanced modules like visual `17.OLED` rendering.

Happy Coding!