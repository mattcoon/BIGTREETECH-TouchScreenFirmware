#include "includes.h"

typedef struct
{
  uint8_t cur;
  uint8_t set;
} LASTLASER;

void menuLaser(void)
{
  // 1 title, ITEM_PER_PAGE items (icon + label)
  MENUITEMS laserItems = {
  // title
  LABEL_LASER,
  // icon                       label
    {
      {ICON_DEC,                  LABEL_DEC},
      {ICON_NULL,                 LABEL_NULL},
      {ICON_NULL,                 LABEL_NULL},
      {ICON_INC,                  LABEL_INC},
      {ICON_LASER_2 ,             LABEL_LASER_2},
      {ICON_LASER_100,            LABEL_LASER_100},
      {ICON_LASER_OFF,            LABEL_LASER_OFF},
      {ICON_BACK,                 LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;
  LASTLASER lastLaser = {0,0};

  laserSetSpeed(laserGetCurSpeed());

  menuDrawPage(&laserItems);
  drawXYZ();

  laserReDraw(true);

  while (MENU_IS(menuLaser))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
      case KEY_DECREASE:
        if (laserGetSetSpeed() > 0)
        {
          if (infoSettings.fan_percentage == 1)
            laserSetPercent(laserGetSetPercent() - 1);
          else
            laserSetSpeed(laserGetSetSpeed() - 1);
        }
        break;

      case KEY_INFOBOX:
      {
        int16_t val;

        if (infoSettings.fan_percentage == 1)
        {
          val = editIntValue(0, 100, 0, laserGetSetPercent());

          if (val != laserGetSetPercent())
            laserSetPercent(val);
        }
        else
        {
          val = editIntValue(0, infoSettings.fan_max[0], 0, laserGetCurSpeed());

          if (val != laserGetCurSpeed())
            laserSetSpeed(val);
        }

        laserReDraw(true);
        break;
      }

      case KEY_ICON_3:
      case KEY_INCREASE:
        if (laserGetSetSpeed() < infoSettings.fan_max[0])
        {
          if (infoSettings.fan_percentage == 1)
            laserSetPercent( laserGetSetPercent() + 1);
          else
            laserSetSpeed( laserGetSetSpeed() + 1);
        }
        break;

      case KEY_ICON_4:
          laserSetPercent(2);
        break;

      case KEY_ICON_5:
        laserSetSpeed(infoSettings.fan_max[0]);  // fan at maximum speed
        break;

      case KEY_ICON_6:
        laserReset();
        break;

      case KEY_ICON_7:
        // stop laser before exit of fan menu
        laserReset();
        CLOSE_MENU();
        break;

      default:
        break;
    }

    if ((lastLaser.cur != laserGetCurSpeed()) || (lastLaser.set != laserGetSetSpeed()))
    { // refresh displayed values if actual or desired speed has changed
      lastLaser = (LASTLASER) {laserGetCurSpeed(), laserGetSetSpeed()};

      laserReDraw(false);
    }
    loopProcess();
    updateGantry();
  }
}
