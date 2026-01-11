#ifndef TIMER_H
#define TIMER_H
#include "S32K144.h"
#define TIMER_ASYNC_CLOCK_FRQ (8000000U)

typedef void (*timer_cb_t)(void);
void init_timer(uint32_t ms);
void disable_timer();
void timer_cb_register(timer_cb_t cb);

#endif
