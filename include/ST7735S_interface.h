#ifndef ST7735S_INTERFACE_H
#define ST7735S_INTERFACE_H


#include "STD_TYPES.h"


/* =========================================================
 * Display Dimensions
 * ========================================================= */

#define ST7735S_WIDTH              128U
#define ST7735S_HEIGHT             160U


/* =========================================================
 * Basic RGB565 Colors
 * ========================================================= */

#define ST7735S_BLACK              0x0000U
#define ST7735S_WHITE              0xFFFFU
#define ST7735S_RED                0xF800U
#define ST7735S_GREEN              0x07E0U
#define ST7735S_BLUE               0x001FU
#define ST7735S_YELLOW             0xFFE0U
#define ST7735S_CYAN               0x07FFU
#define ST7735S_MAGENTA            0xF81FU
#define ST7735S_ORANGE             0xFD20U
#define ST7735S_GRAY               0x8410U


/* =========================================================
 * Rotation Modes
 * ========================================================= */

typedef enum
{
    ST7735S_ROTATION_0 = 0U,
    ST7735S_ROTATION_90,
    ST7735S_ROTATION_180,
    ST7735S_ROTATION_270

} ST7735S_Rotation_t;


/* =========================================================
 * Initialization
 * ========================================================= */

void ST7735S_Init(void);


/* =========================================================
 * Display Control
 * ========================================================= */

void ST7735S_DisplayOn(void);

void ST7735S_DisplayOff(void);

void ST7735S_SetRotation(
    ST7735S_Rotation_t Copy_u8Rotation
);


/* =========================================================
 * Drawing Functions
 * ========================================================= */

void ST7735S_DrawPixel(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Color
);


void ST7735S_FillScreen(
    u16 Copy_u16Color
);


void ST7735S_DrawHLine(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Length,
    u16 Copy_u16Color
);


void ST7735S_DrawVLine(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Length,
    u16 Copy_u16Color
);


void ST7735S_FillRectangle(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    u16 Copy_u16Color
);


void ST7735S_DrawRectangle(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    u16 Copy_u16Color
);

/* =========================================================
 * Text Drawing Functions
 * ========================================================= */

void ST7735S_DrawChar(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    char Copy_cCharacter,
    u16 Copy_u16TextColor,
    u16 Copy_u16BackgroundColor
);


void ST7735S_DrawString(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    const char *Copy_pcString,
    u16 Copy_u16TextColor,
    u16 Copy_u16BackgroundColor
);

/* =========================================================
 * Image Drawing
 *
 * Image must be stored in RGB565 format.
 * ========================================================= */

void ST7735S_DrawImageRGB565(
    u16 Copy_u16X,
    u16 Copy_u16Y,
    u16 Copy_u16Width,
    u16 Copy_u16Height,
    const u16 *Copy_pu16Image
);


#endif /* ST7735S_INTERFACE_H */
