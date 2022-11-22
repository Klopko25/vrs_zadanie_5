/*
 * lps25hb.c
 *
 *  Created on: 07. 11. 2022
 *      Author: LukasHasprun
 */

#include "hts221.h"

uint8_t address_hts = HTS221_DEVICE_ADDRESS;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address_hts, 0));
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address_hts, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, address_hts, 1);
}

void hts221_get_humidity(float* press)
{
	int16_t H0_T0_out, H1_T0_out, H_T_out;
	int16_t H0_rh, H1_rh;
	uint8_t buffer[2];
	float tmp_f;

	hts221_readArray(buffer, HTS221_H0_RH_X2, 2);
	H0_rh = buffer[0] >> 1;
	H1_rh = buffer[1] >> 1;

	hts221_readArray(buffer, HTS221_H0_T0_OUT_L, 2);
	H0_T0_out = (((uint16_t)buffer[1]) << 8) | (uint16_t)buffer[0];

	hts221_readArray(buffer, HTS221_H1_T0_OUT_L, 2);
	H1_T0_out = (((uint16_t)buffer[1]) << 8) | (uint16_t)buffer[0];

	hts221_readArray(buffer, HTS221_ADDRESS_HUMIDITY_L, 2);
	H_T_out = (((uint16_t)buffer[1]) << 8) | (uint16_t)buffer[0];

	tmp_f = (float)((((H1_rh - H0_rh)*(H_T_out - H0_T0_out))/(H1_T0_out - H0_T0_out)) + H0_rh);
	*press = tmp_f;
}

void hts221_get_temp(float* temp)
{
	int16_t T0_out, T1_out, T_out, T0_degC_x8_u16, T1_degC_x8_u16;
	int16_t T0_degC, T1_degC;
	uint8_t buffer[4], tmp;
	int32_t tmp32;

	hts221_readArray(buffer, HTS221_T0_DEGC_X8, 2);
	hts221_readArray(&tmp, HTS221_T0_T1_DEGC_H2, 1);

	T0_degC_x8_u16 = (((uint16_t)(tmp & 0x03)) << 8) | ((uint16_t)buffer[0]);
	T1_degC_x8_u16 = (((uint16_t)(tmp & 0x0C)) << 6) | ((uint16_t)buffer[1]);
	T0_degC = T0_degC_x8_u16>>3;
	T1_degC = T1_degC_x8_u16>>3;

	hts221_readArray(buffer, HTS221_T0_OUT_L, 4);
	T0_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];
	T1_out = (((uint16_t)buffer[3])<<8) | (uint16_t)buffer[2];

	hts221_readArray(buffer, HTS221_TEMP_OUT_L_REG, 2);
	T_out = (((uint16_t)buffer[1])<<8) | (uint16_t)buffer[0];

	tmp32 = ((int32_t)(T_out - T0_out)) * ((int32_t)(T1_degC - T0_degC)*10);
	 *temp = (tmp32 /(T1_out - T0_out) + T0_degC*10)/10;
}

uint8_t hts221_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		status = 0;
	}

	//acc device init

	uint8_t ctrl1 = 129 << 0; // +-2g res
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}
