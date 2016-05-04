#include<stdio.h>
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
    int retval;

    if(argc !=2) {
        LOG(ERROR, "Usage: gscodcov [FILE_PATH]...\n");
        return 0;
    }
    
    LOG(INFO, "Code coverage for \"%s\"\n", argv[1]);

    isFile = checkIfFile(argv[1]);
    if(isFile == 1) {
        LOG(DEBUG, "Parsing a File.\n");
    } else if(isFile == 0) {
        LOG(DEBUG, "Parsing a Directory.\n");
    } else {
        LOG(ERROR, "Invalid path.\n");
    }

    retval = init();
    if (retval != 0) {
        LOG(ERROR, "Failed to initialize.\n");
        return INIT_ERROR;
    }

    retval = makeCopyAndAnnotate(argv[1]);
    if (retval != 0) {
        LOG(ERROR, "Failed to make copy and annotate.\n");
        return retval;
    }

    return retval;
}

int init()
{
    fpDb = fopen(dbFilePath, "w");
    if (fpDb == NULL) {
        return INIT_ERROR;
    }
    return SUCCESS;
}

int checkIfFile(const char* path)
{
    DIR* directory = opendir(path);

    if(directory != NULL) {
        closedir(directory);
        return 0;
    }

    if(errno == ENOTDIR) {
        return 1;
    }

    return -1;
}

int makeCopyAndAnnotate(const char* path) 
{
    int retval = 0;
    int charlen = 0;
    DIR *dir;
    struct dirent *ent;
    struct stat oldSt, st, statBuff;
    char *newPath = NULL;
    char* inputPath, *outputPath;


    if (isFile) {
        retval = stat(path, &statBuff);
        LOG(DEBUG, "Permission = %o\n", statBuff.st_mode);
        inputPath = (char*)malloc(strlen(path) + strlen(BACKUP) + 1);
        outputPath = (char*)malloc(strlen(path) + 1);

        strncpy(inputPath, path, strlen(path));
        strncpy(outputPath, path, strlen(path) + 1);
        strncat(inputPath, BACKUP, strlen(BACKUP));

        LOG(DEBUG, "The copied inputpath = %s\n", inputPath);
        LOG(DEBUG, "The copied outputpath = %s\n", outputPath);

        retval =  makeFileCopy(inputPath, outputPath);

        retval = annotateFile(inputPath, outputPath);

        return retval;
    }

    if (path[strlen(path)-1] == '/') {
        charlen = 1 ;
    }

    newPath = (char*)malloc(strlen(path) + 1 + strlen(BACKUP));

    strncpy(newPath, path, strlen(path) - charlen);
    strncat(newPath, BACKUP, strlen(BACKUP));

    stat(path, &oldSt);

    if (stat(newPath, &st) == -1) {
        retval = mkdir(newPath, oldSt.st_mode);
        if (retval != 0) {
            LOG(ERROR, "Failed to create directory %s\n", newPath);
            return DIRECTORY_CREATE_ERROR;
        }
    }

    if ((dir = opendir(path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            LOG (DEBUG, "%s\n", ent->d_name);
            if (strcmp(ent->d_name, ".") == 0) {
                continue;
            }
            if (strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            inputPath = (char*)malloc(strlen(newPath) + strlen(BACKUP) + 
                    strlen(ent->d_name) + 2);
            outputPath = (char*) malloc(strlen(newPath) + strlen(ent->d_name) + 2);

            strncpy(inputPath, newPath, strlen(newPath));
            strncat(inputPath, "/", 1);
            strncat(inputPath, ent->d_name, strlen(ent->d_name));
            strncat(inputPath, BACKUP, strlen(BACKUP));

            strncpy(outputPath, path, strlen(path) + 1);
            strncat(outputPath, "/", 1);
            strncat(outputPath, ent->d_name, strlen(ent->d_name));
            retval = makeFileCopy (inputPath, outputPath);
            if (retval != 0) {
                return retval;
            }

            retval = annotateFile(inputPath, outputPath);
            if (retval != 0) {
                return retval;
            }
        }
        closedir (dir);
    } else {
        LOG(ERROR, "Failed to open Directory %s\n", path);
        return DIRECTORY_OPEN_ERROR;
    }

    return retval;
}

int makeFileCopy(const char* inputPath, const char* outputPath)
{
    int inputFd = 0, outputFd = 0, openFlags;
    mode_t filePerm;
    ssize_t numRead;
    char buf[BUF_SIZE];
    int retval = 0;



    outputFd = open(outputPath, O_RDONLY);
    if (outputFd == -1) {
        LOG(ERROR, "Error opening file %s\n", outputPath);
        retval = FILE_OPEN_ERROR;
        goto exit;
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
        S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    inputFd = open(inputPath, openFlags, filePerm);
    if (inputFd == -1) {
        LOG(ERROR, "Error opening copy file \"%s\"\n."
                "Please check for permissions to create"
                "file.\n", inputPath);
        retval = FILE_OPEN_ERROR;
        goto exit;
    }

    while ((numRead = read(outputFd, buf, BUF_SIZE)) > 0) {
        if (write(inputFd, buf, numRead) != numRead) {
            LOG(ERROR, "Coudn't write to the copy file.\n");
            retval = FILE_WRITE_ERROR;
            goto exit;
        }
    }

    if (numRead == -1) {
        LOG (ERROR, "Failed to read the input file.\n");
        retval = FILE_READ_ERROR;
        goto exit;
    }

exit:
    if (close(inputFd) == -1) {
        LOG(ERROR, "Failed to close file %s\n", inputPath);
        return FILE_CLOSE_ERROR;
    }
    if (close(outputFd) == -1) {
        LOG(ERROR, "Failed to close file %s\n", outputPath);
        return FILE_CLOSE_ERROR;
    }
    return retval;
}
