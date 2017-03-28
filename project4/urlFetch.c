// urlFetch.cpp
// Basic test driver for urlFetch class
// Author: Kat Herring

#include "urlFetch.h"

int main(void) {
  URLFetch test;
  test.fetchSite("http://www.nd.edu");
  cout << test.getData() << endl;
  test.fetchSite("http://www.cnn.com");
  //cout << test.getData() << endl;
  return 0;
}
