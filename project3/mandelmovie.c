
#include <math.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_IMAGES 50
#define MAX_PROCESSES MAX_IMAGES
#define DIGITS numDigits(MAX_IMAGES)
#define MAX_ZOOM 2
#define MIN_ZOOM .000001

int buildCmd(char ** mandel, char s[], char fileName[]);
char* buildFileName(int i);
int numDigits(int i);
void usage();
int valid(int argc, char *argv[]);

int main( int argc, char *argv[] ) {
	int p = valid(argc, argv), i, n=0, status;
	if(p<0) { //invalid input found
		usage();
	}

	char *mandel[16];
	pid_t pids[MAX_IMAGES];

	for(i=0;i<MAX_IMAGES;++i) {
		pids[i] = fork();
		n++;
		if(pids[i]<0) {
			printf("Err: Unable to fork.");
			--n;
			exit(1);
		} else if(pids[i] == 0) {
			//in child
			// smooth zoom: s=a to s=b, start with s=a and then multiply it each time by exp(log(b/a)/50).
			double base = exp(log(MIN_ZOOM/MAX_ZOOM)/(MAX_IMAGES-1));
			//printf("%lf\n", base);
			double sDouble = 2 * pow(base, i);
			char* fileName = buildFileName(i);
			char sStr[9]; //up to 6 decimal places accepted
			snprintf(sStr, 9, "%.6lf", sDouble);
			buildCmd(mandel, sStr, fileName);
			free(fileName);

			execvp(mandel[0], mandel);

		} else {
			while(n>=p) {
				int pid = wait(&status);
				//printf("Child with PID %ld exited with status %d.\n", (long)pid, status);
				if(status) {
					printf("Warning: Child with PID %ld exited with status %d.\n", (long)pid, status);
				}
				--n;
			}

		}
	}
	while(n>0) { //wait for remaining processes to finish
		int pid = wait(&status);
		//printf("Child with PID %ld exited with status %d.\n", (long)pid, status);
		if(status) {
			printf("Warning: Child with PID %ld exited with status %d.\n", (long)pid, status);
		}
		--n;
	}

	//buildCmd(mandel, ".000001", buildFileName(1));

	//execvp(mandel[0], mandel);



	return 0;
}
int buildCmd(char ** mandel, char s[], char fileName[]) {
	mandel[0] = strdup("./mandel");
	mandel[1] = strdup("-x");
	mandel[2] = strdup("-.09999");
	mandel[3] = strdup("-y");
	mandel[4] = strdup("-.835599");
	mandel[5] = strdup("-s");
	mandel[6] = strdup(s);
	mandel[7] = strdup("-m");
	mandel[8] = strdup("4000");
	mandel[9] = strdup("-W");
	mandel[10] = strdup("875");
	mandel[11] = strdup("-H");
	mandel[12] = strdup("875");
	mandel[13] = strdup("-o");
	mandel[14] = strdup(fileName);
	mandel[15] = NULL;

	return 0;
}
char* buildFileName(int i) {
	char base[] = "mandel";
	char ext[] = ".bmp";
	char num[DIGITS+1];
	char *final = malloc(11+DIGITS); //11 is "mandel.bmp + \0"

	sprintf(num, "%d", i);

	sprintf(final, "%s%s%s", base, num, ext);
	//printf("%s\n", final);
	return final;
}
int numDigits(int i) {
	int digits = 0;
	if (i <= 0) digits = 1;
    while (i) {
        i /= 10;
        digits++;
    }
    return digits;
}
void usage() {
	printf("Usage: mandelmovie <number of processes>\n");
	printf("Default number of processes is 1.\n");
	exit(1);

}
int valid(int argc, char* argv[]) {
	char *ptr;
	int p;

	if(argc>2) {
		printf("Err: Too many args!\n");
		return -1;
	} else if(argc==1) {
		printf("Warning: Number of processes not defined, defaulting to 1.\n");
		return 1;
	} else if(!isdigit(argv[1][0])||atoi(argv[1])==0) {
		printf("Err: '%s' NaN or less than 1.\n", argv[1]);
		return -1;
	} else {
		long num = strtol(argv[1], &ptr, 10);
		if(num > MAX_PROCESSES) {
			printf("Err: Input exceeds the maximum number of processes allowed (%d)\n", MAX_PROCESSES);
			printf("You can't create more processes than there are images to generate.\n");
			return -1;
		} else {
			p = (int)num;
			//printf("%d\n", p);
			return p;
		}
	}
}

// ./mandel -x -.09999 -y -.835599 -s .000001 -m 4000 -W 875 -H 875
