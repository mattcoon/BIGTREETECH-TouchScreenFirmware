#include "Print.h"
#include "includes.h"

// G-code command
#define MOVE_GCODE "G0 X%.2f Y%.2f Z%.2f\n"

float Y_depth = 100;
float X_width = 100;
float stepOver = 15;
float stepDown = 2;
// uint16_t stepCnt = 0;

float targetX = 0; // used for next command X
float targetY = 0; // used for next command Y
float targetZ = 0; // used for next command Z


  LABEL title = {LABEL_SURFACE};

  LISTITEM listPrintSpecial[] = {
    // icon            item type           item title          item value text(only for custom value)
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_ITEM_Y_DEPTH, LABEL_CUSTOM_VALUE}, // Y_depth
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_ITEM_X_WIDTH, LABEL_CUSTOM_VALUE}, // X_width
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_STEPOVER,     LABEL_CUSTOM_VALUE}, // stepOver
    {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_STEPDOWN,     LABEL_CUSTOM_VALUE}, // stepDown
    // {CHARICON_SETTING1, LIST_CUSTOMVALUE,   LABEL_STEPCNT,      VAL_TYPE_INT},   // stepCnt
    {CHARICON_PRINT,    LIST_LABEL,         LABEL_SURFACE,      LABEL_NULL},
  };

  VAL_TYPE typePrintSpecial[] = {
    VAL_TYPE_FLOAT, // Y_depth
    VAL_TYPE_FLOAT, // X_width
    VAL_TYPE_FLOAT, // stepOver
    VAL_TYPE_FLOAT, // stepDown
    // VAL_TYPE_INT,   // stepCnt
  };

// print surface
// return: true if in process, false if done
bool printSurface( void ) {
  if (targetZ == 0) { 
    targetZ = -stepDown;
    mustStoreCmd(MOVE_GCODE, targetX, targetY, targetZ);
  }
  targetY = Y_depth-targetY; // reverse direction
  // clear column
  mustStoreCmd(MOVE_GCODE, targetX, targetY, targetZ);
 
  // prepare next column
  targetX += stepOver;
  mustStoreCmd(MOVE_GCODE, targetX, targetY, targetZ);

  // check if first layer is done
  if (targetX > (X_width+stepOver)) {
    return false;
  }
  // still working
  return true;
}


inline uint8_t getSurfaceParameterCount(void) {
    return COUNT(listPrintSpecial);
}

float getSurfaceParameter(uint8_t index) {
  if (index < getSurfaceParameterCount()) {
    switch (index)
    {
      case 0: return Y_depth;
      case 1: return X_width;
      case 2: return stepOver;
      case 3: return stepDown;
      // case 4: return stepCnt;
    }
  }
  return 0;
}

void setSurfaceParameter(uint8_t index, float value) {
  if (index < getSurfaceParameterCount()) {
    switch (index)
    {
      case 0: Y_depth = value; break;
      case 1: X_width = value; break;
      case 2: stepOver = value; break;
      case 3: stepDown = value; break;
      // case 4: stepCnt = value; break;
    }
  }
}

void loadRoutineElements(LISTITEM * parameterMainItem, uint16_t index, uint8_t itemPos)
{
  uint8_t elementIndex = index;

  if (elementIndex < getSurfaceParameterCount())
  {
    parameterMainItem->icon = listPrintSpecial[elementIndex].icon;
    parameterMainItem->itemType = listPrintSpecial[elementIndex].itemType;
    parameterMainItem->titlelabel.index = listPrintSpecial[elementIndex].titlelabel.index;
    parameterMainItem->titlelabel.address = listPrintSpecial[elementIndex].titlelabel.address;
    parameterMainItem->valueLabel.index = LABEL_CUSTOM_VALUE;

    // load value
    setDynamicValue(itemPos, getSurfaceParameter(elementIndex));

  }
  else
  {
    parameterMainItem->icon = CHARICON_NULL;
  }
}

void menuPrintSpecial (void) {

  static bool isPrinting = false;
  const uint8_t parameterCount = getSurfaceParameterCount();
  // float oldval[parameterCount];
  uint16_t curIndex = KEY_IDLE;
  // uint8_t curItem = parameterCount;

  // uint16_t curPage = curItem / LISTITEM_PER_PAGE;
  // for (uint8_t i = 0; i < parameterCount; i++)
  // {
  //   oldval[i] = getSurfaceParameter(i);
  // }
  
  listViewCreate(title, NULL, parameterCount, NULL, false, NULL, loadRoutineElements);

  while (MENU_IS(menuPrintSpecial))
  {
    curIndex = listViewGetSelectedIndex();

    switch (curIndex)
    {
      case KEY_BACK:
        // curPage = 0;
        CLOSE_MENU();
        break;

      default:
        if (curIndex < parameterCount-1)
        {
          float val = getSurfaceParameter(curIndex);
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
    // for (uint8_t i = 0; i < parameterCount; i++)
    // {
    //   float newVal = getSurfaceParameter(i);

    //   if (oldval[i] != newVal)
    //   {
    //     oldval[i] = newVal;

    //     listViewRefreshItem(i);
    //   }
    // }

    if (isPrinting) isPrinting = printSurface();

    loopProcess();
  }

}

