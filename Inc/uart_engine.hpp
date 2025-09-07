#ifndef UART_ENGINE_HPP_
#define UART_ENGINE_HPP_

#include "stm32h723xx.h"
#include <cstddef>

class UART_ENGINE
{
public:
    UART_ENGINE();

    void initialize();
    void transmit(const char* tx_data);
    void transmit(const uint8_t* tx_data, size_t tx_data_size);

private:
};

#endif /* UART_DRIVER_HPP_ */