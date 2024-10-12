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
    {ICON_SPINDLE,                 LABEL_PROBE_OFFSET},
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
  if (infoSettings.laser_mode != 1)
  {
    zeroItems.items[4].icon = ICON_NULL;
    zeroItems.items[4].label.index = LABEL_NULL;
  }
  else
  {
    zeroItems.items[4].icon = ICON_SPINDLE;
    zeroItems.items[4].label.index = LABEL_PROBE_OFFSET;    
  }
 
  menuDrawPage(&zeroItems);
  drawXYZ();


  while (MENU_IS(menuZero))
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0: 
        break;
      case KEY_ICON_1: 
        setPosition(X_AXIS,0); 
        setPosition(Y_AXIS,0); 
        setPosition(Z_AXIS,0); 
        break;
      case KEY_ICON_2: setPosition(X_AXIS,0); break;
      case KEY_ICON_3: setPosition(Y_AXIS,0); break;
      case KEY_ICON_4: 
        if (infoSettings.laser_mode == 1)
        {
          setPosition(X_AXIS,infoParameters.ProbeOffset[X_AXIS]); 
          setPosition(Y_AXIS,infoParameters.ProbeOffset[Y_AXIS]); 
          laserSetSpeed(0);
        }
        break;
      case KEY_ICON_5: 
        if(infoSettings.touchplate_on == 1)
        {
          storeCmd("G38.3 Z-90\n"); // probe down
          // setPosition(Z_AXIS,infoSettings.touchplate_height);
          setPosition(Z_AXIS,0.5);
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
