/**
  ******************************************************************************
  * @file    main.c
  * @author  SRA - Central Labs
  * @version V2.2.0
  * @date    17-Jan-2020
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "datalog_application.h"
    
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DATAQUEUE_SIZE     ((uint32_t)100)

#define DATALOG_CMD_STARTSTOP  (0x00000007)
    
typedef enum
{
  THREAD_1 = 0,
  THREAD_2
} Thread_TypeDef;
  
/* Private variables ---------------------------------------------------------*/

osThreadId GetDataThreadId, WriteDataThreadId;

osMessageQId dataQueue_id;
osMessageQDef(dataqueue, DATAQUEUE_SIZE, int);

osPoolId sensorPool_id;
osPoolDef(sensorPool, DATAQUEUE_SIZE, T_SensorsData);

osSemaphoreId readDataSem_id;
osSemaphoreDef(readDataSem);

/* LoggingInterface = USB_Datalog  --> Send sensors data via USB */
/* LoggingInterface = SDCARD_Datalog  --> Save sensors data on SDCard (enable with double tap) */
LogInterface_TypeDef LoggingInterface = USB_Datalog;

USBD_HandleTypeDef  USBD_Device;
static volatile uint8_t MEMSInterrupt = 0;
volatile uint8_t no_H_HTS221 = 0;
volatile uint8_t no_T_HTS221 = 0;

/* Private function prototypes -----------------------------------------------*/
static void GetData_Thread(void const *argument);
static void WriteData_Thread(void const *argument);

static void Error_Handler( void );
void dataTimer_Callback(void const *arg);
void dataTimerStart(void);
void dataTimerStop(void);

osTimerId sensorTimId;
osTimerDef(SensorTimer, dataTimer_Callback);

uint32_t  exec;

static int32_t LSM6DSM_Sensor_IO_ITConfig( void );
      
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  HAL_Init();

  /* Configure the System clock to 80 MHz */
  SystemClock_Config();
  
  if(LoggingInterface == USB_Datalog)
  {
    /* Initialize LED */
    BSP_LED_Init(LED1);
    BSP_LED_Off(LED1);
  }
 
  /* enable USB power on Pwrctrl CR2 register */
  HAL_PWREx_EnableVddUSB();
  HAL_PWREx_EnableVddIO2();
  
  if(LoggingInterface == USB_Datalog) /* Configure the USB */
  {
    /*** USB CDC Configuration ***/
    /* Init Device Library */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);
    /* Add Supported Class */
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);
    /* Add Interface callbacks for AUDIO and CDC Class */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);
    /* Start Device Process */
    USBD_Start(&USBD_Device);
  }
  else /* Configure the SDCard */
  {
    DATALOG_SD_Init();
  }
  
  /* Thread 1 definition */
  osThreadDef(THREAD_1, GetData_Thread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE*4);
  
  /* Thread 2 definition */
  osThreadDef(THREAD_2, WriteData_Thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*4);
  
  /* Start thread 1 */
  GetDataThreadId = osThreadCreate(osThread(THREAD_1), NULL);

  /* Start thread 2 */
  WriteDataThreadId = osThreadCreate(osThread(THREAD_2), NULL);  
  
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for (;;);

}

/**
  * @brief  Get data raw from sensors to queue
  * @param  thread not used
  * @retval None
  */
static void GetData_Thread(void const *argument)
{
  (void) argument;
  T_SensorsData *mptr;
  
  sensorPool_id = osPoolCreate(osPool(sensorPool));     
  dataQueue_id = osMessageCreate(osMessageQ(dataqueue), NULL);
  
  readDataSem_id = osSemaphoreCreate(osSemaphore(readDataSem), 1);
  osSemaphoreWait(readDataSem_id, osWaitForever);
  
  /* Initialize and Enable the available sensors */
  MX_X_CUBE_MEMS1_Init();
  
  /* COnfigure LSM6DSM Double Tap interrupt*/  
  LSM6DSM_Sensor_IO_ITConfig();
  
  if(LoggingInterface == USB_Datalog)
  {
    dataTimerStart();
  }
  
  for (;;)
  {
    osSemaphoreWait(readDataSem_id, osWaitForever);
    if(MEMSInterrupt && LoggingInterface == SDCARD_Datalog)
    {
      MEMSInterrupt = 0;
      
      if(DoubleTap())
      {
        if(SD_Log_Enabled) 
        {
          dataTimerStop();
          osMessagePut(dataQueue_id, DATALOG_CMD_STARTSTOP, osWaitForever);
        }
        else
        {
          osMessagePut(dataQueue_id, DATALOG_CMD_STARTSTOP, osWaitForever);
        }
      }
    }
    else
    {
      /* Try to allocate a memory block and check if is not NULL */
      mptr = osPoolAlloc(sensorPool_id);
      if(mptr != NULL)
      {
        if(getSensorsData(mptr) == BSP_ERROR_NONE)
        {
          /* Push the new memory Block in the Data Queue */
          if(osMessagePut(dataQueue_id, (uint32_t)mptr, osWaitForever) != osOK)
          {
            Error_Handler();
          }     
        }
        else
        {
          Error_Handler();
        }
      }
      else
      {
        Error_Handler();
      }
    }
  }
}


/**
  * @brief  Write data in the queue on file or streaming via USB
  * @param  argument not used
  * @retval None
  */
static void WriteData_Thread(void const *argument)
{
  (void) argument;
  osEvent evt;
  T_SensorsData *rptr;
  int size;
  char data_s[256];
  
  for (;;)
  {
    evt = osMessageGet(dataQueue_id, osWaitForever);  // wait for message
    if (evt.status == osEventMessage)
    {
      if(evt.value.v == DATALOG_CMD_STARTSTOP)
      {
        if (SD_Log_Enabled) 
        {
          DATALOG_SD_Log_Disable();
          SD_Log_Enabled=0;
        }
        else
        {
          while(SD_Log_Enabled != 1)
          {
            if(DATALOG_SD_Log_Enable())
            {
              SD_Log_Enabled=1;
              osDelay(100);
              dataTimerStart();
            }
            else
            {
              //DATALOG_SD_Log_Disable();
              DATALOG_SD_DeInit();
              DATALOG_SD_Init();
              osDelay(100);
            }
          }
        }
      }
      else
      {
        rptr = evt.value.p;

        if(LoggingInterface == USB_Datalog)
        {
          size = sprintf(data_s, "TimeStamp: %ld\r\n Acc_X: %d, Acc_Y: %d, Acc_Z :%d\r\n Gyro_X:%d, Gyro_Y:%d, Gyro_Z:%d\r\n Magn_X:%d, Magn_Y:%d, Magn_Z:%d\r\n Press:%5.2f, Temp:%5.2f, Hum:%4.1f\r\n",
                       rptr->ms_counter,
                       (int)rptr->acc.x, (int)rptr->acc.y, (int)rptr->acc.z,
                       (int)rptr->gyro.x, (int)rptr->gyro.y, (int)rptr->gyro.z,
                       (int)rptr->mag.x, (int)rptr->mag.y, (int)rptr->mag.z,
                       rptr->pressure, rptr->temperature, rptr->humidity);
          osPoolFree(sensorPool_id, rptr);      // free memory allocated for message
          BSP_LED_Toggle(LED1);
          CDC_Fill_Buffer(( uint8_t * )data_s, size);
        }
        else
        {
          size = sprintf(data_s, "%ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %5.2f, %5.2f, %4.1f\r\n",
                       rptr->ms_counter,
                       (int)rptr->acc.x, (int)rptr->acc.y, (int)rptr->acc.z,
                       (int)rptr->gyro.x, (int)rptr->gyro.y, (int)rptr->gyro.z,
                       (int)rptr->mag.x, (int)rptr->mag.y, (int)rptr->mag.z,
                       rptr->pressure, rptr->temperature, rptr->humidity);
          osPoolFree(sensorPool_id, rptr);      // free memory allocated for message
          DATALOG_SD_writeBuf(data_s, size);
        }
      }
    }
  }
}


void dataTimer_Callback(void const *arg)
{ 
  osSemaphoreRelease(readDataSem_id);
} 


void dataTimerStart(void)
{
  osStatus  status;
 
  // Create periodic timer
  exec = 1;
  sensorTimId = osTimerCreate(osTimer(SensorTimer), osTimerPeriodic, &exec);
  if (sensorTimId)  {
    status = osTimerStart (sensorTimId, DATA_PERIOD_MS);                // start timer
    if (status != osOK)  {
      // Timer could not be started
    } 
  }
}

void dataTimerStop(void)
{
  osTimerStop(sensorTimId);
}



/**
 * @brief  Configures sensor interrupts interface for LSM6DSM sensor.
 * @param  None
 * @retval BSP_ERROR_NONE in case of success
 * @retval COMPONENT_ERROR in case of failure
 */
int32_t LSM6DSM_Sensor_IO_ITConfig( void )
{

  /* At the moment this feature is only implemented for LSM6DSM */
  GPIO_InitTypeDef GPIO_InitStructureInt2;
  
  /* Enable INT2 GPIO clock */
  BSP_LSM6DSM_INT2_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructureInt2.Pin = BSP_LSM6DSM_INT2;
  GPIO_InitStructureInt2.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructureInt2.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructureInt2.Pull  = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_LSM6DSM_INT2_GPIO_PORT, &GPIO_InitStructureInt2);
  
  /* Enable and set EXTI Interrupt priority */
  HAL_NVIC_SetPriority(BSP_LSM6DSM_INT2_EXTI_IRQn, 0x08, 0x00);
  HAL_NVIC_EnableIRQ(BSP_LSM6DSM_INT2_EXTI_IRQn);
  
  return BSP_ERROR_NONE;
}



/**
* @brief  EXTI line detection callbacks
* @param  GPIO_Pin: Specifies the pins connected EXTI line
* @retval None
*/
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
  MEMSInterrupt=1;
  osSemaphoreRelease(readDataSem_id);
}

/**
* @brief  This function is executed in case of error occurrence
* @param  None
* @retval None
*/
static void Error_Handler( void )
{
  while (1)
  {}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
