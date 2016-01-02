#include "execute.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<signal.h>
// does fork and wait depending on whether it is back=1 or 0
int num_background;
//int ext=100;
/*typedef struct process
{
char cmd[256];
//int count;
int proc_id;
int active;
int no;
}process;*/
extern process proc[1000];
extern int no_bg;
void checkbg()
{
int status,i;
pid_t pid;
while((pid=waitpid(-1,&status,WNOHANG))>0)
{
	for(i=0 ; i< no_bg ; i++)
	{
		if (proc[i].proc_id==pid)
		{
			proc[i].active=0;
			break;
		}
	}
	if (WIFEXITED(status))
	{
		printf("Process with pid %d terminated with exit stauts: ",pid );
		printf("%d\n",WEXITSTATUS(status));
	}
	else
	{
		if (WIFSIGNALED(status))
		{
			printf("Process with pid %d exited due to signal no %d\n",pid,WTERMSIG(status));		
		}
	}

}
}
void job_s()
{
int i;
int j;
//puts("iamhere");
printf("i am so good girl %d\n",no_bg);
for(i=0 ; i< no_bg ; i++)
	{
		if (proc[i].active==1)
		{
			printf("[%d] %s %d\n",j,proc[i].cmd[0],proc[i].proc_id);
			proc[i].no=j;
			j++;

		}
	}
}

void k_job(int no, int sig)
{
int i;
for ( i = 0; i < no_bg; i++)
{
	/* code */
	if (proc[i].no==no)
	{
		kill(proc[i].proc_id,sig);
		/* code */
	}
}
}

void over_kill()
{
int i;
for ( i = 0; i < no_bg; i++)
{
	/* code */
	if(proc[i].active==1)
	{
		kill(proc[i].proc_id,9);
		/* code */
	}
}
}

void  execute(char **argv,int back)
{
    pid_t  pid;
    int    status;
    printf(" i am not prachi %d\n ",back  );
    if ((pid = fork()) < 0) {     /* fork a child process           */
	printf(" forking child process failed\n");
	exit(1);
    }   
    else if (pid == 0) {          /* for the child process:         */

	if(back==1)
	{   
		num_background++;
	    setpgid(0,0);
	    printf("[%d] %d\n",num_background,getpid());
		proc[no_bg].proc_id=getpid();
		strcpy(proc[no_bg].cmd,argv[0]);
		proc[no_bg].active=1;
	    no_bg++;
	    if (execvp(*argv, argv) < 0) {     /* execute the command  */
		printf("*** ERROR: exec failed\n");
		exit(1);
	    }

	}   
	else if(back==0)
	{   
		printf("%d\n",getpid() );
	    if (execvp(*argv, argv) < 0)  
	    {     /* execute the command  */
		printf("*** ERROR: exec failed\n");
		exit(1);
	    }
	}

    }
    else
    {
    	if(back==0)
    {

	while (wait(&status) != pid)       /* wait for completion  */
	    ;
    }
    else
    {
	//signal(SIGCHLD,childhandler);
	 //back=0;
	    //printf("child exited with %d\n",pid);
    }
}
}

