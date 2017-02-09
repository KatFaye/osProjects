#include <stdio.h>
#include <signal.h>

char bKeepLooping = 1;

void sigintHandler(int unusedVar) {
    bKeepLooping = 0;
}
int main( int argc, char *argv[] ) {

	signal(SIGINT, sigintHandler);

    while(bKeepLooping) { //do nothing
    }
    printf("Exited successfully\n");
    return 0;
}