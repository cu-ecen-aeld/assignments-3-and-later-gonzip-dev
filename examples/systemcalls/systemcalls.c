#include "systemcalls.h"
#define _XOPEN_SOURCE

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

    int sysres = 0;
    bool retval = false;

    sysres = system(cmd);
    /*if (WIFSIGNALED(sysres) && 
        ( WTERMSIG(sysres) == SIGINT || WTERMSIG(sysres) == SIGQUIT))
    {
        retval = false;
    }*/
    
        

    if (sysres != -1 ) retval = true;
/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    return retval;
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
    //command[count] = command[count];

    pid_t kidpid;
    int kidstat;
    bool retval = false;
    fflush(stdout);
    switch (kidpid = fork())
    {
        case -1: printf("Fork error");
        case 0:
            execv(command[0],&command[0]);
            exit(-1);//This should never be reached
        default:
            waitpid(kidpid,&kidstat,0);
            if (WIFEXITED(kidstat)&&(WEXITSTATUS(kidstat)==0)) retval=true;
            //We only return true if the kid was exited succesfully and status was 0.

    }
/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    va_end(args);

    return retval;
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
    command[count]=NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];

    int kidpid;
    int fd = creat(outputfile,0644u);
    int kidstat;
    bool retval = false; //We assume failure and only actually return true if the conditions are met ()
                         // conditions for success, child exit succesfull and correct status.
    switch (kidpid = fork())
    {
        case -1: printf("fork_error");
        case 0:
            if (dup2(fd,1)<0) {printf("dup2_Error"); exit(-1);}
            execv(command[0],&command[0]);
            exit(-1);//Never reaching here if execv did not fail
        default:
            waitpid(kidpid,&kidstat,0);
            close(fd);
            if (WIFEXITED(kidstat)&&(WEXITSTATUS(kidstat)==0)) retval=true;
            //We only return true if the kid was exited succesfully and status was 0.
            
    }

    
/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    va_end(args);

    return retval;
}
