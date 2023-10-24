#include "Control.h"


//=================���=============//

void Control_Feedback(double Pos[AXIS], double Vel[AXIS], double PosCmd[AXIS], double(&TorCtrl)[AXIS])
{


	double Kp[AXIS] = {200.0, 200.0, };
	double Kv[AXIS] = { 7.50,  7.50, };


	//int i = AXIS - 1;
	for (int i = 0; i < AXIS; i++)
	{
		TorCtrl[i] = Kv[i] * (Kp[i] * (PosCmd[i] - Pos[i]) - Vel[i]); // ��x����R�O(Nm)
	}
}