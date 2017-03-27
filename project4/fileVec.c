// fileVec.cpp
// Basic test driver for fileProcessor class
// Author: Kat Herring

#include "fileVec.h"

using namespace std;

int main(void) {
  FileProcessor fTest;
  fTest.readSite("ExampleSite.txt");
  fTest.readSearch("ExampleSearch.txt");
  vector<string> sites = fTest.getSites();
  vector<string> search = fTest.getSearch();
  for(auto i = sites.begin(); i != sites.end(); ++i) {
    cout << *i << endl;
  }
  for(auto i = search.begin(); i != search.end(); ++i) {
    cout << *i << endl;
  }
  return 0;
}
