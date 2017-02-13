#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define BUFFERSIZE 4098
#define MAX_WORDS 100

void prompt();
int parseArgs(char *);

int main(int argc, char *argv[]) {
	prompt();
	return 0;
}
void prompt() {
	char buffer[BUFFERSIZE];
	int parsedOK = 0;
	printf("myshell> ");
	while(1 && parsedOK != 15) {
			
		if(fgets(buffer, BUFFERSIZE, stdin) == NULL) {
			if(feof(stdin)) {
				//printf("EOF\n");
				break;
			} else {
				perror("Could not read from stdin");
			}

		} else {
			parsedOK = parseArgs(buffer);
		}
		printf("\nmyshell> ");

	/*int rc = fork();
		if (rc < 0) { // fork failed; exit
			fprintf(stderr, "fork failed\n");
			exit(1);
		 } else if (rc == 0) { // child (new process)
			 printf("hello, I am child (pid:%d)\n", (int) getpid());
			 char *myargs[4];
			 myargs[0] = strdup("cp"); // 
			 myargs[1] = strdup(argv[1]); // argument: file to count
			 myargs[2] = strdup("CloneFile");
			 myargs[3] = NULL; // marks end of array
			 execvp(myargs[0], myargs); // runs word count
			 printf("this shouldn’t print out");
		 } else { // parent goes down this path (main)
			 int wc = wait(NULL);
			 //printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
			 //rc, wc, (int) getpid());

		 }*/
	}

}
int parseArgs(char *buffer) {
	char *arguments[MAX_WORDS]; //100 words allowed
	const char delimiter[] = "\t\r\n\v\f\40"; //space delimited
	char *token;
	char *validcmd[8] = {"wait", "run", "start", "kill", "stop", "continue", "quit", "exit"};
	int ranOK = 0, wstatus = 0, status = 0;
	if(!strchr(buffer, '\n')) {
	//next char was not newline, error
		while((fgetc(stdin)) != '\n');
		//fprintf(stderr, "Err: Input too long. Buffer size %u.\n", BUFFERSIZE-1);
	}
	//printf("%s", buffer);
	token = strtok(buffer, delimiter);
	int counter = 0;
	if(token == NULL) {
		//nothing to parse
		return 1;
	}
	while(token != NULL && counter < MAX_WORDS ) {

		//printf("token: %s\n", token);
		arguments[counter] = malloc(strlen(token) + 1);
		
		strcpy(arguments[counter], token);
		//printf("%s <---- \n", arguments[0]);
		token = strtok(NULL, delimiter);

		counter++;
	}
	if(counter==MAX_WORDS) {
		fprintf(stderr, "Err: Too many args on one line. Discarding arguments after %s.\n", arguments[counter-1]);
	}
	//printf("%s <---- \n", arguments[0]);
	//printf("%s <- arg[0]\n %s <- cmd\n", arguments[0], validcmd[2]);
	//printf("\n%u %u blah", sizeof(arguments[0]), sizeof(validcmd[2]));

	if(!strcmp(arguments[0], validcmd[0])) {
	//printf("\nmatch (wait)");
		int pid = waitpid(-1, &wstatus, 0);
		if (wstatus == 0){
			printf("myshell: process %d exited normally.\n", pid);
		} else {
			printf("myshell: process exited abnormally: %d.\n", wstatus);
		}
		//printf("%d", wstatus);
	} else if(!strcmp(arguments[0], validcmd[1])) {
		//printf("\nmatch (run)");
		int rc = fork();
		if(fork < 0) {
			printf("Unable to fork process.");
		} else if(rc==0) {
			int i;
			if(counter==1) {
				printf("myshell: No arguments supplied to: %s", arguments[0]);
				fflush(stdout);
				_exit(2);
			}
			printf("myshell: Started process %d.", getpid());
			fflush(stdout);
			char *myargs[counter];
			for(i=0;i<counter-1;i++) {
				myargs[i] = strdup(arguments[i+1]);
				//printf("%s %d\n", myargs[i], i);
				//fflush(stdout);
			}
			myargs[counter-1] = NULL;
			execvp(myargs[0], myargs);
						//fflush(stdout);
		} else {
			int wc = wait(&status);
			 printf("Exit status of %d was %d- Exited %s", wc, status,
               (status == 0) ? "normally." : "abnormally:");
			 if(status!=0) {
			 	printf(" %s.\n", strsignal(status));
			 }

			return 0;
		}
	} else if(!strcmp(arguments[0], validcmd[2])) {
		//printf("\nmatch (start)");
		int rc = fork();
		if(fork < 0) {
			printf("Unable to fork process.");
		} else if(rc==0) {
			int i;
			//printf("%d", getpid());
			fflush(stdout);
			if(counter==1) {
				printf("myshell: No arguments supplied to: %s", arguments[0]);
				fflush(stdout);
				_exit(2);
			}
			char *myargs[counter];
			for(i=0;i<counter-1;i++) {
				myargs[i] = strdup(arguments[i+1]);
				//printf("%s %d\n", myargs[i], i);
				//fflush(stdout);
			}
			myargs[counter-1] = NULL;
			execvp(myargs[0], myargs);
						//fflush(stdout);
		} else {
			printf("myshell: Process %d started.", rc);
			return 0;
		}
	} else if(!strcmp(arguments[0], validcmd[3])) {
		//printf("\nmatch (kill)");
		if(counter==1) {
			printf("myshell: No arguments supplied to: %s", arguments[0]);
		}
		kill((int)arguments[1], SIGKILL);
		printf("Killed process %s.\n", arguments[1]);
		waitpid(-1, &status, WNOHANG);
		return 0;
	} else if(!strcmp(arguments[0], validcmd[4])) {
		//printf("\nmatch (stop)");
		if(counter==1) {
			printf("myshell: No arguments supplied to: %s", arguments[0]);
		}
		kill((int)arguments[1], SIGSTOP);
		printf("Stopped process %s.\n", arguments[1]);
	} else if(!strcmp(arguments[0], validcmd[5])) {
		//printf("\nmatch (continue)");
		if(counter==1) {
			printf("myshell: No arguments supplied to: %s", arguments[0]);
		}
		kill((int)arguments[1], SIGCONT);
		printf("Continued process %s.\n", arguments[1]);
	} else if(!strcmp(arguments[0], validcmd[6])) {
		//printf("\nmatch (quit)");
		return 15;
	} else if(!strcmp(arguments[0], validcmd[7])) {
		//printf("\nmatch (exit)");
		return 15;
	} else {
		printf("myshell: Unknown command: %s ", arguments[0]);
	} 

	//printf("%i", counter);
	//int i;
	/*for(i=0;i<counter;i++) {
		printf("%s\n", arguments[i]);
	}*/
	int i;
	for(i=0;i<counter;i++) {
		free (arguments[i]);
	}
	return 0;
}
