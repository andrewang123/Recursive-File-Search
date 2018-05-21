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
	// Create a struct that will represent the time that has elapsed
	// timeval two member variables: tv_sec (seconds) and tv_usec (microseconds)
	struct timeval start; 
	struct timeval end;
	float elapsed; // represents the total amount of time elapsed in miliseconds
	if(argc <= 2) // if the user only types in less than three arguements 
	{
		printf("There are less than three arguments, this program requires at least 3... Exiting program\n");
		exit(0); // exit the program
	} else if (argc == 3) // if the user types in three arguements
	{
		// strdup returns a pointer to a new string which is a duplicate of the parameter passed
		// it is essentially a strcpy and a malloc built into one
		// make a deep copy of the buffer, it is like strcpy but dynamically allocated memory		
		char* searchTerm = strdup(argv[1]);
		char* startDir = strdup(argv[2]);
		// ensure that the path stat with a / and does not end with a / (absolute path)
		validatePath(startDir);
		
		// gets the time of day at the moment and puts it in start
		// returns a number,0 if success and -1 if failure
		// NULL is to specifying the time zone
		if(gettimeofday(&start, NULL) < 0)
		{
			perror("gettimeofday");
			exit(0);
		}
		// recursivly search for the files and directories
		searchFile(searchTerm, startDir);			
		
		// gets the time of day at the moment and puts it in end
		// returns a number,0 if success and -1 if failure
		// NULL is to specifying the time zone	
		if(gettimeofday(&end, NULL) < 0)
		{
			perror("gettimeofday");
			exit(0);
		}
		// calculate the amount of miliseconds it took to complete the task and print it out
		elapsed = getTime(start,end);
		printf("Time: %fms\n", elapsed);

		// free up the strings allocated in the heap
		free(searchTerm);
		free(startDir);
	} else { // if the user types in more than three arguements
		printf("there are more than 3 things in command line\n");
		exit(0); // exit the program
	}
}

// Validates the path that the user specifies in the command line
// Parameters: the path the user specifies
// Returns: N/A
void validatePath(char* startDir)
{
	// check if start is / make sure does NOT end with /	
	if(startDir[0] == '/' && startDir[strlen(startDir) - 1] != '/')
	{
		return;
	} else {
		printf("Starting directory must start with a / and cannot end with a /\n");
		exit(0); // exit the program
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
			
			// only print if matches the term!
			if (strstr(dePtr->d_name, searchTerm) != NULL) // check if it matches 
			{	
				printf("%s/%s:\n", startDir, dePtr->d_name);	
			}


			//startDir + / +dePtr->d_name
			char * newDir = malloc(strlen(startDir) + strlen(dePtr->d_name) + 2);
			strcpy(newDir, startDir);
			strcat(newDir, "/");
			strcat(newDir, dePtr->d_name);
//			printf("THE NEWDIR IS %s \n", newDir);
			searchFile(searchTerm, newDir);	




		} else {

			// only print if matches the term
			if (strstr(dePtr->d_name, searchTerm) != NULL) // check if it matches 
			{
				printf("%s/%s\n", startDir, dePtr->d_name);	
			}
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
	// FREE MEMORY
	closedir(dr);	
}

// Calculates the amount of time it took in miliseconds between two timevals
// Parameter: The start and ending time values
// Returns: difference between the end timeval (2nd param) and the start timeval (1st param)
float getTime(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
