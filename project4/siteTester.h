// siteTester.h
// Class definition for siteTester
// master class encapsulating config, fileVec, urlFetch, and wordCount
//Author: Kat Herring
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <signal.h>
#include <unistd.h>

#include "config.h"
#include "fileVec.h"
#include "urlFetch.h"
#include "wordCount.h"
using namespace std;

class siteTester {
  public:
    siteTester();
    int setConfig(string f);
    void getConfig(); //displays current config
    int run();
    int fetchPeriod();
    int getPeriod();
  private:
    int buildCSV(map<string, int> counts, string currSite);
    static void sigAlrmHandler(int t);
    static void sigIntHandler(int t);
    ConfigProcessor params;
    FileProcessor parsedFiles;
    URLFetch fetcher;
    WordCount totals;
    int batch;
    int e;
};
siteTester::siteTester() {
  batch = 0;
}
int siteTester::getPeriod() {
  return params.getPeriod();
}
int siteTester::fetchPeriod() {
  batch++;
  vector<string> sites = parsedFiles.getSites();
  vector<string> search = parsedFiles.getSearch();
  totals.setQueries(search);
  map<string, int> wordCounts;
  for(auto j = search.begin(); j != search.end(); ++j) {
    //initialize counters
    wordCounts.insert(make_pair(*j, 0));
  }
  for(auto i = sites.begin(); i != sites.end(); ++i) {
    fetcher.fetchSite((*i).c_str());
    map<string, int> counts = totals.getCounts(fetcher.getData());
    for(auto it = counts.cbegin(); it!= counts.cend(); ++it) {
      wordCounts[it->first] += it->second;
      buildCSV(counts, (*i));
    }
  }
  return 0;
}
int siteTester::run() {
  if(parsedFiles.readSite(params.getSite()) || parsedFiles.readSearch(params.getSearch())) {
    // bail if error
    return 1;
  }
  fetchPeriod();
  return 0;
}
int siteTester::buildCSV(map<string, int> counts, string currSite) {
  time_t timer;
  struct tm *ptm;
  time(&timer);
  ptm = localtime(&timer);
  ofstream outFile;
  string fName = to_string(batch) + ".csv";
  outFile.open(fName, ios::out | ios::app);
  for(auto it=counts.cbegin();it!=counts.cend();++it) {
    outFile <<  ptm->tm_year+1900 << "-" << ptm->tm_mon+1 << "-" <<
    ptm->tm_mday << "-" << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << ","
     << it->first << "," << currSite << "," << it->second << endl;
  }
  outFile.close();
  return 0;
}
int siteTester::setConfig(string f) {
  int parseError = params.readFile(f);

  if(parseError) {
    return 1;
  }
  return 0;
}
void siteTester::getConfig() {
  params.display();
}
