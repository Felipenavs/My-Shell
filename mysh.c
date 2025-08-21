#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "myparser.h"

#define MYBUFSIZE 256

int main(int argc, char *argv[]){

    int fd = isatty(STDIN_FILENO);


    //Interactive mode
    if(fd && argc == 1){

        //buffer to read from terminal
        char buffer[MYBUFSIZE];

        //stores the complete command line typed by user
        int sub_buffer_size = MYBUFSIZE;
        char * command_line = malloc(sub_buffer_size);

        //welcome message
        const char * welcome_str = "Welcome to my shell!";

        //prompt to let user know terminal is ready for next command
        const char * prompt_str = "\nmysh> ";

        //prints welcome message
        int os = write(STDOUT_FILENO, welcome_str, strlen(welcome_str));

        
        int read_flag;
        int prompt_flag = 1;
        while(prompt_flag) //runs until exit command is passed
        {
            //writes prompt to let user know terminal is ready for a new command
            os = write(STDOUT_FILENO, prompt_str, strlen(prompt_str));

            read_flag =1;
            int position = 0; 
            int r;
            while(read_flag)
            {
                //reads from terminal
                r = read(STDIN_FILENO, buffer, MYBUFSIZE);
                
                if(r < 0 ){
                    perror("Error reading from terminal");
                    break;
                }

                //makes sure command_line array is big enough for all the data
                if((position +r)>= sub_buffer_size)
                {
                    sub_buffer_size *= 2;
                    char * temp = command_line;
                    command_line = realloc(command_line, sub_buffer_size);
                    if(!command_line)
                    {
                        perror("realloc()");
                        free(temp);
                        break;
                    }
                }

                //copies the contents of buffer into the command line array in the last position to make sure no
                //to overwrite previous data
                memcpy(&command_line[position], buffer, r);
                position +=r;

                //if the last char written in the buffer is new line, then stop reading.
                if(buffer[r-1] == '\n')
                {
                    read_flag = 0;
                }
            }

            //continue

            write(STDOUT_FILENO, command_line, position-1);
        }

    }
    //Batch mode
    else if(!fd || argc <= 2)
    {

    }
    else
    {
        // wrong number of arguments
    }

    




}