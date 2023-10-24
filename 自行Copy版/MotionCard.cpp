#include "MotionCard.h"
#include <iostream>
#include <math.h>

#include "IMCDefine.h"
#include "IMCDriver.h"

//==================== 參數設定 ====================

const double CircleRad = 2.0 * PI;
const double RatedTorque[AXIS] = { 1.27 , 1.27  };			// 馬達額定轉矩
const double GearRatio[AXIS] = { 50.0 ,  50.0 };			// 減速比
const double PulsePerCycle[AXIS] = { 10000.0 , 10000.0 };	//編碼器解析度(ENC_RATE_X4)
const double MaxVoltage =  10;		            // 最大電壓

//const double Direction[AXIS] = { -1, -1 };								
//const double MaxTorque[AXIS] = { 1.27 * 1, 1.27 };

//==================== 開啟軸卡 ===================
void MotionCard_OpenCard()
{
	//  --- 軸卡初始化 ---
	/*  Bool IMC_OpenDevice(		// true：Initial IMP 模組成功 false：Initial IMP 模組失敗
	int nMode,			// 0：初始化 IMP 的 PCI 模式  1：初始化 IMP 的 Standalone 模式
	WORD wCardIndex)		*/

	int nRet = IMC_OpenDevice(0, 0);  //   --- 軸卡初始化 ---

	if (nRet == 0)
	{

		printf("Initialization Fails !");

	}
	else
	{
	
		// ======================= 變動版 ========================== // 
		
		IMC_GLB_ResetModule(RESET_ALL, 0);   // 重置 IMP 模組
		IMC_GLB_SetInterruptMask(IMC_ALL_INT_UNMASK, 0);  // UnMask Interrupt
		
		//----------------- 編碼器 -----------------

		for (int i = 0; i < AXIS; i++)
		{

			IMC_ENC_EnableInAInverse(motor[i], 0, 0);	// 0->encoder counter channel in A腳位輸入訊號不反相
			IMC_ENC_EnableInBInverse(motor[i], 0, 0);	// 0->encoder counter channel in B腳位輸入訊號不反相
			IMC_ENC_EnableInCInverse(motor[i], 0, 0);	// 0->encoder counter channel in C腳位輸入訊號不反相
			IMC_ENC_EnableInABSwap(motor[i], 0, 0); // 0->A/B輸入訊號不對調

			IMC_ENC_SetInputFormat(motor[i], ENC_FMT_AB, 0); // 設定編碼器輸入格式為 A/B 型
			IMC_ENC_SetInputRate(motor[i], ENC_RATE_X4, 0); // 設定編碼器輸入解碼倍率 x4
			IMC_ENC_ClearCounter(motor[i], 1, 0); // 開啟清除編碼器
			IMC_ENC_ClearCounter(motor[i], 0, 0); // 關閉清除編碼器
			IMC_ENC_StartCounter(motor[i], 1, 0); // 開始編碼器輸入 

		}


		//---------------- DAC & ADC ----------------

		for (int i = 0; i < AXIS; i++)
		{
			IMC_DAC_SelectSource(motor[i], DAC_SOURCE_SOFT, 0); // 設定 DAC 命令源為軟體規劃
			IMC_DAC_EnableChannel(motor[i], 1, 0); // 開啟 DAC 功能
			IMC_ADC_EnableChannel(motor[i], 1, 0); // 開啟 ADC 功能

		}

		IMC_ADC_SetConverterMode(2, 0);  

		IMC_DAC_StartConverter(1, 0); // 開始 DAC 轉換
		IMC_ADC_StartConverter(1, 0); // 開始 ADC 轉換

		for (int i = 0; i < AXIS; i++)
		{

			IMC_DAC_SetOutputVoltage(motor[i], 0.0, 0); // DAC 電壓清空

		}

		//---------------- 中斷功能 ----------------

		// --- 設定計時器計時時間 1000 us (1豪秒) ---
		/* IMC_TMR_SetTimer(
		float dfPeriod,			// 計時器時間(µs)，可設定範圍 (0 ~ 2^32 毫秒)
		WORD wCardIndex) */
		IMC_TMR_SetTimer(1000, 0);   // 設定計時器計時時間 1000us (1豪秒)

		// --- 開啟計時器計時功能 ---
		/* IMC_TMR_SetTimerEnable(
		WORD wEnable,			// 1 開啟或 0 關閉計時器功能
		WORD wCardIndex) */
		IMC_TMR_SetTimerEnable(1, 0);  // 開啟計時器計時功能

		// --- 開啟計時器中斷功能 ---
		/* IMC_TMR_SetTimerIntEnable(
		WORD wEnable,			// 1 開啟或 0 關閉計時器中斷功能
		WORD wCardIndex) */
		IMC_TMR_SetTimerIntEnable(1, 0);   // 開啟計時器中斷功能


		//-------------- Servo On/off --------------

		for (int i = 0; i < AXIS; i++)
		{

			IMC_LIO_EnableServoOnOff(motor[i], 1, 0);

		}

	}

}

//==================== 關閉軸卡 ===================
void MotionCard_CloseCard()
{
	//---------------- Servo On/off ----------------

	for (int i = 0; i < AXIS; i++)
	{

		IMC_LIO_EnableServoOnOff(motor[i], 0, 0);

	}

	IMC_TMR_SetTimerIntEnable(0, 0); // 關閉計時器中斷功能
	IMC_TMR_SetTimerEnable(0, 0); // 關閉計時器計時功能

	//------------ DAC & ADC & 編碼器 ------------

	for (int i = 0; i < AXIS; i++)
	{

		IMC_DAC_SetOutputVoltage(motor[i], 0.0, 0); // DAC電壓清空

	}

	IMC_DAC_StartConverter(0, 0); // 停止 DAC 轉換
	IMC_ADC_StartConverter(0, 0); // 停止 ADC 轉換


	for (int i = 0; i < AXIS; i++)
	{

		IMC_ENC_StartCounter(motor[i], 0, 0);  // 停止編碼器輸入
		IMC_DAC_EnableChannel(motor[i], 0, 0); // DAC中斷功能
		IMC_ADC_EnableChannel(motor[i], 0, 0); // ADC中斷功能

	}


	// --- 關閉 IMP 模組 ---
	/* IMC_CloseIfOpen(
	WORD wCardIndex) */
	IMC_CloseIfOpen(0);
}




//================== Servo On ======================
void MotionCard_ServoOn()
{

	for (int i = 0; i < AXIS; i++)
	{

		IMC_LIO_SetServoOn(motor[i], 0);

		printf("Joint %d servo on!\n", motor[i]);

	}

}

//================== Servo Off =====================
void MotionCard_ServoOff()
{

	for ( int i = 0 ; i < AXIS ; i++ )
	{

		IMC_LIO_SetServoOff( motor[i] , 0 ) ;

	}

}

// ================ 編碼器設定 ===================
void MotionCard_Encoder( double ( & RobotPosRad)[AXIS] )
//void MotionCard_Encoder(double(&JointEncRad))
{
	
	//MotorINCRRad = ((double)MotorINCRPulse / 10000.0) * CircleRad; // "馬達端"增量型編碼器數值[Rad] , 增量型編碼器設定一圈 2500*4個Pulse , 驅動器參數P1-46*軸卡4倍頻
	//JointEncRad = Direction[i] * (MotorINCRRad / GearRatio[i]);   
	
	long MotorEncPulse[AXIS] = { 0 } ;

	for (int i = 0; i < AXIS; i++)
	{

		IMC_ENC_ReadCounter(motor[i], &MotorEncPulse[i], 0); // 讀取增量型編碼器值
		RobotPosRad[i] = ((double)MotorEncPulse[i] / PulsePerCycle[i] / GearRatio[i]) * CircleRad;
		// 人體下肢髖關節(Delta A3)：機構角度(Rad)，機構一圈2500個Pulse(可以驅動器參數 P1 - 46 調整) 
		// 外骨骼髖關節(Delta A3)  :機構角度(Rad)，機構一圈2500個Pulse(可以驅動器參數 P1 - 46 調整)

	}

}

void MotionCard_DAC( double RobotTorCtrl[AXIS] )
//void MotionCard_DAC(double(&JointTorCtrl))
{
	for ( int i = 0 ; i < AXIS ; i++ )
	{

		double MotorTorCtrl = RobotTorCtrl[i] / GearRatio[i]; // 馬達轉矩控制命令(Nm)

		if (MotorTorCtrl > RatedTorque[i])
		{
			MotorTorCtrl = RatedTorque[i]; // 馬達轉矩限制
		}

		if (MotorTorCtrl < -RatedTorque[i])
		{
			MotorTorCtrl = -RatedTorque[i]; // 馬達轉矩限制
		}

		double ControlSignal = (MotorTorCtrl / RatedTorque[i]) * MaxVoltage; // 控制訊號(V): 最大電壓對應(額定或最大)轉矩(視TLC設定)

		IMC_DAC_SetOutputVoltage(motor[i], (float)ControlSignal, 0);

	}

}

/*
void MotionCard_DAC(double(&JointTorCtrl), double(&Voltage))
{
	double MotorToCtrl = { 0.0 };

	int i = AXIS - 1;

	MotorToCtrl = JointTorCtrl / GearRatio[i];
	if (MotorToCtrl < -MaxTorque[i])
	{
		MotorToCtrl = -MaxTorque[i];  // 負方向馬達轉矩限制
	}
	else if (MotorToCtrl>MaxTorque[i])
	{
		MotorToCtrl = MaxTorque[i];  // 正方向馬達轉矩限制
	}
	Voltage = (MotorToCtrl / MaxTorque[i] * MaxVoltage[i]);

	IMC_DAC_SetOutputVoltage(i, (float)Voltage, 0);

}
*/

//=================== 切換中斷 =====================
void MotionCard_ChangeTimer( TMRISR Timer )
{

	// --- 串接中斷服務函式 ---
	/*  IMC_TMR_SetISRFunction(
	TMRISR   myTMR_ISR,			// User 自己撰寫的 Timer 中斷副程式之 Function Pointer
	WORD wCardIndex)*/

	IMC_TMR_SetISRFunction(Timer, 0);

}