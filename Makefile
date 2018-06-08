all:
	gcc -o threaded threaded.c -Wall -pthread
	gcc -o file_search file_search.c -Wall	
threaded:
	gcc -o threaded threaded.c -Wall -pthread
file_search:
	gcc -o file_search file_search.c -Wall
clean: 
	rm -f threaded
	rm -f file_search_threaded
