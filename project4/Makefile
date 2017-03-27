CC= g++
CPP=/afs/nd.edu/user14/csesoft/new/bin/g++
LD=/afs/nd.edu/user14/csesoft/new/bin/g++`
LDFLAGS= -lcurl -static-libstdc++
FLAGS= $(LDFLAGS) -Wall -g -std=c++11
TARGETS= config fileVec urlFetch
# NOTE: Above eval gets needed libraries for libcurl

all: $(TARGETS)

%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

config: config.o
	$(CC) $(FLAGS) -o $@ $^

fileVec: fileVec.o
	$(CC) $(FLAGS) -o $@ $^

urlFetch: urlFetch.o
	$(CC) $(FLAGS) -o $@ $^

site-tester: site-tester.o
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm $(TARGETS) *.o
