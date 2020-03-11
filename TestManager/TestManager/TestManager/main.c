//Authors: Yuval Kuperberg 200832525
//		   Yaniv Boneh 201444403
//Project: TestManager
//Description: Main function.
#include "Main_Grades_Directory_Handler.h"
#include <stdio.h>

int main(int argc, char* argv[]){
	int pass_fail_flag = 0;
	pass_fail_flag = get_ids_from_directories_names(argv[1]);
	if (pass_fail_flag == -1) {
		printf("Captain, we were unable to calculate {}\n");
		
	}
	else {
		printf("The grades have arrived, captain\n");
	}
	return(pass_fail_flag);
}