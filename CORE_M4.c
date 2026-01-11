#include "CORE_M4.h"
static volatile  uint32_t tick_ms = 0;
void SysTick_Handler()
{
	tick_ms++;
}
void SysTick_Init()
{
	/*
	 *disable
	 *enable clock 1+ isr 1
	 *enable
	*/
	SysTick->CTRL &=~ (SYSTICK_ENABLE_MASK);
	SysTick->CTRL |= (SYSTICK_CLKSOURCE_MASK | SYSTICK_INT_MASK);
		SysTick->LOAD = 47999;
	SysTick->CTRL |= (SYSTICK_ENABLE_MASK);

}
uint32_t get_tick(){
	return tick_ms;
}
void delayms(uint32_t ms){
	uint32_t start_ms = get_tick();
uint32_t timeout = start_ms+ms;
	while(get_tick() < timeout);
}
