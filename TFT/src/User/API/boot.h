#ifndef _BOOT_H_
#define _BOOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "variants.h"  // for PORTRAIT_MODE

#define W25QXX_SECTOR_SIZE (0x1000)  // 4096-4K

#ifndef LOGO_MAX_SIZE
  #define LOGO_MAX_SIZE           0x4B000
  #define WORD_UNICODE_SIZE      0x480000
  #define BYTE_ASCII_SIZE          0x1000
  #define LARGE_FONT_SIZE          0x3000
  #define _8X16_FONT_SIZE          0x1000
  #define FLASH_SIGN_SIZE          0x1000  // store status of last font/icon/config update
  #define LANGUAGE_SIZE           0x17600  // Language pack size
  #define STRINGS_STORE_MAX_SIZE   0x1000  // label strings max size
  #define PREHEAT_STORE_MAX_SIZE   0x1000  // preheat setting max size
  #define PRINT_GCODES_MAX_SIZE    0x5000  // start/end/cancel gcodes max size
  #define CUSTOM_GCODE_MAX_SIZE    0x5000  // custom gcodes max size
  #define ICON_MAX_SIZE            0x5000
  #define INFOBOX_MAX_SIZE         0xB000
  #define SMALL_ICON_MAX_SIZE      0x2000
#endif

// address in spiflash W25Qxx
#define LOGO_ADDR             0x0
#define WORD_UNICODE_ADDR     LOGO_MAX_SIZE                                  // unicode (+0x480000 4.5M)
#define BYTE_ASCII_ADDR       (WORD_UNICODE_ADDR + WORD_UNICODE_SIZE)        // ascii (+0x1000 4K)
#define LARGE_FONT_ADDR       (BYTE_ASCII_ADDR + BYTE_ASCII_SIZE)            // Large ascii font
#define _8X16_FONT_ADDR       (LARGE_FONT_ADDR + LARGE_FONT_SIZE)            // 8 x 16 ascii font
//#define BYTE_RESERVE_ADDR     0x710000
#define FLASH_SIGN_ADDR       (_8X16_FONT_ADDR + _8X16_FONT_SIZE)            // for language label strings from language file
#define LANGUAGE_ADDR         (FLASH_SIGN_ADDR + FLASH_SIGN_SIZE)            // for label strings from config file
#define STRINGS_STORE_ADDR    (LANGUAGE_ADDR + LANGUAGE_SIZE)                // for label strings from config file
#define PREHEAT_STORE_ADDR    (STRINGS_STORE_ADDR + STRINGS_STORE_MAX_SIZE)  // for preheat settings from config file
#define PRINT_GCODES_ADDR     (PREHEAT_STORE_ADDR + PREHEAT_STORE_MAX_SIZE)  // for start/end/cancel gcodes from config file
#define CUSTOM_GCODE_ADDR     (PRINT_GCODES_ADDR + PRINT_GCODES_MAX_SIZE)    // for custom gcodes from config file

#define ICON_ADDR(num)        ((num) * ICON_MAX_SIZE + CUSTOM_GCODE_ADDR + CUSTOM_GCODE_MAX_SIZE)
#define INFOBOX_ADDR          (ICON_ADDR(ICON_PREVIEW) + ICON_MAX_SIZE)      // total byte size 0xA7F8
#define SMALL_ICON_START_ADDR (INFOBOX_ADDR + INFOBOX_MAX_SIZE)
#define SMALL_ICON_ADDR(num)  ((num) * SMALL_ICON_MAX_SIZE + SMALL_ICON_START_ADDR)
#define FLASH_USED            (INFOBOX_ADDR + INFOBOX_MAX_SIZE)              // currently small icons are not used

#ifdef PORTRAIT_MODE
  #define STR_PORTRAIT STRINGIFY(PORTRAIT_MODE)
#else
  #define STR_PORTRAIT
#endif

#define FIRMWARE_NAME       STRINGIFY(HARDWARE) "." STRINGIFY(SOFTWARE_VERSION) STR_PORTRAIT
#define FIRMWARE_NAME_SHORT STRINGIFY(HARDWARE_SHORT) STRINGIFY(SOFTWARE_VERSION_SHORT)
#define BMP_UPDATE_DIR      UPDATE_DIR "/bmp"
#define FONT_UPDATE_DIR     UPDATE_DIR "/font"
#define TFT_RESET_FILE      "reset.txt"

#define FILE_ADMIN_MODE       "admin.txt"             // to prevent renaming file/folder with `.CUR` after update
#define FILE_ASCII_FONT       "byte_ascii.fon"        // basic ascii font
#define FILE_UNICODE_FONT     "word_unicode.fon"      // unicode font
#define FILE_LARGE_ASCII_FONT "large_byte_ascii.fon"  // large ascii font for displayig values and info
#define FILE_8X16_ASCII_FONT  "8x16_byte_ascii.fon"   // 8x16 ascii font for ST7920 Emilator

// this list is Auto-Generated. Please add new icons in icon_list.inc only
enum
{
  #define X_ICON(NAME) ICON_##NAME ,
    #include "icon_list.inc"
  #undef X_ICON

  // keep the following always at the end of this list
  //ICON_RESERVE,
  ICON_PREVIEW,  // preview should be in the last place before ICON_NULL to save flash storage space
  ICON_NULL      // back ground sign
};

void scanUpdates(void);

#ifdef __cplusplus
}
#endif

#endif
