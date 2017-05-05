#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

void fs_debug();
int  fs_format();
int  fs_mount();

int  fs_create();
int  fs_delete( int inumber );
int  fs_getsize();

int  fs_read( int inumber, char *data, int length, int offset );
int  fs_write( int inumber, const char *data, int length, int offset );

//helper functions'
bool fs_isMounted();
int roundUp(double num);
#endif
