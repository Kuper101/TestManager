// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "testgrades.h"


// Constants -------------------------------------------------------------------

#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))

// Function Declarations -------------------------------------------------------

/*
* A simplified API for creating threads.
* Input Arguments:
*   p_start_routine: A pointer to the function to be executed by the thread.
*     This pointer represents the starting address of the thread.
*     The function has to have this specific signature:
*       DWORD WINAPI FunctionName(LPVOID lpParam);
*     With FunctionName being replaced with the function's name.
* Output Arguments:
*   p_thread_id: A pointer to a variable that receives the thread identifier.
*     If this parameter is NULL, the thread identifier is not returned.
* Return:
*   If the function succeeds, the return value is a handle to the new thread.
*   If the function fails, the return value is NULL.
*   To get extended error information, call GetLastError.
* Notes:
*   This function is just a wrapper for CreateThread.
*/
HANDLE CreateTestGradeThread(LPTHREAD_START_ROUTINE p_start_routine,LPDWORD p_thread_id);
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id);

// Function Definitions --------------------------------------------------------

int main(int argc,char* pDirectoryPath[])
{
	HANDLE pMain_thread_handle;
	DWORD  main_thread_id;
	DWORD returnCode;
	DWORD wait_code;

	pMain_thread_handle = CreateThreadSimple(MainThread, &main_thread_id, pDirectoryPath[1]);
	wait_code = WaitForSingleObject(pMain_thread_handle, INFINITE);
	if (WAIT_FAILED == wait_code)
	{
		printf("Error when waiting");
		return ERROR_CODE;
	}

	GetExitCodeThread(pMain_thread_handle, &returnCode);
	return returnCode;
}

HANDLE CreateTestGradeThread(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, char* argsArray)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		argsArray,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */


	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id,char* argsArray)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		argsArray,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */


	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}
