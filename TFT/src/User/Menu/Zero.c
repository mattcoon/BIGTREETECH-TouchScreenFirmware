#include "Zero.h"
#include "includes.h"


  MENUITEMS zeroItems = {
  // title
  LABEL_HOME,
  // icon                          label
  {
    {ICON_NULL,                    LABEL_NULL},
    {ICON_LEVEL_EDGE_DISTANCE,     LABEL_SET_POSITION},
    {ICON_ZERO_X,                  LABEL_ZERO_X},
    {ICON_ZERO_Y,                  LABEL_ZERO_Y},
    {ICON_NULL,                    LABEL_NULL},
    {ICON_PROBE_OFFSET,            LABEL_TOUCHPLATE},
    {ICON_ZERO_Z,                  LABEL_ZERO_Z},
    {ICON_BACK,                    LABEL_BACK},
  }
};

void menuZero(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  if(infoSettings.touchplate_on != 1) {
    zeroItems.items[5].icon = ICON_NULL;
    zeroItems.items[5].label.index = LABEL_NULL;
  }
  
  menuDrawPage(&zeroItems);
  drawXYZ();


  while (MENU_IS(menuZero))
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      // case KEY_ICON_0:
      case KEY_ICON_1: setPosition(X_AXIS,0); setPosition(Y_AXIS,0); setPosition(Z_AXIS,0); break;
      case KEY_ICON_2: setPosition(X_AXIS,0); break;
      case KEY_ICON_3: setPosition(Y_AXIS,0); break;
      // case KEY_ICON_4: 
      case KEY_ICON_5: 
        if(infoSettings.touchplate_on == 1)
        {
          storeCmd("G38.3 Z-90\n"); // probe down
          setPosition(Z_AXIS,infoSettings.touchplate_height);
          storeCmd("G0 Z10\n"); // lift off of probe
        }
        break;
      case KEY_ICON_6: setPosition(Z_AXIS,0); break;
      case KEY_ICON_7: 
        CLOSE_MENU();
        break;
      default: break;
    }

    loopProcess();
    updateGantry();
  }
}