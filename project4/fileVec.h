// fileVec.h
// Class definition for FileProcessor
// Processes input files and creates vector of file

#ifndef FILEVEC_H
#define FILEVEC_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;
class FileProcessor {
  public:
    int readSite(string f = "Sites.txt");
    int readSearch(string f = "Search.txt");
    vector<string> getSites();
    vector<string> getSearch();
  private:
    vector<string> sites;
    vector<string> search;
};
vector<string> FileProcessor::getSites() {
  return sites;
}
vector<string> FileProcessor::getSearch() {
  return search;
}
int FileProcessor::readSite(string f) {
  string line;
  string delim = "http://";
  ifstream inFile(f);

  if(inFile) {
    while(getline(inFile, line)) {
        if((line.find(delim)) != string::npos) {
          sites.push_back(line);
        } else {
          cout << "Err: '" << line << "' not a valid site! Skipping..." << endl;
        }
    }
  } else {
    cout << "Err: File '" << f << "' not found! Exiting..." << endl;
    return 1;
  }
  return 0;
}
int FileProcessor::readSearch(string f) {
  string line;
  char carriage = '\r';
  char comma = ',';
  ifstream inFile(f);

  if(inFile) {
    while(getline(inFile, line)) {
        if(line.find(carriage) == string::npos && line.find(comma) == string::npos) {
          search.push_back(line);
        } else {
          cout << "Err: '" << line << "' not a valid search string! Skipping..." << endl;
        }
    }
  } else {
    cout << "Err: File '" << f << "' not found! Exiting..." << endl;
    return 1;
  }
  return 0;
}
#endif //FILEVEC_H
