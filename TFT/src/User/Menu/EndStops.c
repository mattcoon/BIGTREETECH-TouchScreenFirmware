#include "Print.h"
#include "includes.h"

#if 0
  LABEL title = {LABEL_ENDSTOPS};

  LISTITEM listEndStops[] = {
    // icon            item type           item title          item value text(only for custom value)
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_ITEM_Y_DEPTH, LABEL_CUSTOM_VALUE}, // Y_depth
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_ITEM_X_WIDTH, LABEL_CUSTOM_VALUE}, // X_width
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_STEPOVER,     LABEL_CUSTOM_VALUE}, // stepOver
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_STEPDOWN,     LABEL_CUSTOM_VALUE}, // stepDown
    {CHARICON_PRINT,    LIST_LABEL,         LABEL_SURFACE,      LABEL_NULL},
  };

  VAL_TYPE typeEndStops[] = {
    VAL_TYPE_FLOAT, // Y_depth
    VAL_TYPE_FLOAT, // X_width
    VAL_TYPE_FLOAT, // stepOver
    VAL_TYPE_FLOAT, // stepDown
  };

inline int16_t getEndStopCount(void) {
  return 5;
}
 
float getEndStop(uint8_t index) {
  if (index < getEndStopCount()) {
    return endStopValue[index];
  }
  return 0;
}

void loadEndStopElements(LISTITEM * parameterMainItem, uint16_t index, uint8_t itemPos)
{
  uint8_t elementIndex = index;

  if (elementIndex < getEndStopCount())
  {
    parameterMainItem->icon = listEndStops[elementIndex].icon;
    parameterMainItem->itemType = listEndStops[elementIndex].itemType;
    parameterMainItem->titlelabel.index = listEndStops[elementIndex].titlelabel.index;
    parameterMainItem->titlelabel.address = listEndStops[elementIndex].titlelabel.address;
    parameterMainItem->valueLabel.index = LABEL_CUSTOM_VALUE;

    // load value
    setDynamicValue(itemPos, getEndStop(elementIndex));

  }
  else
  {
    parameterMainItem->icon = CHARICON_NULL;
  }
}

#define ENDSTOP_REFRESH_TIME 250

void updateEndStop(void)
{
  if (nextScreenUpdate(ENDSTOP_REFRESH_TIME))
  {
    mustStoreCmd("M119\n");
  }
}


void menuEndStop (void) {

  static bool isPrinting = false;
  const uint8_t parameterCount = getEndStopCount();
  uint16_t curIndex = KEY_IDLE;

  listViewCreate(title, NULL, parameterCount, NULL, false, NULL, loadEndStopElements);

  while (MENU_IS(menuPrintSpecial))
  {
    curIndex = listViewGetSelectedIndex();

    switch (curIndex)
    {
      case KEY_BACK:
        CLOSE_MENU();
        break;

      default:
        if (curIndex < parameterCount-1)
        {
          float val = getEndStop(curIndex);
          val = numPadFloat(NULL, val, val, false);  // parameter is a decimal number
          setSurfaceParameter(curIndex, val);
          setDynamicValue(curIndex, val);
          listViewRefreshMenu();
        }
        else if (curIndex == parameterCount-1)
        {
          isPrinting = true;
        }
        break;
    }

    if (isPrinting) isPrinting = printSurface();

    loopProcess();
  }

}

#endif