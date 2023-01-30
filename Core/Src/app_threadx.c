/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
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
/* USER CODE BEGIN PV */
TX_THREAD              SemaphoreThreadOne;
TX_THREAD              SemaphoreThreadTwo;
TX_SEMAPHORE           Sempaphore_0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void    SemaphoreThreadOne_Entry(ULONG thread_input);
void    SemaphoreThreadTwo_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer;

  /* Allocate the stack for SemaphoreThreadOne.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create SemaphoreThreadOne.  */
  if (tx_thread_create(&SemaphoreThreadOne, "Semaphore Thread One",
                   SemaphoreThreadOne_Entry, 0, pointer, APP_STACK_SIZE,
             SEMAPHORE_THREAD_ONE_PRIO, SEMAPHORE_THREAD_ONE_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for SemaphoreThreadTwo.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create SemaphoreThreadTwo.  */
  if (tx_thread_create(&SemaphoreThreadTwo, "Semaphore Thread Two",
                   SemaphoreThreadTwo_Entry, 0, pointer, APP_STACK_SIZE,
             SEMAPHORE_THREAD_TWO_PRIO, SEMAPHORE_THREAD_TWO_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  if (tx_semaphore_create(&Sempaphore_0, "semaphore 0", 1))
  {
    ret = TX_THREAD_ERROR;
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing the SemaphoreThreadOne thread.
  * @param  thread_input: Not used
  * @retval None
  */
void SemaphoreThreadOne_Entry(ULONG thread_input)
{
  UINT status;
  (void) thread_input;
  /* Infinite loop */
  while(1)
  {
    /* Get the semaphore with suspension */
    status = tx_semaphore_get(&Sempaphore_0, TX_WAIT_FOREVER);

    /* Check status */
    if (status != TX_SUCCESS)
      break;

    /* Sleep for 200 ticks to hold the semaphore */
    HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    tx_thread_sleep(200);

    /* Release the semaphore. */
    status = tx_semaphore_put(&Sempaphore_0);

    /* Check status. */
    if (status != TX_SUCCESS)
      break;
  }
}

/**
  * @brief  Function implementing the SemaphoreThreadTwo thread.
  * @param  thread_input: Not used
  * @retval None
  */
void SemaphoreThreadTwo_Entry(ULONG thread_input)
{
  UINT status;
  (void) thread_input;
  /* Infinite loop */
  while(1)
  {
    /* Get the semaphore with suspension */
    status = tx_semaphore_get(&Sempaphore_0, TX_WAIT_FOREVER);

    /* Check status */
    if (status != TX_SUCCESS)
      break;

    /* Sleep for 200 ticks to hold the semaphore */
    tx_thread_sleep(200);

    /* Release the semaphore. */
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    status = tx_semaphore_put(&Sempaphore_0);

    /* Check status. */
    if (status != TX_SUCCESS)
      break;
  }
}
/* USER CODE END 1 */
