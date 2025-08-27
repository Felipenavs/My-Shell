#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "myjob.h"

//initialize job struct, initially can hold up to three processes can increase if neeeded
//returns NULL if malloc cant allocate memory for processes.
int init_job(Job * job) 
{
    job->capacity = 3;
    job->processes_count = 0;
    job->processes = malloc(sizeof(Process) * job->capacity);
    if(!job->processes)
    {
        perror("malloc failed");
        return -1;
    }
    
    return 0;
}

//creates a new process
//returns null if realloc cant allocate more memory
Process * job_add_procs(Job * job)
{

    if(job->processes_count >= job->capacity)
    {
        job->capacity *= 2;
        job->processes = realloc(job->processes, job->capacity);
        if(!job->processes)
        {
            perror("realloc");
            return NULL;
        }
    }

    Process * procs = &job->processes[job->processes_count];
    if(init_process(procs) == -1)
    {
        return NULL;
    }
    job->processes_count++;
    return procs;
}

void destroy_job(Job * job)
{
    
    for(int i =0; i < job->processes_count; i++)
    {
        destroy_process(&job->processes[i]);
    }
    free(job->processes);
}