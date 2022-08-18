#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"

int main(int argc, char *argv[]){
    int x=atoi(argv[1]);
     numOpenFiles(x);
      memAlloc(x);
     getprocesstimedetails(x);
   exit(0);
}