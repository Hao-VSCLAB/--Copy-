#ifndef SAVEDATA
#define SAVEDATA

#include "Setting.h"

void SaveData_CreateFile(char *path);
void SaveData_Data(double a, double b, double c, double d, double e);
void SaveData_Data(double a[]);
void SaveData_CloseFile();

#endif
