# Recursive File Search Lab 4
[  ] means optional, < > means required and | means "or" for the parameters  
Usage: file_search <search term\> <starting directory\>  
Example: file_search .c /home/user/mystuff
* This will search all file and directory names in /home/user/mystuff for occurrences of .c and print them out
* ensures that the starting directory is an absolute path
* there may be spaces in search term if called in quotations
* It will also time the elapsed time til completed of search

---
Recursive File Search Threaded version. This lab will be the exact same as the previous lab except for:  
* There will be four threads involved
