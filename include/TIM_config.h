#ifndef TIM_CONFIG_H
#define TIM_CONFIG_H


/*===========================================================
=                    Timer Default Settings                 =
===========================================================*/

/*
 * Default Prescaler Value
 *
 * Timer Clock =
 * Peripheral Clock / (Prescaler + 1)
 */

#define TIM_DEFAULT_PRESCALER           0


/*===========================================================
=                  Default Auto Reload Value                =
===========================================================*/

#define TIM_DEFAULT_ARR                 0xFFFF


/*===========================================================
=                  Default Counter Direction                =
===========================================================*/

#define TIM_DEFAULT_DIRECTION           TIM_UP_COUNTER


/*===========================================================
=                   Default Alignment Mode                  =
===========================================================*/

#define TIM_DEFAULT_ALIGNMENT           TIM_EDGE_ALIGNED


/*===========================================================
=                    Default Clock Division                 =
===========================================================*/

#define TIM_DEFAULT_CLOCK_DIVISION      TIM_CLOCK_DIV_1


/*===========================================================
=                    Auto Reload Preload                    =
===========================================================*/

/*
 * TIM_ARR_PRELOAD_DISABLE:
 * ARR value is updated immediately.
 *
 * TIM_ARR_PRELOAD_ENABLE:
 * ARR value is buffered and updated on
 * the next update event.
 */

#define TIM_ARR_PRELOAD_DISABLE         0
#define TIM_ARR_PRELOAD_ENABLE          1


#define TIM_DEFAULT_ARR_PRELOAD         TIM_ARR_PRELOAD_DISABLE


/*===========================================================
=                    Default PWM Settings                   =
===========================================================*/

#define TIM_DEFAULT_PWM_MODE            TIM_PWM_MODE_1

#define TIM_DEFAULT_PWM_POLARITY        TIM_PWM_ACTIVE_HIGH

#define TIM_DEFAULT_PWM_COMPARE_VALUE   0


/*===========================================================
=                 Default Input Capture Settings            =
===========================================================*/

#define TIM_DEFAULT_IC_EDGE             TIM_IC_RISING_EDGE

#define TIM_DEFAULT_IC_PRESCALER        0

#define TIM_DEFAULT_IC_FILTER           0


/*===========================================================
=                    Validation Options                     =
===========================================================*/

/*
 * Enable or Disable Parameter Checking
 *
 * Options:
 * TIM_PARAMETER_CHECK_ENABLE
 * TIM_PARAMETER_CHECK_DISABLE
 */

#define TIM_PARAMETER_CHECK_ENABLE      1
#define TIM_PARAMETER_CHECK_DISABLE     0


#define TIM_PARAMETER_CHECK             TIM_PARAMETER_CHECK_ENABLE


#endif
