
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "testgrades.h"


/*Function Declarations*/

int  TestGradeInt(char* pDirectoryPath)
{


	char	grade[4];
	int		intGrade;
	FILE	*fp;

	Sleep(10);
	//cast the input in to pointer to string
	fp = fopen(pDirectoryPath, "r");

	fgets(grade, 4, fp);
	intGrade = atoi(grade);
	fclose(fp);

	return intGrade;



}

DWORD WINAPI MainThread(LPVOID lpParam)
{
	char*	pDirectoryPathOrig[255];
	char*	pDirectoryPath;
	int		gradesArray[NUM_THREADS];
	int		thread_idx = 0;
	int		finalGrade = 0;
	int		didMoedBFlag = 0;
	int		lenOfPath = 0;
	int		idFirstIdx = 0;
	char	idString[10];
	char*	finalPath;
	char*	pPtrToID;

	HANDLE p_thread_handles[NUM_THREADS];
	DWORD p_thread_ids[NUM_THREADS];
	DWORD wait_code;
	DWORD exitCode;
	DWORD NumThreads = 13;
	FILE* fp;

	char finalGradeSTR[3];
	char exNum[2];
	
	char fileNamePrefix[] = "\\final_";
	char fileNameappendix[] = ".txt";
	char basicNameOfEx[] = "ex";
	char nameOfFile[MAX_PATH_TO_FILE_LENGTH];
	char nameOfFileToRead[MAX_PATH_TO_FILE_LENGTH];
	char* pPtrToIDsFirstChar = 0;
	char*	singleGradePath;
	char** paths = malloc(NUM_THREADS * sizeof(char*));
	
	pDirectoryPath = (char*)lpParam; //cast the input in to pointer to string
	lenOfPath = strlen(pDirectoryPath);
	pPtrToIDsFirstChar= &pDirectoryPath[lenOfPath - 9];
	
	char* finalPathForGrades = (char*)malloc((lenOfPath + 30) * sizeof(char));

	for (thread_idx = 0; thread_idx < NUM_THREADS; thread_idx++)
	{
		paths[thread_idx] = (char*)malloc((lenOfPath + 10) * sizeof(char));
		strcpy(paths[thread_idx], pDirectoryPath);
	}
	 

	for (thread_idx = 0; thread_idx < NUM_OF_HW ; thread_idx++)
	{

		
		strcpy(paths[thread_idx], pDirectoryPath);
		strcat(paths[thread_idx], "\\ex");
		itoa(((thread_idx+1)/10), exNum, 10);
		strcat(paths[thread_idx], exNum);
		itoa((thread_idx + 1) % 10 ,exNum, 10);
		strcat(paths[thread_idx], exNum);
		strcat(paths[thread_idx], ".txt");
		p_thread_handles[thread_idx] = CreateTestGradeThread(TestGradeThread, &p_thread_ids[thread_idx], paths[thread_idx]);
		//gradesArray[thread_idx] = TestGradeInt(paths[thread_idx]);
	}
	
	
	strcpy(paths[10], pDirectoryPath);
	strcat(paths[10], MIDTERM);
	p_thread_handles[10] = CreateTestGradeThread(TestGradeThread, &p_thread_ids[10], paths[10]);
	//gradesArray[10] = TestGradeInt(paths[10]);

	strcpy(paths[11], pDirectoryPath);
	strcat(paths[11], MOED_A);
	p_thread_handles[11] = CreateTestGradeThread(TestGradeThread, &p_thread_ids[11], paths[11]);
	//gradesArray[11] = TestGradeInt(paths[11]);

	strcpy(paths[12], pDirectoryPath);
	strcat(paths[12], MOED_B);
	p_thread_handles[12] = CreateTestGradeThread(TestGradeThread, &p_thread_ids[12], paths[12]);
	//gradesArray[12] = TestGradeInt(paths[12]);

	// Wait for IO thread to receive exit command and terminate
	wait_code = WaitForMultipleObjects(NumThreads, &p_thread_handles, TRUE, INFINITE);
	if (WAIT_FAILED == wait_code)
	{	
		printf("Error when waiting");	
		return ERROR_CODE;
	}
	
	for (thread_idx = 0; thread_idx < NUM_THREADS; thread_idx++)
	{
		GetExitCodeThread(p_thread_handles[thread_idx], &exitCode);
		
		if (exitCode >= 60)
		{
			gradesArray[thread_idx] = exitCode;
		}
		else
		{ 
			if (thread_idx == 12)
			{
				if (exitCode)
				{
					didMoedBFlag = 1;
				}
				else
				{
					didMoedBFlag = 0;
				}
			}
			gradesArray[thread_idx] = 0;
		}
	}

	
	//Calculate final grade 
	for (thread_idx = 0; thread_idx < NUM_THREADS -1; thread_idx++)
	{
		if (thread_idx < NUM_OF_HW)
		{ 
			finalGrade += (0.02 * gradesArray[thread_idx]);
		}
		else
		{		
			if(thread_idx == 10)
				finalGrade += 0.2 * gradesArray[thread_idx];
						
			if (thread_idx == 11)
				finalGrade += 0.6 * gradesArray[thread_idx + didMoedBFlag];
		
		}
	}
	
	strcpy(finalPathForGrades, pDirectoryPath);
	strcat(finalPathForGrades, fileNamePrefix);
	strcat(finalPathForGrades, pPtrToIDsFirstChar);
	strcat(finalPathForGrades, fileNameappendix);
	itoa(finalGrade, finalGradeSTR, 10);
	


	fp = fopen(finalPathForGrades, "w");
	fprintf(fp,finalGradeSTR);
	fclose(fp);

	free(finalPathForGrades);
	free(paths);
	
	
	return finalGrade;
}

	