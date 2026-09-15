#ifndef ST7735S_PROGRAM_C
#define ST7735S_PROGRAM_C


#include "STD_TYPES.h"


#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "SPI_interface.h"
#include "SysTick_interface.h"


#include "ST7735S_interface.h"
#include "ST7735S_private.h"
#include "ST7735S_config.h"
#include "ST7735S_font.h"


/* =========================================================
 * Private Variables
 * ========================================================= */

static u16 ST7735S_CurrentWidth =
    ST7735S_CONFIG_WIDTH;


static u16 ST7735S_CurrentHeight =
    ST7735S_CONFIG_HEIGHT;


/* =========================================================
 * Private Functions
 * ========================================================= */


/**
 * @brief Transmit one byte through SPI.
 *
 * SPI_u16TransmitReceive is intentionally used instead of
 * SPI_voidTransmit because SPI is physically full duplex and
 * this function reads the received byte as well.
 */
static void ST7735S_WriteByte(
    u8 Copy_u8Data
)
{
    (void)SPI_u16TransmitReceive(
        ST7735S_SPI_ID,
        (u16)Copy_u8Data
    );
}


/**
 * @brief Wait until SPI is not busy.
 */
static void ST7735S_WaitForSPI(
    void
)
{
    while (
        SPI_u8GetFlagStatus(
            ST7735S_SPI_ID,
            SPI_FLAG_BSY
        ) != 0U
    )
    {
    }
}


/**
 * @brief Start TFT communication.
 */
static void ST7735S_BeginWrite(
    void
)
{
    GPIO_WritePin(
        ST7735S_CS_PORT,
        ST7735S_CS_PIN,
        GPIO_LOW
    );
}


/**
 * @brief Finish TFT communication.
 */
static void ST7735S_EndWrite(
    void
)
{
    ST7735S_WaitForSPI();

    GPIO_WritePin(
        ST7735S_CS_PORT,
        ST7735S_CS_PIN,
        GPIO_HIGH
    );
}


/**
 * @brief Send command.
 *
 * CS must already be LOW.
 */
static void ST7735S_WriteCommandRaw(
    u8 Copy_u8Command
)
{
    GPIO_WritePin(
        ST7735S_DC_PORT,
        ST7735S_DC_PIN,
        GPIO_LOW
    );

    ST7735S_WriteByte(
        Copy_u8Command
    );
}


/**
 * @brief Send one data byte.
 *
 * CS must already be LOW.
 */
static void ST7735S_WriteDataRaw(
    u8 Copy_u8Data
)
{
    GPIO_WritePin(
        ST7735S_DC_PORT,
        ST7735S_DC_PIN,
        GPIO_HIGH
    );

    ST7735S_WriteByte(
        Copy_u8Data
    );
}


/**
 * @brief Send command with optional parameters.
 */
static void ST7735S_SendCommandData(
    u8 Copy_u8Command,
    const u8 *Copy_pu8Data,
    u8 Copy_u8Size
)
{
    u8 Local_u8Index;


    ST7735S_BeginWrite();


    ST7735S_WriteCommandRaw(
        Copy_u8Command
    );


    for (
        Local_u8Index = 0U;
        Local_u8Index < Copy_u8Size;
        Local_u8Index++
    )
    {
        ST7735S_WriteDataRaw(
            Copy_pu8Data[Local_u8Index]
        );
    }


    ST7735S_EndWrite();
}


/**
 * @brief Configure one output GPIO pin.
 */
static void ST7735S_InitOutputPin(
    GPIO_Port_t Copy_u8Port,
    GPIO_Pin_t Copy_u8Pin
)
{
    GPIO_PinConfig_t Local_stPinConfig;


    Local_stPinConfig.Mode =
        GPIO_MODE_OUTPUT;

    Local_stPinConfig.OutputType =
        GPIO_OUTPUT_PUSH_PULL;

    Local_stPinConfig.Speed =
        GPIO_SPEED_VERY_HIGH;

    Local_stPinConfig.Pull =
        GPIO_NO_PULL;

    Local_stPinConfig.AlternateFunction =
        GPIO_AF0;


    GPIO_InitPin(
        Copy_u8Port,
        Copy_u8Pin,
        &Local_stPinConfig
    );
}


/**
 * @brief Configure SPI pins.
 */
static void ST7735S_InitSPIPins(
    void
)
{
    GPIO_PinConfig_t Local_stSPIConfig;


    Local_stSPIConfig.Mode =
        GPIO_MODE_AF;

    Local_stSPIConfig.OutputType =
        GPIO_OUTPUT_PUSH_PULL;

    Local_stSPIConfig.Speed =
        GPIO_SPEED_VERY_HIGH;

    Local_stSPIConfig.Pull =
        GPIO_NO_PULL;

    Local_stSPIConfig.AlternateFunction =
        ST7735S_SPI_AF;


    /* PA5 -> SPI1_SCK */

    GPIO_InitPin(
        ST7735S_SPI_PORT,
        ST7735S_SCK_PIN,
        &Local_stSPIConfig
    );


    /* PA7 -> SPI1_MOSI */

    GPIO_InitPin(
        ST7735S_SPI_PORT,
        ST7735S_MOSI_PIN,
        &Local_stSPIConfig
    );
}


/**
 * @brief Initialize SPI1 for ST7735S.
 *
 * ST7735S works with standard SPI Mode 0:
 *
 * CPOL = 0
 * CPHA = 0
 */
static void ST7735S_InitSPI(
    void
)
{
    SPI_Config_t Local_stSPIConfig;


    Local_stSPIConfig.Mode =
        SPI_MASTER;


    Local_stSPIConfig.CommunicationMode =
        SPI_FULL_DUPLEX;


    Local_stSPIConfig.ClockPolarity =
        SPI_CPOL_LOW;


    Local_stSPIConfig.ClockPhase =
        SPI_CPHA_FIRST_EDGE;


    /*
     * With the current 16 MHz HCLK configuration:
     *
     * SPI Clock = 16 MHz / 2 = 8 MHz
     */

    Local_stSPIConfig.BaudRate =
        SPI_BAUDRATE_DIV_2;


    Local_stSPIConfig.DataSize =
        SPI_DATA_SIZE_8_BIT;


    Local_stSPIConfig.BitOrder =
        SPI_MSB_FIRST;


    Local_stSPIConfig.NSSManagement =
        SPI_NSS_SOFTWARE;


    Local_stSPIConfig.CRCState =
        SPI_CRC_DISABLE;


    Local_stSPIConfig.FrameFormat =
        SPI_MOTOROLA_MODE;


    SPI_voidInit(
        ST7735S_SPI_ID,
        &Local_stSPIConfig
    );


    SPI_voidEnable(
        ST7735S_SPI_ID
    );
}


/**
 * @brief Perform hardware reset sequence.
 */
static void ST7735S_HardwareReset(
    void
)
{
    GPIO_WritePin(
        ST7735S_RST_PORT,
        ST7735S_RST_PIN,
        GPIO_HIGH
    );

    SysTick_DelayMs(
        5U
    );


    GPIO_WritePin(
        ST7735S_RST_PORT,
        ST7735S_RST_PIN,
        GPIO_LOW
    );

    SysTick_DelayMs(
        20U
    );


    GPIO_WritePin(
        ST7735S_RST_PORT,
        ST7735S_RST_PIN,
        GPIO_HIGH
    );

    SysTick_DelayMs(
        120U
    );
}


/**
 * @brief Set drawing address window.
 *
 * CS must already be LOW.
 */
static void ST7735S_SetAddressWindowRaw(
    u16 Copy_u16XStart,
    u16 Copy_u16YStart,
    u16 Copy_u16XEnd,
    u16 Copy_u16YEnd
)
{
    u16 Local_u16XStart;
    u16 Local_u16XEnd;

    u16 Local_u16YStart;
    u16 Local_u16YEnd;


    Local_u16XStart =
        Copy_u16XStart +
        ST7735S_X_OFFSET;


    Local_u16XEnd =
        Copy_u16XEnd +
        ST7735S_X_OFFSET;


    Local_u16YStart =
        Copy_u16YStart +
        ST7735S_Y_OFFSET;


    Local_u16YEnd =
        Copy_u16YEnd +
        ST7735S_Y_OFFSET;


    /* -----------------------------------------------------
     * CASET
     * ----------------------------------------------------- */

    ST7735S_WriteCommandRaw(
        ST7735S_CASET
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16XStart >> 8U)
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16XStart & 0x00FFU)
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16XEnd >> 8U)
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16XEnd & 0x00FFU)
    );


    /* -----------------------------------------------------
     * RASET
     * ----------------------------------------------------- */

    ST7735S_WriteCommandRaw(
        ST7735S_RASET
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16YStart >> 8U)
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16YStart & 0x00FFU)
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16YEnd >> 8U)
    );


    ST7735S_WriteDataRaw(
        (u8)(Local_u16YEnd & 0x00FFU)
    );


    /* -----------------------------------------------------
     * RAMWR
     * ----------------------------------------------------- */

    ST7735S_WriteCommandRaw(
        ST7735S_RAMWR
    );
}


/* =========================================================
 * Public Functions
 * ========================================================= */


/**
 * @brief Initialize ST7735S TFT.
 */
void ST7735S_Init(
    void
)
{
    u8 Local_au8Data[16];


    /* =====================================================
     * Enable Peripheral Clocks
     * ===================================================== */

    RCC_EnablePeripheralClock(
        RCC_AHB1,
        RCC_GPIOA
    );


    RCC_EnablePeripheralClock(
        RCC_APB2,
        RCC_SPI1
    );


    /* =====================================================
     * Initialize Control Pins
     * ===================================================== */

    ST7735S_InitOutputPin(
        ST7735S_CS_PORT,
        ST7735S_CS_PIN
    );


    ST7735S_InitOutputPin(
        ST7735S_DC_PORT,
        ST7735S_DC_PIN
    );


    ST7735S_InitOutputPin(
        ST7735S_RST_PORT,
        ST7735S_RST_PIN
    );


    /* =====================================================
     * Initial States
     * ===================================================== */

    GPIO_WritePin(
        ST7735S_CS_PORT,
        ST7735S_CS_PIN,
        GPIO_HIGH
    );


    GPIO_WritePin(
        ST7735S_DC_PORT,
        ST7735S_DC_PIN,
        GPIO_LOW
    );


    GPIO_WritePin(
        ST7735S_RST_PORT,
        ST7735S_RST_PIN,
        GPIO_HIGH
    );


    /* =====================================================
     * Initialize SPI GPIO Pins
     * ===================================================== */

    ST7735S_InitSPIPins();


    /* =====================================================
     * Initialize SPI
     * ===================================================== */

    ST7735S_InitSPI();


    /* =====================================================
     * Hardware Reset
     * ===================================================== */

    ST7735S_HardwareReset();


    /* =====================================================
     * Software Reset
     * ===================================================== */

    ST7735S_SendCommandData(
        ST7735S_SWRESET,
        0,
        0U
    );


    SysTick_DelayMs(
        150U
    );


    /* =====================================================
     * Sleep Out
     * ===================================================== */

    ST7735S_SendCommandData(
        ST7735S_SLPOUT,
        0,
        0U
    );


    SysTick_DelayMs(
        120U
    );


    /* =====================================================
     * Frame Rate Control 1
     * ===================================================== */

    Local_au8Data[0] = 0x01U;
    Local_au8Data[1] = 0x2CU;
    Local_au8Data[2] = 0x2DU;

    ST7735S_SendCommandData(
        ST7735S_FRMCTR1,
        Local_au8Data,
        3U
    );


    /* =====================================================
     * Frame Rate Control 2
     * ===================================================== */

    Local_au8Data[0] = 0x01U;
    Local_au8Data[1] = 0x2CU;
    Local_au8Data[2] = 0x2DU;

    ST7735S_SendCommandData(
        ST7735S_FRMCTR2,
        Local_au8Data,
        3U
    );


    /* =====================================================
     * Frame Rate Control 3
     * ===================================================== */

    Local_au8Data[0] = 0x01U;
    Local_au8Data[1] = 0x2CU;
    Local_au8Data[2] = 0x2DU;
    Local_au8Data[3] = 0x01U;
    Local_au8Data[4] = 0x2CU;
    Local_au8Data[5] = 0x2DU;

    ST7735S_SendCommandData(
        ST7735S_FRMCTR3,
        Local_au8Data,
        6U
    );


    /* =====================================================
     * Display Inversion Control
     * ===================================================== */

    Local_au8Data[0] = 0x07U;

    ST7735S_SendCommandData(
        ST7735S_INVCTR,
        Local_au8Data,
        1U
    );


    /* =====================================================
     * Power Control 1
     * ===================================================== */

    Local_au8Data[0] = 0xA2U;
    Local_au8Data[1] = 0x02U;
    Local_au8Data[2] = 0x84U;

    ST7735S_SendCommandData(
        ST7735S_PWCTR1,
        Local_au8Data,
        3U
    );


    /* =====================================================
     * Power Control 2
     * ===================================================== */

    Local_au8Data[0] = 0xC5U;

    ST7735S_SendCommandData(
        ST7735S_PWCTR2,
        Local_au8Data,
        1U
    );


    /* =====================================================
     * Power Control 3
     * ===================================================== */

    Local_au8Data[0] = 0x0AU;
    Local_au8Data[1] = 0x00U;

    ST7735S_SendCommandData(
        ST7735S_PWCTR3,
        Local_au8Data,
        2U
    );


    /* =====================================================
     * Power Control 4
     * ===================================================== */

    Local_au8Data[0] = 0x8AU;
    Local_au8Data[1] = 0x2AU;

    ST7735S_SendCommandData(
        ST7735S_PWCTR4,
        Local_au8Data,
        2U
    );


    /* =====================================================
     * Power Control 5
     * ===================================================== */

    Local_au8Data[0] = 0x8AU;
    Local_au8Data[1] = 0xEEU;

    ST7735S_SendCommandData(
        ST7735S_PWCTR5,
        Local_au8Data,
        2U
    );


    /* =====================================================
     * VCOM Control
     * ===================================================== */

    Local_au8Data[0] = 0x0EU;

    ST7735S_SendCommandData(
        ST7735S_VMCTR1,
        Local_au8Data,
        1U
    );


    /* =====================================================
     * Display Inversion Off
     * ===================================================== */

    ST7735S_SendCommandData(
        ST7735S_INVOFF,
        0,
        0U
    );


    /* =====================================================
     * MADCTL
     * ===================================================== */

    Local_au8Data[0] =
        ST7735S_DEFAULT_COLOR_ORDER;

    ST7735S_SendCommandData(
        ST7735S_MADCTL,
        Local_au8Data,
        1U
    );


    /* =====================================================
     * RGB565 Color Mode
     * ===================================================== */

    Local_au8Data[0] =
        ST7735S_COLOR_MODE_RGB565;

    ST7735S_SendCommandData(
        ST7735S_COLMOD,
        Local_au8Data,
        1U
    );


    /* =====================================================
     * Positive Gamma
     * ===================================================== */

    Local_au8Data[0]  = 0x02U;
    Local_au8Data[1]  = 0x1CU;
    Local_au8Data[2]  = 0x07U;
    Local_au8Data[3]  = 0x12U;
    Local_au8Data[4]  = 0x37U;
    Local_au8Data[5]  = 0x32U;
    Local_au8Data[6]  = 0x29U;
    Local_au8Data[7]  = 0x2DU;
    Local_au8Data[8]  = 0x29U;
    Local_au8Data[9]  = 0x25U;
    Local_au8Data[10] = 0x2BU;
    Local_au8Data[11] = 0x39U;
    Local_au8Data[12] = 0x00U;
    Local_au8Data[13] = 0x01U;
    Local_au8Data[14] = 0x03U;
    Local_au8Data[15] = 0x10U;

    ST7735S_SendCommandData(
        ST7735S_GMCTRP1,
        Local_au8Data,
        16U
    );


    /* =====================================================
     * Negative Gamma
     * ===================================================== */

    Local_au8Data[0]  = 0x03U;
    Local_au8Data[1]  = 0x1DU;
    Local_au8Data[2]  = 0x07U;
    Local_au8Data[3]  = 0x06U;
    Local_au8Data[4]  = 0x2EU;
    Local_au8Data[5]  = 0x2CU;
    Local_au8Data[6]  = 0x29U;
    Local_au8Data[7]  = 0x2DU;
    Local_au8Data[8]  = 0x2EU;
    Local_au8Data[9]  = 0x2EU;
    Local_au8Data[10] = 0x37U;
    Local_au8Data[11] = 0x3FU;
    Local_au8Data[12] = 0x00U;
    Local_au8Data[13] = 0x00U;
    Local_au8Data[14] = 0x02U;
    Local_au8Data[15] = 0x10U;

    ST7735S_SendCommandData(
        ST7735S_GMCTRN1,
        Local_au8Data,
        16U
    );


    /* =====================================================
     * Normal Display Mode
     * ===================================================== */

    ST7735S_SendCommandData(
        ST7735S_NORON,
        0,
        0U
    );


    SysTick_DelayMs(
        10U
    );


    /* =====================================================
     * Display ON
     * ===================================================== */

    ST7735S_SendCommandData(
        ST7735S_DISPON,
        0,
        0U
    );


    SysTick_DelayMs(
        100U
    );


    /* =====================================================
     * Default Resolution
     * ===================================================== */

    ST7735S_CurrentWidth =
        ST7735S_CONFIG_WIDTH;


    ST7735S_CurrentHeight =
        ST7735S_CONFIG_HEIGHT;


    /* =====================================================
     * Clear Screen
     * ===================================================== */

    ST7735S_FillScreen(
        ST7735S_BLACK
    );
}


/**
 * @brief Turn display ON.
 */
void ST7735S_DisplayOn(
    void
)
{
    ST7735S_SendCommandData(
        ST7735S_DISPON,
        0,
        0U
    );
}


/**
 * @brief Turn display OFF.
 */
void ST7735S_DisplayOff(
    void
)
{
    ST7735S_SendCommandData(
        ST7735S_DISPOFF,
        0,
        0U
    );
}


/**
 * @brief Change screen rotation.
 */
void ST7735S_SetRotation(
    ST7735S_Rotation_t Copy_u8Rotation
)
{
    u8 Local_u8MADCTL;


    switch (Copy_u8Rotation)
    {
        case ST7735S_ROTATION_0:

            Local_u8MADCTL =
                ST7735S_MADCTL_MX |
                ST7735S_MADCTL_MY |
                ST7735S_DEFAULT_COLOR_ORDER;


            ST7735S_CurrentWidth =
                ST7735S_CONFIG_WIDTH;


            ST7735S_CurrentHeight =
                ST7735S_CONFIG_HEIGHT;

            break;


        case ST7735S_ROTATION_90:

            Local_u8MADCTL =
                ST7735S_MADCTL_MY |
                ST7735S_MADCTL_MV |
                ST7735S_DEFAULT_COLOR_ORDER;


            ST7735S_CurrentWidth =
                ST7735S_CONFIG_HEIGHT;


            ST7735S_CurrentHeight =
                ST7735S_CONFIG_WIDTH;

            break;


        case ST7735S_ROTATION_180:

            Local_u8MADCTL =
                ST7735S_DEFAULT_COLOR_ORDER;


            ST7735S_CurrentWidth =
                ST7735S_CONFIG_WIDTH;


            ST7735S_CurrentHeight =
                ST7735S_CONFIG_HEIGHT;

            break;


        case ST7735S_ROTATION_270:

            Local_u8MADCTL =
                ST7735S_MADCTL_MX |
                ST7735S_MADCTL_MV |
                ST7735S_DEFAULT_COLOR_ORDER;


            ST7735S_CurrentWidth =
                ST7735S_CONFIG_HEIGHT;


            ST7735S_CurrentHeight =
                ST7735S_CONFIG_WIDTH;

            break;


        default:

            return;
    }


    ST7735S_SendCommandData(
        ST7735S_MADCTL,
        &Local_u8MADCTL,
        1U
    );
}


/**
 * @brief Draw one pixel.
 */
void ST7735S_DrawPixel(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Color
)
{
    if (
        (Copy_u16X >= ST7735S_CurrentWidth) ||
        (Copy_u16Y >= ST7735S_CurrentHeight)
    )
    {
        return;
    }


    ST7735S_BeginWrite();


    ST7735S_SetAddressWindowRaw(
        Copy_u16X,
        Copy_u16Y,
        Copy_u16X,
        Copy_u16Y
    );


    ST7735S_WriteDataRaw(
        (u8)(Copy_u16Color >> 8U)
    );


    ST7735S_WriteDataRaw(
        (u8)(Copy_u16Color & 0x00FFU)
    );


    ST7735S_EndWrite();
}


/**
 * @brief Fill complete screen.
 */
void ST7735S_FillScreen(
    u16 Copy_u16Color
)
{
    ST7735S_FillRectangle(
        0U,
        0U,
        ST7735S_CurrentWidth,
        ST7735S_CurrentHeight,
        Copy_u16Color
    );
}


/**
 * @brief Draw horizontal line.
 */
void ST7735S_DrawHLine(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Length,
    u16 Copy_u16Color
)
{
    ST7735S_FillRectangle(
        Copy_u16X,
        Copy_u16Y,
        Copy_u16Length,
        1U,
        Copy_u16Color
    );
}


/**
 * @brief Draw vertical line.
 */
void ST7735S_DrawVLine(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Length,
    u16 Copy_u16Color
)
{
    ST7735S_FillRectangle(
        Copy_u16X,
        Copy_u16Y,
        1U,
        Copy_u16Length,
        Copy_u16Color
    );
}


/**
 * @brief Fill rectangle.
 */
void ST7735S_FillRectangle(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    u16 Copy_u16Color
)
{
    u32 Local_u32PixelsNumber;

    u32 Local_u32Index;


    if (
        (Copy_u16Width == 0U) ||
        (Copy_u16Height == 0U)
    )
    {
        return;
    }


    if (
        (Copy_u16X >= ST7735S_CurrentWidth) ||
        (Copy_u16Y >= ST7735S_CurrentHeight)
    )
    {
        return;
    }


    /* Horizontal clipping */

    if (
        Copy_u16Width >
        (ST7735S_CurrentWidth - Copy_u16X)
    )
    {
        Copy_u16Width =
            ST7735S_CurrentWidth -
            Copy_u16X;
    }


    /* Vertical clipping */

    if (
        Copy_u16Height >
        (ST7735S_CurrentHeight - Copy_u16Y)
    )
    {
        Copy_u16Height =
            ST7735S_CurrentHeight -
            Copy_u16Y;
    }


    Local_u32PixelsNumber =
        (u32)Copy_u16Width *
        (u32)Copy_u16Height;


    ST7735S_BeginWrite();


    ST7735S_SetAddressWindowRaw(
        Copy_u16X,
        Copy_u16Y,
        Copy_u16X + Copy_u16Width - 1U,
        Copy_u16Y + Copy_u16Height - 1U
    );


    for (
        Local_u32Index = 0U;
        Local_u32Index < Local_u32PixelsNumber;
        Local_u32Index++
    )
    {
        ST7735S_WriteDataRaw(
            (u8)(Copy_u16Color >> 8U)
        );


        ST7735S_WriteDataRaw(
            (u8)(Copy_u16Color & 0x00FFU)
        );
    }


    ST7735S_EndWrite();
}


/**
 * @brief Draw rectangle outline.
 */
void ST7735S_DrawRectangle(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    u16 Copy_u16Color
)
{
    if (
        (Copy_u16Width == 0U) ||
        (Copy_u16Height == 0U)
    )
    {
        return;
    }


    ST7735S_DrawHLine(
        Copy_u16X,
        Copy_u16Y,
        Copy_u16Width,
        Copy_u16Color
    );


    if (Copy_u16Height > 1U)
    {
        ST7735S_DrawHLine(
            Copy_u16X,
            Copy_u16Y + Copy_u16Height - 1U,
            Copy_u16Width,
            Copy_u16Color
        );
    }


    ST7735S_DrawVLine(
        Copy_u16X,
        Copy_u16Y,
        Copy_u16Height,
        Copy_u16Color
    );


    if (Copy_u16Width > 1U)
    {
        ST7735S_DrawVLine(
            Copy_u16X + Copy_u16Width - 1U,
            Copy_u16Y,
            Copy_u16Height,
            Copy_u16Color
        );
    }
}


/**
 * @brief Draw RGB565 image.
 */
void ST7735S_DrawImageRGB565(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    const u16 *Copy_pu16Image
)
{
    u32 Local_u32PixelsNumber;

    u32 Local_u32Index;

    u16 Local_u16Color;


    if (
        (Copy_pu16Image == 0) ||
        (Copy_u16Width == 0U) ||
        (Copy_u16Height == 0U)
    )
    {
        return;
    }


    if (
        (Copy_u16X >= ST7735S_CurrentWidth) ||
        (Copy_u16Y >= ST7735S_CurrentHeight)
    )
    {
        return;
    }


    /*
     * Image must fit completely inside the screen.
     */

    if (
        Copy_u16Width >
        (ST7735S_CurrentWidth - Copy_u16X)
    )
    {
        return;
    }


    if (
        Copy_u16Height >
        (ST7735S_CurrentHeight - Copy_u16Y)
    )
    {
        return;
    }


    Local_u32PixelsNumber =
        (u32)Copy_u16Width *
        (u32)Copy_u16Height;


    ST7735S_BeginWrite();


    ST7735S_SetAddressWindowRaw(
        Copy_u16X,
        Copy_u16Y,
        Copy_u16X + Copy_u16Width - 1U,
        Copy_u16Y + Copy_u16Height - 1U
    );


    for (
        Local_u32Index = 0U;
        Local_u32Index < Local_u32PixelsNumber;
        Local_u32Index++
    )
    {
        Local_u16Color =
            Copy_pu16Image[Local_u32Index];


        ST7735S_WriteDataRaw(
            (u8)(Local_u16Color >> 8U)
        );


        ST7735S_WriteDataRaw(
            (u8)(Local_u16Color & 0x00FFU)
        );
    }


    ST7735S_EndWrite();
}



/* =========================================================
 * Text Drawing Functions
 * ========================================================= */

void ST7735S_DrawChar(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    char Copy_cCharacter,
    u16 Copy_u16TextColor,
    u16 Copy_u16BackgroundColor
)
{
    uint8_t Local_u8CharacterIndex;
    uint8_t Local_u8Column;
    uint8_t Local_u8Row;
    uint8_t Local_u8CharacterData;

    Local_u8CharacterIndex =
        ST7735S_FontGetIndex(
            Copy_cCharacter
        );

    for (
        Local_u8Column = 0U;
        Local_u8Column < ST7735S_FONT_WIDTH;
        Local_u8Column++
    )
    {
        Local_u8CharacterData =
            ST7735S_Font[
                Local_u8CharacterIndex
            ][
                Local_u8Column
            ];

        for (
            Local_u8Row = 0U;
            Local_u8Row < ST7735S_FONT_HEIGHT;
            Local_u8Row++
        )
        {
            if (
                (
                    Local_u8CharacterData &
                    (1U << Local_u8Row)
                )
                != 0U
            )
            {
                ST7735S_DrawPixel(
                    Copy_u16X + Local_u8Column,
                    Copy_u16Y + Local_u8Row,
                    Copy_u16TextColor
                );
            }
            else
            {
                ST7735S_DrawPixel(
                    Copy_u16X + Local_u8Column,
                    Copy_u16Y + Local_u8Row,
                    Copy_u16BackgroundColor
                );
            }
        }
    }

    for (
        Local_u8Row = 0U;
        Local_u8Row < ST7735S_FONT_HEIGHT;
        Local_u8Row++
    )
    {
        ST7735S_DrawPixel(
            Copy_u16X + ST7735S_FONT_WIDTH,
            Copy_u16Y + Local_u8Row,
            Copy_u16BackgroundColor
        );
    }
}


void ST7735S_DrawString(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    const char *Copy_pcString,
    u16 Copy_u16TextColor,
    u16 Copy_u16BackgroundColor
)
{
    u16 Local_u16XPosition;

    if (Copy_pcString == NULL_PTR)
    {
        return;
    }

    Local_u16XPosition =
        Copy_u16X;

    while (*Copy_pcString != '\0')
    {
        ST7735S_DrawChar(
            Local_u16XPosition,
            Copy_u16Y,
            *Copy_pcString,
            Copy_u16TextColor,
            Copy_u16BackgroundColor
        );

        Local_u16XPosition +=
            (
                ST7735S_FONT_WIDTH +
                1U
            );

        Copy_pcString++;
    }
}

#endif /* ST7735S_PROGRAM_C */
