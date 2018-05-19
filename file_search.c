// Andrew Ang
// Professor Dingler
// Lab 4: Recursive File Search
// The purpose of this lab is to print out all of the files and directories given the users input into the command line

#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[])
{

	if(argc <= 2)
	{
		printf("There are less than three  arguments, this program requires at least 3... Exiting program\n");
		exit(0);
	} else if (argc == 3)
	{
		printf("there are three things in the command line\n");

	} else {
		printf("there are more than 3 things in command line\n");
	}
}
