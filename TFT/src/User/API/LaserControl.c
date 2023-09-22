#include "FanControl.h"
#include "includes.h"

#define NEXT_FAN_WAIT 500  // 1 second is 1000

const char* laserCmd = LASER_CMD;

static uint8_t setLaserSpeed = 0;
static uint8_t curLaserSpeed = 0;
static bool needSetLaserSpeed = false;

static bool ctrlLaserQueryWait = false;
static uint32_t nextCtrlLaserTime = 0;

void laserResetSpeed(void)
{
  needSetLaserSpeed = false;
  setLaserSpeed = 0;
  curLaserSpeed = 0;
}

void laserSetSpeed(const uint8_t speed)
{
  if (laserGetCurSpeed() != speed) {
    needSetLaserSpeed = true;
    setLaserSpeed = speed;
  }
}

uint8_t laserGetSetSpeed(void)
{
  return setLaserSpeed;
}

void laserSetPercent(const uint8_t percent)
{
  laserSetSpeed((NOBEYOND(0, percent, 100) * infoSettings.fan_max[0]) / 100);
}

uint8_t laserGetSetPercent(void)
{
  return (setLaserSpeed * 100.0f) / infoSettings.fan_max[0] + 0.5f;
}

void laserSetCurSpeed(const uint8_t speed)
{
  curLaserSpeed = speed;
}

uint8_t laserGetCurSpeed(void)
{
  return curLaserSpeed;
}

void laserSetCurPercent(const uint8_t percent)
{
  curLaserSpeed = (NOBEYOND(0, percent, 100) * infoSettings.fan_max[0]) / 100;
}

uint8_t laserGetCurPercent(void)
{
  return (curLaserSpeed * 100.0f) / infoSettings.fan_max[0] + 0.5f;
}

void loopLaser(void)
{
  if (needSetLaserSpeed && (OS_GetTimeMs() > nextCtrlLaserTime))
  {
    if (storeCmd(laserCmd, setLaserSpeed))
    {
      needSetLaserSpeed = false;
    }

    nextCtrlLaserTime = OS_GetTimeMs() + NEXT_FAN_WAIT;  // avoid rapid fire, clogging the queue
  }
}

void ctrlLaserQuerySetWait(const bool wait)
{
  ctrlLaserQueryWait = wait;
}

// query for controller fan only
void ctrlLaserQuery(void)
{
  if (infoHost.connected && !infoHost.wait && !ctrlLaserQueryWait && infoSettings.ctrl_fan_en)
  {
    ctrlLaserQueryWait = storeCmd("M710\n");
  }
}
