#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "myprocess.h"

#define INIT_ARGS_COUNT 10

int init_process(Process * procs)
{
    if(al_init(&procs->arguments, INIT_ARGS_COUNT) == -1)
    {
        return -1;
    }
    procs->execpath = NULL;
    procs->inputfile = NULL;
    procs->outputfile= NULL;
    procs->in_pipe =0;
    procs->out_pipe=0;

    return 0;
}

int procs_add_argument(Process * procs, char * arg)
{

    if(al_append(&procs->arguments, arg) == -1)
    {
        return -1;
    }
    
    return 0;
}

int procs_set_execpath(Process * procs, char * arg)
{
    procs->execpath = strdup(arg);
    if (!procs->execpath) 
    {
        perror("strdup failed");
        return -1;
    }

    return 0;
}

int procs_set_inputfile(Process * procs, char * arg)
{
    procs->inputfile = strdup(arg);
    if (!procs->inputfile) 
    {
        perror("strdup failed");
        return -1;
    }

    return 0;
}

int procs_set_outputfile(Process * procs, char * arg)
{
    procs->outputfile = strdup(arg);
    if (!procs->outputfile) 
    {
        perror("strdup failed");
        return -1;
    }

    return 0;
}

void destroy_process(Process * procs)
{
    al_destroy(&procs->arguments);

    if(procs->execpath)
    {
        free(procs->execpath);
    }

    if(procs->inputfile)
    {
        free(procs->inputfile);
    }

    if(procs->outputfile)
    {
        free(procs->outputfile);
    }
}