/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    29-January-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  static QEvt const *l_qhsmtstQSto[10]; /* Event queue storage for QHsmTst */
  static QEvt const *l_touchscreenQSto[10]; /* Event queue storage for Touchscreen */
  static QF_MPOOL_EL(QEvt) smlPoolSto[10]; /* small pool */
  
  QF_init(); /* initialize the framework and the underlying RT kernel */
  BSP_Init(); /* initialize the Board Support Package */
  
  /* publish-subscribe not used, no call to QF_psInit() */
  
  QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));
  
  /* instantiate and start the active objects... */
  QHsmTst_ctor();
  Touchscreen_ctor();
  
  /* start the active objects... */
  QACTIVE_START(AO_QHsmTst,            /* AO pointer to start */
                1U,                     /* unique QP priority of the AO (lowest) */
                l_qhsmtstQSto,         /* storage for the AO's queue */
                Q_DIM(l_qhsmtstQSto),  /* length of the queue [entries] */
                (void *)0,              /* stack storage (not used in QK) */
                0U,                     /* stack size [bytes] (not used in QK) */
                (QEvt *)0);             /* initial event (or 0) */
  QACTIVE_START(AO_Touchscreen,            /* AO pointer to start */
                2U,                     /* unique QP priority of the AO */
                l_touchscreenQSto,         /* storage for the AO's queue */
                Q_DIM(l_touchscreenQSto),  /* length of the queue [entries] */
                (void *)0,              /* stack storage (not used in QK) */
                0U,                     /* stack size [bytes] (not used in QK) */
                (QEvt *)0);             /* initial event (or 0) */
  
  return QF_run(); /* run the QF application */
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
