
//Project: TestManager
//Description: This module is responsible for handling the main grades directory, including extracting the paths to
// all the student grades directoryies, sending the path and recieving the final result and printing it to the 
//main grades file.
#include "Main_Grades_Directory_Handler.h"
#include "Send_Grades_Path_To_TestGrade.h"
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <string.h>


//Description: The function sends the grades directory to TestGrade and updates the result, GPA of the student,
//             for it to be printed in the main grades file. 
//Parameters: char* path_to_grades_file - Pointer to main grades directory
//            char **current_id_and_grade - Address of the current id and grade to be updated. 
//Returns: Retruns -1 if the process creation function returned an error, 0 otherwise.
int send_directory_to_test_manager_and_get_student_id_and_grade(char* path_to_grades_file,
	char **current_id_and_grade) {
	int ret_val_from_create_process = 0;
	char *current_id = path_to_grades_file, grade_str[MAX_LENGTH_OF_GRADE + 1];

	current_id += (strlen(path_to_grades_file) - 9);
	strcpy(*current_id_and_grade, current_id);
	ret_val_from_create_process = creat_process_simple_main(path_to_grades_file);
	strcat(*current_id_and_grade, " ");
	_itoa(ret_val_from_create_process, grade_str, 10);
	strcat(*current_id_and_grade, grade_str);
	strcat(*current_id_and_grade, "\n");
	return(ret_val_from_create_process);
}

//Description: The function checks wether there were any problems with the allocated pointers. 
//Parameters: char** current_id_and_grade - Adress of the allocated pointer.
//            char** final_grades_with_path - Adress of the allocated pointer.
//Returns: Retruns -1 if a one of the pointers were not allocated properly, 0 otherwise.
int check_if_malloc_succeeded(char** current_id_and_grade, char** final_grades_with_path) {
	if (*current_id_and_grade == NULL && *final_grades_with_path != NULL) {
		printf("An error occurred while trying to allocate memory, exiting\n");
		free(*final_grades_with_path);
		return(-1);
	}
	if (*current_id_and_grade != NULL && *final_grades_with_path == NULL) {
		printf("An error occurred while trying to allocate memory, exiting\n");
		free(*current_id_and_grade);
		return(-1);
	}
	return(0);
}

//Description: The function searches inside the main grades directory for files with id's in their names, sends the
//             path onwards for the grades to be calculated.
//Parameters: char* path_handler_of_main_grades_directory - path to main grades directory.
//Returns: Retruns -1 if there was any kind of error during the process, 0 otherwise.
int get_ids_from_directories_names(char* path_handler_of_main_grades_directory) {
	FILE *main_grades_file;
	WIN32_FIND_DATA directory_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	CHAR grades_path_handler_without_directory[MAX_PATH], grades_path_handler_with_directory[MAX_PATH],
		path_handler[MAX_PATH];
	char *final_grades_with_path, *current_id_and_grade;
	int error_handler = 0;
	current_id_and_grade = (char*)malloc((MAX_LENGTH_OF_ID_AND_GRADE + 2) * sizeof(char));
	final_grades_with_path = (char*)malloc((strlen(path_handler_of_main_grades_directory)
		+ strlen("\\final_grades.txt") + 1) * sizeof(char));
	error_handler = check_if_malloc_succeeded(&current_id_and_grade, &final_grades_with_path);
	if (error_handler == -1) {
		return(-1);
	}
	strcpy(final_grades_with_path, path_handler_of_main_grades_directory);
	strcat(final_grades_with_path, "\\final_grades.txt");
	main_grades_file = fopen(final_grades_with_path, "a");
	if (main_grades_file == NULL) {
		error_handler = -1;
		printf("An error occurred while trying to open a new file, exiting\n");
		goto exit_without_closing_file;
	}
	strcpy(path_handler, path_handler_of_main_grades_directory);
	strcpy(grades_path_handler_without_directory, path_handler);
	strcat(grades_path_handler_without_directory, "\\");
	strcpy(grades_path_handler_with_directory, grades_path_handler_without_directory);
	strcat(path_handler, "\\* ");
	hFind = FindFirstFile(path_handler, &directory_data);
	if (INVALID_HANDLE_VALUE == hFind) {
		error_handler = -1;
		printf("Could not creat handler for directory, exiting program\n");
		goto exit;
	}
	do {
		if (directory_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (strtok(directory_data.cFileName, ".") != NULL) {
				strcat(grades_path_handler_with_directory, directory_data.cFileName);
				error_handler = send_directory_to_test_manager_and_get_student_id_and_grade
				(grades_path_handler_with_directory, &current_id_and_grade);
				if (error_handler == -1)
					goto exit;
				strcpy(grades_path_handler_with_directory, grades_path_handler_without_directory);
				fputs(current_id_and_grade, main_grades_file);
			}
		}
	} while (FindNextFile(hFind, &directory_data) != 0);
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		error_handler = -1;
		printf("An error occurred! There are still some for files, exiting.\n");
		goto exit;
	}
exit:
	fclose(main_grades_file);
	FindClose(hFind);
exit_without_closing_file:
	free(final_grades_with_path);
	free(current_id_and_grade);
	return(error_handler);
}
