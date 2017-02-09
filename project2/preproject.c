#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	//printf("hello world (pid:%d)\n", (int) getpid());
	if(argc<2) { //no file name supplied
		printf("Too few arguments supplied.\n");
		printf("Usage: ./preproject <filename>\n");
		exit(1);
	}
	int rc = fork();
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
	 	char *myargs[4];
	 	myargs[0] = strdup("md5sum");
	 	myargs[1] = strdup(argv[1]);
	 	myargs[2] = strdup("CloneFile");
	 	myargs[3] = NULL;
	 	execvp(myargs[0], myargs);

	 }
	return 0;
}