CC= g++
CPP=/afs/nd.edu/user14/csesoft/new/bin/g++
LD=/afs/nd.edu/user14/csesoft/new/bin/g++`
LDFLAGS= -lcurl -static-libstdc++
FLAGS= $(LDFLAGS) -Wall -g -std=c++11
# NOTE: Above eval gets needed libraries for libcurl

all: config

%.o: %.c %.h
	$(CPP) $(FLAGS) -c $< -o $@

config: config.o
	$(CPP) $(FLAGS) -o $@ $^

site-tester: site-tester.o
	$(CPP) $(FLAGS) -o $@ $^

clean:
	rm config *.o
