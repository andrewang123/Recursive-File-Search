all:
	gcc -o file_search_threaded file_search_threaded.c -Wall -pthread
	gcc -o file_search file_search.c -Wall	
file_search_threaded:
	gcc -o file_search_threaded file_search_threaded.c -Wall -pthread
file_search:
	gcc -o file_search file_search.c -Wall
clean: 
	rm -f file_search
	rm -f file_search_threaded
