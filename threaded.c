#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

//takes a file/dir as argument, recurses,
// prints name if empty dir or not a dir (leaves)
void recur_file_search(char *file);

// takes in a struct that is the starting and ending point of the array to print out
print the name
void* thread_func(void* args);

//share search term globally (rather than passing recursively)
const char *search_term;

// array that has all of the file names and directories
char* everything[255];
// counter with all of the items into the array
int num = 0;
// a struct used to passed two varaibles which is the starting and ending points
struct startEndPoints
{
	int start;
	int end;
};

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Usage: my_file_search <search_term> <dir>\n");
		printf("Performs recursive file search for files/dirs matching\
				<search_term> starting at <dir>\n");
		exit(1);
	}
	
	//don't need to bother checking if term/directory are swapped, since we can't
	// know for sure which is which anyway
	search_term = argv[1];

	//open the top-level directory
	DIR *dir = opendir(argv[2]);

	//make sure top-level dir is openable (i.e., exists and is a directory)
	if(dir == NULL)
	{
		perror("opendir failed");
		exit(1);
	}
	
	//start timer for recursive search
	struct timeval start, end;
	gettimeofday(&start, NULL);

	recur_file_search(argv[2]);
	
	// create structs for each of the thread
	int totalLen = num;
	struct startEndPoints first;
	struct startEndPoints second;
	struct startEndPoints third;
	struct startEndPoints fourth;

	// if the total length of the array is less than four
	if(totalLen < 4)
	{
		// create the structs
		first.start = 0; 
		first.end = 1; 
			
		second.start = 1; 
		second.end =  2; 

		third.start = 2;
		third.end = 3; 
	
		fourth.start = 3; 
		fourth.end = 4; 
	} else {
	
		// create the structs
		first.start = 0; 
		first.end = first.start + totalLen / 4 + 1; 
		
		second.start = first.end + totalLen / 4 - 1; 
		second.end =  second.start + totalLen / 4 + 1; 
	
		third.start = second.end + totalLen / 4 - 1;
		third.end = third.start + totalLen / 4 + 1; 
	
		fourth.start = third.end + totalLen / 4 - 1; 
		fourth.end = totalLen; 
	}
	// Create four threads
	pthread_t firstThread;
	pthread_t secondThread;
	pthread_t thirdThread;
	pthread_t fourthThread;

	// run the threads, first parameter is the thread that is being ran, 2nd is attributes, the funciton being called, and the 
	// arguments passed	
	int ret = pthread_create(&firstThread, NULL, thread_func, &first);
	int ret2 = pthread_create(&secondThread, NULL, thread_func, &second);
	int ret3 = pthread_create(&thirdThread, NULL, thread_func, &third);
	int ret4 = pthread_create(&fourthThread, NULL, thread_func, &fourth);
	
	// waits for the threads to finish 
	pthread_join(firstThread, NULL);
	pthread_join(secondThread, NULL);
	pthread_join(thirdThread, NULL);
	pthread_join(fourthThread, NULL);

		
	gettimeofday(&end, NULL); // get the end of the time
	printf("Time: %ld\n", (end.tv_sec * 1000000 + end.tv_usec)
			- (start.tv_sec * 1000000 + start.tv_usec));

	return 0;
}


//This function takes a path to recurse on, searching for mathes to the
// (global) search_term.  The base case for recursion is when *file is
// not a directory.
//Parameters: the starting path for recursion (char *), which could be a
// directory or a regular file (or something else, but we don't need to
// worry about anything else for this assignment).
//Returns: nothing
//Effects: prints the filename if the base case is reached *and* search_term
// is found in the filename; otherwise, prints the directory name if the directory
// matches search_term.
void recur_file_search(char *file)
{
	//check if directory is actually a file
	DIR *d = opendir(file);

	//NULL means not a directory (or another, unlikely error)
	if(d == NULL)
	{
		//opendir SHOULD error with ENOTDIR, but if it did something else,
		// we have a problem (e.g., forgot to close open files, got
		// EMFILE or ENFILE)
		if(errno != ENOTDIR)
		{	
			perror("Something weird happened!");
			fprintf(stderr, "While looking at: %s\n", file);
			exit(1);
		}

		//nothing weird happened, check if the file contains the search term
		// and if so print the file to the screen (with full path)
		if(strstr(file, search_term) != NULL)
		{
			//printf("%s\n", file);
			everything[num] = (char*)malloc(255 * sizeof(char));	
			strcpy(everything[num], file);
			num++;

		}
		//no need to close d (we can't, it is NULL!)
		return;
	}

	//we have a directory, not a file, so check if its name
	// matches the search term
	if(strstr(file, search_term) != NULL)
	{
		//printf("%s/\n", file);
		everything[num] = (char*)malloc(255 * sizeof(char));	
		strcpy(everything[num], file);
		num++;
	
	}

	//call recur_file_search for each file in d
	//readdir "discovers" all the files in d, one by one and we
	// recurse on those until we run out (readdir will return NULL)
	struct dirent *cur_file;
	while((cur_file = readdir(d)) != NULL)
	{
		//make sure we don't recurse on . or ..
		if(strcmp(cur_file->d_name, "..") != 0 &&\
				strcmp(cur_file->d_name, ".") != 0)
		{
			//we need to pass a full path to the recursive function, 
			// so here we append the discovered filename (cur_file->d_name)
			// to the current path (file -- we know file is a directory at
			// this point)
			char *next_file_str = malloc(sizeof(char) * \
					strlen(cur_file->d_name) + \
					strlen(file) + 2);

			strncpy(next_file_str, file, strlen(file));
			strncpy(next_file_str + strlen(file), \
					"/", 1);
			strncpy(next_file_str + strlen(file) + 1, \
					cur_file->d_name, \
					strlen(cur_file->d_name) + 1);

		
			//recurse on the file
			recur_file_search(next_file_str);

			//free the dynamically-allocated string
			free(next_file_str);
		}
	}

	//close the directory, or we will have too many files opened (bad times)
	closedir(d);
}


// function pointer 
// takes in a parameter which is a structs that are the starting and ending point of the evenly distributed 
// distributed into fourths
void* thread_func(void* args) 
{
	// get the items from the struct
	struct startEndPoints *arg_struct = (struct startEndPoints*) args;
	
	// check if the starting is less than the num which means that there are less values than the total
	if (arg_struct -> start < num)
	{
		int starting = arg_struct -> start;
		int ending = arg_struct -> end;
		for(int i = starting; i < ending; i++)
		{
			printf("%s\n", everything[i]);
		}
	}	
}
 
