#include "Tracking.h"


//=============== Ū���Ĥ@���y��R�O ===============

FILE *TrackingFile;

int a = 0;

void Tracking_FirstPos(char *path, double(&FirstPos)[AXIS])
{

	TrackingFile = fopen(path, "r"); // �}�ҭy��R�O�� 

	double PosCmd[AXIS];
	double VelCmd[AXIS];
	double AccCmd[AXIS];

	if (feof(TrackingFile) == false && fscanf_s(TrackingFile, "%lf %lf %lf %lf %lf %lf", &PosCmd[0], &PosCmd[1], &VelCmd[0], &VelCmd[1], &AccCmd[0], &AccCmd[1]) != EOF)
	{

		if (a == 0)
		{

			for (int i = 0; i < AXIS; i++)
			{

				FirstPos[i] = PosCmd[i];

			}

		}

	}

	a++;

	fclose(TrackingFile); // �����y��R�O��

}


//================= �}�ҭy��R�O�� =================

void Tracking_OpenFile(char *path)
{

	TrackingFile = fopen(path, "r");

}


//================= Ū���y��R�O�� =================

void Tracking_Command(double(&PosCmd)[AXIS], double(&VelCmd)[AXIS], double(&AccCmd)[AXIS], int(&EndFlag))
{

	if (feof(TrackingFile) == false && fscanf_s(TrackingFile, "%lf %lf %lf %lf %lf %lf", &PosCmd[0], &PosCmd[1], &VelCmd[0], &VelCmd[1], &AccCmd[0], &AccCmd[1]) != EOF)
	{

		EndFlag = 0;

	}
	else
	{

		EndFlag = 1;

	}

}

//================= �����y��R�O�� =================

void Tracking_CloseFile()
{

	fclose(TrackingFile);

}