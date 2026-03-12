# Matrix Keypad Interface (Bare Metal)

This directory demonstrates interfacing a standard 4x4 (or 4x3) matrix keypad with the STM32F4x using register-level GPIO polling and scanning.

## 1. `key_pad_4x3`
This project utilizes an interesting arrangement of inputs/outputs on `GPIOC` to rapidly scan the rows and columns of a keypad. When a key is successfully identified, its character is sent to a parallel 16x2 LCD connected to `GPIOB`.

### How it works:
1. **GPIO Initialization**:
   - `GPIOB` is initialized as outputs to drive the Data and Control lines of the standard character LCD (PB0-PB9).
   - `GPIOC` is configured specifically for the matrix keypad:
     - The first 4 pins (`PC0` to `PC3`) are set as Output pins representing the 4 Rows (`R1-R4`).
     - The next 4 pins (`PC4` to `PC7`) are set as Input pins representing the 4 Columns (`C1-C4`).
     - Internal Pull-Up resistors are activated on the input pins (`GPIOC->PUPDR |= 0x5555`).
2. **Matrix Scanning Algorithm**:
   - Due to the pull-up resistors, all column inputs (`C1-C4`) normally read High.
   - The algorithm works by iterating through the 4 Rows sequentially in infinite `while(1)` loops:
     - **Drive one row LOW**: For instance, Row 1 (`PC0`) is pulled Low (`GPIOC->ODR &= ~R1`), while the other rows are held High.
     - **Read the Columns**: It reads the Input Data Register (`GPIOC->IDR`). If a button in that specific Row is pressed, it bridges the Low Row to a Column, pulling that Column Low.
     - By checking `!(GPIOC->IDR & C1)`, the code determines exactly which column in that active row was pulled to GND.
   - The code relies on simple delays for rudimentary switch debouncing. 
3. **LCD Integration**:
   - Once a pressed coordinate (Row, Column) is confirmed, the specific character (e.g., `'1'`, `'5'`, `'#'`) is printed to the LCD using the `data(char)` function.
