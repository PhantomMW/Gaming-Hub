#include "STD_Types.h"

#include "DAC_interface.h"
#include "TIM_interface.h"

#include "RCC_interface.h"
#include "NVIC_interface.h"

#include "AUDIO_interface.h"
#include "audio_data.h"


#define AUDIO_SAMPLE_RATE_HZ      8000UL


static u32 Audio_Phase = 0U;
static u32 Audio_PhaseStep = 140461537UL;

static u8 Audio_Volume = 0U;
static u8 Audio_TargetVolume = 255U;

static u8 Audio_IsPlaying = 0U;


/* =========================================
 * Apply Volume
 * ========================================= */

static u8 AUDIO_ApplyVolume(u8 Copy_u8Sample)
{
    u32 Local_u32CenteredSample;
    u32 Local_u32ScaledSample;


    Local_u32CenteredSample =
        (u32)Copy_u8Sample - 128U;


    Local_u32ScaledSample =
        (Local_u32CenteredSample *
         (u32)Audio_Volume) /
        255U;


    return
        (u8)(128U + Local_u32ScaledSample);
}


/* =========================================
 * Audio Timer Callback
 * ========================================= */

static void AUDIO_TimerCallback(void)
{
    u8 Local_u8Index;
    u8 Local_u8Sample;


    Local_u8Index =
        (u8)(Audio_Phase >> 27U);


    Local_u8Sample =
        Audio_Data[Local_u8Index];


    if
    (
        (Audio_IsPlaying == 1U) &&
        (Audio_Volume < Audio_TargetVolume)
    )
    {
        if (Audio_Volume <= 247U)
        {
            Audio_Volume += 8U;
        }
        else
        {
            Audio_Volume =
                Audio_TargetVolume;
        }
    }


    Local_u8Sample =
        AUDIO_ApplyVolume
        (
            Local_u8Sample
        );


    DAC_Write
    (
        Local_u8Sample
    );


    Audio_Phase +=
        Audio_PhaseStep;
}


/* =========================================
 * Audio Initialization
 * ========================================= */

void AUDIO_Init(void)
{
    TIM_Config_t Local_TimerConfig;


    Audio_Phase = 0U;
    Audio_Volume = 0U;
    Audio_IsPlaying = 0U;


    DAC_Init();


    RCC_EnablePeripheralClock
    (
        RCC_APB1,
        RCC_TIM2
    );


    NVIC_EnableIRQ(28U);


    Local_TimerConfig.Prescaler = 0U;

    Local_TimerConfig.AutoReloadValue =
        (16000000UL / AUDIO_SAMPLE_RATE_HZ) - 1U;

    Local_TimerConfig.CounterDirection =
        TIM_UP_COUNTER;

    Local_TimerConfig.CounterAlignment =
        TIM_EDGE_ALIGNED;

    Local_TimerConfig.ClockDivision =
        TIM_CLOCK_DIV_1;

    Local_TimerConfig.AutoReloadPreload =
        0U;


    TIM_voidInit
    (
        TIM_2,
        &Local_TimerConfig
    );


    TIM_voidSetCallback
    (
        TIM_2,
        TIM_UPDATE_INTERRUPT,
        AUDIO_TimerCallback
    );


    TIM_voidClearFlag
    (
        TIM_2,
        TIM_UPDATE_INTERRUPT
    );


    TIM_voidEnableInterrupt
    (
        TIM_2,
        TIM_UPDATE_INTERRUPT
    );
}


/* =========================================
 * Play Default Audio
 * ========================================= */

void AUDIO_Play(void)
{
    Audio_Phase = 0U;
    Audio_Volume = 0U;
    Audio_IsPlaying = 1U;


    TIM_voidStart
    (
        TIM_2
    );
}


/* =========================================
 * Stop Audio
 * ========================================= */

void AUDIO_Stop(void)
{
    Audio_IsPlaying = 0U;

    Audio_Volume = 0U;


    TIM_voidStop
    (
        TIM_2
    );


    Audio_Phase = 0U;


    DAC_Write(128U);
}


/* =========================================
 * Play Musical Note
 * ========================================= */

void AUDIO_PlayNote(u8 Copy_u8Note)
{
    Audio_Phase = 0U;
    Audio_Volume = 0U;
    Audio_IsPlaying = 1U;


    switch (Copy_u8Note)
    {
        /* Octave 4 */

        case NOTE_DO4:
            Audio_PhaseStep = 140461537UL;
            break;

        case NOTE_RE4:
            Audio_PhaseStep = 157657512UL;
            break;

        case NOTE_MI4:
            Audio_PhaseStep = 176968759UL;
            break;

        case NOTE_FA4:
            Audio_PhaseStep = 187491429UL;
            break;

        case NOTE_SOL4:
            Audio_PhaseStep = 210453398UL;
            break;

        case NOTE_LA4:
            Audio_PhaseStep = 236223201UL;
            break;

        case NOTE_SI4:
            Audio_PhaseStep = 265149806UL;
            break;


        /* Octave 5 */

        case NOTE_DO5:
            Audio_PhaseStep = 280923074UL;
            break;

        case NOTE_RE5:
            Audio_PhaseStep = 315315024UL;
            break;

        case NOTE_MI5:
            Audio_PhaseStep = 353937518UL;
            break;

        case NOTE_FA5:
            Audio_PhaseStep = 374982858UL;
            break;

        case NOTE_SOL5:
            Audio_PhaseStep = 420906796UL;
            break;

        case NOTE_LA5:
            Audio_PhaseStep = 472446402UL;
            break;

        case NOTE_SI5:
            Audio_PhaseStep = 530299612UL;
            break;


        default:
            Audio_PhaseStep = 140461537UL;
            break;
    }


    TIM_voidStart
    (
        TIM_2
    );
}
