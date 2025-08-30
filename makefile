# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -g -std=c99 -Wall -Wvla -fsanitize=address,undefined

# Object files
OBJS = myexec.o myarraylist.o mybuiltincommands.o myjob.o myparser.o myprocess.o myutility.o

mysh: $(OBJS)
	$(CC) $(CFLAGS) mysh.c $(OBJS) -o mysh

myexec.o: myexec.c myexec.h myparser.o myutility.o mybuiltincommands.o
	$(CC) $(CFLAGS) -c myexec.c -o myexec.o

mybuiltincommands.o: builtincommands.c builtincommands.h myprocess.o
	$(CC) $(CFLAGS) -c builtincommands.c -o mybuiltincommands.o

myparser.o: myparser.c myparser.h myjob.o myutility.o
	$(CC) $(CFLAGS) -c myparser.c -o myparser.o

myutility.o: myutility.c myutility.h myarraylist.o
	$(CC) $(CFLAGS) -c myutility.c -o myutility.o

myjob.o: myjob.c myjob.h myprocess.o
	$(CC) $(CFLAGS) -c myjob.c -o myjob.o

myprocess.o: myprocess.c myprocess.h myarraylist.o
	$(CC) $(CFLAGS) -c myprocess.c -o myprocess.o

myarraylist.o: myarraylist.c myarraylist.h
	$(CC) $(CFLAGS) -c myarraylist.c -o myarraylist.o

clean:
	rm -f *.o