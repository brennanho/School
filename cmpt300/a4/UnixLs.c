#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

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

        printf("%d %s %s %d %s %s\n", itemStats->st_nlink, owner->pw_name, group->gr_name, itemStats->st_size, date, de->d_name);
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
 
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
        return -1;
 	
 	LIST* subDirs = ListCreate();

 	//Print contents of current working directory and adds any sub directory to a list of sub directories
    while ((de = readdir(dr)) != NULL) {
    	if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
        	char* subDirName = malloc(1024*sizeof(char));
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
    char* param;
    char arguments[128];
    memset(arguments, '\0', 128);
    
    for (int i = 1; argv[i] != NULL; i++) {
        strcat(arguments, argv[i]);
        if (argv[i+1] != NULL)
            strcat(arguments, " ");
    }

    printf("%s\n",arguments);

    ls(".", arguments);
 
    return 0;
}