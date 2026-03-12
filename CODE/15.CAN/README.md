# Controller Area Network (CAN)

This directory provides a bare-metal implementation of CAN bus communication on the STM32F4x microcontroller, combined with an I2C LCD for data visualization.

## 1. `Transmit_receive` / `self_T_R`
This project configures the bxCAN peripheral (CAN1) for transmission and reception while displaying sent and received data on a 16x2 I2C-based LCD. 

### How it works:
1. **Clock & GPIO Initialization**:
   - `GPIOA` clock is enabled for CAN lines PA11 (CAN_RX) and PA12 (CAN_TX). They are set to Alternate Function 9 (AF9).
   - `GPIOB` clock is enabled for I2C lines PB8 (SCL) and PB9 (SDA), configured for Alternate Function 4 (AF4) as Open-Drain.
2. **I2C & LCD Configuration**:
   - `I2C1` is initialized at standard speed by configuring the `CR1`, `CR2`, `CCR`, and `TRISE` registers directly.
   - Using the `I2C_Write()` function, standard 4-bit initialization commands (`0x30`, `0x20`, `0x28`) are sent to the `0x27` LCD slave address to set up the display.
3. **CAN Initialization (`CAN_Init`)**:
   - The CAN peripheral is put into Initialization mode by setting `INRQ` in `CAN1->MCR`.
   - **Bit Timing**: The `BTR` register is configured. The baud rate is set utilizing specific Time Quanta adjustments (`TS1`, `TS2`, and `BRP`) for the STM32F4's APB1 clock.
   - **Filters**: The CAN Filter master is unlocked (`FINIT` in `FMR`), and Filter Bank 0 is activated. `FR1` and `FR2` are set to `0` to accept all incoming CAN messages (promiscuous mode), after which the filters are locked again.
   - The CAN peripheral is placed back in Normal mode.
4. **Data Transmission & Reception**:
   - In the infinite loop, a character (`'H'` from "HELLO") is loaded into the Transmit Mailbox 0 (`sTxMailBox`). The transmit flag is set to send it out onto the bus.
   - The code then waits for a message to arrive in Receive FIFO 0 (`CAN1->RF0R & 3`). 
   - The received byte is pulled from the Data Length Register (`RDLR`), the FIFO is released, and the character is printed directly to the I2C LCD.
