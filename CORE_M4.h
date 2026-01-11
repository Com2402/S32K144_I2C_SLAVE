#ifndef CORE_M4_H
#define CORE_M4_H
#include <stdint.h>
typedef struct
{
  volatile uint32_t ISER[8];
  uint32_t RESERVED0[24];
  volatile uint32_t ICER[8];
  uint32_t RESERVED1[24];
  volatile uint32_t ISPR[8];
  uint32_t RESERVED2[24];
  volatile uint32_t ICPR[8];
  uint32_t RESERVED3[24];
  volatile uint32_t IABR[8];
  uint32_t RESERVED4[56];
  volatile uint8_t  IP[240];
  uint32_t RESERVED5[644];
  volatile  uint32_t STIR;
} NVIC_Typedef;
typedef struct
{
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile const uint32_t CALIB;
}SysTick_Typedef;

/*Systick*/
/*SysTick CTRL*/
#define SYSTICK_CNT_FLAG_POSITION (16)
#define SYSTICK_CNT_FLAG_BITMASK (1 << SYSTICK_CNT_FLAG_POSITION)
/*SysTick CLKSOURCE*/
#define SYSTICK_CLKSOURCE_POSITION (2)
#define SYSTICK_CLKSOURCE_MASK (1 << SYSTICK_CLKSOURCE_POSITION)

/*SysTick INT*/
#define SYSTICK_INT_POSITION (1)
#define SYSTICK_INT_MASK (1 << SYSTICK_INT_POSITION)

/*SysTick ENABLE*/
#define SYSTICK_ENABLE_POSITION (0)
#define SYSTICK_ENABLE_MASK (1 << SYSTICK_ENABLE_POSITION)
/*Systick*/


#define SysTick ((SysTick_Typedef*) 0xE000E010)
#define NVIC ((NVIC_Typedef*) 0xE000E100)

static inline void NVIC_Set_Peripheral_Priority(int32_t IRQn, uint32_t priority)
{
	if(IRQn>=0)
	{
		NVIC->IP[((uint32_t)IRQn)]=(uint8_t)((priority<<4)&0xFF);
	}
}
static inline void NVIC_Enable_Peripheral_Interrupt(int32_t IRQn)
{
	if(IRQn>=0)
	{
		NVIC->ISER[((uint32_t)IRQn)>>5]|=(1<<(((uint32_t)IRQn)&0x1F));
	}
}
static inline void NVIC_DISABLE_Peripheral_Interrupt(int32_t IRQn)
{
	if(IRQn >= 0)
	{
		NVIC->ICER[((uint32_t)IRQn)>>5]|=(1<<(((uint32_t)IRQn)&0x1F));
	}
}
void SysTick_Init();
void delayms(uint32_t ms);
uint32_t get_tick();
#endif
