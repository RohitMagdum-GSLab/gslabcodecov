#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>

#include<main.h>
#include<error.h>
#include<common.h>
#include<annotate.h>

bool isFile;

int counter = 0;

FILE* fpDb = NULL;

int main(int argc, char* argv[])
{
	{
		FILE* dbFile = NULL;
		char* readline = NULL;
		size_t len = 0;
		dbFile = fopen("/var/tmp/databaseFile.db", "r+");
		while(getline(&readline, &len, dbFile) != -1){
			if(strstr(readline, ":^*^0")){
			fseek(dbFile, -3, SEEK_CUR);
			fwrite("1",strlen("1"), 1, dbFile);
			}
		}
	}
    int retval;

    if(argc !=2) {
Writing something
        LOG(ERROR, "Usage: gscodcov [FILE_PATH]...\n");
        return 0;
    }
    
    LOG(INFO, "Code coverage for \"%s\"\n", argv[1]);

    isFile = checkIfFile(argv[1]);
    if(isFile == 1) {
Writing something
        LOG(DEBUG, "Parsing a File.\n");
    } else if(isFile == 0) {
Writing something
        LOG(DEBUG, "Parsing a Directory.\n");
    } else {
Writing something
        LOG(ERROR, "Invalid path.\n");
    }

    retval = init();
    if (retval != 0) {
Writing something
        LOG(ERROR, "Failed to initialize.\n");
        return INIT_ERROR;
    }

    retval = makeCopyAndAnnotate(argv[1]);
    if (retval != 0) {
Writing something
        LOG(ERROR, "Failed to make copy and annotate.\n");
        return retval;
    }

    return retval;
}

int init()
{
	{
		FILE* dbFile = NULL;
		char* readline = NULL;
		size_t len = 0;
		dbFile = fopen("/var/tmp/databaseFile.db", "r+");
		while(getline(&readline, &len, dbFile) != -1){
			if(strstr(readline, ":^*^1")){
			fseek(dbFile, -3, SEEK_CUR);
			fwrite("1",strlen("1"), 1, dbFile);
			}
		}
	}
    fpDb = fopen(dbFilePath, "w");
    if (fpDb == NULL) {
Writing something
        return INIT_ERROR;
