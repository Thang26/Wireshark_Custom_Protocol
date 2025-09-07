#include "timer_engine.hpp"

TIMER_ENGINE* TIMER_ENGINE::instance_ptr = nullptr;

TIMER_ENGINE::TIMER_ENGINE()
{
    /* We only want one single object, so maybe TODO: Deploy singleton pattern. */
    instance_ptr = this;
}

void TIMER_ENGINE::initialize(uint16_t prescaler, uint16_t autoReload, std::function<void()> onUpdateCallback)
{
    // Store the user's callback function
    callback = onUpdateCallback;

    // 1. Enable TIM6 clock
    RCC->APB1LENR |= RCC_APB1LENR_TIM6EN;

    // 2. Set Prescaler and Auto-Reload Register
    TIM6->PSC = prescaler; // e.g., 64000 - 1
    TIM6->ARR = autoReload;  // e.g., 1000 - 1

    // 3. Enable the Update Interrupt in the timer peripheral
    TIM6->DIER |= TIM_DIER_UIE;

    // 4. Enable the TIM6 interrupt in the NVIC
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void TIMER_ENGINE::start()
{
    TIM6->CR1 |= TIM_CR1_CEN;
}

void TIMER_ENGINE::isr_handler()
{
    if (instance_ptr != nullptr && instance_ptr->callback)
    {
        instance_ptr->callback();
    }
}

extern "C" void TIM6_DAC_IRQHandler(void)
{
    TIM6->SR &= ~TIM_SR_UIF;
    GPIOB->ODR ^= GPIO_ODR_OD0;
    TIMER_ENGINE::isr_handler();
}