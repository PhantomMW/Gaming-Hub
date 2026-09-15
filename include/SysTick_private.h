#ifndef SYSTICK_PRIVATE_H
#define SYSTICK_PRIVATE_H

#include <stdint.h>


/* =========================================================
 * SysTick Base Address
 * ========================================================= */

#define SYSTICK_BASE_ADDRESS      (0xE000E010UL)


/* =========================================================
 * SysTick Register Structure
 * ========================================================= */

typedef struct
{
    volatile uint32_t CTRL;       /* Offset 0x00 */
    volatile uint32_t LOAD;       /* Offset 0x04 */
    volatile uint32_t VAL;        /* Offset 0x08 */
    volatile const uint32_t CALIB;/* Offset 0x0C */

} SysTick_Registers_t;


#define SYSTICK    ((SysTick_Registers_t *)SYSTICK_BASE_ADDRESS)


/* =========================================================
 * CTRL Register Bits
 * ========================================================= */

#define SYSTICK_ENABLE_BIT         (0U)
#define SYSTICK_TICKINT_BIT        (1U)
#define SYSTICK_CLKSOURCE_BIT      (2U)
#define SYSTICK_COUNTFLAG_BIT      (16U)


/* =========================================================
 * Register Masks
 * ========================================================= */

#define SYSTICK_RELOAD_MASK        (0x00FFFFFFUL)
#define SYSTICK_CURRENT_MASK       (0x00FFFFFFUL)


#endif /* SYSTICK_PRIVATE_H */
