#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

#include "STD_TYPES.h"


/* =========================================================
 *                  SPI Base Addresses
 * ========================================================= */

#define SPI1_BASE_ADDRESS    0x40013000UL
#define SPI2_BASE_ADDRESS    0x40003800UL
#define SPI3_BASE_ADDRESS    0x40003C00UL


/* =========================================================
 *                  SPI Register Map
 * ========================================================= */

typedef struct
{
    volatile u16 CR1;
    u16 RESERVED0;

    volatile u16 CR2;
    u16 RESERVED1;

    volatile u16 SR;
    u16 RESERVED2;

    volatile u16 DR;
    u16 RESERVED3;

    volatile u16 CRCPR;
    u16 RESERVED4;

    volatile u16 RXCRCR;
    u16 RESERVED5;

    volatile u16 TXCRCR;
    u16 RESERVED6;

    volatile u16 I2SCFGR;
    u16 RESERVED7;

    volatile u16 I2SPR;
    u16 RESERVED8;

} SPI_Register_t;


/* =========================================================
 *                  SPI Peripheral Pointers
 * ========================================================= */

#define SPI1    ((SPI_Register_t *) SPI1_BASE_ADDRESS)
#define SPI2    ((SPI_Register_t *) SPI2_BASE_ADDRESS)
#define SPI3    ((SPI_Register_t *) SPI3_BASE_ADDRESS)


/* =========================================================
 *                  SPI_CR1 Bits
 * ========================================================= */

#define SPI_CR1_CPHA_BIT            0U
#define SPI_CR1_CPOL_BIT            1U
#define SPI_CR1_MSTR_BIT            2U

#define SPI_CR1_BR0_BIT             3U
#define SPI_CR1_BR1_BIT             4U
#define SPI_CR1_BR2_BIT             5U

#define SPI_CR1_SPE_BIT             6U
#define SPI_CR1_LSBFIRST_BIT        7U
#define SPI_CR1_SSI_BIT             8U
#define SPI_CR1_SSM_BIT             9U
#define SPI_CR1_RXONLY_BIT          10U
#define SPI_CR1_DFF_BIT             11U
#define SPI_CR1_CRCNEXT_BIT         12U
#define SPI_CR1_CRCEN_BIT           13U
#define SPI_CR1_BIDIOE_BIT          14U
#define SPI_CR1_BIDIMODE_BIT        15U


/* =========================================================
 *                  SPI_CR2 Bits
 * ========================================================= */

#define SPI_CR2_RXDMAEN_BIT         0U
#define SPI_CR2_TXDMAEN_BIT         1U
#define SPI_CR2_SSOE_BIT            2U

#define SPI_CR2_FRF_BIT             4U
#define SPI_CR2_ERRIE_BIT           5U
#define SPI_CR2_RXNEIE_BIT          6U
#define SPI_CR2_TXEIE_BIT           7U


/* =========================================================
 *                  SPI_SR Bits
 * ========================================================= */

#define SPI_SR_RXNE_BIT             0U
#define SPI_SR_TXE_BIT              1U

#define SPI_SR_CRCERR_BIT           4U
#define SPI_SR_MODF_BIT             5U
#define SPI_SR_OVR_BIT              6U
#define SPI_SR_BSY_BIT              7U
#define SPI_SR_FRE_BIT              8U


#endif /* SPI_PRIVATE_H_ */
