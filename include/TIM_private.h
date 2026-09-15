#ifndef TIM_PRIVATE_H
#define TIM_PRIVATE_H


/*===========================================================
=                       Base Addresses                      =
===========================================================*/

/* APB2 Timers */

#define TIM1_BASE_ADDRESS       0x40010000UL
#define TIM9_BASE_ADDRESS       0x40014000UL
#define TIM10_BASE_ADDRESS      0x40014400UL
#define TIM11_BASE_ADDRESS      0x40014800UL


/* APB1 Timers */

#define TIM2_BASE_ADDRESS       0x40000000UL
#define TIM3_BASE_ADDRESS       0x40000400UL
#define TIM4_BASE_ADDRESS       0x40000800UL
#define TIM5_BASE_ADDRESS       0x40000C00UL



/*===========================================================
=                   General Timer Registers                 =
===========================================================*/

typedef struct
{
    volatile u32 CR1;        /* 0x00 */
    volatile u32 CR2;        /* 0x04 */
    volatile u32 SMCR;       /* 0x08 */
    volatile u32 DIER;       /* 0x0C */
    volatile u32 SR;         /* 0x10 */
    volatile u32 EGR;        /* 0x14 */
    volatile u32 CCMR1;      /* 0x18 */
    volatile u32 CCMR2;      /* 0x1C */
    volatile u32 CCER;       /* 0x20 */
    volatile u32 CNT;        /* 0x24 */
    volatile u32 PSC;        /* 0x28 */
    volatile u32 ARR;        /* 0x2C */
    volatile u32 RESERVED;   /* 0x30 */
    volatile u32 CCR1;       /* 0x34 */
    volatile u32 CCR2;       /* 0x38 */
    volatile u32 CCR3;       /* 0x3C */
    volatile u32 CCR4;       /* 0x40 */
    volatile u32 RESERVED2;  /* 0x44 */
    volatile u32 DCR;        /* 0x48 */
    volatile u32 DMAR;       /* 0x4C */

} TIM_General_Type;



/*===========================================================
=                  Advanced Timer Registers                 =
===========================================================*/

typedef struct
{
    volatile u32 CR1;        /* 0x00 */
    volatile u32 CR2;        /* 0x04 */
    volatile u32 SMCR;       /* 0x08 */
    volatile u32 DIER;       /* 0x0C */
    volatile u32 SR;         /* 0x10 */
    volatile u32 EGR;        /* 0x14 */
    volatile u32 CCMR1;      /* 0x18 */
    volatile u32 CCMR2;      /* 0x1C */
    volatile u32 CCER;       /* 0x20 */
    volatile u32 CNT;        /* 0x24 */
    volatile u32 PSC;        /* 0x28 */
    volatile u32 ARR;        /* 0x2C */
    volatile u32 RCR;        /* 0x30 */
    volatile u32 CCR1;       /* 0x34 */
    volatile u32 CCR2;       /* 0x38 */
    volatile u32 CCR3;       /* 0x3C */
    volatile u32 CCR4;       /* 0x40 */
    volatile u32 BDTR;       /* 0x44 */
    volatile u32 DCR;        /* 0x48 */
    volatile u32 DMAR;       /* 0x4C */

} TIM_Advanced_Type;



/*===========================================================
=                    Timer Pointers                         =
===========================================================*/

#define TIM1    ((volatile TIM_Advanced_Type *)TIM1_BASE_ADDRESS)

#define TIM2    ((volatile TIM_General_Type  *)TIM2_BASE_ADDRESS)
#define TIM3    ((volatile TIM_General_Type  *)TIM3_BASE_ADDRESS)
#define TIM4    ((volatile TIM_General_Type  *)TIM4_BASE_ADDRESS)
#define TIM5    ((volatile TIM_General_Type  *)TIM5_BASE_ADDRESS)

#define TIM9    ((volatile TIM_General_Type  *)TIM9_BASE_ADDRESS)
#define TIM10   ((volatile TIM_General_Type  *)TIM10_BASE_ADDRESS)
#define TIM11   ((volatile TIM_General_Type  *)TIM11_BASE_ADDRESS)



/*===========================================================
=                     CR1 Register Bits                     =
===========================================================*/

#define TIM_CR1_CEN             0
#define TIM_CR1_UDIS            1
#define TIM_CR1_URS             2
#define TIM_CR1_OPM             3
#define TIM_CR1_DIR             4

#define TIM_CR1_CMS0            5
#define TIM_CR1_CMS1            6

#define TIM_CR1_ARPE            7

#define TIM_CR1_CKD0            8
#define TIM_CR1_CKD1            9



/*===========================================================
=                     DIER Register Bits                    =
===========================================================*/

#define TIM_DIER_UIE            0
#define TIM_DIER_CC1IE          1
#define TIM_DIER_CC2IE          2
#define TIM_DIER_CC3IE          3
#define TIM_DIER_CC4IE          4
#define TIM_DIER_TIE            6

#define TIM_DIER_UDE            8
#define TIM_DIER_CC1DE          9
#define TIM_DIER_CC2DE          10
#define TIM_DIER_CC3DE          11
#define TIM_DIER_CC4DE          12
#define TIM_DIER_TDE            14



/*===========================================================
=                      SR Register Bits                     =
===========================================================*/

#define TIM_SR_UIF              0
#define TIM_SR_CC1IF            1
#define TIM_SR_CC2IF            2
#define TIM_SR_CC3IF            3
#define TIM_SR_CC4IF            4
#define TIM_SR_TIF              6

#define TIM_SR_CC1OF            9
#define TIM_SR_CC2OF            10
#define TIM_SR_CC3OF            11
#define TIM_SR_CC4OF            12



/*===========================================================
=                     EGR Register Bits                     =
===========================================================*/

#define TIM_EGR_UG              0
#define TIM_EGR_CC1G            1
#define TIM_EGR_CC2G            2
#define TIM_EGR_CC3G            3
#define TIM_EGR_CC4G            4
#define TIM_EGR_TG              6



/*===========================================================
=                   CCER Register Bits                      =
===========================================================*/

/* Channel 1 */

#define TIM_CCER_CC1E           0
#define TIM_CCER_CC1P           1
#define TIM_CCER_CC1NE          2
#define TIM_CCER_CC1NP          3


/* Channel 2 */

#define TIM_CCER_CC2E           4
#define TIM_CCER_CC2P           5
#define TIM_CCER_CC2NE          6
#define TIM_CCER_CC2NP          7


/* Channel 3 */

#define TIM_CCER_CC3E           8
#define TIM_CCER_CC3P           9
#define TIM_CCER_CC3NE          10
#define TIM_CCER_CC3NP          11


/* Channel 4 */

#define TIM_CCER_CC4E           12
#define TIM_CCER_CC4P           13



/*===========================================================
=                     CCMR Masks                            =
===========================================================*/

/*
 * Each CCMR register contains configuration
 * for two channels.
 */


/* Capture / Compare Selection */

#define TIM_CCMR_CCxS_MASK          0x3


/* Output Compare Mode */

#define TIM_CCMR_OCxM_MASK          0x7


/* PWM Modes */

#define TIM_OC_MODE_FROZEN          0x0
#define TIM_OC_MODE_ACTIVE          0x1
#define TIM_OC_MODE_INACTIVE        0x2
#define TIM_OC_MODE_TOGGLE          0x3
#define TIM_OC_MODE_FORCE_INACTIVE  0x4
#define TIM_OC_MODE_FORCE_ACTIVE    0x5
#define TIM_OC_MODE_PWM1            0x6
#define TIM_OC_MODE_PWM2            0x7



/*===========================================================
=                  Input Capture Values                     =
===========================================================*/

#define TIM_CCxS_OUTPUT             0x0

#define TIM_CCxS_INPUT_TI1          0x1
#define TIM_CCxS_INPUT_TI2          0x2
#define TIM_CCxS_INPUT_TRC          0x3



/*===========================================================
=                    BDTR Register Bits                    =
===========================================================*/

#define TIM_BDTR_MOE                15



/*===========================================================
=                   Internal Timer Limits                   =
===========================================================*/

#define TIM_MAX_CHANNELS            4
#define TIM_MAX_TIMERS              8
#define TIM_MAX_INTERRUPT_SOURCES   6


#endif
