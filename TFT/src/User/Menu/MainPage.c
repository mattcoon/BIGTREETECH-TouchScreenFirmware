#include "MainPage.h"
#include "includes.h"

void menuMain(void)
{
  // 1 title, ITEM_PER_PAGE items (icon + label)
  MENUITEMS mainPageItems = {
    // title
    LABEL_MAINMENU,
    // icon                          label
    {
      {ICON_HOME,                    LABEL_HOME},
      {ICON_MOVE,                    LABEL_MOVE},
      {ICON_LEVEL_EDGE_DISTANCE,     LABEL_ZERO},
      {ICON_PRINT,                   LABEL_CUT},
      {ICON_SETTINGS,                LABEL_SETTINGS},
      {ICON_DISABLE_STEPPERS,        LABEL_DISABLE_STEPPERS},
      {ICON_LASER,                   LABEL_LASER},
      {ICON_NULL,                    LABEL_NULL},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  if (infoSettings.status_screen == 1)
  {
    mainPageItems.items[7].icon = ICON_BACK;
    mainPageItems.items[7].label.index = LABEL_BACK;
  }
  if (infoSettings.laser_mode == 1)
  {
    mainPageItems.items[6].icon = ICON_LASER;
    mainPageItems.items[6].label.index = LABEL_LASER;
  }
  else
  {
    mainPageItems.items[6].icon = ICON_NULL;
    mainPageItems.items[6].label.index = LABEL_NULL;
  }

  menuDrawPage(&mainPageItems);
  drawXYZ();

  while (MENU_IS(menuMain))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:  OPEN_MENU(menuHome);     break;

      case KEY_ICON_1:  
        if (infoSettings.laser_mode == 1)
          laserReset();
        OPEN_MENU(menuMove);
        break;

      case KEY_ICON_2:  OPEN_MENU(menuZero);    break;

      case KEY_ICON_3:  OPEN_MENU(menuPrint);    break;

      case KEY_ICON_4:  OPEN_MENU(menuSettings); break;

      case KEY_ICON_5:  storeCmd("M84\n");       break;

      case KEY_ICON_6:  
        if (infoSettings.laser_mode == 1)
          laserReset();
          OPEN_MENU(menuLaser);    
        break;
        
      case KEY_ICON_7:
        if (infoSettings.status_screen == 1)
          CLOSE_MENU();
        break;

      default:
        break;
    }

    loopProcess();
    updateGantry();
  }
}
