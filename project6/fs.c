#include "fs.h"
#include "disk.h"

#define FS_MAGIC           0xf0f03410
#define INODES_PER_BLOCK   128
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK 1024

struct fs_superblock {
	int magic;
	int nblocks;
	int ninodeblocks;
	int ninodes;
};

struct fs_inode {
	int isvalid;
	int size;
	int direct[POINTERS_PER_INODE];
	int indirect;
};

union fs_block {
	struct fs_superblock super;
	struct fs_inode inode[INODES_PER_BLOCK];
	int pointers[POINTERS_PER_BLOCK];
	char data[DISK_BLOCK_SIZE];
};

bool *bitmap;

int fs_format()
{
	union fs_block block;
	int i, j;

	disk_read(0,block.data);

	//check if disk already mounted
	if(fs_isMounted()) {
		printf("Error! Disk already formatted. \n");
		return 0;
	}

	//set disk sizeof
	block.super.nblocks = disk_size();

	//set aside blocks for inodes
	block.super.ninodeblocks = roundUp(block.super.nblocks * .1);

	// set total inodes
	block.super.ninodes = block.super.ninodeblocks * INODES_PER_BLOCK;

	//check if magic number is defined
	if(!block.super.magic) {
		block.super.magic = FS_MAGIC;
	}

	//write the created superblock to the disk
	disk_write(0, block.data);

	//clear the inode table
	for(i=1; i<disk_size(); i++) { // for block on disk
		for(j=0; j<INODES_PER_BLOCK; j++) { // for inode in block
			disk_read(i, block.data);
			block.inode[j].isvalid = 0; //reset
			disk_write(i, block.data);
		}
	}

	return 1;
}

void fs_debug()
{
	union fs_block block;
	int i, j, k, l, indirect;
	bool test;

	disk_read(0,block.data);

	test = block.super.magic == FS_MAGIC ? 1 : 0;

	printf("superblock:\n");
	printf("	magic number %s valid\n", test ? "is" : "is not");
	printf("	%d blocks\n",block.super.nblocks);
	printf("	%d inode blocks\n",block.super.ninodeblocks);
	printf("	%d inodes\n",block.super.ninodes);

	for(i=1; i<=block.super.ninodeblocks; i++) { // for block on disk
		for(j=0; j<INODES_PER_BLOCK; j++) { // for inode in block
			disk_read(i, block.data);
			if (block.inode[j].isvalid) { // block has data
				printf("inode %d:\n", j);
				printf("	size: %d bytes\n", block.inode[j].size);
				printf("	direct blocks: ");
				for(k=0; k<POINTERS_PER_INODE; k++) {
					if(block.inode[j].direct[k]) { // non-zero
							printf("%d ", block.inode[j].direct[k]);
					}

				}
				printf("\n");
				//get indirect block
				indirect = block.inode[j].indirect;
				if(indirect) {
					printf("	indirect block: %d\n", indirect);
					printf("	indirect data blocks: ");
					disk_read(indirect, block.data);
					for(l=0; l<POINTERS_PER_BLOCK; l++) {
						if(block.pointers[l]) {
							printf("%d ", block.pointers[l]);
						}
					}
					printf("\n");
				}
			}
		}
	}
	//printf("inode %d:\n", );
}

int fs_mount()
{
	union fs_block block;
	int i;

	//check if filesystem exists
	disk_read(0,block.data);

	if(!block.super.magic) {
		printf("Error: Disk has not been formatted!\n");
		return 0;
	}

	//build bitmap
	bitmap = (bool *)malloc(sizeof(bool) * block.super.ninodes);

	for (i = 1; i<block.super.ninodes; i++ ) {
			bitmap[i] = 0;
	}
	return 1;
}

int fs_create()
{
	union fs_block block;
	int i, j, blockLoc, iLoc;

	disk_read(0,block.data);

	if(!fs_isMounted()) {
		printf("Error: Disk has not been mounted!\n");
		return 0;
	}
	// find available space
	for(i=1;i<block.super.ninodes;i++) {
		if(bitmap[i]==0) {
			blockLoc = roundUp(i/INODES_PER_BLOCK);
			iLoc = i % INODES_PER_BLOCK;
			disk_read(blockLoc,block.data);
			bitmap[i] = 1;
			block.inode[iLoc].isvalid = 1;
			block.inode[iLoc].size = 0;
			for(j=0;j<POINTERS_PER_INODE;j++) {
				if(block.inode[iLoc].direct[j]==(iLoc+block.super.ninodeblocks)) {
					//already listed
					disk_write(blockLoc, block.data);
					return i;
				}
			}
			for(j=0;j<POINTERS_PER_INODE;j++) {
				if(!block.inode[iLoc].direct[j]) {
					//already listed
					block.inode[iLoc].direct[j] = (iLoc + block.super.ninodeblocks);
					disk_write(blockLoc, block.data);
					return i;
				}
			}
		}
	}
	return 0;
}

int fs_delete( int inumber )
{
	union fs_block block;
	int i;

	disk_read(0,block.data);
	if(!fs_isMounted()) {
		printf("Error: Disk has not been mounted!\n");
		return 0;
	}
	//verify valid inode
	if(!block.inode[inumber].isvalid) {
		printf("Error: Invalid node");
		return 0;
	}

	for(i=0;i<INODES_PER_BLOCK;i++) {
		if(block.inode[inumber].direct[i]) { //where data
			disk_read(block.inode[inumber].direct[i],block.data);
		}
	}
	return 0;
}

int fs_getsize( int inumber )
{
	if(!fs_isMounted()) {
		printf("Error: Disk has not been mounted!\n");
		return 0;
	}
	return -1;
}

int fs_read( int inumber, char *data, int length, int offset )
{

	return 0;
}

int fs_write( int inumber, const char *data, int length, int offset )
{
	if(!fs_isMounted()) {
		printf("Error: Disk has not been mounted!\n");
		return 0;
	}
	return 0;
}

bool fs_isMounted() {
	// returns true if disk has been mounted, 0 otherwise
	if(bitmap) {
		return true;
	}
	return false;
}
int roundUp(double num) {
	int check = (int)(num * 10);
	int tenths = check % 10;

	if(tenths != 0) {
		return (int) num + 1;
	}
	return (int) num;

}
