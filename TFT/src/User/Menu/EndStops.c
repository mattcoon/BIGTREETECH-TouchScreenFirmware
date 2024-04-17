#include "Print.h"
#include "includes.h"

LABEL EndstopsTitle = {LABEL_ENDSTOPS};

// x, y, y2, z, z2, z_probe
char * const EndstopsDisplayID[] = {"Endstop X", "Endstop Y", "Endstop Y2", "Endstop Z", "Endstop Z2", "Z Probe"};

LISTITEM listEndStops[] = {
  // icon            item type           item title          item value text(only for custom value)
  {CHARICON_BLANK, LIST_CUSTOMVALUE,   LABEL_CUSTOM_VALUE, LABEL_CUSTOM_VALUE}, // EndStop X
  {CHARICON_BLANK, LIST_CUSTOMVALUE,   LABEL_CUSTOM_VALUE, LABEL_CUSTOM_VALUE}, // EndStop Y
  {CHARICON_BLANK, LIST_CUSTOMVALUE,   LABEL_CUSTOM_VALUE, LABEL_CUSTOM_VALUE}, // EndStop Y2
  {CHARICON_BLANK, LIST_CUSTOMVALUE,   LABEL_CUSTOM_VALUE, LABEL_CUSTOM_VALUE}, // EndStop Z
  {CHARICON_BLANK, LIST_CUSTOMVALUE,   LABEL_CUSTOM_VALUE, LABEL_CUSTOM_VALUE}, // EndStop Z2
  {CHARICON_BLANK, LIST_CUSTOMVALUE,   LABEL_CUSTOM_VALUE, LABEL_CUSTOM_VALUE}, // EndStop Z Probe
};

VAL_TYPE typeEndStops[] = {
  VAL_TYPE_INT, // EndStop X
  VAL_TYPE_INT, // EndStop Y
  VAL_TYPE_INT, // EndStop Y2
  VAL_TYPE_INT, // EndStop Z
  VAL_TYPE_INT, // EndStop Z2
  VAL_TYPE_INT, // EndStop Z Probe
};

inline int16_t getEndStopCount(void) {
  return 6;
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
    parameterMainItem->titlelabel.address = EndstopsDisplayID[elementIndex];
    parameterMainItem->valueLabel.index = LABEL_CUSTOM_VALUE;

    // load value
    setDynamicValue(itemPos, getEndStop(elementIndex));

  }
}

#define ENDSTOP_REFRESH_TIME 500

static uint16_t oldendStopValue[6];  // x, y, z, y2, z2

void updateEndStop(void)
{
  if (nextScreenUpdate(ENDSTOP_REFRESH_TIME))
  {
    mustStoreCmd("M119\n");
  }
}

bool isEndStopUpdated(void)
{
  for(uint8_t i = 0; i < getEndStopCount(); i++)
  {
    if (endStopValue[i] != oldendStopValue[i])
    {
      oldendStopValue[i] = endStopValue[i];
      return true;
    }
  }
  return false;
}

void menuEndStops (void) {

  const uint8_t parameterCount = getEndStopCount();
  uint16_t curIndex = KEY_IDLE;

  listViewCreate(EndstopsTitle, NULL, parameterCount, NULL, true, NULL, loadEndStopElements);

  while (MENU_IS(menuEndStops))
  {
    curIndex = listViewGetSelectedIndex();

    switch (curIndex)
    {
      case KEY_BACK:
        CLOSE_MENU();
        break;
      default:
        if (isEndStopUpdated()) {
          for (uint8_t i = 0; i < parameterCount; i++) {
            setDynamicValue(i, getEndStop(i));
          }
          listViewRefreshMenu();
        }
        break;
    }

    updateEndStop();

    loopProcess();    
  }

}
