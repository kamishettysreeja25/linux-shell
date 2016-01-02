#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<signal.h>
#include<termios.h>
#include<errno.h>
#include "parse.h"
#include "pipeHandler.h"
#include "input.h"
#include<setjmp.h>
int num_background;
typedef struct process
{
	char cmd[256];
	int proc_id;
	int active;
	int no;
}process;
pid_t kidpid;
jmp_buf env;
char **comd;
int no_bg;
process proc[1000];
void child_handler(int sig)
{
	pid_t pid;
	kill(kidpid,SIGCONT);
	//printf("%d\n",kidpid );
	pid=kidpid;
	num_background++;
	printf("\n[%d] stopped %d\n",num_background,pid);
	/*(proc[no_bg].proc_id=pid;
	strcpy(proc[no_bg].cmd,comd[0]);
	proc[no_bg].active=1;
	*/no_bg++;
	longjmp(env,1);
    
}
void sig()
{
	int status,i;
	pid_t pid;
}
void childhandler(int sig)
{
	signal(SIGINT,SIG_DFL);
		signal(SIGTSTP,SIG_DFL);
		signal(SIGCHLD,SIG_DFL); 
	int status,i;
	pid_t pid;
	if(sig==SIGCHLD)
	{
		sleep(0);
	//printf("%d i am printing the pid\n\n",pid);	
	while((pid=waitpid(-1,&status,WNOHANG))>0)
	{
		//	printf("%d background no is\n",no_bg);
		for(i=0 ; i< no_bg ; i++)
		{
			if (proc[i].proc_id==pid)
			{
				//	printf("am i possible\n");
				num_background--;
				proc[i].active=0;
				break;
			}
	//		printf("%d\n",pid);
		}
		for (i = 0; i < no_bg; i++)
		{
		if(proc[i].proc_id==pid )
		{
			//puts("hi kanna");
			if (WIFEXITED(status)>0)
			{
					printf("Process with pid %d terminated with exit stauts: ",pid);
					printf("%d\n",WEXITSTATUS(status));
			}
		}
		}
		  if (WIFSIGNALED(status))
		  {
		  printf("Process with pid %d exited due to signal no %d\n",pid,WTERMSIG(status));		
		  }
			
	}
	//sleep(1);
}
}
void job_s()
{
	int i;
	int j=1;
	//printf("i am so good girl %d\n",no_bg);
	for(i=0 ; i< no_bg ; i++)
	{
		//	printf("%d",proc[i].active);
		if (proc[i].active==1)
		{
			//	puts("iamhere");
			printf("[%d] %s %d\n",j,proc[i].cmd,proc[i].proc_id);
			proc[i].no=j;
			j++;
		}
	}
}

void k_job(char *no, char*  sig)
{
	int i;
	int j=1;
		for(i=0 ; i< no_bg ; i++)
	{
		//	printf("%d",proc[i].active);
		if (proc[i].active==1)
		{
			//	puts("iamhere");
			//printf("[%d] %s %d\n",j,proc[i].cmd,proc[i].proc_id);
			proc[i].no=j;
			j++;
		}
	}
	//	printf("%d---\n",no_bg);
	//	printf("%s==%s\n",no,sig);	
	for ( i = 0; i < no_bg; i++)
	{
		/* code */
		//		printf("%d %d no , sig \n",atoi(no),atoi(sig));
		if (proc[i].no==atoi(no))
		{
			kill(proc[i].proc_id,atoi(sig));
			if(errno == EINVAL)
				printf("invalid signal number\n" );
			if(atoi(no) > no_bg)
				printf("invalid job num\n");
			/* code */
			if(atoi(sig)==9)
				proc[i].active=0;
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
			proc[i].active=0;
			/* code */
		}
	}
}
void  execute(char **argv,int back)
{
	pid_t  pid;
	int    status;
	if ((pid = fork()) < 0) {     /* fork a child process           */
		printf(" forking child process failed\n");
		exit(1);
	}   
	else if (pid == 0) {          /* for the child process:         */
		if(back==1)
		{   
			setpgid(0,0);
			//	printf("%d i am \n",no_bg);
			if (execvp(*argv, argv) < 0) {     /* execute the command  */
				printf("ERROR: exec failed invalid commnand\n");
				exit(1);
			}
		}   
		else if(back==0)
		{   
			//printf("%d\n",getpid() );
			if (execvp(*argv, argv) < 0)  
			{     /* execute the command  */
				printf("ERROR: exec failed command doesn't exist\n");
				exit(1);
			}
		}

	}
	else
	{
		kidpid=getpid();
		if(back==0)
		{
			while (wait(&status) != pid)       /* wait for completion  */
				;
		}
		else
		{

			num_background++;
			printf("[%d] %d\n",num_background,pid);
			proc[no_bg].proc_id=pid;
			strcpy(proc[no_bg].cmd,argv[0]);
			proc[no_bg].active=1;
			no_bg++;
			signal(SIGCHLD,childhandler);
			//			sleep(2);			
			//back=0;
			//printf("child exited with %d\n",pid);
		}
	}
}
void handler(int sig)
{
	printf("\n");
	longjmp(env,1);
}
int main()
{
	// variables declaration
	int count_pipe=0,in=0,out=0,pipe=0,append_out=0;char  line[1024];             /* the input line                 */
	char  *argv[64], *delim,*tokens[256], *si,dub[2],dub1[2], *token, *f[1000], *cmd[1000];
	int count,i,j,k,t, u,v,w,status,temp,flag;
	FILE *user, *system,*dir;
	char st[100], stt[100],std[1000], stdd[1000], s[10000], d[1000],h[1000];
	pid_t c_pid,pid;
	//popen it executes the command and stores it
	if(!(user=popen("whoami ","r")))	return 0;
	if(!(system=popen("hostname","r")))return 0;
	if(!(dir=popen("pwd","r")))return 0;
	if(fgets(st,100,user)!=NULL);
	if(fgets(stt,100,system)!=NULL);
	if(fgets(std,1000,dir)!=NULL);
	pclose(dir);
	int l=strlen(st);
	if(st[l-1]=='\n')st[l-1]='\0';
	l=strlen(stt);
	if(stt[l-1]=='\n')stt[l-1]='\0';
	l=strlen(std);
	if(std[l-1]=='\n')std[l-1]='\0';
	char c='\0';
	while (1) 
	{    
	//	sig();
		//init();
		signal(SIGINT,handler);
		signal(SIGQUIT,SIG_IGN);
		signal(SIGTSTP,child_handler);
		signal(SIGTTIN,SIG_IGN);
		//	signal(SIGTTOU,SIG_IGN);
		//		signal(SIGCHLD,SIG_IGN);           
		t=0;
		//		checkbg();
		setjmp(env);
		int numTokens, ofd,ifd,afd;
		char *argv1[64], *argv2[64], *argv3[64];
		count_pipe=0,in=0,out=0,pipe=0,append_out=0;
		if(!(dir=popen("pwd","r")))return 0;
		if(fgets(stdd,1000,dir)!=NULL);
		i=0;
		int l=strlen(stdd);
		if(stdd[l-1]=='\n')stdd[l-1]='\0';//check whether the directory is changed or not
		if(strlen(std) > strlen(stdd))
		{
			count=1;
			strcpy(d,stdd);
		}
		else
		{
			while(std[i]==stdd[i] && std[i]!='\0')
			{
				i++;
			}
			j=0;count=0;k=i;
			while(stdd[i]!='\0')
			{
				d[j]=stdd[i];
				i++;
				j++;
				count=2;
			}
			d[j]='\0';
		}
		if(count==0)
		{
			d[0]='~';
			d[1]='\0';
		}
		if(count==2)
			printf("<%s@%s:~%s>",st,stt,d);
		else
			printf("<%s@%s:%s>",st,stt,d);
		//gets(line);
		bzero(line,sizeof(line));
		while((c=getchar())!='\n')
		{
			strncat(line,&c,1);
		}
		flag=0;
		while(line[flag]!=NULL)
		{

			dub[0]=line[flag];
			dub[1]='\0';
			if(strcmp(dub,">")==0)
			{
				out=1;
				dub1[0]=line[flag+1];
				dub1[1]='\0';
				//puts(dub1);
				if(strcmp(dub1,">")==0)
				{

					append_out=1;
					out=0;
				}
			}
			else if(strcmp(dub,"<")==0)
			{
				in=1;
			}
			else if(strcmp(dub,"|")==0)
			{
				pipe=1;
				//count_pipe++;
			}
			flag++;
		}

		if(in==1 || out ==1 || pipe==1 || append_out==1  )
		{
		tokens[0] = strtok(line," \n\t");
	    numTokens = 1;
	 	while((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL) 
			numTokens++;
		//puts("123");
		check(tokens);
		//input(tokens, in , out , append_out , pipe);
		}
		else
		{   
			//puts("i am  in else");
			delim=";";
			parse(line, argv,delim);       /*   parse the line               */
			delim=" ";
			i=0;
			while(argv[i]!=NULL)
			{
				temp=0;
				comd=argv1;
				parse(argv[i], argv1,delim);    
				if (strcmp(argv1[0], "quit") == 0)  /* is it an "exit"?     */
					exit(0);
				j=0;
				while(argv1[j]!=NULL)
					j++;
				if(strcmp(argv1[0],"cd")==0)
				{
					if(argv1[1]==NULL)
					{
						chdir(stdd);
					//	printf("excepted arguments for cd \n");
					}
					else if(strcmp(argv1[1],"~")==0)
					{
						chdir(stdd);
					}
					else
					{
						chdir(argv1[1]);
					}
					i++;
				}
				else if(strcmp(argv1[0],"echo")==0)
				{
					h[0]=argv1[1][0];
					h[1]='\0';
					l=strlen(argv1[1]);
					u=1;
					v=0;
					if(strcmp(argv1[1],"$HOME")==0)
					{
						printf("%s",std);
					}

					else if (strcmp(h,"$")==0)
					{
						while(u<=l)
						{
							h[v]=argv1[1][u];
							v++;
							u++;
						}
						h[v]='\0';
						//h=getenv(h);
						printf("%s",getenv(h));
					}
					else
					{
						k=1;
						while(argv1[k]!=NULL)
						{
							printf("%s " ,argv1[k]);
							k++;
						}
					}
					printf("\n");
					i++;
				}
				else if(strcmp(argv1[j-1], "&")==0)
				{
					//puts("watsapp fb also sucks");
					temp=-1;
					argv1[j-1]=NULL;
					i++;
					execute(argv1,1);
				}
				else if(strcmp(argv1[0],"pinfo")==0)
				{
					printf("pid--%d\n",getpid()) ;
					
					i++;
				}
				else if(strcmp(argv1[0],"fg")==0)
				{
					int status;
					//pid_t pid=getpid();
					pid=atoi(argv1[1]);
					//printf("%d\n",no_bg);
					kill(pid,SIGCONT);
					for(u=0; u< no_bg ; u++)
					{
						//puts("hii kanna");
						if(proc[u].proc_id==pid)
							{
							proc[u].active=0;
							//puts("sreeja");
							break;
							}
					}
					waitpid(pid,&status,WCONTINUED);
					i++;
				}
				else if (strcmp(argv1[0],"jobs")==0)
				{
					//puts("i know who am i");
					job_s();
					/* code */
					i++;
				}
				else if (strcmp(argv1[0],"kjobs")==0)
				{
					//puts("i know who am i");
					k_job(argv1[1],argv1[2]);
					/* code */
					i++;
				}
				else if (strcmp(argv1[0],"overkill")==0)
				{
					//	puts("i know who am i");
					over_kill();
					/* code */
					i++;
				}
				else
				{	

					if(temp!=-1)
					{
						i++;
						execute(argv1,0); 
					}
				}
			}
		}
	}
	return 0;
}
