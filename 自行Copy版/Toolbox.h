#ifndef TOOLBOX
#define TOOLBOX

#include <math.h>
#include <conio.h>
#include <stdio.h>
#include "Setting.h"


void Toolbox_LSF(double x[AXIS], double(&y)[AXIS]);
void Toolbox_LSF_Initialization(double(&PosCmd)[AXIS]);
void Toolbox_Filter(double x[AXIS], double(&y)[AXIS]);
double Toolbox_sign(double x);

//void Toolbox_LSF(double x, double(&y));
//void Toolbox_LSF_Initialization(double(&PosCmd));

#endif