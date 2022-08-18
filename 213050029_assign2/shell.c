#include "types.h"
#include "user.h"
#include "fcntl.h"

// structure for simple are io redirection commands

struct simplecomm{
  char* class;
  char *input, *output;
  char **argv;
  
} simplecomm;

// structure for parallel,pipe and file commands
struct multiplecomm{
  char* class;
  struct simplecomm *firstcomm, *seccomm;
} multiplecomm;

int getcomm(char *comm, int n)
{
    // reads command from shell
  printf(1, "\nMyShell>");
  gets(comm, n);
  if(comm[0] == 0) 
    return -1;
  return 0;
}

char* extractsimple(char *comm,struct simplecomm *tempnormal)
{
 
  
  // tokenizes simple and redirection commands
  int next = 0,k;
  char *tok; 
  k = 0;

tok = malloc(sizeof(char) * 1000);
  char * res="";
  strcpy(res,comm);
  
   char *start=res;
    while(*start==' '){
              *start='a';
             start++;
    }
      tempnormal->input[0]='\0';
         tempnormal->output[0]='\0';
   char *nl,*ir,*or,*gap1;
   nl=strchr(res,'\n');
   ir=strchr(res,'<');
   or=strchr(res,'>');
   if(ir==0 && or==0){
        tempnormal->class = "simple";
        while(start!=nl){

        gap1=strchr(res,' ');
    
        if(gap1==0){
            while(start!=nl){
                tok[k++]=*start;
                start++;

            }
            strcpy(tempnormal->argv[next],tok);
            memset(tok,'\0',1000);
            k=0;
            next++;
            tempnormal->argv[next]=0;
            free(tok);
            return "simple";
        }

        while(start!=gap1){
            tok[k++]=*start;
            start++;
        }
        strcpy(tempnormal->argv[next],tok);
        memset(tok,'\0',1000);
            k=0;
        next++;
         
        if(*start==' '){
            
            while(*start==' '){
              
              *start='a';
              start++;
            }
        }
            if(start==nl){
                tempnormal->argv[next]=0;
            }
         memset(tok,'\0',1000);
         k=0;
        }

   }
   else{
           tempnormal->class="redirect";
           
           char* first;
           char* second;
           if(ir!=0 && or!=0){

             if(ir<or){
                 first=ir;
                 second=or;
             }
             else{
                   first=or;
                   second=ir;
             }

             while(start!=first){
               gap1=strchr(res,' ');
               if(gap1>first || gap1==0){
                   while(start!=first){
                       tok[k++]=*start;
                       start++;
                   }
                   strcpy(tempnormal->argv[next],tok);
                    next++;
                    tempnormal->argv[next]=0;
                    next=0;
                    k=0;
                    break;

               }
                 while(start!=gap1){
                      tok[k++]=*start;
                           start++;
                  }
                strcpy(tempnormal->argv[next],tok);
                 next++;
                 if(*start==' '){
            
                while(*start==' '){
              
                     *start='a';
                       start++;
                   }
                }
                if(start==first){
                    tempnormal->argv[next]=0;
                    next=0;
                }
                memset(tok,'\0',1000);
                k=0;

             }
               start++;

            if(*start==' '){
               
               while(*start==' '){
                   *start='a';
                   start++;
               }
            }

            while(*start!=' ' && start!=second){

                tok[k++]=*start;
                start++;
            }
            if(*first=='<')
                strcpy(tempnormal->input,tok);
            else{
                strcpy(tempnormal->output,tok);
            }
            memset(tok,'\0',1000);
            k=0;
            if(*start==' '){
                while(*start==' '){
                    *start='a';
                    start++;
                }
            }

            start++;
            while(*start==' '){
                *start='a';
                start++;
            }

             while(*start!=' ' && start!=nl){

                tok[k++]=*start;
                start++;
            }
             if(*second=='<'){
                strcpy(tempnormal->input,tok);
             }
            else{
                strcpy(tempnormal->output,tok);
            }
            memset(tok,'\0',1000);

           }
           else{

               
                 
                 if(ir==0){
                     
                     first=or;
                     
                 }
                 else{
                     first=ir;
                 }

             while(start!=first){
               gap1=strchr(res,' ');
                
               if(gap1>first || gap1==0){
                 //  printf(1,"\n%s\n",tempnormal->argv[next]);
                
                   while(start!=first){
                       tok[k++]=*start;
                       start++;
                   }
                   strcpy(tempnormal->argv[next],tok);
                    memset(tok,'\0',1000);
                       k=0;
                    next++;
                    tempnormal->argv[next]=0;
                    next=0;
                    k=0;
                    break;

               }
                 while(start!=gap1){
                      tok[k++]=*start;
                           start++;
                  }
                strcpy(tempnormal->argv[next],tok);
               
                memset(tok,'\0',1000);
                k=0;
                 next++;
                 if(*start==' '){
            
                while(*start==' '){
              
                     *start='a';
                       start++;
                   }
                }
                if(start==first){
                    tempnormal->argv[next]=0;
                    next=0;
                }
                

             }
             start++;
            
            if(*start==' '){
               
               while(*start==' '){
                   *start='a';
                   start++;
               }
            }
             
            while(*start!=' ' && start!=nl){
                
                tok[k++]=*start;
                start++;
            }
             
            if(*first=='<'){
               
                strcpy(tempnormal->input,tok);
                
            }
            else{
                 
                strcpy(tempnormal->output,tok);
                
            }
            memset(tok,'\0',1000);
            k=0;
           }
      free(tok);
      return "redirect";
     }
     return "simple";
}

char* getcommclass(char * comm)
{
    char *str;
    str = malloc(sizeof(char) * 1000);
    strcpy(str,comm);
    str[16] = '\0';

    // returns the class of command , which action to be performed
    if(strcmp(str,"executeCommands ") == 0){
        return "fromfile";
    }
    char *t;
    if((t=strchr(comm,'|'))!=0){
       if(*(t+1)=='|'){
           return "or";
       }
       else{
           return "pipetype";
       }
    }
    else if(strchr(comm,';')!=0){
        return "simul";
    }
    else if((t=strchr(comm,'&'))!=0){
       if(*(t+1)=='&'){
           return "and";
       }
    }

    return "simple";
}
int checkcomm(char **argv)
{
     
   // checking whether command entered is legal or illeagal
   //returns 0 if command is legal else -1

    if(strcmp(argv[0],"exit")==0 ){
        if(argv[1]==0){
        return 0;
        }
    }
    
    else if(strcmp(argv[0],"procinfo")==0)
    {
      
      if (argv[1] != 0 && argv[2] ==0)
      {
            for (int i=0; i< strlen(argv[1]); i++)
            { 
            if(argv[1][i] > '9' || argv[1][i] < '0') // pid has to be an integer
                {
                    return -1;            
                }
             }
            if(atoi(argv[1]) < 0)
            {
                return -1;
            }
            else
            {
                return 0;
            }
      }
      else
        return -1;
    }
    else if(strcmp(argv[0],"wc")==0 || strcmp(argv[0],"ls")==0 ||  strcmp(argv[0],"cat")==0 || strcmp(argv[0],"echo")==0 || strcmp(argv[0],"grep")==0)
    {
      return 0;
    }
    else if(strcmp(argv[0],"ps")==0)
    {
      if (argv[1] == 0)
        {
            return 0;
        }
      else
        return -1;
    }

    
    else
    {
      return -1;
    }
    return -1;
}
int executesimplecomm(struct simplecomm *tempnormal, int *status){

         if (checkcomm(tempnormal->argv)== -1)
            {
                 *status = -1;
              printf(1,"\nIllegal command or arguments\n");
              return -1;
            }
            else{
       if(strcmp(tempnormal->argv[0],"exit")==0){

           exit(0);
       }

      if(fork()==0)
                {
                    if(strcmp(tempnormal->argv[0], "ps")==0)
                    {
                        psinfo();
                    }
                   
                    else
                    {
                        /*
                        if(strcmp(tempnormal->argv[0],"procinfo")==0){
                            tempnormal->argv[2]=0;
                        }
                        */
                       // printf(1,"i am here");
                        exec(tempnormal->argv[0],tempnormal->argv);
                       
                        
                    }
                    exit(0);
                }
                wait(status);
                return 0;
            }
}
int executesimple_redirect_comm(struct simplecomm *tempnormal, int *status)
{
           
          if (checkcomm(tempnormal->argv)== -1)
            {
                 *status = -1;
              printf(1,"\nIllegal command or arguments\n");
              return -1;
            }
          else
          {
            
             
                if(fork()==0)
                {
                    if(tempnormal->output[0] != '\0') // some outputfile passed
                    {
                    
                      int fd;   
                    if ((fd =open(tempnormal->output, O_CREATE|O_RDWR)) >= 0)
                    {

                        //O_TRUNC not support by this version of xv6 i guess, not able to do by it
                        
                        close(fd);
                        char *truncatecomm[10]; 
                        truncatecomm[0] = "rm";
                        truncatecomm[1] = tempnormal->output;
                        truncatecomm[2] = 0; // can be truncated
                        if(fork()==0)
                        {

                            // truncating the output file if it exists
                            exec("rm",truncatecomm);          
                            exit(0);
                        }
                        

                        wait(0);
                        close(1);
                       open(tempnormal->output, O_CREATE|O_RDWR);
                      
                    }
                    else
                    {
                        printf(2,"\ncannot open output file!!\n");
                        exit(-1);
                    }
                    }
                    
                    if(tempnormal->input[0] != '\0') // some inputfile passed
                    {
                      
                      close(0);
                      int fd;
                      if ((fd = open(tempnormal->input, O_RDONLY)) < 0)
                      {
                          printf(2,"\ncannot open input file!!\n");
                          exit(-1);
                      }
                    }
                    
                    //printf(1,"i am here");
                    exec(tempnormal->argv[0],tempnormal->argv);
                    exit(0);
                }
                wait(status);
              
             
          }
          return 0;
}
int launchsimplecomm(struct simplecomm *comm , int *status){

   if (checkcomm(comm->argv)==-1)
      {
          
        printf(1,"Illegal command or arguments");
        *status = -1;
        return -1;
      }
      else{

  if(strcmp(comm->argv[0], "ps")==0)
            {
                psinfo();
            }
            else
            {

               
                *status = exec(comm->argv[0],comm->argv);
            }
            exit(0);
       
      }
     return 0;


}
int launchsimple_redirect_comm(struct simplecomm *comm , int *status)
{
    if (checkcomm(comm->argv)==-1)
      {
          
        printf(1,"Illegal command or arguments");
        *status = -1;
        return -1;
      }
    else
    { 

        
                if(fork()==0)
                {
                    if(comm->input[0] != '\0')
                    {
                      
                      close(0);
                      int fd;
                      if ((fd = open(comm->input, O_RDONLY)) < 0)
                      {
                          printf(2,"\ncannot open input file!!\n");
                          exit(-1);
                          return -1;
                      }
                    }
                    if(comm->output[0] != '\0')
                    {
                      
                      int fd;   
                    if ((fd =open(comm->output, O_CREATE|O_RDWR)) >= 0)
                    {
                        close(fd);
                        char *truncatecomm[10]; 
                        truncatecomm[0] = "rm";
                        truncatecomm[1] = comm->output;
                        truncatecomm[2] = 0;
                       
                        if(fork()==0)
                        {

                            // truncating the output file if it exists
                            exec("rm",truncatecomm);          
                            exit(0);
                            return 0;
                        }
                        // wait for child
                        wait(0);
                     
                        close(1);
                        // new output file created
                        open(comm->output, O_CREATE|O_RDWR);
                        
                    }
                    else
                    {
                        printf(2,"\ncannot open output file!!\n");
                        exit(-1);
                        return -1;
                    }
                    }
                   // printf(1,"i am here");
                    exec(comm->argv[0],comm->argv);
                    exit(0);
                    return 0;
                }
                wait(status);
    
    }
    return 0;
}

char* tokenizecomm(char *comm, char **comms,char* class)
{
    int i=0;
    
     char *w;
    char *t;
    char *q;
    if((t=strchr(comm,'|'))!=0){
       if(*(t+1)=='|'){
           class="or";
           i=t-comm;
       }
       else{
           class="pipetype";
            i=t-comm;
       }
    }
    
    else if((q=strchr(comm,';'))!=0){
        class= "simul";
        i=q-comm;
    }
    
    else if((w=strchr(comm,'&'))!=0){
       if(*(w+1)=='&'){
           class= "and";
             i=w-comm;
       }
    }
    else{
        class="simple";
    }

    if(strcmp(class,"pipetype")==0 || strcmp(class,"simul")==0)
    {
          strcpy(comms[0],comm);
        comms[0][i+1] = '\0';
        comms[0][i] = '\n';
        strcpy(comms[1],comm+i+1);
        int i=0;
        if(comms[1][i]==' ')
        {
            while(comms[1][++i]==' ');
        }
        comms[1] = comms[1] + i;

    }
    else
    {
          

        strcpy(comms[0],comm);
        comms[0][i] = '\n';
        comms[0][i+1] = '\0';
        strcpy(comms[1],comm+i+2);
        int i=0;
        if(comms[1][i]==' ')
        {
            while(comms[1][++i]==' ');
        }
        comms[1] = comms[1] + i;

    }

   
    return class;
}



int runconditionalcomm(char *comm,struct multiplecomm *conditionalcomm, int *status)
{
    char *comms[2];
    comms[0] = malloc(sizeof(char) * 1000);
    comms[1] = malloc(sizeof(char) * 1000);

    conditionalcomm->class = tokenizecomm(comm,comms,conditionalcomm->class);
    char *firstcomm = malloc(sizeof(char) * 1000);
    char *seccomm = malloc(sizeof(char) * 1000);
    strcpy(firstcomm,comms[0]);
    strcpy(seccomm,comms[1]);
     char * class;
     
           // firstcomm and secondcomm contain commands from both end
      // of the pipe
    
    int first;
    first = fork();
    if (first == 0)
    {
        class=extractsimple(firstcomm,conditionalcomm->firstcomm);
        if(strcmp(class,"simple")==0){
        launchsimplecomm(conditionalcomm->firstcomm,status);
        }
        else if(strcmp(class,"redirect")==0){
        
        launchsimple_redirect_comm(conditionalcomm->firstcomm,status);
        
        }
    }
    wait(status);

    if((*status == -1 && strcmp(conditionalcomm->class,"or")==0) || (*status == 0 && strcmp(conditionalcomm->class,"and")==0))
    {
        int sec = fork();
        if (sec == 0)
        {
            class=extractsimple(seccomm,conditionalcomm->seccomm);
        if(strcmp(class,"simple")==0){
        *status=launchsimplecomm(conditionalcomm->seccomm,status);
        }
        else if(strcmp(class,"redirect")==0){
        
         *status=launchsimple_redirect_comm(conditionalcomm->seccomm,status);
        
        }
        }
        wait(status);
        return 0;
    }
    return 0;
}
int runpipecomm(char *comm,struct multiplecomm *pipecomm, int *status)
{
    char *comms[2];
    comms[0] = malloc(sizeof(char) * 1000);
    comms[1] = malloc(sizeof(char) * 1000);

    pipecomm->class = tokenizecomm(comm,comms,pipecomm->class);
    char *firstcomm = malloc(sizeof(char) * 1000);
    
    char *seccomm = malloc(sizeof(char) * 1000);
    strcpy(firstcomm,comms[0]);
    strcpy(seccomm,comms[1]);
    
      // firstcomm and secondcomm contain commands from both end
      // of the pipe

     
    int pds[2];
    char* class;
     pipe(pds);

    if(fork() == 0)
    {
        class=extractsimple(firstcomm,pipecomm->firstcomm);
        close(1);
        dup(pds[1]);
        close(pds[1]);
        close(pds[0]);
        
          if(strcmp(class,"simple")==0){ 
        launchsimplecomm(pipecomm->firstcomm,status);
          }
          else if(strcmp(class,"redirect")==0){ 
        launchsimple_redirect_comm(pipecomm->firstcomm,status);
          }
        exit(0);
    }
    if(fork() == 0)
    {
        class=extractsimple(seccomm,pipecomm->seccomm);
        close(0);
        dup(pds[0]);
         close(pds[1]);
        close(pds[0]);
       
         if(strcmp(class,"simple")==0){ 
        *status = launchsimplecomm(pipecomm->seccomm,status);
         }
         else if(strcmp(class,"redirect")==0){
             *status = launchsimple_redirect_comm(pipecomm->seccomm,status);
         }
        exit(0);
    }
    close(pds[0]);
    close(pds[1]);
    wait(0);
    wait(0);

    return 1;
}

int runparallelcomm(char *comm,struct multiplecomm *parallel, int *status)
{
    char *comms[2];
    comms[0] = malloc(sizeof(char) * 1000);
    comms[1] = malloc(sizeof(char) * 1000);

    parallel->class = tokenizecomm(comm,comms,parallel->class);
    char *firstcomm = malloc(sizeof(char) * 1000);
    char *seccomm = malloc(sizeof(char) * 1000);
    strcpy(firstcomm,comms[0]);
    strcpy(seccomm,comms[1]);
    
    int first,sec;
    char* class;

    sec = fork();
    if (sec == 0)
    {
            class=extractsimple(seccomm,parallel->seccomm);
        if(strcmp(class,"simple")==0){
        *status=launchsimplecomm(parallel->seccomm,status);
        }
        else if(strcmp(class,"redirect")==0){
        
         *status=launchsimple_redirect_comm(parallel->seccomm,status);
        
        }
    }
    first = fork();
    if (first == 0)
    {
         class=extractsimple(firstcomm,parallel->firstcomm);
        if(strcmp(class,"simple")==0){
        launchsimplecomm(parallel->firstcomm,status);
        }
        else if(strcmp(class,"redirect")==0){
        
        *status=launchsimple_redirect_comm(parallel->firstcomm,status);
        
        }
    }
    int ret1,ret2;
    ret1=wait(status);
    ret2=wait(status);
    
    if((ret1==sec || ret1==first) && (ret2==first || ret2==sec))
    {

        // checking if both the childs have reaped and are back
        return 1;
    }
    else
    {
        return 0;
    }
}

void runfilecomms(char *comm)
{
    char **comms = malloc(sizeof(char *) * 100);
    char *tok = malloc(sizeof(char) * 1000);
    int t=15;
   //  15 is the length of executecommands
    while(comm[++t]==' ');
    char  *file = comm+t;
      t=0;
       file[strlen(file)-1]='\0';
    int fd = open(file,O_RDWR);
    if(fd < 0)
    {

       //cannot open file to execute commands in the file
        return;
    }

   
    int n,temp=0, next=0;
     int *status = malloc(sizeof(int));
    char arr[2200];
    n= read(fd, arr ,sizeof(arr));
   
    close(fd);
 
    
    
    for(t=0;t<n;t++)
    {
        if(arr[t] == '\n')
        {
            tok[temp++] = '\n';
            comms[next] = malloc(sizeof(char) * 1000);
            tok[temp] = '\0';
            
            strcpy(comms[next],tok);  //taking lines of file as elements of comms
            next++;
            

            for(int u=0;u<1000;u++){
                tok[u]='\0';
            }
            temp=0;

        }
        else
        {
           tok[temp++] = arr[t];
        }
       
        
    }
    
   

    for(int s=0;s<next;s++)
    {
       
        memset(comm, '\0', sizeof(comm));
        comm = comms[s];
        if(comm[0]=='\n')
        {
        
            continue;
        }


        char* class = getcommclass(comm);

         // class contains the kind of command passed by user


        if(strcmp(class,"simul")==0)
        {
            
            struct multiplecomm *parallel;
            parallel = malloc(sizeof(struct multiplecomm));
            parallel->firstcomm = malloc(sizeof(struct simplecomm));
            parallel->seccomm = malloc(sizeof(struct simplecomm));
            parallel->firstcomm->argv = malloc(sizeof(char*) * 10);
            parallel->firstcomm->input = malloc(sizeof(char) * 1000);
            parallel->firstcomm->output = malloc(sizeof(char) * 1000);
            parallel->seccomm->argv = malloc(sizeof(char*) * 10);
            parallel->seccomm->input = malloc(sizeof(char) * 1000);
            parallel->seccomm->output = malloc(sizeof(char) * 1000);

            for(t=0;t<10;t++)
            {
              parallel->firstcomm->argv[t] = malloc(sizeof(char) * 1000);
              parallel->seccomm->argv[t] = malloc(sizeof(char) * 1000);
            }
            runparallelcomm(comm,parallel,status);

        }
         else if (strcmp(class,"simple")==0)
        {
            struct simplecomm *tempnormal;
            char* cls;
            tempnormal = malloc(sizeof(struct simplecomm));
            tempnormal->argv = malloc(sizeof(char*) * 10);
            tempnormal->input = malloc(sizeof(char) * 1000);
            tempnormal->output = malloc(sizeof(char) * 1000);
            for( t=0;t<10;t++)
            {
              tempnormal->argv[t] = malloc(sizeof(char) * 1000);
            }
           
            cls=extractsimple(comm,tempnormal);
            if(strcmp(cls,"simple")==0){
            executesimplecomm(tempnormal,status);
            }
            else if(strcmp(cls,"redirect")==0){

                       executesimple_redirect_comm(tempnormal,status);

            }
        }
        else if(strcmp(class,"and")==0 || strcmp(class,"or")==0)
        {
           
             
            struct multiplecomm *conditionalcomm;
            conditionalcomm = malloc(sizeof(struct multiplecomm));
            conditionalcomm->firstcomm = malloc(sizeof(struct simplecomm));
            conditionalcomm->seccomm = malloc(sizeof(struct simplecomm));
            conditionalcomm->firstcomm->argv = malloc(sizeof(char*) * 10);
            conditionalcomm->firstcomm->input = malloc(sizeof(char) * 1000);
            conditionalcomm->firstcomm->output = malloc(sizeof(char) * 1000);
            conditionalcomm->seccomm->argv = malloc(sizeof(char*) * 10);
            conditionalcomm->seccomm->input = malloc(sizeof(char) * 1000);
            conditionalcomm->seccomm->output = malloc(sizeof(char) * 1000);

            for(t=0;t<10;t++)
            {
              conditionalcomm->firstcomm->argv[t] = malloc(sizeof(char) * 1000);
              conditionalcomm->seccomm->argv[t] = malloc(sizeof(char) * 1000);
            }

            runconditionalcomm(comm,conditionalcomm,status);
        }
        else if(strcmp(class,"pipetype")==0)
        {
            
            struct multiplecomm *pipecomm;
            pipecomm = malloc(sizeof(struct multiplecomm));
            pipecomm->firstcomm = malloc(sizeof(struct simplecomm));
            pipecomm->seccomm = malloc(sizeof(struct simplecomm));
            pipecomm->firstcomm->argv = malloc(sizeof(char*) * 10);
            pipecomm->firstcomm->input = malloc(sizeof(char) * 1000);
            pipecomm->firstcomm->output = malloc(sizeof(char) * 1000);
            pipecomm->seccomm->argv = malloc(sizeof(char*) * 10);
            pipecomm->seccomm->input = malloc(sizeof(char) * 1000);
            pipecomm->seccomm->output = malloc(sizeof(char) * 1000);

            for(t=0;t<10;t++)
            {
              pipecomm->firstcomm->argv[t] = malloc(sizeof(char) * 1000);
              pipecomm->seccomm->argv[t] = malloc(sizeof(char) * 1000);
            }
            runpipecomm(comm,pipecomm,status);

        }
        else if(strcmp(class,"fromfile")==0)
        {
            
            runfilecomms(comm);
        }


      
       

    }
    return;
}

int
main(void)
{
   int n = 1000, *status,val=0;
  char *comm ;                            
  status = malloc(sizeof(int));        
  comm = malloc(sizeof(char)*n);
  while((val=getcomm(comm,n)) >= -1)
  {
      if(val==-1){
          val=0;
         continue;
      }
      if(comm[0]=='\n'){

          continue;
      }
  

        char* class = getcommclass(comm);
        if(strcmp(class,"pipetype")==0)
        {
          
            struct multiplecomm *pipecomm;
            pipecomm = malloc(sizeof(struct multiplecomm));
            pipecomm->firstcomm = malloc(sizeof(struct simplecomm));
            pipecomm->seccomm = malloc(sizeof(struct simplecomm));
            pipecomm->firstcomm->output = malloc(sizeof(char) * 1000);
            pipecomm->seccomm->argv = malloc(sizeof(char*) * 10);
            pipecomm->seccomm->input = malloc(sizeof(char) * 1000);
            pipecomm->seccomm->output = malloc(sizeof(char) * 1000);
            pipecomm->firstcomm->argv = malloc(sizeof(char*) * 10);
            pipecomm->firstcomm->input = malloc(sizeof(char) * 1000);
           

            for(int i=0;i<10;i++)
            {
              pipecomm->firstcomm->argv[i] = malloc(sizeof(char) * 1000);
              pipecomm->seccomm->argv[i] = malloc(sizeof(char) * 1000);
            }
            runpipecomm(comm,pipecomm,status);

        }
        else if (strcmp(class,"simple")==0)
        {
            struct simplecomm *tempnormal;
            char* cls;
            tempnormal = malloc(sizeof(struct simplecomm));
            tempnormal->argv = malloc(sizeof(char*) * 10);
            tempnormal->input = malloc(sizeof(char) * 1000);
            tempnormal->output = malloc(sizeof(char) * 1000);
            for(int i=0;i<10;i++)
            {
              tempnormal->argv[i] = malloc(sizeof(char) * 1000);
            }
           
            cls=extractsimple(comm,tempnormal);
            if(strcmp(cls,"simple")==0){
            executesimplecomm(tempnormal,status);
            }
            else if(strcmp(cls,"redirect")==0){

                       executesimple_redirect_comm(tempnormal,status);

            }
        }
        
        else if(strcmp(class,"simul")==0)
        {
            
            struct multiplecomm *parallel;
            parallel = malloc(sizeof(struct multiplecomm));
            parallel->firstcomm = malloc(sizeof(struct simplecomm));
            parallel->seccomm = malloc(sizeof(struct simplecomm));
            parallel->firstcomm->output = malloc(sizeof(char) * 1000);
            parallel->seccomm->argv = malloc(sizeof(char*) * 10);
            parallel->seccomm->input = malloc(sizeof(char) * 1000);
            parallel->seccomm->output = malloc(sizeof(char) * 1000);
            parallel->firstcomm->argv = malloc(sizeof(char*) * 10);
            parallel->firstcomm->input = malloc(sizeof(char) * 1000);
           

            for(int i=0;i<10;i++)
            {
              parallel->firstcomm->argv[i] = malloc(sizeof(char) * 1000);
              parallel->seccomm->argv[i] = malloc(sizeof(char) * 1000);
            }
            runparallelcomm(comm,parallel,status);

        }
        else if(strcmp(class,"and")==0 || strcmp(class,"or")==0)
        {
            

           struct multiplecomm *conditionalcomm;
            conditionalcomm = malloc(sizeof(struct multiplecomm));
            conditionalcomm->firstcomm = malloc(sizeof(struct simplecomm));
            conditionalcomm->seccomm = malloc(sizeof(struct simplecomm));
            conditionalcomm->firstcomm->output = malloc(sizeof(char) * 1000);
            conditionalcomm->seccomm->argv = malloc(sizeof(char*) * 10);
            conditionalcomm->seccomm->input = malloc(sizeof(char) * 1000);
            conditionalcomm->seccomm->output = malloc(sizeof(char) * 1000);
            conditionalcomm->firstcomm->argv = malloc(sizeof(char*) * 10);
            conditionalcomm->firstcomm->input = malloc(sizeof(char) * 1000);
            
             int max=10;
            for(int i=0;i<max;i++)
            {
              conditionalcomm->firstcomm->argv[i] = malloc(sizeof(char) * 1000);
              conditionalcomm->seccomm->argv[i] = malloc(sizeof(char) * 1000);
            }

            runconditionalcomm(comm,conditionalcomm,status);

        }
        else if(strcmp(class,"fromfile")==0)
        {
            // command type is file(executecommands)
            runfilecomms(comm);
        }
        

    // free(comm);
    //comm = malloc(sizeof(char)*n);
    memset(comm,'\0',sizeof(char)*n);

  }


  exit(0);
}
