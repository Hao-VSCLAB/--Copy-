#ifndef CONTROL
#define	CONTROL

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include "Setting.h"

void Control_Feedback(double Pos[AXIS], double Vel[AXIS], double PosCmd[AXIS], double(&TorCtrl)[AXIS]);

#endif