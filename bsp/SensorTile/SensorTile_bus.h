/**
******************************************************************************
* @file    SensorTile_bus.h
* @author  SRA - Central Labs
* @version v2.1.2
* @date    11-Jun-2019
* @brief   This file provides BSP BUS IO Driver
******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORTILE_BUS_H
#define __SENSORTILE_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif


#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1) 
typedef struct
{
  pSPI_CallbackTypeDef  pMspSpiInitCb;
  pSPI_CallbackTypeDef  pMspSpiDeInitCb;
}BSP_SPI_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */
 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* BUS IO driver over I2C Peripheral */
int32_t BSP_I2C3_Init(void);
int32_t BSP_I2C3_DeInit(void);
int32_t BSP_I2C3_IsReady(void);
int32_t BSP_I2C3_WriteReg(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t len);
int32_t BSP_I2C3_ReadReg(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t len);
int32_t BSP_I2C3_WriteReg16(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t len);
int32_t BSP_I2C3_ReadReg16(uint16_t Addr, uint16_t Reg, uint8_t *pData, uint16_t len);
int32_t BSP_I2C3_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int32_t BSP_I2C3_SendRecv(uint16_t DevAddr, uint8_t *pTxdata, uint8_t *pRxdata, uint16_t Length);
/* BUS IO driver over SPI2 Peripheral */
int32_t BSP_SPI2_Init(void);
int32_t BSP_SPI2_DeInit(void);
int32_t BSP_SPI2_Send(uint8_t *pData, uint16_t len);
int32_t BSP_SPI2_Recv(uint8_t *pData, uint16_t len);
int32_t BSP_SPI2_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len);
/* BUS IO driver over SPI1 Peripheral */
int32_t BSP_SPI1_Init(void);
int32_t BSP_SPI1_DeInit(void);
int32_t BSP_SPI1_Send(uint8_t *pData, uint16_t len);
int32_t BSP_SPI1_Recv(uint8_t *pData, uint16_t len);
int32_t BSP_SPI1_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len);

int32_t BSP_GetTick(void);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_BUS_RegisterDefaultMspCallbacks (void);
int32_t BSP_BUS_RegisterMspCallbacks (BSP_BUS_Cb_t *Callbacks);
#endif /* ((USE_HAL_I2C_REGISTER_CALLBACKS == 1) || (USE_HAL_SPI_REGISTER_CALLBACKS == 1)) */

#ifdef __cplusplus
}
#endif

#endif /* __SENSORTILE_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
