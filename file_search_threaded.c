// Andrew Ang
// Professor Dingler
// Lab 4: Recursive File Search Threaded Version
// The purpose of this lab is to print out all of the files and directories given the users input into the command line
// This lab is the threaded version of the recursive file search.
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define TOTALTHREADS 4

char* searchTerm; // create a global variable for the search term
void* thread_func(void* path);
void validatePath(char* startDir);
void searchFile(char* searchTerm, char* startDir, int num);
float getTime(struct timeval t0, struct timeval t1);

pthread_t threads[TOTALTHREADS];
char* everything[10];

int main(int argc, char* argv[])
{
//	int ret = pthread_create(&firstThread, NULL, thread_func, NULL);// probably file name
	
//	pthread_join(firstThread, 0);
	
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
		searchTerm = strdup(argv[1]);
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
		int num = 0;
		searchFile(searchTerm, startDir, num);			


		for(int i = 0; i < (sizeof(everything) / sizeof(everything[0])); i++)
		{
			printf("EVERYTHING %s\n", everything[i]);

		}


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


void* thread_func(void* parameter) 
{


	pthread_exit(0);

	
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

// Recursively search for all the directories and files that has the search term within
// Parameters: searchTerm that the user wanted to search for and startDir which is the directory that the user wants to look into
// Returns: N/A
void searchFile(char* searchTerm, char* startDir, int num)
{
	// opens a directory and stores it into a DIR pointer variable
	// Returns a pointer to an object of type DIR if a directory otherwise returns a NULLPTR
	// Parameters: it takes a pathname
	DIR * dr = opendir(startDir);
	
	// Create a struct dirent that contains the information that we will need to print the files
	// ino_t d_ino -> the inode number
	// off_t d_off -> not a offset
	// unsigned short d_reclen -> length of the record
	// unsigned char d_type -> the type of the file (We will be using this to determine if directory
	// char d_name[] -> null terminated file (We will be using this to determine the name of the file/dir
	struct dirent *dePtr; // pointer for directory entry;
	if (dr == NULL) // if the directory cannot be opened exit the function
	{
		return;
	}	
	// readdir reads a directory
	// parameters: takes in a DIR * which is the pointer that is the opened directory
	// returns: a struct dirent which has all of the information(name of the file/directory, and type)
	// returns a null if the end of the directory stream is reached
	while((dePtr = readdir(dr)) != NULL)
	{		
		// if we are currently looking at a directory
		if(dePtr -> d_type == DT_DIR)
		{
			// ignore . and .. directories, move onto the next thing in the directory
			if(strcmp(dePtr->d_name, ".") == 0 || strcmp(dePtr->d_name, "..") == 0)
			{
				continue; // continue to the next iteration of the loop
			}
				// create the new directory that is the olddirectory + "/" + directoryname in the heap
				char * newDir = malloc(strlen(startDir) + strlen(dePtr->d_name) + 2);
				// strcpy takes in two parameters the destination to be copied too and the thing that you want to copy
				strcpy(newDir, startDir);	
				// appends the string pointed to by the first parameter, it appends
				// whatever is in the second paramter
				strcat(newDir, "/");	
				// appends the string pointed to by the first parameter, it appends
				// whatever is in the second paramter
				strcat(newDir, dePtr->d_name);
				// Recursivly call the function again since it is a directory

	
			// check if the search term is within
			// Finds the first occurance of the 2nd param in the 1st param
			// Parameter: 1st parameters is the main string that you are trying to find the 2nd string in
			// Returns: the first occurance of the 2nd parameter in the first param, otherwise return NULL
			if (strstr(dePtr->d_name, searchTerm) != NULL) // check if it matches 
			{	
				printf("%s/%s:\n", startDir, dePtr->d_name);	
				scanf(everything[num], newDir);
				num++;
			}
				searchFile(searchTerm, newDir, num);	
				free(newDir);						
		} else {
			// check if the search term is within
			// Finds the first occurance of the 2nd param in the 1st param
			// Parameter: 1st parameters is the main string that you are trying to find the 2nd string in
			// Returns: the first occurance of the 2nd parameter in the first param, otherwise return NULL
			if (strstr(dePtr->d_name, searchTerm) != NULL) // check if it matches 
			{
				printf("%s/%s\n", startDir, dePtr->d_name);	
			}
		}
	}
	closedir(dr);	
}


// Calculates the amount of time it took in miliseconds between two timevals
// Parameter: The start and ending time values
// Returns: difference between the end timeval (2nd param) and the start timeval (1st param)
float getTime(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
