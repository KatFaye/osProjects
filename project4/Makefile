CC= g++
CPP=/afs/nd.edu/user14/csesoft/new/bin/g++
LD=/afs/nd.edu/user14/csesoft/new/bin/g++`
LDFLAGS= -lcurl -static-libstdc++
FLAGS= $(LDFLAGS) -Wall -g -std=c++11
TARGETS= config fileVec urlFetch wordCount
# NOTE: Above eval gets needed libraries for libcurl

all: $(TARGETS)

%.o: %.c %.h
	$(CC) -c $< -o $@ $(FLAGS)

config: config.o
	$(CC) -o $@ $^ $(FLAGS)

fileVec: fileVec.o
	$(CC) -o $@ $^ $(FLAGS)

urlFetch: urlFetch.o
	$(CC) -o $@ $^  $(FLAGS)

site-tester: site-tester.o
	$(CC) -o $@ $^  $(FLAGS)

wordCount: wordCount.o
	$(CC) -o $@ $^ $(FLAGS)
clean:
	rm $(TARGETS) *.o
