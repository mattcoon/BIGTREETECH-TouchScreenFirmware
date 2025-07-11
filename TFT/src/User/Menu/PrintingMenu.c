#include "Printing.h"
#include "includes.h"

#define PROGRESS_BAR_RAW_X0   (START_X)                                 // X0 aligned to first icon
#ifdef PORTRAIT_MODE
  #define PROGRESS_BAR_RAW_X1 (START_X + 3 * ICON_WIDTH + 2 * SPACE_X)  // X1 aligned to last icon
#else
  #define PROGRESS_BAR_RAW_X1 (START_X + 4 * ICON_WIDTH + 3 * SPACE_X)  // X1 aligned to last icon
#endif

#ifdef MARKED_PROGRESS_BAR
  #define PROGRESS_BAR_DELTA_X ((PROGRESS_BAR_RAW_X1 - PROGRESS_BAR_RAW_X0) % 10)  // use marked progress bar. Width rounding factor multiple of 10 slices
#else
  #define PROGRESS_BAR_DELTA_X 2                                                   // use standard progress bar. Reserve a 2 pixels width for vertical borders
#endif

// progress bar rounded and aligned to center of icons
#define PROGRESS_BAR_X0          (PROGRESS_BAR_RAW_X0 + PROGRESS_BAR_DELTA_X - PROGRESS_BAR_DELTA_X / 2)
#define PROGRESS_BAR_X1          (PROGRESS_BAR_RAW_X1 - PROGRESS_BAR_DELTA_X / 2)

#define PROGRESS_BAR_FULL_WIDTH  (PROGRESS_BAR_X1 - PROGRESS_BAR_X0)  // 100% progress bar width
#define PROGRESS_BAR_SLICE_WIDTH (PROGRESS_BAR_FULL_WIDTH / 10)       // 10% progress bar width

#ifdef PORTRAIT_MODE
  static const GUI_RECT progressBar = {PROGRESS_BAR_X0, TITLE_END_Y + 1,
                                       PROGRESS_BAR_X1, PS_ICON_START_Y - PS_ICON_SPACE_Y - 1};
#else
  static const GUI_RECT progressBar = {PROGRESS_BAR_X0, PS_ICON_START_Y + PS_ICON_HEIGHT * 2 + PS_ICON_SPACE_Y * 2 + 1,
                                       PROGRESS_BAR_X1, ICON_START_Y + ICON_HEIGHT + SPACE_Y - PS_ICON_SPACE_Y - 1};
#endif

#define TOGGLE_TIME     2000     // 1 seconds is 1000
#define LAYER_DELTA     0.1      // minimal layer height change to update the layer display (avoid congestion in vase mode)
#define LAYER_TITLE     "Layer"
#define MAX_TITLE_LEN   70
#define TIME_FORMAT_STR "%02u:%02u:%02u"

enum
{
  LIVE_INFO_ICON = (1 << 0),
  LIVE_INFO_TOP_ROW = (1 << 1),
  LIVE_INFO_BOTTOM_ROW = (1 << 2),
};

const uint8_t printingIcon[] = {ICON_PRINTING_XPOS, ICON_PRINTING_YPOS,    ICON_PRINTING_ZPOS,
                                ICON_PRINTING_TIMER,  ICON_PRINTING_FAN, ICON_PRINTING_SPEED};

const uint8_t printingIcon2nd[] = {ICON_NULL, ICON_PRINTING_FLOW};

const char * const speedId[2] = {"Speed", "Flow "};
const char * const laserId = "Laser";
#define TOGGLE_TIME     2000     // 1 seconds is 1000
#define LAYER_DELTA     0.1      // minimal layer height change to update the layer display (avoid congestion in vase mode)
#define LAYER_TITLE     "Layer"
#define MAX_TITLE_LEN   70
#define TIME_FORMAT_STR "%02u:%02u:%02u"

PROGRESS_DISPLAY progDisplayType;
LAYER_TYPE layerDisplayType;
char title[MAX_TITLE_LEN];

enum
{
  PRINT_ICON = (1 << 0),
  PRINT_TOP_ROW = (1 << 1),
  PRINT_BOTTOM_ROW = (1 << 2),
};

enum
{
  ICON_POS_X = 0,
  ICON_POS_Y,
  ICON_POS_Z,
  ICON_POS_TIM,
  ICON_POS_FAN,
  ICON_POS_SPD,
};

static const GUI_RECT printingInfo_rect[6] = {
  {PS_ICON_VAL_X, PS_ICON_VAL_Y, PS_ICON_VAL_LG_EX, PS_ICON_VAL_Y + BYTE_HEIGHT},
  {PS_ICON_VAL_X, PS_ICON_VAL_Y, PS_ICON_VAL_LG_EX, PS_ICON_VAL_Y + BYTE_HEIGHT},
  {PS_ICON_VAL_X, PS_ICON_VAL_Y, PS_ICON_VAL_SM_EX, PS_ICON_VAL_Y + BYTE_HEIGHT},
  {PS_ICON_VAL_X, PS_ICON_VAL_Y, PS_ICON_VAL_LG_EX, PS_ICON_VAL_Y + BYTE_HEIGHT},
  {PS_ICON_VAL_X, PS_ICON_VAL_Y, PS_ICON_VAL_LG_EX, PS_ICON_VAL_Y + BYTE_HEIGHT},
  {PS_ICON_VAL_X, PS_ICON_VAL_Y, PS_ICON_VAL_SM_EX, PS_ICON_VAL_Y + BYTE_HEIGHT}
};

static inline void setPauseResumeIcon(MENUITEMS * curmenu, bool paused)
{
  curmenu->items[KEY_ICON_4].icon = paused ? ICON_RESUME : ICON_PAUSE;
  curmenu->items[KEY_ICON_4].label.index = paused ? LABEL_RESUME : LABEL_PAUSE;
}

static void setLayerPosText(char * layer_height_txt, AXIS axis)
{
  float layer_height;
  layer_height = coordinateGetAxis(axis);

    sprintf(layer_height_txt, "%.1f", layer_height);
}


// initialize printing info before opening Printing menu
static void initMenuPrinting(void)
{
  getPrintTitle(title, MAX_TITLE_LEN);  // get print title according to print originator (remote or local to TFT)
  clearInfoFile();                      // as last, clear and free memory for file list

  progDisplayType = infoSettings.prog_disp_type;

  // layer number can be parsed only when TFT reads directly the G-code file
  // so if printing from onboard media or a remote host, display the layer height
  if (WITHIN(infoFile.source, FS_TFT_SD, FS_TFT_USB))
    layerDisplayType = infoSettings.layer_disp_type * 2;
  else
    layerDisplayType = SHOW_LAYER_HEIGHT;

  coordinateSetAxisActual(Z_AXIS, 0);
  coordinateSetAxisTarget(Z_AXIS, 0);
  setTimeFromSlicer(false);
}

// start print originated and/or hosted (handled) by remote host
// (e.g. print started from remote onboard media or hosted by remote host) and open Printing menu
void startPrintingFromRemoteHost(const char * filename)
{
  if (!startPrintFromRemoteHost(filename))
    return;

  // NOTE: call just before opening Printing menu because initMenuPrinting() function will
  //       call clearInfoFile() function that will clear and free memory for file list
  initMenuPrinting();

  infoMenu.cur = 1;  // clear menu buffer when Printing menu is activated by remote
  REPLACE_MENU(menuPrinting);
}

// start print originated and/or hosted (handled) by TFT
// (e.g. print started from onboard media or hosted by TFT) and open Printing menu
void startPrinting(void)
{
  bool printRestore = powerFailedGetRestore();  // temporary save print restore flag before it is cleared by startPrint() function

  if (!startPrint())
  {
    // in case the calling function is menuPrintFromSource(),
    // remove the filename from path to allow the files scanning from its folder avoiding a scanning error message
    exitFolder();

    return;
  }

  // if restoring a print after a power failure or printing from remote TFT media (with M23 - M24),
  // no filename is available in infoFile. Only infoFile.source and infoFile.path have been set
  //
  if (!printRestore && infoFile.fileCount == 0)  // if printing from remote TFT media
    infoMenu.cur = 0;                            // clear menu buffer

  // NOTE: call just before opening Printing menu because initMenuPrinting() function will
  //       call clearInfoFile() function that will clear and free memory for file list
  initMenuPrinting();

  OPEN_MENU(menuPrinting);
}

static void reDrawPrintingValue(uint8_t icon_pos, uint8_t draw_type)
{
  LIVE_INFO lvIcon;
  GUI_RECT const * curRect = &printingInfo_rect[icon_pos];
  char tempstrTop[14];
  char tempstrBottom[14];

  lvIcon.enabled[2] = false;

  lvIcon.iconIndex = printingIcon[icon_pos];

  if (draw_type & LIVE_INFO_TOP_ROW)
  {
    lvIcon.enabled[0] = true;
    lvIcon.lines[0].h_align = LEFT;
    lvIcon.lines[0].v_align = TOP;
    lvIcon.lines[0].pos = (GUI_POINT){.x = PS_ICON_TITLE_X, .y = PS_ICON_TITLE_Y};
    lvIcon.lines[0].font = FONT_SIZE_NORMAL;
    lvIcon.lines[0].fn_color = infoSettings.font_color;
    lvIcon.lines[0].text_mode = GUI_TEXTMODE_TRANS;
    lvIcon.lines[0].text = (uint8_t *)tempstrTop;

    switch (icon_pos)
    {
      case ICON_POS_X:
  	    setLayerPosText(tempstrTop, X_AXIS);
        break;
      
      case ICON_POS_Y:
	      setLayerPosText(tempstrTop, Y_AXIS);
        break;

      case ICON_POS_Z:
	      setLayerPosText(tempstrTop, Z_AXIS);
        break;

      case ICON_POS_TIM:
        if ((getPrintRemainingTime() == 0) || (progDisplayType != ELAPSED_REMAINING))
          snprintf(tempstrTop, 9, "%d%%      ", getPrintProgress());
        else
          time_2_string(tempstrTop, TIME_FORMAT_STR, getPrintTime());
        break;

      case ICON_POS_FAN:
        lvIcon.lines[0].text = currentFan<infoSettings.fan_count?(uint8_t *)fanID[currentFan]:(uint8_t *)laserId;
        break;

      case ICON_POS_SPD:
        lvIcon.lines[0].text = (uint8_t *)speedId[currentSpeedID];
        break;

      default:
        break;
    }
  }
  else
  {
    lvIcon.enabled[0] = false;
  }

  if (draw_type & LIVE_INFO_BOTTOM_ROW)
  {
    lvIcon.enabled[1] = true;
    lvIcon.lines[1].h_align = CENTER;
    lvIcon.lines[1].v_align = CENTER;
    lvIcon.lines[1].pos = (GUI_POINT) {.x = (curRect->x0 + curRect->x1) / 2, .y = (curRect->y0 + curRect->y1) / 2};
    lvIcon.lines[1].font = FONT_SIZE_NORMAL;
    lvIcon.lines[1].fn_color = infoSettings.font_color;
    lvIcon.lines[1].text_mode = GUI_TEXTMODE_TRANS;
    lvIcon.lines[1].text = (uint8_t *)tempstrBottom;

    tempstrBottom[0] = 0;  // always initialize to empty string as default value

    switch (icon_pos)
    {

      case ICON_POS_FAN:
        if (infoSettings.fan_percentage == 1)
          sprintf(tempstrBottom, "%3d%%", currentFan<infoSettings.fan_count? fanGetCurPercent(currentFan): laserGetSetPercent());  // 4 chars
        else
          sprintf(tempstrBottom, "%3d ", currentFan<infoSettings.fan_count? fanGetCurSpeed(currentFan): laserGetSetSpeed());  // 4 chars
        break;

      case ICON_POS_TIM:
        if ((getPrintRemainingTime() == 0) || (progDisplayType == PERCENTAGE_ELAPSED))
          time_2_string(tempstrBottom, TIME_FORMAT_STR, getPrintTime());
        else
          time_2_string(tempstrBottom, TIME_FORMAT_STR, getPrintRemainingTime());
        break;

      case ICON_POS_SPD:
        sprintf(tempstrBottom, "%3d%%", speedGetCurPercent(currentSpeedID));
        break;

      default:
        break;
    }
  }
  else
  {
    lvIcon.enabled[1] = false;
  }

  RAPID_SERIAL_LOOP();  // perform backend printing loop before drawing to avoid printer idling

  showLiveInfo(icon_pos, &lvIcon, draw_type & LIVE_INFO_ICON);
} // reDrawPrintingValue

static inline void toggleInfo(void)
{
  if (nextScreenUpdate(TOGGLE_TIME))
  {
    if ((infoSettings.fan_count + infoSettings.ctrl_fan_en) > 1)
    {
      currentFan = (currentFan + 1) % (MAX_COOLING_FAN_COUNT + 1);

      reDrawPrintingValue(ICON_POS_FAN, LIVE_INFO_TOP_ROW | LIVE_INFO_BOTTOM_ROW);
    }

    TOGGLE_BIT(currentSpeedID, 0);

    reDrawPrintingValue(ICON_POS_SPD, LIVE_INFO_ICON | LIVE_INFO_TOP_ROW | LIVE_INFO_BOTTOM_ROW);

    speedQuery();

      coordinateQuery(MS_TO_SEC(TOGGLE_TIME));

  }
}

static void reDrawProgressBar(uint8_t prevProgress, uint8_t nextProgress, uint16_t barColor, uint16_t sliceColor)
{
  uint16_t start = (PROGRESS_BAR_FULL_WIDTH * prevProgress) / 100;
  uint16_t end = (PROGRESS_BAR_FULL_WIDTH * nextProgress) / 100;

  GUI_FillRectColor(progressBar.x0 + start, progressBar.y0, progressBar.x0 + end, progressBar.y1, barColor);

  #ifdef MARKED_PROGRESS_BAR
    GUI_SetColor(sliceColor);

    start = prevProgress / 10 + 1;  // number of 10% markers + 1 (to skip redraw of 0% and already drawn marker)
    end = nextProgress / 10;        // number of 10% markers

    if (end == 10)  // avoid to draw the marker for 100% progress
      end--;

    for (int i = start; i <= end; i++)
    {
      GUI_VLine(progressBar.x0 + PROGRESS_BAR_SLICE_WIDTH * i - 1, progressBar.y0 + 1, progressBar.y1 - 1);
    }

    GUI_RestoreColorDefault();
  #endif
}

static void reDrawProgress(uint8_t oldProgress)
{
  uint8_t newProgress = getPrintProgress();

  if (newProgress > oldProgress)
    reDrawProgressBar(oldProgress, newProgress, PB_FILL, PB_STRIPE_ELAPSED);
  else  // if it's a regression, swap indexes and colors
    reDrawProgressBar(newProgress, oldProgress, PB_BCKG, PB_STRIPE_REMAINING);

  if (progDisplayType != ELAPSED_REMAINING)
    reDrawPrintingValue(ICON_POS_TIM, LIVE_INFO_TOP_ROW);
}

static inline void drawLiveInfo(void)
{
  for (uint8_t i = 0; i < COUNT(printingIcon); i++)
  {
    reDrawPrintingValue(i, LIVE_INFO_ICON | LIVE_INFO_TOP_ROW | LIVE_INFO_BOTTOM_ROW);
  }

  // progress
  GUI_SetColor(PB_BORDER);
  GUI_DrawRect(progressBar.x0 - 1, progressBar.y0 - 1, progressBar.x1 + 1, progressBar.y1 + 1);  // draw progress bar border
  reDrawProgressBar(0, 100, PB_BCKG, PB_STRIPE_REMAINING);                                       // draw progress bar
  reDrawProgress(0);                                                                             // draw progress
  GUI_RestoreColorDefault();
}

static inline void drawPrintInfo(void)
{
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  IMAGE_ReadDisplay(rect_of_keySS[KEY_INFOBOX].x0, rect_of_keySS[KEY_INFOBOX].y0, INFOBOX_ADDR);

  GUI_SetColor(INFOMSG_BG_COLOR);
  GUI_DispString(rect_of_keySS[KEY_INFOBOX].x0 + STATUS_MSG_ICON_XOFFSET, rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                 IconCharSelect(CHARICON_INFO));
  GUI_DispStringInRectEOL(rect_of_keySS[KEY_INFOBOX].x0 + BYTE_HEIGHT + STATUS_MSG_TITLE_XOFFSET,
                          rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                          rect_of_keySS[KEY_INFOBOX].x1 - STATUS_MSG_TITLE_XOFFSET,
                          rect_of_keySS[KEY_INFOBOX].y1 - STATUS_MSG_ICON_YOFFSET,
                          (uint8_t *)textSelect((isAborted() == true) ? LABEL_PROCESS_ABORTED : LABEL_PRINT_FINISHED));

  GUI_SetColor(INFOMSG_FONT_COLOR);
  GUI_SetBkColor(INFOMSG_BG_COLOR);
  GUI_DispStringInPrect(&msgRect, LABEL_CLICK_FOR_MORE);
  GUI_RestoreColorDefault();
}

void printSummaryPopup(void)
{
  char showInfo[300];
  char tempstr[60];

  time_2_string(showInfo, (char *)textSelect(LABEL_PRINT_TIME), infoPrintSummary.time);

  if (isAborted() == true)
  {
    sprintf(tempstr, "\n\n%s", (char *)textSelect(LABEL_PROCESS_ABORTED));
    strcat(showInfo, tempstr);
  }
  else if (infoPrintSummary.length + infoPrintSummary.weight + infoPrintSummary.cost == 0)  // all equals 0
  {
    strcat(showInfo, (char *)textSelect(LABEL_NO_FILAMENT_STATS));
  }
  else
  {
    if (infoPrintSummary.length > 0)
    {
      sprintf(tempstr, (char *)textSelect(LABEL_FILAMENT_LENGTH), infoPrintSummary.length);
      strcat(showInfo, tempstr);
    }

    if (infoPrintSummary.weight > 0)
    {
      sprintf(tempstr, (char *)textSelect(LABEL_FILAMENT_WEIGHT), infoPrintSummary.weight);
      strcat(showInfo, tempstr);
    }

    if (infoPrintSummary.cost > 0)
    {
      sprintf(tempstr, (char *)textSelect(LABEL_FILAMENT_COST), infoPrintSummary.cost);
      strcat(showInfo, tempstr);
    }
  }

  popupReminder(DIALOG_TYPE_INFO, (uint8_t *)infoPrintSummary.name, (uint8_t *)showInfo);
}

void menuPrinting(void)
{
  MENUITEMS printingItems = {
    // title
    LABEL_NULL,
    // icon                          label
    {
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_BABYSTEP},
      {ICON_MORE,                    LABEL_MORE},
      {ICON_STOP,                    LABEL_STOP},
    }
  };

  uint8_t nowFan[MAX_FAN_COUNT] = {0};
  uint8_t nowLaser = 0;
  float   nowPos[AXIS_NUM] = {0,0,0};
  float   oldPos[AXIS_NUM] = {0,0,0};
  uint8_t oldProgress = 0;
  uint16_t curspeed[2] = {0};
  uint32_t time = 0;
  HEATER nowHeat;
  float curLayerHeight = 0;
  float usedLayerHeight = 0;
  float prevLayerHeight = 0;
  uint16_t curLayerNumber = 0;
  uint16_t prevLayerNumber = 0;
  bool layerDrawEnabled = false;
  bool lastPause = isPaused();
  bool lastPrinting = isPrinting();

  memset(&nowHeat, 0, sizeof(HEATER));

  if (lastPrinting == true)
  {
    setPauseResumeIcon(&printingItems, lastPause);
    printingItems.items[KEY_ICON_5].icon = (infoFile.source < FS_ONBOARD_MEDIA && isPrintModelIcon()) ? ICON_PREVIEW : ICON_BABYSTEP;
  }
  else  // returned to this menu after print was done or aborted
  {
    // turn off fan if print complete or canceled
    fanSetSpeed(FAN_TYPE_F, 0);
    laserReset();

    // Main Screen
    printingItems.items[KEY_ICON_4].icon = ICON_MAINMENU;
    printingItems.items[KEY_ICON_4].label.index = LABEL_MAIN_SCREEN;
    // Background
    printingItems.items[KEY_ICON_5].icon = ICON_NULL;
    printingItems.items[KEY_ICON_5].label.index = LABEL_NULL;
    printingItems.items[KEY_ICON_6].icon = ICON_NULL;
    printingItems.items[KEY_ICON_6].label.index = LABEL_NULL;
    // Back
    printingItems.items[KEY_ICON_7].icon = ICON_BACK;
    printingItems.items[KEY_ICON_7].label.index = LABEL_BACK;
  }

  printingItems.title.address = title;

  menuDrawPage(&printingItems);
  drawLiveInfo();

  #ifndef PORTRAIT_MODE
    if (lastPrinting == false)
      drawPrintInfo();
  #endif

  while (MENU_IS(menuPrinting))
  {
    //Scroll_DispString(&titleScroll, LEFT);  // scroll display file name will take too many CPU cycles

      nowPos[X_AXIS] = coordinateGetAxis(X_AXIS);
      nowPos[Y_AXIS] = coordinateGetAxis(Y_AXIS);
      nowPos[Z_AXIS] = coordinateGetAxis(Z_AXIS);
    if (oldPos[X_AXIS] != nowPos[X_AXIS])
    {
      oldPos[X_AXIS] = nowPos[X_AXIS];
      reDrawPrintingValue(ICON_POS_X, LIVE_INFO_TOP_ROW);
    }
    if (oldPos[Y_AXIS] != nowPos[Y_AXIS])
    {
      oldPos[Y_AXIS] = nowPos[Y_AXIS];
      reDrawPrintingValue(ICON_POS_Y, LIVE_INFO_TOP_ROW);
    }
    if (oldPos[Z_AXIS] != nowPos[Z_AXIS])
    {
      oldPos[Z_AXIS] = nowPos[Z_AXIS];
      reDrawPrintingValue(ICON_POS_Z, LIVE_INFO_TOP_ROW);
    }

    // check fan speed change
    if (currentFan<infoSettings.fan_count) {
      if (nowFan[currentFan] != fanGetCurSpeed(currentFan))
      {
        nowFan[currentFan] = fanGetCurSpeed(currentFan);
        reDrawPrintingValue(ICON_POS_FAN, LIVE_INFO_BOTTOM_ROW);
      }
    } 
    else {
      if (nowLaser != laserGetCurSpeed()) {
        nowLaser = laserGetCurSpeed();
        reDrawPrintingValue(ICON_POS_FAN, LIVE_INFO_BOTTOM_ROW);
      }
    }

    // check print time change
    if (time != getPrintTime())
    {
      time = getPrintTime();

      if (progDisplayType == ELAPSED_REMAINING)
        reDrawPrintingValue(ICON_POS_TIM, LIVE_INFO_TOP_ROW | LIVE_INFO_BOTTOM_ROW);
      else
        reDrawPrintingValue(ICON_POS_TIM, LIVE_INFO_BOTTOM_ROW);
    }

    // check print progress percentage change
    if (oldProgress != updatePrintProgress())
    {
      reDrawProgress(oldProgress);

      oldProgress = getPrintProgress();
    }

    // Z_AXIS coordinate
    if (layerDisplayType == SHOW_LAYER_BOTH || layerDisplayType == SHOW_LAYER_HEIGHT)
    {
      curLayerHeight = coordinateGetAxis(Z_AXIS);

      if (prevLayerHeight != curLayerHeight)
      {
        if (ABS(curLayerHeight - usedLayerHeight) >= LAYER_DELTA)
          layerDrawEnabled = true;

        if (layerDrawEnabled == true)
        {
          usedLayerHeight = curLayerHeight;

          reDrawPrintingValue(ICON_POS_Z, (layerDisplayType == SHOW_LAYER_BOTH) ? LIVE_INFO_TOP_ROW : LIVE_INFO_BOTTOM_ROW);
        }

        if (ABS(curLayerHeight - prevLayerHeight) < LAYER_DELTA)
          layerDrawEnabled = false;

        prevLayerHeight = curLayerHeight;
      }
    }

    if (layerDisplayType == SHOW_LAYER_BOTH || layerDisplayType == SHOW_LAYER_NUMBER)
    {
      curLayerNumber = getPrintLayerNumber();

      if (curLayerNumber != prevLayerNumber)
      {
        prevLayerNumber = curLayerNumber;

        reDrawPrintingValue(ICON_POS_Z, LIVE_INFO_BOTTOM_ROW);
      }
    }

    // check change in speed or flow
    if (curspeed[currentSpeedID] != speedGetCurPercent(currentSpeedID))
    {
      curspeed[currentSpeedID] = speedGetCurPercent(currentSpeedID);

      reDrawPrintingValue(ICON_POS_SPD, LIVE_INFO_BOTTOM_ROW);
    }

    // check if print is paused
    if (lastPause != isPaused())
    {
      lastPause = isPaused();
      setPauseResumeIcon(&printingItems, lastPause);

      menuDrawItem(&printingItems.items[KEY_ICON_4], KEY_ICON_4);
    }

    // check if print just started or just finished
    if (lastPrinting != isPrinting())
    {
      lastPrinting = isPrinting();

      #ifdef PORTRAIT_MODE
        if (lastPrinting == false)
          printSummaryPopup();
      #endif

      return;  // it will restart this interface if directly return this function without modify the value of infoMenu
    }

    toggleInfo();

    KEY_VALUES key_num = menuKeyGetValue();

    switch (key_num)
    {
      case PS_KEY_0:
      case PS_KEY_1:
      case PS_KEY_2:
        OPEN_MENU(menuFan);
        break;

      case PS_KEY_3:
        progDisplayType = (progDisplayType + 1) % 3;
        reDrawPrintingValue(ICON_POS_TIM, LIVE_INFO_TOP_ROW | LIVE_INFO_BOTTOM_ROW);
        break;

      case PS_KEY_4:
        OPEN_MENU(menuFan);
        break;

      case PS_KEY_5:
        OPEN_MENU(menuSpeed);
        break;

      case PS_KEY_6:
        if (lastPrinting == true)  // if printing
        { // Pause button
          if (getHostDialog())
            addToast(DIALOG_TYPE_ERROR, (char *)textSelect(LABEL_BUSY));
          else if (getPrintRunout())
            addToast(DIALOG_TYPE_ERROR, (char *)textSelect(LABEL_FILAMENT_RUNOUT));
          else
            pausePrint(!isPaused(), PAUSE_NORMAL);
        }
        else
        { // Main button
          infoMenu.cur = 0;
        }
        break;

      case PS_KEY_7:
        // OPEN_MENU(menuBabystep);
        break;

      case PS_KEY_8:
        OPEN_MENU(menuMore);
        break;

      case PS_KEY_9:
        if (lastPrinting == true)  // if printing
        { // Stop button
          popupDialog(DIALOG_TYPE_ALERT, LABEL_WARNING, LABEL_STOP_PRINT, LABEL_CONFIRM, LABEL_CANCEL, abortPrint, NULL, NULL);
        }
        else
        { // Back button
          // in case the print was started from menuPrintFromSource menu,
          // remove the filename from path to allow the files scanning from its folder avoiding a scanning error message
          exitFolder();

          CLOSE_MENU();
        }
        break;

      case PS_KEY_INFOBOX:
        printSummaryPopup();
        break;

      default:
        break;
    }

    loopProcess();
  }
}
