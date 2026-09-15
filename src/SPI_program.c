#ifndef SPI_PROGRAM_C_
#define SPI_PROGRAM_C_


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "SPI_interface.h"
#include "SPI_private.h"
#include "SPI_config.h"


/* =========================================================
 *                  Private Functions
 * ========================================================= */


/**
 * @brief Get SPI peripheral address.
 */
static SPI_Register_t *SPI_GetAddress(
    SPI_ID_t Copy_u8SPI
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    switch (Copy_u8SPI)
    {
        case SPI_1:

            Local_pSPI = SPI1;

            break;


        case SPI_2:

            Local_pSPI = SPI2;

            break;


        case SPI_3:

            Local_pSPI = SPI3;

            break;


        default:

            Local_pSPI = NULL_PTR;

            break;
    }


    return Local_pSPI;
}


/**
 * @brief Check if SPI ID is valid.
 */
static u8 SPI_IsValidID(
    SPI_ID_t Copy_u8SPI
)
{
    u8 Local_u8State = STD_LOW;


    if (Copy_u8SPI <= SPI_3)
    {
        Local_u8State = STD_HIGH;
    }


    return Local_u8State;
}


/**
 * @brief Get interrupt bit position.
 */
static u8 SPI_GetInterruptBit(
    SPI_Interrupt_t Copy_u8Interrupt
)
{
    u8 Local_u8Bit = 0U;


    switch (Copy_u8Interrupt)
    {
        case SPI_INTERRUPT_ERROR:

            Local_u8Bit =
                SPI_CR2_ERRIE_BIT;

            break;


        case SPI_INTERRUPT_RXNE:

            Local_u8Bit =
                SPI_CR2_RXNEIE_BIT;

            break;


        case SPI_INTERRUPT_TXE:

            Local_u8Bit =
                SPI_CR2_TXEIE_BIT;

            break;


        default:

            Local_u8Bit = 0U;

            break;
    }


    return Local_u8Bit;
}


/**
 * @brief Get DMA bit position.
 */
static u8 SPI_GetDMABit(
    SPI_DMA_t Copy_u8DMA
)
{
    u8 Local_u8Bit = 0U;


    switch (Copy_u8DMA)
    {
        case SPI_DMA_RX:

            Local_u8Bit =
                SPI_CR2_RXDMAEN_BIT;

            break;


        case SPI_DMA_TX:

            Local_u8Bit =
                SPI_CR2_TXDMAEN_BIT;

            break;


        default:

            Local_u8Bit = 0U;

            break;
    }


    return Local_u8Bit;
}


/* =========================================================
 *                  Public Functions
 * ========================================================= */


/**
 * @brief Initialize SPI peripheral.
 */
void SPI_voidInit(
    SPI_ID_t Copy_u8SPI,
    SPI_Config_t *Copy_pstConfig
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    /* Validate parameters */

    if ((Copy_pstConfig == NULL_PTR) ||
        (SPI_IsValidID(Copy_u8SPI) == STD_LOW))
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    /* Disable SPI before configuration */

    CLR_BIT(
        Local_pSPI->CR1,
        SPI_CR1_SPE_BIT
    );


    /* =====================================================
     * Master / Slave Mode
     * ===================================================== */

    if (Copy_pstConfig->Mode == SPI_MASTER)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_MSTR_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_MSTR_BIT
        );
    }


    /* =====================================================
     * Communication Mode
     * ===================================================== */

    switch (Copy_pstConfig->CommunicationMode)
    {
        case SPI_FULL_DUPLEX:

            CLR_BIT(
                Local_pSPI->CR1,
                SPI_CR1_BIDIMODE_BIT
            );

            CLR_BIT(
                Local_pSPI->CR1,
                SPI_CR1_RXONLY_BIT
            );

            break;


        case SPI_RECEIVE_ONLY:

            CLR_BIT(
                Local_pSPI->CR1,
                SPI_CR1_BIDIMODE_BIT
            );

            SET_BIT(
                Local_pSPI->CR1,
                SPI_CR1_RXONLY_BIT
            );

            break;


        case SPI_HALF_DUPLEX_RECEIVE:

            SET_BIT(
                Local_pSPI->CR1,
                SPI_CR1_BIDIMODE_BIT
            );

            CLR_BIT(
                Local_pSPI->CR1,
                SPI_CR1_BIDIOE_BIT
            );

            break;


        case SPI_HALF_DUPLEX_TRANSMIT:

            SET_BIT(
                Local_pSPI->CR1,
                SPI_CR1_BIDIMODE_BIT
            );

            SET_BIT(
                Local_pSPI->CR1,
                SPI_CR1_BIDIOE_BIT
            );

            break;


        default:

            return;
    }


    /* =====================================================
     * Clock Polarity
     * ===================================================== */

    if (Copy_pstConfig->ClockPolarity ==
        SPI_CPOL_HIGH)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_CPOL_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_CPOL_BIT
        );
    }


    /* =====================================================
     * Clock Phase
     * ===================================================== */

    if (Copy_pstConfig->ClockPhase ==
        SPI_CPHA_SECOND_EDGE)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_CPHA_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_CPHA_BIT
        );
    }


    /* =====================================================
     * Baud Rate
     * ===================================================== */

    CLR_BIT(
        Local_pSPI->CR1,
        SPI_CR1_BR0_BIT
    );

    CLR_BIT(
        Local_pSPI->CR1,
        SPI_CR1_BR1_BIT
    );

    CLR_BIT(
        Local_pSPI->CR1,
        SPI_CR1_BR2_BIT
    );


    Local_pSPI->CR1 |=
        ((u16)Copy_pstConfig->BaudRate
        << SPI_CR1_BR0_BIT);


    /* =====================================================
     * Data Size
     * ===================================================== */

    if (Copy_pstConfig->DataSize ==
        SPI_DATA_SIZE_16_BIT)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_DFF_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_DFF_BIT
        );
    }


    /* =====================================================
     * Bit Order
     * ===================================================== */

    if (Copy_pstConfig->BitOrder ==
        SPI_LSB_FIRST)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_LSBFIRST_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_LSBFIRST_BIT
        );
    }


    /* =====================================================
     * NSS Management
     * ===================================================== */

    if (Copy_pstConfig->NSSManagement ==
        SPI_NSS_SOFTWARE)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_SSM_BIT
        );

        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_SSI_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_SSM_BIT
        );

        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_SSI_BIT
        );
    }


    /* =====================================================
     * CRC
     * ===================================================== */

    if (Copy_pstConfig->CRCState ==
        SPI_CRC_ENABLE)
    {
        SET_BIT(
            Local_pSPI->CR1,
            SPI_CR1_CRCEN_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR1,
            SPI_CR1_CRCEN_BIT
        );
    }


    /* =====================================================
     * Frame Format
     * ===================================================== */

    if (Copy_pstConfig->FrameFormat ==
        SPI_TI_MODE)
    {
        SET_BIT(
            Local_pSPI->CR2,
            SPI_CR2_FRF_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR2,
            SPI_CR2_FRF_BIT
        );
    }
}


/**
 * @brief Enable SPI peripheral.
 */
void SPI_voidEnable(
    SPI_ID_t Copy_u8SPI
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    SET_BIT(
        Local_pSPI->CR1,
        SPI_CR1_SPE_BIT
    );
}


/**
 * @brief Disable SPI peripheral.
 */
void SPI_voidDisable(
    SPI_ID_t Copy_u8SPI
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    CLR_BIT(
        Local_pSPI->CR1,
        SPI_CR1_SPE_BIT
    );
}


/**
 * @brief Transmit one data frame.
 */
void SPI_voidTransmit(
    SPI_ID_t Copy_u8SPI,
    u16 Copy_u16Data
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    while (GET_BIT(
               Local_pSPI->SR,
               SPI_SR_TXE_BIT
           ) == 0U)
    {
    }


    Local_pSPI->DR =
        Copy_u16Data;
}


/**
 * @brief Receive one data frame.
 */
u16 SPI_u16Receive(
    SPI_ID_t Copy_u8SPI
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;
    u16 Local_u16ReceivedData = 0U;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return Local_u16ReceivedData;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return Local_u16ReceivedData;
    }


    while (GET_BIT(
               Local_pSPI->SR,
               SPI_SR_RXNE_BIT
           ) == 0U)
    {
    }


    Local_u16ReceivedData =
        Local_pSPI->DR;


    return Local_u16ReceivedData;
}


/**
 * @brief Transmit and receive one data frame.
 */
u16 SPI_u16TransmitReceive(
    SPI_ID_t Copy_u8SPI,
    u16 Copy_u16Data
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;
    u16 Local_u16ReceivedData = 0U;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return Local_u16ReceivedData;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return Local_u16ReceivedData;
    }


    while (GET_BIT(
               Local_pSPI->SR,
               SPI_SR_TXE_BIT
           ) == 0U)
    {
    }


    Local_pSPI->DR =
        Copy_u16Data;


    while (GET_BIT(
               Local_pSPI->SR,
               SPI_SR_RXNE_BIT
           ) == 0U)
    {
    }


    Local_u16ReceivedData =
        Local_pSPI->DR;


    return Local_u16ReceivedData;
}


/**
 * @brief Get SPI flag status.
 */
u8 SPI_u8GetFlagStatus(
    SPI_ID_t Copy_u8SPI,
    SPI_Flag_t Copy_u8Flag
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return STD_LOW;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return STD_LOW;
    }


    return (u8)GET_BIT(
        Local_pSPI->SR,
        Copy_u8Flag
    );
}


/**
 * @brief Enable SPI interrupt.
 */
void SPI_voidEnableInterrupt(
    SPI_ID_t Copy_u8SPI,
    SPI_Interrupt_t Copy_u8Interrupt
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;
    u8 Local_u8InterruptBit;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    Local_u8InterruptBit =
        SPI_GetInterruptBit(
            Copy_u8Interrupt
        );


    SET_BIT(
        Local_pSPI->CR2,
        Local_u8InterruptBit
    );
}


/**
 * @brief Disable SPI interrupt.
 */
void SPI_voidDisableInterrupt(
    SPI_ID_t Copy_u8SPI,
    SPI_Interrupt_t Copy_u8Interrupt
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;
    u8 Local_u8InterruptBit;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    Local_u8InterruptBit =
        SPI_GetInterruptBit(
            Copy_u8Interrupt
        );


    CLR_BIT(
        Local_pSPI->CR2,
        Local_u8InterruptBit
    );
}


/**
 * @brief Enable SPI DMA request.
 */
void SPI_voidEnableDMA(
    SPI_ID_t Copy_u8SPI,
    SPI_DMA_t Copy_u8DMA
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;
    u8 Local_u8DMABit;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    Local_u8DMABit =
        SPI_GetDMABit(
            Copy_u8DMA
        );


    SET_BIT(
        Local_pSPI->CR2,
        Local_u8DMABit
    );
}


/**
 * @brief Disable SPI DMA request.
 */
void SPI_voidDisableDMA(
    SPI_ID_t Copy_u8SPI,
    SPI_DMA_t Copy_u8DMA
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;
    u8 Local_u8DMABit;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    Local_u8DMABit =
        SPI_GetDMABit(
            Copy_u8DMA
        );


    CLR_BIT(
        Local_pSPI->CR2,
        Local_u8DMABit
    );
}


/**
 * @brief Set SPI CRC polynomial.
 */
void SPI_voidSetCRCPolynomial(
    SPI_ID_t Copy_u8SPI,
    u16 Copy_u16Polynomial
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    Local_pSPI->CRCPR =
        Copy_u16Polynomial;
}


/**
 * @brief Set SPI frame format.
 */
void SPI_voidSetFrameFormat(
    SPI_ID_t Copy_u8SPI,
    SPI_FrameFormat_t Copy_u8FrameFormat
)
{
    SPI_Register_t *Local_pSPI = NULL_PTR;


    if (SPI_IsValidID(Copy_u8SPI) == STD_LOW)
    {
        return;
    }


    Local_pSPI =
        SPI_GetAddress(Copy_u8SPI);


    if (Local_pSPI == NULL_PTR)
    {
        return;
    }


    if (Copy_u8FrameFormat ==
        SPI_TI_MODE)
    {
        SET_BIT(
            Local_pSPI->CR2,
            SPI_CR2_FRF_BIT
        );
    }
    else
    {
        CLR_BIT(
            Local_pSPI->CR2,
            SPI_CR2_FRF_BIT
        );
    }
}


#endif /* SPI_PROGRAM_C_ */
