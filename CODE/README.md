# STM32F4x Bare-Metal Code Repository

Welcome to the **Master Directory** for all STM32F4x register-level (bare-metal) implementations. This directory contains **17 distinct projects**, spanning from simple GPIO manipulation to complex communication protocols and display interfacing. 

**Zero HAL, Zero Standard Peripheral Libraries.** All projects are coded purely by accessing the hardware memory-mapped registers via the `stm32f4xx.h` CMSIS header.

---

## 🧠 The Bare-Metal Philosophy

When developing these projects, a standard sequence of hardware initialization is rigidly followed directly out of the **Reference Manual**:

1. **Enable the Clock (RCC)**: No peripheral functions without its clock being enabled on the AHB or APB buses (e.g., `RCC->AHB1ENR |= (1<<0);` for GPIOA).
2. **Configure the Pins (GPIO)**: Setting the `MODER` (Input, Output, Alternate Function, Analog), `OTYPER` (Push-Pull, Open Drain), and `PUPDR` (Pull-Up, Pull-Down) registers.
3. **Map the Alternate Function (AFR)**: If a pin is used for a hardware peripheral (like UART TX, SPI SCK, or I2C SCL), the correct AF index is multiplexed to the pin via the `AFR[0]` or `AFR[1]` registers.
4. **Configure the Peripheral**: Setting the corresponding control and baud-rate/timing registers for the specific module (e.g., `USART1->BRR`, `I2C1->CCR`, `TIM2->ARR`).
5. **Enable the Peripheral**: Setting the enable bit (`PE`, `UE`, `SPE`, `CEN`) to start hardware operation.

---

## 📁 Project Directory Breakdown

The projects are chronologically numbered based on complexity. Click into any directory to read its highly detailed `README.md` which breaks down the specific code execution.

### 🔴 Core GPIO & IO Operations
* **[1.LED](./1.LED/)**: Fundamentals of GPIO Output. Demonstrates enabling the AHB1 clock, setting `GPIOA->MODER` to output, and toggling the Output Data Register (`ODR`) to blink LEDs individually and sequentially.
* **[2.Switch](./2.Switch/)**: Fundamentals of GPIO Input. Shows how to configure `GPIOC` pins as inputs, enable internal Pull-Up Resistors (`PUPDR`), and execute software polling of the Input Data Register (`IDR`) to read button presses.
* **[3.7 segment](./3.7%20segment/)**: Advanced GPIO Output. Contains logic to push hexadecimal byte masks to `GPIOB` to form numeric characters on 7-segment displays, including high-speed multiplexing to utilize 2-digit modules.
* **[6.Key pad](./6.Key%20pad/)**: Matrix Input Scanning. Demonstrates reading a 4x4 matrix keypad by dynamically driving Row pins LOW and polling Column pins while utilizing switch-debouncing logic.
* **[7.Interrupt](./7.Interrupt/)**: Hardware Interrupts (EXTI/NVIC). Escaping software polling by mapping `PC13` to EXTI line 13 using the `SYSCFG` multiplexer, enabling falling-edge triggers, and toggling an LED inside the hardware ISR (`EXTI15_10_IRQHandler`).

### 📟 Displays & Visualization
* **[4.LCD](./4.LCD/)**: 8-bit Parallel Alphanumeric LCD. Complete HD44780 peripheral driver writing directly to the `GPIOB` data bus. Implements exact timing delays to strobe the `EN` and `RS` control pins to initialize the display and push ASCII characters.
* **[12.1.I2C based LCD](./12.1.I2C%20based%20LCD/)**: 4-bit serialized LCD via PCF8574 backpack. Abstracting the parallel LCD logic to send data over the I2C bus, severely reducing pin-count to just SCL/SDA. 
* **[17.OLED](./17.OLED/)**: Graphical SSD1306 Display. Sends complex initialization charge-pump algorithms over I2C and pushes byte arrays representing custom 8x8 font structures directly into the OLED's 1024-byte screen buffer.

### 🔄 Communication Protocols
* **[10.UART](./10.UART/)**: Asynchronous Serial. Configuring `USART1` for 9600 baud (`BRR = 0x0683` @ 16MHz). Demonstrates handling `TXE` (Transmit Empty) and `RXNE` (Receive Not Empty) polling flags for strings.
* **[12.I2C](./12.I2C/)**: Inter-Integrated Circuit. Demonstrates standard 100KHz Open-Drain communication. Covers generating START/STOP conditions, waiting for `SB`, `ADDR`, and `BTF` flags to transmit strings to a slave address.
* **[13.SPI](./13.SPI/)**: Serial Peripheral Interface. A synchronous full-duplex driver initializing `SPI1` as a Master, setting baud rate prescalers, utilizing Software Slave Management, and simultaneously transmitting and receiving bytes on the MOSI/MISO lines.
* **[15.CAN](./15.CAN/)**: Controller Area Network. Automotive protocol implementation. Initializing the `bxCAN` peripheral, configuring bit timing (`BTR`), setting promiscuous filter banks, and sending/receiving frames via Mailboxes/FIFOs.

### ⚙️ Analog, Timing & Actuators
* **[5.Motor](./5.Motor/)**: DC Motor Control. Shows how to generate Forward/Reverse/Stop conditions to standard L298N H-Bridge hardware using discrete GPIO pin logic states.
* **[8.PWM](./8.PWM/)**: Pulse Width Modulation. Configuring General Purpose Timer 2 (`TIM2`) in Output Compare Mode. Setting `ARR` for frequency (50Hz) and `CCR1` for duty cycle to generate precise hardware waveforms on Alternate Function pins.
* **[11.ADC](./11.ADC/)**: Analog to Digital. Enabling `ADC1` clock, setting sample cycles, and parsing the Data Register (`DR`) upon `EOC` (End Of Conversion) flags to read analog voltage potentials.

### 📡 Wireless Connectivity
* **[14.Bluetooth](./14.Bluetooth/)**: HC-05 Interfacing. Wraps the UART standard routines into a command parser. Waits for mapped ASCII digits to trigger hardware events (e.g. printing "bluetooth on" to an external LCD).
* **[16.GPS](./16.GPS/)**: NMEA Data Parsing. Interfaces a NEO-6M receiver over UART. Contains customized sequential parsing algorithms to locate `$GPRMC` NMEA sentences, strip comma delimiters, and extract raw Latitude and Longitude blocks.
