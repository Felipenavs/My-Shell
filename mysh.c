#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtincommands.h"
#include "myparser.h"
#include "myarraylist.h"
#include "myexec.h"



int main(int argc, char *argv[]){

    int fd = isatty(STDIN_FILENO);
    int command_line_size = MYBUFSIZE;

    //stores the complete command line
    char * command_line = malloc(command_line_size);

    enum MODE mode = get_terminal_mode(fd, argc);
    
    switch (mode)
    {
    case INTERACTIVE:

        //welcome message
        const char * welcome_str = "Welcome to my shell!\n";
        //prints welcome message
        int ws = write(STDOUT_FILENO, welcome_str, strlen(welcome_str));
        if(ws == -1)
        {
            perror("write");
            exit(1);
        }
        if(process_input(mode,&command_line, &command_line_size) == -1)
        {
            free(command_line);
            exit(EXIT_FAILURE);
        }
        break;
    
    case BATCH:

        if(argc == 2)
        {
            if(set_stdin(argv[1]) == -1)
            {
                exit(EXIT_FAILURE);
            }
        }
        if(process_input(mode,&command_line, &command_line_size) == -1)
        {
            free(command_line);
            exit(EXIT_FAILURE);
        }
    default:

        free(command_line);
        exit(EXIT_FAILURE);
    }

    free(command_line);
    exit(EXIT_SUCCESS);
}