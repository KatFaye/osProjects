// siteTester.c
// driver program for siteTester
// Author: Kat Herring

#include "siteTester.h"

siteTester testProg;

void sigAlrmHandler(int t) {
  cout << "site-tester: Fetching batch..." << endl << flush;
  testProg.fetchPeriod();
  alarm(testProg.getPeriod()); //periodic message
  signal(SIGALRM, sigAlrmHandler);
}
void sigIntHandler(int t) {
  cout << "site-tester: User interrupt. Exiting..." << endl;
  exit(1);
}
int main(int argc, char* argv[]) {
  signal(SIGALRM, sigAlrmHandler);
  signal(SIGINT, sigIntHandler);
  alarm(testProg.getPeriod());
  if(argc < 2) {
    cout << "Warning: no config file provided. Using defaults." << endl;
  } else if(argc > 2) {
    cout << "Err: Too many arguments! Exiting..." << endl;
    return 1;
  } else {
    string fName = argv[1];
    if(testProg.setConfig(fName)) { //error occured
      exit(1);
    }
  }
  if(testProg.run()) {
    //bail on error
    exit(1);
  }
  while(1) {
    //spin
  }
  return 0;
}
