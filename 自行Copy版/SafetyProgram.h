#ifndef SAFETYPROGRAM
#define SAFETYPROGRAM

#include "Settings.h"

void Protection(double Pos[AXIS], double Vel[AXIS], double TotalTorque[AXIS], int(&SafetyFlag));

#endif
