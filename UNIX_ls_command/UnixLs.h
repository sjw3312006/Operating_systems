//Jeongwoon Suh : 301313489 : CMPT 300 : Assignment 4

#include <stdio.h>
#include <string.h>
#include <ftw.h> 
#include <dirent.h>
#include <errno.h> 
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <libgen.h> 
#include <sys/types.h>
#include <sys/stat.h>

/*
The permission is necessary to transfer stat.st_mode to readable string, we have to test the different rights
*/
void filePermission(struct stat theStat);

/*
i = 1 indicates to list inode
i = R = 0 indicates to list files on current path with no option
R = 1 indicates to list the sub-directories file
This function list the files and execute ls command for the path.
*/
void listDirectory(const char *path, void (*listFile)(struct dirent*, const char*, int), int i, int R);

/*
i = 0 indicates no inode
i = 1 indicates inode
This function lists the file's name
*/
void listFileName(struct dirent *entry, const char *path, int i);

/*
i = 0 indicates no inode
i = 1 indicates inode
This function list file's description (for example, long listing)
*/
void listFileLong(struct dirent *entry, const char *path, int i);