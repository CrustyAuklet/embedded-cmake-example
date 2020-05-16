/**
******************************************************************************
* @file    SensorTile_conf_template.h
* @author  SRA - Central Labs
* @version v2.1.2
* @date    11-Jun-2019
* @brief   This file contains definitions for the components bus interfaces
*          This file should be copied to the application folder and renamed
*          to SensorTile_conf.h. 
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORTILE_CONF_H__
#define __SENSORTILE_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "SensorTile_bus.h"
#include "SensorTile_errno.h"
  
#define AUDIO_CHANNELS 			1
#define AUDIO_SAMPLING_FREQUENCY 	16000
  
#define BSP_AUDIO_IN_INSTANCE           1U   /* Define the audio peripheral used: 1U = DFSDM */  
#define AUDIO_IN_BUFFER_SIZE            DEFAULT_AUDIO_IN_BUFFER_SIZE  
#define AUDIO_VOLUME_INPUT              64U
#define BSP_AUDIO_IN_IT_PRIORITY        6U
  
#define BSP_AUDIO_OUT_INSTANCE          0U   /* Define the audio peripheral used: 0U = SAI */ 
#define AUDIO_VOLUME_OUTPUT             32U
#define BSP_AUDIO_OUT_IT_PRIORITY       7U
  
/*Comment this define if you want to configure and start acquisition 
depending on USB functionalities implemented by user*/
#define DISABLE_USB_DRIVEN_ACQUISITION

#define USE_MOTION_SENSOR_LSM6DSM_0        1U
#define USE_MOTION_SENSOR_LSM303AGR_ACC_0  0U
#define USE_MOTION_SENSOR_LSM303AGR_MAG_0  1U
#define USE_ENV_SENSOR_HTS221_0            1U
#define USE_ENV_SENSOR_LPS22HB_0           1U
  
#define BSP_LSM6DSM_INT2_GPIO_PORT           GPIOA
#define BSP_LSM6DSM_INT2_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define BSP_LSM6DSM_INT2_GPIO_CLK_DISABLE()  __GPIOA_CLK_DISABLE()
#define BSP_LSM6DSM_INT2                 GPIO_PIN_2
#define BSP_LSM6DSM_INT2_EXTI_IRQn           EXTI2_IRQn  

#define BSP_LSM6DSM_CS_PORT GPIOB
#define BSP_LSM6DSM_CS_PIN GPIO_PIN_12
#define BSP_LSM6DSM_CS_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

#define BSP_LSM303AGR_M_CS_PORT GPIOB
#define BSP_LSM303AGR_M_CS_PIN GPIO_PIN_1
#define BSP_LSM303AGR_M_CS_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
                                                
#define BSP_LSM303AGR_X_CS_PORT GPIOC
#define BSP_LSM303AGR_X_CS_PIN GPIO_PIN_4
#define BSP_LSM303AGR_X_CS_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()

#define BSP_LPS22HB_CS_PORT GPIOA
#define BSP_LPS22HB_CS_PIN GPIO_PIN_3
#define BSP_LPS22HB_CS_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* __SENSORTILE_CONF_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

