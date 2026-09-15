#ifndef RCC_PRIVATE_H_
#define RCC_PRIVATE_H_

#include <stdint.h>

/*==========================================================
 *                     RCC Base Address
 *==========================================================*/

#define RCC_BASE_ADDRESS        0x40023800UL


/*==========================================================
 *                     RCC Registers
 *==========================================================*/

#define RCC_CR                  (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x00UL))
#define RCC_PLLCFGR             (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x04UL))
#define RCC_CFGR                (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x08UL))
#define RCC_CIR                 (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x0CUL))

#define RCC_AHB1RSTR            (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x10UL))
#define RCC_AHB2RSTR            (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x14UL))
#define RCC_APB1RSTR            (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x20UL))
#define RCC_APB2RSTR            (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x24UL))

#define RCC_AHB1ENR             (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x30UL))
#define RCC_AHB2ENR             (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x34UL))
#define RCC_APB1ENR             (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x40UL))
#define RCC_APB2ENR             (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x44UL))

#define RCC_AHB1LPENR           (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x50UL))
#define RCC_AHB2LPENR           (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x54UL))
#define RCC_APB1LPENR           (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x60UL))
#define RCC_APB2LPENR           (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x64UL))

#define RCC_BDCR                (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x70UL))
#define RCC_CSR                 (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x74UL))

#define RCC_SSCGR               (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x80UL))
#define RCC_PLLI2SCFGR          (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x84UL))
#define RCC_DCKCFGR             (*(volatile uint32_t *)(RCC_BASE_ADDRESS + 0x8CUL))


/*==========================================================
 *                     Bit Operations
 *==========================================================*/

#define RCC_SET_BIT(REG, BIT)       ((REG) |=  (1UL << (BIT)))
#define RCC_CLEAR_BIT(REG, BIT)     ((REG) &= ~(1UL << (BIT)))
#define RCC_READ_BIT(REG, BIT)      (((REG) >> (BIT)) & 1UL)


/*==========================================================
 *                     RCC_CR Bits
 *==========================================================*/

#define RCC_HSION_BIT               0U
#define RCC_HSIRDY_BIT              1U

#define RCC_HSEON_BIT               16U
#define RCC_HSERDY_BIT              17U
#define RCC_HSEBYP_BIT              18U
#define RCC_CSSON_BIT               19U

#define RCC_PLLON_BIT               24U
#define RCC_PLLRDY_BIT              25U

#define RCC_PLLI2SON_BIT            26U
#define RCC_PLLI2SRDY_BIT           27U


/*==========================================================
 *                    RCC_CFGR Fields
 *==========================================================*/

#define RCC_SW_POS                  0U
#define RCC_SWS_POS                 2U
#define RCC_HPRE_POS                4U
#define RCC_PPRE1_POS               10U
#define RCC_PPRE2_POS               13U
#define RCC_RTCPRE_POS              16U
#define RCC_MCO1_POS                21U
#define RCC_MCO1PRE_POS             24U
#define RCC_MCO2PRE_POS             27U
#define RCC_MCO2_POS                30U

#define RCC_SW_MASK                 (0x3UL << RCC_SW_POS)
#define RCC_SWS_MASK                (0x3UL << RCC_SWS_POS)
#define RCC_HPRE_MASK               (0xFUL << RCC_HPRE_POS)
#define RCC_PPRE1_MASK              (0x7UL << RCC_PPRE1_POS)
#define RCC_PPRE2_MASK              (0x7UL << RCC_PPRE2_POS)


/*==========================================================
 *                  RCC_PLLCFGR Fields
 *==========================================================*/

#define RCC_PLLM_POS                0U
#define RCC_PLLN_POS                6U
#define RCC_PLLP_POS                16U
#define RCC_PLLSRC_POS              22U
#define RCC_PLLQ_POS                24U

#define RCC_PLLM_MASK               (0x3FUL << RCC_PLLM_POS)
#define RCC_PLLN_MASK               (0x1FFUL << RCC_PLLN_POS)
#define RCC_PLLP_MASK               (0x3UL << RCC_PLLP_POS)
#define RCC_PLLSRC_MASK             (0x1UL << RCC_PLLSRC_POS)
#define RCC_PLLQ_MASK               (0xFUL << RCC_PLLQ_POS)


/*==========================================================
 *                     RCC_CIR Fields
 *==========================================================*/

#define RCC_CIR_READY_FLAG_MASK     0x3FUL

#define RCC_CIR_READY_IE_POS        8U
#define RCC_CIR_READY_CLEAR_POS     16U


/*==========================================================
 *                  RCC_BDCR Fields
 *==========================================================*/

#define RCC_LSEON_BIT               0U
#define RCC_LSERDY_BIT              1U
#define RCC_LSEBYP_BIT              2U

#define RCC_RTCSEL_POS              8U
#define RCC_RTCSEL_MASK             (0x3UL << RCC_RTCSEL_POS)

#define RCC_RTCEN_BIT               15U
#define RCC_BDRST_BIT               16U


/*==========================================================
 *                   RCC_CSR Fields
 *==========================================================*/

#define RCC_LSION_BIT               0U
#define RCC_LSIRDY_BIT              1U

#define RCC_RMVF_BIT                24U


/*==========================================================
 *                     RCC_SSCGR Fields
 *==========================================================*/

#define RCC_MODPER_POS              0U
#define RCC_INCSTEP_POS             13U

#define RCC_MODPER_MASK             (0x1FFFUL << RCC_MODPER_POS)
#define RCC_INCSTEP_MASK            (0x7FFFUL << RCC_INCSTEP_POS)

#define RCC_SPREADSEL_BIT           30U
#define RCC_SSCGEN_BIT              31U


/*==========================================================
 *                  RCC_PLLI2SCFGR Fields
 *==========================================================*/

#define RCC_PLLI2SN_POS             6U
#define RCC_PLLI2SR_POS             28U

#define RCC_PLLI2SN_MASK            (0x1FFUL << RCC_PLLI2SN_POS)
#define RCC_PLLI2SR_MASK            (0x7UL << RCC_PLLI2SR_POS)


/*==========================================================
 *                    RCC_DCKCFGR
 *==========================================================*/

#define RCC_TIMPRE_BIT              24U

#endif /* RCC_PRIVATE_H_ */
