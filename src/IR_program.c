#include "STD_Types.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "EXTI_interface.h"
#include "NVIC_interface.h"
#include "TIM_interface.h"
#include "IR_interface.h"

#define IR_PORT GPIO_PORT_B
#define IR_PIN GPIO_PIN_7
#define IR_EXTI_LINE EXTI_LINE_7
#define IR_EXTI_IRQ 23U
#define IR_TIMER TIM_3
#define IR_TIMER_PRESCALER 15U
#define IR_TIMER_ARR 65535U

#define IR_LEADER_LOW_MIN 8500U
#define IR_LEADER_LOW_MAX 9500U
#define IR_LEADER_HIGH_MIN 4000U
#define IR_LEADER_HIGH_MAX 5000U
#define IR_REPEAT_HIGH_MIN 1800U
#define IR_REPEAT_HIGH_MAX 2700U
#define IR_BIT_LOW_MIN 300U
#define IR_BIT_LOW_MAX 800U
#define IR_BIT0_HIGH_MIN 300U
#define IR_BIT0_HIGH_MAX 800U
#define IR_BIT1_HIGH_MIN 1300U
#define IR_BIT1_HIGH_MAX 1900U

typedef enum
{
    IR_STATE_IDLE = 0U,
    IR_STATE_LEADER_HIGH,
    IR_STATE_DATA_HIGH,
    IR_STATE_BIT_LOW,
    IR_STATE_BIT_HIGH
} IR_State_t;

static volatile IR_State_t IR_State = IR_STATE_IDLE;
static volatile u32 IR_LastEdgeTime = 0U;
static volatile u32 IR_Data = 0U;
static volatile u8 IR_BitCount = 0U;
static volatile u8 IR_FrameReady = 0U;
static volatile u32 IR_LastCode = 0U;
static volatile u8 IR_Ready = 0U;

static u8 IR_InRange(u32 Copy_u32Value, u32 Copy_u32Min, u32 Copy_u32Max)
{
    return ((Copy_u32Value >= Copy_u32Min) && (Copy_u32Value <= Copy_u32Max)) ? 1U : 0U;
}

static u32 IR_Elapsed(u32 Copy_u32Now, u32 Copy_u32Previous)
{
    if (Copy_u32Now >= Copy_u32Previous)
    {
        return Copy_u32Now - Copy_u32Previous;
    }
    return (IR_TIMER_ARR + 1U) - Copy_u32Previous + Copy_u32Now;
}

static void IR_Reset(void)
{
    IR_State = IR_STATE_IDLE;
    IR_BitCount = 0U;
    IR_Data = 0U;
}

static void IR_ProcessEdge(void)
{
    u32 Local_u32Now;
    u32 Local_u32Duration;
    u8 Local_u8Level;

    if (IR_Ready == 0U)
    {
        EXTI_ClearPending(IR_EXTI_LINE);
        return;
    }

    Local_u32Now = TIM_u32GetCounterValue(IR_TIMER);
    Local_u8Level = GPIO_ReadPin(IR_PORT, IR_PIN);

    if (IR_State == IR_STATE_IDLE)
    {
        if (Local_u8Level == GPIO_LOW)
        {
            IR_LastEdgeTime = Local_u32Now;
            IR_State = IR_STATE_LEADER_HIGH;
        }
        return;
    }

    Local_u32Duration = IR_Elapsed(Local_u32Now, IR_LastEdgeTime);
    IR_LastEdgeTime = Local_u32Now;

    if (IR_State == IR_STATE_LEADER_HIGH)
    {
        if ((Local_u8Level == GPIO_HIGH) &&
            IR_InRange(Local_u32Duration, IR_LEADER_LOW_MIN, IR_LEADER_LOW_MAX))
        {
            IR_State = IR_STATE_DATA_HIGH;
        }
        else
        {
            IR_Reset();
        }
        return;
    }

    if (IR_State == IR_STATE_DATA_HIGH)
    {
        if (Local_u8Level == GPIO_LOW)
        {
            if (IR_InRange(Local_u32Duration, IR_LEADER_HIGH_MIN, IR_LEADER_HIGH_MAX))
            {
                IR_BitCount = 0U;
                IR_Data = 0U;
                IR_State = IR_STATE_BIT_LOW;
            }
            else
            {
                IR_Reset();
            }
        }
        else
        {
            IR_Reset();
        }
        return;
    }

    if (IR_State == IR_STATE_BIT_LOW)
    {
        if (Local_u8Level == GPIO_HIGH)
        {
            if (IR_InRange(Local_u32Duration, IR_BIT_LOW_MIN, IR_BIT_LOW_MAX))
            {
                IR_State = IR_STATE_BIT_HIGH;
            }
            else
            {
                IR_Reset();
            }
        }
        else
        {
            IR_Reset();
        }
        return;
    }

    if (IR_State == IR_STATE_BIT_HIGH)
    {
        if (Local_u8Level == GPIO_LOW)
        {
            if (IR_InRange(Local_u32Duration, IR_BIT0_HIGH_MIN, IR_BIT0_HIGH_MAX))
            {
                IR_BitCount++;
            }
            else if (IR_InRange(Local_u32Duration, IR_BIT1_HIGH_MIN, IR_BIT1_HIGH_MAX))
            {
                IR_Data |= (1UL << IR_BitCount);
                IR_BitCount++;
            }
            else
            {
                IR_Reset();
                return;
            }

            if (IR_BitCount >= 32U)
            {
                u8 Local_u8Byte0;
                u8 Local_u8Byte1;
                u8 Local_u8Byte2;
                u8 Local_u8Byte3;

                Local_u8Byte0 = (u8)((IR_Data >> 0U) & 0xFFU);
                Local_u8Byte1 = (u8)((IR_Data >> 8U) & 0xFFU);
                Local_u8Byte2 = (u8)((IR_Data >> 16U) & 0xFFU);
                Local_u8Byte3 = (u8)((IR_Data >> 24U) & 0xFFU);

                if (((u8)(Local_u8Byte0 ^ Local_u8Byte1) == 0xFFU) &&
                    ((u8)(Local_u8Byte2 ^ Local_u8Byte3) == 0xFFU))
                {
                    IR_LastCode = ((u32)Local_u8Byte0 << 24U) |
                                  ((u32)Local_u8Byte1 << 16U) |
                                  ((u32)Local_u8Byte2 << 8U) |
                                  (u32)Local_u8Byte3;
                    IR_FrameReady = 1U;
                }

                IR_Reset();
            }
            else
            {
                IR_State = IR_STATE_BIT_LOW;
            }
            return;
        }

        IR_Reset();
    }
}

void EXTI9_5_IRQHandler(void)
{
    IR_ProcessEdge();
    EXTI_ClearPending(IR_EXTI_LINE);
}

void IR_Init(void)
{
    GPIO_PinConfig_t Local_IRConfig;
    EXTI_Config_t Local_EXTIConfig;
    TIM_Config_t Local_TimerConfig;

    IR_Ready = 0U;
    IR_FrameReady = 0U;
    IR_LastCode = 0U;
    IR_Reset();

    RCC_EnablePeripheralClock(RCC_AHB1, RCC_GPIOB);
    RCC_EnablePeripheralClock(RCC_APB2, RCC_SYSCFG);
    RCC_EnablePeripheralClock(RCC_APB1, RCC_TIM3);

    Local_IRConfig.Mode = GPIO_MODE_INPUT;
    Local_IRConfig.OutputType = GPIO_OUTPUT_PUSH_PULL;
    Local_IRConfig.Speed = GPIO_SPEED_LOW;
    Local_IRConfig.Pull = GPIO_PULL_UP;
    Local_IRConfig.AlternateFunction = GPIO_AF0;
    GPIO_InitPin(IR_PORT, IR_PIN, &Local_IRConfig);

    Local_TimerConfig.Prescaler = IR_TIMER_PRESCALER;
    Local_TimerConfig.AutoReloadValue = IR_TIMER_ARR;
    Local_TimerConfig.CounterDirection = TIM_UP_COUNTER;
    Local_TimerConfig.CounterAlignment = TIM_EDGE_ALIGNED;
    Local_TimerConfig.ClockDivision = TIM_CLOCK_DIV_1;
    Local_TimerConfig.AutoReloadPreload = 0U;

    TIM_voidInit(IR_TIMER, &Local_TimerConfig);
    TIM_voidResetCounter(IR_TIMER);
    TIM_voidStart(IR_TIMER);

    Local_EXTIConfig.Line = IR_EXTI_LINE;
    Local_EXTIConfig.Port = EXTI_PORT_B;
    Local_EXTIConfig.Trigger = EXTI_TRIGGER_BOTH;
    Local_EXTIConfig.Mode = EXTI_MODE_INTERRUPT;

    EXTI_Init(&Local_EXTIConfig);
    EXTI_ClearPending(IR_EXTI_LINE);
    NVIC_ClearPendingIRQ(IR_EXTI_IRQ);
    NVIC_EnableIRQ(IR_EXTI_IRQ);

    IR_LastEdgeTime = TIM_u32GetCounterValue(IR_TIMER);
    IR_Ready = 1U;
}

u8 IR_GetKey(void)
{
    u32 Local_u32Code;

    if (IR_FrameReady == 0U)
    {
        return IR_KEY_NONE;
    }

    __asm volatile ("cpsid i");
    Local_u32Code = IR_LastCode;
    IR_FrameReady = 0U;
    __asm volatile ("cpsie i");

    if (Local_u32Code == IR_CODE_PREV)
    {
        return IR_KEY_UP;
    }

    if (Local_u32Code == IR_CODE_NEXT)
    {
        return IR_KEY_DOWN;
    }

    if (Local_u32Code == IR_CODE_SELECT)
    {
        return IR_KEY_SELECT;
    }

    return IR_KEY_NONE;
}
