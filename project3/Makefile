
all: mandel mandelmovie

mandel: mandel.o bitmap.o
	gcc mandel.o bitmap.o -o mandel -lpthread

mandelmovie: mandelmovie.o mandel
	gcc mandelmovie.o -o mandelmovie -lm

mandel.o: mandel.c
	gcc -Wall -g -c mandel.c -o mandel.o

bitmap.o: bitmap.c
	gcc -Wall -g -c bitmap.c -o bitmap.o

mandelmovie.o: mandelmovie.c
	gcc -Wall -g -c mandelmovie.c -o mandelmovie.o

clean:
	rm -f *.o mandel mandelmovie *.bmp

test: mandelmovie mandel
	./mandelmovie 5
	./mandel -x -.09999 -y -.835599 -s .000001 -m 4000 -W 875 -H 875 -n 1 -o test1.bmp
	./mandel -x -.09999 -y -.835599 -s .000001 -m 4000 -W 875 -H 875 -n 10 -o test2.bmp
