#include "SaveData.h"

//================================================

FILE *SaveDataFile;

//================ 建立實驗資料檔 ================
void SaveData_CreateFile(char *path)
{

	// --- 建立txt檔 ---
	/* fopen(
			string filename,		// 檔案路徑及名字
			string mode)			// "w" Open for writing only;*/

	SaveDataFile = fopen(path, "w");

}

//================ 寫入實驗資料檔 ================

void SaveData_Data(double a[], double b[], double c[], double d[], double e[], double f[])
{
	for (int i = 0; i < AXIS; i++)
	{
		fprintf(SaveDataFile, "%f\t", a[i]);
	}
	for (int i = 0; i < AXIS; i++)
	{
		fprintf(SaveDataFile, "%f\t", b[i]);
	}
	for (int i = 0; i < AXIS; i++)
	{
		fprintf(SaveDataFile, "%f\t", c[i]);
	}

	for (int i = 0; i < AXIS; i++)
	{
		fprintf(SaveDataFile, "%f\t", d[i]);
	}
	for (int i = 0; i < AXIS; i++)
	{
		fprintf(SaveDataFile, "%f\t", e[i]);
	}
	for (int i = 0; i < AXIS; i++)
	{
		fprintf(SaveDataFile, "%f\t", f[i]);
	}


	fprintf(SaveDataFile, "\n");
}

/*
//================ 寫入實驗資料檔 ================

void SaveData_Data(double a[])
{
	for (int i = 0; i < 1; i++)
	{
		fprintf(SaveDataFile, "%f\t", a[i]);
	}

	fprintf(SaveDataFile, "\n");
}
*/

//================ 關閉實驗資料檔 ================

void SaveData_CloseFile()
{

	// --- 關閉txt檔 ---
	/* fclose(
			string filename)		// 檔案路徑及名字*/
	fclose(SaveDataFile);

}