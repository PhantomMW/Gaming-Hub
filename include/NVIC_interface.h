#ifndef NVIC_INTERFACE_H
#define NVIC_INTERFACE_H

#include <stdint.h>


/* =========================================================
 * NVIC Status
 * ========================================================= */

typedef enum
{
    NVIC_OK = 0U,
    NVIC_ERROR
} NVIC_Status_t;


/* =========================================================
 * Priority Grouping
 *
 * STM32F401 implements 4 priority bits.
 *
 * Group 0 -> 4 Group bits, 0 Sub bits
 * Group 1 -> 3 Group bits, 1 Sub bit
 * Group 2 -> 2 Group bits, 2 Sub bits
 * Group 3 -> 1 Group bit,  3 Sub bits
 * Group 4 -> 0 Group bits, 4 Sub bits
 * ========================================================= */

typedef enum
{
    NVIC_PRIORITY_GROUP_4_SUB_0 = 3U,
    NVIC_PRIORITY_GROUP_3_SUB_1 = 4U,
    NVIC_PRIORITY_GROUP_2_SUB_2 = 5U,
    NVIC_PRIORITY_GROUP_1_SUB_3 = 6U,
    NVIC_PRIORITY_GROUP_0_SUB_4 = 7U
} NVIC_PriorityGroup_t;


/* =========================================================
 * Interrupt Control APIs
 * ========================================================= */

NVIC_Status_t NVIC_EnableIRQ(uint8_t IRQNumber);

NVIC_Status_t NVIC_DisableIRQ(uint8_t IRQNumber);


/* =========================================================
 * Pending APIs
 * ========================================================= */

NVIC_Status_t NVIC_SetPendingIRQ(uint8_t IRQNumber);

NVIC_Status_t NVIC_ClearPendingIRQ(uint8_t IRQNumber);

NVIC_Status_t NVIC_GetPendingIRQ(
    uint8_t IRQNumber,
    uint8_t *PendingStatus
);


/* =========================================================
 * Active APIs
 * ========================================================= */

NVIC_Status_t NVIC_GetActiveIRQ(
    uint8_t IRQNumber,
    uint8_t *ActiveStatus
);


/* =========================================================
 * Priority APIs
 * ========================================================= */

/*
 * Set raw priority value.
 *
 * Valid range for STM32F401:
 * 0 -> Highest priority
 * 15 -> Lowest priority
 */
NVIC_Status_t NVIC_SetPriority(
    uint8_t IRQNumber,
    uint8_t Priority
);


NVIC_Status_t NVIC_GetPriority(
    uint8_t IRQNumber,
    uint8_t *Priority
);


/*
 * Configure priority grouping.
 */
NVIC_Status_t NVIC_SetPriorityGrouping(
    NVIC_PriorityGroup_t PriorityGroup
);


/*
 * Configure Group Priority + Subpriority.
 *
 * The valid values depend on the selected Priority Group.
 */
NVIC_Status_t NVIC_SetGroupedPriority(
    uint8_t IRQNumber,
    NVIC_PriorityGroup_t PriorityGroup,
    uint8_t GroupPriority,
    uint8_t SubPriority
);


/* =========================================================
 * Software Interrupt API
 * ========================================================= */

NVIC_Status_t NVIC_TriggerIRQ(uint8_t IRQNumber);


#endif /* NVIC_INTERFACE_H */
