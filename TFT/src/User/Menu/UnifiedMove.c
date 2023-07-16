#include "UnifiedMove.h"
#include "includes.h"

#if DELTA_PROBE_TYPE != 0  // if Delta printer
  void deltaCalibration(void)
  {
    mustStoreCmd("G33\n");
  }
#endif

void menuUnifiedMove(void)
{
  MENUITEMS UnifiedMoveItems = {
    // title
    LABEL_UNIFIEDMOVE,
    // icon                          label
    {
      {ICON_MOVE,                    LABEL_MOVE},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_DISABLE_STEPPERS,        LABEL_DISABLE_STEPPERS},
      {ICON_BABYSTEP,                LABEL_BABYSTEP},
      {ICON_HOME_MOVE,               LABEL_SET_POSITION},
      {ICON_PROBE_OFFSET,            LABEL_TOUCHPLATE},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  if (infoMachineSettings.leveling != BL_DISABLED)
  {
    UnifiedMoveItems.items[6].icon = ICON_LEVELING;
    UnifiedMoveItems.items[6].label.index = LABEL_BED_LEVELING;
  }

  menuDrawPage(&UnifiedMoveItems);
  drawXYZ();

  while (MENU_IS(menuUnifiedMove))
  {
    key_num = menuKeyGetValue();
    switch (key_num)
    {
      case KEY_ICON_0:
        OPEN_MENU(menuMove);
        break;

      case KEY_ICON_1:
        // OPEN_MENU(menuHome);
        break;

      case KEY_ICON_2:
        // OPEN_MENU(menuExtrude);
        break;

      case KEY_ICON_3:
        storeCmd("M84\n");
        break;

      case KEY_ICON_4:
        OPEN_MENU(menuBabystep);
        break;

      case KEY_ICON_5:
        storeCmd("G92 X0 Y0 Z0.5\n");
        break;

      case KEY_ICON_6:
        storeCmd("G38.3 Z-10\n");
        break;

      case KEY_ICON_7:
        CLOSE_MENU();
        break;

      default:
        break;
    }

    loopProcess();
  }
}
