#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_

#include <stdint.h>

/*==========================================================
 *                       Status
 *==========================================================*/
typedef enum
{
    RCC_OK = 0U,
    RCC_ERROR,
    RCC_TIMEOUT
} RCC_StatusType;


/*==========================================================
 *                    Clock Sources
 *==========================================================*/
#define RCC_HSI                0U
#define RCC_HSE                1U
#define RCC_PLL                2U
#define RCC_LSI                3U
#define RCC_LSE                4U
#define RCC_PLLI2S             5U


/*==========================================================
 *                       Buses
 *==========================================================*/
#define RCC_AHB1               0U
#define RCC_AHB2               1U
#define RCC_APB1               2U
#define RCC_APB2               3U


/*==========================================================
 *                 Peripheral Bit Positions
 *==========================================================*/

/* AHB1 */
#define RCC_GPIOA              0U
#define RCC_GPIOB              1U
#define RCC_GPIOC              2U
#define RCC_GPIOD              3U
#define RCC_GPIOE              4U
#define RCC_GPIOH              7U
#define RCC_CRC                12U
#define RCC_DMA1               21U
#define RCC_DMA2               22U

/* AHB2 */
#define RCC_OTGFS              7U

/* APB1 */
#define RCC_TIM2               0U
#define RCC_TIM3               1U
#define RCC_TIM4               2U
#define RCC_TIM5               3U
#define RCC_WWDG               11U
#define RCC_SPI2               14U
#define RCC_SPI3               15U
#define RCC_USART2             17U
#define RCC_I2C1               21U
#define RCC_I2C2               22U
#define RCC_I2C3               23U
#define RCC_PWR                28U

/* APB2 */
#define RCC_TIM1               0U
#define RCC_USART1             4U
#define RCC_USART6             5U
#define RCC_ADC1               8U
#define RCC_SDIO               11U
#define RCC_SPI1               12U
#define RCC_SPI4               13U
#define RCC_SYSCFG             14U
#define RCC_TIM9               16U
#define RCC_TIM10              17U
#define RCC_TIM11              18U


/*==========================================================
 *                   Prescaler Options
 *==========================================================*/

/* AHB */
#define RCC_AHB_DIV1           0U
#define RCC_AHB_DIV2           8U
#define RCC_AHB_DIV4           9U
#define RCC_AHB_DIV8           10U
#define RCC_AHB_DIV16          11U
#define RCC_AHB_DIV64          12U
#define RCC_AHB_DIV128         13U
#define RCC_AHB_DIV256         14U
#define RCC_AHB_DIV512         15U

/* APB */
#define RCC_APB_DIV1           0U
#define RCC_APB_DIV2           4U
#define RCC_APB_DIV4           5U
#define RCC_APB_DIV8           6U
#define RCC_APB_DIV16          7U


/*==========================================================
 *                     PLL Options
 *==========================================================*/
#define RCC_PLL_SOURCE_HSI     0U
#define RCC_PLL_SOURCE_HSE     1U

#define RCC_PLLP_DIV2          0U
#define RCC_PLLP_DIV4          1U
#define RCC_PLLP_DIV6          2U
#define RCC_PLLP_DIV8          3U


/*==========================================================
 *                       HSE Mode
 *==========================================================*/
#define RCC_HSE_CRYSTAL        0U
#define RCC_HSE_BYPASS         1U


/*==========================================================
 *                     MCO Sources
 *==========================================================*/
#define RCC_MCO_HSI            0U
#define RCC_MCO_LSE            1U
#define RCC_MCO_HSE            2U
#define RCC_MCO_PLL            3U

#define RCC_MCO2_SYSCLK        0U
#define RCC_MCO2_PLLI2S        1U
#define RCC_MCO2_HSE           2U
#define RCC_MCO2_PLL           3U

#define RCC_MCO_DIV1           0U
#define RCC_MCO_DIV2           4U
#define RCC_MCO_DIV3           5U
#define RCC_MCO_DIV4           6U
#define RCC_MCO_DIV5           7U


/*==========================================================
 *                     RTC Sources
 *==========================================================*/
#define RCC_RTC_NO_CLOCK       0U
#define RCC_RTC_LSE            1U
#define RCC_RTC_LSI            2U
#define RCC_RTC_HSE            3U


/*==========================================================
 *                Clock Ready Interrupt Sources
 *==========================================================*/
#define RCC_INT_LSIRDY         0U
#define RCC_INT_LSERDY         1U
#define RCC_INT_HSIRDY         2U
#define RCC_INT_HSERDY         3U
#define RCC_INT_PLLRDY         4U
#define RCC_INT_PLLI2SRDY      5U


/*==========================================================
 *                      Timer Clock
 *==========================================================*/
#define RCC_TIMER_CLOCK_NORMAL     0U
#define RCC_TIMER_CLOCK_ALTERNATE  1U


/*==========================================================
 *                      Public APIs
 *==========================================================*/

/* Initialization */
RCC_StatusType RCC_Init(void);

/* Oscillators */
RCC_StatusType RCC_EnableClockSource(uint8_t Copy_u8ClockSource);
RCC_StatusType RCC_DisableClockSource(uint8_t Copy_u8ClockSource);
uint8_t RCC_GetClockSourceStatus(uint8_t Copy_u8ClockSource);

/* System Clock */
RCC_StatusType RCC_SelectSysClock(uint8_t Copy_u8ClockSource);
uint8_t RCC_GetSysClockStatus(void);

/* Prescalers */
RCC_StatusType RCC_SetAHBPrescaler(uint8_t Copy_u8Prescaler);
RCC_StatusType RCC_SetAPB1Prescaler(uint8_t Copy_u8Prescaler);
RCC_StatusType RCC_SetAPB2Prescaler(uint8_t Copy_u8Prescaler);

/* Main PLL */
RCC_StatusType RCC_ConfigurePLL(uint8_t Copy_u8Source,
                                 uint8_t Copy_u8PLLM,
                                 uint16_t Copy_u16PLLN,
                                 uint8_t Copy_u8PLLP,
                                 uint8_t Copy_u8PLLQ);

/* PLLI2S */
RCC_StatusType RCC_ConfigurePLLI2S(uint16_t Copy_u16PLLI2SN,
                                    uint8_t Copy_u8PLLI2SR);

/* Peripheral Clock */
RCC_StatusType RCC_EnablePeripheralClock(uint8_t Copy_u8BusId,
                                          uint8_t Copy_u8PeripheralId);

RCC_StatusType RCC_DisablePeripheralClock(uint8_t Copy_u8BusId,
                                           uint8_t Copy_u8PeripheralId);

/* Peripheral Reset */
RCC_StatusType RCC_ForcePeripheralReset(uint8_t Copy_u8BusId,
                                         uint8_t Copy_u8PeripheralId);

RCC_StatusType RCC_ReleasePeripheralReset(uint8_t Copy_u8BusId,
                                           uint8_t Copy_u8PeripheralId);

/* Low Power Clock */
RCC_StatusType RCC_EnableClockInLowPower(uint8_t Copy_u8BusId,
                                          uint8_t Copy_u8PeripheralId);

RCC_StatusType RCC_DisableClockInLowPower(uint8_t Copy_u8BusId,
                                           uint8_t Copy_u8PeripheralId);

/* CSS */
void RCC_EnableCSS(void);
void RCC_DisableCSS(void);

/* Clock Interrupts */
RCC_StatusType RCC_EnableClockInterrupt(uint8_t Copy_u8Interrupt);
RCC_StatusType RCC_DisableClockInterrupt(uint8_t Copy_u8Interrupt);
uint8_t RCC_GetClockInterruptFlag(uint8_t Copy_u8Interrupt);
RCC_StatusType RCC_ClearClockInterruptFlag(uint8_t Copy_u8Interrupt);

/* RTC / Backup Domain */
RCC_StatusType RCC_SelectRTCClock(uint8_t Copy_u8RTCSource);
void RCC_EnableRTCClock(void);
void RCC_DisableRTCClock(void);
void RCC_ResetBackupDomain(void);

/* MCO */
RCC_StatusType RCC_ConfigureMCO1(uint8_t Copy_u8Source,
                                  uint8_t Copy_u8Prescaler);

RCC_StatusType RCC_ConfigureMCO2(uint8_t Copy_u8Source,
                                  uint8_t Copy_u8Prescaler);

/* Reset Flags */
uint32_t RCC_GetResetFlags(void);
void RCC_ClearResetFlags(void);

/* SSCG */
RCC_StatusType RCC_ConfigureSSCG(uint16_t Copy_u16ModulationPeriod,
                                  uint16_t Copy_u16IncrementStep,
                                  uint8_t Copy_u8SpreadMode);

void RCC_EnableSSCG(void);
void RCC_DisableSSCG(void);

/* Timer Clock */
RCC_StatusType RCC_SetTimerClockMode(uint8_t Copy_u8Mode);

#endif /* RCC_INTERFACE_H_ */
