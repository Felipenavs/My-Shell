#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "builtincommands.h"
#include "myutility.h"
 
char * mypwd(){
    char * cwd = malloc(PATH_MAX);
    if(!cwd)
    {
        perror("malloc");
        return NULL;
    }
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd()");
        free(cwd);
        return NULL;
    } 
    return cwd;
}

void mycd(Process * procs){
    char path[PATH_MAX];
    
    //makes sure only one argument was passed
    if(procs->arguments.length != 2)
    {
        fprintf(stdout, "%s\n", "cd: invalid number of arguments");
        return;
    }

    //the directory to move to
    char * dir = procs->arguments.data[1];
    
    //full path was passed as argument
    if(dir[0] == '/')
    {
        snprintf(path, sizeof(path), "%s", dir);
    }
    else //relative path was passed as argument
    {
        //gets current directory
        char * cwd = mypwd();
        if(!cwd)
        {
            return;
        }
        //creates full path
        snprintf(path, sizeof(path), "%s/%s", cwd, dir);
        free(cwd);
    }

    //makes sure the directory can be traverse to
    if(access(path, X_OK)!=0)
    {
        perror(path);
        return;
    }

    //change dir
    if(chdir(path) == -1)
    {
        perror("chdir");
    }

}


void mywhich(Process * procs)
{
    //makes sure valid number of arguments were passed
    if(procs->arguments.length != 2)
    {
        return;
    }

    //holds the arg
    char * arg = procs->arguments.data[1];

    if(is_built_in_command(arg) >= 0)
    {
        return;
    }

    char * dirs[] = {"/usr/local/bin", "/usr/bin", "/bin", NULL}; 
    for (int i = 0; dirs[i] != NULL; i++) 
    {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirs[i], arg);
        if (access(fullpath, F_OK) == 0){
            fprintf(stdout, "%s\n", fullpath);
            return;
        }
    }
}

void myexit(Process * procs, int * flag)
{
    if(procs->arguments.length > 1)
    {
        for(int i =1; i<procs->arguments.length; i++ )
        {
            char word[PATH_MAX];
            snprintf(word,sizeof(word), "%s ",procs->arguments.data[i]);
            write(STDOUT_FILENO, word, strlen(word));
        }
    }
    *flag = 0;
}


enum BuiltInCommand is_built_in_command(char * command)
{
    to_lowercase(command);
    if(strcmp(command, "cd") == 0) return CD;
    if(strcmp(command, "pwd") == 0) return PWD;
    if(strcmp(command, "which") == 0) return WHICH;
    if(strcmp(command, "exit") == 0) return EXIT;
    return NONE;
}


