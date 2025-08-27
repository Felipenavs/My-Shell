#include "myarraylist.h"

#ifndef MYUTIL_H
#define MYUTIL_H

/**
 * @brief Types of tokens in a command line.
 * 
 * PIPE           → |
 * INPUT_RDRTN    → <
 * OUTPUT_RDRTN   → >
 * WORD           → general text (commands, args, filenames)
 */
enum token_type {
    PIPE = 0,
    INPUT_RDRTN = 1,
    OUTPUT_RDRTN = 2,
    WORD = -1
};

/**
 * @brief Determine token type from a single character.
 *
 * @param c Character to classify.
 * @return One of PIPE, INPUT_RDRTN, OUTPUT_RDRTN, or WORD.
 */
enum token_type get_token_type(char c);

/**
 * @brief Check if a character is a pipe '|'.
 */
int is_pipe(char c);

/**
 * @brief Check if a character is an input redirection '<'.
 */
int is_input_redirection(char c);

/**
 * @brief Check if a character is an output redirection '>'.
 */
int is_output_redirection(char c);

/**
 * @brief Check if a character is part of a word token.
 *
 * A "word" means it isn’t a pipe, redirection operator, space or '\n'
 */
int is_word_char(char c);

/**
 * @brief Expand wildcard patterns (like *.c) in a single directory.
 *
 * @param dir_path Directory to search.
 * @param pattern  Wildcard pattern (e.g., "*.txt").
 * @param L        Array list to store matches (strings are appended).
 * @return -1 on error.
 */
int expand_wildcard(const char *dir_path, const char *pattern, array_t * L);

/**
 * @brief Match a filename against a wildcard pattern.
 *
 * @param pattern Pattern string (supports * and ?).
 * @param name    File or string to test.
 * @return 1 if it matches, -1 otherwise.
 */
int match_pattern(const char *pattern, const char *name);

/**
 * @brief Split a full file path into directory and file parts.
 *
 * @param fullpath Input path (e.g., "/usr/bin/ls").
 * @param dir      Output: directory string (malloc’d).
 * @param file     Output: filename string (malloc’d).
 * @return 1 on success, -1 on failure.
 */
int split_path(const char *fullpath, char **dir, char **file);

/**
 * @brief Ensure a string buffer has enough capacity.
 *
 * If length == capacity, doubles the buffer size with realloc.
 * 
 * @param str      The string buffer (malloc’d).
 * @param length   Current string length.
 * @param capacity Pointer to capacity (updated on growth).
 * @return Pointer to (possibly new) buffer, or NULL if realloc failed.
 */
char * check_str_capacity(char * str, unsigned length, unsigned * capacity);

/**
 * @brief Convert a string to lowercase in-place.
 *
 * @param str Null-terminated string to modify.
 */
void to_lowercase(char *str);

#endif
