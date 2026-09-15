#include "NVIC_interface.h"
#include "NVIC_private.h"
#include "NVIC_config.h"


/* =========================================================
 * Private Functions
 * ========================================================= */

static uint8_t NVIC_IsValidIRQ(uint8_t IRQNumber)
{
    if (IRQNumber < NVIC_NUMBER_OF_IRQS)
    {
        return 1U;
    }

    return 0U;
}


static uint8_t NVIC_GetRegisterIndex(uint8_t IRQNumber)
{
    return (IRQNumber / 32U);
}


static uint8_t NVIC_GetBitPosition(uint8_t IRQNumber)
{
    return (IRQNumber % 32U);
}


/* =========================================================
 * Enable / Disable Interrupt
 * ========================================================= */

NVIC_Status_t NVIC_EnableIRQ(uint8_t IRQNumber)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;

    if (NVIC_IsValidIRQ(IRQNumber) == 0U)
    {
        return NVIC_ERROR;
    }

    RegisterIndex = NVIC_GetRegisterIndex(IRQNumber);
    BitPosition   = NVIC_GetBitPosition(IRQNumber);

    /*
     * Writing 1 enables the corresponding interrupt.
     * Writing 0 has no effect.
     */
    NVIC_ISER[RegisterIndex] = (1UL << BitPosition);

    return NVIC_OK;
}


NVIC_Status_t NVIC_DisableIRQ(uint8_t IRQNumber)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;

    if (NVIC_IsValidIRQ(IRQNumber) == 0U)
    {
        return NVIC_ERROR;
    }

    RegisterIndex = NVIC_GetRegisterIndex(IRQNumber);
    BitPosition   = NVIC_GetBitPosition(IRQNumber);

    /*
     * Writing 1 disables the corresponding interrupt.
     * Writing 0 has no effect.
     */
    NVIC_ICER[RegisterIndex] = (1UL << BitPosition);

    return NVIC_OK;
}


/* =========================================================
 * Pending Control
 * ========================================================= */

NVIC_Status_t NVIC_SetPendingIRQ(uint8_t IRQNumber)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;

    if (NVIC_IsValidIRQ(IRQNumber) == 0U)
    {
        return NVIC_ERROR;
    }

    RegisterIndex = NVIC_GetRegisterIndex(IRQNumber);
    BitPosition   = NVIC_GetBitPosition(IRQNumber);

    NVIC_ISPR[RegisterIndex] = (1UL << BitPosition);

    return NVIC_OK;
}


NVIC_Status_t NVIC_ClearPendingIRQ(uint8_t IRQNumber)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;

    if (NVIC_IsValidIRQ(IRQNumber) == 0U)
    {
        return NVIC_ERROR;
    }

    RegisterIndex = NVIC_GetRegisterIndex(IRQNumber);
    BitPosition   = NVIC_GetBitPosition(IRQNumber);

    NVIC_ICPR[RegisterIndex] = (1UL << BitPosition);

    return NVIC_OK;
}


NVIC_Status_t NVIC_GetPendingIRQ(
    uint8_t IRQNumber,
    uint8_t *PendingStatus
)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;

    if ((NVIC_IsValidIRQ(IRQNumber) == 0U) ||
        (PendingStatus == 0))
    {
        return NVIC_ERROR;
    }

    RegisterIndex = NVIC_GetRegisterIndex(IRQNumber);
    BitPosition   = NVIC_GetBitPosition(IRQNumber);

    *PendingStatus =
        (uint8_t)((NVIC_ISPR[RegisterIndex] >> BitPosition) & 0x1UL);

    return NVIC_OK;
}


/* =========================================================
 * Active Status
 * ========================================================= */

NVIC_Status_t NVIC_GetActiveIRQ(
    uint8_t IRQNumber,
    uint8_t *ActiveStatus
)
{
    uint8_t RegisterIndex;
    uint8_t BitPosition;

    if ((NVIC_IsValidIRQ(IRQNumber) == 0U) ||
        (ActiveStatus == 0))
    {
        return NVIC_ERROR;
    }

    RegisterIndex = NVIC_GetRegisterIndex(IRQNumber);
    BitPosition   = NVIC_GetBitPosition(IRQNumber);

    *ActiveStatus =
        (uint8_t)((NVIC_IABR[RegisterIndex] >> BitPosition) & 0x1UL);

    return NVIC_OK;
}


/* =========================================================
 * Priority Control
 * ========================================================= */

NVIC_Status_t NVIC_SetPriority(
    uint8_t IRQNumber,
    uint8_t Priority
)
{
    if ((NVIC_IsValidIRQ(IRQNumber) == 0U) ||
        (Priority > NVIC_MAX_PRIORITY))
    {
        return NVIC_ERROR;
    }

    /*
     * STM32F401 implements only the upper 4 bits
     * of each 8-bit priority field.
     */
    NVIC_IPR[IRQNumber] =
        (uint8_t)(Priority << (8U - NVIC_PRIORITY_BITS));

    return NVIC_OK;
}


NVIC_Status_t NVIC_GetPriority(
    uint8_t IRQNumber,
    uint8_t *Priority
)
{
    if ((NVIC_IsValidIRQ(IRQNumber) == 0U) ||
        (Priority == 0))
    {
        return NVIC_ERROR;
    }

    *Priority =
        (uint8_t)(NVIC_IPR[IRQNumber] >>
                  (8U - NVIC_PRIORITY_BITS));

    return NVIC_OK;
}


/* =========================================================
 * Priority Grouping
 * ========================================================= */

NVIC_Status_t NVIC_SetPriorityGrouping(
    NVIC_PriorityGroup_t PriorityGroup
)
{
    uint32_t RegisterValue;

    if ((PriorityGroup < NVIC_PRIORITY_GROUP_4_SUB_0) ||
        (PriorityGroup > NVIC_PRIORITY_GROUP_0_SUB_4))
    {
        return NVIC_ERROR;
    }

    /*
     * AIRCR requires VECTKEY = 0x5FA when writing.
     */

    RegisterValue = SCB_AIRCR;

    /* Clear old PRIGROUP */
    RegisterValue &= ~SCB_AIRCR_PRIGROUP_MASK;

    /* Set new PRIGROUP */
    RegisterValue |=
        ((uint32_t)PriorityGroup << SCB_AIRCR_PRIGROUP_POS);

    /*
     * Write key and preserve required fields.
     */
    SCB_AIRCR =
        (RegisterValue & 0x0000FFFFUL) |
        SCB_AIRCR_VECTKEY;

    return NVIC_OK;
}


/* =========================================================
 * Group + Subpriority
 * ========================================================= */

NVIC_Status_t NVIC_SetGroupedPriority(
    uint8_t IRQNumber,
    NVIC_PriorityGroup_t PriorityGroup,
    uint8_t GroupPriority,
    uint8_t SubPriority
)
{
    uint8_t GroupBits;
    uint8_t SubBits;
    uint8_t Priority;

    if (NVIC_IsValidIRQ(IRQNumber) == 0U)
    {
        return NVIC_ERROR;
    }


    /*
     * STM32F401 has 4 implemented priority bits.
     *
     * PRIGROUP = 3 -> 4 Group / 0 Sub
     * PRIGROUP = 4 -> 3 Group / 1 Sub
     * PRIGROUP = 5 -> 2 Group / 2 Sub
     * PRIGROUP = 6 -> 1 Group / 3 Sub
     * PRIGROUP = 7 -> 0 Group / 4 Sub
     */

    GroupBits =
        (uint8_t)(7U - (uint8_t)PriorityGroup);

    if (GroupBits > NVIC_PRIORITY_BITS)
    {
        GroupBits = NVIC_PRIORITY_BITS;
    }

    SubBits = NVIC_PRIORITY_BITS - GroupBits;


    /* Validate Group Priority */
    if ((GroupBits == 0U && GroupPriority != 0U) ||
        (GroupBits != 0U &&
         GroupPriority >= (1U << GroupBits)))
    {
        return NVIC_ERROR;
    }


    /* Validate Subpriority */
    if ((SubBits == 0U && SubPriority != 0U) ||
        (SubBits != 0U &&
         SubPriority >= (1U << SubBits)))
    {
        return NVIC_ERROR;
    }


    /*
     * Configure grouping first.
     */
    if (NVIC_SetPriorityGrouping(PriorityGroup) != NVIC_OK)
    {
        return NVIC_ERROR;
    }


    Priority =
        (uint8_t)((GroupPriority << SubBits) |
                  SubPriority);

    return NVIC_SetPriority(IRQNumber, Priority);
}


/* =========================================================
 * Software Trigger Interrupt
 * ========================================================= */

NVIC_Status_t NVIC_TriggerIRQ(uint8_t IRQNumber)
{
    if (NVIC_IsValidIRQ(IRQNumber) == 0U)
    {
        return NVIC_ERROR;
    }

    NVIC_STIR =
        ((uint32_t)IRQNumber & NVIC_STIR_INTID_MASK);

    return NVIC_OK;
}
