# My Shell (mysh)

## Authors
- Felipe Navas 


## Project Overview
`mysh` is a simple command-line shell implemented in C.  
It supports **interactive** and **batch** modes, command execution, built-in commands, file redirection, multiple pipelines, and wildcard expansion.  

The shell demonstrates system-level programming concepts such as:
- POSIX unbuffered I/O (`read()`)
- Process management (`fork()`, `waitpid()`)
- File descriptor manipulation (`dup2()`)
- Directory access (`opendir()`, `readdir()`)
- Signal and exit status handling  

## Compilation
A Makefile is included. To build the project, run:
```bash
make
```
This produces the executable:
```
./mysh
```

## Usage
Interactive mode:
```bash
./mysh
Welcome to my shell!
mysh> echo hello
hello
mysh> exit
mysh: exiting
```

Batch mode (from file):
```bash
./mysh script.sh
```

Batch mode (from stdin):
```bash
cat script.sh | ./mysh
```

## Features
- **Interactive mode** with prompts, welcome/goodbye messages, and abnormal exit reporting.  
- **Batch mode** (silent execution).  
- **Built-in commands**:  
  - `cd <dir>` — change working directory  
  - `pwd` — print current directory  
  - `which <cmd>` — show resolved path for command  
  - `exit` — terminate shell (prints any arguments)  
- **External command execution** with search in `/usr/local/bin`, `/usr/bin`, and `/bin`.  
- **Redirection** (`<`, `>`) for stdin/stdout.  
- **Pipelines** (`|`) for connecting multiple processes in sequence.  
- **Wildcards** (`*`) for file name expansion.  

## Limitations
- Wildcards are limited to **one `*` per token**.  
- Built-in commands cannot be piped.  
- While mysh supports string literals passed directly through the terminal, embedded quotes inside strings are not handled correctly.

## Known Bugs
- When reading commands from a file, if commands are **not separated by a `\n` character**, the shell will treat the entire file contents as a **single command**.  


## Test Plan
Tested `mysh` against a variety of scenarios to ensure correctness:

1. **Basic Commands**
   - `echo hello` → prints `hello`  
   - `ls` → lists files  

2. **Built-in Commands**
   - `cd subdir` then `pwd` → path updated correctly  
   - `which ls` → prints `/bin/ls`  
   - `exit goodbye shell` → exits after printing message  

3. **Redirection**
   - `echo hello > out.txt` then `cat out.txt` → `hello`  
   - `wc < out.txt` → counts contents  

4. **Pipelines**
   - `ls | wc -l` → prints file count  
   - `cat file.txt | grep pattern | sort | uniq` → works with multiple pipes  

5. **Wildcards**
   - `cat *.c` → expands to all `.c` files  
   - `echo foo*bar` → unchanged if no match  

6. **Error Handling**
   - `cd nonexisting` → prints error  
   - `ls nosuchfile` → prints error 
   - Invalid redirection file → error reported  

All features were validated in both **interactive** and **batch** modes.  
