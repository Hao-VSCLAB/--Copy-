#ifndef MOTIONCARD
#define MOTIONCARD

#include "C:\\IMP3\\IMCDriver.h"
#include "C:\\IMP3\\IMCDefine.h"
#include <stdio.h>
#include "Setting.h"

void MotionCard_OpenCard();
void MotionCard_CloseCard();
void MotionCard_ServoOn();
void MotionCard_ServoOff();
//void MotionCard_Encoder(double(&JointEncRad));
void MotionCard_Encoder(double(&RobotPosRad[AXIS]));
//void MotionCard_DAC(double(&JointTorCtrl));
//void MotionCard_DAC(double(&JointTorCtrl), double(&Voltage));
void MotionCard_DAC(double RobotTorCtrl[AXIS]);
void MotionCard_ChangeTimer(TMRISR Timer);

#endif