#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<signal.h>
#include "parse.h"
//#include "execute.h"
#include "pipeHandler.h"
#include "input.h"
void input(char **line,int in, int out , int append_out, int pipe)
{
	//puts("i am here");
	pid_t pid;
	int status,inp,oup,end = 0,in_i,out_o;
    int k,j,sout,ofd,ifd,afd,numTokens;
    char *command[256],*tokens[256];
    if(pipe==1)
    {
  		//puts("hii man");
    	pipeHandler(line);
    }
    else
    {
    if(fork()==0)
	{
    if(out==1 && in==0 &&  pipe==0 && append_out==0)
    {
	int sout;
	//puts("i am in out only");
	j=0;k=0;
	//tokens[j+1]=filename
	while (strcmp(line[j],">") != 0)
	{
	    command[k] = line[j];
	    j++;
	    k++;
	}
	// Last position of the command will be NULL to indicate that
	// it is its end when we pass it to the exec function
	command[k] = NULL;
	j++;
	    ofd=open(line[j],O_WRONLY |  O_CREAT  | O_TRUNC,0600);
	    //sout=dup(STDOUT_FILENO);
	    dup2(ofd,STDOUT_FILENO);
	    close(ofd);
	    //dup2(sout,STDOUT_FILENO);
	    
	}
    else if(append_out==1 && in==0 && pipe==0)
    {
	//puts("i am in append only");
	//open(argv1[1],"a");
	//while (tokens[j] != NULL && end != 1){
	k = 0;
	j=0;
	//tokens[j+1]=filename
	while (strcmp(line[j],">>") != 0)
	{
	    command[k] = line[j];
	    j++;
	    k++;
	}
	// Last position of the command will be NULL to indicate that
	// it is its end when we pass it to the exec function
	command[k] = NULL;
	j++;
	//puts(line[j]);
	//puts("====\n");
	    afd=open(line[j],O_APPEND | O_WRONLY|O_CREAT);
	    dup2(afd,1);
	    close(afd);
	}
    else if(out==0 && in==1 && pipe==0 && append_out==0)
    {
	k = 0;
	j=0;
	//tokens[j+1]=filename
	while (strcmp(line[j],"<") != 0)
	{
	    command[k] = line[j];
	    j++;
	    k++;
	}
		command[k] = NULL;
	j++;
	//puts(line[j]);
	//puts(" i\n");
	//puts("hey ");
	    ifd = open(line[j],O_RDONLY|O_EXCL);
	    if (ifd==-1)
	    {
	    	puts("file does't exit");
	    	exit(0);
	    }
	    dup2(ifd,0);
	    close(ifd);
	  //  sleep(2);
	}
    else if(out==1 && in==1 && pipe==0)
    {
    //puts("iknow who am i");
	k = 0;
	j=0;
	//tokens[j+1]=filename
	while (strcmp(line[j],"<") != 0 )
	{
	    command[k] = line[j];
	    j++;
	    k++;
	}
	command[k] = NULL;
	while(line[j]!=NULL)
	{
	if(strcmp(line[j],"<") == 0)
	{
		inp=j;
	}
	else if (strcmp(line[j],">") == 0)
	{
		oup=j;
	}
	j++;

}
//puts("i am input file");
//puts(line[inp+1]);
//puts("i am output file");	
//puts(line[oup+1]);
        //printf("%d %d\n",inp,oup);
	    in_i=open(line[inp+1],O_RDONLY | O_EXCL);
	    out_o=open(line[oup+1],O_RDONLY | O_WRONLY | O_CREAT, S_IRWXU);
	    dup2(in_i,0);
	    dup2(out_o,STDOUT_FILENO);
	    close(in_i);
	    close(out_o);
	   // close(1);  
	}
	//puts("get lost");
	if (execvp(*command, command) < 0)  
	    { 
		printf(" ERROR: exec failed\n");
		exit(1);
	    }
	}
	else

	    while(waitpid(pid,&status,WUNTRACED)>0);       /* wait for completion  */
				;

}

}
    
