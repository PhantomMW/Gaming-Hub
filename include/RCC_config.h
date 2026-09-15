#ifndef RCC_CONFIG_H_
#define RCC_CONFIG_H_

#include "RCC_interface.h"

/*==========================================================
 *             Initial System Clock Configuration
 *==========================================================*/

#define RCC_SYS_CLOCK_SOURCE        RCC_PLL


/*==========================================================
 *                   HSE Configuration
 *==========================================================*/

#define RCC_HSE_MODE                RCC_HSE_CRYSTAL


/*==========================================================
 *                    Bus Prescalers
 *==========================================================*/

#define RCC_AHB_PRESCALER           RCC_AHB_DIV1
#define RCC_APB1_PRESCALER          RCC_APB_DIV2
#define RCC_APB2_PRESCALER          RCC_APB_DIV1


/*==========================================================
 *                  Main PLL Configuration
 *==========================================================
 *
 * Used only when RCC_SYS_CLOCK_SOURCE = RCC_PLL
 *
 * HSI = 16 MHz
 * PLLM = 16
 * PLLN = 336
 * PLLP = 4
 *
 * SYSCLK = 84 MHz
 *
 */

#define RCC_PLL_SOURCE              RCC_PLL_SOURCE_HSI
#define RCC_PLL_M                   16U
#define RCC_PLL_N                   336U
#define RCC_PLL_P                   RCC_PLLP_DIV4
#define RCC_PLL_Q                   7U

#endif /* RCC_CONFIG_H_ */
