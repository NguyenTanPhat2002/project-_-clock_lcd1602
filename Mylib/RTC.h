/*
 * RTC.h
 *
 *  Created on: Feb 16, 2024
 *      Author: LAPTOP
 */

#ifndef RTC_H_
#define RTC_H_

#include "stm32f1xx.h"
#define RTC_ADDR 0x68<<1
typedef struct
{
	int8_t sec;
	int8_t min;
	int8_t hour;
	int8_t day;
	int8_t date;
	int8_t month;
	int8_t year;


}Datetime;

typedef struct
{
	I2C_HandleTypeDef *rtc_i2c;
	Datetime datetime;
}RTC_typedef;
void RTC_WriteTime(RTC_typedef *rtc ,Datetime *dt);

void RTC_ReadTime(RTC_typedef *rtc ,Datetime *dt);

void RTC_Init(RTC_typedef *rtc ,I2C_HandleTypeDef *_rtc_i2c);
uint8_t RTC_READ_DAY_OF_WEEK(Datetime *dt);
#endif /* RTC_H_ */
