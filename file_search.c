// Andrew Ang
// Professor Dingler
// Lab 4: Recursive File Search
// The purpose of this lab is to print out all of the files and directories given the users input into the command line

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>

void validatePath(char* startDir);
void searchFile(char* searchTerm, char* startDir);
float getTime(struct timeval t0, struct timeval t1);
int main(int argc, char* argv[])
{
	struct timeval start;
	struct timeval end;
	float elapsed;
	if(argc <= 2)
	{
		printf("There are less than three  arguments, this program requires at least 3... Exiting program\n");
		exit(0);
	} else if (argc == 3)
	{
		char* searchTerm = strdup(argv[1]);
		char* startDir = strdup(argv[2]);
	
//		printf("%s\n", searchTerm);
//		printf("%s\n", startDir);
	
		validatePath(startDir);
		if(gettimeofday(&start, NULL) < 0)
		{
			perror("gettimeofday");
			exit(0);
		}

		// DO THE RECURSION HERE
		searchFile(searchTerm, startDir);				
		if(gettimeofday(&end, NULL) < 0)
		{
			perror("gettimeofday");
			exit(0);
		}
		elapsed = getTime(start,end);
		printf("Time: %fms\n", elapsed);

	} else {
		printf("there are more than 3 things in command line\n");
		exit(0);
	}
}


void validatePath(char* startDir)
{
	// check if start is / make sure does NOT end with /	
	// if fail exit program
//	printf("%s\n", startDir);
	if(startDir[0] == '/' && startDir[strlen(startDir) - 1] != '/')
	{
		//printf("successfully pass test\n");
		return;
	} else {
		printf("Starting directory must start with a / and cannot end with a /\n");
		exit(0);
	}
}

// Recursivly search
void searchFile(char* searchTerm, char* startDir)
{
	DIR * dr = opendir(startDir);
	struct dirent *dePtr; // pointer for directory entry;
	if (dr == NULL)
	{
		return;
	}	
	while((dePtr = readdir(dr)) != NULL)
	{		
		if(dePtr -> d_type == DT_DIR)
		{
			if(strcmp(dePtr->d_name, ".") == 0 || strcmp(dePtr->d_name, "..") == 0)
			{
				continue; // continue to the next iteration of the loop
			}
			printf("%s/%s:\n", startDir, dePtr->d_name);	
	
		} else {
			printf("%s/%s\n", startDir, dePtr->d_name);	
	
		}
	
	/*		if((strcmp(dePtr->d_name, ".") == 0) || (strcmp(dePtr->d_name, "..") == 0)) 
			{
				// does nothing lol
			}else if (dePtr->d_type == DT_DIR) // check if it is a directory
			{
		//		if (strstr(dePtr->d_name, searchTerm) != NULL) // check if it matches 
		//		{
					printf("%s/%s:\n", startDir, dePtr->d_name);
					// add the entire strcat /
		//		}
				char * newDir = strdup(startDir);
				strcat(newDir, "/");
				strcat(newDir, dePtr->d_name);	
	
//				searchFile(searchTerm, newDir);
						
			} else {
				if (strstr(dePtr->d_name, searchTerm) != NULL) // check if it matches 
				{
					printf("here\n");
					printf("%s/%s\n", startDir, dePtr->d_name);	
				}
			}
	*/	
	}
	closedir(dr);	
}

float getTime(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
