/**
 * Config Keywords Generation
 * Usage: X_CONFIG(VALUE)
 * 'VALUE' is the keyword name without the 'CONFIG_'
 *
 * Description: https://en.wikipedia.org/wiki/X_Macro
 *
 * After changing/adding/removing a config, change the CONFIG_FLASH_SIGN in Settings.h and PARA_SIGN in FlashStore.c
 */

//-----------------------------General Settings
X_CONFIG (SERIAL_PORT)
X_CONFIG (TX_SLOTS)
X_CONFIG (ADVANCED_OK)
X_CONFIG (COMMAND_CHECKSUM)
X_CONFIG (EMULATED_M600)
X_CONFIG (EMULATED_M109_M190)
X_CONFIG (EVENT_LED)
X_CONFIG (FILE_COMMENT_PARSING)
//-----------------------------UI Settings
X_CONFIG (LASER_MODE)
X_CONFIG (ROTATED_UI)
X_CONFIG (LANGUAGE)
X_CONFIG (STATUS_SCREEN)
X_CONFIG (TITLE_BG_COLOR)
X_CONFIG (MENU_BG_COLOR)
X_CONFIG (MENU_FONT_COLOR)
X_CONFIG (REMINDER_FONT_COLOR)
X_CONFIG (STATUS_FONT_COLOR)
X_CONFIG (STATUS_XYZ_BG_COLOR)
X_CONFIG (LIST_BORDER_COLOR)
X_CONFIG (LIST_BUTTON_BG_COLOR)
X_CONFIG (MESH_MIN_COLOR)
X_CONFIG (MESH_MAX_COLOR)
X_CONFIG (TERMINAL_COLOR_SCHEME)

X_CONFIG (ACK_NOTIFICATION)
X_CONFIG (FILES_SORT_BY)
X_CONFIG (FILES_LIST_MODE)
X_CONFIG (FILENAME_EXTENSION)
X_CONFIG (FAN_SPEED_PERCENTAGE)
X_CONFIG (PERSISTENT_INFO)
X_CONFIG (TERMINAL_ACK)
X_CONFIG (NOTIFICATION_M117)
X_CONFIG (PROG_SOURCE)
X_CONFIG (PROG_DISP_TYPE)
X_CONFIG (LAYER_DISP_TYPE)
//-----------------------------Marlin Mode Settings (only for TFT24 V1.1 & TFT28/TFT35/TFT43/TFT50/TFT70 V3.0)
X_CONFIG (MODE)
X_CONFIG (SERIAL_ALWAYS_ON)
X_CONFIG (MARLIN_BG_COLOR)
X_CONFIG (MARLIN_FONT_COLOR)
X_CONFIG (MARLIN_FULLSCREEN)
X_CONFIG (MARLIN_SHOW_TITLE)
X_CONFIG (MARLIN_TITLE)
X_CONFIG (MARLIN_TYPE)
//-----------------------------Printer / Machine Settings
X_CONFIG (HOTEND_COUNT)
X_CONFIG (HEATED_BED)
X_CONFIG (HEATED_CHAMBER)
X_CONFIG (EXT_COUNT)
X_CONFIG (FAN_COUNT)
X_CONFIG (CONTROLLER_FAN)
X_CONFIG (MAX_TEMP)
X_CONFIG (MIN_TEMP)
X_CONFIG (FAN_MAX)
X_CONFIG (SIZE_MIN)
X_CONFIG (SIZE_MAX)
X_CONFIG (XY_SPEED)
X_CONFIG (Z_SPEED)
X_CONFIG (EXT_SPEED)
X_CONFIG (AUTO_LOAD_LEVELING)
X_CONFIG (ONBOARD_SD)
X_CONFIG (M27_REFRESH_TIME)
X_CONFIG (M27_ALWAYS_ACTIVE)
X_CONFIG (LONG_FILENAME)
X_CONFIG (PAUSE_RETRACT)
X_CONFIG (PAUSE_POS)
X_CONFIG (PAUSE_Z_RAISE)
X_CONFIG (PAUSE_FEEDRATE)
X_CONFIG (LEVEL_EDGE_DISTANCE)
X_CONFIG (LEVEL_Z_POS)
X_CONFIG (LEVEL_Z_RAISE)
X_CONFIG (LEVEL_FEEDRATE)
X_CONFIG (INVERTED_AXIS)
X_CONFIG (PROBING_Z_OFFSET)
X_CONFIG (PROBING_Z_RAISE)
X_CONFIG (Z_STEPPERS_ALIGNMENT)
X_CONFIG (TOUCHMI_SENSOR)
X_CONFIG (TOUCHPLATE_ON)
X_CONFIG (TOUCHPLATE_HEIGHT)
X_CONFIG (PREHEAT_NAME_1)
X_CONFIG (PREHEAT_NAME_2)
X_CONFIG (PREHEAT_NAME_3)
X_CONFIG (PREHEAT_NAME_4)
X_CONFIG (PREHEAT_NAME_5)
X_CONFIG (PREHEAT_NAME_6)
X_CONFIG (PREHEAT_TEMP_1)
X_CONFIG (PREHEAT_TEMP_2)
X_CONFIG (PREHEAT_TEMP_3)
X_CONFIG (PREHEAT_TEMP_4)
X_CONFIG (PREHEAT_TEMP_5)
X_CONFIG (PREHEAT_TEMP_6)
//-----------------------------Power Supply Settings (only if connected to TFT controller)
X_CONFIG (PS_ACTIVE_HIGH)
X_CONFIG (PS_AUTO_SHUTDOWN)
X_CONFIG (PS_AUTO_SHUTDOWN_TEMP)
//-----------------------------Filament Runout Settings (only if connected to TFT controller)
X_CONFIG (FIL_RUNOUT)
X_CONFIG (FIL_RUNOUT_INVERTED)
X_CONFIG (FIL_RUNOUT_NC)
X_CONFIG (FIL_RUNOUT_NOISE)
X_CONFIG (FIL_RUNOUT_DISTANCE)
//-----------------------------Power Loss Recovery & BTT UPS Settings
X_CONFIG (PL_RECOVERY)
X_CONFIG (PL_RECOVERY_HOME)
X_CONFIG (PL_RECOVERY_Z_RAISE)
X_CONFIG (BTT_MINI_UPS)
//-----------------------------Other Device-Specific Settings
X_CONFIG (TOUCH_SOUND)
X_CONFIG (TOAST_SOUND)
X_CONFIG (ALERT_SOUND)
X_CONFIG (HEATER_SOUND)
X_CONFIG (LCD_BRIGHTNESS)
X_CONFIG (LCD_IDLE_BRIGHTNESS)
X_CONFIG (LCD_IDLE_TIME)
X_CONFIG (LCD_LOCK_ON_IDLE)
X_CONFIG (LED_COLOR)
X_CONFIG (LED_ALWAYS_ON)
X_CONFIG (KNOB_LED_COLOR)
X_CONFIG (KNOB_LED_IDLE)
X_CONFIG (NEOPIXEL_PIXELS)
//-----------------------------Custom G-code Commands
X_CONFIG (CUSTOM_LABEL_1)
X_CONFIG (CUSTOM_LABEL_2)
X_CONFIG (CUSTOM_LABEL_3)
X_CONFIG (CUSTOM_LABEL_4)
X_CONFIG (CUSTOM_LABEL_5)
X_CONFIG (CUSTOM_LABEL_6)
X_CONFIG (CUSTOM_LABEL_7)
X_CONFIG (CUSTOM_LABEL_8)
X_CONFIG (CUSTOM_LABEL_9)
X_CONFIG (CUSTOM_LABEL_10)
X_CONFIG (CUSTOM_LABEL_11)
X_CONFIG (CUSTOM_LABEL_12)
X_CONFIG (CUSTOM_LABEL_13)
X_CONFIG (CUSTOM_LABEL_14)
X_CONFIG (CUSTOM_LABEL_15)
X_CONFIG (CUSTOM_GCODE_1)
X_CONFIG (CUSTOM_GCODE_2)
X_CONFIG (CUSTOM_GCODE_3)
X_CONFIG (CUSTOM_GCODE_4)
X_CONFIG (CUSTOM_GCODE_5)
X_CONFIG (CUSTOM_GCODE_6)
X_CONFIG (CUSTOM_GCODE_7)
X_CONFIG (CUSTOM_GCODE_8)
X_CONFIG (CUSTOM_GCODE_9)
X_CONFIG (CUSTOM_GCODE_10)
X_CONFIG (CUSTOM_GCODE_11)
X_CONFIG (CUSTOM_GCODE_12)
X_CONFIG (CUSTOM_GCODE_13)
X_CONFIG (CUSTOM_GCODE_14)
X_CONFIG (CUSTOM_GCODE_15)
//-----------------------------Start, End & Cancel G-code Commands
X_CONFIG (START_GCODE_ENABLED)
X_CONFIG (END_GCODE_ENABLED)
X_CONFIG (CANCEL_GCODE_ENABLED)
X_CONFIG (JOG)
X_CONFIG (START_GCODE)
X_CONFIG (END_GCODE)
X_CONFIG (CANCEL_GCODE)
// After changing/adding/removing a config, change the CONFIG_FLASH_SIGN in Settings.h and PARA_SIGN in FlashStore.c
