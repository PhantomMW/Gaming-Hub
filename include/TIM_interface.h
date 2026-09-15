#ifndef TIM_INTERFACE_H
#define TIM_INTERFACE_H


/* ================================================
 *                Timer IDs
 * ================================================ */

#define TIM_1       0
#define TIM_2       1
#define TIM_3       2
#define TIM_4       3
#define TIM_5       4
#define TIM_9       5
#define TIM_10      6
#define TIM_11      7


/* ================================================
 *              Timer Channels
 * ================================================ */

#define TIM_CHANNEL_1       0
#define TIM_CHANNEL_2       1
#define TIM_CHANNEL_3       2
#define TIM_CHANNEL_4       3


/* ================================================
 *              Counter Direction
 * ================================================ */

#define TIM_UP_COUNTER          0
#define TIM_DOWN_COUNTER        1


/* ================================================
 *              Counter Alignment
 * ================================================ */

#define TIM_EDGE_ALIGNED        0
#define TIM_CENTER_ALIGNED_1    1
#define TIM_CENTER_ALIGNED_2    2
#define TIM_CENTER_ALIGNED_3    3


/* ================================================
 *              Clock Division
 * ================================================ */

#define TIM_CLOCK_DIV_1         0
#define TIM_CLOCK_DIV_2         1
#define TIM_CLOCK_DIV_4         2


/* ================================================
 *              Timer Mode
 * ================================================ */

#define TIM_MODE_NORMAL             0
#define TIM_MODE_PWM                1
#define TIM_MODE_INPUT_CAPTURE      2
#define TIM_MODE_ONE_PULSE          3


/* ================================================
 *              PWM Modes
 * ================================================ */

#define TIM_PWM_MODE_1          0
#define TIM_PWM_MODE_2          1


/* ================================================
 *              PWM Polarity
 * ================================================ */

#define TIM_PWM_ACTIVE_HIGH     0
#define TIM_PWM_ACTIVE_LOW      1


/* ================================================
 *              Input Capture Edge
 * ================================================ */

#define TIM_IC_RISING_EDGE      0
#define TIM_IC_FALLING_EDGE     1
#define TIM_IC_BOTH_EDGES       2


/* ================================================
 *              Interrupt Sources
 * ================================================ */

#define TIM_UPDATE_INTERRUPT        0
#define TIM_CC1_INTERRUPT           1
#define TIM_CC2_INTERRUPT           2
#define TIM_CC3_INTERRUPT           3
#define TIM_CC4_INTERRUPT           4
#define TIM_TRIGGER_INTERRUPT       5


/* ================================================
 *              Timer Configuration
 * ================================================ */

typedef struct
{
    u16 Prescaler;

    u32 AutoReloadValue;

    u8 CounterDirection;

    u8 CounterAlignment;

    u8 ClockDivision;

    u8 AutoReloadPreload;

} TIM_Config_t;


/* ================================================
 *              PWM Configuration
 * ================================================ */

typedef struct
{
    u8 Channel;

    u8 PWMMode;

    u8 Polarity;

    u32 CompareValue;

} TIM_PWM_Config_t;


/* ================================================
 *          Input Capture Configuration
 * ================================================ */

typedef struct
{
    u8 Channel;

    u8 CaptureEdge;

    u8 Prescaler;

    u8 Filter;

} TIM_InputCapture_Config_t;


/* ================================================
 *              Callback Function
 * ================================================ */

typedef void (*TIM_Callback_t)(void);


/* ================================================
 *              Core Functions
 * ================================================ */

/*
 * Initialize selected timer
 */
void TIM_voidInit(
        u8 Copy_u8TimerID,
        TIM_Config_t *Copy_pstrConfig);


/*
 * Start timer counter
 */
void TIM_voidStart(
        u8 Copy_u8TimerID);


/*
 * Stop timer counter
 */
void TIM_voidStop(
        u8 Copy_u8TimerID);


/*
 * Reset timer counter
 */
void TIM_voidResetCounter(
        u8 Copy_u8TimerID);


/*
 * Set counter value
 */
void TIM_voidSetCounterValue(
        u8 Copy_u8TimerID,
        u32 Copy_u32Value);


/*
 * Get current counter value
 */
u32 TIM_u32GetCounterValue(
        u8 Copy_u8TimerID);


/*
 * Set Auto Reload Register value
 */
void TIM_voidSetARR(
        u8 Copy_u8TimerID,
        u32 Copy_u32ARRValue);


/*
 * Get Auto Reload Register value
 */
u32 TIM_u32GetARR(
        u8 Copy_u8TimerID);


/*
 * Set Prescaler
 */
void TIM_voidSetPrescaler(
        u8 Copy_u8TimerID,
        u16 Copy_u16Prescaler);


/*
 * Generate update event
 */
void TIM_voidGenerateUpdateEvent(
        u8 Copy_u8TimerID);


/* ================================================
 *             Interrupt Functions
 * ================================================ */

/*
 * Enable timer interrupt source
 */
void TIM_voidEnableInterrupt(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource);


/*
 * Disable timer interrupt source
 */
void TIM_voidDisableInterrupt(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource);


/*
 * Set callback function
 */
void TIM_voidSetCallback(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource,
        TIM_Callback_t Copy_pfunCallback);


/*
 * Clear interrupt flag
 */
void TIM_voidClearFlag(
        u8 Copy_u8TimerID,
        u8 Copy_u8InterruptSource);


/* ================================================
 *              PWM Functions
 * ================================================ */

/*
 * Configure PWM channel
 */
void TIM_voidPWMInit(
        u8 Copy_u8TimerID,
        TIM_PWM_Config_t *Copy_pstrPWMConfig);


/*
 * Start PWM
 */
void TIM_voidPWMStart(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel);


/*
 * Stop PWM
 */
void TIM_voidPWMStop(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel);


/*
 * Change PWM duty cycle
 */
void TIM_voidSetPWMDuty(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel,
        u32 Copy_u32CompareValue);


/* ================================================
 *           Input Capture Functions
 * ================================================ */

/*
 * Initialize Input Capture
 */
void TIM_voidInputCaptureInit(
        u8 Copy_u8TimerID,
        TIM_InputCapture_Config_t *Copy_pstrICConfig);


/*
 * Start Input Capture
 */
void TIM_voidInputCaptureStart(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel);


/*
 * Stop Input Capture
 */
void TIM_voidInputCaptureStop(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel);


/*
 * Read captured value
 */
u32 TIM_u32GetCaptureValue(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel);


/* ================================================
 *              One Pulse Functions
 * ================================================ */

/*
 * Generate one pulse
 */
void TIM_voidGenerateOnePulse(
        u8 Copy_u8TimerID,
        u8 Copy_u8Channel,
        u32 Copy_u32PulseWidth);


/* ================================================
 *                Delay Functions
 * ================================================ */

/*
 * Initialize timer for delay operations
 */
void TIM_voidDelayInit(
        u8 Copy_u8TimerID);


/*
 * Delay in microseconds
 */
void TIM_voidDelayUs(
        u8 Copy_u8TimerID,
        u32 Copy_u32DelayUs);


/*
 * Delay in milliseconds
 */
void TIM_voidDelayMs(
        u8 Copy_u8TimerID,
        u32 Copy_u32DelayMs);


#endif
