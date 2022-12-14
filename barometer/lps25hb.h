

#include "main.h"
#include "i2c.h"


#define 	LPS25HB_DEVICE_ADDRESS_0				0xB9
#define 	LPS25HB_DEVICE_ADDRESS_1				0xBB

#define 	LPS25HB_WHO_AM_I_VALUE					0xBD
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS25HB_ADDRESS_CTRL1					0x20

#define 	LPS25HB_ADDRESS_PRESS_XL				0x28
#define 	LPS25HB_ADDRESS_PRESS_L					0x29
#define 	LPS25HB_ADDRESS_PRESS_H					0x2A

uint8_t lps25hb_init(void);
void lps25hb_get_pressure(float* pressure);
void lps25hb_get_hight(float* hight);
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);
