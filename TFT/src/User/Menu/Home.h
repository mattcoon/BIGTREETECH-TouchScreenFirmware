#ifndef _HOME_H_
#define _HOME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "coordinate.h"

void menuHome(void);

void ProbeTouchPlate(void);

void setPosition(AXIS axis, float position);
void setPositionA(float x, float y, float z);

#ifdef __cplusplus
}
#endif

#endif
