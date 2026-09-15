#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

#include "STD_TYPES.h"


/* =========================================================
 *                  SPI Instances
 * ========================================================= */

typedef enum
{
    SPI_1 = 0U,
    SPI_2,
    SPI_3

} SPI_ID_t;


/* =========================================================
 *                  SPI Mode
 * ========================================================= */

typedef enum
{
    SPI_SLAVE = 0U,
    SPI_MASTER

} SPI_Mode_t;


/* =========================================================
 *                  Communication Mode
 * ========================================================= */

typedef enum
{
    SPI_FULL_DUPLEX = 0U,

    SPI_RECEIVE_ONLY,

    SPI_HALF_DUPLEX_RECEIVE,

    SPI_HALF_DUPLEX_TRANSMIT

} SPI_CommunicationMode_t;


/* =========================================================
 *                  Clock Polarity
 * ========================================================= */

typedef enum
{
    SPI_CPOL_LOW = 0U,
    SPI_CPOL_HIGH

} SPI_ClockPolarity_t;


/* =========================================================
 *                  Clock Phase
 * ========================================================= */

typedef enum
{
    SPI_CPHA_FIRST_EDGE = 0U,
    SPI_CPHA_SECOND_EDGE

} SPI_ClockPhase_t;


/* =========================================================
 *                  Baud Rate
 * ========================================================= */

typedef enum
{
    SPI_BAUDRATE_DIV_2 = 0U,
    SPI_BAUDRATE_DIV_4,
    SPI_BAUDRATE_DIV_8,
    SPI_BAUDRATE_DIV_16,
    SPI_BAUDRATE_DIV_32,
    SPI_BAUDRATE_DIV_64,
    SPI_BAUDRATE_DIV_128,
    SPI_BAUDRATE_DIV_256

} SPI_BaudRate_t;


/* =========================================================
 *                  Data Size
 * ========================================================= */

typedef enum
{
    SPI_DATA_SIZE_8_BIT = 0U,
    SPI_DATA_SIZE_16_BIT

} SPI_DataSize_t;


/* =========================================================
 *                  Bit Order
 * ========================================================= */

typedef enum
{
    SPI_MSB_FIRST = 0U,
    SPI_LSB_FIRST

} SPI_BitOrder_t;


/* =========================================================
 *                  NSS Management
 * ========================================================= */

typedef enum
{
    SPI_NSS_HARDWARE = 0U,
    SPI_NSS_SOFTWARE

} SPI_NSSManagement_t;


/* =========================================================
 *                  CRC State
 * ========================================================= */

typedef enum
{
    SPI_CRC_DISABLE = 0U,
    SPI_CRC_ENABLE

} SPI_CRCState_t;


/* =========================================================
 *                  Frame Format
 * ========================================================= */

typedef enum
{
    SPI_MOTOROLA_MODE = 0U,
    SPI_TI_MODE

} SPI_FrameFormat_t;


/* =========================================================
 *                  Status Flags
 * ========================================================= */

typedef enum
{
    SPI_FLAG_RXNE   = 0U,
    SPI_FLAG_TXE    = 1U,
    SPI_FLAG_CRCERR = 4U,
    SPI_FLAG_MODF   = 5U,
    SPI_FLAG_OVR    = 6U,
    SPI_FLAG_BSY    = 7U,
    SPI_FLAG_FRE    = 8U

} SPI_Flag_t;


/* =========================================================
 *                  Interrupt Types
 * ========================================================= */

typedef enum
{
    SPI_INTERRUPT_ERROR = 0U,
    SPI_INTERRUPT_RXNE,
    SPI_INTERRUPT_TXE

} SPI_Interrupt_t;


/* =========================================================
 *                  DMA Types
 * ========================================================= */

typedef enum
{
    SPI_DMA_RX = 0U,
    SPI_DMA_TX

} SPI_DMA_t;


/* =========================================================
 *                  Main Configuration
 * ========================================================= */

typedef struct
{
    SPI_Mode_t Mode;

    SPI_CommunicationMode_t CommunicationMode;

    SPI_ClockPolarity_t ClockPolarity;

    SPI_ClockPhase_t ClockPhase;

    SPI_BaudRate_t BaudRate;

    SPI_DataSize_t DataSize;

    SPI_BitOrder_t BitOrder;

    SPI_NSSManagement_t NSSManagement;

    SPI_CRCState_t CRCState;

    SPI_FrameFormat_t FrameFormat;

} SPI_Config_t;


/* =========================================================
 *                  SPI APIs
 * ========================================================= */

/* Initialization */

void SPI_voidInit(
    SPI_ID_t Copy_u8SPI,
    SPI_Config_t *Copy_pstConfig
);


/* Enable / Disable */

void SPI_voidEnable(
    SPI_ID_t Copy_u8SPI
);

void SPI_voidDisable(
    SPI_ID_t Copy_u8SPI
);


/* Data Transfer */

void SPI_voidTransmit(
    SPI_ID_t Copy_u8SPI,
    u16 Copy_u16Data
);

u16 SPI_u16Receive(
    SPI_ID_t Copy_u8SPI
);

u16 SPI_u16TransmitReceive(
    SPI_ID_t Copy_u8SPI,
    u16 Copy_u16Data
);


/* Status */

u8 SPI_u8GetFlagStatus(
    SPI_ID_t Copy_u8SPI,
    SPI_Flag_t Copy_u8Flag
);


/* Interrupts */

void SPI_voidEnableInterrupt(
    SPI_ID_t Copy_u8SPI,
    SPI_Interrupt_t Copy_u8Interrupt
);

void SPI_voidDisableInterrupt(
    SPI_ID_t Copy_u8SPI,
    SPI_Interrupt_t Copy_u8Interrupt
);


/* DMA */

void SPI_voidEnableDMA(
    SPI_ID_t Copy_u8SPI,
    SPI_DMA_t Copy_u8DMA
);

void SPI_voidDisableDMA(
    SPI_ID_t Copy_u8SPI,
    SPI_DMA_t Copy_u8DMA
);


/* CRC */

void SPI_voidSetCRCPolynomial(
    SPI_ID_t Copy_u8SPI,
    u16 Copy_u16Polynomial
);


/* Frame Format */

void SPI_voidSetFrameFormat(
    SPI_ID_t Copy_u8SPI,
    SPI_FrameFormat_t Copy_u8FrameFormat
);


#endif /* SPI_INTERFACE_H_ */
