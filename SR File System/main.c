
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
#include <time.h>
#define returnData ((struct rtdata_state *) fuse_get_context()->private_data)

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
static const int MAX_NUMBER_BLOCKS = 10000;
static const int MAX_BLOCK_SIZE = 4096;



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


static void* myInit(struct fuse_conn_info* conn){
    
    // TODO - Check to see if the 10K blocks have been created - Done
    // TODO - Check number of blocks that exist already
    
    
    //char fsPath[]  = {"/home/sean/Desktop/fileSystem/blocks"};
    //    char* fileName = {"fusedata."};
    char fileName[20];
    //int fileNumber = 0;
    //char* mode = "w+";
    //sprintf(fileName, "%sfiledata.%d", fsPath,fileNumber);
    
    int i = 0;
    FILE* tempFilePtr;
    char* superBlockPath = "/home/sean/fusedata/fusedata.0";
    //char* usr = getenv("USER");
    //printf("USER: %s", usr);
    
    struct stat tmp;
    int check = stat(superBlockPath, &tmp);
    
    
    if (check == 0)
        i++;
    
//    
//    if(tempFilePtr == fopen(superBlockPath, "r")){
//        fclose(tempFilePtr);
//        i++;
//    }
    
    
    // Creating SuperBlock if it does not exist
    
    if(i == 0){
        FILE* superBlock = fopen(superBlockPath, "w+");
        fprintf(superBlock, "{\n");
        fprintf(superBlock, "creationTime: %ld\n", time(NULL));
        fprintf(superBlock, "mounted: %d\n", 1);
        fprintf(superBlock, "devID: %d\n", 20);
        fprintf(superBlock, "freeStart: %d\n", 1);
        fprintf(superBlock, "freeEnd: %d\n", 25);
        fprintf(superBlock, "root: %d\n", 26);
        fprintf(superBlock, "maxBlocks: %d\n", 10000);
        fclose(superBlock);
        i++;
    }
    
    
    char* lotsOfZeros = (char*) malloc(MAX_BLOCK_SIZE);
    memset(lotsOfZeros, '0', MAX_BLOCK_SIZE);
    //    puts(fileName);
    
    //Check to see if the files have been created, and if so, do not create the 10K
    
    check = stat("/home/sean/fusedata/fusedata.1", &tmp);
    int filesExist = 0;
    if (check == 0)
        filesExist = 1;
    
    if (filesExist == 0){ //If the files exist, do not create 10K files again
        for (i; i<MAX_NUMBER_BLOCKS; i++){
            //char* stdBlockSize = malloc(MAX_BLOCK_SIZE);
            sprintf(fileName, "/home/sean/fusedata/fusedata.%d", i);
            FILE* blockFile = fopen( fileName, "w+");
            fwrite(lotsOfZeros, MAX_BLOCK_SIZE, 1, blockFile);
            //fprintf(blockFile, "%p", lotsOfZeros);
            fclose(blockFile);
        }
    }
    
    free(lotsOfZeros);
    //free(blockFile);
    //free(mode);
    //free(lotsOfZeros);
    //free(stdBlockSize);
    //free(blockFile);
    
    return returnData;
    
    
}



static struct fuse_operations operationMappings = {
    .init = myInit,
    .getattr = hello_getattr,
    .readdir =  hello_readdir,
    .open = hello_open,
    .read = hello_read,
    
};

int main(int argc, char * argv[]) {
    // insert code here...
    //myInit();
    char* fullpath = realpath(argv[argc-1], NULL);
    return fuse_main(argc, argv, &operationMappings, fullpath);
}
