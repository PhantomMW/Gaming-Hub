#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

#include <stdint.h>

/* =========================================================
 * GPIO Port Definitions
 * STM32F401CC GPIO ports used by this driver
 * ========================================================= */

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_H
} GPIO_Port_t;


/* =========================================================
 * GPIO Pin Definitions
 * ========================================================= */

typedef enum
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
} GPIO_Pin_t;


/* =========================================================
 * GPIO Pin Mode
 *
 * According to GPIOx_MODER:
 * 00 -> Input
 * 01 -> General Purpose Output
 * 10 -> Alternate Function
 * 11 -> Analog
 * ========================================================= */

typedef enum
{
    GPIO_MODE_INPUT  = 0x00U,
    GPIO_MODE_OUTPUT = 0x01U,
    GPIO_MODE_AF     = 0x02U,
    GPIO_MODE_ANALOG = 0x03U
} GPIO_Mode_t;


/* =========================================================
 * GPIO Output Type
 *
 * 0 -> Push-Pull
 * 1 -> Open-Drain
 * ========================================================= */

typedef enum
{
    GPIO_OUTPUT_PUSH_PULL = 0U,
    GPIO_OUTPUT_OPEN_DRAIN
} GPIO_OutputType_t;


/* =========================================================
 * GPIO Output Speed
 *
 * According to GPIOx_OSPEEDR:
 * 00 -> Low
 * 01 -> Medium
 * 10 -> High
 * 11 -> Very High
 * ========================================================= */

typedef enum
{
    GPIO_SPEED_LOW = 0x00U,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_HIGH,
    GPIO_SPEED_VERY_HIGH
} GPIO_Speed_t;


/* =========================================================
 * GPIO Pull Configuration
 *
 * According to GPIOx_PUPDR:
 * 00 -> No Pull
 * 01 -> Pull-Up
 * 10 -> Pull-Down
 * ========================================================= */

typedef enum
{
    GPIO_NO_PULL = 0x00U,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} GPIO_Pull_t;


/* =========================================================
 * GPIO Pin State
 * ========================================================= */

typedef enum
{
    GPIO_LOW = 0U,
    GPIO_HIGH
} GPIO_State_t;


/* =========================================================
 * GPIO Alternate Functions
 *
 * GPIO supports AF0 to AF15.
 * The actual function available on each pin must be selected
 * according to the STM32F401CC datasheet.
 * ========================================================= */

typedef enum
{
    GPIO_AF0  = 0U,
    GPIO_AF1,
    GPIO_AF2,
    GPIO_AF3,
    GPIO_AF4,
    GPIO_AF5,
    GPIO_AF6,
    GPIO_AF7,
    GPIO_AF8,
    GPIO_AF9,
    GPIO_AF10,
    GPIO_AF11,
    GPIO_AF12,
    GPIO_AF13,
    GPIO_AF14,
    GPIO_AF15
} GPIO_AlternateFunction_t;


/* =========================================================
 * GPIO Status
 * ========================================================= */

typedef enum
{
    GPIO_OK = 0U,
    GPIO_ERROR
} GPIO_Status_t;


/* =========================================================
 * GPIO Pin Configuration Structure
 * ========================================================= */

typedef struct
{
    GPIO_Mode_t              Mode;
    GPIO_OutputType_t        OutputType;
    GPIO_Speed_t             Speed;
    GPIO_Pull_t              Pull;
    GPIO_AlternateFunction_t AlternateFunction;

} GPIO_PinConfig_t;


/* =========================================================
 * GPIO APIs
 * ========================================================= */

/* Configure a GPIO pin */
GPIO_Status_t GPIO_InitPin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin,
    const GPIO_PinConfig_t *Config
);

/* Write HIGH or LOW to one pin */
GPIO_Status_t GPIO_WritePin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin,
    GPIO_State_t State
);

/* Read one pin */
GPIO_State_t GPIO_ReadPin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin
);

/* Toggle one output pin */
GPIO_Status_t GPIO_TogglePin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin
);

/* Write the complete 16-bit output port */
GPIO_Status_t GPIO_WritePort(
    GPIO_Port_t Port,
    uint16_t Value
);

/* Read the complete 16-bit input port */
uint16_t GPIO_ReadPort(
    GPIO_Port_t Port
);

/* Select Alternate Function for a pin */
GPIO_Status_t GPIO_SetAlternateFunction(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin,
    GPIO_AlternateFunction_t AlternateFunction
);

/* Lock configuration of one or more pins */
GPIO_Status_t GPIO_LockPins(
    GPIO_Port_t Port,
    uint16_t PinMask
);

#endif /* GPIO_INTERFACE_H */
