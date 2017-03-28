// wordCount.cpp
// Basic test driver for wordCount class
// Author: Kat Herring

#include "wordCount.h"

int main(void) {
  vector<string> queries = {"test", "lost"};
  WordCount test;
  test.setQueries(queries);
  map<string, int> m = test.getCounts("my testing nevertest, lost string here it testis");
  for(auto it = m.cbegin(); it!= m.cend(); ++it) {
    cout << it->first << " " << it->second << endl;
  }
}
