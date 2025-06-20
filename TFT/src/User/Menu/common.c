#include "common.h"
#include "includes.h"

#define DUAL_VAL_FORMAT "%4d/%-4d"

// scrolling text line
SCROLL scrollLine;

// indexes for status icon toggles
uint8_t currentTool = NOZZLE0;
uint8_t currentBCIndex = 0;
uint8_t currentFan = 0;
uint8_t currentSpeedID = 0;

// icons list for tool change
const ITEM itemTool[MAX_HEATER_COUNT] = {
// icon                          label
  {ICON_NOZZLE,                  LABEL_NOZZLE},
  {ICON_NOZZLE,                  LABEL_NOZZLE},
  {ICON_NOZZLE,                  LABEL_NOZZLE},
  {ICON_NOZZLE,                  LABEL_NOZZLE},
  {ICON_NOZZLE,                  LABEL_NOZZLE},
  {ICON_NOZZLE,                  LABEL_NOZZLE},
  {ICON_BED,                     LABEL_BED},
  {ICON_CHAMBER,                 LABEL_CHAMBER},
};

// icons list for Temperature step change
const ITEM itemDegreeSteps[ITEM_DEGREE_NUM] = {
// icon                          label
  {ICON_NULL,                LABEL_NULL},
  {ICON_NULL,                LABEL_NULL},
  {ICON_NULL,                LABEL_NULL},
};

// list for temperature step change
const uint8_t degreeSteps[ITEM_DEGREE_NUM] = {1, 5, 10};

// icons list for speed change steps
const ITEM itemSpeed[ITEM_SPEED_NUM] = {
// icon                          label
  {ICON_SLOW_SPEED,              LABEL_SLOW},
  {ICON_NORMAL_SPEED,            LABEL_NORMAL},
  {ICON_FAST_SPEED,              LABEL_FAST},
};

const ITEM itemFanCnt[MAX_FAN_COUNT+1] =
{
  {ICON_FAN,                     LABEL_FAN0},
  {ICON_FAN,                     LABEL_FAN1},
  {ICON_FAN,                     LABEL_FAN2},
  {ICON_FAN,                     LABEL_FAN3},
  {ICON_FAN,                     LABEL_FAN4},
  {ICON_FAN,                     LABEL_FAN5},
  {ICON_FAN,                     LABEL_OFF},
};

// icons list for percent change steps
const ITEM itemPercent[ITEM_PERCENT_STEPS_NUM] = {
// icon                          label
  {ICON_E_1_PERCENT,             LABEL_1_PERCENT},
  {ICON_E_5_PERCENT,             LABEL_5_PERCENT},
  {ICON_E_10_PERCENT,            LABEL_10_PERCENT},
};

// list for percent change steps
const uint8_t percentSteps[ITEM_PERCENT_STEPS_NUM] = {1, 5, 10};

// icons list for axis length/distance change steps
const ITEM itemMoveLen[ITEM_MOVE_LEN_NUM] = {
// icon                          label
  {ICON_100_MM,                  LABEL_100_MM},
  {ICON_10_MM,                   LABEL_10_MM},
  {ICON_1_MM,                    LABEL_1_MM},
  {ICON_01_MM,                   LABEL_01_MM},
  {ICON_001_MM,                  LABEL_001_MM},
};

// list for length/distance change steps
const float moveLenSteps[ITEM_MOVE_LEN_NUM] = {100, 10, 1, 0.1f, 0.01f};

// icons list for Extruder length/distance change steps
// const ITEM itemExtLenSteps[ITEM_EXT_LEN_NUM] = {
// // icon                          label
//   {ICON_E_1_MM,                  LABEL_1_MM},
//   {ICON_E_5_MM,                  LABEL_5_MM},
//   {ICON_E_10_MM,                 LABEL_10_MM},
//   {ICON_E_100_MM,                LABEL_100_MM},
//   {ICON_E_200_MM,                LABEL_200_MM},
// };

// list for extruder length/distance change steps
const float extlenSteps[ITEM_EXT_LEN_NUM] = {1.0f, 5.0f, 10.0f, 100.0f, 200.0f};

// labels list for ON/OFF settings
const LABEL itemToggle[ITEM_TOGGLE_NUM] = {
  LABEL_OFF,
  LABEL_ON
};

const uint16_t iconToggle[ITEM_TOGGLE_NUM] = {
  CHARICON_TOGGLE_OFF,
  CHARICON_TOGGLE_ON
};

// check time elapsed against the time specified in milliseconds for displaying/updating info on screen.
// Use this for timed screen updates in menu loops only
bool nextScreenUpdate(uint32_t refreshTime)
{
  static uint32_t lastTime = 0;

  if (OS_GetTimeMs() - lastTime < refreshTime)
    return false;

  lastTime = OS_GetTimeMs();

  return true;
}

#ifdef FRIENDLY_Z_OFFSET_LANGUAGE

void invertZAxisIcons(MENUITEMS * menuItems)
{
  if (GET_BIT(infoSettings.inverted_axis, Z_AXIS))
  {
    menuItems->items[KEY_ICON_0].icon = ICON_Z_INC;
    menuItems->items[KEY_ICON_0].label.index = LABEL_UP;
    menuItems->items[KEY_ICON_3].icon = ICON_Z_DEC;
    menuItems->items[KEY_ICON_3].label.index = LABEL_DOWN;
  }
}

#endif

void drawBorder(const GUI_RECT * rect, uint16_t color, uint16_t edgeDistance)
{
  //uint16_t origColor = GUI_GetColor();

  GUI_SetColor(color);
  GUI_DrawRect(rect->x0 + edgeDistance, rect->y0 + edgeDistance,
               rect->x1 - edgeDistance, rect->y1 - edgeDistance);

  //GUI_SetColor(origColor);
}

void drawBackground(const GUI_RECT * rect, uint16_t bgColor, uint16_t edgeDistance)
{
  //uint16_t origBgColor = GUI_GetBkColor();

  GUI_SetBkColor(bgColor);
  GUI_ClearRect(rect->x0 + edgeDistance, rect->y0 + edgeDistance,
                rect->x1 - edgeDistance, rect->y1 - edgeDistance);

  //GUI_SetBkColor(origBgColor);
}

void drawStandardValue(const GUI_RECT * rect, VALUE_TYPE valType, const void * val, uint16_t font,
                       uint16_t color, uint16_t bgColor, uint16_t edgeDistance, bool clearBgColor)
{
  uint16_t origColor = GUI_GetColor();
  uint16_t origBgColor = GUI_GetBkColor();

  if (clearBgColor)
    drawBackground(rect, bgColor, edgeDistance);

  if (val != NULL)
  {
    char tempstr[20] = "\0";
    const char * buf = tempstr;

    switch (valType)
    {
      case VALUE_BYTE:
        sprintf(tempstr, "%d", *((uint8_t *) val));
        break;

      case VALUE_INT:
        sprintf(tempstr, "%d", *((uint16_t *) val));
        break;

      case VALUE_FLOAT:
        sprintf(tempstr, "%.3f", *((float *) val));
        break;

      case VALUE_STRING:
        buf = val;
        break;

      default:
        break;
    }

    GUI_SetColor(color);
    GUI_SetBkColor(bgColor);

    setFontSize(font);
    GUI_DispStringInRect(rect->x0 + edgeDistance, rect->y0 + edgeDistance,
                         rect->x1 - edgeDistance, rect->y1 - edgeDistance,
                         (uint8_t *) buf);
    setFontSize(FONT_SIZE_NORMAL);
  }

  GUI_SetColor(origColor);
  GUI_SetBkColor(origBgColor);
}

// show/draw temperature in a standard menu
void temperatureReDraw(uint8_t toolIndex, int16_t * temp, bool drawHeader)
{
  char tempstr[20];

  if (drawHeader)
    displayExhibitHeader(heatDisplayID[toolIndex], "ºC");

  if (temp != NULL)
    sprintf(tempstr, "  %d  ", *temp);
  else
    sprintf(tempstr, DUAL_VAL_FORMAT, heatGetCurrentTemp(toolIndex), heatGetTargetTemp(toolIndex));

  displayExhibitValue(tempstr);
}

// show/draw fan in a standard menu
void fanReDraw(uint8_t fanIndex, bool drawHeader)
{
  char tempstr[20];

  if (drawHeader)
    displayExhibitHeader(fanID[fanIndex], (infoSettings.fan_percentage == 1) ? " % " : "PWM");

  if (infoSettings.fan_percentage == 1)
    sprintf(tempstr, DUAL_VAL_FORMAT, fanGetCurPercent(fanIndex), fanGetSetPercent(fanIndex));
  else
    sprintf(tempstr, DUAL_VAL_FORMAT, fanGetCurSpeed(fanIndex), fanGetSetSpeed(fanIndex));

  displayExhibitValue(tempstr);
}

void laserReDraw(bool drawHeader)
{
  char tempstr[20];

  if (drawHeader)
  {
    displayExhibitHeader("laser", (infoSettings.fan_percentage == 1) ? " % " : "PWM");
  }

  if (infoSettings.fan_percentage == 1)
    sprintf(tempstr, DUAL_VAL_FORMAT, laserGetCurPercent(), laserGetSetPercent());
  else
    sprintf(tempstr, DUAL_VAL_FORMAT, laserGetCurSpeed(), laserGetSetSpeed());

  displayExhibitValue(tempstr);
}

// Show/draw extruder in a standard menu
void extruderReDraw(uint8_t extruderIndex, float extrusion, bool drawHeader)
{
  char tempstr[20];

  if (drawHeader)
    displayExhibitHeader(extruderDisplayID[extruderIndex], "mm");

  sprintf(tempstr, "  %.2f  ", extrusion);

  displayExhibitValue(tempstr);
}

// show/draw percentage in a standard menu
void percentageReDraw(uint8_t itemIndex, bool drawHeader)
{
  char tempstr[20];

  if (drawHeader)
    displayExhibitHeader((char *)textSelect((itemIndex == 0) ? LABEL_PERCENTAGE_SPEED : LABEL_PERCENTAGE_FLOW), "%");

  sprintf(tempstr, DUAL_VAL_FORMAT, speedGetCurPercent(itemIndex), speedGetSetPercent(itemIndex));

  displayExhibitValue(tempstr);
}

static void redrawMenu(MENU_TYPE menuType)
{ // used only when exiting from numpad
  if (menuType == MENU_TYPE_ICON)
    menuDrawPage(getCurMenuItems());
  else if (menuType == MENU_TYPE_LISTVIEW)
    listViewRefreshMenu();
}

// edit an integer value in a standard menu
int32_t editIntValue(int32_t minValue, int32_t maxValue, int32_t resetValue, int32_t value)
{
  int32_t val;
  char tempstr[30];
  MENU_TYPE menuTypeBackup = getMenuType();

  sprintf(tempstr, "Min:%i | Max:%i", minValue, maxValue);
  val = numPadInt((uint8_t *) tempstr, value, resetValue, false);

  redrawMenu(menuTypeBackup);

  return NOBEYOND(minValue, val, maxValue);
}

// edit a float value in a standard menu
float editFloatValue(float minValue, float maxValue, float resetValue, float value)
{
  float val;
  char tempstr[30];
  MENU_TYPE menuTypeBackup = getMenuType();

  sprintf(tempstr, "Min:%.2f | Max:%.2f", minValue, maxValue);
  val = numPadFloat((uint8_t *) tempstr, value, resetValue, true);

  redrawMenu(menuTypeBackup);

  return NOBEYOND(minValue, val, maxValue);
}

// set the hotend to the minimum extrusion temperature if user selected "OK"
static void heatToMinTemp(void)
{
  heatSetTargetTemp(heatGetToolIndex(), infoSettings.min_ext_temp, FROM_GUI);
}

NOZZLE_STATUS warmupNozzle(void)
{
  uint8_t toolIndex = heatGetToolIndex();

  if (heatGetTargetTemp(toolIndex) < infoSettings.min_ext_temp)
  {
    if (heatGetCurrentTemp(toolIndex) < infoSettings.min_ext_temp)
    { // low temperature warning
      char tempMsg[200];
      char tempStr[100];

      sprintf(tempMsg, (char *)textSelect(LABEL_EXT_TEMPLOW), infoSettings.min_ext_temp);
      sprintf(tempStr, (char *)textSelect(LABEL_HEAT_HOTEND), infoSettings.min_ext_temp);
      strcat(tempMsg, "\n");
      strcat(tempMsg, tempStr);

      popupDialog(DIALOG_TYPE_ERROR, LABEL_WARNING, (uint8_t *)tempMsg, LABEL_CONFIRM, LABEL_CANCEL, heatToMinTemp, NULL, NULL);

      return COLD;
    }
    // temperature falling down to a target lower than the minimal extrusion temperature
    else
    { // contiunue with current temp but no lower than the minimum extruder temperature
      heatSetTargetTemp(toolIndex, MAX(infoSettings.min_ext_temp, heatGetCurrentTemp(toolIndex)), FROM_GUI);

      return SETTLING;
    }
  }
  else
  {
    if (heatGetCurrentTemp(toolIndex) < heatGetTargetTemp(toolIndex) - NOZZLE_TEMP_LAG)
    { // low temperature warning
      char tempMsg[200];
      char tempStr[100];

      sprintf(tempMsg, (char *)textSelect(LABEL_DESIRED_TEMPLOW), heatGetTargetTemp(toolIndex));
      sprintf(tempStr, (char *)textSelect(LABEL_WAIT_HEAT_UP));
      strcat(tempMsg, "\n");
      strcat(tempMsg, tempStr);

      popupReminder(DIALOG_TYPE_ERROR, LABEL_WARNING, (uint8_t *)tempMsg);

      return COLD;
    }
  }

  return HEATED;
}

#ifdef SAFETY_ALERT

// user choice for disabling all heaters/hotends
void cooldownTemperature(void)
{
  if (!isPrinting())
  {
    for (uint8_t i = 0; i < MAX_HEATER_COUNT; i++)
    {
      if (heatGetTargetTemp(i) > 0)
      {
        popupDialog(DIALOG_TYPE_QUESTION, LABEL_WARNING, LABEL_HEATERS_ON, LABEL_CONFIRM, LABEL_CANCEL, heatCoolDown, NULL, NULL);

        break;
      }
    }
  }
}

#endif  // SAFETY_ALERT
