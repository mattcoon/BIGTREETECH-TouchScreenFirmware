#include "coordinate.h"
#include "includes.h"

static COORDINATE targetPosition = {{0.0f, 0.0f, 0.0f, 0.0f}, 3000};
static COORDINATE curPosition    = {{0.0f, 0.0f, 0.0f, 0.0f}, 3000};

const char axis_id[TOTAL_AXIS] = {'X', 'Y', 'Z', 'E'};
// cur2work is what position in current location needed to return to working 0,0
// machine coord = cur2workPosition + curPosition
static COORDINATE cur2workPosition = {{0.0f, 0.0f, 0.0f, 0.0f}, 3000};
E_AXIS_BACKUP eAxisBackup      = {0, 0, false, false};

/**
 * Obtained from "M114 E" instead of "M114", Because the coordinates of "M114" are not real-time coordinates.
 * It may be replaced by "M114 R"
 */
static float extruderPostion = 0.0f;

static bool relative_mode = false;
static bool relative_e = false;
// false means current position is unknown
// false after M18/M84 disable stepper or power up, true after G28
static bool position_known = false;
static bool axisKnown[TOTAL_AXIS] = {false,false,false,false}; 
static uint8_t coordUpdateSeconds = 0;
static bool coordSendingWaiting = false;

bool coorGetRelative(void)
{
  return relative_mode;
}

void coorSetRelative(bool mode)
{
  relative_mode = mode;
}

bool eGetRelative(void)
{
  return relative_e;
}

void eSetRelative(bool mode)
{
  relative_e = mode;
}

bool coordinateIsKnown(void)
{
  return position_known;
}

void coordinateSetKnown(bool known)
{
  position_known = known;
}

bool isAxisKnown(AXIS xyz) {
  return axisKnown[xyz];
}

void setAxisKnown(AXIS xyz, bool known) {
  axisKnown[xyz] =  known;
}

float coordinateGetAxisTarget(AXIS axis)
{
  return targetPosition.axis[axis];
}

void coordinateSetAxisTarget(AXIS axis, float position)
{
  if ((axis == E_AXIS) ? relative_e : relative_mode)
  {
    targetPosition.axis[axis] += position;
  }
  else
  {
    targetPosition.axis[axis] = position;
  }
}

uint32_t coordinateGetFeedRate(void)
{
  return targetPosition.feedrate;
}

void coordinateSetFeedRate(uint32_t feedrate)
{
  targetPosition.feedrate = feedrate;
}

void coordinateGetAll(COORDINATE * tmp)
{
  memcpy(tmp, &targetPosition, sizeof(targetPosition));
}

float coordinateGetExtruderActual(void)
{
  return extruderPostion;
}

void coordinateSetExtruderActualSteps(float steps)
{
  curPosition.axis[E_AXIS] = extruderPostion = steps / getParameter(P_STEPS_PER_MM, E_AXIS);
}

float coordinateGetAxisActual(AXIS axis)
{
  return curPosition.axis[axis];
}

void coordinateSetAxisActual(AXIS axis, float position)
{
  curPosition.axis[axis] = position;
}

void coordinateGetAllActual(COORDINATE * tmp)
{
  memcpy(tmp, &curPosition, sizeof(curPosition));
}

float coordinateGetAxis(AXIS axis)
{
  if (infoFile.source >= FS_ONBOARD_MEDIA)
    return coordinateGetAxisActual(axis);
  else
    return coordinateGetAxisTarget(axis);
}

void coordinateGetAbsActual(COORDINATE *tmp)
{
  tmp->axis[X_AXIS] = curPosition.axis[X_AXIS] + cur2workPosition.axis[X_AXIS];
  tmp->axis[Y_AXIS] = curPosition.axis[Y_AXIS] + cur2workPosition.axis[Y_AXIS];
  tmp->axis[Z_AXIS] = curPosition.axis[Z_AXIS] + cur2workPosition.axis[Z_AXIS];
}

float coordinateGetAbsAxis(AXIS axis) {
  return cur2workPosition.axis[axis] + curPosition.axis[axis];
}

void cur2workSetAxis(AXIS axis, float position) {
  if (Z_AXIS != axis)  //TODO: probe causes issue with move limits
    cur2workPosition.axis[axis] = position;
}

// to be called on setting of axis to zero
void cur2workSetAxisSet(AXIS axis) {
  if (Z_AXIS != axis)  //TODO: probe causes issue with move limits
    cur2workPosition.axis[axis] = curPosition.axis[axis];
}

void cur2workSetAxisCur(AXIS axis){
  cur2workPosition.axis[axis] += curPosition.axis[axis];
}

void cur2workSetAll(COORDINATE *tmp){
  memcpy(tmp, &cur2workPosition, sizeof(cur2workPosition));
}

void coordinateQueryClearSendingWaiting(void)
{
  coordSendingWaiting = false;
}

/**
 * @brief Query gantry position
 * @param seconds: Pass 0 to query manually or disable auto report. Pass delay in seconds
 *                 for auto query if available in marlin
 */
void coordinateQuery(uint8_t seconds)
{ // following conditions ordered by importance
  if (!coordSendingWaiting && infoHost.tx_slots != 0 && infoHost.connected && infoMachineSettings.firmwareType != FW_REPRAPFW)
  {
    if (infoMachineSettings.autoReportPos == 1)  // if auto report is enabled
    {
      if (seconds == 0)  // if manual querying is requested (if query interval is 0)
        coordSendingWaiting = storeCmd("M114\n");

      if (seconds != coordUpdateSeconds)  // if query interval is changed
      {
        if (storeCmd("M154 S%d\n", seconds))  // turn on or off (if query interval is 0) auto report
          coordUpdateSeconds = seconds;       // if gcode will be sent, avoid to enable auto report again on next
      }                                       // function call if already enabled for that query interval
    }
    else  // if auto report is disabled
    {
      coordSendingWaiting = storeCmd("M114\n");
    }
  }
}

void coordinateQueryTurnOff(void)
{
  coordSendingWaiting = false;

  if (infoMachineSettings.autoReportPos == 1)  // if auto report is enabled, turn it off
  {
    storeCmd("M154 S0\n");
    coordUpdateSeconds = 0;
  }
}
