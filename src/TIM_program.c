#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include <stddef.h>

#include "TIM_interface.h"
#include "TIM_private.h"
#include "TIM_config.h"


/*===========================================================
=                    Static Functions                       =
===========================================================*/

/*-----------------------------------------------------------
 * Get Timer Register Pointer
 *----------------------------------------------------------*/

static volatile TIM_General_Type *
TIM_pGetTimer(u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer = NULL;

    switch (Copy_u8TimerID)
    {
        case TIM_1:
            Local_pTimer =
                (volatile TIM_General_Type *)TIM1;
            break;

        case TIM_2:
            Local_pTimer = TIM2;
            break;

        case TIM_3:
            Local_pTimer = TIM3;
            break;

        case TIM_4:
            Local_pTimer = TIM4;
            break;

        case TIM_5:
            Local_pTimer = TIM5;
            break;

        case TIM_9:
            Local_pTimer = TIM9;
            break;

        case TIM_10:
            Local_pTimer = TIM10;
            break;

        case TIM_11:
            Local_pTimer = TIM11;
            break;

        default:
            Local_pTimer = NULL;
            break;
    }

    return Local_pTimer;
}


/*-----------------------------------------------------------
 * Check Timer ID
 *----------------------------------------------------------*/

static u8 TIM_u8IsValidTimer(u8 Copy_u8TimerID)
{
    u8 Local_u8Status = 1;

    if (Copy_u8TimerID >= TIM_MAX_TIMERS)
    {
        Local_u8Status = 0;
    }

    return Local_u8Status;
}


/*-----------------------------------------------------------
 * Check Channel Availability
 *----------------------------------------------------------*/

static u8 TIM_u8IsValidChannel(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel)
{
    u8 Local_u8Status = 1;

    if (Copy_u8Channel > TIM_CHANNEL_4)
    {
        Local_u8Status = 0;
    }

    /*
     * TIM9 has CH1 and CH2 only
     */
    if (Copy_u8TimerID == TIM_9)
    {
        if (Copy_u8Channel > TIM_CHANNEL_2)
        {
            Local_u8Status = 0;
        }
    }

    /*
     * TIM10 and TIM11 have CH1 only
     */
    else if ((Copy_u8TimerID == TIM_10) ||
             (Copy_u8TimerID == TIM_11))
    {
        if (Copy_u8Channel != TIM_CHANNEL_1)
        {
            Local_u8Status = 0;
        }
    }

    return Local_u8Status;
}


/*-----------------------------------------------------------
 * Get CCR Register Pointer
 *----------------------------------------------------------*/

static volatile u32 *
TIM_pGetCCRRegister(
        volatile TIM_General_Type *Copy_pTimer,
        u8 Copy_u8Channel)
{
    volatile u32 *Local_pCCR = NULL;

    switch (Copy_u8Channel)
    {
        case TIM_CHANNEL_1:

            Local_pCCR =
                &(Copy_pTimer->CCR1);

            break;


        case TIM_CHANNEL_2:

            Local_pCCR =
                &(Copy_pTimer->CCR2);

            break;


        case TIM_CHANNEL_3:

            Local_pCCR =
                &(Copy_pTimer->CCR3);

            break;


        case TIM_CHANNEL_4:

            Local_pCCR =
                &(Copy_pTimer->CCR4);

            break;


        default:

            Local_pCCR = NULL;

            break;
    }

    return Local_pCCR;
}


/*===========================================================
=                    Callback Arrays                        =
===========================================================*/

static TIM_Callback_t
TIM_Callbacks[TIM_MAX_TIMERS]
             [TIM_MAX_INTERRUPT_SOURCES]
             =
{
    {NULL}
};


/*===========================================================
=                    Timer Initialization                   =
===========================================================*/

void TIM_voidInit(
        u8 Copy_u8TimerID,
        TIM_Config_t *Copy_pstrConfig)
{
    volatile TIM_General_Type *Local_pTimer;

    u32 Local_u32CR1;

    if (TIM_PARAMETER_CHECK == TIM_PARAMETER_CHECK_ENABLE)
    {
        if (Copy_pstrConfig == NULL)
        {
            return;
        }

        if (TIM_u8IsValidTimer(Copy_u8TimerID) == 0)
        {
            return;
        }
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * Stop Timer before configuration
     */

    Local_pTimer->CR1 &=
        ~(1UL << TIM_CR1_CEN);


    /*
     * Configure Prescaler
     */

    Local_pTimer->PSC =
        Copy_pstrConfig->Prescaler;


    /*
     * Configure Auto Reload Register
     */

    Local_pTimer->ARR =
        Copy_pstrConfig->AutoReloadValue;


    /*
     * Configure CR1
     */

    Local_u32CR1 =
        Local_pTimer->CR1;


    /*
     * Counter Direction
     */

    Local_u32CR1 &=
        ~(1UL << TIM_CR1_DIR);


    if (Copy_pstrConfig->CounterDirection
        == TIM_DOWN_COUNTER)
    {
        Local_u32CR1 |=
            (1UL << TIM_CR1_DIR);
    }


    /*
     * Center / Edge Alignment
     */

    Local_u32CR1 &=
        ~((1UL << TIM_CR1_CMS0) |
          (1UL << TIM_CR1_CMS1));


    Local_u32CR1 |=
        ((u32)Copy_pstrConfig->CounterAlignment
         << TIM_CR1_CMS0);


    /*
     * Clock Division
     */

    Local_u32CR1 &=
        ~((1UL << TIM_CR1_CKD0) |
          (1UL << TIM_CR1_CKD1));


    Local_u32CR1 |=
        ((u32)Copy_pstrConfig->ClockDivision
         << TIM_CR1_CKD0);


    /*
     * Auto Reload Preload
     */

    Local_u32CR1 &=
        ~(1UL << TIM_CR1_ARPE);


    if (Copy_pstrConfig->AutoReloadPreload
        == TIM_ARR_PRELOAD_ENABLE)
    {
        Local_u32CR1 |=
            (1UL << TIM_CR1_ARPE);
    }


    Local_pTimer->CR1 =
        Local_u32CR1;


    /*
     * Reset Counter
     */

    Local_pTimer->CNT = 0;


    /*
     * Generate Update Event
     *
     * Loads Prescaler immediately.
     */

    Local_pTimer->EGR =
        (1UL << TIM_EGR_UG);


    /*
     * Clear Update Flag
     */

    Local_pTimer->SR &=
        ~(1UL << TIM_SR_UIF);
}


/*===========================================================
=                    Start Timer                            =
===========================================================*/

void TIM_voidStart(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    if (TIM_u8IsValidTimer(Copy_u8TimerID) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->CR1 |=
        (1UL << TIM_CR1_CEN);
}


/*===========================================================
=                    Stop Timer                             =
===========================================================*/

void TIM_voidStop(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    if (TIM_u8IsValidTimer(Copy_u8TimerID) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->CR1 &=
        ~(1UL << TIM_CR1_CEN);
}


/*===========================================================
=                    Reset Counter                          =
===========================================================*/

void TIM_voidResetCounter(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->CNT = 0;
}


/*===========================================================
=                    Set Counter Value                      =
===========================================================*/

void TIM_voidSetCounterValue(
        u8 Copy_u8TimerID,
        u32 Copy_u32Value)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->CNT =
        Copy_u32Value;
}


/*===========================================================
=                    Get Counter Value                      =
===========================================================*/

u32 TIM_u32GetCounterValue(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return 0;
    }


    return Local_pTimer->CNT;
}


/*===========================================================
=                    Set ARR                                =
===========================================================*/

void TIM_voidSetARR(
        u8 Copy_u8TimerID,
        u32 Copy_u32ARRValue)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->ARR =
        Copy_u32ARRValue;
}


/*===========================================================
=                    Get ARR                                =
===========================================================*/

u32 TIM_u32GetARR(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return 0;
    }


    return Local_pTimer->ARR;
}


/*===========================================================
=                    Set Prescaler                          =
===========================================================*/

void TIM_voidSetPrescaler(
        u8 Copy_u8TimerID,
        u16 Copy_u16Prescaler)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->PSC =
        Copy_u16Prescaler;


    /*
     * Force Prescaler Reload
     */

    Local_pTimer->EGR =
        (1UL << TIM_EGR_UG);
}


/*===========================================================
=                 Generate Update Event                     =
===========================================================*/

void TIM_voidGenerateUpdateEvent(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->EGR =
        (1UL << TIM_EGR_UG);
}


/*===========================================================
=                    Enable Interrupt                       =
===========================================================*/

void TIM_voidEnableInterrupt(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    switch (Copy_u8InterruptSource)
    {
        case TIM_UPDATE_INTERRUPT:

            Local_pTimer->DIER |=
                (1UL << TIM_DIER_UIE);

            break;


        case TIM_CC1_INTERRUPT:

            Local_pTimer->DIER |=
                (1UL << TIM_DIER_CC1IE);

            break;


        case TIM_CC2_INTERRUPT:

            Local_pTimer->DIER |=
                (1UL << TIM_DIER_CC2IE);

            break;


        case TIM_CC3_INTERRUPT:

            Local_pTimer->DIER |=
                (1UL << TIM_DIER_CC3IE);

            break;


        case TIM_CC4_INTERRUPT:

            Local_pTimer->DIER |=
                (1UL << TIM_DIER_CC4IE);

            break;


        case TIM_TRIGGER_INTERRUPT:

            Local_pTimer->DIER |=
                (1UL << TIM_DIER_TIE);

            break;


        default:

            break;
    }
}


/*===========================================================
=                    Disable Interrupt                      =
===========================================================*/

void TIM_voidDisableInterrupt(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    switch (Copy_u8InterruptSource)
    {
        case TIM_UPDATE_INTERRUPT:

            Local_pTimer->DIER &=
                ~(1UL << TIM_DIER_UIE);

            break;


        case TIM_CC1_INTERRUPT:

            Local_pTimer->DIER &=
                ~(1UL << TIM_DIER_CC1IE);

            break;


        case TIM_CC2_INTERRUPT:

            Local_pTimer->DIER &=
                ~(1UL << TIM_DIER_CC2IE);

            break;


        case TIM_CC3_INTERRUPT:

            Local_pTimer->DIER &=
                ~(1UL << TIM_DIER_CC3IE);

            break;


        case TIM_CC4_INTERRUPT:

            Local_pTimer->DIER &=
                ~(1UL << TIM_DIER_CC4IE);

            break;


        case TIM_TRIGGER_INTERRUPT:

            Local_pTimer->DIER &=
                ~(1UL << TIM_DIER_TIE);

            break;


        default:

            break;
    }
}


/*===========================================================
=                    Set Callback                           =
===========================================================*/

void TIM_voidSetCallback(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource,
        TIM_Callback_t Copy_pfunCallback)
{
    if (Copy_u8TimerID >= TIM_MAX_TIMERS)
    {
        return;
    }


    if (Copy_u8InterruptSource
        >= TIM_MAX_INTERRUPT_SOURCES)
    {
        return;
    }


    TIM_Callbacks
    [Copy_u8TimerID]
    [Copy_u8InterruptSource]
        =
        Copy_pfunCallback;
}


/*===========================================================
=                    Clear Flag                             =
===========================================================*/

void TIM_voidClearFlag(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    switch (Copy_u8InterruptSource)
    {
        case TIM_UPDATE_INTERRUPT:

            Local_pTimer->SR &=
                ~(1UL << TIM_SR_UIF);

            break;


        case TIM_CC1_INTERRUPT:

            Local_pTimer->SR &=
                ~(1UL << TIM_SR_CC1IF);

            break;


        case TIM_CC2_INTERRUPT:

            Local_pTimer->SR &=
                ~(1UL << TIM_SR_CC2IF);

            break;


        case TIM_CC3_INTERRUPT:

            Local_pTimer->SR &=
                ~(1UL << TIM_SR_CC3IF);

            break;


        case TIM_CC4_INTERRUPT:

            Local_pTimer->SR &=
                ~(1UL << TIM_SR_CC4IF);

            break;


        case TIM_TRIGGER_INTERRUPT:

            Local_pTimer->SR &=
                ~(1UL << TIM_SR_TIF);

            break;


        default:

            break;
    }
}


/*===========================================================
=                    PWM Initialization                     =
===========================================================*/

void TIM_voidPWMInit(
        u8 Copy_u8TimerID,
        TIM_PWM_Config_t *Copy_pstrPWMConfig)
{
    volatile TIM_General_Type *Local_pTimer;

    volatile u32 *Local_pCCR;

    u8 Local_u8Shift;

    u32 Local_u32CCMR;


    if (Copy_pstrPWMConfig == NULL)
    {
        return;
    }


    if (TIM_u8IsValidTimer(Copy_u8TimerID) == 0)
    {
        return;
    }


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_pstrPWMConfig->Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * PWM Channel 1 or 2
     */

    if (Copy_pstrPWMConfig->Channel
        <= TIM_CHANNEL_2)
    {
        if (Copy_pstrPWMConfig->Channel
            == TIM_CHANNEL_1)
        {
            Local_u8Shift = 0;
        }
        else
        {
            Local_u8Shift = 8;
        }


        Local_u32CCMR =
            Local_pTimer->CCMR1;


        /*
         * Clear CCxS
         */

        Local_u32CCMR &=
            ~(0x3UL << Local_u8Shift);


        /*
         * Clear OCxM
         */

        Local_u32CCMR &=
            ~(0x7UL
            << (Local_u8Shift + 4));


        /*
         * PWM Mode
         */

        if (Copy_pstrPWMConfig->PWMMode
            == TIM_PWM_MODE_1)
        {
            Local_u32CCMR |=
                ((u32)TIM_OC_MODE_PWM1
                << (Local_u8Shift + 4));
        }
        else
        {
            Local_u32CCMR |=
                ((u32)TIM_OC_MODE_PWM2
                << (Local_u8Shift + 4));
        }


        /*
         * Enable Preload
         */

        Local_u32CCMR |=
            (1UL
            << (Local_u8Shift + 3));


        Local_pTimer->CCMR1 =
            Local_u32CCMR;
    }


    /*
     * PWM Channel 3 or 4
     */

    else
    {
        if (Copy_pstrPWMConfig->Channel
            == TIM_CHANNEL_3)
        {
            Local_u8Shift = 0;
        }
        else
        {
            Local_u8Shift = 8;
        }


        Local_u32CCMR =
            Local_pTimer->CCMR2;


        Local_u32CCMR &=
            ~(0x3UL << Local_u8Shift);


        Local_u32CCMR &=
            ~(0x7UL
            << (Local_u8Shift + 4));


        if (Copy_pstrPWMConfig->PWMMode
            == TIM_PWM_MODE_1)
        {
            Local_u32CCMR |=
                ((u32)TIM_OC_MODE_PWM1
                << (Local_u8Shift + 4));
        }
        else
        {
            Local_u32CCMR |=
                ((u32)TIM_OC_MODE_PWM2
                << (Local_u8Shift + 4));
        }


        Local_u32CCMR |=
            (1UL
            << (Local_u8Shift + 3));


        Local_pTimer->CCMR2 =
            Local_u32CCMR;
    }


    /*
     * Configure Polarity
     */

    if (Copy_pstrPWMConfig->Polarity
        == TIM_PWM_ACTIVE_HIGH)
    {
        Local_pTimer->CCER &=
            ~(1UL
            << ((Copy_pstrPWMConfig->Channel)
            * 4 + 1));
    }
    else
    {
        Local_pTimer->CCER |=
            (1UL
            << ((Copy_pstrPWMConfig->Channel)
            * 4 + 1));
    }


    /*
     * Set Compare Value
     */

    Local_pCCR =
        TIM_pGetCCRRegister(
            Local_pTimer,
            Copy_pstrPWMConfig->Channel);


    if (Local_pCCR != NULL)
    {
        *Local_pCCR =
            Copy_pstrPWMConfig->CompareValue;
    }


    /*
     * Enable ARR Preload
     */

    Local_pTimer->CR1 |=
        (1UL << TIM_CR1_ARPE);


    /*
     * Generate Update Event
     */

    Local_pTimer->EGR =
        (1UL << TIM_EGR_UG);
}


/*===========================================================
=                    Start PWM                              =
===========================================================*/

void TIM_voidPWMStart(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel)
{
    volatile TIM_General_Type *Local_pTimer;


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_u8Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * Enable Channel Output
     */

    Local_pTimer->CCER |=
        (1UL << (Copy_u8Channel * 4));


    /*
     * TIM1 requires Main Output Enable
     */

    if (Copy_u8TimerID == TIM_1)
    {
        TIM1->BDTR |=
            (1UL << TIM_BDTR_MOE);
    }


    /*
     * Start Counter
     */

    TIM_voidStart(Copy_u8TimerID);
}


/*===========================================================
=                    Stop PWM                               =
===========================================================*/

void TIM_voidPWMStop(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel)
{
    volatile TIM_General_Type *Local_pTimer;


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_u8Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->CCER &=
        ~(1UL << (Copy_u8Channel * 4));
}


/*===========================================================
=                    Set PWM Duty                           =
===========================================================*/

void TIM_voidSetPWMDuty(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel,
        u32 Copy_u32CompareValue)
{
    volatile TIM_General_Type *Local_pTimer;

    volatile u32 *Local_pCCR;


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_u8Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pCCR =
        TIM_pGetCCRRegister(
            Local_pTimer,
            Copy_u8Channel);


    if (Local_pCCR != NULL)
    {
        *Local_pCCR =
            Copy_u32CompareValue;
    }
}


/*===========================================================
=                  Input Capture Initialization             =
===========================================================*/

void TIM_voidInputCaptureInit(
        u8 Copy_u8TimerID,
        TIM_InputCapture_Config_t *Copy_pstrICConfig)
{
    volatile TIM_General_Type *Local_pTimer;

    u8 Local_u8Shift;

    u32 Local_u32CCMR;

    u32 Local_u32CCERShift;


    if (Copy_pstrICConfig == NULL)
    {
        return;
    }


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_pstrICConfig->Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * Disable Channel Before Configuration
     */

    Local_u32CCERShift =
        ((u32)Copy_pstrICConfig->Channel * 4);


    Local_pTimer->CCER &=
        ~(1UL << Local_u32CCERShift);


    /*
     * Channels 1 and 2
     */

    if (Copy_pstrICConfig->Channel
        <= TIM_CHANNEL_2)
    {
        if (Copy_pstrICConfig->Channel
            == TIM_CHANNEL_1)
        {
            Local_u8Shift = 0;
        }
        else
        {
            Local_u8Shift = 8;
        }


        Local_u32CCMR =
            Local_pTimer->CCMR1;


        /*
         * Clear CCxS
         */

        Local_u32CCMR &=
            ~(0x3UL << Local_u8Shift);


        /*
         * Direct Input
         */

        Local_u32CCMR |=
            (0x1UL << Local_u8Shift);


        /*
         * Input Capture Prescaler
         */

        Local_u32CCMR &=
            ~(0x3UL
            << (Local_u8Shift + 2));


        Local_u32CCMR |=
            ((u32)Copy_pstrICConfig->Prescaler
            << (Local_u8Shift + 2));


        /*
         * Input Filter
         */

        Local_u32CCMR &=
            ~(0xFUL
            << (Local_u8Shift + 4));


        Local_u32CCMR |=
            ((u32)Copy_pstrICConfig->Filter
            << (Local_u8Shift + 4));


        Local_pTimer->CCMR1 =
            Local_u32CCMR;
    }


    /*
     * Channels 3 and 4
     */

    else
    {
        if (Copy_pstrICConfig->Channel
            == TIM_CHANNEL_3)
        {
            Local_u8Shift = 0;
        }
        else
        {
            Local_u8Shift = 8;
        }


        Local_u32CCMR =
            Local_pTimer->CCMR2;


        Local_u32CCMR &=
            ~(0x3UL << Local_u8Shift);


        Local_u32CCMR |=
            (0x1UL << Local_u8Shift);


        Local_u32CCMR &=
            ~(0x3UL
            << (Local_u8Shift + 2));


        Local_u32CCMR |=
            ((u32)Copy_pstrICConfig->Prescaler
            << (Local_u8Shift + 2));


        Local_u32CCMR &=
            ~(0xFUL
            << (Local_u8Shift + 4));


        Local_u32CCMR |=
            ((u32)Copy_pstrICConfig->Filter
            << (Local_u8Shift + 4));


        Local_pTimer->CCMR2 =
            Local_u32CCMR;
    }


    /*
     * Configure Capture Edge
     */

    Local_pTimer->CCER &=
        ~((1UL << (Local_u32CCERShift + 1))
        | (1UL << (Local_u32CCERShift + 3)));


    if (Copy_pstrICConfig->CaptureEdge
        == TIM_IC_FALLING_EDGE)
    {
        Local_pTimer->CCER |=
            (1UL
            << (Local_u32CCERShift + 1));
    }


    else if (Copy_pstrICConfig->CaptureEdge
             == TIM_IC_BOTH_EDGES)
    {
        /*
         * Both edges are not supported
         * on Channel 4 in the same way.
         */

        if (Copy_pstrICConfig->Channel
            != TIM_CHANNEL_4)
        {
            Local_pTimer->CCER |=
                (1UL
                << (Local_u32CCERShift + 1));

            Local_pTimer->CCER |=
                (1UL
                << (Local_u32CCERShift + 3));
        }
    }
}


/*===========================================================
=                    Start Input Capture                    =
===========================================================*/

void TIM_voidInputCaptureStart(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel)
{
    volatile TIM_General_Type *Local_pTimer;


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_u8Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * Enable Capture Channel
     */

    Local_pTimer->CCER |=
        (1UL << (Copy_u8Channel * 4));


    /*
     * Start Timer
     */

    TIM_voidStart(Copy_u8TimerID);
}


/*===========================================================
=                    Stop Input Capture                     =
===========================================================*/

void TIM_voidInputCaptureStop(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel)
{
    volatile TIM_General_Type *Local_pTimer;


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    Local_pTimer->CCER &=
        ~(1UL << (Copy_u8Channel * 4));
}


/*===========================================================
=                    Get Capture Value                      =
===========================================================*/

u32 TIM_u32GetCaptureValue(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel)
{
    volatile TIM_General_Type *Local_pTimer;

    volatile u32 *Local_pCCR;


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return 0;
    }


    Local_pCCR =
        TIM_pGetCCRRegister(
            Local_pTimer,
            Copy_u8Channel);


    if (Local_pCCR == NULL)
    {
        return 0;
    }


    return *Local_pCCR;
}


/*===========================================================
=                    Generate One Pulse                     =
===========================================================*/

void TIM_voidGenerateOnePulse(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel,
        u32 Copy_u32PulseWidth)
{
    volatile TIM_General_Type *Local_pTimer;

    volatile u32 *Local_pCCR;


    if (TIM_u8IsValidChannel(
            Copy_u8TimerID,
            Copy_u8Channel) == 0)
    {
        return;
    }


    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * Enable One Pulse Mode
     */

    Local_pTimer->CR1 |=
        (1UL << TIM_CR1_OPM);


    /*
     * Reset Counter
     */

    Local_pTimer->CNT = 0;


    /*
     * Set Period
     */

    Local_pTimer->ARR =
        Copy_u32PulseWidth;


    /*
     * Set Compare Value
     */

    Local_pCCR =
        TIM_pGetCCRRegister(
            Local_pTimer,
            Copy_u8Channel);


    if (Local_pCCR != NULL)
    {
        *Local_pCCR =
            Copy_u32PulseWidth / 2;
    }


    /*
     * Update Registers
     */

    Local_pTimer->EGR =
        (1UL << TIM_EGR_UG);


    /*
     * Enable Channel
     */

    Local_pTimer->CCER |=
        (1UL << (Copy_u8Channel * 4));


    /*
     * Start Timer
     */

    TIM_voidStart(Copy_u8TimerID);
}


/*===========================================================
=                    Interrupt Dispatcher                   =
===========================================================*/

static void TIM_voidIRQHandler(
        u8 Copy_u8TimerID)
{
    volatile TIM_General_Type *Local_pTimer;

    Local_pTimer =
        TIM_pGetTimer(Copy_u8TimerID);


    if (Local_pTimer == NULL)
    {
        return;
    }


    /*
     * Update Interrupt
     */

    if (((Local_pTimer->SR
         & (1UL << TIM_SR_UIF)) != 0)
        &&
        ((Local_pTimer->DIER
         & (1UL << TIM_DIER_UIE)) != 0))
    {
        Local_pTimer->SR &=
            ~(1UL << TIM_SR_UIF);


        if (TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_UPDATE_INTERRUPT]
            != NULL)
        {
            TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_UPDATE_INTERRUPT]();
        }
    }


    /*
     * Capture Compare 1
     */

    if (((Local_pTimer->SR
         & (1UL << TIM_SR_CC1IF)) != 0)
        &&
        ((Local_pTimer->DIER
         & (1UL << TIM_DIER_CC1IE)) != 0))
    {
        Local_pTimer->SR &=
            ~(1UL << TIM_SR_CC1IF);


        if (TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC1_INTERRUPT]
            != NULL)
        {
            TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC1_INTERRUPT]();
        }
    }


    /*
     * Capture Compare 2
     */

    if (((Local_pTimer->SR
         & (1UL << TIM_SR_CC2IF)) != 0)
        &&
        ((Local_pTimer->DIER
         & (1UL << TIM_DIER_CC2IE)) != 0))
    {
        Local_pTimer->SR &=
            ~(1UL << TIM_SR_CC2IF);


        if (TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC2_INTERRUPT]
            != NULL)
        {
            TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC2_INTERRUPT]();
        }
    }


    /*
     * Capture Compare 3
     */

    if (((Local_pTimer->SR
         & (1UL << TIM_SR_CC3IF)) != 0)
        &&
        ((Local_pTimer->DIER
         & (1UL << TIM_DIER_CC3IE)) != 0))
    {
        Local_pTimer->SR &=
            ~(1UL << TIM_SR_CC3IF);


        if (TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC3_INTERRUPT]
            != NULL)
        {
            TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC3_INTERRUPT]();
        }
    }


    /*
     * Capture Compare 4
     */

    if (((Local_pTimer->SR
         & (1UL << TIM_SR_CC4IF)) != 0)
        &&
        ((Local_pTimer->DIER
         & (1UL << TIM_DIER_CC4IE)) != 0))
    {
        Local_pTimer->SR &=
            ~(1UL << TIM_SR_CC4IF);


        if (TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC4_INTERRUPT]
            != NULL)
        {
            TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_CC4_INTERRUPT]();
        }
    }


    /*
     * Trigger Interrupt
     */

    if (((Local_pTimer->SR
         & (1UL << TIM_SR_TIF)) != 0)
        &&
        ((Local_pTimer->DIER
         & (1UL << TIM_DIER_TIE)) != 0))
    {
        Local_pTimer->SR &=
            ~(1UL << TIM_SR_TIF);


        if (TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_TRIGGER_INTERRUPT]
            != NULL)
        {
            TIM_Callbacks
            [Copy_u8TimerID]
            [TIM_TRIGGER_INTERRUPT]();
        }
    }
}


/*===========================================================
=                    ISR Functions                          =
===========================================================*/


void TIM1_BRK_TIM9_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_1);
    TIM_voidIRQHandler(TIM_9);
}


void TIM1_UP_TIM10_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_1);
    TIM_voidIRQHandler(TIM_10);
}


void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_1);
    TIM_voidIRQHandler(TIM_11);
}


void TIM1_CC_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_1);
}


void TIM2_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_2);
}


void TIM3_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_3);
}


void TIM4_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_4);
}


void TIM5_IRQHandler(void)
{
    TIM_voidIRQHandler(TIM_5);
}
