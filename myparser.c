#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myutility.h"
#include "myparser.h"


int get_tokens(array_t * tokens, const char * command_line, int array_size)
{
    
    unsigned buffer_size = 32;
    char * word_buffer = malloc(buffer_size);
    if(!word_buffer)
    {
        perror("malloc failed");
        return -1;
    }

    unsigned word_size = 0;

    for (int i = 0; i < array_size; i++)
    {
        if(is_pipe(command_line[i]) || is_input_redirection(command_line[i]) || is_output_redirection(command_line[i]))
        {
            if(word_size == 0)
            {
                word_buffer[0] = command_line[i];
                word_buffer[1] = '\0';
                if(al_append(tokens, word_buffer) == -1)
                {
                    return -1;
                }
                continue;
            }
            
            
            if(!check_str_capacity(word_buffer, word_size, &buffer_size))
            {
                return -1;
            }

            word_buffer[word_size] = '\0';
            char * token = malloc(sizeof(char)*2);
            if(!token)
            {
                perror("malloc failed");
                return -1;
            }
            token[0] = command_line[i];
            token[1] = '\0';

            if(al_append(tokens, word_buffer) ==-1 || al_append(tokens, token) == -1)
            {
                return -1;
            }
            
            word_size = 0;
            free(token);
            
        }
        else if(is_word_char(command_line[i]))
        {
            if(!check_str_capacity(word_buffer, word_size, &buffer_size))
            {
                return -1;
            }

            word_buffer[word_size] = command_line[i];
            word_size++;

        }
        else
        {

            if(word_size == 0)
            {
                continue;
            }

            if(!check_str_capacity(word_buffer, word_size, &buffer_size))
            {
                return -1;
            }

            word_buffer[word_size] = '\0';
            word_size = 0;
            if(al_append(tokens, word_buffer) == -1)
            {
                return -1;
            }
        }

    }

    free(word_buffer);
    return 0;
}



int parse_command_line(Job * job, const char * command_line, int array_size)
{

    // char * word = "pcl test\n";
    
    
    array_t tokens;
    if(al_init(&tokens,20) == -1)
    {
        return -1;
    }
    
    Process * current_process = job_add_procs(job); 
    if(current_process == NULL)
    {
        return -1;
    }

    
    if(get_tokens(&tokens,command_line, array_size) ==-1)
    {
        return -1;
    }

    // for(int i=0; i<tokens.length; i++)
    // {
    //     write(STDOUT_FILENO, tokens.data[i], strlen(tokens.data[i]));
    // }

    // write(STDOUT_FILENO,word , strlen(word));

    char * current_word = NULL;
    char first_char;
    for(int i=0; i<tokens.length; i++)
    {
        
        current_word = tokens.data[i];
        enum token_type token = get_token_type(current_word[0]);

        switch (token)
        {
        case PIPE:

            if(current_process->arguments.length == 0)
            {
                //output error, wrong command format
                //return NULL?
            }

            if(current_process->outputfile)
            {
                //output error, wrong command format
                //return NULL?
            }

            i++;
            if(i >= tokens.length)
            {
                //output error, wrong command format
                return -1;
            }

            current_word = tokens.data[i];
            first_char = current_word[0];
            if(is_input_redirection(first_char) || is_output_redirection(first_char) || is_pipe(first_char))
            {
                //output error, wrong command format
                //return NULL?
            }

            Process * prev_procs = current_process;
            current_process = job_add_procs(job);
            if(!current_process)
            {
                // return NULL;
            }

            prev_procs->out_pipe =1;
            current_process->in_pipe=1;

            if(!procs_add_argument(current_process, current_word) || !procs_set_execpath(current_process, current_word))
            {
                // return NULL;
            }
            break;
        
        case INPUT_RDRTN:

            if(current_process->arguments.length == 0)
            {
                //output error, wrong command format
                //return NULL?
            }

            if(current_process->inputfile)
            {
                //output error, wrong command format
                //return NULL?
            }

            i++;
            if(i >= tokens.length)
            {
                //output error, wrong command format
                // return NULL;
            }

            current_word = tokens.data[i];
            first_char = current_word[0];
            if(is_input_redirection(first_char) || is_output_redirection(first_char) || is_pipe(first_char))
            {
                //output error, wrong command format
                //return NULL?
            }

            if(!procs_set_inputfile(current_process, current_word))
            {
                // return NULL;
            }

            break;

        case OUTPUT_RDRTN:

            if(current_process->arguments.length == 0)
            {
                //output error, wrong command format
                //return NULL?
            }

            if(current_process->outputfile)
            {
                //output error, wrong command format
                //return NULL?
            }

            i++;
            if(i >= tokens.length)
            {
                //output error, wrong command format
                // return NULL;
            }

            current_word = tokens.data[i];
            first_char = current_word[0];
            if(is_input_redirection(first_char) || is_output_redirection(first_char) || is_pipe(first_char))
            {
                //output error, wrong command format
                //return NULL?
            }

            if(!procs_set_outputfile(current_process, current_word))
            {
                // return NULL;
            }
            
            break;

        case WORD:

            //if it is the first argument, add it to arg array and set execpath
            if(current_process->arguments.length == 0)
            {
                
                if(procs_set_execpath(current_process, current_word) ==-1 || al_append(&current_process->arguments, current_word) == -1)
                {
                    return -1;
                }
                break;
            }

            //look for wildcards.
            if(strchr(current_word, '*'))
            {
                
                array_t * wildcards = malloc(sizeof(array_t));
                if(!wildcards)
                {
                    perror("malloc failed");
                    return -1;
                }
                if(al_init(wildcards, 10) == -1)
                {
                    return -1;
                }

                char *dir, *file_pattern;
                if(split_path(current_word, &dir, &file_pattern) < 0 || expand_wildcard(dir, file_pattern, wildcards) < 0)
                {
                    return -1;
                }
                

                for(int i =0; i < wildcards->length; i++){
                    if(procs_add_argument(current_process, wildcards->data[i]) == -1)
                    {
                        return -1;
                    }
                }

                free(dir);
                free(file_pattern);
                free(wildcards);
                break;
            }
            
            if(procs_add_argument(current_process, current_word) == -1)
            {
                return -1;
            }
            break;
            
        }

    }

    return 0;
}