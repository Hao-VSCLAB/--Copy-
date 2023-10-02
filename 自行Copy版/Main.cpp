#include "MotionCard.h"
#include "Setting.h"
#include "SaveData.h"
#include "Toolbox.h"

char INPUT_FILENAME[] = "C:\\Users\\VSCLAB\\Desktop\\Group\\嘉浚\\1_Scurve\\Scurve\\data\\input\\command.txt";
char OUTPUT_FILENAME[] = "C:\\Users\\VSCLAB\\Desktop\\Group\\嘉浚\\1_Scurve\\Scurve\\data\\output\\output.txt";
using namespace std;

double Pos = { 0.0 }; // 編碼器角度
double Vel = { 0.0 }; // LSF速度
double Vol = { 0.0 };
double PosCmd = { 0.0 }; // 角度命令
double VelCmd = { 0.0 }; // 速度命令
double AccCmd = { 0.0 }; // 加速度命令
double TorCtrl = { 0.0 }; // 轉矩控制訊號
double TorCtrl_Feedback = { 0.0 };
double *pos = NULL;
double *spd = NULL;
double *acc = NULL;
int j = 0;
int TimerFlag = 0;
int StayCount = 0;
int numLine = 0;
int count_initial = 0;

FILE *C_openFile(const char *fileName, const char *mode);
void alloc_mem(int n);

//=================控制器=============//
void Control_Feedback(double Pos, double Vel, double PosCmd, double VelCmd, double AccCmd, double(&TorCtrl))
{
	double Jm = 1.6831E-04;
	double Bm = 8.2162E-05;
	double fc = 0.0165;
	double Ka[3] = { 250, 2.0, 150.0 };
	double ba[3] = { 0.01, 0.01, 0.02 };

	int i = AXIS - 1;
	TorCtrl_Feedback = ba[i] * (Ka[i] * (PosCmd - Pos) - Vel);
	TorCtrl = TorCtrl_Feedback;
}
//===============控制緩衝============//

void _stdcall Timer_Stay(TMRINT *pstINTSource)
{
	//----------------------- 儲存讀取角度、速度 -----------------------
	MotionCard_Encoder(Pos); // 機構角度
	Toolbox_LSF(Pos, Vel); // 機構速度

	//----------------------- 控制 -----------------------

	Control_Feedback(Pos, Vel, PosCmd, VelCmd, AccCmd, TorCtrl); // 回授控制

	MotionCard_DAC(TorCtrl); // 送出控制命令
	//----------------------- 切換中斷 -----------------------

	if (TimerFlag == 0 && StayCount >= 1000) // 緩衝 1
	{

		TimerFlag = 1;
		cout << "---------- 緩衝1 結束 ----------" << endl;

	}
	else if (TimerFlag == 2 && StayCount >= 2000) // 緩衝 2
	{

		TimerFlag = 3;
		cout << "---------- 緩衝2 結束 ----------" << endl;

	}
	else if (TimerFlag == 4 && StayCount >= 3000) // 緩衝 3
	{

		TimerFlag = 5;
		cout << "---------- 緩衝3 結束 ----------" << endl;

	}
	else if (TimerFlag == 5 && StayCount >= 4000) // 緩衝4
	{
		TimerFlag = 6;
		cout << "---------- 緩衝4 結束 ----------" << endl;
	}
	//SaveData_Data(Pos, Vel, PosCmd, VelCmd, Vol);
	StayCount++;
}
//==============讓軸卡轉換FUNCTION  (電腦本身與軸卡為不同的個體)=======//

void _stdcall InitialPos(TMRINT *pstINTSource){
	MotionCard_Encoder(Pos); // 機構角度
	Toolbox_LSF(Pos, Vel); // 機構速度

	//----------------------- 控制 -----------------------

	Control_Feedback(Pos, Vel, 0.0, 0.0, 0.0, TorCtrl); // 回授控制        ////////////////////////

	MotionCard_DAC(TorCtrl); // 送出控制命令
	count_initial++;
	if (count_initial > 1000) TimerFlag = 2;
}

void _stdcall Tracking(TMRINT *pstINTSource)
{
	//---------------------- 讀取角度、速度 ----------------------

	MotionCard_Encoder(Pos); // 機構角度(rad)
	Toolbox_LSF(Pos, Vel); // 機構角速度(rad/s)

	//---------------------- PTP Control ----------------------
	PosCmd = pos[j];
	VelCmd = spd[j];
	AccCmd = acc[j];

	Control_Feedback(Pos, Vel, PosCmd, VelCmd, AccCmd, TorCtrl);

	MotionCard_DAC(TorCtrl, Vol);

	////----------------------- 儲存實驗資料 -----------------------

	if (TimerFlag == 3)
	{

		SaveData_Data(Pos, Vel, PosCmd, VelCmd, TorCtrl_Feedback);

	}
	j++;
	if (j == numLine){
		TimerFlag = 4;
	}
}

int main(){
	//=================讀取來自matlab計算完的數據===================//
	FILE *fp = C_openFile(INPUT_FILENAME, "r");
	// Get File Line Number
	char flag;
	while (!feof(fp)){
		flag = fgetc(fp);
		if (flag == '\n')
			numLine++;
	}
	fseek(fp, 0L, SEEK_SET); // File pointer seek to top
	alloc_mem(numLine);
	for (int i = 0; i < numLine; i++)
		fscanf(fp, "%lf	%lf	%lf\n", &pos[i], &spd[i], &acc[i]);

	double inipos = pos[0];

	//=================寫入機械數據===================//
	SaveData_CreateFile(OUTPUT_FILENAME);

	//=================開啟各項功能===================//
	printf("Press 'Esc' to end program.\n");
	printf("Press any key to start !\n");

	while (_kbhit() == 0); // 按任意鍵開始程式
	MotionCard_OpenCard(); // 開啟軸卡
	Toolbox_LSF_Initialization(PosCmd); // 初始化 LSF
	MotionCard_ServoOn(); // Servo On
	//=================操作流程======================//
	int sw = 0;
	while (sw != 0x1b){
		//=================按ESC結束程式======================//
		if (_kbhit())
		{

			sw = _getch(); // 按ESC結束程式

		}

		if (TimerFlag == 0){

			MotionCard_ChangeTimer(Timer_Stay);

		}
		else if (TimerFlag == 1){

			MotionCard_ChangeTimer(InitialPos);

		}
		else if (TimerFlag == 2){

			MotionCard_ChangeTimer(Timer_Stay);

		}
		else if (TimerFlag == 3){

			MotionCard_ChangeTimer(Tracking);

		}
		else if (TimerFlag == 4){

			MotionCard_ChangeTimer(Timer_Stay);

		}
		else if (TimerFlag == 5){

			MotionCard_ChangeTimer(Timer_Stay);

		}
		else if (TimerFlag == 6){

			break;

		}
	}

	MotionCard_Serco_Off(); // Servo Off
	MotionCard_CloseCard(); // 關閉軸卡
	SaveData_CloseFile();
	fclose(fp);

	// Release Memory
	delete[] pos;
	delete[] spd;
	delete[] acc;
}

FILE *C_openFile(const char *fileName, const char *mode)
{
	FILE *fp = fopen(fileName, mode);
	if (fp == NULL)
	{
		fprintf(stderr, "Error while opening \"%s\".\n", fileName);
		system("pause");
		exit(EXIT_FAILURE);
	}
	return fp;
}

void alloc_mem(int n)
{
	try
	{
		pos = new double[n];
		spd = new double[n];
		acc = new double[n];
	}
	catch (bad_alloc& ba)
	{
		cerr << "bad_alloc caught: " << ba.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}