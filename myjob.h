#include <string.h>
#include "myprocess.h"

#ifndef MYJOB_H
#define MYJOB_H

/**
 * @struct Job
 * @brief  Represents a collection of processes (e.g., a pipeline).
 *
 * A Job holds:
 * - capacity        : max number of Process slots allocated
 * - processes_count : current number of processes in use
 * - processes       : dynamically allocated array of Process structs
 *
 * Example: For the command "ls -l | grep foo | wc -l",
 * the Job would contain three Process objects.
 */
typedef struct
{
    unsigned capacity;
    size_t processes_count;
    Process * processes;
} Job;

/**
 * @brief Initialize a Job structure.
 *
 * @param job Pointer to the Job to initialize.
 * @return -1 on failure.
 */
int init_job(Job * job);

/**
 * @brief Add a new Process to the Job.
 *
 * @param j Pointer to the Job.
 * @return Pointer to the newly added Process, or NULL on failure.
 *
 * Note: Expands the underlying storage if necessary.
 */
Process * job_add_procs(Job * j);

/**
 * @brief Free memory owned by a Job and its Processes.
 *
 * @param job Pointer to the Job to destroy.
 *
 * Note: Calls destroy_process() for each Process in the Job,
 *       and frees the process array itself.
 */
void destroy_job(Job * job);

#endif
