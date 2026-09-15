#ifndef NVIC_CONFIG_H
#define NVIC_CONFIG_H


/* =========================================================
 * STM32F401 NVIC Configuration
 * ========================================================= */

/*
 * STM32F401 has 52 maskable external interrupts.
 * Valid IRQ numbers: 0 -> 51
 */
#define NVIC_NUMBER_OF_IRQS        (52U)


/*
 * STM32F401 implements 4 priority bits.
 * Therefore priority values are from 0 to 15.
 */
#define NVIC_PRIORITY_BITS         (4U)

#define NVIC_MAX_PRIORITY          (15U)


#endif /* NVIC_CONFIG_H */
