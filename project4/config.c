// config.cpp
// Basic test driver for ConfigProcessor class
// Author: Kat Herring
// Date: 3/24/17

#include "config.h"

using namespace std;

int main(int argc, char *argv[]) {
  int parseError;
  if(argc!=2) {
    cout << "Err: No config file provided!" << endl;
    return 1;
  }
  string fName = argv[1];

  ConfigProcessor test;
  parseError = test.readFile(fName);
  if(!parseError) {
    test.display();
  }
  return 0;
}
