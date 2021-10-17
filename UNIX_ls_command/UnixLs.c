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
#include "UnixLs.h"

//[Permission]
//reference: https://stackoverflow.com/questions/20450556/linux-ls-l-implementation-using-file-descriptors-in-c-c
//reference: https://linux.die.net/man/2/chmod
void filePermission(struct stat theStat){
    printf(" ");
    printf( (S_ISDIR(theStat.st_mode)) ? "d" : "-");
    printf( (theStat.st_mode & S_IRUSR) ? "r" : "-"); //S_IRUSR (00400): read by owner
    printf( (theStat.st_mode & S_IWUSR) ? "w" : "-"); //S_IWUSR (00200): write by owner
    printf( (theStat.st_mode & S_IXUSR) ? "x" : "-"); //S_IXUSR (00100): execute/search by owner
    printf( (theStat.st_mode & S_IRGRP) ? "r" : "-"); //S_IRGRP (00040): read by group
    printf( (theStat.st_mode & S_IWGRP) ? "w" : "-"); //S_IWGRP (00020): write by group
    printf( (theStat.st_mode & S_IXGRP) ? "x" : "-"); //S_IXGRP (00010): execute/search by group
    printf( (theStat.st_mode & S_IROTH) ? "r" : "-"); //S_IROTH (00004): read by others
    printf( (theStat.st_mode & S_IWOTH) ? "w" : "-"); //S_IWOTH (00002): write by others
    printf( (theStat.st_mode & S_IXOTH) ? "x" : "-"); //S_IXOTH (00001): execute/search by others
}

//reference: https://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
//reference for directory entry: https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
void listDirectory(const char *path, void (*listFile)(struct dirent*, const char*, int), int i, int R){
    DIR *directory;
    struct dirent *entry;

    if(!(directory = opendir(path))){
        printf("There does not exist such file or directory: %s\n", path);
        return;
    }

    //If a file is found (readdir returns a NOT NULL value), the loop starts and keep goes until it has listed all of them. 
    //Thus, list all current directory's files
    while((entry = readdir(directory)) != NULL){
        if(entry -> d_type == DT_REG || entry -> d_type == DT_DIR || entry -> d_type == DT_LNK){ //UnixLs is run on will only contain regular files, directories, and links
            if(entry -> d_name[0] == '.'){
                continue;
            }
            listFile(entry, path, i);
        }
    }
    closedir(directory);

    //list all sub-directory's files
    if(R == 1){
        if(!(directory = opendir(path))){
            return;
        }
        while((entry = readdir(directory)) != NULL){
            if(entry -> d_type == DT_DIR){
                char nextPath[1024];
                if(entry -> d_name[0] == '.'){
                    continue;
                }
                //sprint "entry" which defines the path to our file
                snprintf(nextPath, sizeof(nextPath), "%s/%s", path, entry -> d_name);
                printf("\n\n%s:\n", nextPath);
                listDirectory(nextPath, listFile, i, R);
            }
        }
        closedir(directory);
    }
}

void listFileName(struct dirent *entry, const char *path, int i){
    if(i == 1){
        printf("%llu ", entry -> d_ino);
    }
    printf("%s ", entry -> d_name); //file's name 
}

//reference for time: https://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811
void listFileLong(struct dirent *entry, const char *path, int i){
    char buff[1024];
    char fullPath[1024];

    struct stat theStat; //retrieve the stats associated to the file.

    //will be used to determine the file owner & group
    struct passwd *of = NULL;
    struct group *gf = NULL;

    //to determine time
    struct tm *timeInfo = NULL;

    //sprint "directory/entry" which defines the path to our file
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry -> d_name);
    //Then we use stat function in order to retrieve information about the file
    lstat(fullPath, &theStat);

    const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    timeInfo = localtime(&theStat.st_mtime);

    if(i == 1){
        printf("%10llu", theStat.st_ino);
    }

    filePermission(theStat);
    //"This count keeps track of how many directories have entries for this file. 
    //If the count is ever decremented to zero, then the file itself is discarded as soon as no process still holds it open."
    printf("%4hu", theStat.st_nlink); //[number of hard links] 

    of = getpwuid(theStat.st_uid); //[owner]
    printf("%10s", of -> pw_name);

    gf = getgrgid(theStat.st_gid); //[group]
    printf("%8s", gf -> gr_name);

    printf("%8lld", theStat.st_size); //[size in bytes]
    printf("%7s %02d %04d %02d:%02d\t", months[timeInfo -> tm_mon], timeInfo -> tm_mday, 
        timeInfo -> tm_year + 1900, timeInfo -> tm_hour, timeInfo -> tm_min); //[time of last modification]
    printf("%s", entry -> d_name); //[filename]

    if(entry -> d_type == DT_LNK){
        readlink(entry -> d_name, buff, sizeof(buff));
        printf(" -> %s", buff);
    }

    printf("\n");
}

int main(int argc, char **argv){
    int iFlag = 0;
    int RFlag = 0;
    int lFlag = 0;

    //if there is no argc, UnixLS is the current path with no option.
    if(argc == 1){
        listDirectory(".", listFileName, iFlag, RFlag);
        printf("\n");
        return 0;
    }

    int lastFlag = 1;

    //checking the last argc from user
    //if lastFlag = 1, the last argc is the path which the user want to execute UnixLs on
    //otherwise, if lastFlag = 0, the current will be the path argc for UnixLs
    if(argv[argc - 1][0] == '-'){
        lastFlag = 0;
    }

    //checks the user selected option
    for(int x = 1; x < argc - lastFlag; x++){
        if(argv[x][0] != '-'){
            printf("Please check if your argument formatted correctly.\n");
        }
        for(int y = 1; argv[x][y] != '\0'; y++){
            if(argv[x][y] == 'i'){
                iFlag = 1;
            }
            else if(argv[x][y] == 'l'){
                lFlag = 1;
            }
            else if(argv[x][y] == 'R'){
                RFlag = 1;
            }
            else{
                printf("There is no such option: %c, please provide correct extension. (such as -i, -l, or -R)\n", argv[x][y]);
                return 1;
            }
        }
    }

    //user has the argument with specified path
    if(lastFlag == 1){ 
        if(lFlag == 0){
            listDirectory(argv[argc - 1], listFileName, iFlag, RFlag);
            printf("\n");
            return 0;
        }
        listDirectory(argv[argc - 1], listFileLong, iFlag, RFlag);
        return 0;
    }

    //UnixLs target is the current path
    if(lFlag == 0){
        listDirectory(".", listFileName, iFlag, RFlag);
        printf("\n");
        return 0;
    }

    listDirectory(".", listFileLong, iFlag, RFlag);
    return 0;
}