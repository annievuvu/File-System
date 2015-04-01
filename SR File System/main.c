//
//  main.c
//  SR File System
//
//  Created by Sean Ramzan on 3/28/15.
//  Copyright (c) 2015 Sean Ramzan. All rights reserved.
//

#include <stdio.h>

/*
 FUSE: Filesystem in Userspace
 Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
 
 This program can be distributed under the terms of the GNU GPL.
 See the file COPYING.
 
 gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
 */

#define FUSE_USE_VERSION 26

#include "fuse.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
static const int MAX_NUMBER_BLOCKS = 10000;
static const int MAX_BLOCK_SIZE = 4096;


/*
static int hello_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else if (strcmp(path, hello_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
    } else
        res = -ENOENT;
    
    return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
    
    if (strcmp(path, "/") != 0)
        return -ENOENT;
    
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);
    
    return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
    if (strcmp(path, hello_path) != 0)
        return -ENOENT;
    
    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;
    
    return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    return len;
}
 */

static void* myInit(){
    
    // TODO - Check to see if the 10K blocks have been created
    // TODO - Check number of blocks that exist already
    
    //char fsPath[]  = {"/myFileSystem/blocks/"};
//    char* fileName = {"fusedata."};
    char fileName[20];
    int fileNumber = 0;
    //char* mode = "w+";
    sprintf(fileName, "filedata.%d", fileNumber);
    char* lotsOfZeros = (char*) malloc(MAX_BLOCK_SIZE);
    memset(lotsOfZeros, '0', MAX_BLOCK_SIZE);
//    puts(fileName);
    //DELETE LATER
    char* name;
    name = (char*) malloc(256*4*4);
    char* numDigits = malloc(100);
    char* letter_ch = (char*) malloc(strlen(numDigits));
    
    for (int i = 0; i<3; i++){
        //char* stdBlockSize = malloc(MAX_BLOCK_SIZE);
        //sprintf(fileName, "filedata.%d", fileNumber);
    
        strcpy(name, fuse_get_context()->private_data);
        strcat(name, letter_ch);
        FILE* blockFile = fopen( fileName, "w+");
        fwrite(lotsOfZeros, MAX_BLOCK_SIZE, 1, blockFile);
        //fprintf(blockFile, "%p", lotsOfZeros);
        fclose(blockFile);
        
    }
    
    void* temp2 = malloc(10);
    //free(lotsOfZeros);
    
    return temp2;
    
    
}


                  
static struct fuse_operations operationMappings = {
    .init = myInit,
//    .gettattr = getAttributes,
//    .readdir =  readDirectory,
//    .open = openFile,
//    .read = readFile,
    
};

int main(int argc, char * argv[]) {
    // insert code here...
    myInit();
    printf("hello");
    //char* fullpath = realpath(argv[argc-1], NULL);
    //return fuse_main(argc, argv, operationMappings, fullpath);
    return 0;
}
