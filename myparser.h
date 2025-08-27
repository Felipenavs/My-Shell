#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "myjob.h"

#ifndef MYPARSER_H
#define MYPARSER_H

/**
 * @brief Tokenize a command line string.
 *
 * @param tokens       Pointer to an array_t to hold string tokens.
 * @param command_line Raw input string.
 * @param array_size   size of the raw input string.
 * @return -1 on failure.
 *
 * Example:
 *   Input:  "ls -l | grep foo"
 *   Tokens: ["ls", "-l", "|", "grep", "foo"]
 */
int get_tokens(array_t * tokens, const char * command_line, int string_size);


/**
 * @brief Parse a shell command line into a Job structure.
 *
 * @param job          Pointer to a Job to fill with parsed Processes.
 * @param command_line Input string (e.g., "ls -l | grep foo > out.txt").
 * @param array_size   Initial size of raw input string.
 * @return -1 on failure.
 *
 * This function:
 *  - Breaks the command_line into tokens
 *  - Groups tokens into Processes (split by pipes)
 *  - Fills execpath, arguments, redirections (>, <), etc.
 */
int parse_command_line(Job * job, const char * command_line, int array_size);

#endif
