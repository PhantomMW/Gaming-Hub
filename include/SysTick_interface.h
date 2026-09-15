#ifndef SYSTICK_INTERFACE_H
#define SYSTICK_INTERFACE_H

#include <stdint.h>


/* =========================================================
 * SysTick Clock Sources
 *
 * STM32F401:
 * 0 -> AHB / 8
 * 1 -> Processor Clock (AHB / HCLK)
 * ========================================================= */

typedef enum
{
    SYSTICK_CLOCK_AHB_DIV_8 = 0U,
    SYSTICK_CLOCK_AHB       = 1U
} SysTick_ClockSource_t;


/* =========================================================
 * SysTick Status
 * ========================================================= */

typedef enum
{
    SYSTICK_OK = 0U,
    SYSTICK_ERROR
} SysTick_Status_t;


/* =========================================================
 * SysTick Callback Type
 * ========================================================= */

typedef void (*SysTick_Callback_t)(void);


/* =========================================================
 * SysTick APIs
 * ========================================================= */

/**
 * @brief Initialize SysTick.
 *
 * @param ClockSource SysTick clock source.
 * @param ReloadValue Reload value from 1 to 0xFFFFFF.
 * @param EnableInterrupt Enable or disable SysTick interrupt.
 */
SysTick_Status_t SysTick_Init(
    SysTick_ClockSource_t ClockSource,
    uint32_t ReloadValue,
    uint8_t EnableInterrupt
);


/**
 * @brief Start SysTick counter.
 */
void SysTick_Start(void);


/**
 * @brief Stop SysTick counter.
 */
void SysTick_Stop(void);


/**
 * @brief Set SysTick reload value.
 */
SysTick_Status_t SysTick_SetReloadValue(uint32_t ReloadValue);


/**
 * @brief Get SysTick reload value.
 */
uint32_t SysTick_GetReloadValue(void);


/**
 * @brief Get current SysTick counter value.
 */
uint32_t SysTick_GetCurrentValue(void);


/**
 * @brief Clear SysTick current value.
 *
 * Writing any value clears CURRENT and COUNTFLAG.
 */
void SysTick_ClearCurrentValue(void);


/**
 * @brief Enable SysTick interrupt.
 */
void SysTick_EnableInterrupt(void);


/**
 * @brief Disable SysTick interrupt.
 */
void SysTick_DisableInterrupt(void);


/**
 * @brief Check whether SysTick reached zero.
 *
 * @return 1 if COUNTFLAG is set, otherwise 0.
 */
uint8_t SysTick_HasElapsed(void);


/**
 * @brief Register a callback executed inside SysTick_Handler.
 */
SysTick_Status_t SysTick_SetCallback(SysTick_Callback_t Callback);


/**
 * @brief Blocking delay in milliseconds.
 *
 * Clock frequency is configured in SysTick_config.h.
 */
SysTick_Status_t SysTick_DelayMs(uint32_t DelayMs);


/**
 * @brief Blocking delay in microseconds.
 *
 * Clock frequency is configured in SysTick_config.h.
 */
SysTick_Status_t SysTick_DelayUs(uint32_t DelayUs);


#endif /* SYSTICK_INTERFACE_H */
