#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<signal.h>
#include "parse.h"
//parses the string acc to deliminator
void  parse(char *line, char **argv,char *delim)
{
    char *token;
    *argv=(char *)malloc(sizeof(char)*1000);
    token=strtok(line,delim);
    while(token!=NULL)
    {   
	*argv++=token;

	token=strtok(NULL,delim);
    }   
    *argv = '\0'; 
}
