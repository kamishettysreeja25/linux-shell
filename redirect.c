#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include "util.h"
int main()
{
int numTokens;
int fileDescriptor,standardOut;
char line[256];	
char *tokens[256];	
int j;
char *argv2[256];
	memset ( line, '\0', 256 );
		// We wait for user input
		fgets(line, 256, stdin);
		tokens[0] = strtok(line," \n\t");
	    // We read all the tokens of the input and pass it to our
	    // commandHandler as the argument
	    numTokens = 1;
	    while((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL) 
		numTokens++;
    	while ( tokens[j] != NULL)
		{
			//printf("%s\n", );
		puts(tokens[j]);
		if ( (strcmp(tokens[j],">") == 0) || (strcmp(tokens[j],"<") == 0) || (strcmp(tokens[j],"&") == 0)){
			break;
		}
		argv2[j] = tokens[j];
		j++;
	}
		//argv2[j]=NULL;
	    if(fork()==0)
	    {
	    	fileDescriptor = open(tokens[j+1], O_CREAT | O_TRUNC | O_WRONLY, 0600); 
				// We replace de standard output with the appropriate file
				standardOut = dup(STDOUT_FILENO); 	// first we make a copy of stdout
															// because we'll want it back
				dup2(fileDescriptor, STDOUT_FILENO); 
				close(fileDescriptor);
		//		printf("%s\n", getcwd(currentDirectory, 1024));
				dup2(standardOut, STDOUT_FILENO);

		//ofd=open(argv1[1],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
/*		puts(argv1[1]);
		ofd=open(argv1[1],O_WRONLY |  O_CREAT );
		dup2(ofd,1);
		close(ofd);*/
		printf("i am god\n");
		if (execvp(*argv2, argv2) < 0)  
		{ 

		    printf("*** ERROR: exec failed\n");
		    exit(1);
		}
	    }
}
