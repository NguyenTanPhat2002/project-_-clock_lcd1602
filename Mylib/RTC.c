/*
 * RTC.c
 *
 *  Created on: Feb 16, 2024
 *      Author: LAPTOP
 */

#include "RTC.h"


uint8_t Decimal2BCD(uint8_t num)
{
return (num/10)<<4|(num%10);
}
uint8_t BCD2Decimal(uint8_t num)
{
return (num>>4)*10+(num&0x0F);
}

void RTC_WriteTime(RTC_typedef *rtc ,Datetime *dt)
{
	uint8_t buff[8];
	buff[0] = 0x00;
	buff[1] = Decimal2BCD (dt ->sec);
	buff[2] = Decimal2BCD (dt ->min);
	buff[3] = Decimal2BCD (dt ->hour);
	buff[4] = Decimal2BCD (dt -> day);
	buff[5] = Decimal2BCD (dt ->date);
	buff[6] = Decimal2BCD (dt -> month);
	buff[7] = Decimal2BCD (dt ->year);

	HAL_I2C_Master_Transmit(rtc -> rtc_i2c, RTC_ADDR, buff, 8, 100);
}
void RTC_ReadTime(RTC_typedef *rtc ,Datetime *dt)
{
	uint8_t buff[7];
	uint8_t addr_reg = 0x00;
	HAL_I2C_Master_Transmit(rtc -> rtc_i2c, RTC_ADDR, &addr_reg, 1, 100);
	HAL_I2C_Master_Receive(rtc -> rtc_i2c, RTC_ADDR, buff, 7, 100);
	dt -> sec = BCD2Decimal (buff[0]);
	dt -> min = BCD2Decimal (buff[1]);
	dt -> hour = BCD2Decimal (buff[2]);
	dt -> day = BCD2Decimal (buff[3]);
	dt -> date = BCD2Decimal (buff[4]);
	dt -> month = BCD2Decimal (buff[5]);
	dt -> year = BCD2Decimal (buff[6]);

}
uint8_t RTC_READ_DAY_OF_WEEK(Datetime *dt)
{
	uint16_t d = dt->date;
	uint16_t m = dt->month;
	uint16_t y = 2000+ dt->year;
	uint8_t weekday = (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;
	return weekday;
}
void RTC_Init(RTC_typedef *rtc ,I2C_HandleTypeDef *_rtc_i2c)
{
	rtc -> rtc_i2c = _rtc_i2c;
}
