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

    //tracks the size of the current token
    unsigned word_size = 0;
    for (int i = 0; i < array_size; i++)
    {
        //if current character is a redirection token. eg: '|', '<' or '>'
        if(is_pipe(command_line[i]) || is_input_redirection(command_line[i]) || is_output_redirection(command_line[i]))
        {
            //if no current token, then just adds the char as a token
            if(word_size == 0)
            {
                word_buffer[0] = command_line[i];
                word_buffer[1] = '\0'; //appends null terminator
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

            word_buffer[word_size] = '\0'; //appends null terminator to current word

            //creates a token 
            char * token = malloc(sizeof(char)*2);
            if(!token)
            {
                perror("malloc failed");
                return -1;
            }
            token[0] = command_line[i];
            token[1] = '\0';

            //adds the tokens to the tokens array
            if(al_append(tokens, word_buffer) ==-1 || al_append(tokens, token) == -1)
            {
                return -1;
            }
            
            word_size = 0;
            free(token);
        }
        else if(is_word_char(command_line[i])) //current character is part of a "word"
        {   

            if(is_quote(command_line[i]) && word_size ==0 && (array_size - i > 1))
            {
                if(grab_string(command_line,&i,array_size,command_line[i],tokens))
                    continue;
                
            }

            word_buffer = check_str_capacity(word_buffer, word_size, &buffer_size);
            if( word_buffer== NULL)
            {
                return -1;
            }

            //just append it to current word
            word_buffer[word_size] = command_line[i];
            word_size++;

        }
        else //if char is space or '\n' 
        {

            //if not current word is stored just continue
            if(word_size == 0)
            {
                continue;
            }

            if(!check_str_capacity(word_buffer, word_size, &buffer_size))
            {
                return -1;
            }

            //if current word is stored, append null terminator to word and add it to tokens array
            word_buffer[word_size] = '\0';
            word_size = 0;
            if(al_append(tokens, word_buffer) == -1)
            {
                return -1;
            }
        }
    }
    //if loop ends and there still is a token in the buffer.
    //usually happens when reading from a file that doesn't include
    //a \n character.
    if(word_size > 0)
    {
        if(!check_str_capacity(word_buffer, word_size, &buffer_size))
        {
            return -1;
        }

        //if current word is stored, append null terminator to word and add it to tokens array
        word_buffer[word_size] = '\0';
        word_size = 0;
        if(al_append(tokens, word_buffer) == -1)
        {
            return -1;
        }
    }
    free(word_buffer);
    return 0;
}



int parse_command_line(Job * job, const char * command_line, int array_size)
{
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

    char * current_word = NULL;
    char * prev_word = NULL;
    char first_char;

    //loops through all tokens, based on the first character determines if it is an argument or special char
    for(int i=0; i<tokens.length; i++)
    {
        current_word = tokens.data[i];
        enum token_type token = get_token_type(current_word[0]);

        switch (token)
        {
        case PIPE:

            if(current_process->arguments.length == 0)
            {
                invalid_format(current_word);
                return -1;
            }

            if(current_process->outputfile)
            {
                invalid_format(current_word);
                return -1;
            }

            //moves index one position to grab next token
            i++;
            //makes sure there is a next token before retrivieng it 
            if(i >= tokens.length)
            {
                invalid_format(current_word);
                return -1;
            }

            prev_word = current_word;
            current_word = tokens.data[i];
            first_char = current_word[0];
            //if the token right after the pipe is not a 'word' than it's an error
            if(is_input_redirection(first_char) || is_output_redirection(first_char) || is_pipe(first_char))
            {
                invalid_format(prev_word);
                return -1;
            }

            //creates a new process and adds it to the job
            Process * prev_procs = current_process;
            prev_procs->out_pipe =1;
            current_process = job_add_procs(job);
            if(!current_process)
            {
                return -1;
            }

            current_process->in_pipe=1;
            if(procs_add_argument(current_process, current_word) == -1|| procs_set_execpath(current_process, current_word)==-1)
            {
                return -1;
            }
            break;
        
        case INPUT_RDRTN:

            if(current_process->arguments.length == 0)
            {
                invalid_format(current_word);
                return -1;
            }

            if(current_process->inputfile)
            {
                invalid_format(current_word);
                return -1;
            }

            i++;
            if(i >= tokens.length)
            {
                invalid_format(current_word);
                return -1;
            }

            prev_word = current_word;
            current_word = tokens.data[i];
            first_char = current_word[0];
            if(is_input_redirection(first_char) || is_output_redirection(first_char) || is_pipe(first_char))
            {
                invalid_format(prev_word);
                return -1;
            }

            if(procs_set_inputfile(current_process, current_word) ==-1)
            {
                return -1;
            }
            break;

        case OUTPUT_RDRTN:
            
            if(current_process->arguments.length == 0)
            {
                invalid_format(current_word);
                return -1;
            }

            if(current_process->outputfile)
            {
                invalid_format(current_word);
                return -1;
            }

            i++;
            if(i >= tokens.length)
            {
                invalid_format(current_word);
                return -1;
            }

            prev_word = current_word;
            current_word = tokens.data[i];
            first_char = current_word[0];
            if(is_input_redirection(first_char) || is_output_redirection(first_char) || is_pipe(first_char))
            {
                invalid_format(prev_word);
                return -1;
            }

            if(procs_set_outputfile(current_process, current_word) == -1)
            {
                return -1;
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
    
    al_destroy(&tokens);
    return 0;
}