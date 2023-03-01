#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
  bool exit_bool = (system(cmd) < 0) ? false : true;
/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    return exit_bool;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    bool exit_bool;
    int status;

    pid_t pID = fork();
    if (pID == -1) {
      exit_bool = false;
    } else if (pID == 0){
      int execStatus = execv(command[0],command);
      exit (execStatus==-1);

      exit_bool = (waitpid(pID, &status, 0) <0) ? false : true;
    }

    va_end(args);

    return exit_bool;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    bool exit_bool;
    int status;


    /* 1st commit: For some reason, the O_TRUNC option in open() prevents the first test for this function from expanding $HOME.
    However, removing this option present the second test for this function from passing because it *DOES* expand $HOME, when it apparently shouldn't
    Further examination is needed.*/

    /* 2nd commit: while &command[1] as the second argument for do_exec() worked,
    it did NOT work as intended here. When changed from &command[1] to command, it
    worked exactly as intended.

    I must admit, I'm not sure why this is the case - I thought command[1:end] was what should have been the second argument of execv(), not the entire
    array. Changing this in do_exec() does not seem to affect the test results. This does mean, of course, that the O_TRUNC
    option only exposed the issue, and wasn't the underlying issue itself.*/

    /* 3rd commit: After looking at some printf() statements of what was passed into this function, I think I understand now: from the man pages,
    the argv argument of execv() is, by convention, the name of the file to be executed, which, in these examples, is the also the initial argument of execv()?
    I'm not entirely convinced this is correct, but I'll worry about it later.
    */

    pid_t pID;
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (fd < 0) {
      exit_bool = false;
      exit(-1);
    } else {
      switch (pID = fork()) {
        case -1: exit_bool = false;
        case 0:
          if (dup2(fd,1)<0) {
            exit_bool = false;
          } else {
            close(fd);
            int execStatus = execv(command[0],command);
            exit(execStatus == -1);
          }
        default:
            close(fd);
      }
    }

    exit_bool = (waitpid(pID, &status, 0) == -1) ? false : true;


    va_end(args);

    return exit_bool;
}
