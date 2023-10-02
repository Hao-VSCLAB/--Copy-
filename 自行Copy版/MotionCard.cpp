#include "MotionCard.h"
#include "IMCDefine.h"
#include "IMCDriver.h"
#include <iostream>
#include <math.h>

//==================== 參數設定 ====================

const double CircleRad = 2.0 * PI;
const double GearRatio[3] = { 1, 1, 1 };
const double MaxTorque[3] = { 1, 1.27 * 1, 1.27 };
const double MaxVoltage[3] = { 8, 10, 10 };
const double Direction[3] = { -1, -1, -1 };

//==================== 開啟軸卡 ===================
void MotionCard_OpenCard()
{
	//  --- 軸卡初始化 ---
	/*  Bool IMC_OpenDevice(		// true：Initial IMP 模組成功 false：Initial IMP 模組失敗
	int nMode,			// 0：初始化 IMP 的 PCI 模式  1：初始化 IMP 的 Standalone 模式
	WORD wCardIndex)		*/
	int nRet = IMC_OpenDevice(0, 0);

	if (nRet == 0)
	{

		printf("Initialization Fails !");

	}
	else
	{
		//  --- 重置 IMP 模組 ---
		/*  IMC_GLB_ResetModule(
		WORD ModuleID,		// RESET_ALL: All Modules
		WORD wCardIndex)	// 欲控制的運動控制卡之編號，編號範圍 0~5 */
		IMC_GLB_ResetModule(RESET_ALL, 0);

		// --- 遮閉 IMP 模組中斷功能 ---
		/*  IMC_GLB_SetInterruptMode(
		int mode,			// IMC_ALL_INT_UNMASK: All Modules
		WORD wCardIndex) */
		IMC_GLB_SetInterruptMask(IMC_ALL_INT_UNMASK, 0);

		//----------------- 編碼器 -----------------

		int i = AXIS - 1;
		// --- A腳位開啟或關閉輸入訊號反相 ---
		/* IMC_ENC_EnableInAInverse(
		WORD enc_ch_no,			// encoder counter channel number（0~7）
		WORD wInverse,			// 1 開啟或0 關閉輸入訊號反相
		WORD wCardIndex)*/
		IMC_ENC_EnableInAInverse(i, 0, 0);	// 0->encoder counter channel in A腳位輸入訊號不反相
		IMC_ENC_EnableInBInverse(i, 0, 0);	// 0->encoder counter channel in B腳位輸入訊號不反相
		IMC_ENC_EnableInCInverse(i, 0, 0);	// 0->encoder counter channel in C腳位輸入訊號不反相

		// --- 開啟或關閉輸入訊號 A/B 對調 ---
		/* IMC_ENC_EnableInABSwap(
		WORD enc_ch_no,			// encoder counter channel number（0~7）
		WORD wSwap,				// 1 開啟或 0 關閉輸入訊號 A/B 對調
		WORD wCardIndex) */
		IMC_ENC_EnableInABSwap(i, 0, 0); // 0->A/B輸入訊號不對調

		// --- 設定編碼器輸入格式為 A/B 型 ---
		/* IMC_ENC_SetInputFormat(
		WORD enc_ch_no,			// encoder counter channel number（0~7）
		WORD wFormat,			// Input type is quadratic or A/B phase
		WORD wCardIndex)*/
		IMC_ENC_SetInputFormat(i, ENC_FMT_AB, 0); // 設定編碼器輸入格式為 A/B 型

		// ---  設定編碼器輸入解碼倍率 x4 ---
		/*  IMC_ENC_SetInputRate(
		WORD enc_ch_no,			// encoder counter channel number（0~7）
		WORD rate,				// Multiplier rate to be 4
		WORD wCardIndex) */
		IMC_ENC_SetInputRate(i, ENC_RATE_X4, 0); // 設定編碼器輸入解碼倍率 x4

		// --- 清除 Encoder counter value ---
		/* IMC_ENC_ClearCounter(
		WORD enc_ch_no,			// encoder counter channel number（0~7）
		WORD wClear,			// 1 開啟或 0 關閉清除功能
		WORD wCardIndex)*/
		IMC_ENC_ClearCounter(i, 1, 0);	// 開啟清除編碼器
		IMC_ENC_ClearCounter(i, 0, 0);	// 關閉清除編碼器

		// --- 啟動 ENC Counter 記錄功能 ---
		/* IMC_ENC_StartCounter(
		WORD enc_ch_no,			// encoder counter channel number（0~7）
		WORD wStart,			// 1 開啟或 0 關閉編碼器功能
		WORD wCardIndex) */
		IMC_ENC_StartCounter(i, 1, 0); // 開始編碼器輸入 



		//---------------- DAC & ADC ----------------



		// --- 設定 DAC 命令源為軟體規劃 ---
		/* IMC_DAC_SelectSource(
		WORD channel,		// DAC channel number 0 ~ 7
		WORD source,		// DAC_CMD_SOFT：Source from DAC output buffer
		WORD wCardIndex)*/
		IMC_DAC_SelectSource(i, DAC_SOURCE_SOFT, 0);

		// --- 開啟 DAC 功能 ---
		/* IMC_DAC_EnableChannel(
		WORD channel ,		// DAC channel number 0 ~ 7
		WORD wEnable,		// 1 開啟或 0 關閉指定 DA Channel 轉換功能
		WORD wCardIndex)*/
		IMC_DAC_EnableChannel(i, 1, 0);



		// --- 開啟 DAC 功能 ---
		/* IMC_DAC_StartConverter(
		WORD wStart,			// 1 開啟或 0 關閉 DAC 轉換功能
		WORD wCardIndex)*/
		IMC_DAC_StartConverter(1, 0);



		// --- DAC 電壓清空 ---
		/* IMC_DAC_SetOutputVoltage(
		WORD channel,		// DAC channel number 0 ~ 7
		float fVoltage,		// 類比輸出電壓 (-10V ~ 10 V)
		WORD wCardIndex) */
		IMC_DAC_SetOutputVoltage(i, 0.0, 0);



		//---------------- 中斷功能 ----------------

		// --- 設定計時器計時時間 1000 us (1豪秒) ---
		/* IMC_TMR_SetTimer(
		float dfPeriod,			// 計時器時間(µs)，可設定範圍 (0 ~ 2^32 毫秒)
		WORD wCardIndex) */
		IMC_TMR_SetTimer(1000, 0);

		// --- 開啟計時器計時功能 ---
		/* IMC_TMR_SetTimerEnable(
		WORD wEnable,			// 1 開啟或 0 關閉計時器功能
		WORD wCardIndex) */
		IMC_TMR_SetTimerEnable(1, 0);

		// --- 開啟計時器中斷功能 ---
		/* IMC_TMR_SetTimerIntEnable(
		WORD wEnable,			// 1 開啟或 0 關閉計時器中斷功能
		WORD wCardIndex) */
		IMC_TMR_SetTimerIntEnable(1, 0);

	}

}

//==================== 關閉軸卡 ===================
void MotionCard_CloseCard()
{

	IMC_TMR_SetTimerIntEnable(0, 0); // 關閉計時器中斷功能
	IMC_TMR_SetTimerEnable(0, 0); // 關閉計時器計時功能

	int i = AXIS - 1;

	IMC_DAC_SetOutputVoltage(i, 0.0, 0); // DAC 電壓清空



	IMC_DAC_StartConverter(0, 0); // 停止 DAC 轉換



	IMC_ENC_StartCounter(i, 0, 0); // 停止編碼器輸入
	IMC_DAC_EnableChannel(i, 0, 0); // 關閉DAC功能


	// --- 關閉 IMP 模組 ---
	/* IMC_CloseIfOpen(
	WORD wCardIndex) */
	IMC_CloseIfOpen(0);
}


//=================== 切換中斷 =====================
void MotionCard_ChangeTimer(TMRISR Timer)
{

	// --- 串接中斷服務函式 ---
	/*  IMC_TMR_SetISRFunction(
	TMRISR   myTMR_ISR,			// User 自己撰寫的 Timer 中斷副程式之 Function Pointer
	WORD wCardIndex)*/
	IMC_TMR_SetISRFunction(Timer, 0);

}

//================== Servo On ======================
void MotionCard_ServoOn()
{

	int i = AXIS - 1;

	// --- Servo On ---
	/* IMC_LIO_SetServoOn(
	WORD wChannel,
	WORD wCardIndex) */
	IMC_LIO_SetServoOn(i, 0); // ServoOn


}

//================== Servo Off =====================
void MotionCard_Serco_Off()
{

	int i = AXIS - 1;

	// --- Servo Off ---
	/* IMC_LIO_SetServoOff(
	WORD wChannel,
	WORD wCardIndex) */
	IMC_LIO_SetServoOff(i, 0); // ServoOff


}

// ================ 編碼器設定 ===================
void MotionCard_Encoder(double(&JointEncRad))
{
	long MotorINCRPulse = { 0 };
	double MotorINCRRad = { 0.0 };

	int i = AXIS - 1;
	// --- "馬達端"增量型編碼器數值[Pulse] ---
	/* IMC_ENC_ReadCounter(
	WORD enc_ch_no,			// encoder counter channel number (0~7)
	long *lCounter,			// 讀取Encoder counter 數值
	WORD wCardIndex) */
	IMC_ENC_ReadCounter(i, &MotorINCRPulse, 0);
	MotorINCRRad = ((double)MotorINCRPulse / 10000.0) * CircleRad; // "馬達端"增量型編碼器數值[Rad] , 增量型編碼器設定一圈 2500*4個Pulse , 驅動器參數P1-46*軸卡4倍頻
	JointEncRad = Direction[i] * (MotorINCRRad / GearRatio[i]);

}

void MotionCard_DAC(double(&JointTorCtrl))
{
	double MotorToCtrl = { 0.0 };
	double ControlSignal = 0;

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
	ControlSignal = (MotorToCtrl / MaxTorque[i]) * MaxVoltage[i];

	IMC_DAC_SetOutputVoltage(i, (float)ControlSignal, 0);

}

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