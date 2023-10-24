#ifndef TRACKING
#define TRACKING 

#include "Setting.h"
#include <iostream>
#include <stdio.h>

void Tracking_FirstPos(char *path, double(&FirstPos)[AXIS]);
void Tracking_OpenFile(char *path);
void Tracking_Command(double(&PosCmd)[AXIS], double(&VelCmd)[AXIS], double(&AccCmd)[AXIS], int(&EndFlag));
void Tracking_CloseFile();

#endif