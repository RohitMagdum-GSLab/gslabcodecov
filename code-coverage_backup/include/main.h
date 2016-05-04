#ifndef __MAIN__
#define __MAIN__

#include<sys/stat.h>
#include<unistd.h>

#define BACKUP "__backup"

extern bool isFile;
struct stat statBuff;

int init();

int checkIfFile(const char* path);

int makeCopyAndAnnotate(const char* path);

int makeFileCopy(const char* inputPath, const char* outputPath);

#endif /* __MAIN__ */
