#include "Tracking.h"


//=============== Åª¨ú²Ä¤@¶µ­y¸ñ©R¥O ===============

FILE *TrackingFile;

int a = 0;

void Tracking_FirstPos(char *path, double(&FirstPos)[AXIS])
{

	TrackingFile = fopen(path, "r"); // ¶}±Ò­y¸ñ©R¥OÀÉ 

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

	fclose(TrackingFile); // Ãö³¬­y¸ñ©R¥OÀÉ

}


//================= ¶}±Ò­y¸ñ©R¥OÀÉ =================

void Tracking_OpenFile(char *path)
{

	TrackingFile = fopen(path, "r");

}


//================= Åª¨ú­y¸ñ©R¥OÀÉ =================

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

//================= Ãö³¬­y¸ñ©R¥OÀÉ =================

void Tracking_CloseFile()
{

	fclose(TrackingFile);

}