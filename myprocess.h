#include "myarraylist.h"

#ifndef MYPROCESS_H
#define MYPROCESS_H

/**
 * @struct Process
 * @brief  Represents a single process in a job.
 *
 * A Process holds:
 * - arguments : list of command-line arguments (argv style)
 * - execpath  : the executable path for the process
 * - inputfile : filename to redirect stdin from (NULL if none)
 * - outputfile: filename to redirect stdout to (NULL if none)
 * - in_pipe   : set to 1 if this process receives input from a pipe
 * - out_pipe  : set to 1 if this process outputs to a pipe
 */
typedef struct
{   
    array_t arguments;
    char *execpath;
    char *inputfile;
    char *outputfile;
    int in_pipe;
    int out_pipe;
} Process;

/**
 * @brief Initialize a Process structure.
 *
 * @param procs Pointer to the Process to initialize.
 * @return -1 on failure.
 */
int init_process(Process * procs);

/**
 * @brief Free memory owned by a Process.
 *
 * @param procs Pointer to the Process to destroy.
 *
 * Note: Frees the arguments array and any dynamically
 *       allocated filenames or execpath inside the Process.
 */
void destroy_process(Process * procs);

/**
 * @brief Add an argument to a Process.
 *
 * @param procs Pointer to the Process.
 * @param arg   Argument string.
 * @return -1 on failure.
 */
int procs_add_argument(Process * procs, char * arg);

/**
 * @brief Set the executable path of the Process.
 *
 * @param procs Pointer to the Process.
 * @param arg   String containing the exec path.
 * @return -1 on failure.
 */
int procs_set_execpath(Process * procs, char * arg);

/**
 * @brief Set the input redirection file.
 *
 * @param procs Pointer to the Process.
 * @param arg   String containing the filename for stdin.
 * @return -1 on failure.
 */
int procs_set_inputfile(Process * procs, char * arg);

/**
 * @brief Set the output redirection file.
 *
 * @param procs Pointer to the Process.
 * @param arg   String containing the filename for stdout.
 * @return -1 on failure.
 */
int procs_set_outputfile(Process * procs, char * arg);

#endif
