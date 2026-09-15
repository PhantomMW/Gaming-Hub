#ifndef ST7735S_CONFIG_H
#define ST7735S_CONFIG_H


#include "GPIO_interface.h"
#include "SPI_interface.h"


/* =========================================================
 * SPI Configuration
 * ========================================================= */

#define ST7735S_SPI_ID                 SPI_1


/* =========================================================
 * SPI Pins
 *
 * PA5 -> SPI1_SCK
 * PA7 -> SPI1_MOSI
 *
 * Alternate Function -> AF5
 * ========================================================= */

#define ST7735S_SPI_PORT               GPIO_PORT_A

#define ST7735S_SCK_PIN                GPIO_PIN_5
#define ST7735S_MOSI_PIN               GPIO_PIN_7

#define ST7735S_SPI_AF                 GPIO_AF5


/* =========================================================
 * Control Pins
 *
 * CS  -> PA4
 * A0  -> PA6
 * RST -> PA3
 * ========================================================= */

#define ST7735S_CS_PORT                GPIO_PORT_A
#define ST7735S_CS_PIN                 GPIO_PIN_4


#define ST7735S_DC_PORT                GPIO_PORT_A
#define ST7735S_DC_PIN                 GPIO_PIN_6


#define ST7735S_RST_PORT               GPIO_PORT_A
#define ST7735S_RST_PIN                GPIO_PIN_3


/* =========================================================
 * Screen Resolution
 * ========================================================= */

#define ST7735S_CONFIG_WIDTH           128U
#define ST7735S_CONFIG_HEIGHT          160U


/* =========================================================
 * Memory Offset
 *
 * Standard 1.8" 128x160 configuration.
 * ========================================================= */

#define ST7735S_X_OFFSET               0U
#define ST7735S_Y_OFFSET               0U


/* =========================================================
 * Default Display Color Order
 *
 * BGR is commonly used by this TFT type.
 * ========================================================= */

#define ST7735S_DEFAULT_COLOR_ORDER    ST7735S_MADCTL_BGR


#endif /* ST7735S_CONFIG_H */
