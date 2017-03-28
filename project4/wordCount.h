// wordCount.h
// Class definition for WordCount
// Processes input files and creates vector of file

#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>

using namespace std;

class WordCount {
  public:
    int setQueries(vector<string> q);
    map<string, int> getCounts(string buffer);
  private:
    int search(string token, string query);
    vector<string> queries;
};
int WordCount::setQueries(vector<string> q) {
  queries = q;
  return 0;
}
map<string, int> WordCount::getCounts(string buffer) {
  map<string, int> wordCounts;
  for(auto j = queries.begin(); j != queries.end(); ++j) {
    //initialize counters
    wordCounts.insert(make_pair(*j, 0));
  }
  char delim = ','; //split on commas
  string token;
  size_t pos = buffer.find(delim);
  while ((pos = buffer.find(delim)) != string::npos) {
    token = buffer.substr(0, pos);
    for(auto query = queries.begin(); query != queries.end(); ++query) {
      //count appearances of query in token
      wordCounts[*query] += search(token, *query);
    }
    buffer.erase(0, pos+1);
  }
  //get remainder
  for(auto query = queries.begin(); query != queries.end(); ++query) {
    //count appearances of query in token
    wordCounts[*query] += search(buffer, *query);
  }
  return wordCounts;
}
int WordCount::search(string token, string query) {
  int count = 0;
  size_t pos = 0;
  while((pos = token.find(query)) != string::npos) {
    count++;
    token.erase(0, pos+query.length());
  }
  return count;
}


#endif //WORDCOUNT_H
