/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "RTC.h"
#include "LiquidCrystal_I2C.h"
#include "Btn.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
LiquidCrystal_I2C lcd;
RTC_typedef hrtc ;
Button_Typdef btn_up, btn_ok,btn_down;
int8_t hour_alarm;
int8_t min_alarm;

const char arr_day [] [4] = {
		{"SUN"},
		{"MON"},
		{"TUE"},
		{"WED"},
		{"THU"},
		{"FRI"},
		{"SAT"},

};
void clock_time_update()
{
		static uint32_t t_update =0;
		  if(HAL_GetTick() - t_update >= 500)
		  {
		  RTC_ReadTime(&hrtc,&hrtc.datetime);
		  uint8_t day_of_week = RTC_READ_DAY_OF_WEEK(&hrtc.datetime );
		  lcd_set_cursor(&lcd,0,3);
		  lcd_printf(&lcd, "%02d:%02d:%02d",hrtc.datetime.hour,hrtc.datetime.min,hrtc.datetime.sec);
		  lcd_set_cursor(&lcd, 1, 0);
		  lcd_printf(&lcd, "%s %02d/%02d/20%02d",arr_day[day_of_week],hrtc.datetime.date,hrtc.datetime.month,hrtc.datetime.year);
		  t_update = HAL_GetTick();
		  }
}
typedef enum
{
	NORMAL_STATE,
	SETTING_HOUR_STATE,
	SETTING_MIN_STATE,
	SETTING_SEC_STATE,
	SETTING_DATE_STATE,
	SETTING_MONTH_STATE,
	SETTING_YEAR_STATE,
	ALARM_HOUR_STATE,
	ALARM_MIN_STATE

}ClockState;
ClockState clock_state = NORMAL_STATE;
//----------utils----------------
void up_down_number(int8_t *number, int8_t max,int8_t min, int8_t plus)
{
	if(plus)
	{
		(*number)++;
	if( *number > max)
	{
		*number = min;
	}
	}
	else
	{
		(*number)--;
	if(*number < min)
	{
		*number = max;
	}
	}
}

uint8_t check_leap_year(uint16_t year)
{
	if (((year % 4 == 0) && (year % 100!= 0)) || (year%400 == 0))
	{
		return 1;
	}
	    return 0;
}

uint8_t get_max_date(uint8_t month, uint16_t year)
{
	switch(month)
	{
	case 1: case 3 : case 5 : case 7 : case 8 : case 10 : case 12 :
		return 31;
	case 4: case 6 : case 9 : case 11 :
		return 30;
	case 2 :
		if(check_leap_year(year))
		{
			return 29;
		}
	}
	return 28;

}


//----------blink----------------
void setting_blink()
{
	static uint32_t t_blink =0;
	static uint8_t is_show =1;
	char line1[16] ;
	char line2[16] ;
			  if(HAL_GetTick() - t_blink >= 300)
				 {
				  is_show = ! is_show;
					 RTC_ReadTime(&hrtc,&hrtc.datetime);
					 uint8_t day_of_week = RTC_READ_DAY_OF_WEEK(&hrtc.datetime );
 sprintf(line1, "%02d:%02d:%02d",hrtc.datetime.hour,hrtc.datetime.min,hrtc.datetime.sec);
 sprintf(line2, "%s %02d/%02d/20%02d",arr_day[day_of_week],hrtc.datetime.date,hrtc.datetime.month,hrtc.datetime.year);

					 if(is_show)
				  {

					switch(clock_state)
				 {
					 case SETTING_HOUR_STATE:
						line1[0] = line1[1] = ' ';
						 break;
					 case SETTING_MIN_STATE:
						line1[3] = line1[4] = ' ';
						 break;
					 case SETTING_DATE_STATE:
						line2[4] = line2[5] = ' ';
						 break;
					 case SETTING_MONTH_STATE:
						 line2[7] = line2[8] =' ';
						 break;
					 case SETTING_YEAR_STATE:
						 line2[12]= line2[13] = ' ';
						 break;
					 default : break;

				 }

				  }
					  lcd_set_cursor(&lcd,0,3);
					  lcd_printf(&lcd, "%s",line1);
					  lcd_set_cursor(&lcd, 1, 0);
					  lcd_printf(&lcd, "%s ",line2);
					 t_blink = HAL_GetTick();
			  }
}

// ----------ajust_time----------
void ajust_time(uint8_t plus)
{
	switch(clock_state)
				{
				case SETTING_HOUR_STATE:
				{
					up_down_number(&hrtc.datetime.hour,23,0,plus);
					RTC_WriteTime(&hrtc, &hrtc.datetime);
					break;
				}
				case SETTING_MIN_STATE:
				{
					up_down_number(&hrtc.datetime.min,59,0,plus);
					RTC_WriteTime(&hrtc, &hrtc.datetime);
					break;
				}
				case SETTING_DATE_STATE:
				{
					uint8_t max_date = get_max_date(hrtc.datetime.month, hrtc.datetime.year);
					up_down_number(&hrtc.datetime.date,max_date,1,plus);
					RTC_WriteTime(&hrtc, &hrtc.datetime);
					break;
				}
				case SETTING_MONTH_STATE:
				{
					up_down_number(&hrtc.datetime.month,12,1,plus);
					uint8_t max_date = get_max_date(hrtc.datetime.month, hrtc.datetime.year);
					if(hrtc.datetime.date > max_date)
					{
						hrtc.datetime.date = max_date;
					}
					RTC_WriteTime(&hrtc, &hrtc.datetime);
					break;
				}
				case SETTING_YEAR_STATE:
				{
					up_down_number(&hrtc.datetime.year,99,0,plus);
					RTC_WriteTime(&hrtc, &hrtc.datetime);
					break;
				}
				case ALARM_HOUR_STATE:
				{
					up_down_number(&hour_alarm,23,0,plus);
					break;
				}
				case ALARM_MIN_STATE:
				{
					up_down_number(&min_alarm,59,0,plus);
					break;
				}

				default : break;
				}
}

// ---------callback_button---------------------
void btn_pressing_callback(Button_Typdef *ButtonX)
{
	if(ButtonX == &btn_up)
		{
		ajust_time(1);
		}
	else if(ButtonX == &btn_down)
		{
		ajust_time(0);
		}
}
void btn_press_short_callback(Button_Typdef *ButtonX)
{
	if(ButtonX == &btn_ok)
	{
		switch(clock_state)
		{
		case NORMAL_STATE:
			clock_state = SETTING_HOUR_STATE;
			break;
		case SETTING_HOUR_STATE:
			clock_state = SETTING_MIN_STATE;
			break;
		case SETTING_MIN_STATE:
			clock_state = SETTING_DATE_STATE;
			break;
		case SETTING_DATE_STATE:
			clock_state = SETTING_MONTH_STATE;
			break;
		case SETTING_MONTH_STATE:
			clock_state = SETTING_YEAR_STATE;
			break;
		case SETTING_YEAR_STATE:
			clock_state = NORMAL_STATE;
			break;
		case ALARM_HOUR_STATE:
			clock_state = ALARM_MIN_STATE;
			break;
		case ALARM_MIN_STATE:
			clock_state = NORMAL_STATE;
			break;
		default : break;

		}

	}

}
void btn_press_timeout_callback(Button_Typdef *ButtonX) // nhan giu
{
	if(ButtonX == &btn_ok)
		{
			clock_state = ALARM_HOUR_STATE;
			lcd_clear_display(&lcd);
			lcd_set_cursor(&lcd,0,5);
			lcd_printf(&lcd, "ALARM");
		}
}

//------------alarm------------
void check_alarm()
{
	if((hour_alarm == hrtc.datetime.hour) && (min_alarm == hrtc.datetime.min))
	{
				// todo
	}
}

void alarm_screen()
{
	static uint32_t t_blink =0;
	static uint8_t is_show =1;
	char line2[16] ;
	if(HAL_GetTick() - t_blink >= 300)
	 {
		is_show = ! is_show;
	    RTC_ReadTime(&hrtc,&hrtc.datetime);
/*		uint8_t day_of_week = RTC_READ_DAY_OF_WEEK(&hrtc.datetime);*/
   sprintf(line2,"%02d:%02d",hour_alarm,min_alarm);
		if(is_show)
		{
			switch(clock_state)
			{
			case ALARM_HOUR_STATE:
				line2[0] = line2[1] = ' ';
				break;
			case ALARM_MIN_STATE:
				line2[3] = line2[4] = ' ';
				break;
			default : break;
			}
		 }
			lcd_set_cursor(&lcd,0,5);
			lcd_printf(&lcd, "ALARM");
			lcd_set_cursor(&lcd, 1, 5);
			lcd_printf(&lcd, "%s ",line2);
			t_blink = HAL_GetTick();
	 }
}


// ---------------handle_clock------------
void clock_handle()
{
	switch(clock_state)
				{
	case NORMAL_STATE:

		clock_time_update();
		check_alarm();
		 break;
	case SETTING_HOUR_STATE:
	case SETTING_MIN_STATE:
	case SETTING_DATE_STATE:
	case SETTING_MONTH_STATE:
	case SETTING_YEAR_STATE:
		setting_blink();
		break;
	case ALARM_HOUR_STATE:
	case ALARM_MIN_STATE:
		alarm_screen();
		break;
	default : break;
				}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  lcd_init(&lcd, &hi2c1, LCD_ADDR_DEFAULT);
  RTC_Init(&hrtc ,&hi2c1);
  button_init(&btn_ok, GPIOA, GPIO_PIN_1);
  button_init(&btn_down, GPIOA, GPIO_PIN_0);
  button_init(&btn_up, GPIOA, GPIO_PIN_2);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
/*  hrtc.datetime.hour = 11;
  hrtc.datetime.min = 41;
  hrtc.datetime.sec = 00;
  hrtc.datetime.day = 2;
  hrtc.datetime.date = 19;
  hrtc.datetime.month = 2;
  hrtc.datetime.year = 24;
  RTC_WriteTime(&hrtc,&hrtc.datetime);*/



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  button_handle(&btn_up);
	  button_handle(&btn_ok);
	  button_handle(&btn_down);
	  clock_handle();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
