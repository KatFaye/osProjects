/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or backrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


struct frameEntry {
	int page;
	int bits;
};

//globals
char* virtmem = NULL;
char* physmem = NULL;
struct disk* disk = NULL;
struct frameEntry *frameTable = NULL;
int front=0;
int back=0;
int *farr;
int nframes, npages;
int pageFaults = 0;
int diskReads = 0;
int diskWrites = 0;


void remove_page(struct page_table *pt, int n);
int check();
void rand_fault_handler(struct page_table *pt, int page);
void fifo_fault_handler(struct page_table *pt, int page);
void custom_fault_handler(struct page_table *pt, int page);

int main( int argc, char *argv[] ) {
	srand(time(NULL));

	//ensure correct number of arguments passed
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	//set variables based on user input
	npages = atoi(argv[1]);
	nframes = atoi(argv[2]);
	const char *algorithm = argv[3];
	const char *program = argv[4];

	//create disk and page table
	disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt;
		if(!strcmp(algorithm,"rand")) {
			pt = page_table_create( npages, nframes, rand_fault_handler );
		} else if(!strcmp(algorithm,"fifo")){
			pt = page_table_create( npages, nframes, fifo_fault_handler );
		} else if(!strcmp(algorithm, "custom")){
		// 	pt = page_table_create( npages, nframes, custom_fault_handler );
			printf("Did not implement custom.");
		} else {
			fprintf(stderr, "Unsupported type: %s\n", algorithm);
			exit(1);
		}

	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	farr = malloc (nframes * sizeof(int));
	frameTable = malloc(nframes * sizeof(struct frameEntry));
	if (frameTable == NULL) {
	    printf("Error allocating space for frame table!\n");
	    exit(1);
	}

	//set virtual and physical memory
	virtmem = page_table_get_virtmem(pt);
	physmem = page_table_get_physmem(pt);

	//select the program to run based on user input
	if(!strcmp(program,"sort")){
		sort_program(virtmem,npages*PAGE_SIZE);
	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);
	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);
	} else {
		fprintf(stderr,"unknown program: %s\n",argv[4]);
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		exit(1);
	}

	//clean up
	free(frameTable);
	free(farr);
	page_table_delete(pt);
	disk_close(disk);

	printf("Disk reads (%d), writes (%d), page faults (%d)\n", diskReads, diskWrites, pageFaults);

	return 0;
}

//Random page fault handler
void rand_fault_handler(struct page_table *pt, int page) {
	pageFaults++;
	int frame, bits;
	page_table_get_entry(pt, page, &frame, &bits);
	int fLoc;

	if(!bits) { //not in table
		bits = PROT_READ;
		fLoc = check();

		if(fLoc < 0) { //find random
			fLoc = rand() % nframes;
			remove_page(pt, fLoc);
		}

		disk_read(disk, page, &physmem[fLoc*PAGE_SIZE]);
		diskReads++;

	} else if(bits & PROT_READ) { //no write
		bits = PROT_READ | PROT_WRITE;
		fLoc = frame;
	} else {
		printf("Error on random page fault.\n");
		exit(1);
	}

	page_table_set_entry(pt, page, fLoc, bits);
	frameTable[fLoc].page = page;
	frameTable[fLoc].bits = bits;

}

//First in, first out page fault handler
void fifo_fault_handler(struct page_table *pt, int page)
{
	pageFaults++;
	int frame, bits;
	page_table_get_entry(pt, page, &frame, &bits);
	int fLoc;
	if(!bits) { // not in mem
		bits = PROT_READ;
		fLoc = check();
		if(fLoc < 0)	{
				if (front==back) {
				fLoc = farr[front];
				remove_page(pt, fLoc);
			} else {
				printf("Unknown error on FIFO page fault\n");
				exit(1);
			}
		}
		disk_read(disk, page, &physmem[fLoc*PAGE_SIZE]);
		diskReads++;

		farr[back]=fLoc;
		back = (back +1) % nframes;

	}	else if(bits & PROT_READ){
		bits = PROT_READ | PROT_WRITE;
		fLoc = frame;
	} else {
		printf("Unknown error on FIFO page fault.\n");
		exit(1);
	}

	page_table_set_entry(pt, page, fLoc, bits);
	frameTable[fLoc].page = page;
	frameTable[fLoc].bits = bits;
}

int check() {
	for(int i = 0; i < nframes; i++) {
		if(frameTable[i].bits == 0)
			return i;
	}
	return -1;
}

//remove a page
void remove_page(struct page_table *pt, int n){
	if(frameTable[n].bits & PROT_WRITE) {
		disk_write(disk, frameTable[n].page, &physmem[n*PAGE_SIZE]);
		diskWrites++;
	}
	page_table_set_entry(pt, frameTable[n].page, n, 0);
	frameTable[n].bits = 0;
	front=(front+1)%nframes;
}
