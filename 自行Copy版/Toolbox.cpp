#include "Toolbox.h"
#include "MotionCard.h"

//================= LSF 1/4 =================


void Toolbox_LSF(double x, double(&y))
{
	static double xTemp[4] = { 0.0 };


	//---------------------- 將前一個時刻的資料向前移動 ---------------------- 
	for (int j = 0; j < 3; j++)
	{
		xTemp[j] = xTemp[j + 1];
	}
		
	//---------------------- 新資料填在最後 ---------------------- 
	xTemp[3] = x;

	//---------------------- 計算速度 ---------------------- 
	y = (0.3 * xTemp[3] + 0.1 * xTemp[2] - 0.1 * xTemp[1] - 0.3 * xTemp[0]) / SamplingTime;


}

//================= LSF Initialization =================

void Toolbox_LSF_Initialization(double(&PosCmd))
{

	double Vel = { 0.0 };

	for (int i = 0; i < 4; i++) // LSF更新至少需4次
	{

		MotionCard_Encoder(PosCmd); // 機構角度(rad)
		Toolbox_LSF(PosCmd, Vel); // 機構速度(rad/s)

	}

}