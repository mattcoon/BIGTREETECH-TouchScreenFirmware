#include "Home.h"
#include "includes.h"

const MENUITEMS homeItems = {
  // title
  LABEL_HOME,
  // icon                          label
  {
    {ICON_HOME,                    LABEL_XY},
    {ICON_ZERO_X,                  LABEL_ZERO_X},
    {ICON_ZERO_Y,                  LABEL_ZERO_Y},
    {ICON_NULL,                    LABEL_NULL},
    {ICON_Z_HOME,                  LABEL_Z},
    {ICON_PROBE_OFFSET,            LABEL_TOUCHPLATE},
    {ICON_ZERO_Z,                  LABEL_ZERO_Z},
    {ICON_BACK,                    LABEL_BACK},
  }
};

void menuHome(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&homeItems);
  drawXYZ();


  while (MENU_IS(menuHome))
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: storeCmd("G28 XY\n"); break;
      case KEY_ICON_1: storeCmd("G92 X0\n"); break;
      case KEY_ICON_2: storeCmd("G92 Y0\n"); break;
      case KEY_ICON_3: break;
      case KEY_ICON_4: storeCmd("G28 Z\n"); break;
      case KEY_ICON_5: 
        if(infoSettings.touchplate_on == 1)
        {
          storeCmd("G38.3 Z-90\n");
          storeCmd("G92 Z%.3f\n", infoSettings.touchplate_height);
        }
        break;
      case KEY_ICON_6: storeCmd("G92 Z0\n"); break;
      case KEY_ICON_7: CLOSE_MENU();         break;
      default: break;
    }

    loopProcess();
    updateGantry();
  }
}
