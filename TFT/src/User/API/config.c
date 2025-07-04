#include "config.h"
#include "includes.h"

// after changing/adding/removing a config, change the CONFIG_FLASH_SIGN in Settings.h and PARA_SIGN in FlashStore.c

//-----------------------------General Settings
#define CONFIG_SERIAL_PORT           "serial_port:"
#define CONFIG_TX_SLOTS              "tx_slots:"
#define CONFIG_ADVANCED_OK           "advanced_ok:"
#define CONFIG_COMMAND_CHECKSUM      "command_checksum:"
#define CONFIG_EMULATED_M600         "emulated_m600:"
#define CONFIG_EMULATED_M109_M190    "emulated_m109_m190:"
#define CONFIG_EVENT_LED             "event_led:"
#define CONFIG_FILE_COMMENT_PARSING  "file_comment_parsing:"

//-----------------------------UI Settings
#define CONFIG_LASER_MODE             "laser_mode:"
#define CONFIG_ROTATED_UI            "rotated_ui:"
#define CONFIG_LANGUAGE              "language:"
#define CONFIG_STATUS_SCREEN         "status_screen:"
#define CONFIG_TITLE_BG_COLOR        "title_background_color:"
#define CONFIG_MENU_BG_COLOR         "menu_background_color:"
#define CONFIG_MENU_FONT_COLOR       "menu_font_color:"
#define CONFIG_REMINDER_FONT_COLOR   "reminder_font_color:"
#define CONFIG_STATUS_FONT_COLOR     "status_font_color:"
#define CONFIG_STATUS_XYZ_BG_COLOR   "status_xyz_bg_color:"
#define CONFIG_LIST_BORDER_COLOR     "list_border_color:"
#define CONFIG_LIST_BUTTON_BG_COLOR  "list_button_bg_color:"
#define CONFIG_MESH_MIN_COLOR        "mesh_min_color:"
#define CONFIG_MESH_MAX_COLOR        "mesh_max_color:"
#define CONFIG_TERMINAL_COLOR_SCHEME "terminal_color_scheme:"

#define CONFIG_ACK_NOTIFICATION      "ack_notification:"
#define CONFIG_FILES_SORT_BY         "files_sort_by:"
#define CONFIG_FILES_LIST_MODE       "files_list_mode:"
#define CONFIG_FILENAME_EXTENSION    "filename_extension:"
#define CONFIG_FAN_SPEED_PERCENTAGE  "fan_speed_percentage:"
#define CONFIG_PERSISTENT_INFO       "persistent_info:"
#define CONFIG_TERMINAL_ACK          "terminal_ack:"
#define CONFIG_NOTIFICATION_M117     "notification_m117:"
#define CONFIG_PROG_SOURCE           "prog_source:"
#define CONFIG_PROG_DISP_TYPE        "prog_disp_type:"
#define CONFIG_LAYER_DISP_TYPE       "layer_disp_type:"
//-----------------------------Marlin Mode Settings (only for TFT24 V1.1 & TFT28/TFT35/TFT43/TFT50/TFT70 V3.0)
#define CONFIG_MODE                  "default_mode:"
#define CONFIG_SERIAL_ALWAYS_ON      "serial_always_on:"
#define CONFIG_MARLIN_BG_COLOR       "marlin_background_color:"
#define CONFIG_MARLIN_FONT_COLOR     "marlin_font_color:"
#define CONFIG_MARLIN_FULLSCREEN     "marlin_fullscreen:"
#define CONFIG_MARLIN_SHOW_TITLE     "marlin_show_title:"
#define CONFIG_MARLIN_TITLE          "marlin_title:"
#define CONFIG_MARLIN_TYPE           "marlin_type:"
//-----------------------------Printer / Machine Settings
#define CONFIG_HOTEND_COUNT          "hotend_count:"
#define CONFIG_HEATED_BED            "heated_bed:"
#define CONFIG_HEATED_CHAMBER        "heated_chamber:"
#define CONFIG_EXT_COUNT             "ext_count:"
#define CONFIG_FAN_COUNT             "fan_count:"
#define CONFIG_CONTROLLER_FAN        "controller_fan:"
#define CONFIG_MAX_TEMP              "max_temp:"
#define CONFIG_MIN_TEMP              "min_temp:"
#define CONFIG_FAN_MAX               "fan_max:"
#define CONFIG_SIZE_MIN              "size_min:"
#define CONFIG_SIZE_MAX              "size_max:"
#define CONFIG_XY_SPEED              "xy_speed:"
#define CONFIG_Z_SPEED               "z_speed:"
#define CONFIG_EXT_SPEED             "ext_speed:"
#define CONFIG_AUTO_LOAD_LEVELING    "auto_load_leveling:"
#define CONFIG_ONBOARD_SD            "onboard_sd:"
#define CONFIG_M27_REFRESH_TIME      "M27_refresh_time:"
#define CONFIG_M27_ALWAYS_ACTIVE     "M27_always_active:"
#define CONFIG_LONG_FILENAME         "long_filename:"
#define CONFIG_PAUSE_RETRACT         "pause_retract:"
#define CONFIG_PAUSE_POS             "pause_pos:"
#define CONFIG_PAUSE_Z_RAISE         "pause_z_raise:"
#define CONFIG_PAUSE_FEEDRATE        "pause_feedrate:"
#define CONFIG_LEVEL_EDGE_DISTANCE   "level_edge_distance:"
#define CONFIG_LEVEL_Z_POS           "level_z_pos:"
#define CONFIG_LEVEL_Z_RAISE         "level_z_raise:"
#define CONFIG_LEVEL_FEEDRATE        "level_feedrate:"
#define CONFIG_INVERTED_AXIS         "inverted_axis:"
#define CONFIG_PROBING_Z_OFFSET      "probing_z_offset:"
#define CONFIG_PROBING_Z_RAISE       "probing_z_raise:"
#define CONFIG_Z_STEPPERS_ALIGNMENT  "z_steppers_alignment:"
#define CONFIG_TOUCHMI_SENSOR        "touchmi_sensor:"
#define CONFIG_TOUCHPLATE_ON         "touchplate_on:"
#define CONFIG_TOUCHPLATE_HEIGHT     "touchplate_height:"
#define CONFIG_PREHEAT_NAME_1        "preheat_name_1:"
#define CONFIG_PREHEAT_NAME_2        "preheat_name_2:"
#define CONFIG_PREHEAT_NAME_3        "preheat_name_3:"
#define CONFIG_PREHEAT_NAME_4        "preheat_name_4:"
#define CONFIG_PREHEAT_NAME_5        "preheat_name_5:"
#define CONFIG_PREHEAT_NAME_6        "preheat_name_6:"
#define CONFIG_PREHEAT_TEMP_1        "preheat_temp_1:"
#define CONFIG_PREHEAT_TEMP_2        "preheat_temp_2:"
#define CONFIG_PREHEAT_TEMP_3        "preheat_temp_3:"
#define CONFIG_PREHEAT_TEMP_4        "preheat_temp_4:"
#define CONFIG_PREHEAT_TEMP_5        "preheat_temp_5:"
#define CONFIG_PREHEAT_TEMP_6        "preheat_temp_6:"
//-----------------------------Power Supply Settings (only if connected to TFT controller)
#define CONFIG_PS_ACTIVE_HIGH        "ps_active_high:"
#define CONFIG_PS_AUTO_SHUTDOWN      "ps_auto_shutdown:"
#define CONFIG_PS_AUTO_SHUTDOWN_TEMP "ps_auto_shutdown_temp:"
//-----------------------------Filament Runout Settings (only if connected to TFT controller)
#define CONFIG_FIL_RUNOUT            "fil_runout:"
#define CONFIG_FIL_RUNOUT_INVERTED   "fil_runout_inverted:"
#define CONFIG_FIL_RUNOUT_NC         "fil_runout_nc:"
#define CONFIG_FIL_RUNOUT_NOISE      "fil_runout_noise_threshold:"
#define CONFIG_FIL_RUNOUT_DISTANCE   "fil_runout_distance:"
//-----------------------------Power Loss Recovery & BTT UPS Settings
#define CONFIG_PL_RECOVERY           "pl_recovery:"
#define CONFIG_PL_RECOVERY_HOME      "pl_recovery_home:"
#define CONFIG_PL_RECOVERY_Z_RAISE   "pl_recovery_z_raise:"
#define CONFIG_BTT_MINI_UPS          "btt_mini_ups:"
//-----------------------------Other Device-Specific Settings
#define CONFIG_TOUCH_SOUND           "touch_sound:"
#define CONFIG_TOAST_SOUND           "toast_sound:"
#define CONFIG_ALERT_SOUND           "alert_sound:"
#define CONFIG_HEATER_SOUND          "heater_sound:"
#define CONFIG_LCD_BRIGHTNESS        "lcd_brightness:"
#define CONFIG_LCD_IDLE_BRIGHTNESS   "lcd_idle_brightness:"
#define CONFIG_LCD_IDLE_TIME         "lcd_idle_time:"
#define CONFIG_LCD_LOCK_ON_IDLE      "lcd_lock_on_idle:"
#define CONFIG_LED_COLOR             "led_color:"
#define CONFIG_LED_ALWAYS_ON         "led_always_on:"
#define CONFIG_KNOB_LED_COLOR        "knob_led_color:"
#define CONFIG_KNOB_LED_IDLE         "knob_led_idle:"
#define CONFIG_NEOPIXEL_PIXELS       "neopixel_pixels:"
//-----------------------------Custom G-code Commands
#define CONFIG_CUSTOM_LABEL_1        "custom_label_1:"
#define CONFIG_CUSTOM_LABEL_2        "custom_label_2:"
#define CONFIG_CUSTOM_LABEL_3        "custom_label_3:"
#define CONFIG_CUSTOM_LABEL_4        "custom_label_4:"
#define CONFIG_CUSTOM_LABEL_5        "custom_label_5:"
#define CONFIG_CUSTOM_LABEL_6        "custom_label_6:"
#define CONFIG_CUSTOM_LABEL_7        "custom_label_7:"
#define CONFIG_CUSTOM_LABEL_8        "custom_label_8:"
#define CONFIG_CUSTOM_LABEL_9        "custom_label_9:"
#define CONFIG_CUSTOM_LABEL_10       "custom_label_10:"
#define CONFIG_CUSTOM_LABEL_11       "custom_label_11:"
#define CONFIG_CUSTOM_LABEL_12       "custom_label_12:"
#define CONFIG_CUSTOM_LABEL_13       "custom_label_13:"
#define CONFIG_CUSTOM_LABEL_14       "custom_label_14:"
#define CONFIG_CUSTOM_LABEL_15       "custom_label_15:"
#define CONFIG_CUSTOM_GCODE_1        "custom_gcode_1:"
#define CONFIG_CUSTOM_GCODE_2        "custom_gcode_2:"
#define CONFIG_CUSTOM_GCODE_3        "custom_gcode_3:"
#define CONFIG_CUSTOM_GCODE_4        "custom_gcode_4:"
#define CONFIG_CUSTOM_GCODE_5        "custom_gcode_5:"
#define CONFIG_CUSTOM_GCODE_6        "custom_gcode_6:"
#define CONFIG_CUSTOM_GCODE_7        "custom_gcode_7:"
#define CONFIG_CUSTOM_GCODE_8        "custom_gcode_8:"
#define CONFIG_CUSTOM_GCODE_9        "custom_gcode_9:"
#define CONFIG_CUSTOM_GCODE_10       "custom_gcode_10:"
#define CONFIG_CUSTOM_GCODE_11       "custom_gcode_11:"
#define CONFIG_CUSTOM_GCODE_12       "custom_gcode_12:"
#define CONFIG_CUSTOM_GCODE_13       "custom_gcode_13:"
#define CONFIG_CUSTOM_GCODE_14       "custom_gcode_14:"
#define CONFIG_CUSTOM_GCODE_15       "custom_gcode_15:"
//-----------------------------Start, End & Cancel G-code Commands
#define CONFIG_START_GCODE_ENABLED   "start_gcode_enabled:"
#define CONFIG_END_GCODE_ENABLED     "end_gcode_enabled:"
#define CONFIG_CANCEL_GCODE_ENABLED  "cancel_gcode_enabled:"
#define CONFIG_START_GCODE           "start_gcode:"
#define CONFIG_END_GCODE             "end_gcode:"
#define CONFIG_CANCEL_GCODE          "cancel_gcode:"
#define CONFIG_JOG                   "jog:"
// this list is Auto-Generated. Please add new config in config.inc only
enum
{
  #define X_CONFIG(NAME) C_INDEX_##NAME ,
    #include "config.inc"
  #undef X_CONFIG

  CONFIG_COUNT,
};

// this list is Auto-Generated. Please add new config in config.inc only
static const char * const config_keywords[CONFIG_COUNT] = {
  #define X_CONFIG(NAME) CONFIG_##NAME ,
    #include "config.inc"
  #undef X_CONFIG
};

#if defined(SERIAL_DEBUG_PORT) && defined(DEBUG_SERIAL_CONFIG)  // to be used only when calling 'getConfigFromFile()' after boot process
  #define PRINTDEBUG(x) Serial_Put(SERIAL_DEBUG_PORT, x);
#else
  #define PRINTDEBUG(x)
#endif

#define SET_VALID_INT_VALUE(VARIABLE, MIN, MAX)   VARIABLE = valid_intValue(MIN, MAX, VARIABLE)
#define SET_VALID_FLOAT_VALUE(VARIABLE, MIN, MAX) VARIABLE = valid_floatValue(MIN, MAX, VARIABLE)

typedef struct
{
  FIL file;
  uint32_t size;  // total size of config file
  uint32_t cur;   // total size of
} CONFIGFILE;

typedef enum
{
  CSTAT_FILE_NOTOPEN = 0,
  CSTAT_FILE_INVALID,
  CSTAT_UNKNOWN_KEYWORD,
  CSTAT_INVALID_VALUE,
  CSTAT_OLD_VERSION,
  CSTAT_STORAGE_LOW,
  CSTAT_SPI_WRITE_FAIL,
} CONFIG_STATS;

static const GUI_RECT  rectTitleline     = {0,               BYTE_HEIGHT+4,      LCD_WIDTH,                BYTE_HEIGHT+6};
static const GUI_RECT  recterror         = {BYTE_WIDTH/2-2,  BYTE_HEIGHT*2+2,    LCD_WIDTH-BYTE_WIDTH/2+2, LCD_HEIGHT-(BYTE_HEIGHT*4)-4};
static const GUI_RECT  recterrortxt      = {BYTE_WIDTH/2,    BYTE_HEIGHT*2+4,    LCD_WIDTH-BYTE_WIDTH/2,   LCD_HEIGHT-(BYTE_HEIGHT*4)-6};
static const GUI_RECT  rectProgressframe = {BYTE_WIDTH/2-2, LCD_HEIGHT-(BYTE_HEIGHT*2+BYTE_HEIGHT/2), LCD_WIDTH-BYTE_WIDTH/2+2,LCD_HEIGHT-BYTE_HEIGHT/2};
static const GUI_POINT pointProgressText = {BYTE_WIDTH/2-2, LCD_HEIGHT-(BYTE_HEIGHT*4)};

static const char * const preheatNames[] = PREHEAT_LABELS;
static const uint16_t preheatHotend[]    = PREHEAT_HOTEND;
static const uint16_t preheatBed[]       = PREHEAT_BED;
static const uint8_t cgEnabled[]         = CUSTOM_GCODE_ENABLED;
static const char * const cgNames[]      = CUSTOM_GCODE_LABELS;
static const char * const cgList[]       = CUSTOM_GCODE_LIST;

static CONFIGFILE * CurConfigFile;
static STRINGS_STORE * configStringsStore = NULL;
static PREHEAT_STORE * configPreheatStore = NULL;
static PRINT_GCODES * configPrintGcodes   = NULL;
static CUSTOM_GCODES * configCustomGcodes = NULL;

static char * cur_line = NULL;
static uint16_t c_index = 0;
static uint16_t foundkeys = 0;
static uint8_t customcode_index = 0;
static uint8_t customcode_good[CUSTOM_GCODES_COUNT];
static bool scheduleRotate = false;

static void drawProgressPage(uint8_t * title)
{
  GUI_Clear(BLACK);
  GUI_DispString(2, 2, title);
  GUI_FillRectColor(rectTitleline.x0, rectTitleline.y0, rectTitleline.x1, rectTitleline.y1, BLUE);
  GUI_DrawPrect(&rectProgressframe);
}

static void drawProgress(void)
{
  char tempstr[50];

  sprintf(tempstr, "Total keywords found: %d", foundkeys);
  GUI_DispString(pointProgressText.x, pointProgressText.y, (uint8_t *)tempstr);

  uint16_t p = map(CurConfigFile->cur, 0, CurConfigFile->size, rectProgressframe.x0, rectProgressframe.x1);

  GUI_FillRect(rectProgressframe.x0, rectProgressframe.y0, p, rectProgressframe.y1);
}

static void showError(CONFIG_STATS stat)
{
  char * ttl;
  char * txt;
  char tempstr[50];

  GUI_SetBkColor(BLACK);
  GUI_SetColor(RED);
  GUI_ClearPrect(&recterrortxt);
  GUI_DrawPrect(&recterror);

  switch (stat)
  {
    case CSTAT_INVALID_VALUE:
      GUI_SetColor(ORANGE);
      ttl = "Invalid Value(s) in:";
      txt = cur_line;
      break;

    case CSTAT_UNKNOWN_KEYWORD:
      ttl = "Invalid Keyword:";
      txt = cur_line;
      break;

    case CSTAT_FILE_NOTOPEN:
      ttl = "Error:";
      sprintf(tempstr, "Unable to open %s", CONFIG_FILE_PATH);
      txt = tempstr;
      break;

    case CSTAT_STORAGE_LOW:
      ttl = "Write Error:";
      txt = "Config size is larger than allocated size";
      break;

    case CSTAT_FILE_INVALID:
    default:
      ttl = "Error:";
      txt = "Invalid config File";
      break;
  }

  GUI_DispString(recterrortxt.x0, recterrortxt.y0, (uint8_t *)ttl);
  GUI_DispStringInRect(recterrortxt.x0, recterrortxt.y0 + (BYTE_HEIGHT * 2), recterrortxt.x1, recterrortxt.y1, (uint8_t *)txt);
  GUI_SetColor(WHITE);
  Delay_ms(5000);
}

// check if config keyword exits in the buffer line
static bool key_seen(const char * keyStr)
{
  uint16_t i;

  for (c_index = 0, i = 0; c_index < LINE_MAX_CHAR && cur_line[c_index] != '\0'; c_index++, i = 0)
  {
    while (cur_line[c_index + i] == keyStr[i])
    {
      if (keyStr[++i] == '\0')
      {
        c_index += i;

        return true;
      }
    }
  }

  return false;
}

// check if config keyword exits with a full matching in the buffer line
static inline bool param_seen(const char * keyStr)
{
  bool found = key_seen(keyStr);

  if (found)
  {
    size_t index = c_index - strlen(keyStr);

    if (index > 0)
    {
      if (cur_line[index - 1] != ' ')  // if keyStr is not fully matching (it is a substring of another key))
        return false;
    }
  }

  return found;
}

// check if the value is within min and max limits
static bool inLimit(int val, int min, int max)
{
  if (val < min || val > max)
  {
    showError(CSTAT_INVALID_VALUE);

    return false;
  }
  else
  {
    return true;
  }
}

// get the int after config keyword
static inline int config_int(void)
{
  return (strtol(&cur_line[c_index], NULL, 10));
}

// get valid int value or old value
static inline int valid_intValue(int min, int max, int defaultVal)
{
  if (inLimit(config_int(), min, max))
    return config_int();
  else
    return defaultVal;
}

// treat all values other than 0 as 1
static int8_t getOnOff(void)
{
  if (config_int() == 0)
    return 0;
  else
    return 1;
}

// get the float after config keyword
static inline float config_float(void)
{
  return (strtod(&cur_line[c_index], NULL));
}

// get valid float value or old value
static inline float valid_floatValue(float min, float max, float defaultVal)
{
  if (inLimit(config_float(), min, max))
    return config_float();
  else
    return defaultVal;
}

// check if value is hex format
static inline bool config_is_hex(void)
{
  return (strstr(&cur_line[c_index], "0x") != NULL);
}

// get the hex after config keyword
static inline uint32_t config_hex(void)
{
  return (strtol(&cur_line[c_index], NULL, 16));
}

// convert RGB888 to RGB565
static inline uint16_t RGB888_to_RGB565(uint32_t rgb888)
{
  uint8_t r = ((rgb888 >> 16) & 0xFF) >> 3;  // R5
  uint8_t g = ((rgb888 >> 8) & 0xFF) >> 2;   // G6
  uint8_t b = (rgb888 & 0xFF) >> 3;          // B5

  return ((r << 11) | (g << 5) | b);
}

static inline void config_set_color(uint16_t * color_src)
{
  if (config_is_hex())
  {
    *color_src = RGB888_to_RGB565(config_hex());
  }
  else if (inLimit(config_int(), 0, LCD_COLOR_COUNT - 1))
  {
    *color_src = lcd_colors[config_int()];
  }
}

// parse the keyword values in the buffer
static void parseConfigKey(uint16_t index)
{
  switch (index)
  {
    //----------------------------General Settings

    case C_INDEX_SERIAL_PORT:
      if (key_seen("P1:")) SET_VALID_INT_VALUE(infoSettings.serial_port[0], 1, BAUDRATE_COUNT - 1);
      if (key_seen("P2:")) SET_VALID_INT_VALUE(infoSettings.serial_port[1], 0, BAUDRATE_COUNT - 1);
      if (key_seen("P3:")) SET_VALID_INT_VALUE(infoSettings.serial_port[2], 0, BAUDRATE_COUNT - 1);
      if (key_seen("P4:")) SET_VALID_INT_VALUE(infoSettings.serial_port[3], 0, BAUDRATE_COUNT - 1);
      break;

    case C_INDEX_TX_SLOTS:
      SET_VALID_INT_VALUE(infoSettings.tx_slots, MIN_TX_SLOTS, MAX_TX_SLOTS);
      break;

    case C_INDEX_ADVANCED_OK:
    case C_INDEX_COMMAND_CHECKSUM:
    case C_INDEX_EMULATED_M600:
    case C_INDEX_EMULATED_M109_M190:
    case C_INDEX_EVENT_LED:
    case C_INDEX_FILE_COMMENT_PARSING:
      SET_BIT_VALUE(infoSettings.general_settings, ((index - C_INDEX_ADVANCED_OK) + INDEX_ADVANCED_OK), getOnOff());
      break;

    //----------------------------UI Settings

    case C_INDEX_ROTATED_UI:
      if (infoSettings.rotated_ui != getOnOff())
      {
        scheduleRotate = true;
        infoSettings.rotated_ui = getOnOff();
      }
      break;

    case C_INDEX_LANGUAGE:
      SET_VALID_INT_VALUE(infoSettings.language, 0, LANGUAGE_NUM - 1);
      break;

    case C_INDEX_STATUS_SCREEN:
      infoSettings.status_screen = getOnOff();
      break;

    case C_INDEX_TITLE_BG_COLOR:
      config_set_color(&infoSettings.title_bg_color);
      break;

    case C_INDEX_MENU_BG_COLOR:
      config_set_color(&infoSettings.bg_color);
      break;

    case C_INDEX_MENU_FONT_COLOR:
      config_set_color(&infoSettings.font_color);
      break;

    case C_INDEX_REMINDER_FONT_COLOR:
      config_set_color(&infoSettings.reminder_color);
      break;

    case C_INDEX_STATUS_FONT_COLOR:
      config_set_color(&infoSettings.status_color);
      break;

    case C_INDEX_STATUS_XYZ_BG_COLOR:
      config_set_color(&infoSettings.status_xyz_bg_color);
      break;

    case C_INDEX_LIST_BORDER_COLOR:
      config_set_color(&infoSettings.list_border_color);
      break;

    case C_INDEX_LIST_BUTTON_BG_COLOR:
      config_set_color(&infoSettings.list_button_bg_color);
      break;

    case C_INDEX_MESH_MIN_COLOR:
      config_set_color(&infoSettings.mesh_min_color);
      break;

    case C_INDEX_MESH_MAX_COLOR:
      config_set_color(&infoSettings.mesh_max_color);
      break;

    case C_INDEX_TERMINAL_COLOR_SCHEME:
      SET_VALID_INT_VALUE(infoSettings.terminal_color_scheme, 0, 2);
      break;

    case C_INDEX_ACK_NOTIFICATION:
      SET_VALID_INT_VALUE(infoSettings.ack_notification, 0, 2);
      break;

    case C_INDEX_FILES_SORT_BY:
      SET_VALID_INT_VALUE(infoSettings.files_sort_by, 0, SORT_BY_COUNT);
      break;

    case C_INDEX_FILES_LIST_MODE:
      infoSettings.files_list_mode = getOnOff();
      break;

    case C_INDEX_FILENAME_EXTENSION:
      infoSettings.filename_extension = getOnOff();
      break;

    case C_INDEX_FAN_SPEED_PERCENTAGE:
      infoSettings.fan_percentage = getOnOff();
      break;

    case C_INDEX_PERSISTENT_INFO:
      infoSettings.persistent_info = getOnOff();
      break;

    case C_INDEX_TERMINAL_ACK:
      infoSettings.terminal_ack = getOnOff();
      break;

    case C_INDEX_NOTIFICATION_M117:
      infoSettings.notification_m117 = getOnOff();
      break;

    case C_INDEX_PROG_SOURCE:
      SET_VALID_INT_VALUE(infoSettings.prog_source, 0, 1);
      break;

    case C_INDEX_PROG_DISP_TYPE:
      SET_VALID_INT_VALUE(infoSettings.prog_disp_type, 0, 2);
      break;

    case C_INDEX_LAYER_DISP_TYPE:
      SET_VALID_INT_VALUE(infoSettings.layer_disp_type, 0, 2);
      break;

    //----------------------------Marlin Mode Settings (only for TFT24 V1.1 & TFT28/TFT35/TFT43/TFT50/TFT70 V3.0)

    #ifdef HAS_EMULATOR

      case C_INDEX_MODE:
        SET_VALID_INT_VALUE(infoSettings.mode, 0, MAX_MODE_COUNT - 1);
        break;

      case C_INDEX_SERIAL_ALWAYS_ON:
        infoSettings.serial_always_on = getOnOff();
        break;

      case C_INDEX_MARLIN_BG_COLOR:
        config_set_color(&infoSettings.marlin_bg_color);
        break;

      case C_INDEX_MARLIN_FONT_COLOR:
        config_set_color(&infoSettings.marlin_font_color);
        break;

      case C_INDEX_MARLIN_FULLSCREEN:
        infoSettings.marlin_fullscreen = getOnOff();
        break;

      case C_INDEX_MARLIN_SHOW_TITLE:
        infoSettings.marlin_show_title = getOnOff();
        break;

      case C_INDEX_MARLIN_TITLE:
      {
        char * pchr = strchr(cur_line, ':') + 1;
        int utf8len = getUTF8Length((uint8_t *)pchr);
        int bytelen = strlen(pchr) + 1;

        if (inLimit(utf8len, NAME_MIN_LENGTH, MAX_STRING_LENGTH) && inLimit(bytelen, NAME_MIN_LENGTH, MAX_GCODE_LENGTH))
          strcpy(configStringsStore->marlin_title, pchr);
        break;
      }

      case C_INDEX_MARLIN_TYPE:
        SET_VALID_INT_VALUE(infoSettings.marlin_type, 0, MODE_TYPE_COUNT - 1);
        break;

    #endif  // ST7920_EMULATOR || LCD2004_EMULATOR

    //----------------------------Printer / Machine Settings

    case C_INDEX_HOTEND_COUNT:
      SET_VALID_INT_VALUE(infoSettings.hotend_count, 0, MAX_HOTEND_COUNT);
      break;

    case C_INDEX_HEATED_BED:
      infoSettings.bed_en = getOnOff();
      break;

    case C_INDEX_HEATED_CHAMBER:
      infoSettings.chamber_en = getOnOff();
      break;

    case C_INDEX_EXT_COUNT:
      SET_VALID_INT_VALUE(infoSettings.ext_count, 0, MAX_EXT_COUNT);
      break;

    case C_INDEX_FAN_COUNT:
      SET_VALID_INT_VALUE(infoSettings.fan_count, 1, MAX_FAN_COUNT);
      break;

    case C_INDEX_CONTROLLER_FAN:
      infoSettings.ctrl_fan_en = getOnOff();
      break;

    case C_INDEX_MAX_TEMP:
      if (key_seen("T0:")) SET_VALID_INT_VALUE(infoSettings.max_temp[NOZZLE0], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("T1:")) SET_VALID_INT_VALUE(infoSettings.max_temp[NOZZLE1], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("T2:")) SET_VALID_INT_VALUE(infoSettings.max_temp[NOZZLE2], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("T3:")) SET_VALID_INT_VALUE(infoSettings.max_temp[NOZZLE3], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("T4:")) SET_VALID_INT_VALUE(infoSettings.max_temp[NOZZLE4], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("T5:")) SET_VALID_INT_VALUE(infoSettings.max_temp[NOZZLE5], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("BED:")) SET_VALID_INT_VALUE(infoSettings.max_temp[BED], MIN_BED_TEMP, MAX_BED_TEMP);
      if (key_seen("CHAMBER:")) SET_VALID_INT_VALUE(infoSettings.max_temp[CHAMBER], MIN_CHAMBER_TEMP, MAX_CHAMBER_TEMP);
      break;

    case C_INDEX_MIN_TEMP:
      SET_VALID_INT_VALUE(infoSettings.min_ext_temp, MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      break;

    case C_INDEX_FAN_MAX:
      if (key_seen("F0:")) SET_VALID_INT_VALUE(infoSettings.fan_max[0], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("F1:")) SET_VALID_INT_VALUE(infoSettings.fan_max[1], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("F2:")) SET_VALID_INT_VALUE(infoSettings.fan_max[2], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("F3:")) SET_VALID_INT_VALUE(infoSettings.fan_max[3], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("F4:")) SET_VALID_INT_VALUE(infoSettings.fan_max[4], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("F5:")) SET_VALID_INT_VALUE(infoSettings.fan_max[5], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("CtA:")) SET_VALID_INT_VALUE(infoSettings.fan_max[6], MIN_FAN_SPEED, MAX_FAN_SPEED);
      if (key_seen("CtI:")) SET_VALID_INT_VALUE(infoSettings.fan_max[7], MIN_FAN_SPEED, MAX_FAN_SPEED);
      break;

    case C_INDEX_SIZE_MIN:
      if (key_seen("X")) SET_VALID_FLOAT_VALUE(infoParameters.MachineMin[X_AXIS], MIN_SIZE_LIMIT, MAX_SIZE_LIMIT);
      if (key_seen("Y")) SET_VALID_FLOAT_VALUE(infoParameters.MachineMin[Y_AXIS], MIN_SIZE_LIMIT, MAX_SIZE_LIMIT);
      if (key_seen("Z")) SET_VALID_FLOAT_VALUE(infoParameters.MachineMin[Z_AXIS], MIN_SIZE_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_SIZE_MAX:
      if (key_seen("X")) SET_VALID_FLOAT_VALUE(infoParameters.MachineMax[X_AXIS], MIN_SIZE_LIMIT, MAX_SIZE_LIMIT);
      if (key_seen("Y")) SET_VALID_FLOAT_VALUE(infoParameters.MachineMax[Y_AXIS], MIN_SIZE_LIMIT, MAX_SIZE_LIMIT);
      if (key_seen("Z")) SET_VALID_FLOAT_VALUE(infoParameters.MachineMax[Z_AXIS], MIN_SIZE_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_XY_SPEED:
      if (key_seen("S")) SET_VALID_INT_VALUE(infoSettings.xy_speed[0], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("N")) SET_VALID_INT_VALUE(infoSettings.xy_speed[1], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("F")) SET_VALID_INT_VALUE(infoSettings.xy_speed[2], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      break;

    case C_INDEX_Z_SPEED:
      if (key_seen("S")) SET_VALID_INT_VALUE(infoSettings.z_speed[0], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("N")) SET_VALID_INT_VALUE(infoSettings.z_speed[1], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("F")) SET_VALID_INT_VALUE(infoSettings.z_speed[2], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      break;

    case C_INDEX_EXT_SPEED:
      if (key_seen("S")) SET_VALID_INT_VALUE(infoSettings.ext_speed[0], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("N")) SET_VALID_INT_VALUE(infoSettings.ext_speed[1], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("F")) SET_VALID_INT_VALUE(infoSettings.ext_speed[2], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      break;

    case C_INDEX_AUTO_LOAD_LEVELING:
      infoSettings.auto_load_leveling = getOnOff();
      break;

    case C_INDEX_ONBOARD_SD:
      SET_VALID_INT_VALUE(infoSettings.onboard_sd, 0, 2);
      break;

    case C_INDEX_M27_REFRESH_TIME:
      SET_VALID_INT_VALUE(infoSettings.m27_refresh_time, MIN_DELAY_SEC, MAX_DELAY_SEC);
      break;

    case C_INDEX_M27_ALWAYS_ACTIVE:
      infoSettings.m27_active = getOnOff();
      break;

    case C_INDEX_LONG_FILENAME:
      SET_VALID_INT_VALUE(infoSettings.long_filename, 0, 2);
      break;

    case C_INDEX_PAUSE_RETRACT:
      if (key_seen("R")) SET_VALID_FLOAT_VALUE(infoSettings.pause_retract_len, MIN_RETRACT_LIMIT, MAX_RETRACT_LIMIT);
      if (key_seen("P")) SET_VALID_FLOAT_VALUE(infoSettings.resume_purge_len, MIN_RETRACT_LIMIT, MAX_RETRACT_LIMIT);
      break;

    case C_INDEX_PAUSE_POS:
      if (key_seen("X")) SET_VALID_FLOAT_VALUE(infoSettings.pause_pos[X_AXIS], MIN_XY_POS_LIMIT, MAX_SIZE_LIMIT);
      if (key_seen("Y")) SET_VALID_FLOAT_VALUE(infoSettings.pause_pos[Y_AXIS], MIN_XY_POS_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_PAUSE_Z_RAISE:
      SET_VALID_FLOAT_VALUE(infoSettings.pause_z_raise, MIN_Z_POS_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_PAUSE_FEEDRATE:
      if (key_seen("XY")) SET_VALID_INT_VALUE(infoSettings.pause_feedrate[FEEDRATE_XY], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("Z")) SET_VALID_INT_VALUE(infoSettings.pause_feedrate[FEEDRATE_Z], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("E")) SET_VALID_INT_VALUE(infoSettings.pause_feedrate[FEEDRATE_E], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      break;

    case C_INDEX_LEVEL_EDGE_DISTANCE:
      SET_VALID_INT_VALUE(infoSettings.level_edge, MIN_Z_POS_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_LEVEL_Z_POS:
      SET_VALID_FLOAT_VALUE(infoSettings.level_z_pos, MIN_Z_POS_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_LEVEL_Z_RAISE:
      SET_VALID_FLOAT_VALUE(infoSettings.level_z_raise, MIN_Z_POS_LIMIT, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_LEVEL_FEEDRATE:
      if (key_seen("XY")) SET_VALID_INT_VALUE(infoSettings.level_feedrate[FEEDRATE_XY], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      if (key_seen("Z")) SET_VALID_INT_VALUE(infoSettings.level_feedrate[FEEDRATE_Z], MIN_SPEED_LIMIT, MAX_SPEED_LIMIT);
      break;

    case C_INDEX_INVERTED_AXIS:
      if (key_seen("X")) SET_BIT_VALUE(infoSettings.inverted_axis, X_AXIS, getOnOff());
      if (key_seen("Y")) SET_BIT_VALUE(infoSettings.inverted_axis, Y_AXIS, getOnOff());
      if (key_seen("Z")) SET_BIT_VALUE(infoSettings.inverted_axis, Z_AXIS, getOnOff());

      // leveling Y axis (E_AXIS -> index for param "inverted_axis LY<x>" in "config.ini")
      if (key_seen("LY")) SET_BIT_VALUE(infoSettings.inverted_axis, E_AXIS, getOnOff());
      break;

    case C_INDEX_PROBING_Z_OFFSET:
      infoSettings.probing_z_offset = getOnOff();
      break;

    case C_INDEX_PROBING_Z_RAISE:
      SET_VALID_FLOAT_VALUE(infoSettings.probing_z_raise, MIN_PROBING_Z_RAISE, MAX_SIZE_LIMIT);
      break;

    case C_INDEX_Z_STEPPERS_ALIGNMENT:
      infoSettings.z_steppers_alignment = getOnOff();
      break;

    case C_INDEX_TOUCHMI_SENSOR:
      infoSettings.touchmi_sensor = getOnOff();
      break;

    case C_INDEX_TOUCHPLATE_HEIGHT:
      SET_VALID_FLOAT_VALUE(infoSettings.touchplate_height, MIN_Z_POS_LIMIT, MAX_SIZE_LIMIT);

    case C_INDEX_PREHEAT_NAME_1:
    case C_INDEX_PREHEAT_NAME_2:
    case C_INDEX_PREHEAT_NAME_3:
    case C_INDEX_PREHEAT_NAME_4:
    case C_INDEX_PREHEAT_NAME_5:
    case C_INDEX_PREHEAT_NAME_6:
    {
      char pchr[LINE_MAX_CHAR];

      strcpy(pchr, strchr(cur_line, ':') + 1);

      int utf8len = getUTF8Length((uint8_t *)pchr);
      int bytelen = strlen(pchr) + 1;

      if (inLimit(utf8len, NAME_MIN_LENGTH, MAX_STRING_LENGTH) && inLimit(bytelen, NAME_MIN_LENGTH, MAX_STRING_LENGTH))
        strcpy(configPreheatStore->preheat_name[index - C_INDEX_PREHEAT_NAME_1], pchr);
      break;
    }

    case C_INDEX_PREHEAT_TEMP_1:
    case C_INDEX_PREHEAT_TEMP_2:
    case C_INDEX_PREHEAT_TEMP_3:
    case C_INDEX_PREHEAT_TEMP_4:
    case C_INDEX_PREHEAT_TEMP_5:
    case C_INDEX_PREHEAT_TEMP_6:
    {
      int val_index = index - C_INDEX_PREHEAT_TEMP_1;

      if (key_seen("T")) SET_VALID_INT_VALUE(configPreheatStore->preheat_hotend[val_index], MIN_TOOL_TEMP, MAX_TOOL_TEMP);
      if (key_seen("B")) SET_VALID_INT_VALUE(configPreheatStore->preheat_bed[val_index], MIN_BED_TEMP, MAX_BED_TEMP);
      break;
    }

    //----------------------------Power Supply Settings (only if connected to TFT controller)

    #ifdef PS_ON_PIN
      case C_INDEX_PS_ACTIVE_HIGH:
        infoSettings.ps_active_high = getOnOff();
        break;

      case C_INDEX_PS_AUTO_SHUTDOWN:
        SET_VALID_INT_VALUE(infoSettings.auto_shutdown, 0, 2);
        break;

      case C_INDEX_PS_AUTO_SHUTDOWN_TEMP:
        SET_VALID_INT_VALUE(infoSettings.auto_shutdown_temp, MIN_TOOL_TEMP, MAX_TOOL_TEMP);
        break;
    #endif

    //----------------------------Filament Runout Settings (only if connected to TFT controller)

    #ifdef FIL_RUNOUT_PIN
      case C_INDEX_FIL_RUNOUT:
        if (inLimit(config_int(), 0, 3))
        {
          SET_BIT_VALUE(infoSettings.runout, RUNOUT_ENABLED, GET_BIT(config_int(), RUNOUT_ENABLED));
          SET_BIT_VALUE(infoSettings.runout, RUNOUT_SENSOR_TYPE, GET_BIT(config_int(), RUNOUT_SENSOR_TYPE));
        }
        break;

      case C_INDEX_FIL_RUNOUT_INVERTED:
      case C_INDEX_FIL_RUNOUT_NC:
        SET_BIT_VALUE(infoSettings.runout, (RUNOUT_INVERTED + (index - C_INDEX_FIL_RUNOUT_INVERTED)), getOnOff());
        break;

      case C_INDEX_FIL_RUNOUT_NOISE:
        SET_VALID_INT_VALUE(infoSettings.runout_noise, MIN_DELAY_MS, MAX_DELAY_MS);
        break;

      case C_INDEX_FIL_RUNOUT_DISTANCE:
        SET_VALID_INT_VALUE(infoSettings.runout_distance, MIN_RUNOUT_DISTANCE, MAX_RUNOUT_DISTANCE);
        break;
    #endif

    //----------------------------Power Loss Recovery & BTT UPS Settings

    #ifdef BTT_MINI_UPS
      case C_INDEX_PL_RECOVERY:
        infoSettings.plr = getOnOff();
        break;

      case C_INDEX_PL_RECOVERY_HOME:
        infoSettings.plr_home = getOnOff();
        break;

      case C_INDEX_PL_RECOVERY_Z_RAISE:
        SET_VALID_FLOAT_VALUE(infoSettings.plr_z_raise, MIN_Z_POS_LIMIT, MAX_SIZE_LIMIT);
        break;

      case C_INDEX_BTT_MINI_UPS:
        infoSettings.btt_ups = getOnOff();
        break;
    #endif

    //----------------------------Other Device-Specific Settings

    #ifdef BUZZER_PIN
      case C_INDEX_TOUCH_SOUND:
      case C_INDEX_TOAST_SOUND:
      case C_INDEX_ALERT_SOUND:
      case C_INDEX_HEATER_SOUND:
        SET_BIT_VALUE(infoSettings.sounds, (index - C_INDEX_TOUCH_SOUND), getOnOff());
        break;
    #endif

    #ifdef LCD_LED_PWM_CHANNEL
      case C_INDEX_LCD_BRIGHTNESS:
        SET_VALID_INT_VALUE(infoSettings.lcd_brightness, 0, LCD_BRIGHTNESS_COUNT - 1);

        if (infoSettings.lcd_brightness == 0)  // if someone set it to 0 set it to 1
          infoSettings.lcd_brightness = 1;
        break;

      case C_INDEX_LCD_IDLE_BRIGHTNESS:
        SET_VALID_INT_VALUE(infoSettings.lcd_idle_brightness, 0, LCD_BRIGHTNESS_COUNT - 1);
        break;

      case C_INDEX_LCD_IDLE_TIME:
        SET_VALID_INT_VALUE(infoSettings.lcd_idle_time, 0, LCD_IDLE_TIME_COUNT - 1);
        break;

      case C_INDEX_LCD_LOCK_ON_IDLE:
        infoSettings.lcd_lock_on_idle = getOnOff();
        break;
    #endif

    case C_INDEX_LED_COLOR:
      if (key_seen("R:")) SET_VALID_INT_VALUE(infoSettings.led_color[0], MIN_LED_COLOR_COMP, MAX_LED_COLOR_COMP);
      if (key_seen("G:")) SET_VALID_INT_VALUE(infoSettings.led_color[1], MIN_LED_COLOR_COMP, MAX_LED_COLOR_COMP);
      if (key_seen("B:")) SET_VALID_INT_VALUE(infoSettings.led_color[2], MIN_LED_COLOR_COMP, MAX_LED_COLOR_COMP);
      if (key_seen("W:")) SET_VALID_INT_VALUE(infoSettings.led_color[3], MIN_LED_COLOR_COMP, MAX_LED_COLOR_COMP);
      if (key_seen("P:")) SET_VALID_INT_VALUE(infoSettings.led_color[4], MIN_LED_COLOR_COMP, MAX_LED_COLOR_COMP);
      if (key_seen("I:")) SET_VALID_INT_VALUE(infoSettings.led_color[5], MIN_LED_COLOR_COMP, MAX_LED_COLOR_COMP);
      break;

    case C_INDEX_LED_ALWAYS_ON:
      infoSettings.led_always_on = getOnOff();
      break;

    #ifdef KNOB_LED_COLOR_PIN
      case C_INDEX_KNOB_LED_COLOR:
        SET_VALID_INT_VALUE(infoSettings.knob_led_color, 0, KNOB_LED_COLOR_COUNT - 1);
        break;

      #ifdef LCD_LED_PWM_CHANNEL
        case C_INDEX_KNOB_LED_IDLE:
          infoSettings.knob_led_idle = getOnOff();
          break;
      #endif

      case C_INDEX_NEOPIXEL_PIXELS:
        SET_VALID_INT_VALUE(infoSettings.neopixel_pixels, 0, MAX_NEOPIXEL_PIXELS);
        infoSettings.neopixel_pixels = (infoSettings.neopixel_pixels == 0) ? NEOPIXEL_PIXELS : infoSettings.neopixel_pixels;
        break;
    #endif

    //----------------------------Custom G-code Commands

    case C_INDEX_CUSTOM_LABEL_1:
    case C_INDEX_CUSTOM_LABEL_2:
    case C_INDEX_CUSTOM_LABEL_3:
    case C_INDEX_CUSTOM_LABEL_4:
    case C_INDEX_CUSTOM_LABEL_5:
    case C_INDEX_CUSTOM_LABEL_6:
    case C_INDEX_CUSTOM_LABEL_7:
    case C_INDEX_CUSTOM_LABEL_8:
    case C_INDEX_CUSTOM_LABEL_9:
    case C_INDEX_CUSTOM_LABEL_10:
    case C_INDEX_CUSTOM_LABEL_11:
    case C_INDEX_CUSTOM_LABEL_12:
    case C_INDEX_CUSTOM_LABEL_13:
    case C_INDEX_CUSTOM_LABEL_14:
    case C_INDEX_CUSTOM_LABEL_15:
    {
      char pchr[LINE_MAX_CHAR];

      strcpy(pchr, strchr(cur_line, ':') + 1);

      int utf8len = getUTF8Length((uint8_t *)pchr);
      int bytelen = strlen(pchr) + 1;

      if (inLimit(utf8len, NAME_MIN_LENGTH, MAX_GCODE_NAME_LENGTH) && inLimit(bytelen, NAME_MIN_LENGTH, MAX_GCODE_LENGTH))
      {
        strcpy(configCustomGcodes->name[customcode_index++], pchr);
        customcode_good[index - C_INDEX_CUSTOM_LABEL_1] = 1;  // set name was found ok
      }
      else
      {
        customcode_good[index - C_INDEX_CUSTOM_LABEL_1] = 0;  // set name was not ok
      }
      break;
    }
    case C_INDEX_CUSTOM_GCODE_1:
    case C_INDEX_CUSTOM_GCODE_2:
    case C_INDEX_CUSTOM_GCODE_3:
    case C_INDEX_CUSTOM_GCODE_4:
    case C_INDEX_CUSTOM_GCODE_5:
    case C_INDEX_CUSTOM_GCODE_6:
    case C_INDEX_CUSTOM_GCODE_7:
    case C_INDEX_CUSTOM_GCODE_8:
    case C_INDEX_CUSTOM_GCODE_9:
    case C_INDEX_CUSTOM_GCODE_10:
    case C_INDEX_CUSTOM_GCODE_11:
    case C_INDEX_CUSTOM_GCODE_12:
    case C_INDEX_CUSTOM_GCODE_13:
    case C_INDEX_CUSTOM_GCODE_14:
    case C_INDEX_CUSTOM_GCODE_15:
    {
      int lineIndex = index - C_INDEX_CUSTOM_GCODE_1;  // actual gcode index in config file
      char pchr[LINE_MAX_CHAR];

      strcpy(pchr, strchr(cur_line, ':') + 1);

      int len = strlen(pchr) + 1;

      // check if gcode length is ok and the name was ok
      if (inLimit(len, GCODE_MIN_LENGTH, MAX_GCODE_LENGTH) && (customcode_good[lineIndex] == 1))
        strcpy(configCustomGcodes->gcode[customcode_index - 1], pchr);
      else if (customcode_good[lineIndex] == 1)  // if name was ok but gcode is not ok then reduce count
        customcode_index--;
      break;
    }

    //----------------------------Start, End & Cancel G-code Commands

    case C_INDEX_START_GCODE_ENABLED:
    case C_INDEX_END_GCODE_ENABLED:
    case C_INDEX_CANCEL_GCODE_ENABLED:
      SET_BIT_VALUE(infoSettings.send_gcodes, (index - C_INDEX_START_GCODE_ENABLED), getOnOff());
      break;
    case C_INDEX_JOG:
      SET_BIT_VALUE(infoSettings.jog, 0, getOnOff());
      break;
    case C_INDEX_START_GCODE:
    {
      char * pchr = strchr(cur_line, ':') + 1;
      int len = strlen(pchr);

      if (inLimit(len, GCODE_MIN_LENGTH, MAX_GCODE_LENGTH))
      {
        strcpy(configPrintGcodes->start_gcode, pchr);
        #ifdef CONFIG_DEBUG
          GUI_DispStringInRect(recterrortxt.x0, recterrortxt.y0 + (BYTE_HEIGHT * 2), recterrortxt.x1, recterrortxt.y1,
                               (uint8_t *)configPrintGcodes->start_gcode);
          Delay_ms(1000);
          Delay_ms(1000);
        #endif
      }
      break;
    }

    case C_INDEX_END_GCODE:
    {
      char * pchr = strchr(cur_line, ':') + 1;
      int len = strlen(pchr);

      if (inLimit(len, GCODE_MIN_LENGTH, MAX_GCODE_LENGTH))
      {
        strcpy(configPrintGcodes->end_gcode, pchr);
        #ifdef CONFIG_DEBUG
          GUI_DispStringInRect(recterrortxt.x0, recterrortxt.y0 + (BYTE_HEIGHT * 2), recterrortxt.x1, recterrortxt.y1,
                               (uint8_t *)configPrintGcodes->end_gcode);
          Delay_ms(1000);
          Delay_ms(1000);
        #endif
      }
      break;
    }

    case C_INDEX_CANCEL_GCODE:
    {
      char * pchr = strchr(cur_line, ':') + 1;
      int len = strlen(pchr);

      if (inLimit(len, GCODE_MIN_LENGTH, MAX_GCODE_LENGTH))
      {
        strcpy(configPrintGcodes->cancel_gcode, pchr);
        #ifdef CONFIG_DEBUG
          GUI_DispStringInRect(recterrortxt.x0, recterrortxt.y0 + (BYTE_HEIGHT * 2), recterrortxt.x1, recterrortxt.y1,
                               (uint8_t *)configPrintGcodes->cancel_gcode);
          Delay_ms(1000);
          Delay_ms(1000);
        #endif
      }
      break;
    }

    default:
      break;
  }
}

// check keywords in the config line in buffer
static void parseConfigLine(void)
{
  for (uint16_t i = 0; i < CONFIG_COUNT; i++)
  {
    if (param_seen(config_keywords[i]))
    {
      PRINTDEBUG("\n");
      PRINTDEBUG((char *)config_keywords[i]);

      parseConfigKey(i);
      foundkeys++;

      return;
    }
  }

  showError(CSTAT_UNKNOWN_KEYWORD);
}

// parse keywords from line read from language file
static void parseLangLine(void)
{
  for (int i = 0; i < LABEL_NUM; i++)
  {
    if (key_seen(lang_key_list[i]))
    {
      PRINTDEBUG("\n");
      PRINTDEBUG((char *)lang_key_list[i]);

      uint32_t key_addr = LANGUAGE_ADDR + (MAX_LANG_LABEL_LENGTH * i);
      uint8_t * pchr = (uint8_t *)strchr(cur_line, ':') + 1;
      int bytelen = strlen((char *)pchr);

      if (inLimit(bytelen, 1, MAX_LANG_LABEL_LENGTH))
      {
        char check[MAX_LANG_LABEL_LENGTH];

        W25Qxx_WritePage(pchr, key_addr, MAX_LANG_LABEL_LENGTH);
        W25Qxx_ReadBuffer((uint8_t *)&check, key_addr, MAX_LANG_LABEL_LENGTH);

        if (strcmp(strchr(cur_line, ':') + 1, check) != 0)
          showError(CSTAT_SPI_WRITE_FAIL);
      }
      else
      {
        showError(CSTAT_INVALID_VALUE);
      }

      foundkeys++;

      return;
    }
  }

  showError(CSTAT_UNKNOWN_KEYWORD);
}

static bool readConfigFile(const char * path, void (* lineParser)(), uint16_t maxLineLen)
{
  bool comment_mode = false;
  bool comment_space = true;
  char cur_char;
  char last_char = 0;
  uint8_t count = 0;
  UINT br = 0;
  CONFIGFILE configFile;
  CurConfigFile = &configFile;

  if (f_file_exists(path) == false)
    return false;

  if (f_open(&configFile.file, path, FA_OPEN_EXISTING | FA_READ) != FR_OK)
  {
    PRINTDEBUG("parse error\n");

    showError(CSTAT_FILE_NOTOPEN);

    return false;
  }
  else
  {
    configFile.size = f_size(&configFile.file);

    if (configFile.size == 0)
    {
      showError(CSTAT_FILE_INVALID);
      f_close(&configFile.file);

      return false;
    }

    configFile.cur = 0;

    for (; configFile.cur < configFile.size;)
    {
      if (f_read(&configFile.file, &cur_char, 1, &br) != FR_OK)
      {
        PRINTDEBUG("read error\n");

        return false;
      }

      configFile.cur++;

      //PRINTDEBUG("Line ++\n");

      if (cur_char == '\n')  // start parsing line after new line
      {
        comment_mode = false;  // for new command
        comment_space = true;

        if (count != 0)
        {
          cur_line[count] = '\0';  // terminate string
          lineParser();
          drawProgress();

          PRINTDEBUG("\n");
          PRINTDEBUG(cur_line);

          count = 0;  // clear buffer
        }
      }
      else if (count < maxLineLen - 2)
      {
        if (cur_char == '#')
        {
          comment_mode = true;
        }
        else
        {
          if (comment_space && cur_char != ' ')  // ignore ' ' space bytes
          {
            comment_space = false;
          }
          if (!comment_mode && !comment_space && cur_char != '\r')  // normal code
          {
            if (cur_char == 'n' && last_char == '\\')  // replace "\n" with new line char('\n')
            {
              cur_char = '\n';
              count--;
            }

            cur_line[count++] = cur_char;
            last_char = cur_char;

            if (configFile.cur == configFile.size)
            {
              cur_line[count] = '\0';  // terminate string

              PRINTDEBUG("line read\n");

              lineParser();  // start parsing at the end of the file
              drawProgress();
            }
          }
        }
      }
    }

    f_close(&configFile.file);
    configFile.cur = 0;
    configFile.size = 0;

    return true;
  }
}

static void writeConfig(uint8_t * dataBytes, uint16_t numBytes, uint32_t addr, uint32_t maxSize)
{
  // do not proceed if data size is larger than reserved max size
  if (numBytes > maxSize)
  {
    PRINTDEBUG("\nwrite error\n");

    showError(CSTAT_STORAGE_LOW);

    return;
  }

  int sectorCount = maxSize / W25QXX_SECTOR_SIZE;

  // erase part of flash to be rewritten
  for (int i = 0; i < sectorCount; i++)
  {
    W25Qxx_EraseSector(addr + (i * W25QXX_SECTOR_SIZE));
  }

  Delay_ms(100);                                  // give time for spi flash to settle
  W25Qxx_WriteBuffer(dataBytes, addr, numBytes);  // write data to spi flash
  Delay_ms(100);                                  // give time for spi flash to settle
}

static void saveConfig(void)
{
  writeConfig((uint8_t *)configStringsStore, sizeof(STRINGS_STORE), STRINGS_STORE_ADDR, STRINGS_STORE_MAX_SIZE);
  writeConfig((uint8_t *)configPreheatStore, sizeof(PREHEAT_STORE), PREHEAT_STORE_ADDR, PREHEAT_STORE_MAX_SIZE);
  writeConfig((uint8_t *)configPrintGcodes, sizeof(PRINT_GCODES), PRINT_GCODES_ADDR, PRINT_GCODES_MAX_SIZE);
  writeConfig((uint8_t *)configCustomGcodes, sizeof(CUSTOM_GCODES), CUSTOM_GCODE_ADDR, CUSTOM_GCODE_MAX_SIZE);

  #ifdef CONFIG_DEBUG
    CUSTOM_GCODES tempgcode;  // = NULL;
    uint8_t * data_r = (uint8_t *)&tempgcode;

    W25Qxx_ReadBuffer(data_r, CUSTOM_GCODE_ADDR, sizeof(CUSTOM_GCODES));

    PRINTDEBUG("\nread done");
    PRINTDEBUG("\nread from flash:");
    PRINTDEBUG(tempgcode.gcode[1]);
  #endif
}

// reset & store config settings
void resetConfig(void)
{
  STRINGS_STORE tempST;
  PREHEAT_STORE tempPH;
  PRINT_GCODES tempPC;
  CUSTOM_GCODES tempCG;

  // restore strings store
  strcpy(tempST.marlin_title, MARLIN_TITLE);

  // restore preheat presets
  for (int i = 0; i < PREHEAT_COUNT; i++)
  {
    strcpy(tempPH.preheat_name[i], preheatNames[i]);
    tempPH.preheat_hotend[i] = preheatHotend[i];
    tempPH.preheat_bed[i] = preheatBed[i];
  }

  // restore print gcodes
  strcpy(tempPC.start_gcode, START_GCODE);
  strcpy(tempPC.end_gcode, END_GCODE);
  strcpy(tempPC.cancel_gcode, CANCEL_GCODE);

  // restore custom gcode presets
  int n = 0;

  for (int i = 0; i < CUSTOM_GCODES_COUNT; i++)
  {
    if (cgEnabled[i] == 1)
    {
      strcpy(tempCG.name[n], cgNames[i]);
      strcpy(tempCG.gcode[n], cgList[i]);

      n++;
    }
  }

  tempCG.count = n;

  // write restored config
  writeConfig((uint8_t *)&tempST, sizeof(STRINGS_STORE), STRINGS_STORE_ADDR, STRINGS_STORE_MAX_SIZE);
  writeConfig((uint8_t *)&tempPH, sizeof(PREHEAT_STORE), PREHEAT_STORE_ADDR, PREHEAT_STORE_MAX_SIZE);
  writeConfig((uint8_t *)&tempPC, sizeof(PRINT_GCODES), PRINT_GCODES_ADDR, PRINT_GCODES_MAX_SIZE);
  writeConfig((uint8_t *)&tempCG, sizeof(CUSTOM_GCODES), CUSTOM_GCODE_ADDR, CUSTOM_GCODE_MAX_SIZE);
}

bool getConfigFromFile(char * configPath)
{
  if (f_file_exists(configPath) == false)
  {
    PRINTDEBUG("configFile not found\n");

    return false;
  }

  // initialize all settings to default values before eventually overwriting them with the values from configuration file.
  // resetConfig() function is also invoked by initSettings() function just to initialize the following 4 data structures
  // into the flash before loading them from the flash
  initSettings();

  STRINGS_STORE tempStringStore;
  PREHEAT_STORE tempPreheatStore;
  PRINT_GCODES tempPrintCodes;
  CUSTOM_GCODES tempCustomGcodes;

  W25Qxx_ReadBuffer((uint8_t *)&tempStringStore, STRINGS_STORE_ADDR, sizeof(STRINGS_STORE));
  W25Qxx_ReadBuffer((uint8_t *)&tempPreheatStore, PREHEAT_STORE_ADDR, sizeof(PREHEAT_STORE));
  W25Qxx_ReadBuffer((uint8_t *)&tempPrintCodes, PRINT_GCODES_ADDR, sizeof(PRINT_GCODES));
  W25Qxx_ReadBuffer((uint8_t *)&tempCustomGcodes, CUSTOM_GCODE_ADDR, sizeof(CUSTOM_GCODES));

  configStringsStore = &tempStringStore;
  configPreheatStore = &tempPreheatStore;
  configPrintGcodes = &tempPrintCodes;
  configCustomGcodes = &tempCustomGcodes;
  customcode_index = 0;
  foundkeys = 0;

  char cur_line_buffer[LINE_MAX_CHAR];

  cur_line = cur_line_buffer;

  drawProgressPage((uint8_t *)"Updating Configuration...");

  if (readConfigFile(configPath, parseConfigLine, LINE_MAX_CHAR))
  {
    // store custom codes count
    configCustomGcodes->count = customcode_index;

    PRINTDEBUG("\nCustom gcode stored at 1:");
    PRINTDEBUG(configCustomGcodes->gcode[1]);

    if (scheduleRotate)
    {
      LCD_RefreshDirection(infoSettings.rotated_ui);
      TS_Calibrate();
    }

    storePara();  // TODO: The touch sign will also be written if the touch calibration data is invalid
    saveConfig();

    PRINTDEBUG("config saved\n");

    return true;
  }
  else
  {
    PRINTDEBUG("configFile save failed\n");

    return false;
  }
}

bool getLangFromFile(char * rootDir)
{
  bool success = false;
  foundkeys = 0;
  DIR d;
  FILINFO f;
  FRESULT r = f_findfirst(&d, &f, rootDir, "language_*.ini");

  f_closedir(&d);

  if (r != FR_OK)
    return false;

  char langpath[256];

  sprintf(langpath, "%s/%s", rootDir, f.fname);

  if (!f_file_exists(langpath))
    return false;

  char cur_line_buffer[MAX_LANG_LABEL_LENGTH + 100];

  cur_line = cur_line_buffer;

  drawProgressPage((uint8_t *)f.fname);

  // erase part of flash to be rewritten
  for (int i = 0; i < (LANGUAGE_SIZE / W25QXX_SECTOR_SIZE); i++)
  {
    W25Qxx_EraseSector(LANGUAGE_ADDR + (i * W25QXX_SECTOR_SIZE));
  }

  success = readConfigFile(langpath, parseLangLine, MAX_LANG_LABEL_LENGTH + 100);

  if (foundkeys != LABEL_NUM)
  {
    showError(CSTAT_FILE_INVALID);

    success = false;
  }
  else  // rename file if update was successful
  {
    if (!f_file_exists(FILE_ADMIN_MODE) && f_file_exists(langpath))  // language exists
    {
      char newlangpath[256];

      sprintf(newlangpath, "%s/%s.CUR", rootDir, f.fname);

      if (f_file_exists(newlangpath))  // old language also exists
        f_unlink(newlangpath);

      f_rename(langpath, newlangpath);
    }
  }

  return success;
}
