// config.h
// Class definition for ConfigProcessor
// Processes input config file and displays to stdout

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

class ConfigProcessor {
  public:
    ConfigProcessor();
    int readFile(string f);
    void display(); //display current settings
    int getPeriod();
    int getFetch();
    int getParse();
    string getSearch();
    string getSite();
  private:
    int setArg(string arg, string val);
    int stoiWrapper(string val);
    void checkDefault();
    const string validParam[5] =
      { "PERIOD_FETCH",
      "NUM_FETCH",
      "NUM_PARSE",
      "SEARCH_FILE",
      "SITE_FILE" };
    int PERIOD_FETCH;
    int NUM_FETCH;
    int NUM_PARSE;
    string SEARCH_FILE;
    string SITE_FILE;
};

ConfigProcessor::ConfigProcessor() {
  PERIOD_FETCH = 180;
  NUM_FETCH = 1;
  NUM_PARSE = 1;
  SEARCH_FILE = "Search.txt";
  SITE_FILE = "Sites.txt";
}
int ConfigProcessor::getFetch() {
  return NUM_FETCH;
}
int ConfigProcessor::getPeriod() {
  return PERIOD_FETCH;
}
int ConfigProcessor::getParse() {
  return NUM_PARSE;
}
string ConfigProcessor::getSearch() {
  return SEARCH_FILE;
}
string ConfigProcessor::getSite() {
  return SITE_FILE;
}
void ConfigProcessor::checkDefault() {
  string base = "Warning: Using default paramater for ";
  if(PERIOD_FETCH==180) {
    cout << base << "PERIOD_FETCH: " << PERIOD_FETCH << endl;
  }
  if(NUM_FETCH==1) {
    cout << base << "NUM_FETCH: " << NUM_FETCH << endl;
  }
  if(NUM_PARSE==1) {
    cout << base << "NUM_PARSE: " << NUM_PARSE << endl;
  }
  if(SEARCH_FILE=="Search.txt") {
    cout << base << "SEARCH_FILE: " << SEARCH_FILE << endl;
  }
  if(SITE_FILE=="Sites.txt") {
    cout << base << "SITE_FILE: " << SITE_FILE << endl;
  }
}
int ConfigProcessor::readFile(string f) {
  char delim = '=';
  string line, arg, val;
  size_t pos;
  ifstream inFile(f);

  if(inFile) {
    while(getline(inFile, line)) {
      if((pos = line.find(delim)) != string::npos) {
        arg = line.substr(0, pos);
        val = line.substr(pos+1); //skip delim
        int parseError = setArg(arg, val);
        if(parseError) {
          return 1;
        }
      } else {
        cout << "Warning: Unable to parse line '" << line << "'. Skipping..." << endl;
      }
    }
  } else {
    cout << "Err: File '" << f << "' not found!" << endl;
    return 1;
  }
  checkDefault();
  inFile.close();
  return 0;
}
int ConfigProcessor::setArg(string arg, string val) {
  if(arg=="PERIOD_FETCH") {
    if(stoiWrapper(val)!=-1) {
      if(stoi(val) < 361 && stoi(val) > 0) { //six minutes max
        PERIOD_FETCH = stoiWrapper(val);
      } else {
        cout << "Warning: " << val << " not in range (1-360). Skipping..." << endl;
      }
    }
  } else if(arg=="NUM_FETCH") {
    if(stoiWrapper(val)!=-1) {
      if(stoi(val) < 9 && stoi(val) > 0) {
        NUM_FETCH = stoiWrapper(val);
      } else {
        cout << "Warning: " << val << " not in range (1-8). Skipping..." << endl;
      }
    }
  } else if(arg=="NUM_PARSE") {
    if(stoiWrapper(val)!=-1) {
      if(stoi(val) < 9 && stoi(val) > 0) {
        NUM_PARSE = stoiWrapper(val);
      } else {
        cout << "Warning: " << val << " not in range (1-8). Skipping..." << endl;
      }
    }
  } else if(arg=="SEARCH_FILE") {
    if(!ifstream(val)) {
      cout << "Err: Provided file name '" << val << "' does not exist! Exiting..." << endl;
      return 1;
    } else {
      SEARCH_FILE = val;
    }
  } else if(arg=="SITE_FILE") {
    if(!ifstream(val)) {
      cout << "Err: Provided file name '" << val << "' does not exist! Exiting..." << endl;
      return 1;
    } else {
    SITE_FILE = val;
    }
  } else {
    cout << "Warning: Invalid arg '" << arg << "'. Skipping..." << endl;
  }
  return 0;
}
int ConfigProcessor::stoiWrapper(string val) {
  int v;
  try {
    v = stoi(val);
  } catch (const invalid_argument& ia) {
    cerr << "Warning: Invalid argument '" << val << "'. Skipping..." << endl;
    return -1;
  }
  return v;
}
void ConfigProcessor::display() {
  cout << "PERIOD_FETCH=" << PERIOD_FETCH << endl;
  cout << "NUM_FETCH=" << NUM_FETCH << endl;
  cout << "NUM_PARSE=" << NUM_PARSE << endl;
  cout << "SEARCH_FILE=" << SEARCH_FILE << endl;
  cout << "SITE_FILE=" << SITE_FILE << endl;
}
#endif
