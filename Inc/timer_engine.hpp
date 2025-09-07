#ifndef TIMER_ENGINE_HPP_
#define TIMER_ENGINE_HPP_

#include <functional>
#include "stm32h723xx.h"

class TIMER_ENGINE
{
public:
    TIMER_ENGINE();

    void initialize(uint16_t prescaler, uint16_t autoReload, std::function<void()> onUpdateCallback);   //TODO: Could func overload here for different timers.
    void start();
    static void isr_handler();

private:
    std::function<void()> callback;
    static TIMER_ENGINE* instance_ptr;
};

#endif /* TIMER_ENGINE_HPP_ */