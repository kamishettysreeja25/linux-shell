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
#include "execute.h"
#include "pipeHandler.h"
#include "output.h"
#include "input.h"
#include "both.h"
void commandHandler(char *args[])
{
    pipeHandler(args);
}
int main()
{
    int count_pipe=0,in=0,out=0,pipe=0,append_out=0;char  line[1024];             /* the input line                 */
    char  *argv[64], *delim,*tokens[256], *si,dub[2],dub1[2], *token, *f[1000], *cmd[1000];
    int count,i,j,k,t, u,v,w,status,temp,flag;
    FILE *user, *system,*dir;
    char st[100], stt[100], std[1000], stdd[1000], s[10000], d[1000];
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
    while (1) {                   
	t=0;
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
		count_pipe++;
	    }
	    flag++;
	}
	if(out==1 && in==0 &&  pipe==0)
	{
	    int sout;
	    puts("i am in out only");
	    delim=">";
	    parse(line , argv1 ,delim);
	    delim=" ";
	    parse(argv1[0] , argv2 ,delim);
	    if(fork()==0)
	    {
		ofd=open(argv1[1],O_WRONLY |  O_CREAT  | O_TRUNC,0600);
		sout=dup(STDOUT_FILENO);
		dup2(ofd,STDOUT_FILENO);
		close(ofd);
		dup2(sout,STDOUT_FILENO);
		if (execvp(*argv2, argv2) < 0)  
		{ 
		    printf("*** ERROR: exec failed\n");
		    exit(1);
	
	    }
	    }
		else
		    wait(NULL);
	    
	     // execute(argv2,0);
	}
	else if(append_out==1 && in==0 && pipe==0)
	{
	    puts("i am in append only");
	    delim=">>";
	    parse(line , argv1 ,delim);    
	    delim=" ";
	    parse(argv1[0] , argv2 ,delim);
	    //open(argv1[1],"a");

	    if(fork()==0)
	    {
		afd=open(argv1[1],O_APPEND | O_WRONLY|O_CREAT);
		dup2(afd,1);
		close(afd);
		if (execvp(*argv2, argv2) < 0)  
		{ 
		    printf("*** ERROR: exec failed\n");
		    exit(1);
		}
	    }
	}
	else if(out==0 && in==1 && pipe==0)
	{
	    puts("i am in in only");
	    delim="<";
	    parse(line , argv1 ,delim);    
	    delim=" ";
	    parse(argv1[0] , argv2 ,delim);
	    if(fork()==0)
	    {
		ifd = open(argv1[1],O_RDONLY);
		dup2(ifd,0);
		close(ifd);
		if (execvp(*argv2, argv2) < 0)  
		{ 
		    printf("*** ERROR: exec failed\n");
		    exit(1);	
		}
	    }
	    //execute(argv2,0);
	}
	else if(out==1 && in==1 && pipe==0)
	{
	    puts("i am in & out only");
	    int in_i,out_o;    
	    delim=">";
	    parse(line , argv1 ,delim);
	    delim="<";
	    parse(argv1[0],argv2,delim);
	    if(fork()==0)
	    {
		in_i=open(argv2[1],O_RDONLY);
		out_o=open(argv1[1],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		dup2(in_i,0);
		dup2(out_o,1);
		close(in_i);
		close(out_o);
		delim=" ";
		parse(argv2[0] , argv3 ,delim);
		//execute(argv3,0);

		if (execvp(*argv3, argv3) < 0)  
		{ 
		    printf("*** ERROR: exec failed\n");
		}
	    }
	}
	else if(pipe==1)
	{
	    tokens[0] = strtok(line," \n\t");
	    // We read all the tokens of the input and pass it to our
	    // commandHandler as the argument
	    numTokens = 1;
	    while((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL) 
		numTokens++;
	    commandHandler(tokens);


	}
	else
	{   
	    puts("i am  in else");
	    delim=";";
	    parse(line, argv,delim);       /*   parse the line               */
	}
	delim=" ";
	i=0;
	while(argv[i]!=NULL)
	{
	    temp=0;
	    parse(argv[i], argv1,delim);    
	    if (strcmp(argv1[0], "exit") == 0)  /* is it an "exit"?     */
		exit(0);
	    j=0;
	    while(argv1[j]!='\0')
		j++;
	    if(strcmp(argv1[0],"cd")==0)
	    {
		if(argv1[1]==NULL)
		{
		    printf("excepted arguments for cd \n");
		}
		else
		{
		    chdir(argv1[1]);
		}
		i++;
	    }
	    else if(strcmp(argv1[0],"echo")==0)
	    {

		if(argv1[1]==NULL)
		{
		    printf("\n");
		}
		else
		{
		    /*strcpy(si,argv[1]);
		      }*/
		k=1;
		while(argv1[k]!=NULL)
		{
		    printf("%s " ,argv1[k]);
		    k++;
		}
		printf("\n");
	    }
	    i++;
	}
	else if(strcmp(argv1[j-1],"&")==0)
	{
	    temp=-1;
	    argv1[j-1]="NULL";
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
return 0;
}
