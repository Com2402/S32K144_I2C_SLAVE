#include "Timer_drv.h"
#include "CORE_M4.h"
#include <stdio.h>
static timer_cb_t timer_cb = NULL;
void LPIT0_Ch0_IRQHandler()
{
	if(LPIT0->MSR & LPIT_MSR_TIF0_MASK)
	{
		if(timer_cb != NULL)
		{
			timer_cb();
		}
		LPIT0->MSR = LPIT_MSR_TIF0_MASK;
	}
}
void init_timer(uint32_t ms)
{
	/*
	 * Enable clock
	 *Enable the peripheral clock MCR[M_CEN]
	 *Wait for 4 peripheral clock
	 *Config channel
	 *	Timer mode TCTRLn[MODE]
	 *	Trigger source selections TCTRLn[TRG_SEL], TCTRLn[TRG_SRC]
	 *	Trigger control bit TCTRLn[TROT, TSOT, TSOI]
	 *Config channel that are to be chained setting CHAIN bit in corresponding TCTRLn
	 *Set Timeout val TVAL (for channel in Compare mode)
	 *Configure TIEn bits in MIER register
	 *Enable the channel timers  T_EN bit in the corresponding channel's TCRTLn register
	 * */

	PCC->PCCn[PCC_LPIT_INDEX] &= ~(PCC_PCCn_CGC_MASK);
		PCC->PCCn[PCC_LPIT_INDEX] &= ~(PCC_PCCn_PCS_MASK);
	PCC->PCCn[PCC_LPIT_INDEX] |= (0x02 << PCC_PCCn_PCS_SHIFT);
	PCC->PCCn[PCC_LPIT_INDEX] |= (PCC_PCCn_CGC_MASK);
	LPIT0->MCR |= (LPIT_MCR_M_CEN_MASK);
	delayms(1);
	LPIT0->TMR[0].TCTRL &=  ~(LPIT_TMR_TCTRL_MODE_MASK); // 00b - 32-bit Periodic Counter
	LPIT0->TMR[0].TCTRL &= ~(LPIT_TMR_TCTRL_TROT_MASK);
	LPIT0->TMR[0].TCTRL &= ~(LPIT_TMR_TCTRL_TSOT_MASK);
	LPIT0->TMR[0].TCTRL &= ~(LPIT_TMR_TCTRL_TSOI_MASK);
		LPIT0->TMR[0].TVAL = (uint32_t) (TIMER_ASYNC_CLOCK_FRQ/1000)*ms -1;
	LPIT0->MIER |= LPIT_MIER_TIE0_MASK;
	NVIC_Set_Peripheral_Priority(LPIT0_Ch0_IRQn, 1);
	NVIC_Enable_Peripheral_Interrupt(LPIT0_Ch0_IRQn);
			LPIT0->TMR[0].TCTRL |= (LPIT_TMR_TCTRL_T_EN_MASK);

}
void disable_timer()
{
	LPIT0->MCR &= ~(LPIT_MCR_M_CEN_MASK);
	LPIT0->MCR |= (LPIT_MCR_SW_RST_MASK);
	delayms(1);
	LPIT0->MCR &= ~(LPIT_MCR_SW_RST_MASK);
}
void timer_cb_register(timer_cb_t cb)
{
	if(cb != NULL)
	{
		timer_cb = cb;
	}
}
