#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include<annotate.h>
#include<error.h>
#include<common.h>

int annotateFile(const char* inputPath, const char* outputPath)
{
    FILE *inputFp, *outputFp;
    int initFlag = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *savedLine = NULL;
    char dumpString[1024];
    char searchCounter[512];

    inputFp = fopen(inputPath, "r");
    if (inputFp == NULL) {
        LOG(ERROR, "Failed to open file %s\n", inputPath);
        return FILE_OPEN_ERROR;
    }

    outputFp = fopen(outputPath, "w");
    if (outputFp == NULL) {
        LOG(ERROR, "Failed to open file %s\n", outputPath)
            return FILE_OPEN_ERROR;
    }

    while ((read = getline(&line, &len, inputFp)) != -1) {

        if(fwrite(line, strlen(line), 1, outputFp) < 0) {
            LOG(ERROR, "Failed to write to the file.\n");
            return ERROR_ANNOTATE;
        }

        if(!initFlag) {
            if(fwrite("#include<stdlib.h>\n#include<string.h>\n", 
                        strlen("#include<stdlib.h>\n#include<string.h>\n"), 
                        1, outputFp) < 0) {
                LOG(ERROR, "Failed to write to the file.\n");
                return ERROR_ANNOTATE;
            }
            initFlag = 1;
        }

        if ((strstr(line, "{") != 0) || (strstr(line, "){") != 0) || 
                (strstr(line, ") {") != 0) || (strstr(line, ")  {") != 0)) {
            if (strlen(line) == 2) {
                printf("The savedline = %s\n", savedLine);
                sprintf(dumpString, "f c:^*^%d: func:%s: 0.\n", counter++, savedLine);

                if((fwrite(dumpString, strlen(dumpString), 1, fpDb)) < 0) {
                    LOG(ERROR, "Failed to dump in db file.\n");
                    return FILE_WRITE_ERROR;
                }

                sprintf(searchCounter, ":^*^%d", counter-1);

                sprintf(dumpString, "\t{\n\t\tFILE* dbFile = NULL;\n\t\t"
                        "char* readline = NULL;\n\t\tsize_t len = 0;\n"
                        "\t\tdbFile = fopen(\"/var/tmp/databaseFile.db\", \"r+\");\n"
                        "\t\twhile(getline(&readline, &len, dbFile) != -1){\n"
                        "\t\t\tif(strstr(readline, \"%s\")){\n"
                        "\t\t\tfseek(dbFile, -3, SEEK_CUR);\n"
                        "\t\t\tfwrite(\"1\",strlen(\"1\"), 1, dbFile);"
                        "\n\t\t\t}\n\t\t}\n\t}\n", searchCounter);

                if((fwrite(dumpString, strlen(dumpString), 1, outputFp)) < 0) {
                    LOG(ERROR, "Failed to annotate file.\n");
                    return ERROR_ANNOTATE;
                }
            } else {
                if((fwrite("Writing something\n", strlen("Writing something\n"), 1, outputFp)) < 0) {
                    LOG(ERROR, "Failed to annotate file.\n");
                    return ERROR_ANNOTATE;
                }
            }

        } else {
            printf("Retrieved line = %s", line);
        }
        if(savedLine) {
            free(savedLine);
            savedLine = NULL;
        }
        savedLine = (char*) malloc (strlen(line));
        strcpy(savedLine, line);
        printf("Line here = %s\n", line);
        printf("SavedLine here = %s\n", savedLine);

    }

    if (inputFp) {
        fclose(inputFp);
    }

    if (outputFp) {
        fclose(outputFp);
    }

    if (line) {
        free(line);
    }
    return SUCCESS;
}
