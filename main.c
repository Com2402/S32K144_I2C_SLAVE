/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "S32K144.h"
#include "CORE_M4.h"
#include "ADC_drv.h"
#include "Clock_drv.h"
#include "Port_drv.h"
#include "Timer_drv.h"
#include "i2c_drv.h"


#if defined (__ghs__)
    #define __INTERRUPT_SVC  __interrupt
    #define __NO_RETURN _Pragma("ghs nowarning 111")
#elif defined (__ICCARM__)
    #define __INTERRUPT_SVC  __svc
    #define __NO_RETURN _Pragma("diag_suppress=Pe111")
#elif defined (__GNUC__)
    #define __INTERRUPT_SVC  __attribute__ ((interrupt ("SVC")))
    #define __NO_RETURN
#else
    #define __INTERRUPT_SVC
    #define __NO_RETURN
#endif

int counter, accumulator = 0, limit_value = 1000000;
volatile uint16_t adc_val = 0;
volatile uint32_t adc_flag = 0;
#define FREQ_MIN_HZ (10000U)
#define FREQ_MAX_HZ (100000U)
#define ADC_MAX_VAL (4095U)
#define FREQ_RANGE_HZ (FREQ_MAX_HZ - FREQ_MIN_HZ)
void user_timer_cb()
{
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	pin_toggle(PTD, STATUS_PIN);
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	__asm volatile ("NOP");
	__asm volatile ("NOP");
}
uint8_t volatile flag_change_timer = 0;
uint32_t volatile timer_val = 0;
uint8_t count = 0;
uint8_t receive_buff[4];
void i2c_slave_user_cb(uint8_t data)
{
	receive_buff[count] = data;
	count++;
	if(count == 4)
	{
		uint32_t adc_val = (receive_buff[2]<<8)+receive_buff[3];
		timer_val = FREQ_MIN_HZ + ((adc_val * FREQ_RANGE_HZ) / ADC_MAX_VAL);
		flag_change_timer = 1;
		count = 0;
	}
}
int main(void) {
    counter = 0;
    SysTick_Init();
    Clock_Init();
    setup_pin();
    timer_cb_register(user_timer_cb);
    init_timer(100);
    i2c_slave_cb_register(i2c_slave_user_cb);
    i2c_slave_init();
    uint32_t start_ms = get_tick();
    uint8_t user_request = 0;
    for (;;) {
    	if(get_tick()-start_ms >= 2000)
    	{
    		set_request(user_request++);
    		start_ms = get_tick();
    	}
    	if(flag_change_timer == 1)
    	{
    		init_timer(timer_val);
    		flag_change_timer = 0;
    	}
    }
    /* to avoid the warning message for GHS and IAR: statement is unreachable*/
    __NO_RETURN
    return 0;
}
__INTERRUPT_SVC void SVC_Handler() {
    accumulator += counter;
}
