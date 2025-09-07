#include "led_engine.hpp"

void LED_Init(void) {
    // 1. Enable GPIOB clock
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;

    // 2. Configure PB0 as general-purpose output
    GPIOB->MODER &= ~GPIO_MODER_MODE0_1; // Clear bit 1
    GPIOB->MODER |=  GPIO_MODER_MODE0_0; // Set bit 0 (Mode = 01 for Output)
}