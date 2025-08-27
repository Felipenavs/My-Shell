#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "myutility.h"



int is_pipe(char c)
{
    return c == '|';
}

int is_input_redirection(char c)
{
    return c == '<';
}

int is_output_redirection(char c)
{
    return c == '>';
}

int is_word_char(char c)
{
    return (!isspace(c) && (c!= '\n'));
}

enum token_type get_token_type(char c)
{
    if(is_pipe(c)) return PIPE;
    if(is_input_redirection(c)) return INPUT_RDRTN;
    if(is_output_redirection(c)) return OUTPUT_RDRTN;
    return WORD;
}

char * check_str_capacity(char * str, unsigned length, unsigned * capacity)
{
    if(length >= *capacity)
    {
        *capacity *= 2;
        str = realloc(str, *capacity);
        if(!str)
        {
            perror("realloc failed");
            return NULL;
        }
    }

    return str;
}

int split_path(const char *fullpath, char **dir, char **file) {
    const char *last_slash = strrchr(fullpath, '/');
    if (!last_slash) {
        // No directory part
        *dir = strdup("."); // current directory
        *file = strdup(fullpath);
        if(!dir || !file)
        {
            perror("strdup() failed");
            return -1;
        }
    } else {
        size_t dir_len = last_slash - fullpath;
        *dir = malloc(dir_len + 1);
        if(!dir)
        {
            perror("malloc failed");
            return -1;
        }
        strncpy(*dir, fullpath, dir_len);
        (*dir)[dir_len] = '\0';
        *file = strdup(last_slash + 1);
        if(!file)
        {
            perror("strdup() failed");
            return -1;
        }
    }

    return 1;
}



// Check if name matches pattern with single '*'
int match_pattern(const char *pattern, const char *name) {
    const char *star = strchr(pattern, '*');
    if (!star) return -1;

    size_t prefix_len = star - pattern;
    size_t suffix_len = strlen(star + 1);
    size_t name_len = strlen(name);

    if (name_len < prefix_len + suffix_len) return false;
    if (prefix_len && strncmp(pattern, name, prefix_len) != 0) return false;
    if (suffix_len && strcmp(name + name_len - suffix_len, star + 1) != 0) return false;

    if (name[0] == '.' && pattern[0] == '*') return false; 
    return 1;
}

// Expand wildcard in a single directory
int expand_wildcard(const char *dir_path, const char *pattern, array_t * L) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror(dir_path);
        return -1;
    }

    struct dirent *de;
    while ((de = readdir(dir))) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        if (match_pattern(pattern, de->d_name)) {
            if(!al_append(L, de->d_name))
            {
                return -1;
            }
        }
    }
    closedir(dir);
    return 1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

