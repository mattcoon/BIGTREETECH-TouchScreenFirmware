#ifndef _LASER_CONTROL_H_
#define _LASER_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "Settings.h"

extern const char* laserCmd;

void laserReset(void);
void laserResetSpeed(void);
void laserSetSpeed(const uint8_t speed);
uint8_t laserGetSetSpeed(void);
void laserSetPercent(const uint8_t percent);
uint8_t laserGetSetPercent(void);
void laserSetCurSpeed(const uint8_t speed);
uint8_t laserGetCurSpeed(void);
void laserSetCurPercent(const uint8_t percent);
uint8_t laserGetCurPercent(void);
void loopLaser(void);
void ctrlLaserQuerySetWait(const bool wait);
void ctrlLaserQuery(void);

#ifdef __cplusplus
}
#endif

#endif
