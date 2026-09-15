#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "ST7735S_interface.h"
#include "ST7735S_font.h"
#include "SysTick_interface.h"
#include "AUDIO_interface.h"
#include "LED_MATRIX_interface.h"
#include "IR_interface.h"


/* =========================================
 * Buttons
 * ========================================= */

#define BUTTON_UP              GPIO_PIN_0
#define BUTTON_DOWN            GPIO_PIN_1
#define BUTTON_SELECT          GPIO_PIN_3

#define BUZZER_PIN             GPIO_PIN_6


/* =========================================
 * Game Settings
 * ========================================= */

#define QUIZ_QUESTIONS_COUNT   8U
#define ANSWERS_COUNT          4U


/* =========================================
 * Colors
 * ========================================= */

#define COLOR_DARK_BLUE        0x0015U
#define COLOR_ORANGE           0xFD20U
#define COLOR_LIGHT_GRAY       0xC618U
#define COLOR_DARK_TEXT        0x000FU


/* =========================================
 * Global Variables
 * ========================================= */

static u8 SelectedItem = 0U;

static u8 SoundEnabled = 1U;

#define DIFFICULTY_EASY        0U
#define DIFFICULTY_MEDIUM      1U
#define DIFFICULTY_HARD        2U

#define DISPLAY_BOTH           0U
#define DISPLAY_TFT_ONLY       1U

static u8 DifficultyLevel = DIFFICULTY_MEDIUM;
static u8 DisplayMode = DISPLAY_BOTH;

static u8 HighScoreMath = 0U;
static u8 BestMovesMemory = 255U;
static u16 HighScoreSnake = 0U;

static u32 RandomState = 12345U;

/*
 * Guarantees that the 8 questions contain
 * all four operations twice.
 */
static u8 OperationIndex = 0U;


/* =========================================
 * Simple Delay
 * ========================================= */

static void Delay(volatile u32 Copy_u32Time)
{
    while (Copy_u32Time--)
    {
        __asm volatile ("nop");
    }
}


/* =========================================
 * Buzzer
 * ========================================= */

static void Buzzer_Beep
(
    u32 Copy_u32Duration
)
{
    if (SoundEnabled == 0U)
    {
        return;
    }

    GPIO_WritePin
    (
        GPIO_PORT_B,
        BUZZER_PIN,
        GPIO_HIGH
    );


    Delay
    (
        Copy_u32Duration
    );


    GPIO_WritePin
    (
        GPIO_PORT_B,
        BUZZER_PIN,
        GPIO_LOW
    );
}


static void Buzzer_Pause
(
    u32 Copy_u32Duration
)
{
    if (SoundEnabled == 0U)
    {
        return;
    }

    GPIO_WritePin
    (
        GPIO_PORT_B,
        BUZZER_PIN,
        GPIO_LOW
    );


    Delay
    (
        Copy_u32Duration
    );
}


static void Buzzer_Click(void)
{
    Buzzer_Beep(18000U);
}


static void Buzzer_Select(void)
{
    Buzzer_Beep(30000U);
    Buzzer_Pause(12000U);
    Buzzer_Beep(50000U);
}


static void Buzzer_Correct(void)
{
    Buzzer_Beep(28000U);
    Buzzer_Pause(10000U);
    Buzzer_Beep(55000U);
}


static void Buzzer_Wrong(void)
{
    Buzzer_Beep(70000U);
}


static void Buzzer_Match(void)
{
    Buzzer_Beep(22000U);
    Buzzer_Pause(9000U);
    Buzzer_Beep(22000U);
    Buzzer_Pause(9000U);
    Buzzer_Beep(50000U);
}


static void Buzzer_Mismatch(void)
{
    Buzzer_Beep(35000U);
    Buzzer_Pause(18000U);
    Buzzer_Beep(35000U);
}


static void Buzzer_Eat(void)
{
    Buzzer_Beep(25000U);
    Buzzer_Pause(8000U);
    Buzzer_Beep(25000U);
}


static void Buzzer_GameOver(void)
{
    Buzzer_Beep(80000U);
    Buzzer_Pause(25000U);
    Buzzer_Beep(80000U);
    Buzzer_Pause(25000U);
    Buzzer_Beep(120000U);
}


static void Buzzer_Win(void)
{
    Buzzer_Beep(25000U);
    Buzzer_Pause(8000U);
    Buzzer_Beep(25000U);
    Buzzer_Pause(8000U);
    Buzzer_Beep(25000U);
    Buzzer_Pause(8000U);
    Buzzer_Beep(80000U);
}


static void Buzzer_NewHighScore(void)
{
    Buzzer_Beep(18000U);
    Buzzer_Pause(7000U);
    Buzzer_Beep(18000U);
    Buzzer_Pause(7000U);
    Buzzer_Beep(45000U);
    Buzzer_Pause(7000U);
    Buzzer_Beep(80000U);
}


/* =========================================
 * Wait For Button Release
 * ========================================= */

static u8 IR_ReadyInMain = 0U;
static u8 PendingIRKey = IR_KEY_NONE;
static u8 LastInputWasIR = 0U;

static u8 Input_ReadPin(u8 Copy_u8Port, u16 Copy_u16Pin)
{
    u8 Local_u8Key;

    LastInputWasIR = 0U;

    if (GPIO_ReadPin(Copy_u8Port, Copy_u16Pin) == GPIO_LOW)
    {
        return GPIO_LOW;
    }

    if (IR_ReadyInMain == 0U)
    {
        return GPIO_HIGH;
    }

    if (PendingIRKey == IR_KEY_NONE)
    {
        Local_u8Key = IR_GetKey();
        if (Local_u8Key != IR_KEY_NONE)
        {
            PendingIRKey = Local_u8Key;
        }
    }

    if (((Copy_u16Pin == BUTTON_UP) && (PendingIRKey == IR_KEY_UP)) ||
        ((Copy_u16Pin == BUTTON_DOWN) && (PendingIRKey == IR_KEY_DOWN)) ||
        ((Copy_u16Pin == BUTTON_SELECT) && (PendingIRKey == IR_KEY_SELECT)))
    {
        PendingIRKey = IR_KEY_NONE;
        LastInputWasIR = 1U;
        return GPIO_LOW;
    }

    return GPIO_HIGH;
}

static void WaitForRelease(u16 Copy_u16Pin)
{
    while (GPIO_ReadPin(GPIO_PORT_B, Copy_u16Pin) == GPIO_LOW)
    {
    }
}


/* =========================================
 * Simple Random Generator
 * ========================================= */

static u32 GetRandom(void)
{
    RandomState =
        (RandomState * 1103515245U)
        +
        12345U;

    return RandomState;
}


/* =========================================
 * Display Preference / LED Feedback
 * ========================================= */
static void Display_Refresh(u8 Copy_u8Game)
{
    if (DisplayMode == DISPLAY_TFT_ONLY)
    {
        LED_MATRIX_Clear();
    }
    else
    {
        if (Copy_u8Game == 1U)
        {
            LED_MATRIX_ShowMemory();
        }
        else if (Copy_u8Game == 2U)
        {
            LED_MATRIX_ShowSnake();
        }
        else if (Copy_u8Game == 3U)
        {
            LED_MATRIX_ShowMusic(0U);
        }
        else
        {
            LED_MATRIX_ShowMenu(0U);
        }
    }

    LED_MATRIX_Refresh();
}


/* =========================================
 * Fill Rectangle
 * ========================================= */

static void FillRect
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    u16 Copy_u16Color
)
{
    u16 Local_u16X;
    u16 Local_u16Y;

    for
    (
        Local_u16Y = Copy_u16Y;
        Local_u16Y < (Copy_u16Y + Copy_u16Height);
        Local_u16Y++
    )
    {
        for
        (
            Local_u16X = Copy_u16X;
            Local_u16X < (Copy_u16X + Copy_u16Width);
            Local_u16X++
        )
        {
            ST7735S_DrawPixel
            (
                Local_u16X,
                Local_u16Y,
                Copy_u16Color
            );
        }
    }
}


/* =========================================
 * Draw Rectangle Border
 * ========================================= */

static void DrawRectBorder
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    u16 Copy_u16Color,
    u8 Copy_u8Thickness
)
{
    u8 Local_u8Layer;

    for
    (
        Local_u8Layer = 0U;
        Local_u8Layer < Copy_u8Thickness;
        Local_u8Layer++
    )
    {
        u16 Local_u16X;
        u16 Local_u16Y;

        for
        (
            Local_u16X = Copy_u16X + Local_u8Layer;

            Local_u16X <
            (
                Copy_u16X
                +
                Copy_u16Width
                -
                Local_u8Layer
            );

            Local_u16X++
        )
        {
            ST7735S_DrawPixel
            (
                Local_u16X,
                Copy_u16Y + Local_u8Layer,
                Copy_u16Color
            );

            ST7735S_DrawPixel
            (
                Local_u16X,

                Copy_u16Y
                +
                Copy_u16Height
                -
                1U
                -
                Local_u8Layer,

                Copy_u16Color
            );
        }


        for
        (
            Local_u16Y = Copy_u16Y + Local_u8Layer;

            Local_u16Y <
            (
                Copy_u16Y
                +
                Copy_u16Height
                -
                Local_u8Layer
            );

            Local_u16Y++
        )
        {
            ST7735S_DrawPixel
            (
                Copy_u16X + Local_u8Layer,
                Local_u16Y,
                Copy_u16Color
            );

            ST7735S_DrawPixel
            (
                Copy_u16X
                +
                Copy_u16Width
                -
                1U
                -
                Local_u8Layer,

                Local_u16Y,
                Copy_u16Color
            );
        }
    }
}


/* =========================================
 * Draw Large Character
 * ========================================= */

static void DrawLargeChar
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    char Copy_cCharacter,
    u16 Copy_u16Color,
    u8 Copy_u8Scale
)
{
    u8 Local_u8Column;
    u8 Local_u8Row;
    u8 Local_u8Data;
    u8 Local_u8Index;

    Local_u8Index =
        ST7735S_FontGetIndex
        (
            Copy_cCharacter
        );


    for
    (
        Local_u8Column = 0U;
        Local_u8Column < 5U;
        Local_u8Column++
    )
    {
        Local_u8Data =
            ST7735S_Font
            [
                Local_u8Index
            ]
            [
                Local_u8Column
            ];


        for
        (
            Local_u8Row = 0U;
            Local_u8Row < 7U;
            Local_u8Row++
        )
        {
            if
            (
                (
                    Local_u8Data
                    &
                    (1U << Local_u8Row)
                )
                !=
                0U
            )
            {
                FillRect
                (
                    Copy_u16X
                    +
                    (
                        Local_u8Column
                        *
                        Copy_u8Scale
                    ),

                    Copy_u16Y
                    +
                    (
                        Local_u8Row
                        *
                        Copy_u8Scale
                    ),

                    Copy_u8Scale,
                    Copy_u8Scale,

                    Copy_u16Color
                );
            }
        }
    }
}


/* =========================================
 * Draw Large String
 * ========================================= */

static void DrawLargeString
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    const char *Copy_pcString,
    u16 Copy_u16Color,
    u8 Copy_u8Scale
)
{
    u16 Local_u16XPosition;

    Local_u16XPosition =
        Copy_u16X;


    while
    (
        *Copy_pcString != '\0'
    )
    {
        DrawLargeChar
        (
            Local_u16XPosition,
            Copy_u16Y,
            *Copy_pcString,
            Copy_u16Color,
            Copy_u8Scale
        );


        Local_u16XPosition +=
            (
                6U
                *
                Copy_u8Scale
            );

        Copy_pcString++;
    }
}


/* =========================================
 * Draw Operation Symbol Manually
 *
 * This does NOT depend on the font.
 * ========================================= */

static void DrawOperation
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    char Copy_cOperation,
    u16 Copy_u16Color
)
{
    u16 Local_u16I;
    u16 Local_u16Thickness = 3U;


    /* =====================================
     * Addition +
     * ===================================== */

    if
    (
        Copy_cOperation == '+'
    )
    {
        FillRect
        (
            Copy_u16X + 5U,
            Copy_u16Y,
            Local_u16Thickness,
            16U,
            Copy_u16Color
        );

        FillRect
        (
            Copy_u16X,
            Copy_u16Y + 6U,
            13U,
            Local_u16Thickness,
            Copy_u16Color
        );
    }


    /* =====================================
     * Subtraction -
     * ===================================== */

    else if
    (
        Copy_cOperation == '-'
    )
    {
        FillRect
        (
            Copy_u16X,
            Copy_u16Y + 6U,
            13U,
            Local_u16Thickness,
            Copy_u16Color
        );
    }


    /* =====================================
     * Multiplication X
     * ===================================== */

    else if
    (
        Copy_cOperation == 'X'
    )
    {
        for
        (
            Local_u16I = 0U;
            Local_u16I < 13U;
            Local_u16I++
        )
        {
            FillRect
            (
                Copy_u16X + Local_u16I,
                Copy_u16Y + Local_u16I,
                3U,
                3U,
                Copy_u16Color
            );

            FillRect
            (
                Copy_u16X + (12U - Local_u16I),
                Copy_u16Y + Local_u16I,
                3U,
                3U,
                Copy_u16Color
            );
        }
    }


    /* =====================================
     * Division /
     * ===================================== */

    else if
    (
        Copy_cOperation == '/'
    )
    {
        for
        (
            Local_u16I = 0U;
            Local_u16I < 15U;
            Local_u16I++
        )
        {
            FillRect
            (
                Copy_u16X + (12U - Local_u16I),
                Copy_u16Y + Local_u16I,
                2U,
                2U,
                Copy_u16Color
            );
        }
    }
}


/* =========================================
 * Convert Number To String
 * ========================================= */

static void NumberToString
(
    u16 Copy_u16Number,
    char *Copy_pcString
)
{
    char Local_acTemp[6];

    u8 Local_u8Index = 0U;
    u8 Local_u8Length = 0U;


    if
    (
        Copy_u16Number == 0U
    )
    {
        Copy_pcString[0] = '0';

        Copy_pcString[1] = '\0';

        return;
    }


    while
    (
        Copy_u16Number > 0U
    )
    {
        Local_acTemp
        [
            Local_u8Length
        ]
        =
            (char)
            (
                '0'
                +
                (
                    Copy_u16Number
                    %
                    10U
                )
            );

        Local_u8Length++;

        Copy_u16Number /= 10U;
    }


    while
    (
        Local_u8Length > 0U
    )
    {
        Local_u8Length--;

        Copy_pcString
        [
            Local_u8Index
        ]
        =
            Local_acTemp
            [
                Local_u8Length
            ];

        Local_u8Index++;
    }


    Copy_pcString
    [
        Local_u8Index
    ]
    =
        '\0';
}


/* =========================================
 * Append String
 * ========================================= */

static u8 AppendString
(
    char *Copy_pcDestination,
    u8 Copy_u8Index,
    const char *Copy_pcSource
)
{
    while
    (
        *Copy_pcSource != '\0'
    )
    {
        Copy_pcDestination
        [
            Copy_u8Index
        ]
        =
            *Copy_pcSource;

        Copy_u8Index++;

        Copy_pcSource++;
    }

    return Copy_u8Index;
}


/* =========================================
 * Get String Length
 * ========================================= */

static u8 GetStringLength
(
    const char *Copy_pcString
)
{
    u8 Local_u8Length = 0U;

    while
    (
        *Copy_pcString != '\0'
    )
    {
        Local_u8Length++;

        Copy_pcString++;
    }

    return Local_u8Length;
}


/* =========================================
 * Draw Question
 *
 * Numbers and symbols are drawn separately.
 * ========================================= */

static void DrawQuestion
(
    u16 Copy_u16FirstNumber,
    u16 Copy_u16SecondNumber,
    char Copy_cOperation
)
{
    char Local_acFirstNumber[6];
    char Local_acSecondNumber[6];

    u16 Local_u16X;
    u16 Local_u16Width;

    u8 Local_u8FirstLength;
    u8 Local_u8SecondLength;


    NumberToString
    (
        Copy_u16FirstNumber,
        Local_acFirstNumber
    );


    NumberToString
    (
        Copy_u16SecondNumber,
        Local_acSecondNumber
    );


    Local_u8FirstLength =
        GetStringLength
        (
            Local_acFirstNumber
        );


    Local_u8SecondLength =
        GetStringLength
        (
            Local_acSecondNumber
        );


    /*
     * Scale = 2
     * Character width = 12 pixels
     */

    Local_u16Width =
        (
            Local_u8FirstLength
            *
            12U
        )
        +
        6U
        +
        15U
        +
        6U
        +
        (
            Local_u8SecondLength
            *
            12U
        )
        +
        6U
        +
        12U
        +
        5U
        +
        12U;


    if
    (
        Local_u16Width < 110U
    )
    {
        Local_u16X =
            (
                128U
                -
                Local_u16Width
            )
            /
            2U;
    }

    else
    {
        Local_u16X = 8U;
    }


    /* First Number */

    DrawLargeString
    (
        Local_u16X,
        52U,
        Local_acFirstNumber,
        ST7735S_WHITE,
        2U
    );


    Local_u16X +=
        (
            Local_u8FirstLength
            *
            12U
        )
        +
        6U;


    /* Operation */

    DrawOperation
    (
        Local_u16X,
        52U,
        Copy_cOperation,
        ST7735S_WHITE
    );


    Local_u16X += 19U;


    /* Second Number */

    DrawLargeString
    (
        Local_u16X,
        52U,
        Local_acSecondNumber,
        ST7735S_WHITE,
        2U
    );


    Local_u16X +=
        (
            Local_u8SecondLength
            *
            12U
        )
        +
        6U;


    /* Equal Sign */

    DrawLargeChar
    (
        Local_u16X,
        52U,
        '=',
        ST7735S_WHITE,
        2U
    );


    Local_u16X += 17U;


    /* Question Mark */

    DrawLargeChar
    (
        Local_u16X,
        52U,
        '?',
        ST7735S_WHITE,
        2U
    );
}


/* =========================================
 * Check Duplicate Answer
 * ========================================= */

static u8 IsDuplicateAnswer
(
    u16 Copy_u16Value,
    u16 *Copy_pu16Answers,
    u8 Copy_u8Count
)
{
    u8 Local_u8Index;

    for
    (
        Local_u8Index = 0U;
        Local_u8Index < Copy_u8Count;
        Local_u8Index++
    )
    {
        if
        (
            Copy_u16Value
            ==
            Copy_pu16Answers
            [
                Local_u8Index
            ]
        )
        {
            return 1U;
        }
    }

    return 0U;
}


/* =========================================
 * Generate New Question
 *
 * 0 = Addition
 * 1 = Subtraction
 * 2 = Multiplication
 * 3 = Division
 * ========================================= */

static void GenerateQuestion
(
    u16 *Copy_pu16FirstNumber,
    u16 *Copy_pu16SecondNumber,
    char *Copy_pcOperation,
    u16 *Copy_pu16CorrectAnswer,
    u16 *Copy_pu16Answers,
    u8 *Copy_pu8CorrectIndex
)
{
    u8 Local_u8Operation;
    u8 Local_u8Index;
    u16 Local_u16FirstMax;
    u16 Local_u16SecondMax;

    u16 Local_u16WrongAnswer;
    u16 Local_u16Offset;


    if (DifficultyLevel == DIFFICULTY_EASY)
    {
        Local_u16FirstMax = 20U;
        Local_u16SecondMax = 20U;
    }
    else if (DifficultyLevel == DIFFICULTY_HARD)
    {
        Local_u16FirstMax = 80U;
        Local_u16SecondMax = 60U;
    }
    else
    {
        Local_u16FirstMax = 40U;
        Local_u16SecondMax = 40U;
    }


    /*
     * Do not use GetRandom() % 4 here.
     * The low bits of this simple LCG can repeat
     * predictable patterns, which was causing only
     * some operations to appear.
     *
     * Instead, cycle through the four operations.
     * With 8 questions, each operation appears twice.
     */
    Local_u8Operation = OperationIndex;

    OperationIndex++;

    if
    (
        OperationIndex >= 4U
    )
    {
        OperationIndex = 0U;
    }


    /* Addition */

    if
    (
        Local_u8Operation == 0U
    )
    {
        *Copy_pu16FirstNumber =
            (u16)
            (
                (
                    GetRandom()
                    %
                    40U
                )
                +
                1U
            );

        *Copy_pu16SecondNumber =
            (u16)
            (
                (
                    GetRandom()
                    %
                    40U
                )
                +
                1U
            );

        *Copy_pcOperation = '+';

        *Copy_pu16CorrectAnswer =
            *Copy_pu16FirstNumber
            +
            *Copy_pu16SecondNumber;
    }


    /* Subtraction */

    else if
    (
        Local_u8Operation == 1U
    )
    {
        *Copy_pu16FirstNumber =
            (u16)
            (
                (
                    GetRandom()
                    %
                    50U
                )
                +
                10U
            );


        *Copy_pu16SecondNumber =
            (u16)
            (
                (
                    GetRandom()
                    %
                    *Copy_pu16FirstNumber
                )
                +
                1U
            );


        *Copy_pcOperation = '-';

        *Copy_pu16CorrectAnswer =
            *Copy_pu16FirstNumber
            -
            *Copy_pu16SecondNumber;
    }


    /* Multiplication */

    else if
    (
        Local_u8Operation == 2U
    )
    {
        *Copy_pu16FirstNumber =
            (u16)
            (
                (
                    GetRandom()
                    %
                    Local_u16FirstMax
                )
                +
                2U
            );


        *Copy_pu16SecondNumber =
            (u16)
            (
                (
                    GetRandom()
                    %
                    Local_u16SecondMax
                )
                +
                2U
            );


        *Copy_pcOperation = 'X';

        *Copy_pu16CorrectAnswer =
            *Copy_pu16FirstNumber
            *
            *Copy_pu16SecondNumber;
    }


    /* Division */

    else
    {
        u16 Local_u16Result;
        u16 Local_u16Divisor;


        Local_u16Result =
            (u16)
            (
                (
                    GetRandom()
                    %
                    Local_u16FirstMax
                )
                +
                2U
            );


        Local_u16Divisor =
            (u16)
            (
                (
                    GetRandom()
                    %
                    Local_u16SecondMax
                )
                +
                2U
            );


        *Copy_pu16FirstNumber =
            Local_u16Result
            *
            Local_u16Divisor;


        *Copy_pu16SecondNumber =
            Local_u16Divisor;


        *Copy_pcOperation = '/';


        *Copy_pu16CorrectAnswer =
            Local_u16Result;
    }


    /* Random Correct Answer Position */

    *Copy_pu8CorrectIndex =
        (u8)
        (
            GetRandom()
            %
            ANSWERS_COUNT
        );


    /* Generate Answers */

    for
    (
        Local_u8Index = 0U;
        Local_u8Index < ANSWERS_COUNT;
        Local_u8Index++
    )
    {
        if
        (
            Local_u8Index
            ==
            *Copy_pu8CorrectIndex
        )
        {
            Copy_pu16Answers
            [
                Local_u8Index
            ]
            =
                *Copy_pu16CorrectAnswer;
        }

        else
        {
            do
            {
                Local_u16Offset =
                    (u16)
                    (
                        (
                            GetRandom()
                            %
                            15U
                        )
                        +
                        1U
                    );


                if
                (
                    (
                        GetRandom()
                        %
                        2U
                    )
                    ==
                    0U
                )
                {
                    Local_u16WrongAnswer =
                        *Copy_pu16CorrectAnswer
                        +
                        Local_u16Offset;
                }

                else
                {
                    if
                    (
                        *Copy_pu16CorrectAnswer
                        >
                        Local_u16Offset
                    )
                    {
                        Local_u16WrongAnswer =
                            *Copy_pu16CorrectAnswer
                            -
                            Local_u16Offset;
                    }

                    else
                    {
                        Local_u16WrongAnswer =
                            *Copy_pu16CorrectAnswer
                            +
                            Local_u16Offset;
                    }
                }
            }
            while
            (
                IsDuplicateAnswer
                (
                    Local_u16WrongAnswer,
                    Copy_pu16Answers,
                    Local_u8Index
                )
                ==
                1U
            );


            Copy_pu16Answers
            [
                Local_u8Index
            ]
            =
                Local_u16WrongAnswer;
        }
    }
}


/* =========================================
 * Draw Answer Box
 * ========================================= */

static void DrawAnswerBox
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Answer,
    u8 Copy_u8Selected
)
{
    char Local_acAnswer[6];


    NumberToString
    (
        Copy_u16Answer,
        Local_acAnswer
    );


    if
    (
        Copy_u8Selected == 1U
    )
    {
        FillRect
        (
            Copy_u16X,
            Copy_u16Y,
            50U,
            25U,
            ST7735S_GREEN
        );


        DrawRectBorder
        (
            Copy_u16X,
            Copy_u16Y,
            50U,
            25U,
            ST7735S_WHITE,
            2U
        );


        DrawLargeString
        (
            Copy_u16X + 7U,
            Copy_u16Y + 4U,
            Local_acAnswer,
            ST7735S_BLACK,
            2U
        );
    }

    else
    {
        FillRect
        (
            Copy_u16X,
            Copy_u16Y,
            50U,
            25U,
            COLOR_LIGHT_GRAY
        );


        DrawRectBorder
        (
            Copy_u16X,
            Copy_u16Y,
            50U,
            25U,
            ST7735S_WHITE,
            1U
        );


        DrawLargeString
        (
            Copy_u16X + 7U,
            Copy_u16Y + 4U,
            Local_acAnswer,
            COLOR_DARK_TEXT,
            2U
        );
    }
}


/* =========================================
 * Draw Math Quiz Screen
 * ========================================= */

static void DrawMathQuiz
(
    u16 Copy_u16FirstNumber,
    u16 Copy_u16SecondNumber,
    char Copy_cOperation,
    u16 *Copy_pu16Answers,
    u8 Copy_u8SelectedAnswer,
    u8 Copy_u8QuestionNumber,
    u8 Copy_u8Score
)
{
    char Local_acQuestionInfo[10];
    char Local_acScore[10];
    char Local_acNumber[6];

    u8 Local_u8Index;


    ST7735S_FillScreen
    (
        COLOR_DARK_BLUE
    );


    /* Title */

    /* Centered title: 9 characters  6 pixels  scale 2 = 108 pixels */
    DrawLargeString
    (
        10U,
        5U,
        "MATH QUIZ",
        ST7735S_GREEN,
        2U
    );


    /* Question Number */

    Local_u8Index = 0U;

    Local_acQuestionInfo[Local_u8Index++] = 'Q';
    Local_acQuestionInfo[Local_u8Index++] = ' ';

    NumberToString
    (
        Copy_u8QuestionNumber,
        Local_acNumber
    );

    Local_u8Index =
        AppendString
        (
            Local_acQuestionInfo,
            Local_u8Index,
            Local_acNumber
        );

    Local_acQuestionInfo[Local_u8Index++] = '/';
    Local_acQuestionInfo[Local_u8Index++] = '8';
    Local_acQuestionInfo[Local_u8Index] = '\0';


    DrawLargeString
    (
        5U,
        28U,
        Local_acQuestionInfo,
        ST7735S_WHITE,
        1U
    );


    /* Score */

    Local_u8Index = 0U;

    Local_acScore[Local_u8Index++] = 'S';
    Local_acScore[Local_u8Index++] = 'C';
    Local_acScore[Local_u8Index++] = 'O';
    Local_acScore[Local_u8Index++] = 'R';
    Local_acScore[Local_u8Index++] = 'E';
    Local_acScore[Local_u8Index++] = ':';

    NumberToString
    (
        Copy_u8Score,
        Local_acNumber
    );

    Local_u8Index =
        AppendString
        (
            Local_acScore,
            Local_u8Index,
            Local_acNumber
        );

    Local_acScore[Local_u8Index] = '\0';


    DrawLargeString
    (
        75U,
        28U,
        Local_acScore,
        ST7735S_WHITE,
        1U
    );


    /* Question Box */

    FillRect
    (
        7U,
        40U,
        114U,
        38U,
        ST7735S_BLACK
    );


    DrawRectBorder
    (
        7U,
        40U,
        114U,
        38U,
        COLOR_ORANGE,
        3U
    );


    /* Draw question separately */

    DrawQuestion
    (
        Copy_u16FirstNumber,
        Copy_u16SecondNumber,
        Copy_cOperation
    );


    /* Four Answers */

    DrawAnswerBox
    (
        12U,
        86U,
        Copy_pu16Answers[0],
        (Copy_u8SelectedAnswer == 0U)
    );


    DrawAnswerBox
    (
        66U,
        86U,
        Copy_pu16Answers[1],
        (Copy_u8SelectedAnswer == 1U)
    );


    DrawAnswerBox
    (
        12U,
        108U,
        Copy_pu16Answers[2],
        (Copy_u8SelectedAnswer == 2U)
    );


    DrawAnswerBox
    (
        66U,
        108U,
        Copy_pu16Answers[3],
        (Copy_u8SelectedAnswer == 3U)
    );


    /* =====================================
     * Control Guide
     *
     * Two centered lines are cleaner and easier
     * to read than placing all words on one line.
     * ===================================== */

    DrawLargeString
    (
        25U,
        138U,
        "UP/DOWN: MOVE",
        ST7735S_WHITE,
        1U
    );


    DrawLargeString
    (
        22U,
        149U,
        "SELECT: CHOOSE",
        ST7735S_WHITE,
        1U
    );
}


/* =========================================
 * Show Answer Result
 * ========================================= */

static void ShowAnswerResult
(
    u8 Copy_u8Correct
)
{
    ST7735S_FillScreen
    (
        COLOR_DARK_BLUE
    );


    if
    (
        Copy_u8Correct == 1U
    )
    {
        DrawLargeString
        (
            20U,
            60U,
            "CORRECT",
            ST7735S_GREEN,
            2U
        );
    }

    else
    {
        DrawLargeString
        (
            35U,
            60U,
            "WRONG",
            ST7735S_RED,
            2U
        );
    }


    Delay(1000000U);
}


/* =========================================
 * Show Final Score
 * ========================================= */

static void ShowFinalScore
(
    u8 Copy_u8Score
)
{
    char Local_acScore[6];


    ST7735S_FillScreen
    (
        COLOR_DARK_BLUE
    );


    DrawLargeString
    (
        15U,
        30U,
        "FINAL SCORE",
        ST7735S_GREEN,
        2U
    );


    NumberToString
    (
        Copy_u8Score,
        Local_acScore
    );


    DrawLargeString
    (
        55U,
        70U,
        Local_acScore,
        ST7735S_WHITE,
        5U
    );


    DrawLargeString
    (
        35U,
        125U,
        "OUT OF 8",
        ST7735S_WHITE,
        2U
    );


    DrawLargeString
    (
        25U,
        150U,
        "SELECT EXIT",
        ST7735S_WHITE,
        1U
    );


    while
    (
        Input_ReadPin
        (
            GPIO_PORT_B,
            BUTTON_SELECT
        )
        ==
        GPIO_HIGH
    )
    {
    }


    WaitForRelease
    (
        BUTTON_SELECT
    );
}


/* =========================================
 * Math Quiz Game
 * ========================================= */

static void MathQuiz_Game(void)
{
    u8 Local_u8QuestionNumber;
    u8 Local_u8SelectedAnswer;
    u8 Local_u8PreviousSelectedAnswer;
    u8 Local_u8CorrectIndex;
    u8 Local_u8Score = 0U;

    u16 Local_u16FirstNumber;
    u16 Local_u16SecondNumber;
    u16 Local_u16CorrectAnswer;

    u16 Local_au16Answers
    [
        ANSWERS_COUNT
    ];

    char Local_cOperation;


    /*
     * Start from a different operation each game,
     * then cycle through +, -, X and /.
     */
    OperationIndex =
        (u8)
        (
            (GetRandom() >> 16U)
            %
            4U
        );


    for
    (
        Local_u8QuestionNumber = 1U;

        Local_u8QuestionNumber
        <=
        QUIZ_QUESTIONS_COUNT;

        Local_u8QuestionNumber++
    )
    {
        GenerateQuestion
        (
            &Local_u16FirstNumber,
            &Local_u16SecondNumber,
            &Local_cOperation,
            &Local_u16CorrectAnswer,
            Local_au16Answers,
            &Local_u8CorrectIndex
        );


        Local_u8SelectedAnswer = 0U;


        DrawMathQuiz
        (
            Local_u16FirstNumber,
            Local_u16SecondNumber,
            Local_cOperation,
            Local_au16Answers,
            Local_u8SelectedAnswer,
            Local_u8QuestionNumber,
            Local_u8Score
        );


        while (1)
        {
        Display_Refresh(0U);
            /* UP */

            if
            (
                Input_ReadPin
                (
                    GPIO_PORT_B,
                    BUTTON_UP
                )
                ==
                GPIO_LOW
            )
            {
                Local_u8PreviousSelectedAnswer =
                    Local_u8SelectedAnswer;


                if
                (
                    Local_u8SelectedAnswer == 0U
                )
                {
                    Local_u8SelectedAnswer = 3U;
                }

                else
                {
                    Local_u8SelectedAnswer--;
                }


                if
                (
                    Local_u8PreviousSelectedAnswer == 0U
                )
                {
                    DrawAnswerBox
                    (
                        12U,
                        86U,
                        Local_au16Answers[0U],
                        0U
                    );
                }

                else if
                (
                    Local_u8PreviousSelectedAnswer == 1U
                )
                {
                    DrawAnswerBox
                    (
                        66U,
                        86U,
                        Local_au16Answers[1U],
                        0U
                    );
                }

                else if
                (
                    Local_u8PreviousSelectedAnswer == 2U
                )
                {
                    DrawAnswerBox
                    (
                        12U,
                        108U,
                        Local_au16Answers[2U],
                        0U
                    );
                }

                else
                {
                    DrawAnswerBox
                    (
                        66U,
                        108U,
                        Local_au16Answers[3U],
                        0U
                    );
                }


                if
                (
                    Local_u8SelectedAnswer == 0U
                )
                {
                    DrawAnswerBox
                    (
                        12U,
                        86U,
                        Local_au16Answers[0U],
                        1U
                    );
                }

                else if
                (
                    Local_u8SelectedAnswer == 1U
                )
                {
                    DrawAnswerBox
                    (
                        66U,
                        86U,
                        Local_au16Answers[1U],
                        1U
                    );
                }

                else if
                (
                    Local_u8SelectedAnswer == 2U
                )
                {
                    DrawAnswerBox
                    (
                        12U,
                        108U,
                        Local_au16Answers[2U],
                        1U
                    );
                }

                else
                {
                    DrawAnswerBox
                    (
                        66U,
                        108U,
                        Local_au16Answers[3U],
                        1U
                    );
                }


                WaitForRelease
                (
                    BUTTON_UP
                );

                Delay(30000U);
            }


            /* DOWN */

            if
            (
                Input_ReadPin
                (
                    GPIO_PORT_B,
                    BUTTON_DOWN
                )
                ==
                GPIO_LOW
            )
            {
                Local_u8PreviousSelectedAnswer =
                    Local_u8SelectedAnswer;


                Local_u8SelectedAnswer++;


                if
                (
                    Local_u8SelectedAnswer
                    >=
                    ANSWERS_COUNT
                )
                {
                    Local_u8SelectedAnswer = 0U;
                }


                if
                (
                    Local_u8PreviousSelectedAnswer == 0U
                )
                {
                    DrawAnswerBox(12U, 86U, Local_au16Answers[0U], 0U);
                }
                else if
                (
                    Local_u8PreviousSelectedAnswer == 1U
                )
                {
                    DrawAnswerBox(66U, 86U, Local_au16Answers[1U], 0U);
                }
                else if
                (
                    Local_u8PreviousSelectedAnswer == 2U
                )
                {
                    DrawAnswerBox(12U, 108U, Local_au16Answers[2U], 0U);
                }
                else
                {
                    DrawAnswerBox(66U, 108U, Local_au16Answers[3U], 0U);
                }


                if
                (
                    Local_u8SelectedAnswer == 0U
                )
                {
                    DrawAnswerBox(12U, 86U, Local_au16Answers[0U], 1U);
                }
                else if
                (
                    Local_u8SelectedAnswer == 1U
                )
                {
                    DrawAnswerBox(66U, 86U, Local_au16Answers[1U], 1U);
                }
                else if
                (
                    Local_u8SelectedAnswer == 2U
                )
                {
                    DrawAnswerBox(12U, 108U, Local_au16Answers[2U], 1U);
                }
                else
                {
                    DrawAnswerBox(66U, 108U, Local_au16Answers[3U], 1U);
                }


                WaitForRelease
                (
                    BUTTON_DOWN
                );

                Delay(30000U);
            }


            /* SELECT */

            if
            (
                Input_ReadPin
                (
                    GPIO_PORT_B,
                    BUTTON_SELECT
                )
                ==
                GPIO_LOW
            )
            {
                WaitForRelease
                (
                    BUTTON_SELECT
                );


                if
                (
                    Local_u8SelectedAnswer
                    ==
                    Local_u8CorrectIndex
                )
                {
                    Local_u8Score++;

                    Buzzer_Correct();

                    ShowAnswerResult
                    (
                        1U
                    );
                }

                else
                {
                    Buzzer_Wrong();

                    ShowAnswerResult
                    (
                        0U
                    );
                }


                break;
            }
        }
    }


    if
    (
        Local_u8Score > HighScoreMath
    )
    {
        HighScoreMath =
            Local_u8Score;

        Buzzer_NewHighScore();
    }


    ShowFinalScore
    (
        Local_u8Score
    );
}



/* =========================================
 * MEMORY Game
 * ========================================= */

#define MEMORY_CARDS_COUNT     8U


/*
 * Four matching pairs:
 *
 * Card:   0 1 2 3 4 5 6 7
 * Value:  1 3 2 4 3 1 4 2
 *
 * The cards start hidden. When the player
 * selects a card, its value remains visible.
 */
static const u8 MemoryCards[MEMORY_CARDS_COUNT] =
{
    1U,
    3U,
    2U,
    4U,
    3U,
    1U,
    4U,
    2U
};


static void DrawMemoryCard
(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u8 Copy_u8CardValue,
    u8 Copy_u8Selected,
    u8 Copy_u8Revealed
)
{
    char Local_acCardValue[4];


    /*
     * Revealed card
     */
    if (Copy_u8Revealed == 1U)
    {
        FillRect
        (
            Copy_u16X,
            Copy_u16Y,
            48U,
            22U,
            ST7735S_WHITE
        );


        DrawRectBorder
        (
            Copy_u16X,
            Copy_u16Y,
            48U,
            22U,
            ST7735S_GREEN,
            2U
        );


        NumberToString
        (
            Copy_u8CardValue,
            Local_acCardValue
        );


        DrawLargeString
        (
            Copy_u16X + 20U,
            Copy_u16Y + 5U,
            Local_acCardValue,
            COLOR_DARK_TEXT,
            2U
        );
    }


    /*
     * Hidden selected card
     */
    else if (Copy_u8Selected == 1U)
    {
        FillRect
        (
            Copy_u16X,
            Copy_u16Y,
            48U,
            22U,
            ST7735S_GREEN
        );


        DrawRectBorder
        (
            Copy_u16X,
            Copy_u16Y,
            48U,
            22U,
            ST7735S_WHITE,
            2U
        );


        DrawLargeString
        (
            Copy_u16X + 20U,
            Copy_u16Y + 5U,
            "?",
            ST7735S_BLACK,
            2U
        );
    }


    /*
     * Normal hidden card
     */
    else
    {
        FillRect
        (
            Copy_u16X,
            Copy_u16Y,
            48U,
            22U,
            COLOR_LIGHT_GRAY
        );


        DrawRectBorder
        (
            Copy_u16X,
            Copy_u16Y,
            48U,
            22U,
            ST7735S_WHITE,
            1U
        );


        DrawLargeString
        (
            Copy_u16X + 20U,
            Copy_u16Y + 5U,
            "?",
            COLOR_DARK_TEXT,
            2U
        );
    }
}


/* =========================================
 * Draw MEMORY Screen
 * ========================================= */

static void DrawMemoryGame
(
    u8 Copy_u8SelectedCard,
    const u8 *Copy_pu8RevealedCards,
    u8 Copy_u8Moves,
    u8 Copy_u8PairsFound
)
{
    u8 Local_u8CardIndex;
    u16 Local_u16X;
    u16 Local_u16Y;
    char Local_acText[4];


    ST7735S_FillScreen
    (
        COLOR_DARK_BLUE
    );


    /* Title */

    DrawLargeString
    (
        28U,
        5U,
        "MEMORY",
        ST7735S_GREEN,
        2U
    );


    /* Score */

    DrawLargeString
    (
        5U,
        29U,
        "MOVES:",
        ST7735S_WHITE,
        1U
    );


    NumberToString
    (
        Copy_u8Moves,
        Local_acText
    );


    DrawLargeString
    (
        47U,
        29U,
        Local_acText,
        ST7735S_WHITE,
        1U
    );


    DrawLargeString
    (
        76U,
        29U,
        "PAIRS:",
        ST7735S_WHITE,
        1U
    );


    NumberToString
    (
        Copy_u8PairsFound,
        Local_acText
    );


    DrawLargeString
    (
        118U,
        29U,
        Local_acText,
        ST7735S_WHITE,
        1U
    );


    /*
     * Eight cards arranged in two columns.
     */

    for
    (
        Local_u8CardIndex = 0U;
        Local_u8CardIndex < MEMORY_CARDS_COUNT;
        Local_u8CardIndex++
    )
    {
        if
        (
            (Local_u8CardIndex % 2U) == 0U
        )
        {
            Local_u16X = 14U;
        }

        else
        {
            Local_u16X = 66U;
        }


        Local_u16Y =
            47U
            +
            (
                (Local_u8CardIndex / 2U)
                *
                24U
            );


        DrawMemoryCard
        (
            Local_u16X,
            Local_u16Y,
            MemoryCards[Local_u8CardIndex],
            (Local_u8CardIndex == Copy_u8SelectedCard),
            Copy_pu8RevealedCards[Local_u8CardIndex]
        );
    }


    /* Controls */

    DrawLargeString
    (
        14U,
        145U,
        "UP/DOWN: MOVE",
        ST7735S_WHITE,
        1U
    );


    DrawLargeString
    (
        22U,
        155U,
        "SELECT: FLIP",
        ST7735S_WHITE,
        1U
    );
}


/* =========================================
 * MEMORY Game
 * ========================================= */

static void Memory_Game(void)
{
    u8 Local_au8RevealedCards[MEMORY_CARDS_COUNT] =
    {
        0U, 0U, 0U, 0U,
        0U, 0U, 0U, 0U
    };

    u8 Local_u8SelectedCard = 0U;
    u8 Local_u8PreviousSelectedCard;
    u8 Local_u8FirstCard = 0U;
    u8 Local_u8SecondCard = 0U;
    u8 Local_u8FirstCardSelected = 0U;
    u8 Local_u8Moves = 0U;
    u8 Local_u8PairsFound = 0U;

    char Local_acText[4];


    DrawMemoryGame
    (
        Local_u8SelectedCard,
        Local_au8RevealedCards,
        Local_u8Moves,
        Local_u8PairsFound
    );


    while
    (
        Local_u8PairsFound < 4U
    )
    {
        Display_Refresh(1U);
        if
        (
            Input_ReadPin(GPIO_PORT_B, BUTTON_UP)
            ==
            GPIO_LOW
        )
        {
            Local_u8PreviousSelectedCard =
                Local_u8SelectedCard;

            if
            (
                Local_u8SelectedCard == 0U
            )
            {
                Local_u8SelectedCard =
                    MEMORY_CARDS_COUNT - 1U;
            }
            else
            {
                Local_u8SelectedCard--;
            }


            DrawMemoryCard
            (
                (Local_u8PreviousSelectedCard % 2U) == 0U ? 14U : 66U,
                (u16)(47U + ((Local_u8PreviousSelectedCard / 2U) * 24U)),
                MemoryCards[Local_u8PreviousSelectedCard],
                0U,
                Local_au8RevealedCards[Local_u8PreviousSelectedCard]
            );

            DrawMemoryCard
            (
                (Local_u8SelectedCard % 2U) == 0U ? 14U : 66U,
                (u16)(47U + ((Local_u8SelectedCard / 2U) * 24U)),
                MemoryCards[Local_u8SelectedCard],
                1U,
                Local_au8RevealedCards[Local_u8SelectedCard]
            );

            WaitForRelease
            (
                BUTTON_UP
            );
        }


        else if
        (
            Input_ReadPin(GPIO_PORT_B, BUTTON_DOWN)
            ==
            GPIO_LOW
        )
        {
            Local_u8PreviousSelectedCard =
                Local_u8SelectedCard;

            Local_u8SelectedCard++;

            if
            (
                Local_u8SelectedCard >= MEMORY_CARDS_COUNT
            )
            {
                Local_u8SelectedCard = 0U;
            }


            DrawMemoryCard
            (
                (Local_u8PreviousSelectedCard % 2U) == 0U ? 14U : 66U,
                (u16)(47U + ((Local_u8PreviousSelectedCard / 2U) * 24U)),
                MemoryCards[Local_u8PreviousSelectedCard],
                0U,
                Local_au8RevealedCards[Local_u8PreviousSelectedCard]
            );

            DrawMemoryCard
            (
                (Local_u8SelectedCard % 2U) == 0U ? 14U : 66U,
                (u16)(47U + ((Local_u8SelectedCard / 2U) * 24U)),
                MemoryCards[Local_u8SelectedCard],
                1U,
                Local_au8RevealedCards[Local_u8SelectedCard]
            );

            WaitForRelease
            (
                BUTTON_DOWN
            );
        }


        else if
        (
            Input_ReadPin(GPIO_PORT_B, BUTTON_SELECT)
            ==
            GPIO_LOW
        )
        {
            WaitForRelease
            (
                BUTTON_SELECT
            );


            if
            (
                Local_au8RevealedCards[Local_u8SelectedCard]
                ==
                1U
            )
            {
                continue;
            }


            if
            (
                Local_u8FirstCardSelected == 0U
            )
            {
                Local_u8FirstCard =
                    Local_u8SelectedCard;

                Local_au8RevealedCards[Local_u8FirstCard] =
                    1U;

                Local_u8FirstCardSelected = 1U;

                Buzzer_Click();


                DrawMemoryCard
                (
                    (Local_u8FirstCard % 2U) == 0U ? 14U : 66U,
                    (u16)(47U + ((Local_u8FirstCard / 2U) * 24U)),
                    MemoryCards[Local_u8FirstCard],
                    1U,
                    1U
                );
            }


            else
            {
                Local_u8SecondCard =
                    Local_u8SelectedCard;

                Local_au8RevealedCards[Local_u8SecondCard] =
                    1U;

                Local_u8Moves++;


                FillRect
                (
                    47U,
                    29U,
                    20U,
                    12U,
                    COLOR_DARK_BLUE
                );

                NumberToString
                (
                    Local_u8Moves,
                    Local_acText
                );

                DrawLargeString
                (
                    47U,
                    29U,
                    Local_acText,
                    ST7735S_WHITE,
                    1U
                );


                DrawMemoryCard
                (
                    (Local_u8SecondCard % 2U) == 0U ? 14U : 66U,
                    (u16)(47U + ((Local_u8SecondCard / 2U) * 24U)),
                    MemoryCards[Local_u8SecondCard],
                    1U,
                    1U
                );


                if
                (
                    MemoryCards[Local_u8FirstCard]
                    ==
                    MemoryCards[Local_u8SecondCard]
                )
                {
                    Local_u8PairsFound++;

                    Buzzer_Match();

                    FillRect
                    (
                        118U,
                        29U,
                        10U,
                        12U,
                        COLOR_DARK_BLUE
                    );

                    NumberToString
                    (
                        Local_u8PairsFound,
                        Local_acText
                    );

                    DrawLargeString
                    (
                        118U,
                        29U,
                        Local_acText,
                        ST7735S_WHITE,
                        1U
                    );
                }


                else
                {
                    Buzzer_Mismatch();

                    SysTick_DelayMs
                    (
                        (DifficultyLevel == DIFFICULTY_EASY) ? 1200U : ((DifficultyLevel == DIFFICULTY_HARD) ? 500U : 800U)
                    );

                    Local_au8RevealedCards[Local_u8FirstCard] =
                        0U;

                    Local_au8RevealedCards[Local_u8SecondCard] =
                        0U;


                    DrawMemoryCard
                    (
                        (Local_u8FirstCard % 2U) == 0U ? 14U : 66U,
                        (u16)(47U + ((Local_u8FirstCard / 2U) * 24U)),
                        MemoryCards[Local_u8FirstCard],
                        0U,
                        0U
                    );

                    DrawMemoryCard
                    (
                        (Local_u8SecondCard % 2U) == 0U ? 14U : 66U,
                        (u16)(47U + ((Local_u8SecondCard / 2U) * 24U)),
                        MemoryCards[Local_u8SecondCard],
                        1U,
                        0U
                    );
                }


                Local_u8FirstCardSelected = 0U;
            }
        }
    }


    if
    (
        Local_u8Moves < BestMovesMemory
    )
    {
        BestMovesMemory =
            Local_u8Moves;

        Buzzer_NewHighScore();
    }


    Buzzer_Win();


    /* =====================================
     * WIN Screen
     * ===================================== */

    ST7735S_FillScreen
    (
        COLOR_DARK_BLUE
    );


    DrawLargeString
    (
        16U,
        45U,
        "YOU WIN!",
        ST7735S_GREEN,
        2U
    );


    DrawLargeString
    (
        20U,
        80U,
        "MOVES:",
        ST7735S_WHITE,
        1U
    );


    NumberToString
    (
        Local_u8Moves,
        Local_acText
    );


    DrawLargeString
    (
        72U,
        80U,
        Local_acText,
        ST7735S_WHITE,
        1U
    );


    while
    (
        Input_ReadPin
        (
            GPIO_PORT_B,
            BUTTON_SELECT
        )
        !=
        GPIO_LOW
    )
    {
    }


    WaitForRelease
    (
        BUTTON_SELECT
    );
}



/* =========================================
 * SNAKE Game - Step 2
 * ========================================= */

#define SNAKE_BLOCK_SIZE       6U
#define SNAKE_INITIAL_LENGTH   5U
#define SNAKE_MAX_LENGTH       30U

#define SNAKE_DIR_RIGHT        0U
#define SNAKE_DIR_DOWN         1U
#define SNAKE_DIR_LEFT         2U
#define SNAKE_DIR_UP           3U

#define SNAKE_PLAY_X           4U
#define SNAKE_PLAY_Y           44U
#define SNAKE_PLAY_WIDTH       120U
#define SNAKE_PLAY_HEIGHT      96U


static void Snake_Game(void)
{
    u16 Local_u16SnakeX[SNAKE_MAX_LENGTH];
    u16 Local_u16SnakeY[SNAKE_MAX_LENGTH];

    u16 Local_u16FoodX = 94U;
    u16 Local_u16FoodY = 68U;

    u16 Local_u16OldTailX;
    u16 Local_u16OldTailY;

    u16 Local_u16Index;
    u16 Local_u16SnakeLength = SNAKE_INITIAL_LENGTH;
    u16 Local_u16Score = 0U;

    u8 Local_u8Direction = SNAKE_DIR_RIGHT;
    u8 Local_u8Running = 1U;
    u8 Local_u8AteFood;
    u8 Local_u8GameOver = 0U;

    char Local_acScoreText[12];


    for
    (
        Local_u16Index = 0U;
        Local_u16Index < Local_u16SnakeLength;
        Local_u16Index++
    )
    {
        Local_u16SnakeX[Local_u16Index] =
            (u16)(58U - (Local_u16Index * SNAKE_BLOCK_SIZE));

        Local_u16SnakeY[Local_u16Index] = 86U;
    }


    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );


    DrawLargeString
    (
        34U,
        4U,
        "SNAKE",
        ST7735S_GREEN,
        2U
    );


    DrawLargeString
    (
        5U,
        28U,
        "SCORE:",
        ST7735S_WHITE,
        1U
    );


    NumberToString
    (
        Local_u16Score,
        Local_acScoreText
    );


    DrawLargeString
    (
        55U,
        28U,
        Local_acScoreText,
        ST7735S_WHITE,
        1U
    );


    DrawRectBorder
    (
        2U,
        42U,
        124U,
        102U,
        ST7735S_WHITE,
        1U
    );


    DrawLargeString
    (
        14U,
        150U,
        "UP RIGHT  DOWN LEFT",
        ST7735S_WHITE,
        1U
    );


    FillRect
    (
        Local_u16FoodX,
        Local_u16FoodY,
        SNAKE_BLOCK_SIZE,
        SNAKE_BLOCK_SIZE,
        COLOR_ORANGE
    );


    for
    (
        Local_u16Index = 0U;
        Local_u16Index < Local_u16SnakeLength;
        Local_u16Index++
    )
    {
        if
        (
            Local_u16Index == 0U
        )
        {
            FillRect
            (
                Local_u16SnakeX[Local_u16Index],
                Local_u16SnakeY[Local_u16Index],
                SNAKE_BLOCK_SIZE,
                SNAKE_BLOCK_SIZE,
                ST7735S_GREEN
            );
        }

        else
        {
            FillRect
            (
                Local_u16SnakeX[Local_u16Index],
                Local_u16SnakeY[Local_u16Index],
                SNAKE_BLOCK_SIZE,
                SNAKE_BLOCK_SIZE,
                COLOR_LIGHT_GRAY
            );
        }
    }


    while
    (
        Local_u8Running == 1U
    )
    {
        Display_Refresh(2U);
        if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_UP
            )
            ==
            GPIO_LOW
        )
        {
            if
            (
                Local_u8Direction == SNAKE_DIR_RIGHT
            )
            {
                Local_u8Direction = SNAKE_DIR_UP;
            }

            else if
            (
                Local_u8Direction == SNAKE_DIR_UP
            )
            {
                Local_u8Direction = SNAKE_DIR_LEFT;
            }

            else if
            (
                Local_u8Direction == SNAKE_DIR_LEFT
            )
            {
                Local_u8Direction = SNAKE_DIR_DOWN;
            }

            else
            {
                Local_u8Direction = SNAKE_DIR_RIGHT;
            }


            WaitForRelease
            (
                BUTTON_UP
            );
        }


        else if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_DOWN
            )
            ==
            GPIO_LOW
        )
        {
            if
            (
                Local_u8Direction == SNAKE_DIR_RIGHT
            )
            {
                Local_u8Direction = SNAKE_DIR_DOWN;
            }

            else if
            (
                Local_u8Direction == SNAKE_DIR_DOWN
            )
            {
                Local_u8Direction = SNAKE_DIR_LEFT;
            }

            else if
            (
                Local_u8Direction == SNAKE_DIR_LEFT
            )
            {
                Local_u8Direction = SNAKE_DIR_UP;
            }

            else
            {
                Local_u8Direction = SNAKE_DIR_RIGHT;
            }


            WaitForRelease
            (
                BUTTON_DOWN
            );
        }


        else if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_SELECT
            )
            ==
            GPIO_LOW
        )
        {
            Local_u8Running = 0U;


            WaitForRelease
            (
                BUTTON_SELECT
            );


            break;
        }


        Local_u16OldTailX =
            Local_u16SnakeX[Local_u16SnakeLength - 1U];

        Local_u16OldTailY =
            Local_u16SnakeY[Local_u16SnakeLength - 1U];


        for
        (
            Local_u16Index = Local_u16SnakeLength - 1U;
            Local_u16Index > 0U;
            Local_u16Index--
        )
        {
            Local_u16SnakeX[Local_u16Index] =
                Local_u16SnakeX[Local_u16Index - 1U];

            Local_u16SnakeY[Local_u16Index] =
                Local_u16SnakeY[Local_u16Index - 1U];
        }


        if
        (
            Local_u8Direction == SNAKE_DIR_RIGHT
        )
        {
            Local_u16SnakeX[0U] +=
                SNAKE_BLOCK_SIZE;


            if
            (
                Local_u16SnakeX[0U]
                >=
                (SNAKE_PLAY_X + SNAKE_PLAY_WIDTH)
            )
            {
                Local_u16SnakeX[0U] =
                    SNAKE_PLAY_X;
            }
        }


        else if
        (
            Local_u8Direction == SNAKE_DIR_LEFT
        )
        {
            if
            (
                Local_u16SnakeX[0U]
                <=
                SNAKE_PLAY_X
            )
            {
                Local_u16SnakeX[0U] =
                    (u16)
                    (
                        SNAKE_PLAY_X
                        +
                        SNAKE_PLAY_WIDTH
                        -
                        SNAKE_BLOCK_SIZE
                    );
            }

            else
            {
                Local_u16SnakeX[0U] -=
                    SNAKE_BLOCK_SIZE;
            }
        }


        else if
        (
            Local_u8Direction == SNAKE_DIR_DOWN
        )
        {
            Local_u16SnakeY[0U] +=
                SNAKE_BLOCK_SIZE;


            if
            (
                Local_u16SnakeY[0U]
                >=
                (SNAKE_PLAY_Y + SNAKE_PLAY_HEIGHT)
            )
            {
                Local_u16SnakeY[0U] =
                    SNAKE_PLAY_Y;
            }
        }


        else
        {
            if
            (
                Local_u16SnakeY[0U]
                <=
                SNAKE_PLAY_Y
            )
            {
                Local_u16SnakeY[0U] =
                    (u16)
                    (
                        SNAKE_PLAY_Y
                        +
                        SNAKE_PLAY_HEIGHT
                        -
                        SNAKE_BLOCK_SIZE
                    );
            }

            else
            {
                Local_u16SnakeY[0U] -=
                    SNAKE_BLOCK_SIZE;
            }
        }


        Local_u8AteFood = 0U;


        if
        (
            (Local_u16SnakeX[0U] == Local_u16FoodX)
            &&
            (Local_u16SnakeY[0U] == Local_u16FoodY)
        )
        {
            Local_u8AteFood = 1U;

            Local_u16Score++;

            Buzzer_Eat();


            if
            (
                Local_u16SnakeLength < SNAKE_MAX_LENGTH
            )
            {
                Local_u16SnakeX[Local_u16SnakeLength] =
                    Local_u16OldTailX;

                Local_u16SnakeY[Local_u16SnakeLength] =
                    Local_u16OldTailY;

                Local_u16SnakeLength++;
            }


            Local_u16FoodX =
                (u16)
                (
                    SNAKE_PLAY_X
                    +
                    (
                        (((Local_u16Score * 7U) + 5U) % 19U)
                        *
                        SNAKE_BLOCK_SIZE
                    )
                );


            Local_u16FoodY =
                (u16)
                (
                    SNAKE_PLAY_Y
                    +
                    (
                        (((Local_u16Score * 5U) + 3U) % 15U)
                        *
                        SNAKE_BLOCK_SIZE
                    )
                );


            FillRect
            (
                55U,
                28U,
                25U,
                12U,
                ST7735S_BLACK
            );


            NumberToString
            (
                Local_u16Score,
                Local_acScoreText
            );


            DrawLargeString
            (
                55U,
                28U,
                Local_acScoreText,
                ST7735S_WHITE,
                1U
            );


            FillRect
            (
                Local_u16FoodX,
                Local_u16FoodY,
                SNAKE_BLOCK_SIZE,
                SNAKE_BLOCK_SIZE,
                COLOR_ORANGE
            );
        }


        for
        (
            Local_u16Index = 1U;
            Local_u16Index < Local_u16SnakeLength;
            Local_u16Index++
        )
        {
            if
            (
                (Local_u16SnakeX[0U] == Local_u16SnakeX[Local_u16Index])
                &&
                (Local_u16SnakeY[0U] == Local_u16SnakeY[Local_u16Index])
            )
            {
                Local_u8GameOver = 1U;

                Local_u8Running = 0U;

                break;
            }
        }


        if
        (
            Local_u8GameOver == 1U
        )
        {
            break;
        }


        if
        (
            Local_u8AteFood == 0U
        )
        {
            FillRect
            (
                Local_u16OldTailX,
                Local_u16OldTailY,
                SNAKE_BLOCK_SIZE,
                SNAKE_BLOCK_SIZE,
                ST7735S_BLACK
            );
        }


        if
        (
            Local_u16SnakeLength > 1U
        )
        {
            FillRect
            (
                Local_u16SnakeX[1U],
                Local_u16SnakeY[1U],
                SNAKE_BLOCK_SIZE,
                SNAKE_BLOCK_SIZE,
                COLOR_LIGHT_GRAY
            );
        }


        FillRect
        (
            Local_u16SnakeX[0U],
            Local_u16SnakeY[0U],
            SNAKE_BLOCK_SIZE,
            SNAKE_BLOCK_SIZE,
            ST7735S_GREEN
        );


        SysTick_DelayMs
        (
            (DifficultyLevel == DIFFICULTY_EASY) ? 240U :
            ((DifficultyLevel == DIFFICULTY_HARD) ? 120U : 180U)
        );
    }


    if
    (
        Local_u16Score > HighScoreSnake
    )
    {
        HighScoreSnake =
            Local_u16Score;

        Buzzer_NewHighScore();
    }


    if
    (
        Local_u8GameOver == 1U
    )
    {
        Buzzer_GameOver();

        ST7735S_FillScreen
        (
            ST7735S_BLACK
        );


        DrawLargeString
        (
            16U,
            32U,
            "GAME OVER",
            ST7735S_RED,
            2U
        );


        DrawLargeString
        (
            18U,
            78U,
            "FINAL SCORE",
            ST7735S_WHITE,
            1U
        );


        NumberToString
        (
            Local_u16Score,
            Local_acScoreText
        );


        DrawLargeString
        (
            58U,
            100U,
            Local_acScoreText,
            ST7735S_GREEN,
            2U
        );


        DrawLargeString
        (
            16U,
            150U,
            "SELECT: EXIT",
            ST7735S_WHITE,
            1U
        );


        while
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_SELECT
            )
            !=
            GPIO_LOW
        )
        {
        }


        WaitForRelease
        (
            BUTTON_SELECT
        );
    }
}

/* =========================================
 * MUSIC MAKER Game
 * ========================================= */

static void MusicMaker_Game(void)
{
    static u8 Local_u8AudioInitialized = 0U;

    u8 Local_u8SelectedNote = NOTE_DO4;
    u8 Local_u8ExitGame = 0U;
    u32 Local_u32PressCounter;

    const char *Local_acNotes[AUDIO_NOTES_COUNT] =
    {
        "DO4",
        "RE4",
        "MI4",
        "FA4",
        "SOL4",
        "LA4",
        "SI4",
        "DO5",
        "RE5",
        "MI5",
        "FA5",
        "SOL5",
        "LA5",
        "SI5"
    };


    if (Local_u8AudioInitialized == 0U)
    {
        AUDIO_Init();

        Local_u8AudioInitialized = 1U;
    }


    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );


    DrawLargeString
    (
        20U,
        15U,
        "MUSIC MAKER",
        ST7735S_GREEN,
        1U
    );


    DrawLargeString
    (
        45U,
        55U,
        Local_acNotes[Local_u8SelectedNote],
        ST7735S_WHITE,
        3U
    );


    DrawLargeString
    (
        8U,
        115U,
        "UP/DOWN: NOTE",
        ST7735S_WHITE,
        1U
    );


    DrawLargeString
    (
        8U,
        135U,
        "SELECT: PLAY",
        ST7735S_WHITE,
        1U
    );


    DrawLargeString
    (
        8U,
        150U,
        "HOLD: EXIT",
        ST7735S_WHITE,
        1U
    );


    while (Local_u8ExitGame == 0U)
    {
        Display_Refresh(3U);
        if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_UP
            )
            ==
            GPIO_LOW
        )
        {
            Local_u8SelectedNote++;

            if
            (
                Local_u8SelectedNote
                >=
                AUDIO_NOTES_COUNT
            )
            {
                Local_u8SelectedNote = NOTE_DO4;
            }


            FillRect
            (
                0U,
                45U,
                160U,
                55U,
                ST7735S_BLACK
            );


            DrawLargeString
            (
                45U,
                55U,
                Local_acNotes[Local_u8SelectedNote],
                ST7735S_WHITE,
                3U
            );


            WaitForRelease
            (
                BUTTON_UP
            );


            Buzzer_Click();
        }


        else if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_SELECT
            )
            ==
            GPIO_LOW
        )
        {
            if (LastInputWasIR == 1U)
            {
                LastInputWasIR = 0U;

                if (SoundEnabled == 1U)
                {
                    AUDIO_PlayNote
                    (
                        Local_u8SelectedNote
                    );
                }

                SysTick_DelayMs(250U);
                AUDIO_Stop();
            }
            else
            {
                Local_u32PressCounter = 0U;

                if (SoundEnabled == 1U)
                {
                    AUDIO_PlayNote
                    (
                        Local_u8SelectedNote
                    );
                }

                while
                (
                    GPIO_ReadPin
                    (
                        GPIO_PORT_B,
                        BUTTON_SELECT
                    )
                    ==
                    GPIO_LOW
                )
                {
                    Local_u32PressCounter++;

                    if
                    (
                        Local_u32PressCounter
                        >=
                        1500000U
                    )
                    {
                        Local_u8ExitGame = 1U;
                        break;
                    }
                }

                AUDIO_Stop();

                if
                (
                    Local_u8ExitGame
                    ==
                    0U
                )
                {
                    WaitForRelease
                    (
                        BUTTON_SELECT
                    );
                }
            }
        }
    }
}


/* =========================================
 * Welcome Screen
 * ========================================= */

static void WelcomeScreen(void)
{
    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );


    DrawLargeString
    (
        20U,
        22U,
        "WELCOME",
        ST7735S_GREEN,
        2U
    );


    DrawLargeString
    (
        4U,
        62U,
        "GAMING HUB",
        ST7735S_WHITE,
        2U
    );


    DrawLargeString
    (
        22U,
        112U,
        "4 GAMES",
        COLOR_ORANGE,
        1U
    );


    DrawLargeString
    (
        12U,
        150U,
        "PRESS SELECT",
        ST7735S_WHITE,
        1U
    );


    while
    (
        Input_ReadPin
        (
            GPIO_PORT_B,
            BUTTON_SELECT
        )
        !=
        GPIO_LOW
    )
    {
    }


    WaitForRelease
    (
        BUTTON_SELECT
    );
}

/* =========================================
 * Draw Main Menu
 * ========================================= */

static void DrawMainMenu(void)
{
    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );

    DrawLargeString
    (
        4U,
        8U,
        "GAMING HUB",
        ST7735S_GREEN,
        2U
    );

    DrawLargeString
    (
        10U,
        40U,
        (SelectedItem == 0U) ? "> MATH QUIZ" : "  MATH QUIZ",
        (SelectedItem == 0U) ? ST7735S_GREEN : ST7735S_WHITE,
        1U
    );

    DrawLargeString
    (
        10U,
        58U,
        (SelectedItem == 1U) ? "> MEMORY" : "  MEMORY",
        (SelectedItem == 1U) ? ST7735S_GREEN : ST7735S_WHITE,
        1U
    );

    DrawLargeString
    (
        10U,
        76U,
        (SelectedItem == 2U) ? "> SNAKE" : "  SNAKE",
        (SelectedItem == 2U) ? ST7735S_GREEN : ST7735S_WHITE,
        1U
    );

    DrawLargeString
    (
        10U,
        94U,
        (SelectedItem == 3U) ? "> MUSIC MAKER" : "  MUSIC MAKER",
        (SelectedItem == 3U) ? ST7735S_GREEN : ST7735S_WHITE,
        1U
    );

    DrawLargeString
    (
        10U,
        112U,
        (SelectedItem == 4U) ? "> SETTINGS" : "  SETTINGS",
        (SelectedItem == 4U) ? COLOR_ORANGE : ST7735S_WHITE,
        1U
    );

    DrawLargeString
    (
        8U,
        136U,
        "MATH:",
        ST7735S_WHITE,
        1U
    );

    {
        char Local_acScoreText[6];

        NumberToString
        (
            HighScoreMath,
            Local_acScoreText
        );

        DrawLargeString
        (
            44U,
            136U,
            Local_acScoreText,
            ST7735S_GREEN,
            1U
        );

        DrawLargeString
        (
            72U,
            136U,
            "MEM:",
            ST7735S_WHITE,
            1U
        );

        if (BestMovesMemory == 255U)
        {
            DrawLargeString
            (
                108U,
                136U,
                "0",
                ST7735S_GREEN,
                1U
            );
        }
        else
        {
            NumberToString
            (
                BestMovesMemory,
                Local_acScoreText
            );

            DrawLargeString
            (
                108U,
                136U,
                Local_acScoreText,
                ST7735S_GREEN,
                1U
            );
        }

        DrawLargeString
        (
            36U,
            148U,
            "SNAKE:",
            ST7735S_WHITE,
            1U
        );

        NumberToString
        (
            HighScoreSnake,
            Local_acScoreText
        );

        DrawLargeString
        (
            82U,
            148U,
            Local_acScoreText,
            ST7735S_GREEN,
            1U
        );
    }
}


/* =========================================
 * Update Main Menu Selection Without Clearing
 * the Whole TFT screen.
 * ========================================= */
static void UpdateMainMenuSelection(u8 Copy_u8PreviousItem, u8 Copy_u8CurrentItem)
{
    u8 Local_u8Item;
    u16 Local_u16Y;

    for (Local_u8Item = 0U; Local_u8Item < 5U; Local_u8Item++)
    {
        if ((Local_u8Item == Copy_u8PreviousItem) ||
            (Local_u8Item == Copy_u8CurrentItem))
        {
            Local_u16Y = (u16)(40U + (Local_u8Item * 18U));

            FillRect
            (
                0U,
                Local_u16Y,
                160U,
                16U,
                ST7735S_BLACK
            );

            if (Local_u8Item == 0U)
            {
                DrawLargeString(10U, Local_u16Y,
                    (SelectedItem == 0U) ? "> MATH QUIZ" : "  MATH QUIZ",
                    (SelectedItem == 0U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
            else if (Local_u8Item == 1U)
            {
                DrawLargeString(10U, Local_u16Y,
                    (SelectedItem == 1U) ? "> MEMORY" : "  MEMORY",
                    (SelectedItem == 1U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
            else if (Local_u8Item == 2U)
            {
                DrawLargeString(10U, Local_u16Y,
                    (SelectedItem == 2U) ? "> SNAKE" : "  SNAKE",
                    (SelectedItem == 2U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
            else if (Local_u8Item == 3U)
            {
                DrawLargeString(10U, Local_u16Y,
                    (SelectedItem == 3U) ? "> MUSIC MAKER" : "  MUSIC MAKER",
                    (SelectedItem == 3U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
            else
            {
                DrawLargeString(10U, Local_u16Y,
                    (SelectedItem == 4U) ? "> SETTINGS" : "  SETTINGS",
                    (SelectedItem == 4U) ? COLOR_ORANGE : ST7735S_WHITE, 1U);
            }
        }
    }
}


/* =========================================
 * Settings
 * ========================================= */

static void DrawSettingsMenu(u8 Copy_u8SelectedItem)
{
    const char *Local_pcDifficulty;
    const char *Local_pcDisplay;

    if (DifficultyLevel == DIFFICULTY_EASY)
    {
        Local_pcDifficulty = "EASY";
    }
    else if (DifficultyLevel == DIFFICULTY_HARD)
    {
        Local_pcDifficulty = "HARD";
    }
    else
    {
        Local_pcDifficulty = "MED";
    }

    if (DisplayMode == DISPLAY_TFT_ONLY)
    {
        Local_pcDisplay = "TFT";
    }
    else
    {
        Local_pcDisplay = "BOTH";
    }

    ST7735S_FillScreen(ST7735S_BLACK);

    DrawLargeString(28U, 8U, "SETTINGS", COLOR_ORANGE, 2U);

    DrawLargeString(6U, 40U,
        (Copy_u8SelectedItem == 0U) ? "> SOUND" : "  SOUND",
        (Copy_u8SelectedItem == 0U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
    DrawLargeString(88U, 40U,
        (SoundEnabled == 1U) ? "ON" : "OFF",
        (SoundEnabled == 1U) ? ST7735S_GREEN : ST7735S_RED, 1U);

    DrawLargeString(6U, 62U,
        (Copy_u8SelectedItem == 1U) ? "> DIFFICULTY" : "  DIFFICULTY",
        (Copy_u8SelectedItem == 1U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
    DrawLargeString(94U, 62U, Local_pcDifficulty, ST7735S_GREEN, 1U);

    DrawLargeString(6U, 84U,
        (Copy_u8SelectedItem == 2U) ? "> DISPLAY" : "  DISPLAY",
        (Copy_u8SelectedItem == 2U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
    DrawLargeString(94U, 84U, Local_pcDisplay, ST7735S_GREEN, 1U);

    DrawLargeString(6U, 106U,
        (Copy_u8SelectedItem == 3U) ? "> HIGH SCORES" : "  HIGH SCORES",
        (Copy_u8SelectedItem == 3U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);

    DrawLargeString(6U, 128U,
        (Copy_u8SelectedItem == 4U) ? "> RESET SCORES" : "  RESET SCORES",
        (Copy_u8SelectedItem == 4U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);

    DrawLargeString(6U, 150U,
        (Copy_u8SelectedItem == 5U) ? "> BACK" : "  BACK",
        (Copy_u8SelectedItem == 5U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
}


/* =========================================
 * Update Settings Selection Without Clearing
 * the Whole TFT screen.
 * ========================================= */
static void UpdateSettingsSelection(u8 Copy_u8PreviousItem, u8 Copy_u8CurrentItem)
{
    u8 Local_u8Item;
    u16 Local_u16Y;
    const char *Local_pcDifficulty;
    const char *Local_pcDisplay;

    if (DifficultyLevel == DIFFICULTY_EASY)
    {
        Local_pcDifficulty = "EASY";
    }
    else if (DifficultyLevel == DIFFICULTY_HARD)
    {
        Local_pcDifficulty = "HARD";
    }
    else
    {
        Local_pcDifficulty = "MED";
    }

    if (DisplayMode == DISPLAY_TFT_ONLY)
    {
        Local_pcDisplay = "TFT";
    }
    else
    {
        Local_pcDisplay = "BOTH";
    }

    for (Local_u8Item = 0U; Local_u8Item < 6U; Local_u8Item++)
    {
        if ((Local_u8Item == Copy_u8PreviousItem) ||
            (Local_u8Item == Copy_u8CurrentItem))
        {
            Local_u16Y = (u16)(40U + (Local_u8Item * 22U));

            FillRect(0U, Local_u16Y, 160U, 20U, ST7735S_BLACK);

            if (Local_u8Item == 0U)
            {
                DrawLargeString(6U, Local_u16Y,
                    (Copy_u8CurrentItem == 0U) ? "> SOUND" : "  SOUND",
                    (Copy_u8CurrentItem == 0U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
                DrawLargeString(88U, Local_u16Y,
                    (SoundEnabled == 1U) ? "ON" : "OFF",
                    (SoundEnabled == 1U) ? ST7735S_GREEN : ST7735S_RED, 1U);
            }
            else if (Local_u8Item == 1U)
            {
                DrawLargeString(6U, Local_u16Y,
                    (Copy_u8CurrentItem == 1U) ? "> DIFFICULTY" : "  DIFFICULTY",
                    (Copy_u8CurrentItem == 1U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
                DrawLargeString(94U, Local_u16Y, Local_pcDifficulty, ST7735S_GREEN, 1U);
            }
            else if (Local_u8Item == 2U)
            {
                DrawLargeString(6U, Local_u16Y,
                    (Copy_u8CurrentItem == 2U) ? "> DISPLAY" : "  DISPLAY",
                    (Copy_u8CurrentItem == 2U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
                DrawLargeString(94U, Local_u16Y, Local_pcDisplay, ST7735S_GREEN, 1U);
            }
            else if (Local_u8Item == 3U)
            {
                DrawLargeString(6U, Local_u16Y,
                    (Copy_u8CurrentItem == 3U) ? "> HIGH SCORES" : "  HIGH SCORES",
                    (Copy_u8CurrentItem == 3U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
            else if (Local_u8Item == 4U)
            {
                DrawLargeString(6U, Local_u16Y,
                    (Copy_u8CurrentItem == 4U) ? "> RESET SCORES" : "  RESET SCORES",
                    (Copy_u8CurrentItem == 4U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
            else
            {
                DrawLargeString(6U, Local_u16Y,
                    (Copy_u8CurrentItem == 5U) ? "> BACK" : "  BACK",
                    (Copy_u8CurrentItem == 5U) ? ST7735S_GREEN : ST7735S_WHITE, 1U);
            }
        }
    }
}


static void ShowHighScoresScreen(void)
{
    char Local_acScoreText[6];

    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );

    DrawLargeString
    (
        18U,
        18U,
        "HIGH SCORES",
        COLOR_ORANGE,
        2U
    );

    DrawLargeString
    (
        15U,
        65U,
        "MATH:",
        ST7735S_WHITE,
        1U
    );

    NumberToString
    (
        HighScoreMath,
        Local_acScoreText
    );

    DrawLargeString
    (
        95U,
        65U,
        Local_acScoreText,
        ST7735S_GREEN,
        1U
    );

    DrawLargeString
    (
        15U,
        90U,
        "MEMORY:",
        ST7735S_WHITE,
        1U
    );

    if (BestMovesMemory == 255U)
    {
        DrawLargeString
        (
            95U,
            90U,
            "0",
            ST7735S_GREEN,
            1U
        );
    }
    else
    {
        NumberToString
        (
            BestMovesMemory,
            Local_acScoreText
        );

        DrawLargeString
        (
            95U,
            90U,
            Local_acScoreText,
            ST7735S_GREEN,
            1U
        );
    }

    DrawLargeString
    (
        15U,
        115U,
        "SNAKE:",
        ST7735S_WHITE,
        1U
    );

    NumberToString
    (
        HighScoreSnake,
        Local_acScoreText
    );

    DrawLargeString
    (
        95U,
        115U,
        Local_acScoreText,
        ST7735S_GREEN,
        1U
    );

    DrawLargeString
    (
        12U,
        150U,
        "PRESS SELECT",
        ST7735S_WHITE,
        1U
    );

    while
    (
        Input_ReadPin
        (
            GPIO_PORT_B,
            BUTTON_SELECT
        )
        !=
        GPIO_LOW
    )
    {
    }

    WaitForRelease
    (
        BUTTON_SELECT
    );
}


static void ConfirmResetScores(void)
{
    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );

    DrawLargeString
    (
        25U,
        35U,
        "RESET ALL",
        ST7735S_RED,
        2U
    );

    DrawLargeString
    (
        22U,
        70U,
        "SCORES?",
        ST7735S_RED,
        2U
    );

    DrawLargeString
    (
        8U,
        125U,
        "PRESS SELECT",
        ST7735S_WHITE,
        1U
    );

    DrawLargeString
    (
        24U,
        145U,
        "TO CONFIRM",
        ST7735S_WHITE,
        1U
    );

    while
    (
        Input_ReadPin
        (
            GPIO_PORT_B,
            BUTTON_SELECT
        )
        !=
        GPIO_LOW
    )
    {
    }

    WaitForRelease
    (
        BUTTON_SELECT
    );

    HighScoreMath = 0U;
    BestMovesMemory = 255U;
    HighScoreSnake = 0U;

    Buzzer_Select();

    ST7735S_FillScreen
    (
        ST7735S_BLACK
    );

    DrawLargeString
    (
        30U,
        60U,
        "SCORES",
        ST7735S_GREEN,
        2U
    );

    DrawLargeString
    (
        30U,
        90U,
        "RESET!",
        ST7735S_GREEN,
        2U
    );

    Delay(800000U);
}


static void SettingsMenu(void)
{
    u8 Local_u8SelectedItem = 0U;
    u8 Local_u8Exit = 0U;

    DrawSettingsMenu(Local_u8SelectedItem);

    while (Local_u8Exit == 0U)
    {
        LED_MATRIX_Refresh();

        if (Input_ReadPin(GPIO_PORT_B, BUTTON_UP) == GPIO_LOW)
        {
            {
                u8 Local_u8PreviousItem = Local_u8SelectedItem;

                if (Local_u8SelectedItem == 0U)
                {
                    Local_u8SelectedItem = 5U;
                }
                else
                {
                    Local_u8SelectedItem--;
                }

                WaitForRelease(BUTTON_UP);
                Buzzer_Click();
                UpdateSettingsSelection(Local_u8PreviousItem, Local_u8SelectedItem);
            }
        }
        else if (Input_ReadPin(GPIO_PORT_B, BUTTON_DOWN) == GPIO_LOW)
        {
            {
                u8 Local_u8PreviousItem = Local_u8SelectedItem;

                Local_u8SelectedItem++;
                if (Local_u8SelectedItem >= 6U)
                {
                    Local_u8SelectedItem = 0U;
                }

                WaitForRelease(BUTTON_DOWN);
                Buzzer_Click();
                UpdateSettingsSelection(Local_u8PreviousItem, Local_u8SelectedItem);
            }
        }
        else if (Input_ReadPin(GPIO_PORT_B, BUTTON_SELECT) == GPIO_LOW)
        {
            WaitForRelease(BUTTON_SELECT);

            if (Local_u8SelectedItem == 0U)
            {
                SoundEnabled = (SoundEnabled == 0U) ? 1U : 0U;
                if (SoundEnabled == 1U) { Buzzer_Click(); }
            }
            else if (Local_u8SelectedItem == 1U)
            {
                DifficultyLevel++;
                if (DifficultyLevel > DIFFICULTY_HARD) { DifficultyLevel = DIFFICULTY_EASY; }
                Buzzer_Click();
            }
            else if (Local_u8SelectedItem == 2U)
            {
                DisplayMode = (DisplayMode == DISPLAY_BOTH) ? DISPLAY_TFT_ONLY : DISPLAY_BOTH;
                Buzzer_Click();
            }
            else if (Local_u8SelectedItem == 3U)
            {
                ShowHighScoresScreen();
            }
            else if (Local_u8SelectedItem == 4U)
            {
                ConfirmResetScores();
            }
            else
            {
                Local_u8Exit = 1U;
            }

            if (Local_u8Exit == 0U)
            {
                DrawSettingsMenu(Local_u8SelectedItem);
            }
        }
    }
}

/* =========================================
 * Main Function
 * ========================================= */

int main(void)
{
    GPIO_PinConfig_t ButtonConfig;
    GPIO_PinConfig_t BuzzerConfig;


    /* Initialize TFT */

    ST7735S_Init();


    /* Enable GPIOA Clock for LED Matrix */

    RCC_EnablePeripheralClock
    (
        RCC_AHB1,
        RCC_GPIOA
    );


    /* Initialize LED Matrix */

    LED_MATRIX_Init();



    /* Enable GPIOB Clock */

    RCC_EnablePeripheralClock
    (
        RCC_AHB1,
        RCC_GPIOB
    );


    /* Button Configuration */

    ButtonConfig.Mode =
        GPIO_MODE_INPUT;

    ButtonConfig.OutputType =
        GPIO_OUTPUT_PUSH_PULL;

    ButtonConfig.Speed =
        GPIO_SPEED_LOW;

    ButtonConfig.Pull =
        GPIO_PULL_UP;

    ButtonConfig.AlternateFunction =
        GPIO_AF0;


    /* UP */

    GPIO_InitPin
    (
        GPIO_PORT_B,
        BUTTON_UP,
        &ButtonConfig
    );


    /* DOWN */

    GPIO_InitPin
    (
        GPIO_PORT_B,
        BUTTON_DOWN,
        &ButtonConfig
    );


    /* SELECT */

    GPIO_InitPin
    (
        GPIO_PORT_B,
        BUTTON_SELECT,
        &ButtonConfig
    );


    BuzzerConfig.Mode =
        GPIO_MODE_OUTPUT;

    BuzzerConfig.OutputType =
        GPIO_OUTPUT_PUSH_PULL;

    BuzzerConfig.Speed =
        GPIO_SPEED_LOW;

    BuzzerConfig.Pull =
        GPIO_NO_PULL;

    BuzzerConfig.AlternateFunction =
        GPIO_AF0;


    GPIO_InitPin
    (
        GPIO_PORT_B,
        BUZZER_PIN,
        &BuzzerConfig
    );


    GPIO_WritePin
    (
        GPIO_PORT_B,
        BUZZER_PIN,
        GPIO_LOW
    );


    WelcomeScreen();

    IR_Init();
        IR_ReadyInMain = 1U;

    DrawMainMenu();


    while (1)
    {
        /* UP */

        if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_UP
            )
            ==
            GPIO_LOW
        )
        {
            {
                u8 Local_u8PreviousItem = SelectedItem;

                if (SelectedItem == 0U)
                {
                    SelectedItem = 4U;
                }
                else
                {
                    SelectedItem--;
                }

                UpdateMainMenuSelection(Local_u8PreviousItem, SelectedItem);
            }


            WaitForRelease
            (
                BUTTON_UP
            );


            Buzzer_Click();
        }


        /* DOWN */

        if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_DOWN
            )
            ==
            GPIO_LOW
        )
        {
            {
                u8 Local_u8PreviousItem = SelectedItem;

                SelectedItem++;

                if (SelectedItem >= 5U)
                {
                    SelectedItem = 0U;
                }

                UpdateMainMenuSelection(Local_u8PreviousItem, SelectedItem);
            }


            WaitForRelease
            (
                BUTTON_DOWN
            );


            Buzzer_Click();
        }


        /* SELECT */

        if
        (
            Input_ReadPin
            (
                GPIO_PORT_B,
                BUTTON_SELECT
            )
            ==
            GPIO_LOW
        )
        {
            WaitForRelease
            (
                BUTTON_SELECT
            );


            Buzzer_Select();
            LastInputWasIR = 0U;


            if
            (
                SelectedItem == 0U
            )
            {
                MathQuiz_Game();

                DrawMainMenu();
            }

            else if
            (
                SelectedItem == 1U
            )
            {
                Memory_Game();

                DrawMainMenu();
            }

            else if
            (
                SelectedItem == 2U
            )
            {
                Snake_Game();

                DrawMainMenu();
            }

            else if
            (
                SelectedItem == 3U
            )
            {
                MusicMaker_Game();

                DrawMainMenu();
            }

            else if
            (
                SelectedItem == 4U
            )
            {
                Display_Refresh(0U);

                SettingsMenu();

                DrawMainMenu();
            }
        }
    }
}
