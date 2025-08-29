#include "myarraylist.h"

#ifndef MYUTIL_H
#define MYUTIL_H

/**
 * @file myutil.h
 * @brief Utility functions for tokenizing and processing shell command strings.
 *
 * This header defines helper functions and enums for classifying characters
 * in a shell command, handling tokens (pipes, redirections, words),
 * expanding wildcards, path manipulation, and string utilities.
 */

/**
 * @enum token_type
 * @brief Types of tokens in a command line.
 *
 * Used by the parser to classify characters into logical command tokens.
 *
 * - PIPE        → `|`
 * - INPUT_RDRTN → `<`
 * - OUTPUT_RDRTN→ `>`
 * - WORD        → General text (commands, arguments, filenames)
 */
enum token_type {
    PIPE = 0,         /**< Pipe operator `|` */
    INPUT_RDRTN = 1,  /**< Input redirection operator `<` */
    OUTPUT_RDRTN = 2, /**< Output redirection operator `>` */
    WORD = -1         /**< General word token (not an operator) */
};

/**
 * @brief Determine token type from a single character.
 *
 * @param c Character to classify.
 * @return One of PIPE, INPUT_RDRTN, OUTPUT_RDRTN, or WORD.
 */
enum token_type get_token_type(char c);

/**
 * @brief Check if a character is a pipe ('|').
 *
 * @param c Character to check.
 * @return 1 if pipe, 0 otherwise.
 */
int is_pipe(char c);

/**
 * @brief Check if a character is an input redirection ('<').
 *
 * @param c Character to check.
 * @return 1 if input redirection, 0 otherwise.
 */
int is_input_redirection(char c);

/**
 * @brief Check if a character is an output redirection ('>').
 *
 * @param c Character to check.
 * @return 1 if output redirection, 0 otherwise.
 */
int is_output_redirection(char c);

/**
 * @brief Check if a character is part of a word token.
 *
 * A "word" is any character that isn’t a pipe, redirection operator,
 * space, or newline.
 *
 * @param c Character to check.
 * @return 1 if valid word character, 0 otherwise.
 */
int is_word_char(char c);

/**
 * @brief Expand wildcard patterns (like *.c) in a directory.
 *
 * Searches a directory for files matching a given wildcard pattern
 * and appends matches to the provided array list.
 *
 * @param dir_path Directory to search.
 * @param pattern  Wildcard pattern (supports * and ?).
 * @param L        Array list to store matches (strings are appended).
 * @return -1 on error, 0 otherwise.
 */
int expand_wildcard(const char *dir_path, const char *pattern, array_t * L);

/**
 * @brief Match a filename against a wildcard pattern.
 *
 * Supported wildcards:
 * - `*` matches zero or more characters.
 * - `?` matches exactly one character.
 *
 * @param pattern Pattern string.
 * @param name    File or string to test.
 * @return 1 if it matches, -1 otherwise.
 */
int match_pattern(const char *pattern, const char *name);

/**
 * @brief Split a full file path into directory and file parts.
 *
 * Example:
 * - Input: "/usr/bin/ls"
 * - Output: dir="/usr/bin", file="ls"
 *
 * @param fullpath Input path string.
 * @param dir      Output: directory string (malloc’d, caller frees).
 * @param file     Output: filename string (malloc’d, caller frees).
 * @return 1 on success, -1 on failure.
 */
int split_path(const char *fullpath, char **dir, char **file);

/**
 * @brief Ensure a string buffer has enough capacity.
 *
 * If `length == capacity`, doubles the buffer size using realloc.
 *
 * @param str      String buffer (malloc’d).
 * @param length   Current string length (not including '\0').
 * @param capacity Pointer to current capacity (updated on growth).
 * @return Pointer to valid buffer (may change if realloc succeeds),
 *         or NULL if realloc failed.
 */
char * check_str_capacity(char * str, unsigned length, unsigned * capacity);

/**
 * @brief Convert a string to lowercase in-place.
 *
 * @param str Null-terminated string to modify.
 */
void to_lowercase(char *str);

/**
 * @brief Check if a character is a quote (' or ").
 *
 * @param c Character to check.
 * @return 1 if quote, 0 otherwise.
 */
int is_quote(char c);

/**
 * @brief Grab a quoted string from a command line.
 *
 * Extracts a substring between quotes and appends it to the token list.
 *
 * @param command_line   Input command line string.
 * @param index          Current parsing position (updated).
 * @param command_line_size Size of the command line buffer.
 * @param quote          The quote character used (' or ").
 * @param tokens         Token array list to append the string to.
 * @return 1 if it finds a string, 0 if not string is found(e.g., unterminated quote), -1 on error.
 */
int grab_string(const char *command_line, int *index, int command_line_size,
                char quote, array_t * tokens);

/**
 * @brief Handle an invalid command format.
 *
 * Prints an error message.
 *
 * @param word The invalid word or token.
 */
void invalid_format(char * word);

#endif
