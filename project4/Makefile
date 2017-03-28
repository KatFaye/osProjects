CC= g++
CPP=/afs/nd.edu/user14/csesoft/new/bin/g++
LD=/afs/nd.edu/user14/csesoft/new/bin/g++`
LDFLAGS= -lcurl -static-libstdc++
FLAGS= $(LDFLAGS) -Wall -g -std=c++11
HEADERS = config.h fileVec.h urlFetch.h wordCount.h siteTester.h
TARGET=  site-tester
# NOTE: Above eval gets needed libraries for libcurl

all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(FLAGS)

site-tester: siteTester.o
	$(CC) -o $@ $^  $(FLAGS)

clean:
	rm $(TARGET) *.o *.csv
