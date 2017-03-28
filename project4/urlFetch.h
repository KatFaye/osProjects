// urlFetch.h
// Class definition for URLFetch
// fetches site at input url
// based on example "getinmemory.c"

#ifndef URLFETCH_H
#define URLFETCH_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
class URLFetch {
  public:
    struct MemoryStruct {
      char *memory;
      size_t size;
    };
    int fetchSite(const char* url);
    string getData();
    static size_t WriteMem(void * contents, size_t size, size_t nmemb, void *userp);
  private:
    string fetchData;
};
string URLFetch::getData() {
  return fetchData;
}
size_t URLFetch::WriteMem(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    cerr << "Not enough memory (realloc returned NULL)" << endl;
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
int URLFetch::fetchSite( const char* url) {
  fetchData = ""; //clear buffer
  CURL *curl_handle;
  CURLcode res;

  MemoryStruct chunk;
  chunk.memory = (char *)malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMem);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent-kherring/1.0");

  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if(res != CURLE_OK) {
    cerr << "curl_easy_perform() failed" << curl_easy_strerror(res) << endl;
  } else {
    cout << (long)chunk.size << " bytes retrieved!" << endl;
    fetchData = chunk.memory;
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);
  curl_global_cleanup();

  return 0;
}

#endif //URLFETCH_H
