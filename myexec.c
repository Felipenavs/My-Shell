#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myutility.h"
#include "builtincommands.h"
#include "myexec.h"


int set_stdin(const char * file)
{
    int fd = open(file, O_RDONLY);
    if(fd ==-1)
    {
        perror(file);
        return -1;
    }

    int rs = dup2(fd, STDIN_FILENO);
    if(rs ==-1)
    {
        perror("dup2");
        return -1;
    }
    close(fd);
    return 0;
}


int set_stdout(const char * file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if(fd ==-1)
    {
        perror(file);
        return -1;
    }

    int rs = dup2(fd, STDOUT_FILENO);
    if(rs ==-1)
    {
        perror("dup2");
        return -1;
    }
    close(fd);
    return 0;
}

int process_pipe(Job * myjob, int * pids)
{
    //neg when setting up first process, stores the read side of pipe for subsequent processes
    int prev_fd = -1;
    for (int i = 0; i < myjob->processes_count; i++) {
        int fd[2];

        //if not in the last process
        if (i < myjob->processes_count - 1) {
            if (pipe(fd) == -1) 
            { 
                perror("pipe"); 
                return -1; 
            }
        }

        pid_t pid = fork();
        /**************************** CHILD STARTS HERE ****************************/
        if (pid == 0) {
            //not the first process
            if (prev_fd != -1) {
                if(dup2(prev_fd, STDIN_FILENO)==-1)
                {
                    perror("dup2");
                    return -1;
                }
                close(prev_fd);
            }

            //if it is the first process
            if(prev_fd == -1 && myjob->processes[i].inputfile){
                if(set_stdin(myjob->processes[i].inputfile) == -1)
                    return -1;
            }

            //not the last process
            if (i < myjob->processes_count - 1) {
                if(dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    return -1;
                }
                close(fd[0]);
                close(fd[1]);
            }
            else//if its the last process
            {
                if(myjob->processes[i].outputfile){
                    if(set_stdout(myjob->processes[i].outputfile) == -1)
                        return -1;
                }
            }
            
            execute_process(&myjob->processes[i]);
        }
        /**************************** CHILD ENDS HERE ****************************/

        /**************************** PARENT CONTINUES ****************************/
        //if it's not the first process
        if (prev_fd != -1) close(prev_fd);

        if (i < myjob->processes_count - 1) {
            close(fd[1]);
            prev_fd = fd[0];
        }
        //saves child's pid
        pids[i] = pid;
    }
    return 0;
}



int execute_process(Process * procs)
{
    //file is an executable
    if(strrchr(procs->execpath, '/'))
    {
        //adds null terminator to argv 
        if(al_null_terminator(&procs->arguments) == -1)
        {        
            return -1;
        }  
         
        if(execv(procs->execpath,procs->arguments.data) ==-1)
        {
            perror("execv()");
            return -1;
        }
    }

    if(execute_bare_name_command(procs) == -1)
    {
        return -1;
    }
    return 0;
}

int execute_bare_name_command(Process * procs)
{
    //dir to search for the program
    char * dirs[] = {"/usr/local/bin", "/usr/bin", "/bin", NULL}; 
    for (int i = 0; dirs[i] != NULL; i++) 
    {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirs[i], procs->execpath);

        if (access(fullpath, X_OK) == 0) {

            //adds null terminator to argv 
            if(al_null_terminator(&procs->arguments) == -1)
            {
                return -1;
            }  

            if(execv(fullpath, procs->arguments.data) == -1)
            {
                perror("execv()");
                return -1;
            }

            return 0;  
        }
    }

    fprintf(stdout, "command %s not found\n", procs->execpath);
    return -1;
}


void execute_job(Job * myjob, Process * current_procs)
{
    //if job has atleast one pipe
    if(myjob->processes_count >1)
    {
        //array to hold pids
        int * pid_array = malloc(sizeof(int) * myjob->processes_count);
        process_pipe(myjob, pid_array);

        //waits for all the child's before moving on
        for(int i= 0; i<myjob->processes_count; i++)
        {
            int status;
            pid_t wpid = waitpid(pid_array[i], &status, 0);
            if (wpid == -1) 
            {
                perror("waitpid");
                return;
            } 
            else 
            {
                if (WIFEXITED(status)) 
                {
                    int code = WEXITSTATUS(status);
                    if (code != 0) {
                        printf("Command Failed: code %d\n", code);
                    }
                } 
                else 
                {
                    printf("Child terminated abnormally\n");
                }
            }

        }
        free(pid_array);
        return;
    }

    //job has no pipes
    int pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return;
    }

    //in child
    if(pid == 0)
    {
        if(current_procs->inputfile)
        {
            if(set_stdin(current_procs->inputfile) ==-1)
            {
                exit(1);
            }
        }

        if(current_procs->outputfile)
        {
            if(set_stdout(current_procs->outputfile) ==-1)
            {
                exit(1);
            }
        }

        //execute process
        if(execute_process(current_procs)==-1)
        {
            exit(EXIT_FAILURE);
        }
    }

    int status;
    pid_t wpid = waitpid(pid, &status, 0);
    if (wpid == -1) 
    {
        perror("waitpid");
    } 
    else 
    {
        if (WIFEXITED(status)) 
        {
            int code = WEXITSTATUS(status);
            if (code != 0) {
                printf("Command Failed: code %d\n", code);
            }
        } 
        else 
        {
            printf("Child terminated abnormally\n");
        }
    }
}

enum MODE get_terminal_mode(int fd, int argc)
{
    if((!fd && argc ==1) || argc ==2) return BATCH;
    if(fd ==1 && argc ==1) return INTERACTIVE;
    return NONE;
}

int process_input(enum MODE mode, char **command_line, int *command_line_size)
{
    const char *prompt_str = "mysh> ";
    int prompt_flag = 1;

    while (prompt_flag) 
    {
        // Print prompt in interactive mode
        if (mode == INTERACTIVE) {
            if (write(STDOUT_FILENO, prompt_str, strlen(prompt_str)) == -1) {
                perror("write");
                return -1;
            }
        }

        //keeps track of the position where read() should write into the command_line array.
        int position = 0;
        while (1) {
            int r = read(STDIN_FILENO, *command_line + position, *command_line_size - position);
            if (r < 0) {
                perror("read");
                return -1;
            }

            if (r == 0) { // EOF
                prompt_flag = 0;
                break;
            }

            position += r;

            // Grow buffer if needed
            if (position >= *command_line_size) {
                *command_line_size *= 2;
                char *tmp = realloc(*command_line, *command_line_size);
                if (!tmp) {
                    perror("realloc");
                    return -1;
                }
                *command_line = tmp;
            }

            // Stop when newline is read
            if ((*command_line)[position - 1] == '\n')
                break;
        }

        // Empty line, prompt again if in interactive mode
        if (position == 0 || (*command_line)[0] == '\n')
            continue;

    
        Job myjob;
        if (init_job(&myjob) == -1 || parse_command_line(&myjob, *command_line, position) == -1) {
            destroy_job(&myjob);
            continue;
        }

        Process * current_procs = &myjob.processes[0];
        if (!current_procs) {
            destroy_job(&myjob);
            continue;
        }

        // Handle built-ins
        int builtin = is_built_in_command(current_procs->execpath);
        switch (builtin) {
            case CD:    mycd(current_procs); break;
            case PWD: 
            {
                char *cwd = mypwd();
                if (!cwd) exit(EXIT_FAILURE);
                printf("%s\n", cwd);
                free(cwd);
                break;
            }
            case WHICH: mywhich(current_procs); break;
            case EXIT:  myexit(current_procs, &prompt_flag); break;
            case NONE:  execute_job(&myjob, current_procs); break;
        }

        destroy_job(&myjob);
    }

    return 0;
}
