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
#define MENUSWITCHLIMIT 500
void storeMoveCmd(const AXIS xyz, float amount);  // defined in Move.c

static AXIS nowAxis;
static bool hadJog;
static float amount;

static KEY_VALUES lastTopKey;
static KEY_VALUES lastBottomKey;

const uint16_t unSelectTZIcon[4]= { ICON_NOT_10_MM, ICON_NOT_1_MM,  ICON_NOT_01_MM, ICON_PROBE_OFFSET };
const uint16_t selectTZIcon[4]  = { ICON_10_MM,     ICON_1_MM,      ICON_01_MM,     ICON_PROBE_OFFSET} ;
const uint16_t unSelectTIcon[4] = { ICON_NOT_100_MM,ICON_NOT_10_MM, ICON_NOT_1_MM,  ICON_NOT_01_MM };
const uint16_t selectTIcon[4]   = { ICON_100_MM,    ICON_10_MM,     ICON_1_MM,      ICON_01_MM } ;
const uint16_t unSelectBIcon[4] = { ICON_NOT_X,     ICON_NOT_Y,     ICON_NOT_Z,     ICON_BACK };
const uint16_t selectBIcon[4]   = { ICON_ZERO_X,    ICON_ZERO_Y,    ICON_ZERO_Z,    ICON_BACK };

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

void setMenuJog(KEY_VALUES key)
{
  if (key < 4) { // 0 to 3 meaning distance
    if (nowAxis == Z_AXIS) {
      jogItems.items[lastTopKey].icon = unSelectTZIcon[lastTopKey];
      jogItems.items[key].icon = selectTZIcon[key];
      amount = moveLenSteps[key+1];
    }
    else {
      jogItems.items[lastTopKey].icon = unSelectTIcon[lastTopKey];
      jogItems.items[key].icon = selectTIcon[key];
      amount = moveLenSteps[key];
    }
  
    lastTopKey = key;
  }
  else if (key < 8) { // 4 to 7 meaning axis
    jogItems.items[lastBottomKey+4].icon = unSelectBIcon[lastBottomKey];
    jogItems.items[key].icon = selectBIcon[key-4];
    lastBottomKey = key-4;
    if (key == KEY_Z)
    {
      // update jogItems icons for Z axis by interating through the items
      for (uint8_t i = 0; i < 4; i++) 
        jogItems.items[i].icon = unSelectTZIcon[i];
      if (nowAxis != Z_AXIS) {
        amount = moveLenSteps[1]; // 10mm
        jogItems.items[0].icon = selectTZIcon[0];
        nowAxis = Z_AXIS;
      }
      else {
        jogItems.items[lastTopKey].icon = selectTZIcon[lastTopKey];
      }
    }
    else if (key == KEY_X || key == KEY_Y)
    {
      for (uint8_t i = 0; i < 4; i++) 
        jogItems.items[i].icon = unSelectTIcon[i];
      if (nowAxis == Z_AXIS) {
        lastTopKey = 0;
        amount = moveLenSteps[lastTopKey]; // 100mm
      }
      jogItems.items[lastTopKey].icon = selectTIcon[lastTopKey];

      if (key == KEY_X)
        nowAxis = X_AXIS;
      else
        nowAxis = Y_AXIS;
    }
  }
  if (hadJog) {
    jogItems.items[7].icon = ICON_LEVEL_EDGE_DISTANCE;
    jogItems.items[7].label.index = LABEL_ZERO;
  }
  else {
    jogItems.items[7].icon = ICON_BACK;
    jogItems.items[7].label.index = LABEL_BACK;
  }
  menuDrawPage(&jogItems);
}

void resetMenuJog(void)
{
  // copy icons from unSelectIcon to jogItems
  for (uint8_t i = 0; i < 4; i++)
    jogItems.items[i].icon = unSelectTIcon[i];
  for (uint8_t i = 0; i < 4; i++)
    jogItems.items[i].icon = unSelectBIcon[i];
  setMenuJog(KEY_100_MM);
  setMenuJog(KEY_X);
}

void menuJog(void)
{
  #if LCD_ENCODER_SUPPORT
    bool encButtonReset = !LCD_Enc_ReadBtn(LCD_ENC_BUTTON_INTERVAL);
  #endif
  hadJog = false;
  setMenuJog(KEY_100_MM);
  setMenuJog(KEY_X);
  KEY_VALUES key_num = KEY_IDLE;

  mustStoreCmd("G91\n");
  mustStoreCmd("M114\n");

  resetMenuJog();
  menuDrawPage(&jogItems);

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
        hadJog = true;
        setMenuJog(key_num);
        storeMoveCmd(nowAxis, amount);
        break;

      case KEY_DECREASE:
        hadJog = true;
        setMenuJog(key_num);
        storeMoveCmd(nowAxis, -amount);
        break;

      default:
        #if LCD_ENCODER_SUPPORT
          if (encButtonReset) {
            if (LCD_Enc_ReadBtn(MENUSWITCHLIMIT))
            REPLACE_MENU(menuMove);
          }
          if (!LCD_Enc_ReadBtn(MENUSWITCHLIMIT))
              encButtonReset = true;
        #endif
        break;
    }

    if (infoSettings.laser_mode == 1)
      laserTest();
    loopProcess();

    updateGantry();
  }

  mustStoreCmd("G90\n");
}
