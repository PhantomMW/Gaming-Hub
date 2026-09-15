#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#include <stdint.h>


/* =========================================================
 * Peripheral Base Addresses
 * ========================================================= */

#define EXTI_BASE_ADDRESS        (0x40013C00UL)
#define SYSCFG_BASE_ADDRESS      (0x40013800UL)


/* =========================================================
 * EXTI Registers
 * ========================================================= */

#define EXTI_IMR                 (*(volatile uint32_t *) \
                                  (EXTI_BASE_ADDRESS + 0x00UL))

#define EXTI_EMR                 (*(volatile uint32_t *) \
                                  (EXTI_BASE_ADDRESS + 0x04UL))

#define EXTI_RTSR                (*(volatile uint32_t *) \
                                  (EXTI_BASE_ADDRESS + 0x08UL))

#define EXTI_FTSR                (*(volatile uint32_t *) \
                                  (EXTI_BASE_ADDRESS + 0x0CUL))

#define EXTI_SWIER               (*(volatile uint32_t *) \
                                  (EXTI_BASE_ADDRESS + 0x10UL))

#define EXTI_PR                  (*(volatile uint32_t *) \
                                  (EXTI_BASE_ADDRESS + 0x14UL))


/* =========================================================
 * SYSCFG External Interrupt Configuration Registers
 *
 * EXTICR1 -> EXTI Lines 0  to 3
 * EXTICR2 -> EXTI Lines 4  to 7
 * EXTICR3 -> EXTI Lines 8  to 11
 * EXTICR4 -> EXTI Lines 12 to 15
 * ========================================================= */

#define SYSCFG_EXTICR1           (*(volatile uint32_t *) \
                                  (SYSCFG_BASE_ADDRESS + 0x08UL))

#define SYSCFG_EXTICR2           (*(volatile uint32_t *) \
                                  (SYSCFG_BASE_ADDRESS + 0x0CUL))

#define SYSCFG_EXTICR3           (*(volatile uint32_t *) \
                                  (SYSCFG_BASE_ADDRESS + 0x10UL))

#define SYSCFG_EXTICR4           (*(volatile uint32_t *) \
                                  (SYSCFG_BASE_ADDRESS + 0x14UL))


/* =========================================================
 * Private Constants
 * ========================================================= */

#define EXTI_MAX_GPIO_LINE       (15U)

#define EXTI_EXTICR_FIELD_WIDTH  (4U)

#define EXTI_EXTICR_FIELD_MASK   (0xFUL)


#endif /* EXTI_PRIVATE_H */
