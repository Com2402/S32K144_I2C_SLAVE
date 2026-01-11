#ifndef PORT_DRV_H
#define PORT_DRV_H
#include "S32K144.h"

#define ADC_PIN (14U)
#define SCL_PIN (2U)
#define SDA_PIN (3U)
#define STATUS_PIN (16U)
#define ADC_STATUS_PIN (15U)
#define SCLS_PIN (0U)
#define SDAS_PIN (1U)

#define ADC_PIN_PORT PORTC
#define I2C0_PIN_PORT PORTA
#define STATUS_PIN_PORT PORTD
#define ADC_STATUS_PIN_PORT PORTD

#define ADC_PIN_PORT_INDX PCC_PORTC_INDEX
#define I2C0_PIN_PORT_INDX PCC_PORTA_INDEX
#define STATUS_PIN_PORT_INDX PCC_PORTD_INDEX
#define ADC_STATUS_PIN_PORT_INDX PCC_PORTD_INDEX

void setup_pin();
void pin_set(GPIO_Type* GPIOx, uint32_t pin_number);
void pin_clear(GPIO_Type* GPIOx, uint32_t pin_number);
void pin_toggle(GPIO_Type* GPIOx, uint32_t pin_number);
#endif
