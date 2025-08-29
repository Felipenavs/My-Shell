#include "myprocess.h"

#ifndef MYBUILTINCOMNDS_H
#define MYBUILTINCOMNDS_H

/**
 * @enum BuiltInCommand
 * @brief Represents the set of built-in commands supported by myshell.
 *
 * These are the commands that do not require creating a child process,
 * but instead are executed directly inside myshell.
 */
enum BuiltInCommand {
    CD = 0,     /**< Change the current working directory. */
    PWD = 1,    /**< Print the current working directory. */
    WHICH = 2,  /**< Locate the full path of an executable. */
    EXIT = 3,   /**< Exit the shell. */
    NONE = -1   /**< Not a built-in command. */
};

/**
 * @brief Check if a given command string matches a built-in command.
 *
 * @param command The command string to check (e.g., "cd", "pwd").
 * @return The corresponding BuiltInCommand enum value,
 *         or NONE if the command is not built-in.
 */
enum BuiltInCommand is_built_in_command(char *command);

/**
 * @brief Implements the 'cd' (change directory) built-in command.
 *
 * @param procs The Process structure containing the command arguments.
 *              Expects procs->arguments to contain the target directory.
 */
void mycd(Process *procs);

/**
 * @brief Implements the 'pwd' (print working directory) built-in command.
 *
 * @return A dynamically allocated string containing the current working directory.
 *         The caller is responsible for freeing the returned string.
 */
char *mypwd();

/**
 * @brief Implements the 'which' built-in command.
 *
 * Finds the location of the executable for the command provided in
 * procs->arguments.
 *
 * @param procs The Process structure containing the command arguments.
 */
void mywhich(Process *procs);

/**
 * @brief Implements the 'exit' built-in command.
 *
 * Terminates the shell program, cleaning up any necessary resources.
 *
 * @param procs The Process structure containing any exit arguments.
 *              (In most cases, unused.)
 */
void myexit(Process *procs);

#endif


