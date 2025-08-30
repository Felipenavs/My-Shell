#include "myparser.h"


#ifndef MYEXEC_H
#define MYEXEC_H

#define MYBUFSIZE 256

/**
 * @enum MODE
 * @brief Represents how the shell is running.
 */
enum MODE
{
    INTERACTIVE = 1, ///< Interactive mode: user enters commands via terminal
    BATCH = 2,       ///< Batch mode: commands read from file or pipe
};

/**
 * @brief Detect whether the shell is running in interactive or batch mode.
 * 
 * @param fd   File descriptor (usually stdin).
 * @param argc Argument count (used to distinguish batch mode).
 * @return INTERACTIVE or BATCH.
 */
enum MODE get_terminal_mode(int fd, int argc);

/**
 * @brief Redirect standard input from a file.
 * 
 * @param file Path to the file to use as stdin.
 * @return 0 on success, -1 if opening/dup2 fails.
 */
int set_stdin(const char * file);

/**
 * @brief Redirect standard output to a file (truncate/overwrite,create).
 * 
 * @param file Path to the file to use as stdout.
 * @return 0 on success, -1 if opening/dup2 fails.
 */
int set_stdout(const char * file);

/**
 * @brief Execute a single process, handling arguments, redirection, and exec.
 * 
 * @param procs Pointer to the Process structure.
 * @return 0 on success, -1 on error (e.g., fork or exec failure).
 */
int execute_process(Process * procs);

/**
 * @brief Execute a process where only the bare command name is given.
 * 
 * Searches PATH for the binary if no absolute/relative path is specified.
 * 
 * @param procs Pointer to the Process structure.
 * @return 0 on success, -1 if command not found or exec fails.
 */
int execute_bare_name_command(Process * procs);

/**
 * @brief Handle execution of a pipeline of processes in a job.
 * 
 * @param myjob Pointer to the Job structure containing processes.
 * @param pids  Array to store the process IDs of the created children.
 * @return 0 on success, -1 on failure.
 */
int process_pipe(Job * myjob, int * pids);

/**
 * @brief Execute an entire job consisting of one or more processes.
 * 
 * @param myjob Pointer to the Job structure.
 * @param current_procs Pointer to the current process in the job.
 */
void execute_job(Job * myjob, Process * current_procs);

/**
 * @brief Process input from user or file, allocating/resizing buffer as needed.
 * 
 * Reads one line of input into command_line, growing the buffer if necessary.
 * 
 * @param mode             INTERACTIVE or BATCH mode.
 * @param command_line     Pointer to the buffer pointer for storing input.
 * @param command_line_size Pointer to the size of the buffer.
 * @return 0 on succes, -1 on error.
 */
int process_input(enum MODE mode, char **command_line, int *command_line_size);

#endif
