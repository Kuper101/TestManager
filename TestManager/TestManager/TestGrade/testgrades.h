#include <stdio.h>
#include <Windows.h>

#define MAX_DIRECTORY_PATH_LENGTH 17
#define NUM_THREADS	13
#define NUM_OF_GRADES 13
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))
#define NUM_OF_HW 10
#define MAX_PATH_TO_FILE_LENGTH 256
#define _CRT_NONSTDC_NO_DEPRECATE

	
#define EX1  "\ex1.txt" 
#define EX2 "\ex2.txt"
#define EX3 "\ex3.txt"
#define EX4 "\ex4.txt"
#define EX5 "\ex5.txt"
#define EX6 "\ex6.txt"
#define EX7 "\ex7.txt"
#define EX8 "\ex8.txt"
#define EX9 "\ex9.txt"
#define EX10 "\ex10.txt"
#define MIDTERM "\\midterm.txt"
#define MOED_A "\\moedA.txt"
#define MOED_B "\\moedB.txt"


HANDLE CreateTestGradeThread(LPTHREAD_START_ROUTINE p_start_routine,LPDWORD p_thread_id);
DWORD WINAPI TestGradeThread(LPVOID lpParam);
DWORD WINAPI MainThread(LPVOID lpParam);

