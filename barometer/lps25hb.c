/*
 * lps25hb.c
 *
 *  Created on: 07. 11. 2022
 *      Author: LukasHasprun
 */

#include "lps25hb.h"

uint8_t address_lps = LPS25HB_DEVICE_ADDRESS_0;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_lps, 0));
}


void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_lps, 0);
}


void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_lps, 1);
}

void lps25hb_get_pressure(float* press)
{
//	uint16_t data;
//	int16_t xx, yy, zz;
//
//	uint8_t temp;
//
	//	get current scale and use it for final calculation
//    temp = lsm6ds0_read_byte(LSM6DS0_ADDRESS_CTRL1);
//
//	temp = temp >> 2;
//    temp &= 0x03;			//full scale bits exctracted
//
//	lps25hb_readArray(data, LPS25HB_ADDRESS_PRESS, 6);
//
//	xx = ((uint16_t)data[1]) << 8 | data[0];
//	yy = ((uint16_t)data[3]) << 8 | data[2];
//	zz = ((uint16_t)data[5]) << 8 | data[4];
//
//	*x = (xx >> 4) / 1000.0f;
//	*y = (yy >> 4) / 1000.0f;
//	*z = (zz >> 4) / 1000.0f;
//	*press = data;
//	int32_t raw_press = 0;

//	uint32_t pbaroXLo, pbaroLo, pbaroHi;
//	pbaroXLo = lps25hb_read_byte(LPS25HB_ADDRESS_PRESS_XL);
//	pbaroLo = lps25hb_read_byte(LPS25HB_ADDRESS_PRESS_L);
//	pbaroHi = lps25hb_read_byte(LPS25HB_ADDRESS_PRESS_H);
//	uint16_t data;
//	data[0] = (float)((((pbaroHi << 16) | (pbaroLo << 8) | pbaroXLo)) / 4096.0);
//	*press = data;
//	uint16_t data;
//	lps25hb_readArray(data, LPS25HB_ADDRESS_PRESS_XL, 3);
//	int16_t pressPress = ((uint16_t)data[1]) << 8 | data[0];
//	*press = (pressPress >> 4) / 4096.0f;
	uint8_t pressOutXL = lps25hb_read_byte(LPS25HB_ADDRESS_PRESS_XL);
	uint8_t pressOutL = lps25hb_read_byte(LPS25HB_ADDRESS_PRESS_L);
	uint8_t pressOutH = lps25hb_read_byte(LPS25HB_ADDRESS_PRESS_H);


	uint16_t data = (((uint16_t)pressOutH << 24) | ((uint16_t)pressOutL << 16) | ((uint16_t)pressOutXL << 8)) >> 8;
	*press = data;
}


uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		address_lps = LPS25HB_DEVICE_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			//return status;
		}
	}

	//acc device init

	uint8_t ctrl1 = 8 << 4; // +-2g res
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}
