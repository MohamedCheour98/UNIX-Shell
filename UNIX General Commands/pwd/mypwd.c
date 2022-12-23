/*This program performs the same task as /bin/pwd 
without using the getcwd(3) function described by POSIX. 

It traverses the directories starting from the working 
directory all the way up to the root directory 
and it prints the entire path to stdout*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define CURRENT_DIR 0

void traversal(char *working_directory, 
				ino_t previous_in_num, int *ptr, 
					ino_t current_wd_in_num, 
						ino_t root_in_num);

int main()
{
	struct stat root;
	struct stat current_directory;
	ino_t previous; 
	char working_directory[PATH_MAX] = {'\0'}; 
	/*Setting the directory equal to the null character 
	to avoid any garbage values*/
	int ptr = 0;
	previous = CURRENT_DIR; 
	
	if(lstat(".", &current_directory) < 0) /*Checking correctenss
	for the pathname*/
	{
		/*printf("%d", current_directory.st_size);*/
		perror("/");
		exit(EXIT_FAILURE);  
		/*EXIT_FAILURE to avoid portability issues*/
	}
	
	if(lstat("/", &root) < 0)  
	{
		perror("/");
		exit(EXIT_FAILURE); 
	}
	
	traversal(working_directory, previous, &ptr, 
				current_directory.st_ino, root.st_ino);

	printf("%s \n", working_directory);

	return 0;
}

void traversal(char *working_directory, 
				ino_t previous_in_num, int *ptr, 
					ino_t current_wd_in_num, 
						ino_t root_in_num){

	struct dirent *dir_entry;
	struct stat buff_current_dir;
	DIR *dir_ptr;
	struct stat buffer;

	if(*ptr > PATH_MAX){  
	/*Checking for input correctness*/
		perror("path name to long");
		exit(EXIT_FAILURE);
	}
	
	if(previous_in_num == root_in_num){  
	/*Checking if the previous INode number is equal 
	to the root INode number*/
	
		strcpy(working_directory, "/");
		*ptr = 1;
		return;
	}
	
	if(previous_in_num == CURRENT_DIR){
		lstat(".", &buffer); 
		previous_in_num = buffer.st_ino;  
		/*Getting current directory INode number and 
		assigning it to the previous INode number*/
		
		if (chdir("..") < 0){ /*Changing the directory 
		to be the parent directory*/
			perror("..");
			exit(EXIT_FAILURE);
		}
	}
	
	if((dir_ptr = opendir(".")) == NULL){
		perror(".");
		exit(EXIT_FAILURE);
	}
	
	while((dir_entry = readdir(dir_ptr)) != NULL){ 
	/*Reading the current directory and going through it*/
		
		if(lstat(dir_entry->d_name, &buffer) < 0){
			perror(dir_entry->d_name);
			exit(EXIT_FAILURE);
		}
		
		if(S_ISDIR(buffer.st_mode) 
			&& previous_in_num == buffer.st_ino)
			{ /*Checking if the is a directory and has the 
			inode number of the previous directory*/
			
			lstat(".", &buff_current_dir); /*Getting the current 
			directory's INode*/
			chdir(".."); /*Changing the directory to be the 
			parent directory*/
			
			traversal(working_directory, buff_current_dir.st_ino, 
						ptr, current_wd_in_num, 
							root_in_num);
			
			strcat(working_directory, dir_entry->d_name);
			
			if(current_wd_in_num != previous_in_num)
				strcat(working_directory, "/");
			
			
			*ptr = strlen(working_directory);
			
			closedir(dir_ptr);
			return;
		}

	}

}