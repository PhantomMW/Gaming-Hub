#include "STD_Types.h"
#include "GPIO_interface.h"
#include "LED_MATRIX_interface.h"

#define LED_MATRIX_DATA_PORT GPIO_PORT_A
#define LED_MATRIX_DATA_PIN GPIO_PIN_0
#define LED_MATRIX_CLOCK_PORT GPIO_PORT_A
#define LED_MATRIX_CLOCK_PIN GPIO_PIN_1
#define LED_MATRIX_LATCH_PORT GPIO_PORT_A
#define LED_MATRIX_LATCH_PIN GPIO_PIN_2

static u8 LED_MATRIX_Frame[8] = {0U};
static u8 LED_MATRIX_CurrentRow = 0U;

static void LED_MATRIX_SendBit(u8 Copy_u8Bit)
{
    GPIO_WritePin(LED_MATRIX_DATA_PORT, LED_MATRIX_DATA_PIN, Copy_u8Bit);
    GPIO_WritePin(LED_MATRIX_CLOCK_PORT, LED_MATRIX_CLOCK_PIN, 1U);
    GPIO_WritePin(LED_MATRIX_CLOCK_PORT, LED_MATRIX_CLOCK_PIN, 0U);
}

void LED_MATRIX_Init(void)
{
    GPIO_PinConfig_t Local_Config;

    Local_Config.Mode = GPIO_MODE_OUTPUT;
    Local_Config.OutputType = GPIO_OUTPUT_PUSH_PULL;
    Local_Config.Speed = GPIO_SPEED_LOW;
    Local_Config.Pull = GPIO_NO_PULL;
    Local_Config.AlternateFunction = GPIO_AF0;

    GPIO_InitPin(LED_MATRIX_DATA_PORT, LED_MATRIX_DATA_PIN, &Local_Config);
    GPIO_InitPin(LED_MATRIX_CLOCK_PORT, LED_MATRIX_CLOCK_PIN, &Local_Config);
    GPIO_InitPin(LED_MATRIX_LATCH_PORT, LED_MATRIX_LATCH_PIN, &Local_Config);

    GPIO_WritePin(LED_MATRIX_DATA_PORT, LED_MATRIX_DATA_PIN, 0U);
    GPIO_WritePin(LED_MATRIX_CLOCK_PORT, LED_MATRIX_CLOCK_PIN, 0U);
    GPIO_WritePin(LED_MATRIX_LATCH_PORT, LED_MATRIX_LATCH_PIN, 0U);

    LED_MATRIX_Clear();
    LED_MATRIX_SendData(0U);
}

void LED_MATRIX_SendData(u16 Copy_u16Data)
{
    u8 Local_u8Counter;

    for (Local_u8Counter = 0U; Local_u8Counter < 16U; Local_u8Counter++)
    {
        LED_MATRIX_SendBit((u8)((Copy_u16Data >> (15U - Local_u8Counter)) & 1U));
    }

    GPIO_WritePin(LED_MATRIX_LATCH_PORT, LED_MATRIX_LATCH_PIN, 1U);
    GPIO_WritePin(LED_MATRIX_LATCH_PORT, LED_MATRIX_LATCH_PIN, 0U);
}

void LED_MATRIX_Clear(void)
{
    u8 Local_u8Index;

    for (Local_u8Index = 0U; Local_u8Index < 8U; Local_u8Index++)
    {
        LED_MATRIX_Frame[Local_u8Index] = 0U;
    }
}

void LED_MATRIX_SetPixel(u8 Copy_u8Row, u8 Copy_u8Column, u8 Copy_u8State)
{
    if ((Copy_u8Row < 8U) && (Copy_u8Column < 8U))
    {
        if (Copy_u8State == 0U)
        {
            LED_MATRIX_Frame[Copy_u8Row] &= (u8)(~(1U << Copy_u8Column));
        }
        else
        {
            LED_MATRIX_Frame[Copy_u8Row] |= (u8)(1U << Copy_u8Column);
        }
    }
}

void LED_MATRIX_SetFrame(const u8 *Copy_pu8Frame)
{
    u8 Local_u8Index;

    if (Copy_pu8Frame != 0)
    {
        for (Local_u8Index = 0U; Local_u8Index < 8U; Local_u8Index++)
        {
            LED_MATRIX_Frame[Local_u8Index] = Copy_pu8Frame[Local_u8Index];
        }
    }
}

void LED_MATRIX_Refresh(void)
{
    u8 Local_u8RowData;
    u8 Local_u8ColumnData;
    u16 Local_u16Data;

    Local_u8RowData = (u8)(1U << LED_MATRIX_CurrentRow);
    Local_u8ColumnData = LED_MATRIX_Frame[LED_MATRIX_CurrentRow];

    Local_u16Data = (((u16)Local_u8ColumnData << 8U) | (u16)Local_u8RowData);
    LED_MATRIX_SendData(Local_u16Data);

    LED_MATRIX_CurrentRow++;

    if (LED_MATRIX_CurrentRow >= 8U)
    {
        LED_MATRIX_CurrentRow = 0U;
    }
}

void LED_MATRIX_ShowMenu(u8 Copy_u8Selection)
{
    static const u8 Local_au8Frames[4][8] =
    {
        {0x18U,0x18U,0x18U,0x7EU,0x18U,0x18U,0x18U,0x00U},
        {0x3CU,0x42U,0xA5U,0x81U,0x81U,0xA5U,0x42U,0x3CU},
        {0x18U,0x3CU,0x7EU,0x3CU,0x18U,0x1CU,0x18U,0x10U},
        {0x00U,0x00U,0x24U,0x66U,0xFFU,0x66U,0x24U,0x00U}
    };

    LED_MATRIX_SetFrame(Local_au8Frames[Copy_u8Selection % 4U]);
}

void LED_MATRIX_ShowCorrect(void)
{
    static const u8 Local_au8Frame[8] =
    {0x01U,0x03U,0x06U,0x0CU,0x18U,0x30U,0x60U,0xC0U};

    LED_MATRIX_SetFrame(Local_au8Frame);
}

void LED_MATRIX_ShowWrong(void)
{
    static const u8 Local_au8Frame[8] =
    {0x81U,0x42U,0x24U,0x18U,0x18U,0x24U,0x42U,0x81U};

    LED_MATRIX_SetFrame(Local_au8Frame);
}

void LED_MATRIX_ShowMemory(void)
{
    static const u8 Local_au8Frame[8] =
    {0x3CU,0x42U,0xA5U,0x81U,0xA5U,0x99U,0x42U,0x3CU};

    LED_MATRIX_SetFrame(Local_au8Frame);
}

void LED_MATRIX_ShowSnake(void)
{
    static const u8 Local_au8Frame[8] =
    {0x00U,0x1CU,0x22U,0x40U,0x5EU,0x42U,0x24U,0x18U};

    LED_MATRIX_SetFrame(Local_au8Frame);
}

void LED_MATRIX_ShowMusic(u8 Copy_u8Level)
{
    u8 Local_u8Index;
    u8 Local_u8Height;

    LED_MATRIX_Clear();

    Local_u8Height = (u8)((Copy_u8Level % 8U) + 1U);

    for (Local_u8Index = 0U; Local_u8Index < Local_u8Height; Local_u8Index++)
    {
        LED_MATRIX_SetPixel((u8)(7U - Local_u8Index), 1U, 1U);
        LED_MATRIX_SetPixel((u8)(7U - Local_u8Index), 3U, 1U);
        LED_MATRIX_SetPixel((u8)(7U - Local_u8Index), 5U, 1U);
        LED_MATRIX_SetPixel((u8)(7U - Local_u8Index), 7U, 1U);
    }
}

void LED_MATRIX_Test(void)
{
    LED_MATRIX_ShowMenu(0U);
}
