#include<stdio.h>
#include<cleanup.h>

int main(int argc, char* argv)
{
    if (argc != 2) {
        LOG(ERROR, "Usage: gslabcodcov [FILE_PATH]...\n");
        return 0;
    }

    isFile(checkIfFile(argv[1]));
    if(isFile == 1) {
        LOG(DEBUG, "Parsing a file.\n");
    } else if(isFile == 0) {
        LOG(DEBUG, "Parsing a directory.\n");
    } else {
        LOG(ERROR, "Invalid path\n");
    }

    retval = cleanup(argv[1]);

    return retval;
}

int checkIfFile(const char* path)
{
    DIR* directory = opendir(path);

    if (directory != NULL) {
        closedir(directory);
        return 0;
    }

    if (errno == ENOTDIR) {
        return 1;
    }

    return -1;
}

int cleanup(const char* path)
{
    int retval = 0;
    int charlen = 0;
    DIR *dir;
    struct dirent *ent;
    struct stat oldSt, st, statBuff;
    char *newPath = NULL;
    char *inputPath, *outputPath;

    if (isFile) {
        
    }
}
