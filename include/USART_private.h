#ifndef USART_PRIVATE_H_
#define USART_PRIVATE_H_


/*==========================================================
 *                    Base Addresses
 *==========================================================*/

#define USART1_BASE_ADDRESS     0x40011000UL

#define USART2_BASE_ADDRESS     0x40004400UL

#define USART6_BASE_ADDRESS     0x40011400UL


/*==========================================================
 *                    USART Registers
 *==========================================================*/

typedef struct
{
    volatile u32 SR;

    volatile u32 DR;

    volatile u32 BRR;

    volatile u32 CR1;

    volatile u32 CR2;

    volatile u32 CR3;

    volatile u32 GTPR;

} USART_Registers_t;


/*==========================================================
 *                    USART Instances
 *==========================================================*/

#define USART1    ((USART_Registers_t *)USART1_BASE_ADDRESS)

#define USART2    ((USART_Registers_t *)USART2_BASE_ADDRESS)

#define USART6    ((USART_Registers_t *)USART6_BASE_ADDRESS)


/*==========================================================
 *                    USART_SR Bits
 *==========================================================*/

#define USART_SR_PE_BIT             0U

#define USART_SR_FE_BIT             1U

#define USART_SR_NF_BIT             2U

#define USART_SR_ORE_BIT            3U

#define USART_SR_IDLE_BIT           4U

#define USART_SR_RXNE_BIT           5U

#define USART_SR_TC_BIT             6U

#define USART_SR_TXE_BIT            7U

#define USART_SR_LBD_BIT            8U

#define USART_SR_CTS_BIT            9U


/*==========================================================
 *                    USART_CR1 Bits
 *==========================================================*/

#define USART_CR1_SBK_BIT           0U

#define USART_CR1_RWU_BIT           1U

#define USART_CR1_RE_BIT            2U

#define USART_CR1_TE_BIT            3U

#define USART_CR1_IDLEIE_BIT        4U

#define USART_CR1_RXNEIE_BIT        5U

#define USART_CR1_TCIE_BIT          6U

#define USART_CR1_TXEIE_BIT         7U

#define USART_CR1_PEIE_BIT          8U

#define USART_CR1_PS_BIT            9U

#define USART_CR1_PCE_BIT           10U

#define USART_CR1_WAKE_BIT          11U

#define USART_CR1_M_BIT             12U

#define USART_CR1_UE_BIT            13U

#define USART_CR1_OVER8_BIT         15U


/*==========================================================
 *                    USART_CR2 Bits
 *==========================================================*/

#define USART_CR2_ADD_POS           0U

#define USART_CR2_LBDL_BIT          5U

#define USART_CR2_LBDIE_BIT         6U

#define USART_CR2_LBCL_BIT          8U

#define USART_CR2_CPHA_BIT          9U

#define USART_CR2_CPOL_BIT          10U

#define USART_CR2_CLKEN_BIT         11U

#define USART_CR2_STOP_POS          12U

#define USART_CR2_LINEN_BIT         14U


/*==========================================================
 *                    USART_CR3 Bits
 *==========================================================*/

#define USART_CR3_EIE_BIT           0U

#define USART_CR3_IREN_BIT          1U

#define USART_CR3_IRLP_BIT          2U

#define USART_CR3_HDSEL_BIT         3U

#define USART_CR3_NACK_BIT          4U

#define USART_CR3_SCEN_BIT          5U

#define USART_CR3_DMAR_BIT          6U

#define USART_CR3_DMAT_BIT          7U

#define USART_CR3_RTSE_BIT          8U

#define USART_CR3_CTSE_BIT          9U

#define USART_CR3_CTSIE_BIT         10U

#define USART_CR3_ONEBIT_BIT        11U


/*==========================================================
 *                    USART_GTPR Bits
 *==========================================================*/

#define USART_GTPR_PSC_POS          0U

#define USART_GTPR_GT_POS           8U


#endif /* USART_PRIVATE_H_ */
