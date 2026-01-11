#include "i2c_drv.h"
#include "S32K144.h"
#include "CORE_M4.h"
#include <stdio.h>
static i2c_slave_cb_t i2c_cb = NULL;
uint8_t volatile data = 0;
volatile uint8_t i2c_state = 0;// 0 is write, 1 is read;
volatile uint8_t request = 0x5;
void set_request(uint8_t val)
{
	NVIC_DISABLE_Peripheral_Interrupt(LPI2C0_Slave_IRQn);
	LPI2C0->STDR = val;
	NVIC_Enable_Peripheral_Interrupt(LPI2C0_Slave_IRQn);
}
void LPI2C0_Slave_IRQHandler()
{
	uint32_t status = LPI2C0->SSR;
	if(status & LPI2C_SSR_AM0F_MASK)
		{
			__asm volatile ("NOP");
			__asm volatile ("NOP");
			__asm volatile ("NOP");
			LPI2C0->SSR |= LPI2C_SSR_AM0F_MASK;
			uint32_t read_reg = LPI2C0->SASR;
			if((read_reg & LPI2C_SASR_RADDR_MASK )== ((SLAVE_ADDRESS<<1) | 0))
			{
				i2c_state = 0;
			}
			if((read_reg & LPI2C_SASR_RADDR_MASK ) == ((SLAVE_ADDRESS<<1) | 1))
			{
				i2c_state = 1;
			}
		}
	if(status & LPI2C_SSR_RDF_MASK)
	{
		data = (uint8_t) (LPI2C0->SRDR & LPI2C_SRDR_DATA_MASK);
		if(i2c_cb != NULL && i2c_state == 0)
		{
			i2c_cb(data);
		}
	}
}
void i2c_master_init()
{
	/*MCFGR1 and MCCR0 to set I2C Bus
	 * 		MCFGR1: Nothing to do since use as reset value
	 * 		The MCCR0 cannot be changed when the I2C master is enabled and is used for standard
	 * 		MCCR0: SETHOLD=0x04 CLKI=0x05 DATAVD=0x02 CLKO=0x0B
	 * MCR[MEN] = 1;
	 * Enable interrupt:
	 * 			MIER[TXD, RXD, ALF, NDF, PTLF
	 * Write transmit command and slave address
	 * Write data command
	 * */
	PCC->PCCn[PCC_LPI2C0_INDEX] &= ~(PCC_PCCn_CGC_MASK);
	PCC->PCCn[PCC_LPI2C0_INDEX] &= ~(PCC_PCCn_PCS_MASK);
	PCC->PCCn[PCC_LPI2C0_INDEX] |= (0x02 << PCC_PCCn_PCS_SHIFT); /*the field can only be written when CGC off*/
	PCC->PCCn[PCC_LPI2C0_INDEX] |= PCC_PCCn_CGC_MASK;

	LPI2C0->MCFGR1 &= ~(LPI2C_MCFGR1_PINCFG_MASK | LPI2C_MCFGR1_PRESCALE_MASK | LPI2C_MCFGR1_IGNACK_MASK);

	LPI2C0->MCCR0 &= ~(LPI2C_MCCR0_SETHOLD_MASK | LPI2C_MCCR0_CLKHI_MASK | LPI2C_MCCR0_CLKLO_MASK | LPI2C_MCCR0_DATAVD_MASK);
	LPI2C0->MCCR0 |= (LPI2C_MCCR0_SETHOLD(0x04)|LPI2C_MCCR0_CLKHI(0x05)|LPI2C_MCCR0_CLKLO(0x0B)|LPI2C_MCCR0_DATAVD(0x02));

	LPI2C0->MCR |= LPI2C_MCR_MEN_MASK;
	//LPI2C0->MIER |= (LPI2C_MIER_ALIE_MASK | LPI2C_MIER_NDIE_MASK);
}
void i2c_slave_init()
{
	/*SCFGR1 to set 7 bit address
	 * SAMR for slave address
	 * SIER for slave interrupt
	 * Define ISR
	 * set SEN
	 * */
	PCC->PCCn[PCC_LPI2C0_INDEX] &= ~(PCC_PCCn_CGC_MASK);
	PCC->PCCn[PCC_LPI2C0_INDEX] &= ~(PCC_PCCn_PCS_MASK);
	PCC->PCCn[PCC_LPI2C0_INDEX] = ((0x03 << PCC_PCCn_PCS_SHIFT) | PCC_PCCn_CGC_MASK); /*the field can only be written when CGC off*/

//	LPI2C0->MCR |= LPI2C_MCR_RST_MASK;
//	LPI2C0->MCR &= ~LPI2C_MCR_RST_MASK;
//	LPI2C0->MCR = 0x00;
////	LPI2C0->SCR = LPI2C_SCR_RST_MASK;
////	LPI2C0->SCR = 0x0;
//	LPI2C0->SCFGR1 &= ~(LPI2C_SCFGR1_ADDRCFG_MASK); //set 7bit
//	LPI2C0->SAMR = (SLAVE_ADDRESS)<<1;
//	LPI2C0->SCFGR2 |= (LPI2C_SCFGR2_CLKHOLD(0x24)| LPI2C_SCFGR2_DATAVD(0x02));
//	LPI2C0->SCFGR1 |=  (LPI2C_SCFGR1_TXDSTALL_MASK) | (LPI2C_SCFGR1_RXSTALL_MASK) |
//						 (LPI2C_SCFGR1_ADRSTALL_MASK);
//	LPI2C0->SIER |= (LPI2C_SIER_RDIE_MASK);

	LPI2C0->MCR = LPI2C_MCR_RST_MASK;
	LPI2C0->MCR = 0x00000000;

	    /*  Configure Slave Address */
	LPI2C0->SAMR = LPI2C_SAMR_ADDR0(SLAVE_ADDRESS);
	    /*  Interrupt Enables */
	LPI2C0->SIER = LPI2C_SIER_RDIE_MASK ;
	    /*  Enable Slave Mode */
	//LPI2C0->SCFGR2 |= (LPI2C_SCFGR2_CLKHOLD(0x24)| LPI2C_SCFGR2_DATAVD(0x02));
	//	LPI2C0->SCFGR1 |=  (LPI2C_SCFGR1_TXDSTALL_MASK);
	//						 (LPI2C_SCFGR1_ADRSTALL_MASK);
	LPI2C0->STDR =(uint32_t) request;
	LPI2C0->SCR = LPI2C_SCR_SEN_MASK;
	NVIC_Set_Peripheral_Priority(LPI2C0_Slave_IRQn, 0);
	NVIC_Enable_Peripheral_Interrupt(LPI2C0_Slave_IRQn);
}

master_status_t i2c_master_write(uint8_t add, uint8_t* buff, int len);
void i2c_slave_cb_register(i2c_slave_cb_t cb)
{
	if(cb != NULL)
	{
		i2c_cb = cb;
	}
}
master_status_t i2c_master_read(uint8_t add, uint8_t* buff, int len);
