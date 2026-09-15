#ifndef EXTI_INTERFACE_H
#define EXTI_INTERFACE_H

#include <stdint.h>


/* =========================================================
 * Status Type
 * ========================================================= */

typedef enum
{
    EXTI_OK = 0U,
    EXTI_ERROR
} EXTI_Status_t;


/* =========================================================
 * EXTI Lines
 *
 * External GPIO interrupt lines:
 * EXTI_LINE_0 -> EXTI_LINE_15
 *
 * Other EXTI lines are internal lines.
 * ========================================================= */

typedef enum
{
    EXTI_LINE_0  = 0U,
    EXTI_LINE_1  = 1U,
    EXTI_LINE_2  = 2U,
    EXTI_LINE_3  = 3U,
    EXTI_LINE_4  = 4U,
    EXTI_LINE_5  = 5U,
    EXTI_LINE_6  = 6U,
    EXTI_LINE_7  = 7U,
    EXTI_LINE_8  = 8U,
    EXTI_LINE_9  = 9U,
    EXTI_LINE_10 = 10U,
    EXTI_LINE_11 = 11U,
    EXTI_LINE_12 = 12U,
    EXTI_LINE_13 = 13U,
    EXTI_LINE_14 = 14U,
    EXTI_LINE_15 = 15U

} EXTI_Line_t;


/* =========================================================
 * GPIO Port Selection
 *
 * Values correspond to SYSCFG EXTICR configuration fields.
 * ========================================================= */

typedef enum
{
    EXTI_PORT_A = 0U,
    EXTI_PORT_B = 1U,
    EXTI_PORT_C = 2U,
    EXTI_PORT_D = 3U,
    EXTI_PORT_E = 4U,
    EXTI_PORT_H = 7U

} EXTI_Port_t;


/* =========================================================
 * Trigger Selection
 * ========================================================= */

typedef enum
{
    EXTI_TRIGGER_RISING = 0U,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH

} EXTI_Trigger_t;


/* =========================================================
 * EXTI Mode
 * ========================================================= */

typedef enum
{
    EXTI_MODE_INTERRUPT = 0U,
    EXTI_MODE_EVENT

} EXTI_Mode_t;


/* =========================================================
 * EXTI Configuration Structure
 * ========================================================= */

typedef struct
{
    EXTI_Line_t    Line;
    EXTI_Port_t    Port;
    EXTI_Trigger_t Trigger;
    EXTI_Mode_t    Mode;

} EXTI_Config_t;


/* =========================================================
 * Initialization
 *
 * Configures:
 * - GPIO source for EXTI line
 * - Trigger edge
 * - Interrupt/Event mode
 * ========================================================= */

EXTI_Status_t EXTI_Init(
    const EXTI_Config_t *Config
);


/* =========================================================
 * Enable / Disable
 * ========================================================= */

EXTI_Status_t EXTI_EnableLine(
    EXTI_Line_t Line,
    EXTI_Mode_t Mode
);


EXTI_Status_t EXTI_DisableLine(
    EXTI_Line_t Line,
    EXTI_Mode_t Mode
);


/* =========================================================
 * Trigger Configuration
 * ========================================================= */

EXTI_Status_t EXTI_SetTrigger(
    EXTI_Line_t Line,
    EXTI_Trigger_t Trigger
);


/* =========================================================
 * Software Trigger
 * ========================================================= */

EXTI_Status_t EXTI_SoftwareTrigger(
    EXTI_Line_t Line
);


/* =========================================================
 * Pending Flag Control
 * ========================================================= */

EXTI_Status_t EXTI_GetPending(
    EXTI_Line_t Line,
    uint8_t *PendingStatus
);


EXTI_Status_t EXTI_ClearPending(
    EXTI_Line_t Line
);


/* =========================================================
 * GPIO Source Selection
 *
 * Connects GPIO Port[x] to EXTI Line[x].
 *
 * Example:
 * EXTI_SetPort(EXTI_LINE_0, EXTI_PORT_A)
 * -> PA0 connected to EXTI0
 * ========================================================= */

EXTI_Status_t EXTI_SetPort(
    EXTI_Line_t Line,
    EXTI_Port_t Port
);


#endif /* EXTI_INTERFACE_H */
