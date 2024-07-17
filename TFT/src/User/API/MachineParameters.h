#ifndef _MACHINE_PARAMETERS_H_
#define _MACHINE_PARAMETERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "includes.h"
#include "menu.h"

typedef enum
{
  P_STEPS_PER_MM = 0,
  P_FILAMENT_DIAMETER,
  P_MAX_ACCELERATION,
  P_MAX_FEED_RATE,
  P_ACCELERATION,
  P_JERK,
  P_JUNCTION_DEVIATION,
  P_HOME_OFFSET,
  P_FWRETRACT,
  P_FWRECOVER,
  P_AUTO_RETRACT,
  P_HOTEND_OFFSET,
  P_HOTEND_PID,
  P_BED_PID,
  P_ABL_STATE,
  P_STEALTH_CHOP,
  P_INPUT_SHAPING,
  P_DELTA_CONFIGURATION,
  P_DELTA_TOWER_ANGLE,
  P_DELTA_DIAGONAL_ROD,
  P_DELTA_ENDSTOP,
  P_PROBE_OFFSET,
  P_LIN_ADV,
  P_CURRENT,
  P_HYBRID_THRESHOLD,
  P_BUMPSENSITIVITY,
  P_MBL_OFFSET,
  P_BED_SIZE,
  P_MACHINE_MIN,
  P_MACHINE_MAX,
  // keep below items always at the end
  PARAMETERS_COUNT,
} PARAMETER_NAME;

typedef enum
{
  P_SAVE_SETTINGS = 0,
  P_RESTORE_SETTINGS,
  P_RESET_SETTINGS,
  P_SETTINGS_COUNT,
} PARAMETER_SETTINGS;

typedef enum
{
  AXIS_INDEX_X = 0,
  AXIS_INDEX_Y,
  AXIS_INDEX_Z,
  AXIS_INDEX_E0,
  AXIS_INDEX_E1,
  AXIS_INDEX_E2,
  AXIS_INDEX_COUNT
} AXIS_INDEX;

typedef enum
{
  STEPPER_INDEX_X = 0,
  STEPPER_INDEX_X2,
  STEPPER_INDEX_Y,
  STEPPER_INDEX_Y2,
  STEPPER_INDEX_Z,
  STEPPER_INDEX_Z2,
  STEPPER_INDEX_Z3,
  STEPPER_INDEX_Z4,
  STEPPER_INDEX_E0,
  STEPPER_INDEX_E1,
  STEPPER_INDEX_E2,
  STEPPER_INDEX_COUNT
} STEPPER_INDEX;

typedef enum
{
  VAL_TYPE_INT,
  VAL_TYPE_NEG_INT,
  VAL_TYPE_FLOAT,
  VAL_TYPE_NEG_FLOAT,
} VAL_TYPE;

extern uint16_t endStopValue[6];  // x, y, y2, z, z2, z probe

// param attributes multi purpose hard coded labels
extern char * const axisDisplayID[AXIS_INDEX_COUNT];
extern char * const stepperDisplayID[STEPPER_INDEX_COUNT];

// param attributes hard coded labels
extern char * const filamentDiaDisplayID[];
extern char * const autoRetractDisplayID[];
extern char * const hotendPidDisplayID[];
extern char * const bedPidDisplayID[];
extern char * const ablStateDisplayID[];
extern char * const stealthChopDisplayID[];
extern char * const inputShapingDisplayID[];
extern char * const deltaConfigurationDisplayID[];
extern char * const deltaTowerAngleDisplayID[];
extern char * const deltaDiagonalRodDisplayID[];
extern char * const deltaEndstopDisplayID[];
extern char * const linAdvDisplayID[];
extern char * const BedSizeDisplayID[];
extern char * const machineMinDisplayID[];
extern char * const machineMaxDisplayID[];
// param attributes configurable labels
extern const LABEL accelDisplayID[];
extern const LABEL junctionDeviationDisplayID[];
extern const LABEL retractDisplayID[];
extern const LABEL recoverDisplayID[];

typedef struct {
  const char * const suffix;
  const VAL_TYPE valType;
} suffix_t;

typedef struct {
  const char * const prefix;
  const suffix_t * const suffix;
  const uint8_t cnt;
  uint16_t enabled;  // parameterElementCount must be less than 16
} parameter_member_t;

// Steps/mm (X, Y, Z, E0, E1, E2)
static const suffix_t M92_suffix[]= {
  {"X%.4f\n",    VAL_TYPE_FLOAT}, {"Y%.4f\n",    VAL_TYPE_FLOAT}, {"Z%.2f\n", VAL_TYPE_FLOAT},
  {"T0 E%.2f\n", VAL_TYPE_FLOAT}, {"T1 E%.2f\n", VAL_TYPE_FLOAT}, {"T2 E%.2f\n", VAL_TYPE_FLOAT},
};
// Filament Diameter (Enable, E0, E1, E2)
static const suffix_t M200_suffix[]= {
  {"S%.0f\n",       VAL_TYPE_INT},   {"S1 T0 D%.2f\n", VAL_TYPE_FLOAT}, {"S1 T1 D%.2f\n", VAL_TYPE_FLOAT},
  {"S1 T2 D%.2f\n", VAL_TYPE_FLOAT},
};
// MaxAcceleration (X, Y, Z, E0, E1, E2)
static const suffix_t M201_suffix[]= {
  {"X%.0f\n",    VAL_TYPE_INT}, {"Y%.0f\n",    VAL_TYPE_INT}, {"Z%.0f\n",    VAL_TYPE_INT},
  {"T0 E%.0f\n", VAL_TYPE_INT}, {"T1 E%.0f\n", VAL_TYPE_INT}, {"T2 E%.0f\n", VAL_TYPE_INT},
};
// MaxFeedrate (X, Y, Z, E0, E1, E2)
static const suffix_t M203_suffix[]= {
  {"X%.0f\n",    VAL_TYPE_INT}, {"Y%.0f\n",    VAL_TYPE_INT}, {"Z%.0f\n",    VAL_TYPE_INT},
  {"T0 E%.0f\n", VAL_TYPE_INT}, {"T1 E%.0f\n", VAL_TYPE_INT}, {"T2 E%.0f\n", VAL_TYPE_INT},
};
// Acceleration (Print, Retract, Travel)
static const suffix_t M204_suffix[]= {
  {"P%.0f\n", VAL_TYPE_INT}, {"R%.0f\n", VAL_TYPE_INT}, {"T%.0f\n", VAL_TYPE_INT},
};
// Jerk (X, Y, Z, E)
static const suffix_t M205_jerk_suffix[]= {
  {"X%.0f\n", VAL_TYPE_INT},   {"Y%.0f\n", VAL_TYPE_INT}, {"Z%.2f\n", VAL_TYPE_FLOAT},
  {"E%.2f\n", VAL_TYPE_FLOAT},
};
// Junction Deviation (X, Y, Z, E)
static const suffix_t M205_deviation_suffix[]= {
  {"J%.3f\n", VAL_TYPE_FLOAT},
};
// Home offset (X, Y, Z)
static const suffix_t M206_suffix[]= {
  {"X%.2f\n", VAL_TYPE_NEG_FLOAT}, {"Y%.2f\n", VAL_TYPE_NEG_FLOAT}, {"Z%.2f\n", VAL_TYPE_NEG_FLOAT},
};
// FW retract (Length, Swap Length, Feedrate, Z lift height)
static const suffix_t M207_suffix[]= {
  {"S%.2f\n", VAL_TYPE_FLOAT}, {"W%.2f\n", VAL_TYPE_FLOAT}, {"F%.2f\n", VAL_TYPE_INT},
  {"Z%.2f\n", VAL_TYPE_FLOAT},
};
// FW retract recover (Additional length, Additional Swap Length, Feedrate, Swap feedrate)
static const suffix_t M208_suffix[]= {
  {"S%.2f\n", VAL_TYPE_FLOAT}, {"W%.2f\n", VAL_TYPE_FLOAT}, {"F%.2f\n", VAL_TYPE_INT},
  {"R%.2f\n", VAL_TYPE_INT},
};
// Set auto FW retract
static const suffix_t M209_suffix[]= {
  {"S%.0f\n", VAL_TYPE_INT}
};
// Hotend Offset (X, Y, Z)
static const suffix_t M218_suffix[]= {
  {"T1 X%.2f\n", VAL_TYPE_NEG_FLOAT}, {"T1 Y%.2f\n", VAL_TYPE_NEG_FLOAT}, {"T1 Z%.2f\n", VAL_TYPE_NEG_FLOAT},
};
// Hotend PID
static const suffix_t M301_suffix[]= {
  {"P%.4f\n", VAL_TYPE_FLOAT}, {"I%.4f\n", VAL_TYPE_FLOAT}, {"D%.4f\n", VAL_TYPE_FLOAT},
};
// Bed PID
static const suffix_t M304_suffix[]= {
  {"P%.4f\n", VAL_TYPE_FLOAT}, {"I%.4f\n", VAL_TYPE_FLOAT}, {"D%.4f\n", VAL_TYPE_FLOAT},
};
// ABL State + Z Fade
static const suffix_t M420_suffix[]= {
  {"S%.0f\n", VAL_TYPE_INT}, {"Z%.2f\n", VAL_TYPE_FLOAT},
};
// TMC StealthChop (X, X2, Y, Y2, Z, Z2, Z3, Z4, E0, E1, E2)
static const suffix_t M569_suffix[]= {
  {"S%.0f I0 X\n", VAL_TYPE_INT}, {"S%.0f I1 X\n", VAL_TYPE_INT}, {"S%.0f I0 Y\n", VAL_TYPE_INT},
  {"S%.0f I1 Y\n", VAL_TYPE_INT}, {"S%.0f I0 Z\n", VAL_TYPE_INT}, {"S%.0f I1 Z\n", VAL_TYPE_INT},
  {"S%.0f I2 Z\n", VAL_TYPE_INT}, {"S%.0f I3 Z\n", VAL_TYPE_INT}, {"S%.0f T0 E\n", VAL_TYPE_INT},
  {"S%.0f T1 E\n", VAL_TYPE_INT}, {"S%.0f T2 E\n", VAL_TYPE_INT},
};
// Input Shape (X damping frequency, X damping factor, Y damping frequency, Y damping factor)
static const suffix_t M593_suffix[]= {
  {"X F%.3f\n", VAL_TYPE_FLOAT}, {"X D%.3f\n", VAL_TYPE_FLOAT}, {"Y F%.3f\n", VAL_TYPE_FLOAT},
  {"Y D%.3f\n", VAL_TYPE_FLOAT},
};
// Delta Configuration (Height, Segment per sec, Radius, Diagonal Rod)
static const suffix_t M665_hsrl_suffix[]= {
  {"H%.4f\n", VAL_TYPE_FLOAT}, {"S%.4f\n", VAL_TYPE_FLOAT}, {"R%.4f\n", VAL_TYPE_FLOAT},
  {"L%.4f\n", VAL_TYPE_FLOAT},
};
// Delta Tower Angle (Tx, Ty, Tz)
static const suffix_t M665_xyz_suffix[]= {
  {"X%.4f\n", VAL_TYPE_NEG_FLOAT}, {"Y%.4f\n", VAL_TYPE_NEG_FLOAT}, {"Z%.4f\n", VAL_TYPE_NEG_FLOAT},
};
// Delta Diagonal Rod Trim (Dx, Dy, Dz)
static const suffix_t M665_abc_suffix[]= {
  {"A%.4f\n", VAL_TYPE_NEG_FLOAT}, {"B%.4f\n", VAL_TYPE_NEG_FLOAT}, {"C%.4f\n", VAL_TYPE_NEG_FLOAT},
};
// Delta Endstop Adjustments (Ex, Ey, Ez)
static const suffix_t M666_suffix[]= {
  {"X%.4f\n", VAL_TYPE_NEG_FLOAT}, {"Y%.4f\n", VAL_TYPE_NEG_FLOAT}, {"Z%.4f\n", VAL_TYPE_NEG_FLOAT},
};
// Probe offset (X, Y, Z)
static const suffix_t M851_suffix[]= {
  {"X%.2f\n", VAL_TYPE_NEG_FLOAT}, {"Y%.2f\n", VAL_TYPE_NEG_FLOAT}, {"Z%.2f\n", VAL_TYPE_NEG_FLOAT},
};
// Linear Advance (E0, E1, E2)
static const suffix_t M900_suffix[]= {
  {"T0 K%.3f\n", VAL_TYPE_FLOAT}, {"T1 K%.3f\n", VAL_TYPE_FLOAT}, {"T2 K%.3f\n", VAL_TYPE_FLOAT},
};
// Stepper Motor Current (X, X2, Y, Y2, Z, Z2, Z3, Z4, E0, E1, E2)
static const suffix_t M906_suffix[]= {
  {"I0 X%.0f\n", VAL_TYPE_INT}, {"I1 X%.0f\n", VAL_TYPE_INT}, {"I0 Y%.0f\n", VAL_TYPE_INT},
  {"I1 Y%.0f\n", VAL_TYPE_INT}, {"I0 Z%.0f\n", VAL_TYPE_INT}, {"I1 Z%.0f\n", VAL_TYPE_INT},
  {"I2 Z%.0f\n", VAL_TYPE_INT}, {"I3 Z%.0f\n", VAL_TYPE_INT}, {"T0 E%.0f\n", VAL_TYPE_INT},
  {"T1 E%.0f\n", VAL_TYPE_INT}, {"T2 E%.0f\n", VAL_TYPE_INT},
};
// TMC Hybrid Threshold Speed (X, X2, Y, Y2, Z, Z2, Z3, Z4, E0, E1, E2)
static const suffix_t M913_suffix[]= {
  {"I1 X%.0f\n", VAL_TYPE_INT}, {"I2 X%.0f\n", VAL_TYPE_INT}, {"I1 Y%.0f\n", VAL_TYPE_INT},
  {"I2 Y%.0f\n", VAL_TYPE_INT}, {"I1 Z%.0f\n", VAL_TYPE_INT}, {"I2 Z%.0f\n", VAL_TYPE_INT},
  {"I3 Z%.0f\n", VAL_TYPE_INT}, {"I4 Z%.0f\n", VAL_TYPE_INT}, {"T0 E%.0f\n", VAL_TYPE_INT},
  {"T1 E%.0f\n", VAL_TYPE_INT}, {"T2 E%.0f\n", VAL_TYPE_INT},
};
// TMC Bump Sensitivity (X, X2, Y, Y2, Z, Z2, Z3, Z4)
static const suffix_t M914_suffix[]= {
  {"I1 X%.0f\n", VAL_TYPE_NEG_INT}, {"I2 X%.0f\n", VAL_TYPE_NEG_INT}, {"I1 Y%.0f\n", VAL_TYPE_NEG_INT},
  {"I2 Y%.0f\n", VAL_TYPE_NEG_INT}, {"I1 Z%.0f\n", VAL_TYPE_NEG_INT}, {"I2 Z%.0f\n", VAL_TYPE_NEG_INT},
  {"I3 Z%.0f\n", VAL_TYPE_NEG_INT}, {"I4 Z%.0f\n", VAL_TYPE_NEG_INT},
};
// MBL offset
static const suffix_t G29_suffix[]= {
  {"S4 Z%.2f\nG29 S0\n", VAL_TYPE_NEG_FLOAT},
};
// Bed Size (X, Y, Z)
static const suffix_t C102_suffix[]= {
  {"X%.0f\n", VAL_TYPE_FLOAT}, {"Y%.0f\n", VAL_TYPE_FLOAT}, {"Z%.0f\n", VAL_TYPE_FLOAT},
};
// Machine Min (X, Y, Z)
static const suffix_t C100_suffix[]= {
  {"X%.0f\n", VAL_TYPE_NEG_FLOAT}, {"Y%.0f\n", VAL_TYPE_NEG_FLOAT}, {"Z%.0f\n", VAL_TYPE_NEG_FLOAT},
};
static const suffix_t C101_suffix[]= {
  {"X%.0f\n", VAL_TYPE_NEG_FLOAT}, {"Y%.0f\n", VAL_TYPE_NEG_FLOAT}, {"Z%.0f\n", VAL_TYPE_NEG_FLOAT},
};


typedef struct {
  float StepsPerMM[COUNT(M92_suffix)];
  float FilamentSetting[COUNT(M200_suffix)];
  float MaxAcceleration[COUNT(M201_suffix)];
  float MaxFeedRate[COUNT(M203_suffix)];
  float Acceleration[COUNT(M204_suffix)];
  float Jerk[COUNT(M205_jerk_suffix)];
  float JunctionDeviation[COUNT(M205_deviation_suffix)];
  float HomeOffset[COUNT(M206_suffix)];
  float FwRetract[COUNT(M207_suffix)];
  float FwRecover[COUNT(M208_suffix)];
  float AutoRetract[COUNT(M209_suffix)];
  float HotendOffset[COUNT(M218_suffix)];
  float HotendPid[COUNT(M301_suffix)];
  float BedPid[COUNT(M304_suffix)];
  float ABLState[COUNT(M420_suffix)];
  float StealthChop[COUNT(M569_suffix)];
  float InputShaping[COUNT(M593_suffix)];
  float DeltaConfiguration[COUNT(M665_hsrl_suffix)];
  float DeltaTowerAngle[COUNT(M665_xyz_suffix)];
  float DeltaDiagonalRod[COUNT(M665_abc_suffix)];
  float DeltaEndstop[COUNT(M666_suffix)];
  float ProbeOffset[COUNT(M851_suffix)];
  float LinAdvance[COUNT(M900_suffix)];
  float Current[COUNT(M906_suffix)];
  float HybridThreshold[COUNT(M913_suffix)];
  float BumpSensitivity[COUNT(M914_suffix)];
  float MblOffset[COUNT(G29_suffix)];
  float BedSize[COUNT(C102_suffix)];
  float MachineMin[COUNT(C100_suffix)];
  float MachineMax[COUNT(C101_suffix)];
} PARAMETERS;

extern PARAMETERS infoParameters;

void infoParametersRefreshBackup(void);
bool infoParametersHasChange(void);

// get enable element count for the parameter
uint8_t getEnabledElementCount(PARAMETER_NAME name);

// get element index out of total enabled enabled element.
// If no element is enabled, total element count is returned
uint8_t getEnabledElement(PARAMETER_NAME name, uint8_t index);

// get total enabled parameters
uint8_t getEnabledParameterCount(void);

// get PARAMETER_NAME of selected index out of total enabled parameters.
// If no parameter is enabled, total parameter count is returned
PARAMETER_NAME getEnabledParameter(uint8_t index);

float getParameter(PARAMETER_NAME name, uint8_t index);
void setParameter(PARAMETER_NAME name, uint8_t index, float val);

// get total elements in a parameter
uint8_t getElementCount(PARAMETER_NAME para);

// get type of value a parameter element holds
VAL_TYPE getParameterValType(PARAMETER_NAME para, uint8_t index);

// send parameter cmd (parameter value gets updated after the cmd passes through the cmd cache)
void sendParameterCmd(PARAMETER_NAME para_index, uint8_t stepper_index, float Value);

// save parameter setting to eeprom
void saveEepromSettings(void);

// restore settings from eeprom
void restoreEepromSettings(void);

// reset settings and eeprom to default values
void resetEepromSettings(void);

#ifdef __cplusplus
}
#endif

#endif
