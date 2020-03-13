

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "testgrades.h"


DWORD WINAPI TestGradeThread(LPVOID lpParam)
{

	char*	pDirectoryPath;
	char	grade[4];
	int		intGrade;
	FILE	*fp;
	
	Sleep(30);
	pDirectoryPath = (char*)lpParam; //cast the input in to pointer to string
	fp = fopen(pDirectoryPath, "r");

	fgets(grade,4, fp);
	intGrade = atoi(grade);
	fclose(fp);

	return intGrade;



}