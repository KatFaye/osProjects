
#include "bitmap.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

typedef struct {
	struct bitmap *bm;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int max;
	int id;
	int numThreads;
} ThreadArgs;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int iteration_to_color( int i, int max );
int iterations_at_point( double x, double y, int max );
void *compute_image( void *args );

void show_help() {
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates. (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=500)\n");
	printf("-H <pixels> Height of the image in pixels. (default=500)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-n <thread> Set number of threads (default=1)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}

int main( int argc, char *argv[] ) {
	char c;
	int i, rc;

	// These are the default configuration values used
	// if no command line arguments are given.

	const char *outfile = "mandel.bmp";
	double xcenter = 0;
	double ycenter = 0;
	double scale = 4;
	int    image_width = 500;
	int    image_height = 500;
	int    max = 1000;
	int 	 numThreads = 1;
	int 	 joinStatus;
	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"x:y:s:W:H:m:o:n:h"))!=-1) {
		switch(c) {
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				scale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'n':
				numThreads = atoi(optarg);
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}
	ThreadArgs targs[numThreads];
	pthread_t threads[numThreads];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	// Display the configuration of the image.
	printf("mandel: x=%lf y=%lf scale=%lf max=%d outfile=%s\n",xcenter,ycenter,scale,max,outfile);

	// Create a bitmap of the appropriate size.
	struct bitmap *bm = bitmap_create(image_width,image_height);

	// Fill it with a dark blue, for debugging
	bitmap_reset(bm,MAKE_RGBA(0,0,255,0));

	//collect all arguments for compute_image
	for(i=0;i<numThreads;i++) {
		targs[i].bm = bm;
		targs[i].xmin = xcenter-scale;
		targs[i].xmax = xcenter+scale;
		targs[i].ymin = ycenter-scale;
		targs[i].ymax = ycenter+scale;
		targs[i].max = max;
		targs[i].numThreads = numThreads;
	}

	for(i=0;i<numThreads;i++) {
		//printf("Creating thread %d\n", i);
		targs[i].id = i;
		rc = pthread_create(&threads[i], NULL, compute_image, (void *)&targs[i]);
		if(rc) {
			printf("Err: Unable to create thread %d. Exit code: %d", i, rc);
			exit(1);
		}
	}

	for(i=0; i<numThreads;i++) {
		joinStatus = pthread_join(threads[i], NULL);
		if(joinStatus) {
			printf("Err: Unable to join thread %d. Exit status: %d", i, joinStatus);
			exit(1);
		}
	}
	pthread_attr_destroy(&attr);
	// Save the image in the stated file.

	if(!bitmap_save(bm,outfile)) {
		fprintf(stderr,"mandel: couldn't write to %s: %s\n",outfile,strerror(errno));
		return 1;
	}

	return 0;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void *compute_image( void *args ) {
	ThreadArgs  *local = (ThreadArgs *) args;

	//printf("local id: %d\n", local->id);
	struct bitmap *bm = local->bm;
	double xmin = local->xmin;
	double xmax = local->xmax;
	double ymin = local->ymin;
	double ymax = local->ymax;
	int max = local->max;
 	int id = local->id;
	int i;

	//printf("ID: %d\n", id);
	int width = bitmap_width(bm);
	int height = bitmap_height(bm);
	//printf("%d\n", local->numThreads );
	int j = height/local->numThreads*id;

	// For every pixel in the image...
	//printf("Local %d min:%d\n", id, j);
	//printf("ymin: %lf\n", ymin);
	int end = (height/local->numThreads)*(id+1);
	if(id==local->numThreads-1) { //last thread
		if((height/local->numThreads)*(id+1)) {
			end = height;
		}
	}
	for(j=(height/local->numThreads*id);j<end;j++) {
		//printf("%d\n", j);
		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			bitmap_set(bm,i,j,iters);
		}
	}
	//printf("Local %d max: %d\n", id, j);
	return (void *) args;
}

/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max ) {
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iteration_to_color(iter,max);
}

/*
Convert a iteration number to an RGBA color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int i, int max ) {
	int gray = 255*i/max;
	return MAKE_RGBA(gray,gray,gray,0);
}
