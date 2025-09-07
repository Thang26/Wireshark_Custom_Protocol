#include "uart_engine.hpp"

UART_ENGINE::UART_ENGINE()
{

}

void UART_ENGINE::initialize()
{
    RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN;        // Enable GPIOD clock
    RCC->APB1LENR |= RCC_APB1LENR_USART3EN;     // Enable USART3 clock

    /*  Configure PD8 (TX) as alternate function for USART3_TX
     *  Each pin gets two bits in MODER. Four options, with 10 being AF mode for the pin.
     *  To set GPIOD pin 8 to AF, we set MODE8_1 and clear MODE8_0 to fill in 10. 
     *  Then, we have to say which AF we want. There are two alternate func registers.
     *  AFR[0] for 0-7 and AFR[1] for 8-15. We want AFR[1] for pin 8.
     *  Pin 8 occupies the first 4 bits of AFR[1], and USART3 is defined as AF7 (0111).
     */
    GPIOD->MODER &= ~GPIO_MODER_MODE8_0;
    GPIOD->MODER |= GPIO_MODER_MODE8_1;
    GPIOD->AFR[1] &= ~GPIO_AFRH_AFSEL8;
    GPIOD->AFR[1] |= (7 << GPIO_AFRH_AFSEL8_Pos);

    /*  Set baud rate to 115200 bits/sec or 14,400 bytes/sec or 14 kb/s.
     *  TE allows USART to send data. UE turns on the entire USART peripheral.
     *  Both are within the control register 1.
     */
    USART3->BRR = 556;                          // 64MHz / 115200 = 555.55, rounded to 556
    USART3->CR1 |= USART_CR1_TE | USART_CR1_UE; // Enable Transmitter (TE) and USART (UE)
}

void UART_ENGINE::transmit(const char* tx_data)
{
    // Loop until we reach the null terminator character of the string.
    while (*tx_data)
    {
        // The TXE flag in the Interrupt and Status Register (ISR) is set by hardware
        // when the TDR is ready to accept a new byte.
        while (!(USART3->ISR & USART_ISR_TXE_TXFNF));

        // Writing to TDR clears the TXE flag. The hardware then takes this byte
        // and begins shifting it out serially.
        USART3->TDR = *tx_data++; // Write the byte and increment the pointer
    }

    // The TC flag in the ISR is set when the last frame has finished transmitting.
    // This ensures the UART line is idle before the function returns.
    while (!(USART3->ISR & USART_ISR_TC));
}

void UART_ENGINE::transmit(const uint8_t* tx_data, size_t tx_data_size)
{
    // Loop for the exact number of bytes specified by tx_data_size.
    for (size_t i = 0; i < tx_data_size; ++i)
    {
        // Wait until the Transmit Data Register (TDR) is empty.
        while (!(USART3->ISR & USART_ISR_TXE_TXFNF));

        // Write the current byte of data to the Transmit Data Register.
        USART3->TDR = tx_data[i];
    }

    // Wait for the transmission to be fully complete.
    while (!(USART3->ISR & USART_ISR_TC));
}