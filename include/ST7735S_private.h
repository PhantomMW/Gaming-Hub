#ifndef ST7735S_PRIVATE_H
#define ST7735S_PRIVATE_H


/* =========================================================
 * ST7735S System Commands
 * ========================================================= */

#define ST7735S_NOP                0x00U
#define ST7735S_SWRESET            0x01U

#define ST7735S_SLPIN              0x10U
#define ST7735S_SLPOUT             0x11U

#define ST7735S_NORON              0x13U

#define ST7735S_INVOFF             0x20U
#define ST7735S_INVON              0x21U

#define ST7735S_DISPOFF            0x28U
#define ST7735S_DISPON             0x29U

#define ST7735S_CASET              0x2AU
#define ST7735S_RASET              0x2BU
#define ST7735S_RAMWR              0x2CU

#define ST7735S_MADCTL             0x36U
#define ST7735S_COLMOD             0x3AU


/* =========================================================
 * ST7735S Panel Commands
 * ========================================================= */

#define ST7735S_FRMCTR1            0xB1U
#define ST7735S_FRMCTR2            0xB2U
#define ST7735S_FRMCTR3            0xB3U

#define ST7735S_INVCTR             0xB4U

#define ST7735S_PWCTR1             0xC0U
#define ST7735S_PWCTR2             0xC1U
#define ST7735S_PWCTR3             0xC2U
#define ST7735S_PWCTR4             0xC3U
#define ST7735S_PWCTR5             0xC4U

#define ST7735S_VMCTR1             0xC5U

#define ST7735S_GMCTRP1            0xE0U
#define ST7735S_GMCTRN1            0xE1U


/* =========================================================
 * MADCTL Bits
 * ========================================================= */

#define ST7735S_MADCTL_MY          0x80U
#define ST7735S_MADCTL_MX          0x40U
#define ST7735S_MADCTL_MV          0x20U
#define ST7735S_MADCTL_ML          0x10U
#define ST7735S_MADCTL_BGR         0x08U
#define ST7735S_MADCTL_MH          0x04U


/* =========================================================
 * Pixel Format
 *
 * 0x05 = 16-bit RGB565
 * ========================================================= */

#define ST7735S_COLOR_MODE_RGB565  0x05U


#endif /* ST7735S_PRIVATE_H */
