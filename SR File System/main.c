
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
    
    char* lotsOfZeros = (char*) malloc(MAX_BLOCK_SIZE);
    memset(lotsOfZeros, '0', MAX_BLOCK_SIZE);
    
    // Creating SuperBlock if it does not exist
    
    
    if(i == 0){
        FILE* superB = fopen(superBlockPath, "w+");
        fwrite(lotsOfZeros, MAX_BLOCK_SIZE, 1, superB);
        fclose(superB);
        FILE* superBlock = fopen(superBlockPath, "r+");
        // fseek(superBlock, 0, SEEK_SET); // Set position to beginning of file
        
        fseek(superBlock, 0, SEEK_SET);
        int z = 0;
        for (z; z<27; z++)
            fprintf(superBlock, "%d,", z);
        
        
        char initialSB [100], creation[50], mount[50], devID[50], freeStart[50], freeEnd[50], root[50], maxBlocks[50], bracket[50];
        strcpy(initialSB,  "{");
        sprintf(creation,  "creationTime: %ld,", time(NULL));
        sprintf(mount,     "mounted: %d,", 1);
        sprintf(devID,     "devID: %d,", 20);
        sprintf(freeStart, "freeStart: %d,", 1);
        sprintf(freeEnd,   "freeEnd: %d,", 25);
        sprintf(root,      "root: %d,", 26);
        sprintf(maxBlocks, "maxBlocks: %d", 10000);
        sprintf(bracket,   "}");
        //char* mount     = ("mounted: %d,", 1);
        //char* devID     = ("devID: %d,", 20);
        //char* freeStart = ("freeStart: %d,", 1);
        //char* freeEnd   = ("freeEnd: %d,", 25);
        //    char* root      = ("root: %d,", 26);
        //    char* maxBlocks = ("maxBlocks: %d,", 10000);
        //    char* bracket   = ("}");
    
        strcat(initialSB, creation);
        strcat(initialSB, mount);
        strcat(initialSB, devID);
        strcat(initialSB, freeStart);
        strcat(initialSB, freeEnd);
        strcat(initialSB, root);
        strcat(initialSB, maxBlocks);
        strcat(initialSB, bracket);
    
        fseek(superBlock, 71, SEEK_SET);
        //fprintf(superBlock,"TEST");
        fputs(initialSB, superBlock);
        //fflush(superBlock);
        fclose(superBlock);
        i++;
    }
    
    int j = 400;
    int k = 800;
    
    //Check to see if the files have been created, and if so, do not create the 10K
    
    struct stat tmp2;
    check = stat("/home/sean/fusedata/fusedata.1", &tmp2);
    int filesExist = 0;
    if (check == 0)
        filesExist = 1;
    
    if (filesExist == 0){ //If the files exist, do not create 10K files again
        for (i; i<MAX_NUMBER_BLOCKS; i++){
            sprintf(fileName, "/home/sean/fusedata/fusedata.%d", i);
            FILE* blockFile = fopen( fileName, "w+");
            fwrite(lotsOfZeros, MAX_BLOCK_SIZE, 1, blockFile);
            
            if(i == 1){
                int j = 27;
                fseek(blockFile, 0, SEEK_SET);
                for(j; j < 400; j++){
                    fprintf(blockFile, "%d,", j);
                }
            }
            
            while (i != 1 && i <= 25){
                fseek(blockFile, 0, SEEK_SET);
                
                int k = j + 400;
                for(j; j < k; j++){
                    fprintf(blockFile, "%d,", j);
                }
                break;
            }
            //char* stdBlockSize = malloc(MAX_BLOCK_SIZE);
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

