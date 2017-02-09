// copyit.c
// Author: Kat Herring
// 1/26/2017
//copies a file from one place to another
// usage: copyit <sourcefile> <targetfile>

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

char bKeepLooping = 1;
int bytesCopied = 0;

int checkUsage(int argc) {
	//if length != 3, incorrect usage
	if(argc<3) {
		printf("copyit: Too few arguments supplied!");
		printf("\nusage: copyit <sourcefile> <targetfile>\n");
		exit(1);
	} else if(argc>3) {
		printf("copyit: Too many arguments supplied!");
		printf("\nusage: copyit <sourcefile> <targetfile>\n");
		exit(1);
	}
	return 0;
}
void sigintHandler(int unusedVar) {
    bKeepLooping = 0;
    printf("\ncopyit: User interrupt! Aborting copy...\n");
    exit(1);
}
void sigAlrmHandler(int s) {
	printf("copyit: still copying... %u bytes copied so far... \n", bytesCopied);
	alarm(1); //periodic message
	signal(SIGALRM, sigAlrmHandler);
}
int main( int argc, char *argv[] ) {

	int fdSource, fdTarget;
	char buffer[4096]; //buffer is 4096 bytes
	size_t nRead;
	checkUsage(argc);

	char *sourceFile = argv[1];
	char *targetFile = argv[2];
	//printf("%s %s\n", sourceFile, targetFile);


	signal(SIGINT, sigintHandler);
	signal(SIGALRM, sigAlrmHandler);
	alarm(1); //periodic message

	//open source file
	fdSource = open(sourceFile, O_RDONLY,0);
	if(fdSource<0) { //error
		printf("Unable to open %s: %s\n",sourceFile, strerror(errno));
		exit(1);
	}

	//create target file
	fdTarget = open(targetFile, O_CREAT|O_WRONLY, 0);
	if(fdTarget<0) {
		printf("Unable to create %s: %s\n",targetFile, strerror(errno));
		exit(1);
	}
    while(bKeepLooping) { //copy file loop
    	//read some data from source file
    	size_t nWritten;
    	nRead = read(fdSource, buffer, sizeof(buffer));
    	char *outBuff = buffer;

    	if(nRead<0) {
    		printf("Error reading %s: %s\n",sourceFile, strerror(errno));
			exit(1);
    	} else if(nRead==0) { //copied whole file
    		bKeepLooping = 0;
    	} else { //write some data to target
    		
    		do {
    			nWritten = write(fdTarget, outBuff, nRead);	
    		} while ((nWritten<0) && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)); //interrupted by alarm
    		if(nWritten>=0) {
    			nRead -= nWritten;
    			outBuff += nWritten;
    			bytesCopied += nWritten;
    		} else { //error
    			printf("Error writing to %s: %s\n",targetFile, strerror(errno));
				exit(1);
    		}
    	}

    	
    }
    //close both files
    if(close(fdSource)<0) {
    	printf("Unable to close %s: %s\n",sourceFile, strerror(errno));
		exit(1);
    } else if(close(fdTarget)<0) {
    	printf("Unable to close %s: %s\n",targetFile, strerror(errno));
		exit(1);
    }
    printf("Copied %u bytes from file %s to %s.\n", bytesCopied, sourceFile, targetFile);
    return 0;
}