#include "SysTick_interface.h"
#include "SysTick_private.h"
#include "SysTick_config.h"


/* =========================================================
 * Private Variables
 * ========================================================= */

static SysTick_Callback_t SysTick_Callback = 0;


/* =========================================================
 * Private Functions
 * ========================================================= */

static uint32_t SysTick_GetClockFrequency(
    SysTick_ClockSource_t ClockSource
)
{
    if (ClockSource == SYSTICK_CLOCK_AHB)
    {
        return SYSTICK_HCLK_FREQUENCY_HZ;
    }
    else
    {
        return (SYSTICK_HCLK_FREQUENCY_HZ / 8UL);
    }
}


/* =========================================================
 * Public Functions
 * ========================================================= */

SysTick_Status_t SysTick_Init(
    SysTick_ClockSource_t ClockSource,
    uint32_t ReloadValue,
    uint8_t EnableInterrupt
)
{
    /* Validate reload value */
    if ((ReloadValue == 0U) ||
        (ReloadValue > SYSTICK_RELOAD_MASK))
    {
        return SYSTICK_ERROR;
    }


    /* Stop counter before configuration */
    SysTick_Stop();


    /*
     * Correct initialization sequence:
     * 1. Program reload value
     * 2. Clear current value
     * 3. Configure CTRL register
     */

    SYSTICK->LOAD = ReloadValue & SYSTICK_RELOAD_MASK;

    SysTick_ClearCurrentValue();


    /* Select clock source */
    if (ClockSource == SYSTICK_CLOCK_AHB)
    {
        SYSTICK->CTRL |= (1UL << SYSTICK_CLKSOURCE_BIT);
    }
    else
    {
        SYSTICK->CTRL &= ~(1UL << SYSTICK_CLKSOURCE_BIT);
    }


    /* Configure interrupt */
    if (EnableInterrupt != 0U)
    {
        SysTick_EnableInterrupt();
    }
    else
    {
        SysTick_DisableInterrupt();
    }

    return SYSTICK_OK;
}


void SysTick_Start(void)
{
    SYSTICK->CTRL |= (1UL << SYSTICK_ENABLE_BIT);
}


void SysTick_Stop(void)
{
    SYSTICK->CTRL &= ~(1UL << SYSTICK_ENABLE_BIT);
}


SysTick_Status_t SysTick_SetReloadValue(uint32_t ReloadValue)
{
    if ((ReloadValue == 0U) ||
        (ReloadValue > SYSTICK_RELOAD_MASK))
    {
        return SYSTICK_ERROR;
    }

    SYSTICK->LOAD = ReloadValue & SYSTICK_RELOAD_MASK;

    return SYSTICK_OK;
}


uint32_t SysTick_GetReloadValue(void)
{
    return (SYSTICK->LOAD & SYSTICK_RELOAD_MASK);
}


uint32_t SysTick_GetCurrentValue(void)
{
    return (SYSTICK->VAL & SYSTICK_CURRENT_MASK);
}


void SysTick_ClearCurrentValue(void)
{
    /*
     * Writing any value clears CURRENT
     * and clears COUNTFLAG.
     */
    SYSTICK->VAL = 0U;
}


void SysTick_EnableInterrupt(void)
{
    SYSTICK->CTRL |= (1UL << SYSTICK_TICKINT_BIT);
}


void SysTick_DisableInterrupt(void)
{
    SYSTICK->CTRL &= ~(1UL << SYSTICK_TICKINT_BIT);
}


uint8_t SysTick_HasElapsed(void)
{
    /*
     * COUNTFLAG is cleared when CTRL is read.
     */
    if ((SYSTICK->CTRL & (1UL << SYSTICK_COUNTFLAG_BIT)) != 0U)
    {
        return 1U;
    }

    return 0U;
}


SysTick_Status_t SysTick_SetCallback(
    SysTick_Callback_t Callback
)
{
    if (Callback == 0)
    {
        return SYSTICK_ERROR;
    }

    SysTick_Callback = Callback;

    return SYSTICK_OK;
}


/* =========================================================
 * Blocking Delay Functions
 * ========================================================= */

SysTick_Status_t SysTick_DelayUs(uint32_t DelayUs)
{
    uint32_t ClockFrequency;
    uint64_t TotalTicks;
    uint32_t ReloadValue;

    if (DelayUs == 0U)
    {
        return SYSTICK_OK;
    }


    /* Get currently selected clock source */
    if ((SYSTICK->CTRL &
         (1UL << SYSTICK_CLKSOURCE_BIT)) != 0U)
    {
        ClockFrequency =
            SysTick_GetClockFrequency(SYSTICK_CLOCK_AHB);
    }
    else
    {
        ClockFrequency =
            SysTick_GetClockFrequency(SYSTICK_CLOCK_AHB_DIV_8);
    }


    TotalTicks =
        ((uint64_t)DelayUs *
         (uint64_t)ClockFrequency) / 1000000ULL;


    if (TotalTicks == 0ULL)
    {
        return SYSTICK_ERROR;
    }


    /*
     * Maximum delay in one SysTick cycle
     * is limited by the 24-bit reload register.
     */
    while (TotalTicks > 0ULL)
    {
        if (TotalTicks > 0x01000000ULL)
        {
            ReloadValue = SYSTICK_RELOAD_MASK;
        }
        else
        {
            ReloadValue = (uint32_t)(TotalTicks - 1ULL);
        }


        SYSTICK->LOAD = ReloadValue;

        SysTick_ClearCurrentValue();

        SysTick_Start();


        /* Wait for counter to reach zero */
        while (SysTick_HasElapsed() == 0U)
        {
        }


        SYSTICK->CTRL &=
            ~(1UL << SYSTICK_ENABLE_BIT);


        if (TotalTicks > 0x01000000ULL)
        {
            TotalTicks -= 0x01000000ULL;
        }
        else
        {
            TotalTicks = 0ULL;
        }
    }

    return SYSTICK_OK;
}


SysTick_Status_t SysTick_DelayMs(uint32_t DelayMs)
{
    uint64_t DelayUs;

    if (DelayMs == 0U)
    {
        return SYSTICK_OK;
    }


    DelayUs = (uint64_t)DelayMs * 1000ULL;


    /*
     * SysTick_DelayUs accepts uint32_t,
     * so split very large delays if required.
     */
    while (DelayUs > 0ULL)
    {
        if (DelayUs > 0xFFFFFFFFULL)
        {
            if (SysTick_DelayUs(0xFFFFFFFFUL) != SYSTICK_OK)
            {
                return SYSTICK_ERROR;
            }

            DelayUs -= 0xFFFFFFFFULL;
        }
        else
        {
            return SysTick_DelayUs((uint32_t)DelayUs);
        }
    }

    return SYSTICK_OK;
}


/* =========================================================
 * SysTick Interrupt Handler
 *
 * This function is called automatically by the Cortex-M4
 * when SysTick reaches zero and TICKINT is enabled.
 * ========================================================= */

void SysTick_Handler(void)
{
    if (SysTick_Callback != 0)
    {
        SysTick_Callback();
    }
}
