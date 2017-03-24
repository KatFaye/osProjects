CC=g++
FLAGS= $(eval LIBCURL_LOC = curl-config --libs) -Wall -g
# NOTE: Above eval gets needed libraries for libcurl

all: config

%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

config: config.o
	$(CC) $(FLAGS) -o $@ $^

site-tester: site-tester.o
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm config *.o
