/**
  ******************************************************************************
  * @file    datalog_application.h
  * @author  SRA - Central Labs
  * @version V2.2.0
  * @date    17-Jan-2020
  * @brief   Header for datalog_application.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATALOG_APPLICATION_H
#define __DATALOG_APPLICATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cube_hal.h"
#include "lsm6dsm_settings.h"
#include "lsm303agr_settings.h"
#include "SensorTile_env_sensors.h"
#include "hts221_settings.h"
#include "lps22hb_settings.h"
#include "SensorTile_motion_sensors.h"
#include "SensorTile_motion_sensors_ex.h"
  
#define SAMPLING_50Hz 
//#define SAMPLING_100Hz 

#if defined( SAMPLING_50Hz)
  #define DEFAULT_uhCCR1_Val 190
  #define ACCELERO_ODR 416.0f     /* after filtering will be 46.2 Hz */
  #define ACCELERO_DIV LSM6DSM_ACC_GYRO_HPCF_XL_DIV9
  #define GYRO_ODR 52.0f
  #define MAGNETO_ODR 50.0f
  #define PRESSURE_ODR 50.0f
  #define DATA_PERIOD_MS     (20)

#elif defined( SAMPLING_100Hz)
  #define DEFAULT_uhCCR1_Val 100
  #define ACCELERO_ODR  833.0f    /* after filtering will be 93 Hz */
  #define ACCELERO_DIV LSM6DSM_ACC_GYRO_HPCF_XL_DIV9
  #define GYRO_ODR 104.0f
  #define MAGNETO_ODR 100.0f
  #define PRESSURE_ODR 50.0f
  #define DATA_PERIOD_MS     (10)
#endif

#define TEMPERATURE_ODR 12.5f
#define HUMIDITY_ODR 12.5f

typedef enum
{
  USB_Datalog = 0,
  SDCARD_Datalog
} LogInterface_TypeDef;


typedef struct
{
  uint32_t ms_counter;
  float pressure;
  float humidity;
  float temperature;
  BSP_MOTION_SENSOR_Axes_t acc;
  BSP_MOTION_SENSOR_Axes_t gyro;
  BSP_MOTION_SENSOR_Axes_t mag;
} T_SensorsData;
  
extern LogInterface_TypeDef LoggingInterface;
extern volatile uint8_t SD_Log_Enabled;

void floatToInt( float in, int32_t *out_int, int32_t *out_dec, int32_t dec_prec );

void DATALOG_SD_Init(void);
uint8_t DATALOG_SD_Log_Enable(void);
uint8_t DATALOG_SD_writeBuf(char *s, uint32_t size);
void DATALOG_SD_Log_Disable(void);
void DATALOG_SD_DeInit(void);
void DATALOG_SD_NewLine(void);
int32_t getSensorsData( T_SensorsData *mptr);

void MX_X_CUBE_MEMS1_Init(void);
int32_t DoubleTap(void);

#ifdef __cplusplus
}
#endif

#endif /* __DATALOG_APPLICATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
