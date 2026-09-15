#include "STD_Types.h"
#include "Bit_Math.h"

#include "GPIO_interface.h"

#include "DAC_interface.h"
#include "DAC_config.h"
#include "DAC_private.h"


void DAC_Init(void)
{
    GPIO_PinConfig_t Local_Config;

    Local_Config.Mode              = GPIO_MODE_OUTPUT;
    Local_Config.OutputType        = GPIO_OUTPUT_PUSH_PULL;
    Local_Config.Speed             = GPIO_SPEED_LOW;
    Local_Config.Pull              = GPIO_NO_PULL;
    Local_Config.AlternateFunction = GPIO_AF0;


    /* Bit 0 -> Bit 4 */

    GPIO_InitPin(GPIO_PORT_A, GPIO_PIN_8,  &Local_Config);
    GPIO_InitPin(GPIO_PORT_A, GPIO_PIN_9,  &Local_Config);
    GPIO_InitPin(GPIO_PORT_A, GPIO_PIN_10, &Local_Config);
    GPIO_InitPin(GPIO_PORT_A, GPIO_PIN_11, &Local_Config);
    GPIO_InitPin(GPIO_PORT_A, GPIO_PIN_12, &Local_Config);


    /* Bit 5 -> Bit 7 */

    GPIO_InitPin(GPIO_PORT_C, GPIO_PIN_15, &Local_Config);
    GPIO_InitPin(GPIO_PORT_C, GPIO_PIN_13, &Local_Config);
    GPIO_InitPin(GPIO_PORT_C, GPIO_PIN_14, &Local_Config);
}


void DAC_Write(u8 Copy_u8Data)
{
    /* Bit 0 */

    GPIO_WritePin(
        GPIO_PORT_A,
        GPIO_PIN_8,
        (Copy_u8Data >> 0) & 1U
    );


    /* Bit 1 */

    GPIO_WritePin(
        GPIO_PORT_A,
        GPIO_PIN_9,
        (Copy_u8Data >> 1) & 1U
    );


    /* Bit 2 */

    GPIO_WritePin(
        GPIO_PORT_A,
        GPIO_PIN_10,
        (Copy_u8Data >> 2) & 1U
    );


    /* Bit 3 */

    GPIO_WritePin(
        GPIO_PORT_A,
        GPIO_PIN_11,
        (Copy_u8Data >> 3) & 1U
    );


    /* Bit 4 */

    GPIO_WritePin(
        GPIO_PORT_A,
        GPIO_PIN_12,
        (Copy_u8Data >> 4) & 1U
    );


    /* Bit 5 */

    GPIO_WritePin(
        GPIO_PORT_C,
        GPIO_PIN_15,
        (Copy_u8Data >> 5) & 1U
    );


    /* Bit 6 */

    GPIO_WritePin(
        GPIO_PORT_C,
        GPIO_PIN_13,
        (Copy_u8Data >> 6) & 1U
    );


    /* Bit 7 */

    GPIO_WritePin(
        GPIO_PORT_C,
        GPIO_PIN_14,
        (Copy_u8Data >> 7) & 1U
    );
}
