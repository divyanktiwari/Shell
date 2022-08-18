#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
int main(void){
   printf(1,"  \n",numOpenFiles(3));
   int fd;
   fd=open("backup",O_CREATE | O_RDWR);
   printf(1,"   \n",numOpenFiles(3));
   close(fd);
   printf(1,"   \n",numOpenFiles(3));
    exit(0);
}

