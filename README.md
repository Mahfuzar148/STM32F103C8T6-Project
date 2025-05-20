


# 📘 STM32F103C8T6 – LED Blink Project Documentation

## 🧾 Project Title

**LED Blinking on STM32F103C8T6 using Timer, Manual Delay, and HAL Library**

---

## 📋 Overview

This project demonstrates how to blink an onboard LED connected to **PC13** pin of the **STM32F103C8T6 (Blue Pill)** development board using three different methods:

1. **Bare Metal (Using Timer2)**
2. **Bare Metal (Using Software Delay Loop)**
3. **HAL Library**

---

## 🛠️ Requirements

### 🔧 Hardware

| Item                     | Description                             |
| ------------------------ | --------------------------------------- |
| Microcontroller          | STM32F103C8T6 (Blue Pill)               |
| LED                      | Onboard LED connected to PC13           |
| USB to Serial/Programmer | ST-Link V2 / USB UART (for programming) |
| Breadboard & Wires       | (Optional for external LED)             |
| Power Supply             | 3.3V via USB                            |

### 💻 Software

| Tool                       | Description                                   |
| -------------------------- | --------------------------------------------- |
| STM32CubeIDE               | For development, compilation, and programming |
| STM32CubeMX (optional)     | For HAL configuration                         |
| OpenOCD or ST-Link Utility | For flashing firmware                         |
| Makefile/GCC (optional)    | For manual compilation (bare-metal)           |

---

## 📌 Pin Configuration

| Pin  | Function              |
| ---- | --------------------- |
| PC13 | Output to onboard LED |

---

## ⚙️ Setup Instructions

### 🧰 For Bare Metal (Manual or Timer):

1. **Install toolchain**: Install `arm-none-eabi-gcc`, OpenOCD.
2. **Connect ST-Link Programmer** to Blue Pill.
3. **Write code** in `.c` file using any text editor.
4. **Compile** using Makefile or directly via `gcc`.
5. **Flash** using `st-flash` or OpenOCD.

### 🧰 For STM32CubeIDE (HAL Method):

1. Open STM32CubeIDE.
2. Create a new project for STM32F103C8Tx.
3. Enable GPIOC, and configure **PC13 as Output Push-Pull**.
4. Generate code.
5. Add LED toggle code in `while(1)` loop.
6. Build and flash using built-in tools.

---

## 📄 Code Implementations

---

### 🔹 Method 1: Bare Metal (Using Timer2)

```c
#define RCC_BASE        0x40021000 
#define GPIOC_BASE      0x40011000
#define TIM2_BASE       0x40000000

#define RCC_APB2ENR     *(volatile unsigned int*) (RCC_BASE + 0x18)
#define RCC_APB1ENR     *(volatile unsigned int*) (RCC_BASE + 0x1C)

#define GPIOC_CRH       *(volatile unsigned int*) (GPIOC_BASE + 0x04)
#define GPIOC_ODR       *(volatile unsigned int*) (GPIOC_BASE + 0x0C)

#define TIM2_CR1        *(volatile unsigned int*) (TIM2_BASE + 0x00)
#define TIM2_PSC        *(volatile unsigned int*) (TIM2_BASE + 0x28)
#define TIM2_ARR        *(volatile unsigned int*) (TIM2_BASE + 0x2C)
#define TIM2_SR         *(volatile unsigned int*) (TIM2_BASE + 0x10)

int main() {
    // Enable GPIOC and TIM2
    RCC_APB2ENR |= (1u << 4);  // Enable GPIOC
    RCC_APB1ENR |= (1u << 0);  // Enable TIM2

    // Set PC13 as Output Push-Pull (2 MHz)
    GPIOC_CRH &= ~(0xF << 20);
    GPIOC_CRH |=  (0x2 << 20);

    // Timer2 Setup: 8 MHz / (PSC+1) → 1 KHz timer
    TIM2_PSC = 7999;
    TIM2_ARR = 999;
    TIM2_CR1 |= (1u << 0); // Enable Timer

    while (1) {
        if (TIM2_SR & (1 << 0)) {
            GPIOC_ODR ^= (1u << 13); // Toggle LED
            TIM2_SR &= ~(1 << 0);    // Clear update flag
        }
    }
}
```

---

### 🔹 Method 2: Bare Metal (Using Software Delay)

```c
#define RCC_BASE        0x40021000
#define GPIOC_BASE      0x40011000

#define RCC_APB2ENR     *(volatile unsigned int*) (RCC_BASE + 0x18)
#define GPIOC_CRH       *(volatile unsigned int*) (GPIOC_BASE + 0x04)
#define GPIOC_BSRR      *(volatile unsigned int*) (GPIOC_BASE + 0x10)

#define DELAY 50000

int main() {
    RCC_APB2ENR |= (1 << 4);           // Enable GPIOC
    GPIOC_CRH &= ~(0xF << 20);         // Clear PC13 config
    GPIOC_CRH |=  (0x2 << 20);         // Set PC13 as output

    while (1) {
        GPIOC_BSRR = (1 << (13 + 16)); // Reset PC13 (LED ON)
        for (volatile int i = 0; i < DELAY; i++); // Delay
        GPIOC_BSRR = (1 << 13);        // Set PC13 (LED OFF)
        for (volatile int i = 0; i < DELAY; i++);
    }
}
```

---

### 🔹 Method 3: HAL Library

```c
while (1)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // LED ON
    HAL_Delay(1000);  // Delay 1 second
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   // LED OFF
    HAL_Delay(1000);  // Delay 1 second
}
```

---

## 🧪 Expected Output

* The onboard LED (connected to PC13) will **blink at a 1-second interval**, indicating the microcontroller is working and the GPIO output toggling is functioning correctly.
* All methods result in the same behavior but demonstrate different abstraction levels.

---

## 📙 Notes

* The onboard LED in STM32F103C8T6 is **active-low** (LED turns on when PC13 is `0`).
* Using the timer is more accurate and power-efficient than software delays.
* HAL method is easier for beginners but adds overhead and abstracts hardware.

---

## ✅ Conclusion

This project illustrates how to interface a GPIO pin to blink an LED using three different programming styles on STM32. It helps you understand:

* Hardware registers in bare-metal programming.
* Timer peripheral usage.
* STM32 HAL abstraction.


