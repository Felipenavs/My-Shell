#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "myjob.h"

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

Process * job_add_procs(Job * job)
{

    if(job->processes_count >= job->capacity)
    {
        job->capacity = job->capacity * 2;
        Process * temp = realloc(job->processes, sizeof(Process) * job->capacity);
        if(!temp)
        {
            perror("realloc");
            return NULL;
        }
        job->processes = temp;
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