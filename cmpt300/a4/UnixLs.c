#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

//Helper for printing files when they are found as command line arguments
int is_file(char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

//Unix print what ls returns with different parameters
int printls(struct dirent* de, char* param, char* fullPath) {
	if (strchr(param, 'l') != NULL) {
        struct stat* itemStats = calloc(4, sizeof *itemStats);
        stat(fullPath, itemStats);
        struct passwd* owner = getpwuid(itemStats->st_uid);
        struct group* group = getgrgid(itemStats->st_gid);
        char date[20];
        strftime(date, 20, "%b %d %Y %H:%M", localtime(&(itemStats->st_ctime)));

        if (strchr(param, 'i') != NULL)
            printf("%lld ",de->d_ino);

        printf( (S_ISDIR(itemStats->st_mode)) ? "d" : "-");
        printf( (itemStats->st_mode & S_IRUSR) ? "r" : "-");
        printf( (itemStats->st_mode & S_IWUSR) ? "w" : "-");
        printf( (itemStats->st_mode & S_IXUSR) ? "x" : "-");
        printf( (itemStats->st_mode & S_IRGRP) ? "r" : "-");
        printf( (itemStats->st_mode & S_IWGRP) ? "w" : "-");
        printf( (itemStats->st_mode & S_IXGRP) ? "x" : "-");
        printf( (itemStats->st_mode & S_IROTH) ? "r" : "-");
        printf( (itemStats->st_mode & S_IWOTH) ? "w" : "-");
        printf( (itemStats->st_mode & S_IXOTH) ? "x" : "-");

        printf("%d %s %s %011d %s %s\n", itemStats->st_nlink, owner->pw_name, group->gr_name, itemStats->st_size, date, de->d_name);
        free(itemStats);
    }
    else if (strchr(param, 'i') != NULL)
        printf("%lld %s\n",de->d_ino, de->d_name);
	else
		printf("%s\n", de->d_name);
	return 0;
}

//Unix ls instruction (recursive)
int ls(char* dir, char* param) {
	 
	struct dirent* de;
    DIR *dr = opendir(dir);
    
    if (is_file(dir) != 0) {
        printf("\n%s\n",dir);
        return 0;
    } else if (dr == NULL) { // opendir returns NULL if couldn't open directory 
        printf("UnixLs: cannot access '%s': No such file or directory\n", dir);
        return -1;
    } 
 	
 	LIST* subDirs = ListCreate();

 	//Print contents of current working directory and adds any sub directory to a list of sub directories
    while ((de = readdir(dr)) != NULL) {
    	if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
        	char* subDirName = malloc(255*sizeof(char));
        	strcpy(subDirName, dir);
        	strcat(subDirName, "/");
        	strcat(subDirName, de->d_name);
            printls(de, param, subDirName);
        	if (opendir(subDirName) != NULL && strchr(param,'R') != NULL)
        		ListPrepend(subDirs, subDirName);
        	else
        		free(subDirName);
        }
    }

    //Print all contents of sub directories recursively
    while (ListCount(subDirs) > 0) {
    	char* subDir = ListTrim(subDirs);
    	printf("\n%s:\n",subDir);
    	ls(subDir, param);
    	free(subDir);
    }


    ListFree(subDirs, itemFree);
   	closedir(dr);

   	return 0;
}
 
int main(int argc, char* argv[])
{

    char arguments[128];
    memset(arguments, '\0', 128);

    LIST* directoryArgs = ListCreate();

    //Iterates through command line arguments and parses appropriate parameter options. Also appends files/folders to directoryArgs list to be processed
    for (int i = 1; argv[i] != NULL; i++) {
        if (argv[i][0] == '-') {
            strcat(arguments, argv[i]);
            if (argv[i+1] != NULL)
                strcat(arguments, " ");
        } else {
            char* fileFolder = calloc(255, 255*sizeof(char));
            strcpy(fileFolder, argv[i]);
            ListPrepend(directoryArgs, fileFolder);
        }  
    }

    //Call ls on all directories/files in directoryArgs with appropriate parameters
    if (ListCount(directoryArgs) == 0)
        ls(".", arguments);
    else if (ListCount(directoryArgs) == 1) {
        char* fileFolder = ListTrim(directoryArgs);
        ls(fileFolder, arguments);
        free(fileFolder);
    } else {
        char* fileFolder = ListTrim(directoryArgs);
        if (is_file(fileFolder) == 0)
            printf("%s:\n", fileFolder);
        ls(fileFolder, arguments);
        free(fileFolder);
        while (ListCount(directoryArgs) > 0) {
            fileFolder = ListTrim(directoryArgs);
            if (is_file(fileFolder) == 0)
                printf("\n%s:\n", fileFolder);
            ls(fileFolder, arguments);
            free(fileFolder);
        }
    }
 
    return 0;
}