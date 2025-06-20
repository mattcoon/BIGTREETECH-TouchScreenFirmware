#include "Move.h"
#include "includes.h"

#define LOAD_XYZ_LABEL_INDEX(p0, dir0, p1, dir1, axis)       \
  do                                                         \
  {                                                          \
    moveItems.items[p0].label.index = LABEL_##axis##_##dir0; \
    moveItems.items[p1].label.index = LABEL_##axis##_##dir1; \
  } while (0)

#define X_MOVE_GCODE        "G0 X%.2f F%d\n"  // X axis gcode
#define Y_MOVE_GCODE        "G0 Y%.2f F%d\n"  // Y axis gcode
#define Z_MOVE_GCODE        "G0 Z%.2f F%d\n"  // Z axis gcode
#define GANTRY_REFRESH_TIME 500               // 1 second is 1000
#define MENUSWITCHLIMIT 500
#ifdef PORTRAIT_MODE
  #define OFFSET 0
#else
  #define OFFSET 1
#endif

static const char * const xyzMoveCmd[] = {X_MOVE_GCODE, Y_MOVE_GCODE, Z_MOVE_GCODE};

static uint8_t item_moveLen_index = 0;
static AXIS nowAxis = X_AXIS;
static bool hadMovement = false;

void drawXYZ(void)
{
  if (getReminderStatus() != SYS_STATUS_IDLE || toastRunning()) return;

  if (coordinateIsKnown()) {
    char tempstr[30];
    GUI_SetColor(infoSettings.status_color);

    // if(isAxisKnown(X_AXIS)) sprintf(tempstr, "X:%.2f  ", coordinateGetAbsAxis(X_AXIS));
    if(isAxisKnown(X_AXIS)) sprintf(tempstr, "X:%.2f  ", coordinateGetAxisActual(X_AXIS));
    else sprintf(tempstr,"X:----");
    GUI_DispString(START_X + (OFFSET + 0) * SPACE_X + (OFFSET + 0) * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (uint8_t *)tempstr);

    // if(isAxisKnown(Y_AXIS)) sprintf(tempstr, "Y:%.2f  ", coordinateGetAbsAxis(Y_AXIS));
    if(isAxisKnown(Y_AXIS)) sprintf(tempstr, "Y:%.2f  ", coordinateGetAxisActual(Y_AXIS));
    else sprintf(tempstr,"Y:----");
    GUI_DispString(START_X + (OFFSET + 1) * SPACE_X + (OFFSET + 1) * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (uint8_t *)tempstr);

    // if(isAxisKnown(Z_AXIS)) sprintf(tempstr, "Z:%.2f  ", coordinateGetAbsAxis(Z_AXIS));
    if(isAxisKnown(Z_AXIS)) sprintf(tempstr, "Z:%.2f  ", coordinateGetAxisActual(Z_AXIS));
    else sprintf(tempstr,"Z:----");
    GUI_DispString(START_X + (OFFSET + 2) * SPACE_X + (OFFSET + 2) * ICON_WIDTH, (ICON_START_Y - BYTE_HEIGHT) / 2, (uint8_t *)tempstr);

    GUI_SetColor(infoSettings.font_color);
  }
}

void updateGantry(void)
{
  if (nextScreenUpdate(GANTRY_REFRESH_TIME))
  {
    coordinateQuery(0);  // query position manually for delay less than 1 second

    drawXYZ();
  }
}

  MENUITEMS moveItems = {
    // title
    LABEL_MOVE,
    //   icon                          label
    {
      #ifdef ALTERNATIVE_MOVE_MENU
        {ICON_Z_DEC,                   LABEL_Z_DEC},
        {ICON_Y_INC,                   LABEL_Y_DEC},
        {ICON_Z_INC,                   LABEL_Z_INC},
        {ICON_01_MM,                   LABEL_01_MM},
        {ICON_X_DEC,                   LABEL_X_DEC},
        {ICON_Y_DEC,                   LABEL_Y_INC},
        {ICON_X_INC,                   LABEL_X_INC},
        {ICON_BACK,                    LABEL_BACK},
      #else
        {ICON_X_INC,                   LABEL_X_INC},
        {ICON_Y_DEC,                   LABEL_Y_INC},
        {ICON_Z_INC,                   LABEL_Z_INC},
        {ICON_01_MM,                   LABEL_01_MM},
        {ICON_X_DEC,                   LABEL_X_DEC},
        {ICON_Y_INC,                   LABEL_Y_DEC},
        {ICON_Z_DEC,                   LABEL_Z_DEC},
        {ICON_BACK,                    LABEL_BACK},
      #endif
    }
  };

void replaceMoveBack(bool replace) {
  hadMovement = replace;
  if (replace) {
    // after home move is next. replace "back" with "move"
    moveItems.items[7].icon = ICON_LEVEL_EDGE_DISTANCE;
    moveItems.items[7].label.index = LABEL_ZERO;
  }
  else {
    moveItems.items[7].icon = ICON_BACK;
    moveItems.items[7].label.index = LABEL_BACK;
  }
  menuDrawPage(&moveItems);
}

void storeMoveCmd(const AXIS xyz, float amount)
{
  // don't limit Z due to probing issue limiting movement
  if (xyz != Z_AXIS) {
    float dist2max = infoParameters.MachineMax[xyz] - coordinateGetAbsAxis(xyz);
    float dist2min = infoParameters.MachineMin[xyz] - coordinateGetAbsAxis(xyz);
    amount = MIN(amount, dist2max);
    amount = MAX(amount, dist2min);
  }
  // if invert is true, use 'amount' multiplied by -1
  storeCmd(xyzMoveCmd[xyz], GET_BIT(infoSettings.inverted_axis, xyz) ? -amount : amount,
           ((xyz != Z_AXIS) ? infoSettings.xy_speed[infoSettings.move_speed] : infoSettings.z_speed[infoSettings.move_speed]));

}

#define LASERTIME  500
#define LASERONSPD      1

static uint8_t laserDuty = 0;
void laserTest(void)
{
  
  if (nextScreenUpdate(LASERTIME))
  {
      laserDuty = LASERONSPD - laserDuty;
      laserSetSpeed(laserDuty);
  }
}

void menuMove(void)
{
  #if LCD_ENCODER_SUPPORT
    bool encButtonReset = !LCD_Enc_ReadBtn(LCD_ENC_BUTTON_INTERVAL);
  #endif
  hadMovement = false;
  KEY_VALUES key_num = KEY_IDLE;

  float amount = moveLenSteps[item_moveLen_index];

  mustStoreCmd("G91\n");
  mustStoreCmd("M114\n");

  // keys position table
  uint8_t table[TOTAL_AXIS][2] =
    #ifdef ALTERNATIVE_MOVE_MENU
      /*-------*-------*-------*---------*
       | Z-(0) | Y-(1) | Z+(2) | unit(3) |
       *-------*-------*-------*---------*
       | X-(4) | Y+(5) | X+(6) | back(7) |
       *-------*-------*-------*---------*
       |X+ X-  |Y+ Y-  |Z+ Z-            */
      {{6, 4}, {5, 1}, {2, 0}};
    #else
      /*-------*-------*-------*---------*
       | X+(0) | Y+(1) | Z+(2) | unit(3) |
       *-------*-------*-------*---------*
       | X-(4) | Y-(5) | Z-(6) | back(7) |
       *-------*-------*-------*---------*
       |X+ X-  |Y+ Y-  |Z+ Z-            */
      {{0, 4}, {1, 5}, {2, 6}};
    #endif

  if (!GET_BIT(infoSettings.inverted_axis, X_AXIS))
    LOAD_XYZ_LABEL_INDEX(table[X_AXIS][0], INC, table[X_AXIS][1], DEC, X);  // table[0] <--> INC(+) table[1] <--> DEC(+) if not inverted
  else
    LOAD_XYZ_LABEL_INDEX(table[X_AXIS][0], DEC, table[X_AXIS][1], INC, X);  // table[0] <--> DEC(-) table[1] <--> INC(-) if inverted

  if (!GET_BIT(infoSettings.inverted_axis, Y_AXIS))
    LOAD_XYZ_LABEL_INDEX(table[Y_AXIS][0], INC, table[Y_AXIS][1], DEC, Y);
  else
    LOAD_XYZ_LABEL_INDEX(table[Y_AXIS][0], DEC, table[Y_AXIS][1], INC, Y);

  if (!GET_BIT(infoSettings.inverted_axis, Z_AXIS))
    LOAD_XYZ_LABEL_INDEX(table[Z_AXIS][0], INC, table[Z_AXIS][1], DEC, Z);
  else
    LOAD_XYZ_LABEL_INDEX(table[Z_AXIS][0], DEC, table[Z_AXIS][1], INC, Z);

  moveItems.items[KEY_ICON_3] = itemMoveLen[item_moveLen_index];

  menuDrawPage(&moveItems);
  drawXYZ();

  while (MENU_IS(menuMove))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      #ifdef ALTERNATIVE_MOVE_MENU
        case KEY_ICON_0:   
          nowAxis = Z_AXIS;  // update now axis
          storeMoveCmd(nowAxis, -MIN(10,amount));
          replaceMoveBack(true);
          break;  // Z move down if no invert
        case KEY_ICON_1: 
          nowAxis = Y_AXIS;  // update now axis
          storeMoveCmd(nowAxis, amount); 
          replaceMoveBack(true);
          break;  // Y move decrease if no invert
        case KEY_ICON_2: 
          nowAxis = Z_AXIS;  // update now axis
          storeMoveCmd(nowAxis, MIN(10,amount)); 
          replaceMoveBack(true);
          break;   // Z move up if no invert

        case KEY_ICON_3:
          item_moveLen_index = (item_moveLen_index + 1) % ITEM_MOVE_LEN_NUM;
          moveItems.items[key_num] = itemMoveLen[item_moveLen_index];

          menuDrawItem(&moveItems.items[key_num], key_num);

          amount = moveLenSteps[item_moveLen_index];
          break;

        case KEY_ICON_4: 
          nowAxis = X_AXIS;
          storeMoveCmd(nowAxis, -amount); 
          replaceMoveBack(true);
          break;  // X move decrease if no invert
        case KEY_ICON_5: 
          nowAxis = Y_AXIS;
          storeMoveCmd(nowAxis, -amount); 
          replaceMoveBack(true);
          break;   // Y move increase if no invert
        case KEY_ICON_6: 
          nowAxis = X_AXIS;
          storeMoveCmd(nowAxis, amount); 
          replaceMoveBack(true);
          break;   // X move increase if no invert

        case KEY_ICON_7:
          laserReset();
          if (hadMovement) {
            replaceMoveBack(false);
            REPLACE_MENU(menuZero);
          }
          else
            CLOSE_MENU(); 
          break;
      #else
        case KEY_ICON_0: storeMoveCmd(X_AXIS, amount); break;   // X move increase if no invert
        case KEY_ICON_1: storeMoveCmd(Y_AXIS, amount); break;   // Y move increase if no invert
        case KEY_ICON_2: storeMoveCmd(Z_AXIS, MAX(10,amount)); break;   // Z move up if no invert

        case KEY_ICON_3:
          item_moveLen_index = (item_moveLen_index + 1) % ITEM_MOVE_LEN_NUM;
          moveItems.items[key_num] = itemMoveLen[item_moveLen_index];

          menuDrawItem(&moveItems.items[key_num], key_num);

          amount = moveLenSteps[item_moveLen_index];
          break;

        case KEY_ICON_4: storeMoveCmd(X_AXIS, -amount); break;  // X move decrease if no invert
        case KEY_ICON_5: storeMoveCmd(Y_AXIS, -amount); break;  // Y move decrease if no invert
        case KEY_ICON_6: storeMoveCmd(Z_AXIS, -MAX(10,amount)); break;  // Z move down if no invert

        case KEY_ICON_7: CLOSE_MENU(); break;
      #endif

        case KEY_INCREASE:
          storeMoveCmd(nowAxis, amount);
          break;

        case KEY_DECREASE:
          storeMoveCmd(nowAxis, -amount);
          break;

        default:
        #if LCD_ENCODER_SUPPORT
          if (encButtonReset) {
            if (LCD_Enc_ReadBtn(MENUSWITCHLIMIT))
              REPLACE_MENU(menuJog);
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
