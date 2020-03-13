//Project: TestManager
//Description: This module creates a new process and sends the path to the grades directory for each student to
// the module TestGrade.
#include "Send_Grades_Path_To_TestGrade.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <strsafe.h>


//Description: The function creates a new Testgrades process for the TestManager to send grades directories to.
//Parameters: LPTSTR CommandLine - A string that represents the path to a grades directory.
//			  PROCESS_INFORMATION ProcessInfoPtr - A pointer to a STARTUPINFO struct variable
//                            containing various “Microsoft Windows” parameters
//Returns: TRUE\FALSE - If the process creation failed the function returnes 0, otherwise a non-zero value
//         is returned.
BOOL create_process_simple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}

//Description: The function receives a simple expression and sending it so the TestGrades process. 
//It then sends back the answer it gets from the Son, back to the father. 
//Parameters: char* path_to_current_student_directory - A string that represents a path to grades directory.
//Returns: int result_of_weighing_all_grades - The calculation of weighing all grades together.
int creat_process_simple_main(char* path_to_current_student_directory){
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				result_of_weighing_all_grades;
	BOOL				retVal;
	CHAR				command[MAX_PATH + 1 + LENGTH_OF_STRING_TESTGRADE_EXE];
	CHAR *command_with_path_to_directory;
	int result = 0;
	GetCurrentDirectory(MAX_PATH, command);
	strcat(command, "\\TestGrade.exe ");
	if ((command_with_path_to_directory = malloc(strlen(command) + strlen(path_to_current_student_directory) + 1)) != NULL) {
		command_with_path_to_directory[0] = '\0';   // Ensures the memory is an empty string
		strcat(command_with_path_to_directory, command);
		strcat(command_with_path_to_directory, path_to_current_student_directory);
	}
	else {
		printf("An error occurred while trying to allocate memory, exiting\n");
		return(-1);
	}
	retVal = create_process_simple(command_with_path_to_directory, &procinfo);
	//ErrorExit(TEXT("GetProcessId"));
	if (retVal == 0){
		printf("Process Creation Failed! Exiting program\n");
		result = -1;
		goto exit;
	}
	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); 
	if (waitcode == WAIT_FAILED) {
		printf("WAIT_FAILED. Exiting\n");
		result = -1;
		goto exit;
	}
	if (waitcode == WAIT_TIMEOUT) {
		printf("Process was not terminated before timeout!\n"
			"Terminating brutally!\n");
		TerminateProcess(procinfo.hProcess, BRUTAL_TERMINATION_CODE); 
		Sleep(10); 
		result = -1;
		goto exit;
	}
	GetExitCodeProcess(procinfo.hProcess, &result_of_weighing_all_grades);
	result = result_of_weighing_all_grades;
exit:
	free(command_with_path_to_directory);
	CloseHandle(procinfo.hProcess); 
	CloseHandle(procinfo.hThread); 
	return(result);
}
