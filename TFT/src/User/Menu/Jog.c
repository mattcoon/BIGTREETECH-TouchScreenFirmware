#include "Jog.h"
#include "includes.h"

#define GANTRY_REFRESH_TIME 500               // 1 second is 1000


#ifdef PORTRAIT_MODE
  #define OFFSET 0
#else
  #define OFFSET 1
#endif

#define KEY_100_MM KEY_ICON_0
#define KEY_10_MM  KEY_ICON_1
#define KEY_1_MM   KEY_ICON_2
#define KEY_01_MM  KEY_ICON_3
#define KEY_X      KEY_ICON_4
#define KEY_Y      KEY_ICON_5
#define KEY_Z      KEY_ICON_6
#define KEY_Back   KEY_ICON_7

void storeMoveCmd(const AXIS xyz, float amount);  // defined in Move.c

static AXIS nowAxis;
static bool hadJog;
static float amount;

static KEY_VALUES lastTopKey = KEY_100_MM;
static KEY_VALUES lastBottomKey = KEY_X;

const uint16_t unSelectIcon[8] = {ICON_NOT_100_MM, ICON_NOT_10_MM, ICON_NOT_1_MM, ICON_NOT_01_MM, ICON_NOT_X, ICON_NOT_Y, ICON_NOT_Z, ICON_BACK};
const uint16_t selectIcon[8] = {ICON_100_MM, ICON_10_MM, ICON_1_MM, ICON_01_MM, ICON_ZERO_X, ICON_ZERO_Y, ICON_ZERO_Z, ICON_BACK};

MENUITEMS jogItems = {
  // title
  LABEL_MOVE,
  //   icon                          label
  {
      {ICON_NOT_100_MM,              LABEL_NULL},
      {ICON_NOT_10_MM,               LABEL_NULL},
      {ICON_NOT_1_MM,                LABEL_NULL},
      {ICON_NOT_01_MM,               LABEL_NULL},
      {ICON_NOT_X,                   LABEL_NULL},
      {ICON_NOT_Y,                   LABEL_NULL},
      {ICON_NOT_Z,                   LABEL_NULL},
      {ICON_BACK,                    LABEL_BACK},
  }
};

void resetMenuJog(void)
{
  // copy icons from unSelectIcon to jogItems
  for (uint8_t i = 0; i < 8; i++)
    jogItems.items[i].icon = unSelectIcon[i];
  menuDrawPage(&jogItems);
}

void setMenuJog(KEY_VALUES key)
{
  if (key < 4) {
    jogItems.items[lastTopKey].icon = unSelectIcon[lastTopKey];
    jogItems.items[key].icon = selectIcon[key];
    if (nowAxis == Z_AXIS)
      amount = moveLenSteps[key+1];
    else
      amount = moveLenSteps[key];
  
    lastTopKey = key;
  }
  else if (key < 8) {
    jogItems.items[lastBottomKey].icon = unSelectIcon[lastBottomKey];
    jogItems.items[key].icon = selectIcon[key];
    lastBottomKey = key;
    if (key == KEY_Z)
    {
      jogItems.items[0].icon = ICON_NOT_10_MM;
      jogItems.items[1].icon = ICON_NOT_1_MM;
      jogItems.items[2].icon = ICON_NOT_01_MM;
      jogItems.items[3].icon = ICON_PROBE_OFFSET;
      nowAxis = Z_AXIS;
    }
    else if (key == KEY_X || key == KEY_Y)
    {
      jogItems.items[0].icon = ICON_NOT_100_MM;
      jogItems.items[1].icon = ICON_NOT_10_MM;
      jogItems.items[2].icon = ICON_NOT_1_MM;
      jogItems.items[3].icon = ICON_NOT_01_MM;
      if (key == KEY_X)
        nowAxis = X_AXIS;
      else
        nowAxis = Y_AXIS;
    }
    jogItems.items[lastTopKey].icon = selectIcon[lastTopKey];
  }
  if (hadJog)
    jogItems.items[7].icon = ICON_LEVEL_EDGE_DISTANCE;
  menuDrawPage(&jogItems);
}

void menuJog(void)
{
  hadJog = false;
  nowAxis = X_AXIS;
  amount = 100.0;


  KEY_VALUES key_num = KEY_IDLE;

  mustStoreCmd("G91\n");
  mustStoreCmd("M114\n");

  menuDrawPage(&jogItems);
  lastTopKey = KEY_100_MM;
  lastBottomKey = KEY_X;
  setMenuJog(lastBottomKey);
  setMenuJog(lastTopKey);
  
  drawXYZ();

  while (MENU_IS(menuJog))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      // X Y Z nowAxis set in setMenuJog along with icons
      case KEY_X:
      case KEY_Y:
      case KEY_Z:
        setMenuJog(key_num);
        break;
      // amount set in setMenuJog along with icons
      case KEY_100_MM: // 100mm or if Z axis, 10mm
      case KEY_10_MM:
      case KEY_1_MM:
        setMenuJog(key_num);
        break;
      case KEY_01_MM:
        if (nowAxis == Z_AXIS)
          ProbeTouchPlate();
        else {
          amount = moveLenSteps[3];
          setMenuJog(key_num);
        }
        break;
      case KEY_ICON_7:
        laserReset();
        resetMenuJog();
        if (hadJog) {
          hadJog = false;
          REPLACE_MENU(menuZero);
        }
        else
          CLOSE_MENU(); 
        break;

      case KEY_INCREASE:
        if (!hadJog) {
          hadJog = true;
        }
        storeMoveCmd(nowAxis, amount);
        break;

      case KEY_DECREASE:
        if (!hadJog) {
          hadJog = true;
        }
        storeMoveCmd(nowAxis, -amount);
        break;

      default:
        break;
    }

    if (infoSettings.laser_mode == 1)
      laserTest();
    loopProcess();

    updateGantry();
  }

  mustStoreCmd("G90\n");
}
