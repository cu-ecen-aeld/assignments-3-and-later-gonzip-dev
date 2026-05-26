#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <syslog.h>

#define ARGS_NUM 2U


const char *file;


int main(int argc, char *argv[]){
    //Start logger
    syslog(LOG_INFO,"Started log");
    openlog(NULL,0,LOG_USER);

    int fd;
    if (argc != ARGS_NUM+1)
    {
        syslog(LOG_ERR,"Invalid argument number: %d, it should be 2,",argc);
        return 1;
    }
    

    fd = creat(argv[1],0644u);
    if (fd ==-1)
    {
        syslog(LOG_ERR,"Error when creating file: %d",fd);
        return 1;
    }
    //Write in file
    int wr;
    wr = write(fd,argv[2],strlen(argv[2]));

    if (wr == -1)
    {
        syslog(LOG_ERR,"Error when writing into file");
        return 1;
    }
    else 
    {
        syslog(LOG_DEBUG,"Writing %s to file %s",argv[2],argv[1]);
    }
   
    

  

}