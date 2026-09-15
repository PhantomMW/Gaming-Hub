#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include <stdint.h>


/* =========================================================
 * GPIO Base Addresses
 * ========================================================= */

#define GPIOA_BASE_ADDRESS     (0x40020000UL)
#define GPIOB_BASE_ADDRESS     (0x40020400UL)
#define GPIOC_BASE_ADDRESS     (0x40020800UL)
#define GPIOH_BASE_ADDRESS     (0x40021C00UL)


/* =========================================================
 * GPIO Register Structure
 *
 * Register offsets according to RM0368 GPIO register map.
 * ========================================================= */

typedef struct
{
    volatile uint32_t MODER;      /* 0x00 */
    volatile uint32_t OTYPER;     /* 0x04 */
    volatile uint32_t OSPEEDR;    /* 0x08 */
    volatile uint32_t PUPDR;      /* 0x0C */
    volatile uint32_t IDR;        /* 0x10 */
    volatile uint32_t ODR;        /* 0x14 */
    volatile uint32_t BSRR;       /* 0x18 */
    volatile uint32_t LCKR;       /* 0x1C */
    volatile uint32_t AFRL;       /* 0x20 */
    volatile uint32_t AFRH;       /* 0x24 */

} GPIO_Registers_t;


/* =========================================================
 * GPIO Peripheral Pointers
 * ========================================================= */

#define GPIOA   ((GPIO_Registers_t *)GPIOA_BASE_ADDRESS)
#define GPIOB   ((GPIO_Registers_t *)GPIOB_BASE_ADDRESS)
#define GPIOC   ((GPIO_Registers_t *)GPIOC_BASE_ADDRESS)
#define GPIOH   ((GPIO_Registers_t *)GPIOH_BASE_ADDRESS)


/* =========================================================
 * Internal Constants
 * ========================================================= */

#define GPIO_PINS_NUMBER          (16U)
#define GPIO_MODE_BITS_WIDTH      (2U)
#define GPIO_AF_BITS_WIDTH        (4U)

#define GPIO_LCKK_BIT             (16U)

#endif /* GPIO_PRIVATE_H */
