#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_


#include "STD_TYPES.h"


/*==========================================================
 *                    USART Instances
 *==========================================================*/

typedef enum
{
    USART_1 = 0U,
    USART_2,
    USART_6

} USART_ID_t;


/*==========================================================
 *                    USART Mode
 *==========================================================*/

typedef enum
{
    USART_RECEIVER_ONLY = 0U,
    USART_TRANSMITTER_ONLY,
    USART_TRANSMITTER_RECEIVER

} USART_Mode_t;


/*==========================================================
 *                    Word Length
 *==========================================================*/

typedef enum
{
    USART_WORD_LENGTH_8_BIT = 0U,
    USART_WORD_LENGTH_9_BIT

} USART_WordLength_t;


/*==========================================================
 *                    Parity
 *==========================================================*/

typedef enum
{
    USART_PARITY_DISABLE = 0U,
    USART_PARITY_EVEN,
    USART_PARITY_ODD

} USART_Parity_t;


/*==========================================================
 *                    Stop Bits
 *==========================================================*/

typedef enum
{
    USART_STOP_BITS_1 = 0U,
    USART_STOP_BITS_0_5,
    USART_STOP_BITS_2,
    USART_STOP_BITS_1_5

} USART_StopBits_t;


/*==========================================================
 *                    Oversampling
 *==========================================================*/

typedef enum
{
    USART_OVERSAMPLING_16 = 0U,
    USART_OVERSAMPLING_8

} USART_Oversampling_t;


/*==========================================================
 *                    Baud Rate
 *==========================================================*/

typedef enum
{
    USART_BAUDRATE_1200 = 1200U,
    USART_BAUDRATE_2400 = 2400U,
    USART_BAUDRATE_4800 = 4800U,
    USART_BAUDRATE_9600 = 9600U,
    USART_BAUDRATE_19200 = 19200U,
    USART_BAUDRATE_38400 = 38400U,
    USART_BAUDRATE_57600 = 57600U,
    USART_BAUDRATE_115200 = 115200U

} USART_BaudRate_t;


/*==========================================================
 *                    Status Flags
 *==========================================================*/

typedef enum
{
    USART_FLAG_PARITY_ERROR = 0U,
    USART_FLAG_FRAMING_ERROR,
    USART_FLAG_NOISE_ERROR,
    USART_FLAG_OVERRUN_ERROR,
    USART_FLAG_IDLE_LINE,
    USART_FLAG_RECEIVE_NOT_EMPTY,
    USART_FLAG_TRANSMISSION_COMPLETE,
    USART_FLAG_TRANSMIT_DATA_EMPTY,
    USART_FLAG_LBD,
    USART_FLAG_CTS

} USART_Flag_t;


/*==========================================================
 *                    Interrupt Types
 *==========================================================*/

typedef enum
{
    USART_INTERRUPT_PARITY_ERROR = 0U,
    USART_INTERRUPT_TRANSMIT_DATA_EMPTY,
    USART_INTERRUPT_TRANSMISSION_COMPLETE,
    USART_INTERRUPT_RECEIVE_NOT_EMPTY,
    USART_INTERRUPT_IDLE_LINE,
    USART_INTERRUPT_ERROR

} USART_Interrupt_t;


/*==========================================================
 *                    Main Configuration
 *==========================================================*/

typedef struct
{
    USART_Mode_t Mode;

    USART_BaudRate_t BaudRate;

    USART_WordLength_t WordLength;

    USART_Parity_t Parity;

    USART_StopBits_t StopBits;

    USART_Oversampling_t Oversampling;

} USART_Config_t;


/*==========================================================
 *                    USART APIs
 *==========================================================*/


void USART_voidInit(
    USART_ID_t Copy_u8USART,
    const USART_Config_t *Copy_pstConfig
);


void USART_voidEnable(
    USART_ID_t Copy_u8USART
);


void USART_voidDisable(
    USART_ID_t Copy_u8USART
);


void USART_voidSendData(
    USART_ID_t Copy_u8USART,
    u16 Copy_u16Data
);


u16 USART_u16ReceiveData(
    USART_ID_t Copy_u8USART
);


void USART_voidSendString(
    USART_ID_t Copy_u8USART,
    const u8 *Copy_pu8String
);


u8 USART_u8GetFlagStatus(
    USART_ID_t Copy_u8USART,
    USART_Flag_t Copy_u8Flag
);


void USART_voidEnableInterrupt(
    USART_ID_t Copy_u8USART,
    USART_Interrupt_t Copy_u8Interrupt
);


void USART_voidDisableInterrupt(
    USART_ID_t Copy_u8USART,
    USART_Interrupt_t Copy_u8Interrupt
);


#endif /* USART_INTERFACE_H_ */
