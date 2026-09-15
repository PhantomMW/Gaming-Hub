#include "GPIO_interface.h"
#include "GPIO_private.h"
#include "GPIO_config.h"


/* =========================================================
 * Private Functions
 * ========================================================= */

/**
 * @brief Get GPIO register structure based on port selection.
 */
static GPIO_Registers_t *GPIO_GetPortAddress(GPIO_Port_t Port)
{
    GPIO_Registers_t *PortAddress = 0;

    switch (Port)
    {
        case GPIO_PORT_A:
            PortAddress = GPIOA;
            break;

        case GPIO_PORT_B:
            PortAddress = GPIOB;
            break;

        case GPIO_PORT_C:
            PortAddress = GPIOC;
            break;

        case GPIO_PORT_H:
            PortAddress = GPIOH;
            break;

        default:
            PortAddress = 0;
            break;
    }

    return PortAddress;
}


/**
 * @brief Validate GPIO port.
 */
static uint8_t GPIO_IsValidPort(GPIO_Port_t Port)
{
    if (Port <= GPIO_PORT_H)
    {
        return 1U;
    }

    return 0U;
}


/**
 * @brief Validate GPIO pin.
 */
static uint8_t GPIO_IsValidPin(GPIO_Pin_t Pin)
{
    if (Pin < GPIO_PINS_NUMBER)
    {
        return 1U;
    }

    return 0U;
}


/* =========================================================
 * Public Functions
 * ========================================================= */


/**
 * @brief Configure one GPIO pin.
 */
GPIO_Status_t GPIO_InitPin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin,
    const GPIO_PinConfig_t *Config
)
{
    GPIO_Registers_t *GPIOx;
    uint32_t Shift;

    /* Parameter validation */
    if ((Config == 0) ||
        (GPIO_IsValidPort(Port) == 0U) ||
        (GPIO_IsValidPin(Pin) == 0U))
    {
        return GPIO_ERROR;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_ERROR;
    }

    Shift = ((uint32_t)Pin * GPIO_MODE_BITS_WIDTH);


    /* =====================================================
     * Configure Mode - GPIOx_MODER
     * ===================================================== */

    GPIOx->MODER &= ~(0x3UL << Shift);
    GPIOx->MODER |= ((uint32_t)Config->Mode << Shift);


    /* =====================================================
     * Configure Output Type - GPIOx_OTYPER
     * ===================================================== */

    if (Config->OutputType == GPIO_OUTPUT_OPEN_DRAIN)
    {
        GPIOx->OTYPER |= (1UL << Pin);
    }
    else
    {
        GPIOx->OTYPER &= ~(1UL << Pin);
    }


    /* =====================================================
     * Configure Output Speed - GPIOx_OSPEEDR
     * ===================================================== */

    GPIOx->OSPEEDR &= ~(0x3UL << Shift);
    GPIOx->OSPEEDR |= ((uint32_t)Config->Speed << Shift);


    /* =====================================================
     * Configure Pull-Up / Pull-Down - GPIOx_PUPDR
     * ===================================================== */

    GPIOx->PUPDR &= ~(0x3UL << Shift);
    GPIOx->PUPDR |= ((uint32_t)Config->Pull << Shift);


    /* =====================================================
     * Configure Alternate Function if AF mode is selected
     * ===================================================== */

    if (Config->Mode == GPIO_MODE_AF)
    {
        if (Pin < GPIO_PIN_8)
        {
            Shift = ((uint32_t)Pin * GPIO_AF_BITS_WIDTH);

            GPIOx->AFRL &= ~(0xFUL << Shift);
            GPIOx->AFRL |=
                ((uint32_t)Config->AlternateFunction << Shift);
        }
        else
        {
            Shift =
                (((uint32_t)Pin - GPIO_PIN_8) *
                 GPIO_AF_BITS_WIDTH);

            GPIOx->AFRH &= ~(0xFUL << Shift);
            GPIOx->AFRH |=
                ((uint32_t)Config->AlternateFunction << Shift);
        }
    }

    return GPIO_OK;
}


/**
 * @brief Write HIGH or LOW to one GPIO pin.
 *
 * BSRR is used for atomic pin access.
 */
GPIO_Status_t GPIO_WritePin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin,
    GPIO_State_t State
)
{
    GPIO_Registers_t *GPIOx;

    if ((GPIO_IsValidPort(Port) == 0U) ||
        (GPIO_IsValidPin(Pin) == 0U))
    {
        return GPIO_ERROR;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_ERROR;
    }

    if (State == GPIO_HIGH)
    {
        /* Set pin using BSRR bits [15:0] */
        GPIOx->BSRR = (1UL << Pin);
    }
    else
    {
        /* Reset pin using BSRR bits [31:16] */
        GPIOx->BSRR = (1UL << ((uint32_t)Pin + 16U));
    }

    return GPIO_OK;
}


/**
 * @brief Read the current state of one GPIO pin.
 */
GPIO_State_t GPIO_ReadPin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin
)
{
    GPIO_Registers_t *GPIOx;

    if ((GPIO_IsValidPort(Port) == 0U) ||
        (GPIO_IsValidPin(Pin) == 0U))
    {
        return GPIO_LOW;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_LOW;
    }

    if ((GPIOx->IDR & (1UL << Pin)) != 0U)
    {
        return GPIO_HIGH;
    }

    return GPIO_LOW;
}


/**
 * @brief Toggle one GPIO output pin.
 */
GPIO_Status_t GPIO_TogglePin(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin
)
{
    GPIO_Registers_t *GPIOx;

    if ((GPIO_IsValidPort(Port) == 0U) ||
        (GPIO_IsValidPin(Pin) == 0U))
    {
        return GPIO_ERROR;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_ERROR;
    }

    GPIOx->ODR ^= (1UL << Pin);

    return GPIO_OK;
}


/**
 * @brief Write all 16 GPIO output pins.
 */
GPIO_Status_t GPIO_WritePort(
    GPIO_Port_t Port,
    uint16_t Value
)
{
    GPIO_Registers_t *GPIOx;

    if (GPIO_IsValidPort(Port) == 0U)
    {
        return GPIO_ERROR;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_ERROR;
    }

    GPIOx->ODR = (uint32_t)Value;

    return GPIO_OK;
}


/**
 * @brief Read all 16 GPIO input pins.
 */
uint16_t GPIO_ReadPort(GPIO_Port_t Port)
{
    GPIO_Registers_t *GPIOx;

    if (GPIO_IsValidPort(Port) == 0U)
    {
        return 0U;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return 0U;
    }

    return (uint16_t)(GPIOx->IDR);
}


/**
 * @brief Select Alternate Function for a GPIO pin.
 */
GPIO_Status_t GPIO_SetAlternateFunction(
    GPIO_Port_t Port,
    GPIO_Pin_t Pin,
    GPIO_AlternateFunction_t AlternateFunction
)
{
    GPIO_Registers_t *GPIOx;
    uint32_t Shift;

    if ((GPIO_IsValidPort(Port) == 0U) ||
        (GPIO_IsValidPin(Pin) == 0U) ||
        ((uint32_t)AlternateFunction > GPIO_AF15))
    {
        return GPIO_ERROR;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_ERROR;
    }

    if (Pin < GPIO_PIN_8)
    {
        Shift = ((uint32_t)Pin * GPIO_AF_BITS_WIDTH);

        GPIOx->AFRL &= ~(0xFUL << Shift);
        GPIOx->AFRL |=
            ((uint32_t)AlternateFunction << Shift);
    }
    else
    {
        Shift =
            (((uint32_t)Pin - GPIO_PIN_8) *
             GPIO_AF_BITS_WIDTH);

        GPIOx->AFRH &= ~(0xFUL << Shift);
        GPIOx->AFRH |=
            ((uint32_t)AlternateFunction << Shift);
    }

    return GPIO_OK;
}


/**
 * @brief Lock configuration of selected GPIO pins.
 *
 * Once locked, the configuration cannot be changed until
 * MCU reset or peripheral reset.
 */
GPIO_Status_t GPIO_LockPins(
    GPIO_Port_t Port,
    uint16_t PinMask
)
{
    GPIO_Registers_t *GPIOx;
    uint32_t LockValue;

    if (GPIO_IsValidPort(Port) == 0U)
    {
        return GPIO_ERROR;
    }

    GPIOx = GPIO_GetPortAddress(Port);

    if (GPIOx == 0)
    {
        return GPIO_ERROR;
    }

    LockValue =
        ((uint32_t)PinMask | (1UL << GPIO_LCKK_BIT));


    /*
     * LOCK sequence according to RM0368:
     *
     * 1. Write 1 to LCKK + selected pins
     * 2. Write 0 to LCKK + same selected pins
     * 3. Write 1 to LCKK + same selected pins
     * 4. Read LCKR
     * 5. Read LCKK to confirm lock
     */

    GPIOx->LCKR = LockValue;

    GPIOx->LCKR = (uint32_t)PinMask;

    GPIOx->LCKR = LockValue;

    (void)GPIOx->LCKR;

    if ((GPIOx->LCKR & (1UL << GPIO_LCKK_BIT)) != 0U)
    {
        return GPIO_OK;
    }

    return GPIO_ERROR;
}
