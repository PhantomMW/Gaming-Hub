#include "RCC_interface.h"
#include "RCC_private.h"
#include "RCC_config.h"


/*==========================================================
 *                    Private Helpers
 *==========================================================*/

#define RCC_TIMEOUT_VALUE    1000000UL

static volatile uint32_t *RCC_GetEnableRegister(uint8_t Copy_u8Bus)
{
    switch (Copy_u8Bus)
    {
        case RCC_AHB1: return &RCC_AHB1ENR;
        case RCC_AHB2: return &RCC_AHB2ENR;
        case RCC_APB1: return &RCC_APB1ENR;
        case RCC_APB2: return &RCC_APB2ENR;
        default:       return (void *)0;
    }
}

static volatile uint32_t *RCC_GetResetRegister(uint8_t Copy_u8Bus)
{
    switch (Copy_u8Bus)
    {
        case RCC_AHB1: return &RCC_AHB1RSTR;
        case RCC_AHB2: return &RCC_AHB2RSTR;
        case RCC_APB1: return &RCC_APB1RSTR;
        case RCC_APB2: return &RCC_APB2RSTR;
        default:       return (void *)0;
    }
}

static volatile uint32_t *RCC_GetLowPowerRegister(uint8_t Copy_u8Bus)
{
    switch (Copy_u8Bus)
    {
        case RCC_AHB1: return &RCC_AHB1LPENR;
        case RCC_AHB2: return &RCC_AHB2LPENR;
        case RCC_APB1: return &RCC_APB1LPENR;
        case RCC_APB2: return &RCC_APB2LPENR;
        default:       return (void *)0;
    }
}

static RCC_StatusType RCC_WaitForBit(volatile uint32_t *Copy_pu32Register,
                                     uint8_t Copy_u8Bit,
                                     uint8_t Copy_u8State)
{
    uint32_t Local_u32Timeout = RCC_TIMEOUT_VALUE;

    while (Local_u32Timeout > 0U)
    {
        if (RCC_READ_BIT(*Copy_pu32Register, Copy_u8Bit) == Copy_u8State)
        {
            return RCC_OK;
        }

        Local_u32Timeout--;
    }

    return RCC_TIMEOUT;
}


/*==========================================================
 *                    Initialization
 *==========================================================*/

RCC_StatusType RCC_Init(void)
{
    RCC_StatusType Local_Status;

#if (RCC_SYS_CLOCK_SOURCE == RCC_HSE)

    if (RCC_HSE_MODE == RCC_HSE_BYPASS)
    {
        RCC_SET_BIT(RCC_CR, RCC_HSEBYP_BIT);
    }
    else
    {
        RCC_CLEAR_BIT(RCC_CR, RCC_HSEBYP_BIT);
    }

    Local_Status = RCC_EnableClockSource(RCC_HSE);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

#elif (RCC_SYS_CLOCK_SOURCE == RCC_PLL)

    Local_Status = RCC_ConfigurePLL(RCC_PLL_SOURCE,
                                    RCC_PLL_M,
                                    RCC_PLL_N,
                                    RCC_PLL_P,
                                    RCC_PLL_Q);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

    Local_Status = RCC_EnableClockSource(RCC_PLL);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

#else

    Local_Status = RCC_EnableClockSource(RCC_HSI);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

#endif

    Local_Status = RCC_SetAHBPrescaler(RCC_AHB_PRESCALER);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

    Local_Status = RCC_SetAPB1Prescaler(RCC_APB1_PRESCALER);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

    Local_Status = RCC_SetAPB2Prescaler(RCC_APB2_PRESCALER);

    if (Local_Status != RCC_OK)
    {
        return Local_Status;
    }

    return RCC_SelectSysClock(RCC_SYS_CLOCK_SOURCE);
}


/*==========================================================
 *                    Clock Sources
 *==========================================================*/

RCC_StatusType RCC_EnableClockSource(uint8_t Copy_u8ClockSource)
{
    switch (Copy_u8ClockSource)
    {
        case RCC_HSI:
            RCC_SET_BIT(RCC_CR, RCC_HSION_BIT);
            return RCC_WaitForBit(&RCC_CR, RCC_HSIRDY_BIT, 1U);

        case RCC_HSE:
            RCC_SET_BIT(RCC_CR, RCC_HSEON_BIT);
            return RCC_WaitForBit(&RCC_CR, RCC_HSERDY_BIT, 1U);

        case RCC_PLL:
            RCC_SET_BIT(RCC_CR, RCC_PLLON_BIT);
            return RCC_WaitForBit(&RCC_CR, RCC_PLLRDY_BIT, 1U);

        case RCC_LSI:
            RCC_SET_BIT(RCC_CSR, RCC_LSION_BIT);
            return RCC_WaitForBit(&RCC_CSR, RCC_LSIRDY_BIT, 1U);

        case RCC_LSE:
            RCC_SET_BIT(RCC_BDCR, RCC_LSEON_BIT);
            return RCC_WaitForBit(&RCC_BDCR, RCC_LSERDY_BIT, 1U);

        case RCC_PLLI2S:
            RCC_SET_BIT(RCC_CR, RCC_PLLI2SON_BIT);
            return RCC_WaitForBit(&RCC_CR, RCC_PLLI2SRDY_BIT, 1U);

        default:
            return RCC_ERROR;
    }
}


RCC_StatusType RCC_DisableClockSource(uint8_t Copy_u8ClockSource)
{
    switch (Copy_u8ClockSource)
    {
        case RCC_HSI:
            RCC_CLEAR_BIT(RCC_CR, RCC_HSION_BIT);
            return RCC_OK;

        case RCC_HSE:
            RCC_CLEAR_BIT(RCC_CR, RCC_HSEON_BIT);
            return RCC_OK;

        case RCC_PLL:
            RCC_CLEAR_BIT(RCC_CR, RCC_PLLON_BIT);
            return RCC_OK;

        case RCC_LSI:
            RCC_CLEAR_BIT(RCC_CSR, RCC_LSION_BIT);
            return RCC_OK;

        case RCC_LSE:
            RCC_CLEAR_BIT(RCC_BDCR, RCC_LSEON_BIT);
            return RCC_OK;

        case RCC_PLLI2S:
            RCC_CLEAR_BIT(RCC_CR, RCC_PLLI2SON_BIT);
            return RCC_OK;

        default:
            return RCC_ERROR;
    }
}


uint8_t RCC_GetClockSourceStatus(uint8_t Copy_u8ClockSource)
{
    switch (Copy_u8ClockSource)
    {
        case RCC_HSI:    return RCC_READ_BIT(RCC_CR, RCC_HSIRDY_BIT);
        case RCC_HSE:    return RCC_READ_BIT(RCC_CR, RCC_HSERDY_BIT);
        case RCC_PLL:    return RCC_READ_BIT(RCC_CR, RCC_PLLRDY_BIT);
        case RCC_LSI:    return RCC_READ_BIT(RCC_CSR, RCC_LSIRDY_BIT);
        case RCC_LSE:    return RCC_READ_BIT(RCC_BDCR, RCC_LSERDY_BIT);
        case RCC_PLLI2S: return RCC_READ_BIT(RCC_CR, RCC_PLLI2SRDY_BIT);
        default:         return 0U;
    }
}


/*==========================================================
 *                     System Clock
 *==========================================================*/

RCC_StatusType RCC_SelectSysClock(uint8_t Copy_u8ClockSource)
{
    uint32_t Local_u32ExpectedStatus;

    if (Copy_u8ClockSource > RCC_PLL)
    {
        return RCC_ERROR;
    }

    RCC_CFGR &= ~RCC_SW_MASK;
    RCC_CFGR |= ((uint32_t)Copy_u8ClockSource << RCC_SW_POS);

    Local_u32ExpectedStatus = (uint32_t)Copy_u8ClockSource;

    if (RCC_WaitForBit(&RCC_CFGR,
                       RCC_SWS_POS,
                       (uint8_t)(Local_u32ExpectedStatus & 1U)) == RCC_TIMEOUT)
    {
        return RCC_TIMEOUT;
    }

    while (((RCC_CFGR & RCC_SWS_MASK) >> RCC_SWS_POS)
           != Local_u32ExpectedStatus)
    {
        /* Wait until clock switch is complete */
    }

    return RCC_OK;
}


uint8_t RCC_GetSysClockStatus(void)
{
    return (uint8_t)((RCC_CFGR & RCC_SWS_MASK) >> RCC_SWS_POS);
}


/*==========================================================
 *                      Prescalers
 *==========================================================*/

RCC_StatusType RCC_SetAHBPrescaler(uint8_t Copy_u8Prescaler)
{
    if ((Copy_u8Prescaler > 15U) ||
        ((Copy_u8Prescaler > 0U) && (Copy_u8Prescaler < 8U)))
    {
        return RCC_ERROR;
    }

    RCC_CFGR &= ~RCC_HPRE_MASK;
    RCC_CFGR |= ((uint32_t)Copy_u8Prescaler << RCC_HPRE_POS);

    return RCC_OK;
}


RCC_StatusType RCC_SetAPB1Prescaler(uint8_t Copy_u8Prescaler)
{
    if ((Copy_u8Prescaler > 7U) ||
        ((Copy_u8Prescaler > 0U) && (Copy_u8Prescaler < 4U)))
    {
        return RCC_ERROR;
    }

    RCC_CFGR &= ~RCC_PPRE1_MASK;
    RCC_CFGR |= ((uint32_t)Copy_u8Prescaler << RCC_PPRE1_POS);

    return RCC_OK;
}


RCC_StatusType RCC_SetAPB2Prescaler(uint8_t Copy_u8Prescaler)
{
    if ((Copy_u8Prescaler > 7U) ||
        ((Copy_u8Prescaler > 0U) && (Copy_u8Prescaler < 4U)))
    {
        return RCC_ERROR;
    }

    RCC_CFGR &= ~RCC_PPRE2_MASK;
    RCC_CFGR |= ((uint32_t)Copy_u8Prescaler << RCC_PPRE2_POS);

    return RCC_OK;
}


/*==========================================================
 *                       Main PLL
 *==========================================================*/

RCC_StatusType RCC_ConfigurePLL(uint8_t Copy_u8Source,
                                 uint8_t Copy_u8PLLM,
                                 uint16_t Copy_u16PLLN,
                                 uint8_t Copy_u8PLLP,
                                 uint8_t Copy_u8PLLQ)
{
    if (RCC_READ_BIT(RCC_CR, RCC_PLLON_BIT))
    {
        return RCC_ERROR;
    }

    if ((Copy_u8Source > RCC_PLL_SOURCE_HSE) ||
        (Copy_u8PLLM < 2U) ||
        (Copy_u8PLLM > 63U) ||
        (Copy_u16PLLN < 192U) ||
        (Copy_u16PLLN > 432U) ||
        (Copy_u8PLLP > RCC_PLLP_DIV8) ||
        (Copy_u8PLLQ < 2U) ||
        (Copy_u8PLLQ > 15U))
    {
        return RCC_ERROR;
    }

    RCC_PLLCFGR &= ~(RCC_PLLM_MASK |
                     RCC_PLLN_MASK |
                     RCC_PLLP_MASK |
                     RCC_PLLSRC_MASK |
                     RCC_PLLQ_MASK);

    RCC_PLLCFGR |= ((uint32_t)Copy_u8PLLM << RCC_PLLM_POS);
    RCC_PLLCFGR |= ((uint32_t)Copy_u16PLLN << RCC_PLLN_POS);
    RCC_PLLCFGR |= ((uint32_t)Copy_u8PLLP << RCC_PLLP_POS);
    RCC_PLLCFGR |= ((uint32_t)Copy_u8Source << RCC_PLLSRC_POS);
    RCC_PLLCFGR |= ((uint32_t)Copy_u8PLLQ << RCC_PLLQ_POS);

    return RCC_OK;
}


/*==========================================================
 *                       PLLI2S
 *==========================================================*/

RCC_StatusType RCC_ConfigurePLLI2S(uint16_t Copy_u16PLLI2SN,
                                    uint8_t Copy_u8PLLI2SR)
{
    if (RCC_READ_BIT(RCC_CR, RCC_PLLI2SON_BIT))
    {
        return RCC_ERROR;
    }

    if ((Copy_u16PLLI2SN < 192U) ||
        (Copy_u16PLLI2SN > 432U) ||
        (Copy_u8PLLI2SR < 2U) ||
        (Copy_u8PLLI2SR > 7U))
    {
        return RCC_ERROR;
    }

    RCC_PLLI2SCFGR &= ~(RCC_PLLI2SN_MASK | RCC_PLLI2SR_MASK);

    RCC_PLLI2SCFGR |= ((uint32_t)Copy_u16PLLI2SN << RCC_PLLI2SN_POS);
    RCC_PLLI2SCFGR |= ((uint32_t)Copy_u8PLLI2SR << RCC_PLLI2SR_POS);

    return RCC_OK;
}


/*==========================================================
 *                  Peripheral Clocks
 *==========================================================*/

RCC_StatusType RCC_EnablePeripheralClock(uint8_t Copy_u8BusId,
                                          uint8_t Copy_u8PeripheralId)
{
    volatile uint32_t *Local_pRegister;

    if (Copy_u8PeripheralId > 31U)
    {
        return RCC_ERROR;
    }

    Local_pRegister = RCC_GetEnableRegister(Copy_u8BusId);

    if (Local_pRegister == (void *)0)
    {
        return RCC_ERROR;
    }

    *Local_pRegister |= (1UL << Copy_u8PeripheralId);

    return RCC_OK;
}


RCC_StatusType RCC_DisablePeripheralClock(uint8_t Copy_u8BusId,
                                           uint8_t Copy_u8PeripheralId)
{
    volatile uint32_t *Local_pRegister;

    if (Copy_u8PeripheralId > 31U)
    {
        return RCC_ERROR;
    }

    Local_pRegister = RCC_GetEnableRegister(Copy_u8BusId);

    if (Local_pRegister == (void *)0)
    {
        return RCC_ERROR;
    }

    *Local_pRegister &= ~(1UL << Copy_u8PeripheralId);

    return RCC_OK;
}


/*==========================================================
 *                  Peripheral Reset
 *==========================================================*/

RCC_StatusType RCC_ForcePeripheralReset(uint8_t Copy_u8BusId,
                                         uint8_t Copy_u8PeripheralId)
{
    volatile uint32_t *Local_pRegister = RCC_GetResetRegister(Copy_u8BusId);

    if ((Local_pRegister == (void *)0) || (Copy_u8PeripheralId > 31U))
    {
        return RCC_ERROR;
    }

    *Local_pRegister |= (1UL << Copy_u8PeripheralId);

    return RCC_OK;
}


RCC_StatusType RCC_ReleasePeripheralReset(uint8_t Copy_u8BusId,
                                           uint8_t Copy_u8PeripheralId)
{
    volatile uint32_t *Local_pRegister = RCC_GetResetRegister(Copy_u8BusId);

    if ((Local_pRegister == (void *)0) || (Copy_u8PeripheralId > 31U))
    {
        return RCC_ERROR;
    }

    *Local_pRegister &= ~(1UL << Copy_u8PeripheralId);

    return RCC_OK;
}


/*==========================================================
 *                Low Power Peripheral Clock
 *==========================================================*/

RCC_StatusType RCC_EnableClockInLowPower(uint8_t Copy_u8BusId,
                                          uint8_t Copy_u8PeripheralId)
{
    volatile uint32_t *Local_pRegister = RCC_GetLowPowerRegister(Copy_u8BusId);

    if ((Local_pRegister == (void *)0) || (Copy_u8PeripheralId > 31U))
    {
        return RCC_ERROR;
    }

    *Local_pRegister |= (1UL << Copy_u8PeripheralId);

    return RCC_OK;
}


RCC_StatusType RCC_DisableClockInLowPower(uint8_t Copy_u8BusId,
                                           uint8_t Copy_u8PeripheralId)
{
    volatile uint32_t *Local_pRegister = RCC_GetLowPowerRegister(Copy_u8BusId);

    if ((Local_pRegister == (void *)0) || (Copy_u8PeripheralId > 31U))
    {
        return RCC_ERROR;
    }

    *Local_pRegister &= ~(1UL << Copy_u8PeripheralId);

    return RCC_OK;
}


/*==========================================================
 *                  Clock Security System
 *==========================================================*/

void RCC_EnableCSS(void)
{
    RCC_SET_BIT(RCC_CR, RCC_CSSON_BIT);
}

void RCC_DisableCSS(void)
{
    RCC_CLEAR_BIT(RCC_CR, RCC_CSSON_BIT);
}


/*==========================================================
 *                  Clock Interrupts
 *==========================================================*/

RCC_StatusType RCC_EnableClockInterrupt(uint8_t Copy_u8Interrupt)
{
    if (Copy_u8Interrupt > RCC_INT_PLLI2SRDY)
    {
        return RCC_ERROR;
    }

    RCC_SET_BIT(RCC_CIR, Copy_u8Interrupt + RCC_CIR_READY_IE_POS);

    return RCC_OK;
}


RCC_StatusType RCC_DisableClockInterrupt(uint8_t Copy_u8Interrupt)
{
    if (Copy_u8Interrupt > RCC_INT_PLLI2SRDY)
    {
        return RCC_ERROR;
    }

    RCC_CLEAR_BIT(RCC_CIR, Copy_u8Interrupt + RCC_CIR_READY_IE_POS);

    return RCC_OK;
}


uint8_t RCC_GetClockInterruptFlag(uint8_t Copy_u8Interrupt)
{
    if (Copy_u8Interrupt > RCC_INT_PLLI2SRDY)
    {
        return 0U;
    }

    return RCC_READ_BIT(RCC_CIR, Copy_u8Interrupt);
}


RCC_StatusType RCC_ClearClockInterruptFlag(uint8_t Copy_u8Interrupt)
{
    if (Copy_u8Interrupt > RCC_INT_PLLI2SRDY)
    {
        return RCC_ERROR;
    }

    RCC_SET_BIT(RCC_CIR, Copy_u8Interrupt + RCC_CIR_READY_CLEAR_POS);

    return RCC_OK;
}


/*==========================================================
 *                   RTC / Backup Domain
 *==========================================================*/

RCC_StatusType RCC_SelectRTCClock(uint8_t Copy_u8RTCSource)
{
    if (Copy_u8RTCSource > RCC_RTC_HSE)
    {
        return RCC_ERROR;
    }

    RCC_BDCR &= ~RCC_RTCSEL_MASK;
    RCC_BDCR |= ((uint32_t)Copy_u8RTCSource << RCC_RTCSEL_POS);

    return RCC_OK;
}


void RCC_EnableRTCClock(void)
{
    RCC_SET_BIT(RCC_BDCR, RCC_RTCEN_BIT);
}


void RCC_DisableRTCClock(void)
{
    RCC_CLEAR_BIT(RCC_BDCR, RCC_RTCEN_BIT);
}


void RCC_ResetBackupDomain(void)
{
    RCC_SET_BIT(RCC_BDCR, RCC_BDRST_BIT);
    RCC_CLEAR_BIT(RCC_BDCR, RCC_BDRST_BIT);
}


/*==========================================================
 *                         MCO
 *==========================================================*/

RCC_StatusType RCC_ConfigureMCO1(uint8_t Copy_u8Source,
                                  uint8_t Copy_u8Prescaler)
{
    if ((Copy_u8Source > 3U) ||
        ((Copy_u8Prescaler > 7U) ||
         ((Copy_u8Prescaler > 0U) && (Copy_u8Prescaler < 4U))))
    {
        return RCC_ERROR;
    }

    RCC_CFGR &= ~((0x3UL << RCC_MCO1_POS) |
                  (0x7UL << RCC_MCO1PRE_POS));

    RCC_CFGR |= ((uint32_t)Copy_u8Source << RCC_MCO1_POS);
    RCC_CFGR |= ((uint32_t)Copy_u8Prescaler << RCC_MCO1PRE_POS);

    return RCC_OK;
}


RCC_StatusType RCC_ConfigureMCO2(uint8_t Copy_u8Source,
                                  uint8_t Copy_u8Prescaler)
{
    if ((Copy_u8Source > 3U) ||
        ((Copy_u8Prescaler > 7U) ||
         ((Copy_u8Prescaler > 0U) && (Copy_u8Prescaler < 4U))))
    {
        return RCC_ERROR;
    }

    RCC_CFGR &= ~((0x3UL << RCC_MCO2_POS) |
                  (0x7UL << RCC_MCO2PRE_POS));

    RCC_CFGR |= ((uint32_t)Copy_u8Source << RCC_MCO2_POS);
    RCC_CFGR |= ((uint32_t)Copy_u8Prescaler << RCC_MCO2PRE_POS);

    return RCC_OK;
}


/*==========================================================
 *                       Reset Flags
 *==========================================================*/

uint32_t RCC_GetResetFlags(void)
{
    return (RCC_CSR & 0xFE000000UL);
}


void RCC_ClearResetFlags(void)
{
    RCC_SET_BIT(RCC_CSR, RCC_RMVF_BIT);
}


/*==========================================================
 *                         SSCG
 *==========================================================*/

RCC_StatusType RCC_ConfigureSSCG(uint16_t Copy_u16ModulationPeriod,
                                  uint16_t Copy_u16IncrementStep,
                                  uint8_t Copy_u8SpreadMode)
{
    if ((Copy_u16ModulationPeriod > 0x1FFFU) ||
        (Copy_u16IncrementStep > 0x7FFFU) ||
        (Copy_u8SpreadMode > 1U))
    {
        return RCC_ERROR;
    }

    if (RCC_READ_BIT(RCC_CR, RCC_PLLON_BIT))
    {
        return RCC_ERROR;
    }

    RCC_SSCGR &= ~(RCC_MODPER_MASK |
                   RCC_INCSTEP_MASK |
                   (1UL << RCC_SPREADSEL_BIT));

    RCC_SSCGR |= ((uint32_t)Copy_u16ModulationPeriod << RCC_MODPER_POS);
    RCC_SSCGR |= ((uint32_t)Copy_u16IncrementStep << RCC_INCSTEP_POS);
    RCC_SSCGR |= ((uint32_t)Copy_u8SpreadMode << RCC_SPREADSEL_BIT);

    return RCC_OK;
}


void RCC_EnableSSCG(void)
{
    RCC_SET_BIT(RCC_SSCGR, RCC_SSCGEN_BIT);
}


void RCC_DisableSSCG(void)
{
    RCC_CLEAR_BIT(RCC_SSCGR, RCC_SSCGEN_BIT);
}


/*==========================================================
 *                     Timer Clock Mode
 *==========================================================*/

RCC_StatusType RCC_SetTimerClockMode(uint8_t Copy_u8Mode)
{
    if (Copy_u8Mode > RCC_TIMER_CLOCK_ALTERNATE)
    {
        return RCC_ERROR;
    }

    if (Copy_u8Mode == RCC_TIMER_CLOCK_ALTERNATE)
    {
        RCC_SET_BIT(RCC_DCKCFGR, RCC_TIMPRE_BIT);
    }
    else
    {
        RCC_CLEAR_BIT(RCC_DCKCFGR, RCC_TIMPRE_BIT);
    }

    return RCC_OK;
}
