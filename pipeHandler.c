#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<signal.h>
#include "pipeHandler.h"
void check(char **line)
{
	//puts("i am in check");
int in=0,out=0,append_out=0,pip=0;
int flag=0;
//puts("i am check");
while(line[flag]!=NULL)
		{
			//puts(line[flag]);
			//printf("%d\n",append_out);
			if(strcmp(line[flag],">")==0)
			{
				out=1;
	//			dub1[0]=line[flag+1];
	//			dub1[1]='\0';
			//	puts("fuck");
			//	puts(line[flag+1]);
			}
			else if(strcmp(line[flag],">>")==0)
			{
				append_out=1;
			}
			else if(strcmp(line[flag],"<")==0)
			{
				in=1;
			}
			else if(strcmp(line[flag],"|")==0)
			{
				pip=1;
				//count_pipe++;
			}
			flag++;
		}
		//printf("%d %d %d \n",out,in,append_out);
	if(in==1 || out ==1 || pip==1 || append_out==1  )
			input(line, in , out , append_out , pip);	
}
void pipeHandler(char * args[]){
	//if(in==1 || out ==1 || pipe==1 || append_out==1  )
	//		input(args , in , out , append_out , pipe);
    int filedes[2]; // pos. 0 output, pos. 1 input of the pipe
    int filedes2[2];
    int num_cmds = 0;
    char *command[256];
    pid_t pid;
    int err = -1;
    int end = 0;
    // Variables used for the different loops
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    // First we calculate the number of commands (they are separated
    // by '|')
    while (args[l] != NULL)
    {
	if (strcmp(args[l],"|") == 0)
	{
	    num_cmds++;
	}
	l++;
    }
    num_cmds++;
    //puts("hii kanna");
    // Main loop of this method. For each command between '|', the
    // pipes will be configured and standard input and/or output will
    // be replaced. Then it will be executed
    j=0;
    //if(args[j]==NULL)
    //	puts("yeah i am");
    while (args[j] != NULL && end != 1){
	k = 0;
	// We use an auxiliary array of pointers to store the command
	// that will be executed on each iteration
	//puts("i m sreeja");
	while (strcmp(args[j],"|") != 0){
	//	puts("i am in cmd\n");
	//	puts(args[j]);
	    command[k] = args[j];
	    j++;
	    if (args[j] == NULL){
		// 'end' variable used to keep the program from entering
		// again in the loop when no more arguments are found
		end = 1;
		k++;
		break;
	    }
	    //check(command);
	    k++;
	}
	
	// Last position of the command will be NULL to indicate that
	// it is its end when we pass it to the exec function
	command[k] = NULL;
	check(command);
	j++;

	if (i % 2 != 0)
	{
	    pipe(filedes); // for odd i
	}
	else
	{
	    pipe(filedes2); // for even i
	}
	pid=fork();
	if(pid==-1){
	    if (i != num_cmds - 1){
		if (i % 2 != 0){
		    close(filedes[1]); // for odd i
		}else{
		    close(filedes2[1]); // for even i
		}
	    }
	    printf("Child process could not be created\n");
	    return;
	}
	if(pid==0){
	    // If we are in the first command
	    if (i == 0){
		dup2(filedes2[1], STDOUT_FILENO);
	    }
	    else if (i == num_cmds - 1){
		if (num_cmds % 2 != 0){ // for odd number of commands
		    dup2(filedes[0],STDIN_FILENO);
		}else{ // for even number of commands
		    dup2(filedes2[0],STDIN_FILENO);
		}
		// If we are in a command that is in the middle, we will
		// have to use two pipes, one for input and another for
		// output. The position is also important in order to choose
		// which file descriptor corresponds to each input/output
	    }else{ // for odd i
		if (i % 2 != 0){
		    dup2(filedes2[0],STDIN_FILENO);
		    dup2(filedes[1],STDOUT_FILENO);
		}else{ // for even i
		    dup2(filedes[0],STDIN_FILENO);
		    dup2(filedes2[1],STDOUT_FILENO);
		}
	    }
	  //  puts("i am in pipe handler\n");
	    if (execvp(command[0],command)==err){
		kill(getpid(),SIGTERM);
	    }
	}
	// CLOSING DESCRIPTORS ON PARENT
	if (i == 0){
	    close(filedes2[1]);
	}
	else if (i == num_cmds - 1){
	    if (num_cmds % 2 != 0){
		close(filedes[0]);
	    }else{
		close(filedes2[0]);
	    }
	}else{
	    if (i % 2 != 0){
		close(filedes2[0]);
		close(filedes[1]);
	    }
	    else{
		close(filedes[0]);
		close(filedes2[1]);
	    }
	}
	waitpid(pid,NULL,0);
	i++;
    }
}
