// Base addresses for peripherals
#define RCC_BASE        0x40021000   // Reset and Clock Control base address
#define GPIOC_BASE      0x40011000   // GPIO Port C base address
#define TIM2_BASE       0x40000000   // Timer 2 base address

// RCC register access
#define RCC_APB2ENR     *(volatile unsigned int*) (RCC_BASE + 0x18)  // Enable clock for GPIO ports
#define RCC_APB1ENR     *(volatile unsigned int*) (RCC_BASE + 0x1C)  // Enable clock for Timer2

// GPIOC registers
#define GPIOC_CRH       *(volatile unsigned int*) (GPIOC_BASE + 0x04) // Port configuration register high (for PC8–PC15)
#define GPIOC_ODR       *(volatile unsigned int*) (GPIOC_BASE + 0x0C) // Output data register (used to toggle PC13)

// Timer 2 registers
#define TIM2_CR1        *(volatile unsigned int*) (TIM2_BASE + 0x00)  // Control register 1 (used to enable the timer)
#define TIM2_PSC        *(volatile unsigned int*) (TIM2_BASE + 0x28)  // Prescaler register
#define TIM2_ARR        *(volatile unsigned int*) (TIM2_BASE + 0x2C)  // Auto-reload register (timer period)
#define TIM2_SR         *(volatile unsigned int*) (TIM2_BASE + 0x10)  // Status register (used to check timer update event)

int main()
{
    // 1. Enable clock to GPIOC (Bit 4 of APB2ENR must be set)
    RCC_APB2ENR |= (1u << 4);

    // 2. Enable clock to Timer2 (Bit 0 of APB1ENR must be set)
    RCC_APB1ENR |= (1u << 0);

    // 3. Configure PC13 as General Purpose Output Push-Pull
    // First clear bits [23:20] (for PC13 configuration)
    GPIOC_CRH &= ~(0xF << 20);

    // Set mode: Output mode, max speed 2 MHz (0b0010)
    GPIOC_CRH |= (0x2 << 20);

    // 4. Timer2 configuration
    // Prescaler: (7999 + 1) → Timer clock = 8 MHz / 8000 = 1 KHz (1ms per count)
    TIM2_PSC = 7999;

    // Auto-reload value: 999 → Timer overflows every 1000 ms (1 second)
    TIM2_ARR = 999;

    // Enable Timer2 (CEN bit = 1 in TIM2_CR1)
    TIM2_CR1 |= (1u << 0);

    // 5. Infinite loop to toggle LED using timer update event
    while (1)
    {
        // Wait for update interrupt flag (UIF) to be set in TIM2_SR (bit 0)
        if (TIM2_SR & (1 << 0))
        {
            // Toggle PC13 (XOR with 1 to flip state)
            GPIOC_ODR ^= (1u << 13);

            // Clear the update interrupt flag (write 0 to bit 0)
            TIM2_SR &= ~(1 << 0);
        }
    }
}
