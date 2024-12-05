#include "UnifiedMove.h"
#include "includes.h"

#if DELTA_PROBE_TYPE != 0  // if Delta printer

static void deltaCalibration(void)
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
      {ICON_MOVE,                    LABEL_JOG},
      {ICON_MOVE,                    LABEL_MOVE},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_POINT_1,                 LABEL_ZERO},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  menuDrawPage(&UnifiedMoveItems);

  while (MENU_IS(menuUnifiedMove))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
        OPEN_MENU(menuJog);
        break;

      case KEY_ICON_1:
        OPEN_MENU(menuMove);
        break;

      case KEY_ICON_2:
        break;

      case KEY_ICON_3:
        break;

      case KEY_ICON_4:
        storeCmd("G0 X0 Y0\n");
        break;

      case KEY_ICON_5:
            // popupDialog(DIALOG_TYPE_ALERT, LABEL_WARNING, LABEL_CONNECT_PROBE, LABEL_CONTINUE, LABEL_CANCEL, deltaCalibration, NULL, NULL);
        break;

      case KEY_ICON_6:
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
