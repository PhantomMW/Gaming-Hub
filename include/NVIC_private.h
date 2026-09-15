#ifndef NVIC_PRIVATE_H
#define NVIC_PRIVATE_H

#include <stdint.h>


/* =========================================================
 * NVIC Base Addresses
 * ========================================================= */

#define NVIC_ISER_BASE_ADDRESS     (0xE000E100UL)
#define NVIC_ICER_BASE_ADDRESS     (0xE000E180UL)
#define NVIC_ISPR_BASE_ADDRESS     (0xE000E200UL)
#define NVIC_ICPR_BASE_ADDRESS     (0xE000E280UL)
#define NVIC_IABR_BASE_ADDRESS     (0xE000E300UL)
#define NVIC_IPR_BASE_ADDRESS      (0xE000E400UL)
#define NVIC_STIR_ADDRESS          (0xE000EF00UL)


/* =========================================================
 * System Control Block
 * ========================================================= */

#define SCB_AIRCR_ADDRESS          (0xE000ED0CUL)


/* =========================================================
 * Register Access Macros
 * ========================================================= */

#define NVIC_ISER   ((volatile uint32_t *)NVIC_ISER_BASE_ADDRESS)
#define NVIC_ICER   ((volatile uint32_t *)NVIC_ICER_BASE_ADDRESS)
#define NVIC_ISPR   ((volatile uint32_t *)NVIC_ISPR_BASE_ADDRESS)
#define NVIC_ICPR   ((volatile uint32_t *)NVIC_ICPR_BASE_ADDRESS)
#define NVIC_IABR   ((volatile uint32_t *)NVIC_IABR_BASE_ADDRESS)

/*
 * Priority registers are byte-addressable.
 * Each IRQ has one 8-bit priority field.
 */
#define NVIC_IPR    ((volatile uint8_t *)NVIC_IPR_BASE_ADDRESS)

#define NVIC_STIR   (*(volatile uint32_t *)NVIC_STIR_ADDRESS)
#define SCB_AIRCR   (*(volatile uint32_t *)SCB_AIRCR_ADDRESS)


/* =========================================================
 * AIRCR Definitions
 * ========================================================= */

#define SCB_AIRCR_VECTKEY          (0x5FAUL << 16U)
#define SCB_AIRCR_PRIGROUP_POS     (8U)
#define SCB_AIRCR_PRIGROUP_MASK    (0x7UL << SCB_AIRCR_PRIGROUP_POS)


/* =========================================================
 * STIR Definitions
 * ========================================================= */

#define NVIC_STIR_INTID_MASK       (0x1FFUL)


#endif /* NVIC_PRIVATE_H */
