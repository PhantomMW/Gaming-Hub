#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "USART_config.h"
#include "USART_interface.h"
#include "USART_private.h"


/*==========================================================
 *                    Private Functions
 *==========================================================*/

static USART_Registers_t *USART_pstGetPeripheral(
    USART_ID_t Copy_u8USART
)
{
    USART_Registers_t *Local_pstUSART = NULL_PTR;


    switch (Copy_u8USART)
    {
        case USART_1:
            Local_pstUSART = USART1;
            break;


        case USART_2:
            Local_pstUSART = USART2;
            break;


        case USART_6:
            Local_pstUSART = USART6;
            break;


        default:
            Local_pstUSART = NULL_PTR;
            break;
    }


    return Local_pstUSART;
}


/*==========================================================
 *                    Baud Rate Configuration
 *==========================================================*/

static void USART_voidSetBaudRate(
    USART_Registers_t *Copy_pstUSART,
    u32 Copy_u32BaudRate,
    USART_Oversampling_t Copy_u8Oversampling
)
{
    u32 Local_u32USARTDIV;


    if (Copy_u8Oversampling == USART_OVERSAMPLING_16)
    {
        /*
         * BRR = PCLK / BaudRate
         *
         * OVER8 = 0
         */

        Local_u32USARTDIV =
            (USART_PERIPHERAL_CLOCK + (Copy_u32BaudRate / 2U))
            / Copy_u32BaudRate;


        Copy_pstUSART->BRR =
            Local_u32USARTDIV;
    }
    else
    {
        /*
         * OVER8 = 1
         *
         * DIV_fraction[3] must be kept cleared.
         */

        Local_u32USARTDIV =
            (2U * USART_PERIPHERAL_CLOCK
             + (Copy_u32BaudRate / 2U))
            / Copy_u32BaudRate;


        Copy_pstUSART->BRR =
            (Local_u32USARTDIV & 0xFFF0UL)
            |
            ((Local_u32USARTDIV & 0x000FUL) >> 1U);
    }
}


/*==========================================================
 *                    USART Initialization
 *==========================================================*/

void USART_voidInit(
    USART_ID_t Copy_u8USART,
    const USART_Config_t *Copy_pstConfig
)
{
    USART_Registers_t *Local_pstUSART;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if ((Local_pstUSART != NULL_PTR)
        &&
        (Copy_pstConfig != NULL_PTR))
    {
        /*==================================================
         * Disable USART Before Configuration
         *==================================================*/

        CLR_BIT(
            Local_pstUSART->CR1,
            USART_CR1_UE_BIT
        );


        /*==================================================
         * Configure Oversampling
         *==================================================*/

        if (Copy_pstConfig->Oversampling
            == USART_OVERSAMPLING_8)
        {
            SET_BIT(
                Local_pstUSART->CR1,
                USART_CR1_OVER8_BIT
            );
        }
        else
        {
            CLR_BIT(
                Local_pstUSART->CR1,
                USART_CR1_OVER8_BIT
            );
        }


        /*==================================================
         * Configure Word Length
         *==================================================*/

        if (Copy_pstConfig->WordLength
            == USART_WORD_LENGTH_9_BIT)
        {
            SET_BIT(
                Local_pstUSART->CR1,
                USART_CR1_M_BIT
            );
        }
        else
        {
            CLR_BIT(
                Local_pstUSART->CR1,
                USART_CR1_M_BIT
            );
        }


        /*==================================================
         * Configure Parity
         *==================================================*/

        switch (Copy_pstConfig->Parity)
        {
            case USART_PARITY_EVEN:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PCE_BIT
                );


                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PS_BIT
                );

                break;


            case USART_PARITY_ODD:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PCE_BIT
                );


                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PS_BIT
                );

                break;


            case USART_PARITY_DISABLE:
            default:

                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PCE_BIT
                );

                break;
        }


        /*==================================================
         * Configure Stop Bits
         *==================================================*/

        Local_pstUSART->CR2 &=
            ~(0x03UL << USART_CR2_STOP_POS);


        Local_pstUSART->CR2 |=
            ((u32)Copy_pstConfig->StopBits
             << USART_CR2_STOP_POS);


        /*==================================================
         * Configure USART Mode
         *==================================================*/

        switch (Copy_pstConfig->Mode)
        {
            case USART_RECEIVER_ONLY:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_RE_BIT
                );


                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TE_BIT
                );

                break;


            case USART_TRANSMITTER_ONLY:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TE_BIT
                );


                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_RE_BIT
                );

                break;


            case USART_TRANSMITTER_RECEIVER:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TE_BIT
                );


                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_RE_BIT
                );

                break;


            default:
                break;
        }


        /*==================================================
         * Configure Baud Rate
         *==================================================*/

        USART_voidSetBaudRate(
            Local_pstUSART,
            (u32)Copy_pstConfig->BaudRate,
            Copy_pstConfig->Oversampling
        );
    }
}


/*==========================================================
 *                    Enable USART
 *==========================================================*/

void USART_voidEnable(
    USART_ID_t Copy_u8USART
)
{
    USART_Registers_t *Local_pstUSART;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        SET_BIT(
            Local_pstUSART->CR1,
            USART_CR1_UE_BIT
        );
    }
}


/*==========================================================
 *                    Disable USART
 *==========================================================*/

void USART_voidDisable(
    USART_ID_t Copy_u8USART
)
{
    USART_Registers_t *Local_pstUSART;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        CLR_BIT(
            Local_pstUSART->CR1,
            USART_CR1_UE_BIT
        );
    }
}


/*==========================================================
 *                    Send Data
 *==========================================================*/

void USART_voidSendData(
    USART_ID_t Copy_u8USART,
    u16 Copy_u16Data
)
{
    USART_Registers_t *Local_pstUSART;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        /*
         * Wait until transmit data register is empty.
         */

        while (GET_BIT(
                   Local_pstUSART->SR,
                   USART_SR_TXE_BIT
               ) == 0U)
        {
        }


        /*
         * Write data.
         */

        Local_pstUSART->DR =
            (u32)(Copy_u16Data & 0x01FFU);
    }
}


/*==========================================================
 *                    Receive Data
 *==========================================================*/

u16 USART_u16ReceiveData(
    USART_ID_t Copy_u8USART
)
{
    USART_Registers_t *Local_pstUSART;
    u16 Local_u16Data = 0U;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        /*
         * Wait until data is received.
         */

        while (GET_BIT(
                   Local_pstUSART->SR,
                   USART_SR_RXNE_BIT
               ) == 0U)
        {
        }


        /*
         * Read received data.
         */

        Local_u16Data =
            (u16)(Local_pstUSART->DR & 0x01FFU);
    }


    return Local_u16Data;
}


/*==========================================================
 *                    Send String
 *==========================================================*/

void USART_voidSendString(
    USART_ID_t Copy_u8USART,
    const u8 *Copy_pu8String
)
{
    if (Copy_pu8String != NULL_PTR)
    {
        while (*Copy_pu8String != '\0')
        {
            USART_voidSendData(
                Copy_u8USART,
                (u16)*Copy_pu8String
            );


            Copy_pu8String++;
        }
    }
}


/*==========================================================
 *                    Get Flag Status
 *==========================================================*/

u8 USART_u8GetFlagStatus(
    USART_ID_t Copy_u8USART,
    USART_Flag_t Copy_u8Flag
)
{
    USART_Registers_t *Local_pstUSART;
    u8 Local_u8Status = STD_LOW;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        Local_u8Status =
            (u8)GET_BIT(
                Local_pstUSART->SR,
                Copy_u8Flag
            );
    }


    return Local_u8Status;
}


/*==========================================================
 *                    Enable Interrupt
 *==========================================================*/

void USART_voidEnableInterrupt(
    USART_ID_t Copy_u8USART,
    USART_Interrupt_t Copy_u8Interrupt
)
{
    USART_Registers_t *Local_pstUSART;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        switch (Copy_u8Interrupt)
        {
            case USART_INTERRUPT_PARITY_ERROR:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PEIE_BIT
                );

                break;


            case USART_INTERRUPT_TRANSMIT_DATA_EMPTY:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TXEIE_BIT
                );

                break;


            case USART_INTERRUPT_TRANSMISSION_COMPLETE:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TCIE_BIT
                );

                break;


            case USART_INTERRUPT_RECEIVE_NOT_EMPTY:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_RXNEIE_BIT
                );

                break;


            case USART_INTERRUPT_IDLE_LINE:

                SET_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_IDLEIE_BIT
                );

                break;


            case USART_INTERRUPT_ERROR:

                SET_BIT(
                    Local_pstUSART->CR3,
                    USART_CR3_EIE_BIT
                );

                break;


            default:
                break;
        }
    }
}


/*==========================================================
 *                    Disable Interrupt
 *==========================================================*/

void USART_voidDisableInterrupt(
    USART_ID_t Copy_u8USART,
    USART_Interrupt_t Copy_u8Interrupt
)
{
    USART_Registers_t *Local_pstUSART;


    Local_pstUSART =
        USART_pstGetPeripheral(Copy_u8USART);


    if (Local_pstUSART != NULL_PTR)
    {
        switch (Copy_u8Interrupt)
        {
            case USART_INTERRUPT_PARITY_ERROR:

                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_PEIE_BIT
                );

                break;


            case USART_INTERRUPT_TRANSMIT_DATA_EMPTY:

                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TXEIE_BIT
                );

                break;


            case USART_INTERRUPT_TRANSMISSION_COMPLETE:

                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_TCIE_BIT
                );

                break;


            case USART_INTERRUPT_RECEIVE_NOT_EMPTY:

                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_RXNEIE_BIT
                );

                break;


            case USART_INTERRUPT_IDLE_LINE:

                CLR_BIT(
                    Local_pstUSART->CR1,
                    USART_CR1_IDLEIE_BIT
                );

                break;


            case USART_INTERRUPT_ERROR:

                CLR_BIT(
                    Local_pstUSART->CR3,
                    USART_CR3_EIE_BIT
                );

                break;


            default:
                break;
        }
    }
}
