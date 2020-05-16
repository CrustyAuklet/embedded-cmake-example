/**
  ******************************************************************************
  * @file    SensorTile_errno.h
  * @author  SRA - Central Labs
  * @version V2.2.0
  * @date    17-Jan-2020
  * @brief   Error Code
  ******************************************************************************
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SENSORTILE_ERRNO_H
#define __SENSORTILE_ERRNO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Common Error codes */
#define BSP_ERROR_NONE                    0
#define BSP_ERROR_NO_INIT                -1
#define BSP_ERROR_WRONG_PARAM            -2
#define BSP_ERROR_BUSY                   -3
#define BSP_ERROR_PERIPH_FAILURE         -4
#define BSP_ERROR_COMPONENT_FAILURE      -5
#define BSP_ERROR_UNKNOWN_FAILURE        -6
#define BSP_ERROR_UNKNOWN_COMPONENT      -7 
#define BSP_ERROR_BUS_FAILURE            -8
#define BSP_ERROR_CLOCK_FAILURE          -9
#define BSP_ERROR_MSP_FAILURE            -10

#ifdef __cplusplus
}
#endif

#endif /* __SENSORTILE_ERRNO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
