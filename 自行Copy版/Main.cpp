#include "MotionCard.h"
#include "Setting.h"
#include "SaveData.h"
#include "Toolbox.h"

char INPUT_FILENAME[] = "C:\\Users\\VSCLAB\\Desktop\\Group\\�ůC\\1_Scurve\\Scurve\\data\\input\\command.txt";
char OUTPUT_FILENAME[] = "C:\\Users\\VSCLAB\\Desktop\\Group\\�ůC\\1_Scurve\\Scurve\\data\\output\\output.txt";
using namespace std;
// 1314520
double Pos = { 0.0 }; // �s�X������
double Vel = { 0.0 }; // LSF�t��
double Vol = { 0.0 };
double PosCmd = { 0.0 }; // ���שR�O
double VelCmd = { 0.0 }; // �t�שR�O
double AccCmd = { 0.0 }; // �[�t�שR�O
double TorCtrl = { 0.0 }; // ��x����T��
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

//=================���=============//
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
//===============����w��============//

void _stdcall Timer_Stay(TMRINT *pstINTSource)
{
	//----------------------- �x�sŪ�����סB�t�� -----------------------
	MotionCard_Encoder(Pos); // ���c����
	Toolbox_LSF(Pos, Vel); // ���c�t��

	//----------------------- ���� -----------------------

	Control_Feedback(Pos, Vel, PosCmd, VelCmd, AccCmd, TorCtrl); // �^�±���

	MotionCard_DAC(TorCtrl); // �e�X����R�O
	//----------------------- �������_ -----------------------

	if (TimerFlag == 0 && StayCount >= 1000) // �w�� 1
	{

		TimerFlag = 1;
		cout << "---------- �w��1 ���� ----------" << endl;

	}
	else if (TimerFlag == 2 && StayCount >= 2000) // �w�� 2
	{

		TimerFlag = 3;
		cout << "---------- �w��2 ���� ----------" << endl;

	}
	else if (TimerFlag == 4 && StayCount >= 3000) // �w�� 3
	{

		TimerFlag = 5;
		cout << "---------- �w��3 ���� ----------" << endl;

	}
	else if (TimerFlag == 5 && StayCount >= 4000) // �w��4
	{
		TimerFlag = 6;
		cout << "---------- �w��4 ���� ----------" << endl;
	}
	//SaveData_Data(Pos, Vel, PosCmd, VelCmd, Vol);
	StayCount++;
}
//==============���b�d�ഫFUNCTION  (�q�������P�b�d�����P������)=======//

void _stdcall InitialPos(TMRINT *pstINTSource){
	MotionCard_Encoder(Pos); // ���c����
	Toolbox_LSF(Pos, Vel); // ���c�t��

	//----------------------- ���� -----------------------

	Control_Feedback(Pos, Vel, 0.0, 0.0, 0.0, TorCtrl); // �^�±���        ////////////////////////

	MotionCard_DAC(TorCtrl); // �e�X����R�O
	count_initial++;
	if (count_initial > 1000) TimerFlag = 2;
}

void _stdcall Tracking(TMRINT *pstINTSource)
{
	//---------------------- Ū�����סB�t�� ----------------------

	MotionCard_Encoder(Pos); // ���c����(rad)
	Toolbox_LSF(Pos, Vel); // ���c���t��(rad/s)

	//---------------------- PTP Control ----------------------
	PosCmd = pos[j];
	VelCmd = spd[j];
	AccCmd = acc[j];

	Control_Feedback(Pos, Vel, PosCmd, VelCmd, AccCmd, TorCtrl);

	MotionCard_DAC(TorCtrl, Vol);

	////----------------------- �x�s������ -----------------------

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
	//=================Ū���Ӧ�matlab�p�⧹���ƾ�===================//
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

	//=================�g�J����ƾ�===================//
	SaveData_CreateFile(OUTPUT_FILENAME);

	//=================�}�ҦU���\��===================//
	printf("Press 'Esc' to end program.\n");
	printf("Press any key to start !\n");

	while (_kbhit() == 0); // �����N��}�l�{��
	MotionCard_OpenCard(); // �}�Ҷb�d
	Toolbox_LSF_Initialization(PosCmd); // ��l�� LSF
	MotionCard_ServoOn(); // Servo On
	//=================�ާ@�y�{======================//
	int sw = 0;
	while (sw != 0x1b){
		//=================��ESC�����{��======================//
		if (_kbhit())
		{

			sw = _getch(); // ��ESC�����{��

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
	MotionCard_CloseCard(); // �����b�d
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