#include "Home.h"
#include "includes.h"


bool inhibitHome = false;

  MENUITEMS homeItems = {
  // title
  LABEL_HOME,
  // icon                          label
  {
    {ICON_HOME_MOVE,               LABEL_XY},
    {ICON_LEVEL_EDGE_DISTANCE,     LABEL_SET_POSITION},
    {ICON_ZERO_X,                  LABEL_ZERO_X},
    {ICON_ZERO_Y,                  LABEL_ZERO_Y},
    {ICON_Z_HOME,                  LABEL_Z},
    {ICON_PROBE_OFFSET,            LABEL_TOUCHPLATE},
    {ICON_ZERO_Z,                  LABEL_ZERO_Z},
    {ICON_BACK,                    LABEL_BACK},
  }
};

void setPosition(AXIS axis, float position) {
  // cur2work based on current absolute - position used to reset
  cur2workSetAxis(axis, coordinateGetAbsAxis(axis)-position);
  storeCmd("G92 %c%.3f\n",axis_id[axis],position);
}

void menuHome(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  if(infoSettings.touchplate_on != 1) {
    homeItems.items[5].icon = ICON_NULL;
    homeItems.items[5].label.index = LABEL_NULL;
  }
  if(inhibitHome == true) {
    homeItems.items[0].icon = ICON_NULL;
    homeItems.items[0].label.index = LABEL_NULL;
  }
  else {
    homeItems.items[0].icon = ICON_HOME_MOVE;
    homeItems.items[0].label.index = LABEL_XY;
  }

  menuDrawPage(&homeItems);
  drawXYZ();


  while (MENU_IS(menuHome))
  {
    key_num = menuKeyGetValue();
    switch(key_num)
    {
      case KEY_ICON_0:
        if(inhibitHome == false) {
          if (isAxisKnown(Z_AXIS)==false) {
            storeCmd("G28 Z\n");
            cur2workSetAxis(Z_AXIS,0);
          }
          storeCmd("G28 XY\n");
          cur2workSetAxis(X_AXIS,0);
          cur2workSetAxis(Y_AXIS,0);
        }
        break;
      case KEY_ICON_1: setPosition(X_AXIS,0); setPosition(Y_AXIS,0); break;
      case KEY_ICON_2: setPosition(X_AXIS,0); break;
      case KEY_ICON_3: setPosition(Y_AXIS,0); break;
      case KEY_ICON_4: storeCmd("G28 Z\n"); cur2workSetAxis(Z_AXIS,0); break;
      case KEY_ICON_5: 
        if(infoSettings.touchplate_on == 1)
        {
          storeCmd("G38.3 Z-90\n"); // probe down
          setPosition(Z_AXIS,infoSettings.touchplate_height);
          // storeCmd("G92 Z%.3f\n", infoSettings.touchplate_height); // set height position to touchplate
          storeCmd("G0 Z10\n"); // lift off of probe
        }
        break;
      case KEY_ICON_6: setPosition(Z_AXIS,0); break;
      case KEY_ICON_7: 
        inhibitHome = false; 
        CLOSE_MENU();
        break;
      default: break;
    }

    loopProcess();
    updateGantry();
  }
}
