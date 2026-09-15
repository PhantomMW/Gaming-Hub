#include "EXTI_interface.h"
#include "EXTI_private.h"
#include "EXTI_config.h"
#include <stddef.h>


/* =========================================================
 * Private Functions
 * ========================================================= */

static uint8_t EXTI_IsValidLine(EXTI_Line_t Line)
{
    if ((uint8_t)Line < EXTI_NUMBER_OF_GPIO_LINES)
    {
        return 1U;
    }

    return 0U;
}


static uint8_t EXTI_IsValidPort(EXTI_Port_t Port)
{
    switch (Port)
    {
        case EXTI_PORT_A:
        case EXTI_PORT_B:
        case EXTI_PORT_C:
        case EXTI_PORT_D:
        case EXTI_PORT_E:
        case EXTI_PORT_H:
            return 1U;

        default:
            return 0U;
    }
}


static uint8_t EXTI_IsValidTrigger(EXTI_Trigger_t Trigger)
{
    if ((Trigger == EXTI_TRIGGER_RISING) ||
        (Trigger == EXTI_TRIGGER_FALLING) ||
        (Trigger == EXTI_TRIGGER_BOTH))
    {
        return 1U;
    }

    return 0U;
}


static uint8_t EXTI_IsValidMode(EXTI_Mode_t Mode)
{
    if ((Mode == EXTI_MODE_INTERRUPT) ||
        (Mode == EXTI_MODE_EVENT))
    {
        return 1U;
    }

    return 0U;
}


/* =========================================================
 * EXTI GPIO Source Selection
 *
 * EXTICR1 -> Lines 0  to 3
 * EXTICR2 -> Lines 4  to 7
 * EXTICR3 -> Lines 8  to 11
 * EXTICR4 -> Lines 12 to 15
 * ========================================================= */

EXTI_Status_t EXTI_SetPort(
    EXTI_Line_t Line,
    EXTI_Port_t Port
)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;
    volatile uint32_t *EXTICR;

    if ((EXTI_IsValidLine(Line) == 0U) ||
        (EXTI_IsValidPort(Port) == 0U))
    {
        return EXTI_ERROR;
    }

    RegisterIndex = (uint8_t)Line / 4U;
    BitPosition =
        ((uint8_t)Line % 4U) * EXTI_EXTICR_FIELD_WIDTH;

    /*
     * EXTICR registers are consecutive starting from EXTICR1.
     */
    EXTICR = &SYSCFG_EXTICR1;

    /* Clear old port selection */
    EXTICR[RegisterIndex] &=
        ~(EXTI_EXTICR_FIELD_MASK << BitPosition);

    /* Select new GPIO port */
    EXTICR[RegisterIndex] |=
        ((uint32_t)Port << BitPosition);

    return EXTI_OK;
}


/* =========================================================
 * Trigger Configuration
 * ========================================================= */

EXTI_Status_t EXTI_SetTrigger(
    EXTI_Line_t Line,
    EXTI_Trigger_t Trigger
)
{
    uint32_t LineMask;

    if ((EXTI_IsValidLine(Line) == 0U) ||
        (EXTI_IsValidTrigger(Trigger) == 0U))
    {
        return EXTI_ERROR;
    }

    LineMask = (1UL << (uint8_t)Line);

    /*
     * Clear the line from both trigger registers first.
     */
    EXTI_RTSR &= ~LineMask;
    EXTI_FTSR &= ~LineMask;


    switch (Trigger)
    {
        case EXTI_TRIGGER_RISING:

            EXTI_RTSR |= LineMask;
            break;


        case EXTI_TRIGGER_FALLING:

            EXTI_FTSR |= LineMask;
            break;


        case EXTI_TRIGGER_BOTH:

            EXTI_RTSR |= LineMask;
            EXTI_FTSR |= LineMask;
            break;


        default:

            return EXTI_ERROR;
    }

    return EXTI_OK;
}


/* =========================================================
 * Enable Line
 * ========================================================= */

EXTI_Status_t EXTI_EnableLine(
    EXTI_Line_t Line,
    EXTI_Mode_t Mode
)
{
    uint32_t LineMask;

    if ((EXTI_IsValidLine(Line) == 0U) ||
        (EXTI_IsValidMode(Mode) == 0U))
    {
        return EXTI_ERROR;
    }

    LineMask = (1UL << (uint8_t)Line);

    if (Mode == EXTI_MODE_INTERRUPT)
    {
        EXTI_IMR |= LineMask;
    }
    else
    {
        EXTI_EMR |= LineMask;
    }

    return EXTI_OK;
}


/* =========================================================
 * Disable Line
 * ========================================================= */

EXTI_Status_t EXTI_DisableLine(
    EXTI_Line_t Line,
    EXTI_Mode_t Mode
)
{
    uint32_t LineMask;

    if ((EXTI_IsValidLine(Line) == 0U) ||
        (EXTI_IsValidMode(Mode) == 0U))
    {
        return EXTI_ERROR;
    }

    LineMask = (1UL << (uint8_t)Line);

    if (Mode == EXTI_MODE_INTERRUPT)
    {
        EXTI_IMR &= ~LineMask;
    }
    else
    {
        EXTI_EMR &= ~LineMask;
    }

    return EXTI_OK;
}


/* =========================================================
 * EXTI Initialization
 * ========================================================= */

EXTI_Status_t EXTI_Init(
    const EXTI_Config_t *Config
)
{
    EXTI_Status_t Status;

    if (Config == NULL)
    {
        return EXTI_ERROR;
    }

    /*
     * Configure GPIO source.
     * Note: SYSCFG clock must already be enabled.
     */
    Status = EXTI_SetPort(
        Config->Line,
        Config->Port
    );

    if (Status != EXTI_OK)
    {
        return EXTI_ERROR;
    }


    /* Configure trigger edge */
    Status = EXTI_SetTrigger(
        Config->Line,
        Config->Trigger
    );

    if (Status != EXTI_OK)
    {
        return EXTI_ERROR;
    }


    /*
     * Clear any old pending flag before enabling the line.
     */
    Status = EXTI_ClearPending(Config->Line);

    if (Status != EXTI_OK)
    {
        return EXTI_ERROR;
    }


    /* Enable selected mode */
    return EXTI_EnableLine(
        Config->Line,
        Config->Mode
    );
}


/* =========================================================
 * Software Trigger
 * ========================================================= */

EXTI_Status_t EXTI_SoftwareTrigger(
    EXTI_Line_t Line
)
{
    if (EXTI_IsValidLine(Line) == 0U)
    {
        return EXTI_ERROR;
    }

    /*
     * Writing 1 generates a software interrupt/event request.
     */
    EXTI_SWIER |= (1UL << (uint8_t)Line);

    return EXTI_OK;
}


/* =========================================================
 * Get Pending Flag
 * ========================================================= */

EXTI_Status_t EXTI_GetPending(
    EXTI_Line_t Line,
    uint8_t *PendingStatus
)
{
    if ((EXTI_IsValidLine(Line) == 0U) ||
        (PendingStatus == NULL))
    {
        return EXTI_ERROR;
    }

    *PendingStatus =
        (uint8_t)((EXTI_PR >> (uint8_t)Line) & 0x01UL);

    return EXTI_OK;
}


/* =========================================================
 * Clear Pending Flag
 * ========================================================= */

EXTI_Status_t EXTI_ClearPending(
    EXTI_Line_t Line
)
{
    if (EXTI_IsValidLine(Line) == 0U)
    {
        return EXTI_ERROR;
    }

    /*
     * Pending bits are cleared by writing 1.
     */
    EXTI_PR = (1UL << (uint8_t)Line);

    return EXTI_OK;
}
