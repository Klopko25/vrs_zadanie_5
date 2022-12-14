

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
	uint8_t buffer[3];
	uint32_t combined = 0;
	float tmp_f;

	lps25hb_readArray(buffer, LPS25HB_ADDRESS_PRESS_XL, 3);
	combined = (uint32_t)buffer[2] << 16 | (uint32_t)buffer[1] << 8 | (uint32_t)buffer[0];

	tmp_f = ((float)combined/4096.0);
	*press = tmp_f;
}

//void lps25hb_get_hight(float* hight)
//{
//	uint8_t buffer[3];
//	uint32_t combined = 0;
//	float tmp_f;
//
//	lps25hb_readArray(buffer, LPS25HB_ADDRESS_PRESS_XL, 3);
//	combined = (uint32_t)buffer[2] << 16 | (uint32_t)buffer[1] << 8 | (uint32_t)buffer[0];
//
//	tmp_f = ((float)combined/4096.0);
//	tmp_f = tmp_f * 100;
//
//
//	*hight = tmp_f;
//}


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

	uint8_t ctrl1 = 9 << 4; // +-2g res
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}
