#ifndef I2C_DRV_H
#define I2C_DRV_H
#include "S32K144.h"
#define SLAVE_ADDRESS 0x68
typedef enum
{
	TRANSMIT_DATA = 0x00,
	RECEIVE_DATA,
	GEN_STOP,
	RECEIVE_AND_DISACRD,
	GEN_START,
	MAX_CMD,
}master_cmd_t;

typedef enum
{
	STATUS_OKAY = 0,
	ERROR,
}master_status_t;
typedef void (*i2c_slave_cb_t)(uint8_t data);
void i2c_master_init();
void i2c_slave_init();
master_status_t i2c_master_write(uint8_t add, uint8_t* buff, int len);
master_status_t i2c_master_read(uint8_t add, uint8_t* buff, int len);
void i2c_slave_cb_register(i2c_slave_cb_t cb);
void set_request(uint8_t val);
#endif
