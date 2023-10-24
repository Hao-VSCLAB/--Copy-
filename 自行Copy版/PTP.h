#ifndef PTP
#define PTP

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "Setting.h"

void PTP_Scurve(double InitialPos[AXIS], double FinalPos[AXIS], double(&PosCmd)[AXIS], double(&VelCmd)[AXIS], double(&AccCmd)[AXIS], int(&EndFlag));

#endif